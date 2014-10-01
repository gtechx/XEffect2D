#ifndef _JIA_XINPUTEVENTCORE_
#define _JIA_XINPUTEVENTCORE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XMouseAndKeyBoardDefine.h"

//�����¼�������
enum _XInputType
{
	EVENT_NULL,		//��Ч���¼�
	EVENT_MOUSE,	//����¼�
	EVENT_KEYBOARD,	//�����¼�
	EVENT_JOYSTICK,	//�ֱ��¼�
	EVENT_SYSTEM,	//ϵͳ�¼�
	EVENT_RESIZE,	//�ı䴰���¼�
	EVENT_EXIT,		//�˳��¼�
};
//�����¼�(��δ��ɣ����Ը�����������)
struct _XInputEvent
{
	_XInputType type;	//�����¼�������

	_XKeyState keyState;	//���̵�״̬
	_XKeyValue keyValue;	//���̵ļ�ֵ

	_XMouseState mouseState;	//���״̬
	int mouseX;					//����λ��
	int mouseY;
	unsigned short unicode;
	_XInputEvent()
		:type(EVENT_NULL)
	{}
};
//�����Ƿ���δ������¼�
//inline bool getInputEvent(_XInputEvent &event);

#endif