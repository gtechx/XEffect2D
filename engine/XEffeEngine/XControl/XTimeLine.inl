INLINE XBool XTimeLines::addKeyFrame(const std::string &name,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->addAKeyFrame(time,timeLen,data,mode);
	return XTrue;
}
//��ָ����ʱ���������ӹؼ�֡
INLINE XBool XTimeLines::addKeyFrame(unsigned int index,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->addAKeyFrame(time,timeLen,data,mode);
	return XTrue;
}
INLINE XBool XTimeLines::delCurKeyFrame(const std::string &name)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->delAKeyFrame(m_curTimer);
	return XTrue;
}
INLINE XBool XTimeLines::delCurKeyFrame(unsigned int index)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->delAKeyFrame(m_curTimer);
	return XTrue;
}
INLINE XBool XTimeLines::clearAllFroms(const std::string &name)
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return XFalse;
	m_timeLines[index]->clearAllFrame();
	return XTrue;
}
INLINE XBool XTimeLines::clearAllFroms(unsigned int index)
{
	if(index >= m_timeLines.size()) return XFalse;
	m_timeLines[index]->clearAllFrame();
	return XTrue;
}
INLINE void XTimeLines::clearAllFroms()
{
	for(unsigned int i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->clearAllFrame();
	}
}
INLINE const XOneFrame *XTimeLines::getCurKeyFrame(const std::string &name)	//��ȡ��ǰ���ڵĹؼ�֡
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return NULL;
	return m_timeLines[index]->getCurKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getNextKeyFrame(const std::string &name)	//��ȡ��һ���ؼ�֡��ָ��
{
	int index = getTimeLineIndexByName(name);
	if(index < 0) return NULL;
	return m_timeLines[index]->getNextKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getCurKeyFrame(unsigned int index)		//��ȡ��ǰ���ڵĹؼ�֡
{
	if(index >= m_timeLines.size()) return NULL;
	return m_timeLines[index]->getCurKeyFrame(m_curTimer);
}
INLINE const XOneFrame *XTimeLines::getNextKeyFrame(unsigned int index)	//��ȡ��һ���ؼ�֡��ָ��
{
	if(index >= m_timeLines.size()) return NULL;
	return m_timeLines[index]->getNextKeyFrame(m_curTimer);
}
INLINE void XTimeLines::resetAllTimeLineFrameFlag()
{//�������е�ʱ��֡
	for(unsigned int  i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->resetAllFrameFlag();
	}
}
INLINE void XTimeLines::setAllTimeLineFrameFlag()
{//�������е�ʱ��֡
	for(unsigned int  i = 0;i < m_timeLines.size();++ i)
	{
		m_timeLines[i]->setAllFrameFlag();
	}
}
INLINE void XOneTimeLine::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_title.setPosition(m_position);
}
INLINE void XOneTimeLine::setScale(float x,float y)
{
	m_scale.set(x,y);
	m_title.setScale(x,y);
}