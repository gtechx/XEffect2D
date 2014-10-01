//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XEffeEngine.h"
#include "XBasicWindow.h"
#include "XSoundCommon.h"
#include "XLogBook.h"
#include "XWindowCommon.h"

namespace XEE
{
	int frameTime = 0;
	float nowFPS = 0.0f;
	//SDL_Surface *screen = NULL;
	_XVector2 mousePosition;
	_XFColor defaultBGColor = _XFColor(0.25f,0.25f,0.25f,1.0f);	//Ĭ�ϵı�����ɫ

	int windowWidth = 0;	//��ǰ���ڵĿ��
	int windowHeight = 0;	//��ǰ���ڵĸ߶�
	int sceneX = 0;			//������λ��
	int sceneY = 0;			//������λ��
	int sceneWidth = 0;		//�����Ŀ��
	int sceneHeight = 0;	//�����ĸ߶�
	float sceneSizeX = 1.0f;
	float sceneSizeY = 1.0f;
	float viewAngle3D = 45.0f;
	_XWindowData windowData;
	int audioFrequency = 44100;//48000;
	int audioChannel = 2;
	int audioFormat = MIX_DEFAULT_FORMAT;
	_XResourcePosition defaultResourcePosition = RESOURCE_LOCAL_FOLDER;
	bool isWindowExit = false;
	bool isAlwaysTop = false;
	LRESULT (*customWinProc)(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam,bool &end) = NULL;
	HWND wHandle = 0;
	HDC wHDC = 0;	//����������ָ��
	HGLRC wCurrentHGLRC = 0;	//��ǰ��������
	HGLRC wCopyHGLRC = 0;	//��ǰ�����ĵĸ���
	unsigned int mainThreadHandle = 0xffffffff;

	char isOpenGL2Support = 0;
	int maxTetureSize = 1024;
	char isLineSmooth = 1;
	char isMultiSampleSupport = 0;

	_XBool stopFlag = XFalse;	//�Ƿ���ͣ
	int manualFPS = -1;	//-1�ǲ�����֡��
	float manualFrameTime;	//���õ�ÿ֡��ʱ��
	_XBool needReset3DParam = XFalse;

	_XBool haveSystemFont = XFalse;
	_XFontUnicode systemFont;
	_XBool isSuspend = XFalse;

	int showVersionTimer = 0;			//��ʾ�汾�ŵļ�ʱ��
	_XFontUnicode showVersionStr;	//��ʾ�汾�ŵ��ַ���
	int autoShutDownTimer = 0;
	char autoShutDownState = -1;

	_XWindowTitle customWinTitle;	//�Զ���Ĵ��ڱ���

