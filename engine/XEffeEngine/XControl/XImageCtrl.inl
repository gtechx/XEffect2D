INLINE void XImageCtrl::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_sprite.draw();
}
INLINE XBool XImageCtrl::canGetFocus(float x,float y)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XImageCtrl::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_sprite.setPosition(m_position);
}
INLINE void XImageCtrl::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_sprite.setScale(m_scale);
}
INLINE void XImageCtrl::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	return m_sprite.setColor(r,g,b,a);
}
INLINE void XImageCtrl::setAlpha(float a)
{
	m_color.setA(a);
	return m_sprite.setAlpha(a);
}