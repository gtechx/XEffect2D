#ifndef _JIA_XCRITICAL_
#define _JIA_XCRITICAL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
//#define SIMPLE_MUTEX	//�򵥵��߳�����ʵ�֣�����Ϊ��������ʵ��
#ifndef SIMPLE_MUTEX
enum XMutexType
{
	MUTEX_TYPE_TIMED_NP,	//PTHREAD_MUTEX_TIMED_NP	��ͨ��
	MUTEX_TYPE_RECURSIVE_NP,	//PTHREAD_MUTEX_RECURSIVE_NP	Ƕ����
	MUTEX_TYPE_ERRORCHECK_NP,	//PTHREAD_MUTEX_ERRORCHECK_NP	�����
	MUTEX_TYPE_ADAPTIVE_NP,	//PTHREAD_MUTEX_ADAPTIVE_NP			��Ӧ��
};
#endif
namespace XE{
#ifdef XEE_OS_WINDOWS
//������������̻߳���
class XCritical
{
#ifdef SIMPLE_MUTEX
public:
	XCritical() { InitializeCriticalSection(&m_sec); }
	~XCritical() { DeleteCriticalSection(&m_sec); }
	void Lock() { EnterCriticalSection(&m_sec); }
	void Unlock() { LeaveCriticalSection(&m_sec); }
private:
	CRITICAL_SECTION m_sec;
#else
public:
	XCritical() { m_handle = CreateMutex(NULL, false, NULL); }
	~XCritical()
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}
	void Lock() { WaitForSingleObject(m_handle, INFINITE); }
	void Unlock() { ReleaseMutex(m_handle); }
private:
	HANDLE m_handle;
#endif
};
inline void gLock(XCritical *c)
{
	if (c == nullptr) return;
	c->Lock();
}
inline void gUnlock(XCritical *c)
{
	if (c == nullptr) return;
	c->Unlock();
}
//������Ϊ�˺�linux����Ľӿ��Ǻ϶������
typedef XCritical pthread_mutex_t;
inline void pthread_mutex_init(XCritical &,int){;}	//do nothing
inline void pthread_mutex_lock(XCritical &temp) {temp.Lock();}
inline void pthread_mutex_unlock(XCritical &temp) {temp.Unlock();}
inline int pthread_mutex_destroy(XCritical &temp) {;}  
#endif
}
#endif