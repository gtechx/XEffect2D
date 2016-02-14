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
	EVENT_JOYSTICK,	//�ֱ��¼�
	EVENT_SYSTEM,	//ϵͳ�¼�
	EVENT_RESIZE,	//�ı䴰���¼�
	EVENT_EXIT,		//�˳��¼�
};
//�����¼�(��δ��ɣ����Ը�����������)
struct XInputEvent
{
	XInputType type;	//�����¼�������

	XKeyState keyState;	//���̵�״̬
	XKeyValue keyValue;	//���̵ļ�ֵ

	XMouseState mouseState;	//���״̬
	int mouseX;					//����λ��
	int mouseY;
	unsigned short unicode;
	XInputEvent()
		:type(EVENT_NULL)
	{}
	bool isKeyBoardDown()const{return type == EVENT_KEYBOARD && keyState == KEY_STATE_DOWN;}
	bool isKeyBoardUp()const{return type == EVENT_KEYBOARD && keyState == KEY_STATE_UP;}
};
//�����Ƿ���δ������¼�
//inline bool getInputEvent(XInputEvent &event);
}
#endif