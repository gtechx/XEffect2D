//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectBasic.h"

string objectTypeStr[] = {
	"OBJ_NULL",			//��Ч�����
	"OBJ_SPRITE",			//��������
	"OBJ_FRAME",			//����֡�����
	"OBJ_FRAMEEX",		//����֡�����
	"OBJ_NUMBER",			//�����ַ��������
	"OBJ_FONTUNICODE",	//��ʾ�ַ������
	"OBJ_FONTX",			//��ʾ�ַ������
	"OBJ_NODELINE",		//�ڵ����ߵ����
	"OBJ_CONTROL",		//�ؼ������
};
void _XObjectBasic::updateChildPos()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getSize());
	}
	if(m_parent != NULL)	//��������ߵ���
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//Ѱ�ҵ��Լ��ı��
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativePos[i] = (getPosition() - m_parent->getPosition()) / m_parent->getSize();
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildSize()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setPosition(getPosition() + m_childRelativePos[i] * getSize());
		m_childList[i]->setSize(getSize() * m_childRelativeSize[i]);
	}
	if(m_parent != NULL)
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//Ѱ�ҵ��Լ��ı��
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativeSize[i] = getSize() / m_parent->getSize();
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildAngle()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAngle(getAngle() + m_childRelativeAngle[i]);
	}
	if(m_parent != NULL)
	{
		for(int i = 0;i < m_parent->m_childList.size();++ i)
		{//Ѱ�ҵ��Լ��ı��
			if(m_parent->m_childList[i] == this)
			{
				m_parent->m_childRelativeAngle[i] = getAngle() - m_parent->getAngle();;
				break;
			}
		}
	}
}
void _XObjectBasic::updateChildAlpha()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setAlpha(getColor().fA);
	}
}
void _XObjectBasic::updateChildColor()
{
	for(int i = 0;i < m_childList.size();++ i)
	{
		m_childList[i]->setColor(getColor().fR,getColor().fG,getColor().fB,getColor().fA);
	}
}
void _XObjectBasic::updateChildVisible()
{
	if(getVisible())
	{
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->setVisible();
		}
	}else
	{
		for(int i = 0;i < m_childList.size();++ i)
		{
			m_childList[i]->disVisible();
		}
	}
}
void _XObjectBasic::pushChild(_XObjectBasic * child)
{
	if(child == NULL) return;
	if(child == this) return;	//����Ϊ����
	if(getIsChild(child)) return;
	m_childList.push_back(child);
	child->m_parent = this;
	_XVector2 pos = (child->getPosition() - getPosition()) / getSize();
	m_childRelativePos.push_back(pos);
	_XVector2 size = child->getSize() / getSize();
	m_childRelativeSize.push_back(size);
	float angle = child->getAngle() - getAngle();
	m_childRelativeAngle.push_back(angle);
}
void _XObjectBasic::popChild(_XObjectBasic * child)
{
	if(child == NULL) return;
	_XBool flag = XFalse;
	for(int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child) flag = XTrue;
		if(flag && i < m_childList.size() - 1)
		{
			m_childList[i] = m_childList[i + 1];
			m_childRelativePos[i] = m_childRelativePos[i + 1];	
			m_childRelativeSize[i] = m_childRelativeSize[i + 1];	
			m_childRelativeAngle[i] = m_childRelativeAngle[i + 1];	
		}
	}
	if(flag) 
	{//�������һ��
		m_childList.pop_back();
		m_childRelativePos.pop_back();	
		m_childRelativeSize.pop_back();	
		m_childRelativeAngle.pop_back();
		child->m_parent = NULL;
	}
}
void _XObjectBasic::clearAllChild()
{
	while(m_childList.size() != 0)
	{
		m_childList[m_childList.size() - 1]->m_parent = NULL;
		m_childList.pop_back();
	}
	m_childList.clear();	
	m_childRelativePos.clear();	
	m_childRelativeSize.clear();	
	m_childRelativeAngle.clear();	
}
_XBool _XObjectBasic::getIsChild(_XObjectBasic * child)
{
	if(child == NULL) return XFalse;
	for(int i = 0;i < m_childList.size();++ i)
	{
		if(m_childList[i] == child) return XTrue;
		if(m_childList[i]->getIsChild(child)) return XTrue;
	}
	return XFalse;
}