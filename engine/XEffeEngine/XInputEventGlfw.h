#ifndef _JIA_XINPUTEVENTGLFW_
#define _JIA_XINPUTEVENTGLFW_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#include "windows.h"
#if CREATE_WINDOW_METHOD == 2
namespace XE {
//�����Ƿ���δ������¼�
extern bool getInputEventGlfw(XInputEvent &event);
}
#endif
#endif