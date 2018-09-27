#ifndef _JIA_XSMOOTHCURVE_
#define _JIA_XSMOOTHCURVE_
#include "XMath\XMath.h"
#include "XMath\XVector2.h"
//��������֮�����һ���⻬�����ߣ��Ա��ڿ�������������˶�
namespace XE{
struct XSmoothCurveInfo
{
	float m_cycleSum;	//�����ڵ�����
	float m_maxOffset;	//����ƫ����
	bool m_isAdd;
};
class XSmoothCurve
{
private:
	XVec2 m_start;		//���ߵ����
	XVec2 m_end;			//���ߵ��յ�
	std::vector<XSmoothCurveInfo> m_infos;
	bool m_isInited;
public:
	XSmoothCurve()
		:m_isInited(false)
	{}
	//s���ߵ����
	//e���ߵ��յ�
	//minS:�ߵ���С��
	//maxS:�ߵ����׶�
	//offsetRate:ƫ����
	//��Խ����Խ����
	void initCurve(const XVec2& s,const XVec2& e,int minS = 1,int maxS = 1,float offsetRate = 1.0f);
	//[0.0f - 1.0f]
	XVec2 getPosition(float rate);
	//density���ߵ��ܶȣ���ֵԽ������Խƽ��
	void draw(int density = 1000);
};
}
#endif