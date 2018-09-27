#ifndef _JIA_XRANDOMFUN_
#define _JIA_XRANDOMFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "stdlib.h"
#include "XMath/XMath.h"
namespace XE{
namespace XRand
{
	inline int random(int x)	//ǿ�����������ȡֵ��Χ0 - 0x7fffffff
	{
		if (x < 0) return -random(-x);
		if (x <= 1) return 0;
		if (x <= RAND_MAX) return rand() % x;
		else if (x <= 0x3fffffff) return ((rand() << 15) + rand()) % x;
		else return ((rand() << 16) + (rand() << 1) + (rand() & 0x01)) % x;
	}
	inline float randomf(float tempMax) {return tempMax * static_cast<float>(rand()) / (RAND_MAX + 1.0f);}
	inline float randomf(float x, float y)
	{
		if(x == y) return x; 
	//	float high = (std::max)(x,y);
	//	float low = (std::min)(x,y);
		//float high,low;
		//if(x > y) 
		//{
		//	high = x;
		//	low = y;
		//}else
		//{
		//	high = y;
		//	low = x;
		//}
		//return (float)(low + ((high-low) * rand() / (RAND_MAX + 1.0)));
		return (float)(y + ((x-y) * rand() / (RAND_MAX + 1.0)));
	}
	inline float gaussRand()	//ģ����̬�ֲ����漴��������0.0f - 1.0f����ֵ,���ݼ�����0.5����
	{
		if((rand() % 100) < 50) return sin(randomf(PI_HALF)) * 0.5f;
		else return 1.0f - sin(randomf(PI_HALF)) * 0.5f;
	}
	inline float gaussRandMax()	//ģ����̬�ֲ����漴��������0.0f - 1.0f����ֵ,���ݼ�����1.0����
	{
		return sin(randomf(PI_HALF));
	}
	inline float gaussRandMin()	//ģ����̬�ֲ����漴��������0.0f - 1.0f����ֵ,���ݼ�����0.0����
	{
		return 1.0f - sin(randomf(PI_HALF));
	}
	//0 - RAND_MAX(0x7fff)		//����һ��0 - x֮�������
	extern void randomMomery(void *p,int size);//���ֽ�Ϊ��λ���ڴ���д�����������
	template<class type> void randomArray(type *p,int size) //����һ������
	{
		if(p == NULL ||
			size <= 1) return;
	//	int order;
		for(int i = 0;i < size; ++ i)
		{
	//		order = random(size);
	//	//	//if(order >= 0 && order < size && order != i)
	//	//	if(order != i)
	//			XMath::swapTwoSum<type>(p[i],p[order]);
			XMath::swapTwoSum<type>(p[i], p[random(size)]);
		}
	}
	//���������ݶ�Ϊ0ʱ���ص�����Ч������
	//Բ�������dataΪ���������ķݶsumΪ�ݶ������������������ķݶ���
	inline int randRoundTable(const int *datas,int sum)
	{
		if(sum <= 0 || datas == NULL) return 0;
		int maxSum = 0;
		for(int i = 0;i < sum;++ i)
		{
			if(datas[i] < 0) return 0;	//�Ƿ������ݣ��ݶ�ı�������Ϊ����
			maxSum += datas[i];
		}
		if(maxSum <= 0) return 0;
		int curData = random(maxSum) + 1;
		for(int i = 0;i < sum;++ i)
		{
			curData -= datas[i];
			if(curData <= 0) return i;
		}
		return sum - 1;
	}
}
//��������������ָ����ʱ���ڻᷢ��һ������¼���Ҳ����ÿX���������һ��E�¼�
class XRandByTime
{
private:
	float m_settingTime;	//���õ�ʱ��
	float m_timer;	//��ʱ��
	float m_curHappenTime;	//��һ�εķ���ʱ��
	bool m_isHappened;//�Ƿ��Ѿ�����
public:
	XRandByTime()
		:m_settingTime(1000)
		, m_timer(0.0f)
		, m_isHappened(false)
		, m_curHappenTime(0.5f)
	{}
	void reset()
	{
		m_timer = 0.0f;
		m_isHappened = false;
		m_curHappenTime = XRand::randomf(0.0f,1.0f) * m_settingTime;
	}
	//ע����������������ڲ��ļ�ʱ����
	void setTime(float time)	//���÷���һ�ε�ƽ��ʱ��
	{
		if(time <= 0.0f) return;	//�Ƿ��Ĳ���
		m_settingTime = time;
		reset();
	}
	bool update(float stepTime)
	{
		m_timer += stepTime;
		if(!m_isHappened && m_timer >= m_curHappenTime)
		{
			m_isHappened = true;
			return true;
		}
		if(m_timer >= m_settingTime)
		{//��������¼�
			m_timer -= m_settingTime;
			m_isHappened = false;
			m_curHappenTime = XRand::randomf(0.0f,1.0f) * m_settingTime;
		}
		return false;
	}
};
}
#endif