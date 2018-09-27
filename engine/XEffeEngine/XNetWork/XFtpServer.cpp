#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpServer.h"
#include "XDirectory.h"
#include "XFile.h"
//#include "..\XLogBook.h"
namespace XE{
DWORD WINAPI XFtpServer::serverCMDThread(void * pParam)		//�����������߳�
{
	XFtpServer &pPar = *(XFtpServer *) pParam;
	char srv_cmd[SERVER_COMMAND_LEN];

	while(true)
	{
		printf("ftp_server> ");	
		fgets(srv_cmd,SERVER_COMMAND_LEN,stdin);
		if(strncmp("exit",srv_cmd,4) == 0 || strncmp("quit",srv_cmd,4) == 0 || strncmp("bye",srv_cmd,3) == 0)
		{//���շ������������ֲ�������Ӧ�ķ�Ӧ
			printf("Thank you for use!\nBye!\n");
			exit(1);	//�������˳�
		}
		if(strncmp("clientsum",srv_cmd,9) == 0) printf("There are %d Current active users.\n",pPar.m_clientSum); //��ʾ���ӵĿͻ��˵�����
		if(strncmp("countall",srv_cmd,8) == 0) printf("There are %d visitors\n",pPar.m_allClientSum);
		if(strncmp("listname",srv_cmd,8) == 0) 
		{//�������пͻ��˵�����
			printf("It is not finished!\n");
		}
		if(strncmp("killuser",srv_cmd,8) == 0)
		{//ǿ�ƹر�ĳ���ͻ��˵�����
			printf("It is not finished!\n");
		}
		if(strncmp("help",srv_cmd,4) == 0)
		{//��ʾ������Ϣ
			printf("It is not finished!\n");
		}
	}
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
DWORD WINAPI XFtpServer::serverAcceptThread(void * pParam)	//�����������߳�
{
	int sinSize = sizeof(sockaddr_in);
	sockaddr_in clientAddr;
	XFtpServer & pPar = *(XFtpServer *) pParam;
	int clientSock;	//�ͻ��˵��׽���
	while(true)
	{
		printf("ftp_server> ");	
		clientSock = accept(pPar.m_serverSock,(sockaddr *)&clientAddr,&sinSize);
		if(clientSock == -1)
		{
			printf("Accept error!\n");
			exit(1);
		}
		XFtpClientSockInfo * tempClient = XMem::createMem<XFtpClientSockInfo>();
		if(tempClient == NULL) exit(1);
		
		tempClient->clientSock = clientSock;
		memcpy(&tempClient->clientAddr,&clientAddr,sizeof(clientAddr));
		tempClient->pClass = pParam;
		//�����ͻ�����Ӧ�߳�
		pthread_t thread;
#ifdef XEE_OS_LINUX
		if(pthread_create(&thread,NULL,&serverRequestThread,(void*)tempClient) != 0)
#endif
#ifdef XEE_OS_WINDOWS
		if(CreateThread(0,0,serverRequestThread,tempClient,0,&thread) == 0)
#endif
		{//�߳̽���ʧ��
			printf("Server command thread create error!\n");
			exit(1);
		}
	}
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
DWORD WINAPI XFtpServer::serverRequestThread(void * pParam)	//�����������Ӧ�߳�(��δ���)
{//�Կͻ��˵�����������Ӧ,���������ĺ��Ĳ���
	XFtpClientSockInfo  *clientInfo = (XFtpClientSockInfo *)pParam;
	XFtpServer &pPar = *(XFtpServer *) clientInfo->pClass;
	printf("You got a connection from %s\n",inet_ntoa(clientInfo->clientAddr.sin_addr));
	//login����(��������)
	XFtpUserType clientType = pPar.clientLoginProc(clientInfo->clientSock);
	char recvBuf[RECV_BUFF_SIZE];	//���յ�����
	char retStr[RECV_BUFF_SIZE];	//���ص�����
	if(clientType == FTP_USER_TYPE_DEFAULT)
	{//��ͨ�û��������Ӧ
		while(true)
		{//�����û�������û�������������Ӧ(��δ���)
			if(!pPar.recvData(clientInfo->clientSock,recvBuf)) break;
			//�����������������
			if(strncmp("quit",recvBuf,4) == 0 || strncmp("QUIT",recvBuf,4) == 0)
			{//quit
				pPar.sendData(clientInfo->clientSock,returnStr221,strlen(returnStr221));
				-- pPar.m_clientSum;	//ע����̻߳���
				break;	//��������ѭ��
			}else
			if(strncmp("close",recvBuf,5) == 0 || strncmp("CLOSE",recvBuf,5) == 0)
			{//close(��δ���)
				break;
			}else
			if(strncmp("pwd",recvBuf,3) == 0 || strncmp("PWD",recvBuf,3) == 0)
			{//���õ�ǰ·��
				sprintf_s(retStr,RECV_BUFF_SIZE,returnStr257,clientInfo->curPath);
				pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
			}else
			if(strncmp("cwd",recvBuf,3) == 0 || strncmp("CWD",recvBuf,3) == 0)
			{//�ı䵱ǰ·��
				int len = strlen(recvBuf);
				memcpy(retStr,recvBuf + 4,len - 6);
				retStr[len - 6] = '\0';
				//�ж�·���Ƿ����
				if(XFile::isExistFileEx(retStr) && XFile::isFolderEx(retStr))	//���ļ��в�����
				{//�����򷵻�·�����óɹ�
					strcpy_s(clientInfo->curPath,MAX_FILE_NAME_LENGTH,retStr);
					sprintf_s(retStr,RECV_BUFF_SIZE,returnStr250,clientInfo->curPath);
					pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}else
				{//�������򷵻�·������ʧ��
					sprintf_s(retStr,RECV_BUFF_SIZE,"550 %s is not exist!\r\n",retStr);
					pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}
			}else
			if(strncmp("dele",recvBuf,4) == 0 || strncmp("DELE",recvBuf,4) == 0)
			{//ɾ��ָ���ļ�
				char filename[RECV_BUFF_SIZE];
				int len = strlen(recvBuf);
				memcpy(filename,recvBuf + 5,len - 7);
				filename[len - 7] = '\0';
				//�ж��Ƿ�ɾ���ɹ�
				sprintf_s(retStr,RECV_BUFF_SIZE,"%s/%s",clientInfo->curPath + 1,filename);
				if(XFile::isExistFileEx(retStr) && !XFile::isFolderEx(retStr) && XFile::deleteFile(retStr))
				{//�ļ�����
					sprintf_s(retStr,RECV_BUFF_SIZE,"250 \"%s\" is deleted!\r\n",filename);
					pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}else
				{//���ɾ��ʧ��
					sprintf_s(retStr,RECV_BUFF_SIZE,"550 %s is not exist!\r\n",filename);
					pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}
			}else
			if(strncmp("list",recvBuf,4) == 0 || strncmp("LIST",recvBuf,4) == 0)
			{//���ص�ǰĿ¼���ļ��б�
				pPar.sendData(clientInfo->clientSock,returnStr125,strlen(returnStr125));	//��ʾ����
				//��ʼ����list����
				char curPath[MAX_FILE_NAME_LENGTH];
				GetCurrentDirectory(MAX_FILE_NAME_LENGTH,curPath);
				sprintf_s(retStr,RECV_BUFF_SIZE,"%s/%s/*.*",curPath,clientInfo->curPath);
				std::string needStr = "";
				HANDLE hFind;
				WIN32_FIND_DATA fileInfo;
				SYSTEMTIME systemTime;
				hFind = FindFirstFile(retStr, &fileInfo);
				if(hFind != INVALID_HANDLE_VALUE)
				{
					while(true)
					{
						if(strcmp(fileInfo.cFileName,".") != 0 && strcmp(fileInfo.cFileName,"..") != 0)
						{
							FileTimeToSystemTime(&fileInfo.ftCreationTime,&systemTime);
							if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{//�ļ���
								sprintf_s(curPath,RECV_BUFF_SIZE,"%02d-%02d-%02d %02d:%02d  <DIR>  %s\n",systemTime.wMonth,systemTime.wDay,systemTime.wYear,systemTime.wHour,systemTime.wMinute,
									fileInfo.cFileName);
							}else
							{
								sprintf_s(curPath,RECV_BUFF_SIZE,"%02d-%02d-%02d %02d:%02d  %8d  %s\n",systemTime.wMonth,systemTime.wDay,systemTime.wYear,systemTime.wHour,systemTime.wMinute,
									(fileInfo.nFileSizeHigh << 16) + fileInfo.nFileSizeLow,fileInfo.cFileName);
							}
							needStr += curPath;
						}
						if(!FindNextFile(hFind,&fileInfo)) break;
					}
				}
				int len = needStr.length();
				for(int i = 0;i < len;i += RECV_BUFF_SIZE)
				{
					if(i + RECV_BUFF_SIZE >= len)
					{
						send(clientInfo->clientDataSock,needStr.c_str() + i,len - i,0);
					}else
					{
						send(clientInfo->clientDataSock,needStr.c_str() + i,RECV_BUFF_SIZE,0);
					}
				}

				//���ݴ�����ɷ���
				closesocket(clientInfo->clientDataSock);
				pPar.sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//��ʾ���ݴ������
			}else
			if(strncmp("port",recvBuf,4) == 0 || strncmp("PORT",recvBuf,4) == 0)
			{//���ô���˿�:PORT 192,168,1,110,20,30
				int portData[6];
				if(sscanf(recvBuf + 5,"%d,%d,%d,%d,%d,%d",&portData[0],&portData[1],&portData[2],&portData[3],&portData[4],&portData[5]) == 6)
				{//��ȷ������
					clientInfo->clientDataSock = socket(AF_INET,SOCK_STREAM,0);
					sockaddr_in clientDataSock;
					clientDataSock.sin_addr.S_un.S_un_b.s_b1 = portData[0];
					clientDataSock.sin_addr.S_un.S_un_b.s_b2 = portData[1];
					clientDataSock.sin_addr.S_un.S_un_b.s_b3 = portData[2];
					clientDataSock.sin_addr.S_un.S_un_b.s_b4 = portData[3];
					clientDataSock.sin_family = AF_INET;
					clientDataSock.sin_port = htons((portData[4] << 8) + portData[5]);
					if(connect(clientInfo->clientDataSock,(const sockaddr *)&clientDataSock,sizeof(sockaddr))==SOCKET_ERROR)
					{//�˿�����ʧ��
						printf("connect error!\n");
						return 1;	//������Ҫע��
					}
					sprintf_s(retStr,RECV_BUFF_SIZE,returnStr200,"PORT");
					pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
				}
			}else
			if(strncmp("type",recvBuf,4) == 0 || strncmp("TYPE",recvBuf,4) == 0)
			{//�ı����ݵĴ��䷽ʽ
				char mode = recvBuf[5];
				if(mode == 'I' || mode == 'i') clientInfo->transnatType = FTP_TRANSNAION_TYPE_BIN;else
				if(mode == 'A' || mode == 'a') clientInfo->transnatType = FTP_TRANSNAION_TYPE_ASCII;
				sprintf_s(retStr,RECV_BUFF_SIZE,returnStr200,"MODE");
				pPar.sendData(clientInfo->clientSock,retStr,strlen(retStr));
			}else
			if(strncmp("retr",recvBuf,4) == 0 || strncmp("RETR",recvBuf,4) == 0)
			{//�����ļ�
				pPar.sendData(clientInfo->clientSock,returnStr150,strlen(returnStr150));	//��ʾ����
				int len = strlen(recvBuf);
				memcpy(retStr,recvBuf + 5,len - 7);	//��ȡ�ļ���
				retStr[len - 7] = '\0';
				//��ָ���ļ��������ļ�����
				FILE *fp = NULL;
				std::string filePath = clientInfo->curPath + 1;
				filePath += std::string("/") + retStr;
				if((fp = fopen(filePath.c_str(),"rb")) == NULL)
				{//���ļ�ʧ��
					printf("Open file error!\n");
					return 1;	//������Ҫע��
				}
				len = XFile::getFileLen(fp);
				for(int index = 0;index < len;index += RECV_BUFF_SIZE)
				{
					if(index + RECV_BUFF_SIZE >= len)
					{
						fread(retStr,len - index,1,fp);
						send(clientInfo->clientDataSock,retStr,len - index,0);
					}else
					{
						fread(retStr,RECV_BUFF_SIZE,1,fp);
						send(clientInfo->clientDataSock,retStr,RECV_BUFF_SIZE,0);
					}
				}
				fclose(fp);
				//���ݴ�����ɷ���
				closesocket(clientInfo->clientDataSock);
				pPar.sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//��ʾ���ݴ������
			}else
			if(strncmp("stor",recvBuf,4) == 0 || strncmp("STOR",recvBuf,4) == 0)
			{//�ϴ��ļ�
				pPar.sendData(clientInfo->clientSock,returnStr150,strlen(returnStr150));	//��ʾ����
				int len = strlen(recvBuf);
				memcpy(retStr,recvBuf + 5,len - 7);	//��ȡ�ļ���
				retStr[len - 7] = '\0';
				//�����ļ�����ʼ��������
				FILE *fp = NULL;
				std::string filePath = clientInfo->curPath + 1;
				filePath += std::string("/") + retStr;
				if((fp = fopen(filePath.c_str(),"wb")) == NULL)
				{//���ļ�ʧ��
					printf("Open file error!\n");
					return 1;	//������Ҫע��
				}
				int ret = 0;
				while(true)
				{
					ret = recv(clientInfo->clientDataSock,(char *)retStr,RECV_BUFF_SIZE,0);
					if(ret <= 0) break;
					fwrite(retStr,ret,1,fp);
					Sleep(1);
				}
				fclose(fp);
				//���ݴ�����ɷ���
				closesocket(clientInfo->clientDataSock);
				pPar.sendData(clientInfo->clientSock,returnStr226,strlen(returnStr226));	//��ʾ���ݴ������
			}else
			//if(strncmp("size",recvBuf,4) == 0 || strncmp("SIZE",recvBuf,4) == 0)
			//{//��ȡ�ļ���С
			//}else
			//if(strncmp("syst",recvBuf,4) == 0 || strncmp("SYST",recvBuf,4) == 0)
			//{//����ϵͳ��Ϣ
			//}else
			//if(strncmp("feat",recvBuf,4) == 0 || strncmp("FEAT",recvBuf,4) == 0)
			//{//���ܻ����Ǻ����
			//}else
			//if(strncmp("rest",recvBuf,4) == 0 || strncmp("REST",recvBuf,4) == 0)
			//{//���ܻ����Ǻ����
			//}else
			//if(strncmp("pasv",recvBuf,4) == 0 || strncmp("PASV",recvBuf,4) == 0)
			//{//���ܻ����Ǻ����
			//}else
			//if(strncmp("mkd",recvBuf,3) == 0 || strncmp("MKD",recvBuf,3) == 0)
			//{//���ܻ����Ǻ����
			//}else
			//if(strncmp("rmd",recvBuf,3) == 0 || strncmp("RMD",recvBuf,3) == 0)
			//{//���ܻ����Ǻ����
			//}else
			{//��֧�ֵ������ʽ
				pPar.sendData(clientInfo->clientSock,returnStr202,strlen(returnStr202));
			}
			Sleep(1);
		}
	}else
	if(clientType == FTP_USER_TYPE_ANONY)
	{//������¼���û��Ķ�����Ӧ
		while(true)
		{//�����û�������û�������������Ӧ(��δ���)
			Sleep(1);
		}
	}
	closesocket(clientInfo->clientSock);
	XMem::XDELETE(clientInfo);
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
XBool XFtpServer::createServer()	//����������������accept�̣߳�û����һ�����ӣ�����һ���ͻ��˴����߳����ڴ���ͻ�������
{
	if(m_isCreated) return XFalse;	//��ֹ�ظ�����
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	m_serverSock = socket(AF_INET,SOCK_STREAM,0);
	if(m_serverSock == -1)
	{//�����������׽���ʧ��
		printf("Socket create error!\n");
		return XFalse;
	}
	int opt = SO_REUSEADDR;
	setsockopt(m_serverSock,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(opt));
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(FTP_SOCKET_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(m_serverSock,(sockaddr *)&server,sizeof(sockaddr)) == -1)
	{//���׽���ʧ��
		printf("Bind fail!\n");
		return XFalse;
	}
	if(listen(m_serverSock,SOMAXCONN) == -1)
	{
		printf("Listen error!\n");
		return XFalse;
	}
	//���濪���������߳�
#ifdef XEE_OS_LINUX
	if(pthread_create(&m_serverCMDThread, NULL, &serverCMDThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
	if(CreateThread(0,0,serverCMDThread,this,0,&m_serverCMDThread) == 0)
#endif
	{//�߳̽���ʧ��
		printf("Server command thread create error!\n");
		return XFalse;
	}
	//���濪ʼaccept�߳�
#ifdef XEE_OS_LINUX
	if(pthread_create(&m_serverAcceptThread, NULL, &serverAcceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
	if(CreateThread(0,0,serverAcceptThread,this,0,&m_serverAcceptThread) == 0)
#endif
	{//�߳̽���ʧ��
		printf("Server accept thread create error!\n");
		return XFalse;
	}

	//��ʼ���������
	m_clientSum = 0;	//Ŀǰ���ӵĿͻ��˵�����Ϊ0
	m_allClientSum = 0;

	m_isCreated = XTrue;
	return XTrue;
}
XFtpUserType XFtpServer::clientLoginProc(int clientSock)//����ͻ��˵�½�����������û������ͣ�0:��Ч,1:��ͨ�û�,2:�����û�
{
//	if(!m_isCreated) return XFalse;	//��δ�����������򷵻�ʧ��
	if(!sendData(clientSock,returnStr220,strlen(returnStr220))) return FTP_USER_TYPE_NULL;	//��������
	//�������û��������ݵȲ���
	char recvBuf[RECV_BUFF_SIZE];
	while(true)
	{//�ȴ������û���
		if(recvData(clientSock,recvBuf))
		{//������յ�����
			if(strncmp("USER",recvBuf,4) == 0 || strncmp("user",recvBuf,4) == 0)
			{//��������û���
				break;
			}else
			{//��������ݷǷ�
				sendData(clientSock,returnStr202,strlen(returnStr202));
			}
		}
		Sleep(1);
	}
	//����û�����
	int len = strlen(recvBuf);
	if(len < 7) return FTP_USER_TYPE_NULL;	//���ݷǷ�
	int index = 0;
	for(index = 5;index < len;++ index)
	{
		recvBuf[index - 5] = recvBuf[index];
	}
	recvBuf[index - 7] = '\0';
	XFtpUserType userType = FTP_USER_TYPE_NULL;
	if(strncmp(recvBuf,defaultUserName,strlen(defaultUserName)) == 0) userType = FTP_USER_TYPE_DEFAULT;else
	if(strncmp(recvBuf,anonyUserName,strlen(anonyUserName)) == 0) userType = FTP_USER_TYPE_ANONY;
	else return FTP_USER_TYPE_NULL;	//�û���������
	//��ʾ�û���������
	sendData(clientSock,returnStr331,strlen(returnStr331));
	//���ղ�У������
	recvData(clientSock,recvBuf);
	len = strlen(recvBuf);
	if(len < 7) return FTP_USER_TYPE_NULL;	//���ݷǷ�
	for(index = 5;index < len;++ index)
	{
		recvBuf[index - 5] = recvBuf[index];
	}
	recvBuf[index- 7] = '\0';
	if(userType == FTP_USER_TYPE_DEFAULT && strncmp(recvBuf,defaultUserPassword,strlen(defaultUserPassword)) == 0)
	{//������ȷ
		sendData(clientSock,returnStr230,strlen(returnStr230));	//���ص�¼�ɹ�
		++ m_clientSum;
		++ m_allClientSum;
		return userType;
	}else
	if(userType == FTP_USER_TYPE_ANONY && strncmp(recvBuf,anonyUserPassword,strlen(anonyUserPassword)) == 0)
	{//������ȷ
		sendData(clientSock,returnStr531,strlen(returnStr531));	//���������û���¼
		++ m_clientSum;
		++ m_allClientSum;
		return userType;
	}else
	{//�������
		sendData(clientSock,returnStr530,strlen(returnStr530));	//���ص�¼ʧ��
		return FTP_USER_TYPE_NULL;
	}

//	return FTP_USER_TYPE_NULL;
}
}