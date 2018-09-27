#ifndef _JIA_XMICROPHONE_
#define _JIA_XMICROPHONE_
#include "al.h" 
#include "alc.h" 
#include "stdlib.h"
//#include "XBasicFun.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//������һ������¼���豸���� ����OpenAL
#pragma comment(lib, "Alut/alut.lib")
#pragma comment(lib, "OpenAL/Win32/OpenAL32.lib")

namespace XE{
enum XCaptureState
{
	CAPTURE_STATE_STOP,		//¼���豸ֹͣ
	CAPTURE_STATE_START,	//¼���豸��ʼ����
};
class XMicrophone
{
private:
	XBool m_isInited;				//�Ƿ���г�ʼ��
	ALCdevice * m_pCaptureDev;		//¼���豸��ָ��
	ALCuint m_frequency;			//��Ƶ��Ƶ��
	ALCenum m_format;				//��Ƶ�ĸ�ʽ
	ALCsizei m_buffersize;			//��Ƶ�Ļ���ռ��С
	XCaptureState m_captureState;	//¼���豸��״̬

	unsigned char *m_captureData;	//��˷�¼��������
	int m_dataBuffMaxSize;			//��������ߴ�
	int m_curDataBuffSize;			//��ǰ��Ч�Ļ���ߴ�
	int m_byteRate;					//һ�β������ֽ���
public:
	XCaptureState getCaptureState() const {return m_captureState;}
	void printAllCaptureDevice();	//��ʾ���е�¼���豸
	XBool openDevice(const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize = 20000);	//���豸
	XBool openDeviceByIndex(int index,ALCuint frequency, ALCenum format, ALCsizei buffersize = 20000);	//����Ŵ��豸
	void captureStart() const;	//���ÿ�ʼ¼��
	void captureStop() const;		//����ֹͣ¼��
	//sizeΪ�������������߽���Ƶ֡��
	unsigned char * getCaptureData(int * size = NULL);	//��ȡ��������
	//һ�β�����������
	int getByteRate() const {return m_byteRate;}
	void release();
	XMicrophone()
		:m_pCaptureDev(NULL)
		,m_captureState(CAPTURE_STATE_STOP)
		,m_isInited(XFalse)
		,m_captureData(NULL)
		,m_byteRate(1)
	{}
	~XMicrophone(){release();}
};
#if WITH_INLINE_FILE
#include "XMicrophone.inl"
#endif
}
#endif