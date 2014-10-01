inline void _XGroup::updateData()		//�����ڲ�����
{
	if(m_state == STATE_NORMAL)
	{
		m_caption.setPosition(m_position.x + (m_rect.left + XGROUP_STATE_BTN_SIZE + 5.0f) * m_size.x,m_position.y + m_rect.top * m_size.y);
	//	m_stateBotton.setPosition(m_position.x + (m_rect.right - XGROUP_STATE_BTN_SIZE) * m_size.x,m_position.y + m_rect.top * m_size.y);
		m_stateBotton.setPosition(m_position.x,m_position.y + m_rect.top * m_size.y);
		m_drawRect.set(m_rect.left * m_size.x + m_position.x,m_rect.top * m_size.y + m_position.y,
			m_rect.right * m_size.x + m_position.x,m_rect.bottom * m_size.y + m_position.y);
	}else
	{
		m_caption.setPosition(m_position.x + (XGROUP_STATE_BTN_SIZE + 3.0f) * m_size.x,m_position.y + 3.0f * m_size.y);
	//	m_stateBotton.setPosition(m_position.x + 3.0f * m_size.x + m_caption.getMaxPixelWidth(),m_position.y + 3.0f * m_size.y);
		m_stateBotton.setPosition(m_position.x,m_position.y + 3.0f * m_size.y);
		m_drawRect.left = m_rect.left * m_size.x + m_position.x;
		m_drawRect.top = m_rect.top * m_size.y + m_position.y;
		m_drawRect.right = m_drawRect.left + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x + m_caption.getMaxPixelWidth();
		m_drawRect.bottom = m_drawRect.top + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x;
	}
}
inline void _XGroup::setState(_XGroupState state)
{
	if(m_state == state) return;
	m_state = state;
	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);	//ע���������˳��������ڿɼ�״̬�ı�֮�����ɳ�ԱAction������
	if(m_state == STATE_NORMAL)
	{
		m_stateBotton.setCaptionText("-");
		//��������������
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{
		m_stateBotton.setCaptionText("+");
		//��ʾ����������
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
	updateData();
}
inline void _XGroup::setCallbackFun(void (* funStateChange)(void *,int),
		void *pClass)
{
	m_funStateChange = funStateChange;
	m_pClass = pClass;
}
inline void _XGroup::resetSize(const _XVector2 &size)
{
	m_rect.right = size.x;
	m_rect.bottom = size.y;
	updateData();
}
inline void _XGroup::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateData();
	updateChildPos();
}
inline void _XGroup::setSize(float x,float y) 
{
	m_size.set(x,y);
	m_caption.setSize(m_textSize * m_size);
	m_stateBotton.setSize(m_size);
	updateData();
	updateChildSize();
}
inline void _XGroup::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption == NULL) return;
	m_caption.setString(caption);
}
inline _XBool _XGroup::mouseProc(float,float,_XMouseState)
{
//	if(!m_isInited) return XTrue;	//���û�г�ʼ��ֱ���˳�
//	if(!m_isVisible) return XTrue;	//������ɼ�ֱ���˳�
//	m_stateBotton.mouseProc(x,y,mouseState);
	return XTrue;
}		//do nothing
inline _XBool _XGroup::keyboardProc(int,_XKeyState)
{
//	if(!m_isInited) return XTrue;	//���û�г�ʼ��ֱ���˳�
//	if(!m_isVisible) return XTrue;	//������ɼ�ֱ���˳�
//	m_stateBotton.keyboardProc(keyOrder,keyState);
	return XTrue;
}
inline _XBool _XGroup::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline void _XGroup::setVisible()
{
	m_isVisible = XTrue;
	m_stateBotton.setVisible();
	if(m_state == STATE_NORMAL)
	{//��������������
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{//��ʾ����������
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
	//updateChildVisible();
}
inline void _XGroup::disVisible()
{
	m_isVisible = XFalse;
	m_stateBotton.disVisible();
	updateChildVisible();
}
inline _XVector2 _XGroup::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_drawRect.left,m_drawRect.top);
	case 1: return _XVector2(m_drawRect.right,m_drawRect.top);
	case 2: return _XVector2(m_drawRect.right,m_drawRect.bottom);
	case 3: return _XVector2(m_drawRect.left,m_drawRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XGroup::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_color);
	m_stateBotton.setColor(m_color);
	updateChildColor();
}
inline void _XGroup::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);
	m_stateBotton.setAlpha(a);
	updateChildAlpha();
}