#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNetClient.h"
namespace XE{
XBool XNetClient::connectToServer(const char *ip,int port)
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//��ֹ�ظ�����
	if(ip == NULL) return XFalse;
	//���ӷ�����
	if(!getANewSocket(m_netSocket)) return XFalse;
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_netSocket,(const sockaddr *)&serveraddr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		LogStr("connect error!");
		closesocket(m_netSocket);
		return XFalse;
	}
	m_connectState = CONNECT_STATE_CONNECT;
	//����ͨѶ�߳�
	if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
	{//�߳̽���ʧ��
		LogStr("�����������߳̽���ʧ��!");
		return XFalse;
	}
	if(CreateThread(0,0,sendThread,this,0,&m_sendThread) == 0)
	{//�߳̽���ʧ��
		LogStr("�����������߳̽���ʧ��!");
		return XFalse;
	}

	return XTrue;
}
XBool XNetClient::sendBoardcast()
{
	unsigned char tempCheck;
	int offset = 0;
	int len;
	int bBroadcast = 1;
	sockaddr_in addrUDP;

	memset(&addrUDP,0,sizeof(addrUDP));
	addrUDP.sin_family  = AF_INET;
	addrUDP.sin_port  = htons(0);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    int sendSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(sendSocket < 0) return XFalse;
	resetSocketAddr(sendSocket); 
	if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return XFalse;
	if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return XFalse;	//���ն˿�

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(BOARDCAST_PORT);
#ifdef WITH_LOCAL_BOARDCAST_IP
	addrUDP.sin_addr.s_addr = inet_addr(BOARDCASR_IP); 
#else
	addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
#endif

	//����׼����Ҫ���͵�����
	unsigned char needSendData[BOARDCAST_DATA_LEN];
	needSendData[0] = 0xcc;needSendData[1] = 0x01;offset = 2;
	len = strlen(m_projectStr);
	memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
	memcpy(needSendData + offset,m_projectStr,len);offset += len;
	tempCheck = 0;
	for(int i = 1;i < offset;++ i)
	{
		tempCheck += needSendData[i];
	}
	needSendData[offset] = tempCheck;offset += 1;
	needSendData[offset] = 0xdd;offset += 1;

	if(sendto(sendSocket,(char *)needSendData,offset,0,(sockaddr *)&addrUDP,sizeof(addrUDP)) < offset) 
	{
		closesocket(sendSocket);
		return XFalse;	//������㲥����
	}

	closesocket(sendSocket);
	return XTrue;
}
//XBool XNetClient::recvBoardcast()
//{
//	unsigned char tempCheck;
//	int offset = 0;
//	int len;
//	sockaddr_in addrUDP;
//    memset(&addrUDP,0,sizeof(addrUDP)); 
//	addrUDP.sin_family = AF_INET;
//    addrUDP.sin_port = htons(BOARDCAST_PORT);
//    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
//    int recvSocket = socket(AF_INET,SOCK_DGRAM,0);
//	if(recvSocket < 0) return XFalse;
//	resetSocketAddr(recvSocket);
//	if(bind(recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return XFalse;	//���ն˿�
//
//	sockaddr_in addrPeer;
//	int tempServerPort;
//#ifdef XEE_OS_WINDOWS
//	int nSize = sizeof(addrPeer);
//#endif
//#ifdef XEE_OS_LINUX 
//	socklen_t nSize = sizeof(addrPeer);
//#endif
//	int nRecvLength = 0;
//	char szRecvBuffer[BOARDCAST_DATA_LEN] = {0};	//�˴���ʼ������
//	char recvProjectStr[PROJECT_STRING_LEN];
//	
//	//�ӷ��������շ���
//	memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
//	nRecvLength = recvfrom(recvSocket,szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);		//�������������
//	if(nRecvLength > 2 + sizeof(int))
//	{//������յ�����
//		//|0xcc   |0x00(server),0x01(client)|									|0xdd|
//		//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
//		if(szRecvBuffer[0] != 0xcc) return XFalse;	//���ݲ��Ϸ�ֱ�Ӷ���
//		if(szRecvBuffer[1] != 0x00) return XFalse;	//ֻ����ӷ��������͵�����
//		memcpy(&tempServerPort,szRecvBuffer + 2,sizeof(int));offset = 2 + sizeof(int);
//		memcpy(&len,szRecvBuffer + offset,sizeof(int));offset += sizeof(int);
//		if(len <= 0 || nRecvLength < offset + 2 + len) return XFalse;
//		//�������У���
//		tempCheck = 0;
//		for(int i = 1;i < offset + len;++ i)
//		{
//			tempCheck += (unsigned char)szRecvBuffer[i];
//		}
//		if(tempCheck != (unsigned char)szRecvBuffer[offset + len]) return XFalse;
//		memcpy(recvProjectStr,szRecvBuffer + offset,len);offset += len;
//		recvProjectStr[len] = '\0';
//		LogNull("recv:%s",recvProjectStr);
//		if(strcpy(recvProjectStr,m_projectStr) != 0) return XFalse;
//		//У����ȷ֮����������
//		m_serverPort = tempServerPort;
//		m_serverAddr = addrPeer;
//		closesocket(recvSocket);
//		return XTrue;
//	}
//	closesocket(recvSocket);
//	return XFalse;
//}
XBool XNetClient::connectToServer()
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//��ֹ�ظ�����
	//���ӷ�����
	if(!getANewSocket(m_netSocket)) return XFalse;
	if(!m_boardcastThreadState)
	{//���潨���㲥�߳�
		if(CreateThread(0,0,boardcastThread,this,0,&m_boardcastThread) == 0)
		{//�߳̽���ʧ��
			LogStr("�㲥�߳̽���ʧ��!");
			return XFalse;
		}
	}
	if(!sendBoardcast()) return XFalse;
	//if(!recvBoardcast()) return XFalse;
	Sleep(1000);	//�ȴ�1s
	m_mutexBoardCast.Lock();
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_serverPort);
	if(connect(m_netSocket,(const sockaddr *)&m_serverAddr,sizeof(sockaddr)) == SOCKET_ERROR)
	{
		LogStr("��������ʧ��!");
		closesocket(m_netSocket);
		m_mutexBoardCast.Unlock();
		return XFalse;
	}
	m_mutexBoardCast.Unlock();
	m_connectState = CONNECT_STATE_CONNECT;
	//����ͨѶ�߳�
	if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
	{//�߳̽���ʧ��
		LogStr("�ͻ��������߳̽���ʧ��!");
		closesocket(m_netSocket);
		return XFalse;
	}
	if(CreateThread(0,0,sendThread,this,0,&m_sendThread) == 0)
	{//�߳̽���ʧ��
		LogStr("�ͻ��˷����߳̽���ʧ��!");
		closesocket(m_netSocket);
		return XFalse;
	}

	return XTrue;
}
XBool XNetClient::createClient(XBool useBroadcast,const char *ip,int port)
{
	if(m_connectState == CONNECT_STATE_CONNECT) return XFalse;	//��ֹ�ظ�����
	if(m_isClientCreated) return XFalse;
	m_useBroadcast = useBroadcast;
	if(ip != NULL) m_serverIP = ip;
	else m_serverIP = "192.168.0.1";
	m_serverPort = port;
	if(CreateThread(0,0,connectThread,this,0,&m_connectThread) == 0)
	{//�߳̽���ʧ��
		LogStr("�ͻ��������߳̽���ʧ��!");
		closesocket(m_netSocket);
		return XFalse;
	}
	m_isClientCreated = XTrue;
	return XTrue;
}
XBool XNetClient::sendDataSocket(char * data,int len)
{//ÿ�η���4096������
	if(data == NULL || len <= 0) return XFalse;
	int ret = 0;
	//����1�����������ݶ�һ�η���
	ret = send(m_netSocket,data,len,0);
	if(ret < len)
	{
		LogStr("���ʹ���!");
		return XFalse;
	}
	return XTrue;
	//����2�����Ʒ��͵���С���ݵ�λ
	//int offset = 0;
	//while(true)
	//{
	//	if(len - offset < 4096)
	//	{
	//		ret = send(m_netSocket,data + offset,len - offset,0);
	//		if(ret < len - offset)
	//		{
	//			LogStr("���ʹ���!");
	//			return XFalse;
	//		}
	//		return XTrue;
	//	}else
	//	{
	//		ret = send(m_netSocket,data + offset,4096,0);
	//		if(ret < 4096)
	//		{
	//			LogStr("���ʹ���!");
	//			return XFalse;
	//		}
	//		offset += 4096;
	//	}
	//	Sleep(1);
	//}
	//return XTrue;
}
XBool XNetClient::getDataPacket(unsigned char *buff,int len)
{
	if(len <= 0) return XTrue;
	int offset = 0;
	if(m_recvPacketSize == 0)
	{
		if(len >= PACKET_HEAD_LEN)
		{//�����İ�ͷ
			m_recvPacket = XMem::createMem<XNetData>();
			if(m_recvPacket == NULL) return XFalse;
			m_recvPacket->type = (XNetDataType)buff[0];
			//if(m_recvPacket->type != DATA_TYPE_VIDEO)
			//{
			//	LogStr("haha");
			//}
			memcpy(&(m_recvPacket->dataLen),buff + 1,sizeof(int));
			//if(m_recvPacket->dataLen > 1000000)
			//{
			//	LogStr("haha");
			//}
			m_recvPacket->data = XMem::createArrayMem<unsigned char>(m_recvPacket->dataLen);
			if(m_recvPacket->data == NULL) return XFalse;
			if(len - PACKET_HEAD_LEN >= m_recvPacket->dataLen)
			{//��������
				m_recvPacket->isEnable = XTrue;
				memcpy(m_recvPacket->data,buff + PACKET_HEAD_LEN,m_recvPacket->dataLen);
				offset = PACKET_HEAD_LEN + m_recvPacket->dataLen;
				m_mutexRecv.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				if(m_recvDataBuff.size() > MAX_RECV_DATA_BUFF)
				{
					XNetData *tmp = m_recvDataBuff[0];
					m_recvDataBuff.pop_front();
					LogStr("XNetServer���ն������ݷ���ӵ��,�������ϵ�����!");
					XMem::XDELETE_ARRAY(tmp->data);
					XMem::XDELETE(tmp);
				}
				m_mutexRecv.Unlock();
				m_recvPacketSize = 0;
				//��������
				return getDataPacket(buff + offset,len - offset);
			}else
			{//���ݲ�����
				m_recvPacket->isEnable = XFalse;	//���ݰ��в�����
				memcpy(m_recvPacket->data,buff + PACKET_HEAD_LEN,len - PACKET_HEAD_LEN);
				m_recvPacketSize = len;
				return XTrue;
			}
		}else
		{//�������İ�ͷ
			m_recvPacketSize = len;
			memcpy(m_packetHeadData,buff,len);
			return XTrue;
		}
	}else
	{
		if(m_recvPacketSize >= PACKET_HEAD_LEN)
		{//��ͷ�Ѿ���������
			if(len >= m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN)
			{//������
				memcpy(m_recvPacket->data + m_recvPacketSize - PACKET_HEAD_LEN,buff,m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN);
				m_recvPacket->isEnable = XTrue;
				offset = m_recvPacket->dataLen - m_recvPacketSize + PACKET_HEAD_LEN;
				m_mutexRecv.Lock();
				m_recvDataBuff.push_back(m_recvPacket);
				m_recvPacketSize = 0;
				m_mutexRecv.Unlock();
				//����
				return getDataPacket(buff + offset,len - offset);
			}else
			{//��������
				memcpy(m_recvPacket->data + m_recvPacketSize - PACKET_HEAD_LEN,buff,len);
				m_recvPacketSize += len;
				return XTrue;
			}
		}else
		{//��ͷ��δ��������
			if(m_recvPacketSize + len >= PACKET_HEAD_LEN)
			{//��ͷ��������
				memcpy(m_packetHeadData + m_recvPacketSize,buff,PACKET_HEAD_LEN - m_recvPacketSize);
				//������ͷ
				m_recvPacket = XMem::createMem<XNetData>();
				if(m_recvPacket == NULL) return XFalse;
				m_recvPacket->type = (XNetDataType)m_packetHeadData[0];
				if(m_recvPacket->type != DATA_TYPE_VIDEO)
				{
					LogStr("haha");
				}
				memcpy(&(m_recvPacket->dataLen),m_packetHeadData + 1,sizeof(int));
				if(m_recvPacket->dataLen > 1000000)
				{
					LogStr("haha");
				}
				m_recvPacket->data = XMem::createArrayMem<unsigned char>(m_recvPacket->dataLen);
				if(m_recvPacket->data == NULL) return XFalse;
				//�������µ�����
				if(len - (PACKET_HEAD_LEN - m_recvPacketSize) >= m_recvPacket->dataLen)
				{//��������
					m_recvPacket->isEnable = XTrue;
					memcpy(m_recvPacket->data,buff + (PACKET_HEAD_LEN - m_recvPacketSize),m_recvPacket->dataLen);
					offset = PACKET_HEAD_LEN - m_recvPacketSize + m_recvPacket->dataLen;
					m_mutexRecv.Lock();
					m_recvDataBuff.push_back(m_recvPacket);
					m_recvPacketSize = 0;
					m_mutexRecv.Unlock();
					//��������
					return getDataPacket(buff + offset,len - offset);
				}else
				{//���ݲ�����
					m_recvPacket->isEnable = XFalse;	//���ݰ��в�����
					memcpy(m_recvPacket->data,buff + (PACKET_HEAD_LEN - m_recvPacketSize),len - (PACKET_HEAD_LEN - m_recvPacketSize));
					m_recvPacketSize += len;
					return XTrue;
				}
			}else
			{//��ͷ��Ȼ������
				memcpy(m_packetHeadData + m_recvPacketSize,buff,len);
				m_recvPacketSize += len;
				return XTrue;
			}
		}
	}
}
DWORD WINAPI XNetClient::recvThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	pPar.m_recvPacketSize = 0;
	fd_set readfds;
	timeval timeout;
	while(true)
	{
		//if(pPar.m_isExit) break;
		if(pPar.m_connectState == CONNECT_STATE_DISCONNECT) 
		{
			pPar.m_mutexRecv.Lock();
			if(pPar.m_recvPacketSize != 0)
			{//�������Ͽ����������һ����û�н����������������һ����
				XMem::XDELETE(pPar.m_recvPacket);	//�����������İ�
				pPar.m_recvPacketSize = 0;
			}
			pPar.m_mutexRecv.Unlock();
			break;
		}
		//�����������
		//����������
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(pPar.m_netSocket,&readfds);
		ret = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(ret > 0 && FD_ISSET(pPar.m_netSocket,&readfds))
		{//���Խ�������
			FD_CLR(pPar.m_netSocket, &readfds);
			ret = recv(pPar.m_netSocket,recvBuff,buffLen,0);
			if(ret == SOCKET_ERROR) pPar.m_connectState = CONNECT_STATE_DISCONNECT;	//���ӶϿ�
			if(ret > 0) pPar.getDataPacket((unsigned char *)recvBuff,ret);//���յ�����
		}
		Sleep(1);
	}
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
DWORD WINAPI XNetClient::sendThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	int buffSize = 4096;
	char * sendBuff = XMem::createArrayMem<char>(buffSize);
	if(sendBuff == NULL) return 0;
	while(pPar.m_connectState != CONNECT_STATE_DISCONNECT)
	{
		//if(pPar.m_isExit) break;
		//if(pPar.m_connectState == CONNECT_STATE_DISCONNECT) break;
		pPar.m_mutexSend.Lock();
		if(pPar.m_sendDataBuff.size() > 0)
		{//��������Ҫ����
			XNetData *tempData = (* pPar.m_sendDataBuff.begin());
			pPar.m_sendDataBuff.pop_front();
			pPar.m_mutexSend.Unlock();
			if(tempData->isEnable)
			{
				if(tempData->dataLen + PACKET_HEAD_LEN > buffSize)
				{//���ݳ��Ȳ�����Ҫ���·������ݳߴ�
					XMem::XDELETE_ARRAY(sendBuff);
					buffSize = tempData->dataLen + PACKET_HEAD_LEN;
					sendBuff = XMem::createArrayMem<char>(buffSize);
					if(sendBuff == NULL) return 0;
				}
				sendBuff[0] = tempData->type;
				memcpy(sendBuff + 1,&tempData->dataLen,sizeof(int));
				memcpy(sendBuff + PACKET_HEAD_LEN,tempData->data,tempData->dataLen);
				//��������
				if(!pPar.sendDataSocket(sendBuff,tempData->dataLen + PACKET_HEAD_LEN))
				{//�������ʧ�ܣ������ｫ�����ƻ�
					pPar.m_mutexSend.Lock();
					pPar.m_sendDataBuff.push_front(tempData);
					pPar.m_mutexSend.Unlock();
				}else
				{
					showNetData(tempData);
					XMem::XDELETE(tempData);	//�ͷ���Դ
				}
			}else
			{//��Ч������ֱ�Ӷ���
				showNetData(tempData);
				XMem::XDELETE(tempData);	//�ͷ���Դ
			}
		}else
		{
			pPar.m_mutexSend.Unlock();
		}
		Sleep(1);
	}
	XMem::XDELETE_ARRAY(sendBuff);
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
DWORD WINAPI XNetClient::boardcastThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	pPar.m_boardcastThreadState = XTrue;
	unsigned char tempCheck;
	int offset = 0;
	int len;
	sockaddr_in addrUDP;
    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
    addrUDP.sin_port = htons(BOARDCAST_PORT);
    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
    int recvSocket = socket(AF_INET,SOCK_DGRAM,0);
	if(recvSocket < 0) return 0;
	resetSocketAddr(recvSocket);
	if(bind(recvSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//���ն˿�

	sockaddr_in addrPeer;
	int tempServerPort;
#ifdef XEE_OS_WINDOWS
	int nSize = sizeof(addrPeer);
#endif
#ifdef XEE_OS_LINUX 
	socklen_t nSize = sizeof(addrPeer);
#endif
	int nRecvLength = 0;
	unsigned char szRecvBuffer[BOARDCAST_DATA_LEN];
	//memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
	char recvProjectStr[PROJECT_STRING_LEN];
	while(!pPar.m_isExit)
	{
		//if(pPar.m_isExit) break;
		//�ӷ��������շ���
		//memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
		//����������
		fd_set readfds;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(recvSocket,&readfds);
		int test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
		if(test_error > 0 && FD_ISSET(recvSocket,&readfds))
		{//���Խ�������
			FD_CLR(recvSocket, &readfds);
			memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
			nRecvLength = recvfrom(recvSocket,(char *)szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);		//�������������
			if(nRecvLength > 2 + sizeof(int))
			{//������յ�����
				//|0xcc   |0x00(server),0x01(client)|									|0xdd|
				//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
				if(szRecvBuffer[0] != 0xcc) continue;	//���ݲ��Ϸ�ֱ�Ӷ���
				if(szRecvBuffer[1] != 0x00) continue;	//ֻ����ӷ��������͵�����
				memcpy(&tempServerPort,szRecvBuffer + 2,sizeof(int));offset = 2 + sizeof(int);
				memcpy(&len,szRecvBuffer + offset,sizeof(int));offset += sizeof(int);
				if(len <= 0 || nRecvLength < offset + 2 + len) continue;
				//�������У���
				tempCheck = 0;
				for(int i = 1;i < offset + len;++ i)
				{
					tempCheck += szRecvBuffer[i];
				}
				if(tempCheck != szRecvBuffer[offset + len]) continue;
				memcpy(recvProjectStr,szRecvBuffer + offset,len);offset += len;
				recvProjectStr[len] = '\0';
				LogNull("recv:%s",recvProjectStr);
				if(strcmp(recvProjectStr,pPar.m_projectStr) != 0) continue;
				//У����ȷ֮����������
				pPar.m_mutexBoardCast.Lock();
				pPar.m_serverPort = tempServerPort;
				pPar.m_serverAddr = addrPeer;
				pPar.m_mutexBoardCast.Unlock();
			}
		}
		Sleep(1);
	}
	closesocket(recvSocket);
	pPar.m_boardcastThreadState = XFalse;
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 0;
}
void XNetClient::release()
{
	m_isExit = XTrue;
	if(m_connectState == CONNECT_STATE_CONNECT) m_connectState = CONNECT_STATE_DISCONNECT;
	while(m_threadSum != 0)
	{//�ȴ������߳̽���
		//if(m_threadSum == 0) break;
		Sleep(1);
	}
	std::deque<XNetData *>::iterator i;
	m_mutexRecv.Lock();
	for(i = m_recvDataBuff.begin();i != m_recvDataBuff.end();++ i)
	{
		XMem::XDELETE(*i);
	}
	m_recvDataBuff.clear();
	m_mutexRecv.Unlock();

	m_mutexSend.Lock();
	for(i = m_sendDataBuff.begin();i != m_sendDataBuff.end();++ i)
	{
		XMem::XDELETE(*i);
	}
	m_sendDataBuff.clear();
	m_mutexSend.Unlock();
	closesocket(m_netSocket);
	m_netSocket = INVALID_SOCKET;
	m_isClientCreated = XFalse;
}
DWORD WINAPI XNetClient::connectThread(void * pParam)
{
	XNetClient &pPar = *(XNetClient *)pParam;
	pPar.m_mutexThread.Lock();
	++ pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	XBool upState = pPar.getIsConnect();
	while(!pPar.m_isExit)
	{
		//if(pPar.m_isExit) break;
		if(!pPar.getIsConnect())
		{//�������ʧ�ܣ����￪ʼ���ӷ�����
			Sleep(1000);	//ÿ������һ�Σ���ֹ����Ƶ�ʹ���
			if(upState)
			{
				if(pPar.m_connectBrokenFun != NULL) (*pPar.m_connectBrokenFun)();
				upState = XFalse;
			}
			if(pPar.m_useBroadcast)
			{
				if(pPar.connectToServer())
				{
					if(pPar.m_connectFun != NULL) (*pPar.m_connectFun)();
					upState = XTrue;
				}
			}else
			{
				if(pPar.connectToServer(pPar.m_serverIP.c_str(),pPar.m_serverPort))
				{
					if(pPar.m_connectFun != NULL) (*pPar.m_connectFun)();
					upState = XTrue;
				}
			}
		}
		Sleep(1);
	}
	pPar.m_mutexThread.Lock();
	-- pPar.m_threadSum;
	pPar.m_mutexThread.Unlock();
	return 1;
}
}