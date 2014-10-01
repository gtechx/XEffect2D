#ifndef _JIA_XEFFEENGINE_
#define _JIA_XEFFEENGINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//++++++++++++++++++++++++++++++++
/*˵��:
1���������ͨ��example09�ĳ������Դ�����Զ��Ż�������ע������ϵͳ����Դ����ͨ�������������Ż���
���򽫻��������ϵͳ����ͼ����ԭ����������ϵͳ��Ҫ�ϸߵ���ͼ�����ٶȣ�����ͼƬ��Դ���ܽ����Ż���
���򽫻���ʧЧ�ʣ��ۺϿ���֮��ȡ��������ϵͳ����Դ�����Ż�ƴ�ϡ�
2����Ҫע����ǣ���Դ�Ż�����Դ������loading���֮����ܱ����ã����������loading֮ǰ��ʾ����Դ��
������loading֮ʱʹ�õ���Դ��ò�Ҫ������Դ�Ż�������loading������û��ʲô���塣
3���������е���Դ����music��movie������ͨ��XResourcePack�������Դ���д���ͼ��ܣ�֮���������ֱ
�Ӵ���Դ������ȡ��Դ�ļ���
4����Ȼ����Ҳ֧����Դ���Ż�����֧Ԯ�����������ֱ��ʹ�á�
5��Ŀǰ��Դֻ��һ��loading���룬Ȼ����ʹ�ã�û�в��Թ���̬���룬֮����Ҫ���ԡ�
6��
*/
//--------------------------------

#include "glew.h"
#include <stdio.h>
#include <stdlib.h>

#include "XBasicOpenGL.h"

#include "XSprite.h"
#include "XMaskTex.h"
#include "XFrame.h"
#include "XFont.h"
#include "XFontUnicode.h"
#include "XBasicWindow.h"
#include "XOSDefine.h"

#include "XNumber.h"
#include "XNumberEx.h"
#include "XSound.h"
#include "XMusic.h"
#include "XNodeLine.h"
#include "XBezierSpline.h"
#include "XBezierInterpolation.h"
#include "XLogBook.h"
#include "XString.h"
#include "XParticle/XParticles.h"
#include "XControl/XControls.h"
#include "XPhysics/XPhysicsBasic.h"
#include "XXml.h"
#include "XCommonDefine.h"
#include "XConfigManager.h"
#include "XTimer.h"
#include "XErrorReporter.h"
#include "XThread.h"
#include "XPixelCommon.h"
#include "XInputEventCommon.h"
#include "XWindowCommon.h"
#include "XPressDataMode.h"

#include "XMovieFfmpeg.h"

#include "XEffeEngine.h"
#include "XBmp.h"
#include "XDataBasic.h"
#include "XWindowTitle.h"

#include "XTextureInformation.h"
#include "XFrameEx.h"
#include "X2DPacker.h"
#include "XResourcePack.h"

#pragma comment(lib, "../../engine/bin/XEffectEngine_lib.lib")

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif

extern _XBool setSystemFont(const char * filename);
extern _XBool readSystemConfigData(_XWindowData &data);
extern _XBool saveSystemConfigData(const _XWindowData &data);

//ע�⣺3D��1.0f����1���ף���1mm
extern _XBool initWindowEx(const _XWindowData& windowData);
inline _XBool initWindowEx(void)
{
	//if(!readSystemConfigData(XEE::windowData)) return XFalse;
	if(!readSystemConfigData(XEE::windowData)) LogStr("��ȡ�����ļ�ʧ��!");	//2014.4.13�޸�Ϊ��ȡ�����ļ�ʧ��֮���˳�������ʹ��Ĭ��ֵ��������
	return initWindowEx(XEE::windowData);
}
extern void setProjectAsDaemon();	//������ĿΪ��̨��Ŀ

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"

