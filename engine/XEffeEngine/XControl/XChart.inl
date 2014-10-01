inline _XBool _XChart::canGetFocus(float x,float y)//�Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_nowMouseRect.isInRect(x,y);
}
inline void _XChart::setLineName(const std::string &name,int lineIndex)
{
	if(lineIndex < 0 || lineIndex >= CHART_MAX_LINE_SUM) return;	//���ݲ��Ϸ�
	m_dataChecks[lineIndex].setCaptionText(name.c_str());
}
inline _XBool _XChart::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XChart::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2:return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3:return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}
inline void _XChart::setDrawPointer(bool flag)	//�����Ƿ�����
{
	m_withPoint = flag;
	if(m_withPoint) m_neddUpdateNowLineData = true;
}
inline void _XChart::disable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable) m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XChart::enable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) m_isEnable = XTrue;
}
inline void _XChart::drawUp()
{
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].drawUp();
	}
}
inline void _XChart::update(int stepTime)
{
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].update(stepTime);
	}
}