#include "XStdHead.h"
#include "XException.h"
#include "XErrorReporter.h"
//#include "XLogBook.h"

#include "Dbghelp.h"
#pragma comment(lib, "Dbghelp.lib")
namespace XE{
namespace XException
{
		//�����ǹ����쳣���Ե�����
	void dumpMemory(const char *filename, void *p)
	{
#if _WIN32_WINNT < 0x0501
		return;
#else
		std::string tmpFilename;
		if (filename == NULL)//�����Ե�ǰ��ʱ����Ϊ�ļ���
			tmpFilename = "dump_" + XTime::sysTime2String(XTime::getTimeMs()) + ".dmp";
		else
			tmpFilename = filename;

		_EXCEPTION_POINTERS *pExcPointers = (_EXCEPTION_POINTERS *)p;
		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = GetCurrentThreadId(); //����Ҫ����Ϣ���ȥ
		eInfo.ExceptionPointers = pExcPointers;
		eInfo.ClientPointers = FALSE;
		HANDLE hDumpFile = CreateFile(tmpFilename.c_str(), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
		MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal
			| MiniDumpWithHandleData
			| MiniDumpWithUnloadedModules
			| MiniDumpWithIndirectlyReferencedMemory
			| MiniDumpScanMemory
			| MiniDumpWithProcessThreadData
			| MiniDumpWithThreadInfo);
		//bool bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		//	hDumpFile, MiniDumpWithDataSegs, pExcPointers ? &eInfo : NULL, NULL, NULL);
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hDumpFile, MiniDumpWithDataSegs, pExcPointers ? &eInfo : NULL, NULL, NULL);
		CloseHandle(hDumpFile);
#endif
	}
	LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers)
	{//�������쳣������ 
		//SetErrorMode( SEM_NOGPFAULTERRORBOX );  
		//printf("Error address %x/n",pExceptionPointers->ExceptionRecord->ExceptionAddress);   
		//printf("CPU register:/n");   
		//printf("eax %x ebx %x ecx %x edx %x/n",pExceptionPointers->ContextRecord->Eax,   
		//	pExceptionPointers->ContextRecord->Ebx,pExceptionPointers->ContextRecord->Ecx,   
		//	pExceptionPointers->ContextRecord->Edx); 
		dumpMemory(NULL,pExceptionPointers);	//����ڴ�
		LogStr("���������쳣");					//�����־
		LogRelease();	//������־�ļ���������
		if(!XEG.getIsGameExit() && XEG.m_windowData.isRestartWhenCrash) XEG.restartGame();	//������������
		XEG.setGameExit();				//��ֹ�����˳�ʱ�ٴβ����쳣
		exit(1);								//�˳���ǰ����
//		return EXCEPTION_EXECUTE_HANDLER;//EXCEPTION_CONTINUE_SEARCH; //���� EXCEPTION_EXECUTE_HANDLER �رճ���  
	}
	BOOL WINAPI consoleHandler(DWORD consoleEvent)
	{
		switch(consoleEvent)
		{
		case CTRL_C_EVENT:		//�ر�
		case CTRL_BREAK_EVENT:	//��ͣ
		case CTRL_CLOSE_EVENT:	//�ر�
		case CTRL_LOGOFF_EVENT:	//�û��˳�
		case CTRL_SHUTDOWN_EVENT:	//�ر�
			LogRelease();	//�����˳�ʱ����֤��־��������
			//XEE::release();	//�����˳��¼�������δ������⣬��Ҫ��һ���Ĵ���
			break;
		}
		return TRUE;
	}
	bool initException()
	{
		SetUnhandledExceptionFilter(XException::unhandledExceptionFilter); 
		if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)XException::consoleHandler,TRUE))
		{
			LogStr("Unable to install handler!");
			return false;
		}
		return true;
	}
}
}