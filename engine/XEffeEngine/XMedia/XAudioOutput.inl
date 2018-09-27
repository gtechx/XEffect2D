INLINE void XAudioOutput::release()
{
	stop();
	m_audioBuff.release();
//	XMem::XDELETE_ARRAY(m_audioBuff);
	XMem::XDELETE_ARRAY(m_tmpBuffer);
	if (m_pSwrContext != NULL)
	{
		swr_free(&m_pSwrContext);
		m_pSwrContext = nullptr;
	}
}
INLINE bool XAudioOutput::start()	//��ʼ�ط�
{
	if(m_isStart) return false;	//�����ظ���ʼ
	//����ע��ص�����
	XCurSndCore.setCallBack((SND_CALLBACK_FUN)audioCallBack,this);
	m_isStart = true;
	return true;
}
INLINE bool XAudioOutput::stop()	//ֹͣ�ط�
{
	if(!m_isStart) return false;
	//ע���ص�����
	if(gFrameworkData.pSoundCore != nullptr)
		XCurSndCore.setCallBack(NULL,NULL);
//	m_curUsage = 0;
	m_audioBuff.clear();
	m_isStart = false;
	return true;
}
INLINE float XAudioOutput::getCurCanPlayTime()const//���ص�ǰ�����е����ݿ����ڲ��Ŷ೤��ʱ��
{//��λ��
	return m_audioBuff.getUsage() / (XEG.getAudioSampleRate() * XEG.getAudioChannelSum() * 2.0f);
}	