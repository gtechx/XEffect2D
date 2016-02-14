INLINE XBool XMultiListBasic::addALine()					//��ĩβ����һ��
{
	return setLineSum(m_tableLineSum + 1);
}
INLINE XBool XMultiListBasic::addARow()					//��ĩβ����һ��
{
	return setRowSum(m_tableRowSum + 1);
}
INLINE XBool XMultiListBasic::deleteCurSelectLine()			//ɾ��ѡȡ��һ��
{
	if(!m_isInited) return XFalse;
	if(!m_haveSelect) return XFalse;
	return deleteLine(m_selectLineOrder);
}
INLINE XBool XMultiListBasic::deleteSelectLines()
{
	if(!m_isInited) return XFalse;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_haveSelectFlag[i])
		{
			deleteLine(i);
			-- i;
		}
	}
	m_haveSelect = XFalse;	//���ѡ��ı��
	m_selectLineOrder = 0;
	return XTrue;
}
INLINE XBool XMultiListBasic::moveUpLine(int order)		//��order������
{
	return moveDownLine(order - 1);
}
INLINE XBool XMultiListBasic::moveLeftRow(int order)		//��order������
{
	return moveRightRow(order - 1);
}
INLINE XBool XMultiListBasic::getHaveSelectState(int i)
{
	if(i < 0 || i >= m_tableLineSum) return XFalse;
	return m_haveSelectFlag[i];
}
INLINE int XMultiListBasic::getTableLineSum()
{
	return m_tableLineSum;
}
INLINE int XMultiListBasic::getSelectIndex() const	//����ѡ��������к�
{
	return m_selectLineOrder;
}
//INLINE void XMultiListBasic::setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass)
//{
//	if(!m_isInited) return;
//	m_funCheckStateChange = funCheckStateChange;
//	m_funSelectChange = funSelectChange;
//	m_pClass = pClass;
//}
INLINE XBool XMultiListBasic::getCheckState(int order,int lineOrder)
{
	if(!m_isInited) return XFalse;
//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return XFalse;
//	if(order == 0) return m_check0[lineOrder].getState();
//	else return m_check1[lineOrder].getState();
	if(lineOrder < 0 || lineOrder >= m_tableLineSum) return XFalse;
	if(order == 0) return m_check0State[lineOrder];
	else return m_check1State[lineOrder];
}
INLINE void XMultiListBasic::setCheckState(int order,int lineOrder,XBool state)
{
	if(!m_isInited) return;
//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return;
//	if(order == 0) m_check0[lineOrder].setState(state);
//	else m_check1[lineOrder].setState(state);
	if(lineOrder < 0 || lineOrder >= m_tableLineSum) return;
	if(order == 0) m_check0State[lineOrder] = state;
	else m_check1State[lineOrder] = state;
	//��������б���ѡ��״̬����Ϣ
	if(lineOrder >= m_showStartLine && lineOrder < XEE_Min(m_showStartLine + m_canShowLineSum,m_tableLineSum))
	{
		if(m_check0State[lineOrder]) m_check0[lineOrder - m_showStartLine].setState(XTrue);
		else m_check0[lineOrder - m_showStartLine].setState(XFalse);
		if(m_check1State[lineOrder]) m_check1[lineOrder - m_showStartLine].setState(XTrue);
		else m_check1[lineOrder - m_showStartLine].setState(XFalse);
	}
}
INLINE XBool XMultiListBasic::initEx(const XVector2& position,		//����ӿڵļ򻯰汾
	const XMultiListSkin &tex,		
	const XCheckSkin &checktex0,		
	const XCheckSkin &checktex1,		
	const XFontUnicode &font,			
	float strSize,						
	int rowSum,				
	int lineSum,			
	//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
	const XSlider &vSlider,	
	const XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,checktex0,checktex1,font,strSize,rowSum,lineSum,vSlider,hSlider);
}
INLINE XBool XMultiListBasic::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(keyOrder == XKEY_LSHIFT || keyOrder == XKEY_RSHIFT)
	{
		m_stateOfShiftKey = keyState;
	}
	return XTrue;
}
INLINE XBool XMultiListBasic::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
INLINE void XMultiListBasic::setTextColor(const XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor);
}
INLINE void XMultiListBasic::setColor(float r,float g,float b,float a) 
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);;
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
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].setColor(m_color);
		m_check1[i].setColor(m_color);
	}
	updateChildColor();
}	//���ð�ť����ɫ
INLINE void XMultiListBasic::setAlpha(float a) 
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
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].setAlpha(a);
		m_check1[i].setAlpha(a);
	}
	updateChildAlpha();
}
INLINE XBool XMultiListBasic::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_curMouseRect.isInRect(x,y);
}
INLINE XVector2 XMultiListBasic::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
INLINE XBool XMultiListBasic::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
INLINE void XMultiListBasic::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
INLINE void XMultiListBasic::drawUp()
{
	if(!m_isInited ||
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_needShowVSlider) m_verticalSlider.drawUp();
	if(m_needShowHSlider) m_horizontalSlider.drawUp();
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].drawUp();
		m_check1[i].drawUp();
	}
}
INLINE void XMultiListBasic::update(float stepTime)
{
	if(m_needShowVSlider) m_verticalSlider.update(stepTime);
	if(m_needShowHSlider) m_horizontalSlider.update(stepTime);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].update(stepTime);
		m_check1[i].update(stepTime);
	}
}