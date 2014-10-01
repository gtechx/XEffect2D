//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMovieFfmpeg.h"
#include "XSoundCommon.h"
#include "math.h"
#include "XBasicWindow.h"

_XMovieFfmpeg::_XMovieFfmpeg(void)
:videoStream(-1)
,audioStream(-1)
,m_isBuffing(XTrue)
,m_isLoop(XFalse)
,m_videoWidth(0)
,m_videoHeight(0)
,m_nowFrameNumber(0)
,m_needVoice(XTrue)
,m_autoTimer(XTrue)
//,m_texture(NULL)
,m_isNewFrame(false)
,m_movieTex(NULL)
,m_movieSprite(NULL)
,m_nowFrameData(NULL)
,m_isLoaded(XFalse)
,m_isEnd(XFalse)
,m_isQuit(XFalse)
,m_isStop(XFalse)
,m_isThreadDecoderExit(XTrue)
,m_isThreadDrawExit(XTrue)
,m_pSwsContext(NULL)
,m_pSwrContext(NULL)
,m_outColorMode(COLOR_RGB)
{
	videofilename[0] = '\0';
	m_videoQueue.nb_pict = 0;
}
//����߳����ڸ���ʱ�佫��Ƶ���ݸ��µ���ͼ
#ifdef XEE_OS_LINUX
void * drawThread(void *arg)
#endif
#ifdef XEE_OS_WINDOWS
DWORD WINAPI drawThread(void *arg)
#endif
{
	_XMovieFfmpeg *pthis = (_XMovieFfmpeg *)arg;
	float refreshtime = 1000.0f/(* pthis).m_nowFrameRate;//ӰƬ��ʾһ֡��ʱ�䣬��λ������
	float temptime = refreshtime;
	pthis->m_isThreadDrawExit = XFalse;

	////��һ����ʱ��Ҫ�Ȼ�����
	//while(true)
	//{//�ȴ���Ƶ�������
	//	if(!pthis->m_isBuffing) break;
	//	if(pthis->m_isQuit) break;	//�������˳�ʱ���������Ҫ�˳������������ѭ������ɳ���ֹͣ��Ӧ��
	//	mySleep(1);
	//}	

	int startTime = getCurrentTicks();		//��¼��ʼ���ŵ�ʱ��
	pthis->m_nowPlayedTime = 0;			//���ŵ�ʱ��ͨ�����������

	while(true)
	{	
		if(pthis->m_isQuit) break;
		if(pthis->m_isStop || pthis->m_isEnd || pthis->m_isBuffing)
		{//ֹͣ״̬,���߲��������ȴ�
			mySleep(1);
			startTime = getCurrentTicks();
			continue;
		}
		if(pthis->m_videoQueue.nb_pict > 0)
		{//��Ƶ������������
			pthis->m_gotoMutex1.Lock();
			if(pthis->m_autoTimer) pthis->m_nowPlayedTime += getCurrentTicks() - startTime;
			startTime = getCurrentTicks();
		//	playedTime = getCurrentTicks() - startTime;
			if(pthis->m_nowPlayedTime >= pthis->m_nowFrameNumber * temptime)
			{//����ﵽʱ�����ʼ����
				pthis->getPixelData();
				if(pthis->m_nowFrameNumber >= pthis->m_allFrameSum)
				{//���ʱ��Խ�磬�򲥷����
					pthis->m_isEnd = XTrue;
					if(pthis->m_audioQueue.cond != NULL) 
						SDL_CondSignal(pthis->m_audioQueue.cond);
					LogStr("Time play over+!");
					if(pthis->m_isLoop) pthis->replay();
					startTime = getCurrentTicks();
				}
			}else
			{//������ȵ�ʱ����ٲ���
				if(pthis->m_autoTimer)
				{
					int tempTime = pthis->m_nowFrameNumber * temptime - pthis->m_nowPlayedTime;
					if(tempTime < 0 || tempTime > 2000)
					{
						LogStr("haha");
					}
					mySleep(tempTime);
				}
			}
			pthis->m_gotoMutex1.Unlock();
		}else
		{
			if(pthis->m_isDecodeOver)	//��Ƶ�������
			{//�Ѿ��������
				pthis->m_isEnd = XTrue;
				if(pthis->m_audioQueue.cond != NULL) 
					SDL_CondSignal(pthis->m_audioQueue.cond);
				LogStr("Time play over-!");
				if(pthis->m_isLoop) pthis->replay();
				startTime = getCurrentTicks();
			}else
			{//�ȴ���������
				LogStr("-->buff over!");
				pthis->m_isBuffing = XTrue;	//���ÿ�ʼ��������
			//	mySleep(5000);
			//	pthis->m_isStop = XFalse;
				startTime = getCurrentTicks();
			}
		}
		mySleep(1);
	}
	pthis->m_isThreadDrawExit = XTrue;
//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//��Ƶ�����̣߳���DrawThread�̲߳���ʾ��Ƶ��ʱ�����Ƶ֡���뵽���У�����Ƶ���ŵ�����
#ifdef XEE_OS_WINDOWS
DWORD WINAPI decoderThread(void *arg)
#endif
#ifdef XEE_OS_LINUX
void * decoderThread(void *arg)
#endif
{
	_XMovieFfmpeg *pthis = (_XMovieFfmpeg *)arg;
	//AVPicture pict;
	int bufernum = (int)(BUFF_TIMER * pthis->m_nowFrameRate); //���û�������СΪӰƬ2�벥�ŵ�֡��
	//int frameFinished;		//���붯���Ƿ����
	if(bufernum >= MAX_BUFF_DEEP) bufernum = MAX_BUFF_DEEP;
	//int picIndex = 0;
	//int audioindex = 0;
	pthis->m_isThreadDecoderExit = XFalse;

	while(true)
	{
		if(pthis->m_isQuit) break;	//���ǿ���߳��˳����������˳�
		if(pthis->m_isDecodeOver)
		{//����Ѿ����һ�ν�����ȴ�
			mySleep(1);
			continue;
		}

		if((pthis->m_videoQueue.nb_pict) < bufernum)	//���֡�Ļ�������Ϊ2��
		{//���û�л���������л���
			av_init_packet(&pthis->packet);
			pthis->m_gotoMutex.Lock();
			if(av_read_frame(pthis->pFormatCtx,&pthis->packet) == 0)
			{//�����ȡ֡�ɹ�
				if(pthis->packet.stream_index == pthis->videoStream)// && pthis->packet.duration > 0)
				{//��ȡ������Ƶ֡	
					pthis->putIntoVideoPicList();
					//picIndex ++;
				}else
				if(pthis->packet.stream_index == pthis->audioStream)// && pthis->packet.duration > 0)
				{//�����ȡ������Ƶ֡
					//audioindex ++;
					if(pthis->m_needVoice) pthis->putIntoPacketQueue();
					else av_free_packet(&pthis->packet);	//�������Ҫ������Ƶ��ֱ�ӷ�������Ƶ����(������������)
				}else
				{//��֪����ȡ����ɶ����
					av_free_packet(&pthis->packet);	//(������������)
				}
			}else	//�����ȡ֡ʧ�ܣ���Ĭ���Ѿ�������else
			{//��Ƶ�������
				pthis->m_isDecodeOver = XTrue;
			}
			pthis->m_gotoMutex.Unlock();
		}else//�����ʾ�������� 
		if(pthis->m_isBuffing)	//��һ�β��ŵ�ʱ��һ��Ҫ�Ȼ�����
		{//������ϣ�����bplay , ���в���
			pthis->m_isBuffing = XFalse;
		}
		mySleep(1);
	}
	pthis->m_isThreadDecoderExit = XTrue;
//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//int createNumber = 0;
//����Ƶ������ȡ����Ƶ��
int getFromPacketQueue(_XMovieFfmpeg *pClass,AVPacket *pkt,int block)
{ 
	AVPacketList *pkt1; 
	int ret; 

	SDL_LockMutex(pClass->m_audioQueue.mutex); 
	while(true)
	{ 
		if(pClass->m_isStop)
		{//��ͣ�Ļ�����ֹͣ��������
			SDL_UnlockMutex(pClass->m_audioQueue.mutex); 
			return -1;
		}

		pkt1 = pClass->m_audioQueue.first_pkt; 
		if(pkt1 != NULL) 
		{//������
			pClass->m_audioQueue.first_pkt = pkt1->next; 
			if(pClass->m_audioQueue.first_pkt == NULL) pClass->m_audioQueue.last_pkt = NULL;	//�������ݶ��Ѿ�ȡ��
			--pClass->m_audioQueue.nb_packets; 
			pClass->m_audioQueue.size -= pkt1->pkt.size; 
			*pkt = pkt1->pkt; 
			XDELETE(pkt1);	//ɾ����ʱ����
		//	-- createNumber;
		//	printf("%d,%d\n",createNumber,pClass->m_audioQueue.nb_packets);
			ret = 1; 
			break; 
		}else	//�����Ĵ����п��ܻ��������Խ��Խ��
		if(block == 0)	//ֻ��ȡһ������
		{ 
			ret = 0; 
			break; 
		}else 
		{//�ȴ�ͬ��(ԭ��������)
			//��������Ϊ���ȴ�ͬ��
			if(pClass->m_isQuit || pClass->m_isEnd)
			{
				SDL_UnlockMutex(pClass->m_audioQueue.mutex); 
				return -1;	//���ȴ�ͬ��
			}
			SDL_CondWait(pClass->m_audioQueue.cond,pClass->m_audioQueue.mutex); 
			if(pClass->m_isQuit || pClass->m_isEnd) 
			{
				SDL_UnlockMutex(pClass->m_audioQueue.mutex); 
				return -1;	//���ȴ�ͬ��
			}
		} 
		mySleep(1);
	} 
	SDL_UnlockMutex(pClass->m_audioQueue.mutex); 
	return ret; 
} 
//����Ƶ���������Ƶ����
int audioDecodeFrame(_XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size)
{
	AVCodecContext * c= pClass->aCodeCtx;
	AVPacket pkt,pkt1;
	AVFrame *pAudioFrame = avcodec_alloc_frame();  
	uint8_t *out[] = {audio_buf}; 
	int pkt_pos,pkt_len;
	int len = 0;
	int data_size = 0; 
	int isFinished;
	while(true)
	{
		if(pClass->m_isQuit) return -1;
		if(pClass->m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������
		if(getFromPacketQueue(pClass,&pkt, 1) < 0) return -1;//�����Ƶ��û�����ݣ��򷵻�  
		pkt1 = pkt;  
        pkt_pos = 0;  
        pkt_len = pkt.size;

		if(pkt.size > 0)
		{
			while(true)
			{
				len = avcodec_decode_audio4(c,pAudioFrame,&isFinished,&pkt1);
				if(len < 0)
				{//��ѹʧ��
					av_free_packet(&pkt);
					LogStr("��Ƶ��û������!");
					return -1;
				}
				if(isFinished)
				{//��ʽת��
				//	SwrContext *swr = swr_alloc();
				//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
				//		c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
				//	if(swr_init(pClass->m_pSwrContext)<0)
				//	{
				//		LogStr("swr_init() fail");
				//		return -1;
				//	}
					int outSize = buf_size/XEE::audioChannel/av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
					len = swr_convert(pClass->m_pSwrContext,out,outSize,(const uint8_t **)pAudioFrame->extended_data,pAudioFrame->nb_samples);  
					len = len * XEE::audioChannel * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

	                avcodec_free_frame(&pAudioFrame);  
	                av_free_packet(&pkt);
				//	swr_free(&swr);
	                return len;		//�߼���������ܴ������ݰ���ʧ,�����ʱ���ݰ������ݲ�û����ȫ����ɹ��Ļ�����ᱻ����
	            }else
				{  
	                if(!pkt1.data && c->codec->capabilities & CODEC_CAP_DELAY)
					{  
	                    break;  
	                }  
	            }  
				pkt_pos += len;//�Ѿ�����ĳ���  
				pkt1.data = pkt.data + pkt_pos;  
				pkt1.size = pkt.size - pkt_pos; 
				if(pkt1.size <= 0) break;

				if(pClass->m_isQuit) return -1;
				if(pClass->m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������
			}
		}
		av_free_packet(&pkt);
	}
	avcodec_free_frame(&pAudioFrame);
	return data_size;
}
/*
int audioDecodeFrame(_XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size)
{ 
	static AVPacket pkt;
	static uint8_t *audio_pkt_data = NULL; 
	static int audio_pkt_size = 0; 
	int len = 0;
	int data_size = 0; 
	AVCodecContext * c= pClass->aCodeCtx;
	while(true)
	{ 
		if(pClass->m_isQuit) return -1;
		//while(audio_pkt_size > 0) 
		if(audio_pkt_size > 0)
		{ 
			if(pClass->m_isQuit) return -1;
			data_size = buf_size; 

			len = avcodec_decode_audio3(c,(int16_t *)audio_buf,&data_size,&pkt);//audio_pkt_data,audio_pkt_size); //��������
			if(len < 0) 
			{//���뷢������ 
				av_free_packet(&pkt);
				audio_pkt_size = 0; 
				return -1;
			} 
			audio_pkt_data += len;		//���һ����Ƶ����û��������ظ�����ֱ�����Ϊֹ
			audio_pkt_size -= len; 
			if(pkt.data != NULL) av_free_packet(&pkt);//�ͷ���Դ
			audio_pkt_size = 0;	//���������������
			if(data_size <= 0) return -1; 

			//�������ת��
			if(c->sample_fmt != AV_SAMPLE_FMT_S16)
			{
				SwrContext *swr = swr_alloc();
				swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
					c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);

				if(swr_init(swr)<0)
				{
					printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
					return -1;
				}
				uint8_t *out[] = {audio_buf};
				uint8_t tempTest[50000];	//ע�������ֵ
				int ret = 0;
				memcpy(tempTest,audio_buf,data_size);
				if(c->sample_fmt == AV_SAMPLE_FMT_S16P)
				{
					const uint8_t *in[] = {tempTest,tempTest + (data_size >> 1)};
					ret = swr_convert(swr,out,data_size,in,(data_size >> 2));
				}else
				if(c->sample_fmt == AV_SAMPLE_FMT_FLTP)
				{//�����������,���ص�����
					const uint8_t *in[] = {tempTest};
					ret = swr_convert(swr,out,data_size,in,data_size);
				}
				swr_free(&swr);
			}
			return data_size;
		} 
		if(pkt.data != NULL) av_free_packet(&pkt);	//�ͷŵ��ڴ�ռ� 
 
		if(pClass->m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������
		if(getFromPacketQueue(pClass,&pkt, 1) < 0) return -1;//�����Ƶ��û�����ݣ��򷵻�  

		audio_pkt_data = pkt.data; 
		audio_pkt_size = pkt.size; 
	} 
}*/

//�����ص�������������Ƶ����,����Ƶ�����Ƶ����湩SDL����
void audioCallBack(void *userdata,Uint8 *stream,int len)
{
//	printf("S + ");
	_XMovieFfmpeg * pClass = (_XMovieFfmpeg *)userdata;

	int len1 = 0;
	int audio_size = 0; 
	while(len > 0)
	{//����������Ҫ����ʱ 	
		if(pClass->m_audioBufIndex >= pClass->m_audioBufSize)
		{//����ѹ����Ƶȫ���������ʱ�Ž�ѹ�������Ƶ
			if(pClass->m_isQuit || pClass->m_isBuffing || 
				pClass->m_isStop || pClass->m_isEnd)
			{//����Ҫ����������ʱ�򲥷ſհ�����(�����������հ����ݵĻ����������ʾ����������Ҫ��ϸ����ԭ��)
				if(len > 1024) pClass->m_audioBufSize = 1024;	//���û����������������,��Ҫ����ͣ��ʱ��
				else pClass->m_audioBufSize = len;
				memset(pClass->m_audioBuf, 0, pClass->m_audioBufSize); 
//				printf("O +");
			}else
			{
				audio_size = audioDecodeFrame(pClass,pClass->m_audioBuf,sizeof(pClass->m_audioBuf));//�������뺯�� 
				if(audio_size < 0) 
				{//�����Ƶ����û������.������ڴ�ռ�
					if(len > 1024) pClass->m_audioBufSize = 1024;	//���û����������������,��Ҫ����ͣ��ʱ��
					else pClass->m_audioBufSize = len;
					memset(pClass->m_audioBuf, 0, pClass->m_audioBufSize); 
//					printf("OV +");
				}else
				{//��¼��ѹ��������Ƶ������
					pClass->m_audioBufSize = audio_size; 
				} 
			}
			pClass->m_audioBufIndex = 0; 
		} 
		len1 = pClass->m_audioBufSize - pClass->m_audioBufIndex;	//��¼��Ƶ�ĳ���
		if(len1 > len) len1 = len;
		//�������д�����Ŀǰ�Ŀ���¹���һ��
		memcpy(stream,(uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex,len1);
		//SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
		
		len -= len1; 
		stream += len1; 
		pClass->m_audioBufIndex += len1; 
	}
//	printf("E\n");
}
//void audioCallBack(void *userdata,Uint8 *stream,int len)
//{
//	_XMovieFfmpeg * pClass = (_XMovieFfmpeg *)userdata;
//	//AVCodecContext *aCodecCtx = (AVCodecContext *)userdata; 
//	int len1 = 0;
//	int audio_size = 0; 
//
//	//static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
//	//static unsigned int audio_buf_size = 0; 
//	//static unsigned int audio_buf_index = 0; 
//	//uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
//	//unsigned int audio_buf_size = 0; 
//	//unsigned int audio_buf_index = 0; 
//
//	while(len > 0)
//	{//����������Ҫ����ʱ 	
//		if(pClass->m_audioBufIndex >= pClass->m_audioBufSize)
//		{//����ѹ����Ƶȫ���������ʱ�Ž�ѹ�������Ƶ
//			if(pClass->m_isQuit) return; 
//			audio_size = audioDecodeFrame(pClass,pClass->m_audioBuf,sizeof(pClass->m_audioBuf));//�������뺯�� 
//			if(audio_size < 0) 
//			{//�����Ƶ����û������.������ڴ�ռ�
//			//	pClass->m_audioBufSize = 1024; 
//			//	memset(pClass->m_audioBuf,0,pClass->m_audioBufSize); 
//				pClass->m_audioBufSize = 0;
//			}else
//			{//��¼��ѹ��������Ƶ������
//				pClass->m_audioBufSize = audio_size; 
//			} 
//			pClass->m_audioBufIndex = 0; 
//		} 
//		len1 = pClass->m_audioBufSize - pClass->m_audioBufIndex;	//��¼��Ƶ�ĳ���
//		if(len1 > len) len1 = len;
//		
//		//������Ҫ����Ƶ����ת��,SDL����������
//	/*	if(XEE::audioFrequency != pClass->aCodeCtx->sample_rate && len1 > 0)
//		{
//			SDL_AudioCVT  wav_cvt;
//			int ret = SDL_BuildAudioCVT(&wav_cvt,
//				XEE::audioFormat,XEE::audioChannel,pClass->aCodeCtx->sample_rate,
//				XEE::audioFormat,XEE::audioChannel,XEE::audioFrequency);
//			if(ret == -1) printf("Couldn't build converter!\n");
//			wav_cvt.buf = (Uint8 *)malloc(len1 * wav_cvt.len_mult);
//			wav_cvt.len = len1;
//			memcpy(wav_cvt.buf,pClass->m_audioBuf + pClass->m_audioBufIndex,wav_cvt.len);
//			if(SDL_ConvertAudio(&wav_cvt) == -1)
//			{
//				printf("conver error!");
//			}
//			SDL_MixAudio(stream,wav_cvt.buf, wav_cvt.len_cvt,128);
//			free(wav_cvt.buf);
//		}else
//		if(len1 > 0)
//		{
//			SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		}*/
//		SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		//����ʹ��FFMPEG�ķ�ʽ����ת��
//		//AVCodecContext * c= pClass->aCodeCtx;
//		//if(XEE::audioFrequency != c->sample_rate 
//		//	|| c->sample_fmt != AV_SAMPLE_FMT_S16
//		//	|| c->channels != XEE::audioChannel)
//		//{
//		//	int dataPackSize = av_samples_get_buffer_size(NULL,c->channels,c->frame_size,c->sample_fmt, 1);
//		//	SwrContext *swr = swr_alloc();
//		//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
//		//		c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
//		//	if(swr_init(swr)<0)
//		//	{
//		//		printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
//		//		return;
//		//	}
//		//	uint8_t tempTest[50000];	//ע�������ֵ
//		//	memcpy(tempTest + pClass->m_audioBufIndex,pClass->m_audioBuf,dataPackSize);
//		//	uint8_t *out[] = {tempTest};
//		//	const uint8_t *in[] = {pClass->m_audioBuf + pClass->m_audioBufIndex,pClass->m_audioBuf + pClass->m_audioBufIndex + (dataPackSize >> 1)};
//		//	swr_convert(swr,out,dataPackSize,in,(dataPackSize >> 2));
//		//	swr_free(&swr);
//
//		//	SDL_MixAudio(stream,tempTest,dataPackSize,128);
//		//}else
//		//{
//		//	SDL_MixAudio(stream, (uint8_t *)pClass->m_audioBuf + pClass->m_audioBufIndex, len1,128);
//		//}
//		len -= len1; 
//		stream += len1; 
//		pClass->m_audioBufIndex += len1; 
//	//	mySleep(1);
//	}
//}
int selectChannelLayout(AVCodec *codec) 
{ 
    const uint64_t *p; 
    uint64_t bestChLayout = 0; 
    int bestNbChannells   = 0; 
 
    if (!codec->channel_layouts) 
        return AV_CH_LAYOUT_STEREO; 
 
    p = codec->channel_layouts; 
    while (*p) { 
        int nb_channels = av_get_channel_layout_nb_channels(*p); 
 
        if (nb_channels > bestNbChannells) { 
            bestChLayout    = *p; 
            bestNbChannells = nb_channels; 
        } 
        p++; 
    } 
    return bestNbChannells; 
} 	
//��ӰƬ ����ʼ��SDL , ffmpeg���
_XBool _XMovieFfmpeg::load(const char *filename,_XColorMode mode,bool withVoice)
{
	if(m_isLoaded) return XFalse;
	m_isTexInit = false;
	m_outColorMode = mode;
	videoStream = -1;
	audioStream = -1;
	m_isBuffing = XTrue;
	m_isStop = XTrue;
//	m_isLoop = FALSE;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_nowFrameNumber = 0;
	m_videoQueue.nb_pict = 0;
	m_isEnd = XFalse;
	m_isQuit = XFalse;
	m_needVoice = withVoice;

	int i = 0;
	if(filename == NULL) return XFalse;
	strcpy(videofilename,filename);		//��¼��Ƶ�ļ���
	av_register_all();					//��ʼ��ffmpeg��
//	avcodec_register_all();
//	avformat_network_init();
	pFormatCtx = NULL;
	if(avformat_open_input(&pFormatCtx,videofilename,NULL,NULL) != 0)	//����Ƶ�ļ�
	{
		LogStr("can not open input file!");
		return XFalse;
	}
	if(av_find_stream_info(pFormatCtx) < 0)		//�����Ƶ����Ϣ
	{
		LogStr("can not find stream information!");
		return XFalse;
	}

	for(i = 0;i < pFormatCtx->nb_streams;++ i)
	{
		if((pFormatCtx->streams[i]->codec->codec_type) == AVMEDIA_TYPE_VIDEO && videoStream < 0)
		{//����Ƶ��Ϣ�в�����Ƶ���������Ϣ
			videoStream = i;
		}
		if((pFormatCtx->streams[i]->codec->codec_type) == AVMEDIA_TYPE_AUDIO && audioStream < 0)
		{//����Ƶ��Ϣ�в�����Ƶ���������Ϣ
			audioStream = i;
		}
	}

	m_videoWidth = 0;			//�����Ƶ�Ŀ��
	m_videoHeight = 0;			//�����Ƶ�ĸ߶�
	if(videoStream != -1)
	{//����ļ��д�����Ƶ��
		pCodeCtx = pFormatCtx->streams[videoStream]->codec;		//�õ���Ƶ���ı������Ϣ
		AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);		//��ffmpeg���в�������Ƶ�������Ϣ��ƥ��ı������

		if(pCodec == NULL)
		{//�Ҳ�����Ӧ�Ľ��뷽ʽ
			fprintf(stderr , "Unsupported codec!");
			return XFalse;
		}
		if(avcodec_open2(pCodeCtx ,pCodec,NULL) < 0)
		{//������Ƶ�������Ϣ��ƥ��ı������
			LogStr("can not open video decoder!");
			return XFalse;
		}
		initVideoPictList();	//��ʼ����Ƶ֡����
		m_videoWidth = pCodeCtx->width;			//�����Ƶ�Ŀ��
		m_videoHeight = pCodeCtx->height;			//�����Ƶ�ĸ߶�
	}

	if(audioStream != -1 && m_needVoice)
	{//���������Ƶ�������濪����Ƶ����
		aCodeCtx = pFormatCtx->streams[audioStream]->codec;	//�õ���Ƶ���ı������Ϣ
		AVCodec *aCodec = avcodec_find_decoder(aCodeCtx->codec_id);	//��ffmpeg���в�������Ƶ�������Ϣ��ƥ��ı������
		if(aCodec == NULL)
		{//�Ҳ�����Ƶ������
			LogStr("can not find audio decoder information!");
		}else 
		if(avcodec_open2(aCodeCtx ,aCodec,NULL) < 0)
		{//������Ƶ�������Ϣ��ƥ��ı������
			LogStr("can not open audio decoder!");
		}else
		{//���ݲ��ҵ�����Ƶ�������Ϣ����SDL�貥�ŵ���Ƶ��Ϣ
			initAudioQueue();//��ʼ������������
		}
		//������Ƶ��ʽ�Ķ���ǳ�����,�����������ͳһ
		m_sreAudioFmt = AUDIO_S16;
		switch(aCodeCtx->sample_fmt)	//���в�ͬ�Ķ���ת��
		{
		case AV_SAMPLE_FMT_U8:m_sreAudioFmt = AUDIO_S8;break;
		case AV_SAMPLE_FMT_S16:m_sreAudioFmt = AUDIO_S16LSB;break;
		//case AV_SAMPLE_FMT_S24:m_sreAudioFmt = AUDIO_S16LSB;break;	//��֧��
		case AV_SAMPLE_FMT_S32:m_sreAudioFmt = AUDIO_S16LSB;break;	//��֧��
		case AV_SAMPLE_FMT_FLT:m_sreAudioFmt = AUDIO_S16LSB;break;	//��֧��
		}
		m_pSwrContext = swr_alloc();
		if(m_pSwrContext == NULL) return XFalse;
		if(aCodeCtx->channel_layout == 0) aCodeCtx->channel_layout = av_get_default_channel_layout(aCodeCtx->channels);	//����������û�����õĻ��������������
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
			aCodeCtx->channel_layout,aCodeCtx->sample_fmt,aCodeCtx->sample_rate,0,NULL);
		if(swr_init(m_pSwrContext) < 0)
		{
			LogStr("swr_init() fail");
			return XFalse;
		}
		m_audioBufSize = 0; 
		m_audioBufIndex = 0; 
	}

	pFrame = avcodec_alloc_frame();//����һ֡���ڴ�ռ䣬���ڴ�Ž�������Ƶ��Ϣ
	if(pFrame == NULL)
	{//��������ڴ�ʧ�ܣ���������ʾ
		LogStr("malloc Frame failed!");
		return XFalse;
	}

	if(pFormatCtx->streams[videoStream]->avg_frame_rate.den <= 0)
	{
		m_nowFrameRate = (double)(pFormatCtx->streams[videoStream]->time_base.den)/(double)(pFormatCtx->streams[videoStream]->time_base.num);
	}else
	{
		m_nowFrameRate = (double)(pFormatCtx->streams[videoStream]->avg_frame_rate.num)/(double)(pFormatCtx->streams[videoStream]->avg_frame_rate.den);
	}
	m_allFrameSum = pFormatCtx->streams[videoStream]->duration;
	//����OpenGL��ͼ
	if(videoStream != -1)
	{
		XDELETE(m_movieTex);
		XDELETE(m_movieSprite);
		m_movieTex = createMem<_XTexture>();
		if(m_movieTex == NULL) return XFalse;
		m_movieSprite = createMem<_XSprite>();
		if(m_movieSprite == NULL) return XFalse;
		m_movieSprite->init(m_videoWidth,m_videoHeight,0);
		XDELETE_ARRAY(m_nowFrameData);
		switch(m_outColorMode)
		{
		case COLOR_RGB:
			m_movieTex->createTexture(m_videoWidth,m_videoHeight,COLOR_RGB);
			m_nowFrameData = createArrayMem<unsigned char>(m_videoWidth * m_videoHeight * 3);
			memset(m_nowFrameData,0,m_videoWidth * m_videoHeight * 3);
			break;
		case COLOR_RGBA:
			m_movieTex->createTexture(m_videoWidth,m_videoHeight,COLOR_RGBA);
			m_nowFrameData = createArrayMem<unsigned char>(m_videoWidth * (m_videoHeight << 2));
			memset(m_nowFrameData,0,m_videoWidth * (m_videoHeight << 2));
			break;
		default:
			return XFalse;	//������ʽĿǰ��֧��
			break;
		}
		if(m_nowFrameData == NULL) return XFalse;
//		if(glIsTexture(m_texture)) 
//		{
//			printf("delete texture:%d\n",m_texture);
//			glDeleteTextures(1,&m_texture);
//		}
//		glGenTextures(1,&m_texture);
//		glBindTexture(GL_TEXTURE_2D, m_texture);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if WITH_XSPRITE_EX
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//#endif
//		//�����޸Ľ���һ������2��n�η�����ͼ
//		int wR = (int)powf(2.0, ceilf(logf((float)m_videoWidth)/logf(2.0f)));
//		int hR = (int)powf(2.0, ceilf(logf((float)m_videoHeight)/logf(2.0f)));
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,wR,hR, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,m_videoWidth,m_videoHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		//Ϊ��������ڴ�ռ�,��ͼ������ת����RGB24����ʽ
		int dataSize;
		switch(m_outColorMode)
		{
		case COLOR_RGB:
			dataSize = pCodeCtx->width * pCodeCtx->height * 3;
			m_pSwsContext = sws_getContext(pCodeCtx->width,pCodeCtx->height,pCodeCtx->pix_fmt,   
				pCodeCtx->width,pCodeCtx->height,PIX_FMT_RGB24,SWS_POINT,NULL,NULL,NULL);	//SWS_BICUBIC
			break;
		case COLOR_RGBA:
			dataSize = pCodeCtx->width * (pCodeCtx->height << 2);
			m_pSwsContext = sws_getContext(pCodeCtx->width,pCodeCtx->height,pCodeCtx->pix_fmt,   
				pCodeCtx->width,pCodeCtx->height,PIX_FMT_RGBA,SWS_POINT,NULL,NULL,NULL);	//SWS_BICUBIC
			break;
		default:
			return XFalse;
			break;
		}
		if(m_pSwsContext == NULL) return XFalse;
		//int numBytes = avpicture_get_size(PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);	//��ȡ�ڴ�ߴ�
		m_nowBuffPoint = 0;
		for(i = 0;i < MAX_BUFF_DEEP;++ i)
		{
			m_bmp[i] = createMem<_XVideoFrameData>();
			if(m_bmp[i] == NULL) return XFalse;

			m_bmp[i]->data = createArrayMem<unsigned char>(dataSize);
			if(m_bmp[i]->data == NULL) return XFalse;
			switch(m_outColorMode)
			{
			case COLOR_RGB:
				avpicture_fill(&m_framePic[i],m_bmp[i]->data,PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);
				break;
			case COLOR_RGBA:
				avpicture_fill(&m_framePic[i],m_bmp[i]->data,PIX_FMT_RGBA,pCodeCtx->width,pCodeCtx->height);
				break;
			}

			m_bmp[i]->pict = &m_framePic[i];
		}
	}

	m_isDecodeOver = XFalse;	//��ǿ�ʼ����
    //�����߳�
#ifdef XEE_OS_LINUX
	pthread_t tempP;
    if(pthread_create(&tempP, NULL, &decoderThread, (void*) this) != 0) 
	{
		LogStr("open thread error!");
	}
	if(audioStream != -1 &&  m_needVoice)
	{//���������Ƶ�������￪����Ƶ������
	//	_XSoundHandle.pauseMusic();
		_XSoundHandle.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	_XSoundHandle.resumeMusic();
	}
    if(pthread_create(&tempP, NULL, &drawThread, (void*) this) != 0) 
	{
		LogStr("open thread error!");
	}
#endif
#ifdef XEE_OS_WINDOWS
    if(CreateThread(0,0,decoderThread,this,0,NULL) == 0) 
	{
		LogStr("open thread error!");
	}
	if(audioStream != -1 &&  m_needVoice)
	{//���������Ƶ�������￪����Ƶ������
	//	_XSoundHandle.pauseMusic();
		_XSoundHandle.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	_XSoundHandle.resumeMusic();
	}
    if(CreateThread(0,0,drawThread,this,0,NULL) == 0) 
	{
		LogStr("open thread error!");
	}
#endif
	LogNull("AllFrames:%d,FPS:%f",(* this).m_allFrameSum,(* this).getVideoFrameRate());
	//m_mutex = SDL_CreateMutex();

	m_isLoaded = XTrue;

	return XTrue;
}
////��ʼ����ӰƬ���������Ӧ�����ڲ�ͣ�Ĳ���ѹ��������ƵƬ�ϣ�
//void _XMovieFfmpeg::waitingForBuffOver(void)
//{
//	while(true)
//	{//�ȴ���Ƶ�������
//		if(!m_isBuffing)
//		{//��������֮��������豸
//			if(m_isQuit) break;	//�������˳�ʱ���������Ҫ�˳������������ѭ������ɳ���ֹͣ��Ӧ��
//			break;
//		}
//		mySleep(1);
//	}	
//}
//�ر�ӰƬ����
void _XMovieFfmpeg::closeClip(void)
{
	if(!m_isLoaded) return;
	m_isQuit = XTrue;
	if(m_audioQueue.cond != NULL) 
		SDL_CondSignal(m_audioQueue.cond);

	//mySleep(20);	//�ȴ��߳��˳�
	while(true)
	{
		if(m_isThreadDecoderExit && m_isThreadDrawExit) break;
		Sleep(1);
	}
	_XSoundHandle.setCallBack(NULL,NULL);	//�������֮��ȡ������
	avcodec_free_frame(&pFrame);
	avcodec_close(pCodeCtx);
	if(audioStream != -1 && m_needVoice) avcodec_close(aCodeCtx);
	av_close_input_file(pFormatCtx);
	releaseVideoPictList();
	releaseAudioQueue();

	XDELETE(m_movieTex);
	XDELETE(m_movieSprite);
	XDELETE_ARRAY(m_nowFrameData);
	//if(glIsTexture(m_texture))
	//{
	//	printf("delete texture:%d\n",m_texture);
	//	glDeleteTextures(1,&m_texture);
	//}
	//SDL_DestroyMutex(m_mutex);
	SDL_DestroyMutex(m_audioQueue.mutex);
	SDL_DestroyCond(m_audioQueue.cond);
	SDL_DestroyMutex(m_videoQueue.mutex);
	sws_freeContext(m_pSwsContext);
	if(audioStream != -1 && m_needVoice) swr_free(&m_pSwrContext);
	m_isLoaded = XFalse;
}
void _XMovieFfmpeg::gotoFrame(float temp)	//������ǰ��Ƶ��ĳһ֡
{
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 1.0f) temp = 1.0f;

	int armTime = temp * m_allFrameSum;
	m_gotoMutex.Lock();
	m_gotoMutex1.Lock();
	av_seek_frame(pFormatCtx,videoStream,armTime,AVSEEK_FLAG_ANY);
	//av_seek_frame(pFormatCtx,audioStream,armTime,AVSEEK_FLAG_BACKWARD);
	//printf("%d,%d,%d,%d\n",rt,m_allFrameSum,armTime,pFormatCtx->timestamp);
	m_nowFrameNumber = temp * m_allFrameSum - m_videoQueue.nb_pict;	//�û����е����ݿ��ٲ������
	m_nowPlayedTime = m_nowFrameNumber * 1000.0f / m_nowFrameRate;	//��������ʱ��
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
void _XMovieFfmpeg::gotoFrameIndex(int index)
{
	if(index == m_nowFrameNumber) return;
	if(index < 0 || index > m_allFrameSum) return;
	m_gotoMutex.Lock();
	m_gotoMutex1.Lock();
	av_seek_frame(pFormatCtx,videoStream,index,AVSEEK_FLAG_ANY);
	m_nowFrameNumber = index - m_videoQueue.nb_pict;	//�û����е����ݿ��ٲ������
	m_nowPlayedTime = m_nowFrameNumber * 1000.0f / m_nowFrameRate;	//��������ʱ��
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
//��YUV��ʽ����Ƶ֡��ʾ������
void _XMovieFfmpeg::getPixelData()
{  
	SDL_LockMutex(m_videoQueue.mutex);
	unsigned char * p = getFormVideoPicList();
	if(p == NULL) return;
	m_mutex.Lock();
	//�����ݿ�������
	switch(m_outColorMode)
	{
	case COLOR_RGB:
		memcpy(m_nowFrameData,p,m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
		memcpy(m_nowFrameData,p,m_videoWidth * (m_videoHeight << 2));
		break;
	}
	m_mutex.Unlock();
	SDL_UnlockMutex(m_videoQueue.mutex);
	m_isNewFrame = true;
	++ m_nowFrameNumber;
}

//GLuint * _XMovieFfmpeg::getTexture()
//{
//	if(m_nowFrame != NULL)
//	{//�����ǰ֡��������RGB����ˢ�µ���ͼ
//		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D,m_texture);
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
//			m_videoWidth,m_videoHeight, GL_RGB,//GL_LUMINANCE,	
//			GL_UNSIGNED_BYTE,m_nowFrame);
//		glDisable( GL_TEXTURE_2D );
//		m_nowFrame = NULL;	//��������Ѿ�ˢ��
//	}
//	return &m_texture;
//}

//��ʼ����Ƶ����
void _XMovieFfmpeg::initVideoPictList()
{
	memset(&m_videoQueue,0,sizeof(_XVideoQueue));
	m_videoQueue.mutex = SDL_CreateMutex();
}

void _XMovieFfmpeg::releaseVideoPictList()
{
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
		if(m_bmp[i] != NULL)
			XDELETE_ARRAY(m_bmp[i]->data);
		XDELETE(m_bmp[i]);
	}
} 
void _XMovieFfmpeg::putIntoVideoPicList()
{
	int isFinished = 0;
	if(avcodec_decode_video2(pCodeCtx,pFrame,&isFinished,&packet) < 0) 
	{//��Ƶ����ʧ��
		av_free_packet(&packet); 
		return;	
	}
	av_free_packet(&packet); 
	if(isFinished != 0)
	{//�������ɹ�		
	//	SDL_LockMutex(m_mutex);
		SDL_LockMutex(m_videoQueue.mutex);
		//����Ƶ���ݸ�ʽת����RGB24��ʽ
		imgConvert(m_bmp[m_nowBuffPoint]->pict,(AVPicture *)pFrame,pCodeCtx->height); 
		//����Ƶ�������������
		m_bmp[m_nowBuffPoint]->next = NULL;
		if(m_videoQueue.last_pict == NULL) m_videoQueue.first_pict = m_bmp[m_nowBuffPoint];//����ǿ���
		else m_videoQueue.last_pict->next = m_bmp[m_nowBuffPoint];//�������һ֡
		
		m_videoQueue.last_pict = m_bmp[m_nowBuffPoint];
		++ m_videoQueue.nb_pict;

		++ m_nowBuffPoint;
		if(m_nowBuffPoint >= MAX_BUFF_DEEP) m_nowBuffPoint = 0;	//ѭ������
	//	SDL_UnlockMutex(m_mutex);
		SDL_UnlockMutex(m_videoQueue.mutex);
	}
//	else
//	{//������δ���
//		printf("video decode error!\n");
//	}
}
//�ڶ�����ȡ����Ƶ֡����������
unsigned char* _XMovieFfmpeg::getFormVideoPicList()
{
	_XVideoFrameData *vp = NULL;
	if(m_videoQueue.nb_pict <= 0) return NULL;
//	SDL_LockMutex(m_videoQueue.mutex);
	vp = m_videoQueue.first_pict;
	if(vp != NULL)
	{
		m_videoQueue.first_pict = vp->next;
		if(m_videoQueue.first_pict == NULL)
		{
			m_videoQueue.last_pict = NULL;
			LogStr("buffer no data!");
		}
		--m_videoQueue.nb_pict;
	}
//	SDL_UnlockMutex(m_videoQueue.mutex);
	if(vp != NULL) return vp->data;
	else return NULL;
}

//��ʼ����Ƶ������
void _XMovieFfmpeg::initAudioQueue()
{
	memset(&m_audioQueue,0,sizeof(_XAudioQueue));
	m_audioQueue.mutex = SDL_CreateMutex();
	m_audioQueue.cond = SDL_CreateCond();
}
//����Ƶ��pkt�ŵ�����q��
int _XMovieFfmpeg::putIntoPacketQueue()
{
	if(av_dup_packet(&packet) < 0)
	{
		av_free_packet(&packet); 
		LogStr("audio decode error!");
		return -1;	//��Ƶ����
	}

	AVPacketList * pkt_temp = createMem<AVPacketList>();
//	++ createNumber;
//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	if(pkt_temp == NULL) return -1;	//�ڴ����ʧ��

	pkt_temp->pkt = packet;
	pkt_temp->next = NULL;

	SDL_LockMutex(m_audioQueue.mutex);
	if(m_audioQueue.last_pkt == NULL) m_audioQueue.first_pkt = pkt_temp;	//�����Ƶ����Ϊ��
	else m_audioQueue.last_pkt->next = pkt_temp;		
	m_audioQueue.last_pkt = pkt_temp;
	++ m_audioQueue.nb_packets;
	m_audioQueue.size += pkt_temp->pkt.size;
	SDL_CondSignal(m_audioQueue.cond);
	SDL_UnlockMutex(m_audioQueue.mutex);
	return 0;
}
//�ͷ����л������Ƶ����
void _XMovieFfmpeg::releaseAudioQueue()
{
	AVPacketList *pkt1; 
	while(true)
	{ 
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 == NULL) break;  
		m_audioQueue.first_pkt = pkt1->next; 
		if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//��Ƶ�������Ѿ�û������ 
		-- m_audioQueue.nb_packets; 
		m_audioQueue.size -= pkt1->pkt.size; 
		av_free_packet(&(pkt1->pkt));
		XDELETE(pkt1); 
	//	-- createNumber;
	//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	}
}