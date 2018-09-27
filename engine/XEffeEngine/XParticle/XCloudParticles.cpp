#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCloudParticles.h"
namespace XE{
XCloudParticles::XCloudParticles()
:m_isInited(0)				//�Ƿ��ʼ��
,m_isEnd(0)				//�Ƿ����
,m_isSetEnd(0)				//�Ƿ����ý���
,m_atom(NULL)
,m_texture(NULL)
{}
int XCloudParticles::init(float maxCenterPoint,float minCenterPoint,float maxSize,float minSize,
						   float density,const XRect& liveArea,const XVec2& speed,int maxAtomSum,const XTexture *texture)
{
	if(m_isInited != 0) return 0;
	if(minCenterPoint >= maxCenterPoint) return 0;
	m_maxCenterPoint = maxCenterPoint;
	m_minCenterPoint = minCenterPoint;
	m_curCenterPoint = XRand::random(1000) * 0.001f * (m_maxCenterPoint - m_minCenterPoint) + m_minCenterPoint; 

	if(minSize >= maxSize) return 0;
	m_maxSize = maxSize;
	m_minSize = minSize;
	m_curSize = XRand::random(1000) * 0.001f * (m_maxSize - m_minSize) + m_minSize;

	if(density <= 0) return 0;
	m_density = density;

	m_liveArea = liveArea;		//ע���������û�н��к����Լ��
	m_speed = speed;
	if(maxAtomSum <= 0) return 0;	//ע�������ֵ����̫�󣬲���������м��
	m_maxAtomSum = maxAtomSum;

	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;

	m_atom = XMem::createArrayMem<XAloneParticles>(m_maxAtomSum);
	if(m_atom == NULL) return 0;
	
	m_isInited = 1;				//�Ƿ��ʼ��
	m_isEnd = 0;				//�Ƿ����
	m_isSetEnd = 0;				//�Ƿ����ý���

	return 1;
}
void XCloudParticles::reset()
{
	if(m_isInited == 0 ||
		m_isEnd == 0) return;
	m_curCenterPoint = XRand::random(1000) * 0.001f * (m_maxCenterPoint - m_minCenterPoint) + m_minCenterPoint; 
	m_curSize = XRand::random(1000) * 0.001f * (m_maxSize - m_minSize) + m_minSize;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].m_stage = STAGE_SLEEP;
	}
	m_isEnd = 0;
	m_isSetEnd = 0;
}
void XCloudParticles::move(float timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{
			if(XRand::random(1000 * timeDelay) < 10)
			{//�ƶ�
				m_atom[i].m_dPosition.y += (XRand::random(1000) * 0.002f - 1) * m_speed.y * 0.1f;
			}
			m_atom[i].move(timeDelay);
			if(!m_liveArea.isInRect(m_atom[i].m_curPosition))
			{
				m_atom[i].m_curColor.setA(0.0f);
				m_atom[i].m_stage = STAGE_SLEEP;
			}
	//	}
	}
	if(m_isSetEnd == 0)
	{//�������ӵĲ���
		//�ƶ����ĵ�
		if(XRand::random(100) < 50)
		{
			m_curCenterPoint += 0.025f * timeDelay;
			if(m_curCenterPoint > m_maxCenterPoint) m_curCenterPoint = m_maxCenterPoint;
		}else
		{
			m_curCenterPoint -= 0.025f * timeDelay;
			if(m_curCenterPoint < m_minCenterPoint) m_curCenterPoint = m_minCenterPoint;
		}
		//�ı䷶Χ
		if(XRand::random(100) < 50)
		{
			m_curSize += 0.025f * timeDelay;
			if(m_curSize > m_maxSize) m_curSize = m_maxSize;
		}else
		{
			m_curSize -= 0.025f * timeDelay;
			if(m_curSize < m_minSize) m_curSize = m_minSize;
		}
		if(XRand::random(10000) * 0.0001f <= m_density)
		{//��������
			for(int i = 0;i < m_maxAtomSum;++ i)
			{
				if(m_atom[i].m_stage == STAGE_SLEEP)
				{
				//	m_atom[i].m_isInited = 0;
					float temp = XRand::random(1000) * 0.001f * m_curSize;
					if(XRand::random(100) < 50) temp = -temp;

					m_atom[i].m_curPosition.set(m_liveArea.right,m_curCenterPoint + temp);
					temp = XRand::random(75)/100.0f + 0.5f;
				//	m_atom[i].init(&(m_texture->m_texture),m_texture->m_w * temp,m_texture->m_h * temp);
					m_atom[i].init(m_texture);
					m_atom[i].m_curSize.set(temp);
					m_atom[i].m_stage = STAGE_MOVE;
					m_atom[i].m_dPosition.x = m_speed.x * (0.75f + XRand::random(1000) * 0.0005f);
					m_atom[i].m_dPosition.y = (XRand::random(1000) * 0.002f - 1) * m_speed.y;
					temp = XRand::random(30)/100.0f + 0.2f;
					m_atom[i].m_curColor.set(1.0f,temp);
					m_atom[i].m_curSize.set(XRand::random(50)/100.0f + 0.75f,XRand::random(50)/100.0f + 0.75f);
					break;
				}
			}
		}
	}else
	{//����Ƿ����
		for(int i = 0;i < m_maxAtomSum;++ i)
		{
			if(m_atom[i].m_stage == STAGE_MOVE) return;
		}
		m_isEnd = 1;
	}
}
void XCloudParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{
			m_atom[i].draw();
	//	}
	}
}
}