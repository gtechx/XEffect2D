#include "XStdHead.h"
#include "XSliderRing.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XMath\XColorSpace.h"

namespace XE
{
XSliderRing::XSliderRing()
	:m_isInited(false)
	, m_withFont(false)
	, m_textColor(0.0f, 1.0f)
	, m_mode(SLDR_MODE_HORIZONTAL)
	, m_minValue(0.0f)
	, m_maxValue(100.0f)
	, m_curValue(0.0f)
	, m_curSliderState(SLDR_STATE_NORMAL)
	, m_mouseFlyArea(50.0f)	//���ֵʹ��һ��ϵͳ�趨�����ֵ�����ڴ����н����޸ģ����ǲ�����ģ����ǿ��ǵ������ڵ������ԣ�����Ĭ��������������
	, m_dataChanged(false)
	, m_isClockwise(-1)
	, m_trimValue(0.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDERRING;
}
void XSliderRing::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
bool XSliderRing::initWithoutSkin(const XVec2& size, float maxValue, float minValue,
	XSliderRingMode mode)
{
	if (m_isInited) return false;
	if (size.x <= 0.0f || size.y <= 0.0f) return false;
	if (minValue >= maxValue) XMath::swapTwoSum(minValue, maxValue);
	m_mode = mode;
	m_size = size;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_curValue = minValue;
	m_upValue = minValue;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = true;
	return true;
}
XBool XSliderRing::setFont(const char* caption, const XFontUnicode& font)
{
	if (!m_isInited) return false;
	if (caption != NULL) m_fontString = caption;
	else m_fontString = "";
	if (!m_caption.setACopy(font)) return XFalse;
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
															   //m_textColor.setColor(XFColor::black);
	m_caption.setColor(m_textColor * m_color);
	m_caption.setScale(m_scale);
	updateFontStr();
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		m_caption.setAngle(90.0f);
		m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
		break;
	case SLDR_MODE_HORIZONTAL:
		m_caption.setAngle(0.0f);
		m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
		break;
	case SLDR_MODE_RING:
		m_caption.setAngle(0.0f);
		m_caption.setPosition(m_position);
		m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //�������������
		m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
		break;
	}

