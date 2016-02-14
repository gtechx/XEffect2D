//INLINE  ����
INLINE  void XFrame::setPosition(const XVector2& position)
{
	x = position.x;
	y = position.y;
}
INLINE  void XFrame::setPosition(float a,float b)
{
	x = a;
	y = b;
}
INLINE  XVector2 XFrame::getPosition() const
{
	return XVector2(x,y);
}
INLINE  int XFrame::getCurX() const
{
	int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	if(keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return 0;
	return (int)(x + m_keyFramePosition[keyFrameIndex].x);
}
INLINE  int XFrame::getCurY() const
{
	int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	if(keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return 0;
	return (int)(y + m_keyFramePosition[(int)m_curFramesNumble].y);
}
INLINE  int XFrame::getCurWidth() const
{
	return m_texnum[(int)m_curFramesNumble].m_w;
}
INLINE  int XFrame::getCurHeight() const
{
	int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	if(keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return 0;
	return m_texnum[(int)m_curFramesNumble].m_h;
}
INLINE  int XFrame::getInitHalfWidth() const
{
	return (int)(m_centerX);
}
INLINE  int XFrame::getInitHalfHeight() const
{
	return (int)(m_centerY);
}
INLINE  void XFrame::setSize(const XVector2& size)
{
	xsize = size.x;
	ysize = size.y;
}
INLINE  void XFrame::setSize(float a,float b)
{
	xsize = a;
	ysize = b;
}
INLINE  XVector2 XFrame::getSize()
{
	return XVector2(xsize,ysize);
}
INLINE  XBool XFrame::getIsEnd() const
{
	return m_isEnd;
}
INLINE  void XFrame::setEnd()
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
INLINE  void XFrame::reset()	//�����Ѿ�������ɵ�����֡�������²���
{
	if(!m_isEnd) return;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	m_curFramesNumble = (float)m_startFrame;
}
INLINE  XTexture * XFrame::getTexture(bool isAllFrames)
{
	if(isAllFrames)
		return m_texnum;
	else
	{
		int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
		if(keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return 0;
		return &(m_texnum[(int)(m_curFramesNumble)]);
	}
}
INLINE  void XFrame::setAngle(float temp)
{
	angle = temp;
	//����֡����Ҫ��Щֵ
	angleRadian = (float)(angle * DEGREE2RADIAN);
	sinAngle = sin(angleRadian);
	cosAngle = cos(angleRadian);
}
INLINE  float XFrame::getAngleDegree()	//��ýǶ�
{
	return angle;
}
INLINE  float XFrame::getAngleRadian()	//��û���
{
	return angleRadian;
}
INLINE  void XFrame::setAlpha(float a)
{
	if(a >= 0) alpha = a;
}
INLINE  float XFrame::getAlpha() const
{
	return alpha;
}
INLINE  void XFrame::setColor(float r,float g,float b,float a)
{
	if(r >= 0) colorRed = r;
	if(g >= 0) colorGreen = g;
	if(b >= 0) colorBlue = b;
	if(a >= 0) alpha = a;
}
INLINE  void XFrame::setActionSpeed(float actionSpeed)	//��������֡���ŵ��ٶ�
{
	m_actionSpeed = actionSpeed;
}
INLINE  void XFrame::setRestart()
{
	m_curFramesNumble = (float)(m_startFrame);
}