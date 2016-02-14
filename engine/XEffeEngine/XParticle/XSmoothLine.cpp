#include "XStdHead.h"
#include "XSmoothLine.h"
namespace XE{
void XSmoothLine::release()
{
	if(!m_isInited) return;
	for(unsigned int i = 0;i < m_points.size(); ++ i)
	{
		XMem::XDELETE(m_points[i]);
	}
	m_points.clear();
	m_isInited = false;
}
void XSmoothLine::pushAPoint(const XVector2 &p)	//��������Ѿ�������ֵ�����˵�
{
	XSmoothPoint *last = m_points[(int)(m_points.size()) - 1];
	XSmoothPoint *ip = XMem::createMem<XSmoothPoint>();
	if(ip == NULL) return;
	ip->m_length = p.getLength(last->m_position);
	ip->m_percentage = 0.0f;	//������ʱ�����㣬�ȴ�һ���Դ���
	ip->m_position = p;
	m_points.push_back(ip);
}
void XSmoothLine::updateData()
{//�����ڲ�����
	if(m_points.size() < 2) return;
	float len = 0.0f;
	for(unsigned int i = 0;i < m_points.size();++ i)
	{
		len += m_points[i]->m_length;
		m_points[i]->m_percentage = len;
	}
	len = 1.0f / len;
	for(unsigned int i = 0;i < m_points.size();++ i)
	{
		m_points[i]->m_percentage = m_points[i]->m_percentage * len;
	}
}
void XSmoothLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	for(int i = 0;i < (int)(m_points.size()) - 1;++ i)
	{
		XRender::drawLine(m_points[i]->m_position,m_points[i + 1]->m_position);
		XRender::drawPoint(m_points[i]->m_position.x,m_points[i]->m_position.y,1.0f,0.0f,0.0f,1.0f);
	}
}
void XSmoothLine::addAPoint(const XVector2 & p)	//����һ����
{
	if(!m_isInited) return;
	if(m_points.size() == 0)
	{//��һ���㣬����Ҫ��ֵ
		XSmoothPoint *ip = XMem::createMem<XSmoothPoint>();
		if(ip == NULL) return;
		ip->m_length = 0.0f;
		ip->m_percentage = 0.0f;
		ip->m_position = p;
		m_points.push_back(ip);
	}else
	{
		//����1:���Բ�ֵ
		XSmoothPoint * last = m_points[(int)(m_points.size()) - 1];	//��ȡ���һ����
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//�����һ�����غϲ���Ҫ�����µĵ�
		int sum = len / m_insertLength;
		sum = XEE_Min(m_insertSum,sum);
		//��δ���
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//��������
			pushAPoint(XMath::lineSlerp<XVector2>(last->m_position,p,per));
			per += d;
		}
		//����2:��������ֵ (��δʵ��)
		//�����ڲ�����
		updateData();
	}
}
bool XSmoothLine::init(int insertSum,float insertLength)
{
	if(m_isInited) return false;
	if(insertSum < 0 || insertLength <= 0.0f) return false;
	m_insertSum = insertSum;
	m_insertLength = insertLength;

	m_isInited = true;
	return true;
}
}