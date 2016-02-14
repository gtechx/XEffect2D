#ifndef _JIA_XENGINECOMMON_
#define _JIA_XENGINECOMMON_
//�������ڶ������������й��õĲ��֣���ԭ��C��ʽ���﷨���巽ʽ��ת����C++�ķ�ʽ���й���
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.2
//--------------------------------
#include "tlhelp32.h"
#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XWindowCommon.h"
namespace XE{
namespace XEE //+
{
	//stat�Ƿ񿪻�����
	extern bool setRegRun(bool stat);	//���ó���Ϊ��������
	extern bool isRegRun();				//��ȡ�����Ƿ�Ϊ��������(��δʵ��)
	extern int sleep(unsigned long sleepUSecond);	//���������ʱ����us��1000000��֮1�룬��1000��֮1����
	inline void setVSync(bool sync)	//�����͹رմ�ֱͬ�����رյĻ������Էſ�֡�ʣ������Ļ�������֡��
	{	
		// Function pointer for the wgl extention function we need to enable/disable
		// vsync
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

		const char *extensions = (char*)glGetString( GL_EXTENSIONS );

		if(strstr(extensions,"WGL_EXT_swap_control") == 0) return;
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
			if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
		}
		//˵��
		//In Linux / GLX
		//Use the GLX_EXT_swap_control extension to control swap interval. Check the GLX-specific extensions string via glXQueryExtensionsString() to verify that the extension is actually present.
		//The extension provides glXSwapIntervalEXT(), which also directly specifies the swap interval. glXSwapIntervalEXT(1) is used to enable vsync; glXSwapIntervalEXT(0) to disable vsync.
	}
	inline XBool getCapsLockState(){return (GetKeyState(VK_CAPITAL) == 1);}
	inline XBool getNumLockState(){return (GetKeyState(VK_NUMLOCK) == 1);}
	//���ð���״̬
	inline void setCapsLockState(XBool state)
	{
		BYTE btKeyState[256];
		if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
		if((!(btKeyState[VK_CAPITAL] & 1) && state) ||
			((btKeyState[VK_CAPITAL] & 1) && !state))
		{//�������û������������Ϊ��
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
	inline void setNumLockState(XBool state)
	{
		BYTE btKeyState[256];
		if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
		if((!(btKeyState[VK_NUMLOCK] & 1) && state) ||
			((btKeyState[VK_NUMLOCK] & 1) && !state))
		{//�������û������������Ϊ��
			keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}

	inline void makeABeep(){printf("\a");}	//����һ����񻯵�����
	inline void makeCrash()	//����crash���������ڲ���crash�����
	{
		int *a = 0;
		*a = 0;
	}
	extern XBool startProgram(const char * programPath);//����һ������
	extern XBool startProgram(const char * programPath,HANDLE &hProcess);//����һ������
	extern XBool exitProgram(HANDLE hProcess);
	extern XBool activateGame(HWND h);	//���ǰ����Ϊ��ǰ��
	struct XMemUsageInfo
	{//�ڴ�ʹ�����������
		unsigned int peakWorkSet;		//��ֵ�ڴ�ʹ����
		unsigned int curWorkingSet;		//��ǰ�ڴ�ʹ����

		XMemUsageInfo()
			:peakWorkSet(0)
			, curWorkingSet(0)
		{}
	};
	extern XMemUsageInfo getMemUsageInfo();	//��ȡ�ڴ�ʹ����Ϣ
	extern float getCpuUsage();				//��ȡCPU��ռ����
	extern void simulateFullScreen(HWND hwnd);		//��ָ����������Ϊȫ��

#ifdef XEE_OS_WINDOWS
	enum XShutDownSystemMode
	{
		SYS_SD_MODE_G,		//�ػ�
		SYS_SD_MODE_Z,		//ע��
		SYS_SD_MODE_C,		//����
	};
	//����ϵͳ�ػ�������������ע���ĺ���
	extern void shutDownSystem(XShutDownSystemMode mode = SYS_SD_MODE_G);
	extern bool getSysPrivilege(const char * value);	//����ϵͳȨ��
	//��̨ͣ�ٵȴ���������
	inline void waitingForKey() {system("pause");}
	//���ÿ���̨������ֵ���ɫ
	inline void setConsoleColor(int index)
	{
		switch(index)
		{
		case 1:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE);break;
		case 2:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);break;
		default:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);break;
		}
	}
	//��ȡ��ǰ���̵�ID
	inline DWORD getCurProcessID(){return GetCurrentProcessId();}
	//�ж�ָ�������Ƿ����
	inline bool getIsProcessOver(DWORD id)
	{
	//	HANDLE hApp = OpenProcess(PROCESS_VM_OPERATION|SYNCHRONIZE,FALSE,id);
	//	return !WaitForSingleObject(hApp,0);	//����ָ�����̵�״̬
		if(id == NULL) return true;
		return !WaitForSingleObject(OpenProcess(PROCESS_VM_OPERATION|SYNCHRONIZE,FALSE,id),0);	//����ָ�����̵�״̬
	}
	//��ȡָ�����ƵĽ���ID
#ifdef UNICODE
	inline std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
#endif
	inline DWORD getProcesssID(const std::string &name)
	{
		PROCESSENTRY32  pe32;	
 
		HANDLE hSnaphot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //��ȡ���̿���
		Process32First(hSnaphot, &pe32); //ָ���һ������
		do
		{
#ifdef UNICODE
			if (lstrcmpi(pe32.szExeFile, s2ws(name).c_str()) == 0) //���ҽ�������ΪNotePad.exe�����±��ĳ���
#else
			if (lstrcmpi(pe32.szExeFile, name.c_str()) == 0) //���ҽ�������ΪNotePad.exe�����±��ĳ���
#endif
			{
				return pe32.th32ProcessID;
				break;
			}
		}while(Process32Next(hSnaphot, &pe32)); // ����ѭ��ֱ��ȡ��������
		return NULL;
	}
#endif

}
}

#endif