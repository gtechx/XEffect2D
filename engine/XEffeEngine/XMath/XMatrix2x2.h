#ifndef _JIA_XMATRIX2X2_
#define _JIA_XMATRIX2X2_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
#include "XVector2.h"
namespace XE{
class XMat2
{
public:
	float data[4];
	static const XMat2 identity;	//��λ����
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	void loadIdentity();
	XMat2(float d0,float d1,float d2,float d3)
	{
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
		data[3] = d3;
	}
	XMat2() { loadIdentity(); }

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	XVec2 operator *(const XVec2& v);	//v������������������û�����壬OpenGL��Ĭ��Ϊ������ -- Ӧ��������ϵ�仯�е����ź���ת

	float getValue();//�����������ʽ��ֵ

	XMat2 operator*(const XMat2 & rhs) const;
	XMat2 inverse();	//�������
	XMat2 anti();		//���þ���
//	void anti();
};
#if WITH_INLINE_FILE
#include "XMatrix2x2.inl"
#endif
//v * m
inline XVec2 operator *(const XVec2& v,const XMat2 & m)	//v������������������û�����壬DirectX��Ĭ��Ϊ������
{
	return XVec2(v.x * m.data[0] + v.y * m.data[2],
		v.x * m.data[1] + v.y * m.data[3]);
}
namespace XMath{
//��v������size
inline XMat2 getMatrix2D(float size,const XVec2& v)		//ͨ����ת�ĽǶȺ����ŵĳߴ������ת����
{
	float t = size - 1.0f;
	return XMat2(1.0f + t * v.x * v.x, t * v.x * v.y,
		t * v.x * v.y, 1.0f + t * v.y * v.y);
}
//angle [0 - 2PI]
inline XMat2 getMatrix2D(float angle)		//ͨ����ת�ĽǶȼ�����ת����
{
	return XMat2(cos(angle), sin(angle), -sin(angle), cos(angle));
}
inline XMat2 getMatrix2D(const XVec2& s)	//���Ų����ľ���
{
	return XMat2(s.x, 0.0f, 0.0f, s.y);
}
//inline float getAngle(const XMat2 &m);	//ͨ����ת���������ת�ĽǶ�(��δʵ��)
//inline XVec2 getSize(const XMat2 &m);	//ͨ����ת����������ŵĳߴ�(��δʵ��)
}
}
#endif