#include "XStdHead.h"
#include "XAudioOutput.h"
namespace XE{
void XAudioOutput::audioCallBack(void *userData,Uint8 *stream,int len)
{
	XAudioOutput &pPar = *(XAudioOutput *)userData;
	//�����������������
	if(pPar.m_curUsage == 0)
	{//û������
		memset(stream,0,len);
	}else
	{
		pPar.m_mutex.Lock();
		if(pPar.m_curUsage >= len)
		{//���������
			memcpy(stream,pPar.m_audioBuff,len);
			//ȥ���Ѿ�ʹ�ù�������
			pPar.m_curUsage -= len;
			memmove(pPar.m_audioBuff,pPar.m_audioBuff + len,pPar.m_curUsage);
		}else
		{//�����Ĳ���
			memcpy(stream,pPar.m_audioBuff,pPar.m_curUsage);
			//ʣ�µĲ��ֲ�0
			memset(stream + pPar.m_curUsage,0,len - pPar.m_curUsage);
			pPar.m_curUsage = 0;
		}
		pPar.m_mutex.Unlock();
	}
}
void XAudioOutput::setConversion(int channelSum,AVSampleFormat format,int sampleRate)	//����ת������
{
//	if(channelSum == XEE::audioChannel && format == getSampleFormat() && 
//		sampleRate == XEE::audioSampleRate) return;	//û��Ҫ����ת��
	if(m_pSwrContext != NULL)
		swr_free(&m_pSwrContext);
	m_pSwrContext = swr_alloc();
	if(m_pSwrContext == NULL) return;
	swr_alloc_set_opts(m_pSwrContext,av_get_default_channel_layout(XEG.getAudioChannelSum()),getSampleFormat(),XEG.getAudioSampleRate(),
		av_get_default_channel_layout(channelSum),format,sampleRate,0,NULL);
	switch(format)
	{
	case AV_SAMPLE_FMT_U8:
	case AV_SAMPLE_FMT_U8P:
		m_conversionPerFrame = channelSum;
		break;
	case AV_SAMPLE_FMT_S16:
	case AV_SAMPLE_FMT_S16P:
		m_conversionPerFrame = channelSum << 1;
		break;
	}
	m_conversionRate = (float)(sampleRate * m_conversionPerFrame) / (float)(XEG.getAudioSampleRate() * XEG.getAudioChannelSum());

	if(swr_init(m_pSwrContext) < 0)
		LogStr("swr_init() fail");
}
bool XAudioOutput::insertData(const unsigned char *data,int dataLen)	//��������
{
	if(dataLen <= 0 || data == NULL) return false;
	if(m_withConversion && m_pSwrContext != NULL)
	{
		#define MAX_TMP_BUFF_SIZE (4096)
		if(m_tmpBuffer == NULL)
		{
			m_tmpBuffer = XMem::createArrayMem<unsigned char>(MAX_TMP_BUFF_SIZE);
			if(m_tmpBuffer == NULL) return false;
		}
		//�������ת�벢����
		int pos = 0;
		int sampleSum = (MAX_TMP_BUFF_SIZE >> 1) * m_conversionRate / m_conversionPerFrame;
		while(true)
		{
			uint8_t *out[] = {m_tmpBuffer}; 
			const uint8_t *in[] = {data + pos,data + dataLen / 2 + pos}; 
			if((dataLen - pos) / m_conversionPerFrame < sampleSum)
			{
				int ret = swr_convert(m_pSwrContext,out,MAX_TMP_BUFF_SIZE / m_conversionPerFrame,in,(dataLen - pos) / m_conversionPerFrame);  
				if(!insertDataX(m_tmpBuffer,ret * m_conversionPerFrame)) return false;
				break;
			}else
			{
				int ret = swr_convert(m_pSwrContext,out,MAX_TMP_BUFF_SIZE / m_conversionPerFrame,in,sampleSum);  
				if(!insertDataX(m_tmpBuffer,ret * m_conversionPerFrame)) return false;
				pos += sampleSum * m_conversionPerFrame;
			}
		}
		return true;
	}else
		return insertDataX(data,dataLen);
}
bool XAudioOutput::insertDataX(const unsigned char *data,int dataLen)	//��������
{
	m_mutex.Lock();
	if(m_buffSize - m_curUsage < dataLen)
	{
		if(!XMem::resizeMem(m_audioBuff,m_curUsage,m_curUsage + dataLen))
		{
			LogStr("�ڴ����ʧ��!");
			return false;
		}
		m_buffSize = m_curUsage + dataLen;
	}
	memcpy(m_audioBuff + m_curUsage,data,dataLen);
	m_curUsage += dataLen;
	m_mutex.Unlock();
	return true;
}
#if !WITH_INLINE_FILE
#include "XAudioOutput.inl"
#endif
}