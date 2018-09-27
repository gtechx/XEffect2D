#ifndef _JIA_XSOCKETEX_
#define _JIA_XSOCKETEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.2.25
//--------------------------------
//����һ����ǿ�͵Ļ���socket���������ӵ��࣬���������������������������֧�ֶ�ͻ�������
//���ʹ�����Ӻ�ͨѶ������ƣ����Ӳ㸺�𱣳�����������Լ���������״̬
//ͨѶ�㸺��ͨѶ�Լ�����ͨѶ״̬��ͨѶ�㻹��������ͼ���ģ�顣
#include "XOSDefine.h"
#include <string>
#include <vector>
#include <deque>
#include "XCritical.h"
#include "XThread.h"
#include "XNetWork/XNetCommon.h"
//1����δ��ɶ����ݽ��м���
//2�����Ʋ�����Ψһ�ԣ�Ҳ���ǿ����������Ƿ������������ݾ���Ӧ����ȷ��
//3���������������ͻ��˵�����û�ж�ԭ�пͻ��˵����ݽ��д����������ۻ���Դ���ģ���Ҫ����Ӧ�ĵ��ߴ�����ơ�
namespace XE{
//��������״̬
enum XSocketConnectState
{
	CON_STATE_NULL,		//��δ��������
	CON_STATE_NORMAL,	//��������
	CON_STATE_UNSTABLE,	//���Ӳ��ȶ������ݶϿ�
	CON_STATE_ERROR,	//�������ӶϿ�
};
enum XSocketNetRole
{
	NET_NULL,		//��δ����
	NET_SERVER,		//���������
	NET_CLIENT,		//����ͻ���
};
enum XSocketMode
{
	SOCKET_LAN,		//������
	SOCKET_WAN,		//������
};
enum XSocketWorkMode
{
	SOCKET_WORK_MODE_IP,	//ͨ��IP�ķ�ʽ��������
	SOCKET_WORK_MODE_NAME,	//ͨ�����������Ƶķ�ʽ��������
};
enum XSocketSysDataType
{//ϵͳ���ݵ�����
	SYS_DATA_TYPE_SETNAME,		//��������
	SYS_DATA_TYPE_SETID,		//����ID
	SYS_DATA_TYPE_UPDATEINIFO,	//������Ϣ
	SYS_DATA_TYPE_HEARTBEAT,	//�����ź�
};
enum XSocketExDataType
{
	SOCKET_DATA_TYPE_SYSTEM,	//ϵͳ����
	SOCKET_DATA_TYPE_CUSTOM,	//��������
};
#define SOCKET_PACKET_HEAD_SIZE (16)	//���ݰ��İ�ͷ��С
#define DEFAULT_CLIENT_NAME "xiajia_1981@163.com"
#pragma pack(push)
#pragma pack(1)
struct XSocketDataPack
{//���ݰ��Ľṹ
	int fromID;	//��������
	int toID;	//������ȥ
	XSocketExDataType dataType;	//��������
	int dataLen;	//���ݳ���
	unsigned char * data;	//������
	XSocketDataPack()
		:data(NULL)
		,dataType(SOCKET_DATA_TYPE_CUSTOM)
	{}
};
#pragma pack(pop)
enum XSocketClientState
{
	CLIENT_STATE_ONLINE,	//����
	CLIENT_STATE_OFFLINE,	//����
};
#define SOCKET_INFO_HEAD_SIZE (16)	//�û���Ϣͷ��С
struct XSocketUserInfo
{
	int userID;				//�û�ID���ɷ��������䣩
	int userSocket;			//�û��׽���
	XSocketClientState userState;			//�û�״̬
	int userNameLen;
	char *userName;			//�û�����
	XSocketUserInfo()
		:userName(NULL)
		,userNameLen(0)
	{}
};
enum XSocketClinetState
{//�ͻ��˵�״̬
	CLINET_STATE_UNCONNECT,	//��δ����
	CLINET_STATE_CONNECT,	//������
	CLINET_STATE_DISCONNECT,//���ӶϿ�
};
struct XSocketRecvPacket
{
	int recvDataLen;				//���յ������ݰ��ĳ���
	unsigned char headData[SOCKET_PACKET_HEAD_SIZE];		//���ݰ��İ�ͷ����
	XSocketDataPack *dataPacket;	//���ݰ�������
	XSocketRecvPacket()
		:recvDataLen(0)
		,dataPacket(NULL)
	{}
};
struct XSocketUserData
{
	bool isEnable;	//�Ƿ���Ч
	int userID;		//�û�ID
	int userSocket;	//�û��������׽���
	XSocketClinetState state;
	int heartbeatTimer;	//������ʱ��

	XSocketRecvPacket recvPacket; //���յ����ݰ�
	XThreadState recvThreadState;

	XSocketUserData()
		:isEnable(false)
		,state(CLINET_STATE_UNCONNECT)
		,heartbeatTimer(0)
		,recvThreadState(STATE_BEFORE_START)
	{}
};

class XSocketEx
{
private:
	bool m_isInited;	//�Ƿ��ʼ��
	bool m_clientStateChange;	//�ͻ��˵�״̬�Ƿ����仯
	SOCKET m_netSocket;	//�����������ӵ��׽���
	XSocketNetRole m_socketRole;		//���������߿ͻ���
	int m_timer;	//��ʱ��

