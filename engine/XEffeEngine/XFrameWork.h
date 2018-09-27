#ifndef _JIA_XFRAMEWORK_
#define _JIA_XFRAMEWORK_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//��ܵ�����
#include "XErrorReporter.h"
#if WITH_LOG
#include "XLogBook.h"
#else
#include "XTimer.h"
#endif
#include "XEngineCommon.h"
#include "XMouseAndKeyBoardDefine.h"
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XInputEventBuff.h"
#include "XMath\XMoveData.h"
#include "XCritical.h"
class XLicenseBase
{
public:
	virtual std::string createStr() = 0;	//������֤��
	virtual std::string createKey() = 0;	//������֤��
	virtual bool certification() = 0;		//�Ƿ���֤ͨ��
	virtual std::string getCodeStr()const = 0;
	virtual std::string getKeyStr()const = 0;
	virtual void setKey(const char * str) = 0;	//���ü�����
	virtual void setCodeStr(const char *code) = 0;

	virtual bool saveCode() = 0;
	virtual bool readCode() = 0;
	virtual bool saveKey() = 0;
	virtual bool readKey() = 0;
	virtual bool init() = 0;
};
namespace XE{
class XFrameWork;
class XMusic;
class XSound;
class XConfigManager;
class X3DWorld;
class XFontTTF;
class XOperateManager;
class XSoundCore;
class XWindowCore;
class XTextureResourceManager;
class XResourcePack;
class XResourceManager;
class XControlManager;
#if WITH_OBJECT_MANAGER
class XObjectManager;
#endif
//�����ǿ��ȫ�ֵĶ���
struct XFrameWorkData
{
	XFrameWork *pFrameWork;			//��ܵ�ָ��
#if WITH_OBJECT_MANAGER
	XObjectManager *pObjManager;		//���������
#endif
	XControlManager *pCtrlManager;	//�ؼ�������
#if WITH_LOG
	XLogbook *pLog;					//��־��ָ��
#endif
	XResourceManager *pResManager;	//��Դ������
	XResourcePack *pResPack;			//��Դ�����
	XTextureResourceManager *pTexManager;		//��ͼ������
	XErrorReporter *pErrReporter;	//���������
	XMusic *pMusicPlayer;			//���ֲ�����
	XSound *pSoundPlayer;			//��Ч������
	XConfigManager *pCfgManager;
	X3DWorld *p3DWorld;
	XFontTTF *pFontTTF;
	XOperateManager *pOperateManager;
	XSoundCore *pSoundCore;			//��Ч���ĵ�ָ��
	XWindowCore *pWindowCore;		//���ڵĺ���ָ��
	XLicenseBase *pLicense;		//�������
	XFrameWorkData()
		:pFrameWork(NULL)
#if WITH_OBJECT_MANAGER
		, pObjManager(NULL)
#endif
		, pCtrlManager(NULL)
#if WITH_LOG
		, pLog(NULL)					//��־��ָ��
#endif
		, pResManager(NULL)
		, pResPack(NULL)
		, pTexManager(NULL)
		, pErrReporter(NULL)
		, pMusicPlayer(NULL)			//���ֲ�����
		, pSoundPlayer(NULL)			//��Ч������
		, pCfgManager(NULL)
		, p3DWorld(NULL)
		, pFontTTF(NULL)
		, pOperateManager(NULL)
		, pSoundCore(NULL)
		, pWindowCore(NULL)
		, pLicense(NULL)
	{}
#if WITH_LOG
	bool create(bool isIniLog = false, XLogBookMode mode = LOGBOOK_MODE_SIMPLE,
		const std::string& serverFilename = "../LogServer/LogServer.exe");
#else
	bool create();
#endif
	void release();
};
extern XFrameWorkData gFrameworkData;	//��ܵ�ȫ�ֱ���
//��־����ؽӿ�
#if WITH_LOG
#define LogStr gFrameworkData.pLog->addLogInfoStr
#define LogNull gFrameworkData.pLog->addLogInfoNull
#define LogExp gFrameworkData.pLog->addLogInfoExp
#define LogPause gFrameworkData.pLog->disable
#define LogResume gFrameworkData.pLog->enable
#define LogRelease gFrameworkData.pLog->release
#else
#define LogStr(p) (void)(p)
#define LogNull (void)
#define LogExp (void)
#define LogPause (void)
#define LogResume (void)
#define LogRelease (void)
#endif
inline void reportError(const std::string& errorDescription, const std::string& filename,
	int line, const std::string& funName)
{
	gFrameworkData.pErrReporter->reportError(errorDescription, filename, line, funName);
}
#define REPORT_ERROR(p) reportError(p,__FILE__,__LINE__,__FUNCDNAME__)
//Ϊ�˷���ʱ����д���㣬���涨��һЩ�꣬������Щ�궼����Ϊָ�����Ч�����crash����Ҫ���·�װ
#define XEG (*gFrameworkData.pFrameWork)	//�����÷��߼��ϲ��Ͻ�������Ϊָ��û�г�ʼ�������crash����Ҫ���·�װ
#define XCtrlManager (*gFrameworkData.pCtrlManager)
#define XResManager (*gFrameworkData.pResManager)
#if WITH_OBJECT_MANAGER
#define XObjManager (*gFrameworkData.pObjManager)
#endif
#define XResPack (*gFrameworkData.pResPack)
#define XTexManager (*gFrameworkData.pTexManager)
#define XErrReporter (*gFrameworkData.pErrReporter)
#define XCFGManager (*gFrameworkData.pCfgManager)
#define XTTFFont (*gFrameworkData.pFontTTF)
#define XOpManager (*gFrameworkData.pOperateManager)
#define XCurSndCore (*gFrameworkData.pSoundCore)
#define XWindow (*gFrameworkData.pWindowCore)
#define XSoundPlayer (*gFrameworkData.pSoundPlayer)
#define XMusicPlayer (*gFrameworkData.pMusicPlayer)
#define X3DWld (*gFrameworkData.p3DWorld)
#define pXLicense (gFrameworkData.pLicense)
enum XColorStyle
{
	COLOR_STYLE_NORMAL,
	COLOR_STYLE_LIGHT,
};
class XSoftBoard;
class XPressDataDouble;
class XCheatModule;
class XConfigManager;
class XWindowTitle;
struct XInputEvent;
class XSimpleTimer;
class XCaregiver;
class XBabyBase;
class XPBO;
class XProjectionCorrection;
class XMaskCorrection;
inline void XEE_Error(const char *p){ REPORT_ERROR(p); }
//������Ƶ�Ŀ���Ǽ��ٵ���ʵ����������ֻ������һ������ʵ�������������е���ʵ������Ϊ���ʵ�����ڲ���Ա
//��һ���Ż�Ŀǰ��δ���У���Ϊ���ںܶ���Ҫ���ǵ�����
enum XCorrectionStage
{
	CSTAGE_NULL,
	CSTAGE_PROJECTION,
	CSTAGE_MASK,
};
class XFrameWork
{
public:
	XFrameWork();
	virtual ~XFrameWork();
protected:
	XFrameWork(const XFrameWork&);
	XFrameWork &operator= (const XFrameWork&);
private:
	float m_rightBtnDownTimer;
	bool m_isInited;
	bool m_withVirtualMouse;	//�Ƿ�ʹ���������
	bool m_isPassKeyLicense;	//�Ƿ�ͨ����֤
	XPressDataDouble *m_moveX;
	XPressDataDouble *m_moveY;
	XProjectionCorrection *m_projCorrection;
	XMaskCorrection *m_maskCorrection;
	XCorrectionStage m_corStage;
#if LIMITED_VERSION == 1
	float m_limitedTimer;
#endif
	XCaregiver *m_pCaregiver;	//������
public:
	bool addABaby(XBabyBase *baby, int resetTimer = 1000);
	static bool cmdProc(void *, const char *, std::string &);
	//���е�ȫ�ֱ�����ȫ�ֺ���ȫ����Ҫ���ܵ����������
	XCheatModule *m_cheatModule;	//���������еĳ�Ա
	XSoftBoard *m_softBoard;		//������̵ĳ�Ա
	bool initCommon();		//��ʼ��һЩϵͳ����Դ��������ʼ����
	void updateCommon(float stepTime);
	bool initCommonAfterSet();	//���û�����֮�������Ӧ�ĳ�ʼ��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������������⿪�ŵ����
//private:
//	XConfigManager *m_pConfigMng;
//public:
//	XConfigManager &getConfigManager(){return *m_pConfigMng;}
	//--------------------------------------------------------------
	XWindowInfo m_windowData;		//���ڵ��������

