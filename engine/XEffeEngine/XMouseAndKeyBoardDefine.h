#ifndef _JIA_XMOUSEANDKEYBOARDDEFINE_
#define _JIA_XMOUSEANDKEYBOARDDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
namespace XE{
//�����ǹ������ͼ��̵�һЩ������Ķ���
//enum XMouseState
//{
//	MOUSE_STATE_MOVE,		//����ƶ�
//	MOUSE_STATE_DOWN,		//��갴��
//	MOUSE_STATE_UP			//��굯��
//};
//enum XMouseKeyOrder
//{
//	MOUSE_KEY_ORDER_LEFT,		//������
//	MOUSE_KEY_ORDER_MIDDLE,		//����м�
//	MOUSE_KEY_ORDER_RIGHT,		//����Ҽ�
//	MOUSE_KEY_ORDER_NULL		//û�а���
//};
enum XKeyState
{
	KEY_STATE_DOWN,		//��������
	KEY_STATE_UP		//��������
};
enum XMouseState
{
	MOUSE_MOVE,					//����ƶ�
	MOUSE_LEFT_BUTTON_DOWN,		//����������
	MOUSE_LEFT_BUTTON_UP,		//����������
	MOUSE_RIGHT_BUTTON_DOWN,	//����Ҽ�����
	MOUSE_RIGHT_BUTTON_UP,		//����Ҽ�����
	MOUSE_MIDDLE_BUTTON_DOWN,	//����м�����
	MOUSE_MIDDLE_BUTTON_UP,		//����м�����
	MOUSE_WHEEL_UP_DOWN,		//���������ϰ�ť����
	MOUSE_WHEEL_UP_UP,			//���������ϰ�ť����
	MOUSE_WHEEL_DOWN_DOWN,		//���������°�ť����
	MOUSE_WHEEL_DOWN_UP,		//���������°�ť����
	MOUSE_LEFT_BUTTON_DCLICK,	//���˫��
};
//#include "SDL.h"
enum XKeyValue
{
	//��ͨ����
	XKEY_A,	//��ĸ����
	XKEY_B,
	XKEY_C,
	XKEY_D,
	XKEY_E,
	XKEY_F,
	XKEY_G,
	XKEY_H,
	XKEY_I,
	XKEY_J,
	XKEY_K,
	XKEY_L,
	XKEY_M,
	XKEY_N,
	XKEY_O,
	XKEY_P,
	XKEY_Q,
	XKEY_R,
	XKEY_S,
	XKEY_T,
	XKEY_U,
	XKEY_V,
	XKEY_W,
	XKEY_X,
	XKEY_Y,
	XKEY_Z,

	XKEY_0,	//���ְ���
	XKEY_1,
	XKEY_2,
	XKEY_3,
	XKEY_4,
	XKEY_5,
	XKEY_6,
	XKEY_7,
	XKEY_8,
	XKEY_9,

	XKEY_UNKNOWN,//SDLK_FIRST
	XKEY_BACKSPACE,
	XKEY_TAB,
	XKEY_CLEAR,
	XKEY_RETURN,
	XKEY_PAUSE,
	XKEY_ESCAPE,
	XKEY_SPACE,
	XKEY_EXCLAIM,	//!
	XKEY_QUOTEDBL,	//"
	XKEY_HASH,		//#
	XKEY_DOLLAR,	//$
	XKEY_PERSENT,	//%
	XKEY_AMPERSAND,	//&
	XKEY_QUOTE,		//'
	XKEY_LEFTPAREN,	//(
	XKEY_RIGHTPAREN,//)
	XKEY_ASTERISK,	//*
	XKEY_PLUS,		//+
	XKEY_COMMA,		//,
	XKEY_MINUS,		//-
	XKEY_PERIOD,	//.
	XKEY_SLASH,		///
	XKEY_COLON,		//:
	XKEY_SEMICOLON,	//;
	XKEY_LESS,		//<
	XKEY_EQUALS,	//=
	XKEY_GREATER,	//>
	XKEY_QUESTION,	//?
	XKEY_AT,		//@
	XKEY_LEFTBRACKET,//[
	XKEY_BACKSLASH,	//��б��
	XKEY_RIGHTBRACKET,//]
	XKEY_CARET,		//^
	XKEY_UNDERSCORE,//_
	XKEY_BACKQUOTE,	//`
	XKEY_DELETE,	
	XKEY_LEFTBRACES,	//{
	XKEY_SEPARATOR,		//|
	XKEY_RIGHTBRACES,	//}
	XKEY_WAVE,			//~

	XKEY_N0,	//С���̰���
	XKEY_N1,	
	XKEY_N2,
	XKEY_N3,
	XKEY_N4,
	XKEY_N5,
	XKEY_N6,
	XKEY_N7,
	XKEY_N8,
	XKEY_N9,
	XKEY_N_PERIOD,	//.
	XKEY_N_DIVIDE,	///
	XKEY_N_MULTIPLY,//*
	XKEY_N_MINUS,	//-
	XKEY_N_PLUS,	//+
	XKEY_N_ENTER,
	XKEY_N_EQUALS,	//=
	//���򰴼���
	XKEY_UP,
	XKEY_DOWN,
	XKEY_RIGHT,
	XKEY_LEFT,
	XKEY_INSERT,
	XKEY_HOME,
	XKEY_END,
	XKEY_PAGEUP,
	XKEY_PAGEDOWN,
	//������
	XKEY_F1,
	XKEY_F2,
	XKEY_F3,
	XKEY_F4,
	XKEY_F5,
	XKEY_F6,
	XKEY_F7,
	XKEY_F8,
	XKEY_F9,
	XKEY_F10,
	XKEY_F11,
	XKEY_F12,
	XKEY_F13,
	XKEY_F14,
	XKEY_F15,

	XKEY_NUMLOCK,
	XKEY_CAPSLOCK,
	XKEY_SCROLLOCK,
	XKEY_RSHIFT,
	XKEY_LSHIFT,
	XKEY_RCTRL,
	XKEY_LCTRL,
	XKEY_RALT,
	XKEY_LALT,
	XKEY_RMETA,
	XKEY_LMETA,
	XKEY_LSUPER,		//Left "Windows" key 
	XKEY_RSUPER,		//Right "Windows" key 
	XKEY_MODE,			//"Alt Gr" key 
	XKEY_COMPOSE,	//Multi-key compose key

	XKEY_HELP,
	XKEY_PRINT,
	XKEY_SYSREQ,
	XKEY_BREAK,
	XKEY_MENU,
	XKEY_POWER,		//Power Macintosh power key
	XKEY_EURO,			//Some european keyboards
	XKEY_UNDO,			//Atari keyboard has Undo
	XKEY_MAX,
};
namespace XEE
{
	XKeyValue charToKeyValue(char p);	//��ASCIIӳ��Ϊ����ֵ
	char keyValueToChar(XKeyValue v);	//������ֵӳ��ΪASCII
	std::string keyValueToString(XKeyValue v);	//������ֵӳ��Ϊ��������
	XKeyValue vkToKeyValue(unsigned char vk,bool withShift);	//�����ⰴ��ӳ��Ϊ����ֵ
}
}
#endif