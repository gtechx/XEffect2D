//inline ����
inline void _XFrame::setPosition(const _XVector2& position)
{
	x = position.x;
	y = position.y;
}
inline void _XFrame::setPosition(float a,float b)
{
	x = a;
	y = b;
}
inline _XVector2 _XFrame::getPosition() const
{
	return _XVector2(x,y);
}
inline int _XFrame::getNowX() const
{
	return (int)(x + m_keyFramePosition[(int)m_nowFramesNumble].x);
}
inline int _XFrame::getNowY() const
{
	return (int)(y + m_keyFramePosition[(int)m_nowFramesNumble].y);
}
inline int _XFrame::getNowWidth() const
{
	return m_texnum[(int)m_nowFramesNumble].m_w;
}
inline int _XFrame::getNowHeight() const
{
	return m_texnum[(int)m_nowFramesNumble].m_h;
}
inline int _XFrame::getInitHalfWidth() const
{
	return (int)(m_centerX);
}
inline int _XFrame::getInitHalfHeight() const
{
	return (int)(m_centerY);
}
inline void _XFrame::setSize(const _XVector2& size)
{
	xsize = size.x;
	ysize = size.y;
}
inline void _XFrame::setSize(float x,float y)
{
	xsize = x;
	ysize = y;
}
inline _XVector2 _XFrame::getSize()
{
	return _XVector2(xsize,ysize);
}
inline _XBool _XFrame::getIsEnd() const
{
	return m_isEnd;
}
inline void _XFrame::setEnd()
{
	if(!m_isEndImmediately)
	{
		m_isSetEnd = XTrue;
	}else
	{
		m_isSetEnd = XTrue;
		m_isEnd = XTrue;
	}
}
inline void _XFrame::reset()	//�����Ѿ�������ɵ�����֡�������²���
{
	if(m_isEnd)
	{
		m_isEnd = XFalse;
		m_isSetEnd = XFalse;
		m_nowFramesNumble = (float)m_startFrame;
	}
}
inline _XTexture * _XFrame::getTexture(int frameFlag)
{
	if(frameFlag == 0)
	{
		return &(m_texnum[(int)(m_nowFramesNumble)]);
	}else
	{
		return m_texnum;
	}
}
inline void _XFrame::setAngle(float temp)
{
	angle = temp;
	//����֡����Ҫ��Щֵ
	angleRadian = (float)(angle * DEGREE2RADIAN);
	sinAngle = sin(angleRadian);
	cosAngle = cos(angleRadian);
}
inline float _XFrame::getAngleDegree()	//��ýǶ�
{
	return angle;
}
inline float _XFrame::getAngleRadian()	//��û���
{
	return angleRadian;
}
inline void _XFrame::setAlpha(float a)
{
	if(a >= 0) alpha = a;
}
inline float _XFrame::getAlpha() const
{
	return alpha;
}
inline void _XFrame::setColor(float r,float g,float b,float a)
{
	if(r >= 0) colorRed = r;
	if(g >= 0) colorGreen = g;
	if(b >= 0) colorBlue = b;
	if(a >= 0) alpha = a;
}
inline void _XFrame::setActionSpeed(float actionSpeed)	//��������֡���ŵ��ٶ�
{
	m_actionSpeed = actionSpeed;
}
inline void _XFrame::setRestart()
{
	m_nowFramesNumble = (float)(m_startFrame);
}