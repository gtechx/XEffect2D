inline _XBool _XButtonBar::setBtnEnable(const std::string &name)		//���ð�ťʹ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->enable();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisable(const std::string &name)	//���ð�ťʧЧ
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->disable();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnCaption(const std::string &name,const std::string &caption)	//���ð�ť������
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCaptionText(caption.c_str());
	return XTrue;
}
inline _XBool _XButtonBar::setBtnCallbackFun(const std::string &name,
	void (* funInit)(void *,int),
	void (* funRelease)(void *,int),
	void (* funMouseOn)(void *,int),
	void (* funMouseDown)(void *,int),
	void (* funMouseUp)(void *,int),
	void *pClass)	//���ð�ť�Ļص�����
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setCallbackFun(funInit,funRelease,funMouseOn,
		funMouseDown,funMouseUp,pClass);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnMouseDownCB(const std::string &name,
	void (* funMouseDown)(void *,int),
	void *pClass)	//���ð�ť������ʱ�Ļص�����
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setMouseDownCB(funMouseDown,pClass);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnNormalIcon(const std::string &name,const char * filename,
	_XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(filename,resoursePosition);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnNormalIcon(const std::string &name,const _XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setNormalIcon(icon);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisableIcon(const std::string &name,const char * filename,
	_XResourcePosition resoursePosition)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(filename,resoursePosition);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnDisableIcon(const std::string &name,const _XSprite &icon)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setDisableIcon(icon);
	return XTrue;
}
inline _XBool _XButtonBar::removeBtnIcon(const std::string &name)	//ȥ�����е�icon
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->removeIcon();
	return XTrue;
}
inline _XBool _XButtonBar::setBtnIconPosition(const std::string &name,float x,float y)	//����icon��λ��
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconPosition(x,y);
	return XTrue;
}
inline _XBool _XButtonBar::setBtnIconSize(const std::string &name,float x,float y)	//����icon�����Ŵ�С
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setIconSize(x,y);
	return XTrue;
}
inline _XFColor _XButtonBar::getTextColor() const 
{
	if(m_buttons.size() <= 0) return _XFColor::white;
	else return m_buttons[0].button->getTextColor();
}
inline _XBool _XButtonBar::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XButtonBar::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline _XBool _XButtonBar::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline int _XButtonBar::getBtnID(const std::string &name)	//��ȡָ����ť��ID
{
	int index = getIndexByName(name);
	if(index < 0) return -1;
	return m_buttons[index].button->getControlID();
}
inline _XBool _XButtonBar::setBtnComment(const std::string &name,const std::string &comment)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setComment(comment.c_str());
	return XTrue;
}
inline _XBool _XButtonBar::setBtnHotKey(const std::string &name,int hotKey)
{
	int index = getIndexByName(name);
	if(index < 0) return XFalse;
	m_buttons[index].button->setHotKey(hotKey);
	return XTrue;
}
