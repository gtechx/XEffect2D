#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButtonEx.h"
#include "XObjectManager.h"
#include "XControlManager.h"
namespace XE{
XButtonEx::XButtonEx()
	:m_isInited(XFalse)
	, m_buttonNormal(NULL)
	, m_buttonDown(NULL)
	, m_buttonOn(NULL)
	, m_buttonDisable(NULL)
	, m_hotKey(-1)
	, m_curButtonState(BUTTON_STATE_NORMAL)
	, m_upMousePoint(0.0f)
	, m_pArea(NULL)
	, m_pCurArea(NULL)
	, m_resInfo(NULL)
	, m_colorRate(NULL)
{
	m_ctrlType = CTRL_OBJ_BUTTONEX;
}
void XButtonEx::setTexture(const XButtonSkin& tex)
{
	if(tex.buttonNormal == NULL) return;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
XBool XButtonEx::init(const XVec2& position,		//�ؼ���λ��
		const XVec2 *area,int pointSum,	//����������ť����Ӧ��Χ�����е㣬�Ѿ��������
		const XButtonSkin &tex,					//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,
		const XVec2& captionPosition,	//��ť����ʵ�����ֵ������Ϣ
		const XFontUnicode& font)
{
	if(m_isInited) return XFalse;
	if(area == NULL || pointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	if(tex.buttonNormal == NULL) return XFalse;		//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(captionSize <= 0) return XFalse;
	m_position = position;
	m_textPosition = captionPosition;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_comment.init();

	m_pArea = XMem::createArrayMem<XVec2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVec2>(pointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	m_centerPos.set(0.0f);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
	//	m_pCurArea[i].x = m_position.x + area[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + area[i].y * m_scale.y;
		m_pCurArea[i] = m_position + area[i] * m_scale;
	}
	m_centerPos /= pointSum;

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_scale.set(1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color = XFColor::white;
	m_sprite.setColor(m_color);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_textSize.set(captionSize);
	m_caption.setScale(m_textSize * m_scale);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_INIT);
	else XCtrlManager.eventProc(m_objectID,BTNEX_INIT);

	m_isVisible = m_isEnable = m_isActive = XTrue;
	m_withoutTex = XFalse;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XButtonEx::initWithoutSkin(const XVec2 *area,int pointSum,const char *caption,	//��ť����ʵ�����ֵ������Ϣ
		const XFontUnicode& font,const XVec2& captionPosition,float captionSize)
{//��δʵ��
	if(m_isInited ||
		area == NULL || pointSum <= 2 ||	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
		captionSize <= 0) return XFalse;
	m_position.reset();
	m_textPosition = captionPosition;
	m_comment.init();

	m_pArea = XMem::createArrayMem<XVec2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVec2>(pointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_colorRate = XMem::createArrayMem<float>(pointSum);
	if(m_colorRate == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		XMem::XDELETE_ARRAY(m_pCurArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	float max = area[0].y;
	float min = area[0].y;
	m_centerPos.reset();
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
	//	m_pCurArea[i].x = m_position.x + area[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + area[i].y * m_scale.y;
		m_pCurArea[i] = m_position + area[i] * m_scale;
		if(area[i].y > max) max = area[i].y;
		if(area[i].y < min) min = area[i].y;
	}
	m_centerPos /= pointSum;
	//���������ɫ
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_colorRate[i] = XMath::mapping1D(area[i].y,min,max,0.9f,1.1f);
		m_colorRate[i] = 2.0f - m_colorRate[i];
	}

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_scale.set(1.0f);

	m_color = XFColor::white;
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_textSize.set(captionSize);
	m_caption.setScale(m_textSize * m_scale);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_INIT);
	else XCtrlManager.eventProc(m_objectID,BTNEX_INIT);

	m_isVisible = m_isEnable = m_isActive = XTrue;
	m_withoutTex = XFalse;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_withoutTex = m_isInited = XTrue;
	return XTrue;
}
void XButtonEx::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
	//	m_pCurArea[i].x = m_position.x + m_pArea[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + m_pArea[i].y * m_scale.y;
		m_pCurArea[i] = m_position + m_pArea[i] * m_scale;
	}
	//m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint,MOUSE_MOVE);
}
void XButtonEx::setScale(const XVec2& s)
{
	if(!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_caption.setScale(m_textSize * m_scale);
	m_sprite.setScale(m_scale);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
	//	m_pCurArea[i].x = m_position.x + m_pArea[i].x * m_scale.x;
	//	m_pCurArea[i].y = m_position.y + m_pArea[i].y * m_scale.y;
		m_pCurArea[i] = m_position + m_pArea[i] * m_scale;
	}
	mouseProc(m_upMousePoint,MOUSE_MOVE);
}
XBool XButtonEx::mouseProc(const XVec2& p,XMouseState mouseState)
{
	m_upMousePoint.set(p);
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||	//�����Ч��ֱ���˳�
		(m_withAction && m_isInAction) || m_isSilent) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(XMath::getIsInPolygon(m_upMousePoint,m_pCurArea,m_areaPointSum))
	{//��궯���ڷ�Χ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
		}
		m_comment.updatePosition(p + XVec2(0.0f, 16.0f));
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//�����������������˵������ʧ
		if(mouseState == MOUSE_MOVE && m_curButtonState == BUTTON_STATE_NORMAL)
		{
			m_curButtonState = BUTTON_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_ON);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_ON);
			if(m_withAction)
			{//�������һ����̬Ч��
				m_isInAction = XTrue;
				m_actionMoveData.set(1.0f,1.1f,0.02f,MD_MODE_SIN_MULT,1);
				m_lightMD.set(1.0f,2.0f,0.002f,MD_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_scale;
			}
		}else
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
			(m_curButtonState == BUTTON_STATE_NORMAL || m_curButtonState == BUTTON_STATE_ON))
		{
			m_curButtonState = BUTTON_STATE_DOWN;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_DOWN);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curButtonState == BUTTON_STATE_DOWN)
		{
			m_curButtonState = BUTTON_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_UP);
			m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(m_curButtonState == BUTTON_STATE_ON || m_curButtonState == BUTTON_STATE_DOWN)
		{
			m_curButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
XBool XButtonEx::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||	//�����Ч��ֱ���˳�
		(m_withAction && m_isInAction) ||	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
		m_isSilent) return XFalse;

	if(keyState != KEY_STATE_UP ||  //��������ʱ������Ӧ
		m_curButtonState != BUTTON_STATE_NORMAL) return XFalse;
		//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
	if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose))
	{
		if(m_eventProc != NULL)
		{
			m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_DOWN);
			m_eventProc(m_pClass,m_objectID,BTNEX_MOUSE_UP);
		}else
		{
			XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_DOWN);
			XCtrlManager.eventProc(m_objectID,BTNEX_MOUSE_UP);
		}
		return XTrue;
	}
	return XFalse;
}
void XButtonEx::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(!m_withoutTex)
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL: m_sprite.draw(m_buttonNormal);break;
		case BUTTON_STATE_DOWN: m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON: m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE: m_sprite.draw(m_buttonDisable);break;
		}
	}else
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::normalColor * m_color);break;
		case BUTTON_STATE_DOWN: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::downColor * m_color);break;
		case BUTTON_STATE_ON: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::onColor * m_color);break;
		case BUTTON_STATE_DISABLE: XRender::drawFillPolygonExA(m_pCurArea,m_colorRate,m_areaPointSum,XCCS::disableColor * m_color);break;
		}
	}
	//��ʾ��ť������
	m_caption.draw();
}
void XButtonEx::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	XVec2 tmp = m_centerPos * (m_lightMD.getCurData() * m_oldSize - m_oldSize);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		XRender::drawLine(m_oldPos - tmp + m_pArea[i] * m_oldSize * m_lightMD.getCurData(),
			m_oldPos - tmp + m_pArea[(i + 1)%m_areaPointSum] * m_oldSize * m_lightMD.getCurData(),
			1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
	}
	m_comment.draw();
}
void XButtonEx::release()
{
	if(!m_isInited) return;

	XMem::XDELETE_ARRAY(m_pArea);
	XMem::XDELETE_ARRAY(m_pCurArea);
	XMem::XDELETE_ARRAY(m_colorRate);
	m_areaPointSum = 0;
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTNEX_RELEASE);
	else XCtrlManager.eventProc(m_objectID,BTNEX_RELEASE);

	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
