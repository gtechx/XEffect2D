#ifndef _JIA_XAVSTREAM_ 
#define _JIA_XAVSTREAM_  
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//����һ����Ƶ��Ƶ�����ѹ�����࣬����ffmpeg��OpenAL

#include "XFfmpeg.h" 
#include "XBasicFun.h"
#include "XBasicClass.h"
#include "XMicrophone.h"
#include "XAudioStream.h"
  
#include <stdio.h>  

struct _XAudioInfo
{
	int channelSum;		//ͨ������
	AVSampleFormat sampleFormat;	//��Ƶ������ʽ
	int sampleRate;		//��Ƶ������
};
enum _XAudioDeviceType
{
	AUDIO_DEVICE_TYPE_MIC,
	AUDIO_DEVICE_TYPE_FILE,
};
enum _XAVStreamQuality
{
	AVS_QUALITY_HEIGHT,	//������
	AVS_QUALITY_MIDDLE,	//�е�����
	AVS_QUALITY_LOW,	//������
};

class _XAVStream 
{
private:
	_XBool m_isOpen;				//�Ƿ����
	//������һЩ��������
	int m_videoWidth;			//��Ƶ�����ؿ��
	int m_videoHight;			//��Ƶ�����ظ߶�
	int m_videoFrameRate;		//��Ƶ��֡��
	AVStream * m_videoST;		//��Ƶ����ָ��
    AVStream * m_audioST;		//��Ƶ����ָ��
	AVFormatContext *m_pOutContext;		//��Ƶ��ʽ��ָ��
	AVOutputFormat *m_pFormat;

	AVFrame * m_pictureYUV;		//YUV���ص�ͼ����ɫ
	AVFrame * m_pictureRGB;		//RGB���ص�ͼ����ɫ 
	//��Ƶ
	int m_audioOutbufSize;		//��Ƶ����Ĵ�С
	uint8_t *m_audioOutbuf;		//��Ƶ�Ļ���
	uint8_t *m_audioTempBuff;
	int m_audioDataPos;			//��ǰ��Ƶ���ݵ�λ��
	int m_audioFrameSize;		//������ݴ�������,һ�����ݰ��Ĵ�С
	int m_audioFrameInSize;		//Ҫװ��һ�����ݰ�����Ҫ�������������
	//int m_audioOutFrameSize;	//��Ƶһ֡���ݵĴ�С
	//int m_audioInFrameSize;	//��Ƶ����һ֡�Ĵ�С
	_XBool m_audioBuffFlag;		//�����ڴ潻����Ϊ������Ч��

	AVFrame *m_audioFrame;	//��Ƶ����
	SwrContext *m_audioSwr;	//������Ƶת��
	AVPacket m_audioPkt;	//������Ƶ���ݰ�
	//��Ƶ
	int m_videoOutbufSize;	//��Ƶ����Ĵ�С
	uint8_t *m_videoOutbuf;	//��Ƶ�Ļ���
	AVPacket m_videoPkt;	//������Ƶ���ݰ�

	AVStream *openAudio();	//����Ƶ��
	void closeAudio()		//�ر���Ƶ��
	{
		avcodec_close(m_audioST->codec);
		//av_free(m_audioOutbuf);
		XDELETE_ARRAY(m_audioOutbuf);
		XDELETE_ARRAY(m_audioTempBuff);
	}

	AVStream *openVideo(_XAVStreamQuality quality = AVS_QUALITY_HEIGHT);	//����Ƶ��
	void closeVideo()		//�ر���Ƶ��
	{
		avcodec_close(m_videoST->codec);
		//av_free(m_videoOutbuf);
		XDELETE_ARRAY(m_videoOutbuf);
	} 

	//��������һ���Զ�ͬ����ģʽ��ȥ����һ���֣�
	//_XBool m_autoMode;	//�Ƿ�ʹ���Զ�ͬ����ģʽ
	float m_autoTimer;	//�Զ�ͬ��ģʽ�ļ�ʱ�� ��λ:ms
	float m_frameTime;	//ÿ֡��ʱ��

