#include "XStdHead.h"
#include "XEmail.h" 
#include "XFile.h"
namespace XE{   
void XEmailSender::Char2Base64(char* pBuff64,char* pSrcBuff,int iLen)  
{  
    //1   1   1   1   1   1   1   1  
    // �����pBuff64  �� �����pBuff64+1  
    //         point���ڵ�λ��  
    static const char Base64Encode[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//base64��ӳ����ַ���  
    int point = 2;	//ÿһ��Դ�ַ���ֵ�λ�ã���ȡ2,4,6����ʼΪ2  
    int iIndex = 0;	//base64�ַ�������  
    char n=0;		//��һ��Դ�ַ��Ĳ���ֵ  
    for(int i = 0;i < iLen;++ i)  
    {  
		switch(point)
		{
		case 2:
            iIndex = ((*pSrcBuff)>>point)&0x3f;	//ȡ��pSrcBuff�ĸ�pointλ  
			break;
		case 4:
            iIndex = ((*pSrcBuff)>>point)&0xf;	//ȡ��pSrcBuff�ĸ�pointλ  
			break;
		case 6:
            iIndex = ((*pSrcBuff)>>point)&0x3;	//ȡ��pSrcBuff�ĸ�pointλ  
			break;
		}  
        iIndex += n;//��pSrcBuff-1�ĵ�point������Base64������  
		assert(iIndex < 65);
        *pBuff64 ++ = Base64Encode[iIndex];	//��������õ�pBuff64  
        n = ((*pSrcBuff)<<(6-point));		//����Դ�ַ��еĲ���ֵ  
        n = n&0x3f;		//ȷ��n�������λΪ0  
        point += 2;		//Դ�ַ��Ĳ��λ������2  
        if(point == 8)	//������λ��Ϊ8˵��pSrcBuff��6λ�������������һ��������Base64�ַ�������ֱ�������һ��  
        {  
            iIndex = (*pSrcBuff)&0x3f;		//��ȡ��6λ�������������  
            *pBuff64++ = Base64Encode[iIndex]; 
            n = 0;		//����ֵΪ0  
            point = 2;	//���λ����Ϊ2  
        }  
        ++ pSrcBuff;  
    }  
    if(n != 0) *pBuff64++ = Base64Encode[n];  
	switch(iLen%3)//���Դ�ַ������Ȳ���3�ı���Ҫ��'='��ȫ  
	{
	case 2:
		*pBuff64 = '=';  
		break;
	case 1:
		*pBuff64++ = '=';  
        *pBuff64 = '='; 
		break;
	} 
}  
void XEmailSender::AddFilePath(char * pcFilePath)	//��Ӹ���·��  
{  
    if(pcFilePath == NULL) return;
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {  
		if(strcmp(m_pcFilePathList[i].c_str(),pcFilePath) == 0)	//����Ѿ����ھͲ����������  
			return;  
    }  
	m_pcFilePathList.push_back(pcFilePath);  
}
void XEmailSender::DeleteFilePath(char* pcFilePath)//ɾ������·��  
{    
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {
		if(strcmp(m_pcFilePathList[i].c_str(),pcFilePath) == 0)//�ҵ���ɾ���������û�ҵ�������  
        {  
			//for(int j = i;j < (int)(m_pcFilePathList.size()) - 1;++ j)
			//{
			//	m_pcFilePathList[j] = m_pcFilePathList[j + 1];
			//} 
			//m_pcFilePathList.pop_back();
			m_pcFilePathList.erase(m_pcFilePathList.begin() + i);
            return;  
        }  
    }  
}    
int XEmailSender::GetFileData(const char* FilePath)  
{  
    if(FilePath == NULL) return 0;  
	//���ļ��ж�ȡ����
	FILE *fp = NULL;
	if((fp = fopen(FilePath,"rb")) == NULL) return 0;
	int len = XFile::getFileLen(fp);
	if(len <= 0)
	{
		fclose(fp);
		return 0;
	}
	m_pcFileBuff = XMem::createArrayMem<char>(len + 1);
	if(m_pcFileBuff == NULL)
	{
		fclose(fp);
		return 0;
	}
	fread(m_pcFileBuff,len,1,fp);
	fclose(fp);
    return len;  
} 
bool XEmailSender::CReateSocket(SOCKET &sock)  
{  
    WORD wVersionRequested;  
    WSADATA wsaData;  
    wVersionRequested = MAKEWORD( 2, 2 );  
    if(WSAStartup(wVersionRequested,&wsaData) != 0) return false;  
    if(LOBYTE(wsaData.wVersion) != 2 ||  
        HIBYTE(wsaData.wVersion) != 2 )  
    {  
        WSACleanup( );  
        return false;   
    }  
    sock = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);  
    if (sock == INVALID_SOCKET) return false;  
  
    sockaddr_in servaddr;  
    memset(&servaddr,0,sizeof(sockaddr_in));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(25);//���ʼ�һ�㶼��25�˿�  
    if(m_sMailInfo.m_pcIPName == "")  
    {  
		servaddr.sin_addr.s_addr = inet_addr(m_sMailInfo.m_pcIPAddr.c_str());//ֱ��ʹ��IP��ַ  
    }else  
    {  
        struct hostent *hp=gethostbyname(m_sMailInfo.m_pcIPName.c_str());//ʹ������  
		if(hp == NULL) return false;
        servaddr.sin_addr.s_addr=*(int*)(*hp->h_addr_list);  
    }  
 
    return !(connect(sock,(sockaddr*)&servaddr,sizeof(servaddr)) == SOCKET_ERROR);  //��������    
}  
bool XEmailSender::Logon(SOCKET &sock)  
{//����û�жԷ��ؽ�������жϿ��ܻ��������
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    sprintf_s(m_cSendBuff,"HELO []\r\n");  
    //sprintf_s(m_cSendBuff,"HELO mailto:xiajia_1981@sina.cn\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//��ʼ�Ự  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='5' || m_cReceiveBuff[2]!='0')  
		return false;  

	memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    sprintf_s(m_cSendBuff,"AUTH LOGIN\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//�����¼  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
		return false;  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	char tmpStr[1024];
	int len = strlen(m_sMailInfo.m_pcUserName.c_str());
	if(len >= 1024) return false;
	memcpy(tmpStr,m_sMailInfo.m_pcUserName.c_str(),len);
    Char2Base64(m_cSendBuff,tmpStr,len);  
    m_cSendBuff[strlen(m_cSendBuff)]='\r';  
    m_cSendBuff[strlen(m_cSendBuff)]='\n';  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//�����û���  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='3' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='4')  
	return false;
	  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	len = strlen(m_sMailInfo.m_pcUserPassWord.c_str());
	if(len >= 1024) return false;
	memcpy(tmpStr,m_sMailInfo.m_pcUserPassWord.c_str(),len);
    Char2Base64(m_cSendBuff,tmpStr,len);  
    m_cSendBuff[strlen(m_cSendBuff)]='\r';  
    m_cSendBuff[strlen(m_cSendBuff)]='\n';  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);//�����û�����  
	memset(m_cReceiveBuff,0,1024);
    recv(sock,m_cReceiveBuff,1024,0);  
    if(m_cReceiveBuff[0]!='2' || m_cReceiveBuff[1]!='3' || m_cReceiveBuff[2]!='5')  
		return false;   
    return true;//��¼�ɹ�  
}   
bool XEmailSender::SendHead(SOCKET &sock)  
{    
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"MAIL FROM:<%s>\r\n",m_sMailInfo.m_pcSender.c_str());  
      
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;  
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"RCPT TO:<%s>\r\n",m_sMailInfo.m_pcReceiver.c_str());  
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;   
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
    memcpy(m_cSendBuff,"DATA\r\n",strlen("DATA\r\n"));  
    if(send(sock,m_cSendBuff,strlen(m_cSendBuff),0) != strlen(m_cSendBuff))  
		return false;  
    recv(sock,m_cReceiveBuff,1024,0);  
  
    memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
	sprintf_s(m_cSendBuff,"From:\"%s\"<%s>\r\n",m_sMailInfo.m_pcSenderName.c_str(),m_sMailInfo.m_pcSender.c_str());  
	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"To:\"%s\"<%s>\r\n",m_sMailInfo.m_pcReceiverName.c_str(),m_sMailInfo.m_pcReceiver.c_str());  
	sprintf_s(&m_cSendBuff[strlen(m_cSendBuff)],150,"Subject:%s\r\nMime-Version: 1.0\r\nContent-Type: multipart/mixed;   boundary=\"INVT\"\r\n\r\n",m_sMailInfo.m_pcTitle.c_str());  
    return (send(sock,m_cSendBuff,strlen(m_cSendBuff),0) == strlen(m_cSendBuff));
}  
bool XEmailSender::SendFileBody(SOCKET &sock)  
{  
    const char* filePath;  
    int rt;  
    int len;  
    int pt=0;   
	for(unsigned int i = 0;i < m_pcFilePathList.size();++ i)  
    {  
        pt = 0;  
		filePath = m_pcFilePathList[i].c_str();  
        len = GetFileData(filePath);  
		std::string fileName = XFile::getFilenameFormPath(filePath);
  
		sprintf_s(m_cSendBuff,"--INVT\r\nContent-Type: application/octet-stream;\r\n  name=\"%s\"\r\nContent-Transfer-Encoding: base64\r\nContent-Disposition: attachment;\r\n  filename=\"%s\"\r\n\r\n",fileName.c_str(),fileName.c_str());  
        send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
        while (pt<len)  
        {  
            memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
            Char2Base64(m_cSendBuff,&m_pcFileBuff[pt],min(len-pt,3000));  
            m_cSendBuff[strlen(m_cSendBuff)]='\r';  
            m_cSendBuff[strlen(m_cSendBuff)]='\n';  
            rt=send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
            pt+=min(len-pt,3000);  
            if(rt != strlen(m_cSendBuff))  
				return false;  
        }  
        if(len != 0) XMem::XDELETE_ARRAY(m_pcFileBuff);
	}
	return true;
}
bool XEmailSender::SendEnd(SOCKET &sock)  
{  
    sprintf_s(m_cSendBuff,"--INVT--\r\n.\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
 
    sprintf_s(m_cSendBuff,"QUIT\r\n");  
    send(sock,m_cSendBuff,strlen(m_cSendBuff),0);  
    closesocket(sock);  
    WSACleanup();  
    return true;  
}  
bool XEmailSender::SendMail(sMailInfo &smailInfo)  
{  
	m_sMailInfo = smailInfo;
    SOCKET sock;  
    if(!CReateSocket(sock))
	{
		LogNull("��������ʧ��");
		return false;  //��������
	}
    if(!Logon(sock))
	{
		LogNull("��½���������ʧ��");
		return false;  //��¼����
	}
    if(!SendHead(sock))
	{
		LogNull("�����ʼ�ͷʧ��");
		return false;  //�����ʼ�ͷ 
	}
    if(!SendTextBody(sock))
	{
		LogNull("�����ı���ʧ��");
		return false;  //�����ʼ��ı�����
	}
    if(!SendFileBody(sock))
	{
		LogNull("���͸���ʧ��");
		return false;  //���͸��� 
	}
    if(!SendEnd(sock))
	{
		LogNull("���ͽ�����ʧ��");
		return false;  //�����ʼ������ر�sock  
	}
    return true;  
} 
#if !WITH_INLINE_FILE
#include "XEmail.inl"
#endif
}