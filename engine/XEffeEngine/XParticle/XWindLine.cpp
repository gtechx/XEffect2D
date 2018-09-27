#include "XStdHead.h"
#include "XWindLine.h"
namespace XE{
void XWindLine::draw()
{
	if(!m_isInited) return;
	if(m_points.size() < 2) return;
	float rate = 0;
	//for(int i = 0;i < (int)(m_points.size()) - 1;++ i)
	//{
	//	rate = m_pointsLife[i] / m_maxLife * m_points[i + 1]->m_percentage;
	//	if(rate > 0.666f) rate = (1.0f - rate) * 3.0f;
	//	else rate = rate * 1.5f;
	//	drawLine(m_points[i]->m_position,m_points[i + 1]->m_position,rate * m_wRate,1.0f,1.0f,1.0f,rate * m_aRate);
	//}

	XGL::DisableTexture2D();
	XGL::setBlendAlpha();
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	auto itL = m_pointsLife.begin();
	for(auto it = m_points.begin();it != m_points.end();++ it,++ itL)
	{
		rate = *(itL) / m_maxLife * (*it)->m_percentage;
		if(rate > 0.666f) rate = (1.0f - rate) * 3.0f;
		else rate = rate * 1.5f;
		glColor4f(m_color.r,m_color.g,m_color.b,rate * m_aRate);
		glVertex2fv((*it)->m_position);
	}
	glEnd();
}
void XWindLine::move(float stepTime)	//����˥��
{
	int sum = 0;
	for(auto it = m_pointsLife.begin();it != m_pointsLife.end();++ it)
	{
		*it -= stepTime;
		if(*it <= 0.0f) ++ sum;	//�����ĵ������
	}
	if(sum > 0)
	{
	//	XSmoothPoint *tmp = NULL;
		for(int i = 0;i < sum;++ i)
		{
			m_pointsLife.pop_front();
			XMem::XDELETE(m_points[0]);
			m_points.pop_front();
		}
		updateData();	//���µ��״̬
	}
}
void XWindLine::addAPoint(const XVec2& p)	//����һ����
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
		m_pointsLife.push_back(m_maxLife);
	}else
	{
		//����1:���Բ�ֵ
		XSmoothPoint * last = *(m_points.end() - 1);	//��ȡ���һ����
		assert(m_points.size() == m_pointsLife.size());
		//float lastLife = m_pointsLife[(int)(m_points.size()) - 1];
		float lastLife = *(m_pointsLife.end() - 1);
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//�����һ�����غϲ���Ҫ�����µĵ�
		int sum = len / m_insertLength;
		sum = (std::min)(m_insertSum,sum);
		//��δ���
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//��������
			pushAPoint(XMath::lineSlerp<XVec2>(last->m_position,p,per),
				XMath::lineSlerp<float>(lastLife,m_maxLife,per));
			per += d;
		}
		//����2:��������ֵ (��δʵ��)
		//�����ڲ�����
		updateData();
	}
}
#if !WITH_INLINE_FILE
#include "XWindLine.inl"
#endif
}