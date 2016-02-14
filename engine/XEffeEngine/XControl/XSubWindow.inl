INLINE XBool XSubWindow::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSubWindow::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XSubWindow::setTitle(const char * title)
{
	if(!m_isInited) return;
	if(title == NULL) m_titleString = "";
	else m_titleString = title;
	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),(int)(m_area.getWidth() - 96.0f),15).c_str());
}
INLINE XBool XSubWindow::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_windowType == TYPE_NO_TITLE) return XFalse;
//	XRect tmpRect;
//	tmpRect.set(m_curMouseRect.left,m_curMouseRect.top,
//		m_curMouseRect.right,m_curMouseRect.top + 32.0f * m_scale.y);
//	return tmpRect.isInRect(x,y);
	return m_curMouseRect.isInRect(x,y);
}
INLINE XSubWindow::XShrinkDirection XSubWindow::calShrinkDir()	//���㵱ǰ���ܵ���������
{
	if(m_isInShrink) return DIR_NULL;	//����Ѵ�������״̬�������ٽ�������
	//������Ҫ���ݴ���λ�ü��㴰�ڿ��ܵ���������
	//if(m_position.x < 2.0f) return DIR_LEFT;
	//1����꿿��
	//2���ؼ�����
	if(/*m_curMousePosition.x < 5.0f && */m_position.x < 5.0f) return DIR_LEFT;
	if(/*m_curMousePosition.x > XEE::windowWidth - 5.0f && */
		m_position.x > getWindowWidth() - m_curMouseRect.getWidth() - 5.0f) return DIR_RIGHT;
	if(/*m_curMousePosition.y < 5.0f && */m_position.y < 5.0f) return DIR_UP;
	return DIR_NULL;
}
INLINE void XSubWindow::update(float stepTime)
{
	m_titleButton[0].update(stepTime);
	m_titleButton[1].update(stepTime);
	m_titleButton[2].update(stepTime);

	updateShrinkState(stepTime);
	m_ctrlManager.update(stepTime);
}
INLINE XBool XSubWindow::getIsInDragRect(float x,float y)	//�ж�λ���Ƿ�����ק������
{
	return XRect(m_curMouseRect.left,m_curMouseRect.top,
		m_curMouseRect.right - 96.0f * m_scale.x,m_curMouseRect.top + 32.0f * m_scale.y).isInRect(x,y);
}
INLINE XVector2 XSubWindow::correctPos(const XVector2 &pos)	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��
{
	XVector2 ret = pos;
	//if(pos.x < -(m_curMouseRect.getWidth() - 96.0f * m_scale.x - 5.0f)) ret.x = -(m_curMouseRect.getWidth() - 96.0f * m_scale.x - 5.0f);
	if(pos.x < 0.0f) ret.x = 0.0f;
	if(pos.x > getWindowWidth() - 5.0f) ret.x = getWindowWidth() - 5.0f;
	//if(pos.x > XEE::windowWidth - m_curMouseRect.getWidth()) ret.x = XEE::windowWidth - m_curMouseRect.getWidth();
	if(pos.y < 0.0f) ret.y = 0.0f;
	if(pos.y > getSceneHeight() - 5.0f) ret.y = getSceneHeight() - 5.0f;
	return ret;
}
INLINE void XSubWindow::updateShrinkPosition()
{
	switch(m_curShrinkDir)
	{
	case DIR_UP:		//��������
		setPosition(m_position.x,(5.0f - m_curMouseRect.getHeight()) * m_shrinkRate);
		break;
	case DIR_LEFT:		//��������
		setPosition((5.0f - m_curMouseRect.getWidth()) * m_shrinkRate,m_position.y);
		break;
	case DIR_RIGHT:		//��������
		setPosition(getWindowWidth() - 5.0f * m_shrinkRate - m_curMouseRect.getWidth() * (1.0f - m_shrinkRate),m_position.y);
		break;
	}
}