#ifndef _JIA_XBEZIERINTERPOLATION_
#define _JIA_XBEZIERINTERPOLATION_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "../XCommonDefine.h"
namespace XE{
class XBezierInterpolation
{
private:
	float m_p0;		//3�α����������е����
	float m_p1;		//���ƫ��
	float m_p2;		//�յ�ƫ��
	float m_p3;		//�յ�
public:
	float getBezierSplineValue(float temp);	//temp����(0,1)

	//���������߿����Զ��ƶ�ȡֵ
	float m_curValue;			//��ǰ�ı�����ȡֵ
	float m_curPosition;		//�����е�ĵ�ǰλ��
	float m_speed;				//�����е���ƶ��ٶ�
	XBool m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	void move(float timeDelay);	//�ƶ������еĵ�
	XBezierInterpolation();
	void init(float p0,float p1,float p2,float p3);
	void reset();				//���������ƶ���
};
inline void XBezierInterpolation::init(float p0,float p1,float p2,float p3)
{
	m_p0 = p0;
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
}
inline void XBezierInterpolation::reset()				//���������ƶ���
{
	m_curPosition = 0.0f;
	m_isEnd = XFalse;
	m_curValue = m_p0;
}
}
#endif