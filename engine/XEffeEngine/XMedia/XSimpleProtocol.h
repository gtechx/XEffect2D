#ifndef _JIA_XSIMPLEPROTOCOL_
#define _JIA_XSIMPLEPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.9.5
//--------------------------------
/***********************************Э���İ�˵������**************************************
��������һ����ҵ��ˮ����Ŀ���ù�һ��ͨѶЭ�飬���ڸо���������
�����������£�
ͨѶ���ݷֳ����󲿷֣����������ֺͲ���������������ѡ��>80H����128���Ķ�����������������2�ֽ�ģ�����룬ÿ�ֽھ�<80H��128����
�����������2�ֽ�Ϊm,n������mΪģ���������D=m*128+n��������֤��m��n������>80H
�����ַ�Ϊ�������Ͳ����������֣����������ڰ�ť���ߴ��������룬���ڿ���Լ�����£�
80H��81H��....8FH��16��������ֱ��ʾ1-16�Ű�ť�����������պϣ����ֽڣ����治������
90H��91H��....9FH��16��������ֱ��ʾ1-16�Ű�ť�����������ͷţ����ֽڣ����治����������������һ������²��á�
A0H��A1H��....FFH��96��������ֱ��ʾ1-96���豸�ţ������2�ֽڲ�������ģ����ʽ���룩
���磺    ���յ�85H����ʾ��������ť�����������պϣ�
              ���յ�A3H��7EH��35H����ʾ4���豸����Ϊ 16181��[7EH��126��*128+35H��53��]��126*128+53=16181
              �������ֵΪ2�ֽ�ģ�� 16383��һ������¹����ˣ������Ҫ�����������ʱ���������⿪һ���豸�ţ���������ƶ���������ֺܴ�Ļ���
              �����ָ�����ɶΣ�����1m/�Σ��� ÿ����һ���豸�ţ����Ժܷ���Ĵ��䡣
              ���⣬�������ֽ����ݰ�ʱÿ�ֽڼ�����ᳬ��2���룬��10������ʱ20��������ݷ�����
              ����ͨѶ����У�飬ͨѶ��Ŀ��������δ����ͨѶ��������Ӳ�����������㡣
******************************************************************************************/
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"
#include "XLogBook.h"
enum _XSimProDataType
{
	SIMPRO_TYPE_STATE,	//״̬����
	SIMPRO_TYPE_DATA,	//��ֵ����
};
struct _XSimProData
{
	_XSimProDataType type;	//��������
	int index;	//����������
	int data;	//����ֵ
};
#include <deque>
class _XSimpleProtocol
{
private:
	_XBool m_isInited;
	char m_state[16];			//16��״̬�Ĵ�����״̬
	unsigned short m_data[96];	//96�����ݼĴ���������
	unsigned char m_recvedData[3];	//�Ѿ����յ�����
	int m_recvedDataLen;		//�Ѿ����ܵ����ݳ���

	_XSerialPort m_serialPort;	//�����豸
	friend void simpleProctocolRecvCB(void *pClass,unsigned char * data,int len);	//���ݽ��յ��Ļص�����
	void (*m_callBackFun)(const _XSimProData &data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;

	void recvDataProc(unsigned char *data,int len);
public:
	void setCallBackFun(void (*p)(const _XSimProData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}
	_XBool openDevice(int portIndex,int bitRate,int parity = 0)
	{
		if(m_isInited) return XFalse;
		//�򿪴����豸
		if(!m_serialPort.open(portIndex,bitRate,parity,SP_MODE_AUTO)) return XFalse;
		m_serialPort.setCallBackFun(simpleProctocolRecvCB,this);
		//��������߳�
		m_sendThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

		m_isInited = XTrue;
		return XTrue;
	}
	_XSimpleProtocol()
		:m_isInited(XFalse)
		,m_recvedDataLen(0)
	{}
	~_XSimpleProtocol(){release();}
	void release()
	{
		if(!m_isInited) return;
		m_sendThreadState = STATE_SET_TO_END;
		m_sendData.clear();	//��շ��Ͷ���
		waitThreadEnd(m_sendThreadState);
		m_serialPort.close();
		m_isInited = false;
	}
	char getState(int index) const
	{
		if(index < 0 || index >= 16) return 0;
		return m_state[index];
	}
	int getData(int index) const
	{
		if(index < 0 || index >= 96) return 0;
		return m_data[index];
	}
	int getMaxStateSum() const{return 16;}
	int getMaxDataSum() const{return 96;}
	//������Ϊ�������ݶ������
private:
	std::deque<_XSimProData> m_sendData;	//���͵�����
	_XThreadState m_sendThreadState;
	_XCritical m_sendMutex;
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	void sendData(const _XSimProData&tmp);
public:
	_XBool pushAData(_XSimProData &data)
	{
		m_sendMutex.Lock();
		m_sendData.push_back(data);
		m_sendMutex.Unlock();
		return XTrue;
	}
	//¼�ƺͻطŵļ����ӿ�
	_XBool getWithRecord() const {return m_serialPort.getWithRecord();}
	_XBool getWithPlay() const {return m_serialPort.getWithPlay();}
	_XBool startRecord() {return m_serialPort.startRecord();}	//��ʼ¼��
	_XBool endRecord() {return m_serialPort.endRecord();}		//����¼��
	_XBool startPlay() {return m_serialPort.startPlay();}		//��ʼ����
	_XBool stopPlay() {return m_serialPort.stopPlay();}		//��������
	void update() {m_serialPort.update();}
};
#endif