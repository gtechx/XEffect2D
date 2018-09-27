#include "XStdHead.h"
#include "XException.h"
#include "X3D\X3DWorld.h"
#include "XCheatModule.h"
#include "XMath\XPressDataMode.h"
#include "XControl\XSoftBoard.h"
#include "XConfigManager.h"
#include "XGameBasic.h"
#include "time.h"
#include "XWindowTitle.h"
#include "XMedia/XSoundCommon.h"
#include "XMedia/XSound.h"
#include "XImm.h"
#include <wincon.h>
#include "XWindowTitle.h"
#include "XSavePNG.h"
#include "XTimer.h"
#include "XFile.h"
#include "XMedia\XMusic.h"
#include "XMedia\XSound.h"
#include "XConfigManager.h"
#include "X3D\X3DWorld.h"
#include "XFont\XFontTTF.h"
#include "XMedia\XSoundCommon.h"
#include "XWindowCommon.h"
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceManager.h"
#include "XControl\XControlManager.h"
#include "XMedia\XProjectionCorrection.h"
#include "XMedia\XMaskCorrection.h"

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XCaregiver.h"

#if BACKGROUND_ALPHA_MODE == 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")
#endif
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
namespace XE{
XFrameWorkData gFrameworkData;
#if WITH_LOG
bool XFrameWorkData::create(bool isIniLog,XLogBookMode mode,const std::string& serverFilename)
#else
bool XFrameWorkData::create()
#endif
{
#if WITH_LOG
	pLog = XMem::createMem<XLogbook>();
	if (pLog == NULL) return false;
	if (isIniLog)
	{
		pLog->setWorkMode(mode);
		pLog->setServerFilename(serverFilename);
	}
#endif
	pFrameWork = XMem::createMem<XFrameWork>();
//	printf("%d\n", sizeof(XFrameWork));
	if (pFrameWork == NULL) return false;
#if WITH_OBJECT_MANAGER
	pObjManager = XMem::createMem<XObjectManager>();
	if (pObjManager == NULL) return false;
#endif
	pCtrlManager = XMem::createMem<XControlManager>();
	if (pCtrlManager == NULL) return false;
	pResManager = XMem::createMem<XResourceManager>();
	if (pResManager == NULL) return false;
	pResPack = XMem::createMem<XResourcePack>();
	if (pResPack == NULL) return false;
	pTexManager = XMem::createMem<XTextureResourceManager>();
	if (pTexManager == NULL) return false;
	pErrReporter = XMem::createMem<XErrorReporter>();
	if (pErrReporter == NULL) return false;
	pMusicPlayer = XMem::createMem<XMusic>();
	if (pMusicPlayer == NULL) return false;
	pSoundPlayer = XMem::createMem<XSound>();
	if (pSoundPlayer == NULL) return false;
	pCfgManager = XMem::createMem<XConfigManager>();
	if (pCfgManager == NULL) return false;
	p3DWorld = XMem::createMem<X3DWorld>();
	if (p3DWorld == NULL) return false;
	pFontTTF = XMem::createMem<XFontTTF>();
	if (pFontTTF == NULL) return false;
	pOperateManager = XMem::createMem<XOperateManager>();
	if (pOperateManager == NULL) return false;
#if AUDIO_MATHOD == 0
	pSoundCore = XMem::createMem<XSoundSDLMixer>();
	if (pSoundCore == NULL) return false;
#endif
#if AUDIO_MATHOD == 1
	pSoundCore = XMem::createMem<XSoundFmod>();
	if (pSoundCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 0
	pWindowCore = XMem::createMem<XWindowSDL>();
	if (pWindowCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 1
	pWindowCore = XMem::createMem<XWindowGlut>();
	if (pWindowCore == NULL) return false;
#endif
#if CREATE_WINDOW_METHOD == 2
	pWindowCore = XMem::createMem<XWindowGlfw>();
	if (pWindowCore == NULL) return false;
#endif
	if (pLicense != NULL && !pLicense->init()) return false;

	return true;
}
void XFrameWorkData::release()
{
	XMem::XDELETE(pErrReporter);
	XMem::XDELETE(pMusicPlayer);
	XMem::XDELETE(pSoundPlayer);
	XMem::XDELETE(p3DWorld);
	XMem::XDELETE(pTexManager);
	XMem::XDELETE(pFontTTF);
	XMem::XDELETE(pOperateManager);
	XMem::XDELETE(pCfgManager);
	XMem::XDELETE(pFrameWork);
#if WITH_OBJECT_MANAGER
	XMem::XDELETE(pObjManager);
#endif
	XMem::XDELETE(pCtrlManager);
	XMem::XDELETE(pResManager);
	XMem::XDELETE(pResPack);
	XMem::XDELETE(pSoundCore);
	XMem::XDELETE(pWindowCore);
#if WITH_LOG
	XMem::XDELETE(pLog);
#endif
}
XFrameWork::XFrameWork()
	:m_isInited(false)
	, m_rightBtnDownTimer(5000.0f)
	, m_withVirtualMouse(false)
	, m_isPassKeyLicense(true)

	, m_pGame(NULL)
	, m_needReset3DParam(XFalse)
	, m_drawAfterCtrl(NULL)
	, m_hwnd(0)
	, m_wHDC(0)
	, m_wCurHGLRC(0)
	, m_wCopyHGLRC(0)
	, m_forceToPaint(false)
	, m_isGameExit(false)
	, m_windowWidth(0)		//��ǰ���ڵĿ��
	, m_windowHeight(0)	//��ǰ���ڵĸ߶�
	, m_scenePosX(0)			//������λ��	//Ŀǰ��������в��������ںܶ�ģʽ�²�û��Ч��
	, m_scenePosY(0)			//������λ��
	, m_sceneWidth(0)		//�����Ŀ��
	, m_sceneHeight(0)		//�����ĸ߶�
	, m_sceneScaleX(1.0f)		//���ڵ����ű���
	, m_sceneScaleY(1.0f)		//���ڵ����ű���
	, m_viewAngle3D(45.0f)		//3D�۲�ĽǶ�
	, m_programPropName("xiajia_1981@163.com")
	, m_programPropValue(HANDLE(0x5A))
	, m_isSuspend(XFalse)
	, m_isFrameTimerEscape(true)
	, m_frameTime(0.0f)
	, m_curFPS(0.0f)
	, m_maxFrameStep(-1.0f)
	, m_rendDelay(33)
	, m_stopFlag(false)
	, m_manualFPS(-1)
	, m_manualFrameTime(0.0f)
	, m_defResPos(RES_LOCAL_FOLDER)
	, m_defBGColor(0.25f, 1.0f)
	, m_mainThreadHandle(0xffffffff)

	, m_isOpenGL2Supported(XFalse)	//OpenGL�Ƿ�֧��2.0�Ƿ�֧��GLSL
	, m_isLineSmooth(XTrue)		//�Ƿ�ʹ�û��ߵ�ƽ������
	, m_maxTextureSize(1024)		//�Կ�֧�ֵ������ͼ�ߴ�
	, m_isMultiSampleSupported(XFalse)	//�Ƿ�֧�ֶ��ز���
	, m_maxTextureSum(0)	//һ����ͬʱʹ�õ���ͼ������
	, m_isFboSupported(XFalse)	//�Ƿ�֧��FBO
	, m_multiSampleSum(0)

	, m_audioSampleRate(44100)	//��Ƶ��ʼ����Ƶ��
#if AUDIO_MATHOD == 0
	, m_audioFormat(MIX_DEFAULT_FORMAT)		//��Ƶ�ĸ�ʽ
#endif
#if AUDIO_MATHOD == 1
	, m_audioFormat(0)		//��Ƶ�ĸ�ʽ
#endif
	, m_audioChannel(2)	//��Ƶ������

	, m_deamonWithConsole(true)
	, m_isProjectAsDaemon(false)
	, m_customWinProc(NULL)

	, m_autoShutDownState(-1)
	, m_showVersionTimer(0.0f)
	, m_isSetToFullScreen(false)
	, m_haveSystemFont(XFalse)
	, m_curframeCounter(0)
#if WITH_ENGINE_STATISTICS
	, m_withEngineStatistics(false)	//�Ƿ������漶���ͳ��
	, m_totalTextureSum(0)			//�ܵ���ͼ����
	, m_totalTextureArea(0)		//�ܵ���ͼ���
	, m_totalVideoMemUsage(0)		//�ܵ��Դ�ʹ����
	, m_totalFrameTextureArea(0)	//ÿ֡ˢ����ͼ�����(��δʵ��)
	, m_curVideoMemBandwidthUsage(0.0f)	//��ǰ�ڴ�����ʹ��(��δʵ��)
#endif
#if BACKGROUND_ALPHA_MODE == 1
	, m_isBGAlphaInited(false)
	, m_pBits(NULL)
	, m_hMemDc(NULL)
	, m_oldGDIObj(NULL)
	, m_bgAlphaPbo(NULL)
	, m_hBitmap(NULL)
#endif
	, m_isMouseLBDown(false)
	, m_isMouseRBDown(false)

	, m_textureNeedFit2N(true)
	, m_errorExitTimer(0.0f)
	, m_softBoard(NULL)
	, m_isImmState(false)
	, m_projCorrection(nullptr)
	, m_maskCorrection(nullptr)
	, m_corStage(CSTAGE_NULL)
{
#if BACKGROUND_ALPHA_MODE == 1
	m_ptSrc.x = 0; m_ptSrc.y = 0;
#endif
	m_softBoard = XMem::createMem<XSoftBoard>();
	if (m_softBoard == NULL) LogStr("Mem Error!");
	m_moveX = XMem::createMem<XPressDataDouble>();
	if (m_moveX == NULL) LogStr("Mem Error!");
	m_moveY = XMem::createMem<XPressDataDouble>();
	if (m_moveY == NULL) LogStr("Mem Error!");
	m_cheatModule = XMem::createMem<XCheatModule>();
	if (m_cheatModule == NULL) LogStr("Mem Error!");
//	m_pConfigMng = XMem::createMem<XConfigManager>();
//	if(m_pConfigMng == NULL) LogStr("Mem Error!");
	m_customWinTitle = XMem::createMem<XWindowTitle>();
	if (m_customWinTitle == NULL) LogStr("Mem Error!");
	m_showVersionFont = XMem::createMem<XFontUnicode>();
	if (m_showVersionFont == NULL) LogStr("Mem Error!");
	m_systemFont = XMem::createMem<XFontUnicode>();
	if (m_systemFont == NULL) LogStr("Mem Error!");
	m_periodicLogTimer = XMem::createMem<XSimpleTimer>();
	if (m_periodicLogTimer == NULL) LogStr("Mem Error!");
	m_pCaregiver = XMem::createMem<XCaregiver>();
	if (m_pCaregiver == NULL) LogStr("Mem Error!");
#if LIMITED_VERSION == 1
	m_limitedTimer = XRand::randomf(7200.0f, 4800.0f);
#endif
}
XFrameWork::~XFrameWork()
{
	XMem::XDELETE(m_softBoard);
	XMem::XDELETE(m_moveX);
	XMem::XDELETE(m_moveY);
	XMem::XDELETE(m_cheatModule);
//	XMem::XDELETE(m_pConfigMng);
	XMem::XDELETE(m_customWinTitle);
	XMem::XDELETE(m_showVersionFont);
	XMem::XDELETE(m_systemFont);
	XMem::XDELETE(m_periodicLogTimer);
	XMem::XDELETE(m_pCaregiver);
}
void XFrameWork::engineRelease()
{
	XSoundPlayer.release();
	XCurSndCore.close();
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	UnregisterTouchWindow(m_hwnd);
#endif
	XWindow.release();
#if BACKGROUND_ALPHA_MODE == 1
	releaseBhAlphaData();
#endif
}
void XFrameWork::engineKeyProc(int key, XKeyState keyState)
{
	if(m_windowData.withCheatModule)
		m_cheatModule->keyboardProc(key, keyState);
	if (m_withVirtualMouse)
	{//�����������
		switch (key)
		{
		case XKEY_A:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			break;
		case XKEY_S:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
			break;
		case XKEY_D:
			if (keyState == KEY_STATE_DOWN) mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			else mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			break;
		}
	}

	XCtrlManager.keyProc(key, keyState);
#if WITH_OBJECT_MANAGER
	XObjManager.keyProc(key, keyState);
#endif
}
XVec2 XFrameWork::getMousePosFromWindow(int mouseX, int mouseY)	//ϵͳ����ռ��򴰿�����ռ����ת��
{
	float x = 0, y = 0;
	switch (m_windowData.rotateMode)
	{
	case WINDOW_ROTATE_MODE_0:
		switch (m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_90:
		switch (m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_180:
		switch (m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseY) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = (m_windowData.w - mouseX) * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = mouseX * m_sceneScaleX + m_scenePosX;
			y = mouseY * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	case WINDOW_ROTATE_MODE_270:
		switch (m_windowData.turnOverMode)
		{
		case WINDOW_TURNOVER_MODE_NULL:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = (m_windowData.h - mouseX) * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_TTD:
			x = mouseY * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		case WINDOW_TURNOVER_MODE_LTR_TTD:
			x = (m_windowData.w - mouseY) * m_sceneScaleX + m_scenePosX;
			y = mouseX * m_sceneScaleY + m_scenePosY;
			break;
		}
		break;
	}
	return XVec2(x, y);
}
void XFrameWork::engineMouseProc(int x, int y, XMouseState mouseState)
{
	if (m_windowData.withSoftBoard && mouseState == MOUSE_RIGHT_BUTTON_DOWN)
	{
		if (m_rightBtnDownTimer < 300.0f)
		{//��Ч
			if (m_softBoard->getVisible()) m_softBoard->disVisible();
			else m_softBoard->setVisible();
		}
		else
		{
			m_rightBtnDownTimer = 0.0f;
		}
	}
	m_upMousePosition = m_mousePosition;
	if (m_windowData.isFullScreen && m_windowData.isTouch)
		m_mousePosition = getMousePosFromWindow(XEE_SDL_getMouseX(), XEE_SDL_getMouseY());
	else
		m_mousePosition = getMousePosFromWindow(x, y);
	XCtrlManager.mouseProc(getMousePos(), mouseState);
#if WITH_OBJECT_MANAGER
	XObjManager.mouseProc(getMousePos(), mouseState);
#endif
	if (m_windowData.withCustomTitle) m_customWinTitle->mouseProc(getMousePos(), mouseState);
}
bool XFrameWork::cmdProc(void *pClass, const char *cmd, std::string &ret)
{
	XFrameWork &pPar = *(XFrameWork *)pClass;
	if (strcmp(cmd, "-?") == 0 || strcmp(cmd, "-help") == 0)
	{//������ʾ�����������Ŀ��˵��
		ret = "-windowWidth ��ȡ���ڿ��\n";
		ret += "-windowHeight ��ȡ���ڸ߶�\n";
		ret += "-windowSize ��ȡ���ڳߴ�\n";
		ret += "-fps ��ǰ֡��\n";
		ret += "-switchFullScreen ȫ���봰�ڽ����л�\n";
		ret += "-virtualMouse ������꿪��\n";
		ret += "-setRegRun ���ÿ�������\n";
		ret += "-disRegRun ȡ����������\n";
		ret += "-softBoard ����̿���\n";
		ret += "-exit �����˳�\n";
		ret += "-gameInfo ��ȡ������Ϣ\n";
		ret += "-getCode ��ȡ��֤��\n";
		ret += "-setKey ���ü�����\n";
		ret += "-check ��ȡ����״̬\n";
		return true;
	}
	if (strcmp(cmd, "-getCode") == 0)
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "��֧�ָù���";
		else
		{
			gFrameworkData.pLicense->createStr();
			ret = gFrameworkData.pLicense->getCodeStr();
			gFrameworkData.pLicense->saveCode();
			//	printf("%s\n",gFrameworkData.pLicense->createKey().c_str());
			//	if(gFrameworkData.pLicense->certification()) printf("��֤�ɹ�\n");
			//	else printf("��֤�ɹ�\n");
		}
		return true;
	}
	if (strcmp(cmd, "-check") == 0)
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "��֧�ָù���";
		else
		{
			if (gFrameworkData.pLicense->certification()) ret = "��֤�ɹ�";
			else ret = "��֤ʧ��";
		}
		return true;
	}
	if (cmd[0] == '-' && cmd[1] == 's' && cmd[2] == 'e' && cmd[3] == 't' &&
		cmd[4] == 'K' && cmd[5] == 'e' && cmd[6] == 'y' && cmd[7] == ' ')
	{
		if (gFrameworkData.pLicense == NULL)
			ret = "��֧�ָù���";
		else
		{
			gFrameworkData.pLicense->setKey(cmd + 8);
			//	printf("%s\n",gFrameworkData.pLicense->getKeyStr().c_str());
			gFrameworkData.pLicense->saveKey();
		}
		return true;
	}
	if (strcmp(cmd, "-windowWidth") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth);
		return true;
	}
	if (strcmp(cmd, "-exit") == 0)
	{
		ret = "�����˳�";
		if (gFrameworkData.pFrameWork != NULL)
			gFrameworkData.pFrameWork->setGameExit();
		return true;
	}
	if (strcmp(cmd, "-gameInfo") == 0)
	{
		ret = "������Ϣ:\n";
		ret += "����ʱ�䣺" + XTime::sysTime2String(XTime::getTimeMs());
		ret += "ִ���ļ���" + XFile::getCurrentExeFileFullPath() + "\n";
		ret += "����ʱ�䣺" + std::string(__DATE__) + " " + std::string(__TIME__) + "\n";
		if (gFrameworkData.pFrameWork != NULL)
		{
			ret += "��Ŀ���ƣ�" + gFrameworkData.pFrameWork->m_windowData.windowTitle + "\n";
			ret += "�汾�ţ�" + gFrameworkData.pFrameWork->m_windowData.versionString + "\n";
		}
		return true;
	}
	if (strcmp(cmd, "-windowHeight") == 0)
	{
		ret = XString::toString(pPar.m_windowHeight);
		return true;
	}
	if (strcmp(cmd, "-windowSize") == 0)
	{
		ret = XString::toString(pPar.m_windowWidth) + "," + XString::toString(pPar.m_windowHeight);
		return true;
	}
	if (strcmp(cmd, "-fps") == 0)
	{
		ret = XString::toString(pPar.m_curFPS);
		return true;
	}
	if (strcmp(cmd, "-switchFullScreen") == 0)
	{
		pPar.toggleFullScreen();
		if (pPar.m_isSetToFullScreen)
			ret = "����Ϊȫ��";
		else
			ret = "����Ϊ����";
		return true;
	}
	if (strcmp(cmd, "-virtualMouse") == 0)
	{
		pPar.m_withVirtualMouse = !pPar.m_withVirtualMouse;
		if (pPar.m_withVirtualMouse)
		{
			ret = "�����������";
			//	pPar.m_moveX.setCurData(getMousePos().x);
			//	pPar.m_moveY.move(getMousePos().y);
		}
		else ret = "�ر��������";
		return true;
	}
	if (strcmp(cmd, "-setRegRun") == 0)
	{
		if (XEE::setRegRun(true)) ret = "���ÿ��������ɹ�";
		else ret = "���ÿ�������ʧ��,��Ҫ����ԱȨ��.";
		return true;
	}
	if (strcmp(cmd, "-disRegRun") == 0)
	{
		if (XEE::setRegRun(false)) ret = "ȡ�����������ɹ�";
		else ret = "ȡ����������ʧ��,��Ҫ����ԱȨ��.";
		return true;
	}
	if (strcmp(cmd, "-softBoard") == 0 && pPar.m_windowData.withSoftBoard)
	{
		if (pPar.m_softBoard->getVisible())
		{
			ret = "���������";
			pPar.m_softBoard->disVisible();
		}
		else
		{
			ret = "��ʾ�����";
			pPar.m_softBoard->setVisible();
		}
		return true;
	}
	return false;
}
//bool XFrameWork::drawFont(float x,float y,const char *str)
//{
//	if(m_systemFont == NULL) return false;
//	m_systemFont->setString(str);
//	m_systemFont->setPosition(x,y);
//	m_systemFont->draw();
//	return true;
//}
bool XFrameWork::drawFont(const XVec2& pos, const char *str, const XFColor&c)
{
	if (m_systemFont == NULL) return false;
	XFColor tmp = m_systemFont->getColor();
	m_systemFont->setColor(c);
	m_systemFont->setString(str);
	m_systemFont->setPosition(pos);
	m_systemFont->draw();
	m_systemFont->setColor(tmp);
	return true;
}
bool XFrameWork::drawFont(const XVec2& pos, const char *str)
{
	if (m_systemFont == NULL) return false;
	m_systemFont->setString(str);
	m_systemFont->setPosition(pos);
	m_systemFont->draw();
	return true;
}
bool XFrameWork::initCommon()
{
	if (m_isInited) return false;
	if (m_windowData.withCheatModule)
	{
		if (!m_cheatModule->init()) return false;
		m_cheatModule->addCmdProc(cmdProc, this);
	}
	if (m_windowData.withSoftBoard)
	{
		if (!m_softBoard->initWithoutSkin()) return false;
		m_softBoard->disVisible();
	}

	m_moveX->init(0.0f, m_windowWidth, 0.0f, 500, MODE_NO_RECOVER);
	m_moveX->setKeyValue(XKEY_LEFT, XKEY_RIGHT);
	m_moveX->setParam(1.0f, 0.1f, 1.0f);
	m_moveY->init(0.0f, m_windowHeight, 0.0f, 500, MODE_NO_RECOVER);
	m_moveY->setKeyValue(XKEY_UP, XKEY_DOWN);
	m_moveY->setParam(1.0f, 0.1f, 1.0f);

	if (gFrameworkData.pLicense != NULL && !gFrameworkData.pLicense->certification())
	{
		m_isPassKeyLicense = false;
		LogStr("��֤ʧ��");
	}

	m_isInited = true;
	return true;
}
bool XFrameWork::initCommonAfterSet()
{
	if (m_windowData.withProjectCorrection)
	{
		m_projCorrection = XMem::createMem<XProjectionCorrection>();
		if (m_projCorrection == nullptr)
		{
			LogStr("ͶӰ������ʼ��ʧ��!");
			m_windowData.withProjectCorrection = false;
			return false;
		}
		m_projCorrection->setConfigFilename("projectCFG.xml");
		m_projCorrection->init();
		m_projCorrection->setEnableInput(false);
	}
	if (m_windowData.withMaskCorrection)
	{
		m_maskCorrection = XMem::createMem<XMaskCorrection>();
		if (m_maskCorrection == nullptr)
		{
			LogStr("ͶӰ������ʼ��ʧ��!");
			m_windowData.withMaskCorrection = false;
			return false;
		}
		m_maskCorrection->init(m_windowData.maskFilename.c_str(), "maskCFG.xml");
		m_maskCorrection->setEnableInput(false);
	}
	return true;
}
void XFrameWork::updateCommon(float stepTime)
{
	if (m_rightBtnDownTimer < 1000.0f) m_rightBtnDownTimer += stepTime;
	if (m_withVirtualMouse)
	{//������������λ��
		m_moveX->move(stepTime);
		m_moveY->move(stepTime);
		//printf("%f,%f\n",m_moveX.getCurData(),m_moveY.getCurData());
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, m_moveX->getCurData() * 65536.0f / m_windowWidth,
			m_moveY->getCurData() * 65536.0f / m_windowHeight, 0, 0);
	}
}
bool XFrameWork::getIsLBDrag(const XInputEvent &event)const
{
	return (event.type == EVENT_MOUSE && event.mouseState == MOUSE_MOVE && 
		getIsMouseLBDown());
}
bool XFrameWork::getIsRBDrag(const XInputEvent &event)const
{
	return (event.type == EVENT_MOUSE && event.mouseState == MOUSE_MOVE &&
		getIsMouseRBDown());
}
bool XFrameWork::singletonProc()
{
	return m_windowData.isSingletonMode &&
		isProgramExist(XFile::getCurrentExeFileFullPath());
}
void XFrameWork::errorLoop()
{
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//�����Ҫ�ϸߵķ�ӦЧ�ʣ������������
	float stepTime = engineIdle();
	//TODO:Update
	m_errorExitTimer += stepTime * 0.001f;
	if (m_systemFont != NULL && m_systemFont->getIsInited())
	{
		char tmpStr[512];
		m_systemFont->setPosition(getSceneCenter());
		m_systemFont->setColor(XFColor::red);
		m_systemFont->setScale(1.5f);
		if (!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash)
		{
			sprintf_s(tmpStr, 512, "�������쳣\n����%.0f���[��������]", 10.0f - m_errorExitTimer);
		}
		else
		{
			sprintf_s(tmpStr, 512, "�������쳣\n����%.0f���[�˳�]", 10.0f - m_errorExitTimer);
		}
		m_systemFont->setString(tmpStr);
	}
	if (m_errorExitTimer > 10.0f)
	{
#if WITH_LOG
		LogRelease();	//������־�ļ���������
#endif		
		if (!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//������������
		XEG.setGameExit();				//��ֹ�����˳�ʱ�ٴβ����쳣
		exit(1);
	}

	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withClearBG) clearScreen();			//�����Ļ,���ÿ�ζ�ȫ��ˢ�µĻ����Ż��������һ��Ч��
	//TODO:Draw
	XRender::drawFillRectExA(getSceneCenter() - XVec2(250, 80), XVec2(500, 160), XFColor::yellow, true, true);
	if (m_systemFont != NULL && m_systemFont->getIsInited())
		m_systemFont->draw();
	updateScreen(false);			//������Ļ������
}
void XFrameWork::mainLoop2D()
{
	XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//�����Ҫ�ϸߵķ�ӦЧ�ʣ������������
	float stepTime = engineIdle();
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->update();
	if (m_isPassKeyLicense)
	{
#if LIMITED_VERSION == 1
		if (m_limitedTimer >= 0.0f)
		{//��ʱ�汾ֻ������1Сʱ
			m_limitedTimer -= stepTime * 0.001f;
			tmpGame.move(stepTime);
		}
#else
		tmpGame.move(stepTime);
#endif
	}
	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withProjectCorrection || m_windowData.withMaskCorrection)
	{
		if (m_windowData.withProjectCorrection && m_windowData.withMaskCorrection)
		{
			m_projCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//�����Ļ,���ÿ�ζ�ȫ��ˢ�µĻ����Ż��������һ��Ч��
			tmpGame.draw();
			if (m_projCorrection->getIsShowCtrl())
			{//����ǵ���ͶӰ�����Ļ�����Щ��ʾ����
				int size = XMath::getGCM(XEG.getSceneW(), XEG.getSceneH()) >> 1;
				int w = XEG.getSceneW() / size;
				int h = XEG.getSceneH() / size;
				for (int i = 0, index = 0; i <= w; ++i, index += size)
				{
					XRender::drawLine(XVec2(index, 0), XVec2(index, XEG.getSceneH()), 2.0f, XFColor::blue);
				}
				for (int i = 0, index = 0; i <= h; ++i, index += size)
				{
					XRender::drawLine(XVec2(0, index), XVec2(XEG.getSceneW(), index), 2.0f, XFColor::blue);
				}
			}
			m_projCorrection->end();

			m_maskCorrection->begin();
			m_projCorrection->draw();
			m_maskCorrection->end();
			m_maskCorrection->draw();
		}else
		if (m_windowData.withProjectCorrection)
		{
			m_projCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//�����Ļ,���ÿ�ζ�ȫ��ˢ�µĻ����Ż��������һ��Ч��
			tmpGame.draw();
			if (m_projCorrection->getIsShowCtrl())
			{//����ǵ���ͶӰ�����Ļ�����Щ��ʾ����
				int size = XMath::getGCM(XEG.getSceneW(), XEG.getSceneH()) >> 1;
				int w = XEG.getSceneW() / size;
				int h = XEG.getSceneH() / size;
				for (int i = 0, index = 0; i <= w; ++i, index += size)
				{
					XRender::drawLine(XVec2(index, 0), XVec2(index, XEG.getSceneH()), 2.0f, XFColor::blue);
				}
				for (int i = 0, index = 0; i <= h; ++i, index += size)
				{
					XRender::drawLine(XVec2(0, index), XVec2(XEG.getSceneW(), index), 2.0f, XFColor::blue);
				}
			}
			m_projCorrection->end();
			m_projCorrection->draw();
		}
		else
		{
			m_maskCorrection->begin();
			if (m_windowData.withClearBG) clearScreen();			//�����Ļ,���ÿ�ζ�ȫ��ˢ�µĻ����Ż��������һ��Ч��
			tmpGame.draw();
			m_maskCorrection->end();
			m_maskCorrection->draw();
		}
	}
	else
	{
		if (m_windowData.withClearBG) clearScreen();			//�����Ļ,���ÿ�ζ�ȫ��ˢ�µĻ����Ż��������һ��Ч��
		tmpGame.draw();
	}
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->draw();
	updateScreen();			//������Ļ������
}
void XFrameWork::mainLoop3D()
{
	XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
	if (m_windowData.withLowConsumption) XEE::sleep(1);	//�����Ҫ�ϸߵķ�ӦЧ�ʣ������������
	float stepTime = engineIdle();
	if (gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->move(stepTime);
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->update();
	if (m_isPassKeyLicense)
	{
#if LIMITED_VERSION == 1
		if (m_limitedTimer >= 0.0f)
		{//��ʱ�汾ֻ������1Сʱ
			m_limitedTimer -= stepTime * 0.001f;
			tmpGame.move(stepTime);
		}
#else
		tmpGame.move(stepTime);
#endif
	}
	updateCommon(stepTime);
	if (!getWindowNeedDraw() ||
		!m_drawTimer.update(stepTime)) return;
	if (m_windowData.withClearBG) clearScreen();			//�����Ļ���Ż��������һ��Ч��
	//TODO:���������Ҫ����3D����
	if (gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->draw();
	begin2DDraw();

	tmpGame.draw();
	if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
		gFrameworkData.pCfgManager->draw();
	updateScreen();			//������Ļ������
}
void XFrameWork::miniInputEvent(const XInputEvent &tmpEvent)
{
	switch (tmpEvent.type)
	{
	case EVENT_KEYBOARD:
		if (XImm::getCompositionState()) m_isImmState = true;
		if (!m_isImmState) engineKeyProc(tmpEvent.keyValue, tmpEvent.keyState);
		if (tmpEvent.keyState == KEY_STATE_UP && !XImm::getCompositionState())
			m_isImmState = false;
		break;
	case EVENT_MOUSE:
		engineMouseProc(tmpEvent.mouseX, tmpEvent.mouseY, tmpEvent.mouseState);
		break;
	}
}
//�����Ƿ��˳�
bool XFrameWork::inputEvent(const XInputEvent &tmpEvent, bool send2Game)
{
	switch (tmpEvent.type)
	{
	case EVENT_EXIT:return true;
	case EVENT_RESIZE:	//���ڴ�С�����仯
		m_windowData.w = tmpEvent.mouseX;
		m_windowData.h = tmpEvent.mouseY;
		initWindowMatrix();
		break;
	case EVENT_KEYBOARD:
		if (tmpEvent.keyState == KEY_STATE_DOWN)
		{
			switch (tmpEvent.keyValue)
			{
			case XKEY_ESCAPE: return true;
			case XKEY_F1:
				if (saveSystemConfigData(m_windowData)) XEG.setTopMsgStr("���������ļ��Ѿ��������!");
				else XEG.setTopMsgStr("�����ļ�����ʧ��!");
				break;
			}
		}
		if (XImm::getCompositionState()) m_isImmState = true;
		if (!m_isImmState) engineKeyProc(tmpEvent.keyValue, tmpEvent.keyState);
		if (tmpEvent.keyState == KEY_STATE_UP && !XImm::getCompositionState())
			m_isImmState = false;
		break;
	case EVENT_MOUSE:
		switch (tmpEvent.mouseState)
		{
		case MOUSE_LEFT_BUTTON_DOWN:m_isMouseLBDown = true; break;
		case MOUSE_LEFT_BUTTON_UP:m_isMouseLBDown = false; break;
		case MOUSE_RIGHT_BUTTON_DOWN:m_isMouseRBDown = true; break;
		case MOUSE_RIGHT_BUTTON_UP:m_isMouseRBDown = false; break;
		}
		engineMouseProc(tmpEvent.mouseX, tmpEvent.mouseY, tmpEvent.mouseState);
		break;
	}
	//for test
	//if (tmpEvent.isMouseLeftDown())
	//	printf("����!\n");
	if (m_withVirtualMouse)
	{
		m_moveX->updateState(tmpEvent);
		m_moveY->updateState(tmpEvent);
	}

	if (send2Game)
	{
		m_inputEventBuff.doProc(tmpEvent);
		if (XConfigManager::m_withConfigManager && gFrameworkData.pCfgManager != NULL)
			gFrameworkData.pCfgManager->inputProc(tmpEvent);
		if (m_windowData.windowType == WINDOW_TYPE_3D && gFrameworkData.p3DWorld != NULL)
			gFrameworkData.p3DWorld->keyEvent(tmpEvent);
		if (m_windowData.withProjectCorrection)
		{
			if (tmpEvent.isKeyBoardDown() && tmpEvent.keyValue == XKEY_F2)
			{
				if (m_corStage != CSTAGE_PROJECTION)
				{
					m_corStage = CSTAGE_PROJECTION;
					m_projCorrection->setIsShowCtrl(true);
					m_projCorrection->setEnableInput(true);
					if (m_maskCorrection != nullptr)
					{
						m_maskCorrection->setIsShowCtrl(false);
						m_maskCorrection->setEnableInput(false);
					}
					setTopMsgStr("ͶӰ����");
				}
				else
				{
					m_corStage = CSTAGE_NULL;
					m_projCorrection->setIsShowCtrl(false);
					m_projCorrection->setEnableInput(false);
					setTopMsgStr("��ͨģʽ");
				}
			}
			if (m_projCorrection->inputProc(tmpEvent)) return false;
		}
		if (m_windowData.withMaskCorrection)
		{
			if (tmpEvent.isKeyBoardDown() && tmpEvent.keyValue == XKEY_F3)
			{
				if (m_corStage != CSTAGE_MASK)
				{
					m_corStage = CSTAGE_MASK;
					m_maskCorrection->setIsShowCtrl(true);
					m_maskCorrection->setEnableInput(true);
					if (m_projCorrection != nullptr)
					{
						m_projCorrection->setIsShowCtrl(false);
						m_projCorrection->setEnableInput(false);
					}
					setTopMsgStr("Mask����");
				}
				else
				{
					m_corStage = CSTAGE_NULL;
					m_maskCorrection->setIsShowCtrl(false);
					m_maskCorrection->setEnableInput(false);
					setTopMsgStr("��ͨģʽ");
				}
			}
			if (m_maskCorrection->inputProc(tmpEvent)) return false;
		}
		if (m_pGame != NULL)
			((XGameBasic *)m_pGame)->input(tmpEvent);
	}
	return false;
}
#if CREATE_WINDOW_METHOD == 0
bool XFrameWork::inputEvent(bool send2Game)
{//�������¼������������磺�����¼���������¼�
	XInputEvent tmpEvent;		//SDL�¼����
	try
	{//ע�⣺����SDL�ж���Ϣ�Ĵ������bug���˴����ܻ����crash����Ҫ��һ���ĵ��ԣ����������ʱ�޲�
		while (getInputEvent(tmpEvent))
		{
			if (inputEvent(tmpEvent, send2Game)) return true;
		}
	}
	catch (...)
	{
		return false;
	}
	return m_isGameExit;
}
//void XFrameWork::startGame(void *game,const XWindowInfo* windowInfo)
//{
//	if (game == NULL) return;
//	if (windowInfo == NULL)
//	{
//		if(!readSystemConfigData(m_windowData,true)) LogStr("��ȡ�����ļ�ʧ��!");
//	}else
//	{
//		m_windowData = * windowInfo;
//	}
//	if(m_windowData.withException)
//	{
//		__try{
//			XException::initException();
//			if(!initWindow()) XEE_Error("��������ʧ��");
//			if(!singletonProc())
//				gameProc(game);
//			else
//				LogStr("�����Ѿ������������ظ�����!");
//			engineRelease();
//		}
//		__except (XException::unhandledExceptionFilter(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
//		{
//			//XException::unhandledExceptionFilter(GetExceptionInformation());
//		}
//	}else
//	{
//		if(!initWindow()) XEE_Error("��������ʧ��");
//		if(!singletonProc())
//			gameProc(game);
//		else
//			LogStr("�����Ѿ������������ظ�����!");
//		engineRelease();
//	}
//}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
//	if(m_manualFPS > 0)
//		m_fpsTimer.setStepTime(1000.0f / m_manualFPS);
	initCommon();
	if (m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if (gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("��������ʧ��");

		if (!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//������ѭ��
//			if(m_manualFPS <= 0 || m_fpsTimer.update())
				mainLoop3D();
		}
		setGameExit();
		tmpGame.release();
	}
	else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//������ѭ��
//			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop2D();
		}
		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
#if CREATE_WINDOW_METHOD == 1
/*
void XFrameWork::startGame(XGameBasic *game)
{
	if (game == NULL) return;
	pGame = game;
	__try{
		glutInit(&argc, argv);
		if (!initWindowEx()) XEE_Error("��������ʧ��");
		if (!pGame->init()) XEE_Error("��ʼ��ʧ��");

		glutDisplayFunc(draw);
		glutIdleFunc(idle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(input);
		glutSpecialFunc(sInput);
		glutMouseFunc(mouseProc);
		glutMotionFunc(mouseMove);
		glutPassiveMotionFunc(mouseMove);
		glutMainLoop();

		pGame->release();
		release();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		XEE_Error("�����쳣!");
	}
	return 0;
}*/
void XFrameWork::glutIdle()
{
	if(XEG.m_windowData.windowType == WINDOW_TYPE_3D)
		XEG.mainLoop3D();
	else 
		XEG.mainLoop2D();
}
void XFrameWork::glutDraw(){}
//void draw(){}
//void idle()
//{
//	pGame->move(engineIdle());
//	clearScreen();			//�����Ļ
//	pGame->draw();
//	updateScreen();			//������Ļ������
//	//mySleep(1);
//}
void XFrameWork::inputProc(const XInputEvent &input)
{
	//if(XEG.m_windowData.windowType == WINDOW_TYPE_3D)
	//	((XGameBasic3D *)m_pGame)->input(input);
	//else 
	//	((XGameBasic *)m_pGame)->input(input);
	if(inputEvent(input)) exit(1);
}
void XFrameWork::glutInput(unsigned char c, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(c);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
	if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void XFrameWork::glutsInput(int k, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_KEYBOARD;
	tmpEvent.keyState = KEY_STATE_DOWN;
	tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(k + 512);
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
	if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
}
void XFrameWork::glutMouseMove(int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseState = MOUSE_MOVE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	XEG.inputProc(tmpEvent);
}
void XFrameWork::glutMouseProc(int b, int s, int x, int y)
{
	XInputEvent tmpEvent;
	tmpEvent.type = EVENT_MOUSE;
	tmpEvent.mouseX = x;
	tmpEvent.mouseY = y;
	if (s == GLUT_DOWN)
	{
		switch (b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_DOWN; break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_DOWN; break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_DOWN; break;
		}
	}
	else
	{
		switch (b)
		{
		case GLUT_LEFT_BUTTON: tmpEvent.mouseState = MOUSE_LEFT_BUTTON_UP; break;
		case GLUT_RIGHT_BUTTON: tmpEvent.mouseState = MOUSE_RIGHT_BUTTON_UP; break;
		case GLUT_MIDDLE_BUTTON: tmpEvent.mouseState = MOUSE_MIDDLE_BUTTON_UP; break;
		}
	}
	XEG.inputProc(tmpEvent);
}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
	//��ʼ��glut,���ʼ���Ĳ���Ҳ̫�����˰�
	//����1����
	int ac = 0;
	char *av[1] = {NULL};
	//����2����ֹ����
	//int ac = 1;
	//char filename[MAX_FILE_NAME_LENGTH];
	//char *av[1] = {filename};
	//strcpy(filename,XFile::getCurrentExeFileFullPath().c_str());
	glutInit(&ac, av);

	initCommon();
	if(m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if(gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("��������ʧ��");
		if(!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		
		glutDisplayFunc(glutDraw);
		glutIdleFunc(glutIdle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(glutInput);
		glutSpecialFunc(glutsInput);
		glutMouseFunc(glutMouseProc);
		glutMotionFunc(glutMouseMove);
		glutPassiveMotionFunc(glutMouseMove);
		glutMainLoop();

		setGameExit();
		tmpGame.release();
	}else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		XSimpleTimer drawTimer;	//��ʾ�ļ�ʱ������ʾ̫��ʵ����û�����壬�߼��߳̿����ܺܿ죬������ʾ�߳�û�б�Ҫ�ܿ�
		m_drawTimer.setStepTime(m_rendDelay);

		glutDisplayFunc(glutDraw);
		glutIdleFunc(glutIdle);
		//glutReshapeFunc(changeSize);
		glutKeyboardFunc(glutInput);
		glutSpecialFunc(glutsInput);
		glutMouseFunc(glutMouseProc);
		glutMotionFunc(glutMouseMove);
		glutPassiveMotionFunc(glutMouseMove);
		glutMainLoop();

		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
#if CREATE_WINDOW_METHOD == 2
bool XFrameWork::inputEvent(bool send2Game)
{//�������¼������������磺�����¼���������¼�
	XInputEvent tmpEvent;		//SDL�¼����
	try
	{//ע�⣺����SDL�ж���Ϣ�Ĵ������bug���˴����ܻ����crash����Ҫ��һ���ĵ��ԣ����������ʱ�޲�
		while (getInputEvent(tmpEvent))
		{
			if (inputEvent(tmpEvent, send2Game)) return true;
		}
	}
	catch (...)
	{
		return 0;
	}
	return m_isGameExit;
}
void XFrameWork::gameProc(void *game)
{
	m_pGame = game;
	//	if(m_manualFPS > 0)
	//		m_fpsTimer.setStepTime(1000.0f / m_manualFPS);
	initCommon();
	if (m_windowData.windowType == WINDOW_TYPE_3D)
	{
		XGameBasic3D &tmpGame = *(XGameBasic3D *)m_pGame;
		if (gFrameworkData.p3DWorld == NULL || !gFrameworkData.p3DWorld->init(NULL)) XEE_Error("��������ʧ��");

		if (!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//������ѭ��
		 //			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop3D();
		}
		setGameExit();
		tmpGame.release();
	}
	else
	{
		XGameBasic &tmpGame = *(XGameBasic *)m_pGame;
		if (!tmpGame.init()) XEE_Error("��ʼ��ʧ��");
		initCommonAfterSet();
		XCtrlManager.setGame(&tmpGame);
		m_drawTimer.setStepTime(m_rendDelay);
		while (!inputEvent())
		{//������ѭ��
		 //			if(m_manualFPS <= 0 || m_fpsTimer.update())
			mainLoop2D();
		}
		setGameExit();
		tmpGame.release();
	}
	m_pGame = nullptr;
	XCtrlManager.setGame(nullptr);
}
#endif
XBool XFrameWork::readSystemConfigData(XWindowInfo &data, bool forceTitle)
{//��ȡ��������:�����޸ģ������������ڣ���ʹ��Ĭ��ֵ
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	if (!doc.LoadFile()) return XFalse;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if (rootNode == NULL) return XFalse;
	int temp;
	XXml::getXmlAsInt(rootNode, "windowType", (int &)data.windowType);
	XXml::getXmlAsInt(rootNode, "w", data.w);
	XXml::getXmlAsInt(rootNode, "h", data.h);
	XXml::getXmlAsInt(rootNode, "sceneW", data.sceneW);
	XXml::getXmlAsInt(rootNode, "sceneH", data.sceneH);
	XXml::getXmlAsInt(rootNode, "positionX", data.positionX);
	XXml::getXmlAsInt(rootNode, "positionY", data.positionY);
	XXml::getXmlAsBool(rootNode, "isFullScreen", data.isFullScreen);
	XXml::getXmlAsBool(rootNode, "isTouch", data.isTouch);
	XXml::getXmlAsInt(rootNode, "withLog", data.withLog);
	XXml::getXmlAsBool(rootNode, "isShowCursor", data.isShowCursor);
	XXml::getXmlAsBool(rootNode, "withFrame", data.withFrame);
	XXml::getXmlAsInt(rootNode, "windowMode", (int &)data.mode);
	if(!forceTitle || data.windowTitle.size() == 0)
		XXml::getXmlAsString(rootNode, "windowTitle", data.windowTitle);
	XXml::getXmlAsInt(rootNode, "rotateMode", (int &)data.rotateMode);
	XXml::getXmlAsInt(rootNode, "turnOverMode", (int &)data.turnOverMode);
	XXml::getXmlAsInt(rootNode, "CFGMode", (int &)data.CFGMode);
	XXml::getXmlAsBool(rootNode, "isStartDelay", data.isStartDelay);
	XXml::getXmlAsInt(rootNode, "startDelayTimer", data.startDelayTimer);
	XXml::getXmlAsBool(rootNode, "isShowVersion", data.isShowVersion);
	XXml::getXmlAsString(rootNode, "versionString", data.versionString);
	XXml::getXmlAsBool(rootNode, "isAutoShutDown", data.isAutoShutDown);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_year", data.autoShutDownYear);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_month", data.autoShutDownMonth);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_day", data.autoShutDownDay);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_hour", data.autoShutDownHour);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_minute", data.autoShutDownMinute);
	XXml::getXmlAsInt(rootNode, "shutDownTimer_second", data.autoShutDownSecond);
	XXml::getXmlAsBool(rootNode, "isShowConsole", data.isShowConsole);
	XXml::getXmlAsBool(rootNode, "isProjectAsDaemon", data.isProjectAsDaemon);
	XXml::getXmlAsString(rootNode, "fontFilename", data.systemFontFile);
	XXml::getXmlAsString(rootNode, "commonResource", data.commonResourcePos);
	if (XXml::getXmlAsInt(rootNode, "defaultResourcePosition", temp) != NULL)
	{
		switch (temp)
		{
		case RES_LOCAL_FOLDER:
		case RES_LOCAL_PACK:
		case RES_WEB:
		case RES_AUTO:
			data.defResourcePos = (XResPos)temp;
			break;
		}//����������ı���Դλ�õ�����
	}
	XXml::getXmlAsInt(rootNode, "backgroundValume", data.BGVolume);
	XXml::getXmlAsInt(rootNode, "formwordValume", data.FWVolume);
	XXml::getXmlAsBool(rootNode, "isInitWindowPos", data.isInitWindowPos);
	XXml::getXmlAsInt(rootNode, "windowPosX", data.windowPosX);
	XXml::getXmlAsInt(rootNode, "windowPosY", data.windowPosY);
	XXml::getXmlAsBool(rootNode, "isAlwaysTop", data.isAlwaysTop);
	XXml::getXmlAsBool(rootNode, "withCustomTitle", data.withCustomTitle);
	XXml::getXmlAsBool(rootNode, "withAlphaBG", data.withAlphaBG);
	if(data.withAlphaBG) data.withFrame = false;	//���ʹ��͸������Ļ��������б߿�
	XXml::getXmlAsInt(rootNode, "logicFps", data.logicFps);
	XXml::getXmlAsInt(rootNode, "drawFps", data.drawFps);
	XXml::getXmlAsBool(rootNode, "withException", data.withException);
	XXml::getXmlAsBool(rootNode, "withLowConsumption", data.withLowConsumption);
	XXml::getXmlAsBool(rootNode, "withClearBG", data.withClearBG);
	XXml::getXmlAsBool(rootNode, "isRestartWhenCrash", data.isRestartWhenCrash);
	XXml::getXmlAsBool(rootNode, "isSingletonMode", data.isSingletonMode);
	XXml::getXmlAsBool(rootNode, "withProjectCorrection", data.withProjectCorrection);
	XXml::getXmlAsBool(rootNode, "withMaskCorrection", data.withMaskCorrection);
	XXml::getXmlAsString(rootNode, "maskFilename", data.maskFilename);
	XXml::getXmlAsBool(rootNode, "withCheatModule", data.withCheatModule);
	XXml::getXmlAsBool(rootNode, "withSoftBoard", data.withSoftBoard);
	XXml::getXmlAsBool(rootNode, "withCaregiver", data.withCaregiver);

	if (data.withFrame && data.withCustomTitle) data.withCustomTitle = false;
	if (data.withCustomTitle && data.positionY > -MIN_FONT_CTRL_SIZE)
	{
		int tmpH = data.positionY + MIN_FONT_CTRL_SIZE;
		data.positionY = -MIN_FONT_CTRL_SIZE;
		data.sceneH += tmpH;
		data.h += tmpH;
	}

	return XTrue;
}
XBool XFrameWork::saveSystemConfigData(const XWindowInfo &data)
{//��������Ϣ���浽�����ļ�
	TiXmlDocument doc(XEE_SYSTEM_CONFIG);
	TiXmlDeclaration declaration("1.0", "gb2312", "yes");
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	if (!XXml::addComment(elmRoot, "�������ͣ�0����ͨ���ڣ�1��������Ӧ����ת�ͷ�ת�Ĵ��ڣ�2��֧��3D��Ⱦ�Ĵ���")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowType", XString::toString(data.windowType))) return XFalse;
	if (!XXml::addComment(elmRoot, "���ڿ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "w", data.w)) return XFalse;
	if (!XXml::addComment(elmRoot, "���ڸ߶�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "h", data.h)) return XFalse;
	if (!XXml::addComment(elmRoot, "�������")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneW", data.sceneW)) return XFalse;
	if (!XXml::addComment(elmRoot, "�����߶�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "sceneH", data.sceneH)) return XFalse;
	if (!XXml::addComment(elmRoot, "������xƫ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionX", data.positionX)) return XFalse;
	if (!XXml::addComment(elmRoot, "������yƫ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "positionY", data.positionY)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�ȫ����0����ȫ����1��ȫ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isFullScreen", data.isFullScreen)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�֧�ִ�������0����֧�֣�1��֧��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isTouch", data.isTouch)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ������־��0���������1����̨�����2���ļ������3�������")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withLog", XString::toString(data.withLog))) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ���ʾ��꣺0������ʾ��1����ʾ")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowCursor", data.isShowCursor)) return XFalse;
	if (!XXml::addComment(elmRoot, "�����Ƿ��б߿�0��û�б߿�1���б߿�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withFrame", data.withFrame)) return XFalse;
	if (!XXml::addComment(elmRoot, "����ģʽ��0�����ϽǶ��벢�ü���1���ص���벢�ü���2���̱����쳤�߲ü���3����������̱���հף�4���Զ�����ȫ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowMode", XString::toString(data.mode))) return XFalse;
	if (!XXml::addComment(elmRoot, "���ڱ���")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowTitle", data.windowTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "���ڵ���תģʽ��0������ת��1��90��ת��2��180����ת��3��270��ת")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "rotateMode", XString::toString(data.rotateMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "���ڵķ�תģʽ��0������ת��1�����ҷ�ת��2�����·�ת��3���������·�ת")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "turnOverMode", XString::toString(data.turnOverMode))) return XFalse;
	if (!XXml::addComment(elmRoot, "���õĹ���ģʽ��0����ͨģʽ��1��������ģʽ��2���ͻ���ģʽ")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "CFGMode", data.CFGMode)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ��������ӳ�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isStartDelay", data.isStartDelay)) return XFalse;
	if (!XXml::addComment(elmRoot, "�����ӳٵ�ʱ�䣺��λms")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "startDelayTimer", data.startDelayTimer)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ���ʾ�汾��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowVersion", data.isShowVersion)) return XFalse;
	if (!XXml::addComment(elmRoot, "�汾��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "versionString", data.versionString)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�������ʱ�ػ�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAutoShutDown", data.isAutoShutDown)) return XFalse;
	if (!XXml::addComment(elmRoot, "��ʱ�ػ���ʱ��.С��0��ʾ������Ч")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_year", data.autoShutDownYear)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_month", data.autoShutDownMonth)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_day", data.autoShutDownDay)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_hour", data.autoShutDownHour)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_minut", data.autoShutDownMinute)) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "shutDownTimer_second", data.autoShutDownSecond)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ���ʾ��̨")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isShowConsole", data.isShowConsole)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ���Ϊ��̨����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isProjectAsDaemon", data.isProjectAsDaemon)) return XFalse;
	if (!XXml::addComment(elmRoot, "Ĭ����Դ��λ�ã�0���ļ��У�1����Դ����2��������Դ��3��ϵͳĬ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "defaultResourcePosition", XString::toString(data.defResourcePos))) return XFalse;
	if (!XXml::addComment(elmRoot, "Ĭ�ϵ������ļ�")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "fontFilename", data.systemFontFile)) return XFalse;
	if (!XXml::addComment(elmRoot, "ͨ����Դ��λ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "commonResource", data.commonResourcePos)) return XFalse;
	if (!XXml::addComment(elmRoot, "�������ֵ�����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "backgroundValume", data.BGVolume)) return XFalse;
	if (!XXml::addComment(elmRoot, "ǰ�����ֵ�����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "formwordValume", data.FWVolume)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ����ô���λ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isInitWindowPos", data.isInitWindowPos)) return XFalse;
	if (!XXml::addComment(elmRoot, "����λ��X")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosX", data.windowPosX)) return XFalse;
	if (!XXml::addComment(elmRoot, "����λ��Y")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "windowPosY", data.windowPosY)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ��ö�����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isAlwaysTop", data.isAlwaysTop)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�ʹ���Լ��ı�����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCustomTitle", data.withCustomTitle)) return XFalse;
	if (!XXml::addComment(elmRoot, "�����Ƿ�͸��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withAlphaBG", data.withAlphaBG)) return XFalse;
	if (!XXml::addComment(elmRoot, "ϵͳ����Ĵ�С")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "systemFontSize",data.systemFontSize)) return XFalse;
	if (!XXml::addComment(elmRoot, "�߼�֡��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "logicFps", data.logicFps)) return XFalse;
	if (!XXml::addComment(elmRoot, "��ʾ֡��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "drawFps", data.drawFps)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ����쳣")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withException", data.withException)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ������ͺ�ģʽ")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withLowConsumption", data.withLowConsumption)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ��������")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withClearBG", data.withClearBG)) return XFalse;
	if (!XXml::addComment(elmRoot, "������crash֮���Ƿ��Զ���������")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isRestartWhenCrash", data.isRestartWhenCrash)) return XFalse;
	if (!XXml::addComment(elmRoot, "���������Ƿ�ֻ��һ��ʵ��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "isSingletonMode", data.isSingletonMode)) return XFalse;
	if (!XXml::addComment(elmRoot, "ʹ��ͶӰ��������")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withProjectCorrection", data.withProjectCorrection)) return XFalse;
	if (!XXml::addComment(elmRoot, "ʹ�ܾ����ںڹ���")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withMaskCorrection", data.withMaskCorrection)) return XFalse;
	if (!XXml::addComment(elmRoot, "�ںڵ��ļ�������Ҫ�ĵط�Ϊ�ڣ�����Ҫ�ĵط�Ϊ��ɫ")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "maskFilename", data.maskFilename)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�ʹ���������빦��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCheatModule", data.withCheatModule)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ��ṩ������̵�֧��")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withSoftBoard", data.withSoftBoard)) return XFalse;
	if (!XXml::addComment(elmRoot, "�Ƿ�ʹ�ü໤����")) return XFalse;
	if (!XXml::addLeafNode(elmRoot, "withCaregiver", data.withCaregiver)) return XFalse;

	if (doc.InsertEndChild(elmRoot) == NULL) return XFalse;
	doc.SaveFile();
	return XTrue;
}
XBool XFrameWork::engineInit()
{
	m_defResPos = m_windowData.defResourcePos;
#if WITH_LOG
	gFrameworkData.pLog->setLogLevel((XLogBookLevel)m_windowData.withLog);
#endif
	if (m_windowData.isStartDelay
		&& m_windowData.startDelayTimer > 0) Sleep(m_windowData.startDelayTimer);	//��������ǰ�ӳٵȴ�һ��ʱ��
	if (!m_windowData.isShowConsole)
	{
		//FreeConsole();	//�����Ҫ�����ؿ��ƺ�̨ ����win732bit�л����crash
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}
	if (m_windowData.isProjectAsDaemon) setProjectAsDaemon();
	//��ʼ��һЩ������ֵ
	m_windowWidth = m_windowData.w;
	m_windowHeight = m_windowData.h;
	m_scenePosX = m_windowData.positionX;
	m_scenePosY = m_windowData.positionY;
	m_sceneWidth = m_windowData.sceneW;
	m_sceneHeight = m_windowData.sceneH;
	m_sceneScaleX = m_sceneScaleY = 1.0f;		//���ڵ����ű���
	srand((unsigned)time(0)); //�����������
	m_periodicLogTimer->setStepTime(10000);	//10sһ������
	m_defWorkPath = XFile::getWorkPath();	//��¼����·��
	m_autoShutDownTimer.setStepTime(1000.0f);
	XEE::getCpuUsage();
	return XTrue;
}
float XFrameWork::engineIdle()
{
	//		static XTimer tempTimer;
	//		static Uint32 current,last = 0,five = 0,nframes = 0;
	//		current = tempTimer.getCurrentTime();
	//		++nframes;
	//		if(current - five > 5000) 
	//		{//��֡����ʾ����
	//			curFPS = nframes * 0.2f;
	////#ifdef DEBUG_MODE
	//			if(manualFPS > 0) printf("%.1f FPS@%d\n",curFPS,manualFPS);
	//			else printf("%.1f FPS\n",curFPS);
	////#endif
	//			nframes = 0;
	//			five = current;
	//		}
	//		frameTime = current - last;
	//		last = current;
	//�������ַ�ʽ�ڵ�֡��ʱ���ϴ󣬲�֪��Ϊʲô
	//����1��
	//static Uint32 current,last = 0;
	//if(last <= 0)
	//{
	//	last = XTime::getCurrentTicks();	//��ֹ��һ�ε��õ�ʱ�����ڳ���������ʱ�����Ķ���ɵ�һ֡�ر�
	//	XEE::getCpuUsage();
	//}
	//current = XTime::getCurrentTicks();
	//m_frameTime = current - last;
	//last = current;
	//����2��
	static XStepTimer steptimer;
	float curFrameTime = steptimer.update();
	if (m_isFrameTimerEscape)
	{
		m_frameTime = 1.0f;
		m_isFrameTimerEscape = false;
	}
	else
	{
		m_frameTime = curFrameTime;
	}
	//LogNull("%f",m_frameTime); //for test

	++m_curframeCounter;
	if (curFrameTime > m_maxFrameStep) m_maxFrameStep = curFrameTime;
	if (m_periodicLogTimer->update(curFrameTime))
	{
		m_curFPS = m_curframeCounter * 1000.0f / m_periodicLogTimer->getStepTime();	//���㵱ǰ֡��

		XEE::XMemUsageInfo tmpInfo = XEE::getMemUsageInfo();
		if (m_curMemInfo.curWorkingSet == 0)
		{//��һ�ν����ʱ����ʾ����
			m_curMemInfo = tmpInfo;
			if (m_manualFPS > 0) LogNull("FPS:%.1f@%d Max:%.0fms Top:%.1fMB Cur:%dB CPU:%.1f%%",
				m_curFPS, m_manualFPS, m_maxFrameStep, m_curMemInfo.peakWorkSet * 9.5367431640625e-7,
				m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
			else LogNull("FPS:%.1f Max:%.0fms Top:%.1fMB Cur:%dB CPU:%.1f%%", m_curFPS, m_maxFrameStep,
				m_curMemInfo.peakWorkSet * 9.5367431640625e-7, m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
		}
		else
		{//������ʾ����
			LogNull("f:%.1f m:%.0f t:%.1f c:%d CPU:%.1f%%", m_curFPS, m_maxFrameStep,
				(tmpInfo.peakWorkSet - m_curMemInfo.peakWorkSet) * 9.5367431640625e-7,
				tmpInfo.curWorkingSet - m_curMemInfo.curWorkingSet, XEE::getCpuUsage());
			m_curMemInfo = tmpInfo;
		}
		m_curframeCounter = 0;
		m_maxFrameStep = -1.0f;
	}
	//���ֿ��Ʒ�ʽ��֡�ʿ��Ʋ���ȷ������CPU���Ľ�С
	if (m_manualFPS > 0 && curFrameTime < m_manualFrameTime)
	{
		//SDL_Delay(XEE_setFrameTime - XEE_frameTime);
		Sleep(m_manualFrameTime - curFrameTime);
	}
	if (XErrReporter.isErrored()
		&& !XErrReporter.getIsMainProc())
	{
		XErrReporter.setMainProc();
		XErrReporter.errorProc();
		exit(1);
	}
	engineMove(m_frameTime);

	if (m_windowData.withCaregiver &&m_pCaregiver != NULL)
		m_pCaregiver->update(m_frameTime);

	return m_frameTime;	//�������ε���֮���ʱ���
}
void XFrameWork::engineMove(float steptime)
{
	if (m_windowData.isShowVersion && m_showVersionTimer < 60000.0f)	//�汾����ʾһ����
		m_showVersionTimer += steptime;
	//�������Զ��ػ��Ŀ���
	if (m_windowData.isAutoShutDown && m_autoShutDownTimer.update(steptime))
	{//��������Զ��ػ�
		XSystemTime tempT;
		XTime::getTimeMs(tempT);
		if ((tempT.year >= m_windowData.autoShutDownYear || m_windowData.autoShutDownYear < 0)
			&& (tempT.month >= m_windowData.autoShutDownMonth || m_windowData.autoShutDownMonth < 0)
			&& (tempT.day >= m_windowData.autoShutDownDay || m_windowData.autoShutDownDay < 0)
			&& (tempT.hour >= m_windowData.autoShutDownHour || m_windowData.autoShutDownHour < 0)
			&& (tempT.minute >= m_windowData.autoShutDownMinute || m_windowData.autoShutDownMinute < 0)
			&& (tempT.second >= m_windowData.autoShutDownSecond || m_windowData.autoShutDownSecond < 0))
		{//�������������Զ��ػ�
			if (m_autoShutDownState == 0) XEE::shutDownSystem();
		}
		else
		{
			m_autoShutDownState = 0;
		}
	}
	XCurSndCore.update(steptime);
	XCtrlManager.update(steptime);
#if WITH_OBJECT_MANAGER
	XObjManager.move(steptime);
#endif
	if (m_windowData.withCustomTitle) m_customWinTitle->move(steptime);
#ifdef GET_ALL_PIXEL
	XEE_AllPixel = 0;
#endif
	if (!m_fontMD.getIsEnd())
		m_fontMD.move(steptime);
}
void XFrameWork::engineSysInit()	//���ڽ���֮��ĳ�ʼ��
{
	setSystemFont(m_windowData.systemFontFile.c_str());
	if (m_windowData.withCustomTitle) m_customWinTitle->init();
	if (m_windowData.logicFps > 0) setFPS(m_windowData.logicFps);
	if (m_windowData.drawFps <= 0) m_rendDelay = -1;
	else m_rendDelay = 1000.0f / m_windowData.drawFps;
#if WITH_OBJECT_MANAGER
	XObjManager.init();
#endif
	XErrReporter.init();
	if (m_windowData.withAlphaBG)
	{//���ô��ڱ���Ϊ͸��
#if BACKGROUND_ALPHA_MODE == 0
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE | WS_EX_TRANSPARENT);
		style &= ~WS_OVERLAPPEDWINDOW;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		DWM_BLURBEHIND bb = { 0 };
		HRGN hRgn = CreateRectRgn(0, 0, -1, -1);
		bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		bb.hRgnBlur = hRgn;
		bb.fEnable = TRUE;
		DwmEnableBlurBehindWindow(m_hwnd, &bb);
#endif
#if BACKGROUND_ALPHA_MODE == 1
		DWORD style = GetWindowLong(m_hwnd, GWL_EXSTYLE | WS_EX_TRANSPARENT);
		style |= WS_EX_LAYERED;
		SetWindowLong(m_hwnd, GWL_EXSTYLE, style);
		initBgAlphaData();
#endif
	}
}
void XFrameWork::engineDraw()
{
	if (m_haveSystemFont && m_windowData.isShowVersion
		&& m_showVersionTimer < 60000.0) m_showVersionFont->draw();
	if (m_windowData.withCustomTitle) m_customWinTitle->draw();
	XCtrlManager.draw();
	XCtrlManager.drawUp();
#if WITH_OBJECT_MANAGER
	XObjManager.draw();
#endif
	if (m_drawAfterCtrl != NULL) m_drawAfterCtrl();
	//2014.4.14,�޸�Ϊ��ʾ�Լ��Ĺ��
#ifdef WITHXEE_CURSOR
	if (m_windowData.isShowCursor)
	{//������ʾ���
		XRender::drawCross(getMousePos(), 10.0f, 3, XFColor::white);
		XRender::drawCross(getMousePos(), 8.0f, 2, XFColor::black);
	}
#endif
	if (!m_fontMD.getIsEnd() && m_systemFont != NULL)
		drawFont(getSceneCenter(), m_topMsgStr.c_str(), XFColor(1.0f, m_fontMD.getCurData()));
#if LIMITED_VERSION == 1
	if (m_systemFont == NULL) XEE::makeCrash();
	drawFont(getSceneCenter(), "�ڲ�汾");
#endif
}
void XFrameWork::setTopMsgStr(const std::string &str, bool withLog)
{
	if(withLog) LogStr(str.c_str());
	m_fontMD.setEx(0.0f, 1.0f, 1.0f, MD_MODE_SIN_MULT, 1);
	m_topMsgStr = str;
}
void XFrameWork::begin2DDraw()
{
	if (m_windowData.windowType == WINDOW_TYPE_3D &&
		!m_needReset3DParam)
	{//���ԭ��Ϊ3D���ڲ���Ҫ��ô��
		m_needReset3DParam = XTrue;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_DEPTH);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, m_windowData.w, m_windowData.h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
	}
}
void XFrameWork::end2DDraw()
{
	if (!m_needReset3DParam) return;//�ָ�3D����һЩ����
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
	m_needReset3DParam = XFalse;
}
#if BACKGROUND_ALPHA_MODE == 1
void XFrameWork::initBgAlphaData()				//������Ļ������
{
	if (m_isBGAlphaInited) return;
	m_isBGAlphaInited = true;
	GetWindowRect(m_hwnd, &m_rc);
	m_ptDest.x = m_rc.left;
	m_ptDest.y = m_rc.top;
	m_sizeDest.cx = m_windowWidth;
	m_sizeDest.cy = m_windowHeight;
	m_hMemDc = CreateCompatibleDC(m_wHDC);
	m_bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmpinfo.bmiHeader.biWidth = m_windowWidth;
	m_bmpinfo.bmiHeader.biHeight = m_windowHeight;
	m_bmpinfo.bmiHeader.biPlanes = 1;
	m_bmpinfo.bmiHeader.biBitCount = 32;//32bpp
	m_bmpinfo.bmiHeader.biCompression = BI_RGB;
	m_hBitmap = CreateDIBSection(m_wHDC, &m_bmpinfo, DIB_RGB_COLORS, &m_pBits, NULL, 0);
	assert(m_hBitmap != NULL);
	m_blf.BlendOp = AC_SRC_OVER;
	m_blf.BlendFlags = 0;
	m_blf.SourceConstantAlpha = 255;
	m_blf.AlphaFormat = AC_SRC_ALPHA;//1;
	m_bgAlphaPbo = XMem::createMem<XPBO>();
	assert(m_bgAlphaPbo != NULL);
	m_bgAlphaPbo->init(m_windowWidth, m_windowHeight, 0, 0, m_windowWidth, m_windowHeight, COLOR_BGRA);
	//wglMakeCurrent(m_hMemDc,wCurrentHGLRC);
	m_oldGDIObj = SelectObject(m_hMemDc, m_hBitmap);
	//	bgAlphaFbo.init(windowWidth,windowHeight,COLOR_RGBA);
}
void XFrameWork::releaseBhAlphaData()				//������Ļ������
{
	if (!m_isBGAlphaInited) return;
	if (m_hMemDc != NULL)
	{
		SelectObject(m_hMemDc, m_oldGDIObj);
		DeleteDC(m_hMemDc);
		m_hMemDc = NULL;
	}
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	XMem::XDELETE(m_bgAlphaPbo);
	m_isBGAlphaInited = false;
}
#endif
void XFrameWork::updateScreen(bool withEngin)				//������Ļ������
{
	if (withEngin)
		engineDraw();
	end2DDraw();
#ifdef GET_ALL_PIXEL
	static int maxPixel = 0;
	if (XEE_AllPixel > maxPixel)
	{
		maxPixel = XEE_AllPixel;
		printf("All pixel is %d\n", XEE_AllPixel);
	}
#endif
	if (m_windowData.withAlphaBG)
	{
#if BACKGROUND_ALPHA_MODE == 1
#ifdef ULW_ALPHA
//		m_bgAlphaFbo->removeFBO();
		//����R��BҪ��������ɫ����ȷ
		//����1��ֱ�Ӷ�ȡ��������ͨ�ã�����Ч�ʽ���
	//	glReadPixels(0,0,m_windowWidth,m_windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,m_pBits);	//��һ���������ĺܴ���Ҫ�Ż���
		//����2��ʹ��fbo��̫����
//		XGL::EnableTexture2D();
//		XGL::BindTexture2D(bgAlphaFbo.getTexture(0),0);
//		glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_UNSIGNED_BYTE,pBits);
		//����3��ʹ��PBO��xp�������������
		if (m_bgAlphaPbo->getPixel((unsigned char *)m_pBits))
		{
			UpdateLayeredWindow(m_hwnd, m_wHDC, NULL, &m_sizeDest, m_hMemDc, &m_ptSrc, 0, &m_blf, ULW_ALPHA);	//�������ú�ɫ��Ϊ͸��ɫ ULW_ALPHA = 2
			SwapBuffers(m_wHDC);
		}
//		bgAlphaFbo.useFBO();
		//unsigned char *tmp = XWindow.getWindowBuff();
	//	if(!BitBlt(hMemDc,0,0,windowWidth,windowHeight,wHDC,0,0,SRCCOPY))
	//	{
	//		printf("error!\n");
	//	}
	//	SelectObject(hMemDc,hOldBitmap);
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�����XP��ʹ������ķ���
		//glReadPixels(0,0,m_windowWidth,m_windowHeight,GL_BGRA,GL_UNSIGNED_BYTE,m_pBits);
		//UpdateLayeredWindow(m_hwnd,m_wHDC,NULL,&m_sizeDest,m_hMemDc,&m_ptSrc,0,&m_blf,ULW_ALPHA);
		//-------------------------------------------------------------
#endif
#endif
	}
	else
	{
#if CREATE_WINDOW_METHOD == 2
		XWindow.update();		//���������ǵ�Ч��
#else
		SwapBuffers(m_wHDC);
#endif
	}
}
XBool XFrameWork::initWindow()
{
	if (!engineInit()) return XFalse;
	//if(m_windowData.w <= 0 || m_windowData.h <= 0) return XFalse;//�����������Ƿ���ȷ
	//srand((unsigned)time(0)); //�����������
	bool ret = false;
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		if (m_windowData.rotateMode == WINDOW_ROTATE_MODE_90
			|| m_windowData.rotateMode == WINDOW_ROTATE_MODE_270)
			ret = createWindow(m_windowData.h, m_windowData.w,
				m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		else ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	case WINDOW_TYPE_NORMAL:
		ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	case WINDOW_TYPE_3D:
		ret = createWindow(m_windowData.w, m_windowData.h,
			m_windowData.windowTitle.c_str(), m_windowData.isFullScreen, m_windowData.withFrame);
		break;
	}
	if (!ret) return XFalse;
	XCurSndCore.setMusicVolume(m_windowData.BGVolume * 0.01f * 128.0f);
#if AUDIO_MATHOD == 0
	XCurSndCore.setVolume(-1, m_windowData.FWVolume * 0.01f * 128.0f);	//����û����Ч
#endif
#if AUDIO_MATHOD == 1
	XCurSndCore.setVolume(nullptr, m_windowData.FWVolume * 0.01f * 128.0f);	//����û����Ч
#endif
	initGL();
	XGL::showGLInfoToLog();	//��ʾGL�������Ϣ
	//��ʼ���ڶ���������
	wglMakeCurrent(m_wHDC, m_wCopyHGLRC);
	initGL();
	wglMakeCurrent(m_wHDC, m_wCurHGLRC);	//�ָ�������
	//2014.4.14�����޸�Ϊ��ʾ�Լ��Ĺ��
#ifdef WITHXEE_CURSOR
	XWindow.setCurcor(false);
#else
	if (!m_windowData.isShowCursor) XWindow.setCurcor(false);	//���ع��
#endif
	//if(screen != NULL)
	//{
	//	XEngineLogo EngineLogo;
	//	XResPack.setOutFileName("XEngine.log");
	//	EngineLogo.init(XVec2(m_windowData.w * 0.5 - 256 + 160,
	//		m_windowData.h * 0.5 - 256 + 128 + 96),RES_LOCAL_PACK);
	//	XResPack.setOutFileName(NULL);
	//	EngineLogo.reset();
	//	EngineLogo.setBackSize(m_windowData.w/8.0f,m_windowData.h/8.0f);
	//	int interval = 0;
	//	while(true) 
	//	{
	//		interval = engineIdle();
	//		EngineLogo.move(interval);
	//		EngineLogo.draw();
	//		if(EngineLogo.getIsEnd() != 0) break;
	//	}
	//	EngineLogo.release();
	//}
	//	setSystemFont(XEE_SYSTEM_FONT_PATH);
	//#if WITH_OBJECT_MANAGER
	//	XObjManager.init();
	//#endif
	engineSysInit();
	//atexit(release);
	return XTrue;
}
XBool XFrameWork::initGL()
{
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
	case WINDOW_TYPE_NORMAL:
		if (!initOpenGL2D()) return XFalse;
		break;
	case WINDOW_TYPE_3D:
		if (!initOpenGL3D()) return XFalse;
		break;
	}
	XGL::initOpenGLState();
	return XTrue;
}
void XFrameWork::initWindowMatrix()
{
	switch (m_windowData.windowType)
	{
	case WINDOW_TYPE_EX:
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glLoadIdentity();								//�任����ϵ����
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		switch (m_windowData.mode)
		{
		case WINDOW_SIZE_MODE_CLIP_LP:
			m_scenePosX = m_windowData.positionX;
			m_scenePosY = m_windowData.positionY;
			m_sceneWidth = m_windowData.w;
			m_sceneHeight = m_windowData.h;
			m_sceneScaleX = 1.0f;
			m_sceneScaleY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_MID:
			m_scenePosX = ((m_windowData.sceneW - m_windowData.w) >> 1) + m_windowData.positionX;
			m_scenePosY = ((m_windowData.sceneH - m_windowData.h) >> 1) + m_windowData.positionY;
			m_sceneWidth = m_windowData.w;
			m_sceneHeight = m_windowData.h;
			m_sceneScaleX = 1.0f;
			m_sceneScaleY = 1.0f;
			break;
		case WINDOW_SIZE_MODE_CLIP_RESIZE:
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			if (m_sceneScaleX <= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}
			else
			{
				m_sceneWidth = m_windowData.w * m_sceneScaleY;
				m_sceneHeight = m_windowData.sceneH;
				m_scenePosX = ((m_windowData.sceneW - m_sceneWidth) >> 1) + m_windowData.positionX;
				m_scenePosY = m_windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE_CLIP:
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			if (m_sceneScaleX >= m_sceneScaleY)
			{
				m_sceneWidth = m_windowData.sceneW;
				m_sceneHeight = m_windowData.h * m_sceneScaleX;
				m_scenePosX = m_windowData.positionX;
				m_scenePosY = ((m_windowData.sceneH - m_sceneHeight) >> 1) + m_windowData.positionY;
			}
			else
			{
				m_sceneWidth = m_windowData.w * m_sceneScaleY;
				m_sceneHeight = m_windowData.sceneH;
				m_scenePosX = ((m_windowData.sceneW - m_sceneWidth) >> 1) + m_windowData.positionX;
				m_scenePosY = m_windowData.positionY;
			}
			break;
		case WINDOW_SIZE_MODE_RESIZE:
			m_scenePosX = m_windowData.positionX;
			m_scenePosY = m_windowData.positionY;
			m_sceneWidth = m_windowData.sceneW;
			m_sceneHeight = m_windowData.sceneH;
			m_sceneScaleX = (float)(m_windowData.sceneW) / (float)(m_windowData.w);
			m_sceneScaleY = (float)(m_windowData.sceneH) / (float)(m_windowData.h);
			break;
		}
		switch (m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_scenePosY, m_sceneHeight + m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneWidth + m_scenePosX, m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_scenePosX, m_sceneWidth + m_scenePosX,
					m_sceneHeight + m_scenePosY, m_scenePosY, -1, 1);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch (m_windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				glOrtho(m_scenePosY, m_sceneHeight + m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_sceneWidth + m_scenePosX, m_scenePosX, -1, 1);
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				glOrtho(m_sceneHeight + m_scenePosY, m_scenePosY,
					m_scenePosX, m_sceneWidth + m_scenePosX, -1, 1);
				break;
			}
			break;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch (m_windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0: break;	//do nothing
		case WINDOW_ROTATE_MODE_90:
			switch (m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180: break;	//do nothing
		case WINDOW_ROTATE_MODE_270:
			switch (m_windowData.mode)
			{
			case WINDOW_SIZE_MODE_CLIP_LP:
				glTranslatef(m_windowData.h, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			case WINDOW_SIZE_MODE_CLIP_MID:
			case WINDOW_SIZE_MODE_CLIP_RESIZE:
			case WINDOW_SIZE_MODE_RESIZE_CLIP:
			case WINDOW_SIZE_MODE_RESIZE:
				glTranslatef(m_windowData.sceneH, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			break;
		}
		break;
	case WINDOW_TYPE_NORMAL:
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glLoadIdentity();								//�任����ϵ����
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		glOrtho(0.0, m_windowData.w, m_windowData.h, 0.0, -1.0, 1.0);					//only for 2D��ԭʼ����
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_scenePosX = 0;
		m_scenePosY = 0;
		m_sceneWidth = m_windowData.w;
		m_sceneHeight = m_windowData.h;
		m_sceneScaleX = 1.0f;
		m_sceneScaleY = 1.0f;
		break;
	case WINDOW_TYPE_3D:
		glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ ����ͶӰ����Ӧ��֮��ľ��������
		glLoadIdentity();								//�任����ϵ����
		glViewport(0, 0, m_windowData.w, m_windowData.h);
		gluPerspective(45.0f, (float)(m_windowData.w) / (float)(m_windowData.h), 1.0f, 10000.0f);		//���������ֵ��Ҫ��֮ǰ���Ǻϣ���Ȼ�����ģ�����������
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_scenePosX = 0;
		m_scenePosY = 0;
		m_sceneWidth = m_windowData.w;
		m_sceneHeight = m_windowData.h;
		m_sceneScaleX = 1.0f;
		m_sceneScaleY = 1.0f;
		break;
	}
}
XBool XFrameWork::initOpenGL3D()
{
	initWindowMatrix();	//��ʼ������
	initGlew();			//��ʼ��glew
	if (m_windowData.withAlphaBG) m_defBGColor.set(0.0f, 0.0f);	//�����͸�������Ļ�����������Ϊ͸��
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);				//�����ɫ
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);//���飺�Ż�����һ��������ÿ�ζ�����
	if (m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//��������������ͼ����
		glEnable(GL_POINT_SMOOTH);		//�������ֿ���ݹ���
		if (m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//3D
	glClearDepth(1.0f);									//Depth Buffer Setup
	glClearStencil(0);
	//	glEnable(GL_DEPTH);	
	glEnable(GL_DEPTH_TEST);							//Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								//The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//Really Nice Perspective Calculations
	XGL::EnableTexture2D();							// Enable 2D Texture Mapping
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

	glAlphaFunc(GL_GREATER, 0.1f);						//�������alpha����
	glEnable(GL_ALPHA_TEST);
	//����Ĵ�����Ϊ�˽�������������
	glFrontFace(GL_CCW);  //����������ж�����
	glEnable(GL_CULL_FACE);//�޳�����Ҫ�ı�����Ⱦ ���Ա�������Ч��
	glCullFace(GL_BACK);

	glEnable(GL_NORMALIZE);
	//+++++++++++++++++++++++++++++
	//ע�������ɫ�뷨����ͼ�г�ͻ
//	glEnable(GL_COLOR_MATERIAL);	//�����������ɷ�����ͼ��ɫ̫��������,����������������ɫ����Ч
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//-----------------------------
//	glMaterialfv(GL_FRONT,GL_SPECULAR,XVec4(1.0f,1.0f,1.0f,1.0f));
//	glMaterialf(GL_FRONT,GL_SHININESS,128.0f);

	//GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, XFColor::white);//����ȫ�ֻ�����
//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE); // ���ӵ�����Ϊ�ֲ��ӵ�
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glShadeModel(GL_SMOOTH);		//�������������������û����Ч
	return XTrue;
}
#pragma comment(lib,"glu32.lib")
void XFrameWork::initGlew()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != 0)
	{//��ʼ��ʧ��
		LogStr("Glew init error!");
		m_isOpenGL2Supported = XFalse;
	}
	else
	{
		//��¼�Կ��������ͼ�ߴ�
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
		//��¼���ͬʱ��ͼ�Ĵ�
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxTextureSum);
		//����Ƿ�֧��FBO
		m_isFboSupported = glewIsSupported("GL_ARB_framebuffer_object");
		if (!m_isFboSupported) LogStr("FBO is not supported.");
		//else LogStr("FBO is supported.");
		//���Ӳ���Ƿ�֧��OpenGL2.0
		if (glewIsSupported("GL_VERSION_2_0"))
		{
			//LogStr("Ready for OpenGL 2.0");
			m_isOpenGL2Supported = XTrue;
		}
		else
		{
			LogStr("OpenGL 2.0 not supported");
			m_isOpenGL2Supported = XFalse;
		}
		//�������ز��������
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_MULTISAMPLE);//����ʼ����ʱ������2��
		GLint bufSum = 0;
		glGetIntegerv(GL_SAMPLE_BUFFERS, &bufSum);
		glGetIntegerv(GL_SAMPLES, &m_multiSampleSum);//���Լ����ز����Ƿ���ã�ǰ�߷���1�����߷��ش���1��ʾ����; 3��
		if (bufSum == 1 && m_multiSampleSum > 1)
		{
			m_isMultiSampleSupported = XTrue;
			glEnable(GL_MULTISAMPLE);	//������Ч��Ҫ���Կ��Ŀ������������Ϊ���ͼ������
			//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);	//������������alpha��Ϸǳ��ѿ�
			//glSampleCoverage(0.9f, GL_TRUE);
		}
		else
			m_isMultiSampleSupported = XFalse;
	}
}
XBool XFrameWork::initOpenGL2D()
{
	initWindowMatrix();	//��ʼ������
	initGlew();			//��ʼ��glew
	glShadeModel(GL_SMOOTH);						// Enable Smooth Shading
	if (m_windowData.withAlphaBG) m_defBGColor.set(0.0f, 0.0f);	//�����͸�������Ļ�����������Ϊ͸��
	glClearColor(m_defBGColor.r, m_defBGColor.g, m_defBGColor.b, m_defBGColor.a);			//�����ɫ
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);//���飺�Ż�����һ��������ÿ�ζ�����
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	if (m_isMultiSampleSupported)
	{
		glEnable(GL_POLYGON_SMOOTH);			//��������������ͼ����
		glEnable(GL_POINT_SMOOTH);		//�������ֿ���ݹ���
		if (m_isLineSmooth) glEnable(GL_LINE_SMOOTH);
	}
	//��ʼ��GL���������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glDisable(GL_DEPTH);							//2D��Ŀ����Ҫ��Ȳ���
	glDisable(GL_DEPTH_TEST);						//2D��Ŀ����Ҫ��Ȳ���
	//glEnalbe(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	return XTrue;
}
int XFrameWork::suspendKeyProc()
{
	int ret = 0;
	XInputEvent tmpEvent;
	while (getInputEvent(tmpEvent))
	{
		if (tmpEvent.isKeyBoardDown())
		{
			switch (tmpEvent.keyValue)
			{
			case XKEY_ESCAPE: ret = 1; break;
			}
		}
	}
	return ret;
}
void XFrameWork::suspend(const char * str)
{
	m_isSuspend = XTrue;
	if (m_haveSystemFont)
	{//���Լ�������
		if (str == NULL)
		{
			while (true)
			{
				if (suspendKeyProc() != 0) exit(1);
				clearScreen();
				updateScreen();
				Sleep(1);
			}
		}
		else
		{
		//	string tempStr = str;
		//	tempStr += "\n";
		//	LogStr(tempStr.c_str());
			LogStr(str);
			m_systemFont->setString(str);
			while (true)
			{
				if (suspendKeyProc() != 0)
				{
#if WITH_LOG
					LogRelease();
#endif
					exit(1);
				}
				clearScreen();
				m_systemFont->draw();
				updateScreen();
				Sleep(1);
			}
		}
	}
	else
	{//û���Լ�������
		if (str == NULL)
		{//ɶҲ���ɾ�������
			while (true)
			{
				if (suspendKeyProc() != 0) exit(1);
				Sleep(1);
			}
		}
		else
		{//��ʾ������Ϣ����������
		//	string tempStr = str;
		//	tempStr += "\n";
		//	LogStr(tempStr.c_str());
			LogStr(str);
			while (true)
			{
				if (suspendKeyProc() != 0)
				{
#if WITH_LOG
					LogRelease();
#endif
					exit(1);
				}
				Sleep(1);
			}
		}
	}
}
#ifdef XEE_OS_WINDOWS
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)     
{
	if (gFrameworkData.pFrameWork == NULL) return false;
	if (GetProp(hwnd, gFrameworkData.pFrameWork->m_programPropName.c_str()) == 
		gFrameworkData.pFrameWork->m_programPropValue)
	{     
		*(HWND*)lParam = hwnd;     
		return false;     
	}     
	return true;     
} 
bool XFrameWork::isProgramExist(const std::string& name)
{
	m_programPropName = name;
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc, (LPARAM)&oldHWnd);	//ö���������еĴ���     
	if (oldHWnd != NULL)
	{
		ShowWindow(oldHWnd, SW_SHOWNORMAL);		//�����ҵ���ǰһ������     
		SetForegroundWindow(oldHWnd);			//������Ϊǰ������     
		return true;							//�˳���������     
	}
	SetProp(m_hwnd, m_programPropName.c_str(), m_programPropValue);
	return false;
}
#endif
//�����Լ�����Ϣ������
//����Ϣ�������´�
LONG_PTR currentWndProc = NULL;
static LRESULT CALLBACK winMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (gFrameworkData.pFrameWork == NULL)
		return CallWindowProc((WNDPROC)currentWndProc, hwnd, Msg, wParam, lParam);
	bool end = false;
	//++++++++++++++++++++++++++++++++++++
	//���ں�ȫ���л���һ������
	switch (Msg)
	{
	case WM_ACTIVATE:
		if (!gFrameworkData.pFrameWork->m_isSetToFullScreen) break;
		if (wParam)
		{//�ظ����ڣ����÷ֱ���
			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmPelsWidth = getWindowWidth();
			dm.dmPelsHeight = getWindowHeight();
			dm.dmBitsPerPel = 32;
			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.flags = 0;
			state.showCmd = SW_SHOWNORMAL;
			state.rcNormalPosition.left = 0;
			state.rcNormalPosition.top = 0;
			state.rcNormalPosition.right = getWindowWidth();
			state.rcNormalPosition.bottom = getWindowHeight();
			SetWindowPlacement(hwnd, &state);
			//	printf("����\n");
		}
		else
		{//��С�����ڣ��ظ��ֱ���
			ChangeDisplaySettings(0, CDS_RESET);

			WINDOWPLACEMENT state;
			state.length = sizeof(WINDOWPLACEMENT);
			state.showCmd = SW_MINIMIZE;
			SetWindowPlacement(hwnd, &state);
			//	printf("�Ǽ���\n");
		}
		break;
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	case WM_TOUCH:
		int numInputs = (int)wParam;
		TOUCHINPUT ti[256];
		if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs > 256 ? 256 : numInputs, ti, sizeof(TOUCHINPUT)))
		{
			for (unsigned int i = 0; i < numInputs; ++i)
			{
				XEG.touchProc(ti[i]);
			}
		}
		CloseTouchInputHandle((HTOUCHINPUT)lParam);
		break;
		//return (0);
#endif
	}
	//-----------------------------------------------------
	LRESULT ret;
	if (gFrameworkData.pFrameWork->getIsProjectAsDaemon())
	{
		ret = gFrameworkData.pFrameWork->daemonProc(hwnd, Msg, wParam, lParam, end);
		if (end) return ret;
	}
	if (gFrameworkData.pFrameWork->m_customWinProc != NULL)
	{
		ret = gFrameworkData.pFrameWork->m_customWinProc(hwnd, Msg, wParam, lParam, end);
		if (end) return ret;
	}
	//���洦�����뷨�¼�
	ret = XImm::inputMsgProc(hwnd, Msg, wParam, lParam, end);
	if (end) return ret;
	return CallWindowProc((WNDPROC)currentWndProc, hwnd, Msg, wParam, lParam);
}
void XFrameWork::touchProc(const TOUCHINPUT& ti)
{
	XInputEvent tmp;
	tmp.type = EVENT_TOUCH;
	tmp.touchID = ti.dwID;
	XVec2 tmpPos(ti.x * 0.01f, ti.y * 0.01f);
	tmpPos = getClientPos(tmpPos);
	m_touchPosition = getMousePosFromWindow(tmpPos.x, tmpPos.y);
	tmp.mouseX = tmpPos.x;
	tmp.mouseY = tmpPos.y;
	if (ti.dwFlags & TOUCHEVENTF_DOWN)
	{
		tmp.touchState = TOUCH_DOWN;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
	if (ti.dwFlags & TOUCHEVENTF_MOVE)
	{
		tmp.touchState = TOUCH_MOVE;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
	if (ti.dwFlags & TOUCHEVENTF_UP)
	{
		tmp.touchState = TOUCH_UP;
		gFrameworkData.pFrameWork->inputEvent(tmp);
		return;
	}
}
//--------------------------------------
static void registerMyMsgProc()
{
	if (gFrameworkData.pFrameWork == NULL) return;
	//DragAcceptFiles (wHandle, TRUE);	//�Ƿ������򴰿���ק�ļ�
	//����1
	//currentWndProc = (WNDPROC)SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
	//����2
	currentWndProc = GetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC);
	SetWindowLongPtr(gFrameworkData.pFrameWork->getHWND(), GWLP_WNDPROC, (LONG_PTR)winMsgProc);
}
bool XFrameWork::createWindow(int width, int height, const char *windowTitle, bool isFullScreen, bool withFrame)
{
	if (!XWindow.createWindow(width, height, windowTitle, isFullScreen, withFrame)) return false;
	m_windowWidth = width;
	m_windowHeight = height;
	//��ʼ��һЩ����
	m_wHDC = wglGetCurrentDC();
	m_wCurHGLRC = wglGetCurrentContext();
	m_wCopyHGLRC = wglCreateContext(m_wHDC);
	//wglCopyContext(m_wCurHGLRC,m_wCopyHGLRC,GL_ALL_ATTRIB_BITS);
	wglShareLists(m_wCurHGLRC, m_wCopyHGLRC);
	//	m_hwnd = GetActiveWindow();
	m_hwnd = WindowFromDC(m_wHDC);
	m_mainThreadHandle = GetCurrentThreadId();
#if CREATE_WINDOW_METHOD == 2
	if (!withFrame)
	{
		//�������ô����ޱ߿�ģʽ
		XVec2 pos;
		if (m_windowData.isInitWindowPos)
			pos.set(m_windowData.windowPosX, m_windowData.windowPosY);
		else
			pos = getWindowPos();
		DWORD style = GetWindowLong(m_hwnd, GWL_STYLE);
		DWORD windowstyle =
			(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		style &= ~windowstyle;
		style |= WS_POPUP;
		SetWindowLong(m_hwnd, GWL_STYLE, style);
		if(m_windowData.isAlwaysTop)
			SetWindowPos(m_hwnd, HWND_TOPMOST, pos.x, pos.y, width, height, SWP_SHOWWINDOW);
		else
			SetWindowPos(m_hwnd, HWND_TOP, pos.x, pos.y, width, height, SWP_SHOWWINDOW);
	}
	else
	{
		if (m_windowData.isInitWindowPos
			|| m_windowData.isAlwaysTop) setWindowPos(m_windowData.windowPosX, m_windowData.windowPosY, m_windowData.isAlwaysTop);
	}
#else
	if (windowTitle != NULL) XWindow.setWindowTitle(windowTitle);	//���ô��ڱ���
	if (m_windowData.isInitWindowPos
		|| m_windowData.isAlwaysTop) setWindowPos(m_windowData.windowPosX, m_windowData.windowPosY, m_windowData.isAlwaysTop);
#endif

	registerMyMsgProc();
#if _WIN32_WINNT >= 0x0501 && WITH_TOUCH_SUPPLY == 1
	//RegisterTouchWindow(m_hwnd, TWF_WANTPALM);
	RegisterTouchWindow(m_hwnd, 0);
#endif
	XCurSndCore.init();
	return true;
}
void XFrameWork::setStyle(XColorStyle style)
{
	switch (style)
	{
	case COLOR_STYLE_NORMAL:
		XCCS::lightBGColor.set(0.9f, 1.0f);
		XCCS::onColor.set(0.85f, 1.0f);
		XCCS::blackOnColor.set(0.8f, 1.0f);
		XCCS::normalColor.set(0.75f, 1.0f);
		XCCS::blackNormalColor.set(0.7f, 1.0f);
		XCCS::lightSpecialColor.set(0.65f, 1.0f);
		XCCS::specialColor.set(0.6f, 1.0f);
		XCCS::downColor.set(0.5f, 1.0f);
		XCCS::lightMouseColor.set(0.45f, 1.0f);
		XCCS::mouseColor.set(0.4f, 1.0f);
		XCCS::disableColor.set(0.35f, 1.0f);
		XCCS::blackDownColor.set(0.25f, 1.0f);
		XCCS::lightBlackColor.set(0.2f, 1.0f);
		XCCS::blackColor.set(0.1f, 1.0f);
		XCCS::darkColor.set(0.0f, 1.0f);
		setBGColor(XFColor(0.25f, 1.0f));//Ĭ�ϵı�����ɫ
		m_systemFont->setColor(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	case COLOR_STYLE_LIGHT:
		XCCS::lightBGColor.set(0.9f, 1.0f);
		XCCS::onColor.set(0.875f, 1.0f);
		XCCS::blackOnColor.set(0.85f, 1.0f);
		XCCS::normalColor.set(0.825f, 1.0f);
		XCCS::blackNormalColor.set(0.8f, 1.0f);
		XCCS::lightSpecialColor.set(0.775f, 1.0f);
		XCCS::specialColor.set(0.75f, 1.0f);
		XCCS::downColor.set(0.725f, 1.0f);
		XCCS::lightMouseColor.set(0.7f, 1.0f);
		XCCS::mouseColor.set(0.675f, 1.0f);
		XCCS::disableColor.set(0.65f, 1.0f);
		XCCS::blackDownColor.set(0.625f, 1.0f);
		XCCS::lightBlackColor.set(0.60f, 1.0f);
		XCCS::blackColor.set(0.575f, 1.0f);
		XCCS::darkColor.set(0.55f, 1.0f);
		setBGColor(XFColor(0.95f, 1.0f));//Ĭ�ϵı�����ɫ
		m_systemFont->setColor(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	}
}
#define IDR_HIDE         (16)
#define IDR_HIDECONSOLE  (18)
#define IDR_QUIT         (20)
#define IDI_ICON1		 (101)
void XFrameWork::releaseIcon(){Shell_NotifyIcon(NIM_DELETE,&m_nid);}
//����Ϣ�������´�
LRESULT XFrameWork::daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end)
{
	end = false;
	static UINT WM_TASKBARCREATED = 0;
	static bool isHide = false;
	static bool isHideConsole = false;
	static bool isHideWindows = false;
	static HMENU hmenu;
	static bool isInited = false;
	if (!isInited)
	{//��ʼ����������
		isInited = true;
		WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
		m_nid.cbSize = sizeof(m_nid);
		m_nid.hWnd = hwnd;
		m_nid.uID = 0;
		m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_nid.uCallbackMessage = WM_USER;
		m_nid.hIcon = LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1);   //IDI_APPLICATION
		lstrcpy(m_nid.szTip, TEXT(m_windowData.windowTitle.c_str()));
		Shell_NotifyIcon(NIM_ADD, &m_nid);
		hmenu = CreatePopupMenu();
		AppendMenu(hmenu, MF_STRING, IDR_HIDE, TEXT("���ش���"));
		CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
		if (m_deamonWithConsole)
		{
			AppendMenu(hmenu, MF_STRING, IDR_HIDECONSOLE, TEXT("���ؿ���̨"));
			CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_CHECKED);
		}
		AppendMenu(hmenu, MF_STRING, IDR_QUIT, TEXT("�˳�"));
		//��������Ϊ��һ�ε��õ�ʱ�����ش���
		ShowWindow(hwnd, SW_HIDE);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		isHide = true;
	}
	switch (Msg)
	{
		//case WM_SETTEXT:	//��������Ϊ��һ�ε��õ�ʱ�����ش���
		//	if(!isHide)
		//	{
		//		ShowWindow(hwnd,SW_HIDE);
		//		ShowWindow(GetConsoleWindow(),SW_HIDE);
		//		isHide = true;
		//	}
		//	break;
	case WM_USER:
		if (lParam == WM_RBUTTONDOWN)
		{
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hwnd);
			int menuState = TrackPopupMenu(hmenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);
			switch (menuState)
			{
			case IDR_HIDE:
				isHideWindows = !isHideWindows;
				if (!isHideWindows)
				{
					ShowWindow(hwnd, SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
				}
				else
				{
					ShowWindow(hwnd, SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;
			case IDR_HIDECONSOLE:
				isHideConsole = !isHideConsole;
				if (!isHideConsole)
				{
					ShowWindow(GetConsoleWindow(), SW_HIDE);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_CHECKED);
				}
				else
				{
					ShowWindow(GetConsoleWindow(), SW_NORMAL);
					CheckMenuItem(hmenu, IDR_HIDECONSOLE, MF_BYCOMMAND | MF_UNCHECKED);
				}
				break;
			case IDR_QUIT:
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				releaseIcon();
				break;
			case 0:
				PostMessage(hwnd, Msg, NULL, NULL);
				break;
			default:
				PostMessage(hwnd, Msg, wParam, lParam);
				break;
			}
		}
		if (lParam == WM_LBUTTONDBLCLK)
		{//˫������
			SetForegroundWindow(hwnd);
			isHideWindows = !isHideWindows;
			if (!isHideWindows)
			{
				ShowWindow(hwnd, SW_HIDE);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_CHECKED);
			}
			else
			{
				ShowWindow(hwnd, SW_NORMAL);
				CheckMenuItem(hmenu, IDR_HIDE, MF_BYCOMMAND | MF_UNCHECKED);
			}
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		releaseIcon();
		break;
	default:
		if (Msg == WM_TASKBARCREATED)
		{
			isHide = false;
			SendMessage(hwnd, WM_PAINT, wParam, lParam);
		}
		break;
	}
	return 0;
}
XBool XFrameWork::setSystemFont(const char * filename)
{
	if (m_haveSystemFont) return XFalse;
	if (!m_systemFont->initFromTTF(filename, m_windowData.systemFontSize, RES_LOCAL_FOLDER, false))
	{
		LogStr("ϵͳ�������ʧ��!");
		return XFalse;
	}
	m_systemFont->setPosition(getSceneW() >> 1, getSceneH() >> 1);
	m_systemFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_systemFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	m_systemFont->setColor(XFColor(0.75f, 1.0f));
	//m_systemFont->setDistance(-2.0f);	//��������ÿ�������Ŀǰ����bug�������XEdit��ѡ������������
	m_haveSystemFont = XTrue;
	if (m_windowData.isShowVersion)
	{
		m_showVersionFont->setACopy(*m_systemFont);
		m_showVersionFont->setPosition(XVec2::zero);
		m_showVersionFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_showVersionFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		m_showVersionFont->setColor(XFColor(0.75f, 1.0f));
		m_showVersionFont->setString(m_windowData.versionString.c_str());
	}

	return XTrue;
}
void XFrameWork::toggleFullScreen()
{
	if (m_windowData.isFullScreen) return;
	//printf("���д��ں�ȫ�����л�\n");
	static RECT rect;
	static LONG state = 0;
	if (!m_isSetToFullScreen)
	{//����Ϊȫ��
		GetWindowRect(getHWND(), &rect);
		state = GetWindowLongPtr(getHWND(), GWL_STYLE);

		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = getWinW();
		dm.dmPelsHeight = getWinH();
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			SetWindowLongPtr(getHWND(), GWL_STYLE,
				WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(getHWND(), 0, 0, getWinW(), getWinH(), TRUE);
			//setWindowPos(0,0);
			m_isSetToFullScreen = true;
		}
	}
	else
	{//����Ϊ����
		if (ChangeDisplaySettings(0, CDS_RESET) == DISP_CHANGE_SUCCESSFUL)
		{
			if (state == 0)
			{//û�г�ʼ����
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);	//��ȡ�߿������
				SetWindowLongPtr(getHWND(), GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				MoveWindow(getHWND(), 0, 0, getWinW() + rect.right - rect.left, getWinH() + rect.bottom - rect.top, TRUE);
			}
			else
			{
				SetWindowLongPtr(getHWND(), GWL_STYLE, state);
				MoveWindow(getHWND(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
			}
			m_isSetToFullScreen = false;
		}
	}
}
#if WITH_ENGINE_STATISTICS
void XFrameWork::logEngineStatistics()			//������ͳ�������������־
{
	if(!m_withEngineStatistics) return;
	LogNull("��ͼ��:%d",m_totalTextureSum);
	LogNull("��ͼ���:%d",m_totalTextureArea);
	LogNull("�Դ�ʹ����:%d",m_totalVideoMemUsage);
	LogNull("ƽ��֡��ͼ���:%d",m_totalFrameTextureArea);
	LogNull("ƽ��֡�Դ����ʹ����:%f",m_curVideoMemBandwidthUsage);
}
void XFrameWork::addStaticsticsTexInfo(unsigned int id,int level,int type)
{
	if(!m_withEngineStatistics) return;
	int w = 0,h = 0,format;
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_HEIGHT,&h);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_INTERNAL_FORMAT,&format);
	++ m_totalTextureSum;
	m_totalTextureArea += w * h;
	switch(format)
	{
	case GL_RGB:
	case GL_BGR:
		m_totalVideoMemUsage += w * h * 3;
		break;
	case GL_RGBA:
	case GL_BGRA:
		m_totalVideoMemUsage += (w * h) << 2;
		break;
	case GL_RGBA32F:
		m_totalVideoMemUsage += (w * h) << 3;
		break;
	case GL_RG:
		m_totalVideoMemUsage += (w * h) << 1;
		break;
	case GL_RED:
	case GL_LUMINANCE:
		m_totalVideoMemUsage += w * h;
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_STENCIL:
		break;
	}
}
void XFrameWork::decStaticsticsTexInfo(unsigned int id,int level,int type)
{//δ������֤
	if(!m_withEngineStatistics) return;
	switch(type)
	{
	case GL_TEXTURE_CUBE_MAP:
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP,id);
		break;
	case GL_TEXTURE_2D:
		XGL::BindTexture2D(id);
		break;
	}
	int w = 0,h = 0,format;
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_WIDTH,&w);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_HEIGHT,&h);
	glGetTexLevelParameteriv(type,level,GL_TEXTURE_INTERNAL_FORMAT,&format);
	-- m_totalTextureSum;
	m_totalTextureArea -= w * h;
	switch(format)
	{
	case GL_RGB:
	case GL_BGR:
		m_totalVideoMemUsage -= w * h * 3;
		break;
	case GL_RGBA:
	case GL_BGRA:
		m_totalVideoMemUsage -= (w * h) << 2;
		break;
	case GL_RGBA32F:
		m_totalVideoMemUsage -= (w * h) << 3;
		break;
	case GL_RG:
		m_totalVideoMemUsage -= (w * h) << 1;
		break;
	case GL_RED:
	case GL_LUMINANCE:
		m_totalVideoMemUsage -= w * h;
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_STENCIL:
		break;
	}	
	printf("%d\n",m_totalVideoMemUsage);
	switch(type)
	{
	case GL_TEXTURE_CUBE_MAP:
		glBindTexture(GL_TEXTURE_CUBE_MAP,0);
		glDisable(GL_TEXTURE_CUBE_MAP);
		break;
	case GL_TEXTURE_2D:
		break;
	}
}
#endif
struct XScreenShotData
{
	char fileName[MAX_FILE_NAME_LENGTH];
	int w;
	int h;
	int buffSize;
	unsigned char *data;
	unsigned char *tmpData;	//�������ݷ�ת
	XScreenShotData()
		:data(NULL)
		, tmpData(NULL)
		, buffSize(0)
	{}
	~XScreenShotData()
	{
		XMem::XDELETE_ARRAY(data);
		XMem::XDELETE_ARRAY(tmpData);
	}
	bool createMem()
	{
		if (buffSize >= w * h * 3) return true;
		buffSize = w * h * 3;
		XMem::XDELETE_ARRAY(data);
		data = XMem::createArrayMem<unsigned char>(buffSize);
		if (data == NULL)
		{
			buffSize = 0;
			return false;
		}
		XMem::XDELETE_ARRAY(tmpData);
		tmpData = XMem::createArrayMem<unsigned char>(buffSize);
		if (tmpData == NULL)
		{
			XMem::XDELETE_ARRAY(data);
			buffSize = 0;
			return false;
		}
		return true;
	}
	void top2Down()	//���·�ת����
	{
		unsigned char *pd = tmpData + (w * h - w) * 3;
		unsigned char *ps = data;
		int sum = w * 3;
		for (int i = 0; i < h; ++i)	//���·�ת
		{
			memcpy(pd, ps, sum);
			ps += sum;
			pd -= sum;
		}
	}
};
XScreenShotData *gScreenShotData = NULL;	//ע���������û�����ձ��ͷŵ�
bool gIsScreenShotOver = true;
//��������ʹ���˶��̣߳�������Ҫ�������ݻ���������δ����
#ifdef XEE_OS_WINDOWS
DWORD WINAPI saveDataToPNG(void *)
#endif
#ifdef XEE_OS_LINUX
static void *saveDataToPNG(void * pParam)
#endif
{
	if (gScreenShotData == NULL || gScreenShotData->data == NULL)
	{
#ifdef XEE_OS_WINDOWS
		return 1;
#endif
#ifdef XEE_OS_LINUX
		return;
#endif
	}
	//�����ݱ����ͼƬ
	gScreenShotData->top2Down();
	//	printf("%d\n",getCurrentTicks());
	XPng::savePNG(gScreenShotData->fileName, gScreenShotData->tmpData,
		gScreenShotData->w, gScreenShotData->h, COLOR_RGB, Z_DEFAULT_COMPRESSION);
	//	printf("%d\n",getCurrentTicks());
	gIsScreenShotOver = true;
#ifdef XEE_OS_WINDOWS
	return 1;
#endif
}
bool XFrameWork::addABaby(XBabyBase *baby,int resetTimer)
{
	if(!m_windowData.withCaregiver || m_pCaregiver == NULL) return false;
	return m_pCaregiver->addABaby(baby,resetTimer);
}
//��������ʹ���˶��̣߳�������Ҫ�������ݻ���������δ����
XBool XFrameWork::gameShot(const char *fileName, int w, int h, bool withThread, float x, float y)
{
	if (fileName == NULL) return XFalse;
	if (x < 0.0f || y < 0.0f) return XFalse;
	if (w <= 0 || h <= 0) return XFalse;
	if (!gIsScreenShotOver) return XFalse;
	gIsScreenShotOver = false;
	if (gScreenShotData == NULL)
	{
		gScreenShotData = XMem::createMem<XScreenShotData>();
		if (gScreenShotData == NULL)
		{
			gIsScreenShotOver = true;
			return XFalse;
		}
	}

	gScreenShotData->h = h;
	gScreenShotData->w = w;
	strcpy_s(gScreenShotData->fileName, MAX_FILE_NAME_LENGTH, fileName);

	if (!gScreenShotData->createMem())
	{
		XMem::XDELETE(gScreenShotData);
		gIsScreenShotOver = true;
		return XFalse;
	}

	//��ȡ����
	//glReadPixels(0,0,w,h,GL_RGB,GL_BYTE,gScreenShotData->data);
	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, gScreenShotData->data);	//ע��ߴ�4�ֽڶ��룬��������crash
	if (withThread)
	{//�Ż�֮��ֱ�ӿ���һ���߳�������ͼƬ
#ifdef XEE_OS_WINDOWS
		DWORD tempThreadP;
		if (CreateThread(0, 0, saveDataToPNG, NULL, 0, &tempThreadP) == 0)
		{
			return XFalse;
		}
#endif
#ifdef XEE_OS_LINUX
		pthread_t tempThreadP;
		if (pthread_create(&tempThreadP, NULL, &(saveDataToPNG), NULL) != 0)
		{
			return XFalse;
		}
#endif
	}
	else
	{
		saveDataToPNG(NULL);
	}
	return XTrue;
}
const XFontUnicode& getDefaultFont()
{
	if (gFrameworkData.pFrameWork != NULL && gFrameworkData.pFrameWork->m_systemFont != NULL)
		return *gFrameworkData.pFrameWork->m_systemFont;
	else
	{
		LogStr("��ʹ��Ĭ������ʱ���ȳ�ʼ�����!");
		return XFontUnicode(0);
	}
}
bool getIsImageSupport(const std::string& ext)
{
	return (XFile::fileNameCompare(ext.c_str(), "png") || 
		XFile::fileNameCompare(ext.c_str(), "bmp") || 
		XFile::fileNameCompare(ext.c_str(), "jpg") || 
		XFile::fileNameCompare(ext.c_str(), "tga") || 
		XFile::fileNameCompare(ext.c_str(), "tif") || 
		XFile::fileNameCompare(ext.c_str(), "jpeg") ||
		XFile::fileNameCompare(ext.c_str(), "gif"));
}
bool getIsSoundSupport(const std::string& ext)
{
	return (XFile::fileNameCompare(ext.c_str(), "mp3") ||
		XFile::fileNameCompare(ext.c_str(), "wav") ||
		XFile::fileNameCompare(ext.c_str(), "ogg"));
}
#if !WITH_INLINE_FILE
#include "XFrameWork.inl"
#endif
}