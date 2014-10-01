#ifndef _JIA_XMATRIX3X3_
#define _JIA_XMATRIX3X3_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "XVector2.h"
#include "XVector3.h"
#include "XVector4.h"
class _XMatrix3x3
{
public:
	float data[9];
	static const _XMatrix3x3 identity;	//��λ����
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	float& operator () (int x,int y);
	float operator () (int x,int y) const;
	
	void loadIdentity();
	_XMatrix3x3(float d0,float d1,float d2,float d3,float d4,
		float d5,float d6,float d7,float d8)
	{
		data[0] = d0;data[1] = d1;data[2] = d2;
		data[3] = d3;data[4] = d4;data[5] = d5;
		data[6] = d6;data[7] = d7;data[8] = d8;
	}
	_XMatrix3x3() {loadIdentity();}
	_XMatrix3x3 operator*(const _XMatrix3x3 & rhs) const;
	_XMatrix3x3 inverse();
	float getValue();	//������������ʽ��ֵ	
	//����ĺ�����������Ԫһ�η�����
	//aX + bY = c;
	//dX + eY = f;
	//��Ӧ��4������ʽΪ
	//a,b,c,
	//d,e,f,
	//0,0,1,
	_XVector2 resolveEquation();				//����Ԫһ�η�����
	_XVector3 operator *(const _XVector3 &v);	//v������������������û�����壬OpenGL��Ĭ��Ϊ������
	_XMatrix3x3 anti();
//	void anti();	//��δʵ��
};
#include "XMatrix3x3.inl"
//v * m
inline _XVector3 transform3x3(const _XVector3& v,const _XMatrix3x3& m) 
{
	return _XVector3((m.data[0]*v.x + m.data[3]*v.y + m.data[6]*v.z),
	                 (m.data[1]*v.x + m.data[4]*v.y + m.data[7]*v.z),
	                 (m.data[2]*v.x + m.data[5]*v.y + m.data[8]*v.z));
}
inline _XVector3 operator *(const _XVector3 &v,const _XMatrix3x3 & m)	//v������������������û�����壬DirectX��Ĭ��Ϊ������
{
	return transform3x3(v,m);
}
//m * v
inline _XVector3 transform3x3(const _XMatrix3x3& m,const _XVector3& v) 
{
	return _XVector3((m.data[0]*v.x + m.data[1]*v.y + m.data[2]*v.z),
	                 (m.data[3]*v.x + m.data[4]*v.y + m.data[5]*v.z),
	                 (m.data[6]*v.x + m.data[7]*v.y + m.data[8]*v.z));
}
inline _XMatrix3x3 getMatrix3D(const _XVector3 &s)	//���Ų����ľ���
{
	return _XMatrix3x3(s.x,0,0,
						0,s.y,0,
						0,0,s.z);
}
//��v������size
inline _XMatrix3x3 getMatrix3D(float size,const _XVector3 &v)		//ͨ����ת�ĽǶȺ����ŵĳߴ������ת����
{
	float t = size - 1.0f;
	return _XMatrix3x3(1.0f + t * v.x * v.x,t * v.x * v.y,t * v.x * v.z,
						t * v.x * v.y,1.0f + t * v.y * v.y,t * v.y * v.z,
						t * v.x * v.z,t * v.y * v.z,1.0f + t * v.z * v.z);
}
//��v����תangle����ת����
inline _XMatrix3x3 getRotate3D(float angle,_XVector3 v)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	float tCosA = 1 - cosA;
	v = normalize(v);
	return _XMatrix3x3( tCosA * v.x * v.x + cosA,		tCosA * v.x * v.y + v.z * sinA, tCosA * v.x * v.z - sinA * v.y,
						tCosA * v.x * v.y - sinA * v.z,	tCosA * v.y * v.y + cosA,		tCosA * v.y * v.z + sinA * v.x,
						tCosA * v.x * v.z + sinA * v.y,	tCosA * v.y * v.z - v.x * sinA,	tCosA * v.z * v.z + cosA);
}
//��x����תangle
inline _XMatrix3x3 getRotate3DX(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	1,	0,		0,
						0,	cosA,	sinA,
						0,	-sinA,	cosA);
}
//��y����תangle
inline _XMatrix3x3 getRotate3DY(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	cosA,0,-sinA,
						0,1,0,
						sinA,0,cosA);
}
//��z����תangle
inline _XMatrix3x3 getRotate3DZ(float angle)
{
	float sinA = sin(angle * DEGREE2RADIAN);
	float cosA = cos(angle * DEGREE2RADIAN);
	return _XMatrix3x3(	cosA,sinA,0,
						-sinA,cosA,0,
						0,0,1);
}
//ŷ���ǵ���ת����
inline _XMatrix3x3 getRotate3D(const _XVector3 &a)
{//ͨ����֤
	_XMatrix3x3 ret = getRotate3DZ(a.z);
	ret = ret * getRotate3DX(a.x);
	ret = ret * getRotate3DY(a.y);
	return ret;
}
//ŷ���ǵ���ת����
inline _XMatrix3x3 getRotateMatrix(const _XVector3 &a)
{//ͨ����֤
	float sh,ch, sp,cp, sb,cb;
	sp = sin(a.x * DEGREE2RADIAN);
	cp = cos(a.x * DEGREE2RADIAN);
	sh = sin(a.y * DEGREE2RADIAN);
	ch = cos(a.y * DEGREE2RADIAN);
	sb = sin(a.z * DEGREE2RADIAN);
	cb = cos(a.z * DEGREE2RADIAN);

	_XMatrix3x3 ret;

	ret(0,0) = ch * cb + sh * sp * sb;
	ret(0,1) = -ch * sb + sh * sp * cb;
	ret(0,2) = sh * cp;

	ret(1,0) = sb * cp;
	ret(1,1) = cb * cp;
	ret(1,2) = -sp;

	ret(2,0) = -sh * cb + ch * sp * sb;
	ret(2,1) = sb * sh + ch * sp * cb;
	ret(2,2) = ch * cp;
	return ret;
}
//ͨ����ת�������ŷ����
inline _XVector3 getEulerian(const _XMatrix3x3& m)
{//ͨ����֤
	_XVector3 ret;
	float sp = -(m(1,2));
	if(fabs(sp) > 0.9999f)
	{
		ret.x = PI_HALF * sp;	//pitch
		ret.y = atan2(-(m(2,0)),m(0,0));	//heading
		ret.z = 0.0f;		//bank
	}else
	{
		ret.x = asin(sp);
		ret.y = atan2(m(0,2),m(2,2));
		ret.z = atan2(m(1,0),m(1,1));
	}
	return ret;
}
//��Ԫ��ת��Ϊ��ת����
inline _XMatrix3x3 toMatrix3x3(const _XVector4 &v)
{//������֤
	float n = (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
	float s = (n > 0.0f) ? (2.0f / n) : 0.0f;
	return _XMatrix3x3(1.0f - (v.y * v.y * s + v.z * v.z *s),	v.x * v.y * s - v.w * v.z * s,			v.x * v.z * s + v.w * v.y * s,
						v.x * v.y * s + v.w * v.z * s,			1.0f - (v.x * v.x * s + v.z * v.z * s),	v.y * v.z * s - v.w * v.x * s,
						v.x * v.z * s - v.w * v.y * s,			v.y * v.z * s + v.w * v.x * s,			1.0f - (v.x * v.x * s + v.y * v.y * s));
}
extern _XVector4 toVector4(const _XMatrix3x3 &m);//����ת������Ԫ����ת��

#endif