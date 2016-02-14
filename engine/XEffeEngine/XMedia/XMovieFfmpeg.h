#ifndef XMOVIE_
#define XMOVIE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//������Ƶ������ת�������⣬�����Ƶʹ�õ���Ƶ�����ʱ���Ϊ44.1k���򽫻������Ƶ������һ�¡�
//��Ƶ�е���Ƶ��Ҫ�������Ҫ��2������16bit��λ��44.1�Ĳ�����
#include "XMovieCore.h"
#include "XFfmpeg.h"
#include "XCritical.h"
#include "XSprite.h"

//�Ľ�������ṩ֧�ֲ��Ŵ������ļ�
namespace XE{
//class XSprite;
//class XTexture;
//#define SDL_AUDIO_BUFFER_SIZE 1024
#define BUFF_TIMER (10.0f)		//�����ʱ��
class XMovieFfmpeg:public XMovieCore
{
private:
	XBool m_isLoaded;	//�Ƿ��������
	void release() {}	//������Դ�ͷ�
public:
	XMovieFfmpeg();
	virtual ~XMovieFfmpeg() {closeClip();}
	virtual XBool getIsLoad()const{return m_isLoaded;}
	//��ӰƬ��ffmpeg���
	XBool load(const char *filename,XColorMode mode = COLOR_RGB,bool withVoice = true);		//����ӰƬ�ļ�������صĳ�ʼ��

	int getClipWidth() const {return m_videoWidth;}		//��ȡӰƬ��ʵ�ʿ��
	int getClipHeight() const {return m_videoHeight;}	//��ȡӰƬ��ʵ�ʸ߶�

	void setLoop(XBool isLoop) {m_isLoop = isLoop;}		//����ӰƬ�Ƿ�ѭ������,XTrue��ѭ��,XFalse���
	double getCurrentPlayingTime() const	//��ȡӰƬ��ǰ����ʱ��(��λ������)
	{
		return m_curFrameNumber*(1000.0/m_curFrameRate);
	}
	int getCurFrameIndex() const {return m_curFrameNumber;}
	int getCurPlayTime() const {return m_curPlayedTime;}	//���غ��뼶�Ĳ��Ž���
	int getAllFrameSum() const {return m_allFrameSum;}
	int getAllTime(void) const				//��ȡӰƬ��ʱ��(��λ������)
	{
		return ((double)pFormatCtx->duration * 0.001f);
	}
	float getVideoFrameRate() const {return m_curFrameRate;}	//���ӰƬ��֡��
	XBool getUpdateFlag(void)		//��ȡ��Ƶ�������Ƿ�Ϊ��
	{
		if(m_videoQueue.nb_pict > 0) return XTrue;
		else return XFalse;
	}

	XBool getIsEnd() const {return m_isEnd;}
	XBool getIsLoop() const {return m_isLoop;}	//��ȡ�Ƿ�ѭ��
	//void setPauseOrPlay(void) {m_isStop = !m_isStop;}		//��ͣ���������ӰƬ
	void pause() {if(!m_isEnd) m_isStop = true;}
	void play();
	XBool isPlaying() const {return !m_isStop && !m_isEnd;}
	void closeClip(void);			//�ر���Ƶ���ţ����ͷ���ص���Դ
	void gotoFrame(float temp);		//������ǰ��Ƶ��ĳһ֡(0 - 1�İٷֱ�)	//������Ƶ����ת��
	void gotoFrameIndex(int index);	//��ת��ĳһ֡�������ĳһ֡
	void gotoTime(int t);	//��ת�����뼶��λ��
	void setAutoTime(bool flag) {m_autoTimer = flag;}	//�����Ƿ��Զ���ʱ��������Զ���ʱ�����߳��Լ��ƽ����Ž��ȣ��������ⲿ���Ʋ��Ž��ȡ�
	//GLuint * getTexture();			//��ȡ��ͼ�ı��
private:
	unsigned char *m_curFrameData;		//ӰƬ��ǰ���ŵ�frame
	//SDL_mutex* m_mutex;				//�߳���
	XVideoFrameData *m_bmp[MAX_BUFF_DEEP];	//ͼ��
	AVPicture m_framePic[MAX_BUFF_DEEP];
	int m_curBuffPoint;
	XBool m_needVoice;				//ӰƬ�Ƿ���Ҫ����
	XBool m_autoTimer;				//�Զ���ʱ��������Զ���ʱ�����߳��Լ��ƽ����Ž��ȣ��������ⲿ���Ʋ��Ž��ȡ�
	//GLuint m_texture;
	//2014-3-20�޸�
	XTexture *m_movieTex;	//��ͼ
	bool m_isGetFirstFrame;	//�Ƿ��ȡ����Ƶ�ĵ�һ֡ͼ��
	bool m_isTexInit;	//��ͼ�Ƿ��Ѿ���ʼ��
	bool m_isNewFrame;
	XCritical m_mutex;
	XColorMode m_outColorMode;	//�������ɫ��ʽ��2014-7-17��δ��ɣ�
	XCritical m_gotoMutex;	//������ת����
	XCritical m_gotoMutex1;	//������ת����
public:
	XColorMode getColorMode()const{return m_outColorMode;}
	XSprite *m_movieSprite;	//���ڻ�ͼ�ľ���
	XBool updateFrame();	//�����Ƿ����µ�ͼ����£���������������ݸ��µ���ͼ
	XBool haveNewFrame();	//ֻ�����Ƿ����������ݸ���
	void pixelLock() {m_mutex.Lock();}
	unsigned char * getDataP() {return m_curFrameData;}	//ע����ʹ�����������һ��Ҫʹ���߳��������������̲߳���ȫ
	void pixelUnlock() {m_mutex.Unlock();}
	void getData(unsigned char * p);
	void updateTex(XTexture &tex);
	void draw();
	unsigned int * getTexture() {return (unsigned int *)&m_movieTex->m_texture;}
private:
	//������������ת��
	SwsContext *m_pSwsContext;	//���ڽ���ͼ���ʽת��
	int imgConvert(AVPicture *dst,const AVPicture *src,int src_height);
	//������Ƶ���ݸ���ת��
	//ע�⣺�Ż� 2014��7��30��
	//����ͨ��ֱ�����ý�����Ҫ�ĸ�ʽ�Ͳ�������ʹ�ý��������ֱ�ӷ���Ҫ�󣬶������ٽ���һ��ת��
	//aCodeCtx �� request_channel_layout��request_sample_fmt��request_channels
	//Ŀǰ��δʵ��
	SwrContext *m_pSwrContext;
private:
	XVideoQueue m_videoQueue;		//��Ƶ֡����
	XAudioQueue m_audioQueue;		//��Ƶ֡����
	AVFormatContext *pFormatCtx;	//��Ƶ��ʽ��������
	AVCodecContext *pCodeCtx;		//��Ƶ������������
	AVCodecContext *aCodeCtx;		//��Ƶ������������
	int m_sreAudioFmt;
	AVFrame *pFrame;				//֡����
	AVFrame *pAudioFrame;			//��Ƶ��֡
	AVPacket packet;				//����Ƶ����ȡ�������ݰ�
	int videoStream;				//��Ƶ��������
	int audioStream;				//��Ƶ��������
//	SDL_AudioSpec wanted_spec;		//�����豸������
//	BOOL bdecdraw;					//�Ƿ��ں�̨��ͼ�׶�??
	XBool m_isBuffing;				//��ǰ�Ƿ���ڻ���״̬

