//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrameEx.h"
#include "XBasicWindow.h"
#include "XBasicOpenGL.h"
#include "XObjectManager.h"

#include "XResourcePack.h"
#include "XLogBook.h"

#define FRAME_DEBUG (0)
#if FRAME_DEBUG
int frameTestSum = 0;
#endif

_XBool _XFrameEx::init(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) 
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;

	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_frameName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_frameName == NULL) 
	{
		XDELETE(m_cp);
		return XFalse;
	}
	m_frameName[0] = '\0';

	strcpy(m_frameName,filename);

	//�����������е�����֡ͼƬ
	int nameLength = strlen(m_frameName);
	if(nameLength <= 0)
	{
		LogStr("Action file path error!");
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
		return XFalse;
	}
	//�����ȡ����֡�������ı��ĵ�����
	char tempFrameName[256];
	strcpy(tempFrameName,m_frameName);

	for(int i = nameLength -1;i > 0;-- i)
	{
		if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
		{//find the key char
			tempFrameName[i + 1] = '\0';
			strcat(tempFrameName,"frame.txt");
			break;
		}
	}	
	//�������Ҫ���Ƕ��߳���Դ���⣬Ψһ��һ�����߳����Ѿ����˻��⴦��
	if(m_resoursePosition == RESOURCE_LOCAL_PACK)
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFrameName);
		if(p == NULL) 
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}

		int offset = 0;
		if(sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allKeyFramesSum <= 0)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%d:",&m_allFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allFramesSum <= 0)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		m_keyFrameArray = createArrayMem<int>(m_allFramesSum);
		if(m_keyFrameArray == NULL) 
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]) != 1) {XDELETE_ARRAY(p);return XFalse;}
					offset += getCharPosition((char *)(p + offset),',') + 1;
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						XDELETE_ARRAY(p);
						XDELETE_ARRAY(m_frameName);
						XDELETE_ARRAY(m_keyFrameArray);
						XDELETE(m_cp);
						return XFalse;
					}
				}
			}
		}
		XDELETE_ARRAY(p);
	}else
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(tempFrameName,"rb")) == NULL)
		{
			LogStr("Action text file error!");
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}

		if(fscanf(fp,"%d:",&m_allKeyFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allKeyFramesSum <= 0)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		if(fscanf(fp,"%d:",&m_allFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allFramesSum <= 0)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		m_keyFrameArray = createArrayMem<int>(m_allFramesSum);
		if(m_keyFrameArray == NULL) 
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return XFalse;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(fscanf(fp,"%c:",&tempFlag) != 1) {fclose(fp);return XFalse;}
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(fscanf(fp,"%d,",&m_keyFrameArray[i]) != 1) {fclose(fp);return XFalse;}
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						fclose(fp);
						XDELETE_ARRAY(m_frameName);
						XDELETE_ARRAY(m_keyFrameArray);
						XDELETE(m_cp);
						return XFalse;
					}
				}
			}
		}
		fclose(fp);
	}

#if FRAME_DEBUG
	frameTestSum ++;
	printf("Frame new sum:%d\n",frameTestSum);
#endif
	if(nameLength < 7) return XFalse;
	m_pSprite = createArrayMem<_XSprite>(m_allKeyFramesSum);
	if(m_pSprite == NULL) 
	{
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
		XDELETE_ARRAY(m_keyFrameArray);
		return XFalse;
	}
	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		tempFrameName[nameLength - 5] = '0' + i%10;
		tempFrameName[nameLength - 6] = '0' + (i/10)%10;
		tempFrameName[nameLength - 7] = '0' + (i/100)%10;

		if(!m_pSprite[i].init(tempFrameName,m_resoursePosition,POINT_CENTER))
		{
			LogStr("The action pictrue load error!");
			for(int j = 0;j < i;++ j)
			{
				m_pSprite[j].release();
			}
			XDELETE_ARRAY(m_pSprite);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			XDELETE_ARRAY(m_keyFrameArray);
			return XFalse;
		}
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
		_XObjManger.decreaseAObject(&(m_pSprite[i]));
#endif
		m_pSprite[i].setPosition(0.0f,0.0f);
		m_pSprite[i].setAngle(0);
		m_pSprite[i].setSize(1.0f,1.0f);
	}
#if WITH_OBJECT_MANAGER	//ע������֡������
	_XObjManger.addAObject(this);
#endif

	x = 0.0f;			//����֡��λ��
	y = 0.0f;
	setAngle(0);		//����֡�ĽǶ�
	xsize = 1.0f;		//����֡�����ű���
	ysize = 1.0f;

	m_isInited = XTrue;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
