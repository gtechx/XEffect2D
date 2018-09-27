#ifndef _JIA_XNETCOMMON_
#define _JIA_XNETCOMMON_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include <deque>
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "..\XMemory.h"
namespace XE{
#define MAX_SEND_DATA_BUFF (3200)	//���Ͷ��еĴ�С����ֹ����ӵ������ڴ����
#define MAX_RECV_DATA_BUFF (6400)	//���ն��еĴ�С����ֹ����ӵ������ڴ����

//#define NET_SERVER_IP "192.168.1.110"
#define NET_SERVER_PORT (9988)
#define BOARDCAST_PORT (7898)	//�㲥�˿�
#define BOARDCAST_DATA_LEN (1024)
#define PROJECT_STRING_LEN (256)	//�����ַ����ĳ���
#define DEFAULT_PROJECT_STR "xiajia_1981@163.com"	//Ĭ�ϵĹ����ַ���
//#define WITH_LOCAL_BOARDCAST_IP
#define BOARDCASR_IP "192.168.1.255"
//��������״̬
enum XConnectState
{
	CONNECT_STATE_DISCONNECT,	//���ӶϿ�
	CONNECT_STATE_CONNECT,		//��������
};
//�������ݵ�����
enum XNetDataType
{
	DATA_TYPE_EPC,			//�û���epc��Ϣ
	DATA_TYPE_EXCESS_SUM,	//ʣ���λ������
	DATA_TYPE_VIDEO,		//��Ƶ����
	DATA_TYPE_VIDEO_UPDATE,	//���µ���Ƶ����
	DATA_TYPE_GAME_OVER,	//��Ϸ����
	DATA_TYPE_GAME_START,	//��Ϸ��ʼ
	DATA_TYPE_PHOTO,		//ͼ������:[�Ƿ���epc][epc][ͼ������]
	DATA_TYPE_KEY,			//����״̬

	DATA_TYPE_CONFIG_INFO,		//�������������Ϣ
	DATA_TYPE_CONFIG_ITEM,		//�����������ֵ
	DATA_TYPE_CONFIG_ITEMS,		//�����������ֵ
	DATA_TYPE_CONFIG_INJECT,	//����ע��
	DATA_TYPE_CONFIG_OPERATE,	//�����ý���Ĳ���

	DATA_TYPE_CUSTOM,			//�Զ������������ 
	DATA_TYPE_CUSTOM_01,			//�Զ������������ 
	DATA_TYPE_CUSTOM_02,			//�Զ������������ 
	DATA_TYPE_CUSTOM_03,			//�Զ������������ 
	DATA_TYPE_CUSTOM_04,			//�Զ������������ 
	DATA_TYPE_CUSTOM_05,			//�Զ������������ 
};
//˵�����紫�������Э��Ϊ
//CMD[1byte]Len[4byte][data]
#define PACKET_HEAD_LEN (5)		//��ͷ��С
struct XNetData	//��������
{
	XBool isEnable;
	XNetDataType type;
	int dataLen;
	unsigned char * data;
	XNetData()
		:isEnable(XFalse)
		,dataLen(0)
		,data(NULL)
	{}
	~XNetData()
	{
		if(!isEnable) return;
		XMem::XDELETE_ARRAY(data);
		isEnable = XFalse;
	}
};
//Ϊ��ͨѶ����װ�����ݰ��νṹ
struct XNetPack
{
	int clientID;		//�ͻ��˵ı��
	int buffSize;		//�ڴ�ռ�Ĵ�С	
	int curDataLen;		//��Ч���ݵĴ�С
	unsigned char *data;
	XNetPack()
		:data(NULL)
		, buffSize(0)
		, curDataLen(0)
		, clientID(0)
	{}
};
inline void showNetData(XNetData *)//data)
{
	//if(data == NULL) return;
	//printf("%d|%d:",data->type,data->dataLen);
	//for(int i = 0;i < data->dataLen;++ i)
	//{
	//	printf("%02x",data->data[i]);
	//}
	//printf("\n");
}
//����ָ��socket
inline void resetSocketAddr(SOCKET socket)
{
	int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
//����һ��socket��
inline bool getANewSocket(SOCKET &mySock,int protocol = SOCK_STREAM)
{
	if(mySock != INVALID_SOCKET)
	{//���û�з����׽��֣�������룬��ֹ�ظ�����
		closesocket(mySock);
		mySock = INVALID_SOCKET;
	}
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;
	mySock = socket(AF_INET, protocol, 0); //�����׽���
	if (mySock == INVALID_SOCKET) return false;
	resetSocketAddr(mySock);
	return true;
}
//��ȡ��ǰ�������IP��ַ
inline std::string getMyIP()
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) return XString::gNullStr;
	char hname[128] = "";
	gethostname(hname,sizeof(hname));
	hostent *hent = gethostbyname(hname);
	if(hent != NULL && hent->h_addr_list[0])
	{
		sprintf_s(hname,128,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
			(unsigned char)(hent->h_addr_list[0][1]),
			(unsigned char)(hent->h_addr_list[0][2]),
			(unsigned char)(hent->h_addr_list[0][3]));
	}
	WSACleanup();
	return hname;
}
}
#endif