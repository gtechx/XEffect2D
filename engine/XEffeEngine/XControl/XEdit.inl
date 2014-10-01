inline void _XEditTexture::releaseTex()
{
	XDELETE(editNormal);
	XDELETE(editDisable);
	XDELETE(editSelect);
	XDELETE(editInsert);
	XDELETE(editUpon);
}
inline void _XEditTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
inline void _XEdit::upDateInsertShowPosition()	//����ʵ��������¹����ʾ��λ��
{
	if(m_withoutTex) return;
	m_spriteInsert.setPosition(m_position.x + m_textPosition.x * m_size.x 
		+ (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
		m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
}
inline _XBool _XEdit::initEx(const _XVector2& position,	//����ӿڵļ򻯰汾
	const _XEditTexture &tex,			
	const char *str,					
	const _XFontUnicode &font,			
	float strSize,	
	_XMouseRightButtonMenu * mouseMenu)
{
	return init(position,tex.m_mouseRect,tex,str,font,strSize,mouseMenu);
}
inline _XBool _XEdit::initPlus(const char * path,			//�ؼ�����ͼ
	const char *str,					//�ؼ��ĳ�ʼ���ַ���
	const _XFontUnicode &font,			//�ؼ�������
	float strSize,	//�ؼ���������Ϣ
	_XMouseRightButtonMenu * mouseMenu,//�ؼ����Ҽ��˵�
	_XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XEDIT_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	return initEx(_XVector2::zero,*(_XEditTexture *)m_resInfo->m_pointer,str,font,strSize,mouseMenu);
}
inline void _XEdit::update(int stepTime)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_comment.update(stepTime);
	if(m_isEnable && m_isBeChoose)
	{//��������˸�ļ�ʱ
		m_timer += stepTime;
		if(m_timer > 500) m_timer -= 500;
		if(m_nowKeyDown)
		{
			m_nowKeyDownTimer += XEE::frameTime;
			if(m_nowKeyDownTimer > 500)
			{//��ʼ�ظ�����
				m_nowKeyRepTimer += XEE::frameTime;
				if(m_nowKeyRepTimer >= 50)
				{
					m_nowKeyRepTimer = 0;
					keyProc(m_nowKey);
				}
			}
		}
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_oldSize.x,
			m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_oldSize.y);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
	if(!m_insertActionMD.getIsEnd())
	{
		m_insertActionMD.move(stepTime);
	}
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->update(stepTime);
}
inline void _XEdit::drawUp()				//���С�˵�
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	//��ʾ����Ҽ��˵�
	if(m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu->draw();
		m_mouseRightButtonMenu->drawUp();
	}
	if(!m_insertActionMD.getIsEnd())
	{
		float nowData = m_insertActionMD.getNowData();
		float x = m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth;
		float y = m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f * nowData;
		float r = lineSlerp<float>(1.0f,0.25f * m_color.fR,m_insertActionMD.getNowTimer());
		float g = lineSlerp<float>(1.0f,0.25f * m_color.fG,m_insertActionMD.getNowTimer());
		float b = lineSlerp<float>(1.0f,0.25f * m_color.fB,m_insertActionMD.getNowTimer());
		drawLine(x,y,x,y + m_nowTextHeight * nowData,1.0f * nowData,
			r,g,b,m_color.fA * m_insertActionMD.getNowTimer());
	}
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		drawRect(m_lightRect,1.0f * m_lightMD.getNowData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getNowTimer()) * 0.5f);
	}
	m_comment.draw();
}
inline _XBool _XEdit::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline _XBool _XEdit::canLostFocus(float,float)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XTrue;		//�����Ч��ֱ���˳�
	if(m_selectMouseDown) return false;
	if(m_mouseRightButtonMenu != NULL && m_mouseRightButtonMenu->getVisible()) return XFalse;
	//if(m_isBeChoose) return XFalse;
	return XTrue;
}
inline void _XEdit::setLostFocus() 
{
	if(m_haveSelect) m_haveSelect = XFalse;
	m_isBeChoose = XFalse;
	m_selectMouseDown = XFalse;
	m_nowKeyDown = XFalse;
}
inline void _XEdit::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_spriteBackGround.setColor(m_color);
	m_spriteSelect.setColor(m_color);
	m_spriteInsert.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->setColor(m_color);
	updateChildColor();
}	//���ð�ť����ɫ
inline void _XEdit::setAlpha(float a) 
{
	m_color.setA(a);
	m_spriteBackGround.setColor(m_color);
	m_spriteSelect.setColor(m_color);
	m_spriteInsert.setColor(m_color);
	m_caption.setColor(m_textColor * m_color);
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->setAlpha(a);
	updateChildAlpha();
}
inline _XBool _XEdit::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XEdit::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline void _XEdit::deleteSelectStr()
{
	if(getSelectEnd() == m_nowStringLength)
	{//Ƭѡ������
		m_nowStringLength -= getSelectLength();
		m_nowString[getSelectHead()] = '\0';
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}else
	{
		strcpy(m_nowString + getSelectHead(),m_nowString + getSelectEnd());
		m_nowStringLength -= getSelectLength();
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}
}
inline void _XEdit::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	upDateInsertShowPosition();
	upDataSelectShow();
	updateChildPos();
}
inline void _XEdit::disable()					//ʹ��Ч
{
	m_isEnable = XFalse;
	if(m_withAction && m_isBeChoose)
	{
		m_insertActionMD.set(10.0f,1.0f,0.002f,MOVE_DATA_MODE_SQRT2_MULT);
	}
	m_isBeChoose = XFalse;
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->disVisible(); //ȡ���Ҽ��˵�����ʾ
}
inline void _XEdit::enable()					//ʹ��
{
	m_isEnable = XTrue;
}
inline  char *_XEdit::getString() const				//����������ַ���
{
	return m_nowString;
}
inline int _XEdit::getSelectLength() const				//���ѡ���ĳ���
{
	if(m_selectEnd < m_selectStart) return m_selectStart - m_selectEnd;
	else return m_selectEnd - m_selectStart;
}
inline int _XEdit::getSelectHead() const				//���ѡȡ��ͷ
{
	if(m_selectEnd < m_selectStart)return  m_selectEnd;
	else return m_selectStart;
}
inline int _XEdit::getSelectEnd() const				//���ѡȡ��β
{
	if(m_selectEnd < m_selectStart) return m_selectStart;
	else return m_selectEnd;
}
inline void _XEdit::setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass)		//���ûص�����
{
	m_funInputChenge = funInputChenge;
	m_funInputOver = funInputOver;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline _XBool _XEdit::getEdtIsNumber()	//����������Ƿ�Ϊ��ֵ
{
	if(getIsNumber(getString())) return XTrue;
	return getIsHexNumber(getString()) >= 0;
}
inline int _XEdit::getAsInt()
{
	if(getIsNumber(getString())) return atoi(getString());
	return getIsHexNumber(getString());
}
//_XBool getAsBool();
inline float _XEdit::getAsFloat()
{
	if(getIsNumber(getString())) return toValue<float>(getString());
	return getIsHexNumber(getString());
}
inline void _XEdit::setTextColor(const _XFColor& color) 
{
	m_textColor = color;
	m_caption.setColor(m_textColor * m_color);
}
inline void _XEdit::setIsPassword(bool flag) 
{
	if((m_isPassword && flag) ||
		(!m_isPassword && !flag)) return;	//�ظ�������
	m_isPassword = flag;
	m_caption.setIsPassWord(m_isPassword);
	//upDataShowStr();
}