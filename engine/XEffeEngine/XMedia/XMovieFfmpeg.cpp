#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMovieFfmpeg.h"
#include "XSoundCommon.h"
#if AUDIO_MATHOD == 1
#include "SDL_audio.h"
#endif
#include "XTimer.h"
namespace XE{
XMovieFfmpeg::XMovieFfmpeg(void)
	:videoStream(-1)
	, audioStream(-1)
	, m_isBuffing(XTrue)
	, m_isLoop(XFalse)
	, m_videoWidth(0)
	, m_videoHeight(0)
	, m_curFrameNumber(0)
	, m_needVoice(XTrue)
	, m_autoTimer(XTrue)
	//,m_texture(NULL)
	, m_isNewFrame(false)
	, m_movieTex(NULL)
	, m_movieSprite(NULL)
	, m_curFrameData(NULL)
	, m_isLoaded(XFalse)
	, m_isEnd(XFalse)
	, m_isQuit(XFalse)
	, m_isStop(XFalse)
	, m_isThreadDecoderExit(XTrue)
	, m_isThreadDrawExit(XTrue)
	, m_pSwsContext(NULL)
	, m_pSwrContext(NULL)
	, m_outColorMode(COLOR_RGB)
	, videofilename("")
	, pFormatCtx(NULL)	//��Ƶ��ʽ��������
	, pCodeCtx(NULL)		//��Ƶ������������
	, aCodeCtx(NULL)		//��Ƶ������������
	, pFrame(NULL)
	, pAudioFrame(NULL)
	, m_isGetFirstFrame(false)
	, m_audioBuf(NULL)
#ifdef DEBUG_MODE
	, m_vpSum(0)
	, m_vfSum(0)
#endif
	, m_playSpeed(1.0f)
{
	m_videoQueue.nb_pict = 0;
	//	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	//	{
	//		m_bmp[i] = NULL;
	//	}
	memset(m_bmp, 0, MAX_BUFF_DEEP * sizeof(XVideoFrameData *));
}
//����߳����ڸ���ʱ�佫��Ƶ���ݸ��µ���ͼ
#ifdef XEE_OS_LINUX
void * XMovieFfmpeg::drawThread(void *arg)
#endif
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XMovieFfmpeg::drawThread(void *arg)
#endif
{
	XMovieFfmpeg *pthis = (XMovieFfmpeg *)arg;
	float refreshtime = 1000.0f/(* pthis).m_curFrameRate;//ӰƬ��ʾһ֡��ʱ�䣬��λ������
	float temptime = refreshtime;
	pthis->m_isThreadDrawExit = XFalse;

	////��һ����ʱ��Ҫ�Ȼ�����
	//while(true)
	//{//�ȴ���Ƶ�������
	//	if(!pthis->m_isBuffing) break;
	//	if(pthis->m_isQuit) break;	//�������˳�ʱ���������Ҫ�˳������������ѭ������ɳ���ֹͣ��Ӧ��
	//	mySleep(1);
	//}	

	int startTime = XTime::getCurrentTicks();		//��¼��ʼ���ŵ�ʱ��
	pthis->m_curPlayedTime = 0.0f;			//���ŵ�ʱ��ͨ�����������

	while(true)
	{	
		if(pthis->m_isQuit) break;
		if (pthis->m_isBuffing && pthis->m_isDecodeOver)
			pthis->m_isBuffing = false;

		if(pthis->m_isStop || pthis->m_isEnd || pthis->m_isBuffing)
		{//ֹͣ״̬,���߲��������ȴ�
			XEE::sleep(1);
			startTime = XTime::getCurrentTicks();
			continue;
		}
		if(pthis->m_videoQueue.nb_pict > 0 || pthis->videoStream == -1)
		{//��Ƶ������������
			pthis->m_gotoMutex1.Lock();
			if(pthis->m_autoTimer) pthis->m_curPlayedTime += (XTime::getCurrentTicks() - startTime) * pthis->m_playSpeed;
			startTime = XTime::getCurrentTicks();
		//	playedTime = getCurrentTicks() - startTime;
			if(pthis->m_curPlayedTime >= pthis->m_curFrameNumber * temptime)
			{//����ﵽʱ�����ʼ����
				pthis->getPixelData();
				if(pthis->m_curFrameNumber >= pthis->m_allFrameSum)
				{//���ʱ��Խ�磬�򲥷����
					pthis->m_isEnd = XTrue;
					//pthis->m_audioQueue.cFlag = true;
					//if(pthis->m_audioQueue.cond != NULL) 
					//	SDL_CondSignal(pthis->m_audioQueue.cond);
#ifdef WITH_DEBUG_INFO
					LogStr("Time play over+!");
#endif
					if(pthis->m_isLoop) pthis->replay();
					startTime = XTime::getCurrentTicks();
				}
			}else if(pthis->m_autoTimer)
			{//������ȵ�ʱ����ٲ���
				int tempTime = pthis->m_curFrameNumber * temptime - pthis->m_curPlayedTime;
#ifdef WITH_DEBUG_INFO
				if(tempTime < 0 || tempTime > 2000)
				{
					LogStr("haha");
				}
#endif
				XEE::sleep(tempTime);
			} 
			pthis->m_gotoMutex1.Unlock();
		}else
		{
			if(pthis->m_isDecodeOver)	//��Ƶ�������
			{//�Ѿ��������
				pthis->m_isEnd = XTrue;
				//pthis->m_audioQueue.cFlag = true;
#ifdef WITH_DEBUG_INFO
				LogStr("Time play over-!");
#endif
				if(pthis->m_isLoop) pthis->replay();
				startTime = XTime::getCurrentTicks();
			}else
			{//�ȴ���������
#ifdef WITH_DEBUG_INFO
				LogStr("-->buff over!");
#endif
				pthis->m_isBuffing = XTrue;	//���ÿ�ʼ��������
			//	mySleep(5000);
			//	pthis->m_isStop = XFalse;
				startTime = XTime::getCurrentTicks();
			}
		}
		XEE::sleep(1);
	}
	pthis->m_isThreadDrawExit = XTrue;
//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//��Ƶ�����̣߳���DrawThread�̲߳���ʾ��Ƶ��ʱ�����Ƶ֡���뵽���У�����Ƶ���ŵ�����
#ifdef XEE_OS_WINDOWS
DWORD WINAPI XMovieFfmpeg::decoderThread(void *arg)
#endif
#ifdef XEE_OS_LINUX
void * XMovieFfmpeg::decoderThread(void *arg)
#endif
{
	XMovieFfmpeg *pthis = (XMovieFfmpeg *)arg;
	//AVPicture pict;
	int bufernum = (int)(BUFF_TIMER * pthis->m_curFrameRate); //���û�������СΪӰƬ2�벥�ŵ�֡��
	//int frameFinished;		//���붯���Ƿ����
	if(pthis->videoStream == -1) bufernum = 1;
	if(bufernum >= MAX_BUFF_DEEP) bufernum = MAX_BUFF_DEEP;
	//int picIndex = 0;
	unsigned long audioindex = 0;
	pthis->m_isThreadDecoderExit = XFalse;

	while(!pthis->m_isQuit)//���ǿ���߳��˳����������˳�
	{
		if(pthis->m_isDecodeOver)
		{//����Ѿ����һ�ν�����ȴ�
			if(pthis->videoStream == -1 && pthis->m_audioQueue.nb_packets <= 0 && 
				!pthis->m_isEnd)
			{//���û��ͼ��Ļ�ֱ���������ò������
				pthis->m_isEnd = XTrue;
				//pthis->m_audioQueue.cFlag = true;
#ifdef WITH_DEBUG_INFO
				LogStr("Time play over-!");
#endif
				if(pthis->m_isLoop) pthis->replay();
			}
			XEE::sleep(1);
			continue;
		}
		//ͼƬ������ɣ�������û����Ƶ��Ϣ
		if((pthis->m_videoQueue.nb_pict < bufernum && pthis->videoStream >= 0) || 
			(pthis->videoStream == -1 && pthis->m_audioQueue.nb_packets < bufernum))	//���֡�Ļ�������Ϊ2��
		{//���û�л���������л���
			av_init_packet(&pthis->packet);
			pthis->m_gotoMutex.Lock();
			int ret = av_read_frame(pthis->pFormatCtx, &pthis->packet);
			if(ret == 0)
			{//�����ȡ֡�ɹ�
				if(pthis->packet.stream_index == pthis->videoStream)// && pthis->packet.duration > 0)
				{//��ȡ������Ƶ֡	
					pthis->putIntoVideoPicList();
#ifdef DEBUG_MODE
					++pthis->m_vpSum;
#endif
				}else if(pthis->packet.stream_index == pthis->audioStream)// && pthis->packet.duration > 0)
				{//�����ȡ������Ƶ֡
					audioindex += pthis->packet.duration;
					if(pthis->m_needVoice) pthis->putIntoPacketQueue();
					else av_free_packet(&pthis->packet);	//�������Ҫ������Ƶ��ֱ�ӷ�������Ƶ����(������������)
				}else
				{//��֪����ȡ����ɶ����
					av_free_packet(&pthis->packet);	//(������������)
				}
			}else if(ret < 0)	//�����ȡ֡ʧ�ܣ���Ĭ���Ѿ�������else
			{//��Ƶ�������
				pthis->m_isDecodeOver = XTrue;
#ifdef WITH_DEBUG_INFO
				LogStr("DecodeOver");
#endif
			}
			pthis->m_gotoMutex.Unlock();
		}else if(pthis->m_isBuffing)	//��һ�β��ŵ�ʱ��һ��Ҫ�Ȼ�����
		{//������ϣ�����bplay , ���в���
			pthis->m_isBuffing = XFalse;
		}
		XEE::sleep(1);
	}
	pthis->m_isThreadDecoderExit = XTrue;
#ifdef WITH_DEBUG_INFO
	LogStr("DecodeThread over");
#endif
	//	_endthread();
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
//int createNumber = 0;
//����Ƶ������ȡ����Ƶ��
int XMovieFfmpeg::getFromPacketQueue(AVPacket *pkt)
{ 
	AVPacketList *pkt1; 
	int ret; 

	while(true)
	{ 
		if(m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������

//#ifdef WITH_DEBUG_INFO
//		LogStr("57");
//#endif
		gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//		LogStr("58");
//#endif
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 != NULL) 
		{//������
			m_audioQueue.first_pkt = pkt1->next; 
			if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//�������ݶ��Ѿ�ȡ��
			--m_audioQueue.nb_packets; 
			m_audioQueue.size -= pkt1->pkt.size; 
			*pkt = pkt1->pkt; 
			XMem::XDELETE(pkt1);	//ɾ����ʱ����
		//	-- createNumber;
		//	printf("%d,%d\n",createNumber,pClass->m_audioQueue.nb_packets);
//#ifdef WITH_DEBUG_INFO
//			LogStr("59");
//#endif
			gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//			LogStr("60");
//#endif
			ret = 1; 
			break; 
		}else	//�����Ĵ����п��ܻ��������Խ��Խ��
		{//�ȴ�ͬ��(ԭ��������)
//#ifdef WITH_DEBUG_INFO
//			LogStr("61");
//#endif
			gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//			LogStr("61");
//#endif
			//��������Ϊ���ȴ�ͬ��
			if(m_isQuit || m_isEnd) return -1;	//���ȴ�ͬ��
		} 
		XEE::sleep(1);
	} 
	return ret;
} 
//����Ƶ���������Ƶ����
int XMovieFfmpeg::audioDecodeFrame(uint8_t *audio_buf,int buf_size)
{
	AVPacket pkt,pkt1;
	uint8_t *out[] = {audio_buf}; 
	int pkt_pos,pkt_len;
	int len = 0;
	int data_size = 0; 
	int isFinished;
	if(getFromPacketQueue(&pkt) < 0) return -1;//�����Ƶ��û�����ݣ��򷵻�  
	pkt1 = pkt;  
    pkt_pos = 0;  
    pkt_len = pkt.size;
	int frameDataSum = XEG.getAudioChannelSum() * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

	if(pkt.size > 0)
	{
		while(pkt1.size > 0)
		{
			len = avcodec_decode_audio4(aCodeCtx,pAudioFrame,&isFinished,&pkt1);
			if(len < 0)
			{//��ѹʧ��
				av_free_packet(&pkt);
#ifdef WITH_DEBUG_INFO
				LogStr("��Ƶ��û������!");
#endif
				return -1;
			}
			if(isFinished)
			{//��ʽת��
			//	SwrContext *swr = swr_alloc();
			//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
			//		c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
			//	if(swr_init(pClass->m_pSwrContext)<0)
			//	{
			//		LogStr("swr_init() fail");
			//		return -1;
			//	}
				int outSize = buf_size / frameDataSum;
				len = swr_convert(m_pSwrContext,out,outSize,(const uint8_t **)pAudioFrame->extended_data,pAudioFrame->nb_samples);  
				len = len * frameDataSum;
 
	            av_free_packet(&pkt);
			//	swr_free(&swr);
	            return len;		//�߼���������ܴ������ݰ���ʧ,�����ʱ���ݰ������ݲ�û����ȫ����ɹ��Ļ�����ᱻ����
	        }else
			{  
	            if(!pkt1.data && aCodeCtx->codec->capabilities & CODEC_CAP_DELAY)
				{  
	                break;  
	            }  
	        }  
			pkt_pos += len;//�Ѿ�����ĳ���  
			pkt1.data = pkt.data + pkt_pos;  
			pkt1.size = pkt.size - pkt_pos; 

			if(m_isQuit ||
				m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������
		}
	}
	av_free_packet(&pkt);
	return data_size;
}
//int audioDecodeFrame(XMovieFfmpeg *pClass,uint8_t *audio_buf,int buf_size)
//{ 
//	static AVPacket pkt;
//	static uint8_t *audio_pkt_data = NULL; 
//	static int audio_pkt_size = 0; 
//	int len = 0;
//	int data_size = 0; 
//	AVCodecContext * c= pClass->aCodeCtx;
//	while(true)
//	{ 
//		if(pClass->m_isQuit) return -1;
//		//while(audio_pkt_size > 0) 
//		if(audio_pkt_size > 0)
//		{ 
//			if(pClass->m_isQuit) return -1;
//			data_size = buf_size; 
//
//			len = avcodec_decode_audio3(c,(int16_t *)audio_buf,&data_size,&pkt);//audio_pkt_data,audio_pkt_size); //��������
//			if(len < 0) 
//			{//���뷢������ 
//				av_free_packet(&pkt);
//				audio_pkt_size = 0; 
//				return -1;
//			} 
//			audio_pkt_data += len;		//���һ����Ƶ����û��������ظ�����ֱ�����Ϊֹ
//			audio_pkt_size -= len; 
//			if(pkt.data != NULL) av_free_packet(&pkt);//�ͷ���Դ
//			audio_pkt_size = 0;	//���������������
//			if(data_size <= 0) return -1; 
//
//			//�������ת��
//			if(c->sample_fmt != AV_SAMPLE_FMT_S16)
//			{
//				SwrContext *swr = swr_alloc();
//				swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
//					c->channel_layout,c->sample_fmt,c->sample_rate,0,NULL);
//
//				if(swr_init(swr)<0)
//				{
//					printf("swr_init() for AV_SAMPLE_FMT_S16P fail");
//					return -1;
//				}
//				uint8_t *out[] = {audio_buf};
//				uint8_t tempTest[50000];	//ע�������ֵ
//				int ret = 0;
//				memcpy(tempTest,audio_buf,data_size);
//				if(c->sample_fmt == AV_SAMPLE_FMT_S16P)
//				{
//					const uint8_t *in[] = {tempTest,tempTest + (data_size >> 1)};
//					ret = swr_convert(swr,out,data_size,in,(data_size >> 2));
//				}else
//				if(c->sample_fmt == AV_SAMPLE_FMT_FLTP)
//				{//�����������,���ص�����
//					const uint8_t *in[] = {tempTest};
//					ret = swr_convert(swr,out,data_size,in,data_size);
//				}
//				swr_free(&swr);
//			}
//			return data_size;
//		} 
//		if(pkt.data != NULL) av_free_packet(&pkt);	//�ͷŵ��ڴ�ռ� 
// 
//		if(pClass->m_isStop) return -1;//��ͣ�Ļ�����ֹͣ��������
//		if(getFromPacketQueue(pClass,&pkt, 1) < 0) return -1;//�����Ƶ��û�����ݣ��򷵻�  
//
//		audio_pkt_data = pkt.data; 
//		audio_pkt_size = pkt.size; 
//	} 
//}
//�����ص�������������Ƶ����,����Ƶ�����Ƶ����湩SDL����
void XMovieFfmpeg::audioCallBack(void *userdata,Uint8 *stream,int len)
{
	XMovieFfmpeg &pPar = *(XMovieFfmpeg *)userdata;

	int len1 = 0;
	int audio_size = 0; 
	while(len > 0)
	{//����������Ҫ����ʱ 	
		if(pPar.m_audioBufIndex >= pPar.m_audioBufSize)
		{//����ѹ����Ƶȫ���������ʱ�Ž�ѹ�������Ƶ
			if(pPar.m_isQuit || pPar.m_isBuffing || 
				pPar.m_isStop || pPar.m_isEnd)
			{//����Ҫ����������ʱ�򲥷ſհ�����(�����������հ����ݵĻ����������ʾ����������Ҫ��ϸ����ԭ��)
			//	if(len > 1024) pPar.m_audioBufSize = 1024;	//���û����������������,��Ҫ����ͣ��ʱ��
			//	else pPar.m_audioBufSize = len;
			//	memset(pPar.m_audioBuf, 0, pPar.m_audioBufSize); 
				memset(stream,0,len);
				break;
			}else
			{
				audio_size = pPar.audioDecodeFrame(pPar.m_audioBuf,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);//�������뺯�� 
				if(audio_size < 0) 
				{//�����Ƶ����û������.������ڴ�ռ�
				//	if(len > 1024) pPar.m_audioBufSize = 1024;	//���û����������������,��Ҫ����ͣ��ʱ��
				//	else pPar.m_audioBufSize = len;
				//	memset(pPar.m_audioBuf, 0, pPar.m_audioBufSize); 
					memset(stream,0,len);
					return;
				}else
				{//��¼��ѹ��������Ƶ������
					pPar.m_audioBufSize = audio_size; 
				} 
			}
			pPar.m_audioBufIndex = 0; 
		} 
		len1 = pPar.m_audioBufSize - pPar.m_audioBufIndex;	//��¼��Ƶ�ĳ���
		if(len1 > len) len1 = len;
		//�������д�����Ŀǰ�Ŀ���¹���һ��
		memcpy(stream,(uint8_t *)pPar.m_audioBuf + pPar.m_audioBufIndex,len1);
		
		len -= len1; 
		stream += len1; 
		pPar.m_audioBufIndex += len1; 
	}
}
//void audioCallBack(void *userdata,Uint8 *stream,int len)
//{
//	XMovieFfmpeg * pClass = (XMovieFfmpeg *)userdata;
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
//			audio_size = audioDecodeFrame(pClass,pClass->m_audioBuf,(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);//�������뺯�� 
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
//	/*	if(XEE::audioSampleRate != pClass->aCodeCtx->sample_rate && len1 > 0)
//		{
//			SDL_AudioCVT  wav_cvt;
//			int ret = SDL_BuildAudioCVT(&wav_cvt,
//				XEE::audioFormat,XEE::audioChannel,pClass->aCodeCtx->sample_rate,
//				XEE::audioFormat,XEE::audioChannel,XEE::audioSampleRate);
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
//		//if(XEE::audioSampleRate != c->sample_rate 
//		//	|| c->sample_fmt != AV_SAMPLE_FMT_S16
//		//	|| c->channels != XEE::audioChannel)
//		//{
//		//	int dataPackSize = av_samples_get_buffer_size(NULL,c->channels,c->frame_size,c->sample_fmt, 1);
//		//	SwrContext *swr = swr_alloc();
//		//	swr_alloc_set_opts(swr,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioSampleRate,
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
    while (*p)
	{ 
        int nb_channels = av_get_channel_layout_nb_channels(*p); 
 
        if (nb_channels > bestNbChannells) 
		{ 
            bestChLayout    = *p; 
            bestNbChannells = nb_channels; 
        } 
       ++ p; 
    } 
    return bestNbChannells; 
} 	
//��ӰƬ ����ʼ��SDL , ffmpeg���
XBool XMovieFfmpeg::load(const char *filename,XColorMode mode,bool withVoice)
{
	if(m_isLoaded || filename == NULL) return XFalse;
	m_isTexInit = false;
	m_outColorMode = mode;
	videoStream = -1;
	audioStream = -1;
	m_isBuffing = XTrue;
	m_isStop = XTrue;
//	m_isLoop = FALSE;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_curFrameNumber = 0;
	m_videoQueue.nb_pict = 0;
	m_isEnd = XFalse;
	m_isQuit = XFalse;
	m_needVoice = withVoice;
	m_isGetFirstFrame = false;
	XMem::XDELETE_ARRAY(m_audioBuf);
	m_audioBuf = XMem::createArrayMem<uint8_t>((AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2);

	int i = 0;
	videofilename = filename;		//��¼��Ƶ�ļ���
	av_register_all();					//��ʼ��ffmpeg��
//	avcodec_register_all();
//	avformat_network_init();
	pFormatCtx = NULL;
	if(avformat_open_input(&pFormatCtx,videofilename.c_str(),NULL,NULL) != 0)	//����Ƶ�ļ�
	{
		LogStr("can not open input file!");
		return XFalse;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL) < 0)		//�����Ƶ����Ϣ
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
	if(videoStream == -1 && audioStream == -1) return XFalse;	//�ļ���û�б�Ҫ������Ϣ

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
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),AV_SAMPLE_FMT_S16,XEG.getAudioSampleRate(),
			aCodeCtx->channel_layout,aCodeCtx->sample_fmt,aCodeCtx->sample_rate,0,NULL);
		if(swr_init(m_pSwrContext) < 0)
		{
			LogStr("swr_init() fail");
			return XFalse;
		}
		m_audioBufSize = 0; 
		m_audioBufIndex = 0; 
	}

