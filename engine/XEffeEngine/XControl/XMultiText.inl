INLINE void XMultiText::enable()		//ʹ����Ч
{
	m_isEnable = XTrue;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}
INLINE void XMultiText::disable()		//ʹ����Ч
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}
INLINE int XMultiText::getSelectLine() const			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
{
	if(!m_isInited ||
		!m_haveSelect) return -1;
	else return m_selectLineOrder;
}
INLINE int XMultiText::getLineSum() const
{
	return m_curMaxLineSum;
}
INLINE XBool XMultiText::initEx(const XVector2& position,	//������ӿڵļ�
	const XMultiTextSkin& tex,	
	const char *str,const XFontUnicode& font, float strSize,
	const XSlider &vSlider,const XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,vSlider,hSlider);
}
//INLINE void XMultiText::setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass)
//{
//	m_funSelect = funSelect;
//	m_pClass = pClass;
//}
INLINE XBool XMultiText::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XMultiText::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(m_textColor);
	}
}	//�����������ɫ
INLINE void XMultiText::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_spriteBackGround.setColor(m_color);
	XFColor tmpColor = m_textColor * m_color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(tmpColor);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}	//���ð�ť����ɫ
INLINE void XMultiText::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	XFColor tmpColor = m_textColor * m_color;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setColor(tmpColor);
	}
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
}
INLINE XBool XMultiText::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	//return m_curMouseRect.isInRect(x,y);
	return XMath::getIsInRect(XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XVector2 XMultiText::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0: return XVector2(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);
	case 1: return XVector2(m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);
	case 2: return XVector2(m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	case 3: return XVector2(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y); 
	}
	return XVector2::zero;
}
INLINE XBool XMultiText::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
INLINE void XMultiText::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
INLINE void XMultiText::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_verticalSlider.drawUp();
	m_horizontalSlider.drawUp();
}
INLINE void XMultiText::update(float stepTime)
{
	m_verticalSlider.update(stepTime);
	m_horizontalSlider.update(stepTime);
	if(!m_actionMD.getIsEnd())
	{
		m_actionMD.move(stepTime);
	}
}
INLINE void XMultiText::setAction(XMultiTextActionType type,int index)
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