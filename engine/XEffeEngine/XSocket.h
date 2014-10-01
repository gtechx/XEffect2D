#ifndef _JIA_XSOCKET_
#define _JIA_XSOCKET_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:      2009.10.22
//--------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//˵����
//����ʱ�䣺    2009��10��22��
//�� �� Ա��    ��ʤ��
//�޸�ʱ�䣺    2009��10��31��
//���ļ�Ϊ����Linux�µ� Socket��������һ�����罻������
//ȱ��˵�������ݼ���ֻ�����ݲ��ֽ��У���������ݰ�ͷ���������ܣ��⽫
//�ᵼ�²���������Ϣй¶�����ǵ����ܵİ�ȫ�ԣ��Ժ�İ汾����Ҫ���ⲿ
//�ּ�������һ���Ĵ���
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�޸�ʱ�䣺    2009��12��30��
//�޸��ˣ�      ��ʤ��
//�޸����ݣ�    1���������ʹ�ú궨��ķ�ʽ(���)
//              2��ָ������Ĵ���ʹ��Ӧ�õķ�ʽ(δ���)
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�޸�ʱ�䣺    2010��12��23��
//�޸��ˣ�        ��ʤ��
//�޸����ݣ�    1�����ڳ��������ݰ������Զ����
//                2����ӹ㲥���ݵķ�ʽ
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���˵����
//�������Ľ����ͻ�������
//#define MAX_CLIENT    20
//
//�����壺
//�ֽ�
//0        Data Package Type        ��������    0����Ч�����ݰ� 1���������ݰ� 2���û��Լ���������ݰ� 3:������״̬��Ϣ
//1
//2
//3
//4        source                  Դsocket
//5
//6
//7
//8        target                  Ŀ��socket
//9
//10
//11
//12       length                  ������Ч����λ�ĳ���
//13       key                     ������Կ�ֽڣ���keyΪ0ʱ�����ݲ����м��ܲ��������ܺ�����ֱ�����������￼�ǵ��û��Լ������ݼ����Լ�����ִ��Ч��
//14       check data
//15
//��
//1023     data                    ���ݲ���       
//
//ע�⵽���ݽṹ���޶�������󳤶�ʵ����ֻ��MAXDATASIZE - 15�ĳ���
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�㲥���ݽṹ
//1��������ʹ�õĶ˿ں�
//2���������������־�ַ���
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//��չ��Ĺ㲥���ݽṹ
//0
//1
//2
//3        Data Type        ��������
//4       
//5
//6
//7        Data Length        ���ݳ���
//8
//9
//��
//1023    data            ���ݲ���
//2���������������־�ַ���
//------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//������״̬���ݽṹ����
//0
//1
//2
//3     ��Ч�ͻ�������
//4
//5
//6
//7     ������sock
//8
//9
//10   
//11    ���ͻ����ڷ�������ID
//12
//13
//14
//15    �����ͻ�������������ͻ���Sock
//16
//....
//------------------------------------------------------

#include "XOSDefine.h"
class _XSocket;

//#define DEBUG_FOR_CN        //��ʾ������־��Ϣ
//#define DEBUG_MODE_SHOW    //�Ƿ���ʾ��־��Ϣ

//�ϰ汾�ĵ���ģʽ
#ifdef DEBUG_MODE
#define DSHOW printf

inline void addLogText(_XSocket *p,char *pCN,char *pEN)
{
#ifdef DEBUG_FOR_CN
//	p->m_logFile.addLogInfo(pCN);
#ifdef DEBUG_MODE_SHOW
        DSHOW("%s",pCN);
#endif
#else
//	p->m_logFile.addLogInfo(pEN);
#ifdef DEBUG_MODE_SHOW
	DSHOW("%s",pEN);
#endif
#endif
}
#else
#define DSHOW (void)
#define addLogText (void)
#endif

#pragma comment(lib, "ws2_32.lib")

#ifdef XEE_OS_WINDOWS
#include "winsock.h"
#endif

#include "stdio.h"
#include "stdlib.h"
#include "XBasicFun.h"

#ifdef XEE_OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>        //inet_addr()
#include <unistd.h>           //usleep()
#include <fcntl.h>            //fcntl() ���÷���������
#include <signal.h>           //signal() ���ݷ������⴦��
#endif

