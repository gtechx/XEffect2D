#ifndef _JIA_XVECTOR2_
#define _JIA_XVECTOR2_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XBasicFun.h"
//#include <math.h>
//#include "stdio.h"

//������2D��
class _XVector2
{
public:
    float x;
    float y;
public:
	static const _XVector2 zero;

    void reset();    //���������
	void set(const _XVector2 &temp);
    void set(float a,float b);    //���õ�ǰ���ֵ
	void set(float v) {set(v,v);}
    void add(float a,float b);    //����ĳ��ֵ

    _XVector2 operator + (const _XVector2& temp) const;
    _XVector2 operator - (const _XVector2& temp) const;
	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
    _XVector2 operator * (const _XVector2& temp) const;	
    _XVector2 operator / (const _XVector2& temp) const;
    _XVector2 operator + (const float& temp) const;
    _XVector2 operator - (const float& temp) const;
 	//������������������ѧ��û�����壬ֻ�ǳ����ϵı��
   _XVector2 operator * (const float& temp) const;
    _XVector2 operator / (const float& temp) const;
    void operator += (const _XVector2& temp);
    void operator -= (const _XVector2& temp);
    void operator *= (const _XVector2& temp);
    void operator /= (const _XVector2& temp);
    void operator += (const float & temp);
    void operator -= (const float & temp);
    void operator *= (const float & temp);
    void operator /= (const float & temp);

//	_XVector2& operator = (const _XVector2I & temp);
//	_XVector2(const _XVector2I & temp);

    float getLength(const _XVector2& P0) const;     //��ȡ������֮��ľ���
    float getLength(float a,float b) const;			//��ȡ������֮��ľ���
	float getLength() const;						//��õ�ԭ��ľ���
	float getLengthSqure(const _XVector2& P0) const;//��ȡ����֮������ƽ��
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;					//��õ�ԭ��ľ����ƽ��
    float getAngle(const _XVector2& P0) const;      //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
    float getAngle(float a,float b) const;          //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
	float getAngle() const;							//��ȡԭ�㵽��ǰ��ĽǶ�

	float dot(const _XVector2 &temp) const {return (x * temp.x + y * temp.y);}
	//��ˣ�Ҳ���������ڻ���������������˼�壬�������Ľ����һ������ 
	//����� (x1 , y1 , z1 ) .( x2 , y2 , z2 ) = x1x2 + y1y2 + z1z2 
	//���������������ʸ���ļн�,��ʽ����: 
	//cos (V dot W) =V.W / | V | | W | 
	//��˵ļ��������ǣ���һ��������һ���ߵ�ͶӰ������һ���ߵĳ���
	void normalize();	//��λ�� ���� ��һ��
	_XVector2 perp(){return _XVector2(y,-x);}	//����
    
    _XVector2::_XVector2(float a,float b)
		:x(a),y(b)
	{}
	_XVector2::_XVector2()
		:x(0.0f),y(0.0f)
	{}
	_XVector2(const _XVector2 &temp)
		:x(temp.x),y(temp.y)
	{}
//	_XVector2 &operator = (const _XVector2 &temp);
	_XVector2 &operator = (const float* temp);	//�����߼�������
	_XVector2(const float *temp) //�����߼�������
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
	_XVector2 operator - () const;
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};

typedef _XVector2 _XPoint;
#include "XVector2.inl"

inline _XVector2 operator *(float a,const _XVector2 &temp)
{//���
	return temp * a;
}
//inline float operator^(const _XVector2 &v1,const _XVector2 &v2)  // DOT product
//{
//	return v1.x*v2.x + v1.y*v2.y;
//}
//�����߶�AB��BC�ļн�,0 - PI,ǰ���������ߵĽ�
inline float getAngle(const _XVector2 &A,const _XVector2 &B,const _XVector2 &C)
{
	return acos(((B - A).dot(C - B))/sqrt((B - A).getLengthSqure() * (C - B).getLengthSqure()));
}
inline _XVector2 projector(const _XVector2 &v1,const _XVector2 &v2)
{//��v1������v2ͶӰ�γɵ�����
	return v2 * (v1.dot(v2) / v2.getLengthSqure());
}
#endif