_XBool _XFrameEx::releaseMem()
{
	if(!m_isInited) return XTrue;

	XDELETE_ARRAY(m_pSprite);
	
#if FRAME_DEBUG
	frameTestSum --;
	if(frameTestSum == 10)
	{
		printf("haha\n");
	}
	printf("Frame new sum:%d\n",frameTestSum);
#endif
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(this);
#endif

	m_isInited = XFalse;
	return XTrue;
}
void _XFrameEx::draw()
{//����֡����
	if(!m_isVisible) return;
	if(!m_isInited) return;
	if(m_isDisappearAtEnd && m_isEnd) return;	//�������֮����ʧ

	int temp_Frame = m_keyFrameArray[(int)(m_nowFramesNumble)];
//	m_pSprite[temp_Frame].m_pShaderProc = m_pShaderProc;
	m_pSprite[temp_Frame].setShaderClass(m_pShader);// = m_pShader;
	m_pSprite[temp_Frame].draw();
//	m_pSprite[temp_Frame].m_isFlash = 0;
//	m_pSprite[temp_Frame].m_pShaderProc = NULL;
}
void _XFrameEx::move(int timeDelay)
{
	if(!m_isInited || m_isEnd) return;
	if(m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;

	m_nowFramesNumble += m_actionSpeed * timeDelay;			//�ƶ�����֡����
	if(m_nowFramesNumble >= m_allFramesSum) 
	{//������ŵ����һ֡�������֡
		m_nowFramesNumble = 0.0f;

		if(!m_isEndImmediately)
		{//������ǿ������̽����Ķ��������ﲥ�����һ��ѭ��֮�󣬼��ɽ���
			if( m_isSetEnd) m_isEnd = XTrue;
		}
		if(!m_isLoop) m_isEnd = XTrue;//����ǲ�ѭ���Ķ�������Ҳ���ԱȽϽ���
	}else	
	if(m_nowFramesNumble < 0) 
	{//����ǵ��򲥷ŵĻ��������ж��Ƿ񲥷����(�Լ�������ɴ���)
		m_nowFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if(!m_isEndImmediately)
		{
			if(m_isSetEnd) m_isEnd = XTrue;
		}
		if(!m_isLoop) m_isEnd = XTrue;
	}
}
void _XFrameEx::setAttribute(const _XVector2& position,_XBool loop,_XBool endImmediately,int startFrame,float actionSpeed,_XBool disappearAtEnd,_XBool isOverturn)
{
	if(!m_isInited) return;
	x = position.x;			//����֡��λ��
	y = position.y;

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_nowFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;
	m_isDisappearAtEnd = disappearAtEnd;


	if((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
	{
		m_isOverturn = isOverturn;
	}
	//�����õ��������õ������ڲ�ʹ֮��Ч
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setPosition(x,y);
		m_pSprite[i].setOverturn(m_isOverturn);
	}
}
_XFrameEx::_XFrameEx()
:m_resoursePosition(RESOURCE_LOCAL_FOLDER)
,m_pSprite(NULL)
,angle(0.0f)
,m_isVisible(XTrue)
,x(0.0f),y(0.0f)
,xsize(1.0f),ysize(1.0f)
,alpha(1.0f)
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_isOverturn(XFalse)	//Ĭ������²���ת
,m_allFramesSum(0)							//����֡��������֡��
,m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
,m_nowFramesNumble(0)					//��ǰ���ŵ��ڼ�֡
,m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
,m_isLoop(XFalse)								//����֡�Ƿ����ѭ��
,m_startFrame(0)							//����һ֡��ʼ
,m_isEnd(XFalse)								//����֡�Ƿ񲥷����
,m_isEndImmediately(XFalse)					//�����Ƿ�Ϊ���̽�������
,m_isSetEnd(XFalse)							//is end by user			//�Ƿ���������֡����
,m_isDisappearAtEnd(XFalse)					//����֡�Ƿ��ڲ������֮����ʧ
,m_isInited(XFalse)
,m_frameName(NULL)
,m_keyFrameArray(NULL)
,m_isACopy(XFalse)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
//,m_pShaderProc(NULL)
,m_pShader(NULL)
,m_cp(NULL)
{
	m_objType = OBJ_FRAMEEX;
}
_XFrameEx::~_XFrameEx()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
	}
	releaseMem();
}
_XFrameEx& _XFrameEx::operator = (const _XFrameEx& temp)
{
	if(&temp == this) return *this;			//��ֹû��Ҫ�����ҿ���
	//if(m_isInited != 0) return *this;			//�����Ѿ���ʼ�������Ͳ����ٸ��Ʊ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited)
	{
#if FRAME_DEBUG
		frameTestSum ++;
		printf("Frame new sum:%d\n",frameTestSum);
#endif
		m_pSprite = createArrayMem<_XSprite>(m_allKeyFramesSum);
		if(m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return *this;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			_XObjManger.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//ע������֡������
	if(!m_isACopy) _XObjManger.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void _XFrameEx::setACopy(const _XFrameEx& temp)
{
	if(&temp == this) return;			//��ֹû��Ҫ�����ҿ���
	//if(m_isInited != 0) return;			//�����Ѿ���ʼ�������Ͳ����ٸ��Ʊ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited)
	{
#if FRAME_DEBUG
		frameTestSum ++;
		printf("Frame new sum:%d\n",frameTestSum);
#endif
		m_pSprite = createArrayMem<_XSprite>(m_allKeyFramesSum);
		if(m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			_XObjManger.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//ע������֡������,���m_isACopy���ж���Ϊ�˷�ֹ�ظ�ע�ᣬ����ע�᱾����Ѿ��������ظ�ע��
	if(!m_isACopy) _XObjManger.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
_XFrameEx::_XFrameEx(const _XFrameEx &temp)
{
	if(&temp == this) return;	//��ֹ�����
}