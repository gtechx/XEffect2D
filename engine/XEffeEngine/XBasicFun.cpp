//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicFun.h"
#include <stdlib.h>
#include <stdio.h>
#include "XBasicClass.h"
#include "string.h"

#ifdef SIN_BOOK
float sinData[628] = {0};

void initSinData()
{
	for(int i = 0;i < 628;++i)
	{
		sinData[i] = (float)(sin(PI2 * i /628.0));
	}
}
float xSin(float x)
{
	int temp = (int)(x * 100.0f);
	while(true)
	{
		if(temp < 0) temp += 628;
		else break;
	}
	if(temp >= 628) temp = temp % 628;

	return sinData[temp];
}
#endif

#ifdef SQRT_BOOK
float sqrtData[1000000] = {0};

void initSqrtData()
{
	sinData[0] = 0;
	for(int i = 1;i < 1000000;++i)
	{
		sqrtData[i] = sqrt((double)i);
	}
}

float xSqrt(float x)
{
	if(x < 0 || x >= 1000000) return 0;
	return sqrtData[(int)x];
}
#endif

#ifdef OS_WINDWOS
#include "windows.h"
#include "string.h"
#endif
#ifdef XEE_OS_LINUX
#include <unistd.h>           //usleep()
#include <sys/time.h>
#endif

int mySleep(unsigned long sleepUSecond)
{//ע�⵽�������С�ӳ�����Ϊ1ms������С��1ms���ӳٶ�ģ��Ϊ1ms
#ifdef XEE_OS_WINDOWS
	int delay = (int)(sleepUSecond * 0.001f);
    if(delay <=0) delay =1;
    Sleep(delay);
#endif
#ifdef XEE_OS_LINUX
//����1
//	if(sleepUSecond < 1000) sleepUSecond = 1000;
//	usleep(sleepUSecond);
//����2
/*	if(sleepUSecond < 1000) sleepUSecond = 1000;
	timespec t_timeval;
	if(sleepUSecond >= 1000000)
	{//��������ӳ�ֱ������    
		t_timeval.tv_sec = sleepUSecond /1000000;
		t_timeval.tv_nsec = (sleepUSecond % 1000000) * 1000; 
	}else
	{
		t_timeval.tv_sec = 0;
		t_timeval.tv_nsec = sleepUSecond * 1000; 
	}
	if(nanosleep(&t_timeval,NULL) == -1)
	{
		//DSHOW("sleep error!\n");
	}*/
//����3
	if(sleepUSecond < 1000) sleepUSecond = 1000;
	timeval t_timeval;
	if(sleepUSecond >= 1000000)
	{//��������ӳ�ֱ������    
		t_timeval.tv_sec = sleepUSecond /1000000;
		t_timeval.tv_usec = sleepUSecond % 1000000; 
	}else
	{
		t_timeval.tv_sec = 0;
		t_timeval.tv_usec = sleepUSecond; 
	}
	if(select(0, NULL, NULL, NULL, &t_timeval) == -1)
	{
		//DSHOW("sleep error!\n");
	}
//����4
/*	pthread_cond_t mycond;     
	pthread_mutex_t mymutex;
	
	pthread_cond_init(&mycond, NULL);
   	pthread_mutex_init(&mymutex, NULL);
	
	pthread_mutex_lock(&mymutex); 
	
	if(sleepUSecond < 10000) sleepUSecond = 10000;
	struct timeval now; 
    gettimeofday(&now, NULL); 
    
    timespec abstime;
    abstime.tv_nsec = now.tv_usec * 1000 + (sleepUSecond % 1000000) * 1000; 
    abstime.tv_sec = now.tv_sec + sleepUSecond / 1000000;
	pthread_cond_timedwait(&mycond, &mymutex, &abstime);      
  	//DSHOW("time out dispatch something...\n");   
 
	pthread_mutex_unlock(&mymutex);*/       
#endif
	return 0;	
}

