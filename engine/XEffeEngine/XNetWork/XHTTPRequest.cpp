#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XHTTPRequest.h"
#include "XSocketBasic.h"
namespace XE{
XHTTPRequest::XHTTPRequest()
{}
XHTTPRequest::~XHTTPRequest()
{}
//��ʼ�����沢�����ʼ����MEM_BUFFER_SIZE��С���ڴ�ռ�
void XHTTPRequest::memBufferCreate(XMemBuffer *b)
{
	b->size = MEM_BUFFER_SIZE;
	b->buffer =(unsigned char *)malloc(b->size);
	b->position = b->buffer;
}
//���������󻺴���ڴ�ռ�
void XHTTPRequest::memBufferGrow(XMemBuffer *b)
{
	size_t sz;
	sz = b->position - b->buffer;
	b->size = b->size << 1;
	b->buffer =(unsigned char *)realloc(b->buffer,b->size);	//����û�ж�NULL�����жϴ���
	assert(b->buffer != NULL);
	b->position = b->buffer + sz; // readjust current position
}
//�򻺴�ռ�����д��һ���ֽڵ�����
void XHTTPRequest::memBufferAddByte(XMemBuffer *b,unsigned char byt)
{
	if((size_t)(b->position-b->buffer) >= b->size)
	{
		memBufferGrow(b);
	}

	*(b->position++) = byt;
}
//��һ������д�뵽����ռ���
void XHTTPRequest::memBufferAddBuffer(XMemBuffer *b,unsigned char *buffer, size_t size)
{
	while(((size_t)(b->position-b->buffer)+size) >= b->size)
	{//���ϵı����ռ�֪���ռ��㹻Ϊֹ
		memBufferGrow(b);
	}
	memcpy(b->position,buffer,size);
	b->position+=size;
}
//ʹ��DNS������װ����IP��ַ
unsigned long XHTTPRequest::getHostAddress(const char *host)
{
	struct hostent *phe = NULL;
	char *p = NULL;
	phe = gethostbyname(host);

	if(phe == NULL) return 0;
	p = *phe->h_addr_list;
	return *((unsigned long*)p);
}

#ifdef XEE_OS_LINUX
void strupr(char *p)
{
	if(p == NULL) return;
	int tempLen = strlen(p);
	if(tempLen <= 0) return;
	for(int i = 0;i < tempLen;++ i)
	{
		if(p[i] >= 'a' && p[i] <= 'z')
		{
			p[i] += 'A' - 'a';
		}
	}
}
#endif

//����URL�����н�����Э�飬�˿ڣ�������ַ������
void XHTTPRequest::parseURL(char *url,char *protocol,char *host,char *request,int *port)
{//ʹ�ö��ָ���ͬһ���ַ������н��������˼·�ܲ���
	//��������ﱤ��
	//char tmpStr[] = "ftp://username:password@192.168.1.1:21/folder";
	//char proto[128];
	//char userName[128];
	//char password[128];
	//char ip[100];
	//int port = 80;
	//char page[100];
	////sscanf(text, "http://%99[^:]:%99d/%99[^\n]", ip, &port, page);
	//sscanf(tmpStr, "%99[^:]://%99[^:]:%99[^@]@%99[^:]:%99d/%99[^\n]",proto,userName,password,ip,&port,page);
	char *work = NULL;	//�����ַ���ʱ������ʱ�ݴ��ַ�������ͬ
	char *ptr = NULL;	
	char *ptr2 = NULL;

	*protocol = NULL;
	*host = NULL;
	*request = NULL;
	*port = 80;

#ifdef XEE_OS_WINDOWS
	work = _strdup(url);		//����URL�ַ�������Ϊ����
	_strupr(work);					//���ַ���ת��Ϊ��д
#endif
#ifdef XEE_OS_LINUX
	work = strdup(url);		//����URL�ַ�������Ϊ����
	strupr(work);
#endif
	ptr = strchr(work,':');			//Ѱ�ҵ�":"���Ա����ҵ�Э��
	if(ptr != NULL)
	{//����ҵ�Э�����ͣ��������︴��
		*(ptr++) = 0;
		strcpy(protocol,work);
	}else
	{//���û����Э�����ͣ���Ĭ��ΪHTTPЭ��
		strcpy(protocol,"HTTP");
		ptr = work;
	}
	if((*ptr == '/') && (*(ptr+1) == '/'))
	{//���Э��֮���ַ�б�ܣ������������б��
		ptr += 2;
	}
	//���濪ʼѰ������
	ptr2 = ptr;
	while(validHostChar(*ptr2) && *ptr2)
	{
		++ ptr2;
	}
	*ptr2 = 0;
	strcpy(host,ptr);
	//����֮��Ķ�Ϊ�����ַ���
	strcpy(request,url + (ptr2-work));
	//Ѱ�Ҷ˿ں�
	ptr = strchr(host,':');
	if(ptr!=NULL)
	{
		*ptr=0;
		*port = atoi(ptr+1);
	}//���û�ж˿ں���Ĭ��Ϊ80�˿�
	free(work);
}

//����HTTP��Ϣ
//URL������˼����URL�ַ���
//headerReceive�������������İ�ͷ
//post�����͵����ݵ�ָ��
//postLength�������ַ����ĳ���
//req����Ӧ������
//����ֵ��0��ʧ�� 1���ɹ�(�ù���ķ���ֵ����)
int XHTTPRequest::sendHTTP(char *url,char * headerReceive,unsigned char *post,int postLength,XHTTPRequestStruct *req)
{
	//SOCKADDR_IN sin;			//��������web������
	sockaddr_in sin;			//��������web������
	int sock;					//���ӷ��������׽���
	char buffer[512];		
	char protocol[20];			//�����ݴ�Э��
	char host[256];				//�����ݴ�������
	char request[1024];			//�����ݴ�����
	int l,port,chars;//,err;
	XMemBuffer headersBuffer;	//��ͷ����
	XMemBuffer messageBuffer;	//���建��
	char headerSend[1024];		//���Ͱ�ͷ
	int done;
	
	//����URL
	parseURL(url,protocol,host,request,&port);
	if(strcmp(protocol,"HTTP"))
	{//Ŀǰ���ģ��ֻ֧��HTTPЭ��
		return 0;
	}

#ifdef XEE_OS_WINDOWS
	if (!XSock::initSocket()) return 0;
#endif

	//�����׽���
	sock = socket(AF_INET,SOCK_STREAM,0);
	//if(sock == INVALID_SOCKET) return 0;//�����׽���ʧ��
	if(sock < 0) return 0;//�����׽���ʧ��

	//���ӵ�web������
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short)port);
	sin.sin_addr.s_addr = getHostAddress(host);
	if(connect(sock,(sockaddr*)&sin, sizeof(sockaddr_in)))
	{//�������ʧ���򷵻�
		return 0;
	}

	if(!*request)
	{//���û������������Ϊ��б��
		strcpy_s(request,1024,"/");
	}

	//����HTTP��ͷ��Ϣ
	if(post == NULL)
	{//������Ƿ��ͣ�����Ϊ���մ���
		sendString(sock,"GET ");
		strcpy_s(headerSend,1024, "GET ");
	}else
	{
		sendString(sock,"POST ");
		strcpy_s(headerSend,1024, "POST ");
	}
	sendString(sock,request);
	strcat_s(headerSend,1024, request);

	sendString(sock," HTTP/1.0\r\n");
	strcat_s(headerSend,1024, " HTTP/1.0\r\n");

	sendString(sock,"Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	strcat_s(headerSend,1024, "Accept: image/gif, image/x-xbitmap,"
		" image/jpeg, image/pjpeg, application/vnd.ms-excel,"
		" application/msword, application/vnd.ms-powerpoint,"
		" */*\r\n");
	sendString(sock,"Accept-Language: en-us\r\n");
	strcat_s(headerSend,1024, "Accept-Language: en-us\r\n");

	sendString(sock,"Accept-Encoding: gzip, default\r\n");
	strcat_s(headerSend,1024, "Accept-Encoding: gzip, default\r\n");
	sendString(sock,"User-Agent: Neeao/4.0\r\n");
	strcat_s(headerSend,1024, "User-Agent: Neeao/4.0\r\n");

	if(postLength)
	{
		sprintf_s(buffer,512,"Content-Length: %ld\r\n",postLength);
		sendString(sock,buffer);
		strcat_s(headerSend,1024, buffer);
	}
	//sendString(sock,"Cookie: mycookie=blablabla\r\n");
	// printf("Cookie: mycookie=blablabla\r\n");
	sendString(sock,"Host: ");
	strcat_s(headerSend,1024, "Host: ");

	sendString(sock,host);
	strcat_s(headerSend,1024, host);

	sendString(sock,"\r\n");
	strcat_s(headerSend,1024, "\r\n");

	if((headerReceive != NULL) && *headerReceive)
	{
		sendString(sock,headerReceive);
		strcat_s(headerSend,1024, headerReceive);
	}

	sendString(sock,"\r\n"); // Send a blank line to signal end of HTTP headerReceive
	strcat_s(headerSend,1024, "\r\n");

	if((post != NULL) && postLength)
	{//������Ҫ���͵�����
		send(sock,(const char*)post,postLength,0);
		post[postLength] = '\0';

		strcat_s(headerSend,1024, (const char*)post);
	}

	//strcpy(req->headerSend, headerSend);
	req->headerSend = (char*) malloc( sizeof(char*) * strlen(headerSend));
	if(req->headerSend != NULL)
		strcpy_s(req->headerSend, sizeof(char*) * strlen(headerSend),(char*) headerSend );

	//���濪ʼ���հ�ͷ
	memBufferCreate(&headersBuffer );
	chars = 0;
	done = 0;

	while(!done)
	{
		l = recv(sock,buffer,1,0);
		if(l < 0) done = 1;//�������

		switch(*buffer)
		{
		case '\r':
			break;
		case '\n':
			if(chars==0)
			{//�������
				done = 1;
			}
			chars=0;
			break;
		default:
			++ chars;
			break;
		}

		memBufferAddByte(&headersBuffer,*buffer);
	}
	//�����ܵ��İ�ͷ��������
	req->headerReceive = (char*) headersBuffer.buffer;
	*(headersBuffer.position) = 0;

	//������ܰ���
	memBufferCreate(&messageBuffer); // Cur read the HTTP body
	do
	{
		l = recv(sock,buffer,sizeof(buffer)-1,0);
		if(l<0)
		{//����������
			break;
		}
		*(buffer + l)=0;
		memBufferAddBuffer(&messageBuffer, (unsigned char*)&buffer, l);
	}while(l > 0);

	*messageBuffer.position = 0;
	req->message = (char*) messageBuffer.buffer;
	req->messageLength = (messageBuffer.position - messageBuffer.buffer);

	//�������֮��ر�socket
#ifdef XEE_OS_WINDOWS
	closesocket(sock);
#endif
#ifdef XEE_OS_LINUX
	close(sock);
#endif

	return 1;
}

//����HTTP����
//IsPost��			�Ƿ�Ϊ����
//url��				URL�ַ���
//req��				HTTP����Ľṹ��
//����ֵ��			1���ɹ� 0��ʧ
int XHTTPRequest::sendRequest(XBool IsPost,char *url,XHTTPRequestStruct &req)
{
	int rtn;

	if(req.headerReceive != NULL) free(req.headerReceive);
	if(req.message != NULL) free(req.message);
	req.messageLength = 0;

	if(IsPost)
	{//���� 
		int i = strlen(req.headerSend);
		char *buffer = (char*) malloc(i+1);
		if(buffer != NULL) strcpy_s(buffer,i + 1, req.headerSend);
		//�ͷ��ڴ�ռ�
		free(req.headerSend);

		rtn = sendHTTP(url,"Content-Type: application/x-www-form-urlencoded\r\n",
			(unsigned char*)buffer,i,&req);

		free(buffer);
	}else
	{//����
		rtn = sendHTTP(url,NULL,NULL,0,&req);
	}

	if(rtn != 0) return 1;//���������
	else return 0;
}
}