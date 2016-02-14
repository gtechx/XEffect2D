#ifndef _JIA_XKALMAN_
#define _JIA_XKALMAN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.19
//--------------------------------
//����һ�����ڿ������˲�����
namespace XE{
class XKalmanFilter
{
protected:
	double m_processNioseQ;	//Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
	double m_measureNoiseR;	//R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��  
//	double m_initialPrediction;	//����
	//�м����
	double m_xLast;
	double m_pLast;
	bool m_isFirst;
public:
	virtual double getQ() const{return m_processNioseQ;}
	virtual void setQ(double q){m_processNioseQ = q;}
	virtual double getR() const{return m_measureNoiseR;}
	virtual void setR(double r){m_measureNoiseR = r;}
//	virtual double getP() const{return m_initialPrediction;}
//	virtual void setP(double p){m_initialPrediction = p;}
	double filter(double data)
	{
		if(m_isFirst)
		{
			m_xLast = data;
			m_isFirst = false;
		}
		double x_mid = m_xLast;								//x_last=x(k-1|k-1),x_mid=x(k|k-1)
		double p_mid = m_pLast + m_processNioseQ;			//p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
		double kg = p_mid / (p_mid + m_measureNoiseR);		//kgΪkalman filter��RΪ����
		m_xLast = x_mid + kg * (data - x_mid);			//���Ƴ�������ֵ
		m_pLast = (1.0 - kg) * p_mid;						//����ֵ��Ӧ��covariance

		return m_xLast;                
	}
	virtual void init(double Q,double R)	//��double p
	{
		m_processNioseQ = Q;
		m_measureNoiseR = R;
//		m_initialPrediction = P;
	}
	XKalmanFilter()
		:m_processNioseQ(0.0000001)
		,m_measureNoiseR(10.0)
//		,m_initialPrediction(2210.0)
		,m_xLast(0.0)
		,m_pLast(0.0)
		,m_isFirst(true)
	{}
};
////�����ǿ������˲�����������
////http://www.amobbs.com/thread-5559754-1-1.html
////http://www.amobbs.com/thread-5571611-1-1.html
////�����ǹ��ڿ������˲���һЩ�㷨
//#define KALMAN_Q        0.0000001
//#define KALMAN_R        10.0000
//#define KALMAN_P0        2210.0000
////Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
////R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��        
//inline double KalmanFilter(const double ResrcData,	//��������
//					double ProcessNiose_Q,	//����
//					double MeasureNoise_R,	//����
//					double InitialPrediction)	//����
//{
//	double R = MeasureNoise_R;
//	double Q = ProcessNiose_Q;
//
//	static double x_last = 0.0;
//	static double p_last = 0.0;      
//
//	double x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
//	double p_mid = p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
//	double kg = p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
//	double x_now = x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
//                
//	double p_now = (1-kg)*p_mid;//����ֵ��Ӧ��covariance        
//
//	p_last = p_now; //����covarianceֵ
//	x_last = x_now; //����ϵͳ״ֵ̬
//
//	return x_now;                
//}
}
#endif