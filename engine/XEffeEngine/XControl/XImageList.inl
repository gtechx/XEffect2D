inline _XBool _XImageList::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	!m_isActive ||		//û�м���Ŀؼ������տ���
	!m_isVisible ||	//������ɼ�ֱ���˳�
	!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline void _XImageList::disable()		//ʹ�ؼ���Ч
{
	m_leftBtn.disable();	//��ߵİ�ť
	m_rightBtn.disable();	//�ұߵİ�ť
	m_imageSld.disable();	//������
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XImageList::enable()		//ʹ�ؼ���Ч
{
	m_leftBtn.enable();	//��ߵİ�ť
	m_rightBtn.enable();	//�ұߵİ�ť
	m_imageSld.enable();	//������
	m_isEnable = XTrue;
}
inline void _XImageList::setVisible()
{
	m_leftBtn.setVisible();	//��ߵİ�ť
	m_rightBtn.setVisible();	//�ұߵİ�ť
	m_imageSld.setVisible();	//������
	m_isVisible = XTrue;
}
inline void _XImageList::disVisible()
{
	m_leftBtn.disVisible();	//��ߵİ�ť
	m_rightBtn.disVisible();	//�ұߵİ�ť
	m_imageSld.disVisible();	//������
	m_isVisible = XFalse;
	m_isBeChoose = XFalse;
}
inline void _XImageList::setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funSelectChange = funSelectChange;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}	
//inline void _XImageList::setLostFocus()	//����ʧȥ����
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
inline void _XImageList::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_leftBtn.setPosition(m_position);
	m_rightBtn.setPosition(m_position.x + (m_buttonWidth + m_imageSize.x * m_showImageSum) * m_size.x,m_position.y);
	m_imageSld.setPosition(m_position.x + m_buttonWidth * m_size.x,m_position.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateState(false);
}
inline _XBool _XImageList::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XImageList::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline void _XImageList::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_leftBtn.setColor(m_color);
	m_rightBtn.setColor(m_color);
	m_imageSld.setColor(m_color);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setColor(m_color);
	}
}
inline void _XImageList::setAlpha(float a) 
{
	m_color.setA(a);
	m_leftBtn.setAlpha(a);
	m_rightBtn.setAlpha(a);
	m_imageSld.setAlpha(a);
	for(int i = 0;i < m_imageList.size();++ i)
	{
		m_imageList[i]->setAlpha(a);
	}
}
inline void _XImageList::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_leftBtn.drawUp();
	m_rightBtn.drawUp();
	m_imageSld.drawUp();
}
inline void _XImageList::update(int stepTime)
{
	m_leftBtn.update(stepTime);
	m_rightBtn.update(stepTime);
	m_imageSld.update(stepTime);
}