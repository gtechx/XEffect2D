#ifndef _JIA_XWINDLINE_
#define _JIA_XWINDLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.11
//--------------------------------
#include "XSmoothLine.h"
namespace XE{
class XWindLine:public XSmoothLine
{
private:
	std::deque<float> m_pointsLife;	//�������
	float m_maxLife;
	float m_wRate;
	float m_aRate;
	XFColor m_color;

	virtual void release();	//�ͷ���Դ
	virtual void pushAPoint(const XVector2 &p,float life);	//��������Ѿ�������ֵ�����˵�
public:
	//life:��������������ߵĳ��ȣ�w:�ߵĿ��ϵ����a:�ߵ�͸��ϵ��
	bool init(int insertSum,float insertLength,float life,float w,float a);
	virtual void draw();
	virtual void addAPoint(const XVector2 & p);	//����һ����
	virtual void move(float stepTime);	//����˥��
	virtual void clear();
	XWindLine()
		:m_color(0.6f,0.6f,1.0f,1.0f)
	{}
	void setColor(const XFColor& c) {m_color = c;}
	XFColor getColor() {return m_color;}
};
#if WITH_INLINE_FILE
#include "XWindLine.inl"
#endif
}
#endif