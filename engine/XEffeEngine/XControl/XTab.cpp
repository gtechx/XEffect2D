#include "XStdHead.h"
#include "XTab.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XXml.h"
namespace XE{
void XTab::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XTab &pPar = *(XTab *)pClass;
	if(id == pPar.m_leftBtn.getControlID())
	{
		if(pPar.m_curStartTabIndex > 0)
		{
			-- pPar.m_curStartTabIndex;
			pPar.updateTitle();
			pPar.updateBtnState();
		}
	}else
	if(id == pPar.m_rightBtn.getControlID())
	{
		if(pPar.m_curStartTabIndex < (int)(pPar.m_tabObjects.size()) - 1)
		{
			++ pPar.m_curStartTabIndex;
			pPar.updateTitle();
			pPar.updateBtnState();
		}
	}
}
XBool XTab::initWithoutSkin(const XRect &rect,const XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(rect.getWidth() <= 0 || rect.getHeight() <= 0) return XFalse;
	//������ݵĺϷ���
	m_mouseRect = rect;
	m_position.set(0.0f,0.0f);
	m_scale.set(1.0f,1.0f);
	m_curMouseRect.set(m_mouseRect.left * m_scale.x + m_position.x,
		m_mouseRect.top * m_scale.y + m_position.y,
		m_mouseRect.right * m_scale.x + m_position.x,
		m_mouseRect.bottom * m_scale.y + m_position.y);
	m_titleRect.set(m_curMouseRect.left,m_curMouseRect.top,m_curMouseRect.right,m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	if(!m_texFont.setACopy(font)) return XFalse;
	m_texFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_texFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_texFont.setColor(0.0f,0.0f,0.0f,1.0f);
	m_texFont.setScale(m_scale);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_texFont);
#endif
	m_curTextWidth = m_texFont.getTextSize().x * m_texFont.getScale().x * 0.5f;
	m_maxTitleLen = (m_mouseRect.getWidth() - (m_tabBtnWidth << 1) - m_tabTitleDistance) / m_curTextWidth;	//����������󳤶�
	if(m_maxTitleLen < 3) return false;	//����̫С

	m_leftBtn.initWithoutSkin("<", font, XRect(0.0f, 0.0f, m_tabBtnWidth, TAB_TITLE_HEIGHT));
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x),m_curMouseRect.top);
	m_leftBtn.disable();
	m_leftBtn.setEventProc(ctrlProc,this);
	m_leftBtn.setScale(m_scale);
	XCtrlManager.decreaseAObject(&m_leftBtn);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_leftBtn);
#endif
	m_rightBtn.initWithoutSkin(">", font, XRect(0.0f, 0.0f, m_tabBtnWidth, TAB_TITLE_HEIGHT));
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x,m_curMouseRect.top);
	m_rightBtn.disable();
	m_rightBtn.setEventProc(ctrlProc,this);
	m_rightBtn.setScale(m_scale);
	XCtrlManager.decreaseAObject(&m_rightBtn);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_rightBtn);
