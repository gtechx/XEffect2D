INLINE void XSimpleLine::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XSimpleLine::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSimpleLine::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
}
INLINE void XSimpleLine::setScale(float x,float y)
{
	m_scale.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x,m_position.y,
			m_position.x + 5.0f * m_scale.x,m_position.y + m_lineLen * m_scale.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			m_position.x + m_lineLen * m_scale.x,m_position.y + 5.0f * m_scale.y);
		break;
	}
}
INLINE void XSimpleLine::setPosition(float x,float y)
{
	m_position.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_curMouseRect.set(m_position.x - 5.0f * m_scale.x,m_position.y,
			m_position.x + 5.0f * m_scale.x,m_position.y + m_lineLen * m_scale.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_curMouseRect.set(m_position.x,m_position.y - 5.0f * m_scale.y,
			m_position.x + m_lineLen * m_scale.x,m_position.y + 5.0f * m_scale.y);
		break;
	}
}