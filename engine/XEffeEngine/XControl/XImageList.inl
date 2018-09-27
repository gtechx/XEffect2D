INLINE XBool XImageList::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	!m_isActive ||		//û�м���Ŀؼ������տ���
	!m_isVisible ||	//������ɼ�ֱ���˳�
	!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XImageList::disable()		//ʹ�ؼ���Ч
{
	m_leftBtn.disable();	//��ߵİ�ť
	m_rightBtn.disable();	//�ұߵİ�ť
	m_imageSld.disable();	//������
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
INLINE void XImageList::enable()		//ʹ�ؼ���Ч
{
	m_leftBtn.enable();	//��ߵİ�ť
	m_rightBtn.enable();	//�ұߵİ�ť
	m_imageSld.enable();	//������
	m_isEnable = XTrue;
}
INLINE void XImageList::setVisible()
{
	m_leftBtn.setVisible();	//��ߵİ�ť
	m_rightBtn.setVisible();	//�ұߵİ�ť
	m_imageSld.setVisible();	//������
	m_isVisible = XTrue;
}
INLINE void XImageList::disVisible()
{
	m_leftBtn.disVisible();	//��ߵİ�ť
	m_rightBtn.disVisible();	//�ұߵİ�ť
	m_imageSld.disVisible();	//������
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
}
//INLINE void XImageList::setCallbackFun(void (* funInit)(void *,int),
//		void (* funRelease)(void *,int),
//		void (* funSelectChange)(void *,int),
//		void *pClass)
//{
//	m_funInit = funInit;
//	m_funRelease = funRelease;
//	m_funSelectChange = funSelectChange;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}	
//INLINE void XImageList::setLostFocus()	//����ʧȥ����
//{
//	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
//		!m_isActive ||		//û�м���Ŀؼ������տ���
//		!m_isVisible ||	//������ɼ�ֱ���˳�
//		!m_isEnable) return;		//�����Ч��ֱ���˳�
//
//	//m_leftBtn.setLostFocus();
//	//m_rightBtn.setLostFocus();
//	//m_imageSld.setLostFocus();
//	m_isBeChoose = XFalse;
//}
INLINE void XImageList::setPosition(const XVec2& p)
{
	m_position = p;
	m_leftBtn.setPosition(m_position);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_scale.x,m_position.y);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_scale.x,m_position.y);
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	updateState(false);
}
INLINE XBool XImageList::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XImageList::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XImageList::setColor(const XFColor& c)
{
	m_color = c;
	m_leftBtn.setColor(m_color);
	m_rightBtn.setColor(m_color);
	m_imageSld.setColor(m_color);
	for(auto it = m_imageList.begin();it != m_imageList.end();++ it)
	{
		(*it)->setColor(m_color);
	}
}
INLINE void XImageList::setAlpha(float a) 
{
	m_color.setA(a);
	m_leftBtn.setAlpha(a);
	m_rightBtn.setAlpha(a);
	m_imageSld.setAlpha(a);
	for (auto it = m_imageList.begin(); it != m_imageList.end(); ++it)
	{
		(*it)->setAlpha(a);
	}
}
INLINE void XImageList::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
	m_imageSld.drawUp();
}
INLINE void XImageList::update(float stepTime)
{
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	m_imageSld.update(stepTime);
}