#endif

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_withoutTex = XTrue;
	m_isInited = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	return XTrue;
}
void XTab::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{//��ʹ����ͼʱ
		//����ǩ�ı���
		XRender::drawFillBox(m_curMouseRect.left,m_curMouseRect.top,
			m_curMouseRect.getWidth(),TAB_TITLE_HEIGHT * m_scale.y,
			XCCS::lightBlackColor * m_color);
		//������ʾ��ǩ
		if(m_withAction && m_isInAction)
		{
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//��ʾ��ǩ����
				if(m_tabObjects[i].needDraw)
				{
					XRender::drawFillBoxExAS(XVector2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
						XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color);
				}
			}
		}else
		{
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//��ʾ��ǩ����
				if(m_tabObjects[i].needDraw && m_curChooseTabIndex != i)
				{
					if(m_tabObjects[i].isEnable)
					{//û�б�ѡ�еı�ǩ
						XRender::drawFillBoxExAS(XVector2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
							XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
							XCCS::disableColor * m_color);
						m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
						//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
					}else
					{
						XRender::drawFillBoxExAS(XVector2(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top),
							XVector2(m_tabObjects[i].len,TAB_TITLE_HEIGHT * m_scale.y),
							XCCS::blackDownColor * m_color);
						m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
						//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
					}
				}
			}
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//��ʾ��ǩ����
				if(m_tabObjects[i].needDraw && m_curChooseTabIndex != i)
				{
				//	m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
					m_tabObjects[i].pFont->draw();
				//	m_texFont.setString(m_tabObjects[i].title.c_str());
				//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
				//	m_texFont.draw();
				}
			}
		}
		//�����������
		XRender::drawFillBoxExAS(XVector2(m_curMouseRect.left,m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y),
			XVector2(m_curMouseRect.getWidth(),m_curMouseRect.getHeight() - TAB_TITLE_HEIGHT * m_scale.y),
			XCCS::disableColor * m_color,true);
		//��汻ѡ�е���
		if(m_withAction && m_isInAction)
		{
			//��涯̬��ע������û�п���Դ���ڲ���ʾ״̬�����
			if(m_tabObjects[m_oldChooseTabIndex].needDraw)
			{
				XRender::drawFillBoxExExA(XVector2(m_curMouseRect.left + 
					XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].offset,m_tabObjects[m_curChooseTabIndex].offset,m_actionMoveData.getCurData()),m_curMouseRect.top),
					XVector2(XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_curChooseTabIndex].len,m_actionMoveData.getCurData()),TAB_TITLE_HEIGHT * m_scale.y),
					XCCS::disableColor * m_color,0x0b,0x00,true);
			}else
			{
				if(m_oldChooseTabIndex < m_curChooseTabIndex)
				{
					float pos = m_curMouseRect.left - m_tabObjects[m_oldChooseTabIndex].len;
					pos = XMath::lineSlerp<float>(pos,m_tabObjects[m_curChooseTabIndex].offset,m_actionMoveData.getCurData());
					float len = XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_curChooseTabIndex].len,m_actionMoveData.getCurData());
					if(pos <= 0)
					{
						len += pos;
						pos = 0;
					}
					XRender::drawFillBoxExExA(XVector2(m_curMouseRect.left + pos,m_curMouseRect.top),
						XVector2(len,TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color,0x0b,0x00,true);
				}else
				{
					float pos = m_curMouseRect.right;
					pos = XMath::lineSlerp<float>(pos,m_tabObjects[m_curChooseTabIndex].offset,m_actionMoveData.getCurData());
					float len = XMath::lineSlerp<float>(m_tabObjects[m_oldChooseTabIndex].len,m_tabObjects[m_curChooseTabIndex].len,m_actionMoveData.getCurData());
					if(pos + len > m_curMouseRect.right)
					{
						len -= m_curMouseRect.right - pos;
					}
					XRender::drawFillBoxExExA(XVector2(m_curMouseRect.left + pos,m_curMouseRect.top),
						XVector2(len,TAB_TITLE_HEIGHT * m_scale.y),
						XCCS::disableColor * m_color,0x0b,0x00,true);
				}
			}
			for(unsigned int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{//��ʾ��ǩ����
				if(m_tabObjects[i].needDraw)
				{
					if(m_curChooseTabIndex != i)
					{
						if(m_oldChooseTabIndex == i)
						{
							float temp = m_actionMoveData.getCurData();
							m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color * XFColor(temp,temp,temp,1.0f));
							//m_texFont.setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
						}else
						{
							m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color);
							//m_texFont.setColor(0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
						}
					}else
					{
						float temp = 1.0f - m_actionMoveData.getCurData();
						m_tabObjects[i].pFont->setColor(XCCS::downColor * m_color * XFColor(temp,temp,temp,1.0f));
						//m_texFont.setColor(0.5f * m_color.fR * temp,0.5f * m_color.fG * temp,0.5f * m_color.fB * temp,m_color.fA);
					}
				//	m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
					m_tabObjects[i].pFont->draw();
				//	m_texFont.setString(m_tabObjects[i].title.c_str());
				//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
				//	m_texFont.draw();
				}
			}
		}else
		{
			if(m_curChooseTabIndex >= m_curStartTabIndex && (m_curChooseTabIndex < m_tabObjects.size() || m_curChooseTabIndex < 0) &&
				m_tabObjects[m_curChooseTabIndex].needDraw)
			{//��ѡ�еı�ǩ
			//	drawFillBoxEx(XVector2(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top),
			//		XVector2(m_tabObjects[m_curChooseTabIndex].len,TAB_TITLE_HEIGHT * m_scale.y),0.35f,0.35f,0.35f,false,false,true);
				XRender::drawFillBoxExExA(XVector2(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top),
					XVector2(m_tabObjects[m_curChooseTabIndex].len,TAB_TITLE_HEIGHT * m_scale.y),
					XCCS::disableColor * m_color,0x0b,0x00,true);
				m_tabObjects[m_curChooseTabIndex].pFont->setColor(XCCS::darkColor * m_color);
			//	m_tabObjects[m_curChooseTabIndex].pFont->setPosition(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top);
				m_tabObjects[m_curChooseTabIndex].pFont->draw();
			//	m_texFont.setColor(0.0f,0.0f,0.0f,m_color.fA);
			//	m_texFont.setString(m_tabObjects[m_curChooseTabIndex].title.c_str());
			//	m_texFont.setPosition(m_curMouseRect.left + m_tabObjects[m_curChooseTabIndex].offset,m_curMouseRect.top);
			//	m_texFont.draw();
			}
		}
	}
	m_leftBtn.draw();
	m_rightBtn.draw();

	m_ctrlManager.draw();
}
void XTab::updateBtnState()	//���°�ť��״̬
{//���ɼ������������ʾ��Χ��
	int startIndex = -1;
	int endIndex = -1;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(m_tabObjects[i].isVisible)
		{
			startIndex = i;
			break;
		}
	}
	for(int i = (int)(m_tabObjects.size()) - 1;i >= 0;-- i)
	{
		if(m_tabObjects[i].isVisible)
		{
			endIndex = i;
			break;
		}
	}
	if(m_tabObjects.size() <= 0 || startIndex < 0 || 
		(m_tabObjects[startIndex].needDraw && m_tabObjects[endIndex].needDraw))
	{//����Ҫ��ʾ���Ұ�ť
		m_leftBtn.disVisible();
		m_rightBtn.disVisible();
	}else
	{
		m_leftBtn.setVisible();
		m_rightBtn.setVisible();
		if(m_curStartTabIndex <= startIndex) m_leftBtn.disable();
		else m_leftBtn.enable();
		if(m_tabObjects[endIndex].needDraw) m_rightBtn.disable();
		else m_rightBtn.enable();
	}
}
void XTab::updateTitle()	//���±������ʾ
{
	int pixelOffset = 0;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(i < m_curStartTabIndex)
		{
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		if(!m_tabObjects[i].isVisible)
		{//���ɼ������
			m_tabObjects[i].needDraw = XFalse;
			continue;
		}
		//m_texFont.setString(m_tabObjects[i].title.c_str());
		//m_tabObjects[i].len = m_texFont.getMaxPixelWidth();
		m_tabObjects[i].len = m_tabObjects[i].pFont->getMaxPixelWidth();
		if(pixelOffset + m_tabObjects[i].len + m_tabTitleDistance * m_scale.x <= m_curMouseRect.getWidth() - ((m_tabBtnWidth << 1) * m_scale.x))
		{//��Ҫ���
			m_tabObjects[i].needDraw = XTrue;
			m_tabObjects[i].offset = pixelOffset;
			m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
			pixelOffset += m_tabObjects[i].len + m_tabTitleDistance * m_scale.x;
		}else
		{
			m_tabObjects[i].needDraw = XFalse;
			pixelOffset += m_tabObjects[i].len + m_tabTitleDistance * m_scale.x;
		}
	}
}
XBool XTab::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	m_ctrlManager.mouseProc(x,y,mouseState);
	if(m_titleRect.isInRect(x,y))
	{
		m_leftBtn.mouseProc(x,y,mouseState);
		m_rightBtn.mouseProc(x,y,mouseState);
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{
			XRect tmpRect;
			tmpRect.top = m_curMouseRect.top;
			tmpRect.bottom = m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y;
			for(int i = m_curStartTabIndex;i < m_tabObjects.size();++ i)
			{
				if(m_tabObjects[i].needDraw && m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
				{
					tmpRect.left = m_tabObjects[i].offset + m_curMouseRect.left;
					tmpRect.right = m_tabObjects[i].offset + m_curMouseRect.left + m_tabObjects[i].len;
					if(tmpRect.isInRect(x,y) && i != m_curChooseTabIndex)	//�Ѿ���ѡ�в����ٴα�ѡ��
					{
						startAction();
						m_curChooseTabIndex = i;
						if(m_withAction)
						{
							for(unsigned int j = 0;j < m_tabObjects[m_curChooseTabIndex].objs.size();++ j)
							{
								m_tabObjects[m_curChooseTabIndex].objs[j]->setAlpha(m_actionMoveData.getCurData());
								m_ctrlManager.setNeedDrawAndInput(true,true,m_tabObjects[m_curChooseTabIndex].objs[j]);
							}
						}else
						{
							updateObjState();
						}
						//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[i].srcTitle);
						m_curChooseTabStr = m_tabObjects[i].srcTitle;
						if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,TAB_CHOOSE);
						else XCtrlManager.eventProc(m_objectID,TAB_CHOOSE);
						break;
					}
				}
			}
		}
		return XTrue;
	}
	return XFalse;
}
XBool XTab::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	!m_isActive ||		//û�м���Ŀؼ������տ���
	!m_isVisible ||	//������ɼ�ֱ���˳�
	!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	m_ctrlManager.keyProc(keyOrder,keyState);
	if(keyState == KEY_STATE_UP && m_isBeChoose)
	{//��������ʱ������Ӧ
		switch(keyOrder)
		{
		case XKEY_LEFT:
			if(m_curChooseTabIndex > 0) 
			{
				startAction();
				-- m_curChooseTabIndex;
				if(m_curStartTabIndex > m_curChooseTabIndex) m_curStartTabIndex = m_curChooseTabIndex;
				updateTitle();	//���±������ʾ
				updateBtnState();	//���°�ť��״̬
				if(m_withAction)
				{
					for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
					{
						m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
						m_ctrlManager.setNeedDrawAndInput(true,true,m_tabObjects[m_curChooseTabIndex].objs[i]);
					}
				}else
				{
					updateObjState();
				}
				//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_curChooseTabIndex].srcTitle);
				m_curChooseTabStr = m_tabObjects[m_curChooseTabIndex].srcTitle;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,TAB_CHOOSE);
				else XCtrlManager.eventProc(m_objectID,TAB_CHOOSE);
			}
			return XTrue;
		case XKEY_RIGHT:
			if(m_curChooseTabIndex < (int)(m_tabObjects.size()) - 1)
			{
				startAction();
				++ m_curChooseTabIndex;
				//if(m_curStartTabIndex < m_curChooseTabIndex) m_curStartTabIndex = m_curChooseTabIndex;
				updateTitle();	//���±������ʾ
				updateBtnState();	//���°�ť��״̬
				while(true)
				{//�ƶ�ֱ����ʾ����(������ǩʱ�������ִ���)
					if(!m_tabObjects[m_curChooseTabIndex].needDraw && m_curStartTabIndex < m_curChooseTabIndex)
					{
						++ m_curStartTabIndex;
						updateTitle();	//���±������ʾ
						updateBtnState();	//���°�ť��״̬
					}else
					{
						break;
					}
				}
				if(m_withAction)
				{
					for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
					{
						m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
						m_ctrlManager.setNeedDrawAndInput(true,true,m_tabObjects[m_curChooseTabIndex].objs[i]);
					}
				}else
				{
					updateObjState();
				}
				//if(m_funChooseTab != NULL) m_funChooseTab(m_pClass,m_objectID,m_tabObjects[m_curChooseTabIndex].srcTitle);
				m_curChooseTabStr = m_tabObjects[m_curChooseTabIndex].srcTitle;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,TAB_CHOOSE);
				else XCtrlManager.eventProc(m_objectID,TAB_CHOOSE);
			}
			return XTrue;
			break;
		}
	}
	return XFalse;

}
void XTab::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_curMouseRect.set(m_mouseRect.left * m_scale.x + m_position.x,
		m_mouseRect.top * m_scale.y + m_position.y,
		m_mouseRect.right * m_scale.x + m_position.x,
		m_mouseRect.bottom * m_scale.y + m_position.y);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
		//	m_tabObjects[i].objs[j]->setPosition(XVector2(m_tabObjects[i].pos[j].x * m_scale.x,m_tabObjects[i].pos[j].y * m_scale.y) + m_position);
			m_tabObjects[i].objs[j]->setPosition(m_tabObjects[i].pos[j] * m_scale + m_position);
		}
		if(m_tabObjects[i].needDraw)
			m_tabObjects[i].pFont->setPosition(m_curMouseRect.left + m_tabObjects[i].offset,m_curMouseRect.top);
	}
	m_titleRect.set(m_curMouseRect.left,m_curMouseRect.top,m_curMouseRect.right,m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x),m_curMouseRect.top);
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x,m_curMouseRect.top);
	//����Ƿ���Ҫ�����������
}
void XTab::setScale(float x,float y)
{
	if(x <= 0.0f || y <= 0.0f) return;
	m_scale.set(x,y);
	//��������������
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
		//	m_tabObjects[i].objs[j]->setPosition(XVector2(m_tabObjects[i].pos[j].x * m_scale.x,m_tabObjects[i].pos[j].y * m_scale.y) + m_position);
			m_tabObjects[i].objs[j]->setPosition(m_tabObjects[i].pos[j] * m_scale + m_position);
			m_tabObjects[i].objs[j]->setScale(m_tabObjects[i].scale[j] * m_scale);
			m_tabObjects[i].pFont->setScale(m_scale);
		}
	}
	m_texFont.setScale(m_scale);
	m_curMouseRect.set(m_mouseRect.left * m_scale.x + m_position.x,
		m_mouseRect.top * m_scale.y + m_position.y,
		m_mouseRect.right * m_scale.x + m_position.x,
		m_mouseRect.bottom * m_scale.y + m_position.y);
	m_titleRect.set(m_curMouseRect.left,m_curMouseRect.top,m_curMouseRect.right,m_curMouseRect.top + TAB_TITLE_HEIGHT * m_scale.y);
	m_leftBtn.setPosition(m_curMouseRect.right - ((m_tabBtnWidth << 1) * m_scale.x),m_curMouseRect.top);
	m_leftBtn.setScale(m_scale);
	m_rightBtn.setPosition(m_curMouseRect.right - m_tabBtnWidth * m_scale.x,m_curMouseRect.top);
	m_rightBtn.setScale(m_scale);
	updateTitle();
	updateBtnState();
}
int XTab::getTabIndexByTitle(const std::string &title) const
{
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(m_tabObjects[i].srcTitle == title) return i;
	}
	return -1;
}
void XTab::updateObjState(bool flag)
{
	if(!m_isVisible) return;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(i == m_curChooseTabIndex)
		{
			if(!flag) continue;
			for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
			{
				m_ctrlManager.setNeedDrawAndInput(true,true,m_tabObjects[i].objs[j]);
				//m_tabObjects[i].objs[j]->setVisible();
			}
		}else
		{
			//���ﻻһ�ֿ��Ʒ�ʽ���ı�ؼ����ڲ�״̬������ͨ����������������״̬
			for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
			{
				m_ctrlManager.setNeedDrawAndInput(false,false,m_tabObjects[i].objs[j]);
				//m_tabObjects[i].objs[j]->disVisible();
			}
		}
	}
}
void XTab::release()
{
	if(!m_isInited) return;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		XMem::XDELETE(m_tabObjects[i].pFont);
	}
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_ctrlManager.clearAllObject();	//������Ҫ������е����
	m_isInited = XFalse;
}
void XTab::setTabVisible(bool flag,int index)	//���ñ�ǩ�ɼ�
{
	if(index < 0 || index >= m_tabObjects.size()) return;
	if((m_tabObjects[index].isVisible && flag) ||
		(!m_tabObjects[index].isVisible && !flag)) return;	//�ظ�������
	m_tabObjects[index].isVisible = flag;

	if(!flag)		//��Ҫ�ж����ֵ
	{//�������û�п���ʾ��ֵ����˵���һ��������ʾ��ֵ
		int endIndex = 0;
		for(int i = (int)(m_tabObjects.size()) - 1;i >= 0;-- i)
		{
			if(m_tabObjects[i].isVisible)
			{
				endIndex = i;
				break;
			}
		}
		if(m_curStartTabIndex > endIndex) 
		{
			m_curStartTabIndex = endIndex;
		}
	}
	if((m_curChooseTabIndex == index && !flag)
		|| (m_curChooseTabIndex < 0 && flag)) 
	{//����Ӧ��ѡ���һ������ѡ��
		m_curChooseTabIndex = -1;	//��Ҫ�ж����ֵ
		for(unsigned int i = 0;i < m_tabObjects.size();++ i)
		{
			if(m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_curChooseTabIndex = i;
				break;
			}
		}
	}
	//���½���
	updateTitle();	//���±������ʾ
	updateBtnState();	//���°�ť��״̬
	updateObjState();
}
void XTab::setTabEnable(bool flag,unsigned int index)
{
	if(index >= m_tabObjects.size()) return;
	if((m_tabObjects[index].isEnable && flag) ||
		(!m_tabObjects[index].isEnable && !flag)) return;	//�ظ�������
	m_tabObjects[index].isEnable = flag;

	if((m_curChooseTabIndex == index && !flag)
		|| (m_curChooseTabIndex < 0 && flag)) 
	{//����Ӧ��ѡ���һ������ѡ��
		m_curChooseTabIndex = -1;	//��Ҫ�ж����ֵ
		for(unsigned int i = 0;i < m_tabObjects.size();++ i)
		{
			if(m_tabObjects[i].isEnable && m_tabObjects[i].isVisible)
			{
				m_curChooseTabIndex = i;
				break;
			}
		}
	}
	//���½���
	updateTitle();	//���±������ʾ
	updateBtnState();	//���°�ť��״̬
	updateObjState();
}
void XTab::update(float stepTime)
{//�����Ƕ�������ʾ
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	if(m_isInAction)
	{
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
		{
			m_isInAction = XFalse;
			for(unsigned int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(1.0f);
			}
			updateObjState(false);
		}else
		{
			for(unsigned int i = 0;i < m_tabObjects[m_oldChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_oldChooseTabIndex].objs[i]->setAlpha(1.0f - m_actionMoveData.getCurData());
			}
			for(unsigned int i = 0;i < m_tabObjects[m_curChooseTabIndex].objs.size();++ i)
			{
				m_tabObjects[m_curChooseTabIndex].objs[i]->setAlpha(m_actionMoveData.getCurData());
			}
		}
	}
	m_ctrlManager.update(stepTime);
}
void XTab::addATab(const std::string &title)
{
	if(!m_isInited) return;
	if(getTabIndexByTitle(title) >= 0) return;	//title����
	//������Ҫ��������ǩ
	XTabObject tmp;
	tmp.pFont = XMem::createMem<XFontUnicode>();
	if(tmp.pFont == NULL) return;
	tmp.pFont->setACopy(m_texFont);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(tmp.pFont);
#endif	
	tmp.srcTitle = title;
	if(title.length() > m_maxTitleLen || m_maxTitleLen < 0)
	{//������Ҫ�ضϳ����ַ���
		char *tmpStr = XMem::createArrayMem<char>(m_maxTitleLen + 1);
		if(tmpStr == NULL) 
		{
			XMem::XDELETE(tmp.pFont);
			return;
		}
		memcpy(tmpStr,title.c_str(),m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if(XString::isAtUnicodeEnd(tmpStr,m_maxTitleLen - 3))
		{
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '.';
		}else
		{
			tmpStr[m_maxTitleLen - 3] = '.';
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '\0';
		}
		tmp.title = tmpStr;
		XMem::XDELETE_ARRAY(tmpStr);
	}else
	{
		tmp.title = title;
	}
	tmp.pFont->setString(tmp.title.c_str());
	tmp.isEnable = true;
	m_tabObjects.push_back(tmp);
	updateTitle();
	updateBtnState();
}
XBool XTab::setTabStr(const char *str,unsigned int index)
{
	if(str == NULL || index >= m_tabObjects.size()) return XFalse;
	if(getTabIndexByTitle(str) >= 0) return XFalse;	//title����
	m_tabObjects[index].srcTitle = str;
	if(strlen(str) > m_maxTitleLen || m_maxTitleLen < 0)
	{//������Ҫ�ضϳ����ַ���
		char *tmpStr = XMem::createArrayMem<char>(m_maxTitleLen + 1);
		if(tmpStr == NULL) return XFalse;
		memcpy(tmpStr,str,m_maxTitleLen);
		tmpStr[m_maxTitleLen] = '\0';
		if(XString::isAtUnicodeEnd(tmpStr,m_maxTitleLen - 3))
		{
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '.';
		}else
		{
			tmpStr[m_maxTitleLen - 3] = '.';
			tmpStr[m_maxTitleLen - 2] = '.';
			tmpStr[m_maxTitleLen - 1] = '\0';
		}
		m_tabObjects[index].title = tmpStr;
		XMem::XDELETE_ARRAY(tmpStr);
	}else
	{
		m_tabObjects[index].title = str;
	}
	m_tabObjects[index].pFont->setString(m_tabObjects[index].title.c_str());
	updateTitle();
	updateBtnState();
	return XTrue;
}
XBool XTab::setTabsStr(const char *str)		//;��Ϊ�ָ���
{
	if(str == NULL) return XFalse;
	//������Ҫ������ݵĺϷ��ԣ���ֹ�ظ��ı�������(��δ���)
	int index = 0;
	int offset;
	char *tmp = XMem::createArrayMem<char>(strlen(str) + 1);
	strcpy(tmp,str);
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(index == 0) offset = XString::getCharPosition(tmp + index,';');
		else offset = XString::getCharPosition(tmp + index + 1,';');
		if(offset > 0)
		{
			if(index == 0)
			{
				tmp[index + offset] = '\0';
				setTabStr(tmp + index,i);
				index += offset;
			}else
			{
				tmp[index + offset + 1] = '\0';
				setTabStr(tmp + index + 1,i);
				index += offset + 1;
			}
		}else
		{
			if(tmp[index + 1] == '\0') break;
			else
			{
				setTabStr(tmp + index + 1,i);
				break;
			}
		}
	}
	XMem::XDELETE_ARRAY(tmp);
	return XTrue;
}
void XTab::clearAllObj()
{
	//�����пؼ��Ŀ���Ȩ�����ؼ�������
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		for(unsigned int j = 0;j < m_tabObjects[i].objs.size();++ j)
		{
			XCtrlManager.addACtrl(m_tabObjects[i].objs[j]);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
			XObjManager.addAObject(m_tabObjects[i].objs[j]);
#endif
		}
		m_ctrlManager.clearAllObject();
		m_tabObjects[i].objs.clear();
		m_tabObjects[i].pos.clear();
		m_tabObjects[i].scale.clear();
	}
}
void XTab::delObjFromTab(XControlBasic *obj,int index)
{
	if(obj == NULL || index >= m_tabObjects.size()) return;

	bool find = false;
	for(int i = 0;i < m_tabObjects[index].objs.size();++ i)
	{
		if(m_tabObjects[index].objs[i] == obj)
		{
			m_tabObjects[index].objs.erase(m_tabObjects[index].objs.begin() + i);
			m_tabObjects[index].pos.erase(m_tabObjects[index].pos.begin() + i);
			m_tabObjects[index].scale.erase(m_tabObjects[index].scale.begin() + i);
			find = true;
			break;
		}
	}
	if(!find) return;	//���û���ҵ�ָ���ģ���ֱ�ӷ���
	updateObjState();
	XCtrlManager.addACtrl(obj);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(obj);
#endif
	m_ctrlManager.decreaseAObject(obj);
}
void XTab::addObjToTab(XControlBasic *obj,unsigned int index)	//��һ����ǩ��������
{
	if(obj == NULL || index >= m_tabObjects.size()) return;
	for(int i = 0;i < m_tabObjects[index].objs.size();++ i)
	{
		if(m_tabObjects[index].objs[i] == obj)
			return;	//��ֹ�ظ����
	}

	m_tabObjects[index].objs.push_back(obj);
	m_tabObjects[index].pos.push_back((obj->getPosition() - m_position) / m_scale);
	m_tabObjects[index].scale.push_back(obj->getScale() / m_scale);
	//updateObjState();
	XCtrlManager.decreaseAObject(obj);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(obj);
#endif
	m_ctrlManager.addACtrl(obj);
	//��������Ż�
	if(index == m_curChooseTabIndex)
		m_ctrlManager.setNeedDrawAndInput(true,true,m_ctrlManager.getObjSum() - 1);
	else 
		m_ctrlManager.setNeedDrawAndInput(false,false,m_ctrlManager.getObjSum() - 1);
	//if(index == m_curChooseTabIndex)
	//	m_ctrlManager.setNeedDrawAndInput(true,true,obj);
	//else 
	//	m_ctrlManager.setNeedDrawAndInput(false,false,obj);
}
XBool XTab::saveState(TiXmlNode &e)
{
	TiXmlElement elmNode(m_ctrlName.c_str());
	XBool flag = XTrue;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(!m_tabObjects[i].saveState(elmNode)) flag = XFalse;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = XFalse;
	return flag;
}
XBool XTab::loadState(TiXmlNode *e)
{
	//�������еĿռ䣬��ȡ��״̬
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if(keyNode == NULL) return XFalse;
	XBool flag = XTrue;
	for(unsigned int i = 0;i < m_tabObjects.size();++ i)
	{
		if(!m_tabObjects[i].loadState(keyNode)) flag = XFalse;
	}
	return flag;
}
XBool XTabObject::saveState(TiXmlNode &e)
{
	TiXmlElement elmNode(srcTitle.c_str());
	bool flag = true;
	for(unsigned int i = 0;i < objs.size();++ i)
	{
		if(objs[i] != NULL && 
			!objs[i]->saveState(elmNode)) flag = false;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = false;
	return flag;
}
XBool XTabObject::loadState(TiXmlNode *e)
{
	//�������еĿռ䣬��ȡ��״̬
	TiXmlNode * keyNode = e->FirstChildElement(srcTitle.c_str());
	if(keyNode == NULL) return XFalse;
	bool flag = true;
	for(unsigned int i = 0;i < objs.size();++ i)
	{
		if(objs[i] != NULL && 
			!objs[i]->loadState(keyNode)) flag = false;
	}
	return flag;
}
#if !WITH_INLINE_FILE
#include "XTab.inl"
#endif
}