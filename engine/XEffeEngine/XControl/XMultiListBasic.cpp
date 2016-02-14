#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiListBasic.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XMultiListBasic::ctrlProc(void *pClass,int id,int eventID)
{
	XMultiListBasic &pPar = *(XMultiListBasic *)pClass;
	if(id ==  pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				//��Ҫ���´�ֱƬѡ�����
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
				pPar.m_verticalSlider.setCurValue(temp);
			}else
			{//΢���ƶ�ʱ��Ĵ���
				if(pPar.m_verticalSlider.getCurValue() > pPar.m_showStartLine)
				{
					++temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
					pPar.m_verticalSlider.setCurValue(temp);
				}else
				if(pPar.m_verticalSlider.getCurValue() < pPar.m_showStartLine)
				{
					--temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
					pPar.m_verticalSlider.setCurValue(temp);
				}
			}
		}
		return;
	}
	if(id == pPar.m_horizontalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
				pPar.m_horizontalSlider.setCurValue(temp);
			}else
			{
				if(pPar.m_horizontalSlider.getCurValue() > pPar.m_showStartRow)
				{
					++temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}else
				if(pPar.m_horizontalSlider.getCurValue() < pPar.m_showStartRow)
				{
					--temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}
			}
		}
		return;
	}
	if(eventID == XCheck::CHK_STATE_CHANGE)
	{
		//��ֵ������ȥ
		int row = 0;		//��һ�з����˱仯
		XBool state = XFalse;	//��������ô���ı仯
		int index = 0;
		for(int i = 0;i < min(pPar.m_canShowLineSum,pPar.m_tableLineSum - pPar.m_showStartLine);++ i)
		{
			if(pPar.m_check0State[i + pPar.m_showStartLine] != pPar.m_check0[i].getState()) 
			{
				state = pPar.m_check0[i].getState();
				row = 0;
				index = i + pPar.m_showStartLine;
			}
			if(pPar.m_check1State[i + pPar.m_showStartLine] != pPar.m_check1[i].getState())
			{
				state = pPar.m_check1[i].getState();
				row = 1;
				index = i + pPar.m_showStartLine;
			}

			pPar.m_check0State[i + pPar.m_showStartLine] = pPar.m_check0[i].getState();
			pPar.m_check1State[i + pPar.m_showStartLine] = pPar.m_check1[i].getState();
		}
		if(pPar.m_haveSelectFlag[index])	//��һ�б��봦��ѡ��״̬
		{
			for(int i = 0;i < pPar.m_tableLineSum;++ i)
			{
				if(pPar.m_haveSelectFlag[i])
				{//��ѡ�е�״̬ͳһ�����仯
					if(row == 0 && pPar.m_check0State[i] != state)
					{
						pPar.m_check0State[i] = state;
						if(i >= pPar.m_showStartLine && i < pPar.m_showStartLine + pPar.m_canShowLineSum)
						{
							pPar.m_check0[i - pPar.m_showStartLine].setState(state);
						}
					}else
					if(row == 1 && pPar.m_check1State[i] != state)
					{
						pPar.m_check1State[i] = state;
						if(i >= pPar.m_showStartLine && i < pPar.m_showStartLine + pPar.m_canShowLineSum)
						{
							pPar.m_check1[i - pPar.m_showStartLine].setState(state);
						}
					}
				}
			}
		}
		if(pPar.m_eventProc != NULL) pPar.m_eventProc(pPar.m_pClass,pPar.m_objectID,MLTLSTB_CHK_STATE_CHANGE);
		else XCtrlManager.eventProc(pPar.m_objectID,MLTLSTB_CHK_STATE_CHANGE);
		return;
	}
}
XMultiListBasic::XMultiListBasic()
:m_isInited(XFalse)					//�Ƿ��ʼ��
,m_mutiListNormal(NULL)			//�����б����ͨ״̬
,m_mutiListDisable(NULL)		//�����б����Ч״̬
,m_mutiListSelect(NULL)			//�����б��б�ѡ���еı���
,m_mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
,m_mutiListTitle(NULL)			//�����б�ı��ⱳ��
,m_mutiListTitleEnd(NULL)		//�����б�ı���ָ���
,m_needShowVSlider(XFalse)			//�Ƿ���Ҫ��ʾ��ֱ������
,m_needShowHSlider(XFalse)			//�Ƿ���Ҫ��ʾˮƽ������
,m_tableRowSum(0)				//����е�����
,m_tableRow(NULL)				//�е�����
,m_tableLineSum(0)				//����е�����
,m_tableBox(NULL)				//����Ԫ�ص�����
,m_mouseLeftButtonDown(XFalse)
,m_mouseMoveDown(XFalse)
,m_haveSelectFlag(NULL)
,m_check0(NULL)
,m_check1(NULL)
,m_check0State(NULL)
,m_check1State(NULL)
//,m_funSelectChange(NULL)
//,m_funCheckStateChange(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_MUTILIST;
}
void XMultiListBasic::releaseTempMemory()
{
	XMem::XDELETE_ARRAY(m_check0);
	XMem::XDELETE_ARRAY(m_check1);
	XMem::XDELETE_ARRAY(m_check0State);
	XMem::XDELETE_ARRAY(m_check1State);
	if(m_tableRow != NULL)
	{
		//ɾ�����е�����Ϣ
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp;
		for(int i = 0;i < m_tableRowSum;++ i)
		{
			temp = tempRow;
			tempRow = tempRow->nextRow;
			//XMem::XDELETE_ARRAY(temp->title);
			XMem::XDELETE(temp);
		}
		m_tableRow = NULL;
	}
	if(m_tableBox != NULL)
	{
		//ɾ�����б���е�Ԫ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1;
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			for(int j = 0;j < m_tableRowSum;++ j)
			{
				temp1 = tempBox;
				tempBox = tempBox->nextBox;
				//XMem::XDELETE_ARRAY(temp1->string);
				XMem::XDELETE(temp1);
			}
		}
		m_tableBox = NULL;
	}
	m_tableRowSum = 0;
	m_tableLineSum = 0;
	XMem::XDELETE_ARRAY(m_haveSelectFlag);
}
void XMultiListBasic::release()
{
	if(!m_isInited) return;
	releaseTempMemory();

	XCtrlManager.decreaseAObject(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
XBool XMultiListBasic::init(const XVector2& position,		//�ռ����ڵ�λ��
		const XRect& Area,					//�ؼ���ʵ����ʾ����
		const XMultiListSkin &tex,		//�ؼ�����ͼ
		const XCheckSkin &checktex0,		//��ѡ�����ͼ
		const XCheckSkin &checktex1,		//��ѡ�����ͼ
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	//��ֱ������
		const XSlider &hSlider)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//����Ϊ����
		return XFalse;
	}
	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = Area;

	m_mutiListNormal = tex.mutiListNormal;			//�����б����ͨ״̬
	m_mutiListDisable = tex.mutiListDisable;			//�����б����Ч״̬
	m_mutiListSelect = tex.mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex.mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex.mutiListTitle;			//�����б�ı��ⱳ��
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//�����б�ı���ָ���
	m_withoutTex = XFalse;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(vSlider);		//��ֱ������
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(!m_check0[i].init(XVector2(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex0,
			" ",m_caption,1.0f,XVector2::zero)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
		if(!m_check1[i].init(XVector2(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex1,
			" ",m_caption,1.0f,XVector2::zero)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::initPlus(const char *path,		//��ѡ�����ͼ
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		XResourcePosition resoursePosition)
{
	if(m_isInited || path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXMULTILIST_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	XMultiListSkin * tex = (XMultiListSkin *)m_resInfo->m_pointer;
	//�����ǲ������
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex->mutiListDisable == NULL || tex->mutiListMove == NULL || tex->mutiListNormal == NULL
		|| tex->mutiListSelect == NULL || tex->mutiListTitle == NULL || tex->mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;//����Ϊ����
	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;

	m_mutiListNormal = tex->mutiListNormal;			//�����б����ͨ״̬
	m_mutiListDisable = tex->mutiListDisable;			//�����б����Ч״̬
	m_mutiListSelect = tex->mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex->mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex->mutiListTitle;			//�����б�ı��ⱳ��
	m_mutiListTitleEnd = tex->mutiListTitleEnd;		//�����б�ı���ָ���
	m_withoutTex = XFalse;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	//m_verticalSlider.setACopy(vSlider);		//��ֱ������
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	//m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	char tempPath0[MAX_FILE_NAME_LENGTH];
	char tempPath1[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath0,"%s/checkC",path);
	sprintf(tempPath1,"%s/checkE",path);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
	//	if(!m_check0[i].init(XVector2(m_position.x + 4.0f * m_scale.x,m_position.y + (36.0f * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex0,
	//		" ",m_caption,1.0f,XVector2::zero)) 
		if(!m_check0[i].initPlus(tempPath0," ",m_caption,1.0f,resoursePosition)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
	//	if(!m_check1[i].init(XVector2(m_position.x + 4.0f * m_scale.x + 32.0f * m_scale.x,m_position.y + (36.0f * m_scale.y + m_curTextHeight * i)),XRect(4.0f,4.0f,28.0f,28.0f),checktex1,
	//		" ",m_caption,1.0f,XVector2::zero)) 
		if(!m_check1[i].initPlus(tempPath1," ",m_caption,1.0f,resoursePosition)) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.left + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::initWithoutSkin(const XRect& area,
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum)
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;//����Ϊ����
	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;

	m_scale.set(1.0f,1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVector2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVector2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH));
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}		
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL)
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	if(m_tableLineSum > 0)
	{
		m_haveSelectFlag = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check0State = XMem::createArrayMem<XBool>(m_tableLineSum);
		m_check1State = XMem::createArrayMem<XBool>(m_tableLineSum);
		if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = XFalse;
			m_check0State[i] = XFalse;
			m_check1State[i] = XFalse;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	m_stateOfShiftKey = KEY_STATE_UP;
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
//	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);	//��ǰ�������Ӧ��Χ
//	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	m_check0 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	m_check1 = XMem::createArrayMem<XCheck>(m_canShowLineSum);
	if(m_check0 == NULL || m_check1 == NULL) 
	{
		releaseTempMemory();
		return XFalse;
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(!m_check0[i].initWithoutSkin(" ",m_caption,1.0f,XRect(0,0,DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE),XVector2(DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE))) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((DEFAULT_TITLE_HEIGHT + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		m_check0[i].setScale(m_scale);
		m_check0[i].setWithCaption(XFalse);
		if(!m_check1[i].initWithoutSkin(" ",m_caption,1.0f,XRect(0,0,DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE),XVector2(DEFAULT_CHECK_SIZE,DEFAULT_CHECK_SIZE))) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((DEFAULT_TITLE_HEIGHT + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setWithCaption(XFalse);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
		XCtrlManager.decreaseAObject(&(m_check0[i]));
		XCtrlManager.decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_check0[i]);
		XObjManager.decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setEventProc(ctrlProc,this);
		m_check1[i].setEventProc(ctrlProc,this);
	}
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) 
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::updateSliderState()				//���ݱ�����������»�������״̬
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	//��ֱ������
	if(m_canShowLineSum < m_tableLineSum)
	{//�����ֱ������ʾ��Χ������Ҫ������ֱ��ʾ����������ѡ��Χ
		m_needShowVSlider = XTrue;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}else
	{
		m_showStartLine = 0;
		m_needShowVSlider = XFalse;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}
	//ˮƽ������
	if(m_tableRowSum <= 0)
	{//���û��������Ҫ��ʾˮƽ������
		m_needShowHSlider = XFalse;
	}else
	{
		int width = 0;
		float rowWidth[MUTILIST_MAX_ROW_SUM];
		memset(rowWidth,0,MUTILIST_MAX_ROW_SUM * sizeof(float));
		//����1:ֻҪ����1������ʾˮƽ������
	/*	if(m_tableRowSum > 1)
		{
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}*/
	/*	//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL)
			{
				break;
			}else
			{
				tempRow = tempRow->nextRow;
			}
			++ i
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - i,0);
		//	m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}else
		{
			if(m_needShowHSlider)
			{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
				m_rowDx = 0;
			}
			m_needShowHSlider = 0;
			m_showStartRow = 0;
			m_horizontalSlider.setCurValue(0);
		}*/
		//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
			++ i;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum && flag == 0)
			{
				flag = i;
				//break;
			}
		}
		if(flag != 0)
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
			m_needShowHSlider = XTrue;
			//m_horizontalSlider.setRange(m_tableRowSum - i,0);
			m_horizontalSlider.setRange(width - m_showPixWidth + MUTILIST_TITLE_EXPAND_LENGTH * m_scale.x,0);	//ע�������56��������չ��
		}else
		{
		//	if(m_needShowHSlider)
		//	{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
		//		m_rowDx = 0;
		//	}
			m_needShowHSlider = XFalse;
			m_showStartRow = 0.0f;
			m_horizontalSlider.setCurValue(0.0f);
		}
	}
}
void XMultiListBasic::clearAllSelect()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		m_haveSelectFlag[i] = XFalse;
	}
	//m_haveSelectSum = 0;
	m_haveSelect = XFalse;	//���ѡ��ı��
	m_selectLineOrder = 0;
}
void XMultiListBasic::setTitleStr(const char *str,int order)
{
	if(str == NULL) return;
	if(order < 0 || order >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ������
	XMultiListOneRow *tempRow;
	if(order == 0)
	{
		tempRow = m_tableRow;
	}else
	{
		tempRow = m_tableRow;
		for(int i = 1;i <= order;++ i)
		{
			tempRow = tempRow->nextRow;
		}
	}
	//�޸�ָ���е��ַ���
//	char *tempTitle = XMem::createArrayMem<char>(strlen(str) + 1);
//	if(tempTitle == NULL) return;
//	strcpy(tempTitle,str);
//	XMem::XDELETE_ARRAY(tempRow->title);
//	tempRow->title = tempTitle;
	tempRow->title = str;
	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
				tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		//strcpy(tempRow->title,str);
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
}
void XMultiListBasic::setBoxStr(const char *str,int line,int row)	//����ĳһ����Ԫ�������
{
	//��������������ж�
	if(str == NULL) return;
	if(line < 0 || line >= m_tableLineSum) return;
	if(row < 0 || row >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ���ı��
	XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	//�����ڴ�ռ�
	//char * tempStr = XMem::createArrayMem<char>(strlen(str) + 1);
	//if(tempStr == NULL) return;
	//strcpy(tempStr,str);//��ֵ
	//XMem::XDELETE_ARRAY(tempBox->string);
	//tempBox->string = tempStr;
	tempBox->textStr = str;
	//������ʾ�ַ�
	if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
			{
				tempBox->textStr[tempRow->stringShowWidth] = '\0';
				tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempBox->textStr[tempRow->stringShowWidth] = '\0';
				tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
				tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempBox->textStr[tempRow->stringShowWidth] = '\0';
		}
		tempBox->text.setString(tempBox->textStr.c_str());
		//strcpy(tempBox->string,str);
		tempBox->textStr = str;
	}else
	{
		tempBox->text.setString(tempBox->textStr.c_str());
	}
}
void XMultiListBasic::updateShowChange()				//�����б��ı仯��Ϣ�����б���еı�����ʾ���
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(m_tableRowSum <= 0) return;	//���û�б��ֱ�ӷ���
	//�������ʾ
	XMultiListOneRow *tempRow = m_tableRow;
	float curWidthPix = 0.0f;
	int row = 0;
	int showMaxRow = 0;
//	int showDx = m_showStartRow; //��ʾ�����ݱ���ˮƽƫ��	m_showStartRow(ˮƽƫ����)
	while(true)
	{
		if(curWidthPix + tempRow->pixSize.x * m_scale.x > m_showStartRow && curWidthPix - m_showStartRow <= m_showPixWidth)
		{
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth)
			{//��ʾ�ұ߲���
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//��ߵ��и�λ��
				tempRow->right = tempRow->pixSize.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT < 
						m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / 
						m_mutiListTitle->textureSize.y < m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix)) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth && curWidthPix >= m_showStartRow)
			{//����������ʾ
				tempRow->isShow = 1;
				curWidthPix += tempRow->pixSize.x * m_scale.x;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x;
				}else
				{
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.disClip();
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = 0.0f;					//�ұߵ��и�λ��
			}else
			if(curWidthPix >= m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//��ʾ����һ����
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 2;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(0.0f,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = m_showPixWidth - (curWidthPix - m_showStartRow);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth - (curWidthPix - m_showStartRow)) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//������Ҫ��ͷȥβ��Ŀǰû�д���
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 2;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = m_showPixWidth;
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth) / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			{
				printf("haha\n");
			}
		}else
		{
			curWidthPix += tempRow->pixSize.x * m_scale.x;
			tempRow->isShow = 0;
		}
	//	if(row < m_showStartRow)
	//	{//����ˮƽƫ��
	//		showDx += tempRow->pixSize.x * m_scale.x;
	//	}
		if(tempRow->nextRow == NULL) break;
		else
		{
			++ row;
			tempRow = tempRow->nextRow;
		}
	}
	//�����޸����״̬������ʾ
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisible();
			m_check1[i].disVisible();
		}else
		{
			m_check0[i].setVisible();
			m_check1[i].setVisible();
		}
	}
	//�����б������ȫ������
	if(m_tableLineSum <= 0) return;	//���û����ֱ�ӷ���
	XMultiListOneBox *tempBox = m_tableBox;
	tempRow = m_tableRow;
	row = 0;
	int line = 0;
	while(true)
	{
		if(line >= m_showStartLine && line < m_showStartLine + m_canShowLineSum)
		{
			//if(row >= m_showStartRow && row <= showMaxRow)
			if(tempRow->isShow != 0)
			{
				tempBox->isShow = XTrue;
				tempBox->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y + m_curTextHeight * (line - m_showStartLine + 1));
				if(tempRow->needChip != 0) tempBox->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempBox->text.getTextSize().y);
				else tempBox->text.disClip();
			}else
			{
				tempBox->isShow = XFalse;
			}
		}else
		{
			tempBox->isShow = XFalse;
		}
		if(tempBox->nextBox == NULL) break;
		else tempBox = tempBox->nextBox;
		++row;
		if(row >= m_tableRowSum)
		{
			row = 0;
			++ line;
			tempRow = m_tableRow;
		}else
		{
			assert(tempRow != NULL);
			tempRow = tempRow->nextRow;
		}
	}
	//��������б���ѡ��״̬����Ϣ
	for(int i = m_showStartLine;i < min(m_showStartLine + m_canShowLineSum,m_tableLineSum);++ i)
	{
		if(m_check0State[i]) m_check0[i - m_showStartLine].setState(XTrue);
		else m_check0[i - m_showStartLine].setState(XFalse);
		if(m_check1State[i]) m_check1[i - m_showStartLine].setState(XTrue);
		else m_check1[i - m_showStartLine].setState(XFalse);
	}
}
//ע�����ﴫ��ĳߴ�Ϊ����֮ǰ�ĳߴ磬Ҳ�����ǿռ����ű���Ϊ1��ʱ��óߴ�
XBool XMultiListBasic::setRowWidth(int temp,int order)	//��������һ�еĿ��
{
	if(temp < MUTILIST_MIN_WIDTH || temp > m_showPixWidth / m_scale.x) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	//�ҵ���Ӧ��һ��
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < order;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	
	tempRow->pixSize.set(temp,tempRow->pixSize.y);	//�ı䵱ǰ�е��п�
	tempRow->stringShowWidth = tempRow->pixSize.x * m_scale.x / m_curTextWidth;	//�ı䵱ǰ�е���ʾ�ַ����
	if(!m_withoutTex)
	{
		tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x + tempRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
	}
	tempRow->text.setPosition(tempRow->position.x,tempRow->position.y);
	//�ı䵱ǰ�е���ʾ�ַ�
	//char * str = XMem::createArrayMem<char>(strlen(tempRow->title) + 1);
	//if(str == NULL) return XFalse;
	//strcpy(str,tempRow->title);
	std::string str = tempRow->title;
	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			//����û�п��Ǻ��ӵ����
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				tempRow->title[tempRow->stringShowWidth] = '\0';
				tempRow->title[tempRow->stringShowWidth - 1] = '.';
				tempRow->title[tempRow->stringShowWidth - 2] = '.';
				tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		//strcpy(tempRow->title,str);
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
	//XMem::XDELETE_ARRAY(str);
	//�޸ĺ���������е�λ��
	while(true)
	{
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow = tempRow->nextRow;
		}
	}
	//�ı����е�Ԫ������
	int i,j;
	std::string tmpStr;
	XMultiListOneBox *tempBox = m_tableBox;
	for(i = 0;i < m_tableLineSum;++ i)
	{
		tempRow = m_tableRow;
		for(j = 0;j < m_tableRowSum;++ j)
		{
			if(j == order)
			{//�ı��ַ���
				//tmpStr = XMem::createArrayMem<char>(strlen(tempBox->string) + 1);
				//if(tmpStr == NULL) return XFalse;
				//strcpy(tmpStr,tempBox->string);
				tmpStr = tempBox->textStr;
				if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
				{//���������ʾ����
					if(tempRow->stringShowWidth >= 2)
					{//����3���ַ�������ʾ...������ʾ..
						if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
						{
							tempBox->textStr[tempRow->stringShowWidth] = '\0';
							tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
						}else
						{
							tempBox->textStr[tempRow->stringShowWidth] = '\0';
							tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
							tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
						}
					}else
					{
						tempBox->textStr[tempRow->stringShowWidth] = '\0';
					}
					tempBox->text.setString(tempBox->textStr.c_str());
					//strcpy(tempBox->string,tmpStr);
					tempBox->textStr = tmpStr;
				}else
				{
					tempBox->text.setString(tempBox->textStr.c_str());
				}
				//XMem::XDELETE_ARRAY(tmpStr);
			}
			if(j > order)
			{//�ı�λ��
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
			}

			tempRow = tempRow->nextRow;
			tempBox = tempBox->nextBox;
		}
	}
	//������ʾ����
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::draw()	//��溯��
{
	if(!m_isInited ||
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxA(m_position + XVector2(0.0f,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::mouseColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::specialColor * m_color,true);
		}else
		{
			XRender::drawFillBoxA(m_position + XVector2(0.0f,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.getHeight() * m_scale.y),
				XCCS::downColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::normalColor * m_color,true);
		}
		//������ʾ�б������
		//��ʾ����
		if(m_tableRowSum > 0)
		{
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					XRender::drawFillBoxExA(XVector2(tempRow->posX,tempRow->position.y),XVector2(tempRow->pixLen,DEFAULT_TITLE_HEIGHT * m_scale.y),
						XCCS::blackOnColor * m_color);
		//			tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow != 0 && tempRow->needChip != 2)
					{
						int x = tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							DEFAULT_END_WIDTH * tempRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
						//if(x >= m_curMouseRect.left)
						{
							XRender::drawLine(x,m_tableRow->position.y,x,m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y,1,
								XCCS::mouseColor * m_color);
							//����зֽ���
							XRender::drawLine(x,m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y,x,
								m_tableRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y,1,
								XCCS::blackNormalColor * m_color);
							//tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
						}
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				//������ʾѡ��Ľ��
				for(int i = 0;i < min(m_canShowLineSum,m_tableLineSum - m_showStartLine);++ i)
				{
					if(m_haveSelectFlag[m_showStartLine + i])
					{//��ʾѡ����
				//		m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
				//			m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
				//			i * m_curTextHeight);

				//		m_spriteSelect.draw(m_mutiListSelect);
						XRender::drawFillBoxExA(XVector2(m_position.x + m_mouseRect.left * m_scale.x,
							m_position.y + (m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + i * m_curTextHeight),
							XVector2(m_showPixWidth,m_curTextHeight),XCCS::onColor * m_color);
					}
				}
				if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawRect(m_selectRect,1.0f,XCCS::darkColor * m_color);
				}
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) 
		{
			int x = m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
								DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
			XRender::drawLine(x,m_changeRow->position.y,x,m_changeRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y,1,
				XCCS::blackDownColor * m_color);
			//m_spriteMove.draw(m_mutiListMove);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_mutiListDisable);
		else m_spriteBackGround.draw(m_mutiListNormal);
		//������ʾ�б������
		//��ʾ����
		if(m_tableRowSum > 0)
		{
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow != 0 && tempRow->needChip != 2)
					{
						tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				//������ʾѡ��Ľ��
				for(int i = 0;i < min(m_canShowLineSum,m_tableLineSum - m_showStartLine);++ i)
				{
					if(m_haveSelectFlag[m_showStartLine + i])
					{//��ʾѡ����
						m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
							m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
							i * m_curTextHeight);

						m_spriteSelect.draw(m_mutiListSelect);
					}
				}
				if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawRect(m_selectRect,1.0f,XCCS::darkColor * m_color);
				}
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) m_spriteMove.draw(m_mutiListMove);
	}
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_mouseRect.top * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::downColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),XCCS::downColor * m_color,true);
		}else
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_mouseRect.top * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XCCS::lightSpecialColor * m_color,true);
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::lightSpecialColor * m_color,true);
		}
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::blackDownColor * m_color,true);
		}else
		{
			XRender::drawFillBoxExA(m_position + XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
				XVector2(m_verticalSlider.getMouseRectWidth() * m_scale.x,
				m_horizontalSlider.getMouseRectHeight() * m_scale.y),
				XCCS::lightMouseColor * m_color,true);
		}
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].draw();
		m_check1[i].draw();
	}
}
XBool XMultiListBasic::mouseProc(float x,float y,XMouseState mouseState)					//������궯������Ӧ����
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].mouseProc(x,y,mouseState);
		m_check1[i].mouseProc(x,y,mouseState);
	}
	if(m_needShowVSlider)
	{
		m_verticalSlider.mouseProc(x,y,mouseState);
		if(isInRect(x,y))
		{//������Ӧ�����¼�
			switch(mouseState)
			{
			case MOUSE_WHEEL_UP_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() - 1.0f);
				break;
			case MOUSE_WHEEL_DOWN_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() + 1.0f);
				break;
			}
		}
	}
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(x,y,mouseState);
	//�����ж�����޸ı���������ж�
	if(m_tableRowSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//�������
			XRect tempRect;
			XMultiListOneRow *tempRow = m_tableRow;
			int order = 0;
		//	int showDx = m_showStartRow;	//�����Ҫ����������ڿ�ʼ��ʾ����һ�е�ƫ��������Ϊ�е�λ�����ȶ��ڵ�һ�еľ���λ�ã�������Ҫ����ʵ�ʵ���ʾ������Ե���ʾλ��
			while(true)
			{
				if(tempRow->isShow != 0)
				{//��ʾ�����Ĳ����޸�
					if(m_withoutTex)
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}else
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}
					if(tempRect.isInRect(x,y))
					{//����϶���Ч
						m_mouseLeftButtonDown = XTrue;
						m_startX = x;
					//	m_rowDx = m_showStartRow;
						m_changeRowOrder = order;
						m_changeRow = tempRow;
						//���ÿ�ʼ��ʾ�ƶ�ͼ��
						m_needShowMove = XTrue;
						if(!m_withoutTex)
						{
							m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
								m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
								m_changeRow->position.y);
						}
						break;
					}
				}
			//	if(order < m_showStartRow)
			//	{
			///		showDx += tempRow->pixSize.x * m_scale.x;
			//	}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
				++ order;
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseLeftButtonDown)
		{//�϶���Ч
			//�����϶��Ľ��
			int dx = x - m_startX;
			m_startX = x;
			if(m_changeRow->position.x - m_showStartRow - m_tableRow->position.x + 
				m_changeRow->pixSize.x * m_scale.x + dx > m_showPixWidth ||
				x < m_curMouseRect.left)
			{//����ƶ�������ȡ���ƶ���������չ�����
				setRowWidth((m_showPixWidth - m_changeRow->position.x + m_showStartRow + m_tableRow->position.x) / m_scale.x,m_changeRowOrder);
				m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = XFalse;
			}else
			if(m_changeRow->pixSize.x *m_scale.x + dx < MUTILIST_MIN_WIDTH * m_scale.x)
			{//�����Ԫ����С����Сֵ����ʹ����Сֵ����ȡ���ƶ�
				setRowWidth(MUTILIST_MIN_WIDTH,m_changeRowOrder);
				m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = XFalse;
			}else
			{
				if(!setRowWidth(m_changeRow->pixSize.x + dx / m_scale.x,m_changeRowOrder))
				{//�϶�ʧ����ȥ������
					m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
					m_mouseLeftButtonDown = XFalse;
				}
				if(!m_withoutTex)
				{
					m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_changeRow->position.y);
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP)
	{//���̸����ȥ������
		if(m_mouseLeftButtonDown)
		{
			m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
			m_mouseLeftButtonDown = XFalse;
		}
		if(isInRect(x,y)) m_isBeChoose = XTrue;
	}
	//�����ж���ѡ
	if(m_tableLineSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//�������
			//�����ѡ��Χ
			XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y + m_showPixHight);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y + m_showPixHight);
			}
			if(tempRect.isInRect(x,y))
			{//�����ѡ������һ��
				int DLine = (y - tempRect.top) / m_curTextHeight;
				if(DLine + m_showStartLine < m_tableLineSum)
				{//��ѡ��Ч
					if(m_haveSelect && m_selectLineOrder == DLine + m_showStartLine)
					{//�ظ�ѡ���ʾȡ��ѡ��
						m_haveSelect = XFalse;
						m_selectLineOrder = DLine + m_showStartLine;
						if(m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = XFalse;
							if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTLST_SELECT);
							else XCtrlManager.eventProc(m_objectID,MLTLST_SELECT);;
						}
					}else
					{
						//������Ƭѡ�жϲ�����
						if(m_stateOfShiftKey == KEY_STATE_DOWN && m_haveSelect)
						{//Ƭѡ
							if(m_selectLineOrder < DLine + m_showStartLine)
							{
								for(int i = m_selectLineOrder;i < DLine + m_showStartLine;++ i)
								{
									m_haveSelectFlag[i] = XTrue;
								}
							}else
							{
								for(int i = DLine + m_showStartLine;i < m_selectLineOrder;++ i)
								{
									m_haveSelectFlag[i] = XTrue;
								}
							}
						}
						m_haveSelect = XTrue;
						m_selectLineOrder = DLine + m_showStartLine;
						if(!m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = XTrue;
							if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTLST_SELECT);
							else XCtrlManager.eventProc(m_objectID,MLTLST_SELECT);;
						}
					}

					updateSelectLine();
					//�����갴����Ч
					m_mouseMoveDown = XTrue;
					m_oldLineOrder = m_selectLineOrder;
				}
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseMoveDown)
		{//�϶���Ч
			//�����ѡ��Χ(ע��������չ��20��Ϊ��ʵ���϶�������Χ�Ĵ���)
			XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y - 100,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y + m_showPixHight + 100);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y - 100,
					m_position.x + m_mouseRect.left * m_scale.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y + m_showPixHight + 100);
			}
			if(tempRect.isInRect(x,y))
			{
				int DLine = (y - tempRect.top - 100) / m_curTextHeight;
				if(DLine < 0 && m_showStartLine > 0)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if(moveUpLine(m_oldLineOrder))
					{
						-- m_oldLineOrder;
						-- m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}else
				if(DLine >= m_canShowLineSum && m_showStartLine < m_tableLineSum - m_canShowLineSum)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if(moveDownLine(m_oldLineOrder))
					{
						++ m_oldLineOrder;
						++ m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}else
				if(DLine + m_showStartLine < m_oldLineOrder)
				{//�����ƶ�
					if(moveUpLine(m_oldLineOrder))
					{
						-- m_oldLineOrder;
					}
				}else
				if(DLine + m_showStartLine > m_oldLineOrder)
				{//�����ƶ�
					if(moveDownLine(m_oldLineOrder))
					{
						++ m_oldLineOrder;
					}
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_mouseMoveDown)
	{
		m_mouseMoveDown = XFalse;
		m_oldLineOrder = m_selectLineOrder;
	}
	return XTrue;
}
void XMultiListBasic::updateSelectLine()
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return;
	if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//����ѡ���λ��
		XVector2 temp;
		if(m_withoutTex)
		{
			temp.set(m_position.x + m_mouseRect.left * m_scale.x,
				m_position.y + (m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + 
				(m_selectLineOrder - m_showStartLine) * m_curTextHeight);
		}else
		{
			temp.set(m_position.x + m_mouseRect.left * m_scale.x,
				m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_scale.y + 
				(m_selectLineOrder - m_showStartLine) * m_curTextHeight);
		}
		m_selectRect.set(temp.x,temp.y,temp.x + m_showPixWidth,temp.y + m_curTextHeight);
	}
}
//�ļ���������ʽΪXMultiList_ID.dat
XBool XMultiListBasic::exportData(const char *fileName)	//���ݵ���
{//���ݵ������ļ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiListBasic_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//д������Ϣ
	XMultiListOneRow *tempRow = m_tableRow;
	int strLength = 0;
	float tempWidth = 0;
	fwrite(&m_tableRowSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		tempWidth = tempRow->pixSize.x;
		fwrite(&(tempWidth),sizeof(float),1,fp);
		strLength = strlen(tempRow->title.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempRow->title.c_str(),strLength + 1,1,fp);
		tempRow = tempRow->nextRow;
	}
	//д������Ϣ
	XMultiListOneBox *tempBox = m_tableBox;
	fwrite(&m_tableLineSum,sizeof(int),1,fp);
	//ע��������Ҫд��check��״̬
	char tmp = 0;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check0State[i]) tmp = 0;
		else tmp = 1;
		fwrite(&(tmp),sizeof(char),1,fp);
		if(m_check1State[i]) tmp = 0;
		else tmp = 1;
		fwrite(&(tmp),sizeof(char),1,fp);
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		strLength = strlen(tempBox->textStr.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempBox->textStr.c_str(),strLength + 1,1,fp);
		tempBox = tempBox->nextBox;
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiListBasic::importData(const char *fileName)	//���ݵ���
{//���ļ��н����ݵ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiListBasic_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//��ȡ����Ϣ
	int strLength = 0;
	int tempSum = 0;
	float tempWidth = 0;
	char *str;
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableRowSum)
	{//��񲻷���
		setRowSum(tempSum);
	}
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		fread(&tempWidth,sizeof(float),1,fp);
		setRowWidth(tempWidth,i);
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;

		fread(str,strLength + 1,1,fp);
		setTitleStr(str,i);
		XMem::XDELETE_ARRAY(str);
	}
	//��ȡ�����Ϣ
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableLineSum)
	{//��񲻷���
		setLineSum(tempSum);
	}
	//ע��������Ҫ��ȡcheck��״̬
	char tmp = 0;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		fread(&(tmp),sizeof(char),1,fp);
		if(tmp == 0) m_check0State[i] = XFalse;
		else m_check0State[i] = XTrue;
		fread(&(tmp),sizeof(char),1,fp);
		if(tmp == 0) m_check1State[i] = XFalse;
		else m_check1State[i] = XTrue;
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;

		fread(str,strLength + 1,1,fp);
		setBoxStr(str,i / m_tableRowSum,i % m_tableRowSum);
		XMem::XDELETE_ARRAY(str);
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiListBasic::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	TiXmlElement elmNode(m_ctrlName.c_str());
	XBool flag = XTrue;
	//д�����
	XMultiListOneRow *tempRow = m_tableRow;
	if(!XXml::addLeafNode(elmNode,"RowSum",XString::toString(m_tableRowSum))) flag = XFalse;
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Row",tempRow->title)) flag = XFalse;
		tempRow = tempRow->nextRow;
	}
	XMultiListOneBox *tempBox = m_tableBox;
	if(!XXml::addLeafNode(elmNode,"LineSum",XString::toString(m_tableLineSum))) flag = XFalse;
	//ע��������Ҫ��ȡcheck��״̬(��δ���)
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check0State[i]) 
		{
			if(!XXml::addLeafNode(elmNode,"State0",true)) flag = XFalse;
		}else
		{
			if(!XXml::addLeafNode(elmNode,"State0",false)) flag = XFalse;
		}
	}
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		if(m_check1State[i]) 
		{
			if(!XXml::addLeafNode(elmNode,"State1",true)) flag = XFalse;
		}else
		{
			if(!XXml::addLeafNode(elmNode,"State1",false)) flag = XFalse;
		}
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Box",tempBox->textStr)) flag = XFalse;
		tempBox = tempBox->nextBox;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = XFalse;
	return flag;
}
XBool XMultiListBasic::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if(keyNode == NULL) return XFalse;
	//�����ȡ
	XBool flag = XTrue;
	int tmpI = 0;
	std::string tmpStr = "";
	if(!XXml::getXmlAsInt(keyNode,"RowSum",tmpI)) return XFalse;
	if(tmpI != m_tableRowSum) setRowSum(tmpI);//��񲻷���
	int index = 0;
	TiXmlNode *tmpNode = XXml::getXmlAsString(keyNode,"Row",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setTitleStr(tmpStr.c_str(),index);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Row",tmpStr);
	}

	if(!XXml::getXmlAsInt(keyNode,"LineSum",tmpI)) return XFalse;
	if(tmpI != m_tableLineSum) setLineSum(tmpI);//��񲻷���
	//ע��������Ҫ��ȡcheck��״̬
	bool tmpB = true;
	index = 0;
	tmpNode = XXml::getXmlAsBool(keyNode,"State0",tmpB);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			m_check0State[index] = tmpB;
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsBool(tmpNode,"State0",tmpB);
	}
	index = 0;
	tmpNode = XXml::getXmlAsBool(keyNode,"State1",tmpB);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			m_check1State[index] = tmpB;
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsBool(tmpNode,"State1",tmpB);
	}
	index = 0;
	tmpNode = XXml::getXmlAsString(keyNode,"Box",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setBoxStr(tmpStr.c_str(),index / m_tableRowSum,index % m_tableRowSum);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Box",tmpStr);
	}
	return flag;
}
void XMultiListBasic::initANewRowData(XMultiListOneRow * upRow,int i)
{
	if(i == 0)
	{//��һ��Ԫ��
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;
	}else
	{
		upRow->nextRow->isEnable = XTrue;
		upRow->nextRow->order = i;
		upRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(upRow->nextRow->text));
#endif
		upRow->nextRow->text.setScale(m_scale * m_fontSize);
		upRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		upRow->nextRow->position.set(upRow->position.x + upRow->pixSize.x * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		upRow->nextRow->text.setPosition(upRow->nextRow->position.x,upRow->nextRow->position.y);
		if(!m_withoutTex)
		{
			upRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(upRow->nextRow->m_spriteTitle));
#endif
			upRow->nextRow->m_spriteTitle.setPosition(upRow->nextRow->position);
			upRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			upRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			upRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(upRow->nextRow->m_spriteTitleEnd));
#endif
			upRow->nextRow->m_spriteTitleEnd.setPosition(upRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			upRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			upRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		upRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		upRow->nextRow->title = "";
		setTitleStr("Title",i);	//����Ĭ����ʾ
		upRow->nextRow->isShow = 1;
	}
}
XBool XMultiListBasic::setRowSum(int rowSum)		//��������������������հף�����ɾ��
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		rowSum < 0 ||			//�Ƿ�����ֱ�ӷ�Χʧ��
		rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;	//�����е����ֵ�����˳�
	if(rowSum == m_tableRowSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	//�������ʵ������޸Ŀؼ��е�����
	if(rowSum > m_tableRowSum)
	{//��ӣ���������ӵĻ��������;��Ϊ�����˳�����Ӱ�쵽�߼���������
		int i,j;
		int oldRowSum = m_tableRowSum;
		m_tableRowSum = rowSum;
		//��ӱ�������
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
			m_tableRow = XMem::createMem<XMultiListOneRow>();
			if(m_tableRow == NULL) return XFalse;

			initANewRowData(NULL,0);

			tempRow = m_tableRow;
			for(i = 1;i < m_tableRowSum;++ i)
			{
				tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
				if(tempRow->nextRow == NULL) return XFalse;

				initANewRowData(tempRow,i);

				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
			tempRow->nextRow = NULL;	//���һ����β

		}else
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= oldRowSum)
				{//�����µ���
					assert(temp != NULL);
					temp->nextRow = XMem::createMem<XMultiListOneRow>();
					if(temp->nextRow == NULL) return XFalse;

					initANewRowData(temp,i);

					temp = temp->nextRow;
					temp->nextRow = tempRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}
		//��ӱ������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}else
		{
			if(m_tableLineSum > 0)
			{
				for(i = 0;i < m_tableLineSum;++ i)
				{
					tempRow = m_tableRow;
					for(j = 0;j < m_tableRowSum;++ j)
					{
						if(j >= oldRowSum)
						{
							assert(temp1 != NULL);
							temp1->nextBox = XMem::createMem<XMultiListOneBox>();
							if(temp1->nextBox == NULL) return XFalse;

							temp1 = temp1->nextBox;
							temp1->isEnable = XTrue;

							temp1->isShow = XTrue;
							temp1->order.set(j,i);
							temp1->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
							XObjManager.decreaseAObject(&(temp1->text));
#endif
							temp1->text.setScale(m_scale * m_fontSize);
							temp1->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							temp1->textStr = "";
							setBoxStr("Line",i,j);

							temp1->nextBox = tempBox;
							tempRow = tempRow->nextRow;
						}else
						{
							temp1 = tempBox;
							tempBox = tempBox->nextBox;
							tempRow = tempRow->nextRow;
						}
					}
				}
			}
		}
	}else
	{//ɾ��
		int i,j;
		//ɾ����������
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= rowSum)
				{//ɾ������Ԫ�ص�����
					temp->nextRow = tempRow->nextRow;
					//XMem::XDELETE_ARRAY(tempRow->title);
					XMem::XDELETE(tempRow);
					tempRow = temp->nextRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}else
		{//ȫ��ɾ��
			tempRow = m_tableRow;
			for(i = 0;i < m_tableRowSum;++ i)
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(temp->title);
				XMem::XDELETE(temp);
			}
			m_tableRowSum = 0;
			m_tableRow = NULL;
		}
		//ɾ���������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j >= rowSum)
					{//ɾ�����ڵ�Ԫ��
						//�ͷ���Դ
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			//������ص���ֵ
			m_tableRowSum = rowSum;
		}else
		{
			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j)
{
	curBox->isEnable = XTrue;

	curBox->isShow = XTrue;
	curBox->order.set(j,i);
	curBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&(curBox->text));
