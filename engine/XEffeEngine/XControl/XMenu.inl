inline _XMenuItem *_XMenuGroup::getChooseItem()	//��ȡ��ǰ���б�ѡ��Ĳ˵���
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		return m_items[m_chooseIndex];
	return NULL;
}
inline void _XMenuGroup::draw()
{
	for(int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->draw();
	}
}
inline void _XMenuGroup::setOpen()	//���ô�
{//���г�Ա�ɼ�
	for(int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XTrue;
	}
}
inline void _XMenuGroup::disOpen()
{
	for(int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->m_isVisible = XFalse;
		m_items[i]->disOpen();
	}
	resetChoose();
}
inline _XBool _XMenuGroup::isInRect(float x,float y)
{
	for(int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->isInRect(x,y)) return XTrue;
	}
	return XFalse;
}
inline _XBool _XMenuGroup::mouseProc(float x,float y,_XMouseState mouseState)
{
	_XBool ret = XFalse;
	for(int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->mouseProc(x,y,mouseState)) ret = XTrue;
	}
	return ret;
}
inline void _XMenuGroup::postPross()
{
	for(int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->postPross();
	}
}
inline void _XMenuGroup::openChoose()	//��ѡ�����
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->setOpen();
}
inline void _XMenuGroup::closeChoose()	//�ر�ѡ�����
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->disOpen();
}
inline void _XMenuGroup::resetChoose()
{
	m_chooseIndex = -1;
	for(int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->resetChoose();
	}
}
inline void _XMenuGroup::setHotChooseActive(_XBool flag)
{
	for(int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setHotChooseActive(flag);
	}
}
inline _XMenuItem * _XMenuGroup::getItem(float x,float y)
{
	_XMenuItem * tmp = NULL;
	for(int i = 0;i < m_items.size();++ i)
	{
		if((tmp = m_items[i]->getItem(x,y)) != NULL) return tmp;
	}
	return NULL;
}
inline _XMenuItem * _XMenuGroup::getItemByNameInGroup(const std::string &name)
{
	for(int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->m_originalName == name) return m_items[i];
	}
	return NULL;
}
inline void _XMenuItem::setCheckState(_XBool state)	//����ѡ����˵����ѡ��״̬
{
	if(m_isChecked == state ||
		m_type != MENU_ITEM_TYPE_CHK) return;
	m_isChecked = state;
	if(m_isChecked) m_showName[0] = 'X';
	else m_showName[0] = '*';
	m_font.setString(m_showName.c_str());
}
inline void _XMenuItem::postPross()
{
	if(m_child != NULL) m_child->postPross();
	//���������Ĳ˵�����п�ݼ�
	if(m_levelIndex < 2 || m_type == MENU_ITEM_TYPE_DLT || m_child != NULL) m_hotKey = -1;

	if(m_levelIndex == 0) return;	//���㲻��Ҫ����
	updateStr();
}
inline _XBool _XMenuItem::setHotKey(char k)	//-1Ϊ��Ч
{//���ÿ�ݼ�
	//һ���˵�����Ψһ
	m_hotKey = k;
	return XTrue;
}
inline _XMenuItem *_XMenuItem::getItemByHotKey(char k)
{//ͨ����ݼ������Ҳ˵���
	if(k < 0) return NULL;
	if(m_hotKey == k) return this;
	if(m_child != NULL) return m_child->getItemByHotKey(k);
	return NULL;
}
inline _XMenuItem *_XMenuItem::getItemByHotChooseKey(char k)//���Ӳ˵���Ѱ��ָ�����ȼ��Ĳ˵���
{
	if(m_child == NULL) return NULL;
	return m_child->getItemByHotChooseKey(k);
}
inline void _XMenuItem::setCallbackFun(void (*callbackFun)(void *,const std::string &),void *p)	//���ûص����������˵������ʱ�������صĻص�����
{
	if(m_type == MENU_ITEM_TYPE_DLT) return; //�ָ���û�лص�
	m_callbackFun = callbackFun;
	m_pClass = p;
}
inline void _XMenuItem::setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p)
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
inline void _XMenuItem::setOpen()	//���ô�
{
	if(!m_isEnable || m_isOpen) return;	//��Ч��������ܴ�
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XTrue;
	if(m_child != NULL) m_child->setOpen();
}
inline void _XMenuItem::disOpen()	//���ùر�
{
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	m_isOpen = XFalse;
	if(m_child != NULL) m_child->disOpen();
}
inline _XBool _XMenuItem::isInRect(float x,float y)
{
	if(_XRect(m_position.x,m_position.y,m_position.x + m_size.x * m_scaleSize.x,
		m_position.y + m_size.y * m_scaleSize.y).isInRect(x,y)) return XTrue;
	if(m_child != NULL && m_isOpen) return m_child->isInRect(x,y);
	return XFalse;
}
inline _XMenuItem * _XMenuItem::getItem(float x,float y)
{
	if(_XRect(m_position.x,m_position.y,m_position.x + m_size.x * m_scaleSize.x,
		m_position.y + m_size.y * m_scaleSize.y).isInRect(x,y)) return this;
	if(m_child != NULL && m_isOpen) return m_child->getItem(x,y);
	return NULL;
}
inline void _XMenuItem::resetChoose()
{
	m_isMouseOn = XFalse;
	if(m_child != NULL) m_child->resetChoose();
}
inline _XBool _XMenuItem::setHotChooseKey(char k)	//-1Ϊ��Ч
{//���ÿ�ѡ��
	if(k >= 'a' && k <= 'z') k += 'A' - 'a';	//�����ǿ�����ʾ����ĸ
	if(k < 'A' || k > 'Z') return XFalse;
	//����Ψһ
	if(m_owner != NULL && m_owner->getItemByHotChooseKey(k) != NULL) return XFalse;
	m_hotChooseKey = k;
	return XTrue;
}
inline void _XMenuItem::disIcon()
{
	if(m_withIcon)
	{
		m_spriteIcon.release();
		m_withIcon = XFalse;
	}
}
inline void _XMenuItem::setHotChooseActive(_XBool flag)
{//�����ȼ�����״̬
	if(m_child != NULL) m_child->setHotChooseActive(flag);
	m_isHotChooseActive = flag;
	if(m_hotChooseKey >= 0) updateStr();
}
inline _XBool _XMenu::getCheckState(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	return tmpX->m_isChecked;
}
inline _XBool _XMenu::setCheckState(const std::string &pos,_XBool state)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type != MENU_ITEM_TYPE_CHK) return XFalse;
	tmpX->setCheckState(state);
	return XTrue;
}
inline _XBool _XMenu::setDisable(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XFalse;
	return XTrue;
}
inline _XBool _XMenu::setEnable(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	tmpX->m_isEnable = XTrue;
	return XTrue;
}
inline _XBool _XMenu::getEnable(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->m_isEnable;
}
inline _XBool _XMenu::setItemIcon(const std::string &pos,const char *filename,_XResourcePosition res)
{
	if(filename == NULL) return XFalse;
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	return tmpX->setIcon(filename,res);
}
inline _XBool _XMenu::setHotKey(const std::string &pos,char k)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(k >= 0 && m_rootItem.getItemByHotKey(k) != NULL) return XFalse;//������Ψһ�� 
	m_needPostProcess = XTrue;
	return tmpX->setHotKey(k);
}
inline char _XMenu::getHotKey(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotKey;
}
inline _XBool _XMenu::setHotChooseKey(const std::string &pos,char k)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(tmpX->setHotChooseKey(k))
	{
		m_needPostProcess = XTrue;
		return XTrue;
	}
	return XFalse;
}
inline char _XMenu::getHotChooseKey(const std::string &pos)
{
	_XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return -1;
	return tmpX->m_hotChooseKey;
}
inline void _XMenu::postProcess()
{
	if(!m_needPostProcess) return;
	m_rootItem.postPross();
	m_needPostProcess = XFalse;
}
inline _XBool _XMenu::setCBFunction(const std::string &pos,void (*callbackFun)(void *,const std::string &),void *p)
{//���ò˵���Ӧ�Ļص�����
	_XMenuItem * tmp = getItemByName(pos);
	if(tmp == NULL) return XFalse;
	tmp->setCallbackFun(callbackFun,p);
	return XTrue;
}
inline void _XMenu::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	postProcess();
	m_rootItem.draw();
}
inline std::string _XMenu::getItemFullStr(const _XMenuItem * it)
{
	if(it == NULL) return "";
	return it->getFullStr();
}
inline void _XMenu::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_rootItem.setPosition(m_position.x,m_position.y);
}
inline void _XMenu::setSize(float x,float y)
{
	m_size.set(x,y);
	m_rootItem.setSize(x,y);
	m_rootItem.setPosition(m_position.x,m_position.y);
}
inline void _XMenu::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_font.setColor(m_color);
	m_rootItem.setColor(m_color);
}
inline void _XMenu::setAlpha(float a)
{
	m_color.setA(a);
	m_font.setAlpha(a);
	m_rootItem.setColor(m_color);
}
inline _XVector2 _XMenu::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{//���ﲢû��ʵ�ʵ�����
	if(!m_isInited || 
		m_rootItem.m_child == NULL)	return _XVector2::zero;
	_XMenuItem * it = m_rootItem.m_child->m_items[0];	//�����õ�һ���˵���ķ�Χ�������ϲ�����
	if(it == NULL)	return _XVector2::zero;
	switch(order)
	{
	case 0:return it->m_position;
	case 1:return _XVector2(it->m_position.x + it->m_size.x * it->m_scaleSize.x,it->m_position.y);
	case 2:return _XVector2(it->m_position.x + it->m_size.x * it->m_scaleSize.x,
				it->m_position.y + it->m_size.y * it->m_scaleSize.y);
	case 3:return _XVector2(it->m_position.x,it->m_position.y + it->m_size.y * it->m_scaleSize.y);
	}
	return _XVector2::zero;
}
inline _XBool _XMenu::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline void _XMenu::setLostFocus()	//����ʧȥ����
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}