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
	bool m_isInited;	//�Ƿ�����˳�ʼ��
	WSADATA m_wsaData;
	SOCKET m_localSock;	//������sock
	sockaddr_in m_targetAddr;	//���͵�Ŀ���ַ

	std::deque<XUdpPack> m_freePacks;	//û��ʹ�õ������ݰ���
	std::deque<XUdpPack> m_curPacks;	//��ǰ���յ������ݰ�
	XCritical m_mutex;	//�߳���

	XThreadState m_threadState;
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	void sendAPack();
public:
	bool setup(const std::string&ip,int port);		//��������
	void close();	//�ر�����
	bool sendData(const unsigned char *data,int len);	//��������
	bool sendData(const std::string &str){return sendData((unsigned char *)str.c_str(),str.size());}
	XUdpSender()
		:m_isInited(false)
	{}
	~XUdpSender(){close();}
};
}
#endif