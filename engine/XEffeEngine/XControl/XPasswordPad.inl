INLINE void XPasswordPadSkin::release()
{
	if(!m_isInited) return;
	m_bottomTex.release();
	if(backGround != NULL)
	{
		//backGround->release();			//��Ч״̬�µİ�ť��ͼ
		XMem::XDELETE(backGround);
	}
	m_isInited = XFalse;
}
INLINE int XPasswordPad::getIsEnd()
{
	return m_stage;
}
INLINE void XPasswordPad::setDisShow()
{
	m_isVisible = XFalse;
}
INLINE void XPasswordPad::getPassword(char *p) const	//��ȡ�µ�����
{
	if(p == NULL) return;
	if(strlen(m_newPassword) < m_minPasswardLength) return;
	strcpy(p,m_newPassword);
}
INLINE void XPasswordPad::setPassword(const char *p)	//������Ҫ�������ȷ����
{
	if(p == NULL) return;	//��������Ƿ�
	if(strlen(p) <= m_minPasswardLength || strlen(p) > m_maxPasswardLength) return;	//���볤�Ȳ��Ϸ�
	strcpy_s(m_oldPassword,m_maxPasswardLength + 1,p);
}
INLINE XBool XPasswordPad::initEx(const XVec2& position,XPasswordPadSkin *tex,const XFontUnicode& font)
{
	return init(position,tex,font,tex->m_data);
}
INLINE XBool XPasswordPad::initPlus(const char * path,const XFontUnicode& font,
	XResPos resPos)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXPASSWORDPAD_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(XVec2::zero,(XPasswordPadSkin *)m_resInfo->m_pointer,font);
}
INLINE void XPasswordPad::setScale(const XVec2& s)
{//��δʵ��
	if(!m_isInited ||
		s.x <= 0 && s.y <= 0) return;
	m_scale = s;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	//���ø��������λ��
	m_caption.setPosition(m_position + m_passwardPadData.captionPosition * m_scale);
	m_caption.setScale(m_scale);
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position + 
			(m_passwardPadData.bottonOffsetPosition + XVec2(i % 3,(int)(i / 3)) * m_passwardPadData.bottonDistance) * m_scale);
		m_button[i].setScale(m_scale);
	}
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);

	updateChildScale();
}
INLINE void XPasswordPad::setColor(const XFColor& c)
{
	m_color = c;
	m_caption.setColor(m_color);			//��ʾ���ֵ�����
	m_sprite.setColor(m_color);		//��ʾ������ͼ
	for(int i = 0;i < 12;++ i)
		m_button[i].setColor(m_color);	//12��С���̵İ���
	updateChildColor();
}
INLINE void XPasswordPad::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);			//��ʾ���ֵ�����
	m_sprite.setAlpha(a);		//��ʾ������ͼ
	for(int i = 0;i < 12;++ i)
		m_button[i].setAlpha(a);	//12��С���̵İ���

	updateChildAlpha();
}
INLINE XBool XPasswordPad::canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE XBool XPasswordPad::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(p);
}
INLINE XVec2 XPasswordPad::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return XVec2::zero;

	switch(order)
	{
	case 0: return m_curMouseRect.getLT();
	case 1: return m_curMouseRect.getRT();
	case 2: return m_curMouseRect.getRB();
	case 3: return m_curMouseRect.getLB(); 
	}
	return XVec2::zero;
}
INLINE void XPasswordPad::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].drawUp();
	}
}
INLINE void XPasswordPad::update(float stepTime)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].update(stepTime);
	}
}