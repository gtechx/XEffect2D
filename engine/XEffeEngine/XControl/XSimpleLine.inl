INLINE void XSimpleLine::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XSimpleLine::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSimpleLine::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XSimpleLine::setScale(const XVec2& s)
{
	m_scale = s;
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x, m_position.y,
			XVec2(m_scale.x, m_lineLen * m_scale.y));
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x, m_position.y - 5.0f * m_scale.y,
			XVec2(m_lineLen * m_scale.x, m_scale.y));
		break;
	}
}
INLINE void XSimpleLine::setPosition(const XVec2& p)
{
	m_position = p;
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x, m_position.y,
			XVec2(m_scale.x, m_lineLen * m_scale.y));
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			XVec2(m_lineLen * m_scale.x, m_scale.y));
		break;
	}
}