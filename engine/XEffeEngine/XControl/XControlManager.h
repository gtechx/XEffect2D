#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "XCtrlManagerBase.h"
namespace XE{
//�������Ҫʹ�õ�����ϵͳ
class XControlManager:public XCtrlManagerBase
{
public:
	XControlManager(){}
	virtual ~XControlManager(){;}
protected:
	XControlManager(const XControlManager&);
	XControlManager &operator= (const XControlManager&);
};
}
#endif