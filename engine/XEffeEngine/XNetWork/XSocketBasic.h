#ifndef _JIA_XSOCKETBASIC_
#define _JIA_XSOCKETBASIC_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//���ﶨ�����һЩ����socket��������ؽӿ�
//#include "XOSDefine.h"
namespace XE{
#ifdef XEE_OS_WINDOWS
namespace XSock
{
	extern bool initSocket();
};
#endif
}
#endif