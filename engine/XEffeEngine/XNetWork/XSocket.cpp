#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSocket.h"
#include "XSocketBasic.h"

#ifdef XEE_OS_LINUX
#include <sys/time.h>
#endif
namespace XE{
char XSocketMutualData[4] = {0x5a,0x6a,0x5a,0x6a};    //ע���������Ӳ����ֻ����4λ�������Ҫ�Ż��Ļ���������Ҫ�޸Ĵ���

XSocket::XSocket()
:dataProtectionFlag(XTrue)
,m_needMutual(XTrue)        //Ĭ������Ҫ���ֵ�
,m_flagClientThreadID(-1)
,m_WrongFlag(ERROR_TYPE_NULL)
{
#ifdef XEE_OS_LINUX
    signal(SIGPIPE,SIG_IGN);    //linux��send������ʧЧ��socket��������ʱ���ᵼ�£������˳���ͨ����������������������.
#endif
    m_clientConnectState = XMem::createArrayMem<char>(MAX_CLIENT);
    m_clientDelayTimes = XMem::createArrayMem<char>(MAX_CLIENT);
    m_clientSocket = XMem::createArrayMem<int>(MAX_CLIENT);
    m_getDataBuff = XMem::createArrayMem<XSocketData>(MAX_DATABUFF_DEEP);
    m_broadcastBuff = XMem::createArrayMem<XSocketData>(MAX_DATABUFF_DEEP);
    m_netFlags = XMem::createArrayMem<char>(MAX_NET_FLAGS);

    pthread_mutex_init(m_mutex,NULL);                        //��û�п��Ǻ�����������
    setHostID("Xiajia's PC");
}

XSocket::~XSocket()
{
	if(m_isServer == NET_STATE_SERVER)
	{
		m_serverExitFlag = 1;
	}else
	if(m_isServer == NET_STATE_CLIENT)
	{
		m_clientExitFlag = 1;
	}
	XEE::sleep(100000);
    XMem::XDELETE_ARRAY(m_clientConnectState);
    XMem::XDELETE_ARRAY(m_clientDelayTimes);
    XMem::XDELETE_ARRAY(m_clientSocket);
    XMem::XDELETE_ARRAY(m_getDataBuff);
    XMem::XDELETE_ARRAY(m_broadcastBuff);
    XMem::XDELETE_ARRAY(m_netFlags);
    closesocket(m_mySocket);
}

int XSocket::getDataCheckKey(int length,char *data) const
{
    char temp = 0;
    int i;
    for(i = 0;i < length;++ i)
    {
        temp += data[i];
    }
    return temp;
}

void XSocket::dataLockProc(int length,char *data,char lockKey)
{
    if(lockKey == 0) return;       //��ԿΪ����ֵʱ�������ܹ���
    if(length <= 0) return;        //���ݺ����Լ��
    if(data == NULL) return;
    int i = 0;
    unsigned char temp = lockKey;
    for(i = 0;i < length;++ i)
    {
        data[i] = temp ^ data[i];
        if(temp < 255) temp += 1;
        else temp = 0;
    }
}

int XSocket::dataUnlockProc(int length,char *data,char lockKey)
{
    if(lockKey == 0) return 1;    //��ԿΪ����ֵʱ�������ܹ���
    if(length <= 0) return 1;     //���ݺ����Լ��
    if(data == NULL) return 1;
   
    int i = 0;
    unsigned char temp = lockKey;
    for(i = 0;i < length;++ i)
    {
        data[i] = temp ^ data[i];
        if(temp < 255) temp += 1;
        else temp = 0;
    }
    return 1;
}

void XSocket::dataBale(XSocketData *temp)
{//���������ɵ���Ҫ����������У������ݼ���
    if(dataProtectionFlag) (*temp).lockKey = XRand::random(256);            //���������Կ
    else (*temp).lockKey = 0;
   
    (*temp).checkKey = getDataCheckKey((*temp).dataLength,(*temp).pData);     //����У������
    dataLockProc((*temp).dataLength,(*temp).pData,(*temp).lockKey);           //��������
}

int XSocket::dataRelieve(XSocketData *temp)
{//�������ݽ����Ƿ�ɹ�
    dataUnlockProc((*temp).dataLength,(*temp).pData,(*temp).lockKey);         //���ݽ���   
    (*temp).lockKey = 0;                                                     //���ݽ������֮���������Կ����ֹ�ظ�������ɵ�������
   
    char tempC = getDataCheckKey((*temp).dataLength,(*temp).pData);
    if(tempC != (*temp).checkKey) return 0;                                  //����У��ʧ��
    return 1;
}

int XSocket::createServer(int port)
{
    struct sockaddr_in serverAddr;                                           //��������ַ
    m_serverPort = port;

#ifdef XEE_OS_WINDOWS
    if(!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);              //��ȡ�������׽���
    if(m_serverSocket < 0)
    {
        addLogText(this,"�������׽�������ʧ��!\n","Server socket get fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_APPLY;
        return 0;
    }
   
    resetSocketAddr(m_serverSocket);
   
    memset(&serverAddr,0,sizeof(serverAddr));                                 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(m_serverPort);                                //���÷������˿�
   
    if(bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)  //�󶨷������׽���
    {
        addLogText(this,"�������׽��ְʧ��!\n","Server socket bind fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_BIND;
        return 0;
    }
    if(listen(m_serverSocket, SOMAXCONN) < 0)                                //������������
    {
        addLogText(this,"����������ʧ��!\n","Server listen fail.\n");
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_LISTEN;
        return 0;
    }
    //��ʼ������������
    m_isServer = NET_STATE_SERVER;                                                          //�Ƿ�Ϊ������ 0����Ч 1�������� 2���ͻ���
    m_myConnectState = 1;                                                    //�Ƿ����ӵ�����
    memset(m_clientConnectState,0,sizeof(char) * MAX_CLIENT);
    memset(m_clientDelayTimes,0,sizeof(char) * MAX_CLIENT);
    memset(m_clientSocket,0,sizeof(int) * MAX_CLIENT);

    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
    m_serverExitFlag = 0;    //��־������û���˳�
   
    clearDataBuff();         //�����Ϣ����
   
    //�����߳�
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverAcceptThreadP, NULL, &acceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,acceptThread,this,0,&m_serverAcceptThreadP) == 0)
#endif
    {//����������accept�߳�
        addLogText(this,"����Accept�߳�ʧ��!\n","Open accept thread error.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_ACCEPT_CREATE;
        return 0;
    }
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverBroadcastThreadP, NULL, &broadcastThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,broadcastThread,this,0,&m_serverBroadcastThreadP) == 0)
#endif
    {//����������Broadcast�߳�
        addLogText(this,"����Broadcast�߳�ʧ��!\n","Open broadcast thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_BROADCAST_CREATE;
        return 0;
    }
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        addLogText(this,"����Mutual�߳�ʧ��!\n","Open mutual thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE;
        return 0;
    }
    return 1;
}

void XSocket::distroyServer()
{
    m_serverExitFlag = 1;    //��־������û���˳�
	XEE::sleep(500000);

    m_isServer = 0;                                   
    m_myConnectState = 0;                                   
    for(int i =0;i < MAX_CLIENT;++ i)
    {
        m_clientConnectState[i] = 0;
        m_clientDelayTimes[i] = 0;
        m_clientSocket[i] = 0;
    }
    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
   
    closesocket(m_mySocket);
}

void XSocket::addAClient(int clientSocket)
{
    //����һ��û��ʹ�õĿռ�
    pthread_mutex_lock(m_mutex);
    for(int i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientConnectState[i] == 0)
        {//���ҵ��ռ�֮������Ӧ�Ĵ���
            //++++++++++++++++++++++++++++
            addLogText(this,"��ʼ�ȴ���ǣ�\n","Waiting for flag.\n");
            while(m_flagClientThreadID >= 0)
            {
                //if(m_flagClientThreadID < 0) break;
				XEE::sleep(1);
            }
            m_flagClientThreadID = i;
            addLogText(this,"�ȴ���ǽ��� over��\n","Waiting for flag over.\n");
            //----------------------------
            m_clientSocket[i] = clientSocket;
            m_clientDelayTimes[i] = 0;
            m_clientConnectState[i] = 1;
            ++m_clientSum;
            //����һ���߳�
            #ifdef XEE_OS_LINUX
                if(pthread_create(&(m_getDataThreadPOnServer), NULL, &(getDataThreadOnServer), this) != 0)
            #endif
            #ifdef XEE_OS_WINDOWS
                if(CreateThread(0,0,getDataThreadOnServer,this,0,&(m_getDataThreadPOnServer)) == 0)
            #endif
                {
                    addLogText(this,"���������ͻ������ݻ�ȡ�߳��߳�ʧ��!\n","Open data recv client thread fail.\n");
                    m_WrongFlag = ERROR_TYPE_THREAD_CLIENT_GETDATA_CREATE;
                    break;
                }
            break;
        }
    }

    sendServerState();            //��ͻ��˷��ͱ���ķ�����״̬��Ϣ
    pthread_mutex_unlock(m_mutex);
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::acceptThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::acceptThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    int clientSocket;

    addLogText(pPar,"Accept�߳̿���\n","Start Accept thread.\n");

