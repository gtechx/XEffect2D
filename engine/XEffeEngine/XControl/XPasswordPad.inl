inline void _XPasswordPadTexture::release()
{
	if(!m_isInited) return;
	m_bottomTex.release();
	if(backGround != NULL)
	{
		backGround->release();			//��Ч״̬�µİ�ť��ͼ
		XDELETE(backGround);
	}
	m_isInited = XFalse;
}
inline int _XPasswordPad::getIsEnd()
{
	return m_stage;
}
inline void _XPasswordPad::setDisShow()
{
	m_isVisible = XFalse;
}
inline void _XPasswordPad::getPassword(char *p) const	//��ȡ�µ�����
{
	if(p == NULL) return;
	if(strlen(m_newPassword) < MIN_PASSWARD_LENGTH) return;
	strcpy(p,m_newPassword);
}
inline void _XPasswordPad::setPassword(const char *p)	//������Ҫ�������ȷ����
{
	if(p == NULL) return;	//��������Ƿ�
	if(strlen(p) <= MIN_PASSWARD_LENGTH || strlen(p) > MAX_PASSWARD_LENGTH) return;	//���볤�Ȳ��Ϸ�
	strcpy(m_oldPassword,p);
}
inline _XBool _XPasswordPad::initEx(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font)
{
	return init(position,tex,font,tex->m_data);
}
inline _XBool _XPasswordPad::initPlus(const char * path,const _XFontUnicode& font,
	_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XPASSWORDPAD_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,(_XPasswordPadTexture *)m_resInfo->m_pointer,font);
}
inline void _XPasswordPad::setSize(float x,float y)
{//��δʵ��
	if(!m_isInited ||
		x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//���ø��������λ��
	m_caption.setPosition(m_position + m_passwardPadData.captionPosition * m_size);
	m_caption.setSize(m_size);
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position + _XVector2(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
		m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y) * m_size);
		m_button[i].setSize(m_size);
	}
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);

	updateChildSize();
}
inline void _XPasswordPad::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_color);			//��ʾ���ֵ�����
	m_sprite.setColor(m_color);		//��ʾ������ͼ
	for(int i = 0;i < 12;++ i)
		m_button[i].setColor(m_color);	//12��С���̵İ���
	updateChildColor();
}
inline void _XPasswordPad::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);			//��ʾ���ֵ�����
	m_sprite.setAlpha(a);		//��ʾ������ͼ
	for(int i = 0;i < 12;++ i)
		m_button[i].setAlpha(a);	//12��С���̵İ���

	updateChildAlpha();
}
inline _XBool _XPasswordPad::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline _XBool _XPasswordPad::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XPasswordPad::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline void _XPasswordPad::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].drawUp();
	}
}
inline void _XPasswordPad::update(int stepTime)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].update(stepTime);
	}
}