//#ifdef DEBUG_MODE
//#include "XLogBook.h"       //��־ϵͳ��
//#endif

#include "XBasicClass.h"

#define MAX_CLIENT            2048         //�������������ӵ��ͻ��˵����ͻ�����Ŀ
#define MAX_DELAY_TIMES        10          //��������ʧ������
#define MAX_NET_FLAGS        20          //�������ַ����ĳ���
#define MAX_DATABUFF_DEEP    500         //���ݼĴ��ջ���
#define DATA_HEAD_SIZE        15          //���ݰ���ͷ��С
#define BC_DATA_SIZE        1024        //�㲥���ݰ��Ĵ�С
#define BC_BUFF_DEEP        128            //�㲥���ݶ������

#define SERVER_PORT       8019        //�������㲥�˿�

//TCP/IP�������ݴ�����������͵Ķ���
enum _XSocketDataType
{
    DATA_TYPE_NULL = 0,            //��Ч������
    DATA_TYPE_MUTUAL,                //������������
    DATA_TYPE_USER_DATA,            //�û��Լ�������
    DATA_TYPE_BOARDCAST_DATA,            //�㲥������
    DATA_TYPE_SERVER_STATE        //��������״̬����
};

#pragma pack(push)
#pragma pack(1)
struct _XSocketData
{
    char dataType;                               //��������   
    int sourceSocket;                            //Դ�׽���
    int targetSocket;                            //Ŀ���׽���
    int dataLength;                              //���ݳ���
    char lockKey;                                //������Կ
    char checkKey;                               //У������
    char *pData;                                 //���ݲ��ֵ�ָ��
    _XSocketData()
        :dataType(DATA_TYPE_NULL)
        ,sourceSocket(0)
        ,targetSocket(0)
        ,dataLength(0)
        ,lockKey(0)
        ,checkKey(0)
        ,pData(NULL)
    {
    }
/*	_XSocketData(const _XSocketData& temp)
	{
		dataType = temp.dataType;
		sourceSocket = temp.sourceSocket;
		targetSocket = temp.targetSocket;
		dataLength = temp.dataLength; 
		lockKey = temp.lockKey;	
		checkKey = temp.checkKey;	
		pData = createArrayMem<char>(dataLength);
		if(pData == NULL) 
		{
			dataLength = 0;			
			return;
		}
		memcpy(pData,temp.pData,dataLength);
	}*/
    ~_XSocketData()
    {//��̬�ڴ����������䣬��������ɾ������Ҫ��ɾ��
		//XDELETE_ARRAY(pData);
    }
/*	_XSocketData& operator = (const _XSocketData& temp)
	{
		if(&temp == this) return * this;
		dataType = temp.dataType;
		sourceSocket = temp.sourceSocket;
		targetSocket = temp.targetSocket;
		dataLength = temp.dataLength; 
		lockKey = temp.lockKey;	
		checkKey = temp.checkKey;	
		pData = createArrayMem<char>(dataLength);
		if(pData == NULL) 
		{
			dataLength = 0;			
			return * this;
		}
		memcpy(pData,temp.pData,dataLength);
		return * this;
	}*/
};
#pragma pack(pop)

enum
{
    ERROR_TYPE_NULL = 0,                        //0        û�д���
    ERROR_TYPE_SERVER_SOCKET_APPLY = 10000,        //10000    �������׽�������ʧ��
    ERROR_TYPE_SERVER_SOCKET_BIND,                //10001    �������׽��ְﶨʧ��
    ERROR_TYPE_SERVER_SOCKET_LISTEN,            //10002    ����������ʧ��
    ERROR_TYPE_ACCEPT_CLIENT,                    //10003    ���������ܿͻ��˵���������ʧ��
    ERROR_TYPE_THREAD_ACCEPT_CREATE,            //10004    ������accept�߳̿���ʧ��
    ERROR_TYPE_CLIENT_SOCKET_APPLY,                //10005    �ͻ����׽�������ʧ��
    ERROR_TYPE_CLIENT_SOCKET_BIND,                //10006    �ͻ����׽��ְﶨʧ��
    ERROR_TYPE_THREAD_BROADCAST_CREATE,            //10007    �����������㲥�̳߳���
    ERROR_TYPE_CLIENT_S_SOCKET_APPLY,            //10008    �ͻ������ӷ�����ʧ��
    ERROR_TYPE_THREAD_GETBROADCAST_CREATE_N,    //10009    �ͻ��˽��ܷ������㲥�߳̿���ʧ��
    ERROR_TYPE_THREAD_GETDATA_CREATE_N,            //10010    �ͻ��������߳̿���ʧ��
    ERROR_TYPE_THREAD_MUNUAL_CREATE,            //10011    �������������߳̿���ʧ��
    ERROR_TYPE_THREAD_MUNUAL_CREATE_N,            //10012    �ͻ������ݽ����߳̿���ʧ��
    ERROR_TYPE_THREAD_CLIENT_GETDATA_CREATE,    //10013    �����������ݽ����߳̿���ʧ��
    ERROR_TYPE_CLIENT_CONNECT_SERVER            //10014    �ͻ������ӷ�����ʧ��   
};

