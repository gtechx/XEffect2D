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
#include <complex>
#include <iostream>
#include <math.h>
namespace XE{
class XVector2;
#define PI (3.1415926535897932384626433832795f)
#define PI_HALF (1.5707963267948966192313216916395f)							//(PI * 0.5)
#define PI2 (6.283185307179586476925286766558f)									//(PI * 2.0)
#define RADIAN2DEGREE (57.295779513082320876798154814114f)						//(180.0 / PI)
#define DEGREE2RADIAN (0.01745329251994329576923690768488f)						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)
#define SQRT3 (1.7320508075688772935274463415059f)								//(3 ^ 0.5)
#define SQRT2 (1.4142135623730950488016887242097f)								//(2 ^ 0.5)

#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

class XLine;
class XRect;
namespace XMath
{
	//��ȡf�Ľ������ݣ����ƾ���Ϊlen
	//f = 0.0123456 len = 2;
	//���� 1.23 ex = 2
	//Ҳ���� 0.0123456 = 1.23 * 10^-2 ,����ȡ��ת���ɿ�ѧ������
	extern float getApproximateData(float f,int len,int &ex);
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
	//˫���Բ�ֵ�ļ��㹫ʽ
	inline float bilinear(float a,float b,float c,float d,float rate0,float rate1)
	{
		return a * (1.0f - rate0) * (1.0f - rate1) + (1.0f - rate0) * rate1 * b
			+ rate0 * (1.0f - rate1) * c + rate0 * rate1 * d;
	}
	//��������ȡ��
	inline int toInt(float f) {return int(f + 0.5f);}
	//����ȡ��
	inline int toIntFloor(float f) {return (int)floorf(f);}
	//����ȡ��
	inline int toIntCeil(float f) {return (int)ceilf(f);}
	//��Ҫ������Ӧ����ʱ�����������С�����ű���
	extern float getMinRate(const XVector2 &a,const XVector2 &b);
	//�ǶȻ���
	extern float getAngleXY(float x,float y);	//���ڻ��x,y���ɵĽǶ�
	inline float angleAbbreviation(float angle)	//���ȽǶȻ���
	{
		angle = fmod(angle,PI2);
		if(angle < 0.0f) angle += (float)(PI2);
		return angle;
	}
	inline float angleReduce(float angle)		//�Ƕȼ򻯵�λ:��
	{
		angle = fmod(angle,360.0f);
		if(angle < 0.0f) angle += 360.0f;
		return angle;
	}
	inline float clamp(float value, float min, float max) 
	{
		return value < min ? min : value > max ? max : value;
	}
	//������in��imin��imax�ռ�ӳ�䵽omin��omax�ռ䡣
	inline float maping1D(float in,float imin,float imax,float omin,float omax)//һάӳ��
	{
		//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//���ݷǷ�
		if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//���ݷǷ�
		//����1
		//float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
		//if(ret < omin) return omin;
		//if(ret > omax) return omax;
		//return ret;
		//����2
		if(in <= imin) return omin;
		if(in >= imax) return omax;
		return (in - imin) / (imax - imin) * (omax - omin) + omin;
	}
	//�����maping1D��������ֵ���ǳ���Χ���Ƶ�
	inline float maping1DEx(float in,float imin,float imax,float omin,float omax)//һάӳ��
	{
		if(fabs(imax - imin) < 1.192092896e-07F) return omin;	//���ݷǷ�
		return (in - imin) / (imax - imin) * (omax - omin) + omin;
	}
	template<typename T> 
	bool isInArea(T value,T x,T y)//�ж�value�Ƿ���x��y�ķ�Χ��
	{
		if(x < y) return (value >= x && value <= y);
		else return (value >= y && value <= x);
	}
	//0   1
	//3   2
	//ע��:��Ҫ������֤���������Ϊ͹����Σ������������⡣���ڰ��������Ҫ����ϸ����
	//����������ڴ�������2D�ռ��еĿռ�ӳ������
	extern XVector2 maping2D(const XVector2 *inRect,const XVector2 *outRect,const XVector2 &in);
	//�ı�����������ֵ�����ĸ��ǵ��������ɵ�����������������ĳ���������ʹ�����Բ�ֵ
	extern XVector2 vectorFieldLineSlerp(const XVector2 *posRect,const XVector2 *vectorRect,const XVector2 &in);
	//�ı��α�������ֵ,��2D�ռ��һ������������֪�ı��νǵ�ı���ֵ�������ı����ڲ������ı���ֵʹ�����Բ�ֵ
	extern float scalarFieldLineSlerp(const XVector2 *posRect,const float *scalarRect,const XVector2 &in); 
	//���Բ�ֵ��s:��㣬e:�յ㣬r���ٷֱ�[0 - 1.0]
	template<typename T> 
	T lineSlerp(const T &s,const T &e,float r) {return e * r + s * (1.0f  - r);}
	//��ֵ x[0 - 1]
	template<typename T> 
	T cSlerp(const T &u0,const T &u1,const T &u2,const T &u3,float x)
	{
		T p = (u3 - u2) - (u0 - u1);
		T q = (u0 - u1) - p;
		T r = u2 - u0;
		return x * (x * (x * p + q) + r) + u1;
	}
	//Ѱ�ұ�temp�����С�ķ���2��n�η��ĳߴ�
	inline int getMinWellSize2n(int temp) {return (int)powf(2.0, ceilf(logf((float)temp)/logf(2.0f)));}
	inline XBool isNPOT(int width, int height)		//�ж��Ƿ񲻷���2��n�η�
	{
		if(getMinWellSize2n(width) != width) return XTrue;
		if(getMinWellSize2n(height) != height) return XTrue;
		return XFalse;
	}
	extern void vector4Sort(XVector2* v,XVector2* u);//���������е�����ת���ν���˳ʱ������
	//4�������Ҫ���Ѿ������˵�
	extern int getEdgePoint(float angle,XVector2 * points,const XVector2* v,const XVector2* u);	//ֻ�������ĵ�
	//��ȡһ�������ڵĵ�����εı���ָ���Ƕ���Χ�ɵĶ����(����ķ��������趨���ĸ���)
	extern int getEdgePointEx(XRect &rect,XVector2 &point,float angle,XVector2 * points);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	extern int getEdgePointEx(XRect &vRect,XRect &uRect,XVector2 &vPoint,float angle,XVector2 * vPoints,XVector2 * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	extern int getEdgePointEx(XRect &rect,XVector2 &point,float angle,float * points);
	extern int getEdgePointEx(XRect &vRect,XRect &uRect,XVector2 &vPoint,float angle,float * vPoints,float * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	//+++++++����ֱ���˶��µ�һ�麯��++++++++
	//����ֵ -2�������� -1���ᷢ����ײ ret > 0
	//����ֱ���˶�������ײʱ��ļ���
	extern double getTwoLineTime(const XVector2& L1S,const XVector2& L1E,const XVector2& L2S,const XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
	//����ֱ���˶�����̶������ײʱ�����
	extern double getLineToPointTime(const XVector2& L1S,const XVector2& L1E,const XVector2& point,float distance,float timer);
	//����ֱ���˶�����̶��߶ε���ײʱ�����
	extern double getObjectToLineTime(const XVector2& L1S,const XVector2& L1E,const XVector2& L2S,const XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
	//�˶��������˶��߶ε���ײʱ�����
	//�˶��߶����˶��߶ε���ײʱ�����
	//+++++++�ȼ���ֱ���˶���һ�麯��++++++++
	//�ȼ���ֱ���˶���������ײʱ��ļ���(ʱ�䵥λͳһΪms)
	extern double getCrashTimerB2B(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& position2,const XVector2& speed2,const XVector2& a2,float distance,float time);
	//�ȼ���ֱ���˶�������̶�����ײ��ʱ�����
	extern double getCrashTimerB2P(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& position2,float distance,float time);
	//�ȼ���ֱ���˶�������̶�ֱ����ײ��ʱ�����
	extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,
								   const XVector2& positionS,const XVector2& positionE,float distance,float time);
	//������������ж�XLineʹ��const˵����Ȼ�������֪��Ϊʲô��
	extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,const XLine& pL,float distance,float time);
	//extern double getCrashTimerB2L(const XVector2& position1,const XVector2& speed1,const XVector2& a1,XLine& pL,float distance,float time);
	extern void printfR();
	//������дһЩ�����Ĺ�ʽ
	//�㵽ֱ�߾���Ĺ�ʽ
	extern double distancePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/��(a^2+b^2)
	extern double distancePointToLine(const XVector2& point,double a,double b,double c);
	//�㵽ֱ�ߵĴ��㹫ʽ
	extern XVector2 curtatePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);	//��ֱ�߷���Ϊax+by+c=0,������Ϊ(m,n)����Ϊ((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	extern XVector2 curtatePointToLine(const XVector2& point,double a,double b,double c);
	//��ֱ�߽���Ĺ�ʽ
	extern XVector2 crossPointTineToLine(const XVector2& line1S,const XVector2& line1E,const XVector2& line2S,const XVector2& line2E);
	extern XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1);
	//����ֱ�߹�ʽ
	extern XBool twoPointLine(const XVector2& lineS,const XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
	//�㵽�߶ε���̾���
	extern double minDistancePointToLine(const XVector2& point,const XVector2& lineS,const XVector2& lineE);
	//���߶ε���̾���
	extern XVector2 minDistanceTwoLine(const XVector2& line1S,const XVector2& line1E,const XVector2& line2S,const XVector2& line2E);
	//��ȡp�Ƿ���p1 p2 p3 p4Χ�ɵ��ı����ڲ�
	extern XBool getIsInRect(float x,float y,const XVector2& p1,const XVector2& p2,const XVector2& p3,const XVector2& p4);
	extern XBool getIsInRect(const XVector2& p,const XVector2& p1,const XVector2& p2,const XVector2& p3,const XVector2& p4);
	//�жϵ�p�Ƿ���pR��Χ�ɵĶ�����ڲ�
	extern XBool getIsInRect(float x,float y,const XVector2 *pR,int pRSum);
	extern XBool getIsInRect(const XVector2& p,const XVector2 *pR,int pRSum);
	//�����߶�����εĽ��㣬���ؽǵ�������o0Ϊ��һ���ǵ㣬o1Ϊ�ڶ����ǵ�,���ؽ��������
	extern int getPointsLineToRect(const XVector2 &s,const XVector2 &e,const XRect &r,XVector2 &o0,XVector2 &o1);

	//�����õ��Ĵη��̵ĺ�����������������Ч����������Լ����������ֵĽ�
	extern int quarticEquation(
						  double a,double b,double c,double d,double e,
						  std::complex<double> &x1,std::complex<double> &x2,
						  std::complex<double> &x3,std::complex<double> &x4
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
	extern unsigned short CRC16XModen(unsigned char *buf,unsigned short length);

	template<class type> void swapTwoSum(type &sum0, type &sum1)
	{//��������������ֵ
		type temp = sum0;
		sum0 = sum1;
		sum1 = temp;
	}
	inline float rectResize(float w, float h, float Rx, float Ry)	//����Rx��Ry���ɵĿ򣬽�w��h�Ŀ�������У�����ܷŽ�ȥ���򷵻�1.0f,���򷵻�����Ӧ��С
	{//�ú������ڴ���������Ӧ���������ŵı���
		if (w <= 0.0f || h <= 0.0f || Rx <= 0.0f || Ry <= 0.0f) return 1.0f;
		if (w <= Rx && h <= Ry) return 1.0f;
		float size = Rx / w;
		if (size < Ry / h) return size;
		else return Ry / h;
	}
	inline float squareFloat(float x) { return x * x; }	//��x��ƽ��
	//#define SIN_BOOK	//sin�����Ƿ�ʹ�ò��ֵ䷽ʽ
#ifdef SIN_BOOK
	extern void initSinData();
	extern float xSin(float x);
#endif
	//#define SQRT_BOOK	//�����Ƿ�ʹ�ò��ֵ䷽ʽ

#ifdef SQRT_BOOK
	extern void initSqrtData();
	extern float xSqrt(float x);
#endif
	namespace QSort
	{
		template<class type>void qSort(type *a,int low,int high)
		{//�������԰ٶ�
			if (low >= high) return;
			int first = low;
			int last = high;
			type key = a[first];	//���ֱ�ĵ�һ����¼��Ϊ����
			while (first<last)
			{
				while(first < last && a[last] >= key)
					--last;
				a[first] = a[last];	//���ȵ�һ��С���Ƶ��Ͷ�
				while(first < last && a[first] <= key)
					++first;
				a[last] = a[first];	//���ȵ�һ������Ƶ��߶�
			}
			a[first] = key;	//�����¼��λ
			qSort(a, low, first - 1);
			qSort(a, first + 1, high);
		}
	}
	//ͨ����������������������
	template<class type> void quickSort(type *datas, int sum)
	{//����ͨ����֤
		if (datas == NULL || sum <= 1) return;
		QSort::qSort(datas, 0, sum - 1);
	}
	//quake3�е��Ż��㷨����1.0/sqrt(x)
	inline float invSqrt(float x)
	{
		float xhalf = 0.5f*x;
		int i = *(int*)&x; // get bits for floating VALUE 
		i = 0x5f375a86- (i>>1); // gives initial guess y0
		x = *(float*)&i; // convert bits BACK to float
		x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
		return x;
	} 
}
//������ѧ֪ʶ
//���ھ���Ļ���֪ʶ
//v����
//m����
//mt,ת�þ���Ϊ����m�����жԵ�
//m-1,����m���棬v*m = v1 �� v = v1 * m-1
//�����������ɣ�v*m1 * m2 = v * (m1 * m2)
//(m1 + m2) * m3 = m1 * m3 + m2 * m3
//k * (m1 * m2) = k * m1 * m2 =  m1 * k * m2	//k��ϵ��
//(m1 * m2)t = m1t * m2t
//�������󣬵������ת�þ���ʱ��
}
#endif