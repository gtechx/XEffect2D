#include "XText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XText::init(const _XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	m_font.setACopy(font);
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);
	m_font.setPosition(0.0f,0.0f);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_font);
#endif
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return XTrue;
}
void _XText::release()
{
	if(!m_isInited) return;
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = false;
}