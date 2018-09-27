INLINE XBool XFontBasic::isInRect(const XVec2& p)
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE XBool XFontBasic::setString(const char *p)	//������ʾ���ַ���
{
	if(p == NULL) return XFalse;
	if(strcmp(p,m_string) == 0) return XTrue;	//�������ͬ���ַ���������
	if(m_maxStringLen >= 0 && strlen(p) < m_maxStringLen)
	{
		strcpy_s(m_string,m_maxStringLen,p);
	}else
	{
		assert(m_maxStringLen > 0);
		memcpy(m_string,p,m_maxStringLen);
		m_string[m_maxStringLen - 1] = '\0';	//���ַ���һ��������
		if(XString::isAtUnicodeEnd(m_string,m_maxStringLen - 3))
		{
			memcpy(m_string + m_maxStringLen - 3, &"..", 3);
			//m_string[m_maxStringLen - 3] = '.';
			//m_string[m_maxStringLen - 2] = '.';
			//m_string[m_maxStringLen - 1] = '\0';
		}else
		{
			memcpy(m_string + m_maxStringLen - 4, &"..", 3);
			//m_string[m_maxStringLen - 4] = '.';
			//m_string[m_maxStringLen - 3] = '.';
			//m_string[m_maxStringLen - 2] = '\0';
		}
		//strlen(p) >= m_maxStringLen
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
INLINE void XFontBasic::setPosition(const XVec2& p)			//����������ʾ��λ��
{
	m_setPosition = p;
	updateChildPos();
	int x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	int y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(x,y);

	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setDistance(float distance)			//����������֮��ľ���
{
	m_distance = distance;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAngle(float angle)	//����������ʾ�ĽǶ�
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * DEGREE2RADIAN);
	m_angleCos = cos(angle * DEGREE2RADIAN);
	int x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	int y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set(x, y);

	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setScale(const XVec2& s)	//�����������ʾ��С
{
	m_scale = s;
	updateChildScale();
	m_sprite.setScale(s);
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setRotateBasePoint(const XVec2& r)
{
	m_rotateBasicPoint = r;
	setPosition(m_setPosition);
}
INLINE void XFontBasic::setColor(const XFColor& c)
{
	m_sprite.setColor(c);
	updateChildColor();
}
INLINE void XFontBasic::setAlpha(float a) 
{
	m_sprite.setAlpha(a);
	updateChildAlpha();
}
INLINE void XFontBasic::setVisible() 
{
	m_isVisible = XTrue;
	updateChildVisible();
}					//��������ɼ�
INLINE void XFontBasic::disVisible() 
{
	m_isVisible = XFalse;
	updateChildVisible();
}
INLINE void XFontBasic::setClipRect(const XRect& temp)
{
	m_clipRect = temp;
	m_isCliped = XTrue;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::disClip()
{
	if(!m_isCliped) return; 
	m_isCliped = XFalse;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAlignmentModeX(XFontAlignmentModeX mode)
{
	if(mode == m_alignmentModeX) return;
	m_alignmentModeX = mode;
	m_needUpdateData = XTrue;
}
INLINE void XFontBasic::setAlignmentModeY(XFontAlignmentModeY mode)
{
	if(mode == m_alignmentModeY) return;
	m_alignmentModeY = mode;
	m_needUpdateData = XTrue;
}
INLINE int XFontBasic::getMaxPixelWidth()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelWidth;
}
INLINE int XFontBasic::getMaxPixelHeight()
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	return m_maxPixelHeight;
}
INLINE void XFontBasic::setIsPassWord(XBool temp)
{
	if((m_isPassword && !temp) || (!m_isPassword && temp))
	{
		m_isPassword = temp;
		m_needUpdateData = XTrue;
	}
}