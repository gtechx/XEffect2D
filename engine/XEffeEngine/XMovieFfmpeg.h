#ifndef _XMOVIE_
#define _XMOVIE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//������Ƶ������ת�������⣬�����Ƶʹ�õ���Ƶ�����ʱ���Ϊ44.1k���򽫻������Ƶ������һ�¡�
//��Ƶ�е���Ƶ��Ҫ�������Ҫ��2������16bit��λ��44.1�Ĳ�����
#include "XMovieCore.h"

#include "XFfmpeg.h"

//�Ľ�������ṩ֧�ֲ��Ŵ������ļ�

//#define SDL_AUDIO_BUFFER_SIZE 1024
#define BUFF_TIMER (10.0f)		//�����ʱ��

//#pragma comment(lib, "SDL.lib")

struct _XVideoFrameData
{
	AVPicture *pict;		//��ǰ֡�Ĺ��ƺ����������
	unsigned char *data;	//��ǰ֡����������
	struct _XVideoFrameData *next;	//��һ֡
	_XVideoFrameData()
		:pict(NULL)
		,data(NULL)
		,next(NULL)
	{}
};
struct _XVideoQueue
{
	_XVideoFrameData *first_pict;	//��һ֡
	_XVideoFrameData *last_pict;	//���һ֡
	int nb_pict;			//�������ܵ�֡������
	SDL_mutex *mutex;		//�߳���
	_XVideoQueue()
		:first_pict(NULL)
		,last_pict(NULL)
		,mutex(NULL)
	{}
};
struct _XAudioQueue
{
	AVPacketList *first_pkt;//��һ����Ƶ����ָ��
	AVPacketList *last_pkt;	//���һ����Ƶ����ָ��
	int nb_packets;			//��Ƶ��������
	int size;				//�ܵ���Ƶ���ݵĳߴ�
	SDL_mutex *mutex;		//�߳���
	SDL_cond *cond;			//����ͬ��
	_XAudioQueue()
		:first_pkt(NULL)
		,last_pkt(NULL)
		,mutex(NULL)
		,cond(NULL)
	{}
};
#define MAX_BUFF_DEEP (32)	//��������
class _XMovieFfmpeg:public _XMovieCore
{
private:
	_XBool m_isLoaded;	//�Ƿ��������
	void release() {}	//������Դ�ͷ�
public:
	_XMovieFfmpeg();
	~_XMovieFfmpeg(){closeClip();}
	//��ӰƬ��ffmpeg���
	_XBool load(const char *filename,_XColorMode mode = COLOR_RGB,bool withVoice = true);		//����ӰƬ�ļ�������صĳ�ʼ��
	//void waitingForBuffOver();			//�ȴ�ӰƬ������

	int getClipWide() const {return m_videoWidth;}		//��ȡӰƬ��ʵ�ʿ��
	int getClipHeigh() const {return m_videoHeight;}	//��ȡӰƬ��ʵ�ʸ߶�

	void setLoop(_XBool isLoop) {m_isLoop = isLoop;}		//����ӰƬ�Ƿ�ѭ������,XTrue��ѭ��,XFalse���
	double getCurrentPlayingTime() const	//��ȡӰƬ��ǰ����ʱ��(��λ������)
	{
		return m_nowFrameNumber*(1000.0/m_nowFrameRate);
	}
	int getNowFrameIndex() const {return m_nowFrameNumber;}
	int getNowPlayTime() const {return m_nowPlayedTime;}	//���غ��뼶�Ĳ��Ž���
	int getAllFrameSum() const {return m_allFrameSum;}
	int getAllTime(void) const				//��ȡӰƬ��ʱ��(��λ������)
	{
		return ((double)pFormatCtx->duration * 0.001f);
	}
	float getVideoFrameRate() const {return m_nowFrameRate;}	//���ӰƬ��֡��
	_XBool getUpdateFlag(void)		//��ȡ��Ƶ�������Ƿ�Ϊ��
	{
		if(m_videoQueue.nb_pict > 0) return XTrue;
		else return XFalse;
	}