#endif
	curBox->text.setScale(m_scale * m_fontSize);
	curBox->text.setPosition(curRow->position.x,curRow->position.y + m_curTextHeight * (i + 1));
	curBox->textStr = "";
	setBoxStr("Line",i,j);
}
//�������������Ҫ���ý�������ֵ��ѡ�����ֵ
XBool XMultiListBasic::setLineSum(int lineSum)	//���������������Ĳ�����հף�����ɾ��
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(lineSum < 0) return XFalse;			//�Ƿ�����ֱ�ӷ�Χʧ��
	if(m_tableRowSum <= 0) return XFalse;	//�Ƿ�����ֱ�ӷ�Χʧ��
	if(lineSum == m_tableLineSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	//��ѡ���ǽ��и���
	if(lineSum > 0)
	{
		XBool *tempFlag = XMem::createArrayMem<XBool>(lineSum);
		XBool *tempFlag1 = XMem::createArrayMem<XBool>(lineSum);
		XBool *tempFlag2 = XMem::createArrayMem<XBool>(lineSum);
		if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) 
		{
			XMem::XDELETE_ARRAY(tempFlag);
			XMem::XDELETE_ARRAY(tempFlag1);
			XMem::XDELETE_ARRAY(tempFlag2);
			return XFalse;
		}

		for(int i = 0;i < min(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = m_haveSelectFlag[i];
			tempFlag1[i] = m_check0State[i];
			tempFlag2[i] = m_check1State[i];
		}
		for(int i = min(lineSum,m_tableLineSum);i < max(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = XFalse;
			tempFlag1[i] = XFalse;
			tempFlag2[i] = XFalse;
		}
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}

	int i,j;
	//�������ʵ������޸Ŀؼ��е�����
	if(lineSum > m_tableLineSum)
	{//���
		int oldLine = m_tableLineSum;
		m_tableLineSum = lineSum;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow;
		if(oldLine == 0)
		{//ȫ�����
			m_tableBox = XMem::createMem<XMultiListOneBox>();
			if(m_tableBox == NULL) return XFalse;

			initANewBoxData(m_tableBox,m_tableRow,0,0);

			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
					if(tempBox->nextBox == NULL) return XFalse;

					tempBox = tempBox->nextBox;
					initANewBoxData(tempBox,tempRow,i,j);

					tempRow = tempRow->nextRow;
					//ע������û�м���ڴ����ʧ��
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}else
		{//���
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= oldLine || (i == oldLine - 1 && j == m_tableRowSum - 1))	//���һ��Ԫ��ҲҪ��ô����
					{
						if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
						{//���һ��Ԫ��
							continue;
						}
						tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
						if(tempBox->nextBox == NULL) return XFalse;

						tempBox = tempBox->nextBox;
						tempBox->isEnable = XTrue;

						tempBox->isShow = XTrue;
						tempBox->order.set(j,i);
						tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
						XObjManager.decreaseAObject(&(tempBox->text));
#endif
						tempBox->text.setScale(m_scale * m_fontSize);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
						tempBox->textStr = "";
						if(j + 1 >= m_tableRowSum)
						{
							setBoxStr("Line",i + 1,0);
						}else
						{
							setBoxStr("Line",i,j + 1);
						}

						tempRow = tempRow->nextRow;
						//ע������û�м���ڴ����ʧ��
					}else
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}
	}else
	{//ɾ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(lineSum == 0)
		{//ȫ��ɾ��
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{//ɾ�����Ԫ��
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = lineSum;
			m_tableBox = NULL;
		}else
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= lineSum)
					{//ɾ�����Ԫ��
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			m_tableLineSum = lineSum;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::deleteLine(int order)		//ɾ��ĳһ��
{
	if(!m_isInited) return XFalse;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableLineSum) return XFalse;		//�Ƿ�������
	//��ѡ���ǽ��и���
	if(m_tableLineSum > 1)
	{
		XBool *tempFlag = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		XBool *tempFlag1 = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		XBool *tempFlag2 = XMem::createArrayMem<XBool>(m_tableLineSum - 1);
		if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) 
		{
			XMem::XDELETE_ARRAY(tempFlag);
			XMem::XDELETE_ARRAY(tempFlag1);
			XMem::XDELETE_ARRAY(tempFlag2);
			return XFalse;
		}
		for(int i = 0;i < m_tableLineSum - 1;++ i)
		{
			if(i < order) 
			{
				tempFlag[i] = m_haveSelectFlag[i];
				tempFlag1[i] = m_check0State[i];
				tempFlag2[i] = m_check1State[i];
			}else 
			{
				tempFlag[i] = m_haveSelectFlag[i + 1];
				tempFlag1[i] = m_check0State[i + 1];
				tempFlag2[i] = m_check1State[i + 1];
			}
		}
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{//ȫ��ɾ�������
		XMem::XDELETE_ARRAY(m_haveSelectFlag);
		XMem::XDELETE_ARRAY(m_check0State);
		XMem::XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	int i,j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ,ɾ����һ��
		for(i = 0;i < m_tableRowSum;++ i)
		{
			temp1 = tempBox->nextBox;
			//XMem::XDELETE_ARRAY(tempBox->string);
			XMem::XDELETE(tempBox);
			tempBox = temp1;
		}
		m_tableBox = tempBox;
		//�ı���浥Ԫ���λ��
		if(m_tableLineSum > 1)
		{
			tempBox = m_tableBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			for(i = 0;i < m_tableLineSum - 1;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i == 0 && j == 0)
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->order.set(i,j);
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{//����ɾ��Ҫ��
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				}else
				if(i > order)
				{//���ú��浥Ԫ���λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	-- m_tableLineSum;
	if(m_haveSelect)
	{//����ѡ��
		if(m_selectLineOrder == order)
		{//����ɾ������ѡ�����һ�У���ȡ��ѡ��
			m_haveSelect = XFalse;
			updateSelectLine();
		}else
		if(m_selectLineOrder > order)
		{
			-- m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::deleteRow(int order)		//ɾ��ĳһ��
{
	if(!m_isInited) return XFalse;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableRowSum) return XFalse;		//�Ƿ�������
	if(order == 0)
	{
		int i,j;
		//ɾ����ͷ
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		//XMem::XDELETE_ARRAY(tempRow->title);
		XMem::XDELETE(tempRow);
		if(m_tableRowSum == 1)
		{//���ֻ��һ��
			m_tableRow = NULL;
		}else
		{
			m_tableRow = temp;
			//���ñ����λ��
			m_tableRow->order = 0;
			m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x + MUTILIST_MIN_WIDTH * m_scale.x * 0,
				m_position.y + m_mouseRect.top * m_scale.y);
			m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
			if(!m_withoutTex)
			{
				m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			for(i = 1;i < m_tableRowSum - 1;++ i)	//�Ѿ�ɾ����һ��Ԫ��
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = temp->nextRow;
			}
		}
		//ɾ�����Ԫ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ�ص�ɾ�����⴦��������������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
				//	tempRow = tempRow->nextRow;
					continue;
				}else
				if(j == order)
				{
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						if(m_tableRowSum > 1)
						{
							tempBox->order.set(j,i);
							tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							tempRow = tempRow->nextRow;
						}
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//ɾ����һ��Ԫ��
		tempBox = m_tableBox;
		temp1 = tempBox->nextBox;
		//XMem::XDELETE_ARRAY(tempBox->string);
		XMem::XDELETE(tempBox);
		if(m_tableRowSum == 1)
		{//���ֻ��һ�У���ɾ��֮��û�е�λ���
			m_tableBox = NULL;
			m_tableLineSum = 0;
		}else
		{
			assert(temp1 != NULL);
			tempBox = temp1;
			m_tableBox = tempBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 0);
		}
	}else
	{
		int i,j;
		//ɾ����ͷ
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order)
			{
				temp->nextRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(tempRow->title);
				XMem::XDELETE(tempRow);
				tempRow = temp->nextRow;
				if(tempRow != NULL)
				{
					tempRow->order = i;
					tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
						m_position.y + m_mouseRect.top * m_scale.y);
					tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
					if(!m_withoutTex)
					{
						tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							m_tableRow->position.y);
					}
				}
			}else
			if(i > order)
			{
				tempRow->order = i;
				tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//ɾ�����Ԫ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order)
				{
					assert(temp1 != NULL);
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
					//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						tempBox->order.set(j,i);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
		}
	}
	m_tableRowSum--;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::insertALine(int order)		//��order������һ��
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum) return XFalse;
	//��ѡ���ǽ��и���
	XBool *tempFlag = XMem::createArrayMem<XBool>(m_tableLineSum + 1);
	XBool *tempFlag1 = XMem::createArrayMem<XBool>(m_tableLineSum + 1);
	XBool *tempFlag2 = XMem::createArrayMem<XBool>(m_tableLineSum + 1);

	if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL)
	{
		XMem::XDELETE_ARRAY(tempFlag);
		XMem::XDELETE_ARRAY(tempFlag1);
		XMem::XDELETE_ARRAY(tempFlag2);
		return XFalse;
	}
	for(int i = 0;i < m_tableLineSum + 1;++ i)
	{
		if(i < order) 
		{
			tempFlag[i] = m_haveSelectFlag[i]; 
			tempFlag1[i] = m_check0State[i]; 
			tempFlag2[i] = m_check1State[i]; 
		}else
		if(i == order) 
		{
			tempFlag[i] = XFalse; 
			tempFlag1[i] = XFalse; 
			tempFlag2[i] = XFalse; 
		}else
		{
			tempFlag[i] = m_haveSelectFlag[i + 1];
			tempFlag1[i] = m_check0State[i + 1];
			tempFlag2[i] = m_check1State[i + 1];
		}
	}
	XMem::XDELETE_ARRAY(m_haveSelectFlag);
	XMem::XDELETE_ARRAY(m_check0State);
	XMem::XDELETE_ARRAY(m_check1State);
	m_haveSelectFlag = tempFlag;
	m_check0State = tempFlag1;
	m_check1State = tempFlag2;

	int i,j;
	if(order == 0)
	{//��ͷ������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *temp1 = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);
		tempBox = m_tableBox;
		for(i = 0;i < m_tableRowSum - 1;++ i)
		{
			tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
			if(tempBox->nextBox == NULL) return XFalse;

			tempBox->nextBox->isEnable = XTrue;
			tempBox->nextBox->isShow = XTrue;
			tempBox->nextBox->order.set(i,0);
			tempBox->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempBox->nextBox->text));
#endif
			tempBox->nextBox->text.setScale(m_scale * m_fontSize);
			tempBox->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
			tempBox->nextBox->textStr = "";
			setBoxStr("Line",0,i + 1);
			tempBox = tempBox->nextBox;
			tempRow = tempRow->nextRow;
		}
		tempBox->nextBox = temp1;
		//�����������Ԫ�������ƶ�
		tempBox = m_tableBox;

		++ m_tableLineSum;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
					continue;
				}
				tempBox->order.set(j,i);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox = tempBox->nextBox;
				tempRow = tempRow->nextRow;
			}
		}

		if(m_haveSelect &&m_selectLineOrder >= order)
		{
			++ m_selectLineOrder;
			updateSelectLine();
		}
	}else
	{
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
				//	if(j == m_tableRowSum - 1)
				//	{//���һ����ӵ�Ԫ��
				//		temp1->nextBox = tempBox;
				//	}
					tempRow = tempRow->nextRow;
				}else
				if(i > order)
				{//�ı�λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}
			}
		}
		++ m_tableLineSum;
		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				++ m_selectLineOrder;
				updateSelectLine();
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::insertARow(int order)		//��order������һ��
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	if(m_tableRowSum >= MUTILIST_MAX_ROW_SUM) return XFalse;	//�����е����ֵ�����˳�
	int i,j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp = NULL;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ
		//�ı������
		tempRow = m_tableRow;
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) return XFalse;

		initANewRowData(NULL,0);
