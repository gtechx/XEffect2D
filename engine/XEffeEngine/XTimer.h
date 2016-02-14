#ifndef _JIA_XTIMER_
#define _JIA_XTIMER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
#include "SDL.h"
#include <string>
#include <time.h>
namespace XE{
//����һ���򵥼�ʱ�����࣬���ڼ򵥵ļ�ʱ��Ҫ������ļ�ʱ��
class XSimpleTimer
{
private:
	float m_timer;
	float m_stepTime;	//����ʱ��,С��0Ϊ����ʱ
	bool m_isWorking;	//�Ƿ��ڹ���״̬
public:
	XSimpleTimer()
		:m_timer(0)
		,m_stepTime(0)
		,m_isWorking(true)
	{}
	XSimpleTimer(float stepTime)
		:m_timer(0)
		,m_stepTime(stepTime)
		,m_isWorking(true)
	{}
	void setWorking(bool flag){m_isWorking = flag;}
	bool getIsWorking()const{return m_isWorking;}
	void setStepTime(float stepTime){m_stepTime = stepTime;}
	int getStepTime() const {return m_stepTime;}
	bool update(float stepTime)	//�����Ƿ񴥷�ʱ���¼�
	{
		if(!m_isWorking) return false;
		if(m_stepTime <= 0.0f) return true;
		m_timer += stepTime;
		if(m_timer >= m_stepTime)
		{
			m_timer -= m_stepTime;
			return true;
		}
		return false;
	}
	int getCurTime()const{return m_timer;}	//��ȡ��ǰ��¼��ʱ��
	void reset(){m_timer = 0.0f;}
};
//����һ���߾��ȼ�ʱ������
class XTimer
{
public:
    XTimer();
	~XTimer(){}

    void start();	//���ü�ʱ�����
    void end();		//���ü�ʱ����
    double getElapsedTime();	//��ȡʱ����λΪ��
    double getElapsedTimeInSec();	//��ȡʱ����λΪ��
    double getElapsedTimeInMilliSec();	//��ȡʱ����λΪ����
    double getElapsedTimeInMicroSec();	//��ȡʱ����λΪ΢��

