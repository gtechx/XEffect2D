#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//�µ�����֡���࣬�Ѿ���֧��Ԥ��ƫ�ƣ��������Ż���ԭ�������µ���Դ����Ĺ������Ѿ���ͼ��Ŀհײ��ֽ������Ż���
//����û�б�Ҫ��Ԥ�Ƚ����Ż����Ա��ڽ�ʡ�����Ĺ�����
#include "XCommonDefine.h"
//#include "XObjectBasic.h"
//#include "XShaderGLSL.h"
#include "XSprite.h"
#include "XThread.h"
namespace XE{
class XFrameEx :public XObjectBasic
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����������д������֡����Ķ���
private:
	XSprite* m_pSprite;
	//------------------------------------------------------------------
	XSCounter* m_cp;		//���ü�����
	XResPos m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	float m_angle;			//����ĽǶ�
	XBool m_isVisible;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisible();					//��������ɼ�
	void disVisible();					//����������ɼ�
	XBool getVisible() const { return m_isVisible; }			//��ȡ����Ƿ�ɼ���״̬ 
private:
	//float x, y;				//�����λ��
	XVec2 m_position;
	//float xsize, ysize;		//��������ųߴ�
	XVec2 m_scale;
	XFColor m_color;
	//float alpha;			//͸����
	//float colorRed;			//��ɫ
	//float colorGreen;		//��ɫ
	//float colorBlue;		//��ɫ
	XBool m_isOverturn;		//�Ƿ�x�������ҷ�ת

	int m_allFramesSum;				//����֡��������֡��
	int m_allKeyFramesSum;			//����֡�����йؼ�֡������
	float m_curFramesNumble;		//��ǰ���ŵ��ڼ�֡
	float m_actionSpeed;			//����֡���ŵ��ٶ�
	XBool m_isLoop;					//����֡�Ƿ����ѭ��
	int m_startFrame;				//����һ֡��ʼ
	XBool m_isEnd;					//����֡�Ƿ񲥷����
	XBool m_isEndImmediately;		//�����Ƿ�Ϊ���̽�������
	XBool m_isSetEnd;				//is end by user			//�Ƿ���������֡����
	XBool m_isDisappearAtEnd;		//����֡�Ƿ��ڲ������֮����ʧ
	bool m_isPause;	//�Ƿ�����ͣ״̬
	bool m_isStopAtEnd;	//����֮���Ƿ�ͣ�������һ֡
	int m_pauseAtIndex;	//���Ų�ͣ����ָ��֡

	char *m_frameName;				//����֡�ļ���
	int *m_keyFrameArray;			//����֡�Ĺؼ�֡�������,-1Ϊ�հ�֡ʲôҲ����
	bool m_withFileList;
	std::vector<std::string> m_filesList;

	bool loadData(const char *filename,			//ͼƬ������
		XResPos resPos,
		bool isInThread = false);//������Դ
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool m_isThreadLoading;		//�Ƿ����ڶ��߳�����
	XThreadState m_loadThread;
	std::string m_threadFilename;
	XResPos m_threadResPos;
	bool m_neadUpdateAttribute;	//���߳�����ʱ�Ƿ���Ҫ��������
public:
	//ʹ�ö��̵߳ķ�ʽ�Ӻ�̨������Դ
	bool initThread(const char *filename,			//ͼƬ������
		XResPos resPos = RES_SYS_DEF);
	static DWORD WINAPI loadThread(void *);
	//------------------------------------------------------------
public:
	void setPauseAtIndex(int index) { m_pauseAtIndex = index; }	//���ò��Ų�ͣ����ָ��֡
	void pause() { m_isPause = true; }
	void resume() { m_isPause = false; }
	bool getIsPause()const { return m_isPause; }
	void setIsStopAtEnd(bool flag) { m_isStopAtEnd = flag; }
	bool getIsStopAtEnd()const { return m_isStopAtEnd; }

