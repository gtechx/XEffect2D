INLINE void XMultiListSkin::releaseTex()
{
	XMem::XDELETE(mutiListNormal);
	XMem::XDELETE(mutiListDisable);
	XMem::XDELETE(mutiListSelect);
	XMem::XDELETE(mutiListMove);
	XMem::XDELETE(mutiListTitle);
	XMem::XDELETE(mutiListTitleEnd);
}
INLINE void XMultiListSkin::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}
INLINE int XMultiList::getSelectIndex()
{
	if(!m_haveSelect) return -1;
	return m_selectLineOrder;
}
//INLINE void XMultiList::setSelectFun(void (* funSelectFun)(void *,int),void * pClass)
//{
//	m_funSelectFun = funSelectFun;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}
//INLINE void XMultiList::setDClickFun(void (* funDClick)(void *,int),void * pClass)
//{
//	m_funDClick = funDClick;
//	if(pClass != NULL) m_pClass = pClass;
//	else m_pClass = this;
//}
INLINE XBool XMultiList::initEx(const XVector2& position,		//������ӿڵļ�
	const XMultiListSkin &tex,		
	const XFontUnicode &font,			
	float strSize,						
	int rowSum,					
	int lineSum,				
	//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
	const XSlider &vSlider,	
	const XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,font,strSize,rowSum,lineSum,vSlider,hSlider);
}
INLINE XBool XMultiList::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XMultiList::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	if(!m_withoutTex)
	{
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
	}
	m_caption.setColor(m_textColor * m_color);
	m_verticalSlider.setColor(m_color);
	m_horizontalSlider.setColor(m_color);
	if(m_tableRowSum > 0)
	{
		XMultiListOneRow *tempRow = m_tableRow;
		while(true)
		{
			if(tempRow->isEnable && tempRow->isShow != 0)
			{
				tempRow->text.setAlpha(m_color.fA);//��ʾ��������
			}
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
		}
		if(m_tableLineSum > 0)
		{
			XMultiListOneBox *tempBox = m_tableBox;
			while(true)
			{
				if(tempBox->isEnable && tempBox->isShow) tempBox->text.setAlpha(m_color.fA);//��ʾ��������
				if(tempBox->nextBox == NULL) break;
				else tempBox = tempBox->nextBox;
			}
		}
	}
}	//���ð�ť����ɫ
INLINE void XMultiList::setAlpha(float a) 
{
	if(!m_isInited) return;
	m_color.setA(a);
	if(!m_withoutTex)
	{
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
	}
	m_caption.setColor(m_textColor * m_color);
	m_verticalSlider.setAlpha(a);
	m_horizontalSlider.setAlpha(a);
	if(m_tableRowSum > 0)
	{
		XMultiListOneRow *tempRow = m_tableRow;
		while(true)
		{
			if(tempRow->isEnable && tempRow->isShow != 0)
			{
				tempRow->text.setAlpha(a);//��ʾ��������
			}
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
		}
		if(m_tableLineSum > 0)
		{
			XMultiListOneBox *tempBox = m_tableBox;
			while(true)
			{
				if(tempBox->isEnable && tempBox->isShow) tempBox->text.setAlpha(a);//��ʾ��������
				if(tempBox->nextBox == NULL) break;
				else tempBox = tempBox->nextBox;
			}
		}
	}
}
INLINE XBool XMultiList::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE void XMultiList::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor);
}
INLINE void XMultiList::updateSelectLine()					//����ѡ���е�״̬
{
	if(!m_isInited || 	//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return;
	if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum && !m_withoutTex)
	{//����ѡ���λ��
		m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
			m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
			(m_selectLineOrder - m_showStartLine) * m_curTextHeight);
	}
}
INLINE XBool XMultiList::addALine()					//��ĩβ����һ��
{
	return setLineSum(m_tableLineSum + 1);
}
INLINE XBool XMultiList::addARow()					//��ĩβ����һ��
{
	return setRowSum(m_tableRowSum + 1);
}
INLINE XBool XMultiList::deleteSelectLine()			//ɾ��ѡȡ��һ��
{
	if(!m_isInited ||
		!m_haveSelect) return XFalse;
	return deleteLine(m_selectLineOrder);
}
INLINE XBool XMultiList::moveUpLine(int order)		//��order������
{
	return moveDownLine(order - 1);
}
INLINE XBool XMultiList::moveLeftRow(int order)		//��order������
{
	return moveRightRow(order - 1);
}
INLINE XVector2 XMultiList::getBox(int order)
{
	if(!m_isInited) return XVector2::zero;
	switch(order)
	{
	case 0: return XVector2(m_curMouseRect.left,m_curMouseRect.top);
	case 1: return XVector2(m_curMouseRect.right,m_curMouseRect.top);
	case 2: return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
	case 3: return XVector2(m_curMouseRect.left,m_curMouseRect.bottom); 
	}

	return XVector2::zero;
}
INLINE XBool XMultiList::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
INLINE void XMultiList::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
INLINE void XMultiList::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_needShowVSlider) m_verticalSlider.drawUp();
	if(m_needShowHSlider) m_horizontalSlider.drawUp();
}
INLINE void XMultiList::update(float stepTime)
{
	if(m_needShowVSlider) m_verticalSlider.update(stepTime);
	if(m_needShowHSlider) m_horizontalSlider.update(stepTime);
	if(!m_actionMD.getIsEnd())
	{
		m_actionMD.move(stepTime);
	}
}
INLINE void XMultiList::setAction(XMultiListActionType type,int index)
{
	switch(type)
	{
	case MLTLST_ACTION_TYPE_IN:		//ѡ�����
		m_actionMD.set(0.0f,1.0f,0.005f,MOVE_DATA_MODE_SHAKE);
		break;
	case MLTLST_ACTION_TYPE_MOVE:	//ѡ���ƶ�
		m_actionMD.set(0.0f,1.0f,0.005f);
		break;
	case MLTLST_ACTION_TYPE_DCLICK:	//˫��
		m_actionMD.set(1.0f,1.2f,0.01f,MOVE_DATA_MODE_SIN_MULT,1);
		break;
	case MLTLST_ACTION_TYPE_OUT:		//ȡ��ѡ��
		m_actionMD.set(1.0f,0.0f,0.005f);
		break;
	}
	m_actionType = type;
	m_actionPosition = index;
}