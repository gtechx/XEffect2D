#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"

#include "XNodeLine.h"
#include "XControl/XControlManager.h"
#include "XWindowTitle.h"
#include "XControl\XMultiListBasic.h"

namespace XE{
void XObjectManager::setShow()	
{
	m_isShowUI = XTrue;
	m_mutiList->setVisible();
}	//������ʾ
void XObjectManager::disShow() 
{
	m_isShowUI = XFalse;
	m_mutiList->disVisible();
}	//���ò���ʾ
void XObjectManager::ctrlProc(void*pClass,int,int eventID)
{
	switch(eventID)
	{
	case XMultiListBasic::MLTLST_SELECT:
	//	printf("select change! %d\n",((XObjectManager *)pClass)->m_mutiList->getSelectOrder());
		break;
	case XMultiListBasic::MLTLSTB_CHK_STATE_CHANGE:
		//printf("check change!\n");
		XObjectManager & pPar = *(XObjectManager *)pClass;
		//�������״̬�ĸı�
		int tempChooseOBJSum = pPar.getCurChooseObjectSum();
		for(int i = 0;i < tempChooseOBJSum;++ i)
		{
			if(pPar.m_objInfo[i].m_lineObjOrder >= 0)
			{
				if(pPar.m_mutiList->getCheckState(1,i)) pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objCanEdit = XTrue;
				else 
				{
					pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objCanEdit = XFalse;
					if(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objectKeyOption != OBJ_OPTION_NULL) 
						pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_objectKeyOption = OBJ_OPTION_NULL;	//���óɲ��ɱ༭״̬
				}

				if(pPar.m_mutiList->getCheckState(0,i)) ((XObjectBasic *)(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_pObject))->setVisible();
				else ((XObjectBasic *)(pPar.m_objInfo[pPar.m_objInfo[i].m_lineObjOrder].m_pObject))->disVisible();
			}else
			{
				break;
			}
		}
		break;
	}
}
XObjectManager::XObjectManager()
	:m_isInited(XFalse)
	,m_isShowUI(XTrue)
	,m_isOption(XTrue)
	,m_editWindowsPos(0)
	,m_canSelect(XTrue)		//Ĭ��״̬���������ѡ�����
#if OBJ_MANAGER_WITH_ID
	,m_showObjID(0)
#endif
	,m_ctrlKeyState(KEY_STATE_UP)
	,m_canAddObj(XTrue)
{
	//put single object into auto_ptr object 
	//m_auto_ptr = auto_ptr<XObjectManager>(this);
	m_objInfo.clear();
	m_curMouseOnObjectSum = 0;
	//�����ǹ��ڰ������������ĳ�ʼ��������
	m_keyMaxTime = 500;
	m_keyMinTime = 10;
	for(int i = 0;i < 4;++ i)
	{
		m_keyState[i] = KEY_STATE_UP;
		m_keyTimer[i] = 0;
		m_keyCurTime[i] = m_keyMaxTime;
	}
	m_font = XMem::createMem<XFontUnicode>();
	if(m_font == NULL) LogStr("Mem Error!");
	m_mousePosFont = XMem::createMem<XFontUnicode>();
	if(m_mousePosFont == NULL) LogStr("Mem Error!");
	m_mutiList = XMem::createMem<XMultiListBasic>();
	if(m_mutiList == NULL) LogStr("Mem Error!");
}
int XObjectManager::addAObject(XObjectBasic * object)
{
//	if(!m_isInited) return -1;
	//if(m_curObjectSum >= MAX_OBJECT_SUM) return -1;	//����Ѿ����ˣ�ע��ʧ��
	if(!m_canAddObj ||
		object == NULL ||
		object->m_objType == OBJ_NULL) return -1;	//��Ч���������ע��
	int ret = findObjectID(object);
	if(ret >= 0) return ret;	//�������Ѿ�ע�ᣬ���ֹ�ظ�ע��
	//���濪ʼע�ᶯ��
	XObjectInfo tmp;
	tmp.m_objectMouseState = OBJ_STATE_NULL;		//Ĭ��״̬
	//tmp.m_objectUpMouseState = OBJ_STATE_NULL;		//Ĭ��״̬

	tmp.m_pObject = object;		//��ʼ��ָ����Ч
//	tmp.m_objectType = object->m_objType;	//��Ч���������
	tmp.m_objectMousePoint.set(0.0f,0.0f);
	tmp.m_objectKeyOption = OBJ_OPTION_NULL;
	tmp.m_objectSetState = OBJ_OPTION_STATE_NULL;
	
	tmp.m_objectEditParm = -1;
#if OBJ_MANAGER_WITH_ID		//�������ɵݹ���ö�����ѭ��������
	m_canAddObj = XFalse;
	tmp.m_fontID = XMem::createMem<XFontUnicode>();
	if(tmp.m_fontID == NULL) return -1;
	tmp.m_fontID->setACopy(*m_font);
	decreaseAObject(tmp.m_fontID);
	tmp.m_fontID->setMaxStrLen(5);
	m_canAddObj = XTrue;
#endif

	//��ʼ�������״̬
	tmp.m_objCanEdit = XFalse;	//���ɱ༭
	tmp.m_objBeSelect = XFalse;	//����ѡ��
	
	m_objInfo.push_back(tmp);
	//printf("ObjectSum:%d\n",m_curObjectSum);
	return (int)(m_objInfo.size()) - 1;
}
void XObjectManager::decreaseAObject(unsigned int objectID)
{
//	if(!m_isInited) return;
	if(objectID >= m_objInfo.size()) return;	//��������Ƿ�
	if(m_objInfo.size() <= 0) return;	//���������û�������ֱ�ӷ���
	//�������г�ȡ���ע�ᣬ��������������
	if(objectID == m_objInfo.size() - 1)
	{//�Ѿ������һ������
#if OBJ_MANAGER_WITH_ID
		XMem::XDELETE(m_objInfo[(int)(m_objInfo.size()) - 1].m_fontID);
#endif
		m_objInfo.pop_back();
	}else
	{//�������һ������
#if OBJ_MANAGER_WITH_ID
		XMem::XDELETE(m_objInfo[objectID].m_fontID);
#endif
		//std::copy(m_objInfo[objectID + 1],m_objInfo[(int)(m_objInfo.size()) - 1],m_objInfo[objectID + 1]);
		for(int i = objectID;i < (int)(m_objInfo.size()) - 1;++ i)
		{
			m_objInfo[i] = m_objInfo[i + 1];
		}
#if OBJ_MANAGER_WITH_ID
		m_objInfo[(int)(m_objInfo.size()) - 1].m_fontID = NULL;
#endif
		m_objInfo.pop_back();
	}
	//printf("ObjectSum:%d\n",m_curObjectSum);
}
void XObjectManager::keyProc(int keyID,XKeyState keyState)
{
	if(!m_isInited	||	//���û�г�ʼ���Ļ�ֱ�ӷ���
		m_objInfo.size() == 0) return;	//���û�����ֱ�ӷ���
	if(keyID == XKEY_LCTRL || keyID == XKEY_RCTRL) m_ctrlKeyState = keyState;	//ctrl���ܼ���״̬

	if(m_ctrlKeyState == KEY_STATE_DOWN && keyID == XKEY_U && keyState == KEY_STATE_UP)
	{
		if(!m_isOption) setOption();
		else disOption();
	}

	if(!m_isOption) return;
	switch(keyID)
	{
	case XKEY_H:	//�Ƿ���ʾUI����
		if(keyState == KEY_STATE_UP &&
			m_ctrlKeyState == KEY_STATE_DOWN)
		{
			if(!m_isShowUI) setShow();
			else disShow();
		}
		break;
	case XKEY_P:	//�������Դ��ڵ�λ��
		if(keyState == KEY_STATE_UP &&
			m_ctrlKeyState == KEY_STATE_DOWN)
		{
			++ m_editWindowsPos;
			if(m_editWindowsPos >= 4) m_editWindowsPos = 0;
			setEditWidowsPos();
		}
		break;
	case XKEY_DELETE:	//ɾ�����Դ�����ѡ�����
		if(keyState == KEY_STATE_UP)
		{//�����������,Ŀǰ�в�ʵ��
			for(int i = 0;i < m_mutiList->getTableLineSum();++ i)
			{
				if(m_mutiList->getHaveSelectState(i))
				{
					m_objInfo[m_objInfo[i].m_lineObjOrder].m_objBeSelect = XFalse;	//ȡ��ѡ��
				}
			}
			upDateMultiListData();	//���¶����б�������
		}
		break;
	case XKEY_N:		//�Ƿ����ʹ������ѡ���
		if(keyState == KEY_STATE_UP) chengeCanSelectState();
		break;
#if OBJ_MANAGER_WITH_ID
	case XKEY_B:		//�Ƿ���ʾ�����ID
		if(keyState == KEY_STATE_UP) setShowObjID();
		break;
#endif
	case XKEY_Q:	//ȥ�����в���
		if(keyState == KEY_STATE_UP)
		{
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
				updateObjStateInfo(i);
			}
		}
		break;
	case XKEY_E:	//nodeline�������༭״̬
		if(keyState == KEY_STATE_UP)
		{//����༭״̬���л�
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE && 
					m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{
					if(m_objInfo[i].m_objectKeyOption != OBJ_OPTION_EDIT)
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_EDIT;
						printf("NodeLine Length:%f\n",((XNodeLine *)m_objInfo[i].m_pObject)->getNodeLineLength());
						updateObjStateInfo(i);
					}else
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_S:	//����
		if(keyState == KEY_STATE_UP)
		{//����,��ʱ�������б�ѡ�е�����������Ų���
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//��ѡ���пɱ༭
					if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE && 
						m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
					{//���ﱣ�����ߵ��ļ���
						((XNodeLine *)m_objInfo[i].m_pObject)->saveNodeLine();
					}else
					{
						m_objInfo[i].m_objectKeyOption = OBJ_OPTION_SIZE;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_R:	//��ת
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//��ѡ���ҿɱ༭
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_ROTATE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_M:	//�ƶ�
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//��ѡ���ҿɱ༭
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_POSITION;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_D:
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT && 
					m_objInfo[i].m_objCanEdit && m_objInfo[i].m_objBeSelect)
				{//����������ɾ����ǰ�ĵ�
					XNodeLine * temp = (XNodeLine *)m_objInfo[i].m_pObject;
					if(temp->getNodeSum() > 2 && m_objInfo[i].m_objectEditParm >= 0 && m_objInfo[i].m_objectEditParm < temp->getNodeSum()) //����Ҫ����2�������ɾ����
					{
						temp->decreaseOneNode(m_objInfo[i].m_objectEditParm);
						if(m_objInfo[i].m_objectEditParm >= temp->getNodeSum()) m_objInfo[i].m_objectEditParm = temp->getNodeSum() - 1;
					}
				}
			}
		}
		break;
	case XKEY_A://�����������ڵ�ǰλ������һ����
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT && 
					m_objInfo[i].m_objCanEdit && m_objInfo[i].m_objBeSelect)
				{//����������ɾ����ǰ�ĵ�
					XNodeLine * temp = (XNodeLine *)m_objInfo[i].m_pObject;
					if(m_objInfo[i].m_objectEditParm >= 0 && m_objInfo[i].m_objectEditParm < temp->getNodeSum() && 
						temp->getScale().x != 0.0f && temp->getScale().y != 0.0f) //����Ҫ����2�������ɾ����
					{
						temp->addOneNode((m_curMousePosition.x - temp->getPosition().x) / temp->getScale().x,
							(m_curMousePosition.y - temp->getPosition().y) / temp->getScale().y,
							m_objInfo[i].m_objectEditParm);
					}
				}
			}
		}
		break;
	case XKEY_C:	//�ı���ת��׼��
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//��ѡ���ҿɱ༭
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_ROTATEMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_T:	//���÷�ת״̬
		if(keyState == KEY_STATE_UP)
		{//����
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				if(m_objInfo[i].m_objBeSelect && m_objInfo[i].m_objCanEdit)
				{//��ѡ���ҿɱ༭
					m_objInfo[i].m_objectKeyOption = OBJ_OPTION_TURNOVERMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_LEFT:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[0] = KEY_STATE_DOWN;
			m_keyTimer[0] = 0;
			m_keyCurTime[0] = m_keyMaxTime;
		}else
		{//������������������Ӧ�Ĳ���
			m_keyState[0] = KEY_STATE_UP;
		}
		break;
	case XKEY_RIGHT:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[1] = KEY_STATE_DOWN;
			m_keyTimer[1] = 0;
			m_keyCurTime[1] = m_keyMaxTime;
		}else
		{//����
			m_keyState[1] = KEY_STATE_UP;
		}
		break;
	case XKEY_UP:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[2] = KEY_STATE_DOWN;
			m_keyTimer[2] = 0;
			m_keyCurTime[2] = m_keyMaxTime;
		}else
		{//����
			m_keyState[2] = KEY_STATE_UP;
		}
		break;
	case XKEY_DOWN:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[3] = KEY_STATE_DOWN;
			m_keyTimer[3] = 0;
			m_keyCurTime[3] = m_keyMaxTime;
		}else
		{//����
			m_keyState[3] = KEY_STATE_UP;
		}
		break;
	}
}
void XObjectManager::objectKeyOption(unsigned int order,XObjectOptionType optionType)
{
	if(order >= m_objInfo.size()) return;
	switch(optionType)
	{
	case OBJ_OPTION_SIZE_DOWN:	//��С
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 size = temp->getScale();
				if(size.x - 0.01f >= 0) size.x -= 0.01f;
				else size.x = 0.0f;
				if(size.y - 0.01f >= 0) size.y -= 0.01f;
				else size.y = 0.0f;
				temp->setScale(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_SIZE_ON:	//�Ŵ�
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 size = temp->getScale();
				size += 0.01f;
				temp->setScale(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_DOWN:	//�Ƕ���С
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				float angle = temp->getAngle();
				angle -= 0.5f;
				if(angle < 0) angle += 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_ON:	//�Ƕȱ��
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				float angle = temp->getAngle();
				angle += 0.5f;
				if(angle > 360.0f) angle -= 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_MODE_CHANGE:	//��תģʽ�ı仯
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
			{
				XSprite * temp = (XSprite *)m_objInfo[order].m_pObject;
				XTransformMode mode = temp->getTransformMode();
				if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
				else temp->setIsTransformCenter(POINT_LEFT_TOP);
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		XFrameEx * temp = (XFrameEx *)m_pObject[order];
		//		XTransformMode mode = temp->getTransformMode();
		//		if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
		//		else temp->setIsTransformCenter(POINT_LEFT_TOP);
		//	}
		//	break;
		}
		break;
	case OBJ_OPTION_TURNOVER_MODE_CHANGE:	//��תģʽ�ı仯
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
			{
				XSprite * temp = (XSprite *)m_objInfo[order].m_pObject;
				XTurnOverMode mode = temp->getTurnOverMode();
				if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
				if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
				else temp->resetLeftRightUpDown();
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		XFrameEx * temp = (XFrameEx *)m_pObject[order];
		//		XTurnOverMode mode = temp->getTurnOverMode();
		//		if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
		//		if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
		//		else temp->resetLeftRightUpDown();
		//	}
		//	break;
		}
		break;
	case OBJ_OPTIONX_ON:	//�ƶ�
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.x += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTIONX_DOWN:	//�ƶ�
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.x -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_ON:	//�ƶ�
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.y += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_DOWN:	//�ƶ�
		switch(m_objInfo[order].m_pObject->m_objType)
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				XObjectBasic * temp = (XObjectBasic *)m_objInfo[order].m_pObject;
				XVector2 position = temp->getPosition();
				position.y -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	}
}
void XObjectManager::mouseProc(int x,int y,XMouseState eventType)
{//�����Ӧ����
	if(!m_isInited ||
		!m_isOption) return;
	m_curMouseOnObjectSum = 0;
	XBool stateChangeFlag = XFalse;	//ѡ���״̬�Ƿ����˸ı�
	if(m_isShowUI && m_mutiList->isInRect(x,y)) return;	//��ʾ�ռ��ʱ����Ϣ�������´���

	switch(eventType)
	{
	case MOUSE_LEFT_BUTTON_DOWN:	//������µĶ���
		//printf("Mouse Left Botton Down:%d,%d\n",x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					XObjectBasic * temp = (XObjectBasic *)m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//�����갴��
						m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;	//���崦�ڰ���״̬

						m_objInfo[i].m_objectMousePoint = temp->getPosition();
						m_objInfo[i].m_objectMousePoint.x -= x;
						m_objInfo[i].m_objectMousePoint.y -= y;
					}
				}
				break;
			case OBJ_NODELINE:
				{
					XObjectBasic * temp = (XObjectBasic *)m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//���������ϱ������������Ӧ
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{//���ڱ༭״̬
							XNodeLine *tempNodeLine = (XNodeLine *)m_objInfo[i].m_pObject;
							int tempOrder = -1;
							if(tempNodeLine->getScale().x != 0 && tempNodeLine->getScale().y != 0)
							{//��ֹ�������
								tempOrder = tempNodeLine->getNodeOrder((x - tempNodeLine->getPosition().x) / tempNodeLine->getScale().x,
									(y - tempNodeLine->getPosition().y) / tempNodeLine->getScale().y,5);
								if(tempOrder >= 0)// && tempOrder != m_objectEditParm[i])
								{
									m_objInfo[i].m_objectEditParm = tempOrder;

									m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;
									m_objInfo[i].m_objectMousePoint = tempNodeLine->getNode(tempOrder);
									m_objInfo[i].m_objectMousePoint.x -= x / tempNodeLine->getScale().x;
									m_objInfo[i].m_objectMousePoint.y -= y / tempNodeLine->getScale().y;
								}
							}
						}else
						{//�Ǳ༭״̬
							m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_DOWN;
							m_objInfo[i].m_objectMousePoint = temp->getPosition();
							m_objInfo[i].m_objectMousePoint.x -= x;
							m_objInfo[i].m_objectMousePoint.y -= y;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:		//���������
		//printf("Mouse Left Botton Up:%d,%d\n",x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
				{
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN)	//ֻҪ��굯�𣬲����Ƿ��������λ�ö�����Ӧ�ж�
					{//���������ϱ������������Ӧ
						if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE)
						{
							if(m_objInfo[i].m_objectKeyOption != OBJ_OPTION_EDIT)
							{//�Ǳ༭״̬
								if(m_canSelect) 
								{
									if(!m_objInfo[i].m_objBeSelect) m_objInfo[i].m_objBeSelect = XTrue;
									else 
									{
										m_objInfo[i].m_objBeSelect = XFalse;
										m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
									}
								}
								m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
							}else
							{//�༭״̬
								m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;	//���û�ж���
							}
						}else
						{
							if(m_canSelect) 
							{
								if(!m_objInfo[i].m_objBeSelect) m_objInfo[i].m_objBeSelect = XTrue;
								else 
								{
									m_objInfo[i].m_objBeSelect = XFalse;
									m_objInfo[i].m_objectKeyOption = OBJ_OPTION_NULL;
								}
							}
							m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
						}
						stateChangeFlag = XTrue;
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_MOVE)
					{//�������϶�״̬����
						stateChangeFlag = XTrue;
						m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_MOVE:		//����ƶ��¼�
		m_curMousePosition.set(x,y);
		for(unsigned int i = 0;i < m_objInfo.size();++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
			{
				if(m_objInfo[i].m_objectMouseState == OBJ_STATE_NULL)	//�����¼����ж�
				{//���֮ǰ�������״̬
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					if(temp->isInRect(x,y))
					{//������Ϊ�������״̬
						m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_ON;	//���������¼�
						m_curMouseOnObjectSum ++;
					}
				}else
				if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_ON)	//����Ѿ���������״̬���������ж��Ƕ�����״̬����
				{//����Ѿ���������״̬���������ж��Ƿ��˳�����״̬
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					if(!temp->isInRect(x,y)) m_objInfo[i].m_objectMouseState = OBJ_STATE_NULL;	//��������״̬����
				}else
				if((m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN || 
					m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_MOVE) && m_objInfo[i].m_objCanEdit)	//���״̬�Ļ����������ø�������ƶ�
				{//����Ѿ����ڰ���״̬������ק״̬�����ҿ��Ա༭
					if(m_objInfo[i].m_pObject->m_objType != OBJ_NODELINE)
					{//�ı������λ��
						XObjectBasic * temp = m_objInfo[i].m_pObject;
						temp->setPosition(m_objInfo[i].m_objectMousePoint.x + x,m_objInfo[i].m_objectMousePoint.y + y);
					}else
					{//���������߿����Ǹı������ߵ�λ�û����Ǹı���������ĳ�����λ��
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{//�ı�NodeLine�еĵ��λ��
							((XNodeLine *)m_objInfo[i].m_pObject)->setOneNode(m_objInfo[i].m_objectMousePoint.x + x / ((XNodeLine *)m_objInfo[i].m_pObject)->getScale().x,
								m_objInfo[i].m_objectMousePoint.y + y / ((XNodeLine *)m_objInfo[i].m_pObject)->getScale().y,m_objInfo[i].m_objectEditParm);
						}else
						{
							XObjectBasic * temp = m_objInfo[i].m_pObject;
							temp->setPosition(m_objInfo[i].m_objectMousePoint.x + x,m_objInfo[i].m_objectMousePoint.y + y);
						}
					}
					m_objInfo[i].m_objectMouseState = OBJ_STATE_MOUSE_MOVE;	//����϶�״̬
				}
			}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	if(stateChangeFlag) upDateMultiListData();//�����ѡ���״̬�����˸ı�Ļ���������Ҫ�����б��
}
void XObjectManager::upDateMultiListData()
{
	m_mutiList->clearAllSelect();
	m_mutiList->setLineSum(0);
	char tempStr[256] = "";
	int lineOrder = 0;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect)
		{
			m_mutiList->addALine();
			sprintf(tempStr,"%d",i);
			m_mutiList->setBoxStr(tempStr,lineOrder,0);
			sprintf(tempStr,"%.0f,%.0f",m_objInfo[i].m_position.x,m_objInfo[i].m_position.y);
			m_mutiList->setBoxStr(tempStr,lineOrder,3);
			sprintf(tempStr,"%.2f,%.2f",m_objInfo[i].m_scale.x,m_objInfo[i].m_scale.y);
			m_mutiList->setBoxStr(tempStr,lineOrder,4);
			sprintf(tempStr,"%.2f",m_objInfo[i].m_angle);
			m_mutiList->setBoxStr(tempStr,lineOrder,5);
			m_objInfo[i].m_objLineOrder = lineOrder;
			m_objInfo[lineOrder].m_lineObjOrder = i;
			updateObjStateInfo(i);	//����Ҫ��ʼ���˶�Ӧ��ϵ֮����ܸ���״̬
			//switch(m_objectKeyOption[i])
			//{
			//case OBJ_OPTION_NULL:m_mutiList->setBoxStr("Null",lineOrder,1);break;
			//case OBJ_OPTION_POSITION:m_mutiList->setBoxStr("Position",lineOrder,1);break;
			//case OBJ_OPTION_SIZE:m_mutiList->setBoxStr("Size",lineOrder,1);break;
			//case OBJ_OPTION_ROTATE:m_mutiList->setBoxStr("Rotate",lineOrder,1);break;
			//case OBJ_OPTION_ROTATEMODE:m_mutiList->setBoxStr("RotateMode",lineOrder,1);break;
			//case OBJ_OPTION_TURNOVERMODE:m_mutiList->setBoxStr("TurnOverMode",lineOrder,1);break;
			//}
			//�����������Ŀɱ༭״̬
			if(!m_objInfo[i].m_objCanEdit) m_mutiList->setCheckState(1,lineOrder,XFalse);
			else m_mutiList->setCheckState(1,lineOrder,XTrue); 

			if((m_objInfo[i].m_pObject)->getVisible()) m_mutiList->setCheckState(0,lineOrder,XTrue);
			else m_mutiList->setCheckState(0,lineOrder,XFalse); 
			lineOrder ++;
		}else
		{
			m_objInfo[i].m_objLineOrder = -1;
		}
	}
}
void XObjectManager::updateObjStateInfo(int order)
{
	switch(m_objInfo[order].m_pObject->m_objType)
	{
	case OBJ_NULL:m_mutiList->setBoxStr("Null",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_SPRITE:m_mutiList->setBoxStr("Sprite",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FRAME:m_mutiList->setBoxStr("Frame",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FRAMEEX:m_mutiList->setBoxStr("FrameEx",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_NUMBER:m_mutiList->setBoxStr("Number",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FONTUNICODE:m_mutiList->setBoxStr("FontUnicode",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_FONTX:m_mutiList->setBoxStr("Fontx",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_NODELINE:m_mutiList->setBoxStr("NodeLine",m_objInfo[order].m_objLineOrder,1);break;
	case OBJ_CONTROL:m_mutiList->setBoxStr("Control",m_objInfo[order].m_objLineOrder,1);break;
	}
	switch(m_objInfo[order].m_objectKeyOption)
	{
	case OBJ_OPTION_NULL:m_mutiList->setBoxStr("Null",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_POSITION:m_mutiList->setBoxStr("Position",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_SIZE:m_mutiList->setBoxStr("Size",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_ROTATE:m_mutiList->setBoxStr("Rotate",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_ROTATEMODE:m_mutiList->setBoxStr("RotateMode",m_objInfo[order].m_objLineOrder,2);break;
	case OBJ_OPTION_TURNOVERMODE:m_mutiList->setBoxStr("TurnOverMode",m_objInfo[order].m_objLineOrder,2);break;
	}
}
void XObjectManager::updateObjInfo(int order)
{
	//printf("Update Object Info -> %d.\n",order);
	XObjectBasic * temp = m_objInfo[order].m_pObject;
	m_objInfo[order].m_position = temp->getPosition();
	m_objInfo[order].m_scale = temp->getScale();
	m_objInfo[order].m_angle = temp->getAngle();
//	m_objInfo[order].m_alpha = temp->getAngle();

	char tempStr[256] = "";
#if OBJ_MANAGER_WITH_ID
	sprintf(tempStr,"%d",order);
	m_objInfo[order].m_fontID->setString(tempStr);
	XVector2 fontPosition = m_objInfo[order].m_position;
	if(fontPosition.x < 0.0f) fontPosition.x = 0.0f;	//��ֹ������ʾ������Ļ��Χ����������û���ж����ޣ�ֻ�ж�������
	if(fontPosition.x >= getSceneWidth() - m_objInfo[order].m_fontID->getMaxPixelWidth()) 
		fontPosition.x = getSceneWidth() - m_objInfo[order].m_fontID->getMaxPixelWidth();
	if(fontPosition.y < 0.0f) fontPosition.y = 0.0f;
	if(fontPosition.y >= getSceneHeight() - m_objInfo[order].m_fontID->getMaxPixelHeight()) 
		fontPosition.y = getSceneHeight() - m_objInfo[order].m_fontID->getMaxPixelHeight();
	m_objInfo[order].m_fontID->setPosition(fontPosition);
#endif
	if(m_objInfo[order].m_objLineOrder < 0) return;
	sprintf(tempStr,"%d",order);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,0);
	updateObjStateInfo(order);
	sprintf(tempStr,"%.0f,%.0f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,3);
	sprintf(tempStr,"%.2f,%.2f",m_objInfo[order].m_scale.x,m_objInfo[order].m_scale.y);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,4);
	sprintf(tempStr,"%.2f",m_objInfo[order].m_angle);
	m_mutiList->setBoxStr(tempStr,m_objInfo[order].m_objLineOrder,5);
}
XBool XObjectManager::checkNeedUpdate(int order)
{
	XObjectInfo &objInfo = m_objInfo[order];
	XObjectBasic * temp = objInfo.m_pObject;
	if(objInfo.m_position.x != temp->getPosition().x || objInfo.m_position.y != temp->getPosition().y) return XTrue;
	if(objInfo.m_scale.x != temp->getScale().x || objInfo.m_scale.y != temp->getScale().y) return XTrue;
	if(objInfo.m_angle != temp->getAngle()) return XTrue;
	if(objInfo.m_pObject->m_objType == OBJ_NODELINE && 
		objInfo.length != ((XNodeLine *)m_objInfo[order].m_pObject)->getNodeLineLength()) return XTrue;
	return XFalse;
}
void XObjectManager::release()
{
#if OBJ_MANAGER_WITH_ID
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		XMem::XDELETE(m_objInfo[i].m_fontID);
	}
#endif
	m_objInfo.clear();
}
void XObjectManager::draw()	//�����������̫��ʱ�����������ִ��Ч�ʽϵ�
{//��ʾ����ĵ�ǰ״̬
	if(!m_isInited ||
		!m_isOption) return;

	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objectMouseState != OBJ_STATE_NULL || m_objInfo[i].m_objBeSelect)
		{//��ѡ��״̬�������״̬��
			switch(m_objInfo[i].m_pObject->m_objType)
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					//�������λ�������ʮ��
					XRender::drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						0.5f,1.0f,0.0f,0.0f,1.0f);
					XRender::drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						0.5f,1.0f,0.0f,0.0f,1.0f);
					//ʹ��������Χ���
					if(m_objInfo[i].m_objBeSelect)
					{//����ѡ��״̬
						if(m_objInfo[i].m_objCanEdit)
						{//�ɱ༭Ϊ��ɫ
							if(m_mutiList->getHaveSelectState(m_objInfo[i].m_objLineOrder))
								XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,0.0f,0.0f,1.0f);	//��ѡ�е��������ɫ
							else XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}else
						{//���ɱ༭Ϊ��ɫ
							XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,0.0f,0.0f,0.0f);
						}
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_DOWN)
					{//��갴��״̬
						XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,1.0f,0.0f,0.0f);
					}else
					if(m_objInfo[i].m_objectMouseState == OBJ_STATE_MOUSE_ON)
					{//�������״̬
						XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1.0f,1.0f,1.0f,0.0f);
					}
#if OBJ_MANAGER_WITH_ID
					if(m_showObjID != 0) m_objInfo[i].m_fontID->draw();
#endif
				}
				break;
			case OBJ_NODELINE:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					XObjectBasic * temp = m_objInfo[i].m_pObject;
					//�������λ�������ʮ��
					XRender::drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						2,1.0f,0.0f,0.0f,1.0f);
					XRender::drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						2,1.0f,0.0f,0.0f,1.0f);
					if(m_objInfo[i].m_pObject->m_objType == OBJ_NODELINE)
					{//������Ҫ��ʾ�ڵ�������Ϣ
						XNodeLine * tempNodeLine = (XNodeLine *)m_objInfo[i].m_pObject;
						for(int j = 0;j < tempNodeLine->getNodeSum();++ j)
						{
							//��������
							if(j == 0)
							{//��һ��������ɫ
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4,0.0f,1.0f,0.0f);
							}else
							if(j == m_objInfo[i].m_objectEditParm)
							{//��ѡ�еĵ�ʹ�ð�ɫ
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4);
							}else
							{//�������ú�ɫ
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y + 2,
									4,1.0f,0.0f,0.0f);
							}
							if(j < tempNodeLine->getNodeSum() - 1)
							{
								XRender::drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y ,
									tempNodeLine->getNode(j + 1).x * tempNodeLine->getScale().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j + 1).y * tempNodeLine->getScale().y + tempNodeLine->getPosition().y);
							}
						}
						XRender::drawLine(tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y - 2,
							tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y + 2,
							4,1.0f,1.0f,0.0f);
					}
					if(m_objInfo[i].m_objBeSelect || m_objInfo[i].m_objectMouseState  == OBJ_STATE_MOUSE_MOVE)
					{//�༭״̬
						if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_EDIT)
						{
							XRender::drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}
