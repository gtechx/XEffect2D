#ifndef _JIA_XTHREAD_
#define _JIA_XTHREAD_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//����һ��ͳһ���߳̽��й�����࣬Ŀ�����������е��߳��ܱ�ͳһ����
//��������δ���
#include "XOSDefine.h"
#include <vector>
#include "XTimer.h"

namespace XE{
//���̵߳�״̬
enum XThreadState
{
	STATE_BEFORE_START,		//�߳���δ��ʼ
	STATE_START,			//�߳��Ѿ�����
	STATE_SET_TO_END,		//�̱߳�����Ϊ����
	STATE_END,				//�߳̽���
};
//�ȴ�ָ�����߳̽���
extern void waitThreadEnd(XThreadState &state);
typedef DWORD pthread_t;    

struct XThreadInfo
{
	XThreadState state;	//�̵߳�״̬
	HANDLE h;				//�̵߳ľ��
	void(*fun)(void *);	//�߳��е��õĺ���
	void *p;				//�̺߳����Ĳ���
	XThreadInfo()
		:state(STATE_BEFORE_START)
		, h(0)
		, fun(NULL)
		, p(NULL)
	{}
};
//���ڶ��̵߳ĵ��û��ƣ�������������⣬��Ҫ����
//���̻߳��ƻ᲻ͣ�Ľ����̣߳���ʱ�����У����ᵼ��������buffԽ��Խ��
class XThreadManager
{
protected:
	XThreadManager()
	{}
	XThreadManager(const XThreadManager&);
	XThreadManager &operator= (const XThreadManager&);
	virtual ~XThreadManager() { release(); }
public:
	static XThreadManager& GetInstance()
	{
		static XThreadManager m_instance;
		return m_instance;
	}
private:
	std::vector<XThreadInfo *> m_threadBuff;	//�̵߳Ķ���
	void release();//�ͷ���Դ
public:
	//id,����̵߳ı�ţ�fun�̵߳��õĺ�����p�̴߳��ݵĲ���
	bool createAThread(int *id, void(*fun)(void *), void *p);			//����һ���߳�
	bool closeAThread(unsigned int id)			//�ر�һ���߳�
	{
		if (id >= m_threadBuff.size() || m_threadBuff[id]->state != STATE_START) return false;
		if (TerminateThread(m_threadBuff[id]->h, 0))
		{//�߳̽���
			m_threadBuff[id]->state = STATE_END;
			return true;
		}
		else
		{//�߳�
			return false;
		}
	}
	//�ر������߳�
	bool closeAllThread() { return true; }
	XThreadState getThreadState();	//��ȡһ���̵߳�״̬
	bool suspendAThread();			//����һ���߳�
	bool resumeAThread();			//�ָ�һ���߳�
	bool suspendAllThread();		//���������߳�
	bool resumeAllThread();			//�ָ������߳�

	void update();	//�����̵߳�״̬
};
class XThread
{
private:
	void(*m_cbFun)(void *);	//�ص�����
	void *m_pClass;				//����ָ��
	XThreadState m_threadState;	//�̵߳�״̬

	static DWORD WINAPI threadFun(LPVOID hParameter);
	float m_fps;
	bool m_isSetup;	//�Ƿ��Ѿ������߳�
public:
	float getFps() const { return m_fps; };	//��ȡ��ǰ�̵߳�ִ���ٶ�
	bool setup(void(*cbFun)(void *), void *pClass);
	void threadEnd()
	{
		waitThreadEnd(m_threadState);
		m_isSetup = false;
	}
	XThread()
		:m_isSetup(false)
		, m_cbFun(NULL)
		, m_pClass(NULL)
		, m_threadState(STATE_BEFORE_START)
		, m_fps(0.0f)
	{}
	~XThread() { threadEnd(); }
};
}
#endif