	//����������ʽ��֡�ʿ��ƣ�Ч���ǿ��ƵıȽ�׼ȷ��ȱ���ǿ���ʱCPUռ���ʽϸ�
	//XSimpleTimer m_fpsTimer;
private:
	bool m_isMouseLBDown;	//�Ƿ��������
	bool m_isMouseRBDown;	//�Ƿ��Ҽ�����
public:
	bool getIsMouseLBDown()const { return m_isMouseLBDown; }
	bool getIsLBDrag(const XInputEvent &event)const;
	bool getIsMouseRBDown()const { return m_isMouseRBDown; }
	bool getIsRBDrag(const XInputEvent &event)const;
	//������engine��һЩ����
	void setForceToPaint(bool flag) { m_forceToPaint = flag; }
	bool setIsForceToPaint()const { return m_forceToPaint; }
private:
	HWND m_hwnd;			//���ڵľ��
	HDC m_wHDC;				//����������ָ��
	HGLRC m_wCurHGLRC;		//��ǰ��������
	HGLRC m_wCopyHGLRC;		//��ǰ�����ĵĸ���
	bool m_forceToPaint;	//������û�false�򴰿���С��֮����������ڻ��ƣ�����ǿ�ƻ���
	bool m_isGameExit;		//�Ƿ�׼���˳�����
	int m_windowWidth;		//��ǰ���ڵĿ��
	int m_windowHeight;		//��ǰ���ڵĸ߶�
	int m_scenePosX;			//������λ��	//Ŀǰ��������в��������ںܶ�ģʽ�²�û��Ч��
	int m_scenePosY;			//������λ��
	int m_sceneWidth;		//�����Ŀ��
	int m_sceneHeight;		//�����ĸ߶�
	float m_sceneScaleX;		//���ڵ����ű���
	float m_sceneScaleY;		//���ڵ����ű���
	float m_viewAngle3D;		//3D�۲�ĽǶ�
	XVec2 m_mousePosition;	//��ǰ�����λ�ã�����������
	XVec2 m_touchPosition;	//��ǰ�����ĵ��λ�ã�����������
	XVec2 m_upMousePosition;	//��һ֡�����λ��
	std::string m_defWorkPath;	//Ĭ�ϵĹ���·�����еĲ�����޸Ĺ���·���������·���Ĳ�������ɴ�������������Ҫ������������
	//+++++��Ƶ����+++++
	int m_audioSampleRate;	//��Ƶ��ʼ����Ƶ��
	int m_audioFormat;		//��Ƶ�ĸ�ʽ
	int m_audioChannel;	//��Ƶ������
	//-----��Ƶ����-----
	unsigned int m_mainThreadHandle;	//���̵߳�ID