#if OBJ_MANAGER_WITH_ID
						if(m_showObjID != 0) m_objInfo[i].m_fontID->draw();
#endif
					}
				}
				break;
			}
		}
	}
	if(m_mutiList->getVisible()) m_mousePosFont->draw();
}
void XObjectManager::move(float delay)
{
	if(!m_isInited ||
		!m_isOption) return;
	m_mousePosFont->setString((XString::intToStr((int)(getMousePos().x),10) + "," + XString::intToStr((int)(getMousePos().y),10)).c_str());
	if(m_keyState[0] == KEY_STATE_DOWN)
	{//�������
		m_keyTimer[0] -= delay;
		if(m_keyTimer[0] <= 0)
		{//����ʱ�䵽
			m_keyCurTime[0] = (m_keyCurTime[0] >> 1);
			if(m_keyCurTime[0] < m_keyMinTime) m_keyCurTime[0] = m_keyMinTime;
			m_keyTimer[0] = m_keyCurTime[0];
			//������Ӧһ�ΰ�������
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_SIZE:	//��С
					objectKeyOption(i,OBJ_OPTION_SIZE_DOWN);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_DOWN);
					break;
				case OBJ_OPTION_ROTATEMODE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_MODE_CHANGE);
					break;
				case OBJ_OPTION_TURNOVERMODE:	
					objectKeyOption(i,OBJ_OPTION_TURNOVER_MODE_CHANGE);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTIONX_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[1] == KEY_STATE_DOWN)
	{//�Ҽ�������
		m_keyTimer[1] -= delay;
		if(m_keyTimer[1] <= 0)
		{//����ʱ�䵽
			m_keyCurTime[1] = (m_keyCurTime[1] >> 1);
			if(m_keyCurTime[1] < m_keyMinTime) m_keyCurTime[1] = m_keyMinTime;
			m_keyTimer[1] = m_keyCurTime[1];
			//������Ӧһ�ΰ�������
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_SIZE:	//�Ŵ�
					objectKeyOption(i,OBJ_OPTION_SIZE_ON);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_ON);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTIONX_ON);
					break;
				}
			}
		}
	}
	if(m_keyState[2] == KEY_STATE_DOWN)
	{//�ϼ�������
		m_keyTimer[2] -= delay;
		if(m_keyTimer[2] <= 0)
		{//����ʱ�䵽
			m_keyCurTime[2] = (m_keyCurTime[2] >> 1);
			if(m_keyCurTime[2] < m_keyMinTime) m_keyCurTime[2] = m_keyMinTime;
			m_keyTimer[2] = m_keyCurTime[2];
			//������Ӧһ�ΰ�������
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
				switch(m_objInfo[i].m_objectKeyOption)
				{
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_Y_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[3] == KEY_STATE_DOWN)
	{//�¼�������
		m_keyTimer[3] -= delay;
		if(m_keyTimer[3] <= 0)
		{//����ʱ�䵽
			m_keyCurTime[3] = (m_keyCurTime[3] >> 1);
			if(m_keyCurTime[3] < m_keyMinTime) m_keyCurTime[3] = m_keyMinTime;
			m_keyTimer[3] = m_keyCurTime[3];
			//������Ӧһ�ΰ�������
			for(unsigned int i = 0;i < m_objInfo.size();++ i)
			{
			//	switch(m_objInfo[i].m_objectKeyOption)
			//	{
			//	case OBJ_OPTION_POSITION:	
			//		objectKeyOption(i,OBJ_OPTION_Y_ON);
			//		break;
			//	}
				if(m_objInfo[i].m_objectKeyOption == OBJ_OPTION_POSITION)
					objectKeyOption(i,OBJ_OPTION_Y_ON);
			}
		}
	}
}
XBool XObjectManager::init(XResourcePosition resoursePosition)
{
	if(m_isInited) return XTrue;
	//��ʼ������
	if(!m_font->initEx((getCommonResPos() + "ResourcePack/pic/ObjectManager/Font").c_str(),
		resoursePosition)) return XFalse;
	decreaseAObject(m_font);
	m_mousePosFont->setACopy(*m_font);
	m_mousePosFont->setColor(XFColor::blue);
	m_mousePosFont->setScale(0.5f);
	decreaseAObject(m_mousePosFont);
//#if OBJ_MANAGER_WITH_ID
//	for(int i = 0;i < MAX_OBJECT_SUM;++ i)
//	{
//		m_objInfo[i].m_fontID.setACopy(m_font);
//		decreaseAObject(&(m_objInfo[i].m_fontID));
//		m_objInfo[i].m_fontID.setMaxStrLen(5);
//	}
//#endif

	//��ʼ��һ���б��
	//if(!m_sliderTextureH.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderH",resoursePosition)) return 0;
	//if(!m_sliderH.initEx(XVector2(200.0f,200.0f),m_sliderTextureH,SLIDER_TYPE_HORIZONTAL,100.0f,0.0f)) return 0;
	//XCtrlManager.decreaseAObject(&m_sliderH);
	//decreaseAObject(&m_sliderH);

	//if(!m_sliderTextureV.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderV",resoursePosition)) return 0;
	//if(!m_sliderV.initEx(XVector2(200.0f,200.0f),m_sliderTextureV,SLIDER_TYPE_VERTICAL,100.0f,0.0f)) return 0;
	//XCtrlManager.decreaseAObject(&m_sliderV);
	//decreaseAObject(&m_sliderV);

	//if(!m_checkTex0.initEx("ResourcePack/pic/ObjectManager/MutiList/checkC",resoursePosition)) return 0;
	//if(!m_checkTex1.initEx("ResourcePack/pic/ObjectManager/MutiList/checkE",resoursePosition)) return 0;
	//if(!m_mutiListTexture.initEx("ResourcePack/pic/ObjectManager/MutiList",resoursePosition)) return 0;

	//if(!m_mutiList->initEx(XVector2(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY),m_mutiListTexture,m_checkTex0,m_checkTex1,m_font,1.0f,5,0,m_sliderV,m_sliderH)) return 0;
	if(!m_mutiList->initPlus((getCommonResPos() + "ResourcePack/pic/ObjectManager/MutiList").c_str(),
		*m_font,1.0f,5,0,resoursePosition)) return XFalse;
	//if(!m_mutiList->initWithoutSkin(XRect(64,0,500,500),*m_font,1.0f,5,0)) return 0;
	//m_mutiList->setPosition(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY);
	setEditWidowsPos();
	decreaseAObject(m_mutiList);
	m_mutiList->setScale(0.5f,0.5f);
	m_mutiList->setTitleStr("ID",0);
	m_mutiList->setRowWidth(80,0);
	m_mutiList->setTitleStr("Type",1);
	m_mutiList->setRowWidth(100,1);
	m_mutiList->setTitleStr("State",2);
	m_mutiList->setRowWidth(100,2);
	m_mutiList->setTitleStr("Position",3);
	m_mutiList->setRowWidth(120,3);
	m_mutiList->setTitleStr("Size",4);
	m_mutiList->setRowWidth(150,4);
	m_mutiList->setTitleStr("Angle",5);
	m_mutiList->setRowWidth(150,5);

	m_mutiList->setEventProc(ctrlProc,this);

	m_ctrlKeyState = KEY_STATE_UP;
	m_isInited = XTrue;
	return XTrue;
}
XObjectManager::~XObjectManager()
{
	XMem::XDELETE(m_font);
	XMem::XDELETE(m_mousePosFont);
	XMem::XDELETE(m_mutiList);

	//printf("����������Ѿ�����!\n");
	if(!m_isInited) return;
	release();
	m_isInited = XFalse;
}
void XObjectManager::getCurChooseObjectID(int *ID) const
{
	if(!m_isInited ||
		ID == NULL) return;
	int ret = 0;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_objBeSelect) 
		{
			ID[ret] = i;
			++ ret;
		}
	}
}
int XObjectManager::findObjectID(const void * object) const
{
//	if(!m_isInited) return -1;
	for(unsigned int i = 0;i < m_objInfo.size();++ i)
	{
		if(m_objInfo[i].m_pObject == object) return i;
	}
	return -1;
}
void XObjectManager::setEditWidowsPos()					//	2	3
{
	if(!m_isShowUI) return;
	switch(m_editWindowsPos)
	{
	case 0:
		if(XEG.m_windowData.withFrame == 0 && XEG.m_windowData.withCustomTitle)
			m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY() + MIN_FONT_CTRL_SIZE);
		else
			m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY());
		break;
	case 1:
		if(XEG.m_windowData.withFrame == 0 && XEG.m_windowData.withCustomTitle)
			m_mutiList->setPosition(XEG.getScenePosX(), XEG.getScenePosY() + MIN_FONT_CTRL_SIZE);
		else
			m_mutiList->setPosition(XEG.getScenePosX(),XEG.getScenePosY());
		break;
	case 2:m_mutiList->setPosition(XEG.getScenePosX(),XEG.getScenePosY() + XEG.getSceneH() - 257.0f);break;
	case 3:m_mutiList->setPosition(XEG.getScenePosX() + XEG.getSceneW() - 289.0f,XEG.getScenePosY() + XEG.getSceneH() - 257.0f);break;
	}
	m_mousePosFont->setPosition(m_mutiList->getPosition() + XVector2(0.0f,240.0f));
}
#if !WITH_INLINE_FILE
#include "XObjectManager.inl"
#endif
}