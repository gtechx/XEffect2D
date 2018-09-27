INLINE void XSliderInfinite::draw()//��滬����
{
	if (!m_isInited || !m_isVisible) return;

	m_mainSld.draw();
	m_addMinBtn.draw();
	m_desMinBtn.draw();
	m_addMaxBtn.draw();
	m_desMaxBtn.draw();
}
INLINE void XSliderInfinite::drawUp()
{
	if (!m_isInited || !m_isVisible) return;

	m_mainSld.drawUp();
	m_addMinBtn.drawUp();
	m_desMinBtn.drawUp();
	m_addMaxBtn.drawUp();
	m_desMaxBtn.drawUp();
}
INLINE XBool XSliderInfinite::keyboardProc(int keyOrder, XKeyState keyState)
{
	if (m_isSilent) return XFalse;
	return m_mainSld.keyboardProc(keyOrder, keyState);
}
INLINE XBool XSliderInfinite::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XSliderInfinite::setColor(const XFColor& c)
{
	if (!m_isInited) return;
	m_color = c;
	m_mainSld.setColor(m_color);
	m_addMinBtn.setColor(m_color);
	m_desMinBtn.setColor(m_color);
	m_addMaxBtn.setColor(m_color);
	m_desMaxBtn.setColor(m_color);
	updateChildColor();
}//�����������ɫ
INLINE void XSliderInfinite::setAlpha(float a)
{
	if (!m_isInited) return;
	m_color.setA(a);
	m_mainSld.setColor(m_color);
	m_addMinBtn.setColor(m_color);
	m_desMinBtn.setColor(m_color);
	m_addMaxBtn.setColor(m_color);
	m_desMaxBtn.setColor(m_color);
	updateChildAlpha();
}
INLINE void XSliderInfinite::setCurValue(float temp, bool withEvent)
{
	if(temp >= m_mainSld.getMinValue() && temp <= m_mainSld.getMaxValue())
		m_mainSld.setCurValue(temp, withEvent);
	else
	{//���������Χ����Ҫ��������
		float area = m_mainSld.getMaxValue() - m_mainSld.getMinValue();
		m_mainSld.setRange(temp + area * 0.5f, temp - area * 0.5f, false);
		m_mainSld.setCurValue(temp, withEvent);
	}
}
INLINE void XSliderInfinite::disable()//ʹ�ؼ���Ч
{
	m_mainSld.disable();

	m_isEnable = XFalse;
	m_isBeChoose = XFalse;

	m_addMinBtn.disVisible();
	m_desMinBtn.disVisible();
	m_addMaxBtn.disVisible();
	m_desMaxBtn.disVisible();
}
INLINE void XSliderInfinite::enable()//ʹ�ؼ���Ч
{
	m_mainSld.enable();

	m_addMinBtn.enable();
	m_desMinBtn.enable();
	m_addMaxBtn.enable();
	m_desMaxBtn.enable();
	m_isEnable = XTrue;
}
INLINE float XSliderInfinite::getCurValue() const	//��ȡ���鵱ǰ��ֵ
{
	return m_mainSld.getCurValue();
}
INLINE float XSliderInfinite::getMaxValue() const
{
	return m_mainSld.getMaxValue();
}
INLINE float XSliderInfinite::getMinValue() const
{
	return m_mainSld.getMinValue();
}
INLINE void XSliderInfinite::setVisible()
{
	m_isVisible = XTrue;
	m_mainSld.setVisible();
	//m_secondarySld.setVisible();
	//m_chooseBtn.setVisible();
	updateChildVisible();
}
INLINE XBool XSliderInfinite::isInRect(const XVec2& p) //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if (!m_isInited) return XFalse;
	return XMath::getIsInRect(p, getBox(0), getBox(1), getBox(2), getBox(3));
}
INLINE void XSliderInfinite::setOprateState(void * data)
{
	float index = *(float *)data;
	setCurValue(index);
}
INLINE void *XSliderInfinite::getOprateState() const
{
	float *data = XMem::createMem<float>();
	*data = getCurValue();
	return data;
}
INLINE void XSliderInfinite::releaseOprateStateData(void *p)
{
	float *data = (float *)p;
	XMem::XDELETE(data);
}
INLINE bool XSliderInfinite::isSameState(void * data)
{
	if (data == NULL) return false;
	return(*(float*)data == getCurValue());
}
INLINE void XSliderInfinite::setRange(float max, float min)
{
	m_mainSld.setRange(max, min);
}
INLINE void XSliderInfinite::disVisible()
{
	m_isVisible = m_isBeChoose = XFalse;
	m_mainSld.disVisible();

	m_addMinBtn.disVisible();
	m_desMinBtn.disVisible();
	m_addMaxBtn.disVisible();
	m_desMaxBtn.disVisible();
	updateChildVisible();
}