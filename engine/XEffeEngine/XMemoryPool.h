#ifndef _JIA_XMEMORYPOOL_
#define _JIA_XMEMORYPOOL_

//����һ���̳߳ص��࣬��δ�кõ�ʵ��˼·
//Ŀǰͨ��ʹ�õ��������ڴ��������
//�����Ƽ�ʹ��libtcmalloc_minimal.lib��
//����https://code.google.com/p/gperftools/
//ʹ�÷�����
//1������lib�ļ�
#pragma comment(lib, "MemoryPool/libtcmalloc_minimal.lib")
//����ǿ�Ʒ�������__tcmalloc
#ifdef XEE_OS_WINDOWS
//#pragma comment( linker, "/INCLUDE:__tcmalloc" )	//ʹ���ڴ�صı���˵��
#endif

//#include "stdio.h"
//#include "stdlib.h"
//extern int allMemory;
//inline void* operator new(size_t sz) 
//{
//	allMemory += sz;
//	printf("global op new called, size = %d\n",allMemory);
//	return malloc(sz);
//}
//inline void* operator new[](size_t sz) 
//{
//	allMemory += sz;
//	printf("global op new[] called, size = %d\n",allMemory);
//	return malloc(sz);
//}
//inline void operator delete(void* ptr,size_t sz)
//{
//	allMemory -= sz;
//	printf("global op delete called:%d\n",allMemory);
//	free(ptr);
//}
//inline void operator delete[](void* ptr,size_t sz)
//{
//	allMemory -= sz;
//	printf("global op delete[] called:%d\n",allMemory);
//	free(ptr);
//}
//class  A
//{
//public:
//	A()
//	{
//		printf("���캯��!\n");
//	}
//	~A()
//	{
//		printf("��������!\n");
//	}
//	//static void * operator new(size_t size)
//	//{
//	//	void *p = malloc(sizeof(A));
//	//	printf("new! %d\n",size);
//	//	return p;
//	//}
//	//static void * operator new[](size_t size)
//	//{
//	//	void *p = malloc(size);
//	//	printf("new[]! %d\n",size);
//	//	return p;
//	//}
//	//static void operator delete(void *p,size_t size)
//	//{
//	//	free(p);
//	//	printf("delete! %d\n",size);
//	//}
//	//static void operator delete[](void *p,size_t size)
//	//{
//	//	free(p);
//	//	printf("delete[]! %d\n",size);
//	//}
//};
//void main()
//{
//	A *p = new A();
//	A *q = new A[2];
//	delete p;
//	delete[] q;
//}
#endif