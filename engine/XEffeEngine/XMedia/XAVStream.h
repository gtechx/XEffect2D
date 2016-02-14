#ifndef _JIA_XAVSTREAM_ 
#define _JIA_XAVSTREAM_  
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
//����һ����Ƶ��Ƶ�����ѹ�����࣬����ffmpeg��OpenAL
#include "XFfmpeg.h" 
//#include "XBasicFun.h"
//#include "XBasicClass.h"
#include "XMicrophone.h"
#include "XAudioStream.h"
#include "../XThread.h"
#include "../XCritical.h"

#include <stdio.h>  
namespace XE{
struct XAudioInfo
{
	int channelSum;		//ͨ������
	AVSampleFormat sampleFormat;	//��Ƶ������ʽ
	int sampleRate;		//��Ƶ������
};
enum XAudioDeviceType
{
	AUDIO_DEVICE_TYPE_MIC,
	AUDIO_DEVICE_TYPE_FILE,
};
enum XAVStreamQuality
{
	AVS_QUALITY_HEIGHT,	//������
	AVS_QUALITY_MIDDLE,	//�е�����
	AVS_QUALITY_LOW,	//������
};

class XAVStream 
{
private:
	XBool m_isOpen;				//�Ƿ����
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
	XBool m_audioBuffFlag;		//�����ڴ潻����Ϊ������Ч��

	AVFrame *m_audioFrame;	//��Ƶ����
	SwrContext *m_audioSwr;	//������Ƶת��
	AVPacket m_audioPkt;	//������Ƶ���ݰ�
	//��Ƶ
	int m_videoOutbufSize;	//��Ƶ����Ĵ�С
	uint8_t *m_videoOutbuf;	//��Ƶ�Ļ���
	AVPacket m_videoPkt;	//������Ƶ���ݰ�

	AVStream *openAudio();	//����Ƶ��
	void closeAudio();		//�ر���Ƶ��

	AVStream *openVideo(XAVStreamQuality quality = AVS_QUALITY_HEIGHT);	//����Ƶ��
	void closeVideo();		//�ر���Ƶ��

	//��������һ���Զ�ͬ����ģʽ��ȥ����һ���֣�
	//XBool m_autoMode;	//�Ƿ�ʹ���Զ�ͬ����ģʽ
	float m_autoTimer;	//�Զ�ͬ��ģʽ�ļ�ʱ�� ��λ:ms
	float m_frameTime;	//ÿ֡��ʱ��

	SwsContext *m_pSwsContext;	//���ڽ���ͼ���ʽת��
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height);
	XBool audioNeedData() const;
	XBool openFile(const char *filename,int width,int height,int frameRate,
		XAudioInfo *info = NULL,XAVStreamQuality quality = AVS_QUALITY_HEIGHT);
	bool pushVideoFrame();
	//������������ն��е�
	void flushAudioQueue();
	void flushVideoQueue();
	XAudioInfo m_audioInfo;	//��Ƶ������Ϣ

	int m_inputAudioDataSum;	//�ܵ���Ƶ������������
private:
	int m_videoFrameIndex;		//֡�ı��
	//int m_audioFrameIndex;
public:	//����ӿ�
	//����1��ʱ��֡ͬ����Ҫ�ⲿ����
	XBool open(const char *filename,int width,int height,int frameRate,
		XAudioInfo *info = NULL,XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void addFrameAudio(const unsigned char * data,int size);	//���������������ݱ�����LFLFLF....��������ݣ�����ʹLLLLL����FFFFF���������ݣ������������⡣
	void addFrameRGB(unsigned char*p);
	//����1��һ������汾
	void addFrameAudio(XMicrophone & mic);		//����˷�����ȡ��Ƶ����
	void addFrameAudio(XAudioStream & aStr);	//����Ƶ�ļ�����ȡ��Ƶ����
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����֧����·��Ƶ������
	unsigned char *m_audioBuff[3];	//��·��Ƶ�Ļ���,0:����1��1:����2��2:��Ͻ��
	int m_audioBuffSize[3];			//��·��Ƶ�Ļ����С
	int m_curUsage[3];				//��ǰ�����ʹ����
	void addFrameAudio(XMicrophone &mic,XAudioStream &aStr);		
	void addFrameAudio(XAudioStream &aStr0,XAudioStream &aStr1);	//��δʵ��
	void addFrameAudio(XMicrophone &mic0,XMicrophone &mic1);		//��δʵ��
	//---------------------------------------------------------------------
	//����2�����̣߳���Ƶ�豸�ⲿ����
	XBool open(const char *filename,int width,int height,int frameRate,void * audioDevice,
		XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC,XAudioInfo *info = NULL,
		XAVStreamQuality quality = AVS_QUALITY_HEIGHT);   
	void updataFrameRGB(unsigned char*p){addFrameRGB(p);}
	void setStop();
	void setStart();
	AVSampleFormat getSampleFormat()const{return m_audioInfo.sampleFormat;}
private:
	XCritical m_mutex;
	void * m_audioDevice;
	XAudioDeviceType m_audioDeviceType;
	XBool m_isStop;
	XBool m_withThread;
	XThreadState m_threadState;	//�̵߳�״̬ 1������ 2�������˳���3���˳����
	pthread_t m_encodeThread;
	static DWORD WINAPI encodeThread(void * pParam);	//�����߳�
public:
	//����3����Ƶ�豸�ڲ�����
	XBool openEx(const char *filename,int width,int height,int frameRate,const char * deviceName = NULL,
		XAudioDeviceType deviceType = AUDIO_DEVICE_TYPE_MIC);
private:
	XBool m_withDevice;
	XMicrophone m_micDevice;	//��˷����
	XAudioStream m_AudioStream;	//����Ƶ�ļ��ж�ȡ��Ƶ���ݵ���
public:
	//���ô���
	XBool close();  
	float getFrameTime() const {return m_frameTime;}	//��ȡһ֡��ʱ�䣬��λms

	XAVStream()
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
		,m_bsfc(NULL)
		,m_aacbsfc(NULL)
	{
		m_audioBuff[0] = NULL;
		m_audioBuff[1] = NULL;
		m_audioBuff[2] = NULL;
		m_audioBuffSize[0] = 0;
		m_audioBuffSize[1] = 0;
		m_audioBuffSize[2] = 0;
		m_curUsage[0] = 0;
		m_curUsage[1] = 0;
		m_curUsage[2] = 0;
	}
	~XAVStream(){close();}
private:
	AVBitStreamFilterContext *m_bsfc;
	AVBitStreamFilterContext *m_aacbsfc;
	//AVBitStreamFilterContext* bsfc = av_bitstream_filter_init("h264_mp4toannexb");
    //AVBitStreamFilterContext* aacbsfc = av_bitstream_filter_init("aac_adtstoasc");
};  
#if WITH_INLINE_FILE
#include "XAVStream.inl"
#endif
}
#endif 