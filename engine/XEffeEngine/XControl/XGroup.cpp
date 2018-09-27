#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XGroup.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE {
void XGroup::ctrlProc(void *pClass, int id, int eventID)
{
	if (eventID != XButton::BTN_MOUSE_DOWN) return;
	XGroup & pPar = *(XGroup *)pClass;
	if (id == pPar.m_stateBotton.getControlID())
	{
		if (pPar.m_state == STATE_NORMAL) pPar.setState(STATE_MINISIZE);
		else pPar.setState(STATE_NORMAL);
	}
}
XBool XGroup::init(const XVec2& position,
	const XRect& rect,
	const char *caption, const XFontUnicode& font, float captionSize)
{
	if (m_isInited) return XFalse;	//��ֹ�ظ���ʼ��

	m_position = position;
	m_rect = rect;
	m_textSize.set(captionSize);
	m_scale.set(1.0f);
	if (!m_caption.setACopy(font)) return XFalse;
	m_caption.setScale(captionSize);
	m_caption.setString(caption);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

	m_state = STATE_NORMAL;
	if (!m_stateBotton.initWithoutSkin("", font, captionSize, XRect(0.0f, m_groupStateBtnSize),
		XVec2(m_groupStateBtnSize * 0.5f))) return XFalse;
	m_stateBotton.setEventProc(ctrlProc, this);
	m_stateBotton.setSymbol(BTN_SYMBOL_LINE);
	updateData();

	//XCtrlManager.decreaseAObject(&m_stateBotton);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
	XObjManager.decreaseAObject(&m_stateBotton);
#endif
	//XCtrlManager.decreaseAObject(&m_stateBotton);
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isVisible = m_isEnable = m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void XGroup::draw()
{
	if (!m_isInited ||
		!m_isVisible) return;	//���û�г�ʼ��ֱ���˳�
	if (m_withRect)
	{
		if (m_state == STATE_NORMAL)
		{
			int h = m_caption.getMaxPixelHeight();
			int w = m_caption.getMaxPixelWidth();

			XRender::drawLine(m_drawRect.left, m_drawRect.top + (h >> 1), m_drawRect.left, m_drawRect.bottom, 1, m_color);
			XRender::drawLine(m_drawRect.right, m_drawRect.top + (h >> 1), m_drawRect.right, m_drawRect.bottom, 1, m_color);
			XRender::drawLine(m_drawRect.getLB(), m_drawRect.getRB(), 1, m_color);

			//drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left + 20.0f * m_scale.x ,m_drawRect.top + (h >> 1),1);
			if (m_drawRect.getWidth() > w + m_groupStateBtnSize + 10.0f)
				XRender::drawLine(m_drawRect.left + (m_groupStateBtnSize + 10.0f) * m_scale.x + w, m_drawRect.top + (h >> 1),
					m_drawRect.right, m_drawRect.top + (h >> 1), 1, m_color);
		}
		else
		{
			XRender::drawRect(m_drawRect, 1, m_color);
			//XRender::drawLine(m_drawRect.getLT(),m_drawRect.getLB(),1,m_color);
			//XRender::drawLine(m_drawRect.getRT(),m_drawRect.getRB(),1,m_color);
			//XRender::drawLine(m_drawRect.getLB(),m_drawRect.getRB(),1,m_color);
			//XRender::drawLine(m_drawRect.getLT(),m_drawRect.getRT(),1,m_color);
		}
	}
	m_caption.draw();
	//	m_stateBotton.draw();
}
XBool XGroup::setACopy(const XGroup & temp)
{
	if (&temp == this) return XTrue;	//��ֹ����ֵ
	if (!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;
	if (!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//�ؼ���λ��
	m_scale = temp.m_scale;		//��С
	m_rect = temp.m_rect;			//�ؼ��ķ�Χ
	m_withRect = temp.m_withRect;			//�ؼ��ķ�Χ
//	m_funStateChange = temp.m_funStateChange;	//״̬�����ı�ʱ���õĺ���
//	m_pClass = temp.m_pClass;

	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_textSize = temp.m_textSize;

	return XTrue;
}
void XGroup::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
void XGroup::setState(XGroupState state)
{
	if (m_state == state) return;
	m_state = state;
	//if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);	//ע���������˳��������ڿɼ�״̬�ı�֮�����ɳ�ԱAction������
	if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, GRP_STATE_CHANGE);
	else XCtrlManager.eventProc(m_objectID, GRP_STATE_CHANGE);
	if (m_state == STATE_NORMAL)
	{
		m_stateBotton.setSymbol(BTN_SYMBOL_LINE);
		//��������������
		for (unsigned int i = 0; i < m_childList.size(); ++i)
		{
			m_childList[i]->setVisible();
		}
	}
	else
	{
		m_stateBotton.setSymbol(BTN_SYMBOL_CROSS);
		//��ʾ����������
		for (unsigned int i = 0; i < m_childList.size(); ++i)
		{
			m_childList[i]->disVisible();
		}
	}
	updateData();
}
#if !WITH_INLINE_FILE
#include "XGroup.inl"
#endif
}