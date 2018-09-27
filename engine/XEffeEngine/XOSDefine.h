#ifndef _JIA_OSDEFINE_
#define _JIA_OSDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#define XEE_OS_WINDOWS        //Windowsϵͳ��
//#define XEE_OS_LINUX        //Linuxϵͳ��

#define WITHOUT_SOME_WARNING (1)
//ֻ��������ӹ����޳�һЩ����Ĵ��룬�������������ط����κ����Ƶ����
#if WITHOUT_SOME_WARNING
#pragma warning(disable: 4244)    //conversion from 'int' to 'float', possible loss of data  
#pragma warning(disable: 4267)    //conversion from 'size_t' to 'int', possible loss of data 
#pragma warning(disable: 4100)    //unreferenced formal parameter
#pragma warning(disable: 4245)    //conversion from 'int' to 'unsigned int', signed/unsigned mismatch
#pragma warning(disable: 4127)    //conditional expression is constant
#pragma warning(disable: 4018)    //signed/unsigned mismatch
#pragma warning(disable: 4996)    //'sscanf' was declared deprecated
#pragma warning(disable: 4800)    //��GLboolean��: ��ֵǿ��Ϊ����ֵ��true����false��(���ܾ���)
#else
#define _CRT_SECURE_NO_WARNINGS
#endif

//�����еı��뻷������û�ж���bool���ͣ�����������һЩͳһ���Ĵ���
#ifdef XEE_OS_WINDOWS
#include <winsock2.h>	//���ͷ�ļ�������֮ǰ��������Ȼ����ɱ������
#include <windows.h>
#include <assert.h>
#endif

//#if defined DLL_EXPORT
//#define DECLDIR __declspec(dllexport)
//#else
//#define DECLDIR __declspec(dllimport)
//#endif
#endif
