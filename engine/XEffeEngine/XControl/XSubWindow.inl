INLINE XBool XSubWindow::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XSubWindow::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE void XSubWindow::setTitle(const char * title)
{
	if(!m_isInited) return;
	if(title == NULL) m_titleString = "";
	else m_titleString = title;
	m_titleFont.setString(XString::getCanShowString(m_titleString.c_str(),(int)(m_area.getWidth() - 96.0f),15).c_str());
}
INLINE XBool XSubWindow::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
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
	//������е������������ܻ�ȡ����������壬�������ܻ�ȡ������,����Ư��������Ŀؼ�ʱ������ܷ�ֹ���㴩͸
	for (unsigned int i = 0;i < m_objects.size();++i)
	{
		if (m_objects[i].obj->canGetFocus(p)) return true;
	}
	return m_curMouseRect.isInRect(p);
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
INLINE XBool XSubWindow::getIsInDragRect(const XVec2& p)	//�ж�λ���Ƿ�����ק������
{
	return XRect(m_curMouseRect.getLT(),
		m_curMouseRect.getRT() + XVec2(-96.0f, 32.0f) * m_scale).isInRect(p);
}
INLINE XVec2 XSubWindow::correctPos(const XVec2& pos)	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��
{
	XVec2 ret = pos;
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
		case DIR_DOWN:		//��������
			setPosition(m_position.x,getSceneHeight() - 5.0f * m_shrinkRate - m_curMouseRect.getHeight() * (1.0f - m_shrinkRate));
			break;
		}
	}
	INLINE XBool XSubWindow::keyboardProc(int keyOrder, XKeyState keyState)
	{
		if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
		if (m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
		if (m_isSilent) return XFalse;
		m_ctrlManager.keyProc(keyOrder, keyState);
		return XFalse;
	}