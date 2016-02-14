#ifndef _JIA_XSTAGEOBJECT_
#define _JIA_XSTAGEOBJECT_
#include "XMath\XVector3.h"
#include "XMath\XVector2.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
namespace XE{
//��̨�ϵ����
enum XStageObjectType
{//�����е��������
	STG_OBJ_NULL,	//��Ч�����
	STG_OBJ_MAX,	//����ͳ�����������
};
class XStageObject
{
public:
	std::string m_name;		//����ʹ��Ҳ���Բ�����
	int m_id;				//Ψһ
	XVector3 m_position;		//�����λ��
	XVector2 m_scale;			//��������ű���
	XStageObjectType m_type;	//���������
	bool m_visible;				//����Ƿ�ɼ�

	XVector2 m_originOffset;

	virtual void setPosition(const XVector2 &pos){setPosition(pos.x,pos.y);}
	virtual void setPosition(float x,float y) = 0;
	virtual void setScale(const XVector2 &scale){setScale(scale.x,scale.y);};
	virtual void setScale(float x,float y) = 0;
	virtual void draw() = 0;
	virtual void update(float stepTime) = 0;
	XStageObject()
		:m_scale(1.0f,1.0f)
		,m_visible(true)
		,m_type(STG_OBJ_NULL)
	{
		static int tmpID = 0;
		m_id = tmpID ++;
	}
};
}
#endif