#ifdef GET_ALL_PIXEL
int XEE_AllPixel = 0;
#endif

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
					dst[indexD + 0] = src[indexS + 0] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 0]
						+ xx * (1.0f - yy) * src[indexS + 3 + 0] + xx * yy * src[indexS + sww + 3 + 0];
					dst[indexD + 1] = src[indexS + 1] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 1]
						+ xx * (1.0f - yy) * src[indexS + 3 + 1] + xx * yy * src[indexS + sww + 3 + 1];
					dst[indexD + 2] = src[indexS + 2] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 2]
						+ xx * (1.0f - yy) * src[indexS + 3 + 2] + xx * yy * src[indexS + sww + 3 + 2];
				}else
				{
					dst[indexD + 0] = src[indexS + 0] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 0]
						+ xx * (1.0f - yy) * src[indexS - 3 + 0] + xx * yy * src[indexS + sww - 3 + 0];
					dst[indexD + 1] = src[indexS + 1] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 1]
						+ xx * (1.0f - yy) * src[indexS - 3 + 1] + xx * yy * src[indexS + sww - 3 + 1];
					dst[indexD + 2] = src[indexS + 2] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS + sww + 2]
						+ xx * (1.0f - yy) * src[indexS - 3 + 2] + xx * yy * src[indexS + sww - 3 + 2];
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
					dst[indexD + 0] = src[indexS + 0] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 0]
						+ xx * (1.0f - yy) * src[indexS + 3 + 0] + xx * yy * src[indexS - sww + 3 + 0];
					dst[indexD + 1] = src[indexS + 1] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 1]
						+ xx * (1.0f - yy) * src[indexS + 3 + 1] + xx * yy * src[indexS - sww + 3 + 1];
					dst[indexD + 2] = src[indexS + 2] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 2]
						+ xx * (1.0f - yy) * src[indexS + 3 + 2] + xx * yy * src[indexS - sww + 3 + 2];
				}else
				{
					dst[indexD + 0] = src[indexS + 0] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 0]
						+ xx * (1.0f - yy) * src[indexS - 3 + 0] + xx * yy * src[indexS - sww - 3 + 0];
					dst[indexD + 1] = src[indexS + 1] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 1]
						+ xx * (1.0f - yy) * src[indexS - 3 + 1] + xx * yy * src[indexS - sww - 3 + 1];
					dst[indexD + 2] = src[indexS + 2] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * src[indexS - sww + 2]
						+ xx * (1.0f - yy) * src[indexS - 3 + 2] + xx * yy * src[indexS - sww - 3 + 2];
				}
			}
		}
	}
}
void pixelDataConvertRGB(unsigned char * src,unsigned char * dst,const _XVector2& srcSize,
						 const _XVector2& dstSize,const _XVector2& srcPos,const _XVector2& srcClipSize,int mode)
{//��δ��ȫʵ��
	if(src == NULL) return;
	if(dst == NULL) return;
	if(mode == 0)
	{//����ƽ��
		memset(dst,0,dstSize.x * dstSize.y * 3);//��ʼ����������

		_XVector2 resSrcPos;	//����ȷ����Դλ��
		_XVector2 resDstPos;	//����ȷ����Ŀ��λ��
		_XVector2 resSize;		//����ȷ����ԴĿ��ߴ�
		//λ��
		if(srcClipSize.x >= dstSize.x) resSize.x = dstSize.x;
		else resSize.x = srcClipSize.x;
		if(srcClipSize.y >= dstSize.y) resSize.y = dstSize.y;
		else resSize.y = srcClipSize.y;
		resDstPos.set((dstSize - resSize) * 0.5f);
		resSrcPos.set(srcPos + srcClipSize * 0.5f - resSize * 0.5f);
		//���濪ʼ��������
		int w = resSize.x * 3;
		int wS = srcSize.x * 3;
		int wD = dstSize.x * 3;
		for(int h = 0;h < resSize.y;++ h)
		{
			memcpy(dst + (int)(h + resDstPos.y) * wD + (int)(resDstPos.x * 3),
				src + (int)(h + resSrcPos.y) * wS + (int)(resSrcPos.x * 3),w);
		}
	}else
	if(mode == 1)
	{//�Զ�������Ӧ�����������̱߲���(��δʵ��)�������ͼ������ʹ��˫���Բ�ֵ����
		//�ڲ����Ǽ�����������£��Ƽ�ʹ�����ξ����
		memset(dst,0,dstSize.x * dstSize.y * 3);//��ʼ����������
		_XVector2 size = dstSize / srcClipSize;
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
}
#ifdef XEE_OS_WINDOWS
void shutDownSystem(_XShutDownSystemMode mode)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		switch(mode)
		{
		case SYS_SD_MODE_G:
			//InitiateSystemShutdownEx(NULL,NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//�ر�����ļ����
			//InitiateSystemShutdownEx("192.168.0.1",NULL,0,TRUE,FALSE,SHTDN_REASON_MAJOR_APPLICATION);	//�ر�Զ�̵ļ����
			ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
			break;
		case SYS_SD_MODE_Z:
			ExitWindowsEx(EWX_LOGOFF,0); 
			break;
		case SYS_SD_MODE_C:
			ExitWindowsEx(EWX_REBOOT,0);
			break;
		}
	}
}
#endif