#ifndef _JIA_XSNOWPARTICLES_
#define _JIA_XSNOWPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//ѩ������Ʈ���Ч��
#include "XEchoParticles.h"
namespace XE{
class XSnowParticles
{
private:
	char m_isInited;				//�Ƿ��ʼ��
	char m_isEnd;				//�Ƿ����
	char m_isSetEnd;				//�Ƿ����ý���

	XVec2 m_productArea;		//������Χ�����������Ϸ���һ��ֱ�ߣ�
	XRect m_liveArea;			//���淶Χ
	XVec2 m_flySpeed;			//Ʈ����ٶ�

	int m_maxAtomSum;			//���������
	float m_snowDensity;		//ѩƮ����ܶ�
//	XEchoParticles *m_atom;	//ѩ������
	XAloneParticles *m_atom;	//ѩ������

	const XTexture *m_texture;
	//int m_w;
	//int m_h;
public:
	int init(const XVec2& productArea,const XRect& liveArea,const XVec2& flySpeed,
		int maxAtomSum,float snowDensity,const XTexture *texture);
	void reset();
	void move(float timeDelay);
	void draw() const;
	void release()
	{
		if(m_isInited == 0) return;
		XMem::XDELETE_ARRAY(m_atom);
		m_isInited = 0;		//��ֹrelease֮�����������Ա��������ɴ���
	}
	void setEnd()
	{
		if(m_isInited == 0 ||
			m_isEnd != 0 ||
			m_isSetEnd != 0) return;

		m_isSetEnd = 1;
	}
	XSnowParticles();
};
}
#endif