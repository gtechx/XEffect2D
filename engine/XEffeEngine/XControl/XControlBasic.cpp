//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlBasic.h"

_XControlBasic::_XControlBasic()
:m_mouseRect(0.0f,0.0f,1.0f,1.0f)		//�ؼ��������Ӧ��Χ
,m_ctrlType(CTRL_OBJ_NULL)
,m_size(1.0f,1.0f)		//�ؼ��Ĵ�С
,m_position(0.0f,0.0f)	//�ؼ���λ��
,m_color(1.0f,1.0f,1.0f,1.0f)		//�ؼ�����ɫ
,m_isEnable(XFalse)		//�ؼ��Ƿ���Ч����Ч����������������������
,m_isVisible(XFalse)		//�ؼ��Ƿ�ɼ����ɼ�����������������������
,m_isActive(XFalse)		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
,m_isBeChoose(XFalse)
,m_withAction(XTrue)
,m_isInAction(XFalse)
,m_isMouseInRect(XFalse)
{
	static int controlOrder = 0;
	controlOrder ++;
	m_objectID = controlOrder;
	m_objType = OBJ_CONTROL;
}
_XBool _XControlBasic::setACopy(const _XControlBasic & temp)
{
	m_comment.setACopy(temp.m_comment);
	m_isMouseInRect = temp.m_isMouseInRect;		//�ؼ��������Ӧ��Χ
	m_mouseRect = temp.m_mouseRect;		//�ؼ��������Ӧ��Χ
	m_nowMouseRect = temp.m_nowMouseRect;		//�ؼ��������Ӧ��Χ
	m_size = temp.m_size;		//�ؼ��Ĵ�С
	m_position = temp.m_position;	//�ؼ���λ��
	m_color = temp.m_color;		//�ؼ�����ɫ

	m_isEnable = temp.m_isEnable;		//�ؼ��Ƿ���Ч����Ч����������������������
	m_isVisible = temp.m_isVisible;		//�ؼ��Ƿ�ɼ����ɼ�����������������������
	m_isActive = temp.m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
	m_isBeChoose = temp.m_isBeChoose;
	m_withAction = temp.m_withAction;
	m_isInAction = temp.m_isInAction;
	return XTrue;
}