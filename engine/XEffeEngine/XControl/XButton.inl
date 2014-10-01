inline void _XButtonTexture::releaseTex()
{
	XDELETE(buttonNormal);
	XDELETE(buttonDown);
	XDELETE(buttonOn);
	XDELETE(buttonDisable);
}
inline void _XButtonTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline _XBool _XButton::initEx(const _XVector2& position,	//����һ���ӿڵļ򻯰汾
		const _XButtonTexture &tex,		
		const char *caption,const _XFontUnicode &font,float captionSize)
{
	return init(position,tex.m_mouseRect,tex,caption,font,captionSize,tex.m_fontPosition);
}
inline _XBool _XButton::initPlus(const char * path,const char *caption,const _XFontUnicode &font,float captionSize,
		_XResourcePosition resoursePosition)//���Ǿ��������Ż��İ汾�������Ժ���ֻ��������汾
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,caption,font,captionSize);
}
inline _XBool _XButton::initWithoutTex(const char *caption,const _XFontUnicode &font,const _XRect& area)	//����ӿ����ϸ��ӿڵļ򻯰汾
{
	return initWithoutTex(caption,font,1.0f,area,area.getCenter());
}
inline void _XButton::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XButton::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildColor();
}
inline void _XButton::setAlpha(float a)
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_withNormalIcon) m_normalIcon.setColor(m_color);
	if(m_withDisableIcon) m_disableIcon.setColor(m_color);
	updateChildAlpha();
}
inline _XBool _XButton::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_nowMouseRect.isInRect(x,y);
}
inline void _XButton::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	if(m_nowButtonState != BUTTON_STATE_DISABLE) m_nowButtonState = BUTTON_STATE_NORMAL;
	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
inline _XBool _XButton::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XButton::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XButton::setCaptionPosition(const _XVector2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
inline void _XButton::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
inline void _XButton::setCaptionSize(const _XVector2& size)						//���ð�ť�ı���ĳߴ�
{
	setCaptionSize(size.x,size.y);
}
inline void _XButton::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}
inline void _XButton::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}
inline void _XButton::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL) m_caption.setString(caption);
}
inline int _XButton::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}
inline void _XButton::setState(_XButtonState temp)		//���ð�ť��״̬
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButton::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}
inline void _XButton::disable()
{
	m_isEnable = XFalse;
	m_nowButtonState = BUTTON_STATE_DISABLE;
	m_isBeChoose = XFalse;
	m_caption.setColor((m_textColor * m_color).anti());
}
inline void _XButton::enable()
{
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		m_isEnable = XTrue;
		m_nowButtonState = BUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
		m_caption.setColor(m_textColor * m_color);
	}
}
inline void _XButton::setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass)
{
	m_funMouseDown = funMouseDown;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButton::setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass)
{
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButton::setIconPosition(float x,float y)	//����icon��λ��
{
	m_iconPosition.set(x,y);
	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	if(m_withDisableIcon) m_disableIcon.setPosition(m_position + m_iconPosition * m_size);
}
inline void _XButton::setIconSize(float x,float y)		//����icon�����Ŵ�С
{
	if(x <= 0.0f || y <= 0.0f) return;
	m_iconSize.set(x,y);
	if(m_withNormalIcon) m_normalIcon.setSize(m_iconSize * m_size);
	if(m_withDisableIcon) m_disableIcon.setSize(m_iconSize * m_size);
}
inline void _XButton::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		drawRect(m_lightRect,1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
	m_comment.draw();
}
