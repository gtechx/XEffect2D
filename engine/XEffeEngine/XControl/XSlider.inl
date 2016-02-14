INLINE void XSliderSkin::releaseTex()
{
	XMem::XDELETE(sliderLineNormal);
	XMem::XDELETE(sliderLineDown);
	XMem::XDELETE(sliderLineOn);
	XMem::XDELETE(sliderLineDisable);
	XMem::XDELETE(sliderButtonNormal);
	XMem::XDELETE(sliderButtonDown);
	XMem::XDELETE(sliderButtonOn);
	XMem::XDELETE(sliderButtonDisable);
}
INLINE void XSliderSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE void XSlider::disable()//ʹ�ؼ���Ч
{
	m_curSliderState = SLIDER_STATE_DISABLE;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XSlider::enable()//ʹ�ؼ���Ч
{
	if(m_curSliderState == SLIDER_STATE_DISABLE)
	{
		m_curSliderState = SLIDER_STATE_NORMAL;
		//����Ҫ������꺯��
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
	m_isEnable = XTrue;
}
INLINE float XSlider::getCurValue() const	//��ȡ���鵱ǰ��ֵ
{
	return m_curValue;
}
INLINE float XSlider::getMaxValue() const
{
	return m_maxValue;
}
INLINE float XSlider::getMinValue() const
{
	return m_minValue;
}
INLINE XBool XSlider::initEx(const XVector2& position,	//������ӿڵļ�
	const XSliderSkin &tex,float max,float min,XSliderType type)
{
	return init(position,tex.m_mouseRect,tex.m_mouseRectButton,tex,max,min,type);
}
INLINE XBool XSlider::initPlus(const char * path,float max,float min,XSliderType type,
	XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXSLIDER_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVector2::zero,*(XSliderSkin *)m_resInfo->m_pointer,max,min,type);
}
INLINE void XSlider::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSlider::setTextColor(const XFColor& color)
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XSlider::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_buttonSprite.setColor(m_color);
	m_lineSprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
INLINE XBool XSlider::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XSlider::setLostFocus() 
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�
	if(m_curSliderState != SLIDER_STATE_DISABLE) m_curSliderState = SLIDER_STATE_NORMAL;
	m_isBeChoose = XFalse;
}
INLINE XBool XSlider::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSlider::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0: return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1: return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2: return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3: return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}
INLINE void XSlider::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f);
	}
}
INLINE void XSlider::setOprateState(void * data)
{
	float index = *(float *)data;
	setCurValue(index);
}
INLINE void *XSlider::getOprateState() const
{
	float *data = XMem::createMem<float>();
	*data = getCurValue();
	return data;
}
INLINE void XSlider::releaseOprateStateData(void *p)
{
	float *data = (float *)p;
	XMem::XDELETE(data);
}
INLINE bool XSlider::isSameState(void * data)
{
	if(data == NULL) return false;
	return(*(float*)data == getCurValue());
}