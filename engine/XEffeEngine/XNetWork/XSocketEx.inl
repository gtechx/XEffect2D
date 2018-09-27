INLINE bool XSocketEx::sendAData(XSocketDataPack * data)
{//ע����̻߳���,����Ĭ��Ϊ�����Ѿ���������
	if(data == NULL) return false;
	if(data->toID == m_myID)	//�Լ����͸��Լ�����Ϣֱ�Ӷ���
	{
		XMem::XDELETE_ARRAY(data->data);
		XMem::XDELETE(data);
		return false;
	}
	m_sendMutex.Lock();
	m_sendDeque.push_back(data);
	m_sendMutex.Unlock();
	return true;
}
INLINE bool XSocketEx::createNetWorkByName(const char *serverName,int port)		//ͨ�����������ƽ�������
{
	if(m_isInited ||	//�ظ�����
		serverName == NULL) return false;

	m_workMode = SOCKET_WORK_MODE_NAME;
	m_serverName = serverName;
	m_serverPort = port;
	m_conState = CON_STATE_NULL;
	//m_socketRole = NET_NULL;
	//���潨�������߳�
	m_connectThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
	//���潨�����ݴ����߳�(��δ���)

	m_isInited = true;
	return true;
}
INLINE bool XSocketEx::createNetWorkByIP(const char *serverIP,int port)		//ͨ��������IP��������
{//��δ���
	if(m_isInited ||	//�ظ�����
		serverIP == NULL) return false;

	m_workMode = SOCKET_WORK_MODE_IP;
	m_serverIP = serverIP;
	m_serverPort = port;
	m_conState = CON_STATE_NULL;
	//m_socketRole = NET_NULL;
	//���潨�������߳�
	m_connectThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
	//���潨�����ݴ����߳�(��δ���)

	m_isInited = true;
	return true;
}
INLINE bool XSocketEx::sendToAll(unsigned char * data,int len)
{
	switch(m_socketRole)
	{
	case NET_CLIENT: return sendAData(-1,data,len);
	case NET_SERVER://����Ƿ������������пͻ��˷�������
	//	m_userDataMutex.Lock();
		for(unsigned int i = 0;i < m_usersData.size();++ i)
		{
			if(m_usersData[i]->state == CLINET_STATE_CONNECT) 
				sendAData(m_usersData[i]->userID,data,len);
		}
	//	m_userDataMutex.Unlock();
		break;
	}
	return true;
}	
INLINE XSocketDataPack *XSocketEx::popAData()
{
	m_recvMutex.Lock();
	if(m_recvDeque.size() <= 0)
	{
		m_recvMutex.Unlock();
		return NULL;
	}
	XSocketDataPack *ret = m_recvDeque[0];
	m_recvDeque.pop_front();
	m_recvMutex.Unlock();
	return ret;
}
INLINE void XSocketEx::deleteAData(XSocketDataPack * p)
{
	XMem::XDELETE_ARRAY(p->data);
	XMem::XDELETE_ARRAY(p);
}
INLINE const XSocketUserInfo * XSocketEx::getUserInfo(unsigned int index)
{
	XSocketUserInfo * temp = NULL;
	m_userInfoMutex.Lock();
	if(index >= m_usersInfo.size())
	{
		m_userInfoMutex.Unlock();
		return NULL;
	}
	temp = m_usersInfo[index];
	m_userInfoMutex.Unlock();
	return temp;
}
INLINE void XSocketEx::setAsClient()
{ 
	if(m_isInited) return;
	m_socketRole = NET_CLIENT;
}
INLINE void XSocketEx::setAsServer()
{ 
	if(m_isInited) return;
	m_socketRole = NET_SERVER;
}