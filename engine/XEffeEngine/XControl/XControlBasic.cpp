#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlBasic.h"
namespace XE{
XControlBasic::XControlBasic()
	:m_mouseRect(0.0f, 0.0f, 1.0f, 1.0f)		//�ؼ��������Ӧ��Χ
	, m_ctrlSpecialType(CTRL_TYPE_NORMAL)
	, m_ctrlType(CTRL_OBJ_NULL)
	, m_scale(1.0f)		//�ؼ��Ĵ�С
	, m_position(0.0f)	//�ؼ���λ��
	, m_color(1.0f, 1.0f)		//�ؼ�����ɫ
	, m_isEnable(XFalse)		//�ؼ��Ƿ���Ч����Ч����������������������
	, m_isVisible(XFalse)		//�ؼ��Ƿ�ɼ����ɼ�����������������������
	, m_isActive(XFalse)		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
	, m_isBeChoose(XFalse)
	, m_withAction(XTrue)
	, m_isInAction(XFalse)
	, m_isMouseInRect(XFalse)
	, m_pClass(NULL)
	, m_eventProc(NULL)
	, m_needSaveAndLoad(XTrue)
	, m_isSilent(XFalse)
	, m_withWireframe(true)
{
	static int controlOrder = 0;
	++controlOrder;
	m_objectID = controlOrder;
	m_objType = OBJ_CONTROL;
	m_ctrlName = "OBJ_CONTROL_" + XString::toString(m_objectID);
}
XBool XControlBasic::setACopy(const XControlBasic & temp)
{
	m_comment.setACopy(temp.m_comment);
	m_isMouseInRect = temp.m_isMouseInRect;		//�ؼ��������Ӧ��Χ
	m_mouseRect = temp.m_mouseRect;		//�ؼ��������Ӧ��Χ
	m_curMouseRect = temp.m_curMouseRect;		//�ؼ��������Ӧ��Χ
	m_scale = temp.m_scale;		//�ؼ��Ĵ�С
	m_position = temp.m_position;	//�ؼ���λ��
	m_color = temp.m_color;		//�ؼ�����ɫ
	m_pClass = temp.m_pClass;
	m_eventProc = temp.m_eventProc;	

	m_isEnable = temp.m_isEnable;		//�ؼ��Ƿ���Ч����Ч����������������������
	m_isVisible = temp.m_isVisible;		//�ؼ��Ƿ�ɼ����ɼ�����������������������
	m_isActive = temp.m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
	m_isBeChoose = temp.m_isBeChoose;
	m_withAction = temp.m_withAction;
	m_isInAction = temp.m_isInAction;
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XControlBasic.inl"
#endif
}