#ifndef _JIA_XSONOVOPROTOCOL_
#define _JIA_XSONOVOPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.8.20
//--------------------------------
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"
#include "XLogBook.h"
//����ģʽ
enum _XSPDevType
{
	DEV_MASTER,		//���豸
	DEV_SLAVE,		//���豸
};
enum _XSPCommand
{
	CMD_WRITE = 0x00,		//дָ��
	CMD_READ = 0x01,		//��ָ��
	CMD_WRITE_ACK = 0x10,	//дָ��Ļظ�
	CMD_READ_ACK = 0x11,		//��ָ��Ļظ�
};
struct _XSPData
{
	int dataLen;
	unsigned char * data;
	_XSPData()
		:dataLen(0)
		,data(NULL)
	{}
};
//����������ǰЭ����Ϣ�Ľṹ
struct _XSPInfo
{
	int nPort;		//ͨѶ���ڵĶ˿ں�
	int nBaud;		//ͨѶƵ��
	int nParity;	//��żУ��
	_XSPDevType devType;
	unsigned short * buff;	//�洢��Ԫ��ָ��
	int buffSize;			//�洢��Ԫ�п��Դ洢��Ԫ������
	_XSPInfo()
		:nPort(0)
		,nBaud(19200)
		,nParity(0)
		,devType(DEV_MASTER)
		,buff(NULL)
		,buffSize(0)
	{}
};
#include <deque>
class _XSonovoProtocol
{
private:
	_XBool m_isInited;
	std::deque<_XSPData> m_recvData;	//�յ�������
	std::deque<_XSPData> m_sendData;	//���͵�����

	_XBool m_needRecv;	//�Ƿ�Ҫ�ȴ��ظ�
	int m_delayTime;	//�ȴ��ظ���ʱ��

	_XSerialPort m_serialPort;
	_XThreadState m_sendThreadState;
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	_XCritical m_recvMutex;
	_XCritical m_sendMutex;

