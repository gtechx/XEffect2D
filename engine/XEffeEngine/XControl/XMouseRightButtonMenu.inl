inline _XBool _XMouseRightButtonMenu::initEx(int menuSum,	//������ӿڵļ�
	const _XVector2& position,	
	const _XMouseRightButtonMenuTexture &tex,	
	const _XFontUnicode &font,float captionSize)
{
	return init(menuSum,position,tex.m_mouseRect,tex,font,captionSize,tex.m_fontPosition);
}
inline _XBool _XMouseRightButtonMenu::initPlus(const char * path,int menuSum,	//�˵��е��������
	const _XFontUnicode &font,float captionSize,
	_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(menuSum,_XVector2::zero,*(_XButtonTexture *)m_resInfo->m_pointer,font,captionSize);
}
inline void _XMouseRightButtonMenu::draw()
{
	if(!m_isInited ||		//û�г�ʼ��ʱ����ʾ
		!m_isVisible) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}
inline void _XMouseRightButtonMenu::update(int stepTime)
{
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].update(stepTime);
	}
}
inline void _XMouseRightButtonMenu::drawUp()
{
	if(!m_isInited ||		//û�г�ʼ��ʱ����ʾ
		!m_isVisible) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].drawUp();
	}
}
inline _XBool _XMouseRightButtonMenu::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline _XBool _XMouseRightButtonMenu::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_allArea.isInRect(x,y);
}
inline _XVector2 _XMouseRightButtonMenu::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	if(!m_isInited) return _XVector2::zero;
	switch(order)
	{
	case 0:return _XVector2(m_allArea.left,m_allArea.top);
	case 1:return _XVector2(m_allArea.right,m_allArea.top);
	case 2:return _XVector2(m_allArea.right,m_allArea.bottom);
	case 3:return _XVector2(m_allArea.left,m_allArea.bottom);
	}
	return _XVector2::zero;
}
inline int _XMouseRightButtonMenu::getLastChoose() const										//��������ѡ���ֵ
{
	return m_lastChoose;
}
inline void _XMouseRightButtonMenu::setCallbackFun(void (* funChooseChange)(void *,int),void (* funChooseOver)(void *,int),void *pClass)	//���ò˵��Ļص�����
{
	m_funChooseChange = funChooseChange;
	m_funChooseOver = funChooseOver;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XMouseRightButtonMenu::setHotKey(int hotKey,int order)							//���ò˵���ĳһ����ȼ�
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setHotKey(hotKey);
}
inline void _XMouseRightButtonMenu::setText(const char *temp,int order)								//�ı�˵���ĳһ���ֵ
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setCaptionText(temp);
}
inline void _XMouseRightButtonMenu::setTexture(const _XMouseRightButtonMenuTexture &tex,int order)	//�ı�˵���ĳһ�����ͼ
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return;
	m_menu[order].setTexture(tex);
}
inline void _XMouseRightButtonMenu::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setColor(r,g,b,a);
	}
	updateChildColor();
}
inline void _XMouseRightButtonMenu::setAlpha(float a)
{
	m_color.setA(a);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setAlpha(a);
	}
	updateChildAlpha();
}
