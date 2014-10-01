inline _XBool _XButtonEx::initEx(const _XVector2& position,	//���Ƕ�����ӿڵļ�
		const _XButtonTexture &tex,				
		const char *caption,float captionSize,	
		const _XFontUnicode &font)
{
	if(tex.m_pArea == NULL || tex.m_areaPointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	return init(position,tex.m_pArea,tex.m_areaPointSum,tex,caption,captionSize,tex.m_fontPosition,font);
}
inline _XBool _XButtonEx::initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,_XResourcePosition resoursePosition)				//��ť��ʹ�õ�����
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,caption,captionSize,font);
}
inline void _XButtonEx::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XButtonEx::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}	//���ð�ť����ɫ
inline void _XButtonEx::setAlpha(float a) 
{
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
}
inline _XBool _XButtonEx::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return getIsInRect(x,y,m_pNowArea,m_areaPointSum);
}
inline void _XButtonEx::setLostFocus() 
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	if(m_nowButtonState != BUTTON_STATE_DISABLE) 
	{
		m_nowButtonState = BUTTON_STATE_NORMAL;
//		m_isBeChoose = XFalse;
	}
}
inline _XBool _XButtonEx::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	//return getIsInRect(m_upMousePoint,m_pNowArea,m_areaPointSum);
}
inline void _XButtonEx::setCaptionPosition(const _XVector2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	setCaptionPosition(textPosition.x,textPosition.y);
}
inline void _XButtonEx::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
inline void _XButtonEx::setCaptionSize(const _XVector2& size)						//���ð�ť�ı���ĳߴ�
{
	setCaptionSize(size.x,size.y);
}
inline void _XButtonEx::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}
inline void _XButtonEx::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}
inline void _XButtonEx::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL) m_caption.setString(caption);
}
inline int _XButtonEx::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}
inline void _XButtonEx::setState(_XButtonState temp)		//���ð�ť��״̬
{
	m_nowButtonState = temp;
} 
inline _XButtonState _XButtonEx::getState() const
{
	if(!m_isEnable) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}
inline void _XButtonEx::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_nowButtonState = BUTTON_STATE_DISABLE;
}
inline void _XButtonEx::enable()
{
	if(m_nowButtonState != BUTTON_STATE_DISABLE) return;
	m_isEnable = XTrue;
	m_nowButtonState = BUTTON_STATE_NORMAL;
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
inline void _XButtonEx::setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass)
{
	m_funMouseDown = funMouseDown;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XButtonEx::setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass)
{
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}