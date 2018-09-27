#ifndef _JIA_XSTATISTICS_
#define _JIA_XSTATISTICS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include <deque>
namespace XE{
namespace XMath{
//��߶���Ķ���һЩͳ��ѧ����ķ���
//������v�и�Ԫ�صĺ�
extern double getSum(const double *v,int sum);
extern float getSum(const float *v,int sum);
//��ƽ����
extern double getAverageValue(const double *v,int sum);
extern float getAverageValue(const float *v,int sum);
extern float getAverageValue(const std::deque<float> &data);
extern XVec2 getAverageValue(const std::deque<XVec2> &data);
//���׼��(������)������������������ɢ��
extern double getStandardDeviation(const double *v,int sum);
extern float getStandardDeviation(const float *v,int sum);
extern float getStandardDeviation(const std::deque<float> &data);
extern float getStandardDeviation(const std::deque<XVec2> &data);
}
}
#endif