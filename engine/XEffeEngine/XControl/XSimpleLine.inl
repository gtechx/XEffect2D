inline void _XSimpleLine::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
inline _XBool _XSimpleLine::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XSimpleLine::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XSimpleLine::setSize(float x,float y)
{
	m_size.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_nowMouseRect.set(m_position.x - 5.0f * m_size.x,m_position.y,
			m_position.x + 5.0f * m_size.x,m_position.y + m_lineLen * m_size.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_nowMouseRect.set(m_position.x,m_position.y - 5.0f * m_size.y,
			m_position.x + m_lineLen * m_size.x,m_position.y + 5.0f * m_size.y);
		break;
	}
}
inline void _XSimpleLine::setPosition(float x,float y)
{
	m_position.set(x,y);
	switch(m_type)
	{
	case SIMPLELINE_TYPE_VERTICAL:
		m_nowMouseRect.set(m_position.x - 5.0f * m_size.x,m_position.y,
			m_position.x + 5.0f * m_size.x,m_position.y + m_lineLen * m_size.y);
		break;
	case SIMPLELINE_TYPE_HORIZONTAL:
		m_nowMouseRect.set(m_position.x,m_position.y - 5.0f * m_size.y,
			m_position.x + m_lineLen * m_size.x,m_position.y + 5.0f * m_size.y);
		break;
	}
}