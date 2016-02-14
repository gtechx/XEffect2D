#include "XStdHead.h"
#include "XToolBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XToolBar::setPosition(float x,float y)
{
	m_position.set(x,y);
	float offset = 10.0f;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setPosition(m_position.x + offset * m_scale.x,m_position.y);
		offset += m_objects[i].width;
	}
	m_curMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_scale.x,m_position.y + m_height * m_scale.y);
}
void XToolBar::setScale(float x,float y)
{
	m_scale.set(x,y);
	float offset = 10.0f;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setScale(m_scale * m_objects[i].scale);
		m_objects[i].obj->setPosition(m_position.x + offset * m_scale.x,m_position.y);
		offset += m_objects[i].width;
	}
	m_curMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_scale.x,m_position.y + m_height * m_scale.y);
}
XBool XToolBar::keyboardProc(int,XKeyState)
{
	//if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	//	!m_isActive ||		//û�м���Ŀؼ������տ���
	//	!m_isVisible ||	//������ɼ�ֱ���˳�
	//	!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->keyboardProc(keyOrder,keyState);
	//}
	return XFalse;
}
XBool XToolBar::mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
{
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->mouseProc(x,y,mouseState);
	//}
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if(m_curMouseRect.isInRect(x,y))
		{
			m_isMouseDown = XTrue;
			m_oldMousePosition.set(x,y);
			return XTrue;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isMouseDown) m_isMouseDown = XFalse;
		break;
	case MOUSE_MOVE:
		if(m_isMouseDown)
		{//�����϶��¼�
			XVector2 tmp = XVector2(x,y) - m_oldMousePosition;
			setPosition(m_position.x + tmp.x,m_position.y + tmp.y);
			m_oldMousePosition.set(x,y);
			return XTrue;
		}
		break;
	}
	return XFalse;
}
void XToolBar::addAObject(XControlBasic *obj)
{
	if(obj == NULL) return;
	if(getIndexByP(obj) >= 0) return;//������Ҫ���Ψһ��
	XToolBarUnit tmp;
	tmp.enable = obj->getEnable();
	tmp.scale = obj->getScale();
	tmp.obj = obj;
	tmp.width = obj->getBox(1).x - obj->getBox(0).x;
	tmp.obj->setScale(m_scale * tmp.scale);
	m_objects.push_back(tmp);
	obj->setPosition(m_position.x + m_insertPoint * m_scale.x,m_position.y);
	m_insertPoint += tmp.width;
	//����ע��ָ���ؼ�
//	XCtrlManager.decreaseAObject(obj);	//ע��������
//#if WITH_OBJECT_MANAGER
//	XObjManager.decreaseAObject(obj);
//#endif
}
int XToolBar::getIndexByP(const XControlBasic *obj)
{
	if(obj == NULL) return -1;
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		if(m_objects[i].obj == obj) return i;
	}
	return -1;
}
void XToolBar::releaseMem()
{
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		XMem::XDELETE(m_objects[i].obj);
	}
	m_objects.clear();
}
void XToolBar::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	XRender::drawFillBoxEx(m_position,XVector2(10.0f * m_scale.x,m_height * m_scale.y),0.75f,0.75f,0.75f,true);
	XRender::drawLine(m_position.x + 3.0f * m_scale.x,m_position.y,m_position.x + 3.0f * m_scale.x,m_position.y + m_height * m_scale.y,
		1.0f,0.0f,0.0f,0.0f,1.0f * m_scale.y);
	XRender::drawLine(m_position.x + 7.0f * m_scale.x,m_position.y,m_position.x + 7.0f * m_scale.x,m_position.y + m_height * m_scale.y,
		1.0f,0.0f,0.0f,0.0f,1.0f * m_scale.y);
	//������Ҫ������������
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->draw();
	//}
}
void XToolBar::drawUp()
{
	//if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	//	!m_isVisible) return;	//������ɼ�ֱ���˳�
	//for(unsigned int i = 0;i < m_objects.size();++ i)
	//{
	//	m_objects[i].obj->drawUp();
	//}
}
XBool XToolBar::initWithoutSkin(int height)
{//�����������ظ߶�
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_height = height;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_curMouseRect.set(m_position.x,m_position.y,
		m_position.x + 10.0f * m_scale.x,m_position.y + m_height * m_scale.y);

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XToolBar::release()
{
	if(!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}
void XToolBar::setVisible()
{
	m_isVisible = XTrue;
	m_comment.setVisible();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->setVisible();
	}
	updateChildVisible();
}
void XToolBar::disVisible()
{
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
	m_comment.disVisible();
	for(unsigned int i = 0;i < m_objects.size();++ i)
	{
		m_objects[i].obj->disVisible();
	}
	updateChildVisible();
}
#if !WITH_INLINE_FILE
#include "XToolBar.inl"
#endif
}