    while(pPar.m_serverExitFlag == 0)
    {
        if(pPar.m_clientSum < MAX_CLIENT)                                    //���ӳɹ�֮������Ӧ�Ĵ���
        {
            clientSocket = accept(pPar.m_serverSocket, NULL, NULL);
            addLogText(pPar,"���µĿͻ��˼���\n","A new client join!\n");
            if(clientSocket < 0)
            {//���տͻ���ʧ��
                addLogText(pPar,"�ͻ��˼���ʧ��\n","Client join fail.\n");
                pPar.m_WrongFlag = ERROR_TYPE_ACCEPT_CLIENT;
            }else
            {//���տͻ��˳ɹ�֮����һ�������߳�
                pPar.addAClient(clientSocket);
            }
        }
        XEE::sleep(100);    //�߳���ʹ��˯��
    }
    addLogText(pPar,"accept�߳��˳�!\n","Accept thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

//��������Ż�Ϊ�����������Ľ��ܹ㲥�����ƻ�úܶ�
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::broadcastThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::broadcastThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    addLogText(pPar,"Broadcast�߳̿�ʼ\n","Start Broadcast thread.\n");
   
    int bBroadcast = 1;
    int hSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if(hSocket >= 0) pPar.resetSocketAddr(hSocket);
    XSocketData BCServerInfoData;
    BCServerInfoData.dataType = BROADCAST_DATA_TYPE_SERVER_INFO;
    BCServerInfoData.sourceSocket = 0;
    BCServerInfoData.targetSocket = 0;
    BCServerInfoData.lockKey = 0;
    BCServerInfoData.checkKey = 0;
    BCServerInfoData.dataLength = MAX_NET_FLAGS + sizeof(int);

    int BCServerInfoDataLength = DATA_HEAD_SIZE + BCServerInfoData.dataLength;
    char *broadcastServerInfo = XMem::createArrayMem<char>(BCServerInfoDataLength);
    if(broadcastServerInfo == NULL) return 0;

    memcpy(broadcastServerInfo,&BCServerInfoData,DATA_HEAD_SIZE);                                    //��������ͷ
    memcpy(broadcastServerInfo + DATA_HEAD_SIZE,&pPar.m_serverPort,sizeof(int));                    //����������
    memcpy(broadcastServerInfo + DATA_HEAD_SIZE + sizeof(int),pPar.m_netFlags,MAX_NET_FLAGS);    //����������
    char *broadcastUserData = NULL;	//�������ڹ㲥��ҵ�����

    if(hSocket >= 0 && setsockopt(hSocket, SOL_SOCKET, SO_BROADCAST, (char *) &bBroadcast, sizeof(bBroadcast)) <=0)
    {
        sockaddr_in addrUDP;
        memset(&addrUDP,0,sizeof(addrUDP));
        addrUDP.sin_family  = AF_INET;
        addrUDP.sin_port  = htons(0);
        addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(hSocket, (sockaddr*) &addrUDP, sizeof(addrUDP)) == 0)
        {
            addrUDP.sin_family = AF_INET;
            addrUDP.sin_port = htons(SERVER_PORT);
            addrUDP.sin_addr.s_addr = htonl(INADDR_BROADCAST);       
        //    addrUDP.sin_addr.s_addr = inet_addr("192.168.1.255");        //��OMAP3��,������Ҫ�������

            int time_temp = 100;
            while(true)
            {
                if(pPar.m_broadCastWay == BROADCAST_WAY_UDP)
                {//ʹ��UDP�㲥����ʽ
                    pthread_mutex_lock(pPar.m_mutex);
                    if(pPar.m_broadcastOldPoint != pPar.m_broadcastNewPoint)	//��Ҫ�ȵ��㲥����Ϊ�ղŹ㲥�ͻ������ӵ���Ϣ������ʵ�����ǲ���ȫ��
                    {//����Ϊ�գ����跢��
                        ++ pPar.m_broadcastOldPoint;
                        if(pPar.m_broadcastOldPoint >= BC_BUFF_DEEP) pPar.m_broadcastOldPoint = 0;
						broadcastUserData = XMem::createArrayMem<char>(DATA_HEAD_SIZE + pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength);
						if(broadcastUserData == NULL) return 0;
			
                        memcpy(broadcastUserData,&(pPar.m_broadcastBuff[pPar.m_broadcastOldPoint]),DATA_HEAD_SIZE);                                    //��������ͷ
                        memcpy(broadcastUserData + DATA_HEAD_SIZE,pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData,
                            pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength);    //����������
                        //Ȼ��ʼ����
                        test_error = sendto(hSocket,broadcastUserData,
                            pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength + DATA_HEAD_SIZE,0,(sockaddr *) &addrUDP,sizeof(addrUDP));
                        if(test_error < pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength + DATA_HEAD_SIZE)
                        {
                            addLogText(pPar,"���������ݹ㲥ʧ��!\n","Server send data turn error.\n");
                            //    if(test_error == -1)
                            //    {//�Է����������Ͽ�
                            addLogText(pPar,"����������ת������-!\n","server data guangbo error!");
                            //    }
                        }
                        //�������֮���ͷ�����
                        XMem::XDELETE_ARRAY(pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData);
                        pthread_mutex_unlock(pPar.m_mutex);

						XMem::XDELETE_ARRAY(broadcastUserData);
                        XEE::sleep(1);
                    }else
                    {   
                        pthread_mutex_unlock(pPar.m_mutex);
                        if(time_temp > 25)        //0.25s �㲥һ����Ϣ
                        {
                            time_temp = 0;
                            test_error = sendto(hSocket,broadcastServerInfo,BCServerInfoDataLength, 0, (sockaddr *) &addrUDP, sizeof(addrUDP));
                            if(test_error < MAX_NET_FLAGS + sizeof(int))
                            {
                                addLogText(pPar,"�������㲥�̹߳㲥����ʧ��\n","Server broadcast thread send data fail.\n");
                            }
                            if(test_error == -1)
                            {
                                addLogText(pPar,"�������㲥����!\n","Server Braodcast error!");
                            }
                        }
                        XEE::sleep(10000);
                        ++ time_temp;
                    }
                }else
                {
                    if(time_temp > 25)        //0.25s �㲥һ����Ϣ
                    {
                        time_temp = 0;
                        test_error = sendto(hSocket,broadcastServerInfo,BCServerInfoDataLength, 0, (sockaddr *) &addrUDP, sizeof(addrUDP));
                        if(test_error < MAX_NET_FLAGS + sizeof(int))
                        {
                            addLogText(pPar,"�������㲥�̹߳㲥����ʧ��\n","Server broadcast thread send data fail.\n");
                        }
                        if(test_error == -1)
                        {
                            addLogText(pPar,"�������㲥����!\n","Server Braodcast error!");
                        }
                    }
                    XEE::sleep(10000);
                    ++ time_temp;
                }
                if(pPar.m_serverExitFlag !=0)
                {
                    closesocket(hSocket);
                    break;
                }
            }
        }
    }
    XMem::XDELETE_ARRAY(broadcastServerInfo);
    addLogText(pPar,"Broadcast�߳̽���\n","Broadcast thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::createClient()
{   
    struct sockaddr_in clientAddr;

#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif

    m_mySocket = socket(AF_INET, SOCK_STREAM, 0);         //ȡ�÷������׽���
    if (m_mySocket < 0)
    {
        m_WrongFlag = ERROR_TYPE_CLIENT_SOCKET_APPLY;
        return 0;
    }
    resetSocketAddr(m_mySocket);

    memset(&clientAddr,0,sizeof(clientAddr));             
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(0);                       //���ÿͻ��˶˿�
    if(bind(m_mySocket, (sockaddr*) &clientAddr, sizeof (clientAddr)) < 0)        //�󶨿ͻ����׽���
    {
        m_WrongFlag = ERROR_TYPE_CLIENT_SOCKET_BIND;
        return 0;
    }
    //��ʼ����ص�����
    m_isServer = NET_STATE_CLIENT;                           
    m_myConnectState = 0;                   
    m_myDelayTimes = 0;                       
   
    m_clientExitFlag = 0;                   
    m_serverSocket = 0;                       
    m_serverPort = 0;                       
    memset(m_clientSocket,0,sizeof(m_clientSocket));
    m_clientSum = 0;
    m_serverID = 0;                           
    m_clientID = 0;
   
    clearDataBuff();    //�����Ϣ����
   
    return 1;
}
void XSocket::distroyClient()
{
    m_clientExitFlag = 1;
	XEE::sleep(500000);                   //�ȴ������ӣ��ȴ������߳���Դ�ͷ�       
    m_isServer = NET_STATE_NULL;
    closesocket(m_mySocket);
   
    m_myConnectState = 0;             //�Ƿ����ӵ�����
    m_myDelayTimes = 0;               //����ʧ�ܵĴ���
   
    m_clientExitFlag = 0;             //�ͻ����˳���־
    m_serverSocket = 0;                       
    m_serverPort = 0;                       
    memset(m_clientSocket,0,sizeof(m_clientSocket));
    m_clientSum = 0;
    m_serverID = 0;                           
    m_clientID = 0;
    clearDataBuff();                 //�����Ϣ����
}
int XSocket::connectToServer(sockaddr_in serverAddr)
{//���ӵ�������
#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 
    if(m_serverSocket < 0)
    {
        addLogText(this,"�������׽��ַ���ʧ��!\n","Server socket get error.\n");
        m_WrongFlag = ERROR_TYPE_CLIENT_S_SOCKET_APPLY;
        return 0;
    }
    resetSocketAddr(m_serverSocket);
       
    sockaddr_in addrRemote;

    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(m_serverPort);
    addrRemote.sin_addr.s_addr = serverAddr.sin_addr.s_addr;

    if(connect(m_serverSocket,(sockaddr*)&addrRemote,sizeof(addrRemote)) != 0)
    {
        addLogText(this,"���ӷ�����ʧ��\n","Connect server fail.\n");
        m_WrongFlag = ERROR_TYPE_CLIENT_CONNECT_SERVER;
        return 0;
    }
    //�������ݽ����߳�
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_getDataThreadPOnClient, NULL, &getDataThreadOnClient, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,getDataThreadOnClient,this,0,&m_getDataThreadPOnClient) == 0)
#endif
    {
        addLogText(this,"������������߳�ʧ��!\n","Open data recv thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_GETDATA_CREATE_N;
        return 0;
    }
    //���ӷ������ɹ�֮���������߳�
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        addLogText(this,"����mutual�߳�ʧ��!\n","Open mutual thread fail.\n");
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE_N;
        return 0;
    }
    return 1;
}

