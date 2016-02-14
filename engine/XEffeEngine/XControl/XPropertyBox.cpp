#include "XStdHead.h"
#include "XPropertyBox.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XPropertyBox::ctrlProc(void*pClass,int id,int eventID)
{
	XPropertyBox &pPar = *(XPropertyBox *)pClass;
	if(id == pPar.m_slider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE ||
			eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int curIndex = XMath::toInt(pPar.m_slider.getCurValue() * ((int)(pPar.m_propertyLines.size()) - 
				pPar.m_maxLineSum) * 0.01f);
			if(curIndex != pPar.m_curStartLineIndex) 
				pPar.setCurStartLineIndex(curIndex);
		}
		return;
	}
}
void XPropertyBox::updateSliderState()
{
	if(m_propertyLines.size() <= m_maxLineSum && m_maxLineSum >= 0)
	{
		m_curStartLineIndex = 0;
		m_slider.disVisible();
	}else
	{
		m_slider.setVisible();
		if(m_curStartLineIndex > (int)(m_propertyLines.size()) - m_maxLineSum)
			m_curStartLineIndex = (int)(m_propertyLines.size()) - m_maxLineSum;
		m_slider.setCurValue((float)(m_curStartLineIndex) / (float)((int)(m_propertyLines.size()) - m_maxLineSum) * 100.0f);
	}
	updateRect();
}
XBool XPropertyBox::addAPropertyLine(const char * name,XPropertyLineType type,int menuSum,int showMenuSum)
{
	XPropertyLine tmp;
	tmp.type = type;
	switch(tmp.type)
	{
	case PROPERTY_BLANK:
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		if(name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if(tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(),m_width - 10.0f,16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if(m_haveTitle) 
			tmp.pNameFont->setPosition(m_position + 
			XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_EDIT:	//�����
		if(name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if(tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif		
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(),m_nameWidth,16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if(m_haveTitle) 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		{
			XEdit *pTmp = XMem::createMem<XEdit>();
			if(pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin(m_width - m_nameWidth," ");
			XCtrlManager.decreaseAObject(pTmp);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			pTmp->setScale(m_scale);
			if(m_haveTitle) 
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_CHECK:	//ѡ����
		if(name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if(tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif		
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(),m_nameWidth,16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if(m_haveTitle) 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()))));
		{
			XCheck *pTmp = XMem::createMem<XCheck>();
			if(pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin("");
			XCtrlManager.decreaseAObject(pTmp);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			pTmp->setScale(m_scale);
			if(m_haveTitle) 
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	case PROPERTY_COMBO:	//ѡ��
		if(name == NULL) return XFalse;
		tmp.name = name;
		tmp.pNameFont = XMem::createMem<XFontUnicode>();
		if(tmp.pNameFont == NULL) return XFalse;
		tmp.pNameFont->setACopy(m_font);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(tmp.pNameFont);
#endif	
		tmp.pNameFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		tmp.pNameFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		tmp.pNameFont->setColor(XFColor::black);
		tmp.pNameFont->setString(XString::getCanShowString(tmp.name.c_str(),m_nameWidth,16.0f).c_str());
		tmp.pNameFont->setScale(m_scale);
		if(m_haveTitle) 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
		else 
			tmp.pNameFont->setPosition(m_position + 
				XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
		{
			XCombo *pTmp = XMem::createMem<XCombo>();
			if(pTmp == NULL) return XFalse;
			pTmp->initWithoutSkin(m_width - m_nameWidth - MIN_FONT_CTRL_SIZE,menuSum,showMenuSum);
			XCtrlManager.decreaseAObject(pTmp);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(pTmp);
#endif			
			pTmp->setScale(m_scale);
			if(m_haveTitle) 
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * ((int)(m_propertyLines.size()) + 1)));
			else
				pTmp->setPosition(m_position + 
					XVector2(m_nameWidth,MIN_FONT_CTRL_SIZE * m_scale.y * (int)(m_propertyLines.size())));
			tmp.pCtrl = pTmp;
		}
		m_propertyLines.push_back(tmp);
		updateSliderState();
		setCurStartLineIndex(m_curStartLineIndex);
		return XTrue;
	}
	return XFalse;
}
XBool XPropertyBox::setPropertyName(unsigned int index,const char * name)
{
	if(index >= m_propertyLines.size()) return XFalse;
	if(name == NULL) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:return XFalse;	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		m_propertyLines[index].name = name;
		if(m_propertyLines[index].pNameFont != NULL)
			m_propertyLines[index].pNameFont->setString(XString::getCanShowString(m_propertyLines[index].name.c_str(),m_width - 10.0f,16.0f).c_str());
		return XTrue;
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		m_propertyLines[index].name = name;
		if(m_propertyLines[index].pNameFont != NULL)
			m_propertyLines[index].pNameFont->setString(XString::getCanShowString(m_propertyLines[index].name.c_str(),m_nameWidth,16.0f).c_str());
		return XTrue;
	}
	return XFalse;
}
std::string XPropertyBox::getPropertyName(unsigned int index)
{
	if(index >= m_propertyLines.size()) return "";
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:return "";	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return m_propertyLines[index].name;
	case PROPERTY_EDIT:		//�����
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return m_propertyLines[index].name;
	}
	return "";
}
XBool XPropertyBox::setPropertyStr(unsigned int index,const char * str)
{
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setString(str);
		return XTrue;
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
XBool XPropertyBox::setPropertyCtrlPrama(unsigned int index,int prama)
{
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XEdit *)m_propertyLines[index].pCtrl)->setEditType((XEdit::XEditType)prama);
		return XTrue;
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
std::string XPropertyBox::getPropertyStr(unsigned int index)
{
	if(index >= m_propertyLines.size()) return "";
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return "";
	case PROPERTY_EDIT:		//�����
		if(m_propertyLines[index].pCtrl == NULL) return "";
		return ((XEdit *)m_propertyLines[index].pCtrl)->getString();
	case PROPERTY_CHECK:	//ѡ����
	case PROPERTY_COMBO:	//ѡ��
		return "";
	}
	return "";
}
XBool XPropertyBox::setPropertyIndex(unsigned int index,int order)
{
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		return XFalse;
	case PROPERTY_COMBO:	//ѡ��
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setCurChooseOrder(order);
		return XTrue;
	}
	return XFalse;
}
int XPropertyBox::getPropertyIndex(unsigned int index)
{
	if(index >= m_propertyLines.size()) return -1;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return -1;
	case PROPERTY_EDIT:		//�����
		return -1;
	case PROPERTY_CHECK:	//ѡ����
		return -1;
	case PROPERTY_COMBO:	//ѡ��
		if(m_propertyLines[index].pCtrl == NULL) return -1;
		return ((XCombo *)m_propertyLines[index].pCtrl)->getCurChooseOrder();
	}
	return -1;
}
XBool XPropertyBox::setPropertyState(unsigned int index,XBool state)
{
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCheck *)m_propertyLines[index].pCtrl)->setState(state);
		return XTrue;
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
XBool XPropertyBox::getPropertyState(unsigned int index)
{//�����������Ʋ��Ͻ�����Ҫ���ؿ���
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		return ((XCheck *)m_propertyLines[index].pCtrl)->getState();
	case PROPERTY_COMBO:	//ѡ��
		return XFalse;
	}
	return XFalse;
}
XBool XPropertyBox::setPropertyMenuStr(unsigned int index,int menuIndex,const char *str)
{
	if(index >= m_propertyLines.size()) return XFalse;
	switch(m_propertyLines[index].type)
	{
	case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
	case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
		return XFalse;
	case PROPERTY_EDIT:		//�����
		return XFalse;
	case PROPERTY_CHECK:	//ѡ����
		return XFalse;
	case PROPERTY_COMBO:	//ѡ��
		if(m_propertyLines[index].pCtrl == NULL) return XFalse;
		((XCombo *)m_propertyLines[index].pCtrl)->setMenuStr(str,menuIndex);
		return XTrue;
	}
	return XFalse;
}
void XPropertyBox::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	int h = 0;
	if(m_haveTitle) h = 1;
	XFColor tmpColor = XCCS::normalColor * m_color;
	XFColor tmpDColor = XCCS::lightSpecialColor * m_color;
	float w = m_width * m_scale.x;
	XRender::drawFillBoxA(m_position,XVector2(w,
		MIN_FONT_CTRL_SIZE * (m_maxLineSum + h) * m_scale.y),tmpColor,true);
	//�ڻ�������������Ը�����
