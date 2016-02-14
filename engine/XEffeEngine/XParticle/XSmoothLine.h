#ifndef _JIA_XSMOOTHLINE_
#define _JIA_XSMOOTHLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.10
//--------------------------------
//����һ��ƽ�����ߵ��࣬��Ʒ�ʽ�����ڽڵ�������������β����Ч���ļ���
//#include "XBasicClass.h"
#include "../XMath/XVector2.h"
#include <deque>
namespace XE{
class XSmoothPoint
{
public:
	XVector2 m_position;	//��ǰ���λ��
	float m_length;			//��ǰ�ڵ�����һ�ڵ�ľ���
	float m_percentage;		//��ǰ�ڵ�������ȫ���еİٷֱ�
};
class XSmoothLine
{
protected:
	bool m_isInited;
	std::deque<XSmoothPoint *> m_points;	//�����ϵ����е�
	int m_insertSum;						//��ֵ����
	float m_insertLength;					//��ֵ����	//���ֵ��������Ȩ���ϵ������������������ʹ����С��ֵ����

	virtual void release();	//�ͷ���Դ
	virtual void pushAPoint(const XVector2 &p);	//��������Ѿ�������ֵ�����˵�
	virtual void updateData();	//�����ڲ�����
public:
	XSmoothLine()
		:m_isInited(false)
		,m_insertSum(1)
		,m_insertLength(1.0f)
	{}
	virtual ~XSmoothLine() {release();}
	virtual bool init(int insertSum,float insertLength);
	virtual void draw();
	virtual void addAPoint(const XVector2 & p);	//����һ����
	//void update(float stepTime);
	virtual int getPointSum() const {return m_points.size();}
	virtual void clear() 
	{
		for(unsigned int i = 0;i < m_points.size();++ i)
		{
			XMem::XDELETE(m_points[i]);
		}
		m_points.clear();
	}
	virtual bool getIsEnd(){return m_points.size() <= 0;}
};
}
#endif