#include "XActionEx.h"
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2015.5.20
//--------------------------------
#include "XResourcePack.h"

void _XActionEx::setStart()			//���ö�����ʼ����
{
	if(m_actionState != ACTION_STATE_END) return;
	m_actionState = ACTION_STATE_ACTION;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funResetData();	
		return;
	case ACTION_TYPE_COMPLEX://��ʼ���ŵ�һ������
		if(m_pAction[0] != NULL) m_pAction[0]->setStart();
		m_nowActionIndex = 0;
		return;
	}
}
void _XActionEx::move(int stepTime)
{
	if(!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funMove(stepTime);	
		if(funIsEnd()) 
		{
			m_actionState = ACTION_STATE_END;	//��Ƕ�������
			funAtEnd();
		}
		if(m_actionIsLoop && 
			m_actionState == ACTION_STATE_END) setStart();
		return;
	case ACTION_TYPE_COMPLEX:
		if(!m_pAction[m_nowActionIndex]->getIsEnd()) return;
		++ m_nowActionIndex;
		if(m_nowActionIndex >= m_actionSum)
		{//����
			m_actionState = ACTION_STATE_END;
			if(m_actionIsLoop && m_actionState == ACTION_STATE_END)
			{
				setStart();
			}
		}else
		{//������һ������
			m_pAction[m_nowActionIndex]->setStart();
		}
		return;
	}
}
void _XActionEx::draw()
{
	if(!m_isInited || 
		m_actionState != ACTION_STATE_ACTION) return;	//��������д�����
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funDraw();
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_nowActionIndex]->draw();
		return;
	}
}
void _XActionEx::keepDraw()
{
	if(!m_isInited) return;	//��������д�����
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funKeepDraw();
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_nowActionIndex]->keepDraw();
		return;
	}
}
void _XActionEx::input(const _XInputEvent &inputEvent)
{
	if(!m_isInited || 
		m_actionState != ACTION_STATE_ACTION) return;	//��������д�����
	switch(m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funInput(inputEvent);
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_nowActionIndex]->input(inputEvent);
		return;
	}
}
_XBool _XActionEx::init(_XResourcePosition resoursePosition,
	const char *actionName)
{
	if(m_isInited) return XFalse;
	m_resoursePosition = resoursePosition;

	if(!funInit(resoursePosition)) return XFalse;

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = XTrue;
	return XTrue;
}

#include <stdarg.h>

_XBool _XActionEx::init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...)	//���϶����ĳ�ʼ��
{
	if(m_isInited) return XTrue;
	m_resoursePosition = resoursePosition;

	if(!funInit(resoursePosition)) return XFalse;

	XDELETE(m_pAction);
	m_actionSum = actionSum;
	if(m_actionSum <= 0) return XFalse;
	m_pAction = createArrayMem<_XActionEx *>(m_actionSum);
	if(m_pAction == NULL) return XFalse;
	//����������еĶ���
	va_list arg_ptr;
	va_start(arg_ptr,actionSum);
	for(int i = 0;i < m_actionSum;++ i)
	{
		m_pAction[i] = _XAExManager.getPAction(va_arg(arg_ptr,const char *));
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if(actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = XTrue;
	return XTrue;
}

//�����ǹ����µĶ����������Ľӿ�
_XActionExHandle _XActionExManager::pushAction(_XActionEx *action)	//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����
{
	if(action == NULL) return 0;
	int ret = getActionHandle(action);
	if(ret != -1) return ret;	//��ֹ�ظ�ע��
	ret = getActionHandle(action->getName().c_str());//����Ҫ��ֹ����
	//�ԭ�еĿ�ȱ
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(!m_pActions[i].actionIsEnable)
		{
			m_pActions[i].actionHandle = i;
			m_pActions[i].actionIsEnable = XTrue;
			m_pActions[i].pAction = action;
			return m_pActions[i].actionHandle;
		}
	}
	//���û�п�ȱ����������µ�
	_XActionExData tmp;
	tmp.actionHandle = m_pActions.size();
	m_actionOrder.push_back(m_pActions.size());
	m_actionOrderX.push_back(m_pActions.size());
	tmp.actionIsEnable = XTrue;
	tmp.pAction = action;
	m_pActions.push_back(tmp);
	return tmp.actionHandle;
}
_XActionExHandle _XActionExManager::getActionHandle(const char *actionName)
{
	if(actionName == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName))
				return m_pActions[i].actionHandle;
	}
	return -1;	//û���ҵ��������
}
_XActionExHandle _XActionExManager::getActionHandle(const _XActionEx *action)	//ͨ�����ֻ�ȡ�������
{
	if(action == NULL) return -1;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction == action) 
			return m_pActions[i].actionHandle;
	}
	return -1;	//û���ҵ��������
}
_XBool _XActionExManager::popAction(const char *actionName)
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName)) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
_XBool _XActionExManager::popAction(_XActionEx *action)		//�ӱ���ɾ��һ������
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction == action) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
_XBool _XActionExManager::releaseAction(const char *actionName)	//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
{
	if(actionName == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName)) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
_XBool _XActionExManager::releaseAction(_XActionEx *action)
{
	if(action == NULL) return XFalse;
	for(int i = 0;i < m_pActions.size();++ i)
	{
		if(m_pActions[i].actionIsEnable && m_pActions[i].pAction == action) 
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction->release();
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
//����ĺ���ʹ������ӳ�䣬Ч�ʲ��ߣ�Ҫ�����Ż�
_XBool _XActionExManager::moveToBottom(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for(int i = 0;i < m_actionOrder.size();++ i)
	{
		if(m_actionOrder[i] < m_actionOrder[actionHandle])
		{
			++ m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = 0;
	m_actionOrderX[0] = actionHandle;
	return XTrue;
}
_XBool _XActionExManager::moveToTop(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for(int i = 0;i < m_actionOrder.size();++ i)
	{
		if(m_actionOrder[i] > m_actionOrder[actionHandle])
		{
			-- m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = m_actionOrder.size() - 1;
	m_actionOrderX[m_actionOrder.size() - 1] = actionHandle;

	return XTrue;
}
_XBool _XActionExManager::moveUp(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for(int i = 0;i < m_actionOrder.size();++ i)
	{
		if(m_actionOrder[i] == m_actionOrder[actionHandle] + 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			++ m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
_XBool _XActionExManager::moveDown(_XActionExHandle actionHandle)
{
	if(actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for(int i = 0;i < m_actionOrder.size();++ i)
	{
		if(m_actionOrder[i] == m_actionOrder[actionHandle] - 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			-- m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
_XBool _XActionExManager::moveUpTo(_XActionExHandle s,_XActionExHandle d)
{
	if(s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if(m_actionOrder[s] < m_actionOrder[d])
	{
		for(int i = 0;i < m_actionOrder.size();++ i)
		{
			if(m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] <= m_actionOrder[d])
			{
				-- m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}else
	{
		for(int i = 0;i < m_actionOrder.size();++ i)
		{
			if(m_actionOrder[i] > m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++ m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}
_XBool _XActionExManager::moveDownTo(_XActionExHandle s,_XActionExHandle d)
{
	if(s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if(m_actionOrder[s] < m_actionOrder[d])
	{
		for(int i = 0;i < m_actionOrder.size();++ i)
		{
			if(m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] < m_actionOrder[d])
			{
				-- m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}else
	{
		for(int i = 0;i < m_actionOrder.size();++ i)
		{
			if(m_actionOrder[i] >= m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++ m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}