#include "XStdHead.h"
#include "XWindowTitle.h"
#include "XShaderGLSL.h"
#include "XXml.h"
#include "XFile.h"
#include <SHLOBJ.H>
#include "shobjidl.h"
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
namespace XE{
namespace XEE
{
	bool createLink()
	{
		if(!SUCCEEDED(CoInitialize(NULL))) return false;
		IShellLink *pisl;
		if (!SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl)))
		{
			CoUninitialize();
			return false;
		}
		IPersistFile* pIPF;
		std::string tmpStr = XFile::getCurrentExeFileFullPath();
		pisl->SetPath(tmpStr.c_str());
		pisl->SetWorkingDirectory(XFile::getWorkPath().c_str());
		if(!SUCCEEDED(pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF)))
		{
			pisl->Release();
			CoUninitialize();
			return false;
		}
		tmpStr[tmpStr.size() - 3] = 'l';
		tmpStr[tmpStr.size() - 2] = 'n';
		tmpStr[tmpStr.size() - 1] = 'k';

		wchar_t wsz[MAX_PATH]; // ����Unicode�ַ��� 
		MultiByteToWideChar(CP_ACP, 0, tmpStr.c_str(), -1, wsz, MAX_PATH); 
		pIPF->Save(wsz, FALSE);
		pIPF->Release();
		pisl->Release();
		CoUninitialize();
		return true;
	}
	bool setRegRun(bool stat)
	{
		HKEY hKey;
		//��ָ���Ӽ�
		DWORD dwDisposition = REG_OPENED_EXISTING_KEY;    // ��������ڲ�����
		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL,
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
			return false;
		//����һ���µļ�ֵ�����ü�ֵ����Ϊ�ļ���
		if(stat)
		{
			if(createLink())
			{
				std::string filename = XFile::getCurrentExeFileFullPath();
				filename[filename.size() - 3] = 'l';
				filename[filename.size() - 2] = 'n';
				filename[filename.size() - 1] = 'k';
				//��ȡע����иü��ļ�ֵ������Ѿ���ͬ���ظ����á�
				if(RegSetValueEx(hKey,XEG.m_windowData.windowTitle.c_str(), 0, REG_SZ, (BYTE*)filename.c_str(), filename.length()) == ERROR_SUCCESS)
					LogStr("�Զ��������óɹ�!");
			}else
			{
				RegCloseKey(hKey);
				return false;
			}
		}else
		{
			if(RegDeleteValue(hKey,XEG.m_windowData.windowTitle.c_str()) == ERROR_SUCCESS)
				LogStr("ȡ���Զ�����!");
		}
		// �ر��Ӽ����
		RegCloseKey(hKey);
		return true;
	}
	int sleep(unsigned long sleepUSecond)
	{//ע�⵽�������С�ӳ�����Ϊ1ms������С��1ms���ӳٶ�ģ��Ϊ1ms
	#ifdef XEE_OS_WINDOWS
		int delay = (int)(sleepUSecond * 0.001f);
		if(delay <= 0) Sleep(1);
		else Sleep(delay);
	#endif
	#ifdef XEE_OS_LINUX
	//����1
	//	if(sleepUSecond < 1000) sleepUSecond = 1000;
	//	usleep(sleepUSecond);
	//����2
	/*	if(sleepUSecond < 1000) sleepUSecond = 1000;
		timespec t_timeval;
		if(sleepUSecond >= 1000000)
		{//��������ӳ�ֱ������    
			t_timeval.tv_sec = sleepUSecond /1000000;
			t_timeval.tv_nsec = (sleepUSecond % 1000000) * 1000; 
		}else
		{
			t_timeval.tv_sec = 0;
			t_timeval.tv_nsec = sleepUSecond * 1000; 
		}
		if(nanosleep(&t_timeval,NULL) == -1)
		{
			//DSHOW("sleep error!\n");
		}*/
	//����3
		if(sleepUSecond < 1000) sleepUSecond = 1000;
		timeval t_timeval;
		if(sleepUSecond >= 1000000)
		{//��������ӳ�ֱ������    
			t_timeval.tv_sec = sleepUSecond /1000000;
			t_timeval.tv_usec = sleepUSecond % 1000000; 
		}else
		{
			t_timeval.tv_sec = 0;
			t_timeval.tv_usec = sleepUSecond; 
		}
		if(select(0, NULL, NULL, NULL, &t_timeval) == -1)
		{
			//DSHOW("sleep error!\n");
		}
	//����4
	/*	pthread_cond_t mycond;     
		pthread_mutex_t mymutex;
	
		pthread_cond_init(&mycond, NULL);
   		pthread_mutex_init(&mymutex, NULL);
	
		pthread_mutex_lock(&mymutex); 
	
		if(sleepUSecond < 10000) sleepUSecond = 10000;
		struct timeval now; 
		gettimeofday(&now, NULL); 
    
		timespec abstime;
		abstime.tv_nsec = now.tv_usec * 1000 + (sleepUSecond % 1000000) * 1000; 
		abstime.tv_sec = now.tv_sec + sleepUSecond / 1000000;
		pthread_cond_timedwait(&mycond, &mymutex, &abstime);      
  		//DSHOW("time out dispatch something...\n");   
 
		pthread_mutex_unlock(&mymutex);*/       
	#endif
		return 0;	
	}
	inline ULONGLONG subtractTime(const FILETIME &a, const FILETIME &b)
	{
		LARGE_INTEGER la, lb;
		la.LowPart = a.dwLowDateTime;
		la.HighPart = a.dwHighDateTime;
		lb.LowPart = b.dwLowDateTime;
		lb.HighPart = b.dwHighDateTime;

		return la.QuadPart - lb.QuadPart;
	}
	float getCpuUsage()	//��ȡCPU��ռ����
	{
#if _WIN32_WINNT < 0x0501
		return 0.0f;
#else
		static FILETIME prevSysKernel, prevSysUser;
		static FILETIME prevProcKernel, prevProcUser;
		static bool isFirstRun = true;

		FILETIME sysIdle, sysKernel, sysUser;
		FILETIME procCreation, procExit, procKernel, procUser;

		if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
		{// can't get time info so return
			return -1.;
		}
		// check for first call
		if (isFirstRun)
		{
			isFirstRun = false;
			// save time info before return
			prevSysKernel.dwLowDateTime = sysKernel.dwLowDateTime;
			prevSysKernel.dwHighDateTime = sysKernel.dwHighDateTime;

			prevSysUser.dwLowDateTime = sysUser.dwLowDateTime;
			prevSysUser.dwHighDateTime = sysUser.dwHighDateTime;

			prevProcKernel.dwLowDateTime = procKernel.dwLowDateTime;
			prevProcKernel.dwHighDateTime = procKernel.dwHighDateTime;

			prevProcUser.dwLowDateTime = procUser.dwLowDateTime;
			prevProcUser.dwHighDateTime = procUser.dwHighDateTime;

			return -1.;
		}

		ULONGLONG sysKernelDiff = subtractTime(sysKernel, prevSysKernel);
		ULONGLONG sysUserDiff = subtractTime(sysUser, prevSysUser);

		ULONGLONG procKernelDiff = subtractTime(procKernel, prevProcKernel);
		ULONGLONG procUserDiff = subtractTime(procUser, prevProcUser);

		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
		ULONGLONG procTotal = procKernelDiff + procUserDiff;

		return (float)((100.0 * procTotal) / sysTotal);
#endif
	}
	XMemUsageInfo getMemUsageInfo()
	{
		XMemUsageInfo ret;
		HANDLE handle = GetCurrentProcess();
		if (handle == NULL) return ret;
		PROCESS_MEMORY_COUNTERS pmc;
		if (!GetProcessMemoryInfo(handle, &pmc, sizeof(pmc))) return ret;
		ret.peakWorkSet = pmc.PeakWorkingSetSize;	//��ֵ�ڴ�ռ��
		ret.curWorkingSet = pmc.WorkingSetSize;		//��ǰ�ڴ�ռ��
		return ret;
		//Members
		//cb
		//Size of the structure, in bytes.
		//PageFaultCount
		//Number of page faults.		//ȱҳ�жϴ���
		//PeakWorkingSetSize		//���������:��ֵ������
		//Peak working set size.		//ʹ���ڴ�߷�
		//WorkingSetSize			//���������:������
		//Current working set size.		//��ǰʹ�õ��ڴ�
		//QuotaPeakPagedPoolUsage
		//Peak paged pool usage.		//ʹ��ҳ�滺��ظ߷�
		//QuotaPagedPoolUsage		//���������:ҳ�滺���
		//Current paged pool usage.		//ʹ��ҳ�滺���
		//QuotaPeakNonPagedPoolUsage
		//Peak nonpaged pool usage.		//ʹ�÷Ƿ�ҳ����ظ߷�
		//QuotaNonPagedPoolUsage	//���������:��ҳ�滺���
		//Current nonpaged pool usage. //ʹ�÷Ƿ�ҳ�����
		//PagefileUsage				//���������:�ύ��С(�����ڴ�)
		//Current space allocated for the pagefile. Those pages may or may not be in memory. //ʹ�÷�ҳ�ļ�
		//PeakPagefileUsage			//��������ڴ�
		//Peak space allocated for the pagefile. //ʹ�÷�ҳ�ļ��߷�
	}

	//void winMsgRelease()
	//{
	//	if(customWinProc == daemonProc)
	//	{
	//		Shell_NotifyIcon(NIM_DELETE, &nid);	//��Դ�ͷ�
	//	}
	//}
	void simulateFullScreen(HWND hwnd)
	{
		//����1
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
		newment.rcNormalPosition.left = -DialogX;      //(0,0)���غ�
		newment.rcNormalPosition.top = -DialogY;
		newment.rcNormalPosition.bottom = ScreenX - DialogY; //�ߴ�һ��
		newment.rcNormalPosition.right = ScreenY - DialogX;
		SetWindowPlacement(hwnd, &newment);
		//����2
		//	SetWindowLongPtr(hwnd, GWL_STYLE, 
		//		WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
		//	//MoveWindow(hwnd, 0, 0, windowWidth, windowHeight, TRUE);
		//	setWindowPos(0,0);
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef CREATE_WINDOW_WITH_GLUT
	void draw(){}
	void idle()
	{
		pGame->move(engineIdle());
		clearScreen();			//�����Ļ
		pGame->draw();
		updateScreen();			//������Ļ������
		//mySleep(1);
	}
	void inputProc(const XInputEvent &input)
	{
		pGame->input(input);
		inputEvent(input);
	}
	void input(unsigned char c, int x, int y)
	{
		XInputEvent tmpEvent;
		tmpEvent.type = EVENT_KEYBOARD;
		tmpEvent.keyState = KEY_STATE_DOWN;
		tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(c);
		tmpEvent.mouseX = x;
		tmpEvent.mouseY = y;
		inputProc(tmpEvent);
		if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
	}
	void sInput(int k, int x, int y)
	{
		XInputEvent tmpEvent;
		tmpEvent.type = EVENT_KEYBOARD;
		tmpEvent.keyState = KEY_STATE_DOWN;
		tmpEvent.keyValue = (XKeyValue)XWindow.mapKey(k + 512);
		tmpEvent.mouseX = x;
		tmpEvent.mouseY = y;
		inputProc(tmpEvent);
		if (tmpEvent.keyValue == XKEY_ESCAPE) exit(1);
	}
	void mouseMove(int x, int y)
	{
		XInputEvent tmpEvent;
		tmpEvent.type = EVENT_MOUSE;
		tmpEvent.mouseState = MOUSE_MOVE;
		tmpEvent.mouseX = x;
		tmpEvent.mouseY = y;
		inputProc(tmpEvent);
	}
	void mouseProc(int b, int s, int x, int y)
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
		inputProc(tmpEvent);
	}
