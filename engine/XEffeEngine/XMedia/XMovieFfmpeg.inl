INLINE void XMovieFfmpeg::clearVideoQueue()	//�����Ƶ����
{
	SDL_LockMutex(m_videoQueue.mutex);
	m_videoQueue.first_pict = NULL;
	m_videoQueue.last_pict = NULL;
	m_videoQueue.nb_pict = 0;
	SDL_UnlockMutex(m_videoQueue.mutex);
}
INLINE void XMovieFfmpeg::play()
{
	if(!m_isEnd) m_isStop = false;
	else replay();
}
INLINE void XMovieFfmpeg::gotoTime(int t)	//��ת�����뼶��λ��
{
	m_gotoMutex1.Lock();
	m_curPlayedTime = t;	//��������ʱ��
	m_gotoMutex1.Unlock();
}
INLINE void XMovieFfmpeg::updateTex(XTexture &tex)
{
	m_mutex.Lock();
	tex.updateTexture(m_curFrameData,m_videoWidth,m_videoHeight);
	m_mutex.Unlock();
}
INLINE void XMovieFfmpeg::draw()
{
	if(!m_isLoaded) return;
	if(m_movieSprite != NULL && m_movieTex != NULL && m_isTexInit) 
		m_movieSprite->draw(&m_movieTex->m_texture);
}
INLINE int XMovieFfmpeg::imgConvert(AVPicture *dst,const AVPicture *src,int src_height)
{
	if(m_pSwsContext == NULL) return 0;
	return sws_scale(m_pSwsContext, src->data, src->linesize,  
        0,src_height, dst->data, dst->linesize); 
}
INLINE unsigned char * XMovieFfmpeg::getAVideoFrame()			//����Ƶ������ȡ��һ֡���ݣ����ǲ��ı����
{
	if(m_videoQueue.nb_pict <= 0) return NULL;
	if(m_videoQueue.first_pict != NULL) 
		return m_videoQueue.first_pict->data;
	else return NULL;
}