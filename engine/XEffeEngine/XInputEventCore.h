#ifndef _JIA_XINPUTEVENTCORE_
#define _JIA_XINPUTEVENTCORE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XMouseAndKeyBoardDefine.h"
namespace XE{
//�����¼�������
enum XInputType
{
	EVENT_NULL,		//��Ч���¼�
	EVENT_MOUSE,	//����¼�
	EVENT_KEYBOARD,	//�����¼�
	EVENT_TOUCH,	//�������¼�
	EVENT_JOYSTICK,	//�ֱ��¼�
	EVENT_SYSTEM,	//ϵͳ�¼�
	EVENT_RESIZE,	//�ı䴰���¼�
	EVENT_EXIT,		//�˳��¼�
};
enum XTouchState
{
	TOUCH_DOWN,
	TOUCH_MOVE,
	TOUCH_UP,
};
//�����¼�(��δ��ɣ����Ը�����������)
struct XInputEvent
{
	XInputType type;	//�����¼�������
	int touchID;				//������ID��
	XTouchState touchState;	//������״̬

	XKeyState keyState;	//���̵�״̬
	XKeyValue keyValue;	//���̵ļ�ֵ
	XMouseState mouseState;	//���״̬
	//����λ��,�������������ڴ����е���ʵλ�ã�û�о����������ź�ƽ�ƣ�������ò�Ҫֱ��ʹ���������
	//����ʹ�þ���ת����getMousePos;����
	int mouseX;
	int mouseY;
	unsigned short unicode;
	XInputEvent()
		:type(EVENT_NULL)
	{}
	bool isKeyBoardDown()const { return type == EVENT_KEYBOARD && keyState == KEY_STATE_DOWN; }
	bool isKeyBoardUp()const { return type == EVENT_KEYBOARD && keyState == KEY_STATE_UP; }
	bool isMouseLeftDClick()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_DCLICK; }
	bool isMouseLeftDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_DOWN; }
	bool isMouseLeftUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_LEFT_BUTTON_UP; }
	bool isMouseRightDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_RIGHT_BUTTON_DOWN; }
	bool isMouseRightUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_RIGHT_BUTTON_UP; }
	bool isMouseMove()const { return type == EVENT_MOUSE && mouseState == MOUSE_MOVE; }
	bool isMouseMiddleDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_MIDDLE_BUTTON_DOWN; }
	bool isMouseMiddleUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_MIDDLE_BUTTON_UP; }
	bool isMouseWheelDown()const { return type == EVENT_MOUSE && mouseState == MOUSE_WHEEL_DOWN_DOWN; }
	bool isMouseWheelUp()const { return type == EVENT_MOUSE && mouseState == MOUSE_WHEEL_UP_DOWN; }
};
//�����Ƿ���δ������¼�
//inline bool getInputEvent(XInputEvent &event);
}
#endif