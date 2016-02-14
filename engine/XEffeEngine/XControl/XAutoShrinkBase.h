#ifndef _JIA_XAUTOSHRINKBASE_
#define _JIA_XAUTOSHRINKBASE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
#include "../XOSDefine.h"
//#include "../XMouseAndKeyBoardDefine.h"
#include "../XMath/XVector2.h"
#include "../XMath/XMoveData.h"
namespace XE{
//�����Զ��������ܵ�����Ļ���
class XAutoShrinkBase
{
protected:
	bool m_withAutoShrink;	//�Ƿ���Զ�����
	float m_shrinkTimer;	//������ʱ��
	bool m_isInShrink;		//��ǰ�Ƿ�������״̬
	float m_shrinkRate;		//�����İٷֱ�0.0f - 1.0f;
	XVector2 m_curMousePosition;	//��ǰ����λ��
	bool m_oldIsInRect;
	XMoveData m_shrinkMD;
	enum XShrinkDirection
	{
		DIR_NULL,	//δ������������
		DIR_UP,		//��������
		DIR_LEFT,	//��������
		DIR_RIGHT,	//��������
		DIR_DOWN,	//��������
	};
	XShrinkDirection m_curShrinkDir;	//��ǰ����������
	virtual XShrinkDirection calShrinkDir() = 0;	//���㵱ǰ���ܵ���������
	void startShrink(XShrinkDirection dir)	//��ʼ����
	{
		m_shrinkMD.set(m_shrinkRate,1.0f,0.01f,MOVE_DATA_MODE_COS_MULT);
		m_curShrinkDir = dir;
		m_isInShrink = true;
	}
	void disShrink()	//������״̬��ԭ
	{
		m_shrinkMD.set(m_shrinkRate,0.0f,0.01f,MOVE_DATA_MODE_SIN_MULT);
		m_isInShrink = false;
	}
	void updateShrinkState(float stepTime);	//����������״̬
public:
	void setAutoShrink(bool flag)
	{
		//������Ҫ���ݵ�ǰ״̬�޸Ĵ��ڵ�״̬
		if(m_withAutoShrink && !flag && m_isInShrink)
		{//������Ҫ����Ϊ������״̬(��δ���)
		}
		m_withAutoShrink = flag;
	}
	bool getAutoShrink()const{return m_withAutoShrink;}
public:
	XAutoShrinkBase()
		:m_withAutoShrink(false)
		,m_shrinkTimer(0.0f)
		,m_isInShrink(false)
		,m_shrinkRate(0.0f)
		,m_oldIsInRect(false)
	{}
	virtual ~XAutoShrinkBase(){}
protected:
	virtual void updateShrinkPosition() = 0;	//����������λ��
	virtual XBool getIsInShrinkRect() = 0;		//�ж�λ���Ƿ�����ק������
};
}
#endif