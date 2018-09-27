#include "XStdHead.h"
#include "XSubWindow.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XXml.h"
namespace XE{
void XSubWindow::ctrlProc(void *pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XSubWindow &pPar = *(XSubWindow *)pClass;
	if(id == pPar.m_titleButton[0].getControlID())
	{//��С�������������İ�ť
		switch(pPar.m_windowState)
		{
		case STATE_NORMAL:pPar.changeStage(STATE_MINISIZE);break;
		case STATE_MINISIZE:pPar.changeStage(STATE_NORMAL);break;
		}
		return;
	}
	//����Ĺ�����δʵ��
	//if(id == pPar.m_titleButton[1].getControlID())
	//{//�رմ��ڵİ�ť
	//	return;
	//}
	//if(id == pPar.m_titleButton[2].getControlID())
	//{//����չ�İ�ť
	//	return;
	//}
}
XBool XSubWindow::initWithoutSkin(const XVec2& area, const char * title)	//��Ƥ���ĳ�ʼ��
{
	if (m_isInited) return XFalse;
	//������Ҫ�ж���������ĺϷ���
	if (area.x <= 128.0f || area.y <= 128.0f) return XFalse;
	m_area.set(XVec2::zero, area);

	m_position.reset();
	m_scale.set(1.0f);
	m_color.set(1.0f, 1.0f);
	m_titleFont.setACopy(getDefaultFont());
	m_titleFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_titleFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_titleFont.setPosition(m_position.x + 2.0f, m_position.y + 16.0f);
	m_titleFont.setColor(XFColor::black);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleFont);
#endif

	m_titleButton[0].initWithoutSkin("", 32.0f);
	m_titleButton[0].setPosition(m_position.x + m_area.right - 96.0f, m_position.y);
	m_titleButton[0].setEventProc(ctrlProc, this);
	//	m_titleButton[0].setWithAction(XFalse);
	m_titleButton[0].setSymbol(BTN_SYMBOL_LINE);
	m_titleButton[1].initWithoutSkin("", 32.0f);
	m_titleButton[1].setPosition(m_position.x + m_area.right - 64.0f, m_position.y);
	m_titleButton[1].setEventProc(ctrlProc, this);
	//	m_titleButton[1].setWithAction(XFalse);
	m_titleButton[1].setSymbol(BTN_SYMBOL_MASK_WRONG);
	m_titleButton[2].initWithoutSkin("", 32.0f);
	m_titleButton[2].setPosition(m_position.x + m_area.right - 32.0f, m_position.y);
	m_titleButton[2].disable();
	m_titleButton[2].setEventProc(ctrlProc, this);
//	m_titleButton[2].setWithAction(XFalse);
	m_titleButton[2].setSymbol(BTN_SYMBOL_LEFT);
	XCtrlManager.decreaseAObject(&m_titleButton[0]);	//ע��������
	XCtrlManager.decreaseAObject(&m_titleButton[1]);	//ע��������
	XCtrlManager.decreaseAObject(&m_titleButton[2]);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleButton[0]);
	XObjManager.decreaseAObject(&m_titleButton[1]);
	XObjManager.decreaseAObject(&m_titleButton[2]);
