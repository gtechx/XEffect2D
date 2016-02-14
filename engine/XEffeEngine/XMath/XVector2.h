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
class XVector2
{
public:
    float x;
    float y;
public:
	static const XVector2 zero;

    void reset();    //���������
	void set(const XVector2 &temp);
    void set(float a,float b);    //���õ�ǰ���ֵ
	void set(float v) {set(v,v);}
    void add(float a,float b);    //����ĳ��ֵ

    XVector2 operator + (const XVector2& temp) const;
    XVector2 operator - (const XVector2& temp) const;
	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
    XVector2 operator * (const XVector2& temp) const;	
    XVector2 operator / (const XVector2& temp) const;
    XVector2 operator + (const float& temp) const;
    XVector2 operator - (const float& temp) const;
 	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
	XVector2 operator * (const float& temp) const;
    XVector2 operator / (const float& temp) const;
    void operator += (const XVector2& temp);
    void operator -= (const XVector2& temp);
    void operator *= (const XVector2& temp);
    void operator /= (const XVector2& temp);
    void operator += (const float & temp);
    void operator -= (const float & temp);
    void operator *= (const float & temp);
    void operator /= (const float & temp);

//	XVector2& operator = (const XVector2I & temp);
//	XVector2(const XVector2I & temp);

    float getLength(const XVector2& P0) const;     //��ȡ������֮��ľ���
    float getLength(float a,float b) const;			//��ȡ������֮��ľ���
	float getLength() const;						//��õ�ԭ��ľ���
	float getLengthSqure(const XVector2& P0) const;//��ȡ����֮������ƽ��
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;					//��õ�ԭ��ľ����ƽ��
    float getAngle(const XVector2& P0) const;      //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
    float getAngle(float a,float b) const;          //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
	float getAngle() const;							//��ȡԭ�㵽��ǰ��ĽǶ�

	float dot(const XVector2 &temp) const {return (x * temp.x + y * temp.y);}
	//��ˣ�Ҳ���������ڻ���������������˼�壬�������Ľ����һ������ 
	//����� (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//���������������ʸ���ļн�,��ʽ����: 
	//cos (V dot W) =V.W / | V | | W | 
	//��˵ļ��������ǣ���һ��������һ���ߵ�ͶӰ������һ���ߵĳ���
	float cross(const XVector2 &temp)const{return x * temp.y - y * temp.x;}	//CROSS product
	void normalize();	//��λ�� ���� ��һ��
	XVector2 perp(){return XVector2(y,-x);}	//����
    
    XVector2::XVector2(float a,float b)
		:x(a),y(b)
	{}
	XVector2::XVector2()
		:x(0.0f),y(0.0f)
	{}
	XVector2(const XVector2 &temp)
		:x(temp.x),y(temp.y)
	{}
//	XVector2 &operator = (const XVector2 &temp);
	XVector2 &operator = (const float* temp);	//�����߼�������
	XVector2(const float *temp) //�����߼�������
	{
		if(temp == NULL)
		{
			x = 0.0f;
			y = 0.0f;
		}else
		{
			x = temp[0];
			y = temp[1];
		}
	}
	//-v
	XVector2 operator - () const;
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};

typedef XVector2 XPoint;

#if WITH_INLINE_FILE
#include "XVector2.inl"
#endif

inline XVector2 operator *(float a,const XVector2 &temp)
{//���
	return temp * a;
}
//inline float operator^(const XVector2 &v1,const XVector2 &v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
namespace XMath{
//�����߶�AB��BC�ļн�,0 - PI2,ǰ���������ߵĽ�
inline float angleBetween(const XVector2 &A,const XVector2 &B,const XVector2 &C)
{//δ��������
	XVector2 tmp1 = B - A;
	XVector2 tmp2 = C - B;
    double sin = tmp1.x * tmp2.y - tmp2.x * tmp1.y;  
    double cos = tmp1.x * tmp2.x + tmp1.y * tmp2.y;
	return XMath::angleAbbreviation(atan2(sin, cos));
}
//�����߶�AB��BC�ļн�,0 - PI,ǰ���������ߵĽ�
inline float getAngle(const XVector2 &A,const XVector2 &B,const XVector2 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline XVector2 projector(const XVector2 &v1,const XVector2 &v2)
{//��v1������v2ͶӰ�γɵ�����
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
inline XVector2 normalize(const XVector2 &v)
{
	float r = v.getLengthSqure();
	if(r != 0) return v * (1.0f / sqrt(r));
	return XVector2::zero;
}
}
}
#endif