//	XRender::drawFillBoxA(m_position + XVector2(w,h * MIN_FONT_CTRL_SIZE * m_scale.y),
//		XVector2(MIN_FONT_CTRL_SIZE * m_scale.x,MIN_FONT_CTRL_SIZE * m_maxLineSum * m_scale.y),tmpColor.fR,tmpColor.fG,tmpColor.fB,tmpColor.fA,true);
	if(m_haveTitle)
	{
		if(m_slider.getVisible())
			XRender::drawFillBoxA(m_position,XVector2((m_width + MIN_FONT_CTRL_SIZE) * m_scale.x,MIN_FONT_CTRL_SIZE * m_scale.y),
				tmpColor,true);
		else 
			XRender::drawFillBoxA(m_position,XVector2(w,MIN_FONT_CTRL_SIZE * m_scale.y),
				tmpColor,true);
		m_titleTxt.draw();
	}
	//����������ʾ���е�������
	int size = m_propertyLines.size();
	if(size - m_curStartLineIndex > m_maxLineSum) size = m_maxLineSum + m_curStartLineIndex;
	XFColor tmpLColor = tmpColor;
	for(int i = m_curStartLineIndex;i < size;++ i)
	{
		if(m_propertyLines[i].getIsFocus()) tmpColor = tmpDColor;
		else tmpColor = tmpLColor;
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			break;
		case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_EDIT:		//�����
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(m_nameWidth * m_scale.x,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_CHECK:	//ѡ����
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(w,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(m_nameWidth * m_scale.x,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			m_propertyLines[i].draw();
			break;
		case PROPERTY_COMBO:	//ѡ��
			XRender::drawFillBoxA(m_position + XVector2(0.0f,h * MIN_FONT_CTRL_SIZE * m_scale.y),
				XVector2(m_nameWidth * m_scale.x,MIN_FONT_CTRL_SIZE * m_scale.y),tmpColor,true);
			m_propertyLines[i].draw();
			break;
		}
		++ h;
	}
	m_slider.draw();
}
XBool XPropertyBox::initWithoutSkin(int width,int maxLineSum)
{
	if(m_isInited) return XFalse;
	if(width < 128 || maxLineSum < 1) return XFalse;
	m_width = width;
	m_maxLineSum = maxLineSum;
	m_nameWidth = (width >> 1);
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);

	m_font.setACopy(getDefaultFont());
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_font);
#endif
	m_titleTxt.init();
	m_titleTxt.setPosition(m_position.x + 5.0f,m_position.y);
	m_titleTxt.setColor(0.0f,0.0f,0.0f,1.0f);
	XCtrlManager.decreaseAObject(&m_titleTxt);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_titleTxt);
