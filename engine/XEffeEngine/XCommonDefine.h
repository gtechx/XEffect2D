#ifndef _JIA_XCOMMONDEFINE_
#define _JIA_XCOMMONDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//���й����ĺ꿪�ض������ڴ˴����Ա���ͳһ�޸�
//ע�⣺3D��1.0f����1���ף���1mm
#include <string>
namespace XE{
#define CREATE_WINDOW_WITH_SDL		//�Ƿ�ʹ��SDL��������(�Ѿ�������������)
#ifndef CREATE_WINDOW_WITH_SDL
#define CREATE_WINDOW_WITH_GLUT		//�Ƿ�ʹ��GLUT��������(��δ������������)
#endif
#define WITH_INLINE_FILE (1)			//�Ƿ�ʹ��inline�ļ�����ʽ 0����ı����ٶȣ�1���õ�ִ������
#if WITH_INLINE_FILE
#define INLINE inline
#else
#define INLINE 
#endif
#define BACKGROUND_ALPHA_MODE (1)		//����͸����ʵ�ַ�ʽ:0����겻�ܴ�Խ͸������1���ٶȽ�Ϊ��
#define WITH_OBJECT_MANAGER (0)			//�Ƿ�����ʹ���������
#define WITH_FULL_ALL_CHINESE (1)		//�Ƿ�֧��ȫ����
#define WITH_COMMON_RESOURCE_FOLDER (1)	//�Ƿ�ʹ�ù����ļ���
#define WITHXSPRITE_EX (1)				//�Ƿ�ʹ�ü�ǿ���sprite�������ϵ���Ƿ�ʹ�ù�һ������������
#define BASE_RESOURCE_PATH "ResourcePack/"	//Ĭ�Ϸ����Դ�ļ��е�·������������ļ����µ���Դ�Żᱻ���
#define WITH_64BIT_SUPPLY (0)	//�Ƿ�֧��64λ�ĺ���
//#define WITHXEE_CURSOR		//�Ƿ�ʹ���Լ��Ĺ�꣬���ȡ���������ֱ��ʹ��windows�Ĺ��

#define WITH_LOG (1)
#define WITH_ENGINE_STATISTICS	(1)		//�Ƿ����������ڲ�ͳ��
//#define DEBUG_MODE		//������Ҫ��������ʽ�汾��ͬ��ģ�鶼��Ҫ������ͷ�ļ�
//#define GET_ALL_PIXEL		//ͳ���������ͼ��������ڱ����Ϸ����ͼЧ��

#define XEE_SYSTEM_CONFIG "XEEConfig.xml"	//Ĭ�����������ļ�

#define MAX_FILE_NAME_LENGTH (256)	//�ļ����ĳ���

//�����Ƕ���Դλ�õĶ���
enum XResourcePosition
{
	RESOURCE_LOCAL_FOLDER = 0,	//���ض�Ӧ���ļ���
	RESOURCE_LOCAL_PACK,		//����ѹ����
	RESOURCE_WEB,				//������Դ
	RESOURCE_SYSTEM_DEFINE,		//���ϵͳ����
	RESOURCE_AUTO,				//�Զ����룬���ȴ�ѹ������ȡ��ȡ�����Ŵ��ļ����ж�ȡ
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
	COLOR_RGB,
	COLOR_GRAY,
	COLOR_RGBA_F,	//�ڲ���RGBA32F
	COLOR_GRAY_F,	//�ڲ���R32F	//�����Ҫ��֤	GL_LUMINANCE����ֻ��8��bits
	COLOR_BGRA,
	COLOR_BGR,
	COLOR_DEPTH,	//�����ͼ
	COLOR_RED,		//�����ٶȽϿ�
};
struct XWindowInfo
{
	int w;				//���ڵĿ��pixel
	int h;				//���ڵĸ߶�pixel
	std::string windowTitle;	//���ڵı���
	int sceneW;			//�����Ŀ��pixel
	int sceneH;			//�����ĸ߶�pixel
	int isFullScreen;	//�Ƿ�ȫ��
	int isTouch;		//�Ƿ�ʹ�ô����� �����������������ԭ������ȫ��ʱ���ع��������ʹ�ô�����ʱSDL�����⡣(Ŀǰ��ʹ��SDL�����ع��Ľӿڣ���������Ѿ����)
	int withLog;		//��־���ģʽ
	int isShowCursor;	//�Ƿ���ʾ���
	int positionX;		//����ԭ������ڳ���������X
	int positionY;		//����ԭ������ڳ���������Y
	int withFrame;		//�Ƿ�ӵ�б߿�
	XWindowType windowType;	//��������
	XWindowSizeMode mode;
	int CFGMode;		//������Ĺ���ģʽ
	int startDelayTimer;	//�����ӳٵ�ʱ�䣬��λms
	bool isStartDelay;	//�Ƿ��������ӳ�
	bool isShowVersion;		//�Ƿ���ʾ�汾��
	bool isAutoShutDown;	//�Ƿ��Զ��ػ�
	bool isShowConsole;		//�Ƿ���ʾ����̨
	bool isProjectAsDaemon;	//�Ƿ���Ϊ��̨����
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
	XResourcePosition defResourcePos;	//Ĭ�ϵ���Դλ��
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
	bool isSingletonMode;	//�Ƿ�ֻ��һ��ʵ��
	XWindowInfo()
		:w(800)
		,h(600)
		,sceneW(800)
		,sceneH(600)
		,isFullScreen(0)
		,isTouch(0)
		,withLog(3)	//LB_LEVEL_ALL
		,isShowCursor(1)
		,positionX(0)
		,positionY(0)
		,startDelayTimer(0)
		,autoShutDownYear(-1)	//�Զ��ػ���ʱ��
		,autoShutDownMonth(-1)	
		,autoShutDownDay(-1)	
		,autoShutDownHour(-1)	
		,autoShutDownMinute(-1)	
		,autoShutDownSecond(-1)	
		,withFrame(1)
		,mode(WINDOW_SIZE_MODE_CLIP_LP)
		,rotateMode(WINDOW_ROTATE_MODE_0)
		,turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		,windowType(WINDOW_TYPE_NORMAL)
		,CFGMode(0)
		,isStartDelay(false)
		,isShowVersion(false)
		,isAutoShutDown(false)
		,isShowConsole(true)
		,isProjectAsDaemon(false)
		,defResourcePos(RESOURCE_AUTO)
		,BGVolume(100)	//��������
		,FWVolume(100)	//ǰ������
		,isInitWindowPos(false)
		,windowPosX(0)
		,windowPosY(0)
		,isAlwaysTop(false)
		,withCustomTitle(false)
		,windowTitle("xiajia_1981@163.com")
		,versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		,systemFontFile("../../Common/SIMLI.TTF")
		,commonResourcePos("../../Common/")
#else
		,systemFontFile("SIMLI.TTF")
		,commonResourcePos("./")
#endif
		,systemFontSize(30)
		,withAlphaBG(false)
		,logicFps(-1)
		,drawFps(30)
		,withException(true)
		,withLowConsumption(true)	//�Ƿ����ͺ�ģʽ
		,withClearBG(true)			//�Ƿ��������
		,isRestartWhenCrash(false)
		,isSingletonMode(false)
	{}
	XWindowInfo(int width,int height,XWindowType type = WINDOW_TYPE_NORMAL)
		:w(width)
		,h(height)
		,sceneW(width)
		,sceneH(height)
		,isFullScreen(0)
		,isTouch(0)
		,withLog(3)	//LB_LEVEL_ALL
		,isShowCursor(1)
		,positionX(0)
		,positionY(0)
		,startDelayTimer(0)
		,autoShutDownYear(-1)	//�Զ��ػ���ʱ��
		,autoShutDownMonth(-1)	
		,autoShutDownDay(-1)	
		,autoShutDownHour(-1)	
		,autoShutDownMinute(-1)	
		,autoShutDownSecond(-1)	
		,withFrame(1)
		,mode(WINDOW_SIZE_MODE_CLIP_LP)
		,rotateMode(WINDOW_ROTATE_MODE_0)
		,turnOverMode(WINDOW_TURNOVER_MODE_NULL)
		,windowType(type)
		,CFGMode(0)
		,isStartDelay(false)
		,isShowVersion(false)
		,isAutoShutDown(false)
		,isShowConsole(true)
		,isProjectAsDaemon(false)
		,defResourcePos(RESOURCE_AUTO)
		,BGVolume(100)	//��������
		,FWVolume(100)	//ǰ������
		,isInitWindowPos(false)
		,windowPosX(0)
		,windowPosY(0)
		,isAlwaysTop(false)
		,withCustomTitle(false)
		,windowTitle("xiajia_1981@163.com")
		,versionString("1.0.0.0")
#if WITH_COMMON_RESOURCE_FOLDER
		,systemFontFile("../../Common/SIMLI.TTF")
		,commonResourcePos("../../Common/")
#else
		,systemFontFile("SIMLI.TTF")
		,commonResourcePos("./")
#endif
		,systemFontSize(30)
		,withAlphaBG(false)
		,logicFps(-1)
		,drawFps(30)
		,withException(true)
		,withLowConsumption(true)	//�Ƿ����ͺ�ģʽ
		,withClearBG(true)			//�Ƿ��������
		,isRestartWhenCrash(false)
		,isSingletonMode(false)
	{}
};
}
#endif