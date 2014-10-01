#ifndef _JIA_XTIMER_
#define _JIA_XTIMER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOSDefine.h"
#include "SDL.h"
//����һ���߾��ȼ�ʱ������
class _XTimer
{
public:
    _XTimer();
	~_XTimer(){}

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
    bool m_stopped;
#ifdef XEE_OS_WINDOWS
	double m_perFreq;
    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startCount;
    LARGE_INTEGER m_endCount;
	LARGE_INTEGER m_upCount;
	LARGE_INTEGER m_nowCount;
#else
    timeval m_startCount;
    timeval m_endCount;
	timeval m_upCount;
	timeval m_nowCount;
#endif
};
inline void _XTimer::start()
{
    m_stopped = false; // reset stop flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_startCount);
#else
    gettimeofday(&m_startCount,NULL);
#endif
}
inline void _XTimer::end()
{
    m_stopped = true; // set timer stopped flag
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_endCount);
#else
    gettimeofday(&m_endCount,NULL);
#endif
}
inline double _XTimer::getElapsedTimeInMilliSec()
{
    return getElapsedTimeInMicroSec() * 0.001;
}
inline double _XTimer::getElapsedTimeInSec()
{
    return getElapsedTimeInMicroSec() * 0.000001;
}
inline double _XTimer::getElapsedTime()
{
    return getElapsedTimeInSec();
}
inline double _XTimer::getCurrentTime()		//��ȡ��ǰ��ʱ��
{
#ifdef XEE_OS_WINDOWS
    QueryPerformanceCounter(&m_nowCount);
    return m_nowCount.QuadPart * m_perFreq * 0.001;
#else
	gettimeofday(&m_nowCount,NULL);
	return (m_nowCount.tv_sec * 1000000.0) + m_nowCount.tv_usec;
#endif
}
struct _XSystemTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    _XSystemTime()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {}
    _XSystemTime(int y,int m,int d)
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
	return SDL_GetTicks();	//��SDL������
#if WITH_64BIT_SUPPLY		//windows�µ�API
	return GetTickCount64();
#else 
	return GetTickCount();
#endif
}
extern bool getDateIsCorrect(const _XSystemTime &t);			//�ж�ʱ���ʽ����ȷ��
extern void getTimeMs(_XSystemTime &t);							//������һ��ͨ�õķ������ڻ�ȡϵͳʱ�䣬��λ��ms
inline _XSystemTime getTimeMs()
{
	_XSystemTime tmp;
	getTimeMs(tmp);
	return tmp;
}
extern bool isBefore(const _XSystemTime &t1,const _XSystemTime &t2);		//�ж�t1�Ƿ���t2֮ǰ
extern long getElapsedTime(const _XSystemTime &ts,const _XSystemTime &te);	//��������ʱ��֮���ʱ����λΪms
extern const int dayInMonth[12];
//����ĺ��������Խ����Ż���������δ����
inline int getMsInThisYear(const _XSystemTime &t)	//������֤
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
inline int getDateDaysInYear(const _XSystemTime &t)	//������֤
{
	if(t.month <= 2) return dayInMonth[t.month - 1] + t.day;
	if(isLeapYear(t.year)) return dayInMonth[t.month - 1] + t.day;	//����
	else return dayInMonth[t.month - 1] - 1 + t.day;//������
}
//����ӹ�ԪԪ��������ָ�����ڵ�����
inline int getTotalDays(const _XSystemTime &t)
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
inline int getDataWeek(const _XSystemTime &t)	//������֤
{
	return getTotalDays(t) % 7;
}
//����ָ����ĵڼ�����ʲô���ڣ����ڼ�
extern void getDateData(int year,int day,_XSystemTime &t,int &week);
//Ŀǰʹ�������ϵ��㷨ֻ������1900��-2050���ũ��
extern void solar2lunar(const _XSystemTime &in,_XSystemTime &out);	//����ת����
extern void lunar2solar(const _XSystemTime &in,_XSystemTime &out);	//����ת��������δʵ�֣�
#include <string>
inline std::string sysTime2String(const _XSystemTime &in)	//ʱ��ת����string 20140915-095643-377
{
	char tmp[20];
	sprintf(tmp,"%04d%02d%02d-%02d%02d%02d-%03d",in.year,in.month,in.day,
        in.hour,in.minute,in.second,in.millisecond);
	return tmp;
}
inline _XSystemTime string2sysTime(const std::string &str)
{
	_XSystemTime in;
	sscanf(str.c_str(),"%04d%02d%02d-%02d%02d%02d-%03d",&in.year,&in.month,&in.day,
        &in.hour,&in.minute,&in.second,&in.millisecond);
	return in;
}
#endif