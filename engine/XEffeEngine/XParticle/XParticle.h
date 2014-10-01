#ifndef _JIA_XPARTICLE_
#define _JIA_XPARTICLE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "../XBasicClass.h"
#include "gl.h"
#include "../XSprite.h"
#include "../XBasicOpenGL.h"

//���������е������ӣ�������ϵͳ�е����Ļ������ӹ��ɳɷ�
class _XBasicParticle
{
public:
	char m_isEnable;			//���������Ƿ���Ч

	_XVector2 m_initPosition;	//���ӵ���ʱ��λ��
	_XVector2 m_initSize;		//���ӵ���ʱ�ĳߴ�
	float m_initAngle;		//���ӵ���ʱ�ĽǶ�
	_XFColor m_initColor;	//���ӵ���ʱ�ĳ�ʼ��ɫ

	_XVector2 m_dPosition;	//����λ�ñ仯�ı仯��
	_XVector2 m_dSize;		//���ӳߴ�仯�ı仯��
	float m_dAngle;			//���ӽǶȱ仯�ı仯��
	_XFColor m_dColor;		//������ɫ�仯�ı仯��

	_XVector2 m_nowPosition;	//���ӵĵ�ǰλ��
	_XVector2 m_nowSize;		//���ӵĵ�ǰ�ߴ�
	float m_nowAngle;		//���ӵĵ�ǰ�Ƕ�
	_XFColor m_nowColor;	//���ӵĵ�ǰ��ɫ
	_XBasicParticle()
		:m_isEnable(0)
	{
	}
	virtual~_XBasicParticle(){}
};

enum
{
	STAGE_SLEEP,	//���Ӵ�����Ϣ״̬����Ϣ״̬����ʾҲ���˶�
	STAGE_MOVE,		//���Ӵ����˶�״̬��
	STAGE_STILL,	//���Ӵ��ھ�ֹ״̬����ֹ״ֻ̬��ʾ���˶�
	STAGE_STEALTHY	//���Ӵ�������״̬������״ֻ̬�˶�����ʾ
};

//ĸ���ӣ�����ϵͳ����С�����ĸ��
class _XParentParticle:public _XBasicParticle
{
public:
	char m_stage;		//���ӽ׶α��

	_XParentParticle();
	virtual~_XParentParticle(){}
};

class _XAloneParticles:public _XParentParticle
{
public:
	char m_isInited;
private:
	const _XTexture *m_texture;	//���ӵ���ͼ
//	int m_w;
//	int m_h;

	int halfW;
	int halfH;
	int m_glListOrder;
public:
	int init(const _XTexture *texture);
	void draw() const;
	_XAloneParticles();
	
	void move(int timeDelay);
};

inline void _XAloneParticles::move(int timeDelay)
{
	if(m_isInited == 0 ||
		m_stage == STAGE_SLEEP) return;
	//��ɫ�ı仯
	//�ߴ�ı仯
	//λ�õı仯
//	m_nowPosition.x += m_dPosition.x * timeDelay;
//	m_nowPosition.y += m_dPosition.y * timeDelay;
	m_nowPosition += m_dPosition * (float)timeDelay;
}

#endif