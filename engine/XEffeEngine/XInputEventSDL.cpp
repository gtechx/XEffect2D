#include "XInputEventSDL.h"
#include "XWindowCommon.h"

bool isSDLDoubleClick(const SDL_Event &tmpEvent)
{
	static int upTime = SDL_GetTicks();
	static int counter = 0;	//���¼�����
	if(tmpEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		if(tmpEvent.button.button == SDL_BUTTON_LEFT)
		{
			if(counter == 0)
			{//��һ�ΰ���
				upTime = SDL_GetTicks();
				++counter;
			}else
			{//�ڶ��ΰ���
				if(SDL_GetTicks() - upTime < 500)	//�����ж�ʱ��Ϊ200ms
				{//˫��ʱ�����
					counter = 0;
					//printf("���˫���¼�\n");
					return true;
				}else
				{
					upTime = SDL_GetTicks();
				}
			}
		}else
		{
			counter = 0;	//ȡ����¼
		}
	}else
	if(tmpEvent.type == SDL_MOUSEMOTION && counter != 0)
	{
		counter = 0;	//ȡ����¼
	}
	return false;
}

bool SDLEventToInputEvent(_XInputEvent &event,const SDL_Event &tmpEvent)	//��SDL�������¼�ת����XEE�������¼�
{
	switch(tmpEvent.type)
	{
	case SDL_VIDEORESIZE:
		event.type = EVENT_RESIZE;
		event.mouseX = tmpEvent.resize.w;
		event.mouseY = tmpEvent.resize.h;
		break;
	case SDL_QUIT:event.type = EVENT_EXIT;break;
	case SDL_KEYDOWN:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_DOWN;
		event.keyValue = (_XKeyValue)_XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_KEYUP:
		event.type = EVENT_KEYBOARD;
		event.keyState = KEY_STATE_UP;
		event.keyValue = (_XKeyValue)_XWindow.mapKey(tmpEvent.key.keysym.sym);
		event.unicode = tmpEvent.key.keysym.unicode;
		break;
	case SDL_MOUSEMOTION:
		event.type = EVENT_MOUSE;
		event.mouseState = MOUSE_MOVE;
		event.mouseX = tmpEvent.motion.x;
		event.mouseY = tmpEvent.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
		event.type = EVENT_MOUSE;
		switch(tmpEvent.button.button)
		{
		case SDL_BUTTON_LEFT:
			if(isSDLDoubleClick(tmpEvent)) 
				event.mouseState = MOUSE_LEFT_BUTTON_DCLICK;
			else
				event.mouseState = MOUSE_LEFT_BUTTON_DOWN;
			break;
		case SDL_BUTTON_MIDDLE:event.mouseState = MOUSE_MIDDLE_BUTTON_DOWN;break;
		case SDL_BUTTON_RIGHT:event.mouseState = MOUSE_RIGHT_BUTTON_DOWN;break;
		case SDL_BUTTON_WHEELUP:event.mouseState = MOUSE_WHEEL_UP_DOWN;break;
		case SDL_BUTTON_WHEELDOWN:event.mouseState = MOUSE_WHEEL_DOWN_DOWN;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	case SDL_MOUSEBUTTONUP:
		event.type = EVENT_MOUSE;
		switch(tmpEvent.button.button)
		{
		case SDL_BUTTON_LEFT:event.mouseState = MOUSE_LEFT_BUTTON_UP;break;
		case SDL_BUTTON_MIDDLE:event.mouseState = MOUSE_MIDDLE_BUTTON_UP;break;
		case SDL_BUTTON_RIGHT:event.mouseState = MOUSE_RIGHT_BUTTON_UP;break;
		case SDL_BUTTON_WHEELUP:event.mouseState = MOUSE_WHEEL_UP_UP;break;
		case SDL_BUTTON_WHEELDOWN:event.mouseState = MOUSE_WHEEL_DOWN_UP;break;
		}
		event.mouseX = tmpEvent.button.x;
		event.mouseY = tmpEvent.button.y;
		break;
	}
	return true;
}