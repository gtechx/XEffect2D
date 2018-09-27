#ifndef _JIA_XCOMMONDEFINE_
#define _JIA_XCOMMONDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//���й����ĺ꿪�ض������ڴ˴����Ա���ͳһ�޸�
//ע�⣺3D��1.0f����1���ף���1mm
//ע���1��
//�����������ԱȨ��
//��Ŀ����--������--�嵥�ļ�--UACִ�м����ΪrequireAdministrator
#include <string>

namespace XE{
#include "XPersonalizedDefine.h"
#define WITH_VS2015
#ifdef WITH_VS2015
//��vs2015����Ҫ��������Ŀ�,��ʹ�õ��ϵĿ�ʱ��������win10�²��ð������(���)
//#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#ifndef AUDIO_MATHOD 
#define AUDIO_MATHOD (0)				//0��SDL_Mixer,1��Fmod
#endif
#ifndef CREATE_WINDOW_METHOD
#define CREATE_WINDOW_METHOD (0)		//0��SDL��1��GLUT��2��GLFW
#endif

#define WITH_INLINE_FILE (1)			//�Ƿ�ʹ��inline�ļ�����ʽ 0����ı����ٶȣ�1���õ�ִ������
#if WITH_INLINE_FILE
#define INLINE inline
#else
#define INLINE 
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif
#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F        // min normalized positive value
#endif

#ifndef BACKGROUND_ALPHA_MODE
#define BACKGROUND_ALPHA_MODE (1)		//����͸����ʵ�ַ�ʽ:0����겻�ܴ�Խ͸������1���ٶȽ�Ϊ��
#endif
#ifndef WITH_OBJECT_MANAGER
#define WITH_OBJECT_MANAGER (0)			//�Ƿ�����ʹ���������
#endif
#ifndef WITH_FULL_ALL_CHINESE
#define WITH_FULL_ALL_CHINESE (1)		//�Ƿ�֧��ȫ���� 0:�������ģ�1��ȫ���ģ�2��ֻ֧��Ӣ��
#endif
#ifndef WITH_COMMON_RESOURCE_FOLDER
#define WITH_COMMON_RESOURCE_FOLDER (1)	//�Ƿ�ʹ�ù����ļ���
#endif
#ifndef WITH_TOUCH_SUPPLY
#define WITH_TOUCH_SUPPLY (1)
#endif

#define WITHXSPRITE_EX (1)				//�Ƿ�ʹ�ü�ǿ���sprite�������ϵ���Ƿ�ʹ�ù�һ������������
#define BASE_RESOURCE_PATH "ResourcePack/"	//Ĭ�Ϸ����Դ�ļ��е�·������������ļ����µ���Դ�Żᱻ���
#define WITH_64BIT_SUPPLY (0)	//�Ƿ�֧��64λ�ĺ���

#ifndef WITHXEE_CURSOR
//#define WITHXEE_CURSOR		//�Ƿ�ʹ���Լ��Ĺ�꣬���ȡ���������ֱ��ʹ��windows�Ĺ��
#endif
#ifndef WITH_LOG
#define WITH_LOG (1)
#endif
#ifndef WITH_ENGINE_STATISTICS
#define WITH_ENGINE_STATISTICS	(0)		//�Ƿ����������ڲ�ͳ��
#endif

#ifndef LIMITED_VERSION
#define LIMITED_VERSION (1)
#endif

//#define DEBUG_MODE		//������Ҫ��������ʽ�汾��ͬ��ģ�鶼��Ҫ������ͷ�ļ�
//#define GET_ALL_PIXEL		//ͳ���������ͼ��������ڱ����Ϸ����ͼЧ��

#define XEE_SYSTEM_CONFIG "XEEConfig.xml"	//Ĭ�����������ļ�

#define MAX_FILE_NAME_LENGTH (256)	//�ļ����ĳ���
#define STRINGIFY(A) #A
#if 1
//typedef BOOL XBool;
//#define XFalse (FALSE)
//#define XTrue (TRUE)
typedef int XBool;
#define XFalse (0)
#define XTrue (1)
#else
typedef bool XBool;
#define XFalse (false)
#define XTrue (true)
#endif

//�����Ƕ���Դλ�õĶ���
//ԭ��Ϊ��XResPos
enum XResPos
{
	RES_LOCAL_FOLDER = 0,	//���ض�Ӧ���ļ���
	RES_LOCAL_PACK,		//����ѹ����
	RES_WEB,				//������Դ
	RES_SYS_DEF,		//���ϵͳ����
	RES_AUTO,				//�Զ����룬���ȴ�ѹ������ȡ��ȡ�����Ŵ��ļ����ж�ȡ
};
enum XWindowSizeMode		//���ڳߴ����Ӧģʽ
{
	WINDOW_SIZE_MODE_CLIP_LP,		//���Ͻ�Ϊ��׼��ü�	pos,���pos����ֵ����ʵ����һ��Ϊ��׼��ü�
	WINDOW_SIZE_MODE_CLIP_MID,		//�е�Ϊ��׼��ü�		pos
	WINDOW_SIZE_MODE_CLIP_RESIZE,	//�̱����쳤�߲ü�		pos
	WINDOW_SIZE_MODE_RESIZE_CLIP,	//��������̱���հ�	pos
	WINDOW_SIZE_MODE_RESIZE,		//��������				pos
};
enum XWindowRotateMode		//���ڵ���תģʽ
{
	WINDOW_ROTATE_MODE_0,			//0����ת	
	WINDOW_ROTATE_MODE_90,			//90����ת		
	WINDOW_ROTATE_MODE_180,			//180����ת		
	WINDOW_ROTATE_MODE_270,			//270����ת	
};
enum XWindowTurnOverMode	//���ڵķ�תģʽ
{
	WINDOW_TURNOVER_MODE_NULL,		//����ת
	WINDOW_TURNOVER_MODE_LTR,		//���ҷ�ת
	WINDOW_TURNOVER_MODE_TTD,		//���·�ת
	WINDOW_TURNOVER_MODE_LTR_TTD,	//�������ҷ�ת
};
enum XWindowType
{
	WINDOW_TYPE_NORMAL,			//��ͨ���͵Ĵ���
	WINDOW_TYPE_EX,				//ǿ����Ĵ�������
	WINDOW_TYPE_3D,				//3D��������
};
enum XColorMode		//��ɫģʽ
{
	COLOR_RGBA,
	COLOR_RGBA8,
	COLOR_RGBA16,
	COLOR_RGBA16F,
	COLOR_RGBA32F_ARB,

