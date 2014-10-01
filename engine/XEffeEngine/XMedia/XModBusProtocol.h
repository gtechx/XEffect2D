#ifndef _JIA_XMODBUSPROTOCOL_
#define _JIA_XMODBUSPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
//����һ�����ڴ���ͨѶ��Э���ʵ��

//Э��Ĺ���ģʽ
enum _XModBusWorkMode
{
	MODE_ASCII,		//16���ƹ���ģʽ
	MODE_RTU,		//2���ƹ���ģʽ
};
enum _XModBusType
{
	TYPE_MASTER,	//���豸
	TYPE_SLAVE,		//���豸
};

#include <deque>
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"

struct _XModBusData
{
	int delayTime;		//���յ����ݵ��ӳ�ʱ��
	unsigned char * data;
	unsigned short addr;	//�϶��β����ĵ�ַ
	int dataLen;
	_XModBusData()
		:data(NULL)
		,dataLen(0)
	{}
};
enum _XModeBusCMD
{
	CMD_READ_COIL_VOLUME = 1,
	CMD_READ_INPUT = 2,
	CMD_READ_HOLD_REGISTER = 3,
	CMD_READ_INPUT_REGISTER = 4,
	CMD_SET_ONE_COIL = 5,
	CMD_SET_ONE_REGISTER = 6,
	CMD_SET_COILS = 15,
	CMD_SET_REGISTERS = 16,
};
enum _XModeBusErrorCode
{
	ERROR_READ_COIL_VOLUME = 0x81,
	ERROR_READ_INPUT = 0x82,
	ERROR_READ_HOLD_REGISTER = 0x83,
	ERROR_READ_INPUT_REGISTER = 0x84,
	ERROR_SET_ONE_COIL = 0x85,
	ERROR_SET_ONE_REGISTER = 0x86,
	ERROR_SET_COILS = 0x8f,
	ERROR_SET_REGISTERS = 0x90,
};
#define MODBUS_HEAD_LEN (8)
struct _XModbusState
{
	int nPort;		//ͨѶ���ڵĶ˿ں�
	int nBaud;		//ͨѶƵ��
	int nParity;	//��żУ��
	unsigned char deviceID;			//�������
	_XModBusWorkMode workMode;		//����ģʽ��16���ƻ���2����
	_XModBusType workType;			//�������ͣ��������ߴӻ�
	unsigned char *coilsBuff;		//״̬�Ĵ���
	int coilsBuffSize;				//�ɱ������������bitΪ��λ
	int coilsOffset;				//״̬��ƫ��,ƫ��֮ǰ�Ĳ�ʹ��
	unsigned short *hRegisterBuff;	//���ּĴ���
	int hRegisterBuffSize;			
	int hRegisterOffset;			
	unsigned short *iRegisterBuff;	//����Ĵ���
	int iRegisterBuffSize;
	int iRegisterOffset;
	unsigned char *inputBuff;		//����״̬�Ĵ���
	int inputBuffSize;
	int inputOffset;
	int outTime;					//��ʱʱ�䵥λ��ms
	int maxResendTimes;				//����ط�����
	int maxR2STime;					//���յ�����֮���ٴη��������ʱ������ms��
	int maxWaitingDataTime;			//�ȴ����ݵ�ʱ��(ms)
	_XModbusState()
		:nPort(0)
		,nBaud(19200)
		,nParity(0)
		,workMode(MODE_RTU)
		,workType(TYPE_MASTER)
		,coilsBuff(NULL)
		,coilsBuffSize(0)
		,hRegisterBuff(NULL)
		,hRegisterBuffSize(0)
		,iRegisterBuff(NULL)
		,iRegisterBuffSize(0)
		,inputBuff(NULL)
		,inputBuffSize(0)
		,outTime(300)
		,maxResendTimes(3)
		,deviceID(1)
		,coilsOffset(0)
		,hRegisterOffset(0)
		,iRegisterOffset(0)
		,inputOffset(0)
		,maxR2STime(5)
		,maxWaitingDataTime(0)
	{}
};
class _XModBusProtocol
{
private:
	_XBool m_isInited;
	std::deque<_XModBusData> m_recvData;	//�յ�������
	std::deque<_XModBusData> m_sendData;	//���͵�����

	_XSerialPort m_serialPort;
	//static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�

