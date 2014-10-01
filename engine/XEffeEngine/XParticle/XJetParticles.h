#ifndef _JIA_XJETPARTICLES_
#define _JIA_XJETPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//���������Ч����Ҳ������ǰ������ĺ󷽵�һ���Ƕ��ڰ���һ�����ܶ��������ӵ�Ч��
#include "XEchoParticles.h"

class _XJetParticles
{
private:
	char m_isInited;			//�Ƿ��ʼ��
	char m_isEnd;				//�Ƿ����
	char m_isSetEnd;			//�Ƿ����ý���

	float m_directionAngle;		//������ǰ���ķ���
	float m_jetAngle;			//����������ļн�
	float m_jetDensity;			//������������ܶ�
	_XVector2 m_nowPosition;		//������ĵ�ǰλ��

	float m_initSpeed;			//����ĳ�ʼ�ٶ�

	_XEchoParticles *m_atomBasic;	//����ı���Ч��
	_XEchoParticles *m_atom;		//������������
	int m_maxAtomSum;				//�����������
	int m_nowAtomPoint;				//��ǰʹ�õ����ӵ�ָ��	
	int m_timer;

	const _XTexture *m_texture;				//���䱾�����ͼ
//	int m_w;
//	int m_h;
	const _XTexture *m_textureAtom;			//�������ӵ���ͼ
//	int m_wA;
//	int m_hA;
public:
	int init(const _XVector2& position,float jetSpeed,float jetAngle,float jetDensity,
		const _XTexture *texture,const _XTexture *textureAton = NULL);
	void reset(const _XVector2& position);
	void reset(float x,float y);
	void move(int timeDelay);
	void setPosition(const _XVector2& position);
	void setPosition(float x,float y);
	void draw() const;
	void release();
	void setEnd();

	_XJetParticles();
};

inline void _XJetParticles::setEnd()
{
	if(m_isInited == 0 ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return;
	m_isSetEnd = 1;
	m_atomBasic->setEnd();
}

inline void _XJetParticles::setPosition(const _XVector2& position)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	//����Ƕ�
	m_directionAngle = m_nowPosition.getAngle(position);
	m_nowPosition = position;
	m_atomBasic->setPosition(position);
}

inline void _XJetParticles::setPosition(float x,float y)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	//����Ƕ�
	m_directionAngle = m_nowPosition.getAngle(x,y);
	m_nowPosition.set(x,y);
	m_atomBasic->setPosition(x,y);
}

#endif