//�����������һЩ���ܺ���
namespace XEE
{
	//inline void errProc()
	//{
	//	_XErrorReporter::GetInstance().errorProc();
	//}
	extern int engineIdle();		//�������ѭ����������ѭ���¼�����
	extern void (*drawAfterCtrl)();	//�����ؼ�֮�����涯���Ļص�����
	inline void begin2DDraw()
	{
		if(XEE::windowData.windowType == WINDOW_TYPE_3D &&
			!needReset3DParam)
		{//���ԭ��Ϊ3D���ڲ���Ҫ��ô��
			needReset3DParam = XTrue;
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glDisable(GL_LIGHTING);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_DEPTH);
			glDisable(GL_DEPTH_TEST);
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0,XEE::windowData.w,XEE::windowData.h,0,-1,1);
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			glDisable(GL_CULL_FACE);
		}
	}
	inline void end2DDraw()
	{
		if(needReset3DParam)
		{//�ָ�3D����һЩ����
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glEnable(GL_CULL_FACE);
			//glEnable(GL_LIGHTING);
			//glEnable(GL_ALPHA_TEST);
			//glEnable(GL_DEPTH);
			//glEnable(GL_DEPTH_TEST);
			glPopAttrib();
			needReset3DParam = XFalse;
		}
	}
	inline void clearScreen()		//�����Ļ������
	{
		if(XEE::windowData.windowType == WINDOW_TYPE_3D) 
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
		else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
	}
	inline void clearScreen(const _XFColor & c)
	{
		glClearColor(c.fR,c.fG,c.fB,c.fA);			//�����ɫ
		if(XEE::windowData.windowType == WINDOW_TYPE_3D) 
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
		else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
		glClearColor(XEE::defaultBGColor.fR,XEE::defaultBGColor.fG,XEE::defaultBGColor.fB,XEE::defaultBGColor.fA);			//�����ɫ
	}
	extern _XBool engineInit();	//���ڽ���֮ǰ�ĳ�ʼ��
	inline void engineSysInit()	//���ڽ���֮��ĳ�ʼ��
	{
		setSystemFont(XEE::windowData.systemFontFile.c_str());
		if(XEE::windowData.withCustomTitle) XEE::customWinTitle.init();
#if WITH_OBJECT_MANAGER
		_XObjManger.init();
#endif
		_XErrorReporter::GetInstance().init();
	}
	inline void engineDraw()
	{
		if(XEE::haveSystemFont && XEE::windowData.isShowVersion
			&& XEE::showVersionTimer < 60000) XEE::showVersionStr.draw();
		if(XEE::windowData.withCustomTitle) XEE::customWinTitle.draw();
		_XCtrlManger.draw();
#if WITH_OBJECT_MANAGER
		_XObjManger.draw();
#endif
		if(drawAfterCtrl != NULL) drawAfterCtrl();
		//2014.4.14,�޸�Ϊ��ʾ�Լ��Ĺ��
#ifdef WITH_XEE_CURSOR
		if(XEE::windowData.isShowCursor != 0)
		{//������ʾ���
			drawLine(XEE::mousePosition.x - 10.0f,XEE::mousePosition.y,XEE::mousePosition.x + 10.0f,XEE::mousePosition.y,3,1.0f,1.0f,1.0f);
			drawLine(XEE::mousePosition.x,XEE::mousePosition.y - 10.0f,XEE::mousePosition.x,XEE::mousePosition.y + 10.0f,3,1.0f,1.0f,1.0f);
			drawLine(XEE::mousePosition.x - 10.0f,XEE::mousePosition.y,XEE::mousePosition.x + 10.0f,XEE::mousePosition.y,1,0.0f,0.0f,0.0f);
			drawLine(XEE::mousePosition.x,XEE::mousePosition.y - 10.0f,XEE::mousePosition.x,XEE::mousePosition.y + 10.0f,1,0.0f,0.0f,0.0f);
		}
#endif
	}
	inline void updateScreen()				//������Ļ������
	{
		engineDraw();
		end2DDraw();
		_XWindow.update();
#ifdef GET_ALL_PIXEL
		static int maxPixel = 0;
		if(XEE_AllPixel > maxPixel)
		{
			maxPixel = XEE_AllPixel;
			printf("All pixel is %d\n",XEE_AllPixel);
		}
#endif
	}
	inline void simulateFullScreen()
	{
		HWND hWnd = FindWindow(NULL,XEE::windowData.windowTitle.c_str());
		int ScreenX = GetSystemMetrics(SM_CXSCREEN); //���ȫ���Ŀ�
		int ScreenY = GetSystemMetrics(SM_CYSCREEN); //���ȫ���ĸ�
		int DialogX = GetSystemMetrics(SM_CXDLGFRAME) - 3; //�����Ĵ�����߿հױ߿�Ŀ��	//-3���ֵ����ͬ��ϵͳ�᲻ͬ
		int DialogY = GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);  //�����Ĵ���
		WINDOWPLACEMENT newment;
		newment.length = sizeof(WINDOWPLACEMENT); 
		newment.flags = WPF_RESTORETOMAXIMIZED; 
		newment.showCmd = SW_SHOWMAXIMIZED; 

		newment.ptMaxPosition.x = 0;
		newment.ptMaxPosition.y = 0;
		newment.ptMinPosition.x = 0;
		newment.ptMinPosition.y = 0;
		newment.rcNormalPosition.left = - DialogX;      //(0,0)���غ�
		newment.rcNormalPosition.top = - DialogY;
		newment.rcNormalPosition.bottom = ScreenX - DialogY; //�ߴ�һ��
		newment.rcNormalPosition.right = ScreenY - DialogX;
		SetWindowPlacement(hWnd,&newment); 
	}
}
//���Ǵ�������Ƶ�ǰ��������������Ѿ���_XErrorReporter���滻����ϸ�鿴_XErrorReporter�Ĵ����˵��
extern void suspend(const char * str);	//�жϲ���ʾ
#endif
