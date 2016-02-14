#ifndef _JIA_XLOADING_
#define _JIA_XLOADING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XOSDefine.h"
#include "XCommonDefine.h"
#include "XThread.h"
#ifdef XEE_OS_LINUX
#include <pthread.h>
#include <unistd.h>           //usleep()
#include "glx.h"
#endif
namespace XE{
#ifdef XEE_OS_WINDOWS
class XLoadingScene
{
public:
	XLoadingScene(){}
	virtual ~XLoadingScene(){}
	virtual int init(XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE) = 0;
	virtual void move(float stepTime) = 0;
	virtual void draw() = 0;
	virtual void release() = 0;
	virtual void setLoadPresent(float loadPresent) = 0;	//�����������
};
class XLoading
{
private:
	XResourcePosition m_resoursePosition;	//��Դλ��
public:
	float m_loadPresent;		//�������
	XBool m_isShow;			//�Ƿ���ʾ��������
	XThreadState m_isEnd;			//0û�н��� 1��Դ������� 2����

	//XVector2 m_windowSize;	//���ڵĳߴ�

	HDC m_hDC;				//��ͼ���
	XLoadingScene *m_pScene;
	float m_speed;

public:
	static DWORD WINAPI loadingProc(void * pParam);	//������߳�
	void init(XLoadingScene * scene,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void move();				//����������
	void draw();				//��������
	void setStart();			//���ÿ�ʼ������
	void setEnd();				//���ý���������
	void release();				//�ͷ���Դ
	void setLoadPresent(float present);		//���ü��ؽ���
	void setSpeed(float speed) {m_speed = speed;}

	XLoading()
		:m_pScene(NULL)
		,m_isEnd(STATE_BEFORE_START)
		,m_speed(1.0f)
	{}
};
inline void XLoading::setLoadPresent(float present)		//���ü��ؽ���
{
	m_loadPresent = present;
	if(m_pScene != NULL) m_pScene->setLoadPresent(m_loadPresent);
}
inline void XLoading::move()
{
	if(m_pScene != NULL) m_pScene->move(1);
}
inline void XLoading::draw()
{
	if(m_pScene != NULL) m_pScene->draw();
}
inline void XLoading::release()
{
	if(m_pScene != NULL) m_pScene->release();
}
#endif

#ifdef XEE_OS_LINUX
class XLoading
{
private:
	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
public:
	float m_loadPresent;		//�������
	char m_isShow;			//�Ƿ���ʾ��������
	char m_isEnd;			//0û�н��� 1��Դ������� 2����

	//������Ҫ�����ڶ��߳������л�
	Display *m_mainDisplay;		
	GLXPbuffer m_mainPBuffer;	//���߳������������Ϣ
	GLXContext m_mainContext;
	GLXPbuffer m_threadPBuffer;	//�����̵߳������Ϣ
	GLXContext m_threadContext;

	int (*m_loadFunc)(void);
	void setLoadFunc(int (*loadFunc)(void))
	{
		m_loadFunc = loadFunc;
	}

	XSprite m_loadingBG;		//���뻭�汳��
	XProgress m_process;
	XVector2 m_windowSize;

	XNumber Number;
	XProgressSkin ProgressTexture;

public:
    static void *loadingProc(void * pParam);	//������߳�

	void init(const XVector2 &windowSize,int resoursePosition = 0);
	void move();				//����������
	void draw();				//��������
	void setStart();			//���ÿ�ʼ������
	void release();				//�ͷ���Դ
	void setLoadPresent(float present);		//���ü��ؽ���

	XLoading()
	:m_loadFunc(NULL)
	{}
};
inline void XLoading::setLoadPresent(float present)		//���ü��ؽ���
{
	m_loadPresent = present;
	m_process.setValue(m_loadPresent);
}
inline void XLoading::move()
{
	if(m_loadPresent < 100.0)
	{
		m_loadPresent += 0.1;
		m_process.setValue(m_loadPresent);
	}
}
inline void XLoading::draw()
{
	m_loadingBG.draw();
	m_process.draw();
}
inline void XLoading::release()
{
	m_loadingBG.release();
	Number.release();
	ProgressTexture.release();
}
#endif

extern XBool isLoadingStart;
extern XLoading *xLoading;
extern XLoadingScene *xLoadingScene;
extern XBool startLoading(XLoadingScene * scene,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
extern XBool endLoading();
}
#endif
