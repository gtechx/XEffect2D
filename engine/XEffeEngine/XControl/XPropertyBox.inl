INLINE void XPropertyBox::setTitle(const char * title)	//���ñ���
{
	if(title == NULL) return;
	m_haveTitle = XTrue;
	m_titleStr = title;
	m_titleTxt.setString(XString::getCanShowString(title,(int)(m_width - 10.0f + MIN_FONT_CTRL_SIZE),16).c_str());
	m_titleTxt.setVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE void XPropertyBox::disTitle()					//ȡ������
{
	m_haveTitle = XFalse;
	m_titleTxt.disVisible();
	updateRect();
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
}
INLINE XBool XPropertyBox::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XPropertyBox::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XPropertyBox::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(x,y);
}
INLINE void XPropertyBox::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�
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
	//����������ʾ���е�������
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].drawUp();
	}
}
INLINE void XPropertyLine::draw()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
		break;
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		pNameFont->draw();	//��ʾ����
		break;
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		pNameFont->draw();	//��ʾ����
		pCtrl->draw();
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
INLINE XBool XPropertyLine::canLostFocus(float x,float y)
{
	switch(type)
	{
	case PROPERTY_BLANK:return XTrue;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:return XTrue;	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
		return ((XEdit *)pCtrl)->canLostFocus(x,y);
	case PROPERTY_CHECK:	//ѡ����
		return ((XCheck *)pCtrl)->canLostFocus(x,y);
	case PROPERTY_COMBO:	//ѡ��
		return ((XCombo *)pCtrl)->canLostFocus(x,y);
	}
	return XTrue;
}
INLINE XBool XPropertyLine::canGetFocus()
{
	switch(type)
	{
	case PROPERTY_BLANK:return XFalse;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:return XFalse;	//ֻ��ʾ���ֵ���
	case PROPERTY_EDIT:		//�����
		return ((XEdit *)pCtrl)->getVisible() && ((XEdit *)pCtrl)->getEnable();
	case PROPERTY_CHECK:	//ѡ����
		return ((XCheck *)pCtrl)->getVisible() && ((XCheck *)pCtrl)->getEnable();
	case PROPERTY_COMBO:	//ѡ��
		return ((XCombo *)pCtrl)->getVisible() && ((XCombo *)pCtrl)->getEnable();
	}
	return XFalse;
}
INLINE void XPropertyLine::drawUp()
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		pCtrl->drawUp();
		break;
	}
}
INLINE XBool XPropertyLine::mouseProc(float x,float y,XMouseState mouseState)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->mouseProc(x,y,mouseState);
		break;
	}
	return XTrue;
}
INLINE XBool XPropertyLine::keyboardProc(int keyOrder,XKeyState keyState)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->keyboardProc(keyOrder,keyState);
		break;
	}
	return XTrue;
}
INLINE void XPropertyLine::update(float stepTime)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->update(stepTime);
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->update(stepTime);
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->update(stepTime);
		break;
	}
}
INLINE void XPropertyLine::insertChar(const char *ch,int len)
{
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:		//�����
		((XEdit *)pCtrl)->insertChar(ch,len);
		break;
	case PROPERTY_CHECK:	//ѡ����
		((XCheck *)pCtrl)->insertChar(ch,len);
		break;
	case PROPERTY_COMBO:	//ѡ��
		((XCombo *)pCtrl)->insertChar(ch,len);
		break;
	}
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
INLINE void XPropertyBox::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_slider.setColor(r,g,b,a);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].setColor(r,g,b,a);
	}
	m_titleTxt.setColor(0.0f,0.0f,0.0f,a);
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
		break;
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
INLINE void XPropertyLine::setColor(float r,float g,float b,float a)
{
	if(pNameFont != NULL) pNameFont->setColor(0.0f,0.0f,0.0f,a);
	switch(type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
		break;
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		break;
	case PROPERTY_EDIT:	//�����
		if(pCtrl != NULL) ((XEdit *)pCtrl)->setColor(r,g,b,a);
		break;
	case PROPERTY_CHECK:	//ѡ����
		if(pCtrl != NULL) ((XCheck *)pCtrl)->setColor(r,g,b,a);
		break;
	case PROPERTY_COMBO:	//ѡ��
		if(pCtrl != NULL) ((XCombo *)pCtrl)->setColor(r,g,b,a);
		break;
	}
}