	bool m_isFrameTimerEscape;	//ʱ��֡�Ƿ���͵��
public:
	bool getFrameTimerEscape()const { return m_isFrameTimerEscape; }
	//����֡ʱ��͵�ܣ����⻭����Ծ
	void setFrameTimerEscape() { m_isFrameTimerEscape = true; }
private:
	float m_frameTime;	//��һ֡��ʱ��,��λΪ����
	float m_curFPS;	//��ǰ���߼�֡��
	float m_maxFrameStep;	//֡ʱ���
	int m_rendDelay;	//��ʾ��ˢ��ʱ�䣬��λΪms��Ĭ��Ϊ33��������һ�Σ�̫��Ļ���ˢ���ٶ�ʵ����û�����壬�߼�֡�ʿ��������
	XBool m_stopFlag;	//�Ƿ���ͣ
	int m_manualFPS;	//-1�ǲ�����֡��
	float m_manualFrameTime;	//���õ�ÿ֡��ʱ��

	XResPos m_defResPos;	//Ĭ����Դ��λ��

	XBool m_isOpenGL2Supported;	//OpenGL�Ƿ�֧��2.0�Ƿ�֧��GLSL
	XBool m_isLineSmooth;		//�Ƿ�ʹ�û��ߵ�ƽ������
	XBool m_isMultiSampleSupported;	//�Ƿ�֧�ֶ��ز���
	XBool m_isFboSupported;	//�Ƿ�֧��FBO
	int m_maxTextureSize;		//�Կ�֧�ֵ������ͼ�ߴ�
	int m_maxTextureSum;	//һ����ͬʱʹ�õ���ͼ������
	int m_multiSampleSum;	//���ز���������

