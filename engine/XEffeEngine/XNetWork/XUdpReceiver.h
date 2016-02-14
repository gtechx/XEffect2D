#ifndef _JIA_XUDPRECEIVER_
#define _JIA_XUDPRECEIVER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//����һ������UDP�Ľ�����
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
struct XUdpPack
{
	int buffSize;		//�ڴ�ռ�Ĵ�С	
	int curDataLen;		//��Ч���ݵĴ�С
	unsigned char *data;
	XUdpPack()
		:data(NULL)
		,buffSize(0)
		,curDataLen(0)
	{}
};
class XUdpReceiver
{
private:
	static const int m_recvBuffSize = 2048;
	std::deque<XUdpPack> m_freePacks;	//û��ʹ�õ������ݰ���
	std::deque<XUdpPack> m_curPacks;	//��ǰ���յ������ݰ�
	bool m_isInited;
	WSADATA m_wsaData;
	SOCKET m_localSock;	//������sock
	XThreadState m_threadState;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	void pushAPack(unsigned char *p,int len);	//ѹ��һ�����ݰ�
	XCritical m_mutex;	//�߳���
public:
	//����֮����һ���̲߳�ͣ�Ľ�������
	bool setup(int port);	//����
	void close();			//�ر�
	bool haveData()const{return m_curPacks.size() > 0;}		//�Ƿ��н��յ�����
	bool popAData(XUdpPack &pack)	//�Ӷ������Ƴ�һ�����ݰ�,����ڴ�ռ�
	{
		if(m_curPacks.size() <= 0) return false;
		m_mutex.Lock();
		pack = m_curPacks[0];
		m_curPacks.pop_front();
		m_mutex.Unlock();
		return true;
	}
	bool recoverAData(XUdpPack &pack);	//����һ�����ݰ�
	//�������ݰ��Ĺ������£�
	//����haveData������Ƿ�������ݰ�
	//popAData�Ӷ�����ȡ��һ�����ݰ���Ȼ����д���
	//�������֮�����recoverAData�������ݰ����գ��Ա����ظ�ʹ��

	XUdpReceiver()
		:m_isInited(false)
	{}
	~XUdpReceiver(){close();}
};
}
#endif