	//�ͻ��˵����
	XSocketConnectState m_conState;	//��������״̬
	XSocketRecvPacket m_recvPacket;	//���յ����ݰ�
	//�����������
	int m_curAddClientID;	//��ǰ����Ŀͻ��˵�ID
	void addAClient(int clientSocket);	//�µĿͻ��˼���
	int m_myID;		//����ı��
	int m_IDIndex;	//���ڽ���ID

	XCritical m_recvMutex;				//���յ��߳���
	std::deque<XSocketDataPack *> m_recvDeque;		//���ն���
	XCritical m_sendMutex;				//���͵��߳���
	std::deque<XSocketDataPack *> m_sendDeque;		//���Ͷ���
	XCritical m_userInfoMutex;			//�û���Ϣ���߳���
	std::vector<XSocketUserInfo *> m_usersInfo;	//�û���Ϣ����
	//XCritical m_userDataMutex;			//�û����ݵ��߳���
	std::vector<XSocketUserData *> m_usersData;	//�û����ݶ���

	XSocketWorkMode m_workMode;

	sockaddr_in m_serverAddr;	//��������ַ
	std::string m_serverName;	//������������
	std::string m_serverIP;		//��������IP
	int m_serverPort;		//�������Ķ˿ں�
	std::string m_myName;	//�Լ�������

	bool getDataPacket(XSocketRecvPacket &packet,unsigned char *buff,int len);	//�����ݽ��������������ݰ�

	XThreadState m_connectThreadState;	//�����̵߳�״̬
	XThreadState m_recvThreadState;		//�����̵߳�״̬
	XThreadState m_sendThreadState;		//�����̵߳�״̬
	XThreadState m_acceptThreadState;	//�����̵߳�״̬
	XThreadState m_boardcastThreadState;	//�㲥�̵߳�״̬
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI connectThread(void * pParam);	//�����߳� 
	static DWORD WINAPI recvThread(void * pParam);		//�����߳� 
	static DWORD WINAPI sendThread(void * pParam);		//�����߳� 
	static DWORD WINAPI acceptThread(void * pParam);	//�����߳� 
	static DWORD WINAPI recvThreadX(void * pParam);		//��������Ϊ�ͻ��˿����Ľ����߳� 
	static DWORD WINAPI boardcastThread(void * pParam);		//�㲥�߳� 
#endif
	bool connectToNet();	//���ӵ�����
	//������һЩ˽�еķ���
	bool connectToServer();	//���ӵ�������
	bool createServer();	//����������
	void sendClientInfo();	//�����пͻ��˷���Ŀǰ������״̬
	void systemDataproc(const XSocketDataPack *data);
	void lockData(unsigned char *data,int len);		//��������
	void unlockData(unsigned char *data,int len);	//��������
	bool sendAData(XSocketDataPack * data);
	void requestRename();	//����������͸�������
	void sendHeartbeat();	//���������ź�
	void setOffline(int id);//����ĳ���ͻ�������
	void heartbeatProc(float stepTime);//����ͻ��˵�����������Ӧ�Ĵ���
	bool sendBoardcast();	//�㲥����
public:
	bool createNetWorkByName(const char *serverName,int port);	//ͨ�����������ƽ�������
	bool createNetWorkByIP(const char *serverIP,int port);		//ͨ��������IP��������
	//����
	bool sendAData(int toID,const void * data,int len);	//��ָ���û�������Ϣ,toID = 0�����������������
	bool sendToAll(unsigned char * data,int len);			//��ǰ���ߵ������û�������Ϣ(��δʵ��)
	//����
	XSocketDataPack *popAData();	//�ӽ��ն����е���һ����Ϣ
	void deleteAData(XSocketDataPack * p);
	bool haveRecvData() const {return m_recvDeque.size() > 0;}	//�жϽ��ն������Ƿ��������
	int getRecvDataSum() const {return m_recvDeque.size();}	//��ȡ���ն��������ݵ�����

	void release();
	XSocketEx()
		:m_isInited(false)
		,m_socketRole(NET_NULL)
		,m_conState(CON_STATE_NULL)
		,m_workMode(SOCKET_WORK_MODE_IP)
		,m_netSocket(INVALID_SOCKET)
		,m_curAddClientID(-1)
		,m_IDIndex(0)
		,m_myID(0)
		,m_myName(DEFAULT_CLIENT_NAME)
		,m_timer(0)
		,m_clientStateChange(false)
		,m_connectThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_recvThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_sendThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_acceptThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_boardcastThreadState(STATE_BEFORE_START)	//�㲥�̵߳�״̬
	{}
	~XSocketEx() {release();}

	//������һЩ��ɢ�ķ�������
	int getClientSum() {return m_usersInfo.size();}
	const XSocketUserInfo * getUserInfo(unsigned int index);
	void setAsClient();
	void setAsServer();
	XSocketNetRole getRole() const {return m_socketRole;}
	int getMyID() const {return m_myID;}
	int getMySocket() const {return m_netSocket;}
	bool setMyName(const std::string& myName);
	const std::string& getMyName()const {return m_myName;}
	XSocketConnectState getConState() const {return m_conState;}
	int getSendBuffSize()	//��ȡ���Ͷ����д��������ݵ�����
	{
		m_sendMutex.Lock();
		int ret = m_sendDeque.size();
		m_sendMutex.Unlock();
		return ret;
	}
};
#if WITH_INLINE_FILE
#include "XSocketEx.inl"
#endif
}
#endif