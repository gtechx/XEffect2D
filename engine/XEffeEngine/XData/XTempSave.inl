INLINE unsigned char *XTempSave::getPrevCore()	const	//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
{
	if(m_curIndex <= 0) return NULL;	//û��֮ǰ�Ķ���
	return m_usedBuff[m_curIndex - 1];
}
INLINE unsigned char *XTempSave::getNextCore()	const	//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
{
	if(m_curIndex < 0 || m_curIndex + 1 >= m_usedBuff.size()) return NULL;
	return m_usedBuff[m_curIndex + 1];
}
INLINE unsigned char *XTempSave::getCurCore() const
{
	if(m_curIndex < 0) return NULL;
	return m_usedBuff[m_curIndex];
}