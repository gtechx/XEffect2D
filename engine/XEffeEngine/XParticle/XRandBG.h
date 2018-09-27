#ifndef _JIA_GRANDBG_
#define _JIA_GRANDBG_
#include "XMemory.h"
#include "XShaderGLSL.h"
#include "XMath\XMath.h"
#include "XTextureInformation.h"
#include <deque>

#define WITH_DRAW_OP	//�Ƿ�ʹ������Ż�
namespace XE {
struct XBGBaseObjInfo
{
	XBGBaseObjInfo()
		:m_recoveryRate(200.0f)
	{}
	XE::XVec2 m_maxSpeed;	//�����ٶ�
	XE::XVec2 m_curSpeed;	//��ǰ���ٶ�

	XE::XVec2 m_pos;		//��ǰ��λ��
	float m_scale;			//���ű���
	float m_maxEnergy;		//�ܵ���������
	float m_releaseEnergy;	//ʣ�����������
	float m_curEnergy;		//��ǰ����������
	float m_maxStrength;	//������������
	float m_alpha;
	float m_life;			//������������
	float m_recoveryRate;	//�����ָ����ٶ�
	//�����Ƿ�������steptime������ʱ�䣬��λΪ��
	bool update(float steptime);
};

class XRandBG
{
private:
	bool m_isInited;
	XE::XBuffer m_buff;
	XE::XFBO m_fbo;
	int m_w;
	int m_h;
	std::vector<XBGBaseObjInfo> m_objs;
	std::deque<XBGBaseObjInfo*> m_freeObjs;
	std::deque<XBGBaseObjInfo*> m_usedObjs;
	XE::XTextureData m_tex;
	float m_avgBornTimer;
	float m_bornTime;

	void born();
public:
	//������һЩ���Ա����ķ�Χ
	float m_minSpeed;
	float m_maxSpeed;
	float m_minScale;
	float m_maxScale;
	float m_minEnergy;
	float m_maxEnergy;
	float m_minStrength;
	float m_maxStrength;
	float m_minRecoveryRate;
	float m_maxRecoveryRate;
	void resetParameter();
public:
	XRandBG()
		:m_isInited(false)
	{}
	bool init(int w, int h, const char* filename, int objsSum = 12,
		bool m_withUI = false);
	void update(float steptime);
	void draw();
	unsigned int getPixel(int w, int h)const
	{
		if (w < 0 || w >= m_w || h < 0 || h >= m_h) return 0;
		return m_buff.getBuffer()[w + h * m_w];
	}
	unsigned int getPixel(const XE::XVec2& pos)const
	{
		return getPixel(pos.x * m_w, pos.y * m_h);
	}
	int getW()const { return m_w; }
	int getH()const { return m_h; }
};
}
#endif