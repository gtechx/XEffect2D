#ifndef _JIA_GFIREFLY_
#define _JIA_GFIREFLY_
#include "XMath\XMath.h"
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XTextureInformation.h"
#include <deque>

#define WITH_DRAW_OP	//�Ƿ�ʹ������Ż�
namespace XE {
struct XObject
{
	XE::XVec2 m_pos;		//λ��
	XE::XVec2 m_maxSpeed;	//�����ٶ�
	XE::XVec2 m_curSpeed;	//��ǰ���ٶ�

	float m_lightRecoveryRate;	//���ȵĻָ��ٶ�
	float m_energy;	//����
	float m_maxEnergy;
	float m_light;	//����
	float m_cycle;	//��������
	float m_cycleSpeed;	//�����ٶ�
	XE::XFColor m_color;	//��ɫ
	float m_scale;	//����
	float m_maxLigth;	//��Ȼ�����������������

	bool m_isNear;	//�Ƿ��ǽ�������
	bool update(float steptime);
#ifdef WITH_DRAW_OP
	void draw();
#else
	void draw(unsigned int tex);
#endif
};
class XRandBG;
class XFirefly
{
private:
	bool m_isInited;

	std::vector<XObject> m_allObjs;
	std::deque<XObject*> m_freeObj;
	std::deque<XObject*> m_usedObj;
	XE::XTextureData m_objTxt;

	int m_w;	//���ӳ��ֵķ�Χ
	int m_h;	//���ӳ��ֵķ�Χ
	float m_bornRate;	//ÿ�����������ӵ�����
	float m_bornSum;
public:
	//������һЩ���õĹ�������
	float m_minSpeed;
	float m_maxSpeed;
	float m_minEnergy;
	float m_maxEnergy;
	float m_minCycleSpeed;
	float m_maxCycleSpeed;
	float m_minRecoveryRate;
	float m_maxRecoveryRate;
	float m_colorBase;
	float m_colorArea;
	float m_minLight;
	float m_maxLight;
	float m_nearRate;

	void resetParameter();
public:
	XFirefly()
		:m_isInited(false)
		, m_w(512)
		, m_h(512)
		, m_bornRate(2048.0f)
		, m_bornSum(0.0f)
	{}
	bool init(int maxObjSum, const char* texfilename, int w, int h, bool withUI = true);
	void update(float steptime, const XRandBG& bg);
	void draw();
	int getObjSum()const { return m_usedObj.size(); }
};
}
#endif