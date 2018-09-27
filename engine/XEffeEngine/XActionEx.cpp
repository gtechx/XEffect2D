#include "XStdHead.h"
#include "XActionEx.h"
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2015.5.20
//--------------------------------
#include "XResourcePack.h"
#include <stdarg.h>
namespace XE {
void XActionEx::setStart()			//���ö�����ʼ����
{
	if (m_actionState != ACTION_STATE_END) return;
	m_actionState = ACTION_STATE_ACTION;
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funResetData();
		return;
	case ACTION_TYPE_COMPLEX://��ʼ���ŵ�һ������
		if (m_pAction[0] != NULL) m_pAction[0]->setStart();
		m_curActionIndex = 0;
		return;
	}
}
void XActionEx::move(float stepTime)
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funMove(stepTime);
		if (funIsEnd())
		{
			m_actionState = ACTION_STATE_END;	//��Ƕ�������
			funAtEnd();
		}
		if (m_actionIsLoop &&
			m_actionState == ACTION_STATE_END) setStart();
		return;
	case ACTION_TYPE_COMPLEX:
		if (!m_pAction[m_curActionIndex]->getIsEnd()) return;
		++m_curActionIndex;
		if (m_curActionIndex >= m_actionSum)
		{//����
			m_actionState = ACTION_STATE_END;
			if (m_actionIsLoop && m_actionState == ACTION_STATE_END)
			{
				setStart();
			}
		}
		else
		{//������һ������
			m_pAction[m_curActionIndex]->setStart();
		}
		return;
	}
}
void XActionEx::draw()
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;	//��������д�����
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funDraw();
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_curActionIndex]->draw();
		return;
	}
}
void XActionEx::keepDraw()
{
	if (!m_isInited) return;	//��������д�����
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funKeepDraw();
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		if ((m_curActionIndex >= m_actionSum || m_curActionIndex < 0) && m_actionSum >= 1)
			m_pAction[m_actionSum - 1]->keepDraw();
		else
			m_pAction[m_curActionIndex]->keepDraw();
		return;
	}
}
void XActionEx::input(const XInputEvent &inputEvent)
{
	if (!m_isInited ||
		m_actionState != ACTION_STATE_ACTION) return;	//��������д�����
	switch (m_type)
	{
	case ACTION_TYPE_ATOMIC:
		funInput(inputEvent);
		return;
	case ACTION_TYPE_COMPLEX://���Ա����������������ʾ
		m_pAction[m_curActionIndex]->input(inputEvent);
		return;
	}
}
XBool XActionEx::init(XResPos resPos,
	const char *actionName)
{
	if (m_isInited) return XFalse;
	m_resoursePosition = resPos;

	if (!funInit(resPos)) return XFalse;

	m_actionState = ACTION_STATE_END;
	if (actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_ATOMIC;
	m_isInited = XTrue;
	return XTrue;
}
XBool XActionEx::init(XResPos resPos, const char *actionName, int actionSum, ...)	//���϶����ĳ�ʼ��
{
	if (m_isInited) return XTrue;
	m_resoursePosition = resPos;

	if (!funInit(resPos)) return XFalse;

	XMem::XDELETE(m_pAction);
	m_actionSum = actionSum;
	if (m_actionSum <= 0) return XFalse;
	m_pAction = XMem::createArrayMem<XActionEx *>(m_actionSum);
	if (m_pAction == NULL) return XFalse;
	//����������еĶ���
	va_list arg_ptr;
	va_start(arg_ptr, actionSum);
	for (int i = 0; i < m_actionSum; ++i)
	{
		m_pAction[i] = XAExManager.getPAction(va_arg(arg_ptr, const char *));
	}
	va_end(arg_ptr);

	m_actionState = ACTION_STATE_END;
	if (actionName == NULL) m_actionName = " ";
	else m_actionName = actionName;

	m_type = ACTION_TYPE_COMPLEX;
	m_isInited = XTrue;
	return XTrue;
}
//�����ǹ����µĶ����������Ľӿ�
XActionExHandle XActionExManager::pushAction(XActionEx *action)	//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����
{
	if (action == NULL) return 0;
	int ret = getActionHandle(action);
	if (ret != -1) return ret;	//��ֹ�ظ�ע��
	ret = getActionHandle(action->getName().c_str());//����Ҫ��ֹ����
	//�ԭ�еĿ�ȱ
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (!m_pActions[i].actionIsEnable)
		{
			m_pActions[i].actionHandle = i;
			m_pActions[i].actionIsEnable = XTrue;
			m_pActions[i].pAction = action;
			return m_pActions[i].actionHandle;
		}
	}
	//���û�п�ȱ����������µ�
	XActionExData tmp;
	tmp.actionHandle = m_pActions.size();
	m_actionOrder.push_back(m_pActions.size());
	m_actionOrderX.push_back(m_pActions.size());
	tmp.actionIsEnable = XTrue;
	tmp.pAction = action;
	m_pActions.push_back(tmp);
	return tmp.actionHandle;
}
XActionExHandle XActionExManager::getActionHandle(const char *actionName)
{
	if (actionName == NULL) return -1;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
			&& m_pActions[i].pAction->isName(actionName))
			return m_pActions[i].actionHandle;
	}
	return -1;	//û���ҵ��������
}
XActionExHandle XActionExManager::getActionHandle(const XActionEx *action)	//ͨ�����ֻ�ȡ�������
{
	if (action == NULL) return -1;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
			return m_pActions[i].actionHandle;
	}
	return -1;	//û���ҵ��������
}
XBool XActionExManager::popAction(const char *actionName)
{
	if (actionName == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
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
XBool XActionExManager::popAction(XActionEx *action)		//�ӱ���ɾ��һ������
{
	if (action == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
		{
			m_pActions[i].actionHandle = 0;
			m_pActions[i].actionIsEnable = XFalse;
			m_pActions[i].pAction = NULL;
			return XTrue;
		}
	}
	return XFalse;	//û���ҵ��������
}
XBool XActionExManager::releaseAction(const char *actionName)	//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
{
	if (actionName == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction != NULL
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
XBool XActionExManager::releaseAction(XActionEx *action)
{
	if (action == NULL) return XFalse;
	for (unsigned int i = 0; i < m_pActions.size(); ++i)
	{
		if (m_pActions[i].actionIsEnable && m_pActions[i].pAction == action)
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
XBool XActionExManager::moveToBottom(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] < m_actionOrder[actionHandle])
		{
			++m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = 0;
	m_actionOrderX[0] = actionHandle;
	return XTrue;
}
XBool XActionExManager::moveToTop(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] > m_actionOrder[actionHandle])
		{
			--m_actionOrder[i];
			m_actionOrderX[m_actionOrder[i]] = i;
		}
	}
	m_actionOrder[actionHandle] = (int)(m_actionOrder.size()) - 1;
	m_actionOrderX[(int)(m_actionOrder.size()) - 1] = actionHandle;

	return XTrue;
}
XBool XActionExManager::moveUp(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] == m_actionOrder[actionHandle] + 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			++m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
XBool XActionExManager::moveDown(XActionExHandle actionHandle)
{
	if (actionHandle < 0 || actionHandle >= m_pActions.size()) return XFalse;
	for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
	{
		if (m_actionOrder[i] == m_actionOrder[actionHandle] - 1)
		{
			m_actionOrder[i] = m_actionOrder[actionHandle];
			--m_actionOrder[actionHandle];

			m_actionOrderX[m_actionOrder[i]] = i;
			m_actionOrderX[m_actionOrder[actionHandle]] = actionHandle;
			return XTrue;
		}
	}
	return XFalse;
}
XBool XActionExManager::moveUpTo(XActionExHandle s, XActionExHandle d)
{
	if (s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if (m_actionOrder[s] < m_actionOrder[d])
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] <= m_actionOrder[d])
			{
				--m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	else
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] + 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}
XBool XActionExManager::moveDownTo(XActionExHandle s, XActionExHandle d)
{
	if (s < 0 || s >= m_pActions.size() ||
		d < 0 || d >= m_pActions.size() || s == d) return XFalse;
	if (m_actionOrder[s] < m_actionOrder[d])
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] > m_actionOrder[s] && m_actionOrder[i] < m_actionOrder[d])
			{
				--m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	else
	{
		for (unsigned int i = 0; i < m_actionOrder.size(); ++i)
		{
			if (m_actionOrder[i] >= m_actionOrder[d] && m_actionOrder[i] < m_actionOrder[s])
			{
				++m_actionOrder[i];
				m_actionOrderX[m_actionOrder[i]] = i;
			}
		}
		m_actionOrder[s] = m_actionOrder[d] - 1;
		m_actionOrderX[m_actionOrder[s]] = s;
	}
	return XFalse;
}
#if !WITH_INLINE_FILE
#include "XActionEx.inl"
#endif
}