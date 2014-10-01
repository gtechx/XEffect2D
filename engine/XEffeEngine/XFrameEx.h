#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//�µ�����֡���࣬�Ѿ���֧��Ԥ��ƫ�ƣ��������Ż���ԭ�������µ���Դ����Ĺ������Ѿ���ͼ��Ŀհײ��ֽ������Ż���
//����û�б�Ҫ��Ԥ�Ƚ����Ż����Ա��ڽ�ʡ�����Ĺ�����
#include "XSprite.h"

//#define MAX_FRAME_SUM 256		//����֡���ֻ��256֡

class _XFrameEx:public _XObjectBasic	
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����������д������֡����Ķ���
private:
	_XSprite *m_pSprite;
	//------------------------------------------------------------------
	_XSCounter *m_cp;		//���ü�����
	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	float angle;			//����ĽǶ�
	_XBool m_isVisible;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisible() 
	{
		m_isVisible = XTrue;
		updateChildVisible();
	}					//��������ɼ�
	void disVisible() 
	{
		m_isVisible = XFalse;
		updateChildVisible();
	}					//����������ɼ�
	_XBool getVisible() const {return m_isVisible;}			//��ȡ����Ƿ�ɼ���״̬ 
private:
	float x, y;				//�����λ��
	float xsize, ysize;		//��������ųߴ�
	float alpha;			//͸����
	float colorRed;			//��ɫ
	float colorGreen;		//��ɫ
	float colorBlue;		//��ɫ
	_XBool m_isOverturn;		//�Ƿ�x�������ҷ�ת

	int m_allFramesSum;				//����֡��������֡��
	int m_allKeyFramesSum;			//����֡�����йؼ�֡������
	float m_nowFramesNumble;		//��ǰ���ŵ��ڼ�֡
	float m_actionSpeed;			//����֡���ŵ��ٶ�
	_XBool m_isLoop;					//����֡�Ƿ����ѭ��
	int m_startFrame;				//����һ֡��ʼ
	_XBool m_isEnd;					//����֡�Ƿ񲥷����
	_XBool m_isEndImmediately;		//�����Ƿ�Ϊ���̽�������
	_XBool m_isSetEnd;				//is end by user			//�Ƿ���������֡����
	_XBool m_isDisappearAtEnd;		//����֡�Ƿ��ڲ������֮����ʧ

	char *m_frameName;				//����֡�ļ���
	int *m_keyFrameArray;			//����֡�Ĺؼ�֡�������
public:
	void setAttribute(const _XVector2& position,			//����֡���ŵ�λ��
		_XBool loop,				//����֡�Ƿ�ѭ��
		_XBool endImmediately,		//����֡�Ƿ���������
		int startFrame,			//����֡����ʼ֡
		float actionSpeed,		//����֡�Ĳ����ٶ�(fps * 0.001)
		_XBool disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		_XBool isOverturn = XFalse);	//�Ƿ�ת

	using _XObjectBasic::setPosition;
	void setPosition(float a,float b);			//���þ��������
	void setPositionX(float a);
	void setPositionY(float b);
	_XVector2 getPosition() const;

	using _XObjectBasic::setSize;
	void setSize(float x,float y);	//���þ�������ű���
	_XVector2 getSize() const;

	_XBool getIsEnd() const;	//�������֡�Ƿ񲥷����
	void setEnd();	//��������֡�������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(int timeDelay);	//�ƽ�����֡����
	int getAllFramesSum() const {return m_allFramesSum;}
	float getNowFramesNumber() const {return m_nowFramesNumble;}

	_XSprite * getNowSprite(char frameFlag = 0);	//���ص�ǰ֡�ľ���ָ�룬��� frameFlag = 1�򷵻ص��������������е�ָ��
private:
	_XBool m_isInited;		//�Ƿ��ʼ��
	char m_blendTypeScr;	//blendģʽ���ο�OpenGL��Blend�Ĳ���
	char m_blendTypeDst;
	_XShaderGLSL *m_pShader;
public:
	void setBlendType(int typeScr,int typeDst)	//���û��ģʽ
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr > 8 ) typeScr = 8;
		if(typeDst > 7 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setBlendType(m_blendTypeScr,m_blendTypeDst);
		}
	}
	//void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
	void setShaderClass(_XShaderGLSL * shader) {m_pShader = shader;}

	_XBool init(const char *filename,			//ͼƬ������
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//��Դ��λ��	
	//����һ�����ö��뷽ʽ�Ľӿ�
	void setIsTransformCenter(_XTransformMode temp)
	{//��δʵ��
		if(!m_isInited) return;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setIsTransformCenter(temp);
		}
	}
private:
	_XBool releaseMem();			//�ͷ��Լ�����Դ�����ڱ�������õ���Դ��Ҫ�������ü�����������
public:
	void release(){}			//����ԭ�еĽӿ����¼��ݣ���ʵ����ɶҲ����
	void draw();			//��ʾ����֡

	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	_XFColor getColor() const 
	{
		if(m_pSprite == NULL) return _XFColor::white;
		else return m_pSprite[0].getColor();
	}
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//��������֡���ŵ��ٶ�
	//������Ϊ���������Ľӿ�
	_XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:
	_XBool m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XFrameEx(const _XFrameEx &temp);	//���ظ�ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���
