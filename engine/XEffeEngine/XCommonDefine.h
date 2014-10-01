#ifndef _JIA_XCOMMONDEFINE_
#define _JIA_XCOMMONDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//���й����ĺ꿪�ض������ڴ˴����Ա���ͳһ�޸�
#include <string>

#define CREATE_WINDOW_WITH_SDL		//�Ƿ�ʹ��SDL��������(�Ѿ�������������)
#ifndef CREATE_WINDOW_WITH_SDL
#define CREATE_WINDOW_WITH_GLUT		//�Ƿ�ʹ��GLUT��������(��δ������������)
#endif

#define WITH_OBJECT_MANAGER (0)			//�Ƿ�����ʹ���������
#define WITH_FULL_ALL_CHINESE (0)		//�Ƿ�֧��ȫ����
#define WITH_COMMON_RESOURCE_FOLDER (1)	//�Ƿ�ʹ�ù����ļ���
#define WITH_XSPRITE_EX (1)				//�Ƿ�ʹ�ü�ǿ���sprite�������ϵ���Ƿ�ʹ�ù�һ������������
#define BASE_RESOURCE_PATH ("ResourcePack/")	//Ĭ�Ϸ����Դ�ļ��е�·������������ļ����µ���Դ�Żᱻ���
#define WITH_64BIT_SUPPLY (0)	//�Ƿ�֧��64λ�ĺ���
//#define WITH_XEE_CURSOR		//�Ƿ�ʹ���Լ��Ĺ�꣬���ȡ���������ֱ��ʹ��windows�Ĺ��

#define WITH_LOG (1)
//#define DEBUG_MODE		//������Ҫ��������ʽ�汾��ͬ��ģ�鶼��Ҫ������ͷ�ļ�
//#define GET_ALL_PIXEL		//ͳ���������ͼ��������ڱ����Ϸ����ͼЧ��

#define XEE_SYSTEM_CONFIG ("XEEConfig.xml")	//Ĭ�����������ļ�

#define MAX_FILE_NAME_LENGTH (256)	//�ļ����ĳ���

//�����Ƕ���Դλ�õĶ���
enum _XResourcePosition
{
	RESOURCE_LOCAL_FOLDER = 0,	//���ض�Ӧ���ļ���
	RESOURCE_LOCAL_PACK,		//����ѹ����
	RESOURCE_WEB,				//������Դ
	RESOURCE_SYSTEM_DEFINE,		//���ϵͳ����
};

enum _XWindowSizeMode		//���ڳߴ����Ӧģʽ
{
	WINDOW_SIZE_MODE_CLIP_LP,		//���Ͻ�Ϊ��׼��ü�	pos,���pos����ֵ����ʵ����һ��Ϊ��׼��ü�
	WINDOW_SIZE_MODE_CLIP_MID,		//�е�Ϊ��׼��ü�		pos
	WINDOW_SIZE_MODE_CLIP_RESIZE,	//�̱����쳤�߲ü�		pos
	WINDOW_SIZE_MODE_RESIZE_CLIP,	//��������̱���հ�	pos
	WINDOW_SIZE_MODE_RESIZE,		//��������				pos
};
enum _XWindowRotateMode		//���ڵ���תģʽ
{
	WINDOW_ROTATE_MODE_0,			//0����ת	
	WINDOW_ROTATE_MODE_90,			//90����ת		
	WINDOW_ROTATE_MODE_180,			//180����ת		
	WINDOW_ROTATE_MODE_270,			//270����ת	
};
enum _XWindowTurnOverMode	//���ڵķ�תģʽ
{
	WINDOW_TURNOVER_MODE_NULL,		//����ת
	WINDOW_TURNOVER_MODE_LTR,		//���ҷ�ת
	WINDOW_TURNOVER_MODE_TTD,		//���·�ת
	WINDOW_TURNOVER_MODE_LTR_TTD,	//�������ҷ�ת
};
enum _XWindowType
{
	WINDOW_TYPE_NORMAL,			//��ͨ���͵Ĵ���
	WINDOW_TYPE_EX,				//ǿ����Ĵ�������
	WINDOW_TYPE_3D,				//3D��������
};
enum _XColorMode		//��ɫģʽ
{
	COLOR_RGBA,
	COLOR_RGB,
	COLOR_GRAY,
	COLOR_RGBA_F,
	COLOR_GRAY_F,
	COLOR_BGRA,
	COLOR_BGR,
};
struct _XWindowData
{
	int w;				//���ڵĿ��pixel
	int h;				//���ڵĸ߶�pixel
	std::string windowTitle;	//���ڵı���
	int sceneW;			//�����Ŀ��pixel
	int sceneH;			//�����ĸ߶�pixel
	int isFullScreen;	//�Ƿ�ȫ��
	int isTouch;		//�Ƿ�ʹ�ô����� �����������������ԭ������ȫ��ʱ���ع��������ʹ�ô�����ʱSDL�����⡣(Ŀǰ��ʹ��SDL�����ع��Ľӿڣ���������Ѿ����)
	int withLog;		//�Ƿ�������־
	int isShowCursor;	//�Ƿ���ʾ���
	int positionX;		//����ԭ������ڳ���������X
	int positionY;		//����ԭ������ڳ���������Y
	int withFrame;		//�Ƿ�ӵ�б߿�
	_XWindowType windowType;	//��������
	_XWindowSizeMode mode;
	int CFGMode;		//������Ĺ���ģʽ
	bool isStartDelay;	//�Ƿ��������ӳ�
	int startDelayTimer;	//�����ӳٵ�ʱ�䣬��λms
	bool isShowVersion;		//�Ƿ���ʾ�汾��
	std::string versionString;	//�汾���ַ���
	bool isAutoShutDown;	//�Ƿ��Զ��ػ�
	int autoShutDownYear;	//�Զ��ػ���ʱ��
	int autoShutDownMonth;	
	int autoShutDownDay;	
	int autoShutDownHour;	
	int autoShutDownMinute;	
	int autoShutDownSecond;	
	bool isShowConsole;		//�Ƿ���ʾ����̨
	int BGVolume;	//��������
	int FWVolume;	//ǰ������
	_XWindowRotateMode rotateMode;		//��ת��ģʽ
	_XWindowTurnOverMode turnOverMode;	//��תģʽ
	_XResourcePosition defResourcePos;	//Ĭ�ϵ���Դλ��
	std::string systemFontFile;	//ϵͳ�����ļ�·��
	std::string commonResourcePos;	//������Դ�ļ���λ��
	//���������ô��ڵ�λ�õĲ���
	bool isInitWindowPos;	//�Ƿ����ô��ڵ�λ��
	int windowPosX;			//���ڵ�λ��
	int windowPosY;
	bool isAlwaysTop;		//�Ƿ����ö�
	bool withCustomTitle;	//�Ƿ�ʹ���Լ��ı���������δ�����ù��������
	_XWindowData()
		:w(800)
		,h(600)
		,sceneW(800)
		,sceneH(600)
		,isFullScreen(0)
		,isTouch(0)
		,withLog(1)
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
		,defResourcePos(RESOURCE_LOCAL_FOLDER)
		,BGVolume(100)	//��������
		,FWVolume(100)	//ǰ������
		,isInitWindowPos(false)
		,windowPosX(0)
		,windowPosY(0)
		,isAlwaysTop(false)
		,withCustomTitle(false)
	{
		windowTitle = "xiajia_1981@163.com";
		versionString = "1.0.0.0";
#if WITH_COMMON_RESOURCE_FOLDER
		systemFontFile = "../../Common/SIMLI.TTF";
		commonResourcePos = "../../Common/";
#else
		systemFontFile = "SIMLI.TTF";
		commonResourcePos = "./";
#endif
	}
	_XWindowData(int width,int height,_XWindowType type = WINDOW_TYPE_NORMAL)
		:w(width)
		,h(height)
		,sceneW(width)
		,sceneH(height)
		,isFullScreen(0)
		,isTouch(0)
		,withLog(1)
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
		,defResourcePos(RESOURCE_LOCAL_FOLDER)
		,BGVolume(100)	//��������
		,FWVolume(100)	//ǰ������
		,isInitWindowPos(false)
		,windowPosX(0)
		,windowPosY(0)
		,isAlwaysTop(false)
	{
		windowTitle = "xiajia_1981@163.com";
		versionString = "1.0.0.0";
#if WITH_COMMON_RESOURCE_FOLDER
		systemFontFile = "../../Common/SIMLI.TTF";
		commonResourcePos = "../../Common/";
#else
		systemFontFile = "SIMLI.TTF";
		commonResourcePos = "./";
#endif
	}
};

#endif