void XSocket::setHostID(const char *p)
{
    memset(m_netFlags,0,sizeof(m_netFlags));
    if(p == NULL) return;
    if(strlen(p) < MAX_NET_FLAGS)
    {//�����ڷ�Χ��ֱ�Ӹ���
        strcpy(m_netFlags,p);
    }else
    {//���ȳ�����Χ��Ҫ��ȡ
        memcpy(m_netFlags,p,(MAX_NET_FLAGS - 1) * sizeof(char));
        m_netFlags[MAX_NET_FLAGS - 1] = '\0';
    }
    addLogText(this,"���÷�����ID!\n","Set host ID.\n");
}

int XSocket::searchNet(const char *p)
{
    setHostID(p);
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,searchNetThread,this,0,&m_clientGetBroadcastThreadP) == 0)
#endif
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_clientGetBroadcastThreadP, NULL, &searchNetThread, (void*) this) != 0)
#endif
    {//�ͻ��˽��ܷ������Ĺ㲥�߳�
        m_WrongFlag = ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N;
        return 0;
    }
	XEE::sleep(500000);    //�ȴ�1s
   
//    int tempRandTime = random(3);    //����������������Ϊ�˷�ֹͬʱ����������������ʵ�����������Ѿ���������ԣ�����������ܻ��ʵ��䷴
//    sleep(tempRandTime * 500000)
   
    return m_myConnectState;
}

