//********************************************************************************
//����һ�ֶ��ϵ���βЧ�����ܽ���µ���βЧ�����о�
//program by ��ʤ��
//********************************************************************************
#include "GGameTail.h"
//Environment="PATH=%PATH%;..\..\engine\dll\SDL;..\..\engine\dll\fmodex;"	//��Ҫ����dll·��
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif

int main(int argc, char **argv)
{  
	XE::runGame<GGame>(&XE::XWindowInfo(1280,720));
	return 0;	
}