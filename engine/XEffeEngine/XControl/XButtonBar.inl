INLINE XBool XButtonBar::setBtnEnable(const std::string &name)		//���ð�ťʹ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->enable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisable(const std::string &name)	//���ð�ťʧЧ
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->disable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnCaption(const std::string &name,const std::string &caption)	//���ð�ť������
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCaptionText(caption.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setEventProc(const std::string &name,
		void (* eventProc)(void *,int,int),void *pClass)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setEventProc(eventProc,pClass);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string &name,const char * filename,
	XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(filename,resoursePosition);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string &name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string &name,const char * filename,
	XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(filename,resoursePosition);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string &name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::removeBtnIcon(const std::string &name)	//ȥ�����е�icon
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->removeIcon();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconPosition(const std::string &name,float x,float y)	//����icon��λ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconPosition(x,y);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconSize(const std::string &name,float x,float y)	//����icon�����Ŵ�С
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconSize(x,y);
	return XTrue;
}
INLINE XFColor XButtonBar::getTextColor() const 
{
	if(m_buttons.size() <= 0) return XFColor::white;
	else return m_buttons[0].button->getTextColor();
}
INLINE XBool XButtonBar::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XButtonBar::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XButtonBar::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE int XButtonBar::getBtnID(const std::string &name)	//��ȡָ����ť��ID
{
	int index = getIndexByName(name);
	if(index < 0) return -1;
	return m_buttons[index].button->getControlID();
}
INLINE XBool XButtonBar::setBtnComment(const std::string &name,const std::string &comment)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setComment(comment.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setBtnHotKey(const std::string &name,int hotKey)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setHotKey(hotKey);
	return XTrue;
}