	bool m_isProjectAsDaemon;	//�Ƿ������Ϊ��̨��������

	char m_autoShutDownState;			//�Զ��ػ���״̬��ֻ�п�Խ�趨ʱ��Żᴥ���Զ��ػ�
	XSimpleTimer m_autoShutDownTimer;			//�Զ��ػ��ļ�ʱ
	float m_showVersionTimer;			//��ʾ�汾�ŵļ�ʱ��
	XFontUnicode *m_showVersionFont;	//��ʾ�汾�ŵ��ַ���
public:
	//�����ǹ����Զ����������֧��
	XWindowTitle *m_customWinTitle;	//�Զ���Ĵ��ڱ���
	XFontUnicode *m_systemFont;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ���������֧��
//	bool drawFont(float x, float y, const char *str);
	bool drawFont(const XVec2& pos, const char *str);
	bool drawFont(const XVec2& pos, const char *str, const XFColor&c);
private:
	XMoveData m_fontMD;
	std::string m_topMsgStr;
public:
	void setTopMsgStr(const std::string &str,bool withLog = true);
	//----------------------------------------------------------------
	float m_errorExitTimer;	//���ʹ���ʱ�˳��ļ�ʱ
	XBool m_haveSystemFont;

	bool m_deamonWithConsole;	//���̲˵����Ƿ��п���̨
	NOTIFYICONDATA m_nid;			//���رտ���̨ʱ���������˵�û�������ر�
	LRESULT(*m_customWinProc)(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end);	//�Զ����ϵͳ��Ϣ������
	LRESULT daemonProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, bool &end);

	XFColor m_defBGColor;	//Ĭ�ϵı�����ɫ
	std::string m_programPropName;	//�������Ե�����
	HANDLE m_programPropValue;		//�������Ե�ֵ
	XBool m_isSuspend;	//�����Ƿ�����жϣ���������жϵĻ�Ҫ�˳�loading�Ķ��߳�
private:
	bool m_textureNeedFit2N;	//��ͼ�Ƿ������Ӧ2��n�η��ĳߴ�(Ŀǰ������ܲ������ƣ���Ҫ��һ��ȫ���Ͻ��߼�)
	XCritical m_hdcMutex;
public:
	void hdcLock() { m_hdcMutex.Lock(); }
	void hdcUnlock() { m_hdcMutex.Unlock(); }
	void setTexNeedFit2N(bool flag) { m_textureNeedFit2N = flag; }		//(Ŀǰ������ܲ������ƣ���Ҫ��һ��ȫ���Ͻ��߼�)
	bool getTexNeedFit2N()const { return m_textureNeedFit2N; }			//(Ŀǰ������ܲ������ƣ���Ҫ��һ��ȫ���Ͻ��߼�)
	HDC getHDC()const { return m_wHDC; }
	HGLRC getCopyHGLRC()const { return m_wCopyHGLRC; }
	HWND getHWND()const { return m_hwnd; }
	bool getIsGameExit()const { return m_isGameExit; }
	void setGameExit() { m_isGameExit = true; }		//������Ϸ�˳�
	float getViewAngle()const { return m_viewAngle3D; }
	void setViewAngle(float angle) { m_viewAngle3D = angle; }
	int getWinW()const { return m_windowWidth; }
	int getWinH()const { return m_windowHeight; }
	int getScenePosX()const { return m_scenePosX; }
	int getScenePosY()const { return m_scenePosY; }
	int getSceneW()const { return m_sceneWidth; }
	int getSceneH()const { return m_sceneHeight; }
	XVec2 getWindowSize()const { return XVec2(m_windowWidth, m_windowHeight); }
	XVec2 getSceneSize()const { return XVec2(m_sceneWidth, m_sceneHeight); }
	XVec2 getSceneCenter()const { return XVec2(m_sceneWidth, m_sceneHeight) * 0.5f; }
	float getLastFrameTime()const { return m_frameTime; }	//������һ֡ʹ�õ�ʱ�䵥λms
	float getElapsedTime()const { return XTime::getCurrentTicks(); }	//���س������е�ʱ�䣬��λΪms
	float getCurFps()const { return m_curFPS; }
	XResPos getDefResPos()const { return m_defResPos; }
	XBool getIsFBOSupported()const { return m_isFboSupported; }		//�жϵײ��Ƿ�֧��FBO
	XBool getIsMultiSampleSupported()const { return m_isMultiSampleSupported; }		//�Ƿ�֧�ֶ��ز���
	XBool getIsLineSmooth()const { return m_isLineSmooth; }		//�Ƿ�ʹ������ƽ��
	int getMaxTextureSum()const { return m_maxTextureSum; }		//�������֧�ֵ���ͼ����
	int getMaxTextureSize()const { return m_maxTextureSize; }		//������֧�ֵ���ͼ���ߴ�
	int getMultiSampleSum()const { return m_multiSampleSum; }
	XVec2 getMousePos()const { return m_mousePosition; }		//��ȡ��ǰ����λ��
	XVec2 getTouchPos()const { return m_touchPosition; }		//��ȡ��ǰ����λ��
	XVec2 getUpMousePos()const { return m_upMousePosition; }		//��ȡ��ǰ����λ��

