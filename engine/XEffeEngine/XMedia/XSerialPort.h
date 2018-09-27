#ifndef _JIA_SERIALPORT_
#define _JIA_SERIALPORT_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
#include "../XCommonDefine.h"
#include "../XTimer.h"
#include "../XXml.h"
#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
//�������ݵĽ���ģʽ
enum XSerialPortMode
{
	SP_MODE_MANUAL,	//����Ա�ֶ��������ݽ��պ���
	SP_MODE_AUTO,	//���߳��Զ���������
};
#define SP_WITH_STATISTICS	//�Ƿ�֧��ͳ��
#define SP_RECORDE_FILENAME "serialPortRecv.dat"
struct XSerialPortInfo	//����Ǽƻ�(��δ����)
{
	XSerialPortMode mode;	//����ģʽ���Ǳ���ģʽ
	int byteSize;	//����λ�Ŀ��4 - 8
	int nPort;	//�˿ں�
	int nBaud;	//������
	int nParity;	//У�鷽ʽ 0-4
	XBool withEventMode;	//�Ƿ�֧���¼�ģʽ,��withOverLapedΪfalseʱwithEventMode����Ϊtrue����ɲ��ܷ��ͺͲ����˳���bug
	XBool withOverLaped;	//�Ƿ�ʹ���첽�ķ�ʽ
	int stopBits;	//ֹͣλ
	int dtr;
	int rts;
	int waitingDataTime;	//�����ֽ�֮��ĵȴ�ʱ��(����)
	XSerialPortInfo()
		:mode(SP_MODE_MANUAL)
		, nPort(1)
		, nBaud(9600)
		, nParity(NOPARITY)
		, withEventMode(XFalse)
		, withOverLaped(XFalse)
		, stopBits(ONESTOPBIT)
		, dtr(DTR_CONTROL_ENABLE)
		, rts(RTS_CONTROL_ENABLE)
		, waitingDataTime(0)
		, byteSize(8)
	{}
	bool readFromFile(const char * filename);		//��xml�����ļ��ж�ȡ	//��δ���
	bool writeToFile(const char * filename);		//д�뵽�����ļ���		//��δ���
	bool readFromFile(TiXmlNode * node);			//��xml�����ļ��ж�ȡ	//��δ���
	bool writeToFile(TiXmlElement &element);		//д�뵽�����ļ���		//��δ���
	bool checkData() const;		//������ݵĺϷ���
};
class XSerialPort
{
public:
	XSerialPort()
		:m_hIDComDev(NULL)
		, m_bOpened(XFalse)
		, m_funRecv(NULL)
		, m_pClass(NULL)
		, m_withStatistics(false)
		, m_withRecord(XFalse)
		, m_withPlay(XFalse)
		, m_recordFile(NULL)
		, m_playFile(NULL)
	{
		memset(&m_overlappedRead, 0, sizeof(OVERLAPPED));
		memset(&m_overlappedWrite, 0, sizeof(OVERLAPPED));
	}
	~XSerialPort()
	{
		endRecord();
		stopPlay();
		close();
	}

	XBool open(int nPort = 1, int nBaud = 9600, int nParity = 0,
		XSerialPortMode mode = SP_MODE_MANUAL, XBool withEventMode = XFalse);	//�򿪴����豸
	XBool open(const XSerialPortInfo &info);	//�򿪴����豸
	XBool close();//�ر��豸

	int readData(void *buff, int limit);
	int sendData(const void *, int size);
	int readDataWaiting();	// ��ѯ���ܻ��������Ƿ�������(ֻ��ѯ,����ȡ)

	XBool isOpened() const { return m_bOpened; }
	void setCallBackFun(void(*funRecv)(void *, unsigned char *, int), void *pClass);
protected:
	//XBool writeCommByte(unsigned char);	// �ڲ�ʵ�֣��򴮿�д����
	//XBool m_withEventMode;	//�Ƿ�ʹ���¼�ģʽ,ʵ��ʹ���лᷢ�֣��ֳ�Ӳ��ʹ���¼�ģʽ��ʱ�򲻻ᴥ��,Ŀǰԭ���в���ȷ
	XSerialPortInfo m_serialPortInfo;

	HANDLE m_hIDComDev;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	XBool m_bOpened;
	//XSerialPortMode m_mode;

	XThreadState m_threadState;
	pthread_t m_recvThread;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�

	void(*m_funRecv)(void *, unsigned char *, int);	//�յ����ݵĻص�����
	void *m_pClass;
	//int m_waitingDataTime;	//�ȴ����ݽ��յ�ʱ��
public:
	void setWaitingDataTime(int tmp) { m_serialPortInfo.waitingDataTime = tmp; }
	int getWaitingDataTime() const { return m_serialPortInfo.waitingDataTime; }
private:
	//������ͳ����Ϣ
	bool m_withStatistics;	//�Ƿ�����ͳ��
	int m_sendTimesAll;		//�ܵķ��ʹ���
	int m_sendBytesAll;		//�ܵķ����ֽ���
	int m_recvTimesAll;		//�ܵĽ��մ���
	int m_recvBytesAll;		//�ܵĽ����ֽ���

	int m_statisticsTimer;	//ͳ�Ƽ�ʱ
	int m_sendTimesCur;
	int m_sendBytesCur;
	int m_recvTimesCur;
	int m_recvBytesCur;

	float m_sendTimesAvg;	//ƽ������
	float m_sendBytesAvg;
	float m_recvTimesAvg;
	float m_recvBytesAvg;
public:
	int getSendTimesAll()const { return m_sendTimesAll; }
	int getSendBytesAll()const { return m_sendBytesAll; }
	int getRecvTimesAll()const { return m_recvTimesAll; }
	int getRecvBytesAll()const { return m_recvBytesAll; }
	float getSendTimesAvg()const { return m_sendTimesAvg; }
	float getSendBytesAvg()const { return m_sendBytesAvg; }
	float getRecvTimesAvg()const { return m_recvTimesAvg; }
	float getRecvBytesAvg()const { return m_recvBytesAvg; }
	void setStatistics(bool flag);
	void update();
	//����ʵ�ִ������ݵ�¼�ƵĹ���
private:	//ע��Ŀǰֻ¼�ƽ��չ��ܣ���¼�Ʒ���
	XBool m_withRecord;	//�Ƿ�¼��
	XBool m_withPlay;		//�Ƿ񲥷�
	FILE *m_recordFile;		//¼���ļ���ָ��
	FILE *m_playFile;		//�����ļ���ָ��
	XCritical m_recordMutex;
	void recordRecvAData(const unsigned char *data, int len);//���ļ���д���ȡ��һ������
	XBool readADataFromRecord(std::string &oparetion, XSystemTime &t, unsigned char *date, int &len);	//�Ӽ�¼�ļ��ж�ȡһ������
	int m_playTimer;	//�طŵļ�ʱ��
	XSystemTime m_upTime;
	XSystemTime m_nextTime;
	unsigned char m_nextData[1024];
	int m_nextDataLen;
	std::string m_oparetion;
public:
	XBool getWithRecord() const { return m_withRecord; }
	XBool getWithPlay() const { return m_withPlay; }
	XBool startRecord();	//��ʼ¼��
	XBool endRecord();		//����¼��
	XBool startPlay();		//��ʼ����
	XBool stopPlay();		//��������
};
#if WITH_INLINE_FILE
#include "XSerialPort.inl"
#endif
}
#endif