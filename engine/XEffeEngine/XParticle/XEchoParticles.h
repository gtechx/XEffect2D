#ifndef _JIA_XECHOPARTICLES_
#define _JIA_XECHOPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XParticle.h"
#if WITH_LOG
#else
#include <vector>
#endif
#define WITH_DLIST
namespace XE{
//��������������ϵͳģ��.��ӰЧ��
class XEchoParticles
{
public:
	bool m_isInited;
	bool m_isEnd;
private:
	bool m_isSetEnd;	//�Ƿ����ý���
	bool m_haveGetmemory;
	static const int m_maxEchoParticlesSum = 16;		//��ӰЧ������������

	XGL::XBlendModel m_blendModel;
	int m_insertSum;	//��ֵ������
	int m_density;		//��Ӱ�ܶ�		˵������Ӱ�ܶȺ�����ֵ���໥��Լ�ģ�������ֵ����Լ�£�ȡ�����ܶȵ�Ҫ��
	//Ҳ������������ܶ�С������ֵ�����Ҫ�������������ֵ����ȡ����ֵ
	int m_curPoint;		//��ǰ������ָ��

	//����ı�����Ϊ���Ż��ٶȶ������
	const XTexture *m_texture;
	int w;
	int h;
	int halfW;
	int halfH;
#ifdef WITH_DLIST
	int m_glListOrder;
#endif

	int m_atomSum;
public:
	void setBlendModel(XGL::XBlendModel model) { m_blendModel = model; }
public:
	XParentParticle m_parentParticle;	//һ��ĸ����
	XBasicParticle *m_particles;		//���ɸ�������

	int getAtomSum()const { return m_atomSum; }
	bool init(const XTexture* texture,
		int insertSum,		//�м���������
		int density,		//�ܶ�
		const XVec2& position,
		float dSize,		//��С�仯����
		float dAlpha,		//͸���ȱ仯����
		int atomSum = 0,
		float initSize = 1.0f);	//atomSum = 0 Ϊʹ��Ĭ��ֵ
	void setDSize(float dSize);
	void setDAlpha(float dAlpha);
	void setInsertSum(float insertSum);
	void setDensity(float density);
	void setTexture(const XTexture *texture)
	{
		if(texture == NULL || !glIsTexture(texture->m_texture)) return;
		m_texture = texture;
	}
	void setPosition(const XVec2& position);
	void setPosition(float x,float y);
	void setPosition(float x,float y,float size);
	void setPosition(float x,float y,float size,float udR,float udG = 1.0f,float udB = 1.0f);//float udA = 1.0f);
	void setAlpha(float temp);
	void setColor(float r,float g,float b);
	void move(float timeDelay)
	{
		if (!m_isInited || m_isEnd) return;
		_moveEx(pow(m_parentParticle.m_dSize.x, (timeDelay * 0.05f)), 
			m_parentParticle.m_dColor.a * timeDelay);
	}
	void moveEx(float sizeX, float alpha)	//��������ȡ�������Ż����ٶ�
	{
		if (!m_isInited || m_isEnd) return;
		_moveEx(sizeX, alpha);
	}
	void _moveEx(float sizeX,float alpha);	//��������ȡ�������Ż����ٶ�
	//�������
	void draw() const;
	//�������鷽��Ч�ʸߣ�������ͼ��������ת������
	void drawAsLine() const;
	//�������鷽��Ч�ʽϸߣ�Ч����Ӱ��
	void drawExBegin() const;
	void drawExBody() const;
	void getDrawData(std::vector<float> &v,std::vector<float> &t,std::vector<float> &c) const;	//�����������ת��ʹ��vbo��ͼ
	void getDrawData(std::vector<float> &v,std::vector<float> &c) const;	//�����������ת��ʹ��vbo��ͼ
	void getDrawData(float* v, float* c, int& sum) const;	//�����������ת��ʹ��vbo��ͼ
	void drawExEnd() const
	{
		glPopMatrix();
		XGL::DisableBlend();
	}
	//������һ�鷽��Ч����ߣ����Ǻ�����ת
	void drawExBeginEx() const;
	void drawExBodyEx() const;	//�����ǳߴ�ͽǶȵ�����
	void drawExEndEx() const
	{
		glEnd();
		XGL::DisableBlend();
	}

	XEchoParticles();
	void release();
	bool getMemory(int atomSum);
	void setEnd();
	void reset();
	void resetState(const XVec2& position,float initSize = 1.0f);	//�������ó�ʼλ��
	void resetStateEx(const XVec2& position);	//�������ó�ʼλ��
	bool getIsEnd()const { return m_isEnd; }
};
inline void XEchoParticles::setEnd()
{
	if (m_isInited && !m_isEnd && !m_isSetEnd) m_isSetEnd = true;
}
inline void XEchoParticles::reset()
{
	if (m_isInited && m_isEnd)
	{
		m_isEnd = false;
		m_isSetEnd = false;
	}
}
inline void XEchoParticles::resetState(const XVec2& position,float initSize)	//�������ó�ʼλ��
{
	if(!m_isInited) return;
	m_parentParticle.m_initColor.set(1.0f,1.0f);
	m_parentParticle.m_initPosition = position;
	m_parentParticle.m_curPosition = position;
	m_parentParticle.m_initSize.set(initSize);
	m_curPoint = 0;
	for(int i = 0;i < m_atomSum;++ i)
	{
		m_particles[i].m_isEnable = false;
	}
}
inline void XEchoParticles::resetStateEx(const XVec2& position)	//�������ó�ʼλ��
{
	//if(!m_isInited) return;
	//m_parentParticle.m_initColor.set(1.0f,1.0f);
	m_parentParticle.m_initPosition = m_parentParticle.m_curPosition = position;
	//m_parentParticle.m_initSize.set(initSize);
	m_curPoint = 0;
	m_particles[m_atomSum - 1].m_isEnable = false;
}
inline void XEchoParticles::setAlpha(float temp)
{
	if(temp < 0) m_parentParticle.m_initColor.setA(0.0f);
	else if(temp > 1) m_parentParticle.m_initColor.setA(1.0f);
	else m_parentParticle.m_initColor.setA(temp);
}
inline void XEchoParticles::setColor(float r,float g,float b)
{
	m_parentParticle.m_initColor.setR(r);
	m_parentParticle.m_initColor.setG(g);
	m_parentParticle.m_initColor.setB(b);
}
inline void XEchoParticles::setDSize(float dSize)
{
	m_parentParticle.m_dSize.x = dSize;
}
inline void XEchoParticles::setDAlpha(float dAlpha)
{
	m_parentParticle.m_dColor.setA(dAlpha);
}
inline void XEchoParticles::setInsertSum(float insertSum)
{
	if(insertSum < 0.0f) insertSum = 0.0f;
	m_insertSum = insertSum;
}
inline void XEchoParticles::setDensity(float density)
{
	if(density < 1) density = 1;
	m_density = density;
}
inline void XEchoParticles::setPosition(const XVec2& position)
{
	setPosition(position.x,position.y);
}
}
#endif