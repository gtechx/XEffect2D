//********************************************************************************
//������һ������Ŀ�ܣ���ܰ������ڵĽ������������¼��Ĵ������Լ�ͼ����Ⱦ����
//program by ��ʤ��
//********************************************************************************
#include "GGame.h"
//Environment="PATH=%PATH%;..\..\engine\dll\libfreetype;..\..\engine\dll\SDL;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//��Ҫ����dll·��
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif

int main(int argc, char **argv)
{	
	XE::runGame<GGame>(&XE::XWindowInfo(1366,768));
	return 0;	
}