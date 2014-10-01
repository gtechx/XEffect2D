inline void _XMultiText::enable()		//ʹ����Ч
{
	m_isEnable = XTrue;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}
inline void _XMultiText::disable()		//ʹ����Ч
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}
inline int _XMultiText::getSelectLine() const			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
{
	if(!m_isInited ||
		!m_haveSelect) return -1;
	else return m_selectLineOrder;
}
inline int _XMultiText::getLineSum() const
{
	return m_nowMaxLineSum;
}
inline _XBool _XMultiText::initEx(const _XVector2& position,	//������ӿڵļ�
	const _XMultiTextTexture& tex,	
	const char *str,const _XFontUnicode& font, float strSize,
	const _XSlider &vSlider,const _XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,vSlider,hSlider);
}
inline void _XMultiText::setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass)
{
	m_funSelect = funSelect;
	m_pClass = pClass;
}
inline _XBool _XMultiText::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline void _XMultiText::setTextColor(const _XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor);
	}
}	//�����������ɫ
inline void _XMultiText::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_spriteBackGround.setColor(m_color);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor * m_color);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}	//���ð�ť����ɫ
inline void _XMultiText::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setColor(m_textColor * m_color);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}
inline void _XMultiText::addString(const char *str)			//���ַ���������ַ���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		str == NULL ||
		strlen(str) == 0 ||
		strlen(str) + strlen(m_nowString) >= MAX_STRING_LENGTH) return;
	strcat(m_nowString,str);
	checkStr();
	updateLineStr();
}
inline _XBool _XMultiText::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	//return m_nowMouseRect.isInRect(x,y);
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XMultiText::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y);
	case 1: return _XVector2(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y);
	case 2: return _XVector2(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	case 3: return _XVector2(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); 
	}
	return _XVector2::zero;
}
inline _XBool _XMultiText::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
inline void _XMultiText::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
inline void _XMultiText::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_verticalSlider.drawUp();
	m_horizontalSlider.drawUp();
}
inline void _XMultiText::update(int stepTime)
{
	m_verticalSlider.update(stepTime);
	m_horizontalSlider.update(stepTime);
	if(!m_actionMD.getIsEnd())
	{
		m_actionMD.move(stepTime);
	}
}
inline void _XMultiText::setAction(_XMultiTextActionType type,int index)
{
	switch(type)
	{
	case MLTTXT_ACTION_TYPE_IN:		//ѡ�����
		m_actionMD.set(0.0f,1.0f,0.005f,MOVE_DATA_MODE_SHAKE);
		break;
	case MLTTXT_ACTION_TYPE_MOVE:	//ѡ���ƶ�
		m_actionMD.set(0.0f,1.0f,0.005f);
		break;
	case MLTTXT_ACTION_TYPE_DCLICK:	//˫��
		m_actionMD.set(1.0f,1.2f,0.01f,MOVE_DATA_MODE_SIN_MULT,1);
		break;
	case MLTTXT_ACTION_TYPE_OUT:		//ȡ��ѡ��
		m_actionMD.set(1.0f,0.0f,0.005f);
		break;
	}
	m_actionType = type;
	m_actionPosition = index;
}