	//int m_tempDataOffset;
	//int m_tempDataLen;
	//unsigned char *m_tempDataBuff;
	//unsigned char m_modbusHead[MODBUS_HEAD_LEN];
	//int m_modbusNeedHeadLen;
	//int m_headLen;
	//void recvDataProc(unsigned char * data,int len);		//��������
	_XBool m_needRecv;	//�Ƿ�ȴ��ظ�
	int m_delayTime;	//�ȴ��ظ���ʱ��
	int m_delaySendTime;	//�ȴ����͵�ʱ���

	_XCritical m_sendMutex;
	_XCritical m_recvMutex;
//	_XThreadState m_recvThreadState;
	_XThreadState m_sendThreadState;

	_XModbusState m_modbusState;
	_XModBusData m_nowSendData;	//��ǰ���͵����ݰ�
	void sendNowData();			//���͵�ǰ��Ҫ���͵�����
	int m_sendTime;				//���ʹ���
	_XBool m_connectState;		//����״̬���������״̬ΪXFalse�Ļ������˳��շ��߳�
	_XBool pushAData(_XModBusData &data);			//���Ͷ���������һ������,����Ḵ�����ݣ�ע�⣩
	void answerProc(_XModBusData &data);			//����ָ��������ش�����
	void (*m_callBackFun)(const _XModBusData & data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;
	_XBool m_withLog;
public:
	_XModBusType getWorkType() const {return m_modbusState.workType;}
	void setWithLog(_XBool flag) {m_withLog = flag;}
	_XBool getWithLog() const {return m_withLog;}
	void setCallBackFun(void (*p)(const _XModBusData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}
	_XBool getIsOpen() const {return m_isInited;}

	_XBool openDevice(_XModbusState &modbusState);	//˵����mode��type�Ĺ�����δʵ��
	int getSendBuffSize();							//��ȡ���Ͷ��еĳ���
	_XBool pushData(_XModBusData &data)				//���Ͷ���������һ������
	{
		if(m_modbusState.workType == TYPE_MASTER)
			return pushAData(data);
		return XFalse;
	}
	int getRecvBuffSize();							//��ȡ���ܶ��еĳ���
	_XModBusData popData();							//�ӽ��ܶ�����ȡ��һ������

	void release();
	_XModBusProtocol()
		:m_isInited(XFalse)
	//	,m_tempDataBuff(NULL)
		,m_withLog(XTrue)
		,m_withStatistics(false)
	{}
	~_XModBusProtocol(){release();}
	friend void recvCallback(void *pClass,unsigned char * data,int len);
	//������Ҫ���ӵĽӿ�
	//����ʱ
	//�������в�����δ��װ
	//������Щ�ӿڶ�δ��������
	void readCoilState(int startAddr,int sum,int arm);
	void readInputState(int startAddr,int sum,int arm);
	void writeOneCoilState(int Addr,int value,int arm);
	void writeCoilsState(int startAddr,int sum,unsigned char *value,int arm);
	//��ȡĳ���Ĵ�����ֵ
	void readHoldRegisters(int startAddr,int sum,int arm);	//��ȡ������ּĴ���
	void readInputRegisters(int startAddr,int sum,int arm);	//��ȡ�������Ĵ���
	//д��ĳ���Ĵ�����ֵ
	void writeRegisters(int startAddr,int sum,const unsigned short *data,int arm);		//д�����Ĵ���
	void writeOneRegister(int addr,int value,int arm);	//д��һ���Ĵ���
	int getHoldRegisterOffset(){return m_modbusState.hRegisterOffset;}	//��ȡ����ʱ�ĵ�ַ��ƫ����
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
};
inline int _XModBusProtocol::getRecvBuffSize()
{
	m_recvMutex.Lock();
	int ret = m_recvData.size();
	m_recvMutex.Unlock();
	return ret;
}
inline int _XModBusProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
//����ʱ�ӽ��ܵ���������ȡ����
//ע������Ľӿ�������ʱ�ӽ��յ�����������ȡ��ص�����
extern int getModbusCMDDataSum(const _XModBusData &CMD);	//�������н�����������
inline bool getModbusCMDAddress(const _XModBusData &CMD,unsigned int &addr)
{//����Э���ȡ����Ĳ�����ַ
	if(CMD.dataLen <= 0) return false;
	addr = CMD.addr;
	return true;
}
extern bool getModbusCMDData(const _XModBusData &CMD,int index,unsigned int &data);
inline int getModbusCMDID(const _XModBusData &CMD){return CMD.data[0];}	//��modbus�����л�ȡ���豸���豸��

#endif