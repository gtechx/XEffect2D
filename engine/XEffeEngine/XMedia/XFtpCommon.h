#ifndef _JIA_XFTPCOMMON_
#define _JIA_XFTPCOMMON_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.12
//--------------------------------
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "XBasicFun.h"
#include "XBasicClass.h"
//#include <winsock2.h>
#include "..\XLogBook.h"

#define RECV_BUFF_SIZE (4096)
#define FTP_SOCKET_PORT (21)
#define SERVER_COMMAND_LEN (64)	//������������ܹ����ܵ������
//�û�����
enum _XFtpUserType
{
	FTP_USER_TYPE_NULL,		//�޶���
	FTP_USER_TYPE_DEFAULT,	//Ĭ���û�
	FTP_USER_TYPE_ANONY,	//�����û�
};

//�����Ƿ��ؿͻ��˵�һЩ��Ϣ
extern char returnStr125[];
extern char returnStr150[];
extern char returnStr200[];
extern char returnStr202[];
extern char returnStr211[];
extern char returnStr213[];
extern char returnStr215[];
extern char returnStr220[];
extern char returnStr221[];
extern char returnStr226[];
extern char returnStr230[];
extern char returnStr250[];
extern char returnStr257[];
extern char returnStr331[];
extern char returnStr350[];
extern char returnStr530[];
extern char returnStr531[];

extern char defaultUserName[];		//Ĭ�ϵ��û���
extern char defaultUserPassword[];	//Ĭ�ϵ�����
extern char anonyUserName[];		//�����û���
extern char anonyUserPassword[];	//����������

extern _XBool analyseFTPString(const char * str,char * IP,char * path,int &port);	//��ftp�ַ����н������������ ���ӣ�FTP://192.168.1.110:21/folder
//����ʹ��sscanf�ķ��������ַ�������Ҫ��֤url�ַ�������URL��׼
extern _XBool analyseFTPStrEx(const char * str,std::string &protocol,	//Э�飺http��ftp�ȣ����Ϊ��д
							  std::string &name,		//�û���
							  std::string &password,	//����
							  std::string &IP,			//IP��ַ
							  int &port,				//�˿ں�
							  std::string &path);		//·��

#endif