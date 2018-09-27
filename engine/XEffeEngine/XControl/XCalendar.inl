INLINE XBool XCalendar::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(p);
}
INLINE void XCalendar::setLostFocus() 
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
}
INLINE XBool XCalendar::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XCalendar::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XCalendar::drawUp()
{
	m_yearAddBtn.drawUp();
	m_yearDecBtn.drawUp();
	m_monthAddBtn.drawUp();
	m_monthDecBtn.drawUp();
}
INLINE void XCalendar::update(float stepTime)
{
	m_yearAddBtn.update(stepTime);
	m_yearDecBtn.update(stepTime);
	m_monthAddBtn.update(stepTime);
	m_monthDecBtn.update(stepTime);
}