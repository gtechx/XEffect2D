#ifndef _JIA_XLOGBOOK_
#define _JIA_XLOGBOOK_
#include "stdio.h"
#include <memory>
#include "string.h"
#include "XOSDefine.h"
#include "XBasicClass.h"
#include "XBasicFun.h"
#include "XSocket.h"

#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
using namespace std;
//������һ���������ʱд��־����
//�Ż�˵����
//��־�ཫ��ʹ����������ģʽ
//A����־�ļ��Ὣ���ɵ���־���ݱ��浽ָ�����ļ�
//B����ʱ����־��Ϣ���͸���־������������������־�ڴ汣�浽�ʵ����ļ���
//B��������������־��Ϣ���Լ�ʱ�ı��棬���㷢��crash����־��ϢҲ���ᶪʧ����A��ʽ�кܶ�����
//����B��ʽ��Ҫ������֧�֣����û�����ӵ�����������Ĭ�Ϸ���Aģʽ

//����A��ʽĿǰ��Ҫ�ȵ���ջ����֮��ŻὫ��־��Ϣһ����д�뵽��־�ļ������Ըĳɶ��߳����ߵ�
//��ʽ����־��Ϣд�뵽��־�ļ����ô��ǿ��Լ���ͻ���Ĵ�������־��Ϣд�붯����ʹ����־�����
//��ƽ�����������̻߳����һЩ�����Ӱ�죬��Ҫ��ȶ
//��Ϣ�ۻ�����δ����أ��������ȴ���

//��־���ݵĸ�ʽΪ��
//������ʱ�������|LOGID|Message

#define MAX_RECORD_SUM 256		//�����־�ļ��������Ϣ�����������Щ���ݶ�ʧ
#define MAX_RECORD_LENGTH 512    //��ʼlog��Ϣ�ĳ��ȣ�Ŀǰ���Ȼ����ʵ�ʵ���Ҫ���仯
//YYYYMMDD-hhmmss-xxx:
//01234567890123456789
#define RECORD_HEAD_LENGTH 20    //��Ϣʱ����ĳ���Ϊ20
#define OUTTO_CONSOLE (1)	//�Ƿ�ͬʱ���������̨

//��־��Ϣ�Ľṹ��
struct _XLogbookInfo
{
    _XBool isEnable;                        //������־�Ƿ���Ч
    char *logMessage;                    //��־����Ϣ��
	_XLogbookInfo()
		:isEnable(XFalse)
		,logMessage(NULL)
	{}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++
//logserver��һЩ����
enum _XLogBookMode
{
    LOGBOOK_MODE_SIMPLE,    //��ģʽ
    LOGBOOK_MODE_SERVER    //log������ģʽ
};
#define LOG_SERVER_NAME "XLogBook_Server"
#define LOG_SERVER_PORT 20086
#define LOG_SERVER_IP "127.0.0.1"
//��������Ľ���Э������(��Сд����):
//xxxxx:message body
//sys00:file name        :�ͻ����������������־�ļ���
//sys01:file name        :��������ͻ��˷�����־�ļ���
//sys02:NULL            :�ͻ��˸��߷�������־�ļ���������
//sys03:NULL            :���������߿ͻ�����־�ļ���������
//msg00:message body    :�ͻ��˽���־���ݷ���������
//--------------------------------------------------
class _XLogbook
{
    //+++++++++++++++++++++++++++++++++++++++++++
    //������Ҫ�������ΪSingletonģʽ
protected:
    _XLogbook();
    _XLogbook(const _XLogbook&);
	_XLogbook &operator= (const _XLogbook&);
	virtual ~_XLogbook(){releaseLog();}
public:
    static _XLogbook& GetInstance()
	{
		static _XLogbook m_instance;
		return m_instance;
	}
    //-------------------------------------------
private:
    _XLogbookInfo *m_LogInfo;    //��־������
    int m_logInfoPoint;
    FILE *m_logBookFile;		//��־�ļ����ļ�ָ��
    _XBool m_isInited;				//�Ƿ��ʼ��
    _XThreadState m_isReleased;            //�����־�ļ����Ƿ��Ѿ��ͷŹ�
    //������ʹ��log��������һЩ����
    _XLogBookMode m_workMode;    //��־�Ĺ���ģʽ
    _XSocket m_socket;            //���粿�ֵ�һЩ����
	char *m_tmpBuff;			//������ʱ�洢����
	_XCritical m_locker;

    //+++++++++++++++++++++++++++++++++++++++
    //������A�����н���ʵ��
    int m_logInfoPushPoint;
    int m_logInfoPopPoint;
    _XBool pushAMessage(const char * message);    //�����������һ����Ϣ���������������Ƿ�ɹ�
    _XBool popAMessage(char ** message);    //�Ӷ�����ȡ��һ����Ϣ return is there a message.
    //�߳̾��
    pthread_t m_outputLogMessageThreadP;
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI outputLogMessageThread(void * pParam);    //�����־��Ϣ���ļ�����log���������߳�
#endif

#ifdef XEE_OS_LINUX
    static void *outputLogMessageThread(void * pParam);    //�����־��Ϣ���ļ�����log���������߳�
#endif
    //---------------------------------------

    void popAllLogMessage();    //�Ӷ����е���Ϣ�������־�ļ���
    _XBool doubleStringLength(char ** message,int messageSize);    //�����ڴ�Ŀ��һ��
public:
    _XBool initLog(const char *fileName);
    void releaseLog();
    void addLogInfoStr(const char *p);
    void addLogInfoNull(const char *p,...);    //Ŀǰֻ��֧�ֵ��򵥵�%s %d %c���������ʽ��û�����ӵĴ���
     
    void addLogInfoExp(_XBool exp,const char *p,...);    //Ŀǰֻ��֧�ֵ��򵥵�%s %d %c���������ʽ��û�����ӵĴ���
    //������������ʽ
    void setWorkMode(_XLogBookMode temp)
    {
        if(m_isInited) return;
        m_workMode = temp;
    }
};

#if WITH_LOG
#define LogStr _XLogbook::GetInstance().addLogInfoStr
#define LogNull _XLogbook::GetInstance().addLogInfoNull
#define LogExp _XLogbook::GetInstance().addLogInfoExp
#else
#define LogStr(p) (void)(p)
#define LogNull (void)
#define LogExp (void)
#endif

inline void _XLogbook::addLogInfoStr(const char *p)
{
    pushAMessage(p);
}
#endif