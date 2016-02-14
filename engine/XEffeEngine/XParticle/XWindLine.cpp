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
	XGL::EnableBlend();
	XGL::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(unsigned int i = 0;i < m_points.size();++ i)
	{
		rate = m_pointsLife[i] / m_maxLife * m_points[i]->m_percentage;
		if(rate > 0.666f) rate = (1.0f - rate) * 3.0f;
		else rate = rate * 1.5f;
		glColor4f(m_color.fR,m_color.fG,m_color.fB,rate * m_aRate);
		glVertex2fv(m_points[i]->m_position);
	}
	glEnd();
}
void XWindLine::move(float stepTime)	//����˥��
{
	int sum = 0;
	for(unsigned int i = 0;i < m_pointsLife.size();++ i)
	{
		m_pointsLife[i] -= stepTime;
		if(m_pointsLife[i] <= 0.0f) ++ sum;	//�����ĵ������
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
void XWindLine::addAPoint(const XVector2 & p)	//����һ����
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
		XSmoothPoint * last = m_points[(int)(m_points.size()) - 1];	//��ȡ���һ����
		float lastLife = m_pointsLife[(int)(m_points.size()) - 1];
		float len = p.getLength(last->m_position);
		if(len <= 0.0f)	return;	//�����һ�����غϲ���Ҫ�����µĵ�
		int sum = len / m_insertLength;
		sum = XEE_Min(m_insertSum,sum);
		//��δ���
		float d = 1.0f / (sum + 1.0f);
		float per = d;
		for(int i = 1;i < sum + 1; ++ i)
		{//��������
			pushAPoint(XMath::lineSlerp<XVector2>(last->m_position,p,per),
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