int XSocket::searchNet()
{
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,searchNetThread,this,0,&m_clientGetBroadcastThreadP) == 0)
#endif
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_clientGetBroadcastThreadP, NULL, &searchNetThread, (void*) this) != 0)
#endif
    {//�ͻ��˽��ܷ������Ĺ㲥�߳�
        m_WrongFlag = ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N;
        return 0;
    }
	XEE::sleep(500000);    //�ȴ�1s
    return m_myConnectState;
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::searchNetThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::searchNetThread(void * pParam)
#endif
{
    XSocket &pPar = *(XSocket *) pParam;
    addLogText(pPar,"�������������߳�\n","Start netsearch thread.\n");
   
    sockaddr_in addrUDP;
    memset(&addrUDP,0,sizeof(addrUDP));       
    XSocketData tempBroadCastData;
    tempBroadCastData.pData = XMem::createArrayMem<char>(BC_DATA_SIZE - DATA_HEAD_SIZE);
    if(tempBroadCastData.pData == NULL) return 0;//�ڴ����ʧ��

    addrUDP.sin_family = AF_INET;
    addrUDP.sin_port = htons(SERVER_PORT);
    addrUDP.sin_addr.s_addr = htonl(INADDR_ANY);
   
    int hSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(hSocket >= 0) pPar.resetSocketAddr(hSocket);
    if(hSocket >= 0 && bind(hSocket, (sockaddr*) &addrUDP, sizeof(addrUDP)) >= 0)
    {
    /*  int nTimeout = 100; // ���ó�ʱһ��
        if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &nTimeout, sizeof(int)) == 0)
        {
            addLogText(pPar,"���ճ�ʱ���óɹ�\n","Connect timeout set success\n");
        }*/

        sockaddr_in addrPeer;        //���ܷ��ĵ�ַ
#ifdef XEE_OS_WINDOWS
        int nSize = sizeof(addrPeer);
#endif
#ifdef XEE_OS_LINUX 
        socklen_t nSize = sizeof(addrPeer);
#endif
        int nRecvLength = 0;
        char szRecvBuffer[BC_DATA_SIZE];
		memset(szRecvBuffer,0,BC_DATA_SIZE);

        //int time_temp = 0;
        while(true)
        {
            if(pPar.m_myConnectState == 0)
            {//���û�����ӵ�����,�������������㲥
                addLogText(pPar,"flag X\n","flag X\n");
                //sleep(1);
            }else
            {//��������Ѿ����ӵ�����,�����ﲻ��Ҫ�ٽ�������㲥
                addLogText(pPar,"flag O\n","flag O\n");
                //sleep(500000);
                //continue;
            }
        //  addLogText(pPar,"�ͻ��˽��ܹ㲥����\n","cleint get broadcast data!\n");
			memset(szRecvBuffer,0,BC_DATA_SIZE);
            nRecvLength = recvfrom(hSocket, szRecvBuffer, BC_DATA_SIZE, 0, (sockaddr*)&addrPeer, &nSize);
            if(nRecvLength > 0)
            {
                //+++++++++++++++++++++++++++++++++++++++++++
                //������ȡ����
                memcpy(&tempBroadCastData,szRecvBuffer,DATA_HEAD_SIZE);
                memcpy(tempBroadCastData.pData,szRecvBuffer + DATA_HEAD_SIZE,tempBroadCastData.dataLength);
                //-------------------------------------------
                if(pPar.m_myConnectState == 0)    //����ͻ���û�����ӵ������������ߵ�һ�λ�÷�������Ϣ�����ﶼҪ������Ӧ������
                {//��δ��������
                    if(tempBroadCastData .dataType == BROADCAST_DATA_TYPE_SERVER_INFO)
                    {
                        memcpy(&pPar.m_serverPort,tempBroadCastData.pData,sizeof(pPar.m_serverPort));    //��÷��������Ӷ˿ں�
                       
                        if(strcmp(tempBroadCastData.pData + sizeof(int),pPar.m_netFlags) == 0)
                        {//�����־λ�����ȷ���������������

                            if(pPar.connectToServer(addrPeer) == 1)
                            {
                                pPar.m_myConnectState = 1;
                               
                                //ע���������ӳɹ�֮�󣬲�û�н�������̣߳�Ŀ�������������������֮���ٴ�����ʱ���������ӷ�����
                                //������ܻ��д�����
                            }else
                            {
                                pPar.m_myConnectState = 0;
                            }
                        }
                    }//����ֱ����������
                }else
                {//�Ѿ���������
                    if(tempBroadCastData .dataType == DATA_TYPE_BOARDCAST_DATA && pPar.m_broadCastWay == BROADCAST_WAY_UDP)
                    {//�յ��������˵Ĺ㲥�ź�
                        pthread_mutex_lock(pPar.m_mutex);
                        //pPar.test_data ++;
                        addLogText(pPar,"�ͻ����յ�������ת������!\n","Client get server turn data.\n");

                        ++ pPar.m_getDataNewP;
                        if(pPar.m_getDataNewP >= MAX_DATABUFF_DEEP) pPar.m_getDataNewP = 0;
                        if(pPar.m_getDataNewP == pPar.m_getDataOldP)
                        {//�����������ӵ��,����ɾ�����һ��û�д��������
                            ++ pPar.m_getDataOldP;
                            if(pPar.m_getDataOldP >= MAX_DATABUFF_DEEP) pPar.m_getDataOldP = 0;
                        }
                        XMem::XDELETE_ARRAY(pPar.m_getDataBuff[pPar.m_getDataNewP].pData);
                        //��������ͷ
                        memcpy(&(pPar.m_getDataBuff[pPar.m_getDataNewP]),&tempBroadCastData,DATA_HEAD_SIZE);
                        //����������
                        pPar.m_getDataBuff[pPar.m_getDataNewP].pData = XMem::createArrayMem<char>(tempBroadCastData.dataLength *sizeof(char));
                        if(pPar.m_getDataBuff[pPar.m_getDataNewP].pData == 0) return 0;

                        memcpy(pPar.m_getDataBuff[pPar.m_getDataNewP].pData,tempBroadCastData.pData,tempBroadCastData.dataLength); 

                        pthread_mutex_unlock(pPar.m_mutex);
                    }//����ֱ����������
                }
#if WITH_LOG
                {
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"�ҵ�����״̬:%d,dataType:%d\n",pPar.m_myConnectState,tempBroadCastData .dataType);
                    addLogText(pPar,tempLogTextData,tempLogTextData);
                }
#endif
                addLogText(pPar,"���յ��������Ĺ㲥����\n","Get server broadcast data.\n");
            }else
            {
                addLogText(pPar,"û�н��յ��������Ĺ㲥����\n","No server broadcast data.\n");
            }
            if(pPar.m_clientExitFlag != 0 || pPar.m_isServer == NET_STATE_SERVER)
            {//����Ƿ�����,��ֱ���˳�������������߳�
                closesocket(hSocket);
                break;
            }
			XEE::sleep(1);
        }
    }
    XMem::XDELETE_ARRAY(tempBroadCastData.pData);
    addLogText(pPar,"�˳����������߳�\n","Exit from search net thread.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::mutualThread(void * pParam)
#endif                       
#ifdef XEE_OS_LINUX
void *XSocket::mutualThread(void * pParam)
#endif                       
{
   XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    int i;
    addLogText(pPar,"Mutual�߳̿���!\n","Start Mutual thread.\n");

    XSocketData temp_data;
    temp_data.pData = XMem::createArrayMem<char>(4*sizeof(char));
    if(temp_data.pData == NULL) return 0;

    if(pPar.m_isServer == NET_STATE_SERVER)
    {//����������������Ч�Ŀͻ��˷��������ź�
    //  char tempMutualData[4];
        while(pPar.m_needMutual)
        {
            //if(!pPar.m_needMutual) break;    //�������Ϊ����Ҫ������Ϣ��ֱ���˳�����߳�
			XEE::sleep(1000000);
            if(pPar.m_clientSum > 0)
            {
                //for(int i = 0;i < pPar.m_clientSum;i ++)
                for(i = 0;i < MAX_CLIENT;++ i)
                {
                    if(i < 0) i =0;
                    if(pPar.m_clientConnectState[i] != 0)
                    {
                    //    if(pPar.m_clientDelayTimes[i] < (int)(MAX_DELAY_TIMES / 2.0))
                        {//����ӳ�ʱ��������Χ��,�򲻷����ӳ�����,ֻ���ӳټ���
                            ++ pPar.m_clientDelayTimes[i];
                            //����Ϊ��ʱ����
                            if(pPar.m_clientDelayTimes[i] >= MAX_DELAY_TIMES)
                            {//�пͻ��˵���
                                pthread_mutex_lock(pPar.m_mutex);
                                pPar.dropNetProc(i);
                                pthread_mutex_unlock(pPar.m_mutex);
                          
                                -- i;
                            }
                    //        continue;
                        }
                        temp_data.dataType = DATA_TYPE_MUTUAL;
                        temp_data.sourceSocket = pPar.m_mySocket;
                        temp_data.targetSocket = pPar.m_clientSocket[i];
                        temp_data.dataLength = sizeof(XSocketMutualData); 
                        memcpy(temp_data.pData,XSocketMutualData,sizeof(XSocketMutualData));

                        pPar.dataBale(&temp_data);
                   
                    //  addLogText(pPar,"������������������!\n","Server send mutual data.\n");
                        test_error = pPar.sendDataFunction(pPar.m_clientSocket[i],temp_data,temp_data.dataLength + DATA_HEAD_SIZE);
                        if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
                        {
#if WITH_LOG
                            {
                                char tempLogTextData[256];
                                sprintf(tempLogTextData,"�����źŷ���ʧ��!%d��%d\n",pPar.m_clientSocket[i],i);
                                addLogText(pPar,tempLogTextData,tempLogTextData);
                            }
#endif                         
                            addLogText(pPar,"�����źŷ���ʧ��!\n","Mutual data send error.\n");
                            //�Է����������Ͽ�
                        //    if(test_error == -1)
                        //    {
                                addLogText(pPar,"�����������������ݴ���!\n","Server send mutual error!\n");

                                pthread_mutex_lock(pPar.m_mutex);
                                pPar.dropNetProc(i);
                                pthread_mutex_unlock(pPar.m_mutex);
                           
                                -- i;
                        //    }
                        }else
                        {
#if WITH_LOG
                            {
                                char tempLogTextData[256];
                                sprintf(tempLogTextData,"Server send mutual succes!%d,%d\n",i,pPar.m_clientDelayTimes[i]);
                                addLogText(pPar,tempLogTextData,tempLogTextData);
                            }
#endif                       
                            pthread_mutex_lock(pPar.m_mutex);
                            ++ pPar.m_clientDelayTimes[i];
                            //����Ϊ��ʱ����
                            if(pPar.m_clientDelayTimes[i] >= MAX_DELAY_TIMES)
                            {//�пͻ��˵���
                                pPar.dropNetProc(i);
                                -- i;
                            }
                            pthread_mutex_unlock(pPar.m_mutex);
                            addLogText(pPar,"�������!\n","Opareator over!\n");
                        }
                    }
                }
            }
            if(pPar.m_serverExitFlag != 0)
            {
                break;
            }
        }
    }else
    if(pPar.m_isServer == NET_STATE_CLIENT)
    {//�ͻ���
        while(pPar.m_needMutual)
        {
            //if(!pPar.m_needMutual) break;    //�������Ϊ����Ҫ������Ϣ��ֱ���˳�����߳�
			XEE::sleep(1000000);
            temp_data.dataType = DATA_TYPE_MUTUAL;
            temp_data.sourceSocket = pPar.m_mySocket;
            temp_data.targetSocket = pPar.m_serverSocket;
            temp_data.dataLength = sizeof(XSocketMutualData); 
            memcpy(temp_data.pData,XSocketMutualData,sizeof(XSocketMutualData));
            pPar.dataBale(&temp_data);

            addLogText(pPar,"�ͻ��˷�����������!\n","Client send mutual data.\n");
       
            test_error = pPar.sendDataFunction(pPar.m_serverSocket,temp_data,temp_data.dataLength + DATA_HEAD_SIZE);

            if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(pPar,"�ͻ��������źŷ���ʧ��!\n","Client send mutual data error.\n");

            //  if(test_error == -1)
            //  {//�Է����������Ͽ�
                    addLogText(pPar,"�ͻ��˷��������źŴ���!\n","Client send mutual error!\n");

                    pPar.dropNetProc();
                    break;
            //  }
            }else
            {
                addLogText(pPar,"�ͻ��˷��������źųɹ�!\n","Client send mutual succes!\n");

                ++ pPar.m_myDelayTimes;
       
                //����Ϊ��ʱ����
                if(pPar.m_myDelayTimes >= MAX_DELAY_TIMES)
                {//����ǿͻ��˳�ʱ����ͻ���ȫ��ж�أ��ȴ���������
                    pthread_mutex_lock(pPar.m_mutex);
                    pPar.dropNetProc();
                    pthread_mutex_unlock(pPar.m_mutex);
                    break;
                }
            }           
            if(pPar.m_clientExitFlag != 0)
            {
                break;
            }
        }
    }
    XMem::XDELETE_ARRAY(temp_data.pData);
    addLogText(pPar,"Mutual�߳��˳�!\n","Mutual thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::getAMessageClient(XSocketData tempData)
{
    if(tempData.dataType == DATA_TYPE_USER_DATA || tempData.dataType == DATA_TYPE_BOARDCAST_DATA)
    {//�ͻ���Ϣ �������������ջ
        addLogText(this,"�ͻ����յ�������ת������!\n","Client get server turn data.\n");
       
        pthread_mutex_lock(m_mutex);
        if(pushAMessage(tempData) == 0)
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        pthread_mutex_unlock(m_mutex);
    }else
    if(tempData.dataType == DATA_TYPE_MUTUAL)
    {
        if(dataRelieve(&tempData) != 0)
        {
            addLogText(this,"�ͻ����յ��������ӳ�����!\n","Client get delay data.\n");
            if(tempData.pData[0] == XSocketMutualData[0] &&
                tempData.pData[1] == XSocketMutualData[1] &&
                tempData.pData[2] == XSocketMutualData[2] &&
                tempData.pData[3] == XSocketMutualData[3])
            {
                m_myDelayTimes = 0;    //�ܵ���������֮������ӳټ�¼
                addLogText(this,"�ͻ����յ���������\n","Client get mutual data.\n");
            }
        }else
        {
            addLogText(this,"�ͻ��������ݽ��ʧ��!\n","Client get data.Open fail.\n");
        }
    }else
    if(tempData.dataType == DATA_TYPE_SERVER_STATE)
    {//��������Ϣ
        if(dataRelieve(&tempData) != 0)
        {
            addLogText(this,"�ͻ����յ�������״̬��Ϣ!\n","Client get server state data.\n");
            pthread_mutex_lock(m_mutex);
           
            memcpy(&m_clientSum,&tempData.pData[0],sizeof(int));                    //��ȡ�ͻ�������
            memcpy(&m_serverID,&tempData.pData[sizeof(int)],sizeof(int));            //��ȡ������Socket
            memcpy(&m_clientID,&tempData.pData[2*sizeof(int)],sizeof(int));        //��ȡ������Socket
           
            for(int i = 0;i < m_clientSum -1;++ i)
            {//���ڿͻ�����˵��һ���ͻ���ʱ������ֻ�з�����
                memcpy(&m_clientSocket[i],&tempData.pData[3*sizeof(int) + i*sizeof(int)],sizeof(int));   
            }
            pthread_mutex_unlock(m_mutex);
            addLogText(this,"�ͻ��˵ķ�����״̬��Ϣ�������!\n","Client proc over for server state.\n");
        }else
        {
            addLogText(this,"�ͻ��������ݽ��ʧ��!\n","Client get data.Open fail.\n");
        }
    }else
    {
        addLogText(this,"�ͻ����յ�δ����ṹ����!\n","Client get undefine data.\n");
    }
    return 1;
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::getDataThreadOnClient(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::getDataThreadOnClient(void * pParam)
#endif
{
    XSocketData temp_data;
    //XSocketData temp_BC_data;
    XSocket &pPar = *(XSocket *) pParam;
    int test_error;
    addLogText(pPar,"�������ݽ����߳�!\n","Start data recv thread.\n");

    if(pPar.m_isServer != NET_STATE_CLIENT) return 0;    //ֻ�пͻ�����ʹ������߳�

    while(pPar.m_clientExitFlag == 0)
    {
        XEE::sleep(10);

        temp_data.pData = NULL;
        test_error = pPar.recvDataProc(pPar.m_serverSocket,&temp_data);

        if(test_error > 0)    //��������
        {//���ܵ����ݣ����ﴦ������
            addLogText(pPar,"�ͻ��˽��ܵ�����\n","Client get data.\n");
            if(pPar.getAMessageClient(temp_data) == 0) return 0;
        }else
        if(test_error == -1)
        {
            addLogText(pPar,"�ͻ��˽��մ���!\n","client recv error!\n");
        }
        XMem::XDELETE_ARRAY(temp_data.pData);
        
    //    if(pPar.m_clientExitFlag != 0)
    //    {
    //        break;
     //   }
    }
    addLogText(pPar,"���ݽ����߳̽���\n","Data recv thread exit.\n");
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::pushABroadcastMessage(XSocketData tempData)
{
    //�����ݷ���㲥�����У��Ա���֮�����ȫԱ�㲥
    ++ m_broadcastNewPoint;
    if(m_broadcastNewPoint >= BC_BUFF_DEEP) m_broadcastNewPoint = 0;
    if(m_broadcastNewPoint == m_broadcastOldPoint)
    {//��������ӵ�£���������ӵ�µ�����
        //�ͷ�����
        ++ m_broadcastOldPoint;
        if(m_broadcastOldPoint >= BC_BUFF_DEEP) m_broadcastOldPoint = 0;
    }
    //����������㲥����
    m_broadcastBuff[m_broadcastNewPoint].dataType = DATA_TYPE_BOARDCAST_DATA;
    m_broadcastBuff[m_broadcastNewPoint].sourceSocket = tempData.sourceSocket;

    m_broadcastBuff[m_broadcastNewPoint].targetSocket = 0;                    //�㲥��ʱ��Ĭ��Ŀ��SocketΪ0
    m_broadcastBuff[m_broadcastNewPoint].lockKey = tempData.lockKey;        //Ŀǰ�㲥���ݲ���Ҫ���ܺͽ���
    m_broadcastBuff[m_broadcastNewPoint].checkKey = tempData.checkKey;
    m_broadcastBuff[m_broadcastNewPoint].dataLength = tempData.dataLength;

    XMem::XDELETE_ARRAY(m_broadcastBuff[m_broadcastNewPoint].pData);

    m_broadcastBuff[m_broadcastNewPoint].pData = XMem::createArrayMem<char>(tempData.dataLength);        //�����ڴ�ռ�
    if(m_broadcastBuff[m_broadcastNewPoint].pData == NULL) return 0;    //ע��������һ���ǳ����صĴ��󣬽�����ɷǳ����ص�Ӱ��

    memcpy(m_broadcastBuff[m_broadcastNewPoint].pData,tempData.pData,tempData.dataLength);
    return 1;
}

int XSocket::pushAMessage(XSocketData tempData)
{
    //������������ն���
    ++ m_getDataNewP;
    if(m_getDataNewP >= MAX_DATABUFF_DEEP) m_getDataNewP = 0;
    if(m_getDataNewP == m_getDataOldP)
    {//�����������ӵ��,����ɾ�����һ��û�д��������
        ++ m_getDataOldP;
        if(m_getDataOldP >= MAX_DATABUFF_DEEP) m_getDataOldP = 0;
    }
    XMem::XDELETE_ARRAY(m_getDataBuff[m_getDataNewP].pData);
    //��������ͷ
    memcpy(&(m_getDataBuff[m_getDataNewP]),&tempData,DATA_HEAD_SIZE);
    //����������
    m_getDataBuff[m_getDataNewP].pData = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));
    if(m_getDataBuff[m_getDataNewP].pData == NULL) return 0;

    memcpy(m_getDataBuff[m_getDataNewP].pData,tempData.pData,tempData.dataLength);
    return 1;
}

int XSocket::getAMessageServer(XSocketData tempData,int clientID)
{
    if(tempData.dataType == DATA_TYPE_USER_DATA)
    {//�ͻ���Ϣ�������������ջ
        if(tempData.pData == NULL)
        {//���û��������Ҫ����Ļ���ֱ�ӷ���
            addLogText(this,"�ڴ�������!\n","server data memery error!");
            return 1;
        }       
        if(tempData.targetSocket == -1)
		{
            pthread_mutex_lock(m_mutex);
			if(pushABroadcastMessage(tempData) == 0) 
			{
				pthread_mutex_unlock(m_mutex);
				return 0;
			}
            pthread_mutex_unlock(m_mutex);
		}else
        if(tempData.targetSocket != m_serverSocket)
        {//���Ŀ�겻���Լ���ת��
            if(sendDataFunction(tempData.targetSocket,tempData,tempData.dataLength + DATA_HEAD_SIZE)
                < tempData.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"����ת��ʧ��!\n","Server send data turn error.\n");
            }
        }else
        {//���Ŀ��Ϊ�Լ�,�������������
            pthread_mutex_lock(m_mutex);
            if(pushAMessage(tempData) == 0)
            {
                pthread_mutex_unlock(m_mutex);
                return 0;
            }
            pthread_mutex_unlock(m_mutex);
        }
    }else
    if(tempData.dataType == DATA_TYPE_MUTUAL)
    {//��������
        if(dataRelieve(&tempData) != 0)
        {
            if(tempData.pData[0] == XSocketMutualData[0] &&
                tempData.pData[1] == XSocketMutualData[1] &&
                tempData.pData[2] == XSocketMutualData[2] &&
                tempData.pData[3] == XSocketMutualData[3])
            {
                m_clientDelayTimes[clientID] = 0;    //�յ���������֮������ӳټ�¼
                addLogText(this,"�յ���������,ID:","Server get mutual data,ID:");
#if WITH_LOG
                {
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"%d.\n",clientID);
                    addLogText(this,tempLogTextData,tempLogTextData);
                }
#endif
            }
        }else
        {
            addLogText(this,"�յ���������,���ʧ��!\n","Server get mutual data.Open it fail.\n");
        }
    }else
    if(tempData.dataType == DATA_TYPE_BOARDCAST_DATA)
    {//�յ����ǹ㲥����
        pthread_mutex_lock(m_mutex);
        //�����ݷ���㲥�����У��Ա���֮�����ȫԱ�㲥
        if(pushABroadcastMessage(tempData) == 0) 
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        //������������ն���
        if(pushAMessage(tempData) == 0)
        {
            pthread_mutex_unlock(m_mutex);
            return 0;
        }
        
        pthread_mutex_unlock(m_mutex);
    }else
    {//�Ƿ�����
        addLogText(this,"���������յ�δ֪�ṹ������!\n","Server get undefime data.\n");
    }
    return 1;
}

void XSocket::broadcastDataByTcpIP(XSocketData tempData)
{
    if(tempData.pData == NULL) return;    //���û��������Ҫ���͵Ļ�ֱ�ӷ���
    for(int i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientSocket[i] != 0 && m_clientSocket[i] != tempData.sourceSocket)
        {
            if(sendDataFunction(m_clientSocket[i],tempData,tempData.dataLength + DATA_HEAD_SIZE) < tempData.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"���������ݹ㲥ʧ��!\n","Server send data turn error.\n");
            }
			XEE::sleep(1);
        }
    }
}

