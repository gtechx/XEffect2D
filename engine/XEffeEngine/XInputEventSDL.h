#ifndef _JIA_XINPUTEVENTSDL_
#define _JIA_XINPUTEVENTSDL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#if CREATE_WINDOW_METHOD == 0
#include "SDL.h"
#include "windows.h"
namespace XE {
//�����Ƿ���δ������¼�
extern bool SDLEventToInputEvent(XInputEvent &event, const SDL_Event &tmpEvent);	//��SDL�������¼�ת����XEE�������¼�
//#ifdef _WIN64	//64λ����ĵ��û�crash����Ҫ���¶�SDL��ʹ��
//inline bool getInputEventSDL(XInputEvent &event) { return false; }
//#else
inline bool getInputEventSDL(XInputEvent &event)
{
	SDL_Event tmpEvent;		//SDL�¼����
	if (SDL_PollEvent(&tmpEvent) == 0) return false;	//��x64�´˴�Ҳ��crash���ⲻ��
	//����ת���¼�
	return SDLEventToInputEvent(event, tmpEvent);
}
//#endif
}
#endif
#endif