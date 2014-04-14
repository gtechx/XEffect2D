#include "XSocketEx.h"
#include "XTimer.h"

DWORD WINAPI _XSocketEx::connectThread(void * pParam)
{
	_XSocketEx &par = *(_XSocketEx *)pParam;
	par.m_connectThreadState = STATE_START;
	_XTimer tempTimer;
	int oldTime = tempTimer.getCurrentTime();
	int nowTime;
	int stepTime = 0;
	while(1)
	{
		if(par.m_connectThreadState == STATE_SET_TO_END) break;
		//�����Ǳ�������״̬�Ĵ���
		switch(par.m_conState)
		{//��δ���
		case CON_STATE_NULL:
			par.connectToNet();//��δ�������ӣ���ֱ�ӽ�������
			break;
		case CON_STATE_NORMAL:	//����������ʲôҲ����
			nowTime = tempTimer.getCurrentTime();
			stepTime = nowTime - oldTime;
			oldTime = nowTime;
			par.m_timer += stepTime;
			if(par.m_timer > 2000)
			{//���������������
				par.m_timer -= 2000;
				if(par.m_socketRole == NET_CLIENT)
				{//���������������
					par.sendHeartbeat();
				}
			}
			if(par.m_socketRole == NET_SERVER)
			{//�Կͻ��˽���������ʱ
				par.heartbeatProc(stepTime);
			}
			break;
		case CON_STATE_UNSTABLE:
			par.connectToNet();//���ݵ�ʧȥ�������������
			break;
		case CON_STATE_ERROR:
			par.connectToNet();//�����ʱ��ʧȥ��������д���
			break;
		}
		//������������ʱ
		Sleep(1);
	}
	par.m_connectThreadState = STATE_END;
	return 1;
}
DWORD WINAPI _XSocketEx::recvThreadX(void * pParam)
{
	_XSocketEx &par = *(_XSocketEx *)pParam;
	int clinetID = par.m_nowAddClientID;
	par.m_nowAddClientID = -1;
	par.m_usersData[clinetID]->recvThreadState = STATE_START;
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	fd_set readfds;
	timeval timeout;
	int testError;
	while(1)
	{
		if(par.m_usersData[clinetID]->recvThreadState == STATE_SET_TO_END) break;
		if(par.m_usersData[clinetID]->state == CLINET_STATE_CONNECT
			&& par.m_usersData[clinetID]->isEnable)
		{//�������������Ž�������
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			FD_ZERO(&readfds);
			FD_SET(par.m_usersData[clinetID]->userSocket,&readfds);
			testError = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
			if(testError > 0 && FD_ISSET(par.m_usersData[clinetID]->userSocket,&readfds))
			{//���Խ�������
				FD_CLR(par.m_usersData[clinetID]->userSocket,&readfds);
				ret = recv(par.m_usersData[clinetID]->userSocket,recvBuff,buffLen,0);
				if(ret == SOCKET_ERROR) par.setOffline(par.m_usersData[clinetID]->userID);	//���ݽ���ʧ��,���ʾ��������ʧ��
				if(ret > 0)
				{
					par.getDataPacket(par.m_usersData[clinetID]->recvPacket,(unsigned char *)recvBuff,ret);	//���յ�����,��������ݽ��д���
					par.m_usersData[clinetID]->heartbeatTimer = 0;	//�յ�����Ҳ���������¼
				}
			}
		}
		Sleep(1);
	}
	par.m_usersData[clinetID]->recvThreadState = STATE_END;
	return 1;
}
DWORD WINAPI _XSocketEx::boardcastThread(void * pParam)
{//�㲥�̣߳�����˸�����չ㲥���������ݣ��ͻ��˸�����ղ���¼�������ĵ�ַ
	_XSocketEx &par = *(_XSocketEx *)pParam;
	par.m_boardcastThreadState = STATE_START;

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

	int sendSocket;
	unsigned char needSendData[BOARDCAST_DATA_LEN];	//����Ƿ������Ļ���������Ҫ���͵�����
	if(par.m_socketRole == NET_SERVER)
	{
		int bBroadcast = 1;
		memset(&addrUDP,0,sizeof(addrUDP));
		addrUDP.sin_family  = AF_INET;
		addrUDP.sin_port  = htons(0);
		addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
		sendSocket = socket(AF_INET,SOCK_DGRAM,0);
		if(sendSocket < 0) return 0;
		resetSocketAddr(sendSocket); 
		if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return 0;
		if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return 0;	//���Ͷ˿�

		memset(&addrUDP,0,sizeof(addrUDP)); 
		addrUDP.sin_family = AF_INET;
		addrUDP.sin_port = htons(BOARDCAST_PORT);
		addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST); 
		//����׼����Ҫ���͵�����
		needSendData[0] = 0xcc;needSendData[1] = 0x00;offset = 2;
		memcpy(needSendData + offset,&par.m_serverPort,sizeof(int));offset += sizeof(int);
		len = par.m_serverName.size() + 1;
		memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
		memcpy(needSendData + offset,par.m_serverName.c_str(),len);offset += len;
		tempCheck = 0;
		for(int i = 1;i < offset;++ i)
		{
			tempCheck += needSendData[i];
		}
		needSendData[offset] = tempCheck;offset += 1;
		needSendData[offset] = 0xdd;offset += 1;
	}
	sockaddr_in addrPeer;
	int tempServerPort;
	int nRecvLength = 0;
#ifdef XEE_OS_WINDOWS
	int nSize = sizeof(addrPeer);
