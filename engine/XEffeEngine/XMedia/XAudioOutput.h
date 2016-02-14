#ifndef _JIA_XAUDIOOUTPUT_
#define _JIA_XAUDIOOUTPUT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//����һ����Ƶ�������
#include "../XMemory.h"
#include "XFfmpeg.h"
#include "XSoundCommon.h"
//#include "../XThread.h"
#include "../XCritical.h"
namespace XE{
class XAudioOutput
{
protected:
    XAudioOutput()
		:m_audioBuff(NULL)
		,m_buffSize(0)
		,m_curUsage(0)
		,m_isStart(false)
		,m_pSwrContext(NULL)
		,m_tmpBuffer(NULL)
		,m_withConversion(false)
		{}
    XAudioOutput(const XAudioOutput&);
	XAudioOutput &operator= (const XAudioOutput&);
	virtual ~XAudioOutput(){release();}
public:
    static XAudioOutput& GetInstance()
	{
		static XAudioOutput m_instance;
		return m_instance;
	}
private:
	unsigned char *m_audioBuff;	//���ڷ���Ƶ����
	int m_buffSize;	//����Ĵ�С
	int m_curUsage;	//��ǰ��ʹ����
	bool m_isStart;
	XCritical m_mutex;
	static void audioCallBack(void *userdata,Uint8 *stream,int len);	//�����Ļص�����
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊת������Ĳ���
	SwrContext *m_pSwrContext;
	unsigned char *m_tmpBuffer;	//����ת������ʱ�洢�ռ�
	bool m_withConversion;	//�Ƿ���ת��
	int m_conversionPerFrame;	//ת��ʱ�Ĳ���ת����
	float m_conversionRate;	//ת����
	//--------------------------------------------------
	bool insertDataX(const unsigned char *data,int dataLen);	//��������
public:
	void release();
	AVSampleFormat getSampleFormat()const{return AV_SAMPLE_FMT_S16;}
	bool getIsStart()const{return m_isStart;}
	bool start();	//��ʼ�ط�
	int getBuffUsage()const{return m_curUsage;}
	float getCurCanPlayTime()const;//���ص�ǰ�����е����ݿ����ڲ��Ŷ೤��ʱ��	
	bool insertData(const unsigned char *data,int dataLen);	//��������
	bool stop();	//ֹͣ�ط�
	void setConversion(int channelSum,AVSampleFormat format,int sampleRate);	//����ת������
	void useConversion(bool flag){m_withConversion = flag;}	//����ת��
	bool getIsConversion()const{return m_withConversion;}
};
#define XAudioOut XAudioOutput::GetInstance()
#if WITH_INLINE_FILE
#include "XAudioOutput.inl"
#endif
}

#endif