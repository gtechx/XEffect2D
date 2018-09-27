#ifndef _JIA_XPARTICLE_
#define _JIA_XPARTICLE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//#include "../XBasicClass.h"
//#include "../XSprite.h"
//#include "../XBasicOpenGL.h"
#include "../XMath/XVector2.h"
namespace XE{
//���������е������ӣ�������ϵͳ�е����Ļ������ӹ��ɳɷ�
class XBasicParticle
{
public:
	bool m_isEnable;		//���������Ƿ���Ч

	XVec2 m_initPosition;	//���ӵ���ʱ��λ��
	XVec2 m_initSize;		//���ӵ���ʱ�ĳߴ�
	float m_initAngle;		//���ӵ���ʱ�ĽǶ�
	XFColor m_initColor;	//���ӵ���ʱ�ĳ�ʼ��ɫ

	XVec2 m_dPosition;		//����λ�ñ仯�ı仯��
	XVec2 m_dSize;			//���ӳߴ�仯�ı仯��
	float m_dAngle;			//���ӽǶȱ仯�ı仯��
	XFColor m_dColor;		//������ɫ�仯�ı仯��

	XVec2 m_curPosition;	//���ӵĵ�ǰλ��
	XVec2 m_curSize;		//���ӵĵ�ǰ�ߴ�
	float m_curAngle;		//���ӵĵ�ǰ�Ƕ�
	XFColor m_curColor;		//���ӵĵ�ǰ��ɫ
	XBasicParticle()
		:m_isEnable(false)
	{}
	virtual~XBasicParticle(){}
};
enum
{
	STAGE_SLEEP,	//���Ӵ�����Ϣ״̬����Ϣ״̬����ʾҲ���˶�
	STAGE_MOVE,		//���Ӵ����˶�״̬��
	STAGE_STILL,	//���Ӵ��ھ�ֹ״̬����ֹ״ֻ̬��ʾ���˶�
	STAGE_STEALTHY	//���Ӵ�������״̬������״ֻ̬�˶�����ʾ
};
//ĸ���ӣ�����ϵͳ����С�����ĸ��
class XParentParticle:public XBasicParticle
{
public:
	char m_stage;		//���ӽ׶α��

	XParentParticle();
	virtual~XParentParticle(){}
};
class XAloneParticles:public XParentParticle
{
public:
	char m_isInited;
private:
	const XTexture *m_texture;	//���ӵ���ͼ
//	int m_w;
//	int m_h;

	int halfW;
	int halfH;
	int m_glListOrder;
public:
	int init(const XTexture *texture);
	void draw() const;
	XAloneParticles();
	
	void move(float timeDelay);
};
inline void XAloneParticles::move(float timeDelay)
{
	if(m_isInited == 0 ||
		m_stage == STAGE_SLEEP) return;
	//��ɫ�ı仯
	//�ߴ�ı仯
	//λ�õı仯
//	m_curPosition.x += m_dPosition.x * timeDelay;
//	m_curPosition.y += m_dPosition.y * timeDelay;
	m_curPosition += m_dPosition * (float)timeDelay;
}
}
#endif