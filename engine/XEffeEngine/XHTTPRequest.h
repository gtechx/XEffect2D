#ifndef _JIA_XHTTPREQUEST_
#define _JIA_XHTTPREQUEST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//#include <iostream>
//#include "AtlBase.h"

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef XEE_OS_LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <malloc.h>
#include <arpa/inet.h>        //inet_addr()
#include <unistd.h>           //usleep()
#include <netdb.h>
#endif

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "XBasicClass.h"

#define MEM_BUFFER_SIZE 10

//HTTP����Ľṹ��
struct _XHTTPRequestStruct
{
	char *headerSend;			//���͵İ�ͷ��ָ��
	char *headerReceive;		//���ܵİ�ͷ��ָ��
	char *message;				//���ܵ�HTTP�����ָ��
	long messageLength;			//����ĳ���
	_XHTTPRequestStruct()
		:headerSend(NULL)
		,headerReceive(NULL)
		,message(NULL)
		,messageLength(0)
	{}
};
//����ṹ��
struct _XMemBuffer
{
	unsigned char *buffer;		//�������ָ��
	unsigned char *position;	//����ĵ�ǰλ��ָ��
	size_t size;				//����Ĵ�С
	_XMemBuffer()
		:buffer(NULL)
		,position(NULL)
		,size(0)
	{}
};
class _XHTTPRequest
{
public:
	_XHTTPRequest();			//���캯��
	virtual ~_XHTTPRequest();	//��������
private:
	void memBufferCreate(_XMemBuffer *b);											//�������棬������MEM_BUFFER_SIZE�Ĵ洢�ռ�
	void memBufferGrow(_XMemBuffer *b);												//��������Ĵ洢�ռ�
	void memBufferAddByte(_XMemBuffer *b, unsigned char byt);						//��һ���ֽڵ����ݷ��뻺��
	void memBufferAddBuffer(_XMemBuffer *b, unsigned char *buffer, size_t size);	//��һ���ֽڵ����ݷ��뻺��
	unsigned long getHostAddress(const char *host);												//�����������IP
	int sendString(int sock,char *str);											//��ָ����sock�����ַ���
	int validHostChar(char ch);													//����ַ����Ƿ��������������
	void parseURL(char *url,char * protocol, char * host,char * request,int *port);								//����URL�ַ���
	int sendHTTP(char *url,char * headerReceive,unsigned char *post, int postLength,_XHTTPRequestStruct *req);			//����HTTP����
public:
	int sendRequest(_XBool IsPost,char *url,_XHTTPRequestStruct &req);				//������õķ���HTTP����ĺ���
};
//��ָ�������緢��һ���ַ�����ע���������ַ���������Ҫ�Լ��ַ�������Ϊ�������
inline int _XHTTPRequest::sendString(int sock,char *str)
{
	return send(sock,str,strlen(str),0);
}
//����ַ����Ƿ�������������ص㣺i.e. A-Z or 0-9 or -.:
inline int _XHTTPRequest::validHostChar(char ch)
{
	//return (isalpha(ch) || isdigit(ch) || ch == '-' || ch == '.' || ch == ':');
	return (((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) || ch == '-' || ch == '.' || ch == ':');
}
#endif
