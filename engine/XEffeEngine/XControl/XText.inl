INLINE void XText::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE void XText::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_font.setPosition(x,y);
}
INLINE void XText::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_font.setScale(x,y);
}
INLINE void XText::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(r,g,b,a);
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
