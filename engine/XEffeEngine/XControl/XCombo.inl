inline void _XComboTexture::releaseTex()
{
	XDELETE(comboInputNormal);
	XDELETE(comboInputDisable);
	
	downButtonTex.release();
	downMenuUpButtonTex.release();
	downMenuButtonTex.release();
	downMenuDownButtonTex.release();
}
inline void _XComboTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline _XBool _XCombo::initEx(const _XComboTexture &tex,	//����ӿڵļ򻯰汾
		const _XVector2& position,		
		int menuSum,					
		int drawMenuSum,				
		const _XFontUnicode &font,		
		float fontSize)
{
	return init(tex,position,tex.m_mouseRect,tex.downButtonTex.m_mouseRect,
		tex.downMenuUpButtonTex.m_mouseRect,tex.downMenuButtonTex.m_mouseRect,
		tex.downMenuDownButtonTex.m_mouseRect,menuSum,drawMenuSum,font,fontSize);
}
inline _XBool _XCombo::initPlus(const char * path,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;				//��ֹ�ظ���ʼ��
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XCOMBO_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(*(_XComboTexture *)m_resInfo->m_pointer,_XVector2::zero,menuSum,drawMenuSum,font,fontSize);
}
inline _XBool _XCombo::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline _XBool _XCombo::canLostFocus(float,float)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XTrue;		//�����Ч��ֱ���˳�

	if(m_isDrawDownMenu) return XFalse;
	return XTrue;
}
inline void _XCombo::setColor(float r,float g,float b,float a) 	//���ð�ť����ɫ
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}	//���ð�ť����ɫ
inline void _XCombo::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_sprite.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setColor(m_color);
	}
}
inline _XBool _XCombo::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline void _XCombo::setTextColor(const _XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setTextColor(m_textColor);
	}
}	//�����������ɫ
inline _XBool _XCombo::setNowChooseOrder(int index)	//���õ�ǰѡ����
{
	if(index < 0 || index >= m_menuSum) return XFalse;
	if(m_nowChooseOrder == index) return XTrue;
	m_nowChooseOrder = index;
	updateString();
	return XTrue;
}
inline void _XCombo::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	if(funDataChange != NULL) m_funDataChange = funDataChange;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XCombo::disable()					//ʹ��Ч
{
	m_isEnable = XFalse;
//	m_isBeChoose = XFalse;
}
inline void _XCombo::enable()					//ʹ��
{
	m_isEnable = XTrue;
}
inline void _XCombo::update(int stepTime)
{
	m_comment.update(stepTime);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].update(stepTime);
	}
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		float action;
		if(m_actionMoveData.getIsEnd()) 
		{
			m_isInAction = false;
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XTrue);
			}
			action = 1.0f;	//������Ҫ�ָ�
			if(!m_isDraw)
			{
				for(int i = 0;i < m_menuDrawSum + 2;++ i)
				{
					m_buttom[1 + i].disVisible();
				}
			}
		}else
		{
			action = m_actionMoveData.getNowData();
		}
		m_buttom[1].setAlpha(action * m_color.fA);
		//m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y * action);
		m_buttom[2].setAlpha(action * m_color.fA);
		m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * m_menuDrawSum * action - m_downMenuDownArea.top) * m_size.y);

		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			m_buttom[3 + i].setAlpha(action * m_color.fA);
			m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
				+ m_downMenuArea.getHeight() * i * action - m_downMenuArea.top) * m_size.y);
		}
	}
}