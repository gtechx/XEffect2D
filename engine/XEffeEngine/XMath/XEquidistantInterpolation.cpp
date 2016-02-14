#include "XStdHead.h"
#include "XEquidistantInterpolation.h"
namespace XE{
int XEquidistantIterpolation::calPoint(const XVector2 &p,std::vector<XVector2> &data)
{
	data.clear();
	if (m_isFirstData)
	{//��һ����λ��Ч��
		m_isFirstData = false;
		m_upPoint = p;
		data.push_back(p);
		return data.size();
	}
	//����������ĵ�
	float len = p.getLength(m_upPoint);
	if (len <= 0.0f) return data.size();	//�Ƿ����ٽ�����
	if (m_upLength + len >= m_distance)
	{//���ȳ�����Χ������Ҫ��ֵ,�������μ��������λ��
		float tmpLen = m_distance - m_upLength;
		XVector2 point;
		while (true)
		{
			point = XMath::lineSlerp<XVector2>(m_upPoint, p, tmpLen / len);
			data.push_back(point);
			tmpLen += m_distance;
			if (tmpLen > len) break;
		}
		m_upPoint = p;
		m_upLength = len - (tmpLen - m_distance);
		return data.size();
	}
	else
	{//������δ������Χ�����ﲻ��Ҫ��ֵ
		m_upLength += len;
		m_upPoint = p;
		return data.size();
	}
}
}