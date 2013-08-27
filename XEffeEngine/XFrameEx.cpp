//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrameEx.h"
#include "XBasicSDL.h"
#include "XBasicOpenGL.h"
#include "XObjectManager.h"

#include "XResourcePack.h"
#include "XLogBook.h"

#define FRAME_DEBUG (0)
#if FRAME_DEBUG
int frameTestSum = 0;
#endif

int _XFrameEx::init(const char *filename,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The action have initted!\n");
		return 1;
	}

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;

	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	try
	{
		m_frameName = new char[MAX_FRAME_FILENAME_LENGTH];
		if(m_frameName == NULL) 
		{
			XDELETE(m_cp);
			return 0;
		}
	}catch(...)
	{
		XDELETE(m_cp);
		return 0;
	}
	m_frameName[0] = '\0';

	strcpy(m_frameName,filename);

	//依次载入所有的序列帧图片
	int nameLength = strlen(m_frameName);
	if(nameLength <= 0)
	{
		AddLogInfoStr("Action file path error!\n");
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
		return 0;
	}
	//下面读取序列帧动画的文本文档数据
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
	//下面这个要考虑多线程资源互斥，唯一的一个多线程中已经做了互斥处理
	if(m_resoursePosition == RESOURCE_INSIDE)
	{
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(tempFrameName);
		if(lengthTemp < 0) 
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[lengthTemp + 1];
			if(p == NULL) 
			{
				XDELETE_ARRAY(m_frameName);
				XDELETE(m_cp);
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(tempFrameName,p);
		*(p + lengthTemp) = '\0';
		int offset = 0;
		sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allKeyFramesSum <= 0)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		sscanf((char *)(p + offset),"%d:",&m_allFramesSum);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allFramesSum <= 0)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		try
		{
			m_keyFrameArray = new int[m_allFramesSum];
			if(m_keyFrameArray == NULL) 
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_frameName);
				XDELETE(m_cp);
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		{//读取标记符	D：default, M：menutrue
			char tempFlag = ' ';
			sscanf((char *)(p + offset),"%c:",&tempFlag);
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
					sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]);
					offset += getCharPosition((char *)(p + offset),',') + 1;
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						AddLogInfoNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						XDELETE_ARRAY(p);
						XDELETE_ARRAY(m_frameName);
						XDELETE_ARRAY(m_keyFrameArray);
						XDELETE(m_cp);
						return 0;
					}
				}
			}
		}
		XDELETE_ARRAY(p);
	}else
	{
		FILE *fp;
		if((fp = fopen(tempFrameName,"rb")) == NULL)
		{
			AddLogInfoStr("Action text file error!\n");
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}

		fscanf(fp,"%d:",&m_allKeyFramesSum);
		if(m_allKeyFramesSum <= 0)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		fscanf(fp,"%d:",&m_allFramesSum);
		if(m_allFramesSum <= 0)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		try
		{
			m_keyFrameArray = new int[m_allFramesSum];
			if(m_keyFrameArray == NULL) 
			{
				XDELETE_ARRAY(m_frameName);
				XDELETE(m_cp);
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			return 0;
		}
		{//读取标记符	D：default, M：menutrue
			char tempFlag = ' ';
			fscanf(fp,"%c:",&tempFlag);
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
					fscanf(fp,"%d,",&m_keyFrameArray[i]);
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						AddLogInfoNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						fclose(fp);
						XDELETE_ARRAY(m_frameName);
						XDELETE_ARRAY(m_keyFrameArray);
						XDELETE(m_cp);
						return 0;
					}
				}
			}
		}
		fclose(fp);
	}

	try
	{
#if FRAME_DEBUG
		frameTestSum ++;
		printf("Frame new sum:%d\n",frameTestSum);
#endif
		m_pSprite = new _XSprite[m_allKeyFramesSum];
		if(m_pSprite == NULL) 
		{
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			XDELETE_ARRAY(m_keyFrameArray);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
		XDELETE_ARRAY(m_keyFrameArray);
		return 0;
	}
	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		tempFrameName[nameLength - 5] = '0' + i%10;
		tempFrameName[nameLength - 6] = '0' + (i/10)%10;
		tempFrameName[nameLength - 7] = '0' + (i/100)%10;

		if(m_pSprite[i].init(tempFrameName,m_resoursePosition,POINT_CENTER) == 0)
		{
			AddLogInfoStr("The action pictrue load error!\n");
			for(int j = 0;j < i;++ j)
			{
				m_pSprite[j].release();
			}
			XDELETE_ARRAY(m_pSprite);
			XDELETE_ARRAY(m_frameName);
			XDELETE(m_cp);
			XDELETE_ARRAY(m_keyFrameArray);
			return 0;
		}
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
		_XObjectManager::GetInstance().decreaseAObject(&(m_pSprite[i]));
#endif
		m_pSprite[i].setPosition(0.0f,0.0f);
		m_pSprite[i].setAngle(0);
		m_pSprite[i].setSize(1.0f,1.0f);
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件
	_XObjectManager::GetInstance().addAObject(this,OBJ_FRAMEEX);
#endif

	x = 0.0f;			//序列帧的位置
	y = 0.0f;
	setAngle(0);		//序列帧的角度
	xsize = 1.0f;		//序列帧的缩放比例
	ysize = 1.0f;

	m_isInited = 1;
	m_isEnd = 0;
	m_isSetEnd = 0;
	return 1;
}

