//********************************************************************************
//����һ��չʾ3D��2D��ϵ�����
//program by ��ʤ��
//********************************************************************************
#include "GGameE.h"
//Environment="PATH=%PATH%;..\..\engine\dll\gl;..\..\engine\dll\MemoryPool;"	//��Ҫ����dll·��
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif

int main(int argc, char **argv)
{
	XE::runGame<GGame>();
	return 0;	
}