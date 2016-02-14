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
	XUdpPack tmp = m_curPacks[0];
	m_curPacks.pop_front();
	m_mutex.Unlock();
	//���淢������
	sendto(m_localSock,(char *)tmp.data,tmp.curDataLen,0,(sockaddr *)&m_targetAddr,sizeof(m_targetAddr));
	//�������ݰ�
	m_mutex.Lock();
	m_freePacks.push_back(tmp);
	m_mutex.Unlock();
}
bool XUdpSender::setup(const std::string&ip,int port)	//��������
{
	if(m_isInited) return false;
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	int bBroadcast = 1;
	memset(&m_targetAddr,0,sizeof(m_targetAddr));
	m_targetAddr.sin_family = AF_INET;
	m_targetAddr.sin_port = htons(port);
	m_targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());

	sockaddr_in addrUDP;
	memset(&addrUDP,0,sizeof(addrUDP));
	addrUDP.sin_family = AF_INET;
	addrUDP.sin_port = htons(0);
	addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
	m_localSock = socket(AF_INET,SOCK_DGRAM,0);
	if(m_localSock < 0) return false;
	resetSocketAddr(m_localSock); 
	if(setsockopt(m_localSock,SOL_SOCKET,SO_BROADCAST,(char *)&bBroadcast,sizeof(bBroadcast)) == SOCKET_ERROR) return false;
	if(bind(m_localSock,(sockaddr*)&addrUDP,sizeof(addrUDP)) < 0) return false;	//���Ͷ˿�
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
	closesocket(m_localSock);
	waitThreadEnd(m_threadState);
	//������Ҫ�ͷ����е��ڴ�ռ�
	m_mutex.Lock();
	for(int i = 0;i < m_freePacks.size();++ i)
	{
		XMem::XDELETE_ARRAY(m_freePacks[i].data);
	}
	m_freePacks.clear();
	for(int i = 0;i < m_curPacks.size();++ i)
	{
		XMem::XDELETE_ARRAY(m_curPacks[i].data);
	}
	m_curPacks.clear();
	m_mutex.Unlock();
	m_isInited = false;
}
bool XUdpSender::sendData(const unsigned char *data,int len)	//��������
{
	if(data == NULL || len <= 0) return false;
	XUdpPack tmp;
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