/*		m_tableRow->isEnable = 1;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
		m_tableRow->position.set(m_objRect.left + m_edgeDistance.left * m_scale.x + MUTILIST_MIN_WIDTH * m_scale.x * 0,
			m_objRect.top + m_edgeDistance.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;*/

		m_tableRow->nextRow = tempRow;
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->order = i + 1;
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı�������
		++ m_tableRowSum;
		//�����һ��Ԫ��
		tempBox = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);
		m_tableBox->nextBox = tempBox;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//����
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		//�ı������
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum + 1;++ i)
		{
			if(i == order)
			{//����
				temp->nextRow = XMem::createMem<XMultiListOneRow>();
				if(temp->nextRow == NULL) return XFalse;

				initANewRowData(temp,i);
			/*	temp->nextRow->isEnable = 1;
				temp->nextRow->order = i;
				temp->nextRow->text.setACopy(m_caption);
				temp->nextRow->pixSize.set(MUTILIST_MIN_WIDTH * m_scale.x,m_tableRow->text.m_scale.y * m_tableRow->text.m_showSize.x);
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x,
					m_objRect.top + m_edgeDistance.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				temp->nextRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
				temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
				temp->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
				temp->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
				temp->nextRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
				temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
				temp->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

				temp->nextRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
				temp->nextRow->title = NULL;
				setTileStr("Title",i);	//����Ĭ����ʾ
				temp->nextRow->isShow = 1;*/

				temp = temp->nextRow;
				temp->nextRow = tempRow;
			}else
			if(i > order)
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x -
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//�ı�������
		++ m_tableRowSum;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//����
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::moveDownLine(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	XMath::swapTwoSum(m_haveSelectFlag[order],m_haveSelectFlag[order + 1]);
	XMath::swapTwoSum(m_check0State[order],m_check0State[order + 1]);
	XMath::swapTwoSum(m_check1State[order],m_check1State[order + 1]);
	//���濪ʼ�ƶ�
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	XMultiListOneBox temp2;
	//2�е�6���˵�Ľڵ�
	XMultiListOneBox * point0 = NULL;
	XMultiListOneBox * point1 = NULL;
	XMultiListOneBox * point2 = NULL;
	XMultiListOneBox * point3 = NULL;
	XMultiListOneBox * point4 = NULL;
	XMultiListOneBox * point5 = NULL;
	int i,j;
	if(order == 0)
	{//�ƶ����ǵ�һ��
		temp1 = m_tableBox;
		point1 = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		//point0->nextBox = point3;
		assert(point2 != NULL);
		point2->nextBox = point5;
		assert(point4 != NULL);
		point4->nextBox = point1;
		m_tableBox = point3;
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order - 1 && j == m_tableRowSum - 1)
				{
					point0 = tempBox;
					point1 = tempBox->nextBox;
				}
				if(i == order)
				{
					temp1 = tempBox;
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		assert(point0 != NULL);
		assert(point2 != NULL);
		assert(point4 != NULL);
		point0->nextBox = point3;
		point2->nextBox = point5;
		point4->nextBox = point1;
	}
	if(m_haveSelect)
	{
		if(m_selectLineOrder == order)
		{
			++ m_selectLineOrder;
			updateSelectLine();
		}else
		if(m_selectLineOrder == order + 1)
		{
			-- m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiListBasic::moveRightRow(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	int i = 0,j = 0;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp1 = NULL;
	XMultiListOneRow *point0 = NULL;
	XMultiListOneRow *point1 = NULL;

	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp2 = NULL;
	XMultiListOneBox *point2 = NULL;
	XMultiListOneBox *point3 = NULL;
	if(order == 0)
	{
		//�ƶ���ͷ
	//	point0 = tempRow;
	//	tempRow = tempRow->nextRow;
		temp1 = tempRow->nextRow;
		point1 = temp1->nextRow;

	//	point0->nextRow = temp1;
		m_tableRow = temp1;
		temp1->order = tempRow->order;
		temp1->position = tempRow->position;
		temp1->text.setPosition(temp1->position.x,temp1->position.y);
		temp1->nextRow = tempRow;
		temp1->nextRow->order = temp1->order + 1;
		temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
		temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
		if(!m_withoutTex)
		{
			temp1->m_spriteTitle.setPosition(temp1->position);
			temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
			temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
			temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				temp1->nextRow->position.y);
		}
		temp1->nextRow->nextRow = point1;

		//�ƶ�����
		if(m_tableLineSum > 1)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						tempBox = tempBox->nextBox;
						temp2 = tempBox->nextBox;
						point3 = temp2->nextBox;

						point2->nextBox = temp2;
						temp2->order = tempBox->order;
						temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox = tempBox;
						if(j == m_tableRowSum -1)
						{
							tempRow = m_tableRow;
							temp2->nextBox->order.set(0,temp2->order.y);
							j = 0;
							++ i;
						}else
						{//��������ǲ�����ֵ�
							tempRow = tempRow->nextRow;
							temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
							++ j;
						}
						temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox->nextBox = point3;
						if(i >= m_tableLineSum - 1)
						{
							break;
						}
					}else
					{
						tempRow = tempRow->nextRow;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//����һ��Ԫ��
	//	point2 = tempBox;
		tempBox = m_tableBox;
		temp2 = tempBox->nextBox;
		point3 = temp2->nextBox;

		m_tableBox = temp2;
		temp2->order = tempBox->order;
		temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox = tempBox;

		tempRow = tempRow->nextRow;
		temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);

		temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox->nextBox = point3;
	}else
	{
		//�ƶ���ͷ
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order - 1)
			{
				point0 = tempRow;
				tempRow = tempRow->nextRow;
				temp1 = tempRow->nextRow;
				point1 = temp1->nextRow;

				point0->nextRow = temp1;
				temp1->order = tempRow->order;
				temp1->position = tempRow->position;
				temp1->text.setPosition(temp1->position.x,temp1->position.y);
				temp1->nextRow = tempRow;
				temp1->nextRow->order = temp1->order + 1;
				temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
				temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp1->m_spriteTitle.setPosition(temp1->position);
					temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
					temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
					temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						temp1->nextRow->position.y);
				}
				temp1->nextRow->nextRow = point1;
				break;
			}
			tempRow = tempRow->nextRow;
		}
		//�ƶ�����
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order - 1)
				{
					point2 = tempBox;
					tempBox = tempBox->nextBox;
					temp2 = tempBox->nextBox;
					point3 = temp2->nextBox;

					point2->nextBox = temp2;
					temp2->order = tempBox->order;
					temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox = tempBox;
					if(j == m_tableRowSum -1)
					{//��������ǲ�����ֵ�
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(0,temp2->order.y);
						j = 0;
						++ i;
					}else
					{
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
						++ j;
					}
					temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox->nextBox = point3;
					if(i >= m_tableLineSum - 1)
					{
						break;
					}
				}else
				{
					tempRow = tempRow->nextRow;
					tempBox = tempBox->nextBox;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiListBasic::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_curMouseRect.set(m_position.x + (m_mouseRect.left - 64.0f) * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		//m_check0[i].setScale(m_scale);
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((DEFAULT_CHECK_SIZE + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
		//m_check1[i].setScale(m_scale);
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((DEFAULT_CHECK_SIZE + m_mouseRect.top) * m_scale.y + m_curTextHeight * i));
	}
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;
		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	//updateSliderState();
	updateShowChange();	//������������������ƶ���ʱ���bug
	updateChildPos();
}
void XMultiListBasic::setScale(float x,float y)	//���óߴ�
{
	if(!m_isInited ||
		x <= 0 || y <= 0) return;
	m_scale.set(x,y);
	m_curMouseRect.set(m_position.x + (m_mouseRect.left - 64.0f) * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_spriteMove.setScale(m_scale);
	m_caption.setScale(m_scale * m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	if(m_withoutTex)
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	}else
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setScale(m_scale);
		m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].setScale(m_scale);
		m_check0[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE * 2.0f) * m_scale.x,
			m_position.y + ((m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
		m_check1[i].setScale(m_scale);
		m_check1[i].setPosition(m_position.x + (m_mouseRect.left - DEFAULT_CHECK_SIZE) * m_scale.x,
			m_position.y + ((m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + m_curTextHeight * i));
	}
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->text.setScale(m_scale * m_fontSize);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;

		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitle.setScale(tempRow->nextRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->nextRow->m_spriteTitleEnd.setScale(tempRow->nextRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
					tempRow->nextRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			m_tableBox->text.setScale(m_scale * m_fontSize);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox->text.setScale(m_scale * m_fontSize);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect)
	{
		updateSelectLine();
	}
	updateSliderState();
	updateShowChange();
	updateChildScale();
}
#if !WITH_INLINE_FILE
#include "XMultiListBasic.inl"
#endif
}