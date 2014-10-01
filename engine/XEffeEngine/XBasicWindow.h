#ifndef _JIA_XBASICWINDOW_
#define _JIA_XBASICWINDOW_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XBasicFun.h"
#include "XCommonDefine.h"
#include "XInputEventCommon.h"

class _XFColor;
class _XWindowTitle;
class _XFontUnicode;
namespace XEE
{ 
	extern int windowWidth;		//��ǰ���ڵĿ��
	extern int windowHeight;	//��ǰ���ڵĸ߶�
	extern int sceneX;			//������λ��	//Ŀǰ��������в��������ںܶ�ģʽ�²�û��Ч��
	extern int sceneY;			//������λ��
	extern int sceneWidth;		//�����Ŀ��
	extern int sceneHeight;		//�����ĸ߶�
	extern float sceneSizeX;		//���ڵ����ű���
	extern float sceneSizeY;		//���ڵ����ű���
	extern float viewAngle3D;		//3D�۲�ĽǶ�
	extern _XWindowData windowData;	//���ڵ��������
	extern _XResourcePosition defaultResourcePosition;
	extern bool isWindowExit;		//�Ƿ�׼���˳�����
	extern bool isAlwaysTop;		//�����Ƿ���������ǰ��

	extern int frameTime;	//��һ֡��ʱ��
	extern float nowFPS;	//��ǰ��֡��

	extern int audioFrequency;	//��Ƶ��ʼ����Ƶ��
	extern int audioFormat;		//��Ƶ�ĸ�ʽ
	extern int audioChannel;	//��Ƶ������
	extern _XVector2 mousePosition;	//��ǰ�����λ��

	extern void release();
	extern void inputEvent(const _XInputEvent& inputEvent);
	extern void keyProc(int key,_XKeyState keyState);
	extern void mouseProc(int x,int y,_XMouseState mouseState);
	extern LRESULT (*customWinProc)(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam,bool &end);	//�Զ����ϵͳ��Ϣ������
	extern HWND wHandle;
	extern HDC wHDC;	//����������ָ��
	extern HGLRC wCurrentHGLRC;	//��ǰ��������
	extern HGLRC wCopyHGLRC;	//��ǰ�����ĵĸ���
	extern unsigned int mainThreadHandle;	//���̵߳�ID

	extern char isOpenGL2Support;	//OpenGL�Ƿ�֧��2.0�Ƿ�֧��GLSL
	extern int maxTetureSize;		//�Կ�֧�ֵ������ͼ�ߴ�
	extern char isLineSmooth;		//�Ƿ�ʹ�û��ߵ�ƽ������
	extern _XFColor defaultBGColor;	//Ĭ�ϵı�����ɫ
	extern char isMultiSampleSupport;	//�Ƿ�֧�ֶ��ز���

	extern _XBool stopFlag;	//�Ƿ���ͣ
	extern int manualFPS;	//-1�ǲ�����֡��
	extern float manualFrameTime;	//���õ�ÿ֡��ʱ��
	extern _XBool needReset3DParam;	//�Ƿ���Ҫ�ظ�3D����������

	extern _XBool haveSystemFont;
	extern _XFontUnicode systemFont;
	extern _XBool isSuspend;	//�����Ƿ�����жϣ���������жϵĻ�Ҫ�˳�loading�Ķ��߳�
	//����ı��������Զ���ʾ�汾��
	extern int showVersionTimer;			//��ʾ�汾�ŵļ�ʱ��
	extern _XFontUnicode showVersionStr;	//��ʾ�汾�ŵ��ַ���
	extern int autoShutDownTimer;			//�Զ��ػ��ļ�ʱ
	extern char autoShutDownState;			//�Զ��ػ���״̬��ֻ�п�Խ�趨ʱ��Żᴥ���Զ��ػ�
	//�����ǹ����Զ����������֧��
	extern _XWindowTitle customWinTitle;	//�Զ���Ĵ��ڱ���
	inline void setStop(int temp = -1)	//ֹͣ 0,play,1,stop,-1,change state
	{
		if(temp > 0) XEE::stopFlag = XTrue; 
		else if(temp == 0) XEE::stopFlag = XFalse; 
		else
		if(temp < 0)
		{
			XEE::stopFlag = !XEE::stopFlag;
		}
	}
	inline _XBool getStopState()
	{
		return XEE::stopFlag;
	}
	inline void setFPS(int temp)		//������Ϸ֡�ʣ�С��0Ϊ������֡��(Ŀǰ�����������û����Ӧ�е�����)
	{
		if(temp <= 0) return;	//�����������
		XEE::manualFPS = temp;
		XEE::manualFrameTime = 2000.0f / temp;
	}

}

extern bool createWindow(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0,int withFrame = 1);	//��ʼ��SDL����
inline void setWindowPos(int x,int y,bool always = false)
{//���ô��ڵ�λ��
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	XEE::isAlwaysTop = always;
	if(XEE::isAlwaysTop) SetWindowPos(XEE::wHandle,HWND_TOPMOST,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
	else SetWindowPos(XEE::wHandle,HWND_TOP,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
}
inline void getWindowPos(int &x,int &y)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	x = rect.left;
	y = rect.top;
}
inline void getWindowSize(int &w,int &h)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}
inline void minSizeWindow(){ShowWindow(XEE::wHandle,SW_MINIMIZE);}
inline void exitWindow(){XEE::isWindowExit = true;}	//�˳�����
inline void setWindowSize(float x,float y)
{
	RECT rect;
	GetWindowRect(XEE::wHandle,&rect);
	if(XEE::isAlwaysTop) SetWindowPos(XEE::wHandle,HWND_TOPMOST,rect.left,rect.top,x,y,SWP_SHOWWINDOW);
	else SetWindowPos(XEE::wHandle,HWND_TOP,rect.left,rect.top,x,y,SWP_SHOWWINDOW);
}
//���ô��ڵ� ͸����
inline void setWindowAlpha(float a)
{
	//bool isFirst = true;
	//if(isFirst)
	//{
	//	SetWindowLong(XEE::wHandle, GWL_EXSTYLE, GetWindowLong(XEE::wHandle, GWL_EXSTYLE) | WS_EX_LAYERED);
	//	isFirst = false;
	//}
	////SetLayeredWindowAttributes(XEE::wHandle,RGB(64,64,64),0,LWA_COLORKEY);
	//SetLayeredWindowAttributes(XEE::wHandle,0,a * 255,LWA_ALPHA);
}

#endif