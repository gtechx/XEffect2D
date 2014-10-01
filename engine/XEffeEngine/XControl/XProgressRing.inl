inline void _XProgressRing::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_caption.setPosition(m_position);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_nowMouseRect.set(m_position.x - size * m_size.x,m_position.y - size * m_size.y,
		m_position.x + size * m_size.x,m_position.y + size * m_size.y);
}
inline void _XProgressRing::setSize(float x,float y)
{
	m_size.set(x,y);
	m_caption.setSize(m_size);
	float size = m_r0 > m_r1?m_r0:m_r1;
	m_nowMouseRect.set(m_position.x - size * m_size.x,m_position.y - size * m_size.y,
		m_position.x + size * m_size.x,m_position.y + size * m_size.y);
}
inline void _XProgressRing::setTextColor(const _XFColor& color)	//�����������ɫ
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XProgressRing::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	updateChildColor();
}
inline void _XProgressRing::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	updateChildAlpha();
}
inline void _XProgressRing::setValue(float temp)
{
	if(!m_isInited) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
	int tempI = temp;
	if(tempI == (int)m_nowValue) return;
	m_nowValue = temp;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",tempI);
	m_caption.setString(tempC);
}
inline void _XProgressRing::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
inline _XBool _XProgressRing::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XProgressRing::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}