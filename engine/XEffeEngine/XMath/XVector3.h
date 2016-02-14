#ifndef _JIA_XVECTOR3_
#define _JIA_XVECTOR3_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "math.h"
namespace XE{
class XVector3
{
public:
    float x;
    float y;
    float z;
public:
	static const XVector3 zero;
    void reset();//���������
    void add(float a,float b,float c);//����������ֵ
    void set(float a,float b,float c);//������ֵ����һ�����ֵ
	XVector3(float a,float b,float c)
		:x(a),y(b),z(c)
	{}
	XVector3(const XVector3 & temp)
		:x(temp.x),y(temp.y),z(temp.z)
	{}
	XVector3()
		:x(0.0f),y(0.0f),z(0.0f)
	{}

	XVector3 operator + (const XVector3& temp) const;
    XVector3 operator - (const XVector3& temp) const;
	//��˵ļ�������
	//|a * b| = |a|*|b|*sin(�н�)����˵Ľ����ֱ��a��b������Ϊ���ֶ��򣬴�С����ǰ��Ĺ�ʽ 
    XVector3 operator * (const XVector3& temp) const;	//CROSS product
    XVector3 operator / (const XVector3& temp) const;
	XVector3 operator + (const float& temp) const;
    XVector3 operator - (const float& temp) const;
    XVector3 operator * (const float& temp) const;
    XVector3 operator / (const float& temp) const;
    void operator += (const XVector3& temp);
    void operator -= (const XVector3& temp);
    void operator *= (const XVector3& temp);	//CROSS product
    void operator /= (const XVector3& temp);
    void operator += (const float& temp);
    void operator -= (const float& temp);
    void operator *= (const float& temp);
    void operator /= (const float& temp);

	float getLength(const XVector3& P0) const;     
    float getLength(float a,float b,float c) const;		
	float getLength() const;
	float getLengthSqure(const XVector3& P0) const;     
    float getLengthSqure(float a,float b,float c) const;		
	float getLengthSqure() const;

	float dot(const XVector3& temp) const;//��ˣ���������:a.dot(b) = |a|*|b|*cos(�н�)
	void normalize();	//��λ��
	//�ض��帴�Ʋ������Ϳ������캯��
	XVector3 &operator = (const XVector3 &temp);
	XVector3& operator = (const float* temp);	//�߼�������
	XVector3(const float *temp)	//�����߼�������
	{
		if(temp == NULL)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}else
		{
			x = temp[0];
			y = temp[1];
			z = temp[2];
		}
	}
	XVector3 operator - () const; //-v

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};
typedef XVector3 XPoint3;
#if WITH_INLINE_FILE
#include "XVector3.inl"
#endif
namespace XMath{
inline XVector3 normalize(const XVector3& v) 
{
	float d = v.getLengthSqure();
	if(d == 0.0f) return v * (1.0f/0.0000001f);
	else return v * (1.0f/sqrt(d));
	//float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	//if(d == 0.0f) d = 0.0000001f;
	//return v * (1.0f/d);
}
//����ռ������㹹�ɵ���ķ���
inline XVector3 getNormal(const XVector3 &p0,const XVector3 &p1,const XVector3 &p2)
{//����������ķ���
	//XVector3 w = p0 - p1;
	//XVector3 v = p2 - p1;
	//XVector3 n = XVector3(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
	//return n / n.getLength();
	return normalize((p2 - p1) * (p0 - p1));
}
//����ռ������㹹�ɵ���ķ���
inline XVector3 getNormal(const float *p0,const float *p1,const float *p2)
{
//	XVector3 w(p0[0] - p1[0],p0[1] - p1[1],p0[2] - p1[2]);
//	XVector3 v(p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2]);
//	XVector3 n(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
//	return n / n.getLength();
	return normalize(XVector3(p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2]) * 
		XVector3(p0[0] - p1[0],p0[1] - p1[1],p0[2] - p1[2]));
}
}
inline XVector3 operator *(float a,const XVector3 &temp)
{//���
	return temp * a;
}
//���
//inline float operator^(const XVector3 &v1,const XVector3& v2)  // DOT product
//{
//	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//}
//�����߶�AB��BC�ļн�,0 - PI,ǰ���������ߵĽ�
inline float getAngle(const XVector3 &A,const XVector3 &B,const XVector3 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline XVector3 projector(const XVector3 &v1,const XVector3 &v2)
{//��v1������v2ͶӰ�γɵ�����
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
}
#endif