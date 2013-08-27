#ifndef _JIA_XLIQUIDBALL_
#define _JIA_XLIQUIDBALL_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XEffeEngine.h"

extern float gravity; //重力加速度
//默认重力加速度的方向为垂直向下

//这是一个液体球粒子的类
class _XLiquidBall
{
private:
	char m_isEnable;	//是否有效
public:
	_XVector2 m_position;	//位置
	float m_radius;			//半径,单位米
	float m_mass;			//质量,单位克
	_XVector2 m_speed;		//速度,直角坐标系的速度,单位米每秒
	float m_airResistance;	//空气阻力(在低速的情况下应该可以忽略不计)
	float m_adhereRate;		//粘附系数
	float m_adhereRadius;	//粘附半径(原理类似于质量引力)

	_XVector2 m_positionS;	//内部计算的点,起始
	//_XVector2 m_positionE;	//内部计算的点,结束
	//int m_upCrashOrder;		//一次不能同时与两个物体发生碰撞
	void setEnable(char isEnable)
	{
		if(isEnable == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	int getIsEnable() const
	{
		return m_isEnable;
	}
	void init(const _XVector2& position,float radius,float mass,const _XVector2& speed,float airResistance,float adhereRate,float adhereRadius)
	{
		m_position = position;	//位置
		m_radius = radius;			//半径
		m_mass = mass;			//质量
		m_speed = speed;		//速度,直角坐标系的速度
		m_airResistance = airResistance;	//空气阻力(在低速的情况下应该可以忽略不计)
		m_adhereRate = adhereRate;		//粘附系数
		m_adhereRadius = adhereRadius;	//粘附半径(原理类似于质量引力)
	}

	_XLiquidBall()
		:m_isEnable(0)	//是否有效
		,m_position(0.0f,0.0f)	//位置
		,m_radius(0.0f)			//半径
		,m_mass(0.0f)			//质量
		,m_speed(0.0f,0.0f)		//速度,直角坐标系的速度
		,m_airResistance(0.0f)	//空气阻力(在低速的情况下应该可以忽略不计)
		,m_adhereRate(0.0f)		//粘附系数
		,m_adhereRadius(0.0f)	//粘附半径(原理类似于质量引力)
	{
	}
};

//碰撞数据
struct _XCrashData
{
	double m_crashTime;	//碰撞发生的时间
	float m_crashAngle;	//碰撞发生的法向量
	char m_needFrash;	//是否需要刷新数据
	_XCrashData()
		:m_crashTime(0.0)
		,m_crashAngle(0.0f)
		,m_needFrash(0)
	{
	}
};

class _XLiquidBalls
{
private:
	char m_isInited;	//是否进行了初始化
	char m_isEnable;	//是否有效
	int crashInsideProc(double timeDelay,double upTime);	//是否完成
	void crashTimerProc(double timeDelay,double upTime);
	int checkProc();//检查数据是否合格
public:
	//下面是规则的球物体
	_XLiquidBall *m_pLiquidBall;
	int m_liquidBallSum; //例子的总数目
	//下面是规则的线段物体
	_XLine *m_pLine;
	int m_lineSum;	//线段的数量

	int m_liquidBallDataSum;
	_XCrashData *m_pCrashData;
//	double *m_pLiquidBallData;
//	char *m_pLiquidBallFlag;	//是否需要进行计算的标记
	double m_minTimer;	//多元碰撞的时间次小值
	int m_crashOrder;
	int m_crashObj0;
	int m_crashObj1;
	int m_crashSameTimeSum;
	int m_deep;
	//_XRect m_area;	//物体运动的区域
	char m_haveError;	//是否存在错误

	int init(int liquidBallSum,int lineSum);
	void move(int timeDelay);
	void crashProc(int timeDelay);
	int addOneBall(const _XVector2& position,float radius,float mass,_XVector2 speed,float airResistance,float adhereRate,float adhereRadius);	//增加一个例子，返回增加是否成功
	int addOneLine(const _XVector2& startPosition,_XVector2 endPosition);
	_XLiquidBalls();
	void setEnable(char isEnable)
	{
		if(isEnable == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	int getIsEnable() const
	{
		return m_isEnable;
	}
	int getBallIsEnable(int order) const
	{
		return m_pLiquidBall[order].getIsEnable();
	}
	_XVector2 getBallPosition(int order) const
	{
		return m_pLiquidBall[order].m_position;
	}
};

#endif