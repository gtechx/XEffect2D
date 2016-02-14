#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XPressDataMode.h"
namespace XE{
void XPressDataSingle::init(float normalValue,float maxDeformValue,int sensitivity)
{
	m_normalData = normalValue;
	m_maxDeformData = maxDeformValue;
	m_curData = normalValue;
	if(m_normalData <= m_maxDeformData) m_dataFlag = 1.0f;
	else m_dataFlag = -1.0f;
	m_pressState = PRESS_STATE_NORMAL;
	m_sensitivity = sensitivity;
	if(m_sensitivity < 0) m_sensitivity = 0;
	m_curPressTimer = 0;

	//m_isRecover = XTrue;

	m_curDeformData = m_minChangeData;
}
void XPressDataSingle::setPress()
{
	m_pressState = PRESS_STATE_PRESS;
	m_curPressTimer = 0;
	if(m_maxDeformData > m_normalData)
	{
		if(m_curData < m_maxDeformData)
		{
			m_curData += m_dataFlag * m_startChangeData;
			if(m_curData > m_maxDeformData) m_curData = m_maxDeformData;
		}
	}else
	{
		if(m_curData > m_maxDeformData)
		{
			m_curData += m_dataFlag * m_startChangeData;
			if(m_curData < m_maxDeformData) m_curData = m_maxDeformData;
		}
	}
}
void XPressDataSingle::move(float stepTime)
{
	if(m_pressState == PRESS_STATE_PRESS)
	{//���ڰ���״̬
		if(m_curPressTimer < m_sensitivity)
		{//���Ҳ�Ҫ������Ӧ
			m_curPressTimer += stepTime;
			if(m_curPressTimer > m_sensitivity) m_curPressTimer = m_sensitivity;
		}else
		{//��ʼ������Ӧ
			if(m_curDeformData < m_maxChangeData)
			{
				m_curDeformData += 0.01f * stepTime;
				if(m_curDeformData > m_maxChangeData) m_curDeformData = m_maxChangeData;
			}
			if(m_maxDeformData > m_normalData)
			{
				if(m_curData < m_maxDeformData)
				{
					m_curData += m_dataFlag * m_curDeformData;
					if(m_curData > m_maxDeformData) m_curData = m_maxDeformData;
				}
			}else
			{
				if(m_curData > m_maxDeformData)
				{
					m_curData += m_dataFlag * m_curDeformData;
					if(m_curData < m_maxDeformData) m_curData = m_maxDeformData;
				}
			}
		}
	}else
	{//���ڵ���״̬
		//׼���ָ�
		if(m_curDeformData > m_minChangeData)
		{
			m_curDeformData -= 0.01f * stepTime;
			if(m_curDeformData < m_minChangeData) m_minChangeData = m_minChangeData;
		}
		//��ǰ��ֵ�ָ�
		if(abs(m_normalData - m_curData) < 1.0f)
		{
			m_curData = m_normalData;
		}else
		{
			m_curData = m_curData + (m_normalData - m_curData) * 0.01f * stepTime;
		}
	}
}
void XPressDataDouble::init(float normalValue,float maxDeformValue,
							float minDeformValue,int sensitivity,XPressDataRecoverMode recoverMode)
{
	m_normalData = normalValue;
	if(maxDeformValue > minDeformValue)
	{
		m_maxDeformData = maxDeformValue;
		m_minDeformData = minDeformValue;
	}else
	{
		m_maxDeformData = minDeformValue;
		m_minDeformData = maxDeformValue;
	}
	m_curData = normalValue;

	m_pressMinState = PRESS_STATE_NORMAL;
	m_pressMaxState = PRESS_STATE_NORMAL;
	m_sensitivity = sensitivity;
	if(m_sensitivity < 0) m_sensitivity = 0;
	m_curPressTimer = 0;

	m_isRecover = XTrue;
	m_recoverMode = recoverMode;

	m_curDeformData = m_minChangeData;
}
void XPressDataDouble::setPressMax()
{
	if(m_pressMinState == PRESS_STATE_NORMAL && 
		m_pressMaxState == PRESS_STATE_NORMAL) 
	{
		m_curPressTimer = 0;
		if(m_curData < m_maxDeformData)
		{
			m_curData += m_startChangeData;
			if(m_curData > m_maxDeformData) m_curData = m_maxDeformData;
			m_isRecover = XFalse;
		}
	}
	m_pressMaxState = PRESS_STATE_PRESS;
}
void XPressDataDouble::setPressMin()
{
	if(m_pressMinState == PRESS_STATE_NORMAL && 
		m_pressMaxState == PRESS_STATE_NORMAL) 
	{
		m_curPressTimer = 0;
		if(m_curData > m_minDeformData)
		{
			m_curData -= m_startChangeData;
			if(m_curData < m_minDeformData) m_curData = m_minDeformData;
			m_isRecover = XFalse;
		}
	}
	m_pressMinState = PRESS_STATE_PRESS;
}
void XPressDataDouble::move(float stepTime)
{
	if(m_pressMaxState == PRESS_STATE_PRESS && 
		m_pressMinState == PRESS_STATE_PRESS)	return;//ͬʱ���²�����Ӧ
	if(m_pressMaxState == PRESS_STATE_PRESS)
	{//��������
		if(m_curPressTimer < m_sensitivity)
		{//���Ҳ�Ҫ������Ӧ
			m_curPressTimer += stepTime;
			if(m_curPressTimer > m_sensitivity) m_curPressTimer = m_sensitivity;
		}else
		{//��ʼ������Ӧ
			if(m_curDeformData < m_maxChangeData)
			{
				m_curDeformData += 0.01f * stepTime;
				if(m_curDeformData > m_maxChangeData) m_curDeformData = m_maxChangeData;
			}
			if(m_curData < m_maxDeformData)
			{
				m_curData += m_curDeformData;
				if(m_curData > m_maxDeformData) m_curData = m_maxDeformData;
				m_isRecover = XFalse;	//�߼�����һ�п��Բ���
			}
		}
	}else
	if(m_pressMinState == PRESS_STATE_PRESS)
	{//���¼�С
		if(m_curPressTimer < m_sensitivity)
		{//���Ҳ�Ҫ������Ӧ
			m_curPressTimer += stepTime;
			if(m_curPressTimer > m_sensitivity) m_curPressTimer = m_sensitivity;
		}else
		{//��ʼ������Ӧ
			if(m_curDeformData < m_maxChangeData)
			{
				m_curDeformData += 0.01f * stepTime;
				if(m_curDeformData > m_maxChangeData) m_curDeformData = m_maxChangeData;
			}
			if(m_curData > m_minDeformData)
			{
				m_curData -= m_curDeformData;
				if(m_curData < m_minDeformData) m_curData = m_minDeformData;
				m_isRecover = XFalse;	//�߼�����һ�п��Բ���
			}
		}
	}else
	{//׼���ָ�
		//׼���ָ�
		if(m_curDeformData > m_minChangeData)
		{
			m_curDeformData -= 0.01f * stepTime;
			if(m_curDeformData < m_minChangeData) m_minChangeData = m_minChangeData;
		}
		if(m_recoverMode == MODE_SLOW_RECOVER && !m_isRecover)
		{//��ǰ��ֵ�����ָ�
			if(abs(m_normalData - m_curData) < 1.0f)
			{
				m_curData = m_normalData;
				m_isRecover = XTrue;
			}else
			{
				m_curData = m_curData + (m_normalData - m_curData) * 0.01f * stepTime;
			}
		}else
		if(m_recoverMode == MODE_IMM_RECOVER && !m_isRecover)
		{//�����ظ�
			m_curData = m_normalData;
			m_isRecover = XTrue;
		}
	}
}
}