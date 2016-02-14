#include "XStdHead.h"
#include "XActionSimple.h"
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XResourcePack.h"
#include <stdarg.h>
namespace XE{
void XActionSimpleEx::setStart()			//���ö�����ʼ����
{
	if(m_actionState != ACTION_STATE_END) return;
	m_actionState = ACTION_STATE_ACTION;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funResetData != NULL) m_funResetData(m_pClass);	
		return;
	case ACTION_TYPE_COMPLEX://��ʼ���ŵ�һ������
		if(m_pAction[0] != NULL) m_pAction[0]->setStart();
		m_curActionIndex = 0;
		return;
	}
}
void XActionSimpleEx::move(float stepTime)
{
	if(!m_isInited || 
		m_actionState != ACTION_STATE_ACTION) return;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funMove != NULL) m_funMove(stepTime,m_pClass);	
		if(m_funIsEnd(m_pClass)) 
		{
			m_actionState = ACTION_STATE_END;	//��Ƕ�������
			if(m_funAtEnd != NULL) m_funAtEnd(m_pClass);
		}
		if(m_actionIsLoop && 
			m_actionState == ACTION_STATE_END) setStart();
		return;
	case ACTION_TYPE_COMPLEX:
		if(!m_pAction[m_curActionIndex]->getIsEnd()) return;
		++ m_curActionIndex;
		if(m_curActionIndex >= m_actionSum)
		{//����
			m_actionState = ACTION_STATE_END;
			if(m_actionIsLoop && 
				m_actionState == ACTION_STATE_END) setStart();
		}else
		{//������һ������
			m_pAction[m_curActionIndex]->setStart();
		}
		return;
	}
}
void XActionSimpleEx::draw()
{
	if(!m_isInited) return;
	if(m_actionState != ACTION_STATE_ACTION) return;	//��������д�����
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		if(m_funDraw != NULL) m_funDraw(m_pClass);
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_curActionIndex]->draw();
		return;
	}
}
XBool XActionSimpleEx::init(XResourcePosition resoursePosition,
	const char *actionName,
	XBool (*funIsEnd)(void *),
	void (*funMove)(float,void *),		
	void (*funDraw)(void *),		
	void (*funResetData)(void *),
	void (*funAtEnd)(void *),
	int (*funInit)(int,void *),
	void (*funRelease)(void *),
	void * pClass)
{
	if(m_isInited) return XTrue;
	if(funIsEnd == NULL) return XFalse;
	m_funIsEnd = funIsEnd;
	m_funMove = funMove;
	m_funDraw = funDraw;
	m_funResetData = funResetData;
	m_funAtEnd = funAtEnd;
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_pClass = pClass;

	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if(m_funInit(resoursePosition,m_pClass) == 0) return XFalse;

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = "";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = XTrue;
	return XTrue;
}
XBool XActionSimpleEx::initComplex(XResourcePosition resoursePosition,const char *actionName,int actionSum,...)	//���϶����ĳ�ʼ��
{
	if(m_isInited) return XTrue;
	m_funIsEnd = NULL;
	m_funMove = NULL;
	m_funDraw = NULL;
	m_funResetData = NULL;
	m_funAtEnd = NULL;
	m_pClass = NULL;
	m_resoursePosition = resoursePosition;

	if(m_funInit != NULL) if(m_funInit(resoursePosition,m_pClass) == 0) return XFalse;

	XMem::XDELETE(m_pAction);
	m_actionSum = actionSum;
	if(m_actionSum <= 0) return XFalse;
	m_pAction = XMem::createArrayMem<XActionSimpleEx *>(m_actionSum);
	if(m_pAction == NULL) return XFalse;
	//����������еĶ���
	va_list arg_ptr;
	va_start(arg_ptr,actionSum);
	for(int i = 0;i < m_actionSum;++ i)
	{
		//m_pAction[i] = XASManager.getPAction(va_arg(arg_ptr,const char *));
		m_pAction[i] = va_arg(arg_ptr,XActionSimpleEx *);
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = "";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = XTrue;
	return XTrue;
}
//�����ǹ����µĶ����������Ľӿ�
bool XActionSimpleManager::pushAction(XActionSimpleEx *action)	//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����
{
	if(action == NULL) return false;
	int ret = getActionIndex(action);
	if(ret != -1) return false;	//��ֹ�ظ�ע��
	m_pActions.push_back(action);

	return true;
}
int XActionSimpleManager::getActionIndex(const char *actionName)
{
	if(actionName == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL &&
			m_pActions[i]->isName(actionName)) 
			return i;
	}
	return -1;	//û���ҵ��������
}
int XActionSimpleManager::getActionIndex(const XActionSimpleEx *action)	//ͨ�����ֻ�ȡ�������
{
	if(action == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
			return i;
	}
	return -1;	//û���ҵ��������
}
XBool XActionSimpleManager::popAction(const char *actionName)
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL
			&& m_pActions[i]->isName(actionName)) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
XBool XActionSimpleManager::popAction(XActionSimpleEx *action)		//�ӱ���ɾ��һ������
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
XBool XActionSimpleManager::releaseAction(const char *actionName)	//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] != NULL
			&& m_pActions[i]->isName(actionName)) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
XBool XActionSimpleManager::releaseAction(XActionSimpleEx *action)
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i] == action) 
		{
			m_pActions.erase(m_pActions.begin() + i);
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
#if !WITH_INLINE_FILE
#include "XActionSimple.inl"
#endif
}