	pFrame = av_frame_alloc();//����һ֡���ڴ�ռ䣬���ڴ�Ž�������Ƶ��Ϣ
	if(pFrame == NULL)
	{//��������ڴ�ʧ�ܣ���������ʾ
		LogStr("malloc Frame failed!");
		return XFalse;
	}
	pAudioFrame = av_frame_alloc();//����һ֡���ڴ�ռ䣬���ڴ�Ž�������Ƶ��Ϣ
	if(pAudioFrame == NULL)
	{//��������ڴ�ʧ�ܣ���������ʾ
		LogStr("malloc Frame failed!");
		return XFalse;
	}
	AVStream* curStr = nullptr;
	if(videoStream != -1) curStr = pFormatCtx->streams[videoStream];
	else curStr = pFormatCtx->streams[audioStream];

	if(curStr->avg_frame_rate.den <= 0)
		m_curFrameRate = (double)(curStr->time_base.den)/(double)(curStr->time_base.num);
	else
		m_curFrameRate = (double)(curStr->avg_frame_rate.num)/(double)(curStr->avg_frame_rate.den);

	if (curStr->nb_frames != 0) m_allFrameSum = curStr->nb_frames;
	else
	{
		if (curStr->duration < 0)
		{
			m_allFrameSum = pFormatCtx->duration * 0.000001 * m_curFrameRate;
		}
		else
		{
			m_allFrameSum = curStr->duration * (double)(curStr->time_base.num) /
				(double)(curStr->time_base.den) * m_curFrameRate;
		}
		//m_allFrameSum = curStr->duration;
	}

