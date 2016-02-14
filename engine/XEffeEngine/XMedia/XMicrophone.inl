INLINE void XMicrophone::captureStart() const	//���ÿ�ʼ¼��
{
	if(m_captureState == CAPTURE_STATE_STOP
		&& m_isInited) alcCaptureStart(m_pCaptureDev); 
}
INLINE void XMicrophone::captureStop() const		//����ֹͣ¼��
{
	if(m_captureState == CAPTURE_STATE_START
		&& m_isInited) alcCaptureStop(m_pCaptureDev); 
}
INLINE void XMicrophone::release()
{
	if(!m_isInited) return;
	alcMakeContextCurrent(NULL);
	alcCaptureCloseDevice(m_pCaptureDev); 
	XMem::XDELETE_ARRAY(m_captureData);

	m_isInited = XFalse;
}