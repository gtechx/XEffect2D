#include "XButtonBar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
_XBool _XButtonBar::initWithoutTex(int height)
{
	if(m_isInited) return XFalse;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.f);
	m_insertPoint = 0.0f;
	m_barHeight = height;

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return XTrue;
}
void _XButtonBar::setPosition(float x,float y)
{
	m_position.set(x,y);
	int offset = 0;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_size.x;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
}
void _XButtonBar::setSize(float x,float y)
{
	m_size.set(x,y);
	int offset = 0;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setSize(m_size);
		m_buttons[i].button->setPosition(m_position.x + offset,m_position.y);
		offset += m_buttons[i].width * m_size.x;
	}
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
}
void _XButtonBar::setTextColor(const _XFColor& color)	//�����������ɫ
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setTextColor(color);
	}
}
void _XButtonBar::setColor(float r,float g,float b,float a)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setColor(r,g,b,a);
	}
}
void _XButtonBar::setAlpha(float a)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->setAlpha(a);
	}
}
int _XButtonBar::getIndexByName(const std::string &name)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].name == name) return i;
	}
	return -1;
}
_XBool _XButtonBar::addAButton(const std::string &name,const std::string &caption)
{
	//���������ֵ�Ψһ��
	if(getIndexByName(name) >= 0) return XFalse;
	_XButtonBarCore tmp;
	tmp.button = createMem<_XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutTex(caption.c_str(),_XVector2(m_barHeight,m_barHeight));
	_XCtrlManger.decreaseAObject(tmp.button);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_size.x,m_position.y);
	tmp.button->setSize(m_size);
	m_insertPoint += m_barHeight;
	tmp.width = m_barHeight;
	m_buttons.push_back(tmp);
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
	return XTrue;
}
_XBool _XButtonBar::addAButton(const std::string& name,int width,const std::string &caption)
{
	//���������ֵ�Ψһ��
	if(getIndexByName(name) >= 0) return XFalse;
	_XButtonBarCore tmp;
	tmp.button = createMem<_XButton>();
	if(tmp.button == NULL) return XFalse;
	tmp.name = name;
	tmp.button->initWithoutTex(caption.c_str(),_XVector2(width,m_barHeight));
	_XCtrlManger.decreaseAObject(tmp.button);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(tmp.button);
#endif
	tmp.button->setPosition(m_position.x + m_insertPoint * m_size.x,m_position.y);
	tmp.button->setSize(m_size);
	m_insertPoint += width;
	tmp.width = width;
	m_buttons.push_back(tmp);
	m_nowMouseRect.set(m_position.x,m_position.y,m_position.x + m_insertPoint * m_size.x,
		m_position.y + m_barHeight * m_size.y);
	return XTrue;
}
void _XButtonBar::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible ||	//������ɼ�ֱ���˳�
		m_buttons.size() <= 0) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->draw();
	}
}
void _XButtonBar::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible ||	//������ɼ�ֱ���˳�
		m_buttons.size() <= 0) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->drawUp();
	}
}
void _XButtonBar::update(int stepTime)
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->update(stepTime);
	}
}
void _XButtonBar::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		XDELETE(m_buttons[i].button);
	}
	m_buttons.clear();
	_XCtrlManger.decreaseAObject(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif

	m_isInited = XFalse;
}
void _XButtonBar::disable()
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		m_buttons[i].button->disable();
	}
}
void _XButtonBar::enable()
{
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].isEnable) m_buttons[i].button->enable();
	}
}
_XBool _XButtonBar::mouseProc(float x,float y,_XMouseState mouseState)		//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	!m_isActive ||		//û�м���Ŀؼ������տ���
	!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	_XBool ret = XFalse;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->mouseProc(x,y,mouseState)) ret = XTrue;
	}
	return ret;
}
_XBool _XButtonBar::keyboardProc(int keyOrder,_XKeyState keyState)			//�����Ƿ񴥷���������
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	_XBool ret = XFalse;
	for(int i = 0;i < m_buttons.size();++ i)
	{
		if(m_buttons[i].button->keyboardProc(keyOrder,keyState)) ret = XTrue;
	}
	return ret;
}