	_XBool getIsEnd() const {return m_isEnd;}
	_XBool getIsLoop() const {return m_isLoop;}	//��ȡ�Ƿ�ѭ��
	//void setPauseOrPlay(void) {m_isStop = !m_isStop;}		//��ͣ���������ӰƬ
	void pause() {if(!m_isEnd) m_isStop = true;}
	void play()
	{
		if(!m_isEnd) m_isStop = false;
		else replay();
	}
	_XBool isPlaying() const {return !m_isStop && !m_isEnd;}
	void closeClip(void);			//�ر���Ƶ���ţ����ͷ���ص���Դ
	void gotoFrame(float temp);		//������ǰ��Ƶ��ĳһ֡(0 - 1�İٷֱ�)	//������Ƶ����ת��
	void gotoFrameIndex(int index);	//��ת��ĳһ֡�������ĳһ֡
	void gotoTime(int t)	//��ת�����뼶��λ��
	{
		m_gotoMutex1.Lock();
		m_nowPlayedTime = t;	//��������ʱ��
		m_gotoMutex1.Unlock();
	}
	void setAutoTime(bool flag) {m_autoTimer = flag;}	//�����Ƿ��Զ���ʱ��������Զ���ʱ�����߳��Լ��ƽ����Ž��ȣ��������ⲿ���Ʋ��Ž��ȡ�
	//GLuint * getTexture();			//��ȡ��ͼ�ı��
private:
	unsigned char *m_nowFrameData;		//ӰƬ��ǰ���ŵ�frame
	//SDL_mutex* m_mutex;				//�߳���
	_XVideoFrameData *m_bmp[MAX_BUFF_DEEP];	//ͼ��
	AVPicture m_framePic[MAX_BUFF_DEEP];
	int m_nowBuffPoint;
	_XBool m_needVoice;				//ӰƬ�Ƿ���Ҫ����
	_XBool m_autoTimer;				//�Զ���ʱ��������Զ���ʱ�����߳��Լ��ƽ����Ž��ȣ��������ⲿ���Ʋ��Ž��ȡ�
	//GLuint m_texture;
	//2014-3-20�޸�
	_XTexture *m_movieTex;	//��ͼ
	bool m_isTexInit;	//��ͼ�Ƿ��Ѿ���ʼ��
	bool m_isNewFrame;
	_XCritical m_mutex;
	_XColorMode m_outColorMode;	//�������ɫ��ʽ��2014-7-17��δ��ɣ�
	_XCritical m_gotoMutex;	//������ת����
	_XCritical m_gotoMutex1;	//������ת����
public:
	_XColorMode getColorMode()const{return m_outColorMode;}
	_XSprite *m_movieSprite;	//���ڻ�ͼ�ľ���
	_XBool upDateFrame()
	{
		if(m_isNewFrame)
		{
			m_isNewFrame = XFalse;
			if(m_movieTex != NULL) 
			{
				m_mutex.Lock();
				m_movieTex->updateTexture(m_nowFrameData);
				m_mutex.Unlock();
				if(!m_isTexInit) m_isTexInit = true;
			}
			return XTrue;
		}
		return XFalse;
	}
	void pixelLock() {m_mutex.Lock();}
	unsigned char * getDataP() {return m_nowFrameData;}	//ע����ʹ�����������һ��Ҫʹ���߳��������������̲߳���ȫ
	void pixelUnlock() {m_mutex.Unlock();}
	void getData(unsigned char * p)
	{
		m_mutex.Lock();
		switch(m_outColorMode)
		{
		case COLOR_RGB:
			memcpy(p,m_nowFrameData,m_videoWidth * m_videoHeight * 3);
			break;
		case COLOR_RGBA:
			memcpy(p,m_nowFrameData,m_videoWidth * (m_videoHeight << 2));
			break;
		}
		m_mutex.Unlock();
	}
	void updateTex(_XTexture &tex)
	{
		m_mutex.Lock();
		tex.updateTexture(m_nowFrameData,m_videoWidth,m_videoHeight);
		m_mutex.Unlock();
	}
	void draw()
	{
		if(!m_isLoaded) return;
		if(m_movieSprite != NULL && m_movieTex != NULL && m_isTexInit) 
			m_movieSprite->draw(&m_movieTex->m_texture);
	}
	unsigned int * getTexture() {return (unsigned int *)&m_movieTex->m_texture;}
private:
	//������������ת��
	SwsContext *m_pSwsContext;	//���ڽ���ͼ���ʽת��
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
	{
		if(m_pSwsContext == NULL) return 0;
		return sws_scale(m_pSwsContext, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize); 
	}
	//������Ƶ���ݸ���ת��
	//ע�⣺�Ż� 2014��7��30��
	//����ͨ��ֱ�����ý�����Ҫ�ĸ�ʽ�Ͳ�������ʹ�ý��������ֱ�ӷ���Ҫ�󣬶������ٽ���һ��ת��
	//aCodeCtx �� request_channel_layout��request_sample_fmt��request_channels
	//Ŀǰ��δʵ��
	SwrContext *m_pSwrContext;
private:
	_XVideoQueue m_videoQueue;		//��Ƶ֡����
	_XAudioQueue m_audioQueue;		//��Ƶ֡����
	AVFormatContext *pFormatCtx;	//��Ƶ��ʽ��������
	AVCodecContext *pCodeCtx;		//��Ƶ������������
	AVCodecContext *aCodeCtx;		//��Ƶ������������
	int m_sreAudioFmt;
	AVFrame *pFrame;				//֡����
	AVPacket packet;				//����Ƶ����ȡ�������ݰ�
	int videoStream;				//��Ƶ��������
	int audioStream;				//��Ƶ��������
//	SDL_AudioSpec wanted_spec;		//�����豸������
//	BOOL bdecdraw;					//�Ƿ��ں�̨��ͼ�׶�??
	_XBool m_isBuffing;				//��ǰ�Ƿ���ڻ���״̬

