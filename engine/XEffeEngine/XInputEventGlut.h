#ifndef _JIA_XINPUTEVENTGLUT_
#define _JIA_XINPUTEVENTGLUT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#if CREATE_WINDOW_METHOD == 1
namespace XE{
//�����Ƿ���δ������¼�
inline bool getInputEventGlut(XInputEvent &) {return true;}
}
#endif
#endif