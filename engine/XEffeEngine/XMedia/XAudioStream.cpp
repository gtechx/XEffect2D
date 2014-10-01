//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XAudioStream.h"
#include "../XLogBook.h"

_XBool _XAudioStream::load(const char * filename)
{
	if(m_isLoaded ||
		filename == NULL) return XFalse;
	av_register_all();	
	m_pFormatCtx = NULL;
	if(avformat_open_input(&m_pFormatCtx,filename,NULL,NULL) != 0)
	{
		LogStr("File open error!");
		return XFalse;
	}
	if(av_find_stream_info(m_pFormatCtx) < 0)		//�����Ƶ����Ϣ
	{
		LogStr("can not find stream information!");
		return XFalse;
	}
	m_pAudioCodecCtx = m_pFormatCtx->streams[0]->codec;
	AVCodec *aCodec = avcodec_find_decoder(m_pAudioCodecCtx->codec_id);
	if(aCodec == NULL)
	{//�Ҳ�����Ƶ������
		LogStr("can not find audio decoder information!");
		return XFalse;
	} 
	if(avcodec_open2(m_pAudioCodecCtx,aCodec,NULL) < 0)
	{//�Ҳ�����Ƶ�����
		LogStr("can not open audio decoder!");
		return XFalse;
	}
	//m_pFrame = avcodec_alloc_frame();
	//if(m_pFrame == NULL)
	//{
	//	printf("malloc Frame failed!\n");
	//	return XFalse;
	//}
	m_allFrameSum = m_pFormatCtx->streams[0]->duration;
	av_init_packet(&m_dataPacket);

	m_pSwrContext = swr_alloc();
	if(m_pSwrContext == NULL) return XFalse;
	if(m_pAudioCodecCtx->channel_layout == 0)
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
			av_get_default_channel_layout(m_pAudioCodecCtx->channels),m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}else
	{
		swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEE::audioChannel),AV_SAMPLE_FMT_S16,XEE::audioFrequency,
			m_pAudioCodecCtx->channel_layout,m_pAudioCodecCtx->sample_fmt,m_pAudioCodecCtx->sample_rate,0,NULL);
	}

	m_isLoaded = XTrue;
	return XTrue;
}
_XBool _XAudioStream::getAFrame()	//��������ȡһ֡����
{
	if(!m_isLoaded) return XFalse;
	av_free_packet(&m_dataPacket);	//�ͷ���һ�λ�õ�����
	av_init_packet(&m_dataPacket);
	if(av_read_frame(m_pFormatCtx,&m_dataPacket) == 0)
	{//�������Ƶ���ݽ���
		m_pFrame = avcodec_alloc_frame();

		int isFinished;
		int len = avcodec_decode_audio4(m_pAudioCodecCtx,m_pFrame,&isFinished,&m_dataPacket);
		if(len < 0) return XFalse;
		if(isFinished)
		{
			if(swr_init(m_pSwrContext) < 0)
			{
				LogStr("swr_init() fail");
				return XFalse;
			}
			uint8_t *out[] = {m_audioBuf}; 
			int outSize = sizeof(m_audioBuf)/XEE::audioChannel/av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
			m_dataLen = swr_convert(m_pSwrContext,out,outSize,(const uint8_t **)m_pFrame->extended_data,m_pFrame->nb_samples);  
			m_dataLen = m_dataLen * XEE::audioChannel * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
			avcodec_free_frame(&m_pFrame);
			return XTrue;//����ɹ�
		}
		avcodec_free_frame(&m_pFrame);
	}else 
	{
		gotoFrame(0.0f);	//����ͷ
		LogStr("File end!");
		return XFalse;
	}
	return XFalse;
}