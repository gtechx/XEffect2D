INLINE void XTab::addObjToTab(XControlBasic *obj,const std::string &title)	//��һ����ǩ��������
{
	addObjToTab(obj,getTabIndexByTitle(title));
}
INLINE void XTab::delObjFromTab(XControlBasic *obj,const std::string &title)
{
	delObjFromTab(obj,getTabIndexByTitle(title));
}
INLINE std::string XTab::getTabStr(unsigned int index)
{
	if(index >= m_tabObjects.size()) return "";
	return m_tabObjects[index].srcTitle;
}
INLINE std::string XTab::getTabsStr()
{
	std::string ret = "";
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		ret += m_tabObjects[i].srcTitle + ";";
	}
	return ret;
}
INLINE XBool XTab::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XTab::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XTab::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XTab::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XTab::disable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XTab::enable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) m_isEnable = XTrue;
}
INLINE void XTab::setTabVisible(bool flag,const std::string &title)	//���ñ�ǩ�ɼ�
{
	setTabVisible(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabVisible(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isVisible;
}
INLINE XBool XTab::getTabVisible(const std::string &title) const
{
	return getTabVisible(getTabIndexByTitle(title));
}
INLINE void XTab::setTabEnable(bool flag,const std::string &title)
{
	setTabEnable(flag,getTabIndexByTitle(title));
}
INLINE XBool XTab::getTabEnable(unsigned int index) const
{
	if(index >= m_tabObjects.size()) return XFalse;
	return m_tabObjects[index].isEnable;
}
INLINE XBool XTab::getTabEnable(const std::string &title) const
{
	return getTabEnable(getTabIndexByTitle(title));
}
INLINE void XTab::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setColor(m_color);
		}
	}
}
INLINE void XTab::setAlpha(float a) 
{
	m_color.setA(a);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			m_tabObjects[i].objs[j]->setAlpha(a);
		}
	}
}
//INLINE void XTab::setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass)
//{
//	m_funChooseTab = funChooseTab;
//	if(pClass == NULL) m_pClass = this;
//	else m_pClass = pClass;
//}
INLINE void XTab::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
}
INLINE void XTab::startAction()
{
	if(m_withAction)
	{//�������һ����̬Ч��
		m_isInAction = XTrue;
		m_actionMoveData.set(0.0f,1.0f,0.005f);
		m_oldChooseTabIndex = m_curChooseTabIndex;	//��¼�ϵ�ѡ��
	}
}
INLINE void XTab::setVisible()	//������ʾ
{
	m_isVisible = XTrue;
	updateObjState();
	updateChildVisible();
}
INLINE void XTab::disVisible()	//���ò���ʾ
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			//m_tabObjects[i].objs[j]->disVisible();
			m_ctrlManager.setNeedDrawAndInput(false,false,m_tabObjects[i].objs[j]);
		}
	}
	updateChildVisible();
}