#ifndef XSPRING_
#define XSPRING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XPhysicsBasic.h"
namespace XE{
class XSpring:public XBasicPhysic2DObject
{
private:
	float m_normalLength;	//���ɵ���Ȼ��չ����
	float m_minLength;		//����ѹ������С���ȣ�������С���ȵ�ѹ���ͳ���һ������3��
	float m_maxLength;		//������չ����󳤶ȣ�����������Ƚ��᲻����չ���߶��ѣ�������������ö�����
	float m_springRatio;	//���ɵĵ���ϵ��
	float m_curLength;		//���ɵĵ�ǰ����

	float m_curForce;		//���ɵ�ǰ������
public:
	int init();					//��ʼ�����ɵ�����
	int move(float timeDelay);	//���ɵ��˶�

	XSpring()
	{
	}
};
}
#endif