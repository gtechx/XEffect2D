#ifndef _JIA_XINPUTEVENTSDL_
#define _JIA_XINPUTEVENTSDL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"
#include "SDL.h"
namespace XE{
//�����Ƿ���δ������¼�
extern bool SDLEventToInputEvent(XInputEvent &event,const SDL_Event &tmpEvent);	//��SDL�������¼�ת����XEE�������¼�
inline bool getInputEventSDL(XInputEvent &event)
{
#ifdef _WIN64
	return false;
#endif
	SDL_Event tmpEvent;		//SDL�¼����
	if(SDL_PollEvent(&tmpEvent) == 0) return false;	//��x64�´˴�Ҳ��crash���ⲻ��
	//����ת���¼�
	return SDLEventToInputEvent(event,tmpEvent);
}
}

#endif