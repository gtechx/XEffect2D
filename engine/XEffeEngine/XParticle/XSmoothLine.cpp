#include "XStdHead.h"
#include "XSmoothLine.h"
namespace XE{
void XSmoothLine::pushAPoint(const XVec2& p)	//��������Ѿ�������ֵ�����˵�
{
	XSmoothPoint *last = *(m_points.end() - 1);
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
	for(auto it = m_points.begin();it != m_points.end();++ it)
	{
		len += (*it)->m_length;
		(*it)->m_percentage = len;
	}
	len = 1.0f / len;
	for (auto it = m_points.begin(); it != m_points.end(); ++it)
	{
		(*it)->m_percentage = (*it)->m_percentage * len;
	}
}
void XSmoothLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	for (auto it = m_points.begin(); it != m_points.end() - 1; ++it)
	{
		XRender::drawLine((*it)->m_position,(*(it + 1))->m_position);
		XRender::drawPoint((*it)->m_position,XFColor::red);
	}
}
void XSmoothLine::addAPoint(const XVec2& p)	//����һ����
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
		XSmoothPoint * last = *(m_points.end() - 1);	//��ȡ���һ����
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//�����һ�����غϲ���Ҫ�����µĵ�
		int sum = len / m_insertLength;
		sum = (std::min)(m_insertSum,sum);
		//��δ���
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//��������
			pushAPoint(XMath::lineSlerp<XVec2>(last->m_position,p,per));
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