	SwsContext *m_pSwsContext;	//���ڽ���ͼ���ʽת��
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
	{
		if(m_pSwsContext == NULL) return 0;
		return sws_scale(m_pSwsContext, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize); 
	}
	_XBool audioNeedData() const
	{
		int len = 0;
		switch(m_audioInfo.sampleFormat)
		{
		case AV_SAMPLE_FMT_U8:len = 1;break;
		case AV_SAMPLE_FMT_S16:len = 2;break;
		case AV_SAMPLE_FMT_S32:len = 4;break;
		case AV_SAMPLE_FMT_FLT:len = 4;break;
		case AV_SAMPLE_FMT_U8P:len = 1;break;
		case AV_SAMPLE_FMT_S16P:len = 2;break;
		case AV_SAMPLE_FMT_S32P:len = 4;break;
		case AV_SAMPLE_FMT_FLTP:len = 4;break;
		}
		return m_videoFrameIndex * m_audioInfo.sampleRate * m_audioInfo.channelSum * len / m_videoFrameRate > m_inputAudioDataSum;
	}
	_XBool openFile(const char *filename,int width,int height,int frameRate,
		_XAudioInfo *info = NULL,_XAVStreamQuality quality = AVS_QUALITY_HEIGHT);
	void pushVideoFrame();
	//������������ն��е�
	void flushAudioQueue();
	void flushVideoQueue();
	_XAudioInfo m_audioInfo;	//��Ƶ������Ϣ

	int m_inputAudioDataSum;	//�ܵ���Ƶ������������
private:
	int m_videoFrameIndex;		//֡�ı��
	//int m_audioFrameIndex;
public:	//����ӿ�
	//����1��ʱ��֡ͬ����Ҫ�ⲿ����
	_XBool open(const char *filename,int width,int height,int frameRate,
		_XAudioInfo *info = NULL,_XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void addFrameAudio(const unsigned char * data,int size);	//���������������ݱ�����LFLFLF....��������ݣ�����ʹLLLLL����FFFFF���������ݣ������������⡣
	void addFrameRGB(unsigned char*p);
	//����1��һ������汾
	void addFrameAudio(_XMicrophone & mic)
	{
		int ret = 0;
		unsigned char * audioData = mic.getCaptureData(&ret);
		if(ret != 0 && audioNeedData())  addFrameAudio(audioData,ret * mic.getByteRate());//��ȡ��Ƶ���ݣ���д�뵽��Ƶ�ļ���
	}
	void addFrameAudio(_XAudioStream & aStr)
	{
		while(audioNeedData())
		{
			if(aStr.getAFrame()) addFrameAudio(aStr.getData(),aStr.getDataLen());
			Sleep(1);
		}
	}
	//����2�����̣߳���Ƶ�豸�ⲿ����
	_XBool open(const char *filename,int width,int height,int frameRate,void * audioDevice,
		_XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC,_XAudioInfo *info = NULL,
		_XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void updataFrameRGB(unsigned char*p){addFrameRGB(p);}
	void setStop()
	{
		if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
			((_XMicrophone *)m_audioDevice)->captureStop();
		m_isStop = XTrue;
	}
	void setStart()
	{
		if(m_audioDeviceType == AUDIO_DEVICE_TYPE_MIC)
			((_XMicrophone *)m_audioDevice)->captureStart();
		m_isStop = XFalse;
	}
private:
	_XCritical m_mutex;
	void * m_audioDevice;
	_XAudioDeviceType m_audioDeviceType;
	_XBool m_isStop;
	_XBool m_withThread;
	int m_threadState;	//�̵߳�״̬ 1������ 2�������˳���3���˳����
	pthread_t m_encodeThread;
	static DWORD WINAPI encodeThread(void * pParam);	//�����߳�
public:
	//����3����Ƶ�豸�ڲ�����
	_XBool openEx(const char *filename,int width,int height,int frameRate,const char * deviceName = NULL,
		_XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC);
private:
	_XBool m_withDevice;
	_XMicrophone m_micDevice;	//��˷����
	_XAudioStream m_AudioStream;	//����Ƶ�ļ��ж�ȡ��Ƶ���ݵ���
public:
	//���ô���
	_XBool close();  
	float getFrameTime() const {return m_frameTime;}	//��ȡһ֡��ʱ�䣬��λms

	_XAVStream()
		:m_isOpen(XFalse)
		,m_videoWidth(0)		
		,m_videoHight(0)		
		,m_videoFrameRate(30)	
		,m_videoST(NULL)			
		,m_audioST(NULL)			
		,m_pOutContext(NULL)				
		,m_pFormat(NULL)
		,m_pictureYUV(NULL)		
		,m_pictureRGB(NULL)		

		,m_audioOutbufSize(0)
		,m_audioOutbuf(NULL)
		,m_audioTempBuff(NULL)

		,m_videoOutbufSize(0)
		,m_videoOutbuf(NULL)
		,m_audioFrame(NULL)
		,m_audioSwr(NULL)
	{}
};  

#endif 