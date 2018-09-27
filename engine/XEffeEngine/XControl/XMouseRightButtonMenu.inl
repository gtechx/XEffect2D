INLINE XBool XMouseRightButtonMenu::initEx(int menuSum,	//������ӿڵļ�
	const XVec2& position,	
	const XMouseRightButtonMenuSkin &tex,	
	const XFontUnicode& font,float captionSize)
{
	return init(menuSum,position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition);
}
INLINE XBool XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//�˵��е��������
	const XFontUnicode& font,float captionSize,
	XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXBUTTON_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(menuSum,XVec2::zero,*(XButtonSkin *)m_resInfo->m_pointer,font,captionSize);
}
INLINE void XMouseRightButtonMenu::draw()
{
	if(!m_isInited ||		//û�г�ʼ��ʱ����ʾ
		!m_isVisible) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
INLINE void XMouseRightButtonMenu::update(float stepTime)
{
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].update(stepTime);
	}
	if(m_isInAction)
	{//����Թ�����������(��δ���)
		m_actionMoveData.move(stepTime);
		float action = m_actionMoveData.getCurData();
		if(m_actionMoveData.getIsEnd()) 
		{//�������
			m_isInAction = false;
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setWithAction(XTrue);
			}
			action = 1.0f;	//������Ҫ�ָ�
			if(!m_isDraw)
				m_isVisible = XFalse;
		}
		if(m_isDraw)
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setAlpha(action * m_color.a);
				m_menu[i].setPosition(m_position.x + (100.0f + i * 50.0f) * m_scale.y * (1.0f - action),
					m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				m_menu[i].setAlpha((1.0f - action) * m_color.a);
				m_menu[i].setPosition(m_position.x - (100.0f + i * 50.0f) * m_scale.y * action,
					m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
			}
		}
	}
}
INLINE void XMouseRightButtonMenu::drawUp()
{
	if(!m_isInited ||		//û�г�ʼ��ʱ����ʾ
		!m_isVisible) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].drawUp();
	}
}
INLINE XBool XMouseRightButtonMenu::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE XBool XMouseRightButtonMenu::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_allArea.isInRect(p);
}
INLINE XVec2 XMouseRightButtonMenu::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_allArea.getLT();
	case 1:return m_allArea.getRT();
	case 2:return m_allArea.getRB();
	case 3:return m_allArea.getLB();
	}
	return XVec2::zero;
}
INLINE int XMouseRightButtonMenu::getLastChoose() const										//��������ѡ���ֵ
{
	return m_lastChoose;
}
//INLINE void XMouseRightButtonMenu::setCallbackFun(void (* funChooseChange)(void *,int),void (* funChooseOver)(void *,int),void *pClass)	//���ò˵��Ļص�����
//{
//	m_funChooseChange = funChooseChange;
//	m_funChooseOver = funChooseOver;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}
INLINE void XMouseRightButtonMenu::setHotKey(int hotKey,int order)							//���ò˵���ĳһ����ȼ�
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setHotKey(hotKey);
}
INLINE void XMouseRightButtonMenu::setText(const char *temp,int order)								//�ı�˵���ĳһ���ֵ
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setCaptionText(temp);
}
INLINE void XMouseRightButtonMenu::setTexture(const XMouseRightButtonMenuSkin &tex,int order)	//�ı�˵���ĳһ�����ͼ
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setTexture(tex);
}
INLINE void XMouseRightButtonMenu::setColor(const XFColor& c)
{
	m_color = c;
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setColor(m_color);
	}
	updateChildColor();
}
INLINE void XMouseRightButtonMenu::setAlpha(float a)
{
	m_color.setA(a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setAlpha(a);
	}
	updateChildAlpha();
}