	int getAudioSampleRate()const { return m_audioSampleRate; }	//��ȡ��Ƶ������
	int getAudioChannelSum()const { return m_audioChannel; }		//��ȡ��Ƶͨ����
	int getAudioFormat()const { return m_audioFormat; }			//��ȡĬ����Ƶ��ʽ
	void setAudioSampleRate(int r) { m_audioSampleRate = r; }	//������Ƶ������
	void setAudioChannelSum(int c) { m_audioChannel = c; }		//������Ƶͨ����
	void setAudioFormat(int f) { m_audioFormat = f; }			//����Ĭ����Ƶ��ʽ

	void setStop(int temp = -1);		//ֹͣ 0,play,1,stop,-1,change state
	XBool getStopState()const { return m_stopFlag; }
	//������Ϸ���߼�֡�ʣ�ʵ������ʾ֡���Ѿ������ƣ�ֻ���߼�֡���ǿ��ŵģ������Ҫ�����߼�֡�ʵĻ�����ͨ���������
	void setFPS(int temp);				//������Ϸ֡�ʣ�С��0Ϊ������֡��(Ŀǰ�����������û����Ӧ�е�����)
	void setRendFPS(int tmp);			//����ͼ�����ȾƵ�ʣ�������������ڳ�ʼ�������е��ò�����Ч
	void setBGColor(const XFColor& c);	//���ñ�������ɫ
	void setStyle(XColorStyle style);	//ѡ����ɫ����ɫ����
	unsigned int getMainThreadID()const { return m_mainThreadHandle; }	//��ȡ���̵߳�ID

	void releaseIcon();	//�ͷ�icon��Դ
	void setProjectAsDaemon() { m_isProjectAsDaemon = true; }		//������ĿΪ��̨��Ŀ
	bool getIsProjectAsDaemon()const { return m_isProjectAsDaemon; }	//�ж���Ŀ�Ƿ�Ϊ��̨����
	XBool setSystemFont(const char * filename);
private:
	void gameProc(void *game);	//��Ϸ��������
public:
	XBool readSystemConfigData(XWindowInfo &data,bool forceTitle = false);			//������Ĭ�ϵ������ļ��ж�ȡ��������
	XBool saveSystemConfigData(const XWindowInfo &data);	//������Ĭ�ϵ��������ݱ��浽�����ļ�
	const char *getDefWorkPath()const { return m_defWorkPath.c_str(); }
private:
	bool createWindow(int width, int height, const char *windowTitle = NULL,
		bool isFullScreen = false, bool withFrame = true);	//��ʼ��SDL����
	XBool initWindow();
	XBool initWindowEx(const XWindowInfo& wData);	//����ӿ��Ѿ�����
	XBool initWindowEx(void);	//����ӿ��Ѿ�����
	XBool initGL();				//��ʼ��OpenGL������
	void initGlew();
	void initWindowMatrix();	//��ʼ�����ڵľ���
	XBool initOpenGL3D();	//3D���ڵĳ�ʼ��һЩopenGL����
	XBool initOpenGL2D();	//�����������ת�Ĵ��ڵĳ�ʼ��һЩopenGL����

