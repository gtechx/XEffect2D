#ifndef _JIA_XFTPCLIENT_
#define _JIA_XFTPCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
//����һ����װ������FTPͨѶ�Ŀͻ��˵���
//#include "XStdHead.h"
#include "XFtpCommon.h"
namespace XE{
class XFtpClient
{
private:
	XBool m_isConnect;		//�Ƿ��Ѿ����ӵ�������
	WSADATA m_wsaData;	
	SOCKET m_cmdsocket;		//���ӷ��������׽���
	int m_retCode;			//���һ������ص�����
	char m_commandBuff[RECV_BUFF_SIZE + 1];	//������Ļ���
	bool m_withPortMode;	//ͨѶģʽ��PORTģʽ��Pasvģʽ��Ĭ��ʹ��pasvģʽ�����������ӷ������ӿ�

	XBool recvRetCode();
	XBool getRetCode(const char *buf,int len)
	{
		if(len > 4 && buf[0] >= '0' && buf[0] <= '9'
			&& buf[1] >= '0' && buf[1] <= '9'
			&& buf[2] >= '0' && buf[2] <= '9'
			&& buf[3] == ' ')
		{
			m_retCode = (buf[0] - '0') * 100 + (buf[1] - '0') * 10 + (buf[2] - '0');
			return XTrue;
		}
		return XFalse;
	}
	XBool sendCommand(const char * cmd);		//���������������
public:
	XBool connectServer(const char * serverIP,int port = FTP_SOCKET_PORT);	//���������������
	XBool sendClose()
	{
		if(!m_isConnect) return XFalse;
		if(!sendCommand("QUIT\r\n")) return XFalse;
		closesocket(m_cmdsocket);
		m_isConnect = XFalse;
		if(m_retCode != 221) return XFalse;
		return XTrue;
	}
	//�����Ƕ�ftp��װ��һЩ����
	XBool sendUserName(const char * userName)
	{
		if(userName == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy_s(m_commandBuff,RECV_BUFF_SIZE + 1,"USER ");
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,userName);
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 331) return XFalse;	//��Ҫ����
		return XTrue;
	}
	XBool sendPassword(const char * password)
	{
		if(password == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy_s(m_commandBuff,RECV_BUFF_SIZE + 1,"PASS ");
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,password);
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 230) return XFalse;	//��¼�ɹ�
		return XTrue;
	}
	XBool setPort(SOCKET &listenSock);	//����ͨѶ�˿�
	XBool setPasv(SOCKET &listenSock);
	XBool sendFileList();	//��ȡ�ļ��б�
	XBool sendDeleteFile(const char *filename)	//����ɾ���ļ�������
	{
		if(!m_isConnect) return XFalse;
		if(filename == NULL) return XFalse;
		strcpy_s(m_commandBuff,RECV_BUFF_SIZE + 1,"DELE ");
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,filename);
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,"\r\n");	
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 250) return XFalse;	//delete�ɹ�
		return XTrue;
	}
	XBool sendDownloadFile(const char *filename);	//����ָ���ļ�
	XBool sendUpdateFile(const char *filename);	//�ϴ��ļ�
	XBool sendUpdateFile(const char *inFilename,const char *outFilename);	//�ϴ��ļ�
	XBool sendChangeDirectory(const char *dir)	//��һ��Ŀ¼
	{
		if(!m_isConnect) return XFalse;
		if(dir == NULL) return XFalse;
		strcpy_s(m_commandBuff,RECV_BUFF_SIZE + 1,"CWD ");
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,dir);
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,"\r\n");	
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 250) return XFalse;	//CWD�ɹ�
		if(!sendCommand("PWD\r\n")) return XFalse;
		if(m_retCode != 257) return XFalse;	//CWD�ɹ�
		return XTrue;
	}
	int sendGetFileSize(const char *filename)
	{
		if(!m_isConnect) return -1;
		if(filename == NULL) return -1;
		strcpy_s(m_commandBuff,RECV_BUFF_SIZE + 1,"SIZE ");
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,filename);
		strcat_s(m_commandBuff,RECV_BUFF_SIZE + 1,"\r\n");	
		if(!sendCommand(m_commandBuff)) return -1;
		if(m_retCode != 213) return -1;
		int size = -1;
		if(sscanf(m_commandBuff,"213 %d",&size) != 1) return -1;
		return size;
	}

	XFtpClient()
		:m_isConnect(XFalse)
		,m_withPortMode(false)
	{}
	void setWithPortMode(bool flag){m_withPortMode = flag;}
	bool getWithPortMode()const{return m_withPortMode;}
};
}
#endif