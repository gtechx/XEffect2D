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
class XUdpReceiver
{
private:
	static const int m_recvBuffSize = 4096;
	static const int m_packHeadSize = 12;
	std::deque<XNetPack> m_freePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_curPacks;	//��ǰ���յ������ݰ�
	bool m_isInited;
	SOCKET m_localSock;	//������sock
	XThreadState m_threadState;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	void pushAPack(unsigned char *p,int len);	//ѹ��һ�����ݰ�
	void _pushAPack(unsigned char *p,int len);	//ѹ��һ�����ݰ�
	XCritical m_mutex;	//�߳���
	struct XClientInfo
	{
		int id;
		int packID;
		std::vector<unsigned char> buff;
	};
	std::vector<XClientInfo> m_clientsInfo;
	int getIndexByID(int id)
	{
		for (int i = 0;i < m_clientsInfo.size();++i)
		{
			if (m_clientsInfo[i].id == id) return i;
		}
		return -1;
	}
	bool m_autoPack;	//�Ƿ�����Զ��ְ���������������������ͨѶ�Ļ����ǲ����Զ��ְ��ģ������������ݲ�����
public:
	//����֮����һ���̲߳�ͣ�Ľ�������
	bool getIsAutoPack()const { return m_autoPack; }
	//�������ͨѶʱautoPack����Ϊfalse;
	//��ʱ���ݰ���С���ܴ���m_recvBuffSize���������ɽ���ʧ��
	//autoPackΪtrueʱ��Դ����ݽ����Զ��ֽ�ΪС�ߴ�����ݶ�η��ͣ��Ա��ڽ��ն��ܽ��յ����������ݰ�
	bool setup(int port, bool autoPack = false);	//����
	void close();			//�ر�
	bool haveData()const{return m_curPacks.size() > 0;}		//�Ƿ��н��յ�����
	bool popAData(XNetPack &pack)	//�Ӷ������Ƴ�һ�����ݰ�,����ڴ�ռ���Ҫ����
	{
		if(m_curPacks.size() <= 0) return false;
		m_mutex.Lock();
		pack = m_curPacks[0];
		m_curPacks.pop_front();
		m_mutex.Unlock();
		return true;
	}
	bool recoverAData(XNetPack &pack);	//����һ�����ݰ�
	//�������ݰ��Ĺ������£�
	//����haveData������Ƿ�������ݰ�
	//popAData�Ӷ�����ȡ��һ�����ݰ���Ȼ����д���
	//�������֮�����recoverAData�������ݰ����գ��Ա����ظ�ʹ��

	XUdpReceiver()
		:m_isInited(false)
		, m_autoPack(false)
		, m_localSock(INVALID_SOCKET)
	{}
	~XUdpReceiver(){close();}
};
}
#endif