INLINE XBool XChart::canGetFocus(const XVec2& p)//�Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(p);
}
INLINE void XChart::setLineName(const std::string& name,int lineIndex)
{
	if(lineIndex < 0 || lineIndex >= m_chartMaxLineSum) return;	//���ݲ��Ϸ�
	m_dataChecks[lineIndex].setCaptionText(name.c_str());
}
INLINE XBool XChart::isInRect(const XVec2& p)
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XChart::getBox(int order)
{
	if(!m_isInited) return XVec2::zero;
	switch(order)
	{
	case 0:return m_curMouseRect.getLT();
	case 1:return m_curMouseRect.getRT();
	case 2:return m_curMouseRect.getRB();
	case 3:return m_curMouseRect.getLB();
	}
	return XVec2::zero;
}
INLINE void XChart::setDrawPointer(bool flag)	//�����Ƿ�����
{
	m_withPoint = flag;
	if(m_withPoint) m_neddUpdateCurLineData = true;
}
INLINE void XChart::disable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XChart::enable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) m_isEnable = XTrue;
}
INLINE void XChart::drawUp()
{
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].drawUp();
	}
}
INLINE void XChart::update(float stepTime)
{
	for(int i = 0;i < m_chartMaxLineSum;++ i)
	{
		m_dataChecks[i].update(stepTime);
	}
}