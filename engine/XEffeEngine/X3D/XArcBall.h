#ifndef _JIA_XARCBALL_
#define _JIA_XARCBALL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
//#include "XBasicClass.h"
#include "../XMath/XVector3.h"
#include "../XMath/XVector4.h"
namespace XE{
class XArcBall
{
protected:
	bool m_isInited;	//�Ƿ��ʼ��
	XVec3 m_clickVector;
	XVec3 m_dragVector;
	float m_sizeWRate;	//�ߴ��ת����
	float m_sizeHRate;

	void mapToSphere(const XVec2& point,XVec3& vec) const;	//��2D����ӳ���3D����
public:
	XArcBall(float width,float height)
		:m_isInited(false)
	{
		setSize(width,height);
	}
	XArcBall()
		:m_isInited(false)
	{}
	~XArcBall() {};
	bool setSize(float w,float h)
	{
		if(w <= 1.0f || h <= 1.0f) return false;	//���ݲ��Ϸ�
		m_sizeWRate = 2.0f / (w - 1.0f);
		m_sizeHRate = 2.0f / (h - 1.0f);
		m_isInited = true;
		return true;
	}
	void click(const XVec2& point){mapToSphere(point,m_clickVector);}	//��갴�¼�¼����״̬
	void drag(const XVec2& point,XVec4& rotate); //���������϶�������ת����Ԫ��
};
}
#endif