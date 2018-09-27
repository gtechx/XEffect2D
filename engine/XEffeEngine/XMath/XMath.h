#ifndef _JIA_XMATH_
#define _JIA_XMATH_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//����һЩ��������ѧ�㷨
//floor();	//����ȡ��
//ceil();	//����ȡ��
//round();	//��������ȡ��
#include "XCommonDefine.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <complex>
#include <iostream>
#include <math.h>
namespace XE{
class XVec2;
class XVec3;
#define PI (3.1415926535897932384626433832795f)
#define PI_HALF (1.5707963267948966192313216916395f)							//(PI * 0.5)
#define PI2 (6.283185307179586476925286766558f)									//(PI * 2.0)
#define RADIAN2DEGREE (57.295779513082320876798154814114f)						//(180.0 / PI)
#define DEGREE2RADIAN (0.01745329251994329576923690768488f)						//(PI / 180.0)
#define SIN45 (0.70710678118654752440084436210485f)
#define SQRT3 (1.7320508075688772935274463415059f)								//(3 ^ 0.5)
#define SQRT2 (1.4142135623730950488016887242097f)								//(2 ^ 0.5)
#define ONE_THIRD (0.333333f)	//1/3
#define TWO_THIRD (0.666666f)	//2/3

//#define XEE_Max(a,b) (((a) > (b)) ? (a) : (b))
//#define XEE_Min(a,b) (((a) < (b)) ? (a) : (b))

class XLine;
class XRect;
namespace XMath
{
	//��ȡf�Ľ������ݣ����ƾ���Ϊlen
	//f = 0.0123456 len = 2;
	//���� 1.23 ex = 2
	//Ҳ���� 0.0123456 = 1.23 * 10^-2 ,����ȡ��ת���ɿ�ѧ������
	extern float getApproximateData(float f, int len, int &ex);
	//������һ��sin���Ż��㷨
	inline double sinTay(double x)
	{
		double x2 = x * x;

		const static double a1 = 0.99999748719198;
		const static double a3 = -0.166651680984443;
		const static double a4 = 0.00830951691334793;
		const static double a5 = -0.000184472198573026;
		return x*(a1 + (a3 + (a4 + a5*x2) * x2) * x2);
	}
	inline double cosTay(double x)
	{
		double cos = 0.0;
		//compute cosine: sin(x + PI/2) = cos(x)
		x += 1.57079632;
		if (x > 3.14159265) x -= 6.28318531;
		if (x < 0)
		{
			cos = 1.27323954 * x + 0.405284735 * x * x;
			if (cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
			else cos = 0.225 * (cos * cos - cos) + cos;
		}
		else
		{
			cos = 1.27323954 * x - 0.405284735 * x * x;
			if (cos < 0) cos = 0.225 * (cos *-cos - cos) + cos;
			else cos = 0.225 * (cos * cos - cos) + cos;
		}
		return cos;
	}
	//˫���Բ�ֵ�ļ��㹫ʽ
	inline float bilinear(float a, float b, float c, float d, float rate0, float rate1)
	{
		return a * (1.0f - rate0) * (1.0f - rate1) + (1.0f - rate0) * rate1 * b
			+ rate0 * (1.0f - rate1) * c + rate0 * rate1 * d;
	}
	//��������ȡ��
	inline int toInt(float f) { return int(f + 0.5f); }
	//����ȡ��
	inline int toIntFloor(float f) { return (int)floorf(f); }
	//����ȡ��
	inline int toIntCeil(float f) { return (int)ceilf(f); }
	//��Ҫ������Ӧ����ʱ�����������С�����ű���
	extern float getMinRate(const XVec2& a, const XVec2& b);
	//�ǶȻ���
	//extern float getAngleXY(float x,float y);	//���ڻ��x,y���ɵĽǶ�
	inline float getAngleXY(float x, float y) { return atan2(y, x); }
	//���ط�Χ0.0 - PI2
	inline float angleAbbreviation(float angle)	//���ȽǶȻ���
	{
		angle = fmod(angle, PI2);
		if (angle < 0.0f) angle += (float)(PI2);
		return angle;
	}
	//���ط�Χ0 - 360
	inline float angleReduce(float angle)		//�Ƕȼ򻯵�λ:��
	{
		angle = fmod(angle, 360.0f);
		if (angle < 0.0f) angle += 360.0f;
		return angle;
	}
	template<typename T>
	bool isInArea(const T& value, const T& x, const T& y)	//�ж�value�Ƿ���x��y�ķ�Χ��
	{
		if (x < y) return (value >= x && value <= y);
		else return (value >= y && value <= x);
	}
	template<typename T>
	bool isInAreaEx(const T& value, const T& maxV, const T& minV)	//�ж�value�Ƿ���b��s�ķ�Χ��
	{
		return (value >= minV && value <= maxV);
	}
	template<class T>
	void swapTwoSum(T& sum0, T& sum1)
	{//��������������ֵ
		std::swap(sum0, sum1);
	}
	//�ж�angle0�Ƿ���angle1 + maxArea��angle1 + minArea֮��
	//minArea��maxArea����Ϊ-180��+180��֮�䣬���в�����λΪ����
	inline bool isAngleInAreaR(float angle0, float angle1, float minArea, float maxArea)
	{
		if (minArea < 0.0f)
		{
			minArea += 360.0f;
			if (minArea > maxArea) swapTwoSum(minArea, maxArea);
			return !isInArea(angleReduce(angle0 - angle1), minArea, maxArea);
		}
		else
		{
			if (minArea > maxArea) swapTwoSum(minArea, maxArea);
			return isInArea(angleReduce(angle0 - angle1), minArea, maxArea);
		}
	}
	//�����ǶȲ�ľ���ֵ���ط�ΧΪ0-180
	inline float angleAbsDistance(float a, float b)
	{
		float ret = fmod(a - b, 360.0f);
		if (ret < 0.0f) ret += 360.0f;
		if (ret > 180.0f) ret = 360.0f - ret;
		return ret;
	}
	//���������Ƕ�֮��Ĳ�ֵ����Χ-180 - 180
	inline float angleDistance(float a, float b)
	{
		float ret = fmod(a - b, 360.0f);
		if (ret < 0.0f) ret += 360.0f;
		if (ret > 180.0f) ret -= 360.0f;
		return ret;
	}
	//�����ݽ��м�ֵ
	template<typename T>
	T clamp(T value, T min, T max)
	{
		return value < min ? min : value > max ? max : value;
	}
	//�������麯�����ڴ���Բ�ܵ�����ӳ�䣬�����޼�������һ��Ϊ16384����ת����s��e�ķ�Χ��ʱ��
	//��Ҫ���㵱ǰc�Ƕ�����ڷ�Χ�е�ĽǶ�ʱ����ʹ������ĺ���
	extern float circleLineMapping(float in, float inStart, float inEnd, float inMin, float inMax, float outStart = 0.0f, float outEnd = 1.0f, float outMin = 0.0f, float outMax = 1.0f);
	//����������������������������ʽ�����ֵ�Է�Χ�е�Ϊ0�㣬��ֵ����ϵ��
	inline float circleLineMappingEx(float in, float inStart, float inEnd, float inMin, float inMax, float outRate)
	{
		float ret = circleLineMapping(in, inStart, inEnd, inMin, inMax);
		float rate;
		if (inEnd >= inStart) rate = inEnd - inStart;
		else rate = inEnd - inMin + inMax - inStart;
		return (ret - 0.5f) * outRate * rate;
	}
	//������������������һ�ִ���ʽ�����ص�������-180 �� +180����������Ӧ���� minAngle - maxAngle ֮��
	inline float circleAngle(float middleAngle, float minAngle, float maxAngle, float curAngle)
	{//���������������û������
		if (minAngle >= maxAngle) return 0.0f;	//�Ƿ�������
		if (middleAngle < minAngle || middleAngle > maxAngle) return 0.0f;	//�Ƿ�������
		if (curAngle < minAngle || curAngle > maxAngle) return 0.0f;		//�Ƿ�������
		float half = (maxAngle - minAngle) * 0.5f;
		if (curAngle < middleAngle)
		{
			if (curAngle >= middleAngle - half)
			{
				return (curAngle - middleAngle) * 180.0f / half;
			}
			else
			{//������
				return (curAngle - minAngle + maxAngle - middleAngle) * 180.0f / half;
			}
		}
		else
		{
			if (curAngle <= middleAngle + half)
			{
				return (curAngle - middleAngle) * 180.0f / half;
			}
			else
			{//������
				return (minAngle - (maxAngle - curAngle) - middleAngle) * 180.0f / half;
			}
		}
	}
	//������in��imin��imax�ռ�����ӳ�䵽omin��omax�ռ䡣ע�⣺��������������淽���ӳ��
	//����������䶼����ͷ��β
	template<typename T> T mapping1D(T in, T imin, T imax, T omin, T omax)//һάӳ��
	//inline double mapping1D(double in, double imin, double imax, double omin, double omax)//һάӳ��
	{
		//if(fabs(xmax - xmin) < FLT_EPSILON) return ymin;		//���ݷǷ�
		if (abs(imax - imin) < 1.192092896e-07) return omin;	//���ݷǷ�
		//����1
		//float ret = (in - imin) / (imax - imin) * (omax - omin) + omin;
		//if(ret < omin) return omin;
		//if(ret > omax) return omax;
		//return ret;
		//����2
		if (in <= imin) return omin;
		if (in >= imax) return omax;
		return (in - imin) * (omax - omin) / (imax - imin) + omin;
	}
	//����������䶼����ͷ��β
	inline int mapping1D(int in, int imin, int imax, int omin, int omax)//һάӳ��
	{
		if (imax - imin == 0) return omin;	//���ݷǷ�
		if (in <= imin) return omin;
		if (in >= imax) return omax;
		//�����޸�Ϊ�ȳ��ٳ����Ա��ڶ���������һ���ϺõĽ��,+1��Ϊ�˰�����ͷ
		return (in - imin) * (omax - omin + 1) / (imax - imin) + omin;
	}
	//�����mapping1D��������ֵ���ǳ���Χ���Ƶģ�ע����������������淽���ӳ��
	inline float mapping1DEx(float in, float imin, float imax, float omin, float omax)//һάӳ��
	{
		if (fabs(imax - imin) < 1.192092896e-07F) return omin;	//���ݷǷ�
		return (in - imin) * (omax - omin) / (imax - imin) + omin;
	}
	//����������
	inline float quadraticBezier(float x, float a, float b)
	{
		// adapted from BEZMATH.PS (1993)
		// by Don Lancaster, SYNERGETICS Inc.
		// http://www.tinaja.com/text/bezmath.html

		float epsilon = 0.00001f;
		a = (std::max)(0.0f, (std::min)(1.0f, a));
		b = (std::max)(0.0f, (std::min)(1.0f, b));
		if (a == 0.5f)
			a += epsilon;

		// solve t from x (an inverse operation)
		float om2a = 1.0f - 2.0f * a;
		float t = (sqrt(a * a + om2a * x) - a) / om2a;
		return (1.0f - 2.0f * b) * (t * t) + (2.0f * b) * t;
	}
	//������ĺ�������һ����ֻ���Ƿ����Ե�ӳ�䣬���Ǳ��������ε�ӳ�䷽ʽ
	//pa��pbΪ���������ߵĲ���,�������ֵ�����ڷ�Χ��
	inline float mapping1DQBz(float in, float imin, float imax, float omin, float omax, float pa, float pb)
	{
		if (fabs(imax - imin) <= 1.192092896e-07F ||
			fabs(omin - omax) <= 1.192092896e-07F) return omin;	//���ݷǷ�
		return XMath::clamp(quadraticBezier((in - imin) / (imax - imin), pa, pb), 0.0f, 1.0f) *
			(omax - omin) + omin;
	}
	//0   1
	//3   2
	//ע��:��Ҫ������֤���������Ϊ͹����Σ������������⡣���ڰ��������Ҫ����ϸ����
	//����������ڴ�������2D�ռ��еĿռ�ӳ�����⣬�ı��οռ�
	extern XVec2 mapping2D(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//�������㷨�������(����ĺ�������ȱ�ݣ�Ӧ��������ʹ��)
	extern XVec2 mapping2DPlus(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//0   1
	//3   2
	//����һ���ռ�ӳ�����һ��ʵ�ַ�ʽ��������ӳ�䣬������Ƭ���ƵĽ�����Ǻ�
	//���ǿռ��ӳ��
	extern XVec2 mapping2DTriangle(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);
	//ͨ����������������ι��ɵ��ı��οռ�(���ַ�������û��mapping2Dƽ���ȶ�)
	extern XVec2 mapping2DEx(const XVec2 *inRect, const XVec2 *outRect, const XVec2& in);

	//�ı�����������ֵ�����ĸ��ǵ��������ɵ�����������������ĳ���������ʹ�����Բ�ֵ
	extern XVec2 vectorFieldLineSlerp(const XVec2 *posRect, const XVec2 *vectorRect, const XVec2& in);
	//�ı��α�������ֵ,��2D�ռ��һ������������֪�ı��νǵ�ı���ֵ�������ı����ڲ������ı���ֵʹ�����Բ�ֵ
	extern float scalarFieldLineSlerp(const XVec2 *posRect, const float *scalarRect, const XVec2& in);
	//���Բ�ֵ��s:��㣬e:�յ㣬r���ٷֱ�[0 - 1.0]
	template<typename T>
	T lineSlerp(const T& s, const T& e, float r) { return e * r + s * (1.0f - r); }
	//��ֵ x[0 - 1]
	template<typename T>
	T cSlerp(const T& u0, const T& u1, const T& u2, const T& u3, float x)
	{
		T p = (u3 - u2) - (u0 - u1);
		T q = (u0 - u1) - p;
		T r = u2 - u0;
		return x * (x * (x * p + q) + r) + u1;
	}
	//Ѱ�ұ�temp�����С�ķ���2��n�η��ĳߴ�
	inline int getMinWellSize2n(int temp) { return (int)powf(2.0, ceilf(logf((float)temp) / logf(2.0f))); }
	inline XBool isNPOT(int width, int height)		//�ж��Ƿ񲻷���2��n�η�
	{
		if (getMinWellSize2n(width) != width ||
			getMinWellSize2n(height) != height) return XTrue;
		return XFalse;
	}
	inline int fit4(int value)	//�������4�ֽڶ����ֵ
	{
		if (value % 4 == 0) return value;
		return value + 4 - (value % 4);
	}
	extern void vector4Sort(XVec2* v, XVec2* u);//���������е�����ת���ν���˳ʱ������
	//4�������Ҫ���Ѿ������˵�
	extern int getEdgePoint(float angle, XVec2 * points, const XVec2* v,
		const XVec2* u);	//ֻ�������ĵ�
	//��ȡһ�������ڵĵ�����εı���ָ���Ƕ���Χ�ɵĶ����(����ķ��������趨���ĸ���)
	extern int getEdgePointEx(const XRect& rect, const XVec2& point,
		float angle, XVec2 * points);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	extern int getEdgePointEx(const XRect& vRect, const XRect& uRect, const XVec2& vPoint,
		float angle, XVec2 * vPoints, XVec2 * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	extern int getEdgePointEx(const XRect& rect, const XVec2& point,
		float angle, float * points);
	extern int getEdgePointEx(const XRect& vRect, const XRect& uRect, const XVec2& vPoint,
		float angle, float * vPoints, float * uPoints);	//�Ƕ�Ϊ12���ӷ���ʼ��ʱ��[0 - PI2]
	//+++++++����ֱ���˶��µ�һ�麯��++++++++
	//����ֵ -2�������� -1���ᷢ����ײ ret > 0
	//����ֱ���˶�������ײʱ��ļ���
	extern double getTwoLineTime(const XVec2& L1S, const XVec2& L1E, const XVec2& L2S, const XVec2& L2E, float distance, float timer);	//��ȡ�����߶�ĳ���������С��
	//����ֱ���˶�����̶������ײʱ�����
	extern double getLineToPointTime(const XVec2& L1S, const XVec2& L1E, const XVec2& point, float distance, float timer);
	//����ֱ���˶�����̶��߶ε���ײʱ�����
	extern double getObjectToLineTime(const XVec2& L1S, const XVec2& L1E, const XVec2& L2S, const XVec2& L2E, float distance, float timer);	//��ȡ�����߶�ĳ���������С��
	//�˶��������˶��߶ε���ײʱ�����
	//�˶��߶����˶��߶ε���ײʱ�����
	//+++++++�ȼ���ֱ���˶���һ�麯��++++++++
	//�ȼ���ֱ���˶���������ײʱ��ļ���(ʱ�䵥λͳһΪms)
	extern double getCrashTimerB2B(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, const XVec2& speed2, const XVec2& a2, float distance, float time);
	//�ȼ���ֱ���˶�������̶�����ײ��ʱ�����
	extern double getCrashTimerB2P(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& position2, float distance, float time);
	//�ȼ���ֱ���˶�������̶�ֱ����ײ��ʱ�����
	extern double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1,
		const XVec2& positionS, const XVec2& positionE, float distance, float time);
	//������������ж�XLineʹ��const˵����Ȼ�������֪��Ϊʲô��
	extern double getCrashTimerB2L(const XVec2& position1, const XVec2& speed1, const XVec2& a1, const XLine& pL, float distance, float time);
	//extern double getCrashTimerB2L(const XVec2& position1,const XVec2& speed1,const XVec2& a1,XLine& pL,float distance,float time);
	extern void printfR();
	//������дһЩ�����Ĺ�ʽ
	//�㵽ֱ�߾���Ĺ�ʽ
	extern double distancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/��(a^2+b^2)
	extern double distancePointToLine(const XVec2& point, double a, double b, double c);
	//�㵽ֱ�ߵĴ��㹫ʽ
	extern XVec2 curtatePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);	//��ֱ�߷���Ϊax+by+c=0,������Ϊ(m,n)����Ϊ((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
	extern XVec2 curtatePointToLine(const XVec2& point, double a, double b, double c);
	//��ֱ�߽���Ĺ�ʽ
	extern XVec2 crossPointLineToLine(const XVec2& line1S, const XVec2& line1E, const XVec2& line2S, const XVec2& line2E);
	extern XVec2 crossPointLineToLine(double a0, double b0, double c0, double a1, double b1, double c1);
	//����ֱ�߹�ʽ
	extern XBool twoPointLine(const XVec2& lineS, const XVec2& lineE, double *a, double *b, double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
	//�㵽�߶ε���̾���
	extern double minDistancePointToLine(const XVec2& point, const XVec2& lineS, const XVec2& lineE);
	//���߶ε���̾���
	extern XVec2 minDistanceTwoLine(const XVec2& line1S, const XVec2& line1E, const XVec2& line2S, const XVec2& line2E);
	//��ȡp�Ƿ���p1 p2 p3 p4Χ�ɵ��ı����ڲ�
	extern XBool getIsInRect(float x, float y, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4);
	extern XBool getIsInRect(const XVec2& p, const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4);
	//�жϵ�p�Ƿ���pR��Χ�ɵĶ�����ڲ�
	extern XBool getIsInPolygon(float x, float y, const XVec2 *pR, int pRSum);
	extern XBool getIsInPolygon(const XVec2& p, const XVec2 *pR, int pRSum);
	inline XBool getIsInPolygon(const XVec2& p, const std::vector<XVec2> &points)
	{
		return getIsInPolygon(p, &points[0], (int)(points.size()));
	}
	//�жϵ��Ƿ����������ڲ�
	extern XBool getIsInTriangle(const XVec2& p, const XVec2 *points);
	inline XBool getIsInTriangle(const XVec2& p, const std::vector<XVec2> &points)
	{
		if (points.size() < 3) return false;
		return getIsInTriangle(p, &points[0]);
	}
	//�����߶�����εĽ��㣬���ؽ���������o0Ϊ��һ�����㣬o1Ϊ�ڶ�������,���ؽ��������
	extern int getPointsLineToRect(const XVec2& s, const XVec2& e, const XRect& r, XVec2& o0, XVec2& o1);

	//�����õ��Ĵη��̵ĺ�����������������Ч����������Լ����������ֵĽ�
	extern int quarticEquation(
		double a, double b, double c, double d, double e,
		std::complex<double> &x1, std::complex<double> &x2,
		std::complex<double> &x3, std::complex<double> &x4);
	//���溯�����ٷ�װ�����ز����������ֵĽ�
	extern int quarticEquation(
		double a, double b, double c, double d, double e,
		double *x);
	//�����СΪs�ķ����ֵ��dataΪ����������
	//0,1,2
	//3,4,5
	//6,7,8
	extern double getValueMatrix(const double *d, int s);
	//������CRC16����
	//pΪ�ַ���
	//lenΪ�ַ�������
	extern unsigned short CRC16_Modbus(unsigned char *p, unsigned short len);
	extern unsigned short CRC16XModen(unsigned char *buf, unsigned short length);
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
		template<class type>void qSort(type *a, int low, int high)
		{//�������԰ٶ�
			if (low >= high) return;
			int first = low;
			int last = high;
			type key = a[first];	//���ֱ�ĵ�һ����¼��Ϊ����
			while (first < last)
			{
				while (first < last && a[last] >= key)
					--last;
				a[first] = a[last];	//���ȵ�һ��С���Ƶ��Ͷ�
				while (first < last && a[first] <= key)
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
		i = 0x5f375a86 - (i >> 1); // gives initial guess y0
		x = *(float*)&i; // convert bits BACK to float
		x = x*(1.5f - xhalf*x*x); // Newton step, repeating increases accuracy
		return x;
	}
	//������������ǲ������
	extern std::string to36Encode(const unsigned char *data, int len);				//��һ������ת����36���Ƶ��ַ�����0-9 + A-Z,�����ַ�������
	extern std::string to64Encode(const unsigned char *data, int len);				//��������ת����64�����ַ���0-9 A-Z a-z +=
	extern void to64Decode(unsigned char *data, int len, const std::string& code);	//��64�����ַ���ת��������
	//������Ҫʵ��һЩͼ��ѧ�Ļ����㷨
	//�ж�����ֱ���Ƿ��н���
	extern bool isLineIntersect(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//�ж�����������Ƿ��ཻ
	extern bool isPolygonIntersect(const std::vector<XVec2> &polygon0, const std::vector<XVec2> &polygon1);
	//��ȡ͹����ε����
	extern double getPolygonArea(const std::vector<XVec2> &polygon);
	//�������������ε����
	extern double getTriangleArea(const std::vector<XVec2> &triangle);
	//�ж��������������ߵĽ���
	//http://stackoverflow.com/questions/4039229/checking-if-two-cubic-b%C3%A9zier-curves-intersect
	//http://www.tsplines.com/technology/edu/CurveIntersection.pdf
	//http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-sub.html
	//��ȡ���������������Լ��
	extern int getGCM(int a, int b);
	//���NԪ���η�����
	//inData������ϵ����nԪһ�η���Ӧ����n*��n+1����ϵ��
	//xSum��δ֪��������
	//outData�����
	//������������Ƿ�ɹ�,Ŀǰ�������û�о�����ϸ�Ĳ��ԣ�ֻ�����˳�������
	extern bool calMPEquation(std::vector<double> &inData, int xSum, std::vector<double> &outData);
	//�����߽������²���
	//inLines�������ɵ㹹�ɵ���
	//outLines�����²���֮�����
	//len�����²����ĵ���
	//isLoop�����Ƿ�պ�
	extern void resampleLines(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, float len, bool isLoop);
	extern void resampleLinesP(const std::vector<XVec2> &inLines,
		std::vector<XVec2> &outLines, int pointSum, bool isLoop);
	//���ص�һλ��Чλ֮���ֵ���磺53.123������50�����0.0123������0.01
	//fData�������λ����ֵ
	extern double getTheFirstData(double data, int &fData);
	//�����굽ֱ������ϵ(�ѿ�������ϵ)��ת��
	extern XVec2 polar2Cart(float len, float angle);//angle�ĵ�λ�ǻ���
	//�ڵ����а��վ������򣬷���ָ�����еĵ������꣬���������ͬ��������Ϸ���ʼ˳ʱ������
	//��Ŵ�0��ʼ��0Ϊ�����
	//����Ŀǰû�ж�����ķ����ҵ�ѧ�����㷨����������ķ�ʽ���������ƽ�
	extern XVec2 get2DOrderIndex(int order);
	extern XVec3 get3DOrderIndex(int order);

	//������һЩ��Ҫ������δʵ�ֵĽӿ�
	//�ж������߶��Ƿ��ཻ
	extern bool isLineSegmentIntersect(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//���������߶εĽ���
	extern bool getCrossPoint2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1, XVec2& o);
	//���������߶ε���С����(��δʵ��)
	extern float minDistance2LineSegment(const XVec2& s0, const XVec2& e0, const XVec2& s1, const XVec2& e1);
	//�����߶����ı��εĽ���(��δʵ��)
	//��min��max֮��ȡ������ظ����е�num��������randArray��
	extern bool getRandomArray(std::vector<int> &randArray, int num, int _min, int _max);
	//����һ�ֹ��ܵ���һ��ʵ��
	extern bool getRandomArrayEx(std::vector<int> &randArray, int num, int _min, int _max);
}
class X2DOrderTable
{
private:
	class XTableInfo
	{
	public:
		int x;
		int y;
		int sum;
		int order;
	public:
		//XTableInfo() {}
		XTableInfo(int _x, int _y)
			:x(_x)
			, y(_y)
		{
			order = x * x + y * y;
		}
		bool operator < (const XTableInfo &m) const
		{
			return order < m.order;
		}
	};
	std::vector<XTableInfo> m_table;
	struct XFullTableInfo
	{
		short x;
		short y;
	};
	std::vector<XFullTableInfo> m_fullTable;
	int m_isInited;
	bool m_isFullTable;

	void _init(std::vector<XTableInfo> &table, int size);
	static const XVec2 m_order4[4];
public:
	X2DOrderTable()
		:m_isInited(false)
		, m_isFullTable(false)
	{}
	//���ٵ��ڴ�ռ�
	void initTable(int size)
	{
		if (m_isInited || size <= 0) return;
		_init(m_table, size);
		m_isInited = true;
	}
	//������ٶ�
	int getTableSize()const;
	void initFullTable(int size);
	XE::XVec2 get2DOrderIndex(int index);
};
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