	//����OpenGL��ͼ
	if (videoStream != -1)
	{
		XMem::XDELETE(m_movieTex);
		XMem::XDELETE(m_movieSprite);
		m_movieTex = XMem::createMem<XTexture>();
		if (m_movieTex == NULL) return XFalse;
		m_movieSprite = XMem::createMem<XSprite>();
		if (m_movieSprite == NULL) return XFalse;
		m_movieSprite->init(m_videoWidth, m_videoHeight, 0);
		XMem::XDELETE_ARRAY(m_curFrameData);
		switch (m_outColorMode)
		{
		case COLOR_RGB:
		case COLOR_BGR:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * m_videoHeight * 3);
			memset(m_curFrameData, 0, m_videoWidth * m_videoHeight * 3);
			break;
		case COLOR_RGBA:
		case COLOR_BGRA:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * (m_videoHeight << 2));
			memset(m_curFrameData, 0, m_videoWidth * (m_videoHeight << 2));
			break;
		case COLOR_GRAY:
			m_movieTex->createTexture(m_videoWidth, m_videoHeight, m_outColorMode);
			m_curFrameData = XMem::createArrayMem<unsigned char>(m_videoWidth * m_videoHeight);
			memset(m_curFrameData, 0, m_videoWidth * m_videoHeight);
			break;
		default:
			return XFalse;	//������ʽĿǰ��֧��
			break;
		}
		if (m_curFrameData == NULL) return XFalse;

		//Ϊ��������ڴ�ռ�,��ͼ������ת����RGB24����ʽ
		int dataSize;
		AVPixelFormat avColorMode;
		switch (m_outColorMode)
		{
		case COLOR_RGB:
			dataSize = pCodeCtx->width * pCodeCtx->height * 3;
			avColorMode = AV_PIX_FMT_RGB24;
			break;
		case COLOR_BGR:
			dataSize = pCodeCtx->width * pCodeCtx->height * 3;
			avColorMode = AV_PIX_FMT_BGR24;
			break;
		case COLOR_RGBA:
			dataSize = pCodeCtx->width * (pCodeCtx->height << 2);
			avColorMode = AV_PIX_FMT_RGBA;
			break;
		case COLOR_BGRA:
			dataSize = pCodeCtx->width * (pCodeCtx->height << 2);
			avColorMode = AV_PIX_FMT_BGRA;
			break;
		case COLOR_GRAY:
			dataSize = pCodeCtx->width * pCodeCtx->height;
			avColorMode = AV_PIX_FMT_GRAY8;
			break;
		default:
			return XFalse;
			break;
		}
		m_pSwsContext = sws_getContext(pCodeCtx->width, pCodeCtx->height, pCodeCtx->pix_fmt,
			pCodeCtx->width, pCodeCtx->height, avColorMode, SWS_POINT, NULL, NULL, NULL);	//SWS_BICUBIC
		if (m_pSwsContext == NULL) return XFalse;
		//int numBytes = avpicture_get_size(PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);	//��ȡ�ڴ�ߴ�
		m_curBuffPoint = 0;
		for (i = 0; i < MAX_BUFF_DEEP; ++i)
		{
			m_bmp[i] = XMem::createMem<XVideoFrameData>();
			if (m_bmp[i] == NULL) return XFalse;

			m_bmp[i]->data = XMem::createArrayMem<unsigned char>(dataSize);
			if (m_bmp[i]->data == NULL) return XFalse;
			avpicture_fill(&m_framePic[i], m_bmp[i]->data, avColorMode,
				pCodeCtx->width, pCodeCtx->height);
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
	//	XCurSndCore.pauseMusic();
		XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	XCurSndCore.resumeMusic();
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
	//	XCurSndCore.pauseMusic();
		XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	//	XCurSndCore.resumeMusic();
	}
    if(CreateThread(0,0,drawThread,this,0,NULL) == 0) 
	{
		LogStr("open thread error!");
	}