	void engineMove(float steptime);	//������ڲ�����
	XBool engineInit();		//���ڽ���֮ǰ�ĳ�ʼ��
	void engineSysInit();	//���ڽ���֮��ĳ�ʼ��

	void engineDraw();	//���������صĶ���
	XBool m_needReset3DParam;	//�Ƿ���Ҫ�ظ�3D����������
public:
	void end2DDraw();	//����2D���ʱ�ظ�����״̬
	XBool getWindowNeedDraw();	//��ȡ��ǰ�����Ƿ���Ҫˢ��
	XVec2 getMousePosFromWindow(int mouseX, int mouseY);
private:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǿ�ܵ���Ҫ�ӿ�
	int suspendKeyProc();	//�ж�֮��������¼�����

	XSimpleTimer *m_periodicLogTimer;	//�����Ե���־��ʱ��
	int m_curframeCounter;	//��ǰ����֡��
	XEE::XMemUsageInfo m_curMemInfo;	//��ǰ���ڴ�����
	bool singletonProc();	//�����жϵ�ǰ�����Ƿ���Ҫ��Ϊ����ģʽ���˳���
public:
	//���Ǵ�������Ƶ�ǰ��������������Ѿ���XErrorReporter���滻����ϸ�鿴XErrorReporter�Ĵ����˵��
	void suspend(const char * str);	//�жϲ���ʾ

	float engineIdle();		//�������ѭ����������ѭ���¼�����,�������ε���֮��ĺ����ʱ
	void engineRelease();	//�������Դ�ͷ�
	void engineKeyProc(int key, XKeyState keyState);
	void engineMouseProc(int x, int y, XMouseState mouseState);

