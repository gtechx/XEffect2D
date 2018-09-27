#ifndef XPHYSICSBASIC_
#define XPHYSICSBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.8.3
//--------------------------------
//#include "XBasicClass.h"
#include "..\XMath\XVector2.h"
namespace XE{
//extern XVec2 constGravity(0,9.82);		//��������
union XPhYsicObjectAttribute
{
	unsigned short canMove:1;		//�����Ƿ����ƶ�
	unsigned short canCollide:1;	//�����Ƿ�����ײ
	unsigned short canFlag00:1;		//��־
	unsigned short canFlag01:1;		//��־
	unsigned short canFlag02:1;		//��־
	unsigned short canFlag03:1;		//��־
	unsigned short canFlag04:1;		//��־
	unsigned short canFlag05:1;		//��־
	unsigned short canFlag06:1;		//��־
	unsigned short canFlag07:1;		//��־
	unsigned short canFlag08:1;		//��־
	unsigned short canFlag09:1;		//��־
	unsigned short canFlag0b:1;		//��־
	unsigned short canFlag0c:1;		//��־
	unsigned short canFlag0d:1;		//��־
};
class XBasicPhysic2DObject
{
private:
	char m_isEnable;						//�����Ƿ���Ч
	XPhYsicObjectAttribute m_attribute;	//�����һЩ���ԣ�����˵�Ƿ����˶����Ƿ���Դ�͸��
	float m_mass;							//���������
	XVec2 m_speed;						//������ٶ�
	float m_density;						//������ܶ�
	float m_volume;							//��������
	float m_collideEnergyWastageRadio;		//������ײʱ���������ϵ��[0 - 1]
	float m_airResistanceRadio;				//�˶����������ڿ�������ϵ�������е��ٶ� �� ��������ϵ�� = ��������
	float m_surfaceFrictionCoefficient;		//����ı���Ħ��ϵ��������������ڱ����������˶�ʱ���Ħ��ϵ����Ӵ����Ħ��ϵ����˻����ܵ�Ħ��ϵ��

	XVec2 m_position;					//�������ڵ�λ��
	float m_rotate;							//�������ת�Ƕ�
	XVec2 m_centreOfGravity;			//���ĵ�λ�ã�Ĭ�������ģ�
	float m_rotationSpeed;					//������ת�Ľ��ٶ�
public:
	XBasicPhysic2DObject()
	{}
	virtual ~XBasicPhysic2DObject(){}
};
}
#endif