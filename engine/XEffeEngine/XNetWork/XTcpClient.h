#ifndef _JIA_XTCPCLIENT_
#define _JIA_XTCPCLIENT_
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"

namespace XE {
class XTcpClient
{
private:
	static const int m_recvBuffSize = 2048;
	static const int m_packHeadSize = 8;
	bool m_isInited;	//�Ƿ�����˳�ʼ��
	WSADATA m_wsaData;
	SOCKET m_localSock;	//������sock
	sockaddr_in m_targetAddr;	//���͵�Ŀ���ַ
	//���Ͳ��ֵĶ���
	std::deque<XNetPack> m_freePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_curPacks;	//��ǰ���յ������ݰ�
	XCritical m_mutex;	//�߳���
	XThreadState m_sendThreadState;
	static DWORD WINAPI sendThread(void* pParam);		//�����߳�
	void sendAPack();
	void _sendData(unsigned char* data, int len);
	//������Ϊ��clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	int m_sPackID;	//���ݰ��ı��
	//���ղ��ֵĶ���
	std::deque<XNetPack> m_rfreePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_rcurPacks;	//��ǰ���յ������ݰ�
	XCritical m_rmutex;	//�߳���
	XThreadState m_recvThreadState;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	void _recvAData(unsigned char* data, int len);
	void recvAData(unsigned char* data, int len);
	int m_rPackID;
	std::vector<unsigned char> m_recvBuff;

	bool m_isAutoPack;
public:
	bool setup(const std::string& ip, int port, bool isAutoPack = false);
	void close();
	bool sendData(const void* data, int len);
	bool sendData(const std::string& data)
	{
		return sendData(&data[0], data.size());
	}
	bool haveRecvData()const { return m_rcurPacks.size() > 0; }
	bool recoverAData(XNetPack &pack);	//����һ�����ݰ�
	void popARecvData(XNetPack &data);	//��ȡһ���������ݰ�
	XTcpClient()
		:m_isInited(false)
		, m_isAutoPack(false)
		, m_sPackID(-1)
		, m_rPackID(-1)
	{}
	~XTcpClient() { close(); }
};
}
#endif