#endif

	m_slider.initWithoutSkin(XVector2(MIN_FONT_CTRL_SIZE,m_maxLineSum * MIN_FONT_CTRL_SIZE),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_slider.setWithAction(XFalse);
	m_slider.setScale(m_scale);
	m_slider.setEventProc(ctrlProc,this);
	m_slider.disVisible();
	XCtrlManager.decreaseAObject(&m_slider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_slider);
#endif
	setCurStartLineIndex(0);

	updateRect();

	m_isInited = XTrue;
	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
void XPropertyBox::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_titleTxt.setPosition(m_position.x + 5.0f * m_scale.x,m_position.y);
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else 
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
	//�������������������λ��
	setCurStartLineIndex(m_curStartLineIndex);
	updateRect();
}
void XPropertyBox::updateRect()
{
	if(m_haveTitle)
	{
		if(m_slider.getVisible())
			m_curMouseRect.set(m_position.x,m_position.y,
				m_position.x + (m_width + MIN_FONT_CTRL_SIZE) * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * (m_maxLineSum + 1) * m_scale.y);
		else
			m_curMouseRect.set(m_position.x,m_position.y,
				m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * (m_maxLineSum + 1) * m_scale.y);
	}else 
	{
		if(m_slider.getVisible())
			m_curMouseRect.set(m_position.x,m_position.y,
				m_position.x + (m_width + MIN_FONT_CTRL_SIZE) * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_maxLineSum * m_scale.y);
		else
			m_curMouseRect.set(m_position.x,m_position.y,
				m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_maxLineSum * m_scale.y);
	}
}
void XPropertyBox::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_titleTxt.setPosition(m_position.x + 5.0f * m_scale.x,m_position.y);
	m_titleTxt.setScale(m_scale);
	if(m_haveTitle)
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y + MIN_FONT_CTRL_SIZE * m_scale.y);
	else 
		m_slider.setPosition(m_position.x + m_width * m_scale.x,m_position.y);
	m_slider.setScale(m_scale);
	//�������������������λ��
	setCurStartLineIndex(m_curStartLineIndex);
	updateRect();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
			break;
		case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			break;
		case PROPERTY_EDIT:		//�����
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XEdit *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		case PROPERTY_CHECK:	//ѡ����
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XCheck *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		case PROPERTY_COMBO:	//ѡ��
			if(m_propertyLines[i].pNameFont != NULL) 
				m_propertyLines[i].pNameFont->setScale(m_scale);
			if(m_propertyLines[i].pCtrl != NULL)
				((XCombo *)m_propertyLines[i].pCtrl)->setScale(m_scale);
			break;
		}
	}
}
void XPropertyBox::setCurStartLineIndex(unsigned int index)
{//���õ�ǰ��ʼ�еı��
	if(index >= m_propertyLines.size()) return;	//�Ƿ�����ֵ
	m_curStartLineIndex = index;
	//������Ҫ����������������λ��
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		if((i < m_curStartLineIndex) ||
			i >= m_curStartLineIndex + m_maxLineSum)
		{//���ص���Щ����
			switch(m_propertyLines[i].type)
			{
			case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
				break;
			case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
				break;
			case PROPERTY_EDIT:		//�����
				if(m_propertyLines[i].pCtrl != NULL)
					((XEdit *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			case PROPERTY_CHECK:	//ѡ����
				if(m_propertyLines[i].pCtrl != NULL)
					((XCheck *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			case PROPERTY_COMBO:	//ѡ��
				if(m_propertyLines[i].pCtrl != NULL)
					((XCombo *)m_propertyLines[i].pCtrl)->disVisible();
				break;
			}
		}else
		{//������Щ��Ҫ��ʾ��������λ��
			switch(m_propertyLines[i].type)
			{
			case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
				break;
			case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
				if(m_haveTitle) 
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
				else 
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
				break;
			case PROPERTY_EDIT:		//�����
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XEdit *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XEdit *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
				}
				((XEdit *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			case PROPERTY_CHECK:	//ѡ����
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCheck *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCheck *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
				}
				((XCheck *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			case PROPERTY_COMBO:	//ѡ��
				if(m_haveTitle) 
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCombo *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex + 1)));
				}else
				{
					m_propertyLines[i].pNameFont->setPosition(m_position + 
					XVector2(0.0f,MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
					if(m_propertyLines[i].pCtrl != NULL)
						((XCombo *)m_propertyLines[i].pCtrl)->setPosition(m_position + XVector2(m_nameWidth * m_scale.x,
							MIN_FONT_CTRL_SIZE * m_scale.y * (i - m_curStartLineIndex)));
				}
				((XCombo *)m_propertyLines[i].pCtrl)->setVisible();
				break;
			}
		}
	}
}
void XPropertyBox::release()
{
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	//�����ͷ���Դ
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
			break;
		case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			break;
		case PROPERTY_EDIT:		//�����
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		case PROPERTY_CHECK:	//ѡ����
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		case PROPERTY_COMBO:	//ѡ��
			XMem::XDELETE(m_propertyLines[i].pNameFont);
			XMem::XDELETE(m_propertyLines[i].pCtrl);
			break;
		}
	}
	m_propertyLines.clear();
}
void XPropertyBox::setNameWidth(int width)
{
	if(width <= 0 || width >= m_width) return;
	m_nameWidth = width;
	//����Ҫ�������е�����(��δʵ��)
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		switch(m_propertyLines[i].type)
		{
		case PROPERTY_BLANK:	//�հ��У����У�ʲô��û��
			break;
		case PROPERTY_TITLE:	//ֻ��ʾ���ֵ���
			break;
		case PROPERTY_EDIT:		//�����
			if(m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
				m_nameWidth,16.0f).c_str());
			if(m_propertyLines[i].pCtrl != NULL)
				((XEdit *)m_propertyLines[i].pCtrl)->setInputLen(m_width - m_nameWidth);
			break;
		case PROPERTY_CHECK:	//ѡ����
			if(m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
				m_nameWidth,16.0f).c_str());
			if(m_propertyLines[i].pCtrl != NULL)
				//((XCheck *)m_propertyLines[i].pCtrl)->set(m_width - m_nameWidth);
			break;
		case PROPERTY_COMBO:	//ѡ��
			if(m_propertyLines[i].pNameFont != NULL)
				m_propertyLines[i].pNameFont->setString(XString::getCanShowString(m_propertyLines[i].name.c_str(),
				m_nameWidth,16.0f).c_str());
			//���������δʵ��,���Ҫʵ��������ܣ���Ҫ�޸ĺܶ�����Ŀؼ����
			if(m_propertyLines[i].pCtrl != NULL)
				((XCombo *)m_propertyLines[i].pCtrl)->setInputLen(m_width - m_nameWidth - DEFAULT_COMBO_MN_HEIGHT);
			break;
		}
	}
	setCurStartLineIndex(m_curStartLineIndex);
}
XBool XPropertyBox::mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	m_slider.mouseProc(x,y,mouseState);
	bool focusFlag = true;
	if(m_oldFocusIndex >= 0 && m_oldFocusIndex < m_propertyLines.size()
				&& !m_propertyLines[m_oldFocusIndex].canLostFocus(x,y)) focusFlag = false;
	if(focusFlag)
	{
		for(unsigned int i = 0;i < m_propertyLines.size();++ i)
		{
			m_propertyLines[i].mouseProc(x,y,mouseState);
			//������㽹��ı��
			if(m_propertyLines[i].getIsFocus() && i != m_oldFocusIndex)
			{
				if(m_oldFocusIndex >= 0 && m_oldFocusIndex < m_propertyLines.size())
					m_propertyLines[m_oldFocusIndex].setFocus(false);
				m_oldFocusIndex = i;
			}
		}
	}else
		m_propertyLines[m_oldFocusIndex].mouseProc(x,y,mouseState);
	return m_curMouseRect.isInRect(x,y);
}
XBool XPropertyBox::keyboardProc(int keyOrder,XKeyState keyState)			//�����Ƿ񴥷���������
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	m_slider.keyboardProc(keyOrder,keyState);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].keyboardProc(keyOrder,keyState);
	}
	//������Ҫʵ�ּ���ѡ��ؼ�
	if(keyState == KEY_STATE_DOWN)
	{
		switch(keyOrder)
		{
		case XKEY_LCTRL:
			moveFocus(true);
			break;
		case XKEY_RCTRL:
			moveFocus(false);
			break;
		}
	}
	return XTrue;
}
void XPropertyBox::moveFocus(bool flag)
{
	if(m_oldFocusIndex < 0)
	{
		for(unsigned int i = 0;i < m_propertyLines.size();++ i)
		{
			if(m_propertyLines[i].canGetFocus())
			{
				m_oldFocusIndex = i;
				m_propertyLines[i].setFocus(true);
				break;
			}
		}
	}else
	{
		if(flag)
		{
			for(unsigned int i = m_oldFocusIndex + 1;i < m_propertyLines.size();++ i)
			{
				if(m_propertyLines[i].canGetFocus())
				{
					m_propertyLines[m_oldFocusIndex].setFocus(false);
					m_oldFocusIndex = i;
					m_propertyLines[i].setFocus(true);
					break;
				}
			}
		}else
		{
			for(int i = m_oldFocusIndex - 1;i >= 0;-- i)
			{
				if(m_propertyLines[i].canGetFocus())
				{
					m_propertyLines[m_oldFocusIndex].setFocus(false);
					m_oldFocusIndex = i;
					m_propertyLines[i].setFocus(true);
					break;
				}
			}
		}
	}
}
void XPropertyBox::update(float stepTime)
{
	m_comment.update(stepTime);
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].update(stepTime);
	}
}
void XPropertyBox::insertChar(const char *ch,int len)
{
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].insertChar(ch,len);
	}
}
void XPropertyBox::disable()
{
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	m_slider.disable();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].disable();
	}
}
void XPropertyBox::enable()
{
	m_isEnable = XTrue;
	m_isBeChoose = XFalse;
	m_slider.enable();
	for(unsigned int i = 0;i < m_propertyLines.size();++ i)
	{
		m_propertyLines[i].enable();
	}
}
#if !WITH_INLINE_FILE
#include "XPropertyBox.inl"
#endif
}