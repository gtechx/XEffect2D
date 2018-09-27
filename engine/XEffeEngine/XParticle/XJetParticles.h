#ifndef _JIA_XJETPARTICLES_
#define _JIA_XJETPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//���������Ч����Ҳ������ǰ������ĺ󷽵�һ���Ƕ��ڰ���һ�����ܶ��������ӵ�Ч��
#include "XEchoParticles.h"
namespace XE{
class XJetParticles
{
private:
	char m_isInited;			//�Ƿ��ʼ��
	char m_isEnd;				//�Ƿ����
	char m_isSetEnd;			//�Ƿ����ý���

	float m_directionAngle;		//������ǰ���ķ���
	float m_jetAngle;			//����������ļн�
	float m_jetDensity;			//������������ܶ�
	XVec2 m_curPosition;		//������ĵ�ǰλ��

	float m_initSpeed;			//����ĳ�ʼ�ٶ�

	XEchoParticles *m_atomBasic;	//����ı���Ч��
	XEchoParticles *m_atom;		//������������
	int m_maxAtomSum;				//�����������
	int m_curAtomPoint;				//��ǰʹ�õ����ӵ�ָ��	
	int m_timer;

	const XTexture *m_texture;				//���䱾�����ͼ
//	int m_w;
//	int m_h;
	const XTexture *m_textureAtom;			//�������ӵ���ͼ
//	int m_wA;
//	int m_hA;
public:
	int init(const XVec2& position,float jetSpeed,float jetAngle,float jetDensity,
		const XTexture *texture,const XTexture *textureAton = NULL);
	void reset(const XVec2& position);
	void reset(float x, float y) { reset(XVec2(x,y)); }
	void move(float timeDelay);
	void setPosition(const XVec2& position);
	void setPosition(float x,float y) { setPosition(XVec2(x, y)); }
	void draw() const;
	void release();
	void setEnd();

	XJetParticles();
};

inline void XJetParticles::setEnd()
{
	if(m_isInited == 0 ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return;
	m_isSetEnd = 1;
	m_atomBasic->setEnd();
}

inline void XJetParticles::setPosition(const XVec2& position)
{
	if(m_isInited == 0 ||
		m_isEnd != 0) return;
	//����Ƕ�
	m_directionAngle = m_curPosition.getAngle(position);
	m_curPosition = position;
	m_atomBasic->setPosition(position);
}
}
#endif