enum
{
    NET_STATE_NULL = 0,        //��Ч
    NET_STATE_SERVER,        //������
    NET_STATE_CLIENT        //�ͻ���
};

enum
{
    BROADCAST_DATA_TYPE_NULL = 0,                //�㲥����
    BROADCAST_DATA_TYPE_SERVER_INFO,            //��������Ϣ
    BROADCAST_DATA_TYPE_CLIENT_DATA                //�ͻ�������
};

enum
{
    BROADCAST_WAY_TCPIP,
    BROADCAST_WAY_UDP
};

class _XSocket
{
//#ifdef DEBUG_MODE
//public:   
//    _XLogbook m_logFile;
//#endif
private:
    //state flag
    int m_isServer;                             //�Ƿ�Ϊ������ 0����Ч 1�������� 2���ͻ���
    int m_myConnectState;                       //�Ƿ����ӵ�����
    //char m_clientConnectState[MAX_CLIENT];       //�Ƿ����ӵ�����
    char *m_clientConnectState;                    //�Ƿ����ӵ�����
    int m_myDelayTimes;                         //����ʧ�ܵĴ���
    //char m_clientDelayTimes[MAX_CLIENT];         //�ͻ��˵�����ʧ�ܴ���
    char *m_clientDelayTimes;                    //�ͻ��˵�����ʧ�ܴ���

    _XBool m_needMutual;                            //�Ƿ��������ִ���(2010��12��15�գ��������Ϳͻ��˵�������ñ���һ��)
   
    int m_serverExitFlag;                       //�������˳���־
    int m_clientExitFlag;                       //�ͻ����˳���־
       
    _XBool dataProtectionFlag;                    //�Ƿ�ʹ�����ݱ�������
    //other member
    int m_mySocket;                             //����ʹ�õ�Socket
    int m_serverSocket;                         //��������Socket
    int m_serverPort;                           //������ʹ�õĶ˿ں�
    //int m_clientSocket[MAX_CLIENT];              //���пͻ��˵�Socket
    int *m_clientSocket;              //���пͻ��˵�Socket
    int m_clientSum;                            //��Ч�ͻ��˵���Ŀ   
   
    int m_serverID;                             //�����������Ա�ڷ������ϵ�Socket��Ϊ������ID����ô������ID���Ƿ������������Socket
    int m_clientID;                             //������Ϊ�ͻ���ʱ�������ID
    //�߳��ֱ�
    pthread_t m_serverAcceptThreadP;            //�������Ľ����߳�
    pthread_t m_serverBroadcastThreadP;         //�������Ĺ㲥�߳�
    pthread_t m_mutualThreadP;                  //�����߳�
    pthread_t m_getDataThreadPOnClient;                 //�������Ŀͻ������߳�
    pthread_t m_clientGetBroadcastThreadP;      //�ͻ��˽��ܹ㲥���߳�
    //������߳���Ϊ��3.5�汾���޸Ķ����Ƶ�
    pthread_t m_getDataThreadPOnServer;    //������ÿ����һ���ͻ��˼���,����һ�����߳�
    int m_flagClientThreadID;                //�������������������̴߳������ݣ������߳���Ҫ����Ǹ��˿�
   
