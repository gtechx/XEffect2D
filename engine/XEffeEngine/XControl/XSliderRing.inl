INLINE void XSliderRing::updateFontStr()
{
	char str[1024];
	sprintf_s(str, 1024, m_fontString.c_str(), m_curValue);
	m_caption.setString(str);
}
INLINE XBool XSliderRing::keyboardProc(int keyOrder, XKeyState keyState)
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;

	return XFalse;
}
INLINE XBool XSliderRing::isInRect(const XVec2& p)
{
	if (!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSliderRing::getBox(int order)
{
	if (!m_isInited) return XVec2::zero;
	switch (order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XSliderRing::setTextColor(const XFColor& color)
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
INLINE void XSliderRing::setColor(const XFColor& c)
{
	if (!m_isInited) return;
	m_color = c;
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSliderRing::setAlpha(float a)
{
	if (!m_isInited) return;
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();

}
INLINE XBool XSliderRing::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XSliderRing::setLostFocus()	//����ʧȥ����
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�
	if (m_curSliderState != SLDR_STATE_DISABLE) m_curSliderState = SLDR_STATE_NORMAL;
	m_isBeChoose = XFalse;
}
INLINE void XSliderRing::disable()		//ʹ�ؼ���Ч
{
	m_curSliderState = SLDR_STATE_DISABLE;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XSliderRing::enable()		//ʹ�ؼ���Ч
{
	if (m_curSliderState == SLDR_STATE_DISABLE)
	{
		m_curSliderState = SLDR_STATE_NORMAL;
		//����Ҫ������꺯��
		mouseProc(getMousePos(), MOUSE_MOVE);
	}
	m_isEnable = XTrue;
}