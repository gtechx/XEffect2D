inline void _XText::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
inline void _XText::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_font.setPosition(x,y);
}
inline void _XText::setSize(float x,float y)
{
	m_size.set(x,y);
	m_font.setSize(x,y);
}
inline void _XText::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(r,g,b,a);
}
inline void _XText::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
}
inline void _XText::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_font.draw();
}								//��水ť
