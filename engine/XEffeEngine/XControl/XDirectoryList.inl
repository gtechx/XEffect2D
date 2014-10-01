inline void _XDirListTexture::release()
{
	if(!m_isInited) return;
	XDELETE(dirListNormal);
	XDELETE(dirListDisable);
	m_isInited = XFalse;
}
inline void _XDirListOneLine::release()
{
	if(!m_isEnable) return;
	XDELETE_ARRAY(m_string);
	XDELETE(m_check);
	m_isEnable = XFalse;
}
inline void _XDirectoryList::setCanChangePath(_XBool flag)
{
	if((flag && m_canChangePath) || (!flag && !m_canChangePath)) return;
	m_canChangePath = flag;
	if(flag) m_edit.enable();
	else m_edit.disable();
}
inline _XBool _XDirectoryList::initEx(const _XVector2& position,	//������ӿڵļ�
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,
		const _XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,font,fontSize,check,button,edit,vSlider,hSlider);
}
inline const char * _XDirectoryList::getSelectFileName() const	//��ȡȫ·��
{
	if(m_haveSelect) return m_lineData[m_selectLineOrder]->m_file->allPath;
	else return NULL;
}
inline int _XDirectoryList::getSelectLineOrder() const
{
	if(m_haveSelect) return m_selectLineOrder;
	else return -1;
}
inline _XBool _XDirectoryList::canGetFocus(float x,float y)//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline void _XDirectoryList::setPosition(float x,float y)
{
	m_position.set(x,y);
	updateShowPosition();
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
	m_edit.setPosition(m_position);
	m_nowMouseRect.set(m_position.x,m_position.y,
		m_position.x + (m_mouseRect.getWidth() + m_verticalSlider.getMouseRect().getWidth()) * m_size.x,
		m_position.y + (m_mouseRect.getHeight() + m_edit.getMouseRect().getHeight() + m_horizontalSlider.getMouseRect().getHeight()) * m_size.y);
	updateChildPos();
}
inline void _XDirectoryList::setSize(float x,float y)
{//���ϽǶ���
	m_size.set(x,y);
	updateShowPosition();
	if(!m_withoutTex) 
	{
		m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
		m_spriteBackGround.setSize(m_size);
	}
	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_button.setSize(m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
	m_verticalSlider.setSize(m_size);
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
	m_horizontalSlider.setSize(m_size);
	m_edit.setPosition(m_position);
	m_edit.setSize(m_size);
	m_nowMouseRect.set(m_position.x,m_position.y,
		m_position.x + (m_mouseRect.getWidth() + m_verticalSlider.getMouseRect().getWidth()) * m_size.x,
		m_position.y + (m_mouseRect.getHeight() + m_edit.getMouseRect().getHeight() + m_horizontalSlider.getMouseRect().getHeight()) * m_size.y);
	updateChildSize();
}
inline _XBool _XDirectoryList::isInRect(float x,float y)						//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XBool _XDirectoryList::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return XTrue;	//������ɼ�ֱ���˳�
	if(m_needShowVSlider) m_verticalSlider.keyboardProc(keyOrder,keyState);
	if(m_needShowHSlider) m_horizontalSlider.keyboardProc(keyOrder,keyState);
	m_edit.keyboardProc(keyOrder,keyState);
	m_button.keyboardProc(keyOrder,keyState);
	return XTrue;
}
inline _XVector2 _XDirectoryList::getBox(int order)		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.top);
	case 1: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.top);
	case 2: return _XVector2(m_nowMouseRect.right,m_nowMouseRect.bottom);
	case 3: return _XVector2(m_nowMouseRect.left,m_nowMouseRect.bottom);
	}
	return _XVector2::zero;
}