	double getUpElapsedTime();	//��ȡ���ε���֮���ʱ���,��λΪ����
	double getCurrentTime();	//��ȡ��ǰ��ʱ�䣬��λΪ����
private:
	double m_upTime;	//�ϴε��õ�ʱ��
    double m_startTime;
    double m_endTime;
#ifdef XEE_OS_WINDOWS
	double m_perFreq;
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startCount;
    LARGE_INTEGER m_endCount;
	LARGE_INTEGER m_upCount;
	LARGE_INTEGER m_curCount;
#else
    timeval m_startCount;
    timeval m_endCount;
	timeval m_upCount;
	timeval m_curCount;
#endif
    bool m_stopped;
};
inline void XTimer::start()
{
    m_stopped = false; // reset stop flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_startCount);
#else
    gettimeofday(&m_startCount,NULL);
#endif
}
inline void XTimer::end()
{
    m_stopped = true; // set timer stopped flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_endCount);
#else
    gettimeofday(&m_endCount,NULL);
#endif
}
inline double XTimer::getElapsedTimeInMilliSec()
{
    return getElapsedTimeInMicroSec() * 0.001;
}
inline double XTimer::getElapsedTimeInSec()
{
    return getElapsedTimeInMicroSec() * 0.000001;
}
inline double XTimer::getElapsedTime()
{
    return getElapsedTimeInSec();
}
inline double XTimer::getCurrentTime()		//��ȡ��ǰ��ʱ��
{
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_curCount);
    return m_curCount.QuadPart * m_perFreq * 0.001;
#else
	gettimeofday(&m_curCount,NULL);
	return (m_curCount.tv_sec * 1000000.0) + m_curCount.tv_usec;
#endif
}
struct XSystemTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    XSystemTime()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}
    XSystemTime(int y,int m,int d)
        :year(y)
        ,month(m)
        ,day(d)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}
};
#define MILLISECOND_IN_ONE_DAY (24 * 60 * 60 * 1000)	//һ���ж��ٺ���
#define DAY_IN_ONE_LEAP_YEAR (366)
namespace XTime
{
	inline bool isLeapYear(int year)
	{//�ж��ǻ��Ϊ����
		//����1
		//return (((year % 4) == 0 && year % 100 != 0) || year % 400 == 0);
		//����2,�겻Ϊ����
		return (((year & 0x03) == 0 && year % 100 != 0) || year % 400 == 0);
	}
	//��ȡ�ӳ������е���ǰ��ʱ����λΪms
	inline long getCurrentTicks()
	{
#ifdef CREATE_WINDOW_WITH_SDL
		return SDL_GetTicks();	//��SDL������
#else
	#if WITH_64BIT_SUPPLY		//windows�µ�API
		return GetTickCount64();
	#else 
		return GetTickCount();
	#endif
#endif
	}
	extern bool getDateIsCorrect(const XSystemTime &t);			//�ж�ʱ���ʽ����ȷ��
	extern void getTimeMs(XSystemTime &t);							//������һ��ͨ�õķ������ڻ�ȡϵͳʱ�䣬��λ��ms
	inline XSystemTime getTimeMs()
	{
		XSystemTime tmp;
		getTimeMs(tmp);
		return tmp;
	}
	extern bool isBefore(const XSystemTime &t1,const XSystemTime &t2);		//�ж�t1�Ƿ���t2֮ǰ
	extern long getElapsedTime(const XSystemTime &ts,const XSystemTime &te);	//��������ʱ��֮���ʱ����λΪms
	extern const int dayInMonth[12];
	//����ĺ��������Խ����Ż���������δ����
	inline int getMsInThisYear(const XSystemTime &t)	//������֤
	{//���������������һ���еĺ���ֵ
		if(!getDateIsCorrect(t)) return 0;
		int ret = t.hour * 60 * 60 * 1000 + t.minute * 60 * 1000 + t.second * 1000 + t.millisecond;
		if(t.month <= 2) return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
		if(isLeapYear(t.year))//����
			return (dayInMonth[t.month - 1] + t.day) * MILLISECOND_IN_ONE_DAY + ret;
		else//������
			return (dayInMonth[t.month - 1] - 1 + t.day) * MILLISECOND_IN_ONE_DAY + ret;
	}
	//����ָ��ʱ����һ���еĵڼ���,������������ǺϷ���
	inline int getDateDaysInYear(const XSystemTime &t)	//������֤
	{
		if(t.month <= 2) return dayInMonth[t.month - 1] + t.day;
		if(isLeapYear(t.year)) return dayInMonth[t.month - 1] + t.day;	//����
		else return dayInMonth[t.month - 1] - 1 + t.day;//������
	}
	//����ӹ�ԪԪ��������ָ�����ڵ�����
	inline int getTotalDays(const XSystemTime &t)
	{
		int tmpY = t.year - 1;
		int tmp = 365 * tmpY + getDateDaysInYear(t);	//������ʷ������ʱ��
		//�������������
		//����1
	//	for(int i = 1;i < t.year;++ i)
	//	{//����ÿ�ζ�Ҫ�㣬�����˷Ѽ�����
	//		if(isLeapYear(i)) ++tmp;
	//	}
		//����2
		tmp += (tmpY >> 2);
		tmpY /= 100;
		tmp += (tmpY >> 2) - tmpY;
		return tmp;
	}
	//����ָ��ʱ�������ڼ�
	inline int getDateWeek(const XSystemTime &t)	//������֤
	{
		return getTotalDays(t) % 7;
	}
	inline int getCurrentWeek()	//��ȡ���������ڼ�
	{
		time_t t = time(NULL);
		return localtime(&t)->tm_wday;
	}
	//����ָ����ĵڼ�����ʲô���ڣ����ڼ�
	extern void getDateData(int year,int day,XSystemTime &t,int &week);
	//Ŀǰʹ�������ϵ��㷨ֻ������1900��-2050���ũ��
	extern void solar2lunar(const XSystemTime &in,XSystemTime &out);	//����ת����
	extern void lunar2solar(const XSystemTime &in,XSystemTime &out);	//����ת��������δʵ�֣�
	inline std::string sysTime2String(const XSystemTime &in)	//ʱ��ת����string 20140915-095643-377
	{
		char tmp[20];
		sprintf(tmp,"%04d%02d%02d-%02d%02d%02d-%03d",in.year,in.month,in.day,
			in.hour,in.minute,in.second,in.millisecond);
		return tmp;
	}
	inline XSystemTime string2sysTime(const std::string &str)
	{
		XSystemTime in;
		sscanf(str.c_str(),"%04d%02d%02d-%02d%02d%02d-%03d",&in.year,&in.month,&in.day,
			&in.hour,&in.minute,&in.second,&in.millisecond);
		return in;
	}
}
//����һ���򵥵ļ��������������ڼ�¼�������Լ�Ƶ��
class XSimpleCounter
{
private:
	float m_frequency;	//Ƶ�ʣ���ÿ��
	int m_times;		//����
	int m_upTimes;		//�ϴ�ͳ��ʱ�Ĵ���
	float m_cycle;		//ͳ������,Ƶ�ʵļ��㲽��,��λ��,����С�ڵ���0ʱΪ��ͳ��
	float m_timer;		//��ʱ��,��λ��
	float m_upTimer;	//��һ�ε�ʱ��㣬��λ��
	int m_upTicks;		//��һ�ε�ʱ���ź�
	int m_lastFrameTime;	//��һ֡��ʱ��,��λms
public:
	XSimpleCounter()
		:m_frequency(0.0f)
		,m_times(0)
		,m_upTimes(0)
		,m_cycle(5.0f)
		,m_upTicks(-1)
		,m_upTimer(0.0f)
		,m_timer(0.0f)
		,m_lastFrameTime(0)
	{}
	void setCycle(float cycle){m_cycle = cycle;}	//��λ�룬ͳ�ƵĲ���
	int getTotalTimes()const{return m_times;}		//��ȡ�ܵĵ��ô���
	float getTotalTimer()const{return m_timer;}		//��ȡ�ܵĵ���ʱ��
	float getFrequency()const{return m_frequency;}	//��ȡ��ǰ��Ƶ��
	int getLastFrameTime()const{return m_lastFrameTime;}//������һ֡��ʱ��:��λ ����
	//void reset();	//��λ����δʵ��
	bool update()//�����Ƿ�����µ�Ƶ������
	{
		++ m_times;
		if(m_upTicks < 0)
		{//��һ������
			m_upTicks = XTime::getCurrentTicks();
			return false;
		}else
		{
			int curTicks = XTime::getCurrentTicks();
			m_lastFrameTime = curTicks - m_upTicks;
			m_timer += m_lastFrameTime * 0.001f;
			m_upTicks = curTicks;
		}
		if(m_timer - m_upTimer >= m_cycle && m_cycle > 0.0f)
		{
			m_frequency = (float)(m_times - m_upTimes)/(m_timer - m_upTimer);
			m_upTimes = m_times;
			m_upTimer = m_timer;
			return true;
		}
		return false;
	}
};
//������ʱ��
class XStepTimer
{
private:
	bool m_isFirst;
	int m_upTimer;
	int m_curTimer;
public:
	int update()	//�������ε���֮���ʱ���
	{
		if(m_isFirst)
		{
			m_isFirst = false;
			m_upTimer = m_curTimer = XTime::getCurrentTicks();
			return 0;
		}else
		{
			m_curTimer = XTime::getCurrentTicks();
			int ret = m_curTimer - m_upTimer;
			m_upTimer = m_curTimer;
			return ret;
		}
	}
	XStepTimer()
		:m_isFirst(true)
	{}
};
}
#endif