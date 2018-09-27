#ifndef _JIA_XEQUIDISTANTITERPOLATION_
#define _JIA_XEQUIDISTANTITERPOLATION_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//#include "XMath.h"
#include "XVector2.h"
//����һ��2D�Ⱦ��ֵ���࣬�����ڽڵ�������ͨ���̶��ľ�����в�ֵ��������Ч�ĵ�(��δ���)
#include <vector>
namespace XE{
class XEquidistantIterpolation
{
private:
	bool m_isFirstData;		//�Ƿ�Ϊ����ĵ�һ����
	float m_upLength;		//��һ�εĳ���
	XVec2 m_upPoint;	//��һ�εĵ�
	float m_distance;		//��ֵ�ľ���
public:
	XEquidistantIterpolation()
		:m_isFirstData(true)
		, m_upLength(0.0f)
		, m_distance(10.0f)
	{}
	XVec2 getCurPosition()const{return m_upPoint;}	//��ȡ��ǰ��λ��
	void setDistance(float d)
	{
		if (d <= 0) return;	//�Ƿ�������
		m_distance = d; 
	}
	float getDistance() const { return m_distance; }
	void reset()
	{
		m_isFirstData = true;
		m_upLength = 0.0f;
	}
	void reset(const XVec2& p)	//��ָ����Ϊ���
	{
		m_upLength = 0.0f;
		m_isFirstData = false;
		m_upPoint = p;
	}
	//�����²���ĵ�����м���Ҫ����ĵ�
	std::vector<XVec2> calPoint(const XVec2& p)
	{
		std::vector<XVec2> ret;
		calPoint(p,ret);
		return ret;
	}
	int calPoint(const XVec2& p,std::vector<XVec2> &data);
};
}
#endif