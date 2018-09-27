#ifndef _JIA_XTCPSERVER_
#define _JIA_XTCPSERVER_
#include <vector>
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
//��δ���
//��򵥵�TCP�����������Խ������ӣ����պͷ�������
//֧���Զ��ְ��������Ҫ�����ӵĹ��ܣ���ʹ��XSocketEx����XSocket
namespace XE {
class XTcpServer
{
private:
	static const int m_recvBuffSize = 2048;
	static const int m_packHeadSize = 8;
	bool m_isInited;
	SOCKET m_localSock;	//������sock
	XThreadState m_recvThreadState;		//�����̵߳�״̬
	XThreadState m_sendThreadState;		//�����̵߳�״̬
	XThreadState m_acceptThreadState;	//�����̵߳�״̬

	static DWORD WINAPI recvThread(void * pParam);		//�����߳� 
	static DWORD WINAPI sendThread(void * pParam);		//�����߳� 
	static DWORD WINAPI acceptThread(void * pParam);	//�����߳� 

	struct XClientInfo
	{
		bool isConnectOK;	//���������Ƿ�����
		SOCKET sock;
		int id;
		int rPackID;
		std::vector<unsigned char> recvBuff;
	};
	int m_curClientID;
	std::vector<XClientInfo> m_clients;
	void addAClient(SOCKET sock);
	int getIndexBySock(SOCKET sock)
	{
		for (int i = 0; i < m_clients.size(); ++i)
		{
			if (m_clients[i].sock == sock) return i;
		}
		return -1;
	}
	int getIndexByID(int id)
	{
		for (int i = 0; i < m_clients.size(); ++i)
		{
			if (m_clients[i].id == id) return i;
		}
		return -1;
	}
	//���Ͳ��ֵĶ���
	std::deque<XNetPack> m_sFreePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_sCurPacks;	//��ǰ���յ������ݰ�
	XCritical m_sMutex;	//�߳���
	void sendAPack();
	void _sendData(unsigned char* data, int len, const SOCKET& sock);
	//������Ϊ��clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	int m_sPackID;	//���ݰ��ı��

	//���ղ��ֵĶ���
	std::deque<XNetPack> m_rFreePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_rCurPacks;	//��ǰ���յ������ݰ�
	XCritical m_rMutex;	//�߳���
	void _recvAData(unsigned char* data, int len, int index);
	void recvAData(unsigned char* data, int len, int index);

	bool m_isAutoPack;	//�Ƿ�֧���Զ��ְ�
public:
	bool setup(const std::string& ip, int port, bool isAutoPack = false);
	bool setup(int port, bool isAutoPack = false);
	void close();
	XTcpServer()
		:m_isInited(false)
		, m_localSock(INVALID_SOCKET)
		, m_curClientID(0)
		, m_isAutoPack(false)
		, m_sPackID(-1)
	{}
	~XTcpServer() { close(); }
	bool sendData(const void* data, int len, int cID = 0);
	bool sendData(const std::string& data, int cID = 0)
	{
		return sendData(&data[0], data.size(), cID);
	}
	bool haveRecvData()const { return m_rCurPacks.size() > 0; }
	bool recoverAData(XNetPack &pack);	//����һ�����ݰ�
	void popARecvData(XNetPack &data);	//��ȡһ���������ݰ�
};
}
#endif