int _XFrameEx::releaseMem()
{
	if(m_isInited == 0) return 1;

	XDELETE_ARRAY(m_pSprite);
	
#if FRAME_DEBUG
	frameTestSum --;
	if(frameTestSum == 10)
	{
		printf("haha\n");
	}
	printf("Frame new sum:%d\n",frameTestSum);
#endif
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif

	m_isInited = 0;
	return 1;
}

void _XFrameEx::draw()
{//序列帧精灵
	if(m_isVisiable == 0) return;
	if(m_isInited == 0) return;
	if(0 != m_isDisappearAtEnd && 0 != m_isEnd) return;	//播放完成之后消失

	int temp_Frame = m_keyFrameArray[(int)(m_nowFramesNumble)];
	m_pSprite[temp_Frame].m_pShaderProc = m_pShaderProc;
	m_pSprite[temp_Frame].draw();
//	m_pSprite[temp_Frame].m_isFlash = 0;
//	m_pSprite[temp_Frame].m_pShaderProc = NULL;
}

int _XFrameEx::isInRect(float x,float y)	//点x，y是否在物件身上，这个x，y是屏幕的绝对坐标
{
	int temp = m_nowFramesNumble;
	if(m_nowFramesNumble < 0 || m_nowFramesNumble >= m_allFramesSum) temp = 0;
	return m_pSprite[m_keyFrameArray[temp]].isInRect(x,y);
}

	//获取四个顶点的坐标，目前先不考虑旋转和缩放
_XVector2 _XFrameEx::getBox(int order)
{
	int temp = m_nowFramesNumble;
	if(m_nowFramesNumble < 0 || m_nowFramesNumble >= m_allFramesSum) temp = 0;
	return m_pSprite[m_keyFrameArray[temp]].getBox(order);
}

void _XFrameEx::move(int timeDelay)
{
	if(0 == m_isInited || 0 != m_isEnd)
	{
		return;
	}
	if(0 != m_isEndImmediately && 0 != m_isSetEnd)
	{
		m_isEnd = 1;
	}
	m_nowFramesNumble += m_actionSpeed * timeDelay;			//推动序列帧动画
	if(m_nowFramesNumble >= m_allFramesSum) 
	{//如果播放到最后一帧，这里回帧
		m_nowFramesNumble =0;

		if(0 == m_isEndImmediately)
		{//如果不是可以立刻结束的动画，这里播放完成一个循环之后，即可结束
			if(0 != m_isSetEnd) m_isEnd =1;
		}
		if(0 == m_isLoop)
		{//如果是不循环的动画这里也可以比较结束
			m_isEnd =1;
		}
	}else	
		if(m_nowFramesNumble < 0) 
		{//如果是倒序播放的话，这里判断是否播放完成(以及进行完成处理)
			m_nowFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

			if(0 == m_isEndImmediately)
			{
				if(0 != m_isSetEnd) m_isEnd =1;
			}
			if(0 == m_isLoop)
			{
				m_isEnd =1;
			}
		}
}

void _XFrameEx::setAttribute(const _XVector2& position,int loop,int endImmediately,int startFrame,float actionSpeed,int disappearAtEnd,int isOverturn)
{
	if(m_isInited == 0) return;
	x = position.x;			//序列帧的位置
	y = position.y;

	if(loop != 0) m_isLoop = 1;
	else m_isLoop = 0;

	if(endImmediately != 0) m_isEndImmediately = 1;
	else m_isEndImmediately = 0;

	m_startFrame = startFrame;
	m_nowFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	if(disappearAtEnd != 0) m_isDisappearAtEnd = 1;
	else m_isDisappearAtEnd = 0;

	if(m_isOverturn != isOverturn)
	{
		if(isOverturn != 0) m_isOverturn = 1;
		else m_isOverturn = 0;
	}
	//将设置的属性设置到精灵内部使之生效
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_pSprite[i].setPosition(x,y);
		m_pSprite[i].setOverturn(m_isOverturn);
	}
}
_XFrameEx::_XFrameEx()
:m_resoursePosition(RESOURCE_OUTSIDE)
,m_pSprite(NULL)
,angle(0.0f)
,m_isVisiable(1)
,x(0.0f),y(0.0f)
,xsize(1.0f),ysize(1.0f)
,alpha(1.0f)
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_isOverturn(0)	//默认情况下不翻转
,m_allFramesSum(0)							//序列帧动画中总帧数
,m_allKeyFramesSum(0)						//序列帧动画中关键帧的数量
,m_nowFramesNumble(0)					//当前播放到第几帧
,m_actionSpeed(0)						//序列帧播放的速度
,m_isLoop(0)								//序列帧是否可以循环
,m_startFrame(0)							//从哪一帧开始
,m_isEnd(0)								//序列帧是否播放完成
,m_isEndImmediately(0)					//动画是否为立刻结束类型
,m_isSetEnd(0)							//is end by user			//是否设置序列帧结束
,m_isDisappearAtEnd(0)					//动画帧是否在播放完成之后消失
,m_isInited(0)
,m_frameName(NULL)
,m_keyFrameArray(NULL)
,m_isACopy(0)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_pShaderProc(NULL)
,m_cp(NULL)
{
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
	if(&temp == this) return *this;			//防止没必要的自我拷贝
	//if(m_isInited != 0) return *this;			//自身已经初始化过，就不能再复制别人
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_pShaderProc = temp.m_pShaderProc;
	m_isVisiable = temp.m_isVisiable;					//精灵的角度
	angle = temp.angle;					//精灵的角度
	x = temp.x;
	y = temp.y;							//精灵的位置
	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_nowFramesNumble = temp.m_nowFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited != 0)
	{
		try
		{
#if FRAME_DEBUG
			frameTestSum ++;
			printf("Frame new sum:%d\n",frameTestSum);
#endif
			m_pSprite = new _XSprite[m_allKeyFramesSum];
			if(m_pSprite == NULL)
			{
				m_isInited = 0;
				return *this;
			}
		}catch(...)
		{
			m_isInited = 0;
			return *this;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
			_XObjectManager::GetInstance().decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件
	if(m_isACopy == 0) _XObjectManager::GetInstance().addAObject(this,OBJ_FRAMEEX);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = 1;
	return *this;
}

void _XFrameEx::setACopy(const _XFrameEx& temp)
{
	if(&temp == this) return;			//防止没必要的自我拷贝
	//if(m_isInited != 0) return;			//自身已经初始化过，就不能再复制别人
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//释放自身的资源
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//自身没有引用需要释放
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(m_frameName);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	XDELETE_ARRAY(m_pSprite);

	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_pShaderProc = temp.m_pShaderProc;
	m_isVisiable = temp.m_isVisiable;					//精灵的角度
	angle = temp.angle;					//精灵的角度
	x = temp.x;
	y = temp.y;							//精灵的位置
	xsize = temp.xsize;
	ysize = temp.ysize;					//精灵的缩放尺寸
	alpha = temp.alpha;					//透明度
	colorRed = temp.colorRed;			//红色
	colorGreen = temp.colorGreen;		//绿色
	colorBlue = temp.colorBlue;			//蓝色
	m_isOverturn = temp.m_isOverturn;	//是否x方向左右翻转

	m_allFramesSum = temp.m_allFramesSum;					//序列帧动画中总帧数
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//序列帧动画中关键帧的数量
	m_nowFramesNumble = temp.m_nowFramesNumble;				//当前播放到第几帧
	m_actionSpeed = temp.m_actionSpeed;						//序列帧播放的速度
	m_isLoop = temp.m_isLoop;								//序列帧是否可以循环
	m_startFrame = temp.m_startFrame;						//从哪一帧开始
	m_isEnd = temp.m_isEnd;									//序列帧是否播放完成
	m_isEndImmediately = temp.m_isEndImmediately;			//动画是否为立刻结束类型
	m_isSetEnd = temp.m_isSetEnd;							//是否设置序列帧结束
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//动画帧是否在播放完成之后消失

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;
	if(temp.m_isInited != 0)
	{
		try
		{
#if FRAME_DEBUG
			frameTestSum ++;
			printf("Frame new sum:%d\n",frameTestSum);
#endif
			m_pSprite = new _XSprite[m_allKeyFramesSum];
			if(m_pSprite == NULL)
			{
				m_isInited = 0;
				return;
			}
		}catch(...)
		{
			m_isInited = 0;
			return;
		}
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setACopy(temp.m_pSprite[i]);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
			_XObjectManager::GetInstance().decreaseAObject(&(m_pSprite[i]));
#endif
		}
	}
#if WITH_OBJECT_MANAGER	//注册序列帧这个物件,这个m_isACopy的判断是为了防止重复注册，不过注册本身就已经避免了重复注册
	if(m_isACopy == 0) _XObjectManager::GetInstance().addAObject(this,OBJ_FRAMEEX);
#endif
	m_isInited = temp.m_isInited;
	m_isACopy = 1;
}

_XFrameEx::_XFrameEx(const _XFrameEx &temp)
{
	if(&temp == this) return;	//防止资深拷贝
}
