INLINE void XButtonSkin::releaseTex()
{
	XMem::XDELETE(buttonNormal);
	XMem::XDELETE(buttonDown);
	XMem::XDELETE(buttonOn);
	XMem::XDELETE(buttonDisable);
}
INLINE void XButtonSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE XBool XButton::initEx(const XVec2& position,	//����һ���ӿڵļ򻯰汾
		const XButtonSkin &tex,		
		const char *caption,const XFontUnicode& font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XButton::initPlus(const char * path,const char *caption,const XFontUnicode& font,float captionSize,
		XResPos resPos)//���Ǿ��������Ż��İ汾�������Ժ���ֻ��������汾
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XButtonSkin *)m_resInfo->m_pointer,caption,font,captionSize);
}
INLINE XBool XButton::initWithoutSkin(const char *caption,const XFontUnicode& font,const XRect& area)	//����ӿ����ϸ��ӿڵļ򻯰汾
{
	return initWithoutSkin(caption,font,1.0f,area,area.getCenter());
}
INLINE void XButton::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE void XButton::setColor(const XFColor& c)
{
	m_color = c;
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildColor();
}
INLINE void XButton::setAlpha(float a)
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildAlpha();
}
INLINE XBool XButton::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(p);
}
INLINE void XButton::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	if(m_curButtonState != BUTTON_STATE_DISABLE) m_curButtonState = BUTTON_STATE_NORMAL;
	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XButton::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XButton::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XButton::setCaptionPosition(const XVec2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition = textPosition;
	m_caption.setPosition(m_position + m_textPosition * m_scale);
}
//INLINE void XButton::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
//{
//	setCaptionPosition(XVec2(x,y));
//}
INLINE void XButton::setCaptionSize(const XVec2& size)						//���ð�ť�ı���ĳߴ�
{
	if(size.x < 0 || size.y < 0) return;
	m_textSize = size;
	m_caption.setScale(m_textSize * m_scale);
}
//INLINE void XButton::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
//{
//	setCaptionSize(XVec2(x,y));
//}
INLINE void XButton::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}
INLINE void XButton::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL) m_caption.setString(caption);
}
INLINE int XButton::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}
INLINE void XButton::setState(XButtonState temp)		//���ð�ť��״̬
{
	m_curButtonState = temp;
} 
INLINE XButtonState XButton::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_curButtonState;
}
INLINE void XButton::disable()
{
	m_isEnable = XFalse;
	m_curButtonState = BUTTON_STATE_DISABLE;
	m_isBeChoose = XFalse;
	m_caption.setColor((m_textColor * m_color).anti());
}
INLINE void XButton::enable()
{
	if(m_curButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_curButtonState = BUTTON_STATE_NORMAL;
	//2016.6.5����ĺ��������һЩ��ʾ�ϵ����⣬�����в���ȷΪʲôҪ��ô�������������ε�
//	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XButton::setIconPosition(const XVec2& p)	//����icon��λ��
{
	m_iconPosition = p;
	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
	if(m_withDisableIcon) m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
}
INLINE void XButton::setIconSize(const XVec2& p)		//����icon�����Ŵ�С
{
	if(p.x <= 0.0f || p.y <= 0.0f) return;
	m_iconSize = p;
	if(m_withNormalIcon) m_normalIcon.setScale(m_iconSize * m_scale);
	if(m_withDisableIcon) m_disableIcon.setScale(m_iconSize * m_scale);
}
INLINE void XButton::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