	void setAttribute(const XVec2& position,			//����֡���ŵ�λ��
		XBool loop,				//����֡�Ƿ�ѭ��
		XBool endImmediately,		//����֡�Ƿ���������
		int startFrame,			//����֡����ʼ֡
		float actionSpeed,		//����֡�Ĳ����ٶ�(fps * 0.001)
		XBool disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		XBool isOverturn = XFalse);	//�Ƿ�ת

	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);			//���þ��������
	void setPositionX(float a);
	void setPositionY(float b);
	const XVec2& getPosition() const { return m_position; }

	using XObjectBasic::setScale;
	void setScale(const XVec2& s);	//���þ�������ű���
	const XVec2& getScale() const { return m_scale; }

	XBool getIsEnd() const { return m_isEnd; }	//�������֡�Ƿ񲥷����
	void setEnd();	//��������֡�������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(float timeDelay);	//�ƽ�����֡����
	int getAllFramesSum() const { return m_allFramesSum; }
	float getCurFramesNumber() const { return m_curFramesNumble; }

	XSprite * getCurSprite(bool isAllFrames = false);	//���ص�ǰ֡�ľ���ָ�룬isAllFrames���Ƿ񷵻������������е�ָ��
private:
	XBool m_isInited;		//�Ƿ��ʼ��
//	char m_blendTypeScr;	//blendģʽ���ο�OpenGL��Blend�Ĳ���
//	char m_blendTypeDst;
	XShaderGLSL *m_pShader;
public:
	//void setBlendType(int typeScr, int typeDst);	//���û��ģʽ
	void setBlendModel(XGL::XBlendModel model);
	//void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
	void setShaderClass(XShaderGLSL * shader) { m_pShader = shader; }

	XBool init(const char *filename,			//ͼƬ������
		XResPos resPos = RES_SYS_DEF);		//��Դ��λ��	
	//����һ�����ö��뷽ʽ�Ľӿ�
	void setIsTransformCenter(XTransformMode temp);
	void setTurnOverMode(XTurnOverMode mode);
private:
	XBool releaseMem();			//�ͷ��Լ�����Դ�����ڱ�������õ���Դ��Ҫ�������ü�����������
public:
	void release() {}			//����ԭ�еĽӿ����¼��ݣ���ʵ����ɶҲ����
	void draw();			//��ʾ����֡

	using XObjectBasic::setColor;
	void setColor(const XFColor&c);		//С��0����ı���һԪ��
	const XFColor& getColor() const;
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//��������֡���ŵ��ٶ�
	float getActionSpeed()const { return m_actionSpeed; }
	//������Ϊ���������Ľӿ�
	XBool isInRect(const XVec2& p);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:
	XBool m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	XFrameEx(const XFrameEx& temp);	//���ظ�ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���
public:
	XFrameEx& operator = (const XFrameEx& temp);	//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)
	void setACopy(const XFrameEx& temp);		//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	XFrameEx();
	~XFrameEx();
	//virtual void justForTest() {;}
	//Ϊ�˱�����XFrame�ļ����ԣ�������벿�ֽӿ�
	XVec2 getInitHalfSize()
	{
		if (m_pSprite == nullptr) return XVec2::zero;
		return XVec2(m_pSprite[0].getW(), m_pSprite[0].getH()) * 0.5f;
	}
};
struct XFrameExParamData
{//��������Ĳ���
	XResPos resourcePosition;	//��ȡ��Դ��λ��
	XVec2 pos;			//��ʼλ��
	XBool loop;				//�Ƿ�ѭ��
	XBool endImmediately;
	int startFrame;
	float actionSpeed;
	XBool disappearAtEnd;
	XBool isOverturn;

	char * getStrFromData();
	int getDataFromStr(const char * str);
	XBool getDataFromFile(FILE *fp);
	//Ĭ������
	XFrameExParamData()
		:resourcePosition(RES_SYS_DEF)
		, pos(0.0f)
		, loop(XFalse)
		, endImmediately(XFalse)
		, startFrame(0)
		, actionSpeed(0.03f)
		, disappearAtEnd(XFalse)
		, isOverturn(XFalse)
	{}
};
#if WITH_INLINE_FILE
#include "XFrameEx.inl"
#endif
}
#endif