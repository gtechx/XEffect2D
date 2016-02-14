#ifndef _JIA_XHTTPREQUEST_
#define _JIA_XHTTPREQUEST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XWinSockCommon.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "XBasicClass.h"
namespace XE{
#define MEM_BUFFER_SIZE (10)

//HTTP����Ľṹ��
struct XHTTPRequestStruct
{
	char *headerSend;			//���͵İ�ͷ��ָ��
	char *headerReceive;		//���ܵİ�ͷ��ָ��
	char *message;				//���ܵ�HTTP�����ָ��
	long messageLength;			//����ĳ���
	XHTTPRequestStruct()
		:headerSend(NULL)
		,headerReceive(NULL)
		,message(NULL)
		,messageLength(0)
	{}
};
//����ṹ��
struct XMemBuffer
{
	unsigned char *buffer;		//�������ָ��
	unsigned char *position;	//����ĵ�ǰλ��ָ��
	size_t size;				//����Ĵ�С
	XMemBuffer()
		:buffer(NULL)
		,position(NULL)
		,size(0)
	{}
};
class XHTTPRequest
{
public:
	XHTTPRequest();			//���캯��
	virtual ~XHTTPRequest();	//��������
private:
	void memBufferCreate(XMemBuffer *b);											//�������棬������MEM_BUFFER_SIZE�Ĵ洢�ռ�
	void memBufferGrow(XMemBuffer *b);												//��������Ĵ洢�ռ�
	void memBufferAddByte(XMemBuffer *b, unsigned char byt);						//��һ���ֽڵ����ݷ��뻺��
	void memBufferAddBuffer(XMemBuffer *b, unsigned char *buffer, size_t size);	//��һ���ֽڵ����ݷ��뻺��
	unsigned long getHostAddress(const char *host);												//�����������IP
	int sendString(int sock,char *str);											//��ָ����sock�����ַ���
	int validHostChar(char ch);													//����ַ����Ƿ��������������
	void parseURL(char *url,char * protocol, char * host,char * request,int *port);								//����URL�ַ���
	int sendHTTP(char *url,char * headerReceive,unsigned char *post, int postLength,XHTTPRequestStruct *req);			//����HTTP����
public:
	int sendRequest(XBool IsPost,char *url,XHTTPRequestStruct &req);				//������õķ���HTTP����ĺ���
};
//��ָ�������緢��һ���ַ�����ע���������ַ���������Ҫ�Լ��ַ�������Ϊ�������
inline int XHTTPRequest::sendString(int sock,char *str)
{
	return send(sock,str,strlen(str),0);
}
//����ַ����Ƿ�������������ص㣺i.e. A-Z or 0-9 or -.:
inline int XHTTPRequest::validHostChar(char ch)
{
	//return (isalpha(ch) || isdigit(ch) || ch == '-' || ch == '.' || ch == ':');
	return (((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9')) || ch == '-' || ch == '.' || ch == ':');
}
}
#endif
