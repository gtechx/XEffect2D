inline void _XTab::addObjToTab(_XControlBasic *obj,const std::string &title)	//��һ����ǩ��������
{
	addObjToTab(obj,getTabIndexByTitle(title));
}
inline std::string _XTab::getTabStr(int index)
{
	if(index < 0 || index >= m_tabObjects.size()) return "";
	return m_tabObjects[index].srcTitle;
}
inline std::string _XTab::getTabsStr()
{
	std::string ret = "";
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		ret += m_tabObjects[i].srcTitle + ";";
	}
	return ret;
}
inline _XBool _XTab::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
#if TAB_MODE == 1
	//return isInRect(x,y);
	return m_titleRect.isInRect(x,y);
#else
	return m_titleRect.isInRect(x,y);
#endif
}
inline void _XTab::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
inline _XBool _XTab::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XTab::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline void _XTab::disable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XTab::enable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) m_isEnable = XTrue;
}
inline void _XTab::setTabVisible(bool flag,const std::string &title)	//���ñ�ǩ�ɼ�
{
	setTabVisible(flag,getTabIndexByTitle(title));
}
inline _XBool _XTab::getTabVisible(int index) const
{
	if(index < 0 || index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isVisible;
}
inline _XBool _XTab::getTabVisible(const std::string &title) const
{
	return getTabVisible(getTabIndexByTitle(title));
}
inline void _XTab::setTabEnable(bool flag,const std::string &title)
{
	setTabEnable(flag,getTabIndexByTitle(title));
}
inline _XBool _XTab::getTabEnable(int index) const
{
	if(index < 0 || index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isEnable;
}
inline _XBool _XTab::getTabEnable(const std::string &title) const
{
	return getTabEnable(getTabIndexByTitle(title));
}
inline void _XTab::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setColor(m_color);
		}
	}
}
inline void _XTab::setAlpha(float a) 
{
	m_color.setA(a);
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setAlpha(a);
		}
	}
}
inline void _XTab::setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass)
{
	m_funChooseTab = funChooseTab;
	if(pClass == NULL) m_pClass = this;
	else m_pClass = pClass;
}
inline void _XTab::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
#if TAB_MODE == 1
	//��ʾ��������
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->drawUp();
		}
	}
#endif
}
inline void _XTab::startAction()
{
	if(m_withAction)
	{//�������һ����̬Ч��
		m_isInAction = XTrue;
		m_actionMoveData.set(0.0f,1.0f,0.005f);
		m_oldChooseTabIndex = m_nowChooseTabIndex;	//��¼�ϵ�ѡ��
	}
}
inline void _XTab::setVisible()	//������ʾ
{
	m_isVisible = XTrue;
	updateObjState();
	updateChildVisible();
}
inline void _XTab::disVisible()	//���ò���ʾ
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	for(int i = 0;i < m_tabObjects.size();++ i)
	{
		for(int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->disVisible();
		}
	}
	updateChildVisible();
}