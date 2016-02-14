#ifndef _JIA_XRIGIDBALL_
#define _JIA_XRIGIDBALL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "../XOSDefine.h"
#include "../XMath/XVector2.h"
namespace XE{
extern float gravity; //�������ٶ�
//Ĭ���������ٶȵķ���Ϊ��ֱ����

//����һ�����������ӵ���
class XRigidBall
{
private:
	XBool m_isEnable;	//�Ƿ���Ч
public:
	XVector2 m_position;	//λ��
	float m_radius;			//�뾶,��λ��
	float m_mass;			//����,��λ��
	XVector2 m_speed;		//�ٶ�,ֱ������ϵ���ٶ�,��λ��ÿ��
	float m_airResistance;	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
	float m_adhereRate;		//ճ��ϵ��
	float m_adhereRadius;	//ճ���뾶(ԭ����������������)

	XVector2 m_positionS;	//�ڲ�����ĵ�,��ʼ
	//XVector2 m_positionE;	//�ڲ�����ĵ�,����
	//int m_upCrashOrder;		//һ�β���ͬʱ���������巢����ײ
	void setEnable(XBool isEnable) {m_isEnable = isEnable;}
	XBool getIsEnable() const	{return m_isEnable;}
	void init(const XVector2& position,float radius,float mass,const XVector2& speed,float airResistance,float adhereRate,float adhereRadius)
	{
		m_position = position;	//λ��
		m_radius = radius;			//�뾶
		m_mass = mass;			//����
		m_speed = speed;		//�ٶ�,ֱ������ϵ���ٶ�
		m_airResistance = airResistance;	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
		m_adhereRate = adhereRate;		//ճ��ϵ��
		m_adhereRadius = adhereRadius;	//ճ���뾶(ԭ����������������)
	}
	XRigidBall()
		:m_isEnable(XFalse)	//�Ƿ���Ч
		,m_position(0.0f,0.0f)	//λ��
		,m_radius(0.0f)			//�뾶
		,m_mass(0.0f)			//����
		,m_speed(0.0f,0.0f)		//�ٶ�,ֱ������ϵ���ٶ�
		,m_airResistance(0.0f)	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
		,m_adhereRate(0.0f)		//ճ��ϵ��
		,m_adhereRadius(0.0f)	//ճ���뾶(ԭ����������������)
	{}
};
//��ײ����
struct XCrashData
{
	double m_crashTime;	//��ײ������ʱ��
	float m_crashAngle;	//��ײ�����ķ�����
	char m_needFrash;	//�Ƿ���Ҫˢ������
	XCrashData()
		:m_crashTime(0.0)
		,m_crashAngle(0.0f)
		,m_needFrash(0)
	{}
};
class XRigidBalls
{
private:
	XBool m_isInited;	//�Ƿ�����˳�ʼ��
	XBool m_isEnable;	//�Ƿ���Ч
	XBool crashInsideProc(double timeDelay,double upTime);	//�Ƿ����
	void crashTimerProc(double timeDelay,double upTime);
	XBool checkProc();//��������Ƿ�ϸ�
public:
	//�����ǹ����������
	XRigidBall *m_pRigidBall;
	int m_rigidBallSum; //���ӵ�����Ŀ
	//�����ǹ�����߶�����
	XLine *m_pLine;
	int m_lineSum;	//�߶ε�����

	int m_rigidBallDataSum;
	XCrashData *m_pCrashData;
//	double *m_pLiquidBallData;
//	char *m_pLiquidBallFlag;	//�Ƿ���Ҫ���м���ı��
	double m_minTimer;	//��Ԫ��ײ��ʱ���Сֵ
	int m_crashOrder;
	int m_crashObj0;
	int m_crashObj1;
	int m_crashSameTimeSum;
	int m_deep;
	//XRect m_area;	//�����˶�������
	XBool m_haveError;	//�Ƿ���ڴ���

	XBool init(int rigidBallSum,int lineSum);
	void move(float timeDelay)
	{
		if(!m_isInited &&
		!m_isEnable &&
		m_haveError) return;
		crashProc(timeDelay);
	}
	void crashProc(int timeDelay);
	XBool addOneBall(const XVector2& position,float radius,float mass,const XVector2 &speed,float airResistance,float adhereRate,float adhereRadius);	//����һ�����ӣ����������Ƿ�ɹ�
	XBool addOneLine(const XVector2& startPosition,const XVector2 &endPosition);
	XRigidBalls();
	void setEnable(XBool isEnable)	{m_isEnable = isEnable;}
	XBool getIsEnable() const {return m_isEnable;}
	XBool getBallIsEnable(int order) const	{return m_pRigidBall[order].getIsEnable();}
	XVector2 getBallPosition(int order) const {return m_pRigidBall[order].m_position;}
};
}
#endif