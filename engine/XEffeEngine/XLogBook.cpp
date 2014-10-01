//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XLogBook.h"
#include "XEffeEngine.h"
#include "XTimer.h"
//#include "assert.h"

#ifdef XEE_OS_LINUX
#include <stdarg.h>
#endif

#define DEFAULT_LOG_FILE_NAME ("logBook")

_XBool _XLogbook::initLog(const char *fileName)
{
    if(m_isInited) return XFalse;    //���֮ǰ�Ѿ���ʼ������������ֱ�ӷ���
	if(XEE::windowData.withLog == 0) return XFalse;	//��ʹ��log
    if(fileName == NULL || strlen(fileName) <= 0) return XFalse;
	std::string tmpFileName = fileName + std::string("_") + sysTime2String(getTimeMs()) + ".txt";
	m_LogInfo = createArrayMem<_XLogbookInfo>(MAX_RECORD_SUM);
	if(m_LogInfo == NULL) return XFalse;
    m_logInfoPoint = 0;
	m_tmpBuff = createArrayMem<char>(MAX_RECORD_LENGTH);
	if(m_tmpBuff == NULL)
	{
		XDELETE_ARRAY(m_LogInfo);
		return XFalse;
	}

	int ret = 1;
    if(m_workMode == LOGBOOK_MODE_SERVER)
    {//���ӷ�����
	//	if(m_socket.xCreateClient(LOG_SERVER_IP,LOG_SERVER_PORT) == 0)
	//	{
	//		m_workMode = XLOGBOOK_MODE_SIMPLE;
	//	}
        if(m_socket.connectToNet(LOG_SERVER_PORT,LOG_SERVER_NAME) == 0)
        {//���ӷ�����ʧ�ܣ�ʹ��Ĭ�ϵļ�ģʽ(Ŀǰ������ģ�飬����������Ӳ���������
			//�������Զ����������������ԱȽ���Ҫ��һ���Ĵ���)
            m_workMode = LOGBOOK_MODE_SIMPLE;
        }else
        {//���ӷ������ɹ�֮������������ͳ�ʼ����Ϣ
			if(m_socket.getIsServer() == 2)
			{
				int len = strlen(tmpFileName.c_str()) + 1;
				char *tempMessage = NULL;
				tempMessage = createArrayMem<char>(len + 6);
				if(tempMessage == NULL) ret = 0;

				if(ret != 0)
				{
					strcpy(tempMessage,"sys00:");
					strcat(tempMessage,tmpFileName.c_str());
					m_socket.sendDataToNet(0,tempMessage,strlen(tempMessage) + 1);
					XDELETE_ARRAY(tempMessage);
				}else
				{
					XDELETE_ARRAY(m_LogInfo);
					return XFalse;
				}
			}else
			{//���ӷ�����ʧ�ܣ���ת�ɷǷ�����ģʽ
				m_socket.disconnect();	//�Ͽ���������
				m_workMode = LOGBOOK_MODE_SIMPLE;
			}
        }
    }
    if(m_workMode == LOGBOOK_MODE_SIMPLE)
    {
		if((m_logBookFile = fopen(tmpFileName.c_str(),"a")) == NULL)
        {
			if((m_logBookFile = fopen(tmpFileName.c_str(),"w")) == NULL)
            {
				XDELETE_ARRAY(m_LogInfo);
                return XFalse;
            }
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
		XDELETE_ARRAY(m_LogInfo);
        return XFalse;
    }

    m_isInited = XTrue;    //��ǳ�ʼ���Ѿ����
	m_isReleased = STATE_BEFORE_START;
    return XTrue;
}
void _XLogbook::releaseLog()
{//��û�б���ļ�¼,���浽�ļ���,���ر��ļ�
    if(!m_isInited) return;
    if(m_isReleased == STATE_BEFORE_START)
    {
        m_isReleased = STATE_SET_TO_END;
        while(true)
        {//�ȴ����߳̽���
			if(m_isReleased == STATE_END) break;
            mySleep(1000);
        }

        if(m_workMode == LOGBOOK_MODE_SIMPLE)
        {//��ģʽ����Ҫ����־�ļ���д�������Ϣ
            fprintf(m_logBookFile,"------ End Of File! ------");
			fflush(m_logBookFile);	//ǿ�ƽ��������뵽���̻���
            fclose(m_logBookFile);
        }else
		{//�ر���������
			m_socket.disconnect();
		}
		XDELETE_ARRAY(m_LogInfo);
		m_isInited = XFalse;
        m_isReleased = STATE_SET_TO_END;    //�����Դ�Ѿ�����
    }
}
void _XLogbook::popAllLogMessage()
{//record the date to file
	if(!m_isInited || m_logBookFile == NULL) return;
    for(int i = 0;i < MAX_RECORD_SUM;i ++)
    {
        if(m_LogInfo[i].isEnable)
        {
            fprintf(m_logBookFile,"%s",m_LogInfo[i].logMessage);
#if OUTTO_CONSOLE
			printf("%s\n",m_LogInfo[i].logMessage);
#endif
			XDELETE_ARRAY(m_LogInfo[i].logMessage);
            m_LogInfo[i].isEnable = XFalse;
			fflush(m_logBookFile);	//ǿ�ƽ��������뵽���̻���
        }
    }
}
_XBool _XLogbook::doubleStringLength(char ** message,int messageSize)
{
	if(messageSize <= 0 || message == NULL) return XFalse;
    int len = messageSize;
    char * tempData = (*message);
    (*message) = NULL;

    messageSize = messageSize << 1;
    (*message) = createArrayMem<char>(messageSize);
    if((*message) == NULL)
    {
		(* message) = tempData;	//���ı�ԭ��������
        printf("Memory Error:File:%s,Line:%d\n",__FILE__,__LINE__);
        return XFalse;
    }
    memcpy((*message),tempData,len);
	XDELETE_ARRAY(tempData);
    return XTrue;
}
//void _XLogbook::addLogInfoNull(const char *p,...)
//{
//    if(!m_isInited) initLog(DEFAULT_LOG_FILE_NAME);
//
//    int tempMessageSize = MAX_RECORD_LENGTH;
//    char * tempMessage = createArrayMem<char>(tempMessageSize);
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
//            p++;
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
//							XDELETE_ARRAY(tempMessage);
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
//								XDELETE_ARRAY(tempMessage);
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
//                    sprintf(buf,"%d",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XDELETE_ARRAY(tempMessage);
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
//                    sprintf(buf,"%f",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XDELETE_ARRAY(tempMessage);
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
//							XDELETE_ARRAY(tempMessage);
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
//							XDELETE_ARRAY(tempMessage);
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
//					XDELETE_ARRAY(tempMessage);
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
//	XDELETE_ARRAY(tempMessage);
//}
void _XLogbook::addLogInfoNull(const char *p,...)
{
    if(!m_isInited && !initLog(DEFAULT_LOG_FILE_NAME)) return;

	m_locker.Lock();
	va_list args;
    va_start(args,p);
    vsprintf(m_tmpBuff,p,args);
	va_end(args);

    pushAMessage(m_tmpBuff);
	m_locker.Unlock();
}
//void _XLogbook::addLogInfoExp(_XBool exp,const char *p,...)
//{
//    if(exp) return;//��������Ҫ��ӡ������Ϣ
//    if(!m_isInited)  initLog(DEFAULT_LOG_FILE_NAME);
//
//    int tempMessageSize = MAX_RECORD_LENGTH;
//    char * tempMessage = createArrayMem<char>(tempMessageSize);
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
//            p++;
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
//							XDELETE_ARRAY(tempMessage);
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
//								XDELETE_ARRAY(tempMessage);
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
//                    sprintf(buf,"%d",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XDELETE_ARRAY(tempMessage);
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
//                    sprintf(buf,"%f",data);
//                    if(charPoint < (int)(tempMessageSize - 1 - strlen(buf)))
//                    {
//                        memcpy(&(tempMessage[charPoint]),buf,strlen(buf));
//                    }else
//                    {
//                        if(doubleStringLength(&tempMessage,tempMessageSize) == 0) 
//                        {
//							XDELETE_ARRAY(tempMessage);
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
//							XDELETE_ARRAY(tempMessage);
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
//							XDELETE_ARRAY(tempMessage);
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
//					XDELETE_ARRAY(tempMessage);
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
//	XDELETE_ARRAY(tempMessage);
//}
void _XLogbook::addLogInfoExp(_XBool exp,const char *p,...)
{
    if(!m_isInited && !initLog(DEFAULT_LOG_FILE_NAME)) return;
    if(exp) return;//��������Ҫ��ӡ������Ϣ

	m_locker.Lock();
	va_list args;
    va_start(args,p);
    vsprintf(m_tmpBuff,p,args);
	va_end(args);

    pushAMessage(m_tmpBuff);
	m_locker.Unlock();
}
_XBool _XLogbook::pushAMessage(const char * message)    //�����������һ����Ϣ    
{
    if(!m_isInited && !initLog(DEFAULT_LOG_FILE_NAME)) return XFalse;

    int len = strlen(message) + 1;
    if(len <= 0) return XFalse;    //no message
    //++++++++++++++++++++++++++++++++++++++++
    //Ϊÿ����־����ʱ���
    _XSystemTime tempTime;
	getTimeMs(tempTime);
    char *tempMessage = createArrayMem<char>(len + RECORD_HEAD_LENGTH);
    if(tempMessage == NULL) return XFalse;

    sprintf(tempMessage,"%04d%02d%02d-%02d%02d%02d-%03d:",tempTime.year,tempTime.month,tempTime.day,
        tempTime.hour,tempTime.minute,tempTime.second,tempTime.millisecond);
    strcat(tempMessage,message);
    len += RECORD_HEAD_LENGTH;
    //----------------------------------------
    //push message
    if(!m_LogInfo[m_logInfoPushPoint].isEnable)
    {//can input
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = XTrue;
    }else
    {//full
		//delete now message
        m_LogInfo[m_logInfoPushPoint].isEnable = XFalse;
		XDELETE_ARRAY(m_LogInfo[m_logInfoPushPoint].logMessage);
		m_LogInfo[m_logInfoPushPoint].logMessage = tempMessage;
        m_LogInfo[m_logInfoPushPoint].isEnable = XTrue;
    }
    //move point
    ++ m_logInfoPushPoint;
    if(m_logInfoPushPoint >= MAX_RECORD_SUM)  m_logInfoPushPoint = 0;

	return XTrue;
}
_XBool _XLogbook::popAMessage(char ** message)    //�Ӷ�����ȡ��һ����Ϣ
{
    //if(m_isInited == 0) initLog(DEFAULT_LOG_FILE_NAME);
	if(!m_isInited) return XFalse;

	if(m_LogInfo[m_logInfoPopPoint].isEnable)
    {//have message
        //pop message
		XDELETE_ARRAY((* message));	//ɾ��ԭ�е�����
        (* message) = m_LogInfo[m_logInfoPopPoint].logMessage;
        m_LogInfo[m_logInfoPopPoint].isEnable = XFalse;
        m_LogInfo[m_logInfoPopPoint].logMessage = NULL;
        //move point
        ++ m_logInfoPopPoint;
        if(m_logInfoPopPoint >= MAX_RECORD_SUM) m_logInfoPopPoint = 0;
        return XTrue;
    }else
    {//empty
        return XFalse;
    }
}
#ifdef XEE_OS_WINDOWS
DWORD WINAPI _XLogbook::outputLogMessageThread(void * pParam)
#endif
#ifdef XEE_OS_LINUX
void *_XLogbook::outputLogMessageThread(void * pParam)
#endif
{
    _XLogbook &pPar = *(_XLogbook *) pParam;

    char *tempMessage = NULL;
    int len = 0;
    char *sendMessage = NULL;

    int recvOrder = 0;
    int recvID = 0;
    int recvLength = 0;
    char *recvMessage = NULL;
    while(true)
    {
        //++++++++++++++++++++++++++++++++++++++++++++
        //����ӷ��������յ�����Ϣ
        if(pPar.m_workMode == LOGBOOK_MODE_SERVER && pPar.m_socket.getDataFromNet(&recvOrder,&recvID,&recvMessage,&recvLength) != 0)
        {//���ܵ�����
            if(recvOrder == 0)
            {//�ӷ��������յ�����Ϣ������ֱ�Ӷ���
                if(recvMessage[0] == 's' &&
                    recvMessage[1] == 'y' &&
                    recvMessage[2] == 's' &&
                    recvMessage[3] == '0' &&
                    recvMessage[4] == '1' &&
                    recvMessage[5] == ':')
                {//�����������Լ���ʵ������޸�����־�ı����ļ��������ļ��������������ｫ������ʵ��ʹ�õ���־�ļ������浽����
                    FILE *fp = NULL;
					std::string tmpFileName = DEFAULT_LOG_FILE_NAME + std::string("_") + sysTime2String(getTimeMs()) + ".txt";
					if((fp = fopen(DEFAULT_LOG_FILE_NAME,"a")) == NULL)
                    {
						if((fp = fopen(DEFAULT_LOG_FILE_NAME,"w")) == NULL)
                        {//�ļ���ʧ�ܣ�ֱ�����������Ϣ
                            continue;
                        }
                    }
                    fprintf(fp,"%s",recvMessage + 6);
                    fclose(fp);
                }
            }
        }
        //--------------------------------------------
        if(pPar.popAMessage(&tempMessage))
        {//����Ƴ���Ϣ�ɹ�
            if(pPar.m_workMode == LOGBOOK_MODE_SERVER)
            {//�����ݷ��͵�������
                len = strlen(tempMessage) + 1;
				sendMessage = createArrayMem<char>(len + 6);
                if(sendMessage == NULL)
                {
					XDELETE_ARRAY(tempMessage);
                    continue;
                }

#if OUTTO_CONSOLE
				printf("%s\n",tempMessage);
#endif
                strcpy(sendMessage,"msg00:");
                strcat(sendMessage,tempMessage);
                pPar.m_socket.sendDataToNet(0,sendMessage,strlen(sendMessage) + 1);

				XDELETE_ARRAY(sendMessage);
				XDELETE_ARRAY(tempMessage);
            }
            if(pPar.m_workMode == LOGBOOK_MODE_SIMPLE)
            {//��ģʽֱ�ӽ�����д�뵽�ļ�
                fprintf(pPar.m_logBookFile,"%s\n",tempMessage);	//���뻻�з���
#if OUTTO_CONSOLE
				printf("%s\n",tempMessage);
#endif
				XDELETE_ARRAY(tempMessage);
				fflush(pPar.m_logBookFile);	//ǿ�ƽ��������뵽���̻���
				//_commit(fileno(pPar.m_logBookFile));
				//fsync(fileno(pPar.m_logBookFile));
            }
        }else
        {//��������е������Ѿ�ȫ��������ɣ����ж��Ƿ�Ӧ�ý�������߳�
            if(pPar.m_isReleased == STATE_SET_TO_END)
			//if(pPar.m_isReleased != 0)
            {
                //����������ͽ�����Ϣ
                if(pPar.m_workMode == LOGBOOK_MODE_SERVER)
                {
                    char tempMessageX[] = "sys02:";
                    pPar.m_socket.sendDataToNet(0,tempMessageX,strlen(tempMessageX) + 1);
                }
                pPar.m_isReleased = STATE_END;
                break;
            }
        }
        mySleep(1000);
    }

#ifdef XEE_OS_WINDOWS
    return 1;
#endif
}
_XLogbook::_XLogbook()
:m_isInited(XFalse)
,m_isReleased(STATE_BEFORE_START)
,m_workMode(LOGBOOK_MODE_SIMPLE)
,m_logBookFile(NULL)
,m_LogInfo(NULL)
,m_logInfoPoint(0)
,m_logInfoPushPoint(0)
,m_logInfoPopPoint(0)
{}