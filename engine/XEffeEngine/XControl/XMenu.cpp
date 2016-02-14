#include "XStdHead.h"
#include "XMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XMenuItem *XMenuGroup::getItemByHotKey(char k)
{//�����k����Ҫ����Ҫ�����������������ã��������ﲻ�ж�k�Ƿ����Ҫ��
	XMenuItem * ret;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		ret = m_items[i]->getItemByHotKey(k);
		if(ret != NULL) return ret;
	}
	return NULL;
}
XMenuItem *XMenuGroup::getItemByHotChooseKey(char k)
{//�����k����Ҫ����Ҫ�����������������ã��������ﲻ�ж�k�Ƿ����Ҫ��
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i]->m_hotChooseKey == k) return m_items[i];
	}
	return NULL;
}
void XMenuGroup::setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p)
{
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		m_items[i]->setAllCallBackFun(callbackFun,p);
	}
}
XBool XMenuGroup::setChooseItem(const XMenuItem *it)
{
	if(it == NULL) return XFalse;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{
		if(m_items[i] != it) continue;
		//��ԭԭ�е�ѡ��
		if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
			m_items[m_chooseIndex]->m_isMouseOn = XFalse;
		//�����µ�ѡ��
		m_chooseIndex = i;
		m_items[i]->m_isMouseOn = XTrue;
		return XTrue;
	}
	return XFalse;
}
void XMenuGroup::addIndex()//���ӱ��
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XFalse;
	if(m_chooseIndex < 0)
	{
		initIndex(XTrue);
		return;
	}
	int next = m_chooseIndex + 1;
	while(XTrue)
	{
		if(next == m_chooseIndex)
		{//û�п���ѡ�����Ϊ֮ԭ�е�ѡ��
			next = -1;
			break;
		}
		if(next >= 0 && next >= m_items.size())
		{
			next = 0;
			continue;
		}
		if(m_items[next]->m_type == MENU_ITEM_TYPE_DLT)
		{
			++ next;
			continue;
		}
		if(!m_items[next]->m_isEnable)
		{
			++ next;
			continue;
		}
		break;
	}
	if(next >= 0) m_chooseIndex = next;
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}	
void XMenuGroup::desIndex()	//���ٱ��
{
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XFalse;
	if(m_chooseIndex < 0)
	{
		initIndex(XFalse);
		return;
	}
	int next = m_chooseIndex - 1;
	while(XTrue)
	{
		if(next == m_chooseIndex)
		{//û�п���ѡ�����Ϊ֮ԭ�е�ѡ��
			next = -1;
			continue;
		}
		if(next < 0)
		{
			next = (int)(m_items.size()) - 1;
			continue;
		}
		if(m_items[next]->m_type == MENU_ITEM_TYPE_DLT)
		{
			-- next;
			continue;
		}
		if(!m_items[next]->m_isEnable)
		{
			-- next;
			continue;
		}
		break;
	}
	if(next >= 0) m_chooseIndex = next;
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}
void XMenuGroup::initIndex(XBool orderFlag)
{
	int index = -1;
	if(orderFlag)
	{//�����������Ŀ���ѡ���
		for(unsigned int i = 0;i < m_items.size();++ i)
		{
			if(m_items[i]->m_type != MENU_ITEM_TYPE_DLT &&
				m_items[i]->m_isEnable)
			{
				index = i;
				break;
			}
		}
	}else
	{//������ҿ���ѡ���
		for(int i = (int)(m_items.size()) - 1;i >= 0;-- i)
		{
			if(m_items[i]->m_type != MENU_ITEM_TYPE_DLT &&
				m_items[i]->m_isEnable)
			{
				index = i;
				break;
			}
		}
	}
	m_chooseIndex = index;
	for(unsigned int i = 0;i < m_items.size();++ i)
	{//�������е�ѡ��Ϊ��ѡ��״̬
		m_items[i]->m_isMouseOn = XFalse;
	}
	if(m_chooseIndex >= 0 && m_chooseIndex < m_items.size())
		m_items[m_chooseIndex]->m_isMouseOn = XTrue;
}	//��ʼ�����
void XMenuGroup::updatePos()
{
	if(m_items.size() <= 0) return;
	XVector2 pos;	//���һ���˵������ڵ�λ��
	if(m_parent->m_owner == NULL) pos = m_parent->m_position;
	else
	{
		if(m_parent->m_owner->m_type == MENU_TYPE_VERTICAL)
		{//��ֱ��
			pos = m_parent->m_position;
			pos.x += m_parent->m_size.x * m_parent->m_scaleSize.x;
		}else
		{//ˮƽ��
			pos = m_parent->m_position;
			pos.y += m_parent->m_size.y * m_parent->m_scaleSize.y;
		}
	}
	m_items[0]->setPosition(pos.x,pos.y);
	//����������еĿؼ���������λ��
	if(m_type == MENU_TYPE_VERTICAL)
	{
		XMenuItem *tmpItem = NULL;
		for(unsigned int i = 1; i < m_items.size();++ i)
		{
			tmpItem = m_items[i - 1];
			m_items[i]->setPosition(tmpItem->m_position.x,tmpItem->m_position.y + tmpItem->m_size.y * tmpItem->m_scaleSize.y);
		}
	}else
	{
		XMenuItem *tmpItem = NULL;
		for(unsigned int i = 1; i < m_items.size();++ i)
		{
			tmpItem = m_items[i - 1];
			m_items[i]->setPosition(tmpItem->m_position.x + tmpItem->m_size.x * tmpItem->m_scaleSize.x,tmpItem->m_position.y);
		}
	}
}
void XMenuGroup::updateColor()
{
	if(m_items.size() <= 0) return;
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setColor(m_parent->m_color);
	}
}
void XMenuGroup::updateSize()
{//��δʵ��,�������ű仯����λ��Ҳ�ᷢ���仯
	if(m_items.size() <= 0) return;
	for(unsigned int i = 0; i < m_items.size();++ i)
	{
		m_items[i]->setScale(m_parent->m_scaleSize.x,m_parent->m_scaleSize.y);
	}
}
XBool XMenuItem::init(const std::string &name,const XFontUnicode &font,float/*fontsize*/,XMenuGroup *owner,XMenuItemType type)
{
	if(owner == NULL) return XFalse;
	if(owner != NULL && type != MENU_ITEM_TYPE_DLT && owner->getItemByNameInGroup(name) != NULL) return XFalse;//������ֵ�Ψһ��
	m_originalName = name;
	m_owner = owner;
	if(owner == NULL || owner->m_parent == NULL) m_levelIndex = 0;
	else m_levelIndex = owner->m_parent->m_levelIndex + 1;
	m_type = type;
	m_position = owner->m_insertPos;
	if(owner->m_parent != NULL)
		m_scaleSize = owner->m_parent->m_scaleSize;
	else 
		m_scaleSize.set(1.0f,1.0f);

	m_isEnable = XTrue;

	if(m_type == MENU_ITEM_TYPE_DLT)
	{//����Ƿָ����Ļ����ﴦ��
		m_isVisible = XFalse;
		m_size.set(m_owner->m_width,5.0f);
		owner->m_insertPos.y += 5.0f;
		return XTrue;
	}

	if(!m_font.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	updateStr();

	m_size.set(0,32.0f);
	if(m_owner->m_width <= 0)
	{
		if(m_hotChooseKey >= 0) m_size.x = m_font.getMaxPixelWidth() + 2.0f;	//��Ҫ������
		else m_size.x = m_font.getMaxPixelWidth() + 50.0f;	//��Ҫ������
	}else
	{//������Ҫ������ʾ���ַ����ĳ���
		m_size.x = m_owner->m_width;
	}
	if(m_levelIndex < 2 && owner->m_type == MENU_TYPE_HORIZONTA) m_font.setPosition(m_position + XVector2(m_size.x * 0.5f,16));
	else
	{
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		if(m_type == MENU_ITEM_TYPE_CHK)
		{
			if(m_isChecked) m_showName = "X " + m_showName;
			else m_showName = "* " + m_showName;
			m_font.setPosition(m_position + XVector2(2.0f,0.0f));
		}else
		{
			m_font.setPosition(m_position + XVector2(36.0f,0.0f));
		}
	}
	if(owner->m_parent != NULL && owner->m_parent->m_isOpen) m_isVisible = XTrue;
	else m_isVisible = XFalse;

	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setPosition(m_position);

	switch(owner->m_type)
	{
	case MENU_TYPE_VERTICAL:	//��ֱ��
		owner->m_insertPos.y += 32.0f;
		break;
	case MENU_TYPE_HORIZONTA:	//ˮƽ��
		owner->m_insertPos.x += m_size.x;
		break;
	}
	return XTrue;
}
void XMenuItem::setString(const std::string &name)
{
	if(m_levelIndex == 0 ||	//���㲻��Ҫ����
		m_type == MENU_ITEM_TYPE_DLT) return;	//�ָ���������������
	m_originalName = name;

	updateStr();

	if(m_owner->m_width <= 0)
	{
		if(m_hotChooseKey >= 0) m_size.x = m_font.getMaxPixelWidth() + 2.0f;	//��Ҫ������
		else m_size.x = m_font.getMaxPixelWidth() + 50.0f;
	}else
	{//������Ҫ������ʾ���ַ����ĳ���
		m_size.x = m_owner->m_width;	//��Ҫ������
	}
	if(m_levelIndex < 2 && m_owner->m_type == MENU_TYPE_HORIZONTA) m_font.setPosition(m_position + XVector2(m_size.x * 0.5f,16));
}
void XMenuItem::beTriggered()	//�˵������ʱ�����������
{
	if(!m_isEnable || !m_isVisible) return;	//����״̬�²��ܴ����¼�
	if(m_callbackFun != NULL) m_callbackFun(m_pClass,getFullStr());
	if(m_type == MENU_ITEM_TYPE_CHK)
	{
		m_isChecked = !m_isChecked;
		//����ı�������ʾ
		if(m_isChecked) m_showName[0] = 'X';
		else m_showName[0] = '*';
		m_font.setString(m_showName.c_str());
	}
}
std::string XMenuItem::getFullStr() const	//��ȡ��ǰ�˵����ȫ·��
{//ʵʱ�Ļ�ȡ��ǰ�˵����ȫ·����������Կ����Ż�Ϊ�ռ任ʱ��ķ�ʽ
	std::string ret = "";
	const XMenuItem * it = this;
	while(XTrue)
	{
		if(it == NULL) return ret;
		ret = it->m_originalName + ";" + ret;
		if(it->m_owner == NULL) return ret;
		it = it->m_owner->m_parent;
	}
	return ret;
}
void XMenuItem::setPosition(float x,float y)
{
	m_position.set(x,y);	//���������λ��
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setPosition(m_position);
	if(m_type != MENU_ITEM_TYPE_DLT)
	{
		if(m_levelIndex < 2 && (m_owner == NULL || m_owner->m_type == MENU_TYPE_HORIZONTA)) 
			m_font.setPosition(m_position + XVector2(m_size.x * 0.5f * m_scaleSize.x,16 * m_scaleSize.y));
		else
		{
			if(m_type == MENU_ITEM_TYPE_CHK)
				m_font.setPosition(m_position + XVector2(2.0f,0.0f) * m_scaleSize);
			else
				m_font.setPosition(m_position + XVector2(36.0f,0.0f) * m_scaleSize);
		}
	}
	//�����Ӳ˵���λ�ö���Ҫ�޸�
	if(m_child != NULL) m_child->updatePos();
}
void XMenuItem::setColor(const XFColor &c)
{
	m_color = c;
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
		m_spriteIcon.setColor(c);
	if(m_type != MENU_ITEM_TYPE_DLT)
	{
		m_font.setAlpha(c.fA);
	}
	//�����Ӳ˵���λ�ö���Ҫ�޸�
	if(m_child != NULL) m_child->updateColor();
}
void XMenuItem::setScale(float x,float y)
{
	m_scaleSize.set(x,y);
	m_font.setScale(m_scaleSize);
	if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
	{
		float size = 32.0f * m_scaleSize.x / m_spriteIcon.getW();
		float tmp = 32.0f * m_scaleSize.y / m_spriteIcon.getH();
		m_spriteIcon.setScale((size < tmp)?size:tmp);
	}
	//��δ���
	if(m_child != NULL) m_child->updateSize();
}
XBool XMenuItem::mouseProc(float x,float y,XMouseState mouseState)
{
	if(m_type == MENU_ITEM_TYPE_DLT) return XFalse;

	if(m_isEnable && m_isVisible)//�ж��Ƿ�����
	{
		m_isMouseOn = XRect(m_position.x,m_position.y,m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * m_scaleSize.y).isInRect(x,y);
		if(m_child != NULL)
		{
			if(m_isOpen && m_child->mouseProc(x,y,mouseState)) return XTrue;
		}else
		{//�жϲ˵����¼��Ƿ񴥷�
			if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
				&& m_isMouseOn)
			{//����¼�
				beTriggered();
				return XTrue;
			}
		}
	}
	return XFalse;
}
XBool XMenuItem::setIcon(const char *filename,XResourcePosition res)
{
	if(m_type != MENU_ITEM_TYPE_BTN ||
		filename == NULL) return XFalse;
	if(m_withIcon) m_spriteIcon.release();
	if(!m_spriteIcon.init(filename,res,POINT_LEFT_TOP)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteIcon);
#endif
	float size = 32.0f * m_scaleSize.x / m_spriteIcon.getW();
	float tmp = 32.0f * m_scaleSize.y / m_spriteIcon.getH();
	m_spriteIcon.setScale((size < tmp)?size:tmp);
	m_spriteIcon.setPosition(m_position);
	m_withIcon = XTrue;
	return XTrue;
}
void XMenuItem::draw()
{
	if(!m_isVisible) return;
	if(m_type == MENU_ITEM_TYPE_DLT)
	{
		XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::normalColor * m_color);
		XRender::drawLine(m_position.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,
			m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,2.0f,
			XCCS::blackOnColor * m_color);
		XRender::drawLine(m_position.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,
			m_position.x + m_size.x * m_scaleSize.x,m_position.y + m_size.y * 0.5f * m_scaleSize.y,1.0f,
			XCCS::mouseColor * m_color);
	}else
	{
		if(!m_isEnable) XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::mouseColor * m_color); else
		if(m_isMouseOn) XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::onColor * m_color); else
		XRender::drawFillBox(m_position.x,m_position.y,m_size.x * m_scaleSize.x,m_size.y * m_scaleSize.y,
			XCCS::normalColor * m_color);

		m_font.draw();
		if(m_child != NULL && m_isOpen) m_child->draw();
		if(m_withIcon &&  m_type == MENU_ITEM_TYPE_BTN)
			m_spriteIcon.draw();
	}
}
void XMenuItem::updateStr()
{
	if(m_type == MENU_ITEM_TYPE_DLT) return;
	//������Ҫ���ÿ�ݼ���ʾ����
	if(m_owner->m_width <= 0)
	{
		m_showName = m_originalName;
		if(m_hotChooseKey >= 0)
		{
			if(!m_isHotChooseActive) m_showName = m_showName + "(" + m_hotChooseKey + ")";		//������ʱ��ʾ��д
			else m_showName = m_showName + "(" + (char)(m_hotChooseKey - ('A' - 'a')) + ")";	//����ʱ��ʾСд
		}
		m_font.setString(m_showName.c_str());
	}else
	{//������Ҫ������ʾ���ַ����ĳ���
		int stringShowWidth = 0;
		if(m_levelIndex >= 2) stringShowWidth = (floor((m_owner->m_width - 36.0f) / m_font.getTextSize().x) * 2) - 1;
		else stringShowWidth = (floor(m_owner->m_width / m_font.getTextSize().x) * 2) - 1;
		if(m_originalName.length() <= stringShowWidth)
		{
			m_showName = m_originalName;
		}else
		if(stringShowWidth >= 2)
		{
			if(m_originalName.length() >= m_maxMenuItemLength - 1) 
			{
				printf("XMenu item's name length error!\n");
				return;	//���ȷǷ�
			}
			char p[m_maxMenuItemLength];
			strcpy(p,m_originalName.c_str());
			if(XString::isAtUnicodeEnd(m_originalName.c_str(),stringShowWidth - 2))
			{

				p[stringShowWidth] = '\0';
				p[stringShowWidth - 1] = '.';
				p[stringShowWidth - 2] = '.';
			}else
			{
				p[stringShowWidth] = '\0';
				p[stringShowWidth - 1] = '.';
				p[stringShowWidth - 2] = '.';
				p[stringShowWidth - 3] = '.';
			} 
			m_showName = p;
		}else
		{
			m_showName = "";
		}
		if(m_hotChooseKey >= 0)
		{
			if(!m_isHotChooseActive) m_showName = m_showName + "(" + m_hotChooseKey + ")";
			else m_showName = m_showName + "(" + (char)(m_hotChooseKey - ('A' - 'a')) + ")";
		}
		if(m_levelIndex >= 2 && m_child != NULL)
		{//������Ҫ��㹻�Ŀո�
			while((int)(m_showName.length()) < stringShowWidth - 1)
			{
				//if((int)(m_showName.length()) >= stringShowWidth - 1) break;
				m_showName += " ";
			}
			m_showName += ">";
		}
		if(m_type == MENU_ITEM_TYPE_CHK)
		{
			if(m_isChecked) m_showName = "X " + m_showName;
			else m_showName = "* " + m_showName;
		}
		m_font.setString(m_showName.c_str());
	}
}
XMenuItem * XMenu::getItemFromGroup(char *p,XMenuGroup *g)
{
	if(g == NULL || p == NULL) return NULL;
	int offset = XString::getCharPosition(p,';');
	if(offset == 0 || offset >= 1024) return NULL;	//�Ƿ�������
	if(offset == -1)
	{//�������ַ���
		for(unsigned int i = 0;i < g->m_items.size();++ i)
		{
			if(strcmp(p,g->m_items[i]->m_originalName.c_str()) == 0)
			{
				return g->m_items[i];
			}
		}
		return NULL;	//�Ҳ���ƥ���
	}else
	{
		char tmp[XMenuItem::m_maxMenuItemLength];
		memcpy(tmp,p,offset);
		tmp[offset] = '\0';
		int len = strlen(p);
		if(len - 1 == offset)
		{//��';'�ָ�������
			for(unsigned int i = 0;i < g->m_items.size();++ i)
			{
				if(strcmp(tmp,g->m_items[i]->m_originalName.c_str()) == 0)
				{
					return g->m_items[i];
				}
			}
			return NULL;	//�Ҳ���ƥ���
		}else
		{
			for(unsigned int i = 0;i < g->m_items.size();++ i)
			{
				if(strcmp(tmp,g->m_items[i]->m_originalName.c_str()) == 0)
				{//��������һ�������Ѱ��
					return getItemFromGroup(p + offset + 1,g->m_items[i]->m_child);
				}
			}
			return NULL;	//�Ҳ���ƥ���
		}
	}
	//return NULL;
}
XBool XMenu::setMenuItemStr(const std::string &pos,const std::string &name)
{//���ò˵��������
	if(name.length() >= XMenuItem::m_maxMenuItemLength - 1) return XFalse;	//�ַ���̫��
	XMenuItem * tmpX = getItemByName(pos);
	if(tmpX == NULL || tmpX->m_type == MENU_ITEM_TYPE_DLT) return XFalse;
	if(tmpX->m_owner != NULL && tmpX->m_owner->getItemByNameInGroup(name) != NULL) return XFalse;	//������������޸ģ����־���Ψһ��

	tmpX->setString(name);
	if(tmpX->m_owner != NULL && tmpX->m_owner->m_type == MENU_TYPE_HORIZONTA)
	{//�����ˮƽ�ģ�����Ҫ������λ��
		int size = tmpX->m_owner->m_items.size();
		for(int i = 1;i < size;++ i)
		{
			tmpX->m_owner->m_items[i]->setPosition(tmpX->m_owner->m_items[i - 1]->m_position.x + tmpX->m_owner->m_items[i - 1]->m_size.x,
				tmpX->m_owner->m_items[i - 1]->m_position.y);
		}
		tmpX->m_owner->m_insertPos.x = tmpX->m_owner->m_items[size - 1]->m_position.x + tmpX->m_owner->m_items[size - 1]->m_size.x;
	}
	return XTrue;
}
void XMenu::ctrlProc(void *pClass,const std::string &pos)
{
	XMenu &pPar = *(XMenu *)pClass;
	pPar.m_curChooseMenuStr = pos;
	if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,MNU_CHOOSE);
	else XCtrlManager.eventProc(pPar.m_objectID,MNU_CHOOSE);
}
XBool XMenu::addAItem(const std::string &pos,const std::string &name,int width,XMenuItemType type)
{//��˵�������һ������
	if(name.length() >= XMenuItem::m_maxMenuItemLength - 1) return XFalse;	//�ַ���̫��
	if(XString::getCharPosition(name.c_str(),';') >= 0) return XFalse;	//�����в����зָ���
	if(width >= 0 && width <= 256) width = 256;
	if(pos == "root" || pos == "root;")
	{//���������
		if(type != MENU_ITEM_TYPE_BTN) return XFalse;	//���ϲ��ܼ���������
		if(m_rootItem.m_child == NULL)
		{//������δ����
			m_rootItem.m_isOpen = XTrue;
			m_rootItem.m_child = XMem::createMem<XMenuGroup>();
			if(m_rootItem.m_child == NULL) return XFalse;
			//�����ʼ�������Ϣ
			switch(m_type)
			{
			case MENU_TYPE_VERTICAL:
				m_rootItem.m_child->m_type = MENU_TYPE_VERTICAL;
				m_rootItem.m_child->m_width = 256;
				break;
			case MENU_TYPE_HORIZONTA:
				m_rootItem.m_child->m_type = MENU_TYPE_HORIZONTA;
				m_rootItem.m_child->m_width = -1;
				break;
			}

			m_rootItem.m_child->m_parent = &m_rootItem;
			m_rootItem.m_child->m_insertPos = m_position;
		}else
		{//��Ҫ������ֵ�Ψһ��
			if(m_rootItem.m_child->getItemByNameInGroup(name) != NULL) return XFalse;
		}
		//����һ����������ӽ�ȥ
		XMenuItem * tmp = XMem::createMem<XMenuItem>();
		if(tmp == NULL) return XFalse;
		tmp->init(name,m_font,m_fontSize,m_rootItem.m_child);
		tmp->setCallbackFun(ctrlProc,this);
		m_rootItem.m_child->m_items.push_back(tmp);
	}else
	{//�Ǹ�������ӻ��߷Ƿ�����
		XMenuItem * tmpX = getItemByName(pos);
		if(tmpX == NULL ||
			tmpX->m_type != MENU_ITEM_TYPE_BTN ||	//�������������͵Ŀؼ���������µĿؼ�
			type != MENU_ITEM_TYPE_BTN && tmpX->m_levelIndex < 1) return XFalse;
		if(tmpX->m_child == NULL)
		{//����������
			if(width < 256) width = 256;	//��С��ȱ�����256�������ݲ�С��Ҫ�Ŀռ�
			tmpX->m_child = XMem::createMem<XMenuGroup>();
			if(tmpX->m_child == NULL) return XFalse;
			//�����ʼ�������Ϣ
			tmpX->m_child->m_parent = tmpX;
			tmpX->m_child->m_insertPos = tmpX->m_position;
			switch(tmpX->m_owner->m_type)
			{
			case MENU_TYPE_HORIZONTA:
				tmpX->m_child->m_insertPos.y += tmpX->m_size.y;
				break;
			case MENU_TYPE_VERTICAL:
				tmpX->m_child->m_insertPos.x += tmpX->m_size.x;
				break;
			}
			tmpX->m_child->m_type = MENU_TYPE_VERTICAL;
			tmpX->m_child->m_width = width;
		}else
		{
			if(type != MENU_ITEM_TYPE_DLT && tmpX->m_child->getItemByNameInGroup(name) != NULL) return XFalse;
		}
		//����һ����������ӽ�ȥ
		XMenuItem * tmp = XMem::createMem<XMenuItem>();
		if(tmp == NULL) return XFalse;
		tmp->init(name,m_font,m_fontSize,tmpX->m_child,type);
		tmp->setCallbackFun(ctrlProc,this);
		tmpX->m_child->m_items.push_back(tmp);
	}
	m_needPostProcess = XTrue;
	return XTrue;
}
XMenuItem * XMenu::getItemByName(const std::string &name)
{//��δ���
	if(name == "root" || name == "root;") return &m_rootItem;
	int offset = XString::getCharPosition(name.c_str(),';');
	if(offset <= 0 || offset >= 1024) return NULL;	//�Ƿ�������
	char tmp[XMenuItem::m_maxMenuItemLength];
	memcpy(tmp,name.c_str(),offset);
	tmp[offset] = '\0';
	if(strcmp(tmp,"root") != 0) return NULL;
	char *p = XMem::createArrayMem<char>(name.length() + 1);
	if(p == NULL) return NULL;
	strcpy(p,name.c_str());
	XMenuItem * ret = getItemFromGroup(p + offset + 1,m_rootItem.m_child);
	XMem::XDELETE_ARRAY(p);
	return ret;
}
XBool XMenu::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_rootItem.mouseProc(x,y,mouseState))
	{//�����˵���
		if(m_hotChooseKeyIsActive)
		{
			m_hotChooseKeyIsActive = XFalse;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
		}
		closeAll();
		m_oldChooseItem = NULL;
		m_isClicked = XFalse;
		return XTrue;
	}
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{
		if(m_hotChooseKeyIsActive)
		{//ֻҪ����������¼���ȡ��ѡ�񼤻�״̬
			m_hotChooseKeyIsActive = XFalse;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
			closeAll();
			m_oldChooseItem = NULL;
			m_isClicked = XFalse;
		}
		if(m_rootItem.isInRect(x,y))
		{//����¼������ڲ˵���
			XMenuItem * tmp = m_rootItem.getItem(x,y);
			if(!m_isClicked)
			{
				m_isClicked = XTrue;
				if(tmp != NULL) tmp->setOpen();
			}else
			{
				if(tmp != NULL && tmp->m_levelIndex < 2)
				{
					closeAll();
					m_oldChooseItem = NULL;
					m_isClicked = XFalse;
					return XTrue;
				}
			}
			return XTrue;
		}else
		{
			if(m_isClicked)
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
				return XTrue;
			}
		}
	}
	if(m_isClicked && mouseState == MOUSE_MOVE)
	{//����صĲ˵�(��δ���)
		//��ȡ��ǰ�������λ�õĲ˵���
		XMenuItem * tmp = m_rootItem.getItem(x,y);
		if(tmp != NULL && tmp != m_oldChooseItem)
		{
			tmp->setOpen();	//���������򿪵�ǰѡ��
			//�ж�Ŀ���Ƿ����ı�
			if(m_oldChooseItem != NULL)
			{
				//�жϹر�����
				while(XTrue)
				{
					if(m_oldChooseItem == NULL || getIsPath(m_oldChooseItem,tmp)) break;	//�����·�������˳�
					//������رգ�����һ���ж�
					m_oldChooseItem->disOpen();
					if(m_oldChooseItem->m_owner == NULL) break;
					m_oldChooseItem = m_oldChooseItem->m_owner->m_parent;
				}
			}
			m_oldChooseItem = tmp;
		}
	}
	return XFalse;
}
XBool XMenu::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(keyState == KEY_STATE_DOWN)
	{
		switch(keyOrder)
		{
		case XKEY_LALT:
		case XKEY_RALT:
			m_hotChooseKeyIsActive = !m_hotChooseKeyIsActive;
			m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
			if(!m_hotChooseKeyIsActive)//�ر����еĲ˵���
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
			}else if(m_isClicked)
			{
				closeAll();
				m_oldChooseItem = NULL;
				m_isClicked = XFalse;
			}
			break;
		}
		if(m_hotChooseKeyIsActive)
		{//�������ȼ��Ĵ���
			switch(keyOrder)
			{
			case XKEY_LEFT:
				if(m_oldChooseItem == NULL)
				{//ѡ����Ǹ�
					if(m_rootItem.m_child != NULL)
						m_rootItem.m_child->desIndex();
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL://��ѡ�����Ǵ򿪲���
							if(m_oldChooseItem->m_owner != NULL)
							{
								if(m_oldChooseItem->m_levelIndex < 2 && 
									m_oldChooseItem->m_owner->m_type == MENU_TYPE_HORIZONTA)
								{
									XMenuItem *it = m_rootItem.m_child->getChooseItem();
									closeAll();
									m_rootItem.m_child->setChooseItem(it);
									m_rootItem.m_child->desIndex();
									//���Ҵ������
									it = m_rootItem.m_child->getChooseItem();
									it->setOpen();
									it->m_child->initIndex(XTrue);
									m_oldChooseItem = it;
								}else
								{
									XMenuItem * it = m_oldChooseItem->m_owner->getChooseItem();
									if(it != NULL)
									{
										it->disOpen();
										m_oldChooseItem = it->m_owner->m_parent;
									}
								}
							}
							break;
						case MENU_TYPE_HORIZONTA:
							m_oldChooseItem->m_child->desIndex();
							break;
						}
					}
				}
				break;
			case XKEY_RIGHT:
				if(m_oldChooseItem == NULL)
				{//ѡ����Ǹ�
					if(m_rootItem.m_child != NULL)
					{
						m_rootItem.m_child->addIndex();
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL://��ѡ�����ǹرղ���
							{
								XMenuItem * it = m_oldChooseItem->m_child->getChooseItem();
								if(it != NULL)
								{
									if(it->m_child != NULL)
									{//�򿪲���
										it->setOpen();
										it->m_child->initIndex(XTrue);
										m_oldChooseItem = it;
									}else
									{//����
										if(m_oldChooseItem->m_owner != NULL && 
											m_oldChooseItem->m_owner->m_type == MENU_TYPE_HORIZONTA)
										{
											it = m_rootItem.m_child->getChooseItem();
											closeAll();
											m_rootItem.m_child->setChooseItem(it);
											m_rootItem.m_child->addIndex();
											//���Ҵ������
											it = m_rootItem.m_child->getChooseItem();
											it->setOpen();
											it->m_child->initIndex(XTrue);
											m_oldChooseItem = it;
										}
									}
								}
							}
							break;
						case MENU_TYPE_HORIZONTA:
							m_oldChooseItem->m_child->addIndex();
							break;
						}
					}
				}
				break;
			case XKEY_UP:
				if(m_oldChooseItem == NULL)
				{//ѡ����Ǹ�
					if(m_rootItem.m_child != NULL)
					{
						XMenuItem * it = m_rootItem.m_child->getChooseItem();
						if(it != NULL && it->m_child != NULL)
						{//�򿪲���
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL:
							m_oldChooseItem->m_child->desIndex();
							break;
						case MENU_TYPE_HORIZONTA://��ѡ�����ǹرղ���
							break;
						}
					}
				}
				break;
			case XKEY_DOWN:
				if(m_oldChooseItem == NULL)
				{//ѡ����Ǹ�
					if(m_rootItem.m_child != NULL)
					{
						XMenuItem * it = m_rootItem.m_child->getChooseItem();
						if(it != NULL && it->m_child != NULL)
						{//�򿪲���
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}
					}
				}else
				{
					if(m_oldChooseItem->m_child != NULL)
					{
						switch(m_oldChooseItem->m_child->m_type)
						{
						case MENU_TYPE_VERTICAL:
							m_oldChooseItem->m_child->addIndex();
							break;
						case MENU_TYPE_HORIZONTA://��ѡ�����ǹرղ���
							break;
						}
					}
				}
				break;
			case XKEY_RETURN:	//ȷ�϶���
				if(m_oldChooseItem == NULL)
				{//ѡ����Ǹ�
					XMenuItem * it = m_rootItem.m_child->getChooseItem();
					if(it != NULL && it->m_child != NULL)
					{//�򿪲���
						it->setOpen();
						it->m_child->initIndex(XTrue);
						m_oldChooseItem = it;
					}
				}else
				{
					XMenuItem * it = m_oldChooseItem->m_child->getChooseItem();
					if(it != NULL)
					{
						if(it->m_child != NULL)
						{//�򿪲���
							it->setOpen();
							it->m_child->initIndex(XTrue);
							m_oldChooseItem = it;
						}else
						{//ȷ������
							//������ûص�����
							it->beTriggered();
							//��£�˵�
							closeAll();
							m_oldChooseItem = NULL;
							m_isClicked = XFalse;
							//ȡ���˵��ļ��̼���
							m_hotChooseKeyIsActive = XFalse;
							m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
						}
					}
				}
				break;
			default:
				{
					char k = XEE::keyValueToChar((XKeyValue)keyOrder);
					if(k < 'A' || k > 'Z') return XFalse;
					if(m_oldChooseItem == NULL)
					{//ѡ����Ǹ�
						XMenuItem *it = m_rootItem.getItemByHotChooseKey(k);
						if(it != NULL)
						{
							m_rootItem.m_child->setChooseItem(it);
							it->setOpen();
							it->m_child->initIndex();
							m_oldChooseItem = it;
						}
					}else
					{
						XMenuItem *it = m_oldChooseItem->getItemByHotChooseKey(k);
						if(it != NULL)
						{
							if(it->m_child == NULL)
							{//ʹ���¼���Ч
								it->beTriggered();	//���ô����¼�
								//��£�˵�
								closeAll();
								m_oldChooseItem = NULL;
								m_isClicked = XFalse;
								//ȡ���˵��ļ��̼���
								m_hotChooseKeyIsActive = XFalse;
								m_rootItem.setHotChooseActive(m_hotChooseKeyIsActive);
							}else
							{
								m_oldChooseItem->m_owner->setChooseItem(it);
								it->setOpen();
								m_oldChooseItem = it;
							}
						}
					}
				}
				break;
			}
		}
	}
	return XFalse;
}
XBool XMenu::getIsPath(XMenuItem *s,XMenuItem *d)
{
	while(XTrue)
	{
		if(s == d) return XTrue;
		if(d == NULL) return XFalse;
		if(d->m_owner == NULL) return XFalse;
		d = d->m_owner->m_parent;
	}
	return XFalse;
}
void XMenu::closeAll()	//�۵����еĲ˵���
{
	if(m_rootItem.m_child == NULL) return;
	for(unsigned int i = 0;i < m_rootItem.m_child->m_items.size();++ i)
	{
		m_rootItem.m_child->m_items[i]->disOpen();
		m_rootItem.m_child->resetChoose();
	}
}
//XBool XMenu::setAllCBFunction(void (*callbackFun)(void *,const std::string &),void *p)
//{//��δʵ��
//	m_rootItem.setAllCallBackFun(callbackFun,p);
//	return XTrue;
//}
XBool XMenu::initWithoutSkin(const XFontUnicode &font,float captionSize,XMenuType type)
{
	if(m_isInited) return XFalse;
	if(captionSize <= 0.0f) return XFalse;
	if(!m_font.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif

	m_fontSize = captionSize;
	m_type = type;

	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_rootItem.m_originalName = "root";
	m_rootItem.m_position = m_position;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XMenu::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif

	//�����ͷ������Դ
	XMem::XDELETE(m_rootItem.m_child);
	m_isInited = XFalse;
}
#if !WITH_INLINE_FILE
#include "XMenu.inl"
#endif
}