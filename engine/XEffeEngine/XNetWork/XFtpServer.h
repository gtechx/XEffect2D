#ifndef _JIA_XFTPSERVER_
#define _JIA_XFTPSERVER_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
#include "XFtpCommon.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
//��δ���
//����ͻ��˵����������xʱ����û��ͨѶ�����Զ��Ͽ�
enum XFtpTransnationType	//FTP���ݴ����ģʽ
{
	FTP_TRANSNAION_TYPE_ASCII,	//�ַ����Ĵ��䷽ʽ
	FTP_TRANSNAION_TYPE_BIN,	//�����ƵĴ��䷽ʽ
};
struct XFtpClientSockInfo
{
	void * pClass;
	int clientSock;
	int clientDataSock;	//���ݴ���Ķ˿�
	sockaddr_in clientAddr;	
	XFtpTransnationType transnatType;					//��ǰ���õĴ���ģʽ
	char curPath[MAX_FILE_NAME_LENGTH];	//��ǰ���õ����·��

	XFtpClientSockInfo()
		:pClass(NULL)
		,transnatType(FTP_TRANSNAION_TYPE_ASCII)
	{
		curPath[0] = '/';
		curPath[1] = '\0';
	}
};
class XFtpServer
{
private:
	XBool m_isCreated;	//�������Ƿ��Ѿ���������
	int m_serverSock;	//���������׽���
	int m_clientSum;	//��ǰ�ͻ��˵�����
	int m_allClientSum;	//���з��ʵĿͻ��˵�����
	WSADATA m_wsaData;

	XBool sendData(int clientSock,const char * data,int len)	//��ͻ��˷�������
	{
	//	if(!m_isCreated) return XFalse;	//��δ�����������򷵻�ʧ��
		if(data == NULL || len <= 0) return XFalse;
		int ret = send(clientSock,data,len,0);
		if(ret < 0) return XFalse;	//���ݷ���ʧ��
		return XTrue;
	}
	XBool recvData(int clientSock,char * p)	//�ӿͻ��˻�ȡ����
	{
	//	if(!m_isCreated) return XFalse;	//��δ�����������򷵻�ʧ��
		if(p == NULL) return XFalse;
		int ret = recv(clientSock,p,RECV_BUFF_SIZE,0);
		if(ret < 0) return XFalse;	//��������ʧ��
		p[ret] = '\0';
		printf("%s",p);	//��̨��ʾ���յ�������
		return XTrue;
	}
	XFtpUserType clientLoginProc(int clientSock);//����ͻ��˵�½�����������û������ͣ�0:��Ч,1:��ͨ�û�,2:�����û�
	pthread_t m_serverCMDThread;
	pthread_t m_serverAcceptThread;
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI serverCMDThread(void * pParam);		//�����������߳�
	static DWORD WINAPI serverAcceptThread(void * pParam);	//�����������߳�
	static DWORD WINAPI serverRequestThread(void * pParam);	//�����������Ӧ�߳�(��δ���)
#endif
public:
	XBool createServer();	//����������������accept�̣߳�û����һ�����ӣ�����һ���ͻ��˴����߳����ڴ���ͻ�������
	void close()
	{//�ͷ����е���Դ�����Ͽ����е�����(��δ���)
		if(!m_isCreated) return;
		//todo:
		m_isCreated = XFalse;
	}
	XFtpServer()
		:m_isCreated(XFalse)
		,m_clientSum(0)
		,m_allClientSum(0)
	{}
	~XFtpServer(){close();}
};
}
#endif 