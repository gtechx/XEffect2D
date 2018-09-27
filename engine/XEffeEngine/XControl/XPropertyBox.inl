INLINE void XPropertyBox::setTitle(const char * title)	//���ñ���
{
	if(title == NULL) return;
	m_haveTitle = XTrue;
	m_titleStr = title;
	m_titleTxt.setString(XString::getCanShowString(title,(int)(m_width - 10.0f + MIN_FONT_CTRL_SIZE),16).c_str());
	m_titleTxt.setVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE void XPropertyBox::disTitle()					//ȡ������
{
	m_haveTitle = XFalse;
	m_titleTxt.disVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE XBool XPropertyBox::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XPropertyBox::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XPropertyBox::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	//������е������������ܻ�ȡ����������壬�������ܻ�ȡ������,����Ư��������Ŀؼ�ʱ������ܷ�ֹ���㴩͸
	for (unsigned int i = 0;i < m_propertyLines.size();++i)
	{
		if (m_propertyLines[i].canGetFocus(p)) return true;
	}
	return m_curMouseRect.isInRect(p);
}
INLINE void XPropertyBox::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�
	for (unsigned int i = 0;i < m_propertyLines.size();++i)
	{
		m_propertyLines[i].setFocus(false);
	}
	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE void XPropertyBox::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_haveTitle)
		m_titleTxt.drawUp();
	m_slider.drawUp();	
	//��������������Կ������(��δ���)
	//for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
	//{
	//	m_propertyLines[i].drawUp();
	//}
	m_ctrlManager.drawUp();
}
INLINE void XPropertyLine::draw()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
		break;
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		pNameFont->draw();	//��ʾ����
		break;
	}
}
INLINE XBool XPropertyLine::getIsFocus()
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:return XFalse;	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return pCtrl->isFocus();
	}
	return XFalse;
}
INLINE void XPropertyLine::setFocus(bool flag)
{
	switch(type)
	{
	case PROPERTY_BLANK:break;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:break;	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
		if(flag) ((XEdit *)pCtrl)->setFocus();
		else ((XEdit *)pCtrl)->setLostFocus();
		break;
	case PROPERTY_CHECK:	//ѡ����
		if(flag) ((XCheck *)pCtrl)->setFocus();
		else ((XCheck *)pCtrl)->setLostFocus();
		break;
	case PROPERTY_COMBO:	//ѡ��
		if(flag) ((XCombo *)pCtrl)->setFocus();
		else ((XCombo *)pCtrl)->setLostFocus();
		break;
	}
}
INLINE XBool XPropertyLine::canGetFocus(const XVec2& p)
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:return XFalse;	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
		return ((XEdit *)pCtrl)->canGetFocus(p);
	case PROPERTY_CHECK:	//ѡ����
		return ((XCheck *)pCtrl)->canGetFocus(p);
	case PROPERTY_COMBO:	//ѡ��
		return ((XCombo *)pCtrl)->canGetFocus(p);
	}
	return XFalse;
}
INLINE void XPropertyLine::disable()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->disable();
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->disable();
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->disable();
		break;
	}
}
INLINE void XPropertyLine::enable()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->enable();
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->enable();
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->enable();
		break;
	}
}
INLINE void XPropertyBox::setColor(const XFColor& c)
{
	m_color = c;
	m_slider.setColor(c);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setColor(c);
	}
	m_titleTxt.setColor(XFColor(0.0f,c.a));
}
INLINE void XPropertyBox::setAlpha(float a)
{
	m_color.setA(a);
	m_slider.setAlpha(a);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setAlpha(a);
	}
	m_titleTxt.setAlpha(a);
}
INLINE void XPropertyLine::setAlpha(float a)
{
	if(pNameFont != NULL) pNameFont->setAlpha(a);
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:	//�����
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setAlpha(a);
		break;
	case PROPERTY_CHECK:	//ѡ����
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setAlpha(a);
		break;
	case PROPERTY_COMBO:	//ѡ��
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setAlpha(a);
		break;
	}
}
INLINE void XPropertyLine::setColor(const XFColor& c)
{
	if(pNameFont != NULL) pNameFont->setColor(XFColor(0.0f,c.a));
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:	//�����
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setColor(c);
		break;
	case PROPERTY_CHECK:	//ѡ����
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setColor(c);
		break;
	case PROPERTY_COMBO:	//ѡ��
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setColor(c);
		break;
	}
}
INLINE const std::string& XPropertyBox::getPropertyName(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return XString::gNullStr;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:return XString::gNullStr;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return m_propertyLines[index].name;
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return m_propertyLines[index].name;
	}
	return XString::gNullStr;
}
INLINE XBool XPropertyBox::setPropertyStr(unsigned int index, const char * str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setString(str);
		return XTrue;
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyCtrlPrama(unsigned int index, int prama)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setEditType((XEdit::XEditType)prama);
		return XTrue;
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
INLINE std::string XPropertyBox::getPropertyStr(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return XString::gNullStr;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XString::gNullStr;
	case PROPERTY_EDIT:		//�����
		if (m_propertyLines[index].pCtrl == NULL) return XString::gNullStr;
		return ((XEdit *)m_propertyLines[index].pCtrl)->getString();
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return XString::gNullStr;
	}
	return XString::gNullStr;
}
INLINE XBool XPropertyBox::setPropertyIndex(unsigned int index, int order)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
		return XFalse;
	case PROPERTY_COMBO:	//ѡ��
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setCurChooseOrder(order);
		return XTrue;
	}
	return XFalse;
}
INLINE int XPropertyBox::getPropertyIndex(unsigned int index)const
{
	if (index >= m_propertyLines.size()) return -1;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
		return -1;
	case PROPERTY_COMBO:	//ѡ��
		if (m_propertyLines[index].pCtrl == NULL) return -1;
		return ((XCombo *)m_propertyLines[index].pCtrl)->getCurChooseOrder();
	}
	return -1;
}
INLINE XBool XPropertyBox::setPropertyState(unsigned int index, XBool state)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCheck *)m_propertyLines[index].pCtrl)->setState(state);
		return XTrue;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::getPropertyState(unsigned int index)const
{//�����������Ʋ��Ͻ�����Ҫ���ؿ���
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		return ((XCheck *)m_propertyLines[index].pCtrl)->getState();
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyMenuStr(unsigned int index, int menuIndex, const char *str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
		return XFalse;
	case PROPERTY_COMBO:	//ѡ��
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setMenuStr(str, menuIndex);
		return XTrue;
	}
	return XFalse;
}
INLINE XBool XPropertyBox::setPropertyMenuStr(unsigned int index, const char *str)
{
	if (index >= m_propertyLines.size()) return XFalse;
	switch (m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
		return XFalse;
	case PROPERTY_COMBO:	//ѡ��
		if (m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setMenuStr(str);
		return XTrue;
	}
	return XFalse;
}
INLINE void XPropertyBox::setPosition(const XVec2& p)
{
	m_position = p;
	m_titleTxt.setPosition(m_position + XVec2(5.0f * m_scale.x, 0.0f));
	if (m_haveTitle)
		m_slider.setPosition(m_position + XVec2(m_width, MIN_FONT_CTRL_SIZE) * m_scale);
	else
		m_slider.setPosition(m_position + XVec2(m_width * m_scale.x, 0.0f));
	//�������������������λ��
	setCurStartLineIndex(m_curStartLineIndex);
	updateRect();
}
INLINE XBool XPropertyBox::mouseProc(const XVec2& p, XMouseState mouseState)		//������궯������Ӧ����
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;
	m_slider.mouseProc(p, mouseState);
	m_ctrlManager.mouseProc(p, mouseState);
	return m_curMouseRect.isInRect(p);
}
INLINE XBool XPropertyBox::keyboardProc(int keyOrder, XKeyState keyState)			//�����Ƿ񴥷���������
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;
	m_slider.keyboardProc(keyOrder, keyState);
	m_ctrlManager.keyProc(keyOrder, keyState);
	return XTrue;
}
INLINE void XPropertyBox::update(float stepTime)
{
	m_comment.update(stepTime);
	m_slider.update(stepTime);
	m_ctrlManager.update(stepTime);
}

