INLINE void XParameterCtrl::setCurValue(float value)
{
	m_curData = value;
	m_font.setString(XString::toString(m_curData).c_str());
}
INLINE void XParameterCtrl::setParamType(XParamType type, float iRate, float rRate)
{
	m_paramType = type;
	m_reduceRate = rRate;
	m_increaseRate = iRate;
}
INLINE XBool XParameterCtrl::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if (!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(p);
}
INLINE void XParameterCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	m_btn[0].setPosition(m_position);
	m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y, m_position.y);
	m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//�ַ������м�
	m_curMouseRect.set(m_position, m_position + m_pixelSize * m_scale);
}
INLINE void XParameterCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	m_btn[0].setScale(m_scale);
	m_btn[0].setPosition(m_position);
	m_btn[1].setScale(m_scale);
	m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y, m_position.y);
	m_font.setScale(m_scale);
	m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//�ַ������м�
	m_curMouseRect.set(m_position, m_position + m_pixelSize * m_scale);
}
INLINE void XParameterCtrl::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_textColor * m_color);
	m_btn[0].setColor(m_color);
	m_btn[1].setColor(m_color);
	updateChildColor();
}
INLINE void XParameterCtrl::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setColor(m_textColor * m_color);
	m_btn[0].setColor(m_color);
	m_btn[1].setColor(m_color);
	updateChildColor();
}
INLINE void XParameterCtrl::draw()
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
								//����м�ı���
	XRender::drawFillRectA(m_position, m_pixelSize * m_scale, XFColor::gray, true);
	m_font.draw();
	m_btn[0].draw();
	m_btn[1].draw();
}
INLINE void XParameterCtrl::drawUp()
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_btn[0].drawUp();
	m_btn[1].drawUp();
}
INLINE void XParameterCtrl::update(float stepTime)
{
	if (!m_isInited) return;
	m_btn[0].update(stepTime);
	m_btn[1].update(stepTime);
}
INLINE XBool XParameterCtrl::mouseProc(const XVec2& p, XMouseState mouseState)		//������궯������Ӧ����
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;
	XBool ret = m_btn[0].mouseProc(p, mouseState);
	XBool ret1 = m_btn[1].mouseProc(p, mouseState);
	return (ret || ret1);
}
INLINE XBool XParameterCtrl::keyboardProc(int keyOrder, XKeyState keyState)			//�����Ƿ񴥷���������
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;
	XBool ret = m_btn[0].keyboardProc(keyOrder, keyState);
	XBool ret1 = m_btn[1].keyboardProc(keyOrder, keyState);
	return (ret || ret1);
}
INLINE void XParameterCtrl::setLostFocus()	//����ʧȥ����
{
	if (!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XParameterCtrl::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if (!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XParameterCtrl::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