XBool XButtonEx::setACopy(const XButtonEx &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_curButtonState = temp.m_curButtonState;		//��ǰ�İ�ť״̬

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//��ť�ı���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_withoutTex = temp.m_withoutTex;
	m_areaPointSum = temp.m_areaPointSum;
	m_pArea = XMem::createArrayMem<XVec2>(m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pCurArea = XMem::createArrayMem<XVec2>(m_areaPointSum);
	if(m_pCurArea == NULL)
	{
		XMem::XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	if(m_withoutTex)
	{
		m_colorRate = XMem::createArrayMem<float>(m_areaPointSum);
		if(m_colorRate == NULL)
		{
			XMem::XDELETE_ARRAY(m_pArea);
			XMem::XDELETE_ARRAY(m_pCurArea);
			return XFalse;
		}
		memcpy(m_pArea, temp.m_pArea, m_areaPointSum * sizeof(XVec2));
		memcpy(m_pCurArea, temp.m_pCurArea, m_areaPointSum * sizeof(XVec2));
		memcpy(m_colorRate, temp.m_colorRate, m_areaPointSum * sizeof(float));
	}else
	{
		memcpy(m_pArea, temp.m_pArea, m_areaPointSum * sizeof(XVec2));
		memcpy(m_pCurArea, temp.m_pCurArea, m_areaPointSum * sizeof(XVec2));
	}

	m_textColor = temp.m_textColor;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;				//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;					//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_sprite.setACopy(temp.m_sprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼�����λ��

	m_actionMoveData = temp.m_actionMoveData;	//��̬Ч���ı���
	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_centerPos = temp.m_centerPos;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;

	m_hotKey = temp.m_hotKey;
	return XTrue;
}
XVec2 XButtonEx::getBox(int order)
{//Ѱ�����е�������Χ�ĵ�
	if(!m_isInited) return XVec2::zero;
	float left = m_pCurArea[0].x;
	float right = m_pCurArea[0].x;
	float top = m_pCurArea[0].y;
	float bottom = m_pCurArea[0].y;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		if(m_pCurArea[i].x < left) left = m_pCurArea[i].x;else
		if(m_pCurArea[i].x > right) right = m_pCurArea[i].x;
		if(m_pCurArea[i].y < top) top = m_pCurArea[i].y;else
		if(m_pCurArea[i].y > bottom) bottom = m_pCurArea[i].y;
	}
	switch(order)
	{
	case 0: return XVec2(left,top);
	case 1: return XVec2(right,top);
	case 2: return XVec2(right,bottom);
	case 3: return XVec2(left,bottom);
	}

	return XVec2::zero;
}
inline void XButtonEx::update(float stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//�����������
		setScale(m_actionMoveData.getCurData() * m_oldSize);
		//������Ҫ�����е�
		setPosition(m_oldPos - m_centerPos * (m_actionMoveData.getCurData() * m_oldSize - m_oldSize));
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
	//	XVec2 pos(m_oldPos.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
	//		m_oldPos.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
	//	XVec2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
	//		m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
	//	m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}
#if !WITH_INLINE_FILE
#include "XButtonEx.inl"
#endif
}