	void(*m_drawAfterCtrl)();	//�����ؼ�֮�����涯���Ļص�����
	void begin2DDraw();			//��ʼ���2DԪ��ʱ�ظ�����״̬(��3D���͵���Ŀ�в���Ҫ�������)
	//withEngin�Ƿ����������ڲ�������
	void updateScreen(bool withEngin = true);		//������Ļ������
	void *m_pGame;				//����Ϸ��ָ��
	void restartGame();			//�������е�ǰ����
	void clearScreen();			//�����Ļ������
	void clearScreen(const XFColor& c);
	//void startGame(void *game,const XWindowInfo* windowInfo = NULL);	//���Ƽ�ʹ��
	template<class Type> void runGame(const XWindowInfo* windowInfo = NULL)
	{
		//�ȶ�ȡ�����ļ�
		if (windowInfo == NULL)
		{
			if (!readSystemConfigData(m_windowData,true)) LogStr("��ȡ�����ļ�ʧ��!");
		}
		else
		{
			m_windowData = *windowInfo;
		}
		if (m_windowData.withException)
		{//�����쳣
			__try {
				XException::initException();
				LogStr("����ʼ");
				if (!initWindow())
					XEE_Error("��������ʧ��");
				if (!singletonProc())
				{
					Type *tmpGame = XMem::createMem<Type>();
					if (tmpGame == NULL) XEE_Error("��Ϸ����ʧ��");
					gameProc(tmpGame);
					XMem::XDELETE(tmpGame);
				}
				else
				{
					LogStr("�����Ѿ������������ظ�����!");
				}
				engineRelease();
			}
			__except (XException::unhandledExceptionFilter(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
			{//�����ǹ����쳣�Ĵ���
				//XException::unhandledExceptionFilter(GetExceptionInformation());
				if (!m_isInited)
				{//���û�г�ʼ����������ֱ�ӽ����߼�����������
					LogStr("��ܳ�ʼ��ʧ��!");
#if WITH_LOG
					LogRelease();	//������־�ļ���������
#endif
				//	Sleep(5000);	//�ȴ�10s�ӣ�Ȼ����������
				//	if(!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//������������
					XEG.setGameExit();				//��ֹ�����˳�ʱ�ٴβ����쳣
					exit(1);						//�˳���ǰ����
				}
				else
				{//������ʾ�����Ϣ
					m_errorExitTimer = 0.0f;
					while (!inputEvent(false))
					{//������ѭ��
						errorLoop();
					}
				}
			}
		}
		else
		{
			LogStr("����ʼ");
			if (!initWindow())
				XEE_Error("��������ʧ��");
			if (!singletonProc())
			{
				Type *tmpGame = XMem::createMem<Type>();
				if (tmpGame == NULL) XEE_Error("��Ϸ����ʧ��");
				gameProc(tmpGame);
				XMem::XDELETE(tmpGame);
			}
			else
			{
				LogStr("�����Ѿ������������ظ�����!");
			}
			engineRelease();
		}
	}
	void touchProc(const TOUCHINPUT& ti);
private:
#if CREATE_WINDOW_METHOD == 0 || CREATE_WINDOW_METHOD == 2
	//send2Game�Ƿ������¼����ݸ���Ϸ
	bool inputEvent(bool send2Game = true);
#endif
#if CREATE_WINDOW_METHOD == 1
	static void glutIdle();
	static void glutDraw();
	void inputProc(const XInputEvent &input);
	bool inputEvent(bool send2Game) { return false; }
	static void glutInput(unsigned char c, int x, int y);
	static void glutsInput(int k, int x, int y);
	static void glutMouseMove(int x, int y);
	static void glutMouseProc(int b, int s, int x, int y);
#endif
	bool m_isImmState;
public:
	bool inputEvent(const XInputEvent &tmpEvent, bool send2Game = true);
	void miniInputEvent(const XInputEvent &tmpEvent);
private:
	//Ϊ�˷���ͳһSDL��Glut�ķ�װ�����ﶨ�弸���ӿ�
	void mainLoop2D();	//2D��Ϸ��ѭ��
	void mainLoop3D();	//3D��Ϸ��ѭ��
	void errorLoop();	//���ʹ����ѭ��
	XSimpleTimer m_drawTimer;	//��ʾ�ļ�ʱ������ʾ̫��ʵ����û�����壬�߼��߳̿����ܺܿ죬������ʾ�߳�û�б�Ҫ�ܿ�
	//--------------------------------------------------------------
public:
	//������صĲ���
	//���ô��ڵ� ͸����
	void setWindowAlpha(float a);
	void getWindowPos(int &x, int &y);
	XVec2 getWindowPos();
	XVec2 getClientPos(const XVec2& screenPos);
	void getWindowSize(int &w, int &h);
	void minSizeWindow() { ShowWindow(m_hwnd, SW_MINIMIZE); }
	void setWindowSize(float x, float y);
	void setWindowPos(int x, int y, bool always = false);	//���ô��ڵ�λ��
	void setWindowHide() { ShowWindow(m_hwnd, SW_HIDE); }	//���ش���
	void setWindowShow() { ShowWindow(m_hwnd, SW_NORMAL); }	//��ʾ����
#ifdef XEE_OS_WINDOWS
	//�ú������ڸ���������һ�����ԣ���ͨ�����������жϸó����Ƿ��Ѿ����У��Ӷ����ֳ������е�Ψһ��
	bool isProgramExist(const std::string& name);
#endif
	//++++++++++++++++++++++++++++++++++++
	//���ں�ȫ���л���һ������
	bool m_isSetToFullScreen;	//�Ƿ�����Ϊȫ��ģʽ
	void toggleFullScreen();	//Ŀǰ��������в��ȶ�(beta)
	//------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�һЩϵͳ���ݵ�ͳ��
	//1������ͼ����
	//2������ͼ���
	//3������ͼռ�õ��Դ��С
	//4��ÿ��ˢ�µ���ͼ���
	//5���ڴ�ʹ�ô���
#if WITH_ENGINE_STATISTICS
private:
	bool m_withEngineStatistics;	//�Ƿ������漶���ͳ��
	int m_totalTextureSum;			//�ܵ���ͼ����
	int m_totalTextureArea;		//�ܵ���ͼ���
	int m_totalVideoMemUsage;		//�ܵ��Դ�ʹ����
	int m_totalFrameTextureArea;	//ÿ֡ˢ����ͼ�����(��δʵ��)
	float m_curVideoMemBandwidthUsage;	//��ǰ�ڴ�����ʹ��(��δʵ��)
public:
	void enableEngineStatistics() { m_withEngineStatistics = true; }
	void addStaticsticsTexInfo(unsigned int id, int level = 0, int type = GL_TEXTURE_2D);
	void decStaticsticsTexInfo(unsigned int id, int level = 0, int type = GL_TEXTURE_2D);
	void logEngineStatistics();			//������ͳ�������������־
#endif
	//--------------------------------------------------------------
	//withThread���Ƿ�ʹ�ö��߳�
	//����Ļ�ӵ�x��y��ʼ��w��h�����ڵ�ͼ�񱣴浽png�ļ�
	XBool gameShot(const char *fileName, int w, int h, bool withThread = true, float x = 0.0f, float y = 0.0f);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǹ���͸�����ڵĶ���
#if BACKGROUND_ALPHA_MODE == 1
private:
	bool m_isBGAlphaInited;
	RECT m_rc;
	POINT m_ptDest;
	SIZE m_sizeDest;
	HDC m_hMemDc;
	BITMAPINFO m_bmpinfo;
	void* m_pBits;
	HBITMAP m_hBitmap;
	HGDIOBJ m_oldGDIObj;
	BLENDFUNCTION m_blf;
	POINT m_ptSrc;
	XPBO *m_bgAlphaPbo;
	void initBgAlphaData();		//��ʼ��͸��������Ҫ����Դ
	void releaseBhAlphaData();	//�ͷ�͸����������Դ
#endif
	//--------------------------------------------------------------
	//++++++++
private:
	XInputEventBuff m_inputEventBuff;
public:
	template <class T>
	bool addEventProc(T *pClass, void(T::*fun)(const XInputEvent &e))
	{
		return m_inputEventBuff.addEventProc<T>(pClass, fun);
	}
	template <class T>
	bool desEventProc(T *pClass, void(T::*fun)(const XInputEvent &e))
	{
		return m_inputEventBuff.desEventProc(pClass, fun);
	}
public:
	//--------
};
//����ʹһЩȫ�ֵķ����Ķ���
#if WITH_LOG
template<class Type> void runGame(const XWindowInfo* windowInfo = NULL,
	const std::string& title = "",
	bool isIniLog = false,XLogBookMode mode = LOGBOOK_MODE_SIMPLE,
	const std::string& serverFilename = "../LogServer/LogServer.exe")
#else
template<class Type> void runGame(const XWindowInfo* windowInfo = NULL,
	const std::string& title = "")
#endif
{
#if WITH_LOG
	if (!gFrameworkData.create(isIniLog, mode, serverFilename)) return;
#else
	if (!gFrameworkData.create()) return;
#endif
	XEG.m_windowData.windowTitle = title;
	XEG.runGame<Type>(windowInfo);
	gFrameworkData.release();
}
inline std::string getCommonResPos()
{
	if(gFrameworkData.pFrameWork == NULL) return XString::gNullStr;
	else return gFrameworkData.pFrameWork->m_windowData.commonResourcePos;
}
inline XResPos getDefResPos()
{
	if(gFrameworkData.pFrameWork == NULL) return RES_AUTO;
	else return gFrameworkData.pFrameWork->getDefResPos();
}
inline int getWindowWidth()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getWinW();
}
inline int getWindowHeight()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getWinH();
}
inline XVec2 getWindowSize()
{
	if (gFrameworkData.pFrameWork == NULL) return XVec2::zero;
	else return gFrameworkData.pFrameWork->getWindowSize();
}
inline int getSceneWidth()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getSceneW();
}
inline int getSceneHeight()
{
	if(gFrameworkData.pFrameWork == NULL) return 0;
	else return gFrameworkData.pFrameWork->getSceneH();
}
extern const XFontUnicode& getDefaultFont();
inline const XVec2 getMousePos()
{
	if(gFrameworkData.pFrameWork == NULL) return XVec2::zero;
	else return gFrameworkData.pFrameWork->getMousePos();
}
//�ƶ���ʽ��ͼƬ�ļ��Ƿ�֧��
extern bool getIsImageSupport(const std::string& ext);
extern bool getIsSoundSupport(const std::string& ext);
#if WITH_INLINE_FILE
#include "XFrameWork.inl"
#endif
}

#endif