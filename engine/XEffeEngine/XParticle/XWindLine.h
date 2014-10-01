#ifndef _JIA_XWINDLINE_
#define _JIA_XWINDLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.11
//--------------------------------
#include "XSmoothLine.h"

class _XWindLine:public _XSmoothLine
{
private:
	std::deque<float> m_pointsLife;	//�������
	float m_maxLife;
	float m_wRate;
	float m_aRate;

	virtual void release()	//�ͷ���Դ
	{
		if(!m_isInited) return;
		_XSmoothLine::release();
		m_pointsLife.clear();
	}
	virtual void pushAPoint(const _XVector2 &p,float life)	//��������Ѿ�������ֵ�����˵�
	{
		_XSmoothLine::pushAPoint(p);
		m_pointsLife.push_back(life);
	}
public:
	//life:��������������ߵĳ��ȣ�w:�ߵĿ��ϵ����a:�ߵ�͸��ϵ��
	bool init(int insertSum,float insertLength,float life,float w,float a)
	{
		if(!_XSmoothLine::init(insertSum,insertLength)) return false;
		m_maxLife = life;
		m_wRate = w;
		m_aRate = a;
		return true;
	}
	virtual void draw();
	virtual void addAPoint(const _XVector2 & p);	//����һ����
	virtual void move(int stepTime);	//����˥��
	virtual void clear() 
	{
		for(int i = 0;i < m_points.size();++ i)
		{
			XDELETE(m_points[i]);
		}
		m_points.clear();
		m_pointsLife.clear();
	}
};

#endif