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
class XMatrix2x2
{
public:
	float data[4];
	static const XMatrix2x2 identity;	//��λ����
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	void loadIdentity();
	XMatrix2x2(float d0,float d1,float d2,float d3)
	{
		data[0] = d0;
		data[1] = d1;
		data[2] = d2;
		data[3] = d3;
	}
	XMatrix2x2() {loadIdentity();}

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	XVector2 operator *(const XVector2 &v);	//v������������������û�����壬OpenGL��Ĭ��Ϊ������ -- Ӧ��������ϵ�仯�е����ź���ת

	float getValue();//�����������ʽ��ֵ

	XMatrix2x2 operator*(const XMatrix2x2 & rhs) const;
	XMatrix2x2 inverse();	//�������
	XMatrix2x2 anti();		//���þ���
//	void anti();
};
#if WITH_INLINE_FILE
#include "XMatrix2x2.inl"
#endif
inline XVector2 operator *(const XVector2 &v,const XMatrix2x2 & m)	//v������������������û�����壬DirectX��Ĭ��Ϊ������
{
	return XVector2(v.x * m.data[0] + v.y * m.data[2],v.x * m.data[1] + v.y * m.data[3]);
}
namespace XMath{
//��v������size
inline XMatrix2x2 getMatrix2D(float size,const XVector2 &v)		//ͨ����ת�ĽǶȺ����ŵĳߴ������ת����
{
	float t = size - 1.0f;
	return XMatrix2x2(1.0f + t * v.x * v.x,t * v.x * v.y,
						t * v.x * v.y,1.0f + t * v.y * v.y);
}
//angle [0 - 2PI]
inline XMatrix2x2 getMatrix2D(float angle)		//ͨ����ת�ĽǶȼ�����ת����
{
	return XMatrix2x2(cos(angle),sin(angle),-sin(angle),cos(angle));
}
inline XMatrix2x2 getMatrix2D(const XVector2 &s)	//���Ų����ľ���
{
	return XMatrix2x2(s.x,0,0,s.y);
}
//inline float getAngle(const XMatrix2x2 &m);	//ͨ����ת���������ת�ĽǶ�(��δʵ��)
//inline XVector2 getSize(const XMatrix2x2 &m);	//ͨ����ת����������ŵĳߴ�(��δʵ��)
}
}
#endif