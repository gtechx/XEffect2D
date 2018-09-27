#include "XStdHead.h"
#include "XUdpReceiver.h"
namespace XE{
void XUdpReceiver::_pushAPack(unsigned char *p, int len)	//ѹ��һ�����ݰ�
{
	if (len <= m_packHeadSize) return;	//�Ƿ�������
	int id = *(int *)p;
	int packID = *(int *)(p + 4);
	int size = *(int *)(p + 8);
	if (size == len - m_packHeadSize)
	{//�������ݰ����������ģ�����Ҫ���������һϵ�в���
		pushAPack(p + m_packHeadSize, len - m_packHeadSize);
		return;
	}
	int index = getIndexByID(id);
	if (index < 0)
	{//��Ҫ���
		m_clientsInfo.push_back(XClientInfo());
		XClientInfo &ref = m_clientsInfo[m_clientsInfo.size() - 1];
		ref.id = id;
		ref.packID = packID;
		ref.buff.resize(len - m_packHeadSize);
		memcpy(&ref.buff[0], p + m_packHeadSize, len - m_packHeadSize);
		if (ref.buff.size() == size)
		{//�����Ѿ���������
			pushAPack(&ref.buff[0],size);
			ref.buff.clear();
		}
	}
	else
	{
		XClientInfo &ref = m_clientsInfo[index];
		if (ref.buff.size() > 0 && ref.packID != packID)
		{//�����˶����¼�������������������
			LogStr("��������");
			ref.packID = packID;
			ref.buff.resize(len - m_packHeadSize);
			memcpy(&ref.buff[0], p + m_packHeadSize, len - m_packHeadSize);
		}
		else
		{//׷������
			ref.packID = packID;
			int tmpSize = ref.buff.size();
			ref.buff.resize(tmpSize + len - m_packHeadSize);
			memcpy(&ref.buff[tmpSize], p + m_packHeadSize, len - m_packHeadSize);
		}
		if (ref.buff.size() == size)
		{//�����Ѿ���������
			pushAPack(&ref.buff[0],size);
			ref.buff.clear();
		}
	}
}
void XUdpReceiver::pushAPack(unsigned char *p,int len)	//ѹ��һ�����ݰ�
{
	if(p == NULL || len <= 0) return;
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
			return;
		}
		tmp.buffSize = len;
	}
	memcpy(tmp.data,p,len);
	tmp.curDataLen = len;
	m_mutex.Lock();
	m_curPacks.push_back(tmp);
	m_mutex.Unlock();
}
bool XUdpReceiver::setup(int port, bool autoPack)	//����
{
	if(m_isInited) return false;
	m_autoPack = autoPack;
	//��������sock
	if (!getANewSocket(m_localSock, SOCK_DGRAM)) return false;
	sockaddr_in addrUDP;
	memset(&addrUDP,0,sizeof(addrUDP)); 
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(port);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(m_localSock,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//���ն˿�
	//���������߳�
	m_threadState = STATE_BEFORE_START;
	if(CreateThread(0,0,recvThread,this,0,NULL) == 0)
	{//�߳̽���ʧ��
		LogNull("���������߳�ʧ��!");
		return false;
	}

	m_isInited = true;
	return true;
}
void XUdpReceiver::close()			//�ر�
{
	if(!m_isInited) return;
	waitThreadEnd(m_threadState);
	closesocket(m_localSock);
	//������Ҫ�ͷ����е��ڴ�ռ�
	m_mutex.Lock();
	for (auto it = m_freePacks.begin(); it != m_freePacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_freePacks.clear();
	for (auto it = m_curPacks.begin(); it != m_curPacks.end(); ++it)
	{
		XMem::XDELETE_ARRAY(it->data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	m_isInited = false;
}
DWORD WINAPI XUdpReceiver::recvThread(void * pParam)		//�����߳�
{
	XUdpReceiver &pPar = *(XUdpReceiver *)pParam;
	int nRecvLength = 0;
	XBuffer recvBuff;
	recvBuff.fitSize(m_recvBuffSize);
	memset(recvBuff.getBuffer(), 0, m_recvBuffSize);
	//��ȡ�����С
	//int nRevBuf;
	//int nRevOpt = sizeof(nRevBuf);
	//getsockopt(pPar.m_localSock, SOL_SOCKET, SO_RCVBUF, (CHAR *)&nRevBuf, &nRevOpt);
	sockaddr_in addrPeer;
	int nSize = sizeof(addrPeer);
	//����������
	fd_set readfds;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	pPar.m_threadState = STATE_START;
	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		FD_ZERO(&readfds);
		FD_SET(pPar.m_localSock,&readfds);
		if (select(FD_SETSIZE, &readfds, NULL, NULL, &timeout) > 0 &&
			FD_ISSET(pPar.m_localSock, &readfds))
		{//���Խ�������
			FD_CLR(pPar.m_localSock, &readfds);
			if (pPar.m_autoPack)
			{
				nRecvLength = recvfrom(pPar.m_localSock, (char *)recvBuff.getBuffer(),
					recvBuff.getBuffSize(), 0, (sockaddr*)&addrPeer, &nSize);
				if (nRecvLength > 0)//���յ�����
					pPar._pushAPack(recvBuff.getBuffer(), nRecvLength);
			}
			else
			{
				while (true)
				{
					nRecvLength = recvfrom(pPar.m_localSock, (char *)recvBuff.getBuffer(),
						recvBuff.getBuffSize(), 0, (sockaddr*)&addrPeer, &nSize);
					if (nRecvLength > 0)//���յ�����
					{//�ɹ�����
						pPar.pushAPack(recvBuff.getBuffer(), nRecvLength);
						break;
					}
					if (nRecvLength < 0)
					{//�����С����
						recvBuff.fitSize(recvBuff.getBuffSize() * 2);
						continue;
					}
					LogStr("������ֹ");
					break;
				}
			}
		}
		//Sleep(1);
	}
	pPar.m_threadState = STATE_END;
	return 1;
}
bool XUdpReceiver::recoverAData(XNetPack &pack)	//����һ�����ݰ�
{
	m_mutex.Lock();
	//������Ҫ��������Ƿ��Ѿ�����
	for (auto it = m_freePacks.begin(); it != m_freePacks.end(); ++it)
	{
		if (it->data != pack.data) continue;
		m_mutex.Unlock();
		return false;
	}
	m_freePacks.push_back(pack);
	m_mutex.Unlock();
	return true;
}
}