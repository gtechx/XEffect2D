#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicFun.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include "XBasicClass.h"
//#include "string.h"

#ifdef XEE_OS_LINUX
#include <unistd.h>           //usleep()
#include <sys/time.h>
#endif
namespace XE{
void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo)
{
	int temp = 0;
	if(H1 >= 0x80) temp = -(L1 + ((H1 & 0x7f) << 8));
	else temp = L1 + (H1 << 8);

	int temp1 = 0;
	if(H2 >= 0x80) temp1 = -(L2 + ((H2 & 0x7f) << 8));
	else temp1 = L2 + (H2 << 8);

	temp = ((temp + temp1) >> 1);
	if(temp < 0)
	{
		Ho = (((- temp) >> 8) | 0x80);
		Lo = (- temp) % 256;
	}else
	{
		Ho = (temp >> 8);
		Lo = (temp % 256);
	}
}
namespace XPixel
{
	#ifdef GET_ALL_PIXEL
	int XEE_AllPixel = 0;
	#endif
	void pixelsZoomRGBA(unsigned char * src,unsigned char * dst,	//ԭʼͼ�����ݣ�Ŀ��ͼ������
						  int sw,int sh,int dw,int dh)	//ԭʼͼ��Ĳü���С
	{
		if(src == NULL || dst == NULL || sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;	//��������Ƿ�
		float tempSizeX = (float)(sw) / (float)(dw);
		float tempSizeY = (float)(sh) / (float)(dh);
		float xx,yy;
		int x,y,indexS,indexD,indexTS,indexTD;
		int sww = sw << 2;
		//���������Ҫ�����ݽ��д���
		for(int h = 0;h < dh;++ h)
		{
			yy = h * tempSizeY;
			y = yy;
			yy = yy - y;
			indexTD = h * dw;
			indexTS = y * sw;
			if(y < sh - 1)
			{//���һ��
				for(int w = 0;w < dw;++ w)
				{
					xx = w * tempSizeX;
					x = xx;		//��������
					xx = xx - x;	//С������
				
					indexD = (w + indexTD) << 2;
					indexS = (x + indexTS) << 2;
					if(x < sw - 1)
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS + sww + 0],src[indexS + 4 + 0],src[indexS + sww + 4 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS + sww + 1],src[indexS + 4 + 1],src[indexS + sww + 4 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS + sww + 2],src[indexS + 4 + 2],src[indexS + sww + 4 + 2],xx,yy);
						dst[indexD + 3] = XMath::bilinear(src[indexS + 3],src[indexS + sww + 3],src[indexS + 4 + 3],src[indexS + sww + 4 + 3],xx,yy);
					}else
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS + sww + 0],src[indexS - 4 + 0],src[indexS + sww - 4 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS + sww + 1],src[indexS - 4 + 1],src[indexS + sww - 4 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS + sww + 2],src[indexS - 4 + 2],src[indexS + sww - 4 + 2],xx,yy);
						dst[indexD + 3] = XMath::bilinear(src[indexS + 3],src[indexS + sww + 3],src[indexS - 4 + 3],src[indexS + sww - 4 + 3],xx,yy);
					}
				}
			}else
			{
				for(int w = 0;w < dw;++ w)
				{
					xx = w * tempSizeX;
					x = xx;		//��������
					xx = xx - x;	//С������
				
					indexD = (w + indexTD) << 2;
					indexS = (x + indexTS) << 2;
					if(x < sw - 1)
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS - sww + 0],src[indexS + 4 + 0],src[indexS - sww + 4 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS - sww + 1],src[indexS + 4 + 1],src[indexS - sww + 4 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS - sww + 2],src[indexS + 4 + 2],src[indexS - sww + 4 + 2],xx,yy);
						dst[indexD + 3] = XMath::bilinear(src[indexS + 3],src[indexS - sww + 3],src[indexS + 4 + 3],src[indexS - sww + 4 + 3],xx,yy);
					}else
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS - sww + 0],src[indexS - 4 + 0],src[indexS - sww - 4 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS - sww + 1],src[indexS - 4 + 1],src[indexS - sww - 4 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS - sww + 2],src[indexS - 4 + 2],src[indexS - sww - 4 + 2],xx,yy);
						dst[indexD + 3] = XMath::bilinear(src[indexS + 3],src[indexS - sww + 3],src[indexS - 4 + 3],src[indexS - sww - 4 + 3],xx,yy);
					}
				}
			}
		}	
	}
	//ͼ�������㷨
	//void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//ԭʼͼ�����ݣ�Ŀ��ͼ�����ݣ����ųߴ�
	//					  int sw,int sh,int dw,int dh,	//ԭʼͼ��ߴ磬Ŀ��ͼ��ߴ�
	//					  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//ԭʼͼ��ü���λ�ã���Ŀ��ͼ���λ��
	//					  int sClipW,int sClipH)	//ԭʼͼ��Ĳü���С
	void pixelDataZoomRGB(unsigned char * src,unsigned char * dst,float zoom,	//ԭʼͼ�����ݣ�Ŀ��ͼ�����ݣ����ųߴ�
						  int sw,int,int dw,int,	//ԭʼͼ��ߴ磬Ŀ��ͼ��ߴ�
						  int sOffsetX,int sOffsetY,int dOffsetX,int dOffsetY,	//ԭʼͼ��ü���λ�ã���Ŀ��ͼ���λ��
						  int sClipW,int sClipH)	//ԭʼͼ��Ĳü���С
	{
		int width = sClipW * zoom;
		int height = sClipH * zoom;
		float tempSize = 1.0f / zoom;
		float xx,yy;
		int x,y,indexS,indexD,indexTS,indexTD;
		int sww = sw * 3;
		//���������Ҫ�����ݽ��д���
		for(int h = 0;h < height;++ h)
		{
			yy = h * tempSize;
			y = yy;
			yy = yy - y;
			indexTD = (h + dOffsetY) * dw;
			indexTS = (y + sOffsetY) * sw;
			if(y < sClipH - 1)
			{//���һ��
				for(int w = 0;w < width;++ w)
				{
					xx = w * tempSize;
					x = xx;		//��������
					xx = xx - x;	//С������
				
					indexD = (w + dOffsetX + indexTD) * 3;
					indexS = (x + sOffsetX + indexTS) * 3;
					if(x < sClipW - 1)
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS + sww + 0],src[indexS + 3 + 0],src[indexS + sww + 3 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS + sww + 1],src[indexS + 3 + 1],src[indexS + sww + 3 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS + sww + 2],src[indexS + 3 + 2],src[indexS + sww + 3 + 2],xx,yy);
					}else
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS + sww + 0],src[indexS - 3 + 0],src[indexS + sww - 3 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS + sww + 1],src[indexS - 3 + 1],src[indexS + sww - 3 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS + sww + 2],src[indexS - 3 + 2],src[indexS + sww - 3 + 2],xx,yy);
					}
				}
			}else
			{
				for(int w = 0;w < width;++ w)
				{
					xx = w * tempSize;
					x = xx;		//��������
					xx = xx - x;	//С������
				
					indexD = (w + dOffsetX + indexTD) * 3;
					indexS = (x + sOffsetX + indexTS) * 3;
					if(x < sClipW - 1)
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS - sww + 0],src[indexS + 3 + 0],src[indexS - sww + 3 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS - sww + 1],src[indexS + 3 + 1],src[indexS - sww + 3 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS - sww + 2],src[indexS + 3 + 2],src[indexS - sww + 3 + 2],xx,yy);
					}else
					{
						dst[indexD + 0] = XMath::bilinear(src[indexS + 0],src[indexS - sww + 0],src[indexS - 3 + 0],src[indexS - sww - 3 + 0],xx,yy);
						dst[indexD + 1] = XMath::bilinear(src[indexS + 1],src[indexS - sww + 1],src[indexS - 3 + 1],src[indexS - sww - 3 + 1],xx,yy);
						dst[indexD + 2] = XMath::bilinear(src[indexS + 2],src[indexS - sww + 2],src[indexS - 3 + 2],src[indexS - sww - 3 + 2],xx,yy);
					}
				}
			}
		}
	}
	void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,const XVector2& srcSize,
							 const XVector2& dstSize,const XVector2& srcPos,const XVector2& srcClipSize,XPixelFillMode mode)
	{//��δ��ȫʵ��
		if(src == NULL ||
			dst == NULL) return;
		switch(mode)
		{
		case PIXEL_MODE_CENTER_TILE://����ƽ��
			{
				//memset(dst,0,dstSize.x * dstSize.y * 3);//��ʼ����������
				//λ��
				XVector2 resSize(XEE_Min(srcClipSize.x,dstSize.x),XEE_Min(srcClipSize.y,dstSize.y));				//����ȷ����ԴĿ��ߴ�
				XVector2 resDstPos((dstSize - resSize) * 0.5f);					//����ȷ����Դλ��
				XVector2 resSrcPos(srcPos + srcClipSize * 0.5f - resSize * 0.5f);	//����ȷ����Ŀ��λ��
				//���濪ʼ��������
				int w = resSize.x * 3;
				int wS = srcSize.x * 3;
				int wD = dstSize.x * 3;
				int tmpD = int(resDstPos.x) * 3;
				int tmpS = int(resSrcPos.x) * 3;
				for(int h = 0;h < resSize.y;++ h)
				{
					memcpy(dst + (int)(h + resDstPos.y) * wD + tmpD,
						src + (int)(h + resSrcPos.y) * wS + tmpS,w);
				}
			} 
			break;
		case PIXEL_MODE_ZOOM:
			{//�Զ�������Ӧ�����������̱߲���(��δʵ��)�������ͼ������ʹ��˫���Բ�ֵ����
				//�ڲ����Ǽ�����������£��Ƽ�ʹ�����ξ����
				//memset(dst,0,dstSize.x * dstSize.y * 3);//��ʼ����������
				XVector2 size = dstSize / srcClipSize;
				if(size.x > size.y)
				{//���ݽϴ��ѹ������ѹ��
					pixelDataZoomRGB(src,dst,size.y,srcSize.x,srcSize.y,dstSize.x,dstSize.y,
						srcPos.x,srcPos.y,(dstSize.x - srcClipSize.x * size.y) * 0.5f,0,srcClipSize.x,srcClipSize.y);
				}else
				{
					pixelDataZoomRGB(src,dst,size.x,srcSize.x,srcSize.y,dstSize.x,dstSize.y,
						srcPos.x,srcPos.y,0,(dstSize.y - srcClipSize.y * size.x) * 0.5f,srcClipSize.x,srcClipSize.y);
				}
			}
			break;
		}
	}
}
}