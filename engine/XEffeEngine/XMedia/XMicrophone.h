#ifndef _JIA_XMICROPHONE_
#define _JIA_XMICROPHONE_
#include "al.h" 
#include "alc.h" 
#include "stdlib.h"
#include "XBasicFun.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//������һ������¼���豸���� ����OpenAL

enum _XCaptureState
{
	CAPTURE_STATE_STOP,		//¼���豸ֹͣ
	CAPTURE_STATE_START,		//¼���豸��ʼ����
};

#pragma comment(lib, "../../engine/lib/Alut/alut.lib")
#pragma comment(lib, "../../engine/lib/OpenAL/Win32/OpenAL32.lib")
class _XMicrophone
{
private:
	_XBool m_isInited;				//�Ƿ���г�ʼ��
	ALCdevice * m_pCaptureDev;		//¼���豸��ָ��
	ALCuint m_frequency;			//��Ƶ��Ƶ��
	ALCenum m_format;				//��Ƶ�ĸ�ʽ
	ALCsizei m_buffersize;			//��Ƶ�Ļ���ռ��С
	_XCaptureState m_captureState;			//¼���豸��״̬

	unsigned char *m_captureData;	//��˷�¼��������
	int m_dataBuffMaxSize;			//��������ߴ�
	int m_nowDataBuffSize;			//��ǰ��Ч�Ļ���ߴ�
	int m_byteRate;					//һ�β������ֽ���
public:
	_XCaptureState getCaptureState() const {return m_captureState;}
	void printAllCaptureDevice();	//��ʾ���е�¼���豸
	_XBool openDevice(const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize);	//���豸
	void captureStart() const	//���ÿ�ʼ¼��
	{
		if(m_captureState == CAPTURE_STATE_STOP
			&& m_isInited) alcCaptureStart(m_pCaptureDev); 
	}
	void captureStop() const		//����ֹͣ¼��
	{
		if(m_captureState == CAPTURE_STATE_START
			&& m_isInited) alcCaptureStop(m_pCaptureDev); 
	}
	unsigned char * getCaptureData(int * size = NULL);	//��ȡ��������
	int getByteRate() const {return m_byteRate;}

	void release()
	{
		if(!m_isInited) return;
		alcMakeContextCurrent(NULL);
		alcCaptureCloseDevice(m_pCaptureDev); 
		XDELETE_ARRAY(m_captureData);

		m_isInited = XFalse;
	}
	_XMicrophone()
		:m_pCaptureDev(NULL)
		,m_captureState(CAPTURE_STATE_STOP)
		,m_isInited(XFalse)
		,m_captureData(NULL)
		,m_byteRate(1)
	{}
	~_XMicrophone(){release();}
};

#endif