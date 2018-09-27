#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMouseRightButtonMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"
namespace XE{
XMouseRightButtonMenu::XMouseRightButtonMenu()
	:m_isInited(XFalse)
	, m_menu(NULL)				//��ť��ָ��
	, m_menuSum(0)				//��ť������
	, m_curChoose(-1)			//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	, m_lastChoose(-1)			//����ѡ���ֵ
	//,m_funChooseChange(NULL)	//�ı�ѡֵʱ����
	//,m_funChooseOver(NULL)		//����ȷ��ѡֵʱ����
	, m_resInfo(NULL)
{
	m_ctrlType = CTRL_OBJ_MOUSERIGHTBUTTONMENU;
}
inline void XMouseRightButtonMenu::release()	//�ͷ���Դ
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_menu);
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = XFalse;
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
XBool XMouseRightButtonMenu::init(int menuSum,	//�˵��е��������
		const XVec2& position,	//�˵���λ��
		const XRect& Area,	//�˵���������Ӧ��Χ
		const XMouseRightButtonMenuSkin &tex,	//�˵�����ͼ
		const XFontUnicode& font,float captionSize, const XVec2& textPosition)		//�˵�������
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return XFalse;//�⼸����ͼ�Ǳ���Ҫ�е�
	if(menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position = position;
	m_mouseRect = Area;

	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right, m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if (!m_menu[i].init(XVec2(m_position.x, m_position.y + m_mouseRect.getHeight() * i * m_scale.y),
			m_mouseRect, tex, " ", font, captionSize, textPosition))
		{
			XMem::XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setScale(m_scale);
	//����Щ����������������ע����
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}
	m_curChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisible = XFalse;
	m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XMouseRightButtonMenu::initWithoutSkin(int menuSum,const XRect& area,
	const XFontUnicode& font,float captionSize, const XVec2& textPosition)
{
	if(m_isInited ||
		menuSum <= 0) return XFalse;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position.reset();
	m_mouseRect = area;

	m_scale.set(1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].initWithoutSkin(" ",font,captionSize,m_mouseRect,textPosition))
		{
			XMem::XDELETE_ARRAY(m_menu);
			return XFalse;
		}
		m_menu[i].setScale(m_scale);
	//����Щ����������������ע����
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}
	m_curChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisible = XFalse;
	m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XMouseRightButtonMenu::setPosition(const XVec2& p)
{
	m_position = p;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
	}
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_curChoose = -1;
	updateChildPos();
}
void XMouseRightButtonMenu::setScale(const XVec2& s)
{
	if(s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	m_allArea.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + XVec2(m_mouseRect.right,m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_scale);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_scale.y);
		m_menu[i].setScale(m_scale);
	}
	m_upMousePoint.set(m_position + m_mouseRect.getCenter() * m_scale);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_curChoose = -1;
	updateChildScale();
}
XBool XMouseRightButtonMenu::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	//if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;

	if(!m_isVisible)
	{//û����ʾ��ʱ����Ҫͨ���������Ҽ�������Ƿ񵯳��˵�
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//�Ҽ���������ʾ�˵�
			m_isVisible = XTrue;
			setPosition(p);
			//�˵�����
			if(m_withAction)
			{//���涨�����Ķ���
				m_isInAction = XTrue;
				m_isDraw = true;
				m_actionMoveData.set(0.0f,1.0f,0.005f);
				//��ʼ��������ť��״̬
				for(int i = 0;i < m_menuSum;++ i)
				{
					m_menu[i].setWithAction(XFalse);
					m_menu[i].setAlpha(0.0f);
				}
			}
		}
	}else
	{//��ʾʱ�԰�����������Ӧ
		for(int i = 0;i < m_menuSum;++ i)
		{
			m_menu[i].mouseProc(p,mouseState);
		}
		if(m_allArea.isInRect(p))
		{
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = (p.y - m_position.y - m_mouseRect.top * m_scale.y) / (m_mouseRect.getHeight() * m_scale.y);
				//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
				else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
				//�ָ����а�����״̬Ϊ��ͨ״̬
				for(int i = 0;i < m_menuSum;++ i)
				{
					if(m_menu[i].getEnable())
						m_menu[i].setState(BUTTON_STATE_NORMAL);
				}
				//m_isVisible = XFalse;
				if(m_withAction)//���涨�����Ķ���
					playOutAction();
				return XTrue;
			}
		//	m_upMousePoint.set(x,y);
			//m_upMousePoint.y = (int)((y - m_objRect.top) / m_objRect.getHeight() * m_scale.y) * m_objRect.getHeight() * m_scale.y + m_objRect.getHeight() * m_scale.y * 0.5;
		}else
		{//������Ĳ���
			if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK || mouseState == MOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = -1;
				//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
				else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
				//m_isVisible = XFalse;
				//�˵���ʧ
				if(m_withAction)//���涨�����Ķ���
					playOutAction();
				return XTrue;
			}
		}
	}
	return XFalse;
}
void XMouseRightButtonMenu::playOutAction()
{//������Ҫ���⣬����Ѿ���������Ҫ�ظ�����
	if(!m_isDraw) return;	//����Ѿ���ʼ�˳������ظ��˳�
	m_isInAction = XTrue;
	m_isDraw = false;
	m_actionMoveData.set(0.0f,1.0f,0.005f);
	//��ʼ��������ť��״̬
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setWithAction(XFalse);
		m_menu[i].setAlpha(1.0f);
	}
}
XBool XMouseRightButtonMenu::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isEnable ||		//�����Ч��ֱ���˳�
		!m_isVisible) return XFalse;	//���̲���ֻ������ʾ������²�����ʾ
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;
	if(keyState == KEY_STATE_UP)
	{
		if(keyOrder == XKEY_UP)
		{//���ϼ�
			-- m_curChoose;
			if(m_curChoose < 0) m_curChoose = m_menuSum -1;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_curChoose + 0.5f)) * m_scale.y;

		//	m_upMousePoint.y -= m_objRect.getHeight() * m_scale.y;
		//	if(m_upMousePoint.y < m_allArea.top)
		//	{
		//		m_upMousePoint.y += m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint,MOUSE_MOVE);
		}else
		if(keyOrder == XKEY_DOWN)
		{//���¼�
			++ m_curChoose;
			if(m_curChoose >= m_menuSum) m_curChoose = 0;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_curChoose + 0.5f)) * m_scale.y;

		//	m_upMousePoint.y += m_objRect.getHeight() * m_scale.y;
		//	if(m_upMousePoint.y > m_allArea.bottom)
		//	{
		//		m_upMousePoint.y -= m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint,MOUSE_MOVE);
		}else
		if(keyOrder ==  XKEY_RETURN)
		{//�س���
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(m_menu[i].getState() == BUTTON_STATE_ON)
				{
					m_lastChoose = i;
					//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
					else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
					//m_isVisible = XFalse;
					if(m_withAction)//���涨�����Ķ���
						playOutAction();

					return XTrue;
				}
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(keyOrder == m_menu[i].getHotKey() && m_menu[i].getState() != BUTTON_STATE_DISABLE)
				{//���������˵��Ŀ�ݼ���ͬ�����Ҳ˵���״̬������Ч
					m_lastChoose = i;
					//if(m_funChooseOver != NULL) m_funChooseOver(m_pClass,m_objectID);
					if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MRBM_CHOOSE_OVER);
					else XCtrlManager.eventProc(m_objectID,MRBM_CHOOSE_OVER);
					//m_isVisible = XFalse;
					if(m_withAction)//���涨�����Ķ���
						playOutAction();
					
					return XTrue;
				}
			}
		}
	}
	return XFalse;
}
XBool XMouseRightButtonMenu::setACopy(const XMouseRightButtonMenu &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ����ֱ�ӷ���
	if(m_isInited)	release();//��������Ѿ���ʼ��,�ͷŵ���Ӧ����Դ
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	//���ø����˵���
	m_menuSum = temp.m_menuSum;		//��ť������
	m_menu = XMem::createArrayMem<XButton>(m_menuSum);
	if(m_menu == NULL) return XFalse;	//�ڴ����ʧ����ֱ�ӷ���ʧ��

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(!m_menu[i].setACopy(temp.m_menu[i])) return XFalse;
		//����Щ�ؼ��ӿؼ���������ע����
		XCtrlManager.decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_menu[i]);
#endif
	}

	m_curChoose = temp.m_curChoose;		//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	m_lastChoose = temp.m_lastChoose;		//����ѡ���ֵ
	m_isDraw = temp.m_isDraw;	//�Ƿ��ǲ˵�����
	m_actionMoveData = temp.m_actionMoveData;
//	m_funChooseChange = temp.m_funChooseChange;	//�ı�ѡֵʱ����
//	m_funChooseOver = temp.m_funChooseOver;		//����ȷ��ѡֵʱ����

	m_allArea = temp.m_allArea;		//��궯������Ӧ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼������ڷ�Χ�ڵ�λ��

	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMouseRightButtonMenu.inl"
#endif
}