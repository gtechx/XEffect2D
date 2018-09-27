#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBezierSpline.h"
namespace XE{
XBezierSpline::XBezierSpline()
	:m_curPosition(0.0f)
	, m_isEnd(XTrue)
	, m_curValue(0.0f)
	, m_speed(0.0f)
	, m_centerValue(0.5f)
	, m_partLenght(1.0f)
{}
float XBezierSpline::getBezierSplineAngle(float temp)
{
	XVec2 tempV;
	if(temp <= 0.0f)
	{
		tempV = m_pSP - m_pS;
		return tempV.getAngle();
	}
	if(temp >= 1.0f)
	{
		tempV = m_pE - m_pEP;
		return tempV.getAngle();
	}
	//�󵼹�ʽ����
	if(temp < 0.99f) tempV = getBezierSplineValue(temp + 0.01f) - getBezierSplineValue(temp);
	else tempV = getBezierSplineValue(temp) - getBezierSplineValue(temp - 0.01f);
	return tempV.getAngle();
}
void XBezierSpline::move(float timeDelay)
{
	if(m_isEnd) return;
	m_curPosition += m_speed * timeDelay;
	if(m_curPosition < 0.0f) m_curPosition = 0.0f;
	if(m_curPosition > 1.0f)
	{
		m_isEnd = XTrue;
		m_curPosition = 1.0f;
	}
	m_curValue = getBezierSplineValue(m_curPosition);
	m_curAngle = getBezierSplineAngle(m_curPosition);
}
void XBezierSpline::draw()
{
	XRender::drawLine(m_pS,m_pSP,1.0f,XFColor::red);
	XRender::drawLine(m_pEP,m_pE,1.0f,XFColor::red);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		XRender::drawLine(getBezierSplineValue(i * 0.02f),
			getBezierSplineValue((i + 1.0f) * 0.02f), 1, XFColor::blue);
	}
	XRender::drawLine(m_curValue,m_curAngle * RADIAN2DEGREE,20.0f);
	//drawLine(m_curValue,m_curValue + XVec2(1.0f,1.0f));
}
XBezierSpline2::XBezierSpline2()
	:m_curPosition(0.0f)
	, m_isEnd(XTrue)
	, m_curValue(0.0f)
	, m_speed(0.0f)
	, m_centerValue(0.5f)
	, m_partLenght(1.0f)
{}
float XBezierSpline2::getBezierSplineAngle(float temp)
{
	XVec2 tempV;
	if(temp <= 0.0f)
	{
		tempV = m_pP - m_pS;
		return tempV.getAngle();
	}
	if(temp >= 1.0f)
	{
		tempV = m_pE - m_pP;
		return tempV.getAngle();
	}
	//Ŀǰ����ʹ���󵼹�ʽ�ķ���
	if(temp < 0.99f) tempV = getBezierSplineValue(temp + 0.01f) - getBezierSplineValue(temp);
	else tempV = getBezierSplineValue(1.0f) - getBezierSplineValue(temp);
	return tempV.getAngle();
}
void XBezierSpline2::move(float timeDelay)
{
	if(m_isEnd) return;
	m_curPosition += m_speed * timeDelay;
	if(m_curPosition < 0.0f) m_curPosition = 0.0f;
	if(m_curPosition > 1.0f)
	{
		m_isEnd = XTrue;
		m_curPosition = 1.0f;
	}
	m_curValue = getBezierSplineValue(m_curPosition);
	m_curAngle = getBezierSplineAngle(m_curPosition);
}
void XBezierSpline2::draw()
{
	XRender::drawLine(m_pS,m_pP,1.0f,XFColor::red);
	XRender::drawLine(m_pP,m_pE,1.0f,XFColor::red);
	//drawLine(m_pS,m_pE);
	for(int  i = 0;i < 50;++ i)
	{
		XRender::drawLine(getBezierSplineValue(i * 0.02f),
			getBezierSplineValue((i + 1.0f) * 0.02f), 1, XFColor::blue);
	}
	XRender::drawLine(m_curValue, m_curAngle * RADIAN2DEGREE, 20.0f);
	//drawLine(m_curValue,m_curValue + XVec2(1.0f,1.0f));
}
double XBezierSpline::getLength(double t)  
{//�㷨������  
	//���ܳ��ȷ�Χ�ڣ�ʹ��simpson�㷨�ķָ���  
	#define TOTAL_SIMPSON_STEP  (512)  
	//�ָ����  
	int stepCounts = (int)(TOTAL_SIMPSON_STEP * t);  
	if(stepCounts & 1) ++ stepCounts; else    //��֤stepCountsΪż��
	if(stepCounts == 0) return 0.0;  

	double dStepH = t / stepCounts;  
	double dStep = dStepH * 2.0;  
	double sum1 = speed(dStepH);
	double sum2 = 0.0;
	double data = dStep;
	for(int i = 2; i < stepCounts;i += 2,data += dStep)  
	{  
		sum1 += speed(data + dStepH);  
		sum2 += speed(data); 
	}  
	return (m_startSpeed + m_endSpeed + 2.0f * sum2 + 4.0f * sum1) * dStep / 6.0;  
}
//double XBezierSpline::getLengthX(double t)  
//{//�㷨������  
//	#define TOTAL_SIMPSON_STEP  (512)  
//	//�ָ����  
//	int stepCounts = (int)(TOTAL_SIMPSON_STEP*t);
//	if (stepCounts & 1) ++stepCounts;    //ż��  
//	if (stepCounts == 0) return 0.0;
//
//	int halfCounts = stepCounts / 2;
//	double sum1 = 0.0, sum2 = 0.0;
//	double dStep = t / stepCounts;
//	for (int i = 0; i<halfCounts; ++i)
//	{
//		sum1 += speed((2 * i + 1)*dStep);
//	}
//	for (int i = 1; i<halfCounts; ++i)
//	{
//		sum2 += speed((2 * i)*dStep);
//	}
//	return (speed(0.0) + speed(1.0) + 2 * sum2 + 4 * sum1)*dStep / 3.0;
//}
double XBezierSpline::getT(double t)  
{  
	if (t <= 0.0f) return 0.0f;
	if (t >= 1.0f) return 1.0f;
	double len = t * m_length; //���������������,��ʱ��Ӧ�����߳���  
	//����1��//������
	//double t1 = t;  
	//double t2 = t1 - (getLength(t1) - len) / speed(t1);
	//double oldT = abs(t1 - t2);//���������ѭ�������߼��������ģ����Զ������������ֹ�޷�����
	//while(oldT > 0.001)  
	//{  
	//	t1 = t2;  
	//	t2 = t1 - (getLength(t1) - len) / speed(t1);  
	//	t = abs(t1 - t2);
	//	if(oldT <= t) break;
	//	oldT = t;
	//} 
	//����2��//С���ʲ��ɿ����ٶȿ�
	//double t1 = t, t2;
	//for (int i = 0; i < 256; ++i)
	//{
	//	t2 = t1 - (getLength(t1) - len) / speed(t1);
	//	if (abs(t1 - t2) < 0.001) break;
	//	t1 = t2;
	//}
	//����3:�ɿ��������ٶȽ���
	double left = 0.0;
	double right = 1.0;
	double t2 = t;
	for(int i = 0;i < 16;++ i)
	{//ʹ��2�ַ����ָ�16�ξ����㹻
		double tmpLen = getLength(t2);
		if (abs(tmpLen - len) < 0.001) break;	//��������Ҫ��
		if (tmpLen > len)
		{
			right = t2;
			t2 = (t2 + left) * 0.5;
		}
		else
		{
			left = t2;
			t2 = (t2 + right) * 0.5;
		}
	}

	return t2;
}
double XBezierSpline::getTEx(double t)
{
	if (!m_withOpti) return getT(t);
	if (t <= 0.0f) return 0.0f;
	if (t >= 1.0f) return 1.0f;
	double len = t * m_length; //���������������,��ʱ��Ӧ�����߳���  
	double step = m_tmpData.size() - 1.0;
	step = 1.0f / step;
	int index = t / step;

	double left = m_tmpData[index];
	double right = m_tmpData[index + 1];
	double t2 = t;
	for (int i = 0; i < 8; ++i)
	{//ʹ��2�ַ����ָ�16�ξ����㹻
		double tmpLen = getLength(t2);
		if (abs(tmpLen - len) < 0.001) break;	//��������Ҫ��
		if (tmpLen > len)
		{
			right = t2;
			t2 = (t2 + left) * 0.5;
		}
		else
		{
			left = t2;
			t2 = (t2 + right) * 0.5;
		}
	}

	return t2;
}
XBezierSpline XBezierSpline::getUpHalf()const	//��ȡ�ϰ벿������
{
	XBezierSpline tmp;
	XVec2 tmpSP = (m_pS + m_pSP) * 0.5f;
	//tmp.init(m_pS,tmpSP,((m_pS + m_pSP) * 0.5f + (m_pSP + m_pEP) * 0.5f) * 0.5f,getBezierSplineValue(0.5f));
	tmp.init(m_pS, tmpSP, (m_pS + m_pEP) * 0.25f + m_pSP * 0.5f, getBezierSplineValue(0.5f));
	tmp.m_partLenght = m_partLenght * 0.5f;
	tmp.m_centerValue = m_centerValue - m_partLenght * 0.25f;
	return tmp;
}
XBezierSpline XBezierSpline::getDownHalf()const	//��ȡ�ϰ벿������
{
	XBezierSpline tmp;
	XVec2 tmpEP = (m_pE + m_pEP) * 0.5f;
	tmp.init(getBezierSplineValue(0.5f), (m_pSP + m_pE) * 0.25f + m_pEP * 0.5f, tmpEP, m_pE);
	tmp.m_partLenght = m_partLenght * 0.5f;
	tmp.m_centerValue = m_centerValue + m_partLenght * 0.25f;
	return tmp;
}
void XBezierSpline2::calculate()
{
	ax = m_pS.x - 2.0*m_pP.x + m_pE.x;
	ay = m_pS.y - 2.0*m_pP.y + m_pE.y;
	bx = 2.0*m_pP.x - 2.0*m_pS.x;  
	by = 2.0*m_pP.y - 2.0*m_pS.y;  

	A = 4.0 * (ax*ax + ay*ay);
	B = 4.0 * (ax*bx + ay*by);
	C = bx*bx + by*by;
	sqrtA = sqrt(A);
	sqrtC = sqrt(C);
	D = log(B + 2.0 * sqrtA * sqrtC);
	powA = 1.0 / (8.0 * pow(A, 1.5));
	E = (B * B - 4.0 * A * C);
	D = D * E;
	m_length = getLength(1.0f);
}
double XBezierSpline2::getT(float t)  //���㳤�ȵ��ٷֱ�֮���ת��
{  
	float len = t * m_length;
	//����2��//С���ʲ��ɿ����ٶȿ�
	//float t1 = t,t2;  
	//for(int i = 0;i < 256;++ i)
	//{  
	//	t2 = t1 - (getLength(t1) - len) / speed(t1);
	//	if(abs(t1 - t2) < 0.001) break;  //�������
	//	t1 = t2;  
	//} 
	//����3:�ɿ��������ٶȽ���
	double left = 0.0;
	double right = 1.0;
	double t2 = t;
	for (int i = 0; i < 16; ++i)
	{//ʹ��2�ַ����ָ�16�ξ����㹻
		double tmpLen = getLength(t2);
		if (abs(tmpLen - len) < 0.001) break;	//��������Ҫ��
		if (tmpLen > len)
		{
			right = t2;
			t2 = (t2 + left) * 0.5;
		}
		else
		{
			left = t2;
			t2 = (t2 + right) * 0.5;
		}
	}
	return t2;
} 
XBezierSpline2 XBezierSpline2::getUpHalf()const	//��ȡ�ϰ벿������
{
	XBezierSpline2 tmp;
	tmp.init(m_pS, (m_pS + m_pP) * 0.5f, getBezierSplineValue(0.5f));
	tmp.m_partLenght = m_partLenght * 0.5f;
	tmp.m_centerValue = m_centerValue - m_partLenght * 0.25f;
	return tmp;
}
XBezierSpline2 XBezierSpline2::getDownHalf()const	//��ȡ�°벿������
{
	XBezierSpline2 tmp;
	tmp.init(getBezierSplineValue(0.5f),(m_pP + m_pE) * 0.5f,m_pE);
	tmp.m_partLenght = m_partLenght * 0.5f;
	tmp.m_centerValue = m_centerValue + m_partLenght * 0.25f;
	return tmp;
}
namespace XMath
{
void getBez2IntersectPoints(const XBezierSpline2 &bez0,const XBezierSpline2 &bez1,std::vector<XVec2> &points)
{
	if (XMath::isPolygonIntersect(bez0.getPolygon(), bez1.getPolygon()))
	{//���ܴ��ڽ���
		if (XMath::getPolygonArea(bez0.getPolygon()) < 1e-6 && 
			XMath::getPolygonArea(bez1.getPolygon()) < 1e-6)
		{//����ǰ���е��Ϊ����
			points.push_back(bez0.getBezierSplineValue(0.5f));
			return;
		}
		else
		{//ϸ����������������
			XBezierSpline2 u0 = bez0.getUpHalf();
			XBezierSpline2 u1 = bez1.getUpHalf();
			XBezierSpline2 d0 = bez0.getDownHalf();
			XBezierSpline2 d1 = bez1.getDownHalf();
			getBez2IntersectPoints(u0, u1, points);
			getBez2IntersectPoints(u0, d1, points);
			getBez2IntersectPoints(d0, u1, points);
			getBez2IntersectPoints(d0, d1, points);
		}
	}
	else
	{
		return;
	}
}
void getBez2IntersectPointsEx(const XBezierSpline2 &bez0,const XBezierSpline2 &bez1,std::vector<XVec2> &points)
{
	if (XMath::isPolygonIntersect(bez0.getPolygon(), bez1.getPolygon()))
	{//���ܴ��ڽ���
		if (XMath::getPolygonArea(bez0.getPolygon()) < 1e-6 &&
			XMath::getPolygonArea(bez1.getPolygon()) < 1e-6)
		{//����ǰ���е��Ϊ����
			points.push_back(XVec2(bez0.m_centerValue, bez1.m_centerValue));
			return;
		}
		else
		{//ϸ����������������
			XBezierSpline2 u0 = bez0.getUpHalf();
			XBezierSpline2 u1 = bez1.getUpHalf();
			XBezierSpline2 d0 = bez0.getDownHalf();
			XBezierSpline2 d1 = bez1.getDownHalf();
			getBez2IntersectPointsEx(u0, u1, points);
			getBez2IntersectPointsEx(u0, d1, points);
			getBez2IntersectPointsEx(d0, u1, points);
			getBez2IntersectPointsEx(d0, d1, points);
		}
	}
	else
	{
		return;
	}
}
void getBezIntersectPoints(const XBezierSpline &bez0,const XBezierSpline &bez1,std::vector<XVec2> &points)
{
	if (XMath::isPolygonIntersect(bez0.getPolygon(), bez1.getPolygon()))
	{//���ܴ��ڽ���
		if (XMath::getPolygonArea(bez0.getPolygon()) < 1e-6 &&
			XMath::getPolygonArea(bez1.getPolygon()) < 1e-6)
		{//����ǰ���е��Ϊ����
			points.push_back(bez0.getBezierSplineValue(0.5f));
			return;
		}
		else
		{//ϸ����������������
			XBezierSpline u0 = bez0.getUpHalf();
			XBezierSpline u1 = bez1.getUpHalf();
			XBezierSpline d0 = bez0.getDownHalf();
			XBezierSpline d1 = bez1.getDownHalf();
			getBezIntersectPoints(u0, u1, points);
			getBezIntersectPoints(u0, d1, points);
			getBezIntersectPoints(d0, u1, points);
			getBezIntersectPoints(d0, d1, points);
		}
	}
	else
	{
		return;
	}
}
void getBezIntersectPointsEx(const XBezierSpline &bez0,const XBezierSpline &bez1,std::vector<XVec2> &points)
{
	if (XMath::isPolygonIntersect(bez0.getPolygon(), bez1.getPolygon()))
	{//���ܴ��ڽ���
		if (XMath::getPolygonArea(bez0.getPolygon()) < 1e-6 && 
			XMath::getPolygonArea(bez1.getPolygon()) < 1e-6)
		{//����ǰ���е��Ϊ����
			points.push_back(XVec2(bez0.m_centerValue, bez1.m_centerValue));
			return;
		}
		else
		{//ϸ����������������
			XBezierSpline u0 = bez0.getUpHalf();
			XBezierSpline u1 = bez1.getUpHalf();
			XBezierSpline d0 = bez0.getDownHalf();
			XBezierSpline d1 = bez1.getDownHalf();
			getBezIntersectPointsEx(u0, u1, points);
			getBezIntersectPointsEx(u0, d1, points);
			getBezIntersectPointsEx(d0, u1, points);
			getBezIntersectPointsEx(d0, d1, points);
		}
	}
	else
	{
		return;
	}
}
void getBezIntersectPoints(const XBezierSpline &bez, std::vector<XVec2> &points)
{//Ŀǰ����㷨Ч�ʺܵ�,���α�������������ֻ������һ������
	if (points.size() > 0) return;//���α�������������ֻ������һ������
	XBezierSpline u = bez.getUpHalf();
	if (XMath::getPolygonArea(u.getPolygon()) < 1e-6) return;
	XBezierSpline d = bez.getDownHalf();
	if (XMath::getPolygonArea(d.getPolygon()) < 1e-6) return;
	static std::vector<XVec2> tmp;
	tmp.clear();
	getBezIntersectPoints(u, d, tmp);	//����û�������˵�
	if (tmp.size() > 1)
	{//������Ҫ�ų��м�Ķ˵�
		XVec2 tmpV = bez.getBezierSplineValue(0.5);
		for (int i = 0; i < tmp.size(); ++i)
		{
			if (tmp[i].getLengthSqure(tmpV) < 1e-2)
			{//ֻ��Ҫ�ų�һ�μ���
				for (int j = i + 1; j < tmp.size(); ++j)
					points.push_back(tmp[j]);
				break;
			}
			else
			{
				points.push_back(tmp[i]);
			}
		}
		return;//���α�������������ֻ������һ������
	}
	getBezIntersectPoints(u, points);
	getBezIntersectPoints(d, points);
}
}
}