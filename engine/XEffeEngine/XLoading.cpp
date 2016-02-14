#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XLoading.h"
#include "XResourcePack.h"
//#include "XLogBook.h"
//#include "XBasicOpenGL.h"

namespace XE{
XBool isLoadingStart = XFalse;
XLoading *xLoading = NULL;
XLoadingScene *xLoadingScene = NULL;

XBool startLoading(XLoadingScene * scene,
		XResourcePosition/*resoursePosition*/)
{
	if(isLoadingStart) return XFalse;
	if(scene == NULL) return XFalse;

	xLoading = XMem::createMem<XLoading>();
	if(xLoading == NULL) return XFalse;
	xLoadingScene = scene;

	xLoading->init(xLoadingScene,RESOURCE_LOCAL_PACK);
	xLoading->setStart();

	isLoadingStart = XTrue;
	return XTrue;
}
XBool endLoading()
{
	if(!isLoadingStart) return XFalse;

	xLoading->setLoadPresent(100.0f);
	xLoading->setEnd();
	XMem::XDELETE(xLoading);
	XMem::XDELETE(xLoadingScene);

	isLoadingStart = XFalse;
	return XTrue;
}
#ifdef XEE_OS_WINDOWS
void XLoading::init(XLoadingScene * scene,XResourcePosition resoursePosition)
{
	m_resoursePosition = resoursePosition;

	m_isShow = XFalse;
	m_loadPresent = 0.0f;
	m_isEnd = STATE_END;
	//m_windowSize = windowSize;
	m_pScene = scene;
}
void XLoading::setStart()
{//����һ���߳�
	m_isShow = XTrue;
	m_isEnd = STATE_BEFORE_START;
	pthread_t moveThreadP;

	m_hDC = wglGetCurrentDC();

	if(CreateThread(0,0,loadingProc,this,0,&moveThreadP) == 0) 
	{
		LogStr("���߳̿���ʧ�ܣ�");
	}
	while(m_isEnd != STATE_START)
	{
		XEE::sleep(10);
	}
}
void XLoading::setEnd()
{
	setLoadPresent(100.0);
	XEE::sleep(100);
	m_isShow = XFalse;
	while(m_isEnd != STATE_END)
	{
		XEE::sleep(10);
	}
	//�������ڶ��̵߳����⣬��Ҫ�ָ�GL��״̬
	XGL::initOpenGLState();
}
DWORD WINAPI XLoading::loadingProc(void * pParam)
{
	XLoading &pPar = *(XLoading *) pParam;

	//pPar.m_hDC = wglGetCurrentDC();
	HGLRC hGlrc = wglCreateContext(pPar.m_hDC);
	wglMakeCurrent(pPar.m_hDC,hGlrc);

	glMatrixMode(GL_PROJECTION);					//���õ�ǰ����ģʽ����ͶӰ����Ӧ��֮��ľ��������
	glLoadIdentity();								//�任����ϵ����
	glOrtho(0,getWindowWidth(),getWindowHeight(), 0, -1, 1);;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(XEG.m_defBGColor.fR,XEG.m_defBGColor.fG,XEG.m_defBGColor.fB,XEG.m_defBGColor.fA);			//�����ɫ
	glDisable(GL_DEPTH);							//2D��Ŀ����Ҫ��Ȳ���

	if(pPar.m_pScene != NULL) pPar.m_pScene->init(pPar.m_resoursePosition);

	LogStr("Enter LoadingProc!");
	pPar.m_isEnd = STATE_START;
	while(!XEG.m_isSuspend)
	{
		//if(XEE::isSuspend) break;
		pPar.move();

		//wglMakeCurrent(pPar.m_hDC,hGlrc);
		//�����Ļ
	//	clearScreen();
		pPar.draw();
		XEG.updateScreen();
		//wglMakeCurrent(NULL,NULL);

		if(!pPar.m_isShow)
		{
	//		clearScreen();
			break;
		}
		if(pPar.m_loadPresent < 100) pPar.m_loadPresent += pPar.m_speed;
		if(pPar.m_pScene != NULL) pPar.m_pScene->setLoadPresent(pPar.m_loadPresent);
		XEE::sleep(20000);
	}
	pPar.release();
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hGlrc);
	LogStr("Loading End!");
	pPar.m_isEnd = STATE_END;
	return 1;
}
#endif

