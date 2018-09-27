#include "XStdHead.h"
#include "XColorChoose.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XFile.h"
#include "../XMath/XCColor.h"
namespace XE{
XBool XColorChoose::initWithoutSkin(const XVec2& size,const XCColor &color)
{
	if(m_isInited) return XFalse;
	m_area = size;
	if(m_area.x < 32.0f) m_area.x = 32.0f;
	if(m_area.y < 32.0f) m_area.y = 32.0f;
	m_curColor = color;
	m_scale.set(1.0f);
	m_position.reset();
	m_font.setACopy(getDefaultFont());
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif	
	char tmpStr[32];
	sprintf_s(tmpStr,32,"%02X%02X%02X",color.r,color.g,color.b);
	m_font.setString(tmpStr);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_font.setPosition(m_position.x + 2,m_position.y + m_area.y * 0.5f);
	m_font.setColor(XFColor::black);

	m_curMouseRect.set(m_position,
		m_position + m_area * m_scale);

	m_isVisible = m_isEnable = m_isActive = m_isInited = XTrue;
	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XColorChoose::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	XRender::drawFillRectA(m_position,m_area * m_scale,
		XCCS::normalColor * m_color,true);
	if(m_type == TYPE_WITH_TEXT)
	{
		m_font.draw();
		XRender::drawFillRectA(m_position + XVec2(96,3) * m_scale,XVec2(m_area.x - 100,m_area.y - 6) * m_scale,
			m_curColor,true);
	}else
	{
		XRender::drawFillRectA(m_position + XVec2(3,3),XVec2(m_area.x - 6,m_area.y - 6) * m_scale,
			m_curColor,true);
	}
}
XBool XColorChoose::mouseProc(const XVec2& p,XMouseState mouseState)		//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;
	if(m_curMouseRect.isInRect(p))
	{
	//	if(mouseState == MOUSE_LEFT_BUTTON_DOWN ||
	//		mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		if (mouseState == MOUSE_LEFT_BUTTON_UP)
		{
			m_needChooseColor = true;
			return XTrue;
		}
	}
	return XFalse;
}
void XColorChoose::updateColor()
{
	char tmpStr[32];
	XCColor tmp = m_curColor;
	sprintf_s(tmpStr,32,"%02X%02X%02X",tmp.r,tmp.g,tmp.b);
	m_font.setString(tmpStr);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CLC_COLOR_CHANGE);
	else XCtrlManager.eventProc(m_objectID,CLC_COLOR_CHANGE);
}
void XColorChoose::update(float/*stepTime*/)
{
	if(!m_isInited) return;
	if(m_needChooseColor)
	{
		m_needChooseColor = false;
		XCColor ret = m_curColor;
		ret = XFile::getChooseColor(ret);
		m_curColor = ret;
		updateColor();
	}
}
void XColorChoose::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
XBool XColorChoose::setACopy(const XColorChoose &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	if(!m_font.setACopy(temp.m_font))	 return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_isInited = temp.m_isInited;
	m_curColor = temp.m_curColor;	//��ǰ����ɫ
	m_area = temp.m_area;		//��Χ

	m_type = temp.m_type;
	m_needChooseColor = temp.m_needChooseColor;
	return XTrue;
}
}