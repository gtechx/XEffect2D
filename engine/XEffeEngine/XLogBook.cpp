#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLogBook.h"
#include "XNetWork/XSocketEx.h"
#include "XFile.h"
#include <direct.h>	//for mkdir()
#ifdef XEE_OS_LINUX
#include <stdarg.h>
#endif
namespace XE{
#define DEFAULT_LOG_FILE_NAME "logBook"

XBool XLogbook::init(const char *fileName)
{
    if(m_isInited ||    //���֮ǰ�Ѿ���ʼ������������ֱ�ӷ���
		m_logLevel == LB_LEVEL_NULL ||	//��ʹ��log
		fileName == NULL || strlen(fileName) <= 0) return XFalse;
	m_filename = fileName + std::string("_") + XTime::sysTime2String(XTime::getTimeMs()) + ".txt";
	m_LogInfo = XMem::createArrayMem<XLogbookInfo>(m_maxRecordSum);
	if(m_LogInfo == NULL) return XFalse;
    m_logInfoPoint = 0;
	m_tmpBuff = XMem::createArrayMem<char>(m_maxRecordLength);
	if(m_tmpBuff == NULL)
	{
		XMem::XDELETE_ARRAY(m_LogInfo);
		return XFalse;
	}

    if(m_workMode == LOGBOOK_MODE_SERVER)
    {//���ӷ�����
		//����������������
		if(gFrameworkData.pFrameWork != NULL && !XEG.isProgramExist(LOG_SERVER_NAME))
		{//������������򲻴���,��������Ӧ�ó���
			if(!XFile::isAbsolutePath(m_logServerFilename.c_str()))
				m_logServerFilename = XFile::getFullPath(m_logServerFilename);	//������Ǿ���·���������ｫ���·��ת���ɾ���·��
			std::string curWorkPath = XFile::getWorkPath();	//��ȡ��ǰ��·��
			XFile::setWorkPath(XFile::getFilePath(m_logServerFilename.c_str()));	//������־��������·��Ϊ����·���Ա�����־�ļ�����������־��������·����
			if(!XEE::startProgram(m_logServerFilename.c_str()))	//���з���������ʧ�ܣ�����ת���ɵ���ģʽ
				m_workMode = LOGBOOK_MODE_SIMPLE;
			else
			{
				XFile::setWorkPath(curWorkPath);
				//Sleep(1000);	//�ȴ�5s	//�ȴ�������ȫ����������
			}
		}
		if(m_workMode == LOGBOOK_MODE_SERVER)
		{//���ӵ�������
			m_socket->setAsClient();
			//if(!m_socket.createNetWorkByName(LOG_SERVER_NAME,m_logServerPort))
			if(!m_socket->createNetWorkByIP(LOG_SERVER_IP,m_logServerPort))
			{//���ӷ�����ʧ�ܣ�������Ϊ������ģʽ
				m_workMode = LOGBOOK_MODE_SIMPLE;
			}
		}
    }
	if(m_workMode == LOGBOOK_MODE_SIMPLE)
	{//�����ļ��У���������־�ļ�
		std::string tmp = XFile::getWorkPath() + "\\Logs";
		//if(mkdir(tmp.c_str()) < 0) return XFalse;
		mkdir(tmp.c_str());
		if(!XFile::isAbsolutePath(m_filename.c_str()))
			m_filename = "Logs\\" + m_filename;
		if((m_logBookFile = fopen(m_filename.c_str(),"a")) == NULL
			&& (m_logBookFile = fopen(m_filename.c_str(),"w")) == NULL)
		{
			XMem::XDELETE_ARRAY(m_LogInfo);
			return XFalse;
		}
	}

    m_logInfoPushPoint = 0;
    m_logInfoPopPoint = 0;
    //������Ϣ����߳�
#ifdef XEE_OS_LINUX
    if(pthread_create(&m_outputLogMessageThreadP, NULL, &outputLogMessageThread, (void*) this) != 0) 
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,outputLogMessageThread,this,0,&m_outputLogMessageThreadP) == 0) 
#endif
    {//�߳̽���ʧ�ܣ�ֱ�ӷ���
		XMem::XDELETE_ARRAY(m_LogInfo);
        return XFalse;
    }

    m_isInited = XTrue;    //��ǳ�ʼ���Ѿ����
	m_isReleased = STATE_BEFORE_START;
	//��־��ʼ��֮�����һЩ��־�Ļ�����Ϣ
	LogNull("����ʱ�䣺%s",XTime::sysTime2String(XTime::getTimeMs()).c_str());
	LogNull("ִ���ļ���%s",XFile::getCurrentExeFileFullPath().c_str());
	LogNull("����ʱ�䣺%s %s",__DATE__,__TIME__);
	if(gFrameworkData.pFrameWork != NULL)
	{
		LogNull("��Ŀ���ƣ�%s",XEG.m_windowData.windowTitle.c_str());
		LogNull("�汾�ţ�%s",XEG.m_windowData.versionString.c_str());
	}
    return XTrue;
}
void XLogbook::release()
{//��û�б���ļ�¼,���浽�ļ���,���ر��ļ�
    if(!m_isInited) return;
	LogStr("------ End Of File! ------");
	waitThreadEnd(m_isReleased);//�ȴ����߳̽���
	popAllLogMessage();	//��������ʣ�����־���
    if(m_workMode == LOGBOOK_MODE_SIMPLE)
    {//��ģʽ����Ҫ����־�ļ���д�������Ϣ
		if(m_needFlushFile) fflush(m_logBookFile);	//ǿ�ƽ��������뵽���̻���
        fclose(m_logBookFile);
    }else
	{//��������������ͽ�����־���ر���������
		XLogServerDataType sendType = LOG_DATA_TYPE_OVER;
		unsigned char sendData[8];
		memcpy(sendData,&sendType,sizeof(XLogServerDataType));
		memcpy(sendData + sizeof(XLogServerDataType),&m_clientID,sizeof(int));
		m_socket->sendAData(0,sendData,8);
		while(m_socket->getSendBuffSize() > 0)	//�ȴ��������ݷ������
		{
			Sleep(1);
		}
		m_socket->release();
	}
	XMem::XDELETE_ARRAY(m_LogInfo);
	m_isInited = XFalse;
}
void XLogbook::popAllLogMessage()
{//��ն����е�������־��Ϣ
	if(!m_isInited) return;
	if(m_workMode == LOGBOOK_MODE_SERVER)
	{
		int len = 0;
		int curDataLen = 0;
		char *tempMessage = NULL;
		unsigned char *sendMessage = NULL;
		XLogServerDataType dataType = LOG_DATA_TYPE_MESSAGE;

		for(int i = 0;i < m_maxRecordSum;++ i)
		{
			if(!m_LogInfo[i].isEnable) continue;
			if(m_logLevel == LB_LEVEL_CONSOLE
				|| m_logLevel == LB_LEVEL_ALL)
				printf("%s\n",m_LogInfo[i].logMessage);	//���������������

			if(m_logLevel == LB_LEVEL_FILE
				|| m_logLevel == LB_LEVEL_ALL)
			{
				tempMessage = m_LogInfo[i].logMessage;

				len = strlen(tempMessage) + 1;
				if(len + 8 > curDataLen)
				{//�����Ҫ������ڴ�ռ䣬��������������ڴ�ռ�
					XMem::XDELETE_ARRAY(sendMessage);
					sendMessage = XMem::createArrayMem<unsigned char>(len + 8);
					if(sendMessage == NULL)
					{
						curDataLen = 0;
						XMem::XDELETE_ARRAY(tempMessage);
						continue;
					}
					curDataLen = len + 8;
				}
				assert(sendMessage != NULL);
				memcpy(sendMessage,&dataType,sizeof(int));
				memcpy(sendMessage + sizeof(int),&m_clientID,sizeof(int));
				memcpy(sendMessage + sizeof(int) + sizeof(int),tempMessage,len);
				m_socket->sendAData(0,sendMessage,len + 8);
			}
			XMem::XDELETE_ARRAY(m_LogInfo[i].logMessage);
			m_LogInfo[i].isEnable = XFalse;
		}
	}else
	{
		if(m_logBookFile == NULL) return;
		for(int i = 0;i < m_maxRecordSum;i ++)
		{
			if(!m_LogInfo[i].isEnable) continue;
			if(m_logLevel == LB_LEVEL_CONSOLE
				|| m_logLevel == LB_LEVEL_ALL)
				printf("%s\n",m_LogInfo[i].logMessage);
			if(m_logLevel == LB_LEVEL_FILE
				|| m_logLevel == LB_LEVEL_ALL)
				fprintf(m_logBookFile,"%s\n",m_LogInfo[i].logMessage);
			XMem::XDELETE_ARRAY(m_LogInfo[i].logMessage);
			m_LogInfo[i].isEnable = XFalse;
			if(m_needFlushFile) fflush(m_logBookFile);	//ǿ�ƽ��������뵽���̻���
		}
	}
}
//XBool XLogbook::doubleStringLength(char ** message,int messageSize)
//{
//	if(messageSize <= 0 || message == NULL) return XFalse;
//    int len = messageSize;
//    char * tempData = (*message);
//    (*message) = NULL;
//
//    messageSize = messageSize << 1;
//    (*message) = XMem::createArrayMem<char>(messageSize);
//    if((*message) == NULL)
//    {
//		(* message) = tempData;	//���ı�ԭ��������
//        printf("Memory Error:File:%s,Line:%d\n",__FILE__,__LINE__);
//        return XFalse;
//    }
//    memcpy((*message),tempData,len);
//	XMem::XDELETE_ARRAY(tempData);
//    return XTrue;
//}
//void XLogbook::addLogInfoNull(const char *p,...)
//{
//    if(!m_isInited) initLog(DEFAULT_LOG_FILE_NAME);
//
//    int tempMessageSize = m_maxRecordLength;
//    char * tempMessage = XMem::createArrayMem<char>(tempMessageSize);
//    if(tempMessage == NULL) return;
//
//    va_list arg_ptr;
//    char c,nc;
//    int charPoint = 0;
//
//    va_start(arg_ptr,p);
//    while(c = * p++) 
//    {
//        if(c == '%'&& (nc = *p) != '\0')
//        {
//            ++ p;
//            switch(nc) 
//            {
//            case 'c': //%c
//                {
//                    /*Ϊ���ڴ���룬����Ҫдint*/
//                    char ch = va_arg(arg_ptr, int);
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = ch;
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = ch;
//                    }
//                    ++ charPoint;
//                    break;
//                }
//            case 's': //%s
//                {
//                    char *str = va_arg(arg_ptr, char *);
//
//                    int len = tempMessageSize - 1 - strlen(str);
//                    if(charPoint < len)
//                    {
//                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
//                    }else
//                    {
//                        while(true)
//                        {
//                            if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                            {
//								XMem::XDELETE_ARRAY(tempMessage);
//                                return;
//                            }
//                            tempMessageSize = tempMessageSize << 1;
//                            len = tempMessageSize - 1 - strlen(str);
//                            if(charPoint < len) break;
//                        }
//
//                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
//                    }
//
//                    charPoint += strlen(str);
//                    break;
//                }
//            case 'd'://%d
//                {
//                    int data = va_arg(arg_ptr,int);
//                    char buf[16];
//                    sprintf_s(buf,16,"%d",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }
//                    charPoint += strlen(buf);
//                    break;
//                }
//            case 'f'://%f
//                {
//                    double data = va_arg(arg_ptr,double);    //ע�������double ���ʹ��float �ǻ�����
//                    char buf[16];
//                    sprintf_s(buf,16,"%f",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }
//                    charPoint += strlen(buf);
//                    break;
//                }
//            default:
//                {
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = '%';
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = '%';
//                    }
//                    ++ charPoint;
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = nc;
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = nc;
//                    }
//                    ++ charPoint;
//                }
//            }
//        }else
//        {
//            if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//            {
//                tempMessage[charPoint] = c;
//            }else
//            {
//                if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                {
//					XMem::XDELETE_ARRAY(tempMessage);
//                    return;
//                }
//                tempMessageSize = tempMessageSize << 1;
//
//                tempMessage[charPoint] = c;
//            }
//            ++ charPoint;
//        }
//        if((*p) == '\0') break;
//    }
//    va_end(arg_ptr);
//    tempMessage[charPoint] = '\0';
//
//    pushAMessage(tempMessage);
//	XMem::XDELETE_ARRAY(tempMessage);
//}
void XLogbook::addLogInfoNull(const char *p,...)
{
	if(!m_isEnable ||
		(!m_isInited && !init(DEFAULT_LOG_FILE_NAME))) return;

	if (m_tmpBuff == NULL) return;
	m_locker.Lock();
	va_list args;
	va_start(args,p);
	vsnprintf(m_tmpBuff,m_maxRecordLength,p,args);
	va_end(args);

	pushAMessage(m_tmpBuff);
	m_locker.Unlock();
}
//void XLogbook::addLogInfoExp(XBool exp,const char *p,...)
//{
//    if(exp) return;//��������Ҫ��ӡ������Ϣ
//    if(!m_isInited)  initLog(DEFAULT_LOG_FILE_NAME);
//
//    int tempMessageSize = m_maxRecordLength;
//    char * tempMessage = XMem::createArrayMem<char>(tempMessageSize);
//    if(tempMessage == NULL) return;
//
//    va_list arg_ptr;
//    char c,nc;
//    int charPoint = 0;
//
//    va_start(arg_ptr,p);
//    while(c = * p++) 
//    {
//        if(c == '%'&& (nc = *p) != '\0')
//        {
//            ++ p;
//            switch(nc) 
//            {
//            case 'c': //%c
//                {
//                    /*Ϊ���ڴ���룬����Ҫдint*/
//                    char ch = va_arg(arg_ptr, int);
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = ch;
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = ch;
//                    }
//                    ++ charPoint;
//                    break;
//                }
//            case 's': //%s
//                {
//                    char *str = va_arg(arg_ptr, char *);
//                    int len = tempMessageSize - 1 - strlen(str);
//                    if(charPoint < len)
//                    {
//                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
//                    }else
//                    {
//                        while(true)
//                        {
//                            if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                            {
//								XMem::XDELETE_ARRAY(tempMessage);
//                                return;
//                            }
//                            tempMessageSize = tempMessageSize << 1;
//                            len = tempMessageSize - 1 - strlen(str);
//                            if(charPoint < len) break;
//                        }
//
//                        memcpy(&(tempMessage[charPoint]),str,strlen(str) + 1);
//                    }
//                    charPoint += strlen(str);
//                    break;
//                }
//            case 'd'://%d
//                {
//                    int data = va_arg(arg_ptr,int);
//                    char buf[16];
//                    sprintf_s(buf,16,"%d",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }
//                    charPoint += strlen(buf);
//                    break;
//                }
//            case 'f'://%f 
//                {
//                    double data = va_arg(arg_ptr,double);    //ע�������double ���ʹ��float �ǻ�����
//                    char buf[16];
//                    sprintf_s(buf,16,"%f",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }
//                    charPoint += strlen(buf);
//                    break;
//                }
//            default:
//                {
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = '%';
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = '%';
//                    }
//                    ++ charPoint;
//                    if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//                    {
//                        tempMessage[charPoint] = nc;
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XMem::XDELETE_ARRAY(tempMessage);
//                            return;
//                        }
//                        tempMessageSize = tempMessageSize << 1;
//
//                        tempMessage[charPoint] = nc;
//                    }
//                    ++ charPoint;
//                }
//            }
//        }else
//        {
//            if(charPoint < (int)(tempMessageSize - 1 - sizeof(char)))
//            {
//                tempMessage[charPoint] = c;
//            }else
//            {
//                if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                {
//					XMem::XDELETE_ARRAY(tempMessage);
//                    return;
//                }
//                tempMessageSize = tempMessageSize << 1;
//
//                tempMessage[charPoint] = c;
//            }
//            ++ charPoint;
//        }
//        if((*p) == '\0') break;
//    }
//    va_end(arg_ptr);
//    tempMessage[charPoint] = '\0';
//
//    pushAMessage(tempMessage);
//	XMem::XDELETE_ARRAY(tempMessage);
//}
void XLogbook::addLogInfoExp(XBool exp,const char *p,...)
{
	if(!m_isEnable ||
		(!m_isInited && !init(DEFAULT_LOG_FILE_NAME)) ||
		exp) return;//��������Ҫ��ӡ������Ϣ

	m_locker.Lock();
	va_list args;
    va_start(args,p);
    vsnprintf(m_tmpBuff,m_maxRecordLength,p,args);
	va_end(args);

    pushAMessage(m_tmpBuff);
	m_locker.Unlock();
}
XBool XLogbook::pushAMessage(const char * message)    //�����������һ����Ϣ    
{
    if(!m_isInited && !init(DEFAULT_LOG_FILE_NAME)) return XFalse;

    int len = strlen(message) + 1;
    if(len <= 1) return XFalse;    //no message
    //++++++++++++++++++++++++++++++++++++++++
    //Ϊÿ����־����ʱ���
    XSystemTime tempTime;
	XTime::getTimeMs(tempTime);
    char *tempMessage = XMem::createArrayMem<char>(len + m_recordHeadLength);
    if(tempMessage == NULL) return XFalse;

    sprintf_s(tempMessage,len + m_recordHeadLength,"%02d%02d%02d-%03d:",tempTime.hour,
		tempTime.minute,tempTime.second,tempTime.millisecond);
    strcat_s(tempMessage,len + m_recordHeadLength,message);
    len += m_recordHeadLength;
    //----------------------------------------
    //push message
    if(!m_LogInfo[m_logInfoPushPoint].isEnable)
    {//can input
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = XTrue;
    }else
    {//full
		//delete now message
		m_locker.Lock();	//�������᲻��ȫ
		if(m_logInfoPushPoint == m_logInfoPopPoint)
		{
			++ m_logInfoPopPoint;
			 if(m_logInfoPopPoint >= m_maxRecordSum) m_logInfoPopPoint = 0;
		}
		m_locker.Unlock();
        m_LogInfo[m_logInfoPushPoint].isEnable = XFalse;
		XMem::XDELETE_ARRAY(m_LogInfo[m_logInfoPushPoint].logMessage);
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = XTrue;
    }
	//static int sum = 0;
	//++sum;
	//printf("+%d\n",sum);
    //move point
    ++ m_logInfoPushPoint;
    if(m_logInfoPushPoint >= m_maxRecordSum)  m_logInfoPushPoint = 0;
	return XTrue;
}
XBool XLogbook::popAMessage(char ** message)    //�Ӷ�����ȡ��һ����Ϣ
{
    //if(m_isInited == 0) initLog(DEFAULT_LOG_FILE_NAME);
	if(!m_isInited) return XFalse;
	m_locker.Lock();	//�������᲻��ȫ
	if(m_LogInfo[m_logInfoPopPoint].isEnable)
    {//have message
		//static int sum = 0;
		//++sum;
		//printf("-%d\n",sum);
        //pop message
		XMem::XDELETE_ARRAY((* message));	//ɾ��ԭ�е�����
        (* message) = m_LogInfo[m_logInfoPopPoint].logMessage;
        m_LogInfo[m_logInfoPopPoint].isEnable = XFalse;
        m_LogInfo[m_logInfoPopPoint].logMessage = NULL;
        //move point
        ++ m_logInfoPopPoint;
        if(m_logInfoPopPoint >= m_maxRecordSum) m_logInfoPopPoint = 0;
		m_locker.Unlock();
        return XTrue;
    }else
    {//empty
		m_locker.Unlock();
        return XFalse;
    }
}
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XLogbook::outputLogMessageThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *XLogbook::outputLogMessageThread(void * pParam)
#endif
{
    XLogbook &pPar = *(XLogbook *) pParam;

    char *tempMessage = NULL;
	int curDataLen = 0;
    unsigned char *sendMessage = NULL;

//    int recvOrder = 0;
//    int recvID = 0;
//    int recvLength = 0;
//    char *recvMessage = NULL;
	XLogServerDataType dataType = LOG_DATA_TYPE_MESSAGE;
	pPar.m_isReleased = STATE_START;
	bool isSendInfo = false;
    while(pPar.m_isReleased != STATE_SET_TO_END)
    {
        //++++++++++++++++++++++++++++++++++++++++++++
        //����ӷ��������յ�����Ϣ
        if(pPar.m_workMode == LOGBOOK_MODE_SERVER)
		{
			if(pPar.m_socket->haveRecvData())
			{//���ﴦ����յ�������
				XSocketDataPack * socketData = pPar.m_socket->popAData();
				if(socketData->dataLen < 8)
				{
					pPar.m_socket->deleteAData(socketData);
					continue;
				}
				XLogServerDataType type = LOG_DATA_TYPE_NULL;
				memcpy(&type,socketData->data,sizeof(XLogServerDataType));
				if(type == LOG_DATA_TYPE_ID)//���������ص�ID
					memcpy(&pPar.m_clientID,socketData->data + sizeof(XLogServerDataType),sizeof(int));
				pPar.m_socket->deleteAData(socketData);
			}
			if(pPar.m_clientID == 0 && pPar.m_socket->getConState() == CON_STATE_NORMAL && !isSendInfo)
			{//���������������һЩ��Ϣ
				Sleep(1000);	//������Ҫ�ȴ��������ӳ�ʼ����ɣ��������߼��ϲ��Ͻ�����Ҫ����һ��״̬��ʹ�������߼��Ͻ�
				isSendInfo = true;	//ȷ������������ֻ����һ��
				int len = sizeof(XLogServerDataType) + sizeof(int) + (int)(pPar.m_filename.size()) + 1 + sizeof(unsigned int);
				unsigned char *tmpData = XMem::createArrayMem<unsigned char>(len);
				if(tmpData != NULL)
				{
					int offset = 0;
					XLogServerDataType tmpType = LOG_DATA_TYPE_INFO;
					memcpy(tmpData + offset,&tmpType,sizeof(XLogServerDataType));
					offset += sizeof(XLogServerDataType);
					int tmpLen = (int)(pPar.m_filename.size()) + 1;
					memcpy(tmpData + offset,&tmpLen,sizeof(int));
					offset += sizeof(int);
					memcpy(tmpData + offset,pPar.m_filename.c_str(),tmpLen);
					offset += tmpLen;
					unsigned int handle = XEE::getCurProcessID();
					//printf("Process ID��%d\n",handle);
					memcpy(tmpData + offset,&handle,sizeof(unsigned int));
					offset += sizeof(unsigned int);
					pPar.m_socket->sendAData(0,tmpData,len);
				}
				XMem::XDELETE_ARRAY(tmpData);
			}
		}
        //--------------------------------------------
		switch(pPar.m_workMode)
		{
		case LOGBOOK_MODE_SERVER:
			if(pPar.m_clientID != 0 && pPar.popAMessage(&tempMessage))
			{//�����ݷ��͵�������
				if(pPar.m_logLevel == LB_LEVEL_CONSOLE
					|| pPar.m_logLevel == LB_LEVEL_ALL)
					printf("%s\n",tempMessage);
				if(pPar.m_logLevel == LB_LEVEL_FILE
					|| pPar.m_logLevel == LB_LEVEL_ALL)
				{
					int len = strlen(tempMessage) + 1;
					if(len + 8 > curDataLen)
					{//�����Ҫ������ڴ�ռ䣬��������������ڴ�ռ�
						XMem::XDELETE_ARRAY(sendMessage);
						sendMessage = XMem::createArrayMem<unsigned char>(len + 8);
						if(sendMessage == NULL)
						{
							curDataLen = 0;
							XMem::XDELETE_ARRAY(tempMessage);
							continue;
						}
						curDataLen = len + 8;
					}
					assert(sendMessage != NULL);
					memcpy(sendMessage,&dataType,sizeof(int));
					memcpy(sendMessage + sizeof(int),&pPar.m_clientID,sizeof(int));
					memcpy(sendMessage + sizeof(int) + sizeof(int),tempMessage,len);
					pPar.m_socket->sendAData(0,sendMessage,len + 8);
				}
				XMem::XDELETE_ARRAY(tempMessage);
			}
			break;
		case LOGBOOK_MODE_SIMPLE:
			if(pPar.popAMessage(&tempMessage))
			{//��ģʽֱ�ӽ�����д�뵽�ļ�
				if(pPar.m_logLevel == LB_LEVEL_CONSOLE
					|| pPar.m_logLevel == LB_LEVEL_ALL)
					printf("%s\n",tempMessage);
				if(pPar.m_logLevel == LB_LEVEL_FILE
					|| pPar.m_logLevel == LB_LEVEL_ALL)
					fprintf(pPar.m_logBookFile,"%s\n",tempMessage);	//���뻻�з���

				XMem::XDELETE_ARRAY(tempMessage);
				if(pPar.m_needFlushFile) fflush(pPar.m_logBookFile);	//ǿ�ƽ��������뵽���̻���
				//_commit(fileno(pPar.m_logBookFile));
				//fsync(fileno(pPar.m_logBookFile));
			}
			break;
		}
        XEE::sleep(1);
	}
	XMem::XDELETE_ARRAY(sendMessage);
	pPar.m_isReleased = STATE_END;
#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
XLogbook::XLogbook()
	:m_isInited(XFalse)
	, m_isReleased(STATE_BEFORE_START)
	, m_workMode(LOGBOOK_MODE_SIMPLE)
	, m_logBookFile(NULL)
	, m_LogInfo(NULL)
	, m_logInfoPoint(0)
	, m_logInfoPushPoint(0)
	, m_logInfoPopPoint(0)
	, m_needFlushFile(XFalse)
	, m_isEnable(XTrue)
	, m_logServerFilename("../LogServer/LogServer.exe")
	, m_clientID(0)
	, m_logLevel(LB_LEVEL_ALL)
{
	m_socket = XMem::createMem<XSocketEx>();
}
XLogbook::~XLogbook()
{
	release();
	waitThreadEnd(m_isReleased);
	XMem::XDELETE(m_socket);
}
}