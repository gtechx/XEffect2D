INLINE void XImageCtrl::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_connectWithTex)
	{
		m_sprite.draw(m_connectTex);
	}else
	{
		if(m_onlyWithTexture) return m_sprite.draw(m_texID);
		else m_sprite.draw();
	}
}
INLINE XBool XImageCtrl::canGetFocus(const XVec2& p)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XImageCtrl::setPosition(const XVec2& p)
{
	m_position = p;
	m_sprite.setPosition(m_position);
}
INLINE void XImageCtrl::setScale(const XVec2& s)
{
	m_scale = s;
	m_sprite.setScale(m_scale);
}
INLINE void XImageCtrl::fitScale(float x,float y)
{
	setScale(x / m_w,y / m_h);
}
INLINE void XImageCtrl::setColor(const XFColor& c)
{
	m_color = c;
	return m_sprite.setColor(m_color);
}
INLINE void XImageCtrl::setAlpha(float a)
{
	m_color.setA(a);
	return m_sprite.setAlpha(a);
}