	friend void SPRecvCB(void *pClass,unsigned char * data,int len);	//���ݽ��յ��Ļص�����
	void (*m_callBackFun)(const _XSPData & data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;

	_XBool pushAData(_XSPData &data);			//���Ͷ���������һ������(ע�⣬����Ḵ������)
	void answerProc(_XSPData &data);			//����ָ��������ش�����

	_XSPInfo m_info;
	_XSPData m_nowSendData;	//��ǰ���͵����ݰ�
	_XBool m_withLog;

	void sendNowData();			//���͵�ǰ��Ҫ���͵�����
	unsigned char *m_sendBuff;	//���ڷ��͵���ʱ����
	int m_sendBuffSize;	
public:
	_XSPDevType getWorkType() const {return m_info.devType;}
	void setWithLog(_XBool flag) {m_withLog = flag;}
	_XBool getWithLog() {return m_withLog;}
	void setCallBackFun(void (*p)(const _XSPData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}

	_XBool getIsOpen() const {return m_isInited;}
	_XBool openDevice(const _XSPInfo &info);	//���豸
	int getSendBuffSize();							//��ȡ���Ͷ��еĳ���

	_XSonovoProtocol()
		:m_isInited(false)
		,m_withStatistics(false)
		,m_callBackFun(NULL)
		,m_pClass(NULL)
	{}
	~_XSonovoProtocol(){release();}
	void release()
	{
		if(!m_isInited) return;
		waitThreadEnd(m_sendThreadState);
		//������е�����
		for(int i = 0;i < m_sendData.size();++ i)
		{
			XDELETE_ARRAY(m_sendData[i].data);
		}
		m_sendData.clear();
		for(int i = 0;i < m_sendData.size();++ i)
		{
			XDELETE_ARRAY(m_recvData[i].data);
		}
		m_recvData.clear();

		m_serialPort.close();
		m_isInited = XFalse; 
	}
	_XSPData popData()							//�ӽ��ܶ�����ȡ��һ������
	{
		_XSPData tempData;
		if(m_info.devType == DEV_MASTER)
		{
			m_recvMutex.Lock();
			if(m_recvData.size() > 0)
			{
				tempData = m_recvData[0];
				m_recvData.pop_front();
			}
			m_recvMutex.Unlock();
		}
		return tempData;
	}
	_XBool pushData(_XSPData &data)				//���Ͷ���������һ������
	{
		if(m_info.devType == DEV_MASTER)
			return pushAData(data);
		return XFalse;
	}

	void showData(const _XSPData &data,const std::string &title);
private:
	//������ͳ����Ϣ
	bool m_withStatistics;	//�Ƿ�����ͳ��
	int m_statisticsTimer;	//ͳ�Ƽ�ʱ
	int m_comunicateTimesNow;	//��������
	int m_delayTimeNow;			//���ӳ�
	float m_delayTimeAvg;	//ƽ���ӳ�
public:
	int getSendTimesAll()const{return m_serialPort.getSendTimesAll();}
	int getSendBytesAll()const{return m_serialPort.getSendBytesAll();}
	int getRecvTimesAll()const{return m_serialPort.getRecvTimesAll();}
	int getRecvBytesAll()const{return m_serialPort.getRecvBytesAll();}
	float getSendTimesAvg()const{return m_serialPort.getSendTimesAvg();}
	float getSendBytesAvg()const{return m_serialPort.getSendBytesAvg();}
	float getRecvTimesAvg()const{return m_serialPort.getRecvTimesAvg();}
	float getRecvBytesAvg()const{return m_serialPort.getRecvBytesAvg();}

	float getDelayTimeAvg()const{return m_delayTimeAvg;}
	void setStatistics(bool flag)
	{
		if(flag == m_withStatistics) return;
		if(flag)
		{//����ͳ��
			m_withStatistics = true;
			//���ݹ���
			m_statisticsTimer = 0;	//ͳ�Ƽ�ʱ
			m_comunicateTimesNow = 0;	//��������
			m_delayTimeNow = 0;			//���ӳ�
			m_delayTimeAvg = 0.0f;	//ƽ���ӳ�
			m_serialPort.setStatistics(true);
		}else
		{//�ر�ͳ��
			m_withStatistics = false;
			m_serialPort.setStatistics(false);
		}
	}
	void update();

	void writeOneRegister(int addr,int value);	//дһ���Ĵ���
	void readRegisters(int startAddr,int sum);	//��ȡ������ּĴ���
};

inline void _XSonovoProtocol::showData(const _XSPData &data,const std::string &title)
{
	std::string tmp = title + "data:";
	char tmpStr[32];
	for(int i = 0;i < data.dataLen;++ i)
	{
		sprintf(tmpStr,"0x%02x ",data.data[i]);
		tmp += tmpStr;
	}
	LogStr(tmp.c_str());
}
inline int _XSonovoProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
inline bool getSPCMDAddress(const _XSPData &CMD,unsigned int &addr)
{//����Э���ȡ����Ĳ�����ַ
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
	//	addr = (CMD.data[1] << 8) + CMD.data[2];
		addr = CMD.data[1];
		return true;
	case CMD_WRITE_ACK:
		return false;
	case CMD_READ_ACK:
		//addr = (CMD.data[3] << 8) + CMD.data[4];
		addr = CMD.data[2];
		return true;
	}
	return false;
}
inline int getSPCMDDataSum(const _XSPData &CMD)	//�������н�����������
{
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
		return (CMD.dataLen - 4) >> 1;
	case CMD_READ:
		return 0;
	case CMD_WRITE_ACK:
		return 0;
	case CMD_READ_ACK:
		return (CMD.dataLen - 5) >> 1;
	}
	return false;
}
inline bool getSPCMDData(const _XSPData &CMD,int index,unsigned int &data)
{//����Э���ȡ����Ĳ�����ַ
	if(CMD.dataLen <= 0) return false;
	if(index < 0 || index >= getSPCMDDataSum(CMD)) return false;
	int offset;
	switch(CMD.data[0])
	{
	case CMD_WRITE:
		offset = (index << 1) + 2;
		data = (CMD.data[offset] << 8) + CMD.data[offset + 1];
		return true;
	case CMD_READ:
		return false;
	case CMD_WRITE_ACK:
		return false;
	case CMD_READ_ACK:
		offset = (index << 1) + 3;
		data = (CMD.data[offset] << 8) + CMD.data[offset + 1];
		return true;
	}
	return false;
}
#include "XSonovoProtocol.inl"
/****************************Э��˵���ĵ�******************************
�Ĵ浥Ԫ��2Bytes�����ڴ�ռ�Ϊһ���洢��Ԫ��ÿ���洢��Ԫ��һ����ţ�0��n
�洢�ռ䣺��n���洢��Ԫ���ɣ��洢�ռ��д洢��Ԫ�ı�Ŵ�0��ʼ��n-1����

������û�д洢�ռ�
�ӻ���ӵ�д洢�ռ�

ϵͳ�ṹ��һ��ϵͳ�а���һ������һ���ӻ���Ŀǰ��֧�ֳ���2̨���������
ͨѶЭ�飺

����ָ��
дָ��
1Byte	|1Byte			|2*nBytes		|2Bytes
0x00    |��ʼ�Ĵ������ |n���Ĵ�����ֵ 	|У���ֽ�(CRC16У��)
��ָ��
1Byte	|1Byte			|1Byte					|2Bytes
0x01    |��ʼ�Ĵ������ |��Ҫ��ȡ�ļĴ������� 	|У���ֽ�(CRC16У��)

�ӻ�ָ��
��Ӧдָ��
1Byte	|1Byte							|2Bytes
0x10    |д�������:0x00ʧ�ܣ�0x01�ɹ�	|У���ֽ�(CRC16У��)
��Ӧ��ָ��
1Byte	|1Byte							|1Byte			|2*nBytes		|2Bytes
0x11    |���������:0x00ʧ�ܣ�0x01�ɹ�	|��ʼ�Ĵ������ |n���Ĵ�����ֵ 	|У���ֽ�(CRC16У��)

Ĭ�ϵĴ������ù���Ϊ:�����ʣ�9600����żУ�飺�ޣ�ֹͣλ��1��
����ָ����Ҫ�ȴ��ӻ���Ӧ֮����ܼ�����һ��ָ���ʱʱ��Ϊ300ms����ʱ����ʽ���������ط����ݾ������ȷ��
CRC16У��ʹ�õĹ�ʽΪ��x16 + x12 + x5 + 1(XModen)
ȷ��ʹ��CRC16У�飬����(ͷ�ֽ�)��Ҫ����У�顣
������ܻ����ͨѶ���������Э������޶���Ҫ���ǽ�������⡣
***********************************************************************/

#endif