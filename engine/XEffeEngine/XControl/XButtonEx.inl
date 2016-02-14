INLINE XBool XButtonEx::initEx(const XVector2& position,	//���Ƕ�����ӿڵļ�
		const XButtonSkin &tex,				
		const char *caption,float captionSize,	
		const XFontUnicode &font)
{
	if(tex.m_pArea == NULL || tex.m_areaPointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	return init(position,tex.m_pArea,tex.m_areaPointSum,tex,caption,captionSize,tex.m_fontPosition,font);
}
INLINE XBool XButtonEx::initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const XFontUnicode &font,XResourcePosition resoursePosition)				//��ť��ʹ�õ�����
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XButtonSkin *)m_resInfo->m_pointer,caption,captionSize,font);
}
INLINE void XButtonEx::setTextColor(const XFColor& color) 
{
	m_textColor = color;
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE void XButtonEx::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}	//���ð�ť����ɫ
INLINE void XButtonEx::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);
}
INLINE XBool XButtonEx::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return XMath::getIsInRect(x,y,m_pCurArea,m_areaPointSum);
}
INLINE void XButtonEx::setLostFocus() 
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	if(m_curButtonState != BUTTON_STATE_DISABLE) 
	{
		m_curButtonState = BUTTON_STATE_NORMAL;
//		m_isBeChoose = XFalse;
	}
}
INLINE XBool XButtonEx::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	//return getIsInRect(m_upMousePoint,m_pCurArea,m_areaPointSum);
}
INLINE void XButtonEx::setCaptionPosition(const XVector2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
INLINE void XButtonEx::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
}
INLINE void XButtonEx::setCaptionSize(const XVector2& size)						//���ð�ť�ı���ĳߴ�
{
	setCaptionSize(size.x,size.y);
}
INLINE void XButtonEx::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
}
INLINE void XButtonEx::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}
INLINE void XButtonEx::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL) m_caption.setString(caption);
}
INLINE int XButtonEx::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}
INLINE void XButtonEx::setState(XButtonState temp)		//���ð�ť��״̬
{
	m_curButtonState = temp;
} 
INLINE XButtonState XButtonEx::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_curButtonState;
}
INLINE void XButtonEx::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_curButtonState = BUTTON_STATE_DISABLE;
}
INLINE void XButtonEx::enable()
{
	if(m_curButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_curButtonState = BUTTON_STATE_NORMAL;
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}