	#include <wincon.h>
	extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
	//�����Լ�����Ϣ������
	//����Ϣ�������´�
	WNDPROC currentWndProc;
	static LRESULT CALLBACK winMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) 
	{
		if(customWinProc != NULL) 
		{
			bool end = false;
			LRESULT ret = customWinProc(hwnd,Msg,wParam,lParam,end);
			if(end) return ret;
		}
		return CallWindowProc(currentWndProc,hwnd,Msg,wParam,lParam);
	}
	//--------------------------------------
	static void registerMyMsgProc()
	{
		//DragAcceptFiles (wHandle, TRUE);	//�Ƿ������򴰿���ק�ļ�
		//����1
		//currentWndProc = (WNDPROC)SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
		//����2
		currentWndProc = (WNDPROC)GetWindowLongPtr(wHandle, GWL_WNDPROC);
		SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
	}
}
bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame)
{
	if(!_XWindow.createWindow(width,height,windowTitle,isFullScreen,withFrame)) return false;
	XEE::windowWidth = width;
	XEE::windowHeight = height;
	//��ʼ��һЩ����
	XEE::wHDC = wglGetCurrentDC();
	XEE::wCurrentHGLRC = wglGetCurrentContext();
	XEE::wCopyHGLRC = wglCreateContext(XEE::wHDC);
	//wglCopyContext(XEE::wCurrentHGLRC,XEE::wCopyHGLRC,GL_ALL_ATTRIB_BITS);
	wglShareLists(XEE::wCurrentHGLRC,XEE::wCopyHGLRC);
	XEE::wHandle = WindowFromDC(XEE::wHDC);
	XEE::mainThreadHandle = GetCurrentThreadId();

	if(windowTitle != NULL) _XWindow.setWindowTitle(windowTitle);	//���ô��ڱ���
	if(XEE::windowData.isInitWindowPos
		|| XEE::windowData.isAlwaysTop) setWindowPos(XEE::windowData.windowPosX,XEE::windowData.windowPosY,XEE::windowData.isAlwaysTop);

	XEE::registerMyMsgProc();

	_XSoundHandle.init();
	return true;
}

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"
#include "XSound.h"
namespace XEE
{
	_XVector2 getMousePosFromWindow(int mouseX,int mouseY)	//ϵͳ����ռ��򴰿�����ռ����ת��
	{
		float x = 0,y = 0;
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		}
		return _XVector2(x,y);
	}
	void release()
	{
		_XSound::GetInstance().release();
		_XSoundHandle.close();
		_XWindow.release();
	}
	void keyProc(int key,_XKeyState keyState)
	{
		_XCtrlManger.keyProc(key,keyState);
#if WITH_OBJECT_MANAGER
		_XObjManger.keyProc(key,keyState);
#endif
	}
	void mouseProc(int x,int y,_XMouseState mouseState)
	{
	/*	_XVector2 tempPos(x,y);
		switch(mouseState)
		{
		case MOUSE_MOVE:
			XEE::mousePosition = tempPos = getMousePosFromWindow(x,y);
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_RIGHT_BUTTON_DOWN:
			if(XEE::windowData.isFullScreen != 0 && XEE::windowData.isTouch != 0)
				tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
			else
				tempPos = getMousePosFromWindow(x,y);
			break;
		case MOUSE_LEFT_BUTTON_UP:
		case MOUSE_RIGHT_BUTTON_UP:
			tempPos = getMousePosFromWindow(x,y);
			break;
		case MOUSE_MIDDLE_BUTTON_DOWN:
		case MOUSE_MIDDLE_BUTTON_UP:
			break;
		}
		_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
		_XObjManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
		*/
		//����2,��Ӱ��������ƣ������ٶȿ�
		_XVector2 tempPos(x,y);
		switch(mouseState)
		{
		case MOUSE_MOVE:
			XEE::mousePosition = tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
			_XObjManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
			if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if(XEE::windowData.isFullScreen != 0 && XEE::windowData.isTouch != 0)
				tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
			else
				tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
			_XObjManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
			if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		case MOUSE_RIGHT_BUTTON_DOWN:
			if(XEE::windowData.isFullScreen != 0 && XEE::windowData.isTouch != 0)
				tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
			else
				tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
			//if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		case MOUSE_LEFT_BUTTON_UP:
			tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#if WITH_OBJECT_MANAGER
			_XObjManger.mouseProc(tempPos.x,tempPos.y,mouseState);
#endif
			if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		case MOUSE_RIGHT_BUTTON_UP:
			tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
			//if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		case MOUSE_MIDDLE_BUTTON_DOWN:
		case MOUSE_MIDDLE_BUTTON_UP:
			break;
		case MOUSE_WHEEL_UP_DOWN:
		case MOUSE_WHEEL_UP_UP:
		case MOUSE_WHEEL_DOWN_DOWN:
		case MOUSE_WHEEL_DOWN_UP:
			tempPos = getMousePosFromWindow(x,y);
			_XCtrlManger.mouseProc(tempPos.x,tempPos.y,mouseState);
			//if(XEE::windowData.withCustomTitle) XEE::customWinTitle.mouseProc(tempPos.x,tempPos.y,mouseState);
			break;
		}
	}

	void inputEvent(const _XInputEvent& inputEvent)
	{
		switch(inputEvent.type)
		{
		case EVENT_KEYBOARD:
			{
				switch(inputEvent.keyState)
				{
				case KEY_STATE_DOWN:	//���̰���
#ifdef XEE_OS_WINDOWS	//windowsϵͳ��ͨ�����뷨�������ַ�
					if(inputEvent.unicode != 0)
					{//�ַ�����
						char temp[2];
						memcpy(temp,&inputEvent.unicode,2);
						if(temp[0] == 0) _XCtrlManger.insertChar(&temp[1],1); else
						if(temp[0] < 128 && temp[1] == 0) 
						{
							if(temp[0] >= ' ' && temp[0] <= '~') _XCtrlManger.insertChar(temp,1);
						}
						else _XCtrlManger.insertChar(temp,2);
					}
#endif
					keyProc(inputEvent.keyValue,KEY_STATE_DOWN);
					break;
				case KEY_STATE_UP:	//���̵���
					keyProc(inputEvent.keyValue,KEY_STATE_UP);
					break;
				}
			}
			break;
		case EVENT_MOUSE:
			mouseProc(inputEvent.mouseX,inputEvent.mouseY,inputEvent.mouseState);
			break;
		}
	}
}//namespace XEE