#endif
#ifdef WITH_DEBUG_INFO
	LogNull("AllFrames:%u,FPS:%f", m_allFrameSum, getVideoFrameRate());
#endif
	//m_mutex = SDL_CreateMutex();

	m_isLoaded = XTrue;

	return XTrue;
}
////��ʼ����ӰƬ���������Ӧ�����ڲ�ͣ�Ĳ���ѹ��������ƵƬ�ϣ�
//void XMovieFfmpeg::waitingForBuffOver(void)
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
void XMovieFfmpeg::closeClip(void)
{
	if (!m_isLoaded) return;
	m_isQuit = XTrue;
	//m_audioQueue.cFlag = true;

	//mySleep(20);	//�ȴ��߳��˳�
	while (!m_isThreadDecoderExit || !m_isThreadDrawExit)
	{
		Sleep(1);
	}
	XCurSndCore.setCallBack(NULL, NULL);	//�������֮��ȡ������
	if (pFrame != NULL) av_frame_free(&pFrame);
	if (pAudioFrame != NULL) av_frame_free(&pAudioFrame);
	if (pCodeCtx != NULL) avcodec_close(pCodeCtx);
	if (audioStream != -1 && m_needVoice) avcodec_close(aCodeCtx);
	avformat_close_input(&pFormatCtx);
	releaseVideoPictList();
	releaseAudioQueue();

	XMem::XDELETE(m_movieTex);
	XMem::XDELETE(m_movieSprite);
	XMem::XDELETE_ARRAY(m_curFrameData);

	XMem::XDELETE_ARRAY(m_audioBuf);
	//if(glIsTexture(m_texture))
	//{
	//	printf("delete texture:%d\n",m_texture);
	//	glDeleteTextures(1,&m_texture);
	//}
	//SDL_DestroyMutex(m_mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("69");
//#endif
	XMem::XDELETE(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("70");
//#endif
	XMem::XDELETE(m_videoQueue.mutex);
	sws_freeContext(m_pSwsContext);
	if (audioStream != -1 && m_needVoice) swr_free(&m_pSwrContext);
	m_isLoaded = XFalse;
}
void XMovieFfmpeg::gotoFrameIndex(int index, bool isForce)
{
	if((!isForce && index == m_curFrameNumber) || index < 0 || index > m_allFrameSum) return;
	m_gotoMutex.Lock();
	m_gotoMutex1.Lock();

	if (av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_BACKWARD) < 0)	//����������ܻ���ɲ��ܶ�ȡ����
		av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//if(index == m_curFrameNumber)
	//	av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//else
	//{
	//	if(av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_BACKWARD) < 0)
	//		av_seek_frame(pFormatCtx, -1, index, AVSEEK_FLAG_ANY);
	//}

	if (videoStream >= 0)
	{
		m_curFrameNumber = index - m_videoQueue.nb_pict;	//�û����е����ݿ��ٲ������
		m_curPlayedTime = m_curFrameNumber * 1000.0f / m_curFrameRate;	//��������ʱ��
	}
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
//��YUV��ʽ����Ƶ֡��ʾ������
void XMovieFfmpeg::getPixelData()
{
	gLock(m_videoQueue.mutex);
	unsigned char * p = getFormVideoPicList();
	if (p == NULL)
	{
		gUnlock(m_videoQueue.mutex);
		return;
	}
	m_mutex.Lock();
	//�����ݿ�������
	switch (m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(m_curFrameData, p, m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(m_curFrameData, p, m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(m_curFrameData, p, m_videoWidth * m_videoHeight);
		break;
	}
	m_mutex.Unlock();
	gUnlock(m_videoQueue.mutex);
	m_isNewFrame = true;
	++m_curFrameNumber;
}
XBool XMovieFfmpeg::getFirstPixelData()
{
	gLock(m_videoQueue.mutex);
	unsigned char * p = getAVideoFrame();	//����Ƶ������ȡ��һ֡�����ı���е�����
	if(p == NULL) 
	{
		gUnlock(m_videoQueue.mutex);
		return false;
	}
	m_mutex.Lock();
	//�����ݿ�������
	switch(m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(m_curFrameData,p,m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(m_curFrameData,p,m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(m_curFrameData,p,m_videoWidth * m_videoHeight);
		break;
	}
	m_mutex.Unlock();
	gUnlock(m_videoQueue.mutex);
	m_isNewFrame = true;
	return true;
}
void XMovieFfmpeg::releaseVideoPictList()
{
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
		if(m_bmp[i] != NULL)
			XMem::XDELETE_ARRAY(m_bmp[i]->data);
		XMem::XDELETE(m_bmp[i]);
	}
} 
void XMovieFfmpeg::putIntoVideoPicList()
{
	int isFinished = 0;
	while (packet.size > 0)
	{
		int ret = avcodec_decode_video2(pCodeCtx, pFrame, &isFinished, &packet);
		if (ret < 0)
		{//��Ƶ����ʧ��
			av_free_packet(&packet);
			return;
		}
		packet.data += ret;
		packet.size -= ret;
		if (isFinished == 0) continue;
		//�������ɹ�		
	//	SDL_LockMutex(m_mutex);
		gLock(m_videoQueue.mutex);
		//����Ƶ���ݸ�ʽת����RGB24��ʽ
		imgConvert(m_bmp[m_curBuffPoint]->pict, (AVPicture *)pFrame, pCodeCtx->height);
		//����Ƶ�������������
		m_bmp[m_curBuffPoint]->next = NULL;
		if (m_videoQueue.last_pict == NULL) m_videoQueue.first_pict = m_bmp[m_curBuffPoint];//����ǿ���
		else m_videoQueue.last_pict->next = m_bmp[m_curBuffPoint];//�������һ֡

		m_videoQueue.last_pict = m_bmp[m_curBuffPoint];
		++m_videoQueue.nb_pict;

		++m_curBuffPoint;
		if (m_curBuffPoint >= MAX_BUFF_DEEP) m_curBuffPoint = 0;	//ѭ������
	//	SDL_UnlockMutex(m_mutex);
		gUnlock(m_videoQueue.mutex);
#ifdef DEBUG_MODE
		++m_vfSum;
#endif
	}
	av_free_packet(&packet);
}
//�ڶ�����ȡ����Ƶ֡����������
unsigned char* XMovieFfmpeg::getFormVideoPicList()
{
	XVideoFrameData *vp = NULL;
	if(m_videoQueue.nb_pict <= 0) return NULL;
	vp = m_videoQueue.first_pict;
	if(vp != NULL)
	{
		m_videoQueue.first_pict = vp->next;
		if(m_videoQueue.first_pict == NULL)
		{
			m_videoQueue.last_pict = NULL;
#ifdef WITH_DEBUG_INFO
			LogStr("buffer no data!");
#endif
		}
		--m_videoQueue.nb_pict;
	}
	if(vp != NULL) return vp->data;
	else return NULL;
}
//����Ƶ��pkt�ŵ�����q��
int XMovieFfmpeg::putIntoPacketQueue()
{
	if(av_dup_packet(&packet) < 0)
	{
		av_free_packet(&packet); 
		LogStr("audio decode error!");
		return -1;	//��Ƶ����
	}

	AVPacketList* pkt_temp = XMem::createMem<AVPacketList>();
//	++ createNumber;
//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	if(pkt_temp == NULL) return -1;	//�ڴ����ʧ��

	pkt_temp->pkt = packet;
	pkt_temp->next = NULL;

//#ifdef WITH_DEBUG_INFO
//	LogStr("51");
//#endif
	gLock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("52");
//#endif
	if(m_audioQueue.last_pkt == NULL) m_audioQueue.first_pkt = pkt_temp;	//�����Ƶ����Ϊ��
	else m_audioQueue.last_pkt->next = pkt_temp;		
	m_audioQueue.last_pkt = pkt_temp;
	++ m_audioQueue.nb_packets;
	m_audioQueue.size += pkt_temp->pkt.size;
	//m_audioQueue.cFlag = true;
//#ifdef WITH_DEBUG_INFO
//	LogStr("53");
//#endif
	gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("54");
//#endif
	return 0;
}
//�ͷ����л������Ƶ����
void XMovieFfmpeg::releaseAudioQueue()
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
		XMem::XDELETE(pkt1); 
	//	-- createNumber;
	//	printf("%d,%d\n",createNumber,m_audioQueue.nb_packets);
	}
}
void XMovieFfmpeg::replay()
{
	m_gotoMutex.Lock();	//��ֹ�r�g͵��
	m_gotoMutex1.Lock();	//��ֹ�r�g͵��
	//������Ҫ�������
//	printf("All times:%d\n",m_curPlayedTime);
//	printf("Loop!\n");
	gotoFrame(0.0f, true);
	m_isDecodeOver = XFalse;
	m_isEnd = XFalse;
	//������Ҫ��ջ���
	clearAudioQueue();
	clearVideoQueue();
	m_curFrameNumber = 0;
	m_curPlayedTime = 0.0f;
	m_audioBufSize = 0;
	m_audioBufIndex = 0;
#ifdef DEBUG_MODE
	m_vpSum = 0;
	m_vfSum = 0;
	LogStr("replay()");
#endif
	m_gotoMutex.Unlock();
	m_gotoMutex1.Unlock();
}
void XMovieFfmpeg::clearAudioQueue()	//�����Ƶ����
{
//#ifdef WITH_DEBUG_INFO
//	LogStr("71");
//#endif
	gLock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("72");
//#endif
	AVPacketList *pkt1 = NULL;
	while(true)
	{ 
		pkt1 = m_audioQueue.first_pkt; 
		if(pkt1 == NULL) break;  
		m_audioQueue.first_pkt = pkt1->next; 
		if(m_audioQueue.first_pkt == NULL) m_audioQueue.last_pkt = NULL;	//��Ƶ�������Ѿ�û������ 
		-- m_audioQueue.nb_packets; 
		m_audioQueue.size -= pkt1->pkt.size; 
		av_free_packet(&(pkt1->pkt));
		XMem::XDELETE(pkt1); 
	}
//#ifdef WITH_DEBUG_INFO
//	LogStr("73");
//#endif
	gUnlock(m_audioQueue.mutex);
//#ifdef WITH_DEBUG_INFO
//	LogStr("74");
//#endif
}
void XMovieFfmpeg::getData(unsigned char * p)
{
	m_mutex.Lock();
	switch (m_outColorMode)
	{
	case COLOR_RGB:
	case COLOR_BGR:
		memcpy(p, m_curFrameData, m_videoWidth * m_videoHeight * 3);
		break;
	case COLOR_RGBA:
	case COLOR_BGRA:
		memcpy(p, m_curFrameData, m_videoWidth * (m_videoHeight << 2));
		break;
	case COLOR_GRAY:
		memcpy(p, m_curFrameData, m_videoWidth * m_videoHeight);
		break;
	default:	//������ʽ��ʱ��֧��
		break;
	}
	m_mutex.Unlock();
}
XBool XMovieFfmpeg::updateFrame()
{
	if(!m_isGetFirstFrame && getFirstPixelData())
		m_isGetFirstFrame = true;
	if(!m_isNewFrame) return XFalse;
	m_isNewFrame = XFalse;
	if(m_movieTex != NULL) 
	{
		m_mutex.Lock();
		m_movieTex->updateTexture(m_curFrameData);
		m_mutex.Unlock();
		if(!m_isTexInit) m_isTexInit = true;
	}
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMovieFfmpeg.inl"
#endif
}