#ifdef XEE_OS_WINDOWS
DWORD WINAPI XSocket::getDataThreadOnServer(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XSocket::getDataThreadOnServer(void * pParam)
#endif
{
    XSocketData tempData;
    XSocketData tempBCData;
    XSocket &pPar = *(XSocket *) pParam;
    int test_error = 0;
    int myClientID = 0;
    addLogText(pPar,"���������ݽ����߳�!\n","Start single data recv thread.\n");
   
    pthread_mutex_lock(pPar.m_mutex);
    if(pPar.m_flagClientThreadID < 0 || pPar.m_flagClientThreadID >= MAX_CLIENT)
    {
        addLogText(pPar,"���ͻ������̲߳������ݴ���!\n","Client data recv thread param error.\n");
    }
    myClientID = pPar.m_flagClientThreadID;
    pPar.m_flagClientThreadID = -1;    //ȥ�����
    pthread_mutex_unlock(pPar.m_mutex);

    //DSHOW("�������������߳�:%d\n",myClientID);

    if(pPar.m_isServer != NET_STATE_SERVER) return 0;//ֻ�з���������ʹ������߳�
    while(pPar.m_serverExitFlag == 0)
    {
		XEE::sleep(10);
        tempData.pData = NULL;
        if(myClientID >= 0 && myClientID < MAX_CLIENT)
        {
            test_error = pPar.recvDataProc(pPar.m_clientSocket[myClientID],&tempData);
            if(test_error > 0)    //��������
            {//���ܵ����ݣ����ﴦ������
                if(pPar.getAMessageServer(tempData,myClientID) == 0) return 0;
            }else
            if(test_error == 0)
            {//���û�н��ܵ�����
                if(pPar.m_broadCastWay == BROADCAST_WAY_TCPIP)
                {//ʹ��TCPIP��ʽ���й㲥
                    pthread_mutex_lock(pPar.m_mutex);
                    if(pPar.m_broadcastOldPoint != pPar.m_broadcastNewPoint)
                    {//����Ϊ�գ����跢��
                        ++ pPar.m_broadcastOldPoint;
                        if(pPar.m_broadcastOldPoint >= BC_BUFF_DEEP) pPar.m_broadcastOldPoint = 0;
                    /*    temp_BC_data.dataType = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataType;
                        temp_BC_data.checkKey = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].checkKey;
                        temp_BC_data.lockKey = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].lockKey;
                        temp_BC_data.sourceSocket = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].sourceSocket;
                        temp_BC_data.targetSocket = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].targetSocket;
                        temp_BC_data.dataLength = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].dataLength;
                        temp_BC_data.pData = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData;
                        */
                        tempBCData = pPar.m_broadcastBuff[pPar.m_broadcastOldPoint]; //����ʹ��Ĭ�ϸ�ֵ��������������ָ����������Ҫ�ر�С��
                        pPar.m_broadcastBuff[pPar.m_broadcastOldPoint].pData = NULL;
                        pthread_mutex_unlock(pPar.m_mutex);

                        //�����й㲥��ҷ�������
                        pPar.broadcastDataByTcpIP(tempBCData);

                        //�������֮���ͷ�����
                        XMem::XDELETE_ARRAY(tempBCData.pData);
                    }else
                    {
                        pthread_mutex_unlock(pPar.m_mutex);
                    }
                }
            }else
            if(test_error == -1)
            {
                addLogText(pPar,"���������մ���","server recv error!\n");
            }
            XMem::XDELETE_ARRAY(tempData.pData);
        }

        //+++++++++++++++++++++++++++++++++
        //�������ͻ����˳�,�������߳�Ҳ��Ҫ�˳�
        //---------------------------------
        if(pPar.m_clientConnectState[myClientID] == 0)
        {
            break;
        }

        //if(pPar.m_serverExitFlag != 0)
        //{
        //    break;
        //}
    }

    addLogText(pPar,"�����ݽ����߳̽���:","Single data recv thread exit.\n");
#if WITH_LOG
    {
        char tempLogTextData[256];
        sprintf(tempLogTextData,"%d.\n",myClientID);
        addLogText(pPar,tempLogTextData,tempLogTextData);
    }
#endif   
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}

int XSocket::sendDataToNet(int clientOrder,char *p,int length)
{
//  m_logFile.addLogInfo("��������");
    //addLogText(this,"�������ݷ��ͺ���!\n","Enter function of data send!\n");
    int testError = 0;
    XSocketData tempData;

	if(clientOrder < 0)
	{//�㲥
		if(length >= BC_DATA_SIZE - DATA_HEAD_SIZE) return 0;    //�����㲥������ֽ�����
	}
    if(m_isServer == NET_STATE_SERVER)
    {//������
		if(clientOrder >= MAX_CLIENT) return 0;
        if(clientOrder >= 0)
		{
			if(m_clientConnectState[clientOrder] == 0) return 0;
			tempData.dataType = DATA_TYPE_USER_DATA;
			tempData.sourceSocket = m_mySocket;
			tempData.targetSocket = m_clientSocket[clientOrder];
			tempData.dataLength = length;   
	       
			char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //�����ڴ�ռ����ݴ�����
			if(sendData == NULL) return 0;

			memcpy(sendData,p,length);
			tempData.pData = sendData;

			dataBale(&tempData);
			//addLogText(this,"--> ��������������!\n","--> Server send data!\n");
			testError = sendDataFunction(m_clientSocket[clientOrder],tempData,length + DATA_HEAD_SIZE);
	       
			XMem::XDELETE_ARRAY(sendData);//�������֮���ݴ�����ݿռ��ͷ�

			if(testError < length + DATA_HEAD_SIZE)
			{//���ݷ���ʧ��
				addLogText(this,"�������������ݴ���!\n","server send data error!");
				dropNetProc(clientOrder);
#if WITH_LOG
				{
					char tempLogTextData[256];
					sprintf(tempLogTextData,"Server data send error!%d��%d\n",m_clientSocket[clientOrder],clientOrder);
					addLogText(this,tempLogTextData,tempLogTextData);
				}
#endif
				return 0;
			}
		}else
		{//�㲥
			tempData.dataType = DATA_TYPE_BOARDCAST_DATA;
			tempData.sourceSocket = m_mySocket;
			tempData.targetSocket = -1;
			tempData.dataLength = length;   
	       
			char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //�����ڴ�ռ����ݴ�����
			if(sendData == NULL) return 0;

			memcpy(sendData,p,length);
			tempData.pData = sendData;

			dataBale(&tempData);
			pthread_mutex_lock(m_mutex);
			if(pushABroadcastMessage(tempData) == 0) 
			{
				pthread_mutex_unlock(m_mutex);
				return 0;
			}
			pthread_mutex_unlock(m_mutex);
			XMem::XDELETE_ARRAY(sendData);//�������֮���ݴ�����ݿռ��ͷ�
		}
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//�ͻ���
        if(clientOrder >= m_clientSum) return 0;
        if(clientOrder >= 0)
        {//TCP/IP ��Ե�����ݷ���
            tempData.dataType = DATA_TYPE_USER_DATA;
            tempData.sourceSocket = m_clientID;
            if(clientOrder == 0) tempData.targetSocket = m_serverID;
            else tempData.targetSocket = m_clientSocket[clientOrder -1];
        }else
        {//�㲥����
            tempData.dataType = DATA_TYPE_BOARDCAST_DATA;
            tempData.sourceSocket = m_clientID;
            tempData.targetSocket = -1;
        }
        tempData.dataLength = length;  

        char * sendData = XMem::createArrayMem<char>(length * sizeof(char));    //�����ڴ�ռ����ݴ�����
        if(sendData == NULL) return 0;

        memcpy(sendData,p,length);
        tempData.pData = sendData;                 

        dataBale(&tempData);
       
        //addLogText(this,"�ͻ��˷�������!\n","Client send data.\n");
        testError = sendDataFunction(m_serverSocket,tempData,length + DATA_HEAD_SIZE);

        XMem::XDELETE_ARRAY(sendData);//�������֮���ݴ�����ݿռ��ͷ�

        if(testError < length + DATA_HEAD_SIZE)
        {
            addLogText(this,"�ͻ��˷�������ʧ��!\n","Client send data error!\n");
            dropNetProc();
#if WITH_LOG
            {
                char tempLogTextData[256];
                sprintf(tempLogTextData,"�ͻ����û����ݷ���ʧ��!%d��%d\n",tempData.targetSocket,clientOrder);
                addLogText(this,tempLogTextData,tempLogTextData);
            }
#endif
            return 0;
        }
    }
    //addLogText(this,"�������ݷ��ͺ���!\n","Exit function of data send!\n");
    return 1;
}

int XSocket::popAMessage(XSocketData *tempData)
{
    pthread_mutex_lock(m_mutex);
    if(m_getDataOldP != m_getDataNewP)
    {
        ++ m_getDataOldP;
        if(m_getDataOldP >= MAX_DATABUFF_DEEP) m_getDataOldP = 0;
       
        //XSocketData temp_data;
        tempData->pData = NULL;
        memcpy(tempData,&m_getDataBuff[m_getDataOldP],DATA_HEAD_SIZE);                //��������ͷ
        tempData->pData = XMem::createArrayMem<char>(tempData->dataLength * sizeof(char));
        if(tempData->pData == NULL) return 0;

        if(m_getDataBuff[m_getDataOldP].pData == NULL) return 0;    //���û������ֱ�ӷ���

        memcpy(tempData->pData,m_getDataBuff[m_getDataOldP].pData,tempData->dataLength);
        XMem::XDELETE_ARRAY(m_getDataBuff[m_getDataOldP].pData);//ɾ���ݴ���ڴ�ռ�
        pthread_mutex_unlock(m_mutex);
        return 1;
    }else
    {
        pthread_mutex_unlock(m_mutex);
        return 0;
    }
}

int XSocket::getDataFromNet(int *clientOrder,int *clientID,char **p,int *length)
{
//  m_logFile.addLogInfo("��������");
    if(m_isServer == NET_STATE_SERVER)
    {//������
        XSocketData tempData;
        if(popAMessage(&tempData) == 0)
        {//û����Ϣ
            return 0;
        }else
        {//��ȡ��Ϣ�ɹ�
            if(dataRelieve(&tempData) != 0)
            {//���ݷ���Ҫ��֮�󣬽����ݴ���ȥ
                (*length) = tempData.dataLength;

                XMem::XDELETE_ARRAY((*p));//�ͷ�p�Ѿ��е���Դ

                (*p) = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));    //���¸�p������Դ
                if((*p) == NULL) 
                {
                    XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ
                    return 0;
                }

                memcpy((*p),tempData.pData,tempData.dataLength);    //�����ݿ�����ȥ
                XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ

                for(int i = 0;i < MAX_CLIENT;++ i)
                {
                    if(m_clientSocket[i] == tempData.sourceSocket && m_clientConnectState[i] != 0)
                    {
                        (* clientOrder) = i;
                        (* clientID) = tempData.sourceSocket;
                        return 1;
                    }
                }
#if WITH_LOG
                {//����ڿͻ��˵���֮ǰ����Ӧ�Ľ�������û�д����꣬����ͻ���������������Ĵ�������ֱ�ӽ����ݶ���
                    char tempLogTextData[256];
                    sprintf(tempLogTextData,"-->�ӻ����ж�ȡ����ʧ��!%d,%d,%d\n",m_clientSocket[0],tempData.sourceSocket,m_getDataOldP);
                    addLogText(this,tempLogTextData,tempLogTextData);
                }
#endif
                return 0;
            }else
            {//���ݽ��ʧ��
                XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ

                addLogText(this,"�������˻�����ݰ����������!\n","Server get data,Open data error!\n");
                return 0;
            }
        }
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//�ͻ���
        XSocketData tempData;
        if(popAMessage(&tempData) == 0)
        {//û����Ϣ
            return 0;
        }else
        {
            if(dataRelieve(&tempData) != 0)// || temp_data.dataType == DATA_TYPE_BOARDCAST_DATA)
            {
                (*length) = tempData.dataLength;
                XMem::XDELETE_ARRAY((*p));//�ͷ�p�Ѿ��е���Դ
               
                (*p) = XMem::createArrayMem<char>(tempData.dataLength * sizeof(char));    //���¸�p������Դ
                if((*p) == NULL) 
                {
                    XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ
                    return 0;
                }
                memcpy((*p),tempData.pData,tempData.dataLength);    //�����ݿ�����ȥ
                XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ

                if(tempData.sourceSocket == m_serverID)
                {
                    (* clientOrder) = 0;
                    (* clientID) = tempData.sourceSocket;
                    return 1;
                }
                //�ͻ����ǰ���˳�����еģ����Կ��Բ����޸�
                for(int i = 0;i < m_clientSum;++ i)
                {
                    if(m_clientSocket[i] == tempData.sourceSocket)
                    {
                        (* clientOrder) = i +1;
                        (* clientID) = tempData.sourceSocket;
                        return 1;
                    }
                }

                addLogText(this,"�ͻ��˻��������ݶ�ȡʧ��!\n","Client data buff get data error.\n");
                return 0;
            }else
            {//���ݽ��ʧ��
                XMem::XDELETE_ARRAY(tempData.pData);//ɾ���ݴ���Դ
                addLogText(this,"�ͻ��˻�����ݰ����������!\n","Client get data��Open data error.\n");
                return 0;
            }
        }
    }else
    {//�Ȳ��Ƿ�����Ҳ���ǿͻ���
        //addLogText(this,"���ص��������!\n","A error!\n");
        return 0;
    }
