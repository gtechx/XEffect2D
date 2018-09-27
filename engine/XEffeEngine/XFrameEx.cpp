#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrameEx.h"
#include "XResourcePack.h"
#include "XSprite.h"
#include "XFile.h"
#include "XDirectory.h"
namespace XE {
#define FRAME_DEBUG (0)
#if FRAME_DEBUG
int frameTestSum = 0;
#endif
XBool XFrameEx::init(const char *filename, XResPos resPos)
{
	if (filename == NULL) return false;
	if (m_isInited)
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if (resPos == RES_SYS_DEF) resPos = getDefResPos();

	if (!loadData(filename, resPos)) return false;
	m_position.reset();	//����֡��λ��
	setAngle(0.0f);		//����֡�ĽǶ�
	m_scale.set(1.0f);	//����֡�����ű���

	m_isInited = XTrue;
	return XTrue;
}
XBool XFrameEx::releaseMem()
{
	if (!m_isInited) return XTrue;
	if (m_isThreadLoading)//������Ҫ�ȴ��߳̽���
		waitThreadEnd(m_loadThread);

	XMem::XDELETE_ARRAY(m_pSprite);

#if FRAME_DEBUG
	-- frameTestSum;
	if (frameTestSum == 10)
	{
		printf("haha\n");
	}
	printf("Frame new sum:%d\n", frameTestSum);
#endif
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(this);
#endif

	m_isInited = XFalse;
	return XTrue;
}
void XFrameEx::draw()
{//����֡����
	if (!m_isVisible ||
		!m_isInited ||
		(m_isDisappearAtEnd && m_isEnd)) return;	//�������֮����ʧ
	if (m_isThreadLoading) return;	//���������ʱ�������
	int keyFrameIndex = 0;
	if (m_isEnd)
	{
		if (m_isStopAtEnd) keyFrameIndex = m_keyFrameArray[m_allFramesSum - 1];
		else keyFrameIndex = 0;
	}
	else
	{
		keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	}
	if (keyFrameIndex >= 0 && keyFrameIndex < m_allKeyFramesSum)
	{
		//	m_pSprite[keyFrameIndex].m_pShaderProc = m_pShaderProc;
		m_pSprite[keyFrameIndex].setShaderClass(m_pShader);// = m_pShader;
		m_pSprite[keyFrameIndex].draw();
		//	m_pSprite[keyFrameIndex].m_isFlash = 0;
		//	m_pSprite[keyFrameIndex].m_pShaderProc = NULL;
	}
}
void XFrameEx::move(float timeDelay)
{
	if (!m_isInited || m_isEnd ||
		m_isThreadLoading || m_isPause) return;	//���������ʱ���ܸ���
	if (m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;

	float tmp = m_curFramesNumble;
	m_curFramesNumble += m_actionSpeed * timeDelay;			//�ƶ�����֡����
	if (m_pauseAtIndex >= 0 &&
		tmp < m_pauseAtIndex && m_curFramesNumble >= m_pauseAtIndex)
	{//������Ʋ��ŵ�ָ��֮֡����ͣ
		m_curFramesNumble = m_pauseAtIndex;
		m_pauseAtIndex = -1;
		pause();
	}
	if (m_curFramesNumble >= m_allFramesSum)
	{//������ŵ����һ֡�������֡
		m_curFramesNumble = 0.0f;

		if (!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;//������ǿ������̽����Ķ��������ﲥ�����һ��ѭ��֮�󣬼��ɽ���
		if (!m_isLoop) m_isEnd = XTrue;//����ǲ�ѭ���Ķ�������Ҳ���ԱȽϽ���
	}
	else
	if (m_curFramesNumble < 0.0f)
	{//����ǵ��򲥷ŵĻ��������ж��Ƿ񲥷����(�Լ�������ɴ���)
		m_curFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if (!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if (!m_isLoop) m_isEnd = XTrue;
	}
}
void XFrameEx::setAttribute(const XVec2& position, XBool loop, XBool endImmediately, 
	int startFrame, float actionSpeed, XBool disappearAtEnd, XBool isOverturn)
{
	if (!m_isInited) return;
	m_position = position;	//����֡��λ��

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_curFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;
	m_isDisappearAtEnd = disappearAtEnd;

	if ((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
		m_isOverturn = isOverturn;
	if (m_isThreadLoading)
	{
		m_neadUpdateAttribute = true;
	}
	else
	{
		//�����õ��������õ������ڲ�ʹ֮��Ч
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			m_pSprite[i].setPosition(m_position);
			m_pSprite[i].setOverturn(m_isOverturn);
		}
	}
}
XFrameEx::XFrameEx()
	:m_resoursePosition(RES_AUTO)
	, m_pSprite(NULL)
	, m_angle(0.0f)
	, m_isVisible(XTrue)
	, m_position(0.0f)
	, m_scale(1.0f)
	, m_color(1.0f, 1.0f)
//	,alpha(1.0f)
//	,colorRed(1.0f)
//	,colorGreen(1.0f)
//	,colorBlue(1.0f)
	, m_isOverturn(XFalse)	//Ĭ������²���ת
	, m_allFramesSum(0)							//����֡��������֡��
	, m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
	, m_curFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
	, m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
	, m_isLoop(XFalse)								//����֡�Ƿ����ѭ��
	, m_startFrame(0)							//����һ֡��ʼ
	, m_isEnd(XFalse)								//����֡�Ƿ񲥷����
	, m_isEndImmediately(XFalse)					//�����Ƿ�Ϊ���̽�������
	, m_isSetEnd(XFalse)							//is end by user			//�Ƿ���������֡����
	, m_isDisappearAtEnd(XFalse)					//����֡�Ƿ��ڲ������֮����ʧ
	, m_isInited(XFalse)
	, m_frameName(NULL)
	, m_keyFrameArray(NULL)
	, m_isACopy(XFalse)
//	, m_blendTypeScr(4)
//	, m_blendTypeDst(5)
	//,m_pShaderProc(NULL)
	, m_pShader(NULL)
	, m_cp(NULL)
	, m_isThreadLoading(false)
	, m_neadUpdateAttribute(false)
	, m_isPause(false)
	, m_isStopAtEnd(false)
	, m_pauseAtIndex(-1)
	, m_withFileList(false)
{
	m_objType = OBJ_FRAMEEX;
}
XFrameEx::~XFrameEx()
{
	releaseMem();
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
}
XFrameEx& XFrameEx::operator = (const XFrameEx& temp)
{
	if (&temp == this) return *this;			//��ֹû��Ҫ�����ҿ���
	//if(m_isInited != 0) return *this;			//�����Ѿ���ʼ�������Ͳ����ٸ��Ʊ���
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XMem::XDELETE_ARRAY(m_pSprite);

//	m_blendTypeScr = temp.m_blendTypeScr;
//	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	m_angle = temp.m_angle;					//����ĽǶ�
	m_position = temp.m_position;		//�����λ��
	m_scale = temp.m_scale;				//��������ųߴ�
	m_color = temp.m_color;
//	alpha = temp.alpha;					//͸����
//	colorRed = temp.colorRed;			//��ɫ
//	colorGreen = temp.colorGreen;		//��ɫ
//	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת
	m_isPause = temp.m_isPause;
	m_isStopAtEnd = temp.m_isStopAtEnd;
	m_pauseAtIndex = temp.m_pauseAtIndex;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_curFramesNumble = temp.m_curFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if (temp.m_isInited)
	{
#if FRAME_DEBUG
		++ frameTestSum;
		printf("Frame new sum:%d\n", frameTestSum);
#endif
		m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
		if (m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return *this;
		}
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//ע������֡������
	if (!m_isACopy) XObjManager.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void XFrameEx::setACopy(const XFrameEx& temp)
{
	if (&temp == this) return;			//��ֹû��Ҫ�����ҿ���
	if (m_isThreadLoading || temp.m_isThreadLoading) return;	//���ڶ��߳������Ԫ�ز��ܸ���
	//if(m_isInited != 0) return;			//�����Ѿ���ʼ�������Ͳ����ٸ��Ʊ���
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XMem::XDELETE_ARRAY(m_pSprite);

//	m_blendTypeScr = temp.m_blendTypeScr;
//	m_blendTypeDst = temp.m_blendTypeDst;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	m_angle = temp.m_angle;					//����ĽǶ�
	m_position = temp.m_position;		//�����λ��
	m_scale = temp.m_scale;				//��������ųߴ�
	m_color = temp.m_color;
	//	alpha = temp.alpha;					//͸����
	//	colorRed = temp.colorRed;			//��ɫ
	//	colorGreen = temp.colorGreen;		//��ɫ
	//	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת
	m_isPause = temp.m_isPause;
	m_isStopAtEnd = temp.m_isStopAtEnd;
	m_pauseAtIndex = temp.m_pauseAtIndex;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_curFramesNumble = temp.m_curFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if (temp.m_isInited)
	{
#if FRAME_DEBUG
		++ frameTestSum;
		printf("Frame new sum:%d\n", frameTestSum);
#endif
		m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
		if (m_pSprite == NULL)
		{
			m_isInited = XFalse;
			return;
		}
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//ע������֡������,���m_isACopy���ж���Ϊ�˷�ֹ�ظ�ע�ᣬ����ע�᱾����Ѿ��������ظ�ע��
	if (!m_isACopy) XObjManager.addAObject(this);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
XFrameEx::XFrameEx(const XFrameEx &temp)
{
	if (&temp == this) return;	//��ֹ�����
}
bool XFrameEx::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if ((fp = fopen(filename, "rb")) == NULL)
	{//���ͼƬ�ļ����������������ļ�������������ļ�
		XDirectory dir;
		if (!dir.init(XFile::getFilePath(XFile::getFullPath(m_frameName).c_str()).c_str(), false)) return false;
		dir.sortFiles();
		for (int i = 0; i < dir.m_sortResult.size(); ++i)
		{
			if (!getIsImageSupport(XFile::getFileExtension(dir.m_sortResult[i]->filename.c_str()))) continue;
			m_filesList.push_back(dir.m_sortResult[i]->filename);
		}

		m_withFileList = !XFile::isExistFileEx(m_frameName);

		m_allKeyFramesSum = m_filesList.size();
		if (m_allKeyFramesSum == 0)
		{
			LogStr("Action text file error!");
			return XFalse;
		}
		m_allFramesSum = m_allKeyFramesSum;
		m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
		if (m_keyFrameArray == NULL) return XFalse;
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			m_keyFrameArray[i] = i;
		}
		return true;
	}
	else
	{//��������ļ����ڣ���ͼƬ������Ҫ��˳���
		if (!XFile::isExistFileEx(m_frameName))
		{//���ָ���ļ������ڣ��ҵ���һ��
			XDirectory dir;
			if (!dir.init(XFile::getFilePath(XFile::getFullPath(m_frameName).c_str()).c_str(), false)) return false;
			dir.sortFiles();
			for (int i = 0; i < dir.m_sortResult.size(); ++i)
			{
				if (getIsImageSupport(XFile::getFileExtension(dir.m_sortResult[i]->filename.c_str())))
				{
					int index = XFile::getPathDeepByChar(m_frameName);
					m_frameName[index + 1] = '\0';
					strcat_s(m_frameName, MAX_FILE_NAME_LENGTH, dir.m_sortResult[i]->filename.c_str());
					break;
				}
			}
		}
	}

	if (fscanf(fp, "%d:", &m_allKeyFramesSum) != 1) { fclose(fp); return XFalse; }
	if (m_allKeyFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%d:", &m_allFramesSum) != 1) 
	{//���ֻ�е�һ�����������ȫ��ʹ��Ĭ�ϲ���
		m_allFramesSum = m_allKeyFramesSum;
		m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
		if (m_keyFrameArray == NULL) return XFalse;
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			m_keyFrameArray[i] = i;
		}
		fclose(fp);
		return true; 
	}
	if (m_allFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
	if (m_keyFrameArray == NULL) return XFalse;
	//��ȡ��Ƿ�	D��default, M��menutrue
	char tempFlag = ' ';
	if (fscanf(fp, "%c:", &tempFlag) != 1) { fclose(fp); return XFalse; }
	if (tempFlag == 'D' || tempFlag == 'd')
	{
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			m_keyFrameArray[i] = i;
		}
	}
	else
	{
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			if (fscanf(fp, "%d,", &m_keyFrameArray[i]) != 1) { fclose(fp); return XFalse; }
			if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
			{
				LogNull("Blank frame:%s -> %d!\n", filename, i);
				//	fclose(fp);
				//	XMem::XDELETE_ARRAY(m_keyFrameArray);
				//	return XFalse;
			}
		}
	}
	
	fclose(fp);
	return true;
}
bool XFrameEx::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	unsigned char *p = XResPack.getFileData(filename);
	if (p == NULL) return XFalse;

	int offset = 0;
	if (sscanf((char *)(p + offset), "%d:", &m_allKeyFramesSum) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allKeyFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		XMem::XDELETE_ARRAY(m_frameName);
		return XFalse;
	}
	if (sscanf((char *)(p + offset), "%d:", &m_allFramesSum) != 1) 
	{//���ֻ��ȡ��һ��������ʹ��Ĭ��ֵ
		m_allFramesSum = m_allKeyFramesSum;
		m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
		if (m_keyFrameArray == NULL) return XFalse;
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			m_keyFrameArray[i] = i;
		}

		XMem::XDELETE_ARRAY(p);
		return true; 
	}
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allFramesSum <= 0)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	m_keyFrameArray = XMem::createArrayMem<int>(m_allFramesSum);
	if (m_keyFrameArray == NULL)
	{
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//��ȡ��Ƿ�	D��default, M��menutrue
	char tempFlag = ' ';
	if (sscanf((char *)(p + offset), "%c:", &tempFlag) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (tempFlag == 'D' || tempFlag == 'd')
	{
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			m_keyFrameArray[i] = i;
		}
	}
	else
	{
		for (int i = 0; i < m_allFramesSum; ++i)
		{
			if (sscanf((char *)(p + offset), "%d,", &m_keyFrameArray[i]) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
			offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
			if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
			{
				LogNull("Blank frame:%s -> %d!\n", filename, i);
				//	XMem::XDELETE_ARRAY(p);
				//	XMem::XDELETE_ARRAY(m_keyFrameArray);
				//	return XFalse;
			}
		}
	}
	
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFrameEx::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return true;
}
bool XFrameEx::loadData(const char *filename,			//ͼƬ������
	XResPos resPos, bool isInThread)
{
	m_resoursePosition = resPos;
	try
	{
		m_cp = new XSCounter;
		if (m_cp == NULL) return XFalse;
	}
	catch (...)
	{
		return XFalse;
	}
	m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if (m_frameName == NULL)
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_frameName[0] = '\0';

	strcpy_s(m_frameName, MAX_FILE_NAME_LENGTH, filename);

	//�����������е�����֡ͼƬ
	int nameLength = strlen(m_frameName);
	if (nameLength <= 0)
	{
		LogStr("Action file path error!");
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	std::string tmpExt = XFile::getFileExtension(m_frameName);
	if (tmpExt.size() == 0 && (m_frameName[nameLength - 1] != '\\' ||
		m_frameName[nameLength - 1] != '/'))
	{
		strcat_s(m_frameName, MAX_FILE_NAME_LENGTH, "/");
	}
	//�����ȡ����֡�������ı��ĵ�����
	char tempFrameName[MAX_FILE_NAME_LENGTH];
	strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);

	int index = XFile::getPathDeepByChar(tempFrameName);
	tempFrameName[index + 1] = '\0';
	strcat_s(tempFrameName, MAX_FILE_NAME_LENGTH, "frame.txt");
	//�Զ��޸��ļ�������ֹ��������
	if (index == nameLength)
	{//�������ļ�����ʹ��Ĭ���ļ���
		strcat_s(m_frameName, MAX_FILE_NAME_LENGTH, "frame_000.png");
		nameLength = strlen(m_frameName);
	}
	else
	{
		if (!getIsImageSupport(tmpExt))
		{
			m_frameName[index + 1] = '\0';
			strcat_s(m_frameName, MAX_FILE_NAME_LENGTH, "frame_000.png");
			nameLength = strlen(m_frameName);
		}
	}
	//�������Ҫ���Ƕ��߳���Դ���⣬Ψһ��һ�����߳����Ѿ����˻��⴦��
	switch (m_resoursePosition)
	{
	case RES_LOCAL_PACK:
		if (!loadFromPacker(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_LOCAL_FOLDER:
		if (!loadFromFolder(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_WEB:
		if (!loadFromWeb(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_AUTO:
		if (!loadFromPacker(tempFrameName) && !loadFromFolder(tempFrameName) &&
			!loadFromWeb(tempFrameName))
		{
			XMem::XDELETE_ARRAY(m_frameName);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	}

#if FRAME_DEBUG
	++ frameTestSum;
	printf("Frame new sum:%d\n", frameTestSum);
#endif
	m_pSprite = XMem::createArrayMem<XSprite>(m_allKeyFramesSum);
	if (m_pSprite == NULL)
	{
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE(m_cp);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		return XFalse;
	}
	if (m_withFileList)
	{
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			tempFrameName[index + 1] = '\0';				//��·��
			strcat_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_filesList[i].c_str());

			bool ret;
			if (isInThread) ret = m_pSprite[i].initThread(tempFrameName, m_resoursePosition, POINT_CENTER);
			else ret = m_pSprite[i].init(tempFrameName, m_resoursePosition, POINT_CENTER);
			if (!ret)
			{
				LogStr("The action pictrue load error!");
				for (int j = 0; j < i; ++j)
				{
					m_pSprite[j].release();
				}
				XMem::XDELETE_ARRAY(m_pSprite);
				XMem::XDELETE_ARRAY(m_frameName);
				XMem::XDELETE(m_cp);
				XMem::XDELETE_ARRAY(m_keyFrameArray);
				return XFalse;
			}
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
			m_pSprite[i].setPosition(XVec2::zero);
			m_pSprite[i].setAngle(0.0f);
			m_pSprite[i].setScale(XVec2::one);
		}
	}
	else
	{
		strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);	//���ļ���

		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{

			tempFrameName[nameLength - 5] = '0' + i % 10;
			tempFrameName[nameLength - 6] = '0' + (i / 10) % 10;
			tempFrameName[nameLength - 7] = '0' + (i / 100) % 10;
			bool ret;
			if (isInThread) ret = m_pSprite[i].initThread(tempFrameName, m_resoursePosition, POINT_CENTER);
			else ret = m_pSprite[i].init(tempFrameName, m_resoursePosition, POINT_CENTER);
			if (!ret)
			{
				LogStr("The action pictrue load error!");
				for (int j = 0; j < i; ++j)
				{
					m_pSprite[j].release();
				}
				XMem::XDELETE_ARRAY(m_pSprite);
				XMem::XDELETE_ARRAY(m_frameName);
				XMem::XDELETE(m_cp);
				XMem::XDELETE_ARRAY(m_keyFrameArray);
				return XFalse;
			}
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
			XObjManager.decreaseAObject(&(m_pSprite[i]));
#endif
			m_pSprite[i].setPosition(XVec2::zero);
			m_pSprite[i].setAngle(0.0f);
			m_pSprite[i].setScale(XVec2::one);
		}
	}
#if WITH_OBJECT_MANAGER	//ע������֡������
	XObjManager.addAObject(this);
#endif

	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
DWORD WINAPI XFrameEx::loadThread(void *p)
{
	XFrameEx &pPar = *(XFrameEx *)p;
	pPar.m_loadThread = STATE_START;
	//��������������
//	printf("��ʼ����!\n");
	if (gFrameworkData.pFrameWork == NULL)
	{
		printf("������δ��ʼ��!");
		pPar.m_loadThread = STATE_END;
		pPar.m_isThreadLoading = false;
		return 1;
	}
//	XEG.hdcLock();
//	wglMakeCurrent(XEG.getHDC(), XEG.getCopyHGLRC());
	if (!pPar.loadData(pPar.m_threadFilename.c_str(), pPar.m_threadResPos,true))
	{
		pPar.m_isInited = false;
		LogStr("Frame load error!");
	}
//	wglMakeCurrent(NULL, NULL);//nullptr
//	XEG.hdcUnlock();
	if (pPar.m_neadUpdateAttribute)
	{
		pPar.m_neadUpdateAttribute = false;
		for (int i = 0; i < pPar.m_allKeyFramesSum; ++i)
		{
			pPar.m_pSprite[i].setPosition(pPar.m_position);
			pPar.m_pSprite[i].setOverturn(pPar.m_isOverturn);
		}
	}

//	printf("�������!\n");
	pPar.m_loadThread = STATE_END;
	pPar.m_isThreadLoading = false;
	return 1;
}
bool XFrameEx::initThread(const char *filename,			//ͼƬ������
	XResPos resPos)
{//���￪��һ���߳�������Դ
	if (filename == NULL) return false;
	if (resPos == RES_SYS_DEF) resPos = getDefResPos();
	//ò�ƿ��Բ�����������ж�(ע��)
	if (resPos == RES_LOCAL_PACK)
		return init(filename, resPos);	//����Ǵ���Դ����������Դ�����ܽ��ж��߳����룬ֱ��ʹ����ͨ��ʽ��

	if (m_isInited)
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	//���濪��һ�������߳̽����������
	m_threadFilename = filename;
	m_threadResPos = resPos;
	m_position.reset();	//����֡��λ��
	m_scale.set(1.0f);	//����֡�����ű���

	m_isInited = true;
	m_loadThread = STATE_BEFORE_START;
	m_isThreadLoading = true;
	if (CreateThread(0, 0, loadThread, this, 0, 0) == 0)
	{
		m_isInited = false;
		return false;
	}
	if (m_isInited)
		setAngle(0);		//����֡�ĽǶ�
	return XTrue;
}
char * XFrameExParamData::getStrFromData()
{
	char *tempStr = XMem::createArrayMem<char>(256);
	if (tempStr == NULL) return NULL;
	int offset = 0;
	sprintf_s(tempStr + offset, 256 - offset, "ResPos:%d,\n", resourcePosition);
	offset = strlen(tempStr);
	sprintf_s(tempStr + offset, 256 - offset, "ObjPos:%f|%f,\n", pos.x, pos.y);
	offset = strlen(tempStr);
	if (loop) sprintf_s(tempStr + offset, 256 - offset, "Loop:1,\n");
	else sprintf_s(tempStr + offset, 256 - offset, "Loop:0,\n");
	offset = strlen(tempStr);
	if (endImmediately) sprintf_s(tempStr + offset, 256 - offset, "EndIMD:1,\n");
	else sprintf_s(tempStr + offset, 256 - offset, "EndIMD:0,\n");
	offset = strlen(tempStr);
	sprintf_s(tempStr + offset, 256 - offset, "StartFRM:%d,\n", startFrame);
	offset = strlen(tempStr);
	sprintf_s(tempStr + offset, 256 - offset, "Speed:%f,\n", actionSpeed);
	offset = strlen(tempStr);
	if (disappearAtEnd) sprintf_s(tempStr + offset, 256 - offset, "disappear:1,\n");
	else sprintf_s(tempStr + offset, 256 - offset, "disappear:0,\n");
	offset = strlen(tempStr);
	if (isOverturn) sprintf_s(tempStr + offset, 256 - offset, "Overturn:1,\n");
	else sprintf_s(tempStr + offset, 256 - offset, "Overturn:0,\n");
	offset = strlen(tempStr);
	return tempStr;
}
int XFrameExParamData::getDataFromStr(const char * str)
{
	if (str == NULL) return 0;
	int tempData = 0;
	int offset = 0;
	if (sscanf(str + offset, "ResPos:%d,\n", &resourcePosition) != 1) return offset;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "ObjPos:%f|%f,\n", &pos.x, &pos.y) != 2) return offset;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "Loop:%d,\n", &tempData) != 1) return offset;
	if (tempData == 0) loop = XFalse;
	else loop = XTrue;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "EndIMD:%d,\n", &tempData) != 1) return offset;
	if (tempData == 0) endImmediately = XFalse;
	else endImmediately = XTrue;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "StartFRM:%d,\n", &startFrame) != 1) return offset;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "Speed:%f,\n", &actionSpeed) != 1) return offset;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "disappear:%d,\n", &tempData) != 1) return offset;
	if (tempData == 0) disappearAtEnd = XFalse;
	else disappearAtEnd = XTrue;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	if (sscanf(str + offset, "Overturn:%d,\n", &tempData) != 1) return offset;
	if (tempData == 0) isOverturn = XFalse;
	else isOverturn = XTrue;
	offset += XString::getCharPosition(str + offset, ',') + 1;
	return offset;
}
XBool XFrameExParamData::getDataFromFile(FILE *fp)
{
	if (fp == NULL) return XFalse;
	int tempData = 0;
	if (fscanf(fp, "ResPos:%d,\n", &resourcePosition) != 1) return XFalse;
	if (fscanf(fp, "ObjPos:%f|%f,\n", &pos.x, &pos.y) != 2) return XFalse;
	if (fscanf(fp, "Loop:%d,\n", &tempData) != 1) return XFalse;
	if (tempData == 0) loop = XFalse;
	else loop = XTrue;
	if (fscanf(fp, "EndIMD:%d,\n", &tempData) != 1) return XFalse;
	if (tempData == 0) endImmediately = XFalse;
	else endImmediately = XTrue;
	if (fscanf(fp, "StartFRM:%d,\n", &startFrame) != 1) return XFalse;
	if (fscanf(fp, "Speed:%f,\n", &actionSpeed) != 1) return XFalse;
	if (fscanf(fp, "disappear:%d,\n", &tempData) != 1) return XFalse;
	if (tempData == 0) disappearAtEnd = XFalse;
	else disappearAtEnd = XTrue;
	if (fscanf(fp, "Overturn:%d,\n", &tempData) != 1) return XFalse;
	if (tempData == 0) isOverturn = XFalse;
	else isOverturn = XTrue;
	return XTrue;
}
void XFrameEx::setPosition(const XVec2& p)
{
	if (!m_isInited) return;
	m_position = p;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setPosition(m_position);
	}
	updateChildPos();
}
void XFrameEx::setScale(const XVec2& s)
{
	if (!m_isInited || s.x < 0.0f || s.y < 0.0f) return;
	m_scale = s;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setScale(m_scale);
	}
	updateChildScale();
}
void XFrameEx::setAngle(float temp)
{
	if (!m_isInited) return;
	m_angle = temp;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setAngle(m_angle);
	}
	updateChildAngle();
}
void XFrameEx::setColor(const XFColor&c)
{
	if (!m_isInited) return;
	if (c.r >= 0.0f) m_color.r = c.r;
	if (c.g >= 0.0f) m_color.g = c.g;
	if (c.b >= 0.0f) m_color.b = c.b;
	if (c.a >= 0.0f) m_color.a = c.a;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setColor(c);
	}
	updateChildColor();
}
void XFrameEx::setAlpha(float a)
{
	if (!m_isInited) return;
	if (a >= 0) m_color.a = a;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_pSprite[i].setAlpha(a);
	}
	updateChildAlpha();
}
XBool XFrameEx::isInRect(const XVec2& p)	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	int temp = m_curFramesNumble;
	if (m_curFramesNumble < 0.0f || m_curFramesNumble >= m_allFramesSum) temp = 0;
	if (m_keyFrameArray[temp] < 0 || m_keyFrameArray[temp] >= m_allKeyFramesSum) return false;
	return m_pSprite[m_keyFrameArray[temp]].isInRect(p);
}
//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
XVec2 XFrameEx::getBox(int order)
{
	int temp = m_curFramesNumble;
	if (m_curFramesNumble < 0.0f || m_curFramesNumble >= m_allFramesSum) temp = 0;
	if (m_keyFrameArray[temp] < 0 || m_keyFrameArray[temp] >= m_allKeyFramesSum) return XVec2::zero;
	return m_pSprite[m_keyFrameArray[temp]].getBox(order);
}
#if !WITH_INLINE_FILE
#include "XFrameEx.inl"
#endif
}