	XBool m_isStop;				//�Ƿ���ͣ����
	XBool m_isEnd;					//�Ƿ񲥷����
	XBool m_isQuit;				//�Ƿ��˳�����
	unsigned long m_curPlayedTime;	//��ǰ���ŵ�ʱ��
	XBool m_isDecodeOver;			//�Ƿ�������

	unsigned long m_allFrameSum;		//ӰƬ����֡��
	unsigned long m_curFrameNumber;		//��ǰ���ŵ�֡��
	float m_curFrameRate;			//��ǰ��Ƶ��֡��
	int m_videoWidth;				//��Ƶ�Ŀ��
	int m_videoHeight;				//��Ƶ�ĸ߶�
	XBool m_isLoop;				//��Ƶ�Ƿ�ѭ������
	//char videofilename[MAX_FILE_NAME_LENGTH];		//��Ƶ���ļ���
	std::string videofilename;		//��Ƶ���ļ���
	XBool m_isThreadDecoderExit;	//�����߳��Ƿ��˳�
	XBool m_isThreadDrawExit;	//��ͼ�߳��Ƿ��˳�

	void getPixelData();				//��ͼ����������Ƶ�����е�ͼƬ������Ļ��
	XBool getFirstPixelData();			//��ȡ�����еĵ�һ֡	2015.1.16���
#ifdef XEE_OS_LINUX
	static void *decoderThread(void *arg);			//��Ƶ�����߳�
	static void *drawThread(void *arg);				//��ͼ�߳�
	static void audioCallBack(void *userdata,Uint8 *stream,int len);	//�����ص�����
#endif
#ifdef XEE_OS_WINDOWS
	static DWORD WINAPI decoderThread(void *arg);	//��Ƶ�����߳�
	static DWORD WINAPI drawThread(void *arg);		//��ͼ�߳�
	static void audioCallBack(void *userdata,Uint8 *stream,int len);	//�����ص�����
#endif
	void initAudioQueue();				//��ʼ����Ƶ����
	void initVideoPictList();			//��ʼ����Ƶ����
	void releaseAudioQueue();			//�ͷ���Ƶ����
	void releaseVideoPictList();		//�ͷ���Ƶ����
	int putIntoPacketQueue();					//����ǰ��õ���Ƶ�����������
	int getFromPacketQueue(AVPacket *pkt);		//����Ƶ��������ȡ����
	int audioDecodeFrame(uint8_t *audio_buf,int buf_size);	//��Ƶ����֡
	void putIntoVideoPicList();					//����ǰ��õ���Ƶ�������뵽����
	unsigned char * getFormVideoPicList();		//����Ƶ֡��������ȡһ֡ͼ������
	unsigned char * getAVideoFrame();			//����Ƶ������ȡ��һ֡���ݣ����ǲ��ı����
	void clearAudioQueue();	//�����Ƶ����
	void clearVideoQueue();	//�����Ƶ����

	//�����ǹ����������ŵ�һЩ����
	uint8_t *m_audioBuf;
	unsigned int m_audioBufSize; 
	unsigned int m_audioBufIndex; 

	//����֮�������ز�
public:
	void replay();
	bool withVideo()const{return videoStream >= 0;}	//�����Ƿ�����Ƶͼ��
	bool withAudio()const{return audioStream >= 0;}	//�����Ƿ�����Ƶ��Ϣ
};
#if WITH_INLINE_FILE
#include "XMovieFfmpeg.inl"
#endif
}
#endif
