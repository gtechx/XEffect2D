#ifndef _JIA_XFTPCLIENT_
#define _JIA_XFTPCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
//����һ����װ������FTPͨѶ�Ŀͻ��˵���

#include "XFtpCommon.h"

class _XFtpClient
{
private:
	_XBool m_isConnect;		//�Ƿ��Ѿ����ӵ�������
	WSADATA m_wsaData;	
	SOCKET m_cmdsocket;		//���ӷ��������׽���
	int m_retCode;			//���һ������ص�����
	char m_commandBuff[RECV_BUFF_SIZE + 1];	//������Ļ���

	_XBool recvRetCode()
	{
		int ret = recv(m_cmdsocket,m_commandBuff,RECV_BUFF_SIZE,0);
		if(ret == SOCKET_ERROR || ret == 0) 
		{
			LogStr("Recv error!");
			return XFalse;
		}
		m_commandBuff[ret] = '\0';
		LogNull("%s",m_commandBuff);
		if(!getRetCode(m_commandBuff,ret)) return XFalse;
		return XTrue;
	}
	_XBool getRetCode(const char *buf,int len)
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
	_XBool sendCommand(const char * cmd)		//���������������
	{
		LogNull("%s",cmd);
		send(m_cmdsocket,cmd,strlen(cmd),0);
		return recvRetCode();
	}
public:
	_XBool connectServer(const char * serverIP,int port = FTP_SOCKET_PORT);	//���������������
	_XBool sendClose()
	{
		if(!m_isConnect) return XFalse;
		if(!sendCommand("QUIT\r\n")) return XFalse;
		closesocket(m_cmdsocket);
		m_isConnect = XFalse;
		if(m_retCode != 221) return XFalse;
		return XTrue;
	}
	//�����Ƕ�ftp��װ��һЩ����
	_XBool sendUserName(const char * userName)
	{
		if(userName == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy(m_commandBuff,"USER ");
		strcat(m_commandBuff,userName);
		strcat(m_commandBuff,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 331) return XFalse;	//��Ҫ����
		return XTrue;
	}
	_XBool sendPassword(const char * password)
	{
		if(password == NULL) return XFalse;
		if(!m_isConnect) return XFalse;
		strcpy(m_commandBuff,"PASS ");
		strcat(m_commandBuff,password);
		strcat(m_commandBuff,"\r\n");
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 230) return XFalse;	//��¼�ɹ�
		return XTrue;
	}
	_XBool setPort(SOCKET &listenSock);	//����ͨѶ�˿�
	_XBool setPasv(SOCKET &listenSock);
	_XBool sendFileList();	//��ȡ�ļ��б�
	_XBool sendDeleteFile(const char *filename)	//����ɾ���ļ�������
	{
		if(!m_isConnect) return XFalse;
		if(filename == NULL) return XFalse;
		strcpy(m_commandBuff,"DELE ");
		strcat(m_commandBuff,filename);
		strcat(m_commandBuff,"\r\n");	
		if(!sendCommand(m_commandBuff)) return XFalse;
		if(m_retCode != 250) return XFalse;	//delete�ɹ�
		return XTrue;
	}
	_XBool sendDownloadFile(const char *filename);	//����ָ���ļ�
	_XBool sendUpdateFile(const char *filename);	//�ϴ��ļ�
	_XBool sendUpdateFile(const char *inFilename,const char *outFilename);	//�ϴ��ļ�
	_XBool sendChangeDirectory(const char *dir)	//��һ��Ŀ¼
	{
		if(!m_isConnect) return XFalse;
		if(dir == NULL) return XFalse;
		strcpy(m_commandBuff,"CWD ");
		strcat(m_commandBuff,dir);
		strcat(m_commandBuff,"\r\n");	
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
		strcpy(m_commandBuff,"SIZE ");
		strcat(m_commandBuff,filename);
		strcat(m_commandBuff,"\r\n");	
		if(!sendCommand(m_commandBuff)) return -1;
		if(m_retCode != 213) return -1;
		int size = -1;
		if(sscanf(m_commandBuff,"213 %d",&size) != 1) return -1;
		return size;
	}

	_XFtpClient()
		:m_isConnect(XFalse)
	{}
};

#endif