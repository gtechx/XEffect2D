#ifndef _JIA_XLOGBOOK_
#define _JIA_XLOGBOOK_
#include "XOSDefine.h"
#include "XThread.h"
#include "XCritical.h"
#include <string>
namespace XE{
//#ifdef XEE_OS_WINDOWS
//#include "windows.h"
//#endif
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//������һ���������ʱд��־����
//�Ż�˵����
//��־�ཫ��ʹ����������ģʽ
//A����־�ļ��Ὣ���ɵ���־���ݱ��浽ָ�����ļ�
//B����ʱ����־��Ϣ���͸���־������������������־���ݱ��浽�ʵ����ļ���
//B��������������־��Ϣ���Լ�ʱ�ı��棬���㷢��crash����־��ϢҲ���ᶪʧ����A��ʽ�кܶ�����
//����B��ʽ��Ҫ������֧�֣����û�����ӵ�����������Ĭ�Ϸ���Aģʽ

//����A��ʽĿǰ��Ҫ�ȵ���ջ����֮��ŻὫ��־��Ϣһ����д�뵽��־�ļ������Ըĳɶ��߳����ߵ�
//��ʽ����־��Ϣд�뵽��־�ļ����ô��ǿ��Լ���ͻ���Ĵ�������־��Ϣд�붯����ʹ����־�����
//��ƽ�����������̻߳����һЩ�����Ӱ�죬��Ҫ��ȶ
//��Ϣ�ۻ�����δ����أ��������ȴ���

//��־���ݵĸ�ʽΪ��
//������ʱ�������|LOGID|Message

//��־��Ϣ�Ľṹ��
struct XLogbookInfo
{
    XBool isEnable;                    //������־�Ƿ���Ч
    char *logMessage;					//��־����Ϣ��
//	XLogLevel level;					//��־��Ϣ�ĵȼ�(��δ��Ч)
	XLogbookInfo()
		:isEnable(XFalse)
		,logMessage(NULL)
//		,level(LOG_LEVEL_MESSAGE)
	{}
};
//++++++++++++++++++++++++++++++++++++++++++++++++++
//logserver��һЩ����
enum XLogBookMode
{
    LOGBOOK_MODE_SIMPLE,    //��ģʽ
    LOGBOOK_MODE_SERVER    //log������ģʽ
};
enum XLogBookLevel
{
	LB_LEVEL_NULL = 0,	//����־���
	LB_LEVEL_CONSOLE,	//��̨��־���
	LB_LEVEL_FILE,		//�ļ���־���
	LB_LEVEL_ALL,		//���ں�̨���Ҳ����־�ļ����
};
//��������Ľ���Э������(��Сд����):
enum XLogServerDataType
{
	LOG_DATA_TYPE_NULL,		//��Ч����Ϣ
	LOG_DATA_TYPE_ID,		//��������ͻ��˷���ID��Ϣ
	LOG_DATA_TYPE_INFO,		//�ͻ�����������˷��͵Ŀͻ�����Ϣ
	LOG_DATA_TYPE_OVER,		//�ͻ�����������˷��Ϳͻ��˽���
	LOG_DATA_TYPE_MESSAGE,	//��־����
	LOG_DATA_TYPE_MAX,		//������Чֵ
};
enum XLogLevel	//��־��Ϣ����Ҫ�̶�(��δ��Ч)
{
	LOG_LEVEL_NULL,		//��͵ȼ�
	LOG_LEVEL_MESSAGE,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
};
//xxxxx:message body
//sys00:file name			//�ͻ����������������ؿͻ��˵���Ϣ
//sys80:file name			//��������ͻ��˷�����־�ļ���(δʹ��)
//sys01:NULL				//�ͻ��˸��߷�������־�ļ���������
//sys81:NULL				//���������߿ͻ�����־�ļ���������(δʹ��)
//sys82:ID					//��������ͻ��˷��Ϳͻ��˵�ID
//msg00:ID message body		//�ͻ��˽���־���ݷ�����������IDΪ�ͻ��˵�ID
//--------------------------------------------------
class XSocketEx;
class XLogbook
{
public:
	XLogbook();
	virtual ~XLogbook();
protected:
	XLogbook(const XLogbook&);
	XLogbook &operator= (const XLogbook&);
public:
	static const int m_logServerPort = 20086;
private:
#define LOG_SERVER_NAME "XLogBook_Server"	//ʹ����������ھ��������Զ����ӷ�����
#define LOG_SERVER_IP "127.0.0.1"			//ʹ�����ֻ���ڱ������ӷ�����
	static const int m_maxRecordSum = 256;		//�����־�ļ��������Ϣ�����������Щ���ݶ�ʧ
	static const int m_maxRecordLength = 512;	//��ʼlog��Ϣ�ĳ��ȣ�Ŀǰ���Ȼ����ʵ�ʵ���Ҫ���仯
	//YYYYMMDD-hhmmss-xxx:
	//01234567890123456789  
	static const int m_recordHeadLength = 20;	//��Ϣʱ����ĳ���Ϊ20

	XLogBookLevel m_logLevel;
	XBool m_isEnable;				//�Ƿ�ʹ����־���
	XLogbookInfo *m_LogInfo;		//��־������
	int m_logInfoPoint;
	FILE *m_logBookFile;			//��־�ļ����ļ�ָ��
	XBool m_isInited;				//�Ƿ��ʼ��
	XThreadState m_isReleased;            //�����־�ļ����Ƿ��Ѿ��ͷŹ�
	//������ʹ��log��������һЩ����
	XLogBookMode m_workMode;		//��־�Ĺ���ģʽ
	XSocketEx *m_socket;				//�������ӵ��׽���	
	char *m_tmpBuff;				//������ʱ�洢����
	XCritical m_locker;
	//�¼��������
	std::string m_logServerFilename;	//�������ľݶ�·��
	int m_clientID;						//��ǰ��־�ͻ��˵�ΨһID
	std::string m_filename;

	//+++++++++++++++++++++++++++++++++++++++
	//������A�����н���ʵ��
	int m_logInfoPushPoint;
	int m_logInfoPopPoint;
	XBool pushAMessage(const char * message);    //�����������һ����Ϣ���������������Ƿ�ɹ�
	XBool popAMessage(char ** message);    //�Ӷ�����ȡ��һ����Ϣ return is there a message.
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
	//XBool doubleStringLength(char ** message,int messageSize);    //�����ڴ�Ŀ��һ��
	XBool m_needFlushFile;
public:
	XBool init(const char *fileName);
	void release();
	void addLogInfoStr(const char *p);
	void addLogInfoNull(const char *p, ...);    //Ŀǰֻ��֧�ֵ��򵥵�%s %d %c���������ʽ��û�����ӵĴ���
	void setLogLevel(XLogBookLevel level) { m_logLevel = level; }

	void addLogInfoExp(XBool exp, const char *p, ...);    //Ŀǰֻ��֧�ֵ��򵥵�%s %d %c���������ʽ��û�����ӵĴ���
	//������������ʽ
	void setWorkMode(XLogBookMode temp)
	{
		if (m_isInited) return;
		m_workMode = temp;
	}
	void setServerFilename(const std::string& filename)
	{
		if (m_isInited) return;
		m_logServerFilename = filename;
	}
	void enable() { m_isEnable = XTrue; }		//ʹ����־���
	void disable() { m_isEnable = XFalse; }		//��ʱ�ر���־���
};
inline void XLogbook::addLogInfoStr(const char *p)
{
	if(!m_isEnable) return;
	m_locker.Lock();
    pushAMessage(p);
	m_locker.Unlock();
}
}
#endif