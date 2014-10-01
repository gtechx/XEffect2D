#ifndef _JIA_SERIALPORT_
#define _JIA_SERIALPORT_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
#include <windows.h>
#include "XOSDefine.h"
#include "XBasicClass.h"
#include "../XTimer.h"
#include "../XLogBook.h"
//�������ݵĽ���ģʽ
enum _XSerialPortMode
{
	SP_MODE_MANUAL,	//��ѯģʽ
	SP_MODE_AUTO,	//�¼�ģʽ
};

#define SP_WITH_STATISTICS	//�Ƿ�֧��ͳ��
#define SP_RECORDE_FILENAME ("serialPortRecv.dat")

class _XSerialPort
{
public:
	_XSerialPort()
		:m_hIDComDev(NULL)
		,m_bOpened(XFalse)
		,m_mode(SP_MODE_MANUAL)
		,m_funRecv(NULL)
		,m_pClass(NULL)
		,m_withStatistics(false)
		,m_withRecord(XFalse)
		,m_withPlay(XFalse)
		,m_recordFile(NULL)
		,m_playFile(NULL)
		,m_waitingDataTime(0)
	{
		memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
		memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));
	}
	~_XSerialPort()
	{
		endRecord();
		stopPlay();
		close();
	}

	_XBool open(int nPort = 1,int nBaud = 9600,int nParity = 0,_XSerialPortMode mode = SP_MODE_MANUAL);	//�򿪴����豸
	_XBool close();//�ر��豸

	int readData(void *buff, int limit);
	int sendData(const unsigned char *,int size);
	int readDataWaiting();	// ��ѯ���������Ƿ���δ��ȡ������

	_XBool isOpened() const {return m_bOpened;}
	void setCallBackFun(void (* funRecv)(void *,unsigned char *,int),void *pClass)
	{
		m_funRecv = funRecv;
		m_pClass = pClass;
	}
protected:
	//_XBool writeCommByte(unsigned char);	// �ڲ�ʵ�֣��򴮿�д����

	HANDLE m_hIDComDev;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	_XBool m_bOpened;
	_XSerialPortMode m_mode;

	_XThreadState m_threadState;
	pthread_t m_recvThread;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�

	void (*m_funRecv)(void *,unsigned char *,int);	//�յ����ݵĻص�����
	void *m_pClass;
	int m_waitingDataTime;	//�ȴ����ݽ��յ�ʱ��
public:
	void setWaitingDataTime(int tmp){m_waitingDataTime = tmp;}
	int getWaitingDataTime() const{return m_waitingDataTime;}
private:
	//������ͳ����Ϣ
	bool m_withStatistics;	//�Ƿ�����ͳ��
	int m_sendTimesAll;		//�ܵķ��ʹ���
	int m_sendBytesAll;		//�ܵķ����ֽ���
	int m_recvTimesAll;		//�ܵĽ��մ���
	int m_recvBytesAll;		//�ܵĽ����ֽ���

	int m_statisticsTimer;	//ͳ�Ƽ�ʱ
	int m_sendTimesNow;
	int m_sendBytesNow;
	int m_recvTimesNow;
	int m_recvBytesNow;

	float m_sendTimesAvg;	//ƽ������
	float m_sendBytesAvg;
	float m_recvTimesAvg;
	float m_recvBytesAvg;
public:
	int getSendTimesAll()const{return m_sendTimesAll;}
	int getSendBytesAll()const{return m_sendBytesAll;}
	int getRecvTimesAll()const{return m_recvTimesAll;}
	int getRecvBytesAll()const{return m_recvBytesAll;}
	float getSendTimesAvg()const{return m_sendTimesAvg;}
	float getSendBytesAvg()const{return m_sendBytesAvg;}
	float getRecvTimesAvg()const{return m_recvTimesAvg;}
	float getRecvBytesAvg()const{return m_recvBytesAvg;}
	void setStatistics(bool flag);
	void update();
	//����ʵ�ִ������ݵ�¼�ƵĹ���
private:	//ע��Ŀǰֻ¼�ƽ��չ��ܣ���¼�Ʒ���
	_XBool m_withRecord;	//�Ƿ�¼��
	_XBool m_withPlay;		//�Ƿ񲥷�
	FILE *m_recordFile;		//¼���ļ���ָ��
	FILE *m_playFile;		//�����ļ���ָ��
	_XCritical m_recordMutex;
	void recordRecvAData(const unsigned char *data,int len)
	{//���ļ���д���ȡ��һ������
		if(!m_withRecord || m_recordFile == NULL) return;
		fprintf(m_recordFile,"recv:");	//д���־
		fprintf(m_recordFile,(sysTime2String(getTimeMs()) + ":").c_str());	//д��ʱ��
		for(int i = 0;i < len;++ i)
		{
			fprintf(m_recordFile,"0x%02x ",data[i]);
		}
		fprintf(m_recordFile,"\n");
	}
	_XBool readADataFromRecord(std::string &oparetion,_XSystemTime &t,unsigned char *date,int &len)	//�Ӽ�¼�ļ��ж�ȡһ������
	{
		len = 0;
		if(m_playFile == NULL) return XFalse;
		static char lineData[2048];
		if(fgets(lineData,2048,m_playFile) == NULL) return XFalse;
		int ret = getCharPosition(lineData,':');
		if(ret <= 0) return XFalse;	//����
		lineData[ret] = '\0';
		oparetion = lineData;
		int timeEnd = getCharPosition(lineData + ret + 1,':');
		if(timeEnd <= 0) return XFalse;	//����
		lineData[ret + 1 + timeEnd] = '\0';
		t = string2sysTime(lineData + ret + 1);
		if(oparetion == "recv")
		{//�����ȡʱ�������
			int offset = ret + 1 + timeEnd;
			for(len = 0;;++ len)
			{
				if(sscanf(lineData + offset + 1,"0x%02x ",&date[len]) != 1) break;
				ret = getCharPosition(lineData + offset + 1,' ');
				if(ret <= 0) break;
				offset += ret + 1;
			}
		}
		return true;
	}
	int m_playTimer;	//�طŵļ�ʱ��
	_XSystemTime m_upTime;
	_XSystemTime m_nextTime;
	unsigned char m_nextData[1024];
	int m_nextDataLen;
	std::string m_oparetion;
public:
	_XBool getWithRecord() const {return m_withRecord;}
	_XBool getWithPlay() const {return m_withPlay;}
	_XBool startRecord()	//��ʼ¼��
	{
		if(m_withRecord || m_withPlay) return XFalse;
		m_recordMutex.Lock();
		if((m_recordFile = fopen(SP_RECORDE_FILENAME,"w")) == NULL) return XFalse;	//��¼���ļ�
		fprintf(m_recordFile,("start:" + sysTime2String(getTimeMs()) + ":\n").c_str());	//д��ʱ��
		m_withRecord = XTrue;
		m_recordMutex.Unlock();
		return XTrue;
	}
	_XBool endRecord()		//����¼��
	{
		if(!m_withRecord) return XFalse;
		m_recordMutex.Lock();
		fprintf(m_recordFile,("stop:" + sysTime2String(getTimeMs()) + ":\n").c_str());	//д��ʱ��
		fclose(m_recordFile);
		m_recordFile = NULL;
		m_withRecord = XFalse;
		m_recordMutex.Unlock();
		return XTrue;
	}
	_XBool startPlay()		//��ʼ����
	{
		if(m_withPlay || m_withRecord) return XFalse;
		m_playTimer = 0;
		//��¼���ļ�(��δ���)
		if((m_playFile = fopen(SP_RECORDE_FILENAME,"r")) == NULL) return XFalse;	//��¼���ļ�
		m_withPlay = XTrue;
		if(!readADataFromRecord(m_oparetion,m_upTime,m_nextData,m_nextDataLen))
		{//�������ʧ����ֱ�ӹر�
			LogStr("record file error!");
			stopPlay();
		}
		if(m_oparetion != "start")
		{
			LogStr("record file error!");
			stopPlay();
		}
		//��ȡ��һ������
		if(!readADataFromRecord(m_oparetion,m_nextTime,m_nextData,m_nextDataLen))
		{//�������ʧ����ֱ�ӹر�
			LogStr("record file error!");
			stopPlay();
		}
		return XTrue;
	}
	_XBool stopPlay()		//��������
	{
		if(!m_withPlay) return XFalse;
		//�ر������ļ�(��δ���)
		fclose(m_playFile);
		m_withPlay = XFalse;
		return XTrue;
	}
};

#endif