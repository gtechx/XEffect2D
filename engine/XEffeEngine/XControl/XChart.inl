INLINE XBool XChart::canGetFocus(float x,float y)//�Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(x,y);
}
INLINE void XChart::setLineName(const std::string &name,int lineIndex)
{
	if(lineIndex < 0 || lineIndex >= m_chartMaxLineSum) return;	//���ݲ��Ϸ�
	m_dataChecks[lineIndex].setCaptionText(name.c_str());
}
INLINE XBool XChart::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XChart::getBox(int order)
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
	}
	return XVector2::zero;
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