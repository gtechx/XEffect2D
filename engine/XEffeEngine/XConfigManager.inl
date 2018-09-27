INLINE void XConfigManager::setItemValue(int ID, const XConfigValue& value)
{
	XConfigItem * it = getItemByID(ID);
	if (it == NULL) return;
	it->m_curValue = value;
	updateItemFromCFG(it);
}
INLINE void XConfigGroup::useANewRow()
{//ʹ���µ�һ��
	m_curInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_scale.x;
	m_curInsertPos.y = m_position.y + (32.0f/* + CFG_MANAGER_H_SPACE*/) * m_scale.y;
	m_isNewRow = true;
}
INLINE void XConfigManager::moveDown(int pixelW, int pixelH)	//����ƫ����������һ�������
{
	m_curInsertPos.y += (pixelH + CFG_MANAGER_H_SPACE) * m_scale.y;
	if (m_isNewRow || pixelW > m_maxRowWidth) m_maxRowWidth = pixelW;
	m_isNewRow = false;
	if (m_curInsertPos.y >= m_position.y + m_maxHeight)
	{//������Ҫ����
		useANewRow();
	}
}
INLINE void XConfigManager::useANewRow()
{//ʹ���µ�һ��
	m_curInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_scale.x;
	m_curInsertPos.y = m_position.y + (32.0f/* + CFG_MANAGER_H_SPACE*/) * m_scale.x;
	m_isNewRow = true;
}
INLINE void XConfigManager::setPosition(const XVec2& p)
{
	if (!m_isInited) return;
	m_position = p;
	relayout();	//���²���
}
INLINE void XConfigManager::setScale(const XVec2& s)
{
	if (!m_isInited) return;
	m_scale = s;
	relayout();	//���²���
}
INLINE void XConfigManager::setMaxHeight(float h)
{
	if (h < 1) h = 1;
	m_maxHeight = h;
	relayout();
}
INLINE bool XConfigManager::isSpecialItemExist(void *it)
{
	if (it == NULL) return false;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->m_pVariable == it) return true;
	}
	return false;
}
INLINE bool XConfigManager::renameGroup(const char *oldName, const char *newName)	//�ı��������
{
	if (oldName == NULL || newName == NULL) return false;
	if (isGroupExist(newName)) return false;
	XConfigGroup *gp = getGroup(oldName);
	if (gp == NULL) return false;
	gp->m_name = newName;
	gp->m_group.setCaptionText(newName);
	return true;
}