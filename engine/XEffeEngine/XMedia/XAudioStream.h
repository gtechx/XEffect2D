#ifndef _JIA_XAUDIOSTREAM_
#define _JIA_XAUDIOSTREAM_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.8.10
//--------------------------------
//����һ������Ƶ�ļ�����ȡ��Ƶ���ݰ�����
#include "XBasicWindow.h"
#include "XBasicFun.h"
#include "XFfmpeg.h"

class _XAudioStream
{
private:
	_XBool m_isLoaded;		//�Ƿ��������
	AVFormatContext *m_pFormatCtx;		//�ļ����ݸ�ʽ����������
	AVCodecContext *m_pAudioCodecCtx;	//��Ƶѹ�����ݵ���Ϣ
	AVFrame *m_pFrame;					//һ����Ƶ�����������
	AVPacket m_dataPacket;
	int m_allFrameSum;					//��֡��

	//������Ƶ���ݸ���ת��
	SwrContext *m_pSwrContext;
	uint8_t m_audioBuf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	int m_dataLen;
public:
	unsigned char * getData() {return m_audioBuf;}
	int getDataLen() const {return m_dataLen;}
	_XBool load(const char * filename);
	_XBool getAFrame();	//��������ȡһ֡����
	void close()
	{
		if(!m_isLoaded) return;
		avcodec_free_frame(&m_pFrame);
		av_free_packet(&m_dataPacket);
		avcodec_close(m_pAudioCodecCtx);
		av_close_input_file(m_pFormatCtx);
		swr_free(&m_pSwrContext);
		m_isLoaded = XFalse;
	}
	void gotoFrame(float temp)	//[0.0 - 1.0]
	{
		if(temp < 0.0f) temp = 0.0f;
		if(temp > 1.0f) temp = 1.0f;

		av_seek_frame(m_pFormatCtx,0,temp * m_allFrameSum,AVSEEK_FLAG_BACKWARD);
	}
	_XAudioStream()
		:m_isLoaded(XFalse)
		,m_pFormatCtx(NULL)
		,m_pAudioCodecCtx(NULL)
		,m_pFrame(NULL)
		,m_pSwrContext(NULL)
	{}
	~_XAudioStream() {close();}
};

#endif