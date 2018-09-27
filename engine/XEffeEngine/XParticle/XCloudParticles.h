#ifndef _JIA_XCLOUDPARTICLES_
#define _JIA_XCLOUDPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//���Ƶ�����Ч��
//��Ҫע�����,��ֻ��һ��Ч����ʵ��,Ŀǰû�п�������ͨ����
//�ں�̵���չ�н��ῼ������ͨ����
#include "XParticle.h"
namespace XE{
class XCloudParticles
{
private:
	char m_isInited;				//�Ƿ��ʼ��
	char m_isEnd;				//�Ƿ����
	char m_isSetEnd;				//�Ƿ����ý���

	float m_curCenterPoint;		//��ǰ���Ʋ������ĵ����ڵ�λ��	(�仯��Ҫ������)
	float m_maxCenterPoint;		//���Ʋ������ĵ�������Χ
	float m_minCenterPoint;		//���Ʋ������ĵ����С���Χ
	float m_maxSize;			//���Ʋ�����Χ������
	float m_minSize;			//���Ʋ�����Χ������
	float m_curSize;			//���Ʋ����ĵ�ǰ��Χ	(�仯��Ҫ������)
	float m_density;			//��ǰ���Ʋ������ܶ�	(�仯��Ҫ������)
	XRect m_liveArea;			//������������Ŀռ�
	XVec2 m_speed;			//�����ƶ����ٶ�	(�仯��Χ����̫��)

	int m_maxAtomSum;			//���������
	XAloneParticles *m_atom;	//����

	const XTexture *m_texture;			//��ͼ
//	int m_w;
//	int m_h;

public:
	int init(float maxCenterPoint,float minCenterPoint,float maxSize,float minSize,
		float density,const XRect& liveArea,const XVec2& speed,int maxAtomSum,const XTexture *tex);
	void reset();
	void move(float timeDelay);
	void draw() const;
	void release();
	void setEnd();
	XCloudParticles();
};
inline void XCloudParticles::release()
{
	if(m_isInited == 0) return;
	XMem::XDELETE_ARRAY(m_atom);
	m_isInited = 0;		//��ֹrelease֮�����������Ա��������ɴ���
}

inline void XCloudParticles::setEnd()
{
	if(m_isInited == 0 ||
		m_isEnd != 0 ||
		m_isSetEnd != 0) return;
	m_isSetEnd = 1;
}
}
#endif