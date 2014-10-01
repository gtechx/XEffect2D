#ifndef _JIA_XMATH_
#define _JIA_XMATH_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//����һЩ��������ѧ�㷨
#include "XOSDefine.h"
#include <math.h>

#define PI 3.1415926535897932384626433832795f
#define PI_HALF 1.5707963267948966192313216916395f								//(PI * 0.5)
#define PI2 6.283185307179586476925286766558f									//(PI * 2.0)
#define RADIAN2DEGREE	57.295779513082320876798154814114f						//(180.0 / PI)
#define DEGREE2RADIAN	0.01745329251994329576923690768488f						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)

#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

//��ȡf�Ľ������ݣ����ƾ���Ϊlen
//f = 0.0123456 len = 2;
//���� 1.23 ex = 2
//Ҳ���� 0.0123456 = 1.23 * 10^-2 ,����ȡ��ת���ɿ�ѧ������
inline float getApproximateData(float f,int len,int &ex)
{
	ex = 0;
	if(f == 0.0f || len <= 0 || len >= 6) return 0.0f;
	float tmp = pow(10.0f,len);
	if(int (f) != 0)
	{//������
		for(ex = 0;ex < 10;++ ex)
		{
			if(int(f) == 0)
			{
				ex = ex - 1;
				if(f < 0.0f) return ceil(f * tmp * 10.0f) / tmp;
				else return floor(f * tmp * 10.0f) / tmp;
			}
			f *= 0.1f;
		}
	}else
	{//��С��
		for(ex = 0;ex < 10;++ ex)
		{
			if(int(f) != 0)
			{
				ex = -ex;
				if(f < 0.0f) return ceil(f * tmp) / tmp;
				else return floor(f * tmp) / tmp;
			}
			f *= 10.0f;
		}
	}
	return 0.0f;
}
//������һ��sin���Ż��㷨
inline double sinTay(double x)
{
    double x2 = x * x;
 
    const static double a1 = 0.99999748719198;
    const static double a3 = -0.166651680984443;
    const static double a4 = 0.00830951691334793;
    const static double a5 = -0.000184472198573026;
    return x*( a1+ ( a3 + ( a4 + a5*x2 ) * x2 ) * x2);  
}
inline double cosTay(double x) 
{
    double cos = 0.0;
    //compute cosine: sin(x + PI/2) = cos(x)
    x += 1.57079632;
    if(x > 3.14159265) x -= 6.28318531;
    if(x < 0) 
	{
        cos = 1.27323954 * x + 0.405284735 * x * x;
        if(cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
        else cos = 0.225 * (cos * cos - cos) + cos;
    }else
    {
        cos = 1.27323954 * x - 0.405284735 * x * x;
        if(cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
        else cos = 0.225 * (cos * cos - cos) + cos;
    }
    return cos;
}
//��������ȡ��
inline int toInt(float f) {return int(f + 0.5f);}
//����ȡ��
inline int toIntFloor(float f) {return floorf(f);}
//����ȡ��
inline int toIntCeil(float f) {return ceilf(f);}

//�ǶȻ���
extern float getAngleXY(float x,float y);	//���ڻ��x,y���ɵĽǶ�
inline float angleAbbreviation(float angle)	//���ȽǶȻ���
{
	angle = fmod(angle,PI2);
	if(angle < 0.0f) angle += (float)(PI2);
//	while(true)
//	{
//		if(angle >= PI2) angle -= (float)(PI2);
//		else if(angle < 0) angle += (float)(PI2);
//		else break;
//	}
	return angle;
}
inline float angleReduce(float angle)		//�Ƕȼ򻯵�λ:��
{
	angle = fmod(angle,360.0f);
	if(angle < 0.0f) angle += 360.0f;
	return angle;
}
inline float maping1D(float in,float imin,float imax,float omin,float omax)//һάӳ��
{
	//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//���ݷǷ�
	if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//���ݷǷ�
	float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
	if(ret < omin) return omin;
	if(ret > omax) return omax;
	return ret;
}
class _XVector2;
//0   1
//3   2
//ע��:��Ҫ������֤���������Ϊ͹����Σ������������⡣���ڰ��������Ҫ����ϸ����
extern _XVector2 maping2D(const _XVector2 *inRect,const _XVector2 *outRect,const _XVector2 &in);

template<typename T> 
T lineSlerp(const T &s,const T &e,float r)//���Բ�ֵ��s:��㣬e:�յ㣬r���ٷֱ�[0 - 1.0]
{
	return e * r + s * (1.0f  - r);
}
//��ֵ x[0 - 1]
template<typename T> 
T cSlerp(const T &u0,const T &u1,const T &u2,const T &u3,float x)
{
	T p = (u3 - u2) - (u0 - u1);
	T q = (u0 - u1) - p;
	T r = u2 - u0;
	return x * (x * (x * p + q) + r) + u1;
}
///////////////////////////////////////////////////////////
inline int getMinWellSize2n(int temp)			//Ѱ�ұ�temp�����С�ķ���2��n�η��ĳߴ�
{
	return (int)powf(2.0, ceilf(logf((float)temp)/logf(2.0f)));
	//int i = 1;
	//while(true)
	//{
	//	if(i > temp) break;
	//	i = i << 1;
	//}
	//return i;
}
inline _XBool isNPOT(int width, int height)		//�ж��Ƿ񲻷���2��n�η�
{
	if((int)powf(2.0f,ceilf(logf((float)width)/logf(2.0f))) != width) return XTrue;
	if((int)powf(2.0f,ceilf(logf((float)height)/logf(2.0f))) != height) return XTrue;
	return XFalse;
}

class _XLine;
class _XRect;

extern void vector4Sort(_XVector2* v,_XVector2* u);//���������е�����ת���ν���˳ʱ������
//4�������Ҫ���Ѿ������˵�
extern int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u);	//ֻ�������ĵ�
//��ȡһ�������ڵĵ�����εı���ָ���Ƕ���Χ�ɵĶ����(����ķ��������趨���ĸ���)
class _XRect;
extern int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,_XVector2 * points);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
extern int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,_XVector2 * vPoints,_XVector2 * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
extern int getEdgePointEx(_XRect &rect,_XVector2 &point,float angle,float * points);
extern int getEdgePointEx(_XRect &vRect,_XRect &uRect,_XVector2 &vPoint,float angle,float * vPoints,float * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]

