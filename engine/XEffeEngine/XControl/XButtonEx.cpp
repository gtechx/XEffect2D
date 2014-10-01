//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButtonEx.h"
#include "XObjectManager.h"
#include "XControlManager.h"

_XButtonEx::_XButtonEx()
:m_isInited(XFalse)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_funInit(NULL)
,m_funRelease(NULL)
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)
,m_pClass(NULL)
,m_hotKey(-1)
,m_nowButtonState(BUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
,m_pArea(NULL)
,m_pNowArea(NULL)
,m_resInfo(NULL)
,m_colorRate(NULL)
{
	m_ctrlType = CTRL_OBJ_BUTTONEX;
}
void _XButtonEx::setTexture(const _XButtonTexture& tex)
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
_XBool _XButtonEx::init(const _XVector2& position,		//�ؼ���λ��
		const _XVector2 *area,int pointSum,	//����������ť����Ӧ��Χ�����е㣬�Ѿ��������
		const _XButtonTexture &tex,					//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,
		const _XVector2 &captionPosition,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font)
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

	m_pArea = createArrayMem<_XVector2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(pointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	m_centerPos.set(0.0f,0.0f);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
		m_pNowArea[i].x = m_position.x + area[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + area[i].y * m_size.y;
	}
	m_centerPos /= pointSum;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color = _XFColor::white;
	m_sprite.setColor(m_color);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_withoutTex = XFalse;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButtonEx::initWithoutTex(const _XVector2 *area,int pointSum,const char *caption,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,const _XVector2 &captionPosition,float captionSize)
{//��δʵ��
	if(m_isInited ||
		area == NULL || pointSum <= 2 ||	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
		captionSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	m_textPosition = captionPosition;
	m_comment.init();

	m_pArea = createArrayMem<_XVector2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(pointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_colorRate = createArrayMem<float>(pointSum);
	if(m_colorRate == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		XDELETE_ARRAY(m_pNowArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	float max = area[0].y;
	float min = area[0].y;
	m_centerPos.set(0.0f,0.0f);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_centerPos += area[i];
		m_pArea[i] = area[i];
		m_pNowArea[i].x = m_position.x + area[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + area[i].y * m_size.y;
		if(area[i].y > max) max = area[i].y;
		if(area[i].y < min) min = area[i].y;
	}
	m_centerPos /= pointSum;
	//���������ɫ
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_colorRate[i] = maping1D(area[i].y,min,max,0.9f,1.1f);
		m_colorRate[i] = 2.0f - m_colorRate[i];
	}

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_size.set(1.0f,1.0f);

	m_color = _XFColor::white;
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_withoutTex = XFalse;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_withoutTex = XTrue;
	m_isInited = XTrue;
	return XTrue;
}
void _XButtonEx::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pNowArea[i].x = m_position.x + m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + m_pArea[i].y * m_size.y;
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
void _XButtonEx::setSize(float x,float y)
{
	if(!m_isInited ||
		(x <= 0 && y <= 0)) return;
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_sprite.setSize(m_size);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pNowArea[i].x = m_position.x + m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + m_pArea[i].y * m_size.y;
	}
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
_XBool _XButtonEx::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(getIsInRect(m_upMousePoint,m_pNowArea,m_areaPointSum))
	{//��궯���ڷ�Χ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState == MOUSE_MOVE && m_nowButtonState == BUTTON_STATE_NORMAL)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
			if(m_withAction)
			{//�������һ����̬Ч��
				m_isInAction = XTrue;
				m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_size;
			}
		}else
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
			(m_nowButtonState == BUTTON_STATE_NORMAL || m_nowButtonState == BUTTON_STATE_ON))
		{
			m_nowButtonState = BUTTON_STATE_DOWN;
			if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
			m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(m_nowButtonState == BUTTON_STATE_ON || m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
_XBool _XButtonEx::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if(m_nowButtonState == BUTTON_STATE_NORMAL)
		{//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
			if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose))
			{
				if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				return XTrue;
			}
		}
	}
	return XFalse;
}
void _XButtonEx::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(!m_withoutTex)
	{
		switch(m_nowButtonState)
		{
		case BUTTON_STATE_NORMAL: m_sprite.draw(m_buttonNormal);break;
		case BUTTON_STATE_DOWN: m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON: m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE: m_sprite.draw(m_buttonDisable);break;
		}
	}else
	{
		switch(m_nowButtonState)
		{
		case BUTTON_STATE_NORMAL: drawFillPolygonExA(m_pNowArea,m_colorRate,m_areaPointSum,0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA);break;
		case BUTTON_STATE_DOWN: drawFillPolygonExA(m_pNowArea,m_colorRate,m_areaPointSum,0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);break;
		case BUTTON_STATE_ON: drawFillPolygonExA(m_pNowArea,m_colorRate,m_areaPointSum,0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA);break;
		case BUTTON_STATE_DISABLE: drawFillPolygonExA(m_pNowArea,m_colorRate,m_areaPointSum,0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA);break;
		}
	}
	//��ʾ��ť������
	m_caption.draw();
}
void _XButtonEx::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	_XVector2 tmp = m_centerPos * (m_lightMD.getNowData() * m_oldSize - m_oldSize);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		drawLine(m_oldPos - tmp + m_pArea[i] * m_oldSize * m_lightMD.getNowData(),
			m_oldPos - tmp + m_pArea[(i + 1)%m_areaPointSum] * m_oldSize * m_lightMD.getNowData(),
			1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
	m_comment.draw();
}
void _XButtonEx::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
void _XButtonEx::release()
{
	if(!m_isInited) return;

	XDELETE_ARRAY(m_pArea);
	XDELETE_ARRAY(m_pNowArea);
	XDELETE_ARRAY(m_colorRate);
	m_areaPointSum = 0;
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);

	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XButtonEx::setACopy(const _XButtonEx &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_nowButtonState = temp.m_nowButtonState;		//��ǰ�İ�ť״̬

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//��ť�ı���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_withoutTex = temp.m_withoutTex;
	m_areaPointSum = temp.m_areaPointSum;
	m_pArea = createArrayMem<_XVector2>(m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(m_areaPointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	if(m_withoutTex)
	{
		m_colorRate = createArrayMem<float>(m_areaPointSum);
		if(m_colorRate == NULL)
		{
			XDELETE_ARRAY(m_pArea);
			XDELETE_ARRAY(m_pNowArea);
			return XFalse;
		}
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			m_pArea[i] = temp.m_pArea[i];
			m_pNowArea[i] = temp.m_pNowArea[i];
			m_colorRate[i] = temp.m_colorRate[i];
		}
	}else
	{
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			m_pArea[i] = temp.m_pArea[i];
			m_pNowArea[i] = temp.m_pNowArea[i];
		}
	}

	m_textColor = temp.m_textColor;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;				//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;					//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_funInit = temp.m_funInit;
	m_funRelease = temp.m_funRelease;
	m_funMouseOn = temp.m_funMouseOn;
	m_funMouseDown = temp.m_funMouseDown;
	m_funMouseUp = temp.m_funMouseUp;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_sprite.setACopy(temp.m_sprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
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
_XVector2 _XButtonEx::getBox(int order)
{//Ѱ�����е�������Χ�ĵ�
	if(!m_isInited) return _XVector2::zero;
	float left = m_pNowArea[0].x;
	float right = m_pNowArea[0].x;
	float top = m_pNowArea[0].y;
	float bottom = m_pNowArea[0].y;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		if(m_pNowArea[i].x < left) left = m_pNowArea[i].x;
		if(m_pNowArea[i].x > right) right = m_pNowArea[i].x;
		if(m_pNowArea[i].y < top) top = m_pNowArea[i].y;
		if(m_pNowArea[i].y > bottom) bottom = m_pNowArea[i].y;
	}
	switch(order)
	{
	case 0: return _XVector2(left,top);
	case 1: return _XVector2(right,top);
	case 2: return _XVector2(right,bottom);
	case 3: return _XVector2(left,bottom);
	}

	return _XVector2::zero;
}
inline void _XButtonEx::update(int stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//�����������
		setSize(m_actionMoveData.getNowData() * m_oldSize);
		//������Ҫ�����е�
		_XVector2 tmp = m_centerPos * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize);
		setPosition(m_oldPos - tmp);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
	//	_XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
	//		m_oldPos.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
	//	_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
	//		m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
	//	m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}