//    return 1;
}

//����Ϊ����ӿ�
int XSocket::connectToNet(int port,const char *serverID)
{
    setBroadCastWay(BROADCAST_WAY_UDP);
    setHostID(serverID);                        //��������ID
    if(createClient() == 0)
    {
        addLogText(this,"�����ͻ���ʧ��!\n","Create client error.\n");
        return 0;            //�����ͻ���
    }
    if(searchNet() == 0)                        //�������粢���ӷ�����
    {
        addLogText(this,"������û�з�����!\n","There is no server online\n");
        distroyClient();
       
        if(createServer(port) == 0)
        {
            addLogText(this,"����������ʧ��!\n","Create server error.\n");
            return 0;    //����������
        }
    }
    return 1;
}

int XSocket::xCreateServer(const char *serverIP,int port)
{
    setBroadCastWay(BROADCAST_WAY_TCPIP);
    struct sockaddr_in serverAddr;                                        //��������ַ
    m_serverPort = port;

#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        //��ȡ�������׽���

    if(m_serverSocket < 0)
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_APPLY;
        return 0;
    }
   
    resetSocketAddr(m_serverSocket);

    memset(&serverAddr,0,sizeof(serverAddr));                            
    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(m_serverPort);                            //���÷������˿�
   
    if(bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)        //�󶨷������׽���
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_BIND;
        return 0;
    }
    addLogText(this,"��������\n","Open listen thread.\n");
    if(listen(m_serverSocket, SOMAXCONN) < 0)                //������������
    {
        m_WrongFlag = ERROR_TYPE_SERVER_SOCKET_LISTEN;
        return 0;
    }
    //��ʼ������������
    m_isServer = NET_STATE_SERVER;                                        //�Ƿ�Ϊ������ 0����Ч 1�������� 2���ͻ���
    m_myConnectState = 1;                                    //�Ƿ����ӵ�����
    for(int i =0;i < MAX_CLIENT;++ i)
    {
        m_clientConnectState[i] = 0;
        m_clientDelayTimes[i] = 0;
        m_clientSocket[i] = 0;
    }
    m_myDelayTimes = 0;
    m_mySocket = m_serverSocket;
    m_clientSum = 0;
    m_serverExitFlag = 0;    //��־������û���˳�
   
    clearDataBuff();    //�����Ϣ����
   
    addLogText(this,"��ʼ�����߳�\n","Start open thread.\n");
    //�����߳�
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_serverAcceptThreadP, NULL, &acceptThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,acceptThread,this,0,&m_serverAcceptThreadP) == 0)
#endif
    {//����������accept�߳�
        m_WrongFlag = ERROR_TYPE_THREAD_ACCEPT_CREATE;
        return 0;
    }
    addLogText(this,"����accept�߳�\n","Open accept thread.\n");
