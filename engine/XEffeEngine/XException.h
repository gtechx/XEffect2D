#ifndef _JIA_XEXCEPTION_
#define _JIA_XEXCEPTION_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
//�����װ��һЩ�쳣����Ľӿ�
//Ŀǰ�����쳣����û��̫����Ч��������ļ��⣬�ܿ�ϧ
#include "XOSDefine.h"
namespace XE{
namespace XException
{
	extern LONG WINAPI unhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers);  
	extern BOOL WINAPI consoleHandler(DWORD consoleEvent);
	extern bool initException();	//��ʼ���쳣�������
	//Ϊ�˼�����ǰ�����Ӷ����⿪�ŵĽӿ�
	extern void dumpMemory(const char *filename, void *p);
};
}
#endif