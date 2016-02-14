//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.7.2
//--------------------------------
#ifndef _JIA_XFFMPEG_
#define _JIA_XFFMPEG_

#include "XOSDefine.h"

#ifdef XEE_OS_WINDOWS
#include <Windows.h>
extern "C" 
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}
#endif
#ifdef XEE_OS_LINUX
#include <pthread.h>
#include "avformat.h"
#include "avcodec.h"
#endif

#pragma comment(lib, "../../engine/lib/ffmpeg/avcodec.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/avformat.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/avutil.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/swresample.lib")
#pragma comment(lib, "../../engine/lib/ffmpeg/swscale.lib")

#include "SDL.h"

namespace XE{
#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE (192000)
#endif

//��Ƶ֡���ݣ�����
struct XVideoFrameData
{
	AVPicture *pict;		//��ǰ֡�Ĺ��ƺ����������
	unsigned char *data;	//��ǰ֡����������
	XVideoFrameData *next;	//��һ֡
	bool isOverFrame;	//�Ƿ��ǽ���֡�����Ľ��������һ������֡�����ڱ�����Ѿ�����
	XVideoFrameData()
		:pict(NULL)
		,data(NULL)
		,next(NULL)
		,isOverFrame(false)
	{}
};
struct XVideoFrameDataEx	//ǿ����
{
	AVFrame *pict;		//��ǰ֡�Ĺ��ƺ����������
	XVideoFrameDataEx *next;	//��һ֡
	bool isOverFrame;	//�Ƿ��ǽ���֡�����Ľ��������һ������֡�����ڱ�����Ѿ�����
	XVideoFrameDataEx()
		:pict(NULL)
		,next(NULL)
		,isOverFrame(false)
	{}
};
//��Ƶ֡����
struct XVideoQueue
{
	XVideoFrameData *first_pict;	//��һ֡
	XVideoFrameData *last_pict;	//���һ֡
	int nb_pict;			//�������ܵ�֡������
	SDL_mutex *mutex;		//�߳���
	XVideoQueue()
		:first_pict(NULL)
		,last_pict(NULL)
		,mutex(NULL)
		,nb_pict(0)
	{}
};
struct XVideoQueueEx	//ǿ����
{
	XVideoFrameDataEx *first_pict;	//��һ֡
	XVideoFrameDataEx *last_pict;	//���һ֡
	int nb_pict;			//�������ܵ�֡������
	SDL_mutex *mutex;		//�߳���
	XVideoQueueEx()
		:first_pict(NULL)
		,last_pict(NULL)
		,mutex(NULL)
		,nb_pict(0)
	{}
};
//��Ƶ֡����
struct XAudioFrameData 
{
    AVPacket pkt;
    XAudioFrameData *next;
	bool isOverFrame;
	XAudioFrameData()
		:next(NULL)
		,isOverFrame(false)
	{}
};
//��Ƶ֡����
struct XAudioQueueEx
{
	XAudioFrameData *first_pkt;//��һ����Ƶ����ָ��
	XAudioFrameData *last_pkt;	//���һ����Ƶ����ָ��
	int nb_packets;			//��Ƶ��������
	int size;				//�ܵ���Ƶ���ݵĳߴ�
	SDL_mutex *mutex;		//�߳���
//	SDL_cond *cond;			//����ͬ��
	XAudioQueueEx()
		:first_pkt(NULL)
		,last_pkt(NULL)
		,mutex(NULL)
//		,cond(NULL)
		,nb_packets(0)
		,size(0)
	{}
};
//��Ƶ֡����
struct XAudioQueue
{
	AVPacketList *first_pkt;//��һ����Ƶ����ָ��
	AVPacketList *last_pkt;	//���һ����Ƶ����ָ��
	int nb_packets;			//��Ƶ��������
	int size;				//�ܵ���Ƶ���ݵĳߴ�
	SDL_mutex *mutex;		//�߳���
	SDL_cond *cond;			//����ͬ��
	XAudioQueue()
		:first_pkt(NULL)
		,last_pkt(NULL)
		,mutex(NULL)
		,cond(NULL)
		,nb_packets(0)
		,size(0)
	{}
};
#define MAX_BUFF_DEEP (40)	//��������,�������Ҫ�ܶ�̬����,���ֵ�������1
inline AVHWAccel *ffFindHwaccel(AVCodecID codec_id,AVPixelFormat pix_fmt)
{
    AVHWAccel *hwaccel = NULL;
	//int i = 0;
    while((hwaccel= av_hwaccel_next(hwaccel)))
	{
        if(hwaccel->id == codec_id && hwaccel->pix_fmt == pix_fmt)
		{
		//	printf("%s\n",hwaccel->name);
		//	++i;
		//	if(i == 2) return hwaccel;
			return hwaccel;
		}
    }
    return NULL;
}
/*
//���������ʹ�ô������⣬��Ҫ����ʹ��
inline int img_convert(AVPicture *dst, AVPixelFormat dst_pix_fmt,  
                const AVPicture *src, AVPixelFormat src_pix_fmt,  
                int src_width, int src_height)  
{  
	static int isInited = 0;
    static SwsContext *pSwsCtx = NULL;  
	if(isInited == 0)
	{
		pSwsCtx = sws_getContext(src_width,src_height,src_pix_fmt,   
                            src_width,src_height,dst_pix_fmt,  
                            SWS_BICUBIC,NULL,NULL,NULL); 
		isInited = 1;
	}
    sws_scale(pSwsCtx, src->data, src->linesize,  
            0,src_height, dst->data, dst->linesize);  
	//sws_freeContext(pSwsCtx);  //��������ͷ���Դ��
    return 0;  
}
*/
//ͨ���Ժ�,����Ч����
namespace XFfmpeg
{
	inline int img_convert(AVPicture *dst, AVPixelFormat dst_pix_fmt,  
					const AVPicture *src, AVPixelFormat src_pix_fmt,  
					int src_width, int src_height)  
	{  
		SwsContext *pSwsCtx = sws_getContext(src_width,src_height,src_pix_fmt,   
								src_width,src_height,dst_pix_fmt,  
								SWS_BICUBIC,NULL,NULL,NULL); 
		sws_scale(pSwsCtx, src->data, src->linesize,  
				0,src_height, dst->data, dst->linesize);  
		sws_freeContext(pSwsCtx);  
		return 0;  
	}
}
}
#endif