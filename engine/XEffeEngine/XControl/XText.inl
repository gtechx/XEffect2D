INLINE void XText::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE void XText::setPosition(const XVec2& p)
{
	m_position = p;
	m_font.setPosition(m_position);
}
INLINE void XText::setScale(const XVec2& s)
{
	m_scale = s;
	m_font.setScale(m_scale);
}
INLINE void XText::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_color);
}
INLINE void XText::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
}
INLINE void XText::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_font.draw();
}								//��水ť
INLINE void XText::setAlignmentMode(XFontAlignmentModeX x, XFontAlignmentModeY y)
{
	if (!m_isInited) return;
	m_font.setAlignmentModeX(x);
	m_font.setAlignmentModeY(y);
}