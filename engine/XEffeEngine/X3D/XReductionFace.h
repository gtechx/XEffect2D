#ifndef _JIA_XREDUCTIONFACE_
#define _JIA_XREDUCTIONFACE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
//����һ��ģ���Զ�������㷨����
//(��δ���)
#include "XBasic3D.h"

#include <vector>
#include <list>
namespace XE{
class XReductionFace
{
private:
	XBool m_isInited;
public:
//	void reductionFace(std::vector<XVector3> &v,	//���������
//		std::vector<XFaceInfo> &f,
//		std::vector<int> &map,
//		std::vector<int> &permutation);

	XReductionFace()
		:m_isInited(XFalse)
	{}
};
}
#endif