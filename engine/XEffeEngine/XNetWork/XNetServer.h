#ifndef _JIA_XNETSERVER_
#define _JIA_XNETSERVER_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
//ע�⣺��δ���Ƕ��̻߳�������
//ֻ֧��1c1s��ģʽ
#include "XNetCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
class XNetServer
{
private:
	WSADATA m_wsaData;
	//XBool m_clientConnect;
	SOCKET m_netSocket;	//����ͨѶ������ӿ�	
	SOCKET m_serverSocket;

	XCritical m_mutex;	//�߳���

	XConnectState m_connectState; 
	XBool m_isExit;				//�Ƿ������˳�
	XNetData * m_recvPacket;		//��δ��������������
	std::deque<XNetData *> m_recvDataBuff;	//���յ�������
	std::deque<XNetData *> m_sendDataBuff;	//���յ�������
	pthread_t m_recvThread;
	pthread_t m_sendThread;
	pthread_t m_acceptThread;
	pthread_t m_boardcastThread;		//�㲥�߳�
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	static DWORD WINAPI acceptThread(void * pParam);	//�����߳�
	static DWORD WINAPI boardcastThread(void * pParam);	//�㲥�߳�

	int m_recvPacketSize;	//���յ������ݰ��Ĵ�С
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	XBool getDataPacket(unsigned char *buff,int len);	//�Ӳ���������������ȡ�����������ݰ�
	XBool sendDataSocket(char * data,int len);

	XBool m_autoIPMode;	//�Ƿ�֧���Զ���ȡipģʽ
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//���̵������ַ���
public:
	XBool createServer(int port = NET_SERVER_PORT,XBool autoIP = XFalse);	//���ӷ�����
	XBool setProjectStr(const char * str)
	{
		if(str == NULL ||
			strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy(m_projectStr,str);
		return XTrue;
	}
	XBool sendData(XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutex.Lock();
		m_sendDataBuff.push_back(data);
		if(m_sendDataBuff.size() > 32)	//Ϊ�˷�ֹ����ӵ�£�������Զֻ�������µ�32֡����
		{
			data = m_sendDataBuff[0];
			m_sendDataBuff.pop_front();
			m_mutex.Unlock();
			LogStr("XNetServer���ն������ݷ���ӵ��,�������ϵ�����!");
			XMem::XDELETE_ARRAY(data->data);
			XMem::XDELETE(data);
		}else
			m_mutex.Unlock();
		return XTrue;
	}
	void clearSendData(XNetDataType type)	//�ӷ���������ȡ�����еķ�������
	{
		m_mutex.Lock();
		std::deque<XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutex.Unlock();
	}
	XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutex.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//���ݰ�����
			XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutex.Unlock();
			return ret;
		}else
		{
			m_mutex.Unlock();
			return NULL;
		}
	}
	XConnectState getConnectState() const {return m_connectState;}
	void release();
	XNetServer()
		:m_connectState(CONNECT_STATE_DISCONNECT)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~XNetServer()
	{
		release();
	}
};
}
#endif