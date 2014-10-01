//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpClient.h"
#include "XNetCommon.h"

_XBool _XFtpClient::connectServer(const char * serverIP,int port)	//���������������
{
	if(serverIP == NULL ||
		m_isConnect) return XFalse;		//��ֹ�ظ�����
	
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) LogStr("Socket init error");
	m_cmdsocket = socket(AF_INET,SOCK_STREAM,0); //�����׽���
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(serverIP);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_cmdsocket,(const sockaddr *)&serveraddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		LogStr("connect error!");
		closesocket(m_cmdsocket);
		return XFalse;
	}
	//���շ�������
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 220) return XFalse;	//�������ݲ���
	m_isConnect = XTrue;
	return XTrue;
}
//�����Ƕ�ftp��װ��һЩ����
_XBool _XFtpClient::setPort(SOCKET &listenSock)	//����ͨѶ�˿�
{
	if(!m_isConnect) return XFalse;
	//��ȡ�����ip
	//char hname[128];
	//gethostname(hname,sizeof(hname));
	//hostent *hent = gethostbyname(hname);
	//sprintf(hname,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
	//	(unsigned char)(hent->h_addr_list[0][1]),
	//	(unsigned char)(hent->h_addr_list[0][2]),
	//	(unsigned char)(hent->h_addr_list[0][3]));
	//std::string dip = hname;
	std::string dip = getMyIP();
	std::string::size_type pos = 0;
	sockaddr_in localAddr;
	int sizeAddr = sizeof(sockaddr);
	UINT dataport;
	std::string str;
	listenSock = socket(AF_INET,SOCK_STREAM,0);
	//localAddr.sin_addr.s_addr = inet_addr(hname);
	localAddr.sin_addr.s_addr = inet_addr(dip.c_str());
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(0);
	if(bind(listenSock,(const sockaddr*)& localAddr,sizeAddr) == SOCKET_ERROR)
	{
		LogNull("bind error:%d",GetLastError());
		closesocket(listenSock);
		return XFalse;
	}
	listen(listenSock,5);
	getsockname(listenSock,(struct sockaddr*)&localAddr,&sizeAddr);
	dataport = ntohs(localAddr.sin_port);	
	while((pos = dip.find(".", pos)) != std::string::npos)
		dip.replace(pos,1, ",");

	sprintf(m_commandBuff,"PORT %s,%d,%d\r\n",dip.c_str(),dataport >> 8,dataport%256);
	if(!sendCommand(m_commandBuff))
	{
		closesocket(listenSock);
		return XFalse;
	}
	if(m_retCode != 200)
	{
		closesocket(listenSock);
		return XFalse;	//�������ݲ���
	}
	//�����޸�Ϊ���������ӣ���һЩ����»���ɿ���
	////����ȴ�������������
	//sockaddr_in clientaddr;
	//int sizeaddr = sizeof(sockaddr);
	//SOCKET clientsocket = accept(listenSock,(struct sockaddr*)&clientaddr,&sizeaddr);
	//closesocket(listenSock);
	//listenSock = clientsocket;
	return XTrue;
}
_XBool _XFtpClient::setPasv(SOCKET &listenSock) 
{ 
	sprintf(m_commandBuff,"PASV\r\n");
	if(!sendCommand(m_commandBuff)) return XFalse;
	if(m_retCode != 227) return XFalse;	//�������ݲ���
    listenSock = socket(AF_INET,SOCK_STREAM,0); 
    unsigned int v[6]; 
    union { 
        struct sockaddr sa; 
        struct sockaddr_in in; 
    } sin; 
    sscanf(m_commandBuff,"%*[^(](%u,%u,%u,%u,%u,%u",&v[2],&v[3],&v[4],&v[5],&v[0],&v[1]); 
    sin.sa.sa_family = AF_INET; 
    sin.sa.sa_data[2] = v[2]; 
    sin.sa.sa_data[3] = v[3]; 
    sin.sa.sa_data[4] = v[4]; 
    sin.sa.sa_data[5] = v[5]; 
    sin.sa.sa_data[0] = v[0]; 
    sin.sa.sa_data[1] = v[1]; 
 
    int on =1; 
    if(setsockopt(listenSock,SOL_SOCKET,SO_REUSEADDR, 
        (const char*) &on,sizeof(on)) == -1) 
    { 
        perror("setsockopt"); 
        closesocket(listenSock); 
        return XFalse; 
    } 
    struct linger lng = { 0, 0 }; 
 
    if(setsockopt(listenSock,SOL_SOCKET,SO_LINGER, 
        (const char*) &lng,sizeof(lng)) == -1) 
    { 
        perror("setsockopt"); 
        closesocket(listenSock); 
        return XFalse; 
    } 
 
	if(connect(listenSock,(sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		closesocket(listenSock);
		return XFalse;
	}
	return XTrue;
}
_XBool _XFtpClient::sendFileList()	//��ȡ�ļ��б�
{//����ļ��б�����ݲ�û�д��ݳ���
	if(!m_isConnect) return XFalse;
	SOCKET listensocket;
	bool isPortMode = true;	//�Ƿ�ʹ��portģʽ
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket)) return XFalse;
		isPortMode = false;
	}
	if(!sendCommand("LIST\r\n") ||
		m_retCode != 125)
	{
		closesocket(listensocket);
		return XFalse;	//�������ݲ���
	}
	//���濪ʼ��������
	SOCKET clientsocket = listensocket;
	if(isPortMode)
	{
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	char buf[RECV_BUFF_SIZE + 1];
	int ret = 0;
	while(true)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0)
		{
			buf[ret]='\0';
			LogNull("%s",buf);
		}else
		{//���ݷ������
			break;
		}
	}
	//�鿴�����Ƿ��Ѿ��������
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	closesocket(clientsocket);
	return XTrue;
}
_XBool _XFtpClient::sendDownloadFile(const char *filename)	//����ָ���ļ�
{
	if(!m_isConnect ||
		filename == NULL) return XFalse;
	SOCKET listensocket;
	bool isPortMode = true;	//�Ƿ�ʹ��portģʽ
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket)) return XFalse;
		isPortMode = false;
	}
	strcpy(m_commandBuff,"RETR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		m_retCode != 150)
	{
		closesocket(listensocket);
		return XFalse;
	}
	//���濪ʼ�����ļ�����
	SOCKET clientsocket = listensocket;
	if(isPortMode)
	{
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	std::string str = std::string(".") + "//" + filename;
	ofstream file;
	file.open(str.c_str(),ios::binary);
	int ret = 0;
	char buf[RECV_BUFF_SIZE];
	while(true)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0) file.write(buf,ret);
		else break;	//���ݽ������
	}
	file.close();
	closesocket(clientsocket);
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *filename)	//�ϴ��ļ�
{
	if(!m_isConnect ||
		filename == NULL) return XFalse;
	//����ļ��Ƿ����
	std::string str = std::string(".") + "//" + filename;
	ifstream file;
	file.open(str.c_str(),ios::binary);
	if(!file.is_open())
	{
		LogStr("file not exist!");
		return XFalse;	//�����Ҫ�ϴ����ļ���������ֱ�ӷ���
	}
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//�������ļ��ĳ���
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	if(length < 0)
	{
		file.close();
		return XFalse;
	}
	//���潨���ڷ�����������
	SOCKET listensocket;
	bool isPortMode = true;	//�Ƿ�ʹ��portģʽ
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket))
		{
			file.close();
			return XFalse;
		}
		isPortMode = false;
	}
	//�����ļ��洢����
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		(m_retCode != 150 && m_retCode != 125))
	{
		closesocket(listensocket);
		file.close();
		return XFalse;
	}
	SOCKET clientSocket = listensocket;
	if(isPortMode)
	{//�����portģʽ�Ļ�������ȴ�������������
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientSocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	//���濪ʼ�ϴ�����
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//�����ļ�����ֱ���������
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientSocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientSocket);
	if(!recvRetCode() ||
		m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *inFilename,const char *outFilename)
{
	if(!m_isConnect ||
		inFilename == NULL ||
		outFilename == NULL) return XFalse;
	//����ļ��Ƿ����
	std::string str;
	if(isAbsolutePath(inFilename)) str = inFilename;	//����·��
	else str = std::string(".") + "//" + inFilename;	//���·��
	ifstream file;
	file.open(str.c_str(),ios::binary);
	if(!file.is_open())
	{
		LogStr("file not exist!");
		return XFalse;	//����ļ���������ֱ�ӷ���
	}
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//�������ļ��ĳ���
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	if(length < 0)
	{//�ļ���ʧ��
		file.close();
		return XFalse;
	}
	//���ӷ�����
	SOCKET listensocket;
	bool isPortMode = true;	//�Ƿ�ʹ��portģʽ
	if(!setPort(listensocket))
	{
		if(!setPasv(listensocket))
		{
			file.close();
			return XFalse;
		}
		isPortMode = false;
	}
	//�����ļ��ϴ�����
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,outFilename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff) ||
		(m_retCode != 150 && m_retCode != 125))
	{
		closesocket(listensocket);
		file.close();
		return XFalse;
	}
	SOCKET clientSocket = listensocket;
	if(isPortMode)
	{//�����portģʽ�Ļ�������ȴ�������������
		sockaddr_in clientaddr;
		int sizeaddr = sizeof(sockaddr);
		clientSocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
		closesocket(listensocket);
	}
	//���濪ʼ��������
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//�����ļ�����ֱ���������
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientSocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientSocket);
	if(!recvRetCode() ||
		m_retCode != 226) return XFalse;
	return XTrue;
}