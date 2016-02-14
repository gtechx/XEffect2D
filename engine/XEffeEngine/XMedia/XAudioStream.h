#ifndef _JIA_XAUDIOSTREAM_
#define _JIA_XAUDIOSTREAM_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.8.10
//--------------------------------
//����һ������Ƶ�ļ�����ȡ��Ƶ���ݰ�����
//
//#include "XBasicFun.h"
#include "XFfmpeg.h"
namespace XE{
class XAudioStream
{
private:
	XBool m_isLoaded;		//�Ƿ��������
	AVFormatContext *m_pFormatCtx;		//�ļ����ݸ�ʽ����������
	AVCodecContext *m_pAudioCodecCtx;	//��Ƶѹ�����ݵ���Ϣ
	AVFrame *m_pFrame;					//һ����Ƶ�����������
	AVPacket m_dataPacket;
	int m_allFrameSum;					//��֡��

	//������Ƶ���ݸ���ת��
	SwrContext *m_pSwrContext;
	uint8_t *m_audioBuf; 
	int m_dataLen;
	int m_frameDataSum;	//ÿһ֡���ݵ�������
	float m_speed;	//�����ٶ�
public:
	float getSpeed() const{return m_speed;}
	void setSpeed(float speed);
	unsigned char * getData() {return m_audioBuf;}
	int getDataLen() const {return m_dataLen;}
	XBool load(const char * filename);
	XBool getAFrame();	//��������ȡһ֡����
	AVSampleFormat getSampleFormat()const{return AV_SAMPLE_FMT_S16;}
	void close()
	{
		if(!m_isLoaded) return;
		av_frame_free(&m_pFrame);
		av_free_packet(&m_dataPacket);
		avcodec_close(m_pAudioCodecCtx);
		avformat_close_input(&m_pFormatCtx);
		swr_free(&m_pSwrContext);
		m_isLoaded = XFalse;
	}
	void gotoFrame(float temp)	//[0.0 - 1.0]
	{
		XMath::clamp(temp,0.0f,1.0f);
		//if(temp < 0.0f) temp = 0.0f;
		//if(temp > 1.0f) temp = 1.0f;

		av_seek_frame(m_pFormatCtx,0,temp * m_allFrameSum,AVSEEK_FLAG_BACKWARD);
	}
	XAudioStream()
		:m_isLoaded(XFalse)
		,m_pFormatCtx(NULL)
		,m_pAudioCodecCtx(NULL)
		,m_pFrame(NULL)
		,m_pSwrContext(NULL)
		,m_audioBuf(NULL)
		,m_speed(1.0f)
	{}
	~XAudioStream() {close();}
};
}
#endif