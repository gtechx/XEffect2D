//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMicrophone.h"
#include "stdio.h"
#include "string.h"
#include "../XLogBook.h"

void _XMicrophone::printAllCaptureDevice()
{
	const ALCchar * devices; 
	const ALCchar * ptr; 
	LogStr("����ʹ�õ�¼���豸��:"); 
	devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	ptr = devices; 
	while (*ptr)
	{ 
		LogNull("   %s\n", ptr); 
		ptr += strlen(ptr) + 1; 
	}
}
_XBool _XMicrophone::openDevice(const ALCchar *devicename,ALCuint frequency,ALCenum format,ALCsizei buffersize)
{
	if(m_isInited) return XFalse;		//��ֹ�ظ����豸
	const ALCchar * devices = NULL; 
	m_frequency = frequency;		//��Ƶ��Ƶ��
	m_format = format;				//��Ƶ�ĸ�ʽ
	m_buffersize = buffersize;		//��Ƶ�Ļ���ռ��С
	LogStr("��Ĭ�ϵ�¼���豸:"); 
	m_pCaptureDev = alcCaptureOpenDevice(devicename,frequency,format,buffersize); 
	if(m_pCaptureDev == NULL) 
	{  
		LogStr("   ��¼���豸ʧ��!"); 
		return XFalse; 
	} 
	devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	LogNull("   �Ѿ���¼���豸 %s\n", devices);

	m_dataBuffMaxSize = 10000;	//Ĭ�ϵ�������ݴ�С
	m_captureData = createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_nowDataBuffSize = 0;

	switch(format)
	{
	case AL_FORMAT_MONO8:m_byteRate = 1;break;
	case AL_FORMAT_MONO16:m_byteRate = 2;break;
	case AL_FORMAT_STEREO8:m_byteRate = 2;break;
	case AL_FORMAT_STEREO16:m_byteRate = 4;break;
	}

	m_isInited = XTrue;		//����豸�Ѿ���
	return XTrue;
}
unsigned char * _XMicrophone::getCaptureData(int * size)
{
	int ret = 0;
	if(size == NULL) size = & ret; 
	(* size) = 0;
	if(!m_isInited) return NULL;
	alcGetIntegerv(m_pCaptureDev,ALC_CAPTURE_SAMPLES,1,size); 
	if((*size) > 0)	//��¼���������� 
	{//����ȡ����������
		m_nowDataBuffSize = (*size) << 2;
		if(m_nowDataBuffSize <= m_dataBuffMaxSize)
		{
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//ȡ����������
			return m_captureData;	//�����ж�ȡ����������
		}else
		{//������Ҫ���·����ڴ�ռ�
			XDELETE_ARRAY(m_captureData);
			m_dataBuffMaxSize = m_nowDataBuffSize;
			m_captureData = createArrayMem<unsigned char>(m_dataBuffMaxSize);//�½��ڴ�ռ�
			if(m_captureData == NULL) 
			{
				m_dataBuffMaxSize = 0;
				return NULL;	
			}
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//ȡ����������
			return m_captureData;	//�����ж�ȡ����������
		}
		//printf("%d\n",(*size));
	}
	return NULL;	//����û�ж�ȡ����������
}