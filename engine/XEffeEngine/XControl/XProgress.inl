inline void _XProgressTexture::release()
{
	if(!m_isInited) return;
	XDELETE(progressBackGround);
	XDELETE(progressMove);
	XDELETE(progressUpon);
	m_isInited = XFalse;
}
inline float _XProgress::getValue() const
{
	return m_nowValue;
}
inline void _XProgress::enable()
{
	m_isEnable = XTrue;
}
inline void _XProgress::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline _XBool _XProgress::initEx(const _XVector2& position,//������ӿڵļ�
	const _XProgressTexture &tex,	
	_XNumber* font,float captionSize,
	int mode)
{
	return init(position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition,mode);
}
inline _XBool _XProgress::initPlus(const char * path,
	_XNumber* font,float captionSize,	//�ؼ�������
	int mode,_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XPROGRESS_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XProgressTexture *)m_resInfo->m_pointer,font,captionSize,mode);
}
inline void _XProgress::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XProgress::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}	//���ð�ť����ɫ
inline void _XProgress::setAlpha(float a) 
{
	m_color.setA(a);
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}
inline _XBool _XProgress::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XProgress::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}