#include "XEffeEngine.h"
#include "XSmoothLine.h"

void _XSmoothLine::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_points.size(); ++ i)
	{
		XDELETE(m_points[i]);
	}
	m_points.clear();
	m_isInited = false;
}
void _XSmoothLine::pushAPoint(const _XVector2 &p)	//��������Ѿ�������ֵ�����˵�
{
	_XSmoothPoint *last = m_points[m_points.size() - 1];
	_XSmoothPoint *ip = createMem<_XSmoothPoint>();
	if(ip == NULL) return;
	ip->m_length = p.getLength(last->m_position);
	ip->m_percentage = 0.0f;	//������ʱ�����㣬�ȴ�һ���Դ���
	ip->m_position = p;
	m_points.push_back(ip);
}
void _XSmoothLine::updateData()
{//�����ڲ�����
	if(m_points.size() < 2) return;
	float len = 0.0f;
	for(int i = 0;i < m_points.size();++ i)
	{
		len += m_points[i]->m_length;
		m_points[i]->m_percentage = len;
	}
	len = 1.0f / len;
	for(int i = 0;i < m_points.size();++ i)
	{
		m_points[i]->m_percentage = m_points[i]->m_percentage * len;
	}
}
void _XSmoothLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	for(int i = 0;i < m_points.size() - 1;++ i)
	{
		drawLine(m_points[i]->m_position,m_points[i + 1]->m_position);
		drawPoint(m_points[i]->m_position.x,m_points[i]->m_position.y,1.0f,0.0f,0.0f,1.0f);
	}
}
void _XSmoothLine::addAPoint(const _XVector2 & p)	//����һ����
{
	if(!m_isInited) return;
	if(m_points.size() == 0)
	{//��һ���㣬����Ҫ��ֵ
		_XSmoothPoint *ip = createMem<_XSmoothPoint>();
		if(ip == NULL) return;
		ip->m_length = 0.0f;
		ip->m_percentage = 0.0f;
		ip->m_position = p;
		m_points.push_back(ip);
	}else
	{
		//����1:���Բ�ֵ
		_XSmoothPoint * last = m_points[m_points.size() - 1];	//��ȡ���һ����
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//�����һ�����غϲ���Ҫ�����µĵ�
		int sum = len / m_insertLength;
		sum = XEE_Min(m_insertSum,sum);
		//��δ���
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//��������
			pushAPoint(lineSlerp<_XVector2>(last->m_position,p,per));
			per += d;
		}
		//����2:��������ֵ (��δʵ��)
		//�����ڲ�����
		updateData();
	}
}
bool _XSmoothLine::init(int insertSum,float insertLength)
{
	if(m_isInited) return false;
	if(insertSum < 0 || insertLength <= 0.0f) return false;
	m_insertSum = insertSum;
	m_insertLength = insertLength;

	m_isInited = true;
	return true;
}