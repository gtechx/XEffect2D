#ifndef _XMOVIE_DLL_
#define _XMOVIE_DLL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XEffeEngine.h"

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#include "windows.h"

extern "C"
{
	DECLDIR bool startHook();	//��ʼ������
	DECLDIR void stopHook();	//��������
	DECLDIR bool popAInputEvent(_XInputEvent & e);	//���б��е���һ������ʱ��
	DECLDIR std::string getCurWindowTitle();
}

#endif