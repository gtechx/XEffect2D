INLINE XMenuItem *XMenuGroup::getChooseItem()	//��ȡ��ǰ���б�ѡ��Ĳ˵���
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		return m_items[m_chooseIndex];
	return NULL;
}
INLINE void XMenuGroup::draw()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->draw();
	}
}
INLINE void XMenuGroup::setOpen()	//���ô�
{//���г�Ա�ɼ�
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XTrue;
	}
}
INLINE void XMenuGroup::disOpen()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XFalse;
		m_items[i]->disOpen();
	}
	resetChoose();
}
INLINE XBool XMenuGroup::isInRect(const XVec2& p)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->isInRect(p)) return XTrue;
	}
	return XFalse;
}
INLINE XBool XMenuGroup::mouseProc(const XVec2& p,XMouseState mouseState)
{
	XBool ret = XFalse;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->mouseProc(p,mouseState)) ret = XTrue;
	}
	return ret;
}
INLINE void XMenuGroup::postPross()
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->postPross();
	}
}
INLINE void XMenuGroup::openChoose()	//��ѡ�����
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		m_items[m_chooseIndex]->setOpen();
}
INLINE void XMenuGroup::closeChoose()	//�ر�ѡ�����
{
	if(m_chooseIndex >= 0 && m_chooseIndex < (int)(m_items.size()))
		m_items[m_chooseIndex]->disOpen();
}
INLINE void XMenuGroup::resetChoose()
{
	m_chooseIndex = -1;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->resetChoose();
	}
}
INLINE void XMenuGroup::setHotChooseActive(XBool flag)
{
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setHotChooseActive(flag);
	}
}
INLINE XMenuItem * XMenuGroup::getItem(const XVec2& p)
{
	XMenuItem * tmp = NULL;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if((tmp = m_items[i]->getItem(p)) != NULL) return tmp;
	}
	return NULL;
}
INLINE XMenuItem * XMenuGroup::getItemByNameInGroup(const std::string& name)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->m_originalName == name) return m_items[i];
	}
	return NULL;
}
INLINE void XMenuItem::setCheckState(XBool state)	//����ѡ����˵����ѡ��״̬
{
	if(m_isChecked == state ||
		m_type != MENU_ITEM_TYPE_CHK) return;
	m_isChecked = state;
	if(m_isChecked) m_showName[0] = 'X';
	else m_showName[0] = '*';
	m_font.setString(m_showName.c_str());
}
INLINE void XMenuItem::postPross()
{
	if(m_child != NULL) m_child->postPross();
	//���������Ĳ˵�����п�ݼ�
	if(m_levelIndex < 2 || m_type == MENU_ITEM_TYPE_DLT || m_child != NULL) m_hotKey = -1;

	if(m_levelIndex == 0) return;	//���㲻��Ҫ����
	updateStr();
}
INLINE XBool XMenuItem::setHotKey(char k)	//-1Ϊ��Ч
{//���ÿ�ݼ�
	//һ���˵�����Ψһ
	m_hotKey = k;
	return XTrue;
}
INLINE XMenuItem *XMenuItem::getItemByHotKey(char k)
{//ͨ����ݼ������Ҳ˵���
	if(k < 0) return NULL;
	if(m_hotKey == k) return this;
	if(m_child != NULL) return m_child->getItemByHotKey(k);
	return NULL;
}
INLINE XMenuItem *XMenuItem::getItemByHotChooseKey(char k)//���Ӳ˵���Ѱ��ָ�����ȼ��Ĳ˵���
{
	if(m_child == NULL) return NULL;
	return m_child->getItemByHotChooseKey(k);
}
INLINE void XMenuItem::setCallbackFun(void (*callbackFun)(void *,const std::string& ),void *p)	//���ûص����������˵������ʱ�������صĻص�����
{
	if(m_type == MENU_ITEM_TYPE_DLT) return; //�ָ���û�лص�
	m_callbackFun = callbackFun;
	m_pClass = p;
}
INLINE void XMenuItem::setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p)
{
	if(m_type == MENU_ITEM_TYPE_DLT) return; //�ָ���û�лص�
	if(m_child == NULL)
	{
		m_callbackFun = callbackFun;
		m_pClass = p;
	}else
	{
		m_child->setAllCallBackFun(callbackFun,p);
	}
}
INLINE void XMenuItem::setOpen()	//���ô�
{
	if(!m_isEnable || m_isOpen) return;	//��Ч��������ܴ�
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XTrue;
	if(m_child != NULL) m_child->setOpen();
}
INLINE void XMenuItem::disOpen()	//���ùر�
{
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XFalse;
	if(m_child != NULL) m_child->disOpen();
}
INLINE XBool XMenuItem::isInRect(const XVec2& p)
{
	if(XRect(m_position,m_position + m_size * m_scaleSize).isInRect(p)) return XTrue;
	if(m_child != NULL && m_isOpen) return m_child->isInRect(p);
	return XFalse;
}
INLINE XMenuItem * XMenuItem::getItem(const XVec2& p)
{
	if(XRect(m_position,m_position + m_size * m_scaleSize).isInRect(p)) return this;
	if(m_child != NULL && m_isOpen) return m_child->getItem(p);
	return NULL;
}
INLINE void XMenuItem::resetChoose()
{
	m_isMouseOn = XFalse;
	if(m_child != NULL) m_child->resetChoose();
}
INLINE XBool XMenuItem::setHotChooseKey(char k)	//-1Ϊ��Ч
{//���ÿ�ѡ��
	if(k >= 'a' && k <= 'z') k += 'A' - 'a';	//�����ǿ�����ʾ����ĸ
	if(k < 'A' || k > 'Z') return XFalse;
	//����Ψһ
	if(m_owner != NULL && m_owner->getItemByHotChooseKey(k) != NULL) return XFalse;
	m_hotChooseKey = k;
	return XTrue;
}
INLINE void XMenuItem::disIcon()
{
	if(m_withIcon)
	{
		m_spriteIcon.release();
		m_withIcon = XFalse;
	}
}
INLINE void XMenuItem::setHotChooseActive(XBool flag)
{//�����ȼ�����״̬
	if(m_child != NULL) m_child->setHotChooseActive(flag);
	m_isHotChooseActive = flag;
	if(m_hotChooseKey >= 0) updateStr();
}
INLINE XBool XMenu::getCheckState(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	return tmpX->m_isChecked;
}
INLINE XBool XMenu::setCheckState(const std::string& pos,XBool state)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	tmpX->setCheckState(state);
	return XTrue;
}
INLINE XBool XMenu::setDisable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XFalse;
	return XTrue;
}
INLINE XBool XMenu::setEnable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XTrue;
	return XTrue;
}
INLINE XBool XMenu::getEnable(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->m_isEnable;
}
INLINE XBool XMenu::setItemIcon(const std::string& pos,const char *filename,XResPos res)
{
	if(filename == NULL) return XFalse;
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->setIcon(filename,res);
}
INLINE XBool XMenu::setHotKey(const std::string& pos,char k)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(k >= 0 && m_rootItem.getItemByHotKey(k) != NULL) return XFalse;//������Ψһ�� 
	m_needPostProcess = XTrue;
	return tmpX->setHotKey(k);
}
INLINE char XMenu::getHotKey(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotKey;
}
INLINE XBool XMenu::setHotChooseKey(const std::string& pos,char k)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(tmpX->setHotChooseKey(k))
	{
		m_needPostProcess = XTrue;
		return XTrue;
	}
	return XFalse;
}
INLINE char XMenu::getHotChooseKey(const std::string& pos)
{
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotChooseKey;
}
INLINE void XMenu::postProcess()
{
	if(!m_needPostProcess) return;
	m_rootItem.postPross();
	m_needPostProcess = XFalse;
}
//INLINE XBool XMenu::setCBFunction(const std::string& pos,void (*callbackFun)(void *,const std::string& ),void *p)
//{//���ò˵���Ӧ�Ļص�����
//	XMenuItem * tmp = getItemByName(pos);
//	if(tmp == NULL) return XFalse;
//	tmp->setCallbackFun(callbackFun,p);
//	return XTrue;
//}
INLINE void XMenu::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	postProcess();
	m_rootItem.draw();
}
INLINE const std::string XMenu::getItemFullStr(const XMenuItem * it)
{
	if(it == NULL) return XString::gNullStr;
	return it->getFullStr();
}
INLINE void XMenu::setPosition(const XVec2& p)
{
	m_position = p;
	m_rootItem.setPosition(m_position);
}
INLINE void XMenu::setScale(const XVec2& s)
{
	m_scale = s;
	m_rootItem.setScale(m_scale);
	m_rootItem.setPosition(m_position);
}
INLINE void XMenu::setColor(const XFColor& c)
{
	m_color = c;
	m_font.setColor(m_color);
	m_rootItem.setColor(m_color);
}
INLINE void XMenu::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	m_rootItem.setColor(m_color);
}
INLINE XVec2 XMenu::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{//���ﲢû��ʵ�ʵ�����
	if(!m_isInited || 
		m_rootItem.m_child == NULL)	return XVec2::zero;
	XMenuItem * it = m_rootItem.m_child->m_items[0];	//�����õ�һ���˵���ķ�Χ�������ϲ�����
	if(it == NULL)	return XVec2::zero;
	switch(order)
	{
	case 0:return it->m_position;
	case 1:return XVec2(it->m_position.x + it->m_size.x * it->m_scaleSize.x,it->m_position.y);
	case 2:return it->m_position + it->m_size * it->m_scaleSize;
	case 3:return XVec2(it->m_position.x,it->m_position.y + it->m_size.y * it->m_scaleSize.y);
	}
	return XVec2::zero;
}
INLINE XBool XMenu::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XMenu::setLostFocus()	//����ʧȥ����
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
	//�˵�ʧȥ����֮����Ҫ���˵��ر�
	closeAll();
	m_oldChooseItem = NULL;
	m_isClicked = XFalse;
}