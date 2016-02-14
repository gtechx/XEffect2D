#ifndef _JIA_XMODBUSPROTOCOL_
#define _JIA_XMODBUSPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
#include <deque>
//#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"

//����һ�����ڴ���ͨѶ��Э���ʵ��
namespace XE{
//Э��Ĺ���ģʽ
enum XModBusWorkMode
{
	MODE_ASCII,		//16���ƹ���ģʽ
	MODE_RTU,		//2���ƹ���ģʽ
};
enum XModBusType
{
	TYPE_MASTER,	//���豸
	TYPE_SLAVE,		//���豸
};

struct XModBusData
{
	int delayTime;		//���յ����ݵ��ӳ�ʱ��
	unsigned char * data;
	unsigned short addr;	//�϶��β����ĵ�ַ
	int dataLen;
	XModBusData()
		:data(NULL)
		,dataLen(0)
	{}
};
enum XModeBusCMD
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
enum XModeBusErrorCode
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
struct XModbusState
{
	XSerialPortInfo serialPortInfo;
	unsigned char deviceID;			//�������
	XModBusWorkMode workMode;		//����ģʽ��16���ƻ���2����
	XModBusType workType;			//�������ͣ��������ߴӻ�
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
	XModbusState()
		:workMode(MODE_RTU)
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
	{}
};
class XModBusProtocol
{
private:
	XBool m_isInited;
	std::deque<XModBusData> m_recvData;	//�յ�������
	std::deque<XModBusData> m_sendData;	//���͵�����

	XSerialPort m_serialPort;
	//static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�

	//int m_tempDataOffset;
	//int m_tempDataLen;
	//unsigned char *m_tempDataBuff;
	//unsigned char m_modbusHead[MODBUS_HEAD_LEN];
	//int m_modbusNeedHeadLen;
	//int m_headLen;
	//void recvDataProc(unsigned char * data,int len);		//��������
	XBool m_needRecv;	//�Ƿ�ȴ��ظ�
	int m_delayTime;	//�ȴ��ظ���ʱ��
	int m_delaySendTime;	//�ȴ����͵�ʱ���

	XCritical m_sendMutex;
	XCritical m_recvMutex;
//	XThreadState m_recvThreadState;
	XThreadState m_sendThreadState;

	XModbusState m_modbusState;
	XModBusData m_curSendData;	//��ǰ���͵����ݰ�
	void sendCurData();			//���͵�ǰ��Ҫ���͵�����
	int m_sendTime;				//���ʹ���
	XBool m_connectState;		//����״̬���������״̬ΪXFalse�Ļ������˳��շ��߳�
	XBool pushAData(XModBusData &data);			//���Ͷ���������һ������,����Ḵ�����ݣ�ע�⣩
	void answerProc(XModBusData &data);			//����ָ��������ش�����
	void (*m_callBackFun)(const XModBusData & data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;
	XBool m_withLog;
public:
	XModBusType getWorkType() const {return m_modbusState.workType;}
	void setWithLog(XBool flag) {m_withLog = flag;}
	XBool getWithLog() const {return m_withLog;}
	void setCallBackFun(void (*p)(const XModBusData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}
	XBool getIsOpen() const {return m_isInited;}

	XBool openDevice(XModbusState &modbusState);	//˵����mode��type�Ĺ�����δʵ��
	int getSendBuffSize();							//��ȡ���Ͷ��еĳ���
	XBool pushData(XModBusData &data)				//���Ͷ���������һ������
	{
		if(m_modbusState.workType == TYPE_MASTER)
			return pushAData(data);
		return XFalse;
	}
	int getRecvBuffSize();							//��ȡ���ܶ��еĳ���
	XModBusData popData();							//�ӽ��ܶ�����ȡ��һ������

	void release();
	XModBusProtocol()
		:m_isInited(XFalse)
	//	,m_tempDataBuff(NULL)
		,m_withLog(XTrue)
		,m_withStatistics(false)
	{}
	~XModBusProtocol(){release();}
	static void recvCallback(void *pClass,unsigned char * data,int len);
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
	int m_comunicateTimesCur;	//��������
	int m_delayTimeCur;			//���ӳ�
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
			m_comunicateTimesCur = 0;	//��������
			m_delayTimeCur = 0;			//���ӳ�
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
inline int XModBusProtocol::getRecvBuffSize()
{
	m_recvMutex.Lock();
	int ret = m_recvData.size();
	m_recvMutex.Unlock();
	return ret;
}
inline int XModBusProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
namespace XModBus{
//����ʱ�ӽ��ܵ���������ȡ����
//ע������Ľӿ�������ʱ�ӽ��յ�����������ȡ��ص�����
extern int getModbusCMDDataSum(const XModBusData &CMD);	//�������н�����������
inline bool getModbusCMDAddress(const XModBusData &CMD,unsigned int &addr)
{//����Э���ȡ����Ĳ�����ַ
	if(CMD.dataLen <= 0) return false;
	addr = CMD.addr;
	return true;
}
extern bool getModbusCMDData(const XModBusData &CMD,int index,unsigned int &data);
inline int getModbusCMDID(const XModBusData &CMD){return CMD.data[0];}	//��modbus�����л�ȡ���豸���豸��
}
}
#endif