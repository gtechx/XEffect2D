//INLINE void XRadios::setCallbackFun(void (* funStateChange)(void *,int),void *pClass)
//{
//	m_funStateChange = funStateChange;
//	if(pClass == NULL) m_pClass = this;
//	else m_pClass = pClass;
//}
INLINE int XRadios::getCurChoose() const	//���ص�ǰ��ѡ����ѡ�����ı��
{
	return m_curChoose;
}
INLINE void XRadios::setRadioText(const char *temp,int order)	//���õ�ѡ����ĳһ�������
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		temp == NULL ||
		order < 0 || order >= m_radioSum) return;
	m_radio[order].setCaptionText(temp);
}
INLINE void XRadios::setRadioState(bool state,int order)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		order < 0 || order >= m_radioSum) return;
	if(state) m_radio[order].enable();
	else m_radio[order].disable();
}
INLINE XBool XRadios::initEx(int radioSum,			//������ӿڵļ�
	const XVec2& distance,	
	const XVec2& position,	
	const XRadiosSkin* tex,const XFontUnicode& font,float captionSize)
{
	return init(radioSum,distance,position,tex->m_mouseRect,tex,font,captionSize,tex->m_fontPosition);
}
INLINE XBool XRadios::initPlus(int radioSum,			//ѡ�������
	const XVec2& distance,	//ÿ����ѡ��֮��ľ���
	const char *path,const XFontUnicode& font,float captionSize,
	XResPos resPos)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXCHECK_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	return initEx(radioSum,distance,XVec2::zero,(XRadiosSkin *)m_resInfo->m_pointer,font,captionSize);
}
INLINE XBool XRadios::canGetFocus(const XVec2& p)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(p);
}
INLINE void XRadios::disable()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isEnable) return;
	m_isEnable = XFalse;
	m_isBeChoose = XFalse;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].disable();
	}
}
INLINE void XRadios::enable()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		m_isEnable) return;
	m_isEnable = XTrue;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].enable();
	}
}
//INLINE void XRadios::setRadioPosition(float x,float y,int order)			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
//{
//	setRadioPosition(XVec2(x,y),order);
//}
INLINE void XRadios::setRadioPosition(const XVec2& position,int order)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		order < 0 || order >= m_radioSum) return;
	m_checkPosition[order] = position;
	m_radio[order].setPosition(m_position + m_checkPosition[order] * m_scale);
}
INLINE void XRadios::setDistance(const XVec2& distance)
{
	if(!m_isInited) return;
	m_distance = distance;
	//��������
	for(int i = 0;i < m_radioSum;++ i)
	{
	//	m_checkPosition[i].set(m_distance * i);
	//	m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
		m_checkPosition[i].set(m_distance * i);
		m_radio[i].setPosition(m_position + m_checkPosition[i] * m_scale);
	}
}
INLINE void XRadios::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	XFColor tmpColor = m_textColor * m_color;
	m_caption.setColor(tmpColor);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setTextColor(tmpColor);
	}
}	//�����������ɫ
INLINE void XRadios::setColor(const XFColor& c)
{
	if(!m_isInited) return;
	m_color = c;
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setColor(m_color);
	}
}	//���ð�ť����ɫ
INLINE void XRadios::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setColor(m_color);
	}
}	
INLINE XBool XRadios::isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return XMath::getIsInRect(p,getBox(0),getBox(1),getBox(2),getBox(3));
}
INLINE void XRadios::draw()
{	
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].draw();
	}
}
INLINE void XRadios::drawUp()
{	
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].drawUp();
	}
}
INLINE void XRadios::update(float stepTime)
{
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].update(stepTime);
	}
}