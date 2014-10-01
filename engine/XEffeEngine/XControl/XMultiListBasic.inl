inline _XBool _XMultiListBasic::addALine()					//��ĩβ����һ��
{
	return setLineSum(m_tableLineSum + 1);
}
inline _XBool _XMultiListBasic::addARow()					//��ĩβ����һ��
{
	return setRowSum(m_tableRowSum + 1);
}
inline _XBool _XMultiListBasic::deleteNowSelectLine()			//ɾ��ѡȡ��һ��
{
	if(!m_isInited) return XFalse;
	if(!m_haveSelect) return XFalse;
	return deleteLine(m_selectLineOrder);
}
inline _XBool _XMultiListBasic::deleteSelectLines()
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
inline _XBool _XMultiListBasic::moveUpLine(int order)		//��order������
{
	return moveDownLine(order - 1);
}
inline _XBool _XMultiListBasic::moveLeftRow(int order)		//��order������
{
	return moveRightRow(order - 1);
}
inline _XBool _XMultiListBasic::getHaveSelectState(int i)
{
	if(i < 0 || i >= m_tableLineSum) return XFalse;
	return m_haveSelectFlag[i];
}
inline int _XMultiListBasic::getTableLineSum()
{
	return m_tableLineSum;
}
inline int _XMultiListBasic::getSelectIndex() const	//����ѡ��������к�
{
	return m_selectLineOrder;
}
inline void _XMultiListBasic::setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass)
{
	if(!m_isInited) return;
	m_funCheckStateChange = funCheckStateChange;
	m_funSelectChange = funSelectChange;
	m_pClass = pClass;
}
inline _XBool _XMultiListBasic::getCheckState(int order,int lineOrder)
{
	if(!m_isInited) return XFalse;
//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return XFalse;
//	if(order == 0) return m_check0[lineOrder].getState();
//	else return m_check1[lineOrder].getState();
	if(lineOrder < 0 || lineOrder >= m_tableLineSum) return XFalse;
	if(order == 0) return m_check0State[lineOrder];
	else return m_check1State[lineOrder];
}
inline void _XMultiListBasic::setCheckState(int order,int lineOrder,_XBool state)
{
	if(!m_isInited) return;
//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return;
//	if(order == 0) m_check0[lineOrder].setState(state);
//	else m_check1[lineOrder].setState(state);
	if(lineOrder < 0 || lineOrder >= m_tableLineSum) return;
	if(order == 0) m_check0State[lineOrder] = state;
	else m_check1State[lineOrder] = state;
	//��������б���ѡ��״̬����Ϣ
	if(lineOrder >= m_showStartLine && lineOrder < min(m_showStartLine + m_canShowLineSum,m_tableLineSum))
	{
		if(m_check0State[lineOrder]) m_check0[lineOrder - m_showStartLine].setState(XTrue);
		else m_check0[lineOrder - m_showStartLine].setState(XFalse);
		if(m_check1State[lineOrder]) m_check1[lineOrder - m_showStartLine].setState(XTrue);
		else m_check1[lineOrder - m_showStartLine].setState(XFalse);
	}
}
inline _XBool _XMultiListBasic::initEx(const _XVector2& position,		//����ӿڵļ򻯰汾
	const _XMultiListTexture &tex,		
	const _XCheckTexture &checktex0,		
	const _XCheckTexture &checktex1,		
	const _XFontUnicode &font,			
	float strSize,						
	int rowSum,				
	int lineSum,			
	//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
	const _XSlider &vSlider,	
	const _XSlider &hSlider)
{
	return init(position,tex.m_mouseRect,tex,checktex0,checktex1,font,strSize,rowSum,lineSum,vSlider,hSlider);
}
inline _XBool _XMultiListBasic::keyboardProc(int keyOrder,_XKeyState keyState)
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
inline _XBool _XMultiListBasic::canGetFocus(float x,float y)	//�����жϵ�ǰ����Ƿ���Ի�ý���
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
inline void _XMultiListBasic::setTextColor(const _XFColor& color) 
{
	if(!m_isInited) return;
	m_textColor = color;
	m_caption.setColor(m_textColor);
}
inline void _XMultiListBasic::setColor(float r,float g,float b,float a) 
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
		_XMultiListOneRow *tempRow = m_tableRow;
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
			_XMultiListOneBox *tempBox = m_tableBox;
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
inline void _XMultiListBasic::setAlpha(float a) 
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
		_XMultiListOneRow *tempRow = m_tableRow;
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
			_XMultiListOneBox *tempBox = m_tableBox;
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
inline _XBool _XMultiListBasic::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return m_nowMouseRect.isInRect(x,y);
}
inline _XVector2 _XMultiListBasic::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
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
inline _XBool _XMultiListBasic::canLostFocus(float x,float y) 
{
	if(m_needShowVSlider && !m_verticalSlider.canLostFocus(x,y)) return XFalse;
	if(m_needShowHSlider && !m_horizontalSlider.canLostFocus(x,y)) return XFalse;
	return XTrue;
}
inline void _XMultiListBasic::setLostFocus() 
{
	m_verticalSlider.setLostFocus();
	m_horizontalSlider.setLostFocus();
	m_isBeChoose = XFalse;
}
inline void _XMultiListBasic::drawUp()
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
inline void _XMultiListBasic::update(int stepTime)
{
	if(m_needShowVSlider) m_verticalSlider.update(stepTime);
	if(m_needShowHSlider) m_horizontalSlider.update(stepTime);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].update(stepTime);
		m_check1[i].update(stepTime);
	}
}