#endif

	updateMouseRect();
	m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	setTitle(title);
	setAlpha(0.5f);
	return XTrue;
}
void XSubWindow::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//������Ҫ������е����
	m_isInited = false;
}
void XSubWindow::updateMouseRect()
{
	switch(m_windowState)
	{
	case STATE_NORMAL:
		if(m_windowType != TYPE_NO_TITLE)
		{
			m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
				m_position + XVec2(m_area.right,m_area.bottom + 32.0f) * m_scale);
		}else
		{
			m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
				m_position + m_area.getRB() * m_scale);
		}
		break;
	case STATE_MINISIZE:
		m_curMouseRect.set(m_position + m_area.getLT() * m_scale,
			m_position + XVec2(256.0f, 32.0f) * m_scale);
		break;
	}
}
void XSubWindow::draw()
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if (m_windowState == STATE_NORMAL)
	{
		if (m_windowType != TYPE_NO_TITLE)
		{//��������������
			XRender::drawFillRectA(m_position + m_area.getLT(),
				XVec2(m_area.getWidth(), 32.0f) * m_scale,
				XCCS::normalColor * m_color, true);
			m_titleFont.draw();
			m_titleButton[0].draw();
			m_titleButton[1].draw();
			m_titleButton[2].draw();
		}
		//������湤����
		if (m_withBackGround)
		{//������ڱ����������濪ʼ��汳��
			if (m_windowType == TYPE_NO_TITLE)
			{
				XRender::drawFillRectA(m_position + m_area.getLT(),
					m_area.getSize() * m_scale,
					XCCS::downColor * m_color, true);
			}
			else
			{
				XRender::drawFillRectA(m_position + XVec2(m_area.left, m_area.top + 32.0f) * m_scale,
					m_area.getSize() * m_scale,
					XCCS::downColor * m_color, true);
			}
		}
		//���濪ʼ������е����

		m_ctrlManager.draw();
		m_ctrlManager.drawUp();
	}
	else
	//if (m_windowState == STATE_MINISIZE)
	{
		XRender::drawFillRectA(m_position + m_area.getLT(),
			XVec2(256.0f, 32.0f) * m_scale,
			XFColor(m_color * 0.75f, 1.0f * m_color.a), true);
		m_titleFont.draw();
		m_titleButton[0].draw();
		m_titleButton[1].draw();
		m_titleButton[2].draw();
	}
	//����ӿؼ�
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		m_objects[i].obj->draw();
//	}
}
void XSubWindow::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if((m_windowState == STATE_NORMAL && m_windowType != TYPE_NO_TITLE) ||
		m_windowState == STATE_MINISIZE)
	{//��������������
		m_titleButton[0].drawUp();
		m_titleButton[1].drawUp();
		m_titleButton[2].drawUp();
	}
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		m_objects[i].obj->drawUp();
//	}
	//m_ctrlManager.drawUp();
}
XBool XSubWindow::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;
	m_curMousePosition.set(p);
	if(m_curMouseRect.isInRect(p))
	{//�����ж���ק����
		if(m_isBeChoose)
		{
			m_titleButton[0].mouseProc(p,mouseState);
			m_titleButton[1].mouseProc(p,mouseState);
			m_titleButton[2].mouseProc(p,mouseState);
		}
		switch(mouseState)
		{
		case MOUSE_LEFT_BUTTON_DCLICK:	
			//�ı䴰�ڵ�״̬
			if(m_windowType != TYPE_NO_TITLE && getIsInDragRect(p))
			{
				switch(m_windowState)//�������Ҫ�޶��ڱ��ⷶΧ�ڣ�������Ѳ���
				{
				case STATE_NORMAL:changeStage(STATE_MINISIZE);break;
				case STATE_MINISIZE:changeStage(STATE_NORMAL);break;
				}
			}
		case MOUSE_LEFT_BUTTON_DOWN:
			setFocus();
			break;
		}
	}
	if(!m_isBeChoose) return XFalse;		
	if(m_windowType != TYPE_NO_TITLE)
		mouseEventProc(p,mouseState);
	m_ctrlManager.mouseProc(p,mouseState);
	return XFalse;
}
XBool XSubWindow::addACtrlObj(XControlBasic *obj)	//�򴰿������һ���ؼ�
{
	if(obj == NULL) return XFalse;
	if(getObjectIndex(obj) >= 0) return XFalse;	//���������ظ�����Դ
	XSubWindowObj tmpObj;
	tmpObj.obj = obj;
	tmpObj.pos = obj->getPosition() - m_position;
	tmpObj.scale = obj->getScale() / m_scale;
	m_objects.push_back(tmpObj);
	//����1���ؼ��������������������ؼ�
	//XCtrlManager.setNeedDraw(false,obj);	//ע��������
	//XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(obj),XCtrlManager.findObjectID(this));
	//����2��
	XCtrlManager.decreaseAObject(obj);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(obj);
#endif
	m_ctrlManager.addACtrl(obj);

	tmpObj.obj->setColor(m_color);
	tmpObj.obj->setScale(tmpObj.scale * m_scale);
	return true;
}
XBool XSubWindow::desACtrlObj(XControlBasic* obj)
{
	if (obj == NULL) return XFalse;
	int index = getObjectIndex(obj);
	if(index < 0) return XFalse;	//���������ظ�����Դ

	m_objects.erase(m_objects.begin() + index);
	m_ctrlManager.decreaseAObject(obj);
	//����1���ؼ��������������������ؼ�
	//XCtrlManager.setNeedDraw(false,obj);	//ע��������
	//XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(obj),XCtrlManager.findObjectID(this));
	//����2��
	XCtrlManager.addACtrl(obj);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(obj);
#endif
	return true;
}
void XSubWindow::setFocus() 
{
	m_isBeChoose = XTrue;
	XCtrlManager.moveToFirst(XCtrlManager.findObjectID(this));
//	for(unsigned int i = 0;i < m_objects.size();++ i)
//	{
//		XCtrlManager.moveAndInsert(XCtrlManager.findObjectID(m_objects[i].obj),XCtrlManager.findObjectID(this));
//	}
	setAlpha(1.0f);
}	
int XSubWindow::getObjectIndex(XControlBasic *obj)
{//ok
	if(obj == NULL) return -1;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i].obj == obj) return i;
	}
	return -1;
}
void XSubWindow::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	XVec2 tmpOldPos = m_position;
	m_position = p;
	m_titleFont.setPosition(m_position.x + 2.0f * m_scale.x,m_position.y + 16.0f * m_scale.y);
	switch(m_windowState)
	{
	case STATE_NORMAL:
		m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
		break;
	case STATE_MINISIZE:
		m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
		break;
	}
	updateMouseRect();
	//����1��
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
	//}
	//����2��
	XVec2 offset;
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->pos = it->obj->getPosition() - tmpOldPos;
		it->obj->setPosition(it->pos * m_scale + m_position);
	}
}
void XSubWindow::setScale(const XVec2& s)
{
	if(!m_isInited) return;
	m_scale = s;
	m_titleFont.setScale(m_scale);
	m_titleButton[0].setScale(m_scale);
	m_titleButton[1].setScale(m_scale);
	m_titleButton[2].setScale(m_scale);
	m_titleFont.setPosition(m_position + XVec2(2.0f, 16.0f) * m_scale);
	switch(m_windowState)
	{
	case STATE_NORMAL:
		m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
		break;
	case STATE_MINISIZE:
		m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
		m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
		m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
		break;
	}
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setPosition(m_objects[i].pos * m_scale + m_position);
		m_objects[i].obj->setScale(m_objects[i].scale * m_scale);
	}
}
void XSubWindow::setMiniSize()
{
	if(m_windowType == TYPE_NO_TITLE) return;	//�޴���ģʽ��������Ϊ��С��
	m_windowState = STATE_MINISIZE;
	//�ı��������ʾ���ı䰴ť��λ��

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),256.0f - 96.0f,15.0f).c_str());
		
	//m_titleButton[0].setCaptionText("��");
	m_titleButton[0].setSymbol(BTN_SYMBOL_RECT);
	m_titleButton[0].setPosition(m_position.x + (256.0f - 96.0f) * m_scale.x,m_position.y);
	m_titleButton[1].setPosition(m_position.x + (256.0f - 64.0f) * m_scale.x,m_position.y);
	m_titleButton[2].setPosition(m_position.x + (256.0f - 32.0f) * m_scale.x,m_position.y);
	updateMouseRect();
	//�����������������Ϊ���ɼ�
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->disVisible();
	}
}
void XSubWindow::setNormalSize()
{
	m_windowState = STATE_NORMAL;
	//�ı��������ʾ���ı䰴ť��λ��

	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),m_area.getWidth() - 96.0f,15.0f).c_str());
		
	//m_titleButton[0].setCaptionText("-");
	m_titleButton[0].setSymbol(BTN_SYMBOL_LINE);
	m_titleButton[0].setPosition(m_position.x + (m_area.right - 96.0f) * m_scale.x,m_position.y);
	m_titleButton[1].setPosition(m_position.x + (m_area.right - 64.0f) * m_scale.x,m_position.y);
	m_titleButton[2].setPosition(m_position.x + (m_area.right - 32.0f) * m_scale.x,m_position.y);
	updateMouseRect();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setVisible();
	}
}
void XSubWindow::setColor(const XFColor& c)
{
	m_color = c;
	m_titleFont.setAlpha(c.a);
	m_titleButton[0].setColor(m_color);
	m_titleButton[1].setColor(m_color);
	m_titleButton[2].setColor(m_color);
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setColor(m_color);
	}
}
void XSubWindow::setAlpha(float a)
{
	m_color.setA(a);
	m_titleFont.setAlpha(a);
	m_titleButton[0].setAlpha(a);
	m_titleButton[1].setAlpha(a);
	m_titleButton[2].setAlpha(a);
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setAlpha(a);
	}
}
void XSubWindow::changeStage(XSubWindowState state)
{
	if(!m_isInited || m_windowState == state) return;
	switch(state)
	{
	case STATE_NORMAL:setNormalSize();break;
	case STATE_MINISIZE:setMiniSize();break;
	}
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_STATE_CHANGE);
	else XCtrlManager.eventProc(getControlID(),SWD_STATE_CHANGE);
}
void XSubWindow::setLostFocus()	//����ʧȥ����
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�
	setAlpha(0.5f);
	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
	//��Ҫ�������е�������ʧȥ����
	for (int i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i].obj->setLostFocus();
	}
	if(m_eventProc != 0) m_eventProc(m_pClass,getControlID(),SWD_LOSE_FOCUS);
	else XCtrlManager.eventProc(getControlID(),SWD_LOSE_FOCUS);
}
XBool XSubWindow::saveState(TiXmlNode &e)
{
	if (!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	//�������еĿؼ���������״̬
	TiXmlElement elmNode(m_ctrlName.c_str());
	bool ret = true;
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		if (!m_objects[i].obj->saveState(elmNode)) ret = false;
	}
	if (e.InsertEndChild(elmNode) == NULL) ret = false;
	return ret;
}
XBool XSubWindow::loadState(TiXmlNode *e)
{
	if (!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	//�������еĿռ䣬��ȡ��״̬
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if (keyNode == NULL) return XFalse;
	bool ret = true;
	for (unsigned int i = 0; i < m_objects.size(); ++i)
	{
		if (!m_objects[i].obj->loadState(keyNode)) ret = false;
	}
	return ret;
}
#if !WITH_INLINE_FILE
#include "XSubWindow.inl"
#endif
}