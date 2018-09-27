#ifndef _JIA_XVECTOR2_
#define _JIA_XVECTOR2_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "XBasicFun.h" 
#include "XMath.h"
#include "../XCommonDefine.h"

namespace XE{
//������2D��
class XVec2
{
public:
	float x;
	float y;
public:
	static const XVec2 zero;
	static const XVec2 zeroOne;
	static const XVec2 oneZero;
	static const XVec2 one;

	void reset();    //���������
	void set(const XVec2& temp);
	void set(float a, float b);    //���õ�ǰ���ֵ
	void set(float v) { x = y = v; }
	void set(float* v);    //���õ�ǰ���ֵ
	void add(float a, float b);    //����ĳ��ֵ

	XVec2 operator + (const XVec2& temp) const;
	XVec2 operator - (const XVec2& temp) const;
	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
	XVec2 operator * (const XVec2& temp) const;
	XVec2 operator / (const XVec2& temp) const;
	XVec2 operator + (const float& temp) const;
	XVec2 operator - (const float& temp) const;
	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
	XVec2 operator * (const float& temp) const;
	XVec2 operator / (const float& temp) const;
	void operator += (const XVec2& temp);
	void operator -= (const XVec2& temp);
	void operator *= (const XVec2& temp);
	void operator /= (const XVec2& temp);
	void operator += (const float& temp);
	void operator -= (const float& temp);
	void operator *= (const float& temp);
	void operator /= (const float& temp);

	//	XVec2& operator = (const XVector2I & temp);
	//	XVec2(const XVector2I & temp);

	float getLength(const XVec2& P0) const;     //��ȡ������֮��ľ���
	float getLength(float a, float b) const;			//��ȡ������֮��ľ���
	float getLength() const;						//��õ�ԭ��ľ���
	float getLengthSqure(const XVec2& P0) const;//��ȡ����֮������ƽ��
	float getLengthSqure(float a, float b) const;
	float getLengthSqure() const;					//��õ�ԭ��ľ����ƽ��
	float getAngle(const XVec2& P0) const;      //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
	float getAngle(float a, float b) const;          //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
	float getAngle() const;							//��ȡԭ�㵽��ǰ��ĽǶ�

	float dot(const XVec2& temp) const { return (x * temp.x + y * temp.y); }
	//��ˣ�Ҳ���������ڻ���������������˼�壬�������Ľ����һ������ 
	//����� (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//���������������ʸ���ļн�,��ʽ����: 
	//cos (V dot W) =V.W / | V | | W | 
	//��˵ļ��������ǣ���һ��������һ���ߵ�ͶӰ������һ���ߵĳ���
	float cross(const XVec2& temp)const { return x * temp.y - y * temp.x; }	//CROSS product
	void normalize();	//��λ�� ���� ��һ��
	XVec2 perp() { return XVec2(y, -x); }	//����

	XVec2::XVec2(float a, float b)
		:x(a), y(b)
	{}
	XVec2::XVec2(float a)
		: x(a), y(a)
	{}
	XVec2::XVec2()
		: x(0.0f), y(0.0f)
	{}
	XVec2(const XVec2& temp)
		:x(temp.x), y(temp.y)
	{}
	//	XVec2& operator = (const XVec2& temp);
	XVec2& operator = (const float* temp);	//�����߼�������
	XVec2(const float *temp) //�����߼�������
	{
		if (temp == NULL)
		{
			x = y = 0.0f;
		}
		else
		{
			x = temp[0];
			y = temp[1];
		}
	}
	//-v
	XVec2 operator - () const;
	friend std::ostream& operator<<(std::ostream& os, const XVec2& vec)
	{
		os << vec.x << "," << vec.y;
		return os;
	}
	friend std::istream& operator >> (std::istream& is, XVec2& vec)
	{
		is >> vec.x;
		is.ignore(1);
		is >> vec.y;
		return is;
	}
	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }
	XVec2 rotation(float angle) const	//���鲻Ҫʹ���������������ʹ����ת������ʵ��
	{
		float c = cos(angle);
		float s = sin(angle);
		return XVec2(x * c - y * s, x * s + y * c);
	}
};

typedef XVec2 XPoint;

#if WITH_INLINE_FILE
#include "XVector2.inl"
#endif

inline XVec2 operator *(float a,const XVec2& temp)
{//���
	return temp * a;
}
//inline float operator^(const XVec2& v1,const XVec2& v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
namespace XMath{
//�����߶�AB��BC�ļн�,0 - PI2,ǰ���������ߵĽ�
inline float angleBetween(const XVec2& A,const XVec2& B,const XVec2& C)
{//δ��������
	XVec2 tmp1 = B - A;
	XVec2 tmp2 = C - B;
	return XMath::angleAbbreviation(atan2f(tmp1.cross(tmp2), tmp1.dot(tmp2)));
    //double sin = tmp1.cross(tmp2);
    //double cos = tmp1.dot(tmp2);
	//return XMath::angleAbbreviation(atan2f(sin, cos));
}
//�����߶�AB��BC�ļн�,0 - PI,ǰ���������ߵĽ�
inline float getAngle(const XVec2& A,const XVec2& B,const XVec2& C)
{
	XVec2 tmp1 = B - A;
	XVec2 tmp2 = C - B;
	return acos(tmp1.dot(tmp2) / sqrt(tmp1.getLengthSqure() * tmp2.getLengthSqure()));
}
inline XVec2 projector(const XVec2& v1,const XVec2& v2)
{//��v1������v2ͶӰ�γɵ�����
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
inline XVec2 normalize(const XVec2& v)
{
	float r = v.getLengthSqure();
	if(r != 0) return v * (1.0f / sqrt(r));
	return XVec2::zero;
}
//angle��λ����
//���ؼ�����ת֮������Ĳ���
inline XVec2 getRotateRate(float angle)
{
	return XVec2(cos(angle), sin(angle));
}
//pos:λ��
//RPos:��������ĵ�
//w������İ��
//h������İ��
inline bool isInArea(const XVec2& pos, const XVec2& RPos, float w, float h)
{
	return isInArea<float>(pos.x, RPos.x - w, RPos.x + w) &&
		isInArea<float>(pos.y, RPos.y - h, RPos.y + h);
}
}
}
#endif