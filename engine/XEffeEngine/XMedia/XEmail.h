#ifndef _JIA_XEMAIL_
#define _JIA_XEMAIL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.29
//--------------------------------
#include <iostream>
#include <String.h>
#include <fstream>
#include <string>
#include "XBasicFun.h"
#include "XBasicClass.h"
#include <List>
#include <vector>
//����һ�����ڷ����ʼ�����,�Խ��г�������ͨ��
struct sMailInfo	//�ʼ���Ϣ  
{  
	std::string m_pcUserName;		//�û���¼���������  
	std::string m_pcUserPassWord;	//�û���¼���������  
	std::string m_pcSenderName;	//�û�����ʱ��ʾ������  
	std::string m_pcSender;		//�����ߵ������ַ  
	std::string m_pcReceiver;		//�����ߵ������ַ  
	std::string m_pcReceiverName;		//�ռ��˵�����  
	std::string m_pcTitle;		//�������  
	std::string m_pcBody;			//�ʼ��ı�����  
	std::string m_pcIPAddr;		//��������IP  
	std::string m_pcIPName;		//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
	sMailInfo()
		:m_pcUserName("")
		,m_pcUserPassWord("")
		,m_pcSenderName("")
		,m_pcSender("")
		,m_pcReceiver("")
		,m_pcReceiverName("")
		,m_pcTitle("")
		,m_pcBody("")
		,m_pcIPAddr("")
		,m_pcIPName("")
	{}  
}; 
class _XEmailSender
{
public:  
    _XEmailSender()
		:m_pcFileBuff(NULL)
	{
		memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
		memset(m_cReceiveBuff,0,sizeof(m_cReceiveBuff));  
	}
	~_XEmailSender(){DeleteAllPath();}
public:  
    bool SendMail(sMailInfo &smailInfo);		//�����ʼ�����Ҫ�ڷ��͵�ʱ���ʼ���ʼ���Ϣ  
    void AddFilePath(char * pcFilePath);		//��Ӹ����ľ���·���������б���  
    void DeleteFilePath(char* pcFilePath);		//ɾ������·��������еĻ�  
	void DeleteAllPath(){m_pcFilePathList.clear();}					//ɾ��ȫ��������·��  
protected:  
    void Char2Base64(char* pBuff64,char* pSrcBuff,int iLen);	//��char����ת����Base64����  
    bool  CReateSocket(SOCKET &sock);	//����socket����  
    bool Logon(SOCKET &sock);			//��¼���䣬��Ҫ���з��ʼ�ǰ��׼������  
    int GetFileData(const char* FilePath);	//���ļ�·����ȡ��������  
  
    bool SendHead(SOCKET &sock);		//�����ʼ�ͷ  
    bool SendTextBody(SOCKET &sock)	//�����ʼ��ı�����  
	{
		memset(m_cSendBuff,0,sizeof(m_cSendBuff));  
		sprintf_s(m_cSendBuff,"--INVT\r\nContent-Type: text/plain;\r\n  charset=\"gb2312\"\r\n\r\n%s\r\n\r\n",m_sMailInfo.m_pcBody.c_str());  
		return (send(sock,m_cSendBuff,strlen(m_cSendBuff),0) == strlen(m_cSendBuff)); 
	}
    bool SendFileBody(SOCKET &sock);	//�����ʼ�����  
    bool SendEnd(SOCKET &sock);			//�����ʼ���β  
protected:  
    //CList<char*,char*> m_pcFilePathList;	//��¼����·��  
	//std::list<char*,char*> m_pcFilePathList;	//��¼����·��  
	std::vector<std::string> m_pcFilePathList;
  
    char  m_cSendBuff[4096];		//���ͻ�����  
    char  m_cReceiveBuff[1024];  
    char* m_pcFileBuff;				//ָ�򸽼�����  
    sMailInfo m_sMailInfo;  
};
#include "XEmail.inl"
#endif