INLINE void XProgressSkin::release()
{
	if(!m_isInited) return;
	XMem::XDELETE(progressBackGround);
	XMem::XDELETE(progressMove);
	XMem::XDELETE(progressUpon);
	m_isInited = XFalse;
}
INLINE float XProgress::getValue() const
{
	return m_curValue;
}
INLINE void XProgress::enable()
{
	m_isEnable = XTrue;
}
INLINE void XProgress::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE XBool XProgress::initEx(const XVec2& position,//������ӿڵļ�
	const XProgressSkin &tex,	
	const XFontUnicode* font,float captionSize,
	int mode)
{
	return init(position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition,mode);
}
INLINE XBool XProgress::initPlus(const char * path,
	const XFontUnicode* font,float captionSize,	//�ؼ�������
	int mode,XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXPROGRESS_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XProgressSkin *)m_resInfo->m_pointer,font,captionSize,mode);
}
INLINE void XProgress::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	if(m_withNumber)
		m_captionN.setColor(m_textColor * m_color);
	else
		m_caption.setColor(m_textColor * m_color);
}
INLINE void XProgress::setColor(const XFColor&c)
{
	m_color = c;
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	if(m_withNumber)
		m_captionN.setColor(m_textColor * m_color);
	else
		m_caption.setColor(m_textColor * m_color);
}	//���ð�ť����ɫ
INLINE void XProgress::setAlpha(float a) 
{
	m_color.setA(a);
	m_spriteBackground.setColor(m_color);
	m_spriteMove.setColor(m_color);
	m_spriteUpon.setColor(m_color);
	if(m_withNumber)
		m_captionN.setColor(m_textColor * m_color);
	else
		m_caption.setColor(m_textColor * m_color);
}
INLINE XBool XProgress::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XProgress::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0: return m_curMouseRect.getLT();
	case 1: return m_curMouseRect.getRT();
	case 2: return m_curMouseRect.getRB();
	case 3: return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}