#endif
	unsigned char szRecvBuffer[BOARDCAST_DATA_LEN];
	std::string serverName;
	while(1)
	{
		if(par.m_boardcastThreadState == STATE_SET_TO_END) break;
		memset(szRecvBuffer,0,BOARDCAST_DATA_LEN);
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
			nRecvLength = recvfrom(recvSocket,(char *)szRecvBuffer,BOARDCAST_DATA_LEN,0,(sockaddr*)&addrPeer,&nSize);
			if(nRecvLength > 2 + sizeof(int))
			{//������յ�����
				if(par.m_socketRole == NET_SERVER)
				{//�����������տͻ��˵����ݲ�������Ӧ
					//|0xcc   |0x00(server),0x01(client)|									|0xdd|
					//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
					if(szRecvBuffer[0] != 0xcc) continue;	//���ݲ��Ϸ�ֱ�Ӷ���
					if(szRecvBuffer[1] != 0x01) continue;	//ֻ�����ӿͻ��˽��յ�����
					memcpy(&len,szRecvBuffer + 2,sizeof(int));
					if(len <= 0 || nRecvLength < 4 + sizeof(int) + len) continue;
					//�������У���
					tempCheck = 0;
					for(int i = 0;i < len + 1 + sizeof(int);++ i)
					{
						tempCheck += szRecvBuffer[i + 1];
					}
					if(tempCheck != szRecvBuffer[2 + sizeof(int) + len]) continue;
					serverName = (char *)(szRecvBuffer + 2 + sizeof(int));
				//	printf("recv:%s\n",serverName.c_str());
					if(serverName != par.m_serverName) continue;
					//�����Ӧ�ͻ��˵�����
					sendto(sendSocket,(char *)needSendData,offset,0,(sockaddr *)&addrUDP,sizeof(addrUDP));
				}else
				if(par.m_socketRole == NET_CLIENT
					|| par.m_socketRole == NET_NULL)
				{//�ͻ��ˣ����շ������˵����ݲ���������
					//|0xcc   |0x00(server),0x01(client)|									|0xdd|
					//|Head(1)|Type(1)|Port(4)[for server]|StrLen(4)|String(x)|Check(1)|End(1)|
					if(szRecvBuffer[0] != 0xcc) continue;	//���ݲ��Ϸ�ֱ�Ӷ���
					if(szRecvBuffer[1] != 0x00) continue;	//ֻ�����ӷ��������͵�����
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
					serverName = (char *)(szRecvBuffer + offset);
				//	printf("recv:%s\n",serverName.c_str());
					if(serverName != par.m_serverName) continue;
					//У����ȷ֮����������
					par.m_serverPort = tempServerPort;
					par.m_serverAddr = addrPeer;
				}
			}
		}
		Sleep(1);
	}
	par.m_boardcastThreadState = STATE_END;
	return 1;
}
DWORD WINAPI _XSocketEx::recvThread(void * pParam)
{
	_XSocketEx &par = *(_XSocketEx *)pParam;
	par.m_recvThreadState = STATE_START;
	int buffLen = 4096;
	char recvBuff[4096];
	int ret = 0;
	fd_set readfds;
	timeval timeout;
	int testError;
	while(1)
	{
		if(par.m_recvThreadState == STATE_SET_TO_END) break;
		//������������ݣ��������ݽ��д����������Ϊ�������յ���Ҫת�������ݣ���ֱ��ת��
		//����������
		if(par.m_conState == CON_STATE_NORMAL)
		{//�������������Ž�������
			if(par.m_socketRole == NET_CLIENT)
			{//��������Ϊÿ���ͻ��˿���һ�������߳�
				timeout.tv_sec = 0;
				timeout.tv_usec = 0;
				FD_ZERO(&readfds);
				FD_SET(par.m_netSocket,&readfds);
				testError = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
				if(testError > 0 && FD_ISSET(par.m_netSocket,&readfds))
				{//���Խ�������
					FD_CLR(par.m_netSocket,&readfds);
					ret = recv(par.m_netSocket,recvBuff,buffLen,0);
					if(ret == SOCKET_ERROR) par.m_conState = CON_STATE_UNSTABLE;	//���ݽ���ʧ��,���ʾ��������ʧ��
					if(ret > 0)
					{
						par.getDataPacket(par.m_recvPacket,(unsigned char *)recvBuff,ret);	//���յ�����,��������ݽ��д���
					//	par.m_timer = 0;	//�ͻ����յ���������Բ��÷�������
					}
				}
			}
		}
		Sleep(1);
	}
	par.m_recvThreadState = STATE_END;
	return 1;
}
DWORD WINAPI _XSocketEx::sendThread(void * pParam)
{
	_XSocketEx &par = *(_XSocketEx *)pParam;
	par.m_sendThreadState = STATE_START;
	int ret,len;
	int tempDataSize = 2048;
	unsigned char * tempData = createArrayMem<unsigned char>(tempDataSize);
	while(1)
	{
		if(par.m_sendThreadState == STATE_SET_TO_END) break;
		if(par.m_sendDeque.size() <= 0) 
		{
			Sleep(1);
			continue;
		}//��������Ҫ���ͣ������﷢������
		par.m_sendMutex.Lock();
		_XSocketDataPack *data = par.m_sendDeque[0];
		par.m_sendMutex.Unlock();
		if(par.m_socketRole == NET_CLIENT)
		{//ֱ�ӽ����ݷ��͵�������
			len = SOCKET_PACKET_HEAD_SIZE + data->dataLen;
			if(len > tempDataSize)
			{
				tempDataSize = (tempDataSize << 1);
				XDELETE_ARRAY(tempData);
				tempData = createArrayMem<unsigned char>(tempDataSize);
			}
			memcpy(tempData,(unsigned char *)data,SOCKET_PACKET_HEAD_SIZE);
			memcpy(tempData + SOCKET_PACKET_HEAD_SIZE,data->data,data->dataLen);
			ret = send(par.m_netSocket,(char *)tempData,len,0);
			if(ret == SOCKET_ERROR) par.m_conState = CON_STATE_UNSTABLE;
			if(ret == len)
			{//���ͳɹ�,�����ʱ����
				par.m_sendMutex.Lock();
				par.m_sendDeque.pop_front();
				par.m_sendMutex.Unlock();
				XDELETE_ARRAY(data->data);
				XDELETE(data);
				par.m_timer = 0;	//�ͻ����յ���������Բ��÷�������
			}
		}else
		if(par.m_socketRole == NET_SERVER)
		{//��Ҫ�������ݵ�Ŀ���ٽ����ݷ��ͳ�ȥ
			len = 0;
			par.m_userInfoMutex.Lock();
			for(int i = 0;i < par.m_usersInfo.size();++ i)
			{
				if(par.m_usersInfo[i]->userID == data->toID)
				{//��Ŀ�귢������
					if(par.m_usersInfo[i]->userState == CLIENT_STATE_OFFLINE)
					{//����Ѿ����ߣ���ֱ�ӽ����ݰ������
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendDeque.push_back(data);
						par.m_sendMutex.Unlock();
						len = 1;
						break;
					}

					len = SOCKET_PACKET_HEAD_SIZE + data->dataLen;
					if(len > tempDataSize)
					{
						tempDataSize = (tempDataSize << 1);
						XDELETE_ARRAY(tempData);
						tempData = createArrayMem<unsigned char>(tempDataSize);
					}
					memcpy(tempData,(unsigned char *)data,SOCKET_PACKET_HEAD_SIZE);
					memcpy(tempData + SOCKET_PACKET_HEAD_SIZE,data->data,data->dataLen);
					ret = send(par.m_usersInfo[i]->userSocket,(char *)tempData,len,0);
					if(ret == SOCKET_ERROR)
					{//�������ʧ���������˺�
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendDeque.push_back(data);
						par.m_sendMutex.Unlock();
						par.setOffline(par.m_usersInfo[i]->userID);
					}
					if(ret == len)
					{//���ͳɹ�,�����ʱ����
						par.m_sendMutex.Lock();
						par.m_sendDeque.pop_front();
						par.m_sendMutex.Unlock();
						XDELETE_ARRAY(data->data);
						XDELETE(data);
					}
					break;
				}
			}
			par.m_userInfoMutex.Unlock();
			if(len == 0)
			{//Ŀ�겻���������ݶ���(������ܴ�������)
				par.m_sendMutex.Lock();
				par.m_sendDeque.pop_front();
				par.m_sendMutex.Unlock();
				XDELETE_ARRAY(data->data);
				XDELETE(data);
			}
		}
		Sleep(1);
	}
	XDELETE_ARRAY(tempData);
	par.m_sendThreadState = STATE_END;
	return 1;
}
DWORD WINAPI _XSocketEx::acceptThread(void * pParam)
{
	_XSocketEx &par = *(_XSocketEx *)pParam;
	par.m_acceptThreadState = STATE_START;
	int clientSocket;
	while(1)
	{
		if(par.m_acceptThreadState == STATE_SET_TO_END) break;
        clientSocket = accept(par.m_netSocket,NULL,NULL);
		if(clientSocket < 0)
		{//���տͻ���ʧ��
			printf("�ͻ��˼���ʧ��!\n");
		}else
		{//���տͻ��˳ɹ�֮����һ�������߳�
			printf("�ͻ��˼���ɹ�!\n");
			par.addAClient(clientSocket);
		}
		Sleep(100);
	}
	par.m_acceptThreadState = STATE_END;
	return 1;
}
void _XSocketEx::addAClient(int clientSocket)
{
	if(m_nowAddClientID != -1)
	{//��������ɿͻ��˽��룬������ȴ��ͻ��˼������
		while(m_nowAddClientID != -1)
		{
			Sleep(1);
		}
	}
	++ m_IDIndex;
	//����һ���ͻ�����Ϣ
	_XSocketUserData *data = createMem<_XSocketUserData>();
	if(data == NULL) return;	
	data->userID = m_IDIndex;
	data->userSocket = clientSocket;
	data->state = CLINET_STATE_CONNECT;
	data->isEnable = true;
	data->recvThreadState = STATE_BEFORE_START;
	m_usersData.push_back(data);
	m_nowAddClientID = m_usersData.size() - 1;
	//Ϊ����ͻ��˿��������߳�
	if(CreateThread(0,0,recvThreadX,this,0,NULL) == 0) return;
	//�������ͻ��˷����¿ͻ��˽������Ϣ
	//�����û���Ϣ
	char tempCStr[128];
	sprintf(tempCStr,"%d",m_IDIndex);
	std::string tempStr = DEFAULT_CLIENT_NAME;
	tempStr += tempCStr;
	_XSocketUserInfo *newInfo = createMem<_XSocketUserInfo>();
	if(newInfo == NULL) return;
	newInfo->userID = m_IDIndex;
	newInfo->userSocket = clientSocket;
	newInfo->userNameLen = tempStr.size() + 1;
	newInfo->userName = createArrayMem<char>(newInfo->userNameLen);
	strcpy(newInfo->userName,tempStr.c_str());
	newInfo->userState = CLIENT_STATE_ONLINE;
	m_userInfoMutex.Lock();
	m_usersInfo.push_back(newInfo);
	m_userInfoMutex.Unlock();
	//���������������ID������
	_XSocketDataPack *tempPacket = createMem<_XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = m_IDIndex;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(_XSocketSysDataType);
	tempPacket->data = createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	_XSocketSysDataType tempType = SYS_DATA_TYPE_SETID;
	memcpy(tempPacket->data,&tempType,sizeof(_XSocketSysDataType));
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
	sendClientInfo();
}
void _XSocketEx::sendClientInfo()
{
	int buffSize = sizeof(int) * 2;
	m_userInfoMutex.Lock();
	for(int i = 0;i < m_usersInfo.size();++ i)
	{
		buffSize += SOCKET_INFO_HEAD_SIZE;
		buffSize += m_usersInfo[i]->userNameLen;
	}
	unsigned char * tempData = createArrayMem<unsigned char>(buffSize);
	if(tempData == NULL)
	{
		m_userInfoMutex.Unlock();
		return;
	}
	int offset = 0;
	//��������
	int size = SYS_DATA_TYPE_UPDATEINIFO;
	memcpy(tempData + offset,&size,sizeof(int));
	offset += sizeof(int);
	size = m_usersInfo.size();
	memcpy(tempData + offset,&size,sizeof(int));
	offset += sizeof(int);
	for(int  i = 0;i < m_usersInfo.size();++ i)
	{
		memcpy(tempData + offset,&m_usersInfo[i]->userID,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userSocket,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userState,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,&m_usersInfo[i]->userNameLen,sizeof(int));
		offset += sizeof(int);
		memcpy(tempData + offset,m_usersInfo[i]->userName,m_usersInfo[i]->userNameLen);
		offset += m_usersInfo[i]->userNameLen;
	}
	m_userInfoMutex.Unlock();
	lockData(tempData,buffSize);
	_XSocketDataPack *tempPacket = NULL;
	m_userInfoMutex.Lock();
	for(int i = 0;i < m_usersInfo.size();++ i)
	{//��ÿ���ͻ��˷�������
		if(m_usersInfo[i]->userID == m_myID) continue;	//���ø��Լ�������Ϣ
		if(m_usersInfo[i]->userState == CLIENT_STATE_OFFLINE) continue;
		tempPacket = createMem<_XSocketDataPack>();
		if(tempPacket == NULL) return;
		tempPacket->fromID = m_myID;
		tempPacket->toID = m_usersInfo[i]->userID;
		tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
		tempPacket->dataLen = buffSize;
		tempPacket->data = createArrayMem<unsigned char>(buffSize);
		if(tempPacket->data == NULL) return;
		memcpy(tempPacket->data,tempData,buffSize);
		sendAData(tempPacket);
	}
	m_userInfoMutex.Unlock();
	//�ͷ���ʱ��Դ
	XDELETE_ARRAY(tempData);
}
void _XSocketEx::systemDataproc(const _XSocketDataPack *data)
{
	if(m_socketRole == NET_CLIENT)
	{
		if(data->dataType == SOCKET_DATA_TYPE_SYSTEM)
		{//������������Ϣ//��δ���,��Ҫ�ӷ�����֪���Լ���ID
			_XSocketSysDataType type;
			if(data->dataLen < sizeof(_XSocketSysDataType)) return;	//Error
			memcpy(&type,data->data,sizeof(_XSocketSysDataType));
			switch(type)
			{
			case SYS_DATA_TYPE_SETID:
				m_myID = data->toID;
				break;
			case SYS_DATA_TYPE_UPDATEINIFO:
				int sum = 0;
				memcpy(&sum,data->data + sizeof(int),sizeof(int));
				if(sum <= 0) return;	//Error
				//ɾ��ԭ�е���Ϣ
				m_userInfoMutex.Lock();
				for(int i = 0;i < m_usersInfo.size();++ i)
				{
					XDELETE_ARRAY(m_usersInfo[i]->userName);
					XDELETE(m_usersInfo[i]);
				}
				m_usersInfo.clear();
				//�����µ���Ϣ
				_XSocketUserInfo *tempInfo = NULL;
				int offset = sizeof(int) * 2;
				for(int i = 0;i < sum;++ i)
				{
					tempInfo = createMem<_XSocketUserInfo>();
					if(tempInfo == NULL)
					{
						m_userInfoMutex.Unlock();
						return;	//Error
					}
					memcpy(&tempInfo->userID,data->data + offset,sizeof(int));
					offset += sizeof(int);
					memcpy(&tempInfo->userSocket,data->data + offset,sizeof(int));
					offset += sizeof(int);
					memcpy(&tempInfo->userState,data->data + offset,sizeof(int));
					offset += sizeof(int);
					memcpy(&tempInfo->userNameLen,data->data + offset,sizeof(int));
					offset += sizeof(int);
					tempInfo->userName = createArrayMem<char>(tempInfo->userNameLen);
					if(tempInfo->userName == NULL)
					{
						m_userInfoMutex.Unlock();
						return;	//Error
					}
					memcpy(tempInfo->userName,data->data + offset,tempInfo->userNameLen);
					offset += tempInfo->userNameLen;

					m_usersInfo.push_back(tempInfo);
				}
				m_userInfoMutex.Unlock();
				break;
			}
		}
	}else
	if(m_socketRole == NET_SERVER)
	{
		if(data->dataType == SOCKET_DATA_TYPE_SYSTEM)
		{//�ͻ��˸���,�ҵ�ָ���Ŀͻ��˲����޸����֣�Ȼ��㲥
			_XSocketSysDataType type;
			if(data->dataLen < sizeof(_XSocketSysDataType)) return;	//Error
			memcpy(&type,data->data,sizeof(_XSocketSysDataType));
			switch(type)
			{
			case SYS_DATA_TYPE_SETNAME:
				m_userInfoMutex.Lock();
				for(int  i = 0;i < m_usersInfo.size();++ i)
				{
					if(m_usersInfo[i]->userID == data->fromID)
					{//�޸����ֲ��㲥
						if(m_usersInfo[i]->userNameLen != data->dataLen - sizeof(_XSocketSysDataType))
						{
							m_usersInfo[i]->userNameLen = data->dataLen - sizeof(_XSocketSysDataType);
							XDELETE_ARRAY(m_usersInfo[i]->userName);
							m_usersInfo[i]->userName = createArrayMem<char>(m_usersInfo[i]->userNameLen);
							if(m_usersInfo[i]->userName == NULL) return;
							memcpy(m_usersInfo[i]->userName,data->data + sizeof(_XSocketSysDataType),m_usersInfo[i]->userNameLen);
						}
						sendClientInfo();	//��������Ż�
						break;
					}
				}
				m_userInfoMutex.Unlock();
				break;
			case SYS_DATA_TYPE_HEARTBEAT:
				for(int  i = 0;i < m_usersData.size();++ i)
				{
					if(m_usersData[i]->userID == data->fromID)
					{//�յ������ź����������ʱ��
						m_usersData[i]->heartbeatTimer = 0;
					}
				}
				break;
			}
		}
	}
}
void _XSocketEx::lockData(unsigned char *data,int len)
{//��δ���,��æ��ʵ��
}
void _XSocketEx::unlockData(unsigned char *data,int len)
{//��δ���,��æ��ʵ��
}
bool _XSocketEx::getDataPacket(_XSocketRecvPacket &packet,unsigned char *buff,int len)
{//��δ���
	if(len <= 0 || buff == NULL) return false;
	if(packet.recvDataLen == 0)
	{//��һ�ν���
		if(len >= SOCKET_PACKET_HEAD_SIZE)
		{//��ͷ����
			memcpy(packet.headData,buff,SOCKET_PACKET_HEAD_SIZE);
			packet.recvDataLen = SOCKET_PACKET_HEAD_SIZE;
			packet.dataPacket = createMem<_XSocketDataPack>();
			if(packet.dataPacket == NULL) return false;	//���ش���
			//���潫��ͷ���ݽ�������
			memcpy(&(packet.dataPacket->fromID),packet.headData,sizeof(int));
			memcpy(&(packet.dataPacket->toID),packet.headData + sizeof(int),sizeof(int));
			memcpy(&(packet.dataPacket->dataType),packet.headData + sizeof(int) * 2,sizeof(int));
			memcpy(&(packet.dataPacket->dataLen),packet.headData + sizeof(int) * 3,sizeof(int));
			packet.dataPacket->data = createArrayMem<unsigned char>(packet.dataPacket->dataLen);
			if(packet.dataPacket->data == NULL) return false;	//���ش���
			return getDataPacket(packet,buff + SOCKET_PACKET_HEAD_SIZE,len - SOCKET_PACKET_HEAD_SIZE);
		}else
		{//��ͷ������
			memcpy(packet.headData,buff,len);
			packet.recvDataLen = len;
			return true;
		}
	}else
	{//�Ѿ������˲���
		if(packet.recvDataLen >= SOCKET_PACKET_HEAD_SIZE)
		{//��ͷ�Ѿ���������
			if(len + packet.recvDataLen >= packet.dataPacket->dataLen + SOCKET_PACKET_HEAD_SIZE)
			{//���ݰ�����
				int needLen = packet.dataPacket->dataLen + SOCKET_PACKET_HEAD_SIZE - packet.recvDataLen;
				memcpy(packet.dataPacket->data + (packet.recvDataLen - SOCKET_PACKET_HEAD_SIZE),buff,needLen);
				//���ݰ����������������ݰ����д���
				if(m_socketRole == NET_CLIENT)
				{//����ǿͻ�����ֱ�ӽ�����������ܶ���
					unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);
					if(packet.dataPacket->dataType == SOCKET_DATA_TYPE_SYSTEM)
					{//������յ�����ϵͳ���ݵĻ������ﴦ��ϵͳ����
						systemDataproc(packet.dataPacket);
						XDELETE_ARRAY(packet.dataPacket->data);
						XDELETE(packet.dataPacket);
					}else
					{
						m_recvMutex.Lock();
						m_recvDeque.push_back(packet.dataPacket);
						m_recvMutex.Unlock();
					}
				}else
				if(m_socketRole == NET_SERVER)
				{//����Ƿ�������Ҫ�ж������Ƿ��Ƿ����Լ���
					if(packet.dataPacket->toID != m_myID)
					{
						if(packet.dataPacket->toID > 0) sendAData(packet.dataPacket);	//������Ҫת��
						else
						{//��Ҫ�㲥������
							//���������⣬���͵�Դ�ı���
							_XSocketDataPack *tempPacket = NULL;
							for(int i = 0;i < m_usersData.size();++ i)
							{
								if(m_usersData[i]->state == CLINET_STATE_CONNECT
									&& m_usersData[i]->userID != packet.dataPacket->fromID)
								{
									tempPacket = createMem<_XSocketDataPack>();
									tempPacket->fromID = packet.dataPacket->fromID;
									tempPacket->toID = m_usersData[i]->userID;
									tempPacket->dataType = packet.dataPacket->dataType;
									tempPacket->dataLen = packet.dataPacket->dataLen;
									tempPacket->data = createArrayMem<unsigned char>(tempPacket->dataLen);
									memcpy(tempPacket->data,packet.dataPacket->data,packet.dataPacket->dataLen);
									sendAData(tempPacket);
								}
									//sendAData(m_usersData[i]->userID,packet.dataPacket->data,packet.dataPacket->dataLen);
							}
							unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);	//����
							m_recvMutex.Lock();
							m_recvDeque.push_back(packet.dataPacket);
							m_recvMutex.Unlock();
						}
					}else
					{
						unlockData(packet.dataPacket->data,packet.dataPacket->dataLen);
						if(packet.dataPacket->dataType == SOCKET_DATA_TYPE_SYSTEM)
						{//������յ�����ϵͳ���ݵĻ������ﴦ��ϵͳ����
							systemDataproc(packet.dataPacket);
							XDELETE_ARRAY(packet.dataPacket->data);
							XDELETE(packet.dataPacket);
						}else
						{
							m_recvMutex.Lock();
							m_recvDeque.push_back(packet.dataPacket);
							m_recvMutex.Unlock();
						}
					}
				}
				//�����µ����ݼ�������
				packet.recvDataLen = 0;
				return getDataPacket(packet,buff + needLen,len - needLen);
			}else
			{//���ݰ�������
				memcpy(packet.dataPacket->data + (packet.recvDataLen - SOCKET_PACKET_HEAD_SIZE),buff,len);
				packet.recvDataLen += len;
				return true;
			}
		}else
		{//��ͷ��δ��������
			if(packet.recvDataLen + len >= SOCKET_PACKET_HEAD_SIZE)
			{//�����İ�ͷ
				int needLen = SOCKET_PACKET_HEAD_SIZE - packet.recvDataLen;
				memcpy(packet.headData + packet.recvDataLen,buff,needLen);
				packet.recvDataLen = SOCKET_PACKET_HEAD_SIZE;
				packet.dataPacket = createMem<_XSocketDataPack>();
				if(packet.dataPacket == NULL) return false;	//���ش���
				//���潫��ͷ���ݽ�������
				memcpy(&(packet.dataPacket->fromID),packet.headData,sizeof(int));
				memcpy(&(packet.dataPacket->toID),packet.headData + sizeof(int),sizeof(int));
				memcpy(&(packet.dataPacket->dataType),packet.headData + sizeof(int) * 2,sizeof(int));
				memcpy(&(packet.dataPacket->dataLen),packet.headData + sizeof(int) * 3,sizeof(int));
				packet.dataPacket->data = createArrayMem<unsigned char>(packet.dataPacket->dataLen);
				if(packet.dataPacket->data == NULL) return false;	//���ش���
				return getDataPacket(packet,buff + needLen,len - needLen);
			}else
			{//��ͷ��Ȼ������
				memcpy(packet.headData + packet.recvDataLen,buff,len);
				packet.recvDataLen += len;
				return true;
			}
		}
	}
	return true;
}
bool _XSocketEx::connectToServer()
{//ͨ��IP���ӵ�������
	if(m_netSocket == INVALID_SOCKET && !getANewSocket(m_netSocket)) return false;
	resetSocketAddr(m_netSocket);
	if(m_workMode == SOCKET_WORK_MODE_IP)
	{//ͨ��������IP���ӵ�������
		sockaddr_in addrRemote;
		memset(&addrRemote,0,sizeof(addrRemote));
		addrRemote.sin_addr.s_addr = inet_addr(m_serverIP.c_str());
		addrRemote.sin_family = AF_INET;
		addrRemote.sin_port = htons(m_serverPort);
		if(connect(m_netSocket,(const sockaddr*)&addrRemote,sizeof(sockaddr)) == SOCKET_ERROR) return false;	//��������ʧ��
	}else
	if(m_workMode == SOCKET_WORK_MODE_NAME)
	{//ͨ���������������ӵ�������
		if(m_conState == CON_STATE_NULL)
		{
			m_boardcastThreadState = STATE_BEFORE_START;
			if(CreateThread(0,0,boardcastThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
		}
		sendBoardcast();	//������㲥
		Sleep(1000);		//�ȴ���������Ӧ
		m_serverAddr.sin_family = AF_INET;
		m_serverAddr.sin_port = htons(m_serverPort);
		if(connect(m_netSocket,(const sockaddr*)&m_serverAddr,sizeof(sockaddr)) == SOCKET_ERROR)
		{
			waitThreadEnd(m_boardcastThreadState);
			return false;	//��������ʧ��
		}
	}

	//������Ҫ���������̣߳������߳�(��δ���)
	if(m_conState == CON_STATE_NULL)
	{//��һ�ν������ӣ�������Ҫ���������̺߳ͷ����߳�
		m_recvThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
		m_sendThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,sendThread,this,0,NULL) == 0)
		{
			waitThreadEnd(m_recvThreadState);
			return false;	//�����߳̽���ʧ��
		}
	}
	requestRename();//�ͻ������ӵ�������֮����Ҫ����������͸�������

	return true;
}
bool _XSocketEx::createServer()
{
	if(m_netSocket == INVALID_SOCKET && !getANewSocket(m_netSocket)) return false;
	resetSocketAddr(m_netSocket);
	int opt = SO_REUSEADDR;
	setsockopt(m_netSocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(opt));
	sockaddr_in addrRemote;
    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(m_serverPort);
	if(m_workMode == SOCKET_WORK_MODE_NAME) addrRemote.sin_addr.s_addr = htonl(INADDR_ANY); else
	if(m_workMode == SOCKET_WORK_MODE_IP) addrRemote.sin_addr.s_addr = inet_addr(m_serverIP.c_str());

	if(bind(m_netSocket,(sockaddr*)&addrRemote, sizeof(addrRemote)) < 0) return false;	//��
	if(listen(m_netSocket,SOMAXCONN) < 0) return false;	//����
	//����������ʱ���Լ����ӵ���Ա�б���
	if(m_usersInfo.size() <= 0)
	{
		_XSocketUserInfo *newInfo = createMem<_XSocketUserInfo>();
		if(newInfo == NULL) return false;
		newInfo->userID = m_myID;
		newInfo->userSocket = m_netSocket;
		//newInfo->userNameLen = sizeof(DEFAULT_CLIENT_NAME);
		newInfo->userNameLen = m_myName.size() + 1;
		newInfo->userName = createArrayMem<char>(newInfo->userNameLen);
		strcpy(newInfo->userName,m_myName.c_str());
		newInfo->userState = CLIENT_STATE_ONLINE;
		m_userInfoMutex.Lock();
		m_usersInfo.push_back(newInfo);
		m_userInfoMutex.Unlock();
	}
	//������Ҫ���������̣߳������̣߳������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
	m_acceptThreadState = STATE_BEFORE_START;
	if((m_acceptThreadHandle = CreateThread(0,0,acceptThread,this,0,NULL)) == 0)
	{//�����߳̽���ʧ��
		waitThreadEnd(m_sendThreadState);
		return false;
	}
	//�����㲥�߳�
	if(m_workMode == SOCKET_WORK_MODE_NAME)
	{
		m_boardcastThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,boardcastThread,this,0,NULL) == 0)
		{
			waitThreadEnd(m_recvThreadState);
			waitThreadEnd(m_sendThreadState);
			return false;	//�����߳̽���ʧ��
		}
	}
	return true;
}
bool _XSocketEx::connectToNet()	//���ӵ�����
{
	switch(m_conState)
	{
	case CON_STATE_NULL:	//��һ�ν�������
		if(!getANewSocket(m_netSocket)) return false;
		switch(m_socketRole)
		{
		case NET_SERVER://����Ƿ������Ͽ�����֮��ʲôҲ�����ȴ�����
			if(!createServer()) return false;
			m_conState = CON_STATE_NORMAL;
			break;
		case NET_NULL:
			//��������
			if(!connectToServer())
			{
				if(m_workMode == SOCKET_WORK_MODE_NAME || getMyIP() == m_serverIP)
				{//�������IP������IPһֱ���Խ���������
					closesocket(m_netSocket);
					m_netSocket = INVALID_SOCKET;
					if(createServer())
					{
						m_conState = CON_STATE_NORMAL;
						m_socketRole = NET_SERVER;
					}
				}
			}else
			{
				m_conState = CON_STATE_NORMAL;
				m_socketRole = NET_CLIENT;
			}
			break;
		case NET_CLIENT:
			if(!connectToServer()) return false;
			m_conState = CON_STATE_NORMAL;
			break;
		}
		break;
	case CON_STATE_NORMAL:
		return true;
		break;
	case CON_STATE_UNSTABLE:
	case CON_STATE_ERROR:
		switch(m_socketRole)
		{
		case NET_SERVER://����Ƿ������Ͽ�����֮��ʲôҲ�����ȴ�����
			return true;
			break;
		case NET_NULL:
		case NET_CLIENT:
			//����ǿͻ��˶Ͽ�����֮���������ӷ�����
			if(!getANewSocket(m_netSocket)) return false;
			//��������
			if(connectToServer())
			{
				m_conState = CON_STATE_NORMAL;
			}
			break;
		}
		break;
	}
	return false;
}
void _XSocketEx::release()
{
	if(!m_isInited) return;
	//������Ҫ�������е��߳�(��δ���)
	waitThreadEnd(m_connectThreadState);
	waitThreadEnd(m_recvThreadState);
	waitThreadEnd(m_sendThreadState);
	CloseHandle(m_acceptThreadHandle);
	//waitThreadEnd(m_acceptThreadState);	//����������
	waitThreadEnd(m_boardcastThreadState);
	if(m_socketRole == NET_SERVER)
	{//����Ƿ������˵Ļ�����Ҫ�ȴ����пͻ��˽����߳̽���
		for(int i = 0;i < m_usersData.size();++ i)
		{
			waitThreadEnd(m_usersData[i]->recvThreadState);
		}
	}
	
	closesocket(m_netSocket);
	m_netSocket = INVALID_SOCKET;
	WSACleanup(); //++
	//�����ͷŵ����е���Դ
	for(int i = 0;i < m_recvDeque.size();++ i)
	{
		XDELETE_ARRAY(m_recvDeque[i]->data);
		XDELETE(m_recvDeque[i]);
	}
	m_recvDeque.clear();
	for(int i = 0;i < m_sendDeque.size();++ i)
	{
		XDELETE_ARRAY(m_sendDeque[i]->data);
		XDELETE(m_sendDeque[i]);
	}
	m_sendDeque.clear();
	for(int i = 0;i < m_usersInfo.size();++ i)
	{
		XDELETE_ARRAY(m_usersInfo[i]->userName);
		XDELETE(m_usersInfo[i]);
	}
	m_usersInfo.clear();
	for(int i = 0;i < m_usersData.size();++ i)
	{
		XDELETE(m_usersData[i]);
	}
	m_usersData.clear();
	
	m_isInited = false;
}
bool _XSocketEx::setMyName(const std::string &myName)
{//ע�����ﲻ������������
	if(m_myName == myName) return false;
	if(m_socketRole == NET_NULL)
	{//��δ�������磬��ֱ�ӽ����޸�
		m_myName = myName;
		return true;
	}
	if(m_socketRole == NET_SERVER)
	{
		m_myName = myName;
		m_userInfoMutex.Lock();
		m_usersInfo[0]->userNameLen = m_myName.size() + 1;
		XDELETE_ARRAY(m_usersInfo[0]->userName);
		m_usersInfo[0]->userName = createArrayMem<char>(m_usersInfo[0]->userNameLen);
		strcpy(m_usersInfo[0]->userName,m_myName.c_str());
		m_userInfoMutex.Unlock();
		sendClientInfo();
	}
	if(m_socketRole == NET_CLIENT)
	{
		m_myName = myName;
		requestRename();
	}
	return true;
}
bool _XSocketEx::sendAData(int toID,unsigned char * data,int len)
{
	if(len <= 0 || data == NULL) return false;
	if(toID == m_myID) return false;
	_XSocketDataPack *tempPacket = createMem<_XSocketDataPack>();
	if(tempPacket == NULL) return false;
	tempPacket->fromID = m_myID;
	tempPacket->toID = toID;
	tempPacket->dataType = SOCKET_DATA_TYPE_CUSTOM;
	tempPacket->dataLen = len;
	tempPacket->data = createArrayMem<unsigned char>(len);
	if(tempPacket->data == NULL) return false;
	memcpy(tempPacket->data,data,len);
	lockData(tempPacket->data,len);
	sendAData(tempPacket);
	return true;
}
void _XSocketEx::requestRename()	//����������͸�������
{
	_XSocketDataPack *tempPacket = createMem<_XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = 0;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(_XSocketSysDataType) + m_myName.size() + 1;
	tempPacket->data = createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	_XSocketSysDataType temp = SYS_DATA_TYPE_SETNAME;
	memcpy(tempPacket->data,&temp,sizeof(_XSocketSysDataType));
	memcpy(tempPacket->data + sizeof(_XSocketSysDataType),m_myName.c_str(),m_myName.size() + 1);
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
}
void _XSocketEx::sendHeartbeat()
{//���������ź�
	_XSocketDataPack *tempPacket = createMem<_XSocketDataPack>();
	if(tempPacket == NULL) return;
	tempPacket->fromID = m_myID;
	tempPacket->toID = 0;
	tempPacket->dataType = SOCKET_DATA_TYPE_SYSTEM;
	tempPacket->dataLen = sizeof(_XSocketSysDataType);
	tempPacket->data = createArrayMem<unsigned char>(tempPacket->dataLen);
	if(tempPacket->data == NULL) return;
	_XSocketSysDataType temp = SYS_DATA_TYPE_HEARTBEAT;
	memcpy(tempPacket->data,&temp,tempPacket->dataLen);
	lockData(tempPacket->data,tempPacket->dataLen);
	sendAData(tempPacket);
}
void _XSocketEx::setOffline(int id)
{//����ĳ���ͻ�������
//	m_userDataMutex.Lock();
	for(int i = 0;i < m_usersData.size();++ i)
	{
		if(m_usersData[i]->userID == id)
		{
			m_usersData[i]->state = CLINET_STATE_DISCONNECT;
			break;
		}
	}
//	m_userDataMutex.Unlock();
	m_userInfoMutex.Lock();
	for(int i = 0;i < m_usersInfo.size();++ i)
	{
		if(m_usersInfo[i]->userID == id)
		{
			m_usersInfo[i]->userState = CLIENT_STATE_OFFLINE;
			break;
		}
	}
	m_userInfoMutex.Unlock();
	m_clientStateChange = true;
}
void _XSocketEx::heartbeatProc(int stepTime)
{//����ͻ��˵�����������Ӧ�Ĵ���
//	m_userDataMutex.Lock();
	for(int i = 0;i < m_usersData.size();++ i)
	{
		if(m_usersData[i]->state == CLINET_STATE_CONNECT)
		{
			m_usersData[i]->heartbeatTimer += stepTime;
			if(m_usersData[i]->heartbeatTimer >= 10000)
			{//�����쳣
				setOffline(m_usersData[i]->userID);
			}
		}
	}
//	m_userDataMutex.Unlock();
	if(m_clientStateChange)
	{
		m_clientStateChange = false;
		sendClientInfo();	//��ͻ��˷���״̬���
	}
}
bool _XSocketEx::sendBoardcast()
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
	if(sendSocket < 0) return false;
	resetSocketAddr(sendSocket); 
	if(setsockopt(sendSocket,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return false;
	if(bind(sendSocket,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//���ն˿�

    memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(BOARDCAST_PORT);
	addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
	//addrUDP.sin_addr.s_addr = inet_addr("192.168.1.255"); 

	//����׼����Ҫ���͵�����(��δ���)
	unsigned char needSendData[BOARDCAST_DATA_LEN];
	needSendData[0] = 0xcc;needSendData[1] = 0x01;offset = 2;
	len = m_serverName.size() + 1;
	memcpy(needSendData + offset,&len,sizeof(int));offset += sizeof(int);
	memcpy(needSendData + offset,m_serverName.c_str(),len);offset += len;
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
		return false;	//������㲥����
	}

	closesocket(sendSocket);
	return true;
}