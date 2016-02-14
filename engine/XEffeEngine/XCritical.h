#ifndef _JIA_XCRITICAL_
#define _JIA_XCRITICAL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
//������������̻߳���
class XCritical 
{ 
public: 
	XCritical() {InitializeCriticalSection(&m_sec);}
	~XCritical() {DeleteCriticalSection(&m_sec);}
    void Lock() {EnterCriticalSection(&m_sec);}
    void Unlock() {LeaveCriticalSection(&m_sec);}
private: 
    CRITICAL_SECTION m_sec; 
};
//������Ϊ�˺�linux����Ľӿ��Ǻ϶������
typedef XCritical pthread_mutex_t;
inline void pthread_mutex_init(XCritical &,int){;}	//do nothing
inline void pthread_mutex_lock(XCritical &temp) {temp.Lock();}
inline void pthread_mutex_unlock(XCritical &temp) {temp.Unlock();}

#endif
}
#endif