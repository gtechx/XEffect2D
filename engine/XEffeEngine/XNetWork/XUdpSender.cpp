#include "XStdHead.h"
#include "XUdpSender.h"

namespace XE{
DWORD WINAPI XUdpSender::sendThread(void * pParam)		//�����߳�
{
	XUdpSender &pPar = *(XUdpSender *)pParam;
	pPar.m_threadState = STATE_START;
	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		pPar.sendAPack();	//����һ������
		Sleep(1);
	}
	pPar.m_threadState = STATE_END;
	return 1;
}
void XUdpSender::sendAPack()
{
	if(m_curPacks.size() <= 0) return;
	//��������
	m_mutex.Lock();
	XNetPack tmp = m_curPacks[0];
	m_curPacks.pop_front();
	m_mutex.Unlock();
	//���淢������
	if (m_autoPack)
	{
		//ʹ��2048�Ĵ�С���з��
		memcpy(&m_sendBuff[4], &m_packID, sizeof(unsigned int));
		memcpy(&m_sendBuff[8], &tmp.curDataLen, sizeof(int));
		_sendPack(tmp.data, tmp.curDataLen);
		++m_packID;
	}
	else
	{
		sendto(m_localSock, (char *)tmp.data, tmp.curDataLen, 0, (sockaddr *)&m_targetAddr, sizeof(m_targetAddr));
	}
	//�������ݰ�
	m_mutex.Lock();
	m_freePacks.push_back(tmp);
	m_mutex.Unlock();
}
void XUdpSender::_sendPack(unsigned char* data, int len)
{
	if (len <= m_recvBuffSize - m_packHeadSize)
	{
		memcpy(&m_sendBuff[m_packHeadSize], data, len);
		sendto(m_localSock, (char *)(&m_sendBuff[0]), len + m_packHeadSize, 0, (sockaddr *)&m_targetAddr, sizeof(m_targetAddr));
		Sleep(1);//��ֹ����ķ������ݶ���ɿ���
	}
	else
	{
		int size = m_recvBuffSize - m_packHeadSize;
		memcpy(&m_sendBuff[m_packHeadSize], data, size);
		sendto(m_localSock, (char *)(&m_sendBuff[0]), m_recvBuffSize, 0, (sockaddr *)&m_targetAddr, sizeof(m_targetAddr));
		_sendPack(data + size, len - size);
	}
}
bool XUdpSender::setup(const std::string&ip,int port,int myPort, bool autoPack, int id)	//��������
{
	if(m_isInited) return false;
	m_autoPack = autoPack;
	if (m_autoPack)
	{
		if (id < 0)
		{//����һ�������ID
			XSystemTime tmpT = XTime::getTimeMs();
			m_id = tmpT.hour;
			m_id = (m_id << 6) + tmpT.minute;
			m_id = (m_id << 6) + tmpT.second;
			m_id = (m_id << 10) + tmpT.millisecond;
			m_id = (m_id << 5) + XRand::random(32);
		}
		else
		{
			m_id = id;
		}
	}
	if(!getANewSocket(m_localSock, SOCK_DGRAM)) return false;

	int bBroadcast = 1;
	memset(&m_targetAddr,0,sizeof(m_targetAddr));
	m_targetAddr.sin_family = AF_INET;
	m_targetAddr.sin_port = htons(port);
	m_targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	sockaddr_in addrUDP;
	memset(&addrUDP,0,sizeof(addrUDP));
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(myPort);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
	if(setsockopt(m_localSock,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return false;
	if(bind(m_localSock,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//���Ͷ˿�
	m_sendBuff.resize(m_recvBuffSize);
	memcpy(&m_sendBuff[0], &m_id, sizeof(unsigned int));
	//���������߳�
	m_threadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0)
	{//�߳̽���ʧ��
		LogNull("���������߳�ʧ��!");
		return false;
	}

	m_isInited = true;
	return true;
}
void XUdpSender::close()	//�ر�����
{
	if(!m_isInited) return;
	waitThreadEnd(m_threadState);
	closesocket(m_localSock);
	//������Ҫ�ͷ����е��ڴ�ռ�
	m_mutex.Lock();
	for(auto it = m_freePacks.begin();it != m_freePacks.end();++ it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_freePacks.clear();
	for(auto it = m_curPacks.begin();it != m_curPacks.end();++ it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	m_isInited = false;
}
bool XUdpSender::sendData(const void *data,int len)	//��������
{
	if(data == NULL || len <= 0) return false;
	XNetPack tmp;
	m_mutex.Lock();
	if(m_freePacks.size() > 0)
	{
		tmp = m_freePacks[0];
		m_freePacks.pop_front();
	}
	m_mutex.Unlock();
	if(tmp.buffSize < len)
	{
		XMem::XDELETE_ARRAY(tmp.data);
		tmp.data = XMem::createArrayMem<unsigned char>(len);
		if(tmp.data == NULL)
		{
			LogNull("�ڴ����ʧ��!");
			return false;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data,data,len);
	tmp.curDataLen = len;
	m_mutex.Lock();
	m_curPacks.push_back(tmp);
	m_mutex.Unlock();
	return true;
}
}