/*    if(pthread_create(&m_serverBroadcastThreadP, NULL, &broadcastThread, (void*) this) != 0)
    {//����������Broadcast�߳�
        m_WrongFlag = ERROR_TYPE_THREAD_BROADCAST_CREATE;
        return 0;
    }
    addLogText(this,"����Broadcast�߳�\n","Open broadcast thread.\n");*/
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_mutualThreadP, NULL, &mutualThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,mutualThread,this,0,&m_mutualThreadP) == 0)
#endif
    {
        m_WrongFlag = ERROR_TYPE_THREAD_MUNUAL_CREATE;
        return 0;
    }
    addLogText(this,"����Mutual�߳�\n","Open mutual thread.\n");

    return 1;
}

int XSocket::xCreateClient(const char *serverIP,int serverPort)
{
    setBroadCastWay(BROADCAST_WAY_TCPIP);
   if(createClient() == 0) return 0;
    m_serverPort = serverPort;

    sockaddr_in addrRemote;
    memset(&addrRemote,0,sizeof(addrRemote));
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_port = htons(serverPort);
    addrRemote.sin_addr.s_addr = inet_addr(serverIP);
   
    if(connectToServer(addrRemote) == 0) return 0;
    return 1;
}

void XSocket::sendServerState()
{
	if(m_clientSum <= 0) return;
    XSocketData temp_data;
    char tempSeverStateData[(MAX_CLIENT + 3) * sizeof(int)];
    int test_error;
    int i,j,k;
    //for(int i = 0;i < m_clientSum;i ++)
    for(i = 0;i < MAX_CLIENT;++ i)
    {
        if(m_clientConnectState[i] != 0)
        {
        /*
#if WITH_LOG
            {//����ڿͻ��˵���֮ǰ����Ӧ�Ľ�������û�д����꣬����ͻ���������������Ĵ�������ֱ�ӽ����ݶ���
                char tempLogTextData[256];
                sprintf(tempLogTextData,"��������ͻ��� %d ���ͷ�����״̬��Ϣ!\n",i);
                addLogText(pPar,tempLogTextData,tempLogTextData);
            }
#endif    */
            temp_data.dataType = DATA_TYPE_SERVER_STATE;
            temp_data.sourceSocket = m_mySocket;
            temp_data.targetSocket = m_clientSocket[i];
            temp_data.dataLength = m_clientSum * sizeof(int) + 2 * sizeof(int);      
            memcpy(&tempSeverStateData[0],&m_clientSum,sizeof(int));
            memcpy(&tempSeverStateData[sizeof(int)],&m_mySocket,sizeof(int));   
               
            memcpy(&tempSeverStateData[2 * sizeof(int)],&m_clientSocket[i],sizeof(int));
            //3.5�汾�����ݷ�����Ҫ�޸�
            //for(int j = 0,k = 0;j < m_clientSum ;j ++)
            for(j = 0,k = 0;j < MAX_CLIENT;++ j)
            {
                if(m_clientSocket[j] != m_clientSocket[i] && m_clientConnectState[j] != 0)
                {//��������,��������
                    memcpy(&tempSeverStateData[3 * sizeof(int) + k * sizeof(int)],&m_clientSocket[j],sizeof(int));   
                    ++ k;   
                }
            }                   
            temp_data.pData = tempSeverStateData;
            dataBale(&temp_data);
           
            addLogText(this,"����������״̬��Ϣ\n","Server send state data.\n");
            test_error = sendDataFunction(m_clientSocket[i],temp_data,temp_data.dataLength + DATA_HEAD_SIZE);
            if(test_error < temp_data.dataLength + DATA_HEAD_SIZE)
            {
                addLogText(this,"���������ݷ��ʹ���\n","Server data send error.\n");
            //    if(test_error == -1)
            //    {//�Է�����Ͽ�
                    addLogText(this,"���ͷ�����״̬���ݴ���!\n","Send server state error!\n");
                    dropNetProc(i);
                    -- i;
            //    }
            }
        }
    }
}

void XSocket::dropNetProc(int clientID)
{
    addLogText(this,"�����Ա�˳�,ID:","Connect member exit.ID:");
#if WITH_LOG
    {
        char tempLogTextData[256];
        sprintf(tempLogTextData,"%d\n",clientID);
        addLogText(this,tempLogTextData,tempLogTextData);
    }
#endif
    if(m_isServer == NET_STATE_SERVER)
    {//������
        if(clientID < 0 || clientID >= MAX_CLIENT) return;
        //3.5�汾���ߴ����������
        m_clientSocket[clientID] = 0;
        m_clientDelayTimes[clientID] = 0;
        m_clientConnectState[clientID] = 0;
        -- m_clientSum;
		if(m_clientSum < 0) m_clientSum = 0;
      /*for(int j = clientID;j < m_clientSum -1;j++)
        {
            m_clientConnectState[j] = m_clientConnectState[j +1];
            m_clientDelayTimes[j] = m_clientDelayTimes[j +1];
            m_clientSocket[j] = m_clientSocket[j +1];
        }
        m_clientConnectState[m_clientSum -1] = 0;
        -- m_clientSum;*/
        sendServerState();
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//�ͻ���
        distroyClient();
    }
}
int XSocket::recvDataFunction(int sock,char *temp_data,int length)
{
//    addLogText(this,"�������ݽ��պ���!\n","Enter data recv proc.\n");
    if(sock < 0) return 0;
    int test_error;
    {//����������
        fd_set readfds;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(sock,&readfds);
        test_error = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
        if(test_error >0 && FD_ISSET(sock,&readfds))
        {//���Խ�������
            FD_CLR(sock, &readfds);
            test_error = recv(sock,temp_data,length,0);
        }
    }
//    addLogText(this,"�˳����ݽ��պ���!\n","Exit data recv proc.\n");
    return test_error;
}

