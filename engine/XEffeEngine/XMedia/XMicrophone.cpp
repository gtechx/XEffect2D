#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMicrophone.h"
namespace XE{
void XMicrophone::printAllCaptureDevice()
{
	LogStr("����ʹ�õ�¼���豸��:"); 
	const ALCchar *ptr = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	while (*ptr)
	{ 
		LogNull("   %s\n", ptr); 
		ptr += strlen(ptr) + 1; 
	}
}
XBool XMicrophone::openDevice(const ALCchar *devicename,ALCuint frequency,ALCenum format,ALCsizei buffersize)
{
	if(m_isInited) return XFalse;		//��ֹ�ظ����豸
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
	//const ALCchar * devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	LogNull("   �Ѿ���¼���豸 %s\n", alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER));

	m_dataBuffMaxSize = 10000;	//Ĭ�ϵ�������ݴ�С
	m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_curDataBuffSize = 0;

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
XBool XMicrophone::openDeviceByIndex(int index,ALCuint frequency, ALCenum format, ALCsizei buffersize)	//����Ŵ��豸
{
	if(m_isInited) return XFalse;		//��ֹ�ظ����豸
	//Ѱ�ҵ�ָ�����豸
	std::string deviceName = "";
	const ALCchar *ptr = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER); 
	while (*ptr)
	{ 
		if(index == 0)
		{
			deviceName = ptr;
			break;
		}
		ptr += strlen(ptr) + 1; 
		-- index;
	}
	if(deviceName.size() <= 0)
	{//û���ҵ�ָ�����豸
		LogStr("û��ָ�����豸!"); 
		return XFalse;
	}
	//��ָ���豸
	m_frequency = frequency;		//��Ƶ��Ƶ��
	m_format = format;				//��Ƶ�ĸ�ʽ
	m_buffersize = buffersize;		//��Ƶ�Ļ���ռ��С
	m_pCaptureDev = alcCaptureOpenDevice(deviceName.c_str(),frequency,format,buffersize); 
	if(m_pCaptureDev == NULL) 
	{  
		LogStr("   ��¼���豸ʧ��!"); 
		return XFalse; 
	} 
	//const ALCchar devices = alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER); 
	LogNull("   �Ѿ���¼���豸 %s\n",alcGetString(m_pCaptureDev, ALC_CAPTURE_DEVICE_SPECIFIER));

	m_dataBuffMaxSize = 10000;	//Ĭ�ϵ�������ݴ�С
	m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);
	if(m_captureData == NULL) return XFalse;
	m_curDataBuffSize = 0;

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
unsigned char * XMicrophone::getCaptureData(int * size)
{
	int ret = 0;
	if(size == NULL) size = & ret; 
	(* size) = 0;
	if(!m_isInited) return NULL;
	alcGetIntegerv(m_pCaptureDev,ALC_CAPTURE_SAMPLES,1,size); 
	if((*size) > 0)	//��¼���������� 
	{//����ȡ����������
		m_curDataBuffSize = (*size) * m_byteRate;
		if(m_curDataBuffSize <= m_dataBuffMaxSize)
		{
			alcCaptureSamples(m_pCaptureDev,m_captureData,(*size));	//ȡ����������
			return m_captureData;	//�����ж�ȡ����������
		}else
		{//������Ҫ���·����ڴ�ռ�
			XMem::XDELETE_ARRAY(m_captureData);
			m_dataBuffMaxSize = m_curDataBuffSize;
			m_captureData = XMem::createArrayMem<unsigned char>(m_dataBuffMaxSize);//�½��ڴ�ռ�
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
#if !WITH_INLINE_FILE
#include "XMicrophone.inl"
#endif
}