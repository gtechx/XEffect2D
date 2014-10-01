#include "XEffeEngine.h"
#include "XThread.h"

bool _XThread::createAThread(int *,void (* fun)(void *),void *p)			//����һ���߳�
{
	HANDLE h = CreateThread(0,0,(LPTHREAD_START_ROUTINE)fun,p,0,NULL);
	if(h == 0) return false;	//�����߳̽���ʧ��
	_XThreadInfo *pInfo = createMem<_XThreadInfo>();
	if(pInfo == NULL) return false;
	pInfo->fun = fun;
	pInfo->h = h;
	pInfo->state = STATE_START;
	pInfo->p = p;
	m_threadBuff.push_back(pInfo);
	return true;
}
void _XThread::release()
{//�ͷ���Դ
	closeAllThread();
	for(int i = 0;i < m_threadBuff.size();++ i)
	{
		XDELETE(m_threadBuff[i]);
	}
	m_threadBuff.clear();
}