    //char m_netFlags[MAX_NET_FLAGS];               //�������ַ������������ַ�����������
    char *m_netFlags;               //�������ַ������������ַ�����������
   
    //���ݻ���    �������Ǽ��������Ի���5������
    //_XSocketData m_getDataBuff[MAX_DATABUFF_DEEP];
    _XSocketData *m_getDataBuff;
    int m_getDataNewP;
    int m_getDataOldP;
    int pushAMessage(_XSocketData tempData);    //�����������һ����Ϣ
    int popAMessage(_XSocketData *tempData);    //�Ӷ�����ȡ��һ����Ϣ
   
    void clearDataBuff();
    pthread_mutex_t m_mutex;                        //���ݻ���

    int m_WrongFlag;                                //������   

    //++++++++++++++++++++++++++++++++++++++
    //������Ϊ��ʵ�ֹ㲥������ĳ�Ա�ͷ���
    int m_broadCastWay;        //�㲥ʹ�õķ���
    //_XSocketData m_broadcastBuff[BC_BUFF_DEEP];
    _XSocketData *m_broadcastBuff;				//�㲥�Ķ��У�������Ҫ�㲥�����ݶ�ͨ�����������������������ڷ������ϲ��У���Ҫ�㲥�����ݻ�������������У��ڹ㲥�����д������Щ����
    int m_broadcastNewPoint;
    int m_broadcastOldPoint;
    int pushABroadcastMessage(_XSocketData tempData);    //���������Ƿ�ɹ������ʧ�ܷ���0
public:
    void setBroadCastWay(int temp);
    //int broadcastData(char *pData,int length,int source = -1);        //���͹㲥����
    //--------------------------------------
  
public:        //�������Եļ�ӷ��ʷ���
    void setDataProtection(_XBool temp);
    void disNeedMutual();
    int getClientSum() const;
    int getMyDelay() const;
    int getWrongFlag() const;
    int getIsServer() const;
    //+++++++++++++++++++++++++++++++++++++++
    //2009��12��29��
    //����µ��ⲿ���ʽӿ�
    int getClientID(int order) const;
    int getIsOnline() const;
    //---------------------------------------
private:
    //������Ϊ�˼򻯴��������ĺ���
    void addAClient(int clientSocket);
    int getAMessageServer(_XSocketData tempData,int clientID);
    int getAMessageClient(_XSocketData tempData);
    void broadcastDataByTcpIP(_XSocketData tempData);

private:   
    int getDataCheckKey(int length,char *data) const;                  //�������У��λ
    void dataLockProc(int length,char *data,char lockKey);       //���ݼ��ܺ���
    int dataUnlockProc(int length,char *data,char lockKey);      //���ݽ��ܺ���
    void dataBale(_XSocketData *temp);                           //���ݴ������
    int dataRelieve(_XSocketData *temp);                         //���ݽ�����������ؽ���Ƿ�ɹ�

#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI acceptThread(void * pParam);             //��������accept�߳�
    static DWORD WINAPI broadcastThread(void * pParam);          //�������Ĺ㲥�߳�
    static DWORD WINAPI searchNetThread(void * pParam);          //���������߳�
    static DWORD WINAPI mutualThread(void * pParam);             //�����߳�
    static DWORD WINAPI getDataThreadOnClient(void * pParam);    //�ͻ����������ݽ����߳�
    //������߳�Ϊ3.5�汾����
    static DWORD WINAPI getDataThreadOnServer(void * pParam);      //������ÿ��⵽һ���ͻ�������һ�����߳�(Ŀǰ��������ʹ�õ������ַ�ʽ)
#endif

#ifdef XEE_OS_LINUX
    static void *acceptThread(void * pParam);                    //��������accept�߳�
    static void *broadcastThread(void * pParam);                 //�������Ĺ㲥�߳�
    static void *searchNetThread(void * pParam);                 //���������߳�
    static void *mutualThread(void * pParam);                    //�����߳�
    //static void *getDataThread(void * pParam);                   //�������ݽ����߳�
    //������߳�Ϊ3.5�汾����
    static void *getDataThreadOnClient(void * pParam);      //������ÿ��⵽һ���ͻ�������һ�����߳�
    static void *getDataThreadOnServer(void * pParam);
#endif

