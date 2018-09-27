#ifndef _JIA_XNETCLIENT_
#define _JIA_XNETCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
//ע�⣺��δ���Ƕ��̻߳�������
class XNetClient
{
private:
	//WSADATA m_wsaData;
	SOCKET m_netSocket;	//����ͨѶ������ӿ�	

	XCritical m_mutexThread;
	XCritical m_mutexSend;	//�����߳���
	XCritical m_mutexRecv;	//�����߳���
	XCritical m_mutexBoardCast;	//�߳���

	XConnectState m_connectState; 
	XNetData * m_recvPacket;	//��δ��������������
	std::deque<XNetData *> m_recvDataBuff;	//���յ�������
	std::deque<XNetData *> m_sendDataBuff;	//���յ�������
	pthread_t m_recvThread;
	pthread_t m_sendThread;
	pthread_t m_boardcastThread;
	static DWORD WINAPI recvThread(void * pParam);	//�����߳�
	static DWORD WINAPI sendThread(void * pParam);	//�����߳�
	static DWORD WINAPI boardcastThread(void * pParam);	//�����߳�
	//++++++++++++++++++++++++++++++++++++++++++++
	//20130903���޸�
private:
	pthread_t m_connectThread;
	static DWORD WINAPI connectThread(void * pParam);	//�����߳�
	XBool m_isClientCreated;
	int m_threadSum;			//�̵߳�����
	XBool m_useBroadcast;
	std::string m_serverIP;
public:
	XBool createClient(XBool useBroadcast = XTrue,const char *ip = NULL,int port = 0);			//ͨ���㲥����ʽ���ӷ�����
	void (*m_connectBrokenFun)();	//����Ͽ�ʱ����õĻص�����
	void (*m_connectFun)();			//�������ӽ�����ʱ����õĺ���
	//--------------------------------------------
private:
	int m_recvPacketSize;	//���յ������ݰ��Ĵ�С
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	XBool getDataPacket(unsigned char *buff,int len);
	XBool sendDataSocket(char * data,int len);

	//������Ϊ�˹㲥�Ŷ���Ľӿ�
	sockaddr_in m_serverAddr;
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//���̵������ַ���
	XBool m_boardcastThreadState;	//�㲥�̵߳�״̬
	//XBool recvBoardcast();	//���ջ�Ӧ
	XBool sendBoardcast();	//�㲥����
	XBool m_isExit;
public:
	XBool connectToServer(const char *ip,int port = NET_SERVER_PORT);	//���ӷ�����,ͨ��������IP��port���ӷ�����
	XBool connectToServer();			//�Լ������㲥���ӷ�����
	XBool getIsConnect() const{return m_connectState == CONNECT_STATE_CONNECT;}
	XBool setProjectStr(const char * str)
	{
		if(str == NULL ||
			strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy_s(m_projectStr,PROJECT_STRING_LEN,str);
		return XTrue;
	}
	XBool sendData(XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutexSend.Lock();
		m_sendDataBuff.push_back(data);
		if(m_sendDataBuff.size() > MAX_SEND_DATA_BUFF)	//Ϊ�˷�ֹ����ӵ�£�������Զֻ�������µ�32֡����
		{
			data = m_sendDataBuff[0];
			m_sendDataBuff.pop_front();
			m_mutexSend.Unlock();
			LogStr("XNetServer���ն������ݷ���ӵ��,�������ϵ�����!");
			XMem::XDELETE_ARRAY(data->data);
			XMem::XDELETE(data);
		}else
			m_mutexSend.Unlock();

		return XTrue;
	}
	void clearSendData(XNetDataType type)	//�ӷ���������ȡ�����еķ�������
	{
		m_mutexSend.Lock();
		std::deque<XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutexSend.Unlock();
	}
	XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutexRecv.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//���ݰ�����
			XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutexRecv.Unlock();
			return ret;
		}else
		{
			m_mutexRecv.Unlock();
			return NULL;
		}
	}
	XConnectState getConnectState() const {return m_connectState;}
	void release();
	XNetClient()
		:m_connectState(CONNECT_STATE_DISCONNECT)
		//,m_netSocket(-1)
		,m_netSocket(0)
		,m_boardcastThreadState(XFalse)
		,m_isExit(XFalse)
		,m_connectBrokenFun(NULL)
		,m_connectFun(NULL)
		,m_isClientCreated(XFalse)
		,m_threadSum(0)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~XNetClient(){release();}
};
}
#endif