//+++++++����ֱ���˶��µ�һ�麯��++++++++
//����ֵ -2�������� -1���ᷢ����ײ ret > 0
//����ֱ���˶�������ײʱ��ļ���
extern double getTwoLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
//����ֱ���˶�����̶������ײʱ�����
extern double getLineToPointTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& point,float distance,float timer);
//����ֱ���˶�����̶��߶ε���ײʱ�����
extern double getObjectToLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
//�˶��������˶��߶ε���ײʱ�����
//�˶��߶����˶��߶ε���ײʱ�����
//+++++++�ȼ���ֱ���˶���һ�麯��++++++++
//�ȼ���ֱ���˶���������ײʱ��ļ���(ʱ�䵥λͳһΪms)
extern double getCrashTimerB2B(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,const _XVector2& speed2,const _XVector2& a2,float distance,float time);
//�ȼ���ֱ���˶�������̶�����ײ��ʱ�����
extern double getCrashTimerB2P(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,float distance,float time);
//�ȼ���ֱ���˶�������̶�ֱ����ײ��ʱ�����
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& positionS,const _XVector2& positionE,float distance,float time);
//������������ж�_XLineʹ��const˵����Ȼ�������֪��Ϊʲô��
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,const _XLine& pL,float distance,float time);
//extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,_XLine& pL,float distance,float time);
extern void printfR();
//������дһЩ�����Ĺ�ʽ
//�㵽ֱ�߾���Ĺ�ʽ
extern double distancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/��(a^2+b^2)
extern double distancePointToLine(const _XVector2& point,double a,double b,double c);
//�㵽ֱ�ߵĴ��㹫ʽ
extern _XVector2 curtatePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);	//��ֱ�߷���Ϊax+by+c=0,������Ϊ(m,n)����Ϊ((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
extern _XVector2 curtatePointToLine(const _XVector2& point,double a,double b,double c);
//��ֱ�߽���Ĺ�ʽ
extern _XVector2 crossPointTineToLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
extern _XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1);
//����ֱ�߹�ʽ
extern _XBool twoPointLine(const _XVector2& lineS,const _XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
//�㵽�߶ε���̾���
extern double minDistancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);
//���߶ε���̾���
extern _XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
//��ȡp�Ƿ���p1 p2 p3 p4Χ�ɵ��ı����ڲ�
extern _XBool getIsInRect(float x,float y,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
extern _XBool getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
//�жϵ�p�Ƿ���pR��Χ�ɵĶ�����ڲ�
extern _XBool getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum);
extern _XBool getIsInRect(float x,float y,const _XVector2 *pR,int pRSum);

#include  <complex>
#include  <iostream>
#include  <math.h>
using namespace std;
//�����õ��Ĵη��̵ĺ�����������������Ч����������Լ����������ֵĽ�
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  );
//���溯�����ٷ�װ�����ز����������ֵĽ�
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  double *x);
//�����СΪs�ķ����ֵ��dataΪ����������
//0,1,2
//3,4,5
//6,7,8
extern double getValueMatrix(const double *d,int s);
//������CRC16����
//pΪ�ַ���
//lenΪ�ַ�������
extern unsigned short CRC16_Modbus(unsigned char *p,unsigned short len);
extern unsigned short CRC16_XModen(unsigned char *buf,unsigned short length);
#endif