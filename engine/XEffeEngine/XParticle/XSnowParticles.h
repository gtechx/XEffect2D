#ifndef _JIA_XSNOWPARTICLES_
#define _JIA_XSNOWPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//ѩ������Ʈ���Ч��
#include "XEchoParticles.h"

class _XSnowParticles
{
private:
	char m_isInited;				//�Ƿ��ʼ��
	char m_isEnd;				//�Ƿ����
	char m_isSetEnd;				//�Ƿ����ý���

	_XVector2 m_productArea;		//������Χ�����������Ϸ���һ��ֱ�ߣ�
	_XRect m_liveArea;			//���淶Χ
	_XVector2 m_flySpeed;			//Ʈ����ٶ�

	int m_maxAtomSum;			//���������
	float m_snowDensity;		//ѩƮ����ܶ�
//	_XEchoParticles *m_atom;	//ѩ������
	_XAloneParticles *m_atom;	//ѩ������

	const _XTexture *m_texture;
	//int m_w;
	//int m_h;
public:
	int init(const _XVector2& productArea,const _XRect& liveArea,const _XVector2& flySpeed,
		int maxAtomSum,float snowDensity,const _XTexture *texture);
	void reset();
	void move(int timeDelay);
	void draw() const;
	void release()
	{
		if(m_isInited == 0) return;
		XDELETE_ARRAY(m_atom);
		m_isInited = 0;		//��ֹrelease֮�����������Ա��������ɴ���
	}
	void setEnd()
	{
		if(m_isInited == 0 ||
			m_isEnd != 0 ||
			m_isSetEnd != 0) return;

		m_isSetEnd = 1;
	}
	_XSnowParticles();
};
#endif