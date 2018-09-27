#ifndef _JIA_XUDPSENDER_
#define _JIA_XUDPSENDER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include <string>
#include "XUdpReceiver.h"
namespace XE{
class XUdpSender
{
private:
	static const int m_recvBuffSize = 4096;
	static const int m_packHeadSize = 12;
	unsigned int m_id;	//�ͻ���ID������������ʱ��ʶ��
	bool m_isInited;	//�Ƿ�����˳�ʼ��
	SOCKET m_localSock;	//������sock
	sockaddr_in m_targetAddr;	//���͵�Ŀ���ַ

	std::deque<XNetPack> m_freePacks;	//û��ʹ�õ������ݰ���
	std::deque<XNetPack> m_curPacks;	//��ǰ���յ������ݰ�
	XCritical m_mutex;	//�߳���

	XThreadState m_threadState;
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	void sendAPack();
	void _sendPack(unsigned char* data, int len);
	//������Ϊ��clientID,packID,packSize,data
	std::vector<unsigned char> m_sendBuff;
	unsigned int m_packID;	//���ݰ��ı��
	bool m_autoPack;	//�Ƿ�����Զ��ְ���������������������ͨѶ�Ļ����ǲ����Զ��ְ��ģ������������ݲ�����
public:
	bool getIsAutoPack()const { return m_autoPack; }
	//�������ͨѶʱautoPack����Ϊfalse;
	//autoPackΪtrueʱ��Դ����ݽ����Զ��ֽ�ΪС�ߴ�����ݶ�η��ͣ��Ա��ڽ��ն��ܽ��յ����������ݰ�
	bool setup(const std::string&ip,int port,int myPort = 0,bool autoPack = false,int id = -1);		//��������
	void close();	//�ر�����
	bool sendData(const void *data,int len);	//��������
	bool sendData(const std::string& str){return sendData((unsigned char *)str.c_str(),str.size());}
	XUdpSender()
		:m_isInited(false)
		, m_id(0)
		, m_packID(0)
		, m_autoPack(false)
		, m_localSock(INVALID_SOCKET)
	{}
	~XUdpSender(){close();}
	bool getIsInited()const { return m_isInited; }
};
}
#endif