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
//#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"
#include <deque>
namespace XE{
enum XSimProDataType
{
	SIMPRO_TYPE_STATE,	//״̬����
	SIMPRO_TYPE_DATA,	//��ֵ����
};
struct XSimProData
{
	XSimProDataType type;	//��������
	int index;	//����������
	int data;	//����ֵ
};	
class XSimpleProtocol
{
public:
	const static int m_maxXSPStateSum = 16;//״̬���ݵ�����
	const static int m_maxXSPValueSum = 96;//��ֵ���ݵ�����
private:
	XBool m_isInited;
	XBool m_isSigned;	//�Ƿ�Ϊ�з��ŵ�
	char m_state[m_maxXSPStateSum];			//16��״̬�Ĵ�����״̬
	short m_data[m_maxXSPValueSum];	//96�����ݼĴ���������
	unsigned char m_recvedData[3];	//�Ѿ����յ�����
	int m_recvedDataLen;		//�Ѿ����ܵ����ݳ���

	XSerialPort m_serialPort;	//�����豸
	static void simpleProctocolRecvCB(void *pClass,unsigned char * data,int len);	//���ݽ��յ��Ļص�����
	void (*m_callBackFun)(const XSimProData &data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;

	void recvDataProc(unsigned char *data,int len);
public:
	void setCallBackFun(void (*p)(const XSimProData &,void *),void *pC);
	XBool openDevice(int portIndex,int bitRate,int parity = 0,XBool withEventMode = XFalse);
	XBool openDevice(const XSerialPortInfo &info);
	XSimpleProtocol()
		:m_isInited(XFalse)
		,m_recvedDataLen(0)
		,m_isSigned(XFalse)
	{
		memset(m_state,0,m_maxXSPStateSum);
		memset(m_data,0,m_maxXSPValueSum * sizeof(unsigned short));
	}
	~XSimpleProtocol(){release();}
	void release();
	char getState(int index) const;
	int getData(int index) const;
	//������Ϊ�������ݶ������
private:
	std::deque<XSimProData> m_sendData;	//���͵�����
	XThreadState m_sendThreadState;
	XCritical m_sendMutex;
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�
	void sendData(const XSimProData&tmp);
	int decodeData(unsigned char *data);			//��ԭʼ���ݽ���ΪĿ������
	void encodeData(int src,unsigned char *data);	//��Ŀ�����ݱ���ΪԴ����
public:
	XBool pushAData(XSimProData &data);	//���Ͷ���������һ������
	//¼�ƺͻطŵļ����ӿ�
	XBool getWithRecord() const {return m_serialPort.getWithRecord();}
	XBool getWithPlay() const {return m_serialPort.getWithPlay();}
	XBool startRecord() {return m_serialPort.startRecord();}	//��ʼ¼��
	XBool endRecord() {return m_serialPort.endRecord();}		//����¼��
	XBool startPlay() {return m_serialPort.startPlay();}		//��ʼ����
	XBool stopPlay() {return m_serialPort.stopPlay();}		//��������
	void update() {m_serialPort.update();}
	void setIsSigned(XBool flag) {m_isSigned = flag;}
	XBool getIsSigned() const {return m_isSigned;}
};
#if WITH_INLINE_FILE
#include "XSimpleProtocol.inl"
#endif
}
#endif