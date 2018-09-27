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
		mouseProc(m_upMousePoint,MOUSE_MOVE);
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
INLINE XBool XSlider::initEx(const XVec2& position,	//������ӿڵļ�
	const XSliderSkin &tex,float max,float min,XSliderType type)
{
	return init(position,tex.m_mouseRect,tex.m_mouseRectButton,tex,max,min,type);
}
INLINE XBool XSlider::initPlus(const char * path,float max,float min,XSliderType type,
	XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXSLIDER_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,*(XSliderSkin *)m_resInfo->m_pointer,max,min,type);
}
INLINE void XSlider::setColor(const XFColor& c)
{
	if(!m_isInited) return;
	m_color = c;
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
INLINE XBool XSlider::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
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
INLINE XBool XSlider::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSlider::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XSlider::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
INLINE void XSlider::setOprateState(void * data)
{
	setCurValue(*(float *)data,true);
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