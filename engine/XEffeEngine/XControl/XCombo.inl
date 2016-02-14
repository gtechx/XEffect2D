INLINE void XComboSkin::releaseTex()
{
	XMem::XDELETE(comboInputNormal);
	XMem::XDELETE(comboInputDisable);
	
	downButtonTex.release();
	downMenuUpButtonTex.release();
	downMenuButtonTex.release();
	downMenuDownButtonTex.release();
}
INLINE void XComboSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE XBool XCombo::initEx(const XComboSkin &tex,	//����ӿڵļ򻯰汾
		const XVector2& position,		
		int menuSum,					
		int drawMenuSum,				
		const XFontUnicode &font,		
		float fontSize)
{
	return init(tex,position,tex.m_mouseRect,tex.downButtonTex.m_mouseRect,
		tex.downMenuUpButtonTex.m_mouseRect,tex.downMenuButtonTex.m_mouseRect,
		tex.downMenuDownButtonTex.m_mouseRect,menuSum,drawMenuSum,font,fontSize);
}
INLINE XBool XCombo::initPlus(const char * path,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize,
		XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;				//��ֹ�ظ���ʼ��
	if(path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCOMBO_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(*(XComboSkin *)m_resInfo->m_pointer,XVector2::zero,menuSum,drawMenuSum,font,fontSize);
}
INLINE XBool XCombo::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE XBool XCombo::canLostFocus(float,float)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XTrue;		//�����Ч��ֱ���˳�

	if(m_isDrawDownMenu) return XFalse;
	return XTrue;
}
INLINE void XCombo::setColor(float r,float g,float b,float a) 	//���ð�ť����ɫ
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
INLINE void XCombo::setAlpha(float a) 
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
INLINE XBool XCombo::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE void XCombo::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setTextColor(m_textColor);
	}
}	//�����������ɫ
INLINE XBool XCombo::setCurChooseOrder(int index)	//���õ�ǰѡ����
{
	if(index < 0 || index >= m_menuSum) return XFalse;
	if(m_curChooseOrder == index) return XTrue;
	m_curChooseOrder = index;
	updateString();
	return XTrue;
}
INLINE void XCombo::disable()					//ʹ��Ч
{
	m_isEnable = XFalse;
//	m_isBeChoose = XFalse;
}
INLINE void XCombo::enable()					//ʹ��
{
	m_isEnable = XTrue;
}
INLINE std::string XCombo::getCurChooseMenu() 	//��ȡ��ǰѡ��Ĳ˵�������
{
	if(m_curChooseOrder < 0) return "";
	if(m_curChooseOrder == 0)
	{
		int len = XString::getCharPosition(m_menuData.c_str(),'\n');
		if(len < 0) return m_menuData;
		return m_menuData.substr(0,len);
	//	m_menuData[len] = '\0';
	//	std::string ret = m_menuData;
	//	m_menuData[len] = '\n';
	//	return ret;
	}else
	{
		int s = XString::getCharPositionEx(m_menuData.c_str(),'\n',m_curChooseOrder);
		if(s < 0) return "";
		int e = XString::getCharPositionEx(m_menuData.c_str(),'\n',m_curChooseOrder + 1);
		if(e < 0) return "";
		return m_menuData.substr(s + 1,e - s - 1);
	//	m_menuData[e] = '\0';
	//	std::string ret = m_menuData.substr(s + 1,(int)(m_menuData.size()) - s - 1);
	//	m_menuData[e] = '\n';
	//	return ret;
	}
}