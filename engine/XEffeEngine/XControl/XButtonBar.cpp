#include "XStdHead.h"
#include "XButtonBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XButtonBar::initWithoutSkin(int height)
{
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.f);
	m_insertPoint = 0.0f;
	m_barHeight = height;

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_scale.x,
		m_position.y + m_barHeight * m_scale.y);

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XButtonBar::setPosition(float x,float y)
{
	m_position.set(x,y);
	int offset = 0;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_scale.x;
	}
	m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_scale.x,
		m_position.y + m_barHeight * m_scale.y);
}
void XButtonBar::setScale(float x,float y)
{
	m_scale.set(x,y);
	int offset = 0;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setScale(m_scale);
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_scale.x;
	}
	m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_scale.x,
		m_position.y + m_barHeight * m_scale.y);
}
void XButtonBar::setTextColor(const XFColor& color)	//�����������ɫ
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setTextColor(color);
	}
}
void XButtonBar::setColor(float r,float g,float b,float a)
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setColor(r,g,b,a);
	}
}
void XButtonBar::setAlpha(float a)
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setAlpha(a);
	}
}
int XButtonBar::getIndexByName(const std::string &name)
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].name == name) return i;
	}
	return -1;
}
XBool XButtonBar::addAButton(const std::string &name,const std::string &caption)
{
	//���������ֵ�Ψһ��
	if(getIndexByName(name) >= 0) return XFalse;
	XButtonBarCore tmp;
	tmp.button = XMem::createMem<XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutSkin(caption.c_str(),XVector2(m_barHeight,m_barHeight));
	XCtrlManager.decreaseAObject(tmp.button);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_scale.x,m_position.y);
	tmp.button->setScale(m_scale);
	m_insertPoint += m_barHeight;
	tmp.width = m_barHeight;
	m_buttons.push_back(tmp);
	m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_scale.x,
		m_position.y + m_barHeight * m_scale.y);
	return XTrue;
}
XBool XButtonBar::addAButton(const std::string& name,int width,const std::string &caption)
{
	//���������ֵ�Ψһ��
	if(getIndexByName(name) >= 0) return XFalse;
	XButtonBarCore tmp;
	tmp.button = XMem::createMem<XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutSkin(caption.c_str(),XVector2(width,m_barHeight));
	XCtrlManager.decreaseAObject(tmp.button);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_scale.x,m_position.y);
	tmp.button->setScale(m_scale);
	m_insertPoint += width;
	tmp.width = width;
	m_buttons.push_back(tmp);
	m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_scale.x,
		m_position.y + m_barHeight * m_scale.y);
	return XTrue;
}
void XButtonBar::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible ||	//������ɼ�ֱ���˳�
		m_buttons.size() <= 0) return;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->draw();
	}
}
void XButtonBar::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible ||	//������ɼ�ֱ���˳�
		m_buttons.size() <= 0) return;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->drawUp();
	}
}
void XButtonBar::update(float stepTime)
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->update(stepTime);
	}
}
void XButtonBar::release()
{
	if(!m_isInited) return;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		XMem::XDELETE(m_buttons[i].button);
	}
	m_buttons.clear();
	XCtrlManager.decreaseAObject(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif

	m_isInited = XFalse;
}
void XButtonBar::disable()
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->disable();
	}
}
void XButtonBar::enable()
{
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].isEnable) m_buttons[i].button->enable();
	}
}
XBool XButtonBar::mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	!m_isActive ||		//û�м���Ŀؼ������տ���
	!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	XBool ret = XFalse;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->mouseProc(x,y,mouseState)) ret = XTrue;
	}
	return ret;
}
XBool XButtonBar::keyboardProc(int keyOrder,XKeyState keyState)			//�����Ƿ񴥷���������
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	XBool ret = XFalse;
	for(unsigned int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->keyboardProc(keyOrder,keyState)) ret = XTrue;
	}
	return ret;
}
#if !WITH_INLINE_FILE
#include "XButtonBar.inl"
#endif
}