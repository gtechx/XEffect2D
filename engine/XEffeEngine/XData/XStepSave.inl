INLINE unsigned char *XStepSave::getPrevCore()	const	//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
{
	if(m_curIndex <= 0) return NULL;	//û��֮ǰ�Ķ���
	return m_usedBuff[m_curIndex - 1];
}
INLINE unsigned char *XStepSave::getNextCore()	const	//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
{
	if(m_curIndex < 0 || m_curIndex + 1 >= m_usedBuff.size()) return NULL;
	return m_usedBuff[m_curIndex + 1];
}
INLINE unsigned char *XStepSave::getCurCore() const
{
	if(m_curIndex < 0) return NULL;
	return m_usedBuff[m_curIndex];
}