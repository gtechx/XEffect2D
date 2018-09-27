#ifndef _JIA_XSHAREMEM_
#define _JIA_XSHAREMEM_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XOSDefine.h"

#ifdef XEE_OS_LINUX	//����linux�µĹ����ڴ���ƽ�Ϊ���ӣ�������δ�������ԣ�linux�²���ʹ��
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
namespace XE{
class XShareMem
{
private:
	bool m_isInited;
	unsigned char *m_pAddress;
#ifdef XEE_OS_WINDOWS
	HANDLE m_hMapFile;
	HANDLE m_hMutexRW;	//���ڶ���д�Ļ���
#endif
#ifdef XEE_OS_LINUX
	sem_t *m_prsem;
	int m_shmid;
#endif
	int m_size;
public:
	bool init(const char *name,int size);
	unsigned char *getBuff() {return m_pAddress;}
	int getSize() const {return m_size;}
	void updateData(void *pData)
	{
		if(pData == NULL) return;
		lock();
		memcpy(m_pAddress,pData,m_size);
		unlock();
	}
#ifdef XEE_OS_WINDOWS
	void lock(){WaitForSingleObject(m_hMutexRW, INFINITE);}
	void unlock(){ReleaseMutex(m_hMutexRW);}
#endif

	void release();
	XShareMem()
		:m_isInited(false)
		,m_pAddress(NULL)
		,m_hMutexRW(NULL)
#ifdef XEE_OS_LINUX
		,m_prsem(NULL)
#endif
	{}
	~XShareMem(){release();}
};
}
#endif