	COLOR_RGB,
	COLOR_RGB8,
	COLOR_RGB16,
	COLOR_RGB16F,
	COLOR_RGB32F_ARB,

	COLOR_GRAY,
	COLOR_GRAY8,
	COLOR_GRAY16,
	COLOR_GRAY32F_ARB,

	COLOR_GRAY_ALPHA,
	COLOR_GRAY8_ALPHA8,
	COLOR_GRAY16_ALPHA16,
	COLOR_GRAY_ALPHA32F_ARB,

	COLOR_DEPTH,	//�����ͼ
	COLOR_DEPTH16,
	COLOR_DEPTH24,
	COLOR_DEPTH32,
	COLOR_DEPTH_STENCIL,

	COLOR_R8,
	COLOR_R16,
	COLOR_R16F,
	COLOR_R32F,

	COLOR_ALPHA,
	COLOR_ALPHA8,

	COLOR_RG8,
	COLOR_RG16,
	COLOR_RG16F,
	COLOR_RG32F,

	COLOR_STENCIL_INDEX,
	//�������Զ���ĸ�ʽ
	COLOR_RED,		//�����ٶȽϿ�
	COLOR_RGBA_F,	//�ڲ���RGBA32F
	COLOR_RGB_F,	//�ڲ���RGB32F
	COLOR_GRAY_F,	//�ڲ���R32F	//�����Ҫ��֤	GL_LUMINANCE����ֻ��8��bits
	COLOR_BGRA,
	COLOR_BGR,
};
struct XWindowInfo
{
	int w;				//���ڵĿ��pixel
	int h;				//���ڵĸ߶�pixel
	std::string windowTitle;	//���ڵı���
	int sceneW;			//�����Ŀ��pixel
	int sceneH;			//�����ĸ߶�pixel
	bool isFullScreen;	//�Ƿ�ȫ��
	bool isTouch;		//�Ƿ�ʹ�ô����� �����������������ԭ������ȫ��ʱ���ع��������ʹ�ô�����ʱSDL�����⡣(Ŀǰ��ʹ��SDL�����ع��Ľӿڣ���������Ѿ����)
	bool isShowCursor;	//�Ƿ���ʾ���
	bool withFrame;		//�Ƿ�ӵ�б߿�
	int withLog;		//��־���ģʽ
	int positionX;		//����ԭ������ڳ���������X
	int positionY;		//����ԭ������ڳ���������Y
	XWindowType windowType;	//��������
	XWindowSizeMode mode;
	int CFGMode;		//������Ĺ���ģʽ
	int startDelayTimer;	//�����ӳٵ�ʱ�䣬��λms
	bool isStartDelay;	//�Ƿ��������ӳ�
	bool isShowVersion;		//�Ƿ���ʾ�汾��
	bool isAutoShutDown;	//�Ƿ��Զ��ػ�
	bool isShowConsole;		//�Ƿ���ʾ����̨
	std::string versionString;	//�汾���ַ���
	int autoShutDownYear;	//�Զ��ػ���ʱ��
	int autoShutDownMonth;
	int autoShutDownDay;
	int autoShutDownHour;
	int autoShutDownMinute;
	int autoShutDownSecond;
	int BGVolume;	//��������
	int FWVolume;	//ǰ������
	XWindowRotateMode rotateMode;		//��ת��ģʽ
	XWindowTurnOverMode turnOverMode;	//��תģʽ
	XResPos defResourcePos;	//Ĭ�ϵ���Դλ��
	std::string systemFontFile;	//ϵͳ�����ļ�·��
	std::string commonResourcePos;	//������Դ�ļ���λ��
	//���������ô��ڵ�λ�õĲ���
	bool isInitWindowPos;	//�Ƿ����ô��ڵ�λ��
	bool isAlwaysTop;		//�Ƿ����ö�
	bool withCustomTitle;	//�Ƿ�ʹ���Լ��ı���������δ�����ù��������
	bool withAlphaBG;		//�Ƿ�ʹ�ñ���͸��
	int windowPosX;			//���ڵ�λ��
	int windowPosY;
	int systemFontSize;
	int logicFps;	//�߼�֡��,<=0Ϊ��������
	int drawFps;	//��ʾ֡��,<=0Ϊ��������
	bool withException;			//�Ƿ����쳣
	bool withLowConsumption;	//�Ƿ����ͺ�ģʽ
	bool withClearBG;			//�Ƿ��������
	bool isRestartWhenCrash;	//����crash֮���Ƿ�������������
	bool isProjectAsDaemon;		//�Ƿ���Ϊ��̨����
	bool isSingletonMode;		//�Ƿ�ֻ��һ��ʵ��
	bool withProjectCorrection;	//�Ƿ�ʹ��ͶӰ����
	bool withMaskCorrection;	//�Ƿ�ʹ��mask����
	std::string maskFilename;	//mask�ļ����ļ���
	bool withCheatModule;		//�Ƿ�֧������������
	bool withSoftBoard;			//�Ƿ�ʹ���������
	bool withCaregiver;			//�Ƿ�ʹ�ÿ�������
	XWindowInfo()
		:w(800)
		, h(600)
		, sceneW(800)
		, sceneH(600)
		, isFullScreen(false)
		, isTouch(false)
		, withLog(3)	//LB_LEVEL_ALL
		, isShowCursor(true)
		, positionX(0)
		, positionY(0)
		, startDelayTimer(0)
		, autoShutDownYear(-1)	//�Զ��ػ���ʱ��
		, autoShutDownMonth(-1)
		, autoShutDownDay(-1)
		, autoShutDownHour(-1)
		, autoShutDownMinute(-1)
		, autoShutDownSecond(-1)
		, withFrame(true)
		, mode(WINDOW_SIZE_MODE_CLIP_LP)
		, rotateMode(WINDOW_ROTATE_MODE_0)
		, turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		, windowType(WINDOW_TYPE_NORMAL)
		, CFGMode(0)
		, isStartDelay(false)
		, isShowVersion(false)
		, isAutoShutDown(false)
		, isShowConsole(true)
		, isProjectAsDaemon(false)
		, defResourcePos(RES_LOCAL_FOLDER)
		, BGVolume(100)	//��������
		, FWVolume(100)	//ǰ������
		, isInitWindowPos(false)
		, windowPosX(0)
		, windowPosY(0)
		, isAlwaysTop(false)
		, withCustomTitle(false)
		, windowTitle("xiajia_1981@163.com")
		, versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		, systemFontFile("../../Common/SIMLI.TTF")
		, commonResourcePos("../../Common/")
#else
		, systemFontFile("SIMLI.TTF")
		, commonResourcePos("./")
#endif
		, systemFontSize(30)
		, withAlphaBG(false)
		, logicFps(-1)
		, drawFps(30)
		, withException(true)
		, withLowConsumption(true)	//�Ƿ����ͺ�ģʽ
		, withClearBG(true)			//�Ƿ��������
		, isRestartWhenCrash(true)
		, isSingletonMode(false)
		, withProjectCorrection(false)
		, withMaskCorrection(false)
		, maskFilename("ResourcePack/normalResource/maskX.png")
		, withCheatModule(false)
		, withSoftBoard(false)
		, withCaregiver(false)
	{}
	XWindowInfo(int width, int height, XWindowType type = WINDOW_TYPE_NORMAL)
		:w(width)
		, h(height)
		, sceneW(width)
		, sceneH(height)
		, isFullScreen(false)
		, isTouch(false)
		, withLog(3)	//LB_LEVEL_ALL
		, isShowCursor(true)
		, positionX(0)
		, positionY(0)
		, startDelayTimer(0)
		, autoShutDownYear(-1)	//�Զ��ػ���ʱ��
		, autoShutDownMonth(-1)
		, autoShutDownDay(-1)
		, autoShutDownHour(-1)
		, autoShutDownMinute(-1)
		, autoShutDownSecond(-1)
		, withFrame(true)
		, mode(WINDOW_SIZE_MODE_CLIP_LP)
		, rotateMode(WINDOW_ROTATE_MODE_0)
		, turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		, windowType(type)
		, CFGMode(0)
		, isStartDelay(false)
		, isShowVersion(false)
		, isAutoShutDown(false)
		, isShowConsole(true)
		, isProjectAsDaemon(false)
		, defResourcePos(RES_LOCAL_FOLDER)
		, BGVolume(100)	//��������
		, FWVolume(100)	//ǰ������
		, isInitWindowPos(false)
		, windowPosX(0)
		, windowPosY(0)
		, isAlwaysTop(false)
		, withCustomTitle(false)
		, windowTitle("xiajia_1981@163.com")
		, versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		, systemFontFile("../../Common/SIMLI.TTF")
		, commonResourcePos("../../Common/")
#else
		, systemFontFile("SIMLI.TTF")
		, commonResourcePos("./")
#endif
		, systemFontSize(30)
		, withAlphaBG(false)
		, logicFps(-1)
		, drawFps(30)
		, withException(true)
		, withLowConsumption(true)	//�Ƿ����ͺ�ģʽ
		, withClearBG(true)			//�Ƿ��������
		, isRestartWhenCrash(true)
		, isSingletonMode(false)
		, withProjectCorrection(false)
		, withMaskCorrection(false)
		, maskFilename("ResourcePack/normalResource/maskX.png")
		, withCheatModule(false)
		, withSoftBoard(false)
		, withCaregiver(false)
	{}
};
}
#endif