    void resetSocketAddr(int socket);                            //����socket���Ա�����
   
//public:    //Ϊ�˲��Եķ������ﶨ��Ϊ��������   
    int createServer(int port);                                  //����������������������֮����Ҫ��������̣߳������߳� �� ���������߳� �� Ӧ���߳�
    void distroyServer();                                        //ȡ����������
    int createClient();                                          //�����ͻ���
    void distroyClient();                                        //ȡ���ͻ���
    int connectToServer(sockaddr_in serverAddr);                 //�ͻ������ӵ�������
   
    void setHostID(const char *p);                               //���÷����������־��
   
    int searchNet(const char *p);                                //����ָ������
    int searchNet();
   
    void sendServerState();                                      //��������ͻ��˷��ͷ�����״̬��Ϣ
   
    void dropNetProc(int clientID = 0);                          //����ͨѶ������������߻��߿ͻ��˵���
   
    int recvDataProc(int sock,_XSocketData *temp_data);          //��ĳsocket��ȡ����
    int recvDataFunction(int sock,char *temp_data,int length);    //���·�װ���ݽ��պ�����ʹ��windows��socket������
    int sendDataFunction(int sock,_XSocketData temp_data,int length);    //���·�װ���ݷ��ͺ�����ʹ��windows��socket������
   
    //����Ϊ���׶���Ĺ����ӿڣ�һ�����ھ�������һ�����ڹ�����
public:
    int connectToNet(int port,const char *serverID);             //�Զ����ܵ����ӵ��������ķ�����
   
    int xCreateServer(const char *serverIP,int port);            //����������������
    int xCreateClient(const char *serverIP,int serverPort);      //�����������ͻ��˲����ӷ�����
   
	//����˵�� <0:�㲥��0 - n����ָ��������ϵ�û�
    int sendDataToNet(int clientOrder,char *p,int length);        //�����緢������
    int getDataFromNet(int *clientOrder,int *clientID,char **p,int *length);     //�������ȡ����

    void disconnect();        //�Ͽ�����
   
    int isRecvBuffEmpty() const;    //�Ƿ���ܶ���Ϊ��
    int isSendBuffEmptyBC() const;    //�Ƿ�㲥���Ͷ���Ϊ��
                   
    _XSocket();
    ~_XSocket();
};

inline void _XSocket::setBroadCastWay(int temp)
{
    if(temp != BROADCAST_WAY_TCPIP)
    {
        m_broadCastWay = BROADCAST_WAY_UDP;
    }else
    {
        m_broadCastWay = BROADCAST_WAY_TCPIP;
    }
}
inline int _XSocket::getClientSum() const
{
    return m_clientSum;
}
inline int _XSocket::getMyDelay() const
{
    return m_myDelayTimes;
}
inline int _XSocket::getWrongFlag() const
{
    return m_WrongFlag;
}
inline int _XSocket::getIsServer() const
{
    return m_isServer;
}
inline int _XSocket::getClientID(int order) const
{
    if(m_clientSum <= 0) return 0;
    if(order < 0 || order >= MAX_CLIENT) return 0;
    return m_clientSocket[order];
}
inline int _XSocket::getIsOnline() const
{
    if(m_isServer == NET_STATE_SERVER && m_serverExitFlag == 1) return 0;
    if(m_isServer == NET_STATE_CLIENT && m_clientExitFlag == 1) return 0;
    return 1;
}
inline void _XSocket::resetSocketAddr(int socket)                             //����socket���Ա�����
{
    int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
inline void _XSocket::disconnect()
{
    m_serverExitFlag =1;
    m_clientExitFlag =1;
    mySleep(500);
    closesocket(m_mySocket);
}
inline int _XSocket::isRecvBuffEmpty() const
{
    if(m_getDataBuff[m_getDataOldP].pData == NULL) return 1;
    else return 0;
}
inline int _XSocket::isSendBuffEmptyBC() const
{
    if(m_broadcastBuff[m_broadcastOldPoint].pData == NULL) return 1;
    else return 0;
}
inline void _XSocket::setDataProtection(_XBool temp)
{
    dataProtectionFlag = temp;
}
inline void _XSocket::disNeedMutual()
{
    m_needMutual = XFalse;
}

#endif
