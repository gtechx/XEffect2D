#ifndef _JIA_XLOADING_
#define _JIA_XLOADING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XEffeEngine.h"
#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
namespace XE{
class XLoading
{
private:
	XResPos m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
public:
	float m_loadPresent;		//�������
	char m_isShow;			//�Ƿ���ʾ��������
	char m_isEnd;			//0û�н��� 1��Դ������� 2����

//	HDC m_hDC;				//��ͼ���
	XSprite m_loadingBG;		//���뻭�汳��
	XSprite m_gameTitle;
	XSprite m_loadingText;
	XSprite m_loadingTextLight;
	XMoveData m_gameTitleMoveData;
	XProgress m_process;
	XVec2 m_windowSize;

public:
	static DWORD WINAPI loadingProc(void * pParam);	//������߳�
	void init(XVec2 windowSize,XResPos resoursePosition = RES_LOCAL_FOLDER);
	void move();				//����������
	void draw();				//��������
	void setStart();			//���ÿ�ʼ������
	void setEnd();				//���ý���������
	void release();				//�ͷ���Դ
	void setLoadPresent(float present);		//���ü��ؽ���
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
		m_loadPresent += 0.1f;
		m_process.setValue(m_loadPresent);

		m_gameTitleMoveData.move(10);
		if(m_gameTitleMoveData.getIsEnd())
		{
			m_gameTitleMoveData.set(120.0f,350.0f,0.05f,MD_MODE_LINE);
		}
		m_loadingTextLight.setPosition(680.0f + 128.0f - 32.0f,350.0f - 256.0f + 32.0f + m_gameTitleMoveData.getCurData());
	}
}

inline void XLoading::draw()
{
	m_loadingBG.draw();
	//m_process.draw();
	m_gameTitle.draw();
	m_loadingText.draw();
	m_loadingTextLight.draw();
}

inline void XLoading::release()
{
	m_loadingBG.release();
	m_gameTitle.release();
	m_loadingText.release();
	m_loadingTextLight.release();
	m_process.release();
}
}
#endif

#ifdef XEE_OS_LINUX
#include <pthread.h>
#include <unistd.h>           //usleep()
#include "glx.h"
namespace XE{
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
	XVec2 m_windowSize;

	XNumber Number;
	_XProgressTexture ProgressTexture;

public:
    static void *loadingProc(void * pParam);	//������߳�

	void init(XVec2 windowSize,int resoursePosition = 0);
	void move();				//����������
	void draw();				//��������
	void setStart();			//���ÿ�ʼ������
	void release();				//�ͷ���Դ
	void setLoadPresent(float present);		//���ü��ؽ���

	XLoading()
	:m_loadFunc(NULL)
	{
	}
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
}
#endif

#endif
