//INLINE �����Ķ���
INLINE float XLine::getLineLength() const
{
	return m_startPosition.getLength(m_endPosition);
}
INLINE float XLine::getLineAngle() const
{
	return m_angle;
}
INLINE XVector2 XLine::getStartPosition() const
{
	return m_startPosition;
}
INLINE XVector2 XLine::getEndPosition() const
{
	return m_endPosition;
}