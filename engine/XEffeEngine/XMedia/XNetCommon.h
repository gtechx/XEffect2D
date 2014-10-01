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
#include "XBasicFun.h"
#include "XBasicClass.h"

#define NET_SERVER_IP ("192.168.1.110")
#define NET_SERVER_PORT (9988)
#define BOARDCAST_PORT (7898)	//�㲥�˿�
#define BOARDCAST_DATA_LEN (1024)
#define PROJECT_STRING_LEN (256)	//�����ַ����ĳ���
#define DEFAULT_PROJECT_STR ("xiajia_1981@163.com")	//Ĭ�ϵĹ����ַ���
//��������״̬
enum _XConnectState
{
	CONNECT_STATE_DISCONNECT,	//���ӶϿ�
	CONNECT_STATE_CONNECT,		//��������
};
//�������ݵ�����
enum _XNetDataType
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
};
//˵�����紫�������Э��Ϊ
//CMD[1byte]Len[4byte][data]
#define PACKET_HEAD_LEN (5)		//��ͷ��С
struct _XNetData	//��������
{
	_XBool isEnable;
	_XNetDataType type;
	int dataLen;
	unsigned char * data;
	_XNetData()
		:isEnable(XFalse)
		,dataLen(0)
		,data(NULL)
	{}
	~_XNetData()
	{
		if(!isEnable) return;
		XDELETE_ARRAY(data);
		isEnable = XFalse;
	}
};
inline void showNetData(_XNetData *)//data)
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
inline void resetSocketAddr(int socket)
{
	int opt = 1;
    int len = sizeof(opt);
    setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,len);
}
//����һ��socket��
inline bool getANewSocket(SOCKET &mySock)
{
	if(mySock != INVALID_SOCKET)
	{//���û�з����׽��֣�������룬��ֹ�ظ�����
		closesocket(mySock);
		mySock = INVALID_SOCKET;
	}
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) return false;
	mySock = socket(AF_INET,SOCK_STREAM,0); //�����׽���
	if(mySock == INVALID_SOCKET) return false;
	resetSocketAddr(mySock);
	return true;
}
//��ȡ��ǰ�������IP��ַ
inline string getMyIP()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	char hname[128] = "";
	gethostname(hname,sizeof(hname));
	hostent *hent = gethostbyname(hname);
	if(hent != NULL && hent->h_addr_list[0])
	{
		sprintf(hname,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
			(unsigned char)(hent->h_addr_list[0][1]),
			(unsigned char)(hent->h_addr_list[0][2]),
			(unsigned char)(hent->h_addr_list[0][3]));
	}
	WSACleanup();
	return hname;
}
//#include <IPHlpApi.h>
////����pMacAddrӦ����8���ֽڵ�����
//BOOL GetMacAddress(unsigned char* pMacAddr)
//{
//    DWORD nRet;
//    //ֻ��ѯ�����ַ
//    DWORD nFlags = GAA_FLAG_SKIP_UNICAST 
//        | GAA_FLAG_SKIP_ANYCAST
//        | GAA_FLAG_SKIP_FRIENDLY_NAME
//        | GAA_FLAG_SKIP_MULTICAST
//        | GAA_FLAG_SKIP_DNS_SERVER;
//
//    ULONG bufLen = 1024;
//    PIP_ADAPTER_ADDRESSES pAdapterAddr = (PIP_ADAPTER_ADDRESSES)malloc(bufLen);
//    if(pAdapterAddr == NULL)
//        return FALSE;
//
//    //AF_INET: return only IPv4 addresses.
//    nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
//    if(nRet == ERROR_BUFFER_OVERFLOW)
//    {
//        pAdapterAddr = (PIP_ADAPTER_ADDRESSES)realloc(pAdapterAddr, bufLen);
//        if(pAdapterAddr == NULL)
//            return FALSE;
//
//        nRet = GetAdaptersAddresses(AF_INET, nFlags,  NULL, pAdapterAddr, &bufLen);
//    }
//
//    if(nRet == ERROR_SUCCESS)
//    {
//        memcpy(pMacAddr, &pAdapterAddr->PhysicalAddress, pAdapterAddr->PhysicalAddressLength);
//        free(pAdapterAddr);
//        return TRUE;
//    }else
//    {
//        //ff-ff-ff-ff-ff-ff: ��ʾ��ȡʧ�ܣ�δ֪��
//        memset(pMacAddr, 0xff, 6);
//        free(pAdapterAddr);
//        return FALSE;
//    }
//}
#endif