#endif
	XBool startProgram(const char * programPath)
	{
		if(programPath == NULL) return XFalse;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			programPath,// �����ȫ·��,�磺"../Debug/x.exe"
			NULL, // Command line.  
			NULL, // Process handle not inheritable.  
			NULL, // Thread handle not inheritable.  
			FALSE, // Set handle inheritance to FALSE.  
			0, // No creation flags.  
			NULL, // Use parent's environment block.  
			NULL, // Use parent's starting directory.  
			&si, // Pointer to STARTUPINFO structure.
			&pi) // Pointer to PROCESS_INFORMATION structure.
			)
		{
			LogNull("CreateProcess failed (%d).", GetLastError());
			return XFalse;
		}
		//��ʹ�õľ����ùص�
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return XTrue;
	}
	XBool startProgram(const char * programPath,HANDLE &hProcess)
	{
		if(programPath == NULL) return XFalse;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));
		if (!CreateProcess(
			programPath,// �����ȫ·��,�磺"../Debug/x.exe"
			NULL, // Command line.  
			NULL, // Process handle not inheritable.  
			NULL, // Thread handle not inheritable.  
			FALSE, // Set handle inheritance to FALSE.  
			0, // No creation flags.  
			NULL, // Use parent's environment block.  
			NULL, // Use parent's starting directory.  
			&si, // Pointer to STARTUPINFO structure.
			&pi) // Pointer to PROCESS_INFORMATION structure.
			)
		{
			LogNull("CreateProcess failed (%d).", GetLastError());
			return XFalse;
		}
		//��ʹ�õľ����ùص�
		CloseHandle(pi.hThread);
		hProcess = pi.hProcess;
		return XTrue;
	}
	XBool exitProgram(HANDLE hProcess)
	{
		return TerminateProcess(hProcess,0);
	}
	XBool activateGame(HWND h)
	{
		//����1��//����
		//return BringWindowToTop(XEG.getHWND());
		//����2��//����
		//typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL);
		//PROCSWITCHTOTHISWINDOW SwitchToThisWindow;
		//HMODULE hUser32 = GetModuleHandle("user32");
		//SwitchToThisWindow = ( PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");  
		//if(SwitchToThisWindow == NULL) return false;
		////������ֻҪ���κ��ִ洰�ڵľ����������������ɣ��ڶ�������ָ��������ڼ�С�����Ƿ�ָ���ԭ״̬��
		//SwitchToThisWindow(XEG.getHWND(), TRUE);
		//return true;
		//����3��//����
		HWND hCurWnd = GetForegroundWindow();
		DWORD dwMyID = GetCurrentThreadId();
		DWORD dwCurID = GetWindowThreadProcessId(hCurWnd, NULL);
		AttachThreadInput(dwCurID, dwMyID, TRUE);
		SetForegroundWindow(h);
		AttachThreadInput(dwCurID, dwMyID, FALSE);
		return  true;
		//����4��//����
		//ShowWindow(XEG.getHWND(),SW_SHOWNA);//�򵥵���ʾ���������¶� 
		//SetActiveWindow(XEG.getHWND());
		//SetForegroundWindow(XEG.getHWND());    
		////this->SetWindowPos(this,LOWORD(lParam),HIWORD(lParam),c.Width(),c.Height(),SWP_NOACTIVATE);
		//BringWindowToTop(XEG.getHWND());
		//return true;
	}
	//--------------------------------------------------------------
#ifdef XEE_OS_WINDOWS
	bool getSysPrivilege(const char * value)
	{
		HANDLE hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			TOKEN_PRIVILEGES tkp;
			LookupPrivilegeValue(NULL, value, &tkp.Privileges[0].Luid);
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if(AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
			{
				CloseHandle(hToken);
				return true;
			}
		}
		CloseHandle(hToken);
		return false;
	}
	void shutDownSystem(XShutDownSystemMode mode)
	{
		if (!getSysPrivilege(SE_SHUTDOWN_NAME)) return;
		switch(mode)
		{
		case SYS_SD_MODE_G:
			//InitiateSystemShutdownEx(NULL,NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//�ر�����ļ����
			//InitiateSystemShutdownEx("192.168.0.1",NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//�ر�Զ�̵ļ����
			ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
			break;
		case SYS_SD_MODE_Z:
			ExitWindowsEx(EWX_LOGOFF,0); 
			break;
		case SYS_SD_MODE_C:
			ExitWindowsEx(EWX_REBOOT,0);
			break;
		}
	}
#endif
}
}