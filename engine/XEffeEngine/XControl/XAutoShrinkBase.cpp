#include "XStdHead.h"
#include "XAutoShrinkBase.h"
namespace XE{
void XAutoShrinkBase::updateShrinkState(float stepTime)	//����������״̬
{
	if(!m_withAutoShrink) return;
	if(getIsInShrinkRect())
	{
		if(!m_oldIsInRect && m_isInShrink) m_shrinkTimer = 0.0f;
		m_oldIsInRect = true;
	}else
	{
		if(m_oldIsInRect && !m_isInShrink) m_shrinkTimer = 0.0f;
		m_oldIsInRect = false;
	}
	if(m_isInShrink)
	{//�ж��Ƿ�Ҫ��
		if(m_oldIsInRect)
		{
			m_shrinkTimer += stepTime;
			if(m_shrinkTimer >= 200.0f)
			{//���Ŵ򿪶���
				disShrink();
			}
		}
	}else
	{//�ж��Ƿ�Ҫ����
		XShrinkDirection tmp = calShrinkDir();
		if(tmp != DIR_NULL && !m_oldIsInRect)
		{//�з����������
			m_shrinkTimer += stepTime;
			if(m_shrinkTimer >= 200.0f)
			{//������������
				startShrink(tmp);
			}
		}
	}
	if(!m_shrinkMD.getIsEnd())
	{
		m_shrinkMD.move(stepTime);
		m_shrinkRate = m_shrinkMD.getCurData();
		updateShrinkPosition();
	}
}
}