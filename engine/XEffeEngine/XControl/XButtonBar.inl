INLINE XBool XButtonBar::setBtnEnable(const std::string& name)		//���ð�ťʹ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->enable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisable(const std::string& name)	//���ð�ťʧЧ
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->disable();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnCaption(const std::string& name,const std::string& caption)	//���ð�ť������
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCaptionText(caption.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setEventProc(const std::string& name,
		void (* eventProc)(void *,int,int),void *pClass)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setEventProc(eventProc,pClass);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string& name,const char * filename,
	XResPos resPos)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(filename,resPos);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnNormalIcon(const std::string& name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string& name,const char * filename,
	XResPos resPos)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(filename,resPos);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnDisableIcon(const std::string& name,const XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(icon);
	return XTrue;
}
INLINE XBool XButtonBar::removeBtnIcon(const std::string& name)	//ȥ�����е�icon
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->removeIcon();
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconPosition(const std::string& name, const XVec2& p)	//����icon��λ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconPosition(p);
	return XTrue;
}
INLINE XBool XButtonBar::setBtnIconSize(const std::string& name, const XVec2& s)	//����icon�����Ŵ�С
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconSize(s);
	return XTrue;
}
INLINE const XFColor& XButtonBar::getTextColor() const 
{
	if(m_buttons.size() <= 0) return XFColor::white;
	else return m_buttons[0].button->getTextColor();
}
INLINE XBool XButtonBar::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XButtonBar::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XButtonBar::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE int XButtonBar::getBtnID(const std::string& name)	//��ȡָ����ť��ID
{
	int index = getIndexByName(name);
	if(index < 0) return -1;
	return m_buttons[index].button->getControlID();
}
INLINE XBool XButtonBar::setBtnComment(const std::string& name,const std::string& comment)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setComment(comment.c_str());
	return XTrue;
}
INLINE XBool XButtonBar::setBtnHotKey(const std::string& name,int hotKey)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setHotKey(hotKey);
	return XTrue;
}
