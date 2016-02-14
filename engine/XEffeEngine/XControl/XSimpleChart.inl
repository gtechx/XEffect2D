INLINE void XSimpleChart::clearAll()
{
	m_datas.clear();
	m_isFirstPush = false;	//�Ƿ��ǵ�һ�η�������
	m_maxData = 0.0f;	//�������ݵ����ֵ
	m_minData = 0.0f;	//�������ݵ���Сֵ
	m_curData = 0.0f;
	m_localMaxData = 0.0f;
	m_localMinData = 0.0f;
	m_localAverageData = 0.0f;
	m_localSDData = 0.0f;
	m_rateY = 1.0f;
	updateInfoStr();
}
INLINE void XSimpleChart::updateInfoStr()
{
	char tmpStr[256];
	switch (m_showInfo)
	{
	case INFO_NULL:break;
	case INFO_CUR_VALUE:
		sprintf(tmpStr,"Val:%8.5f",m_curData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_ALL_RANGLE:
		sprintf(tmpStr,"ARng:%8.5f - %8.5f",m_minData,m_maxData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_RANGLE:
		sprintf(tmpStr,"LRng:%8.5f - %8.5f",m_localMinData,m_localMaxData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_AVERAGE:
		sprintf(tmpStr,"Ave:%8.5f",m_localAverageData);
		m_infoFont.setString(tmpStr);
		break;
	case INFO_LOCAL_SD:
		sprintf(tmpStr,"SD:%8.5f",m_localSDData);
		m_infoFont.setString(tmpStr);
		break;
	}
}
INLINE void XSimpleChart::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + 1.0f * m_scale.y);
	m_infoFont.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + (1.0f + 32.0f) * m_scale.y);
	m_curMouseRect.set(m_position,m_position + m_size * m_scale);
	updateChildPos();
}
INLINE void XSimpleChart::setScale(float x,float y)
{
	if(x < 0.0f || y < 0.0f) return;
	m_scale.set(x,y);
	m_caption.setScale(m_scale);
	m_caption.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + 1.0f * m_scale.y);
	m_infoFont.setScale(m_scale);
	m_infoFont.setPosition(m_position.x + 1.0f * m_scale.x,m_position.y + (1.0f + 32.0f) * m_scale.y);
	m_curMouseRect.set(m_position,m_position + m_size * m_scale);
	updateChildScale();
}
INLINE void XSimpleChart::setColor(float r,float g,float b,float a)
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_textColor * m_color);
	m_infoFont.setColor(m_textColor * m_color);
	updateChildColor();
}
INLINE void XSimpleChart::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setColor(m_textColor * m_color);
	m_infoFont.setColor(m_textColor * m_color);
	updateChildAlpha();
}
INLINE void XSimpleChart::setCaption(const char *p)
{
	if(!m_isInited) return;
	if(p == NULL)
	{
		m_showCaption = false;
	}else
	{
		m_caption.setString(p);
		m_showCaption = true;
	}
}
INLINE XBool XSimpleChart::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XSimpleChart::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XSimpleChart::mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	return m_curMouseRect.isInRect(x,y);
}
INLINE XBool XSimpleChart::canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||		//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_curMouseRect.isInRect(x,y);
}