//˵��3.5�汾�޸�֮�����ݷ�����Ҫ�ȷ�������ͷ���֣�Ȼ���ٷ������ݲ��֡�
//int XSocket::sendDataFunction(int sock,char *temp_data,int length)
int XSocket::sendDataFunction(int sock,XSocketData temp_data,int length)
{
    addLogText(this,"�������ݷ��ͺ���!\n","Enter data send proc.\n");
   
    if(sock < 0) return 0;
    int test_error = 0;
    char *sendData = XMem::createArrayMem<char>((DATA_HEAD_SIZE + temp_data.dataLength) * sizeof(char));    //Ϊ���ݷ��ʹ����洢�ռ�
    if(sendData == NULL) return 0;

    memcpy(sendData,&temp_data,DATA_HEAD_SIZE);                                    //��������ͷ
    memcpy(sendData + DATA_HEAD_SIZE,temp_data.pData,temp_data.dataLength);        //����������
   
    {//����������
        fd_set writefds;
        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1;
        FD_ZERO(&writefds);
        FD_SET(sock,&writefds);
        test_error = select(FD_SETSIZE,NULL,&writefds,NULL,&timeout);
        if(test_error > 0 && FD_ISSET(sock,&writefds))
        {//���Է�������
            FD_CLR(sock, &writefds);
            test_error = send(sock,sendData,length,0);
        }
    }
    //test_error = send(sock,sendData,length,0);
    XMem::XDELETE_ARRAY(sendData);
    //addLogText(this,"�˳����ݷ��ͺ���!\n","Exit data send proc.\n");
    return test_error;
}

int XSocket::recvDataProc(int sock,XSocketData *temp_data)
{
    if(sock < 0) return 0;
    int test_error = 0;
    test_error = recvDataFunction(sock,(char *)temp_data,DATA_HEAD_SIZE);
    if(test_error == 0) return 0;
    if(test_error > 0 && test_error < DATA_HEAD_SIZE)
    {//��ȡ��������ͷ,ѭ����ȡ����
    //  addLogText(this,"�յ���������ͷ!\n","Get a part of head data.\n");
        int test_getSum;
        char test_data[DATA_HEAD_SIZE];
        while(true)
        {
            test_getSum = recvDataFunction(sock,test_data,DATA_HEAD_SIZE - test_error);
            if(test_getSum == -1)
            {//ͨѶ����
                test_error = -1;
                break;
            }
            if(test_getSum > 0)
            {
                memcpy((unsigned char *)(temp_data) + test_error,test_data,test_getSum);
                test_error += test_getSum;
                if(test_error >= DATA_HEAD_SIZE) break;
            }
        }
    }
    if(test_error == -1) return -1;            //���ݴ���
    if(test_error >= DATA_HEAD_SIZE)
    {//��ȡ������ͷ֮��,��ȡ���������ݲ���
        /*
#if WITH_LOG
        {
            char tempLogTextData[256];
            sprintf(tempLogTextData,"���ܵ�������ͷ,%d\n",temp_data->dataLength);
            addLogText(this,tempLogTextData,tempLogTextData);
        }
#endif    */
        //�����㹻���ڴ�ռ�������������
        temp_data->pData = XMem::createArrayMem<char>(temp_data->dataLength * sizeof(char));    //�����ڴ�ռ�
        if(temp_data->pData == NULL) return 0;

        test_error = recvDataFunction(sock,temp_data->pData,temp_data->dataLength);
        if(test_error > 0 && test_error < temp_data->dataLength)
        {
            char *test_data = XMem::createArrayMem<char>(temp_data->dataLength * sizeof(char));
            if(test_data == NULL) return 0;

            int test_getSum;
            while(true)
            {
                test_getSum = recvDataFunction(sock,test_data, temp_data->dataLength - test_error);
                if(test_getSum == -1)
                {//ͨѶ����
                    test_error = -1;
                    break;
                }
                if(test_getSum > 0)
                {
                    memcpy(temp_data->pData + test_error,test_data,test_getSum);
                    test_error += test_getSum;
                    if(test_error >= temp_data->dataLength) break;
                }
            }
            XMem::XDELETE_ARRAY(test_data);
        }
        if(test_error == -1)
        {
            XMem::XDELETE_ARRAY(temp_data->pData);
            return -1;
        }
        if(test_error >= temp_data->dataLength)
        {//���ݶ�ȡ���lockKey
            /*
#if WITH_LOG
            {
                char tempLogTextData[256];
                sprintf(tempLogTextData,"һ�����ݰ���ȡ���,��������Ϊ:%d,���ݳ���Ϊ:%d,%d,%d\n",temp_data->dataType,temp_data->dataLength,temp_data->lockKey,temp_data->checkKey);
                addLogText(this,tempLogTextData,tempLogTextData);
            }
#endif        */
            return  temp_data->dataLength + DATA_HEAD_SIZE;
        }
    }
    return 0;
}

void XSocket::clearDataBuff()
{
    //memset(m_getDataBuff,0,sizeof(m_getDataBuff));    //�����Ϣ��ջ
    int i = 0;
    for(i = 0;i < MAX_DATABUFF_DEEP;++ i)
    {
        m_getDataBuff[i].checkKey = 0;
        m_getDataBuff[i].dataLength = 0;
        m_getDataBuff[i].dataType = 0;
        m_getDataBuff[i].lockKey = 0;
        m_getDataBuff[i].sourceSocket = 0;
        m_getDataBuff[i].targetSocket = 0;
        XMem::XDELETE_ARRAY(m_getDataBuff[i].pData);
    }
    m_getDataNewP = 0;
    m_getDataOldP = 0;   

    //memset(m_broadcastBuff,0,sizeof(m_broadcastBuff));    //�����Ϣ��ջ
    for(i = 0;i < BC_BUFF_DEEP;++ i)
    {
        m_broadcastBuff[i].checkKey = 0;
        m_broadcastBuff[i].dataLength = 0;
        m_broadcastBuff[i].dataType = 0;
        m_broadcastBuff[i].lockKey = 0;
        m_broadcastBuff[i].sourceSocket = 0;
        m_broadcastBuff[i].targetSocket = 0;
        XMem::XDELETE_ARRAY(m_broadcastBuff[i].pData);
    }
    m_broadcastNewPoint = 0;
    m_broadcastOldPoint = 0;   
}
/*
int XSocket::broadcastData(char *pData,int length,int source)        //���͹㲥����
{
    //���ݺ����Լ��
    if(length <= 0) return 0;
    if(pData == NULL) return 0;
    if(length >= BC_DATA_SIZE - DATA_HEAD_SIZE) return 0;    //�����㲥������ֽ�����

    if(m_isServer == NET_STATE_SERVER)
    {//������ֱ�ӽ��������뷢�Ͷ���
        pthread_mutex_lock(&m_mutex);
        ++ m_broadcastNewPoint;
        if(m_broadcastNewPoint >= BC_BUFF_DEEP) m_broadcastNewPoint = 0;
        if(m_broadcastNewPoint == m_broadcastOldPoint)
        {//��������ӵ�£���������ӵ�µ�����
            ++ m_broadcastOldPoint;
            if(m_broadcastOldPoint >= BC_BUFF_DEEP) m_broadcastOldPoint = 0;
        }
        //����������㲥����
        m_broadcastBuff[m_broadcastNewPoint].dataType = DATA_TYPE_BOARDCAST_DATA;
        if(source == -1)
        {//�Լ���Դ
            m_broadcastBuff[m_broadcastNewPoint].sourceSocket = m_mySocket;
        }else
        {//������ת��
            m_broadcastBuff[m_broadcastNewPoint].sourceSocket = source;
        }
        m_broadcastBuff[m_broadcastNewPoint].targetSocket = 0;    //�㲥��ʱ��Ĭ��Ŀ��SocketΪ0
        m_broadcastBuff[m_broadcastNewPoint].lockKey = 0;        //Ŀǰ�㲥���ݲ���Ҫ���ܺͽ���
        m_broadcastBuff[m_broadcastNewPoint].checkKey = 0;
        m_broadcastBuff[m_broadcastNewPoint].dataLength = length;
        XMem::XDELETE_ARRAY(m_broadcastBuff[m_broadcastNewPoint].pData);
        m_broadcastBuff[m_broadcastNewPoint].pData = XMem::createArrayMem<char>(length);        //�����ڴ�ռ�
        if(m_broadcastBuff[m_broadcastNewPoint].pData == NULL) return 0;    //ע��������һ���ǳ����صĴ��󣬽�����ɷǳ����ص�Ӱ��
        memcpy(m_broadcastBuff[m_broadcastNewPoint].pData,pData,length);
        dataBale(&m_broadcastBuff[m_broadcastNewPoint]);
        pthread_mutex_unlock(&m_mutex);
    }else
    if(m_isServer == NET_STATE_CLIENT)
    {//�ͻ���ͨ��tcp/ip�����������͹㲥����
        if(sendDataToNet(-1,pData,length) == 0) return 0;
    }
    return 1;
}
*/
}