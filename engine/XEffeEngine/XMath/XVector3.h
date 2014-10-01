#ifndef _JIA_XVECTOR3_
#define _JIA_XVECTOR3_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XBasicFun.h"
class _XVector3
{
public:
    float x;
    float y;
    float z;
public:
	static const _XVector3 zero;
    void reset();//���������
    void add(float a,float b,float c);//����������ֵ
    void set(float a,float b,float c);//������ֵ����һ�����ֵ
	_XVector3(float a,float b,float c)
		:x(a),y(b),z(c)
	{}
	_XVector3(const _XVector3 & temp)
		:x(temp.x),y(temp.y),z(temp.z)
	{}
	_XVector3()
		:x(0.0f),y(0.0f),z(0.0f)
	{}

	_XVector3 operator + (const _XVector3& temp) const;
    _XVector3 operator - (const _XVector3& temp) const;
	//��˵ļ�������
	//|a * b| = |a|*|b|*sin(�н�)����˵Ľ����ֱ��a��b������Ϊ���ֶ��򣬴�С����ǰ��Ĺ�ʽ 
    _XVector3 operator * (const _XVector3& temp) const;	//CROSS product
    _XVector3 operator / (const _XVector3& temp) const;
	_XVector3 operator + (const float& temp) const;
    _XVector3 operator - (const float& temp) const;
    _XVector3 operator * (const float& temp) const;
    _XVector3 operator / (const float& temp) const;
    void operator += (const _XVector3& temp);
    void operator -= (const _XVector3& temp);
    void operator *= (const _XVector3& temp);	//CROSS product
    void operator /= (const _XVector3& temp);
    void operator += (const float& temp);
    void operator -= (const float& temp);
    void operator *= (const float& temp);
    void operator /= (const float& temp);

	float getLength(const _XVector3& P0) const;     
    float getLength(float a,float b,float c) const;		
	float getLength() const;
	float getLengthSqure(const _XVector3& P0) const;     
    float getLengthSqure(float a,float b,float c) const;		
	float getLengthSqure() const;

	float dot(const _XVector3& temp) const;//��ˣ���������:a.dot(b) = |a|*|b|*cos(�н�)
	void normalize();	//��λ��
	//�ض��帴�Ʋ������Ϳ������캯��
	_XVector3 &operator = (const _XVector3 &temp);
	_XVector3& operator = (const float* temp);	//�߼�������
	_XVector3(const float *temp)	//�����߼�������
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
	_XVector3 operator - () const; //-v

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};
typedef _XVector3 _XPoint3;
#include "XVector3.inl"
inline _XVector3 normalize(const _XVector3& v) 
{
	float d = v.getLengthSqure();
	if(d == 0.0f) return v * (1.0f/0.0000001f);
	else return v * (1.0f/sqrt(d));
	//float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	//if(d == 0.0f) d = 0.0000001f;
	//return v * (1.0f/d);
}
//����ռ������㹹�ɵ���ķ���
inline _XVector3 getNormal(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2)
{//����������ķ���
	//_XVector3 w = p0 - p1;
	//_XVector3 v = p2 - p1;
	//_XVector3 n = _XVector3(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
	//return n / n.getLength();
	return normalize((p2 - p1) * (p0 - p1));
}
//����ռ������㹹�ɵ���ķ���
inline _XVector3 getNormal(const float *p0,const float *p1,const float *p2)
{
//	_XVector3 w(p0[0] - p1[0],p0[1] - p1[1],p0[2] - p1[2]);
//	_XVector3 v(p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2]);
//	_XVector3 n(w.y * v.z - w.z * v.y,w.x * v.z - w.z * v.x,w.x * v.y - w.y * v.x);
//	return n / n.getLength();
	return normalize(_XVector3(p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2]) * 
		_XVector3(p0[0] - p1[0],p0[1] - p1[1],p0[2] - p1[2]));
}
inline _XVector3 operator *(float a,const _XVector3 &temp)
{//���
	return temp * a;
}
//���
//inline float operator^(const _XVector3 &v1,const _XVector3& v2)  // DOT product
//{
//	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
//}
//�����߶�AB��BC�ļн�,0 - PI,ǰ���������ߵĽ�
inline float getAngle(const _XVector3 &A,const _XVector3 &B,const _XVector3 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline _XVector3 projector(const _XVector3 &v1,const _XVector3 &v2)
{//��v1������v2ͶӰ�γɵ�����
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
#endif