	m_withFont = XTrue;
	return XTrue;
}
void XSliderRing::draw()
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	float w, step, offset;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::downColor * m_color, true);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::mouseColor * m_color, true);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::specialColor * m_color, true);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::blackDownColor * m_color, true);
			break;
		}
		w = m_mouseRect.getHeight();
		step = 0.0f;
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (int i = 1; i < w; i += step)
		{
			float rate = static_cast<float>(i) / w;
			step = 3.0f + 5.0f * sin(rate * PI);
			rate -= offset;
			XFColor color = XColorSpace::getWheelColor(rate * 360.0f);
			XRender::drawLine(XVec2(m_mouseRect.left + 1.0f, m_mouseRect.top + i),
				XVec2(m_mouseRect.right - 1.0f, m_mouseRect.top + i), 2.0f, color);
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::downColor * m_color, true);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::mouseColor * m_color, true);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::specialColor * m_color, true);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRectExA(m_curMouseRect.getLT(),
				m_curMouseRect.getSize(), XCCS::blackDownColor * m_color, true);
			break;
		}
		w = m_mouseRect.getWidth();
		step = 0.0f;
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (int i = 1; i < w; i += step)
		{
			float rate = static_cast<float>(i) / w;
			step = 3.0f + 5.0f * sin(rate * PI);
			rate -= offset;
			XFColor color = XColorSpace::getWheelColor(rate * 360.0f);
			XRender::drawLine(XVec2(i + m_mouseRect.left, m_mouseRect.top + 1.0f),
				XVec2(i + m_mouseRect.left, m_mouseRect.bottom - 1.0f), 2.0f, color);
		}
		break;
	case SLDR_MODE_RING:
		switch (m_curSliderState)
		{
		case SLDR_STATE_NORMAL:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::downColor * m_color);
			break;
		case SLDR_STATE_DOWN:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::mouseColor * m_color);
			break;
		case SLDR_STATE_ON:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::specialColor * m_color);
			break;
		case SLDR_STATE_DISABLE:
			XRender::drawFillRing(m_position, m_size.x * m_scale.x, m_size.y * m_scale.x,
				0.0f, 360.0f, 60, XCCS::blackDownColor * m_color);
			break;
		}
		w = m_mouseRect.getWidth();
		step = 360.0f / (PI * m_size.x * m_scale.x * 2.0f / 5.0f);
		offset = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
		for (float i = 0.0f; i < 360.0f; i += step)
		{
			XVec2 e = m_scale.x * XMath::getRotateRate(i * DEGREE2RADIAN);
			XVec2 s = m_position + m_size.y * e;
			e = m_position + m_size.x * e;
			XFColor color = XColorSpace::getWheelColor(i + offset * 360.0f);
			XRender::drawLine(s, e, 2.0f, color);
		}
		if (m_curSliderState == SLDR_STATE_DOWN && m_isClockwise != -1)
		{//��滬��������
			float sAngle,eAngle;
			if (m_isClockwise == 0)
			{//��ʱ�뷽��
				sAngle = m_position.getAngle(m_oldMousePos);
				eAngle = m_position.getAngle(getMousePos());
				if (eAngle > sAngle) eAngle -= PI2;
				std::swap(eAngle, sAngle);
			}
			else
			{//˳ʱ�뷽��
				sAngle = m_position.getAngle(m_oldMousePos);
				eAngle = m_position.getAngle(getMousePos());
				if (eAngle < sAngle) eAngle += PI2;
			}

			XRender::drawFillFan(m_position, m_size.y * m_scale.x, (sAngle + PI) * RADIAN2DEGREE,
				(eAngle + PI) * RADIAN2DEGREE, 60, XFColor(1.0f, 0.25f));
		}
		break;
	}
	if (m_withFont) m_caption.draw();
}
float XSliderRing::getDistanceValue()const
{
	if (!m_isInited) return 0.0f;
	if (m_curSliderState != SLDR_STATE_DOWN) return 0.0f;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		if (getMousePos().y < m_oldMousePos.y)
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		if (getMousePos().x < m_oldMousePos.x)
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	case SLDR_MODE_RING:
		if (m_isClockwise == -1) return 0.0f;
		if (m_isClockwise == 0)
		{
			if (m_curValue < m_upValue) return m_curValue - m_upValue + m_maxValue;
			else return m_curValue - m_upValue;
		}
		else
		{
			if (m_curValue > m_upValue) return m_curValue - m_upValue - m_maxValue;
			else return m_curValue - m_upValue;
		}
		break;
	}
	return 0.0f;
}
XBool XSliderRing::mouseProc(const XVec2& p, XMouseState mouseState)
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if (m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if (m_isSilent) return XFalse;

	if (m_curSliderState == SLDR_STATE_DOWN)
	{//ע�������ǻ������϶�ʱ��Ϊ���ǳ�����Χ���϶���Ȼ��Ч���������������⴦��
		XRect tempFly(m_curMouseRect.getLT() - m_mouseFlyArea, m_curMouseRect.getRB() + m_mouseFlyArea);
		if (tempFly.isInRect(p))
		{//�϶�(ע����������϶�������Χ����ʹ�����һ�ε���Чֵ)
			switch (mouseState)
			{
			case MOUSE_MOVE://���¼���Slider�ĵ�ǰֵ
				updateCurValue(p);
				m_dataChanged = XTrue;	//�����ֵ�����ı�
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_MOVE);
				else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_MOVE);
				break;
			case MOUSE_LEFT_BUTTON_UP://��������¼����ڰ�ť�ϰ��µĵ����¼�
				m_curSliderState = SLDR_STATE_ON;
				if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_UP);
				if (m_dataChanged)
				{
					if (m_eventProc != NULL)
						m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
					else
						XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
					m_dataChanged = XFalse;
				}
				break;
			}
			m_isBeChoose = XTrue;
		}
		else
		{
			m_curSliderState = SLDR_STATE_NORMAL;
			if (m_dataChanged)
			{
				if (m_eventProc != NULL)
					m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
		}
	}
	if (m_curMouseRect.isInRect(p))
	{//��궯���ڷ�Χ��
		switch (mouseState)
		{
		case MOUSE_MOVE:
			if (m_curSliderState != SLDR_STATE_NORMAL) break; //����
			m_curSliderState = SLDR_STATE_ON;
			if (m_withAction)
			{//�������һ����̬Ч��
			//	m_isInAction = XTrue;
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if (m_curSliderState != SLDR_STATE_NORMAL && m_curSliderState != SLDR_STATE_ON) break;

			m_upValue = m_curValue;
			m_oldMousePos = p;
			m_isClockwise = -1;

			m_curSliderState = SLDR_STATE_DOWN;
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_DOWN);
			break;
		case MOUSE_LEFT_BUTTON_UP:
			if (m_curSliderState != SLDR_STATE_DOWN) break;
			//����ᴥ����ֵ�ı��¼�(��������¼�Ӧ���������ϰ��µĵ���)(*����ò�Ʋ��ᷢ��*)
			m_curSliderState = SLDR_STATE_ON;
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLDR_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID, SLDR_MOUSE_UP);
			if (m_dataChanged)
			{
				if (m_eventProc != NULL)
					m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
			break;
		case MOUSE_WHEEL_UP_DOWN:
			if (m_curSliderState != SLDR_STATE_ON) break;
			m_trimValue = m_keyOneValue * 0.1f;
			m_curValue += m_trimValue;
			if (m_curValue > m_maxValue) m_curValue = m_minValue + m_curValue - m_maxValue;
			updateFontStr();
			if (m_eventProc != NULL)
			{
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_TRIM);
			}
			else
			{
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_TRIM);
			}
			m_dataChanged = XFalse;
			break;
		case MOUSE_WHEEL_DOWN_DOWN:
			if (m_curSliderState != SLDR_STATE_ON) break;
			m_trimValue = -m_keyOneValue * 0.1f;
			m_curValue += m_trimValue;
			if (m_curValue < m_minValue) m_curValue = m_maxValue - m_minValue + m_curValue;
			updateFontStr();
			if (m_eventProc != NULL)
			{
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_CHANGE);
				m_eventProc(m_pClass, m_objectID, SLDR_VALUE_TRIM);
			}
			else
			{
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_CHANGE);
				XCtrlManager.eventProc(m_objectID, SLDR_VALUE_TRIM);
			}
			m_dataChanged = XFalse;
			break;
		}
	}
	else
	{
		if (m_curSliderState == SLDR_STATE_ON) m_curSliderState = SLDR_STATE_NORMAL;
	}

	return XTrue;
}
void XSliderRing::updateCurValue(const XVec2& pos)
{
	float changeRate, tmpAngle;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
		changeRate = (pos.y - m_oldMousePos.y) / m_mouseRect.getHeight() * (m_maxValue - m_minValue);
		m_curValue = m_upValue + changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	case SLDR_MODE_HORIZONTAL:
		changeRate = (pos.x - m_oldMousePos.x) / m_mouseRect.getWidth() * (m_maxValue - m_minValue);
		m_curValue = m_upValue + changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	case SLDR_MODE_RING:
		tmpAngle = m_position.getAngle(m_oldMousePos);
		changeRate = m_position.getAngle(pos);
		if (m_isClockwise == -1)
		{//���жϷ���
			if (XMath::angleDistance(tmpAngle * RADIAN2DEGREE, changeRate * RADIAN2DEGREE) <= 0)
				m_isClockwise = 1;
			else
				m_isClockwise = 0;
		}
		if (m_isClockwise == 0)
		{//��ʱ�뷽��
			tmpAngle = m_position.getAngle(m_oldMousePos);
			changeRate = m_position.getAngle(pos);
			if (changeRate > tmpAngle) changeRate -= PI2;
			changeRate = changeRate - tmpAngle;
		}
		else
		{//˳ʱ�뷽��
			tmpAngle = m_position.getAngle(m_oldMousePos);
			changeRate = m_position.getAngle(pos);
			if (changeRate < tmpAngle) changeRate += PI2;
			changeRate = changeRate - tmpAngle;
		}
		changeRate = changeRate / PI2 * (m_maxValue - m_minValue);

		m_curValue = m_upValue - changeRate;
		while (m_curValue < m_minValue)
		{
			m_curValue = m_maxValue - m_minValue + m_curValue;
		}
		while (m_curValue > m_maxValue)
		{
			m_curValue = m_minValue + m_curValue - m_maxValue;
		}
		break;
	}
	updateFontStr();
}
void XSliderRing::setPosition(const XVec2& p)
{
	if (!m_isInited) return;
	m_position = p;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;

	if (m_withFont)
	{
		m_caption.setScale(m_scale);
		switch (m_mode)
		{
		case SLDR_MODE_VERTICAL:
			m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
			break;
		case SLDR_MODE_HORIZONTAL:
			m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
			break;
		case SLDR_MODE_RING:
			m_caption.setPosition(m_position);
			break;
		}
	}
	updateChildPos();
}
void XSliderRing::setScale(const XVec2& s)
{
	if (!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	switch (m_mode)
	{
	case SLDR_MODE_VERTICAL:
	case SLDR_MODE_HORIZONTAL:
		m_curMouseRect.set(m_position, m_position + m_size * m_scale);
		break;
	case SLDR_MODE_RING:
		m_curMouseRect.set(m_position - m_scale * m_size.x,
			m_position + m_scale * m_size.x);
		break;
	}
	m_mouseRect = m_curMouseRect;
	if (m_withFont)
	{
		m_caption.setScale(m_scale);
		switch (m_mode)
		{
		case SLDR_MODE_VERTICAL:
			m_caption.setPosition(m_position + XVec2(m_caption.getTextSize().x * 0.5f + 0.0f) * m_scale);
			break;
		case SLDR_MODE_HORIZONTAL:
			m_caption.setPosition(m_position + XVec2(0.0f, m_caption.getTextSize().y * 0.5f) * m_scale);
			break;
		case SLDR_MODE_RING:
			m_caption.setPosition(m_position);
			break;
		}
	}
	updateChildScale();
}
#if !WITH_INLINE_FILE
#include "XSliderRing.inl"
#endif
}