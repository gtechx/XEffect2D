#include "XStdHead.h"
#include "XTcpClient.h"

namespace XE {
DWORD WINAPI XTcpClient::sendThread(void * pParam)		//�����߳�
{
	XTcpClient &ref = *(XTcpClient *)pParam;
	ref.m_sendThreadState = STATE_START;
	while (ref.m_sendThreadState != STATE_SET_TO_END)
	{
		ref.sendAPack();	//����һ������
		Sleep(1);
	}
	ref.m_sendThreadState = STATE_END;
	LogStr("m_sendThreadState over");
	return 1;
}
DWORD WINAPI XTcpClient::recvThread(void * pParam)		//�����߳�
{
	XTcpClient &ref = *(XTcpClient *)pParam;
	fd_set readfds;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	int ret;
	XBuffer recvBuff;
	recvBuff.fitSize(m_recvBuffSize);
	memset(recvBuff.getBuffer(), 0, m_recvBuffSize);
	ref.m_recvThreadState = STATE_START;
	while (ref.m_recvThreadState != STATE_SET_TO_END)
	{
		//����������
		FD_ZERO(&readfds);
		FD_SET(ref.m_localSock, &readfds);
		ret = select(FD_SETSIZE, &readfds, NULL, NULL, &timeout);
		if (ret > 0 && FD_ISSET(ref.m_localSock, &readfds))
		{//���Խ�������
			FD_CLR(ref.m_localSock, &readfds);
			ret = recv(ref.m_localSock, (char*)recvBuff.getBuffer(), recvBuff.getBuffSize(), 0);
			if (ret == SOCKET_ERROR)
			{//�ڴ�ռ䲻��
				if (!ref.m_isAutoPack)
					recvBuff.fitSize(recvBuff.getBuffSize() * 2);
				LogStr("XTcpClient::recvThread() error!");
				continue;
			}
			if (ret == 0)
			{
				LogStr("�������ӶϿ�!");
				continue;
			}
			if (ret > 0)
			{
				if (ref.m_isAutoPack)
					ref._recvAData(recvBuff.getBuffer(), ret);//���յ�����
				else
					ref.recvAData(recvBuff.getBuffer(), ret);//���յ�����
			}
		}
		Sleep(1);
	}
	ref.m_recvThreadState = STATE_END;
	LogStr("m_recvThreadState over");
	return 1;
}
void XTcpClient::recvAData(unsigned char* data, int len)
{
	if (data == NULL || len <= 0) return;
	XNetPack tmp;
	m_rmutex.Lock();
	if (m_rfreePacks.size() > 0)
	{
		tmp = m_rfreePacks[0];
		m_rfreePacks.pop_front();
	}
	m_rmutex.Unlock();
	if (tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if (tmp.data == NULL)
		{
			LogNull("�ڴ����ʧ��!");
			return;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data, data, len);
	tmp.curDataLen = len;
	m_rmutex.Lock();
	m_rcurPacks.push_back(tmp);
	m_rmutex.Unlock();
}
void XTcpClient::_recvAData(unsigned char* data, int len)
{
	if (len <= m_packHeadSize) return;	//�Ƿ�������
	int packID = *(int *)(data);
	int size = *(int *)(data + 4);
	if (size == len - m_packHeadSize)
	{//�������ݰ����������ģ�����Ҫ���������һϵ�в���
		recvAData(data + m_packHeadSize, len - m_packHeadSize);
		return;
	}

	if (m_recvBuff.size() > 0 && m_rPackID != packID)
	{//�����˶����¼�������������������
		LogStr("��������");
		m_rPackID = packID;
		m_recvBuff.resize(len - m_packHeadSize);
		memcpy(&m_recvBuff[0], data + m_packHeadSize, len - m_packHeadSize);
	}
	else
	{//׷������
		m_rPackID = packID;
		int tmpSize = m_recvBuff.size();
		m_recvBuff.resize(tmpSize + len - m_packHeadSize);
		memcpy(&m_recvBuff[tmpSize], data + m_packHeadSize, len - m_packHeadSize);
	}
	if (m_recvBuff.size() == size)
	{//�����Ѿ���������
		recvAData(&m_recvBuff[0], size);
		m_recvBuff.clear();
	}
}
void XTcpClient::sendAPack()
{
	if (m_curPacks.size() <= 0) return;
	//��������
	m_mutex.Lock();
	XNetPack tmp = m_curPacks[0];
	m_curPacks.pop_front();
	m_mutex.Unlock();
	//���淢������
	if (m_isAutoPack)
	{
		//ʹ��2048�Ĵ�С���з��
		memcpy(&m_sendBuff[0], &m_sPackID, sizeof(unsigned int));
		memcpy(&m_sendBuff[4], &tmp.curDataLen, sizeof(int));
		_sendData(tmp.data, tmp.curDataLen);
		++m_sPackID;
	}
	else
	{
		send(m_localSock, (char *)tmp.data, tmp.curDataLen, 0);
	}
	//�������ݰ�
	m_mutex.Lock();
	m_freePacks.push_back(tmp);
	m_mutex.Unlock();
}
void XTcpClient::_sendData(unsigned char* data, int len)
{
	if (len <= m_recvBuffSize - m_packHeadSize)
	{
		memcpy(&m_sendBuff[m_packHeadSize], data, len);
		send(m_localSock, (char *)(&m_sendBuff[0]), len + m_packHeadSize, 0);
		Sleep(1);//��ֹ����ķ������ݶ���ɿ���
	}
	else
	{
		int size = m_recvBuffSize - m_packHeadSize;
		memcpy(&m_sendBuff[m_packHeadSize], data, size);
		send(m_localSock, (char *)(&m_sendBuff[0]), m_recvBuffSize, 0);
		_sendData(data + size, len - size);
	}
}
bool XTcpClient::setup(const std::string&ip, int port, bool isAutoPack)	//��������
{
	if (m_isInited) return false;
	m_isAutoPack = isAutoPack;
	if (!getANewSocket(m_localSock)) return XFalse;

	if (m_isAutoPack)
	{
		m_sendBuff.resize(m_recvBuffSize);
		m_recvBuff.resize(m_recvBuffSize);
	}

	memset(&m_targetAddr, 0, sizeof(m_targetAddr));
	m_targetAddr.sin_family = AF_INET;
	m_targetAddr.sin_port = htons(port);
	m_targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	//���ӷ�����
	if (connect(m_localSock, (const sockaddr *)&m_targetAddr, sizeof(sockaddr)) == SOCKET_ERROR)
	//���������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, sendThread, this, 0, NULL) == 0)
	{//�߳̽���ʧ��
		LogNull("���������߳�ʧ��!");
		return false;
	}
	//���������߳�
	m_recvThreadState = STATE_BEFORE_START;
	if (CreateThread(0, 0, recvThread, this, 0, NULL) == 0)
	{//�߳̽���ʧ��
		LogNull("���������߳�ʧ��!");
		return false;
	}

	m_isInited = true;
	return true;
}
void XTcpClient::close()	//�ر�����
{
	if (!m_isInited) return;
	waitThreadEnd(m_sendThreadState);
	waitThreadEnd(m_recvThreadState);
	closesocket(m_localSock);
	//������Ҫ�ͷ����е��ڴ�ռ�
	m_mutex.Lock();
	for (auto it = m_freePacks.begin();it != m_freePacks.end();++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_freePacks.clear();
	for (auto it = m_curPacks.begin();it != m_curPacks.end();++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	//������Ҫ�ͷ����е��ڴ�ռ�
	m_rmutex.Lock();
	for (auto it = m_rfreePacks.begin();it != m_rfreePacks.end();++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_rfreePacks.clear();
	for (auto it = m_rcurPacks.begin();it != m_rcurPacks.end();++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_rcurPacks.clear();
	m_rmutex.Unlock();
	m_isInited = false;
}
bool XTcpClient::sendData(const void *data, int len)	//��������
{
	if (data == NULL || len <= 0) return false;
	XNetPack tmp;
	m_mutex.Lock();
	if (m_freePacks.size() > 0)
	{
		tmp = m_freePacks[0];
		m_freePacks.pop_front();
	}
	m_mutex.Unlock();
	if (tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if (tmp.data == NULL)
		{
			LogNull("�ڴ����ʧ��!");
			return false;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data, data, len);
	tmp.curDataLen = len;
	m_mutex.Lock();
	m_curPacks.push_back(tmp);
	m_mutex.Unlock();
	return true;
}
void XTcpClient::popARecvData(XNetPack &data)
{
	if (m_rcurPacks.size() <= 0) return;
	m_rmutex.Lock();
	data = m_rcurPacks[0];
	m_rcurPacks.pop_front();
	m_rmutex.Unlock();
}
bool XTcpClient::recoverAData(XNetPack &pack)	//����һ�����ݰ�
{
	m_rmutex.Lock();
	//������Ҫ��������Ƿ��Ѿ�����
	for (auto it = m_rfreePacks.begin(); it != m_rfreePacks.end(); ++it)
	{
		if (it->data != pack.data) continue;
		m_rmutex.Unlock();
		return false;
	}
	m_rfreePacks.push_back(pack);
	m_rmutex.Unlock();
	return true;
}
}