//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCloudParticles.h"

_XCloudParticles::_XCloudParticles()
:m_isInited(0)				//�Ƿ��ʼ��
,m_isEnd(0)				//�Ƿ����
,m_isSetEnd(0)				//�Ƿ����ý���
,m_atom(NULL)
,m_texture(NULL)
{
}

int _XCloudParticles::init(float maxCenterPoint,float minCenterPoint,float maxSize,float minSize,
						   float density,const _XRect& liveArea,const _XVector2& speed,int maxAtomSum,const _XTexture *texture)
{
	if(m_isInited != 0) return 0;
	if(minCenterPoint >= maxCenterPoint) return 0;
	m_maxCenterPoint = maxCenterPoint;
	m_minCenterPoint = minCenterPoint;
	m_nowCenterPoint = random(1000) * 0.001f * (m_maxCenterPoint - m_minCenterPoint) + m_minCenterPoint; 

	if(minSize >= maxSize) return 0;
	m_maxSize = maxSize;
	m_minSize = minSize;
	m_nowSize = random(1000) * 0.001f * (m_maxSize - m_minSize) + m_minSize;

	if(density <= 0) return 0;
	m_density = density;

	m_liveArea = liveArea;		//ע���������û�н��к����Լ��
	m_speed = speed;
	if(maxAtomSum <= 0) return 0;	//ע�������ֵ����̫�󣬲���������м��
	m_maxAtomSum = maxAtomSum;

	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;

	m_atom = createArrayMem<_XAloneParticles>(m_maxAtomSum);
	if(m_atom == NULL) return 0;
	
	m_isInited = 1;				//�Ƿ��ʼ��
	m_isEnd = 0;				//�Ƿ����
	m_isSetEnd = 0;				//�Ƿ����ý���

	return 1;
}

void _XCloudParticles::reset()
{
	if(m_isInited == 0 ||
		m_isEnd == 0) return;
	m_nowCenterPoint = random(1000) * 0.001f * (m_maxCenterPoint - m_minCenterPoint) + m_minCenterPoint; 
	m_nowSize = random(1000) * 0.001f * (m_maxSize - m_minSize) + m_minSize;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].m_stage = STAGE_SLEEP;
	}
	m_isEnd = 0;
	m_isSetEnd = 0;
}

void _XCloudParticles::move(int timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
	//	if(m_atom[i].m_stage == STAGE_MOVE)
	//	{
			if(random(1000 * timeDelay) < 10)
			{//�ƶ�
				m_atom[i].m_dPosition.y += (random(1000) * 0.002f - 1) * m_speed.y * 0.1f;
			}
			m_atom[i].move(timeDelay);
			if(!m_liveArea.isInRect(m_atom[i].m_nowPosition))
			{
				m_atom[i].m_nowColor.setA(0.0f);
				m_atom[i].m_stage = STAGE_SLEEP;
			}
	//	}
	}
	if(m_isSetEnd == 0)
	{//�������ӵĲ���
		//�ƶ����ĵ�
		if(random(100) < 50)
		{
			m_nowCenterPoint += 0.025f * timeDelay;
			if(m_nowCenterPoint > m_maxCenterPoint) m_nowCenterPoint = m_maxCenterPoint;
		}else
		{
			m_nowCenterPoint -= 0.025f * timeDelay;
			if(m_nowCenterPoint < m_minCenterPoint) m_nowCenterPoint = m_minCenterPoint;
		}
		//�ı䷶Χ
		if(random(100) < 50)
		{
			m_nowSize += 0.025f * timeDelay;
			if(m_nowSize > m_maxSize) m_nowSize = m_maxSize;
		}else
		{
			m_nowSize -= 0.025f * timeDelay;
			if(m_nowSize < m_minSize) m_nowSize = m_minSize;
		}
		if(random(10000) * 0.0001f <= m_density)
		{//��������
			for(int i = 0;i < m_maxAtomSum;++ i)
			{
				if(m_atom[i].m_stage == STAGE_SLEEP)
				{
				//	m_atom[i].m_isInited = 0;
					float temp = random(1000) * 0.001f * m_nowSize;
					if(random(100) < 50) temp = -temp;

					m_atom[i].m_nowPosition.set(m_liveArea.right,m_nowCenterPoint + temp);
					temp = random(75)/100.0f + 0.5f;
				//	m_atom[i].init(&(m_texture->m_texture),m_texture->m_w * temp,m_texture->m_h * temp);
					m_atom[i].init(m_texture);
					m_atom[i].m_nowSize.set(temp,temp);
					m_atom[i].m_stage = STAGE_MOVE;
					m_atom[i].m_dPosition.x = m_speed.x * (0.75f + random(1000) * 0.0005f);
					m_atom[i].m_dPosition.y = (random(1000) * 0.002f - 1) * m_speed.y;
					temp = random(30)/100.0f + 0.2f;
					m_atom[i].m_nowColor.setColor(1.0f,1.0f,1.0f,temp);
					m_atom[i].m_nowSize.set(random(50)/100.0f + 0.75f,random(50)/100.0f + 0.75f);
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

void _XCloudParticles::draw() const
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