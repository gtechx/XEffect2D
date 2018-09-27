#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XJetParticles.h"
namespace XE{
XJetParticles::XJetParticles()
:m_isInited(0)				//�Ƿ��ʼ��
,m_isEnd(0)				//�Ƿ����
,m_isSetEnd(0)				//�Ƿ����ý���
,m_texture(NULL)
,m_textureAtom(NULL)
{}
int XJetParticles::init(const XVec2& position,float jetSpeed,float jetAngle,float jetDensity,
						 const XTexture *texture,const XTexture *textureAton)
{
	if(m_isInited != 0) return 0; 

	m_curPosition = position;
	m_initSpeed = jetSpeed;
	m_jetAngle = jetAngle;
	if(jetDensity <= 0) jetDensity = 0.01f;
	m_jetDensity = jetDensity;
	m_directionAngle = 0;
	if(texture == NULL || !glIsTexture(texture->m_texture)) return 0;
	m_texture = texture;
	if(textureAton == NULL)
	{
		m_textureAtom = texture;
	}else
	{
		if(!glIsTexture(texture->m_texture))
		{
			return 0;
		}else
		{
			m_textureAtom = textureAton;
		}
	}
	m_atomBasic = XMem::createMem<XEchoParticles>();
	if(m_atomBasic == NULL) return 0;

	m_maxAtomSum = jetDensity * 10;
	if(m_maxAtomSum < 2) m_maxAtomSum = 2;
	m_atom = XMem::createArrayMem<XEchoParticles>(m_maxAtomSum);
	if(m_atom == NULL) 
	{
		XMem::XDELETE(m_atomBasic);
		return 0;
	}

	m_curAtomPoint = 0;
	m_timer = 0;

	m_isInited = 1;
	m_isEnd = 1;
	m_isSetEnd = 0;
	return 1;
}
void XJetParticles::reset(const XVec2& position)
{
	if(m_isInited == 0 ||
		m_isEnd == 0) return;
	m_atomBasic->m_isInited = false;
	m_atomBasic->init(m_texture,100,2,position,0.95f,-0.001f);
	m_atomBasic->m_parentParticle.m_initColor.set(1.0f,1.0f);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].m_isInited = false;
		m_atom[i].m_isEnd = true;
	}
	m_curAtomPoint = 0;
	m_timer = 0;
	m_isEnd = 0;
	m_isSetEnd = 0;
}
void XJetParticles::move(float timeDelay)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	m_atomBasic->move(timeDelay);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].move(timeDelay);
		if(m_atom[i].m_parentParticle.m_stage == STAGE_MOVE)
		{
			m_atom[i].m_parentParticle.m_initColor.a -= 0.00125f * timeDelay;
		//	m_atom[i].m_parentParticle.m_initPosition.x += m_atom[i].m_parentParticle.m_dPosition.x * timeDelay;
		//	m_atom[i].m_parentParticle.m_initPosition.y += m_atom[i].m_parentParticle.m_dPosition.y * timeDelay;
			m_atom[i].m_parentParticle.m_initPosition += m_atom[i].m_parentParticle.m_dPosition * timeDelay;
			m_atom[i].setPosition(m_atom[i].m_parentParticle.m_initPosition);
			if(m_atom[i].m_parentParticle.m_initColor.a <= 0)
			{
				m_atom[i].m_parentParticle.m_initColor.a = 0;
				m_atom[i].m_parentParticle.m_stage = STAGE_SLEEP;
				m_atom[i].setEnd();
			}
		}
	}
	//�����µ�����
	if(m_isSetEnd == 0)
	{
		m_timer += timeDelay;
		int temp = 120 / m_jetDensity;
		if(m_timer > temp || XRand::random(temp - m_timer) < temp * 0.1)
		{//�����µ���������
			m_timer = 0;
			if(m_atom[m_curAtomPoint].getIsEnd() != 0)
			{//���µ�����
				m_atom[m_curAtomPoint].m_isInited = false;
			//	float tempR = 1.0;//random(75)/100.0 + 0.5;
				m_atom[m_curAtomPoint].m_parentParticle.m_curPosition = m_curPosition;//XVec2(m_curPosition.x + m_w * 0.5,m_curPosition.y + m_h * 0.5);
			//	m_atom[m_curAtomPoint].init(&(m_textureAtom->m_texture),64,2,m_atom[m_curAtomPoint].m_parentParticle.m_curPosition,
			//		0.92,-0.001,m_textureAtom->m_w * tempR,m_textureAtom->m_h * tempR,64);
				m_atom[m_curAtomPoint].init(m_textureAtom,64,2,m_atom[m_curAtomPoint].m_parentParticle.m_curPosition,
					0.92f,-0.001f,64);
				m_atom[m_curAtomPoint].m_parentParticle.m_initSize.set(0.75f);
				m_atom[m_curAtomPoint].m_parentParticle.m_stage = STAGE_MOVE;
				m_atom[m_curAtomPoint].m_parentParticle.m_dPosition = 
					XMath::getRotateRate((XRand::random(4000) / 2000.0f - 1.0f) * m_jetAngle + m_directionAngle + PI) * m_initSpeed;
				m_atom[m_curAtomPoint].m_parentParticle.m_initColor.set(1.0f,1.0f);

				++ m_curAtomPoint;
				if(m_curAtomPoint >= m_maxAtomSum)
				{
					m_curAtomPoint = 0;
				}
			}
		}
	}else
	{//�ж��Ƿ����
		int flag = 0;
		if(m_atomBasic->getIsEnd() == 0) return;
		for(int i = 0;i < m_maxAtomSum;++ i)
		{
			if(m_atom[i].getIsEnd() == 0) 
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) m_isEnd = 1;
	}
}
void XJetParticles::draw() const
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	m_atomBasic->draw();
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].draw();
	}
}
void XJetParticles::release()
{
	if(m_isInited == 0) return;
	XMem::XDELETE(m_atomBasic);
	for(int i = 0;i < m_maxAtomSum;++ i)
	{
		m_atom[i].release();
	}
	XMem::XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//��ֹrelease֮�����������Ա��������ɴ���
}
}