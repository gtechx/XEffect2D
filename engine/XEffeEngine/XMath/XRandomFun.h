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
	inline int random(int x)	//ǿ�����������ȡֵ��Χ0 - 0x07ffffff
	{
		if(x == 0) return 0;
		if(x < 0) x = -x;
		if(x <= RAND_MAX) return rand() % x;
		else return ((rand() << 12) + (rand() % 0x0fff)) % x;
	}
	inline float randomf(float tempMax) {return tempMax * rand() / (RAND_MAX + 1.0f);}
	inline float randomf(float x, float y)
	{
		if(x == y) return x; 
	//	float high = XEE_Max(x,y);
	//	float low = XEE_Min(x,y);
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
	inline float gaussRand()	//ģ����̬�ֲ����漴��������0.0f - 1.0f����ֵ
	{
		if((rand() % 100) < 50) return sin(randomf(PI_HALF)) * 0.5f;
		else return 0.5f + (1.0f - sin(randomf(PI_HALF))) * 0.5f;
	}
	//0 - RAND_MAX(0x7fff)		//����һ��0 - x֮�������
	extern void randomMomery(char *p,int size);	//���ֽ�Ϊ��λ����p��ָ��Ĵ�СΪsize���ڴ�ռ�
	template<class type> void randomArray(type *p,int size) //����һ������
	{
		if(p == NULL ||
			size <= 1) return;
		int order;
		for(int i = 0;i < size; ++ i)
		{
			order = random(size);
			if(order >= 0 && order < size && order != i)
				XMath::swapTwoSum<type>(p[i],p[order]);
		}
	}
}
}
#endif