#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XOSDefine.h"
//#include "XMemoryPool.h"

//#include "stdio.h"
//#include <math.h>
//#include "string.h"
//#include "XCommonDefine.h"
//#include "XStringFun.h"
//#include "XMath/XByteFun.h"
//#include "XMath/XRandomFun.h"
//#include "XMath/XMath.h"
//#include "XMemory.h"
//#include "XFile.h"
namespace XE{
class XVector2;

//���HL1��HL2�ĺ͵�һ�룬�������������������ģ��������û����
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);

namespace XPixel
{
	#ifdef GET_ALL_PIXEL
	extern int XEE_AllPixel;	//�ܵ���ͼ����
	#endif
	extern void pixelsZoomRGBA(unsigned char * src,unsigned char * dst,	//ԭʼͼ�����ݣ�Ŀ��ͼ������
						  int sw,int sh,int dw,int dh);	//ԭʼͼ��Ĳü���С
	//ͼ�������㷨��˫���Բ�ֵ
	extern void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//ԭʼͼ�����ݣ�Ŀ��ͼ�����ݣ����ųߴ�
						  int sw,int sh,int dw,int dh,	//ԭʼͼ��ߴ磬Ŀ��ͼ��ߴ�
						  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//ԭʼͼ��ü���λ�ã���Ŀ��ͼ���λ��
						  int sClipW,int sClipH);	//ԭʼͼ��Ĳü���С
	//��ͨͼ�����ŵ�Ӧ����ʽΪpixelDataZoomRGB(src,dst,zoom,sw,sh,dw,dh,0,0,0,0,sw,sh);
	//mode 0:����ƽ��,1:��������Ӧ��������ֺ�ɫ
	enum XPixelFillMode	//ͼ�����ģʽ
	{
		PIXEL_MODE_CENTER_TILE,	//����ƽ��
		PIXEL_MODE_ZOOM,		//����Ӧ����
	};
	extern void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,
									const XVector2 &srcSize,const XVector2 &dstSize,const XVector2& srcPos,
									const XVector2 &dstClipSize,XPixelFillMode mode = PIXEL_MODE_CENTER_TILE);
	//ʹ�þ���ƽ�̵ķ�ʽ��src��������䵽dst����
	template<class T>
	void pixelFillRGBA(T *src,T *dst,int sw,int sh,int dw,int dh)	//�������û�о�����֤
	{
		//λ��
		XVector2 resSize(XEE_Min(sw,dw),XEE_Min(sh,dh));				//����ȷ����ԴĿ��ߴ�
		XVector2 resDstPos((XVector2(dw,dh) - resSize) * 0.5f);		//����ȷ����Ŀ��λ��
		XVector2 resSrcPos((XVector2(sw,sh) - resSize) * 0.5f);	//����ȷ����Դλ��
		//���濪ʼ��������
		int w = resSize.x * 4 * sizeof(T);
		int wS = (sw * sizeof(T)) << 2;
		int wD = (dw * sizeof(T)) << 2;
		int tmpD = int(resDstPos.x) * 4 * sizeof(T);
		int tmpS = int(resSrcPos.x) * 4 * sizeof(T);
		unsigned char *s = (unsigned char *)src;
		unsigned char *d = (unsigned char *)dst;
		for(int h = 0;h < resSize.y;++ h)
		{
			memcpy(d + (int)(h + resDstPos.y) * wD + tmpD,
				s + (int)(h + resSrcPos.y) * wS + tmpS,w);
		}
	}
}

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
			dataPacket = XMem::createArrayMem<unsigned char>(packetSize);
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
struct XShaderHandle
{
	unsigned int shaderHandle;
	unsigned int shaderF;	//frag
	unsigned int shaderV;	//vert
	unsigned int shaderG;	//geomrtry
	XShaderHandle()
		:shaderHandle(0)
		,shaderF(0)
		,shaderV(0)
		,shaderG(0)
	{}
};
}
#endif 
