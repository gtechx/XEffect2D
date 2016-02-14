INLINE void XFrameWork::setStop(int temp)	//ֹͣ 0,play,1,stop,-1,change state
{
	if(temp > 0) m_stopFlag = XTrue;else 
	if(temp == 0) m_stopFlag = XFalse;else
		m_stopFlag = !m_stopFlag;
}
INLINE void XFrameWork::setFPS(int temp)		//������Ϸ֡�ʣ�С��0Ϊ������֡��(Ŀǰ�����������û����Ӧ�е�����)
{
	if(temp <= 0) return;	//�����������
	m_manualFPS = temp;
	m_manualFrameTime = 2000.0f / temp;
}
INLINE void XFrameWork::setRendFPS(int tmp)
{
	m_windowData.drawFps = tmp;
	if(m_windowData.drawFps <= 0) m_rendDelay = -1;
	else m_rendDelay = 1000.0f / m_windowData.drawFps;
}
INLINE void XFrameWork::setBGColor(const XFColor &c)	//���ñ�������ɫ
{
	m_defBGColor = c;
	glClearColor(m_defBGColor.fR,m_defBGColor.fG,m_defBGColor.fB,m_defBGColor.fA);
}
INLINE XBool XFrameWork::initWindowEx(const XWindowInfo& wData)	//����ӿ��Ѿ�����
{
	m_windowData = wData;
	return initWindow();
}
INLINE XBool XFrameWork::initWindowEx(void)	//����ӿ��Ѿ�����
{
	//if(!readSystemConfigData(windowData)) return XFalse;f
	if (!readSystemConfigData(m_windowData)) LogStr("��ȡ�����ļ�ʧ��!");	//2014.4.13�޸�Ϊ��ȡ�����ļ�ʧ��֮���˳�������ʹ��Ĭ��ֵ��������
	return initWindow();
}
INLINE XBool XFrameWork::getWindowNeedDraw()	//��ȡ��ǰ�����Ƿ���Ҫˢ��
{
	if(m_forceToPaint) return XTrue;	//�������Ϊǿ�Ƹ���������ֱ������
	if(!(GetWindowLong(m_hwnd, GWL_STYLE) & WS_VISIBLE)) return XFalse;	//�жϴ����Ƿ�ɼ�
	if(IsIconic(m_hwnd)) return XFalse;	//�жϴ����Ƿ���С��
	return XTrue;
}
INLINE void XFrameWork::restartGame()		//�������е�ǰ����
{
	char szFileName[255];
	GetModuleFileNameA(NULL, szFileName, 255);
	XEE::startProgram(szFileName);
}
INLINE void XFrameWork::clearScreen()		//�����Ļ������
{
	if (m_windowData.windowType == WINDOW_TYPE_3D)
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//3D
	else glClear(GL_COLOR_BUFFER_BIT);	//only 2D
}
INLINE void XFrameWork::clearScreen(const XFColor & c)
{
	glClearColor(c.fR, c.fG, c.fB, c.fA);			//�����ɫ
	clearScreen();
	glClearColor(m_defBGColor.fR,m_defBGColor.fG,m_defBGColor.fB,m_defBGColor.fA);			//�����ɫ
}
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif
//#ifndef LWA_ALPHA
//#define LWA_ALPHA 0x00000002
//#endif
INLINE void XFrameWork::setWindowAlpha(float a)
{
	static bool isFirst = true;
	if(isFirst)
	{
		if(SetWindowLong(m_hwnd, GWL_EXSTYLE, GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED) == 0)
		{
			printf("SetWindowLong error:%lu!\n",GetLastError());
			return;
		}
		isFirst = false;
	}
#ifdef LWA_ALPHA
//	if(!SetLayeredWindowAttributes(wHandle,RGB(255,255,255),0,LWA_COLORKEY))
//	{
//		printf("SetLayeredWindowAttributes error!\n");
//	}
	SetLayeredWindowAttributes(m_hwnd,0,a * 255,LWA_ALPHA);
	//SetLayeredWindowAttributes(wHandle,RGB(0,0,0),a * 255,LWA_ALPHA | LWA_COLORKEY);
#endif
}
INLINE void XFrameWork::getWindowPos(int &x,int &y)
{
	RECT rect;
	GetWindowRect(m_hwnd,&rect);
	x = rect.left;
	y = rect.top;
}
INLINE void XFrameWork::getWindowSize(int &w,int &h)
{
	RECT rect;
	GetWindowRect(m_hwnd,&rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}
INLINE void XFrameWork::setWindowSize(float x,float y)
{
	RECT rect;
	GetWindowRect(m_hwnd,&rect);
	if(m_windowData.isAlwaysTop) SetWindowPos(m_hwnd,HWND_TOPMOST,(int)rect.left,(int)rect.top,(int)x,(int)y,SWP_SHOWWINDOW);
	else SetWindowPos(m_hwnd,HWND_TOP,(int)rect.left,(int)rect.top,(int)x,(int)y,SWP_SHOWWINDOW);
}
INLINE void XFrameWork::setWindowPos(int x,int y,bool always)
{
	RECT rect;
	GetWindowRect(m_hwnd,&rect);
	m_windowData.isAlwaysTop = always;
	if(m_windowData.isAlwaysTop) SetWindowPos(m_hwnd,HWND_TOPMOST,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
	else SetWindowPos(m_hwnd,HWND_TOP,x,y,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
}