public:
	_XFrameEx& operator = (const _XFrameEx& temp);	//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)
	void setACopy(const _XFrameEx& temp);		//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	_XFrameEx();
	~_XFrameEx();
	//virtual void justForTest() {;}
};

struct _XFrameExParamData
{//��������Ĳ���
	_XResourcePosition resourcePosition;	//��ȡ��Դ��λ��
	_XVector2 pos;			//��ʼλ��
	_XBool loop;				//�Ƿ�ѭ��
	_XBool endImmediately;		
	int startFrame;
	float actionSpeed;
	_XBool disappearAtEnd;
	_XBool isOverturn;

	char * getStrFromData()
	{
		char *tempStr = createArrayMem<char>(256);
		if(tempStr == NULL) return NULL;
		int offset = 0;
		sprintf(tempStr + offset,"ResPos:%d,\n",resourcePosition);
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"ObjPos:%f|%f,\n",pos.x,pos.y);
		offset = strlen(tempStr);
		if(loop) sprintf(tempStr + offset,"Loop:1,\n");
		else sprintf(tempStr + offset,"Loop:0,\n");
		offset = strlen(tempStr);
		if(endImmediately) sprintf(tempStr + offset,"EndIMD:1,\n");
		else sprintf(tempStr + offset,"EndIMD:0,\n");
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"StartFRM:%d,\n",startFrame);
		offset = strlen(tempStr);
		sprintf(tempStr + offset,"Speed:%f,\n",actionSpeed);
		offset = strlen(tempStr);
		if(disappearAtEnd) sprintf(tempStr + offset,"disappear:1,\n");
		else sprintf(tempStr + offset,"disappear:0,\n");
		offset = strlen(tempStr);
		if(isOverturn) sprintf(tempStr + offset,"Overturn:1,\n");
		else sprintf(tempStr + offset,"Overturn:0,\n");
		offset = strlen(tempStr);
		return tempStr;
	}
	int getDataFromStr(const char * str)
	{
		if(str == NULL) return 0;
		int tempData = 0;
		int offset = 0;
		if(sscanf(str + offset,"ResPos:%d,\n",&resourcePosition) != 1) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"ObjPos:%f|%f,\n",&pos.x,&pos.y) != 2) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"Loop:%d,\n",&tempData) != 1) return offset;
		if(tempData == 0) loop = XFalse;
		else loop = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"EndIMD:%d,\n",&tempData) != 1) return offset;
		if(tempData == 0) endImmediately = XFalse;
		else endImmediately = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"StartFRM:%d,\n",&startFrame) != 1) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"Speed:%f,\n",&actionSpeed) != 1) return offset;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"disappear:%d,\n",&tempData) != 1) return offset;
		if(tempData == 0) disappearAtEnd = XFalse;
		else disappearAtEnd = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		if(sscanf(str + offset,"Overturn:%d,\n",&tempData) != 1) return offset;
		if(tempData == 0) isOverturn = XFalse;
		else isOverturn = XTrue;
		offset += getCharPosition(str + offset,',') + 1;
		return offset;
	}
	_XBool getDataFromFile(FILE *fp)
	{
		if(fp == NULL) return XFalse;
		int tempData = 0;
		if(fscanf(fp,"ResPos:%d,\n",&resourcePosition) != 1) return XFalse;
		if(fscanf(fp,"ObjPos:%f|%f,\n",&pos.x,&pos.y) != 2) return XFalse;
		if(fscanf(fp,"Loop:%d,\n",&tempData) != 1) return XFalse;
		if(tempData == 0) loop = XFalse;
		else loop = XTrue;
		if(fscanf(fp,"EndIMD:%d,\n",&tempData) != 1) return XFalse;
		if(tempData == 0) endImmediately = XFalse;
		else endImmediately = XTrue;
		if(fscanf(fp,"StartFRM:%d,\n",&startFrame) != 1) return XFalse;
		if(fscanf(fp,"Speed:%f,\n",&actionSpeed) != 1) return XFalse;
		if(fscanf(fp,"disappear:%d,\n",&tempData) != 1) return XFalse;
		if(tempData == 0) disappearAtEnd = XFalse;
		else disappearAtEnd = XTrue;
		if(fscanf(fp,"Overturn:%d,\n",&tempData) != 1) return XFalse;
		if(tempData == 0) isOverturn = XFalse;
		else isOverturn = XTrue;
		return XTrue;
	}
	//Ĭ������
	_XFrameExParamData()
		:resourcePosition(RESOURCE_SYSTEM_DEFINE)
		,pos(0.0f,0.0f)
		,loop(XFalse)
		,endImmediately(XFalse)
		,startFrame(0)
		,actionSpeed(0.03f)
		,disappearAtEnd(XFalse)
		,isOverturn(XFalse)
	{}
};
#include "XFrameEx.inl"

#endif