	_XBool m_isStop;				//�Ƿ���ͣ����
	_XBool m_isEnd;				//�Ƿ񲥷����
	_XBool m_isQuit;				//�Ƿ��˳�����
	int m_nowPlayedTime;		//��ǰ���ŵ�ʱ��
	_XBool m_isDecodeOver;		//�Ƿ�������

	int m_allFrameSum;			//ӰƬ����֡��
	int m_nowFrameNumber;		//��ǰ���ŵ�֡��
	float m_nowFrameRate;		//��ǰ��Ƶ��֡��
	int m_videoWidth;			//��Ƶ�Ŀ��
	int m_videoHeight;			//��Ƶ�ĸ߶�
	_XBool m_isLoop;				//��Ƶ�Ƿ�ѭ������
	char videofilename[MAX_FILE_NAME_LENGTH];		//��Ƶ���ļ���
	_XBool m_isThreadDecoderExit;	//�����߳��Ƿ��˳�
	_XBool m_isThreadDrawExit;	//��ͼ�߳��Ƿ��˳�

	void getPixelData();								//��ͼ����������Ƶ�����е�ͼƬ������Ļ��
#ifdef XEE_OS_LINUX
	friend void *decoderThread(void *arg);			//��Ƶ�����߳�
	friend void *drawThread(void *arg);				//��ͼ����
	friend void audioCallBack(void *userdata,Uint8 *stream,int len);	//�����ص�����
#endif
#ifdef XEE_OS_WINDOWS
	friend DWORD WINAPI decoderThread(void *arg);	//��Ƶ�����߳�
	friend DWORD WINAPI drawThread(void *arg);		//��ͼ����
	friend void audioCallBack(void *userdata,Uint8 *stream,int len);	//�����ص�����
#endif
	void initAudioQueue();			//��ʼ����Ƶ����
	void initVideoPictList();		//��ʼ����Ƶ����
	void releaseAudioQueue();		//�ͷ���Ƶ����
	void releaseVideoPictList();	//�ͷ���Ƶ����
	int putIntoPacketQueue();					//����ǰ��õ���Ƶ�����������
	friend int getFromPacketQueue(_XMovieFfmpeg *pClass,AVPacket *pkt,int block);			//����Ƶ��������ȡ����
	friend int audioDecodeFrame(_XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size);	//��Ƶ����֡
	void putIntoVideoPicList();					//����ǰ��õ���Ƶ�������뵽����
	unsigned char * getFormVideoPicList();		//����Ƶ֡��������ȡһ֡ͼ������

	//�����ǹ����������ŵ�һЩ����
	uint8_t m_audioBuf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	unsigned int m_audioBufSize; 
	unsigned int m_audioBufIndex; 

	//����֮�������ز�
public:
	void replay()
	{
		//������Ҫ�������
		printf("All times:%d\n",m_nowPlayedTime);
		m_nowFrameNumber = 0;
		m_nowPlayedTime = 0;
		printf("Loop!\n");
		gotoFrame(0.0f);
		m_isDecodeOver = XFalse;
		m_isEnd = XFalse;
	}
};
#endif
