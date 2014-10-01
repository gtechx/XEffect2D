#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XOSDefine.h"
#include "XMemoryPool.h"

#include "stdio.h"
//#include <math.h>
#include "string.h"
#include "XCommonDefine.h"
#include "XStringFun.h"
#include "XByteFun.h"
#include "XRandomFun.h"
#include "XMath/XMath.h"

//#define XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
//#define XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
//#define FILE_ERROR(p) {fclose(p);return XFalse;}
//�Լ�����new��delete
//A* a = (A*)malloc(sizeof(A));
//new (a) A();	//����������
//
//a->~A();
//free(a);

template<typename T>
void XDELETE(T *&p)
{
	if(p != NULL)
	{
		delete p;
		p = NULL;
	}
}
template<typename T>
void XDELETE_ARRAY(T *&p)
{
	if(p != NULL)
	{
		delete [] p;
		p = NULL;
	}
}
template<typename T>
T * createMem()
{
	T * ret = NULL;
	try
	{
		ret = new T;
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}
template<typename T>
T * createArrayMem(int size)
{
	if(size <= 0) return NULL;
	T * ret = NULL;
	try
	{
		ret = new T[size];
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}
inline float squareFloat(float x) {return x * x;}	//��x��ƽ��
//#define SIN_BOOK	//sin�����Ƿ�ʹ�ò��ֵ䷽ʽ
#ifdef SIN_BOOK
extern void initSinData();
extern float xSin(float x);
#endif
//#define SQRT_BOOK	//�����Ƿ�ʹ�ò��ֵ䷽ʽ

#ifdef SQRT_BOOK
extern void initSqrtData();
extern float xSqrt(float x);
#endif
//��ȡ�ļ��Ĵ�С
inline int getFileLen(FILE * fp)
{
	if(fp == NULL) return 0;
	int tempOffset = ftell(fp);	//��¼��ǰ���ļ�λ��
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);	//��ȡ�ļ��ĳ���
	fseek(fp,tempOffset,SEEK_SET);	//�ָ�֮ǰ���ļ�λ��
	return len;
}
template<class type> void xChangeTwoSum(type &sum0,type &sum1)
{//��������������ֵ
	type temp = sum0;
	sum0 = sum1;
	sum1 = temp;
}
extern int mySleep(unsigned long sleepUSecond);	//���������ʱ����us��1000000��֮1�룬��1000��֮1����

//���HL1��HL2�ĺ͵�һ�룬�������������������ģ��������û����
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);

#ifdef GET_ALL_PIXEL
extern int XEE_AllPixel;	//�ܵ���ͼ����
#endif
//ͼ�������㷨��˫���Բ�ֵ
extern void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//ԭʼͼ�����ݣ�Ŀ��ͼ�����ݣ����ųߴ�
					  int sw,int sh,int dw,int dh,	//ԭʼͼ��ߴ磬Ŀ��ͼ��ߴ�
					  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//ԭʼͼ��ü���λ�ã���Ŀ��ͼ���λ��
					  int sClipW,int sClipH);	//ԭʼͼ��Ĳü���С
//��ͨͼ�����ŵ�Ӧ����ʽΪpixelDataZoomRGB(src,dst,zoom,sw,sh,dw,dh,0,0,0,0,sw,sh);
//mode 0:����ƽ��,1:��������Ӧ��������ֺ�ɫ
extern void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,
								const _XVector2 &srcSize,const _XVector2 &dstSize,const _XVector2& srcPos,
								const _XVector2 &dstClipSize,int mode = 0);

inline float rectResize(float w,float h,float Rx,float Ry)	//����Rx��Ry���ɵĿ򣬽�w��h�Ŀ�������У�����ܷŽ�ȥ���򷵻�1.0f,���򷵻�����Ӧ��С
{//�ú������ڴ���������Ӧ���������ŵı���
	if(w <= 0.0f || h <= 0.0f || Rx <= 0.0f || Ry <= 0.0f) return 1.0f;
	if(w <= Rx && h <= Ry) return 1.0f;
	float size = Rx / w;
	if(size < Ry / h) return size;
	else return Ry / h;
}

#ifdef XEE_OS_WINDOWS
enum _XShutDownSystemMode
{
	SYS_SD_MODE_G,		//�ػ�
	SYS_SD_MODE_Z,		//ע��
	SYS_SD_MODE_C,		//����
};
//����ϵͳ�ػ�������������ע���ĺ���
extern void shutDownSystem(_XShutDownSystemMode mode = SYS_SD_MODE_G);
//��̨ͣ�ٵȴ���������
inline void waitingForKey() {system("pause");}
//���ÿ���̨������ֵ���ɫ
inline void setConsoleColor(int index)
{
	HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE); 
	switch(index)
	{
	case 1:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_BLUE);
		break;
	case 2:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_GREEN);
		break;
	default:
		SetConsoleTextAttribute(consolehwnd,FOREGROUND_RED);
		break;
	}
}
#endif
/*
//CC 00 00 00 00 00 00 00 00 00 00 00 DD
//��Ҫ��������в�������ͷ������
#include <deque>
std::deque<unsigned char *> packetDeque;
unsigned char *dataPacket = NULL;
int dataSum = 0;
int packetSize = 13;
void recvDataProc(unsigned char * data,int len)	//���ݼ򵥵�Э�����������ȡ���ݰ��ĺ���
{
	if(data == NULL || len <= 0) return ;
	if(dataSum == 0)
	{
		if(data[0] != 0xCC)
		{
			for(int i = 0;i < len;++ i)
			{
				if(data[i] == 0xCC)
				{
					recvDataProc(data + i,len - i);
					break;
				}
			}
		}else
		{
			dataPacket = createArrayMem<unsigned char>(packetSize);
			if(dataPacket == NULL) exit(0);
			if(len >= packetSize)
			{
				memcpy(dataPacket,data,packetSize);
				if(dataPacket[packetSize - 1] != 0xDD) exit(0);
				packetDeque.push_back(dataPacket);
				dataPacket = NULL;
				dataSum = 0;
				recvDataProc(data + packetSize,len - packetSize);
			}else
			{	
				memcpy(dataPacket,data,len);
				dataSum = len;
			}
		}
	}else
	{
		if(dataSum + len >= packetSize)
		{
			int need = packetSize - dataSum;
			memcpy(dataPacket + dataSum,data,need);
			if(dataPacket[packetSize - 1] != 0xDD) exit(0);
			packetDeque.push_back(dataPacket);
			dataPacket = NULL;
			dataSum = 0;
			recvDataProc(data + need,len - need);
		}else
		{
			memcpy(dataPacket + dataSum,data,len);
			dataSum += len;
		}
	}
}*/
struct _XShaderHandle
{
	unsigned int shaderHandle;
	unsigned int shaderF;	//frag
	unsigned int shaderV;	//vert
	unsigned int shaderG;	//geomrtry
	_XShaderHandle()
		:shaderHandle(0)
		,shaderF(0)
		,shaderV(0)
		,shaderG(0)
	{}
};
#endif 