#ifdef XEE_OS_LINUX
void XLoading::init(const XVector2 &windowSize,int resoursePosition)
{
	if(resoursePosition != RESOURCE_OUTSIDE) m_resoursePosition = RESOURCE_INSIDE;
	else m_resoursePosition = RESOURCE_OUTSIDE;

	m_isShow = 0;
	m_loadPresent = 0;
	m_isEnd = 2;
	m_windowSize = windowSize;
	//���������������Դ
	m_loadingBG.init("pic/Loading/LoadingBG.png",NULL,1,m_resoursePosition);		//���ý���ĵ�һ������
	m_loadingBG.setPosition(XVector2::zero);
//	m_loadingBG.setSize(m_windowSize.x/64.0,m_windowSize.y/64.0);
//	m_loadingBG.setIsResizeCenter(POINT_LEFT_TOP);

	Number.init("pic/Number.png",XVector2I(15,30),XVector2I(8,2),m_resoursePosition);
	Number.setPosition(10,10);
	ProgressTexture.init("pic/Progress/PB_00.png","pic/Progress/PM_00.png","pic/Progress/PU_00.png",m_resoursePosition);
	float tempSize = m_windowSize.x/1280.0;
	m_process.init(XRect(m_windowSize.x * 0.5 - 256 * tempSize,m_windowSize.y * 0.5 - 16 * tempSize,
		m_windowSize.x * 0.5 - 256 * tempSize + 512,m_windowSize.y * 0.5 - 16 * tempSize + 32),
		&ProgressTexture,&Number,1.0,XVector2(250,0));
	m_process.setSize(tempSize,tempSize);
	m_loadPresent = 0;
	m_process.setValue(m_loadPresent);
}
void XLoading::setStart()
{//����һ���߳�
	m_isShow = 1;
	m_isEnd = 0;
	pthread_t moveThreadP;
	//record the old context
	m_mainDisplay = glXGetCurrentDisplay();
	m_mainPBuffer = glXGetCurrentDrawable();
	m_mainContext = glXGetCurrentContext();
	//create new context
	int att[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT,GLX_DOUBLEBUFFER, True,GLX_DEPTH_SIZE,24,None};
	//int att[] = {GLX_RENDER_TYPE,GLX_RGBA_BIT,GLX_DRAWABLE_TYPE,GLX_PBUFFER_BIT,GLX_RED_SIZE,8,
	//	GLX_GREEN_SIZE,8,GLX_BLUE_SIZE,8,GLX_ALPHA_SIZE,8,GLX_DEPTH_SIZE,24,GLX_STENCIL_SIZE,8,
	//	GLX_FLOAT_COMPONENTS_NV,XFalse,GLX_SAMPLE_BUFFERS_ARB,XTrue,GLX_SAMPLES_ARB,2,0};
	int pattrib[] = {GLX_LARGEST_PBUFFER,XTrue,GLX_PRESERVED_CONTENTS,XTrue,
		GLX_PBUFFER_WIDTH,m_windowSize.x,GLX_PBUFFER_HEIGHT,m_windowSize.y,0};

	int screen = DefaultScreen(m_mainDisplay);
	const char *extensions = glXQueryExtensionsString(m_mainDisplay,screen);
	int nelements;
	GLXFBConfig *config = glXChooseFBConfig(m_mainDisplay,screen,att,&nelements);
	m_threadPBuffer = glXCreatePbuffer(m_mainDisplay,*config,pattrib);
	m_threadContext = glXCreateNewContext (m_mainDisplay, *config, GLX_RGBA_TYPE,m_mainContext,XTrue);
	if(config == NULL ||m_threadContext == NULL || m_threadPBuffer == NULL)
	{
		printf("Create error!\n");
		exit(1);
	}else
	{
		printf("Create OK!\n");
	}


    if(pthread_create(&moveThreadP, NULL, &loadingProc, (void*) this) != 0) 
	{
		LogStr("���߳̿���ʧ�ܣ�");
	}
}
void *XLoading::loadingProc(void * pParam)
{
	XLoading &pPar = *(XLoading *) pParam;
	//�л�������
	glXMakeCurrent(pPar.m_mainDisplay,pPar.m_threadPBuffer,pPar.m_threadContext);
	int isSuccess = 0;					//��¼�����Ƿ�ɹ��ı�Ǳ���
	if(pPar.m_loadFunc != NULL)
	{
		isSuccess = pPar.m_loadFunc();	//��¼�����Ƿ�ɹ�
	}
	//��ԭԭ�е�������
	glXMakeCurrent(pPar.m_mainDisplay,None,NULL);
	glXDestroyContext(pPar.m_mainDisplay,pPar.m_threadContext);
	glXDestroyPbuffer(pPar.m_mainDisplay,pPar.m_threadPBuffer);
	glXMakeCurrent(pPar.m_mainDisplay,pPar.m_mainPBuffer,pPar.m_mainContext);
	if(isSuccess == 1) pPar.m_isEnd = 1;	//���سɹ�
	else pPar.m_isEnd = 2;					//����ʧ��
}
#endif
}