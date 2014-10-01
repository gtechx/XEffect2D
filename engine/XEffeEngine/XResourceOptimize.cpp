//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
//#include "XEffeEngine.h"
//#include "png.h"
//#include "XBasicWindow.h"
#include "SDL_image.h"
#include "XBasicClass.h"
#include "X2DPacker.h"
#include "XSavePNG.h"
#include "XResourceOptimize.h"

//�ǳ��ź�����ͼƬ��Դ�ƶ����ݲ�Ƚϵķ�ʽû����ɣ������ƶ��Ƚ����֮����Ҫ��ͼ����вü����ü�������߼��Ĵ���
//Ҳ���Ǻ�Ƚϵ�ͼƬ�޸Ĳü�����֮���֮ǰ�Ƚϵ�������ɴ���

//����ʹ���µķ�������ɵķ�����������Ч��һ���������ٶȿ�ܶ�
//�ɵķ�����������ɫ�ݲ�����ƶ��Ƚϵķ�Χ��Ȼ���ڸ��㷶Χ���ƶ�ͼƬ���Ƚ��Ƿ����������ķ�Χ�ڡ�����ͼƬ�Ƚϱ�
//������ҪС�űȽϴ�ļ��������ټ�����һ����Χ���ƶ�����αȽϣ������ΧΪ5����Ҫ�Ƚ�5*5=25�Ρ����ұȽ����֮����Ҫ
//�����ƶ��Ƚϵľ�����ֵ�����޸ı�ͼ��Ŀ��ͼ�Ĳü���Χ���⽫����ɺ���Ƚ�ͼƬ���޸�Ӱ�쵽֮ǰ�޸ĵ�ͼƬ�Ĳü����ݣ�
//������ص�bug
//�µķ������ڲü�ͼƬ��ʱ���������ɫ�ݲ�Զ�������ݲ����ڵ���ɫ�Ͳü�����Ȼ���ڱȽϵ�ʱ��Ͳ���Ҫ���ƶ���Χ�Ƚ��ˣ�
//����ֱ�ӱȽϲü������С�����Ƿ������ɫ�ݲ�����ơ�
//���ַ���Ч����ͬ��Ч�ʺ�������Ҫ�ߣ����Ҳ������bug��
struct _XPicProcData
{
	_XVector2 leftTopPoint;			//ͼƬ�ü������Ͻǲü�����
	_XVector2 rightBottomPoint;		//ͼƬ�ü������½ǲü�����
	_XVector2 picTureSize;			//ͼƬ��ԭʼ�ߴ�
	_XVector2 picOutSize;			//ͼƬ�ü�֮��ĳߴ�
	_XVector2 picOffsetPosition;	//ͼƬ���õ�ƫ������
	int textureOrder;				//ͼƬ���ܱ��
	int sameTexOrder;				//��ͬͼƬ�ı��
	SDL_Surface *pPic;				//ͼ���ָ��
	char fileName[MAX_FILE_NAME_LENGTH];				//ͼƬ������
	char outFileName[MAX_FILE_NAME_LENGTH];			//������ͼ������
	
	_XPicProcData()
	:leftTopPoint(0.0f,0.0f)
	,rightBottomPoint(0.0f,0.0f)
	,picTureSize(0.0f,0.0f)
	,picOutSize(0.0f,0.0f)
	,pPic(NULL)
	,picOffsetPosition(0.0f,0.0f)
	,textureOrder(0)
	,sameTexOrder(-1)
	{
		fileName[0] = '\0';
		outFileName[0] = '\0';
	}
	//~_XPicProcData()
	//{
	//	if(pPic != NULL) 	
	//	{
	//		SDL_FreeSurface(pPic);
	//		pPic = NULL;
	//	}
	//}
};

#define MAX_PNG_FILE_SUM (4096)
#define TEXTURE_FILE_NAME "Texture_000.png"

struct _XFileName
{
	char fileName[MAX_FILE_NAME_LENGTH];
};

#define MAX_BASICBOXSUM (1024)	//���ʹ�õ�1024�ű�׼��ͼ��
#define MAX_BOXSIZE (1024)	//��׼��ͼ�ĳߴ�Ϊ1024

int getTop(SDL_Surface *temp,int maxPixelsOffset)
{
	if(temp == NULL) return 0;
	int flag = 0;
	int top = 0;
	unsigned char *p = (unsigned char *)temp->pixels;
	unsigned char * tempC;
	for(int h = 0;h < temp->h;++ h)
	{
		flag = 0;
		tempC = p + (h << 2) * temp->w + 3;
		for(int w = 0;w < temp->w;++ w)
		{
			if(* (tempC + (w << 2)) > maxPixelsOffset)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) top = h + 1;
		else break;
	}
	return top;
}

int getBottom(SDL_Surface *temp,int maxPixelsOffset)
{
	if(temp == NULL) return 0;
	int flag = 0;
	int bottom = temp->h;
	unsigned char *p = (unsigned char *)temp->pixels;
	unsigned char *tempC;
	for(int h = temp->h - 1;h >= 0;-- h)
	{
		flag = 0;
		tempC = p + (h << 2) * temp->w + 3;
		for(int w = 0;w < temp->w;++ w)
		{
			if(* (tempC + (w << 2)) > maxPixelsOffset)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) bottom = h;
		else break;
	}
	return bottom;
}

int getLeft(SDL_Surface *temp,int maxPixelsOffset)
{
	if(temp == NULL) return 0;
	int flag = 0;
	int left = 0;
	unsigned char *p = (unsigned char *)temp->pixels;
	unsigned char *tempC;
	for(int w = 0;w < temp->w;++ w)
	{
		flag = 0;
		tempC = p + (w << 2) + 3;
		for(int h = 0;h < temp->h;++ h)
		{
			if(* (tempC + (h << 2) * temp->w) > maxPixelsOffset)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) left = w + 1;
		else break;
	}
	return left;
}

int getRight(SDL_Surface *temp,int maxPixelsOffset)
{
	if(temp == NULL) return 0;
	int flag = 0;
	int right = temp->w;
	unsigned char *p = (unsigned char *)temp->pixels;
	unsigned char * tempC;
	for(int w = temp->w - 1;w >= 0;-- w)
	{
		flag = 0;
		tempC = p + (w << 2) + 3;
		for(int h = 0;h < temp->h;++ h)
		{
			if(* (tempC + (h << 2) * temp->w) > maxPixelsOffset)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) right = w;
		else break;
	}
	return right;
}

int pictureProc(_XPicProcData *result,int maxPixelsOffset)
{
	//��ȡͼƬ
	char * tmp = ANSIToUTF8(result->fileName);
	result->pPic = IMG_Load(tmp);
	XDELETE_ARRAY(tmp);
	if(result->pPic == NULL) return 0;
	//Ѱ�Ҿ���
	int top = getTop(result->pPic,maxPixelsOffset);
	int left = getLeft(result->pPic,maxPixelsOffset);
	int bottom = getBottom(result->pPic,maxPixelsOffset);
	int right = getRight(result->pPic,maxPixelsOffset);

//	if(top >= bottom) return 0;
//	if(left >= right) return 0;
	//��������������ǵ�Ϊ��ͼƬ��ʱ��Ҳ��������ͼƬ����͸����ʱ��������������
	//����Ĵ������Ϊ�˴������������д��
	if(top >= bottom)
	{
		top = 0;
		bottom = result->pPic->h;
	}
	if(left >= right)
	{
		left = 0;
		right = result->pPic->w;
	}

	result->leftTopPoint.set(left,top);
	result->picTureSize.set(result->pPic->w,result->pPic->h);
	result->picOutSize.set(right - left,bottom - top);
	result->rightBottomPoint.set(result->pPic->w - right,result->pPic->h - bottom);
	return 1;
}

int writeFileData(_XPicProcData *picData,int allPicSum,int allOutputSum)
{
	FILE *fp;
	if((fp = fopen(PNG_INFORMATION_FILE_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return 0;
	}

	fprintf(fp,"%d,%d;\n",allPicSum,allOutputSum);
	//д������ĳһ֡����Ϣ
	for(int i = 0;i < allPicSum;++ i)
	{
		fprintf(fp,"%s:%s:%d:%d,%d:%d,%d,%d,%d:%d,%d;\n",picData[i].fileName,picData[i].outFileName,
			picData[i].textureOrder,
			(int)(picData[i].picOffsetPosition.x),(int)(picData[i].picOffsetPosition.y),
			(int)(picData[i].leftTopPoint.x),(int)(picData[i].leftTopPoint.y),
			(int)(picData[i].rightBottomPoint.x),(int)(picData[i].rightBottomPoint.y)
			,(int)(picData[i].picOutSize.x),(int)(picData[i].picOutSize.y));
	}
	fclose(fp);

	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return 0;
	}
	//д������ĳһ֡����Ϣ
	for(int i = 0;i < allPicSum;++ i)
	{
		fprintf(fp,"%s\n",picData[i].fileName);
	}
	fclose(fp);
	return 1;
}

int makePNGFileList()
{
	char temp[512];
	//����һ����׼�ļ������ҵ���׼��·��
	//+++++++++++++++++++++++++++++++++++++++++++++++++++
	//ע�⣺������Ҫע�⣬����·���²�����!!BasePath.png
	//����ļ����������ɴ���
	//---------------------------------------------------
	FILE *fp;
	if((fp = fopen("!!BasePath.png","w")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	fclose(fp);
	//��������png�ļ����б�
	sprintf(temp,"dir *.png /s/b>%s",PNG_FILE_LIST_NAME);
	system(temp);
	//��������ļ�Ϊ���·��
	//���ļ��б�
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	//����ļ��б����Ϣ
	int pathBaseDeep = 1000;
	int tempPathDeep;
	char fileNameTemp[MAX_FILE_NAME_LENGTH];

	_XFileName *fileList = NULL;
	fileList = createArrayMem<_XFileName>(MAX_PNG_FILE_SUM);
	if(fileList == NULL) return 0;

	int fileSum = 0;	//ʵ�ʲ������ļ�����
	for(int i = 0;i < MAX_PNG_FILE_SUM;++ i)
	{
		if(feof(fp)) break;

		fileNameTemp[0] = '\0';
		fscanf(fp,"%s",fileNameTemp);
		if(strlen(fileNameTemp) < 4 || fileNameTemp[strlen(fileNameTemp) - 4]  != '.') continue;
		
		int len = (int)(strlen(fileNameTemp));
		for(int j = 0;j < len;++ j)
		{
			if(fileNameTemp[j] == '\\') fileNameTemp[j] = '/';
		}
		memcpy(fileList[i].fileName,fileNameTemp,sizeof(fileNameTemp));
		
	//	if(i == 0)
	//	{//����ǻ�׼·���������ַ�ʽ�һ���·��������ȫ��
	//		pathBaseDeep = getPathDeepByChar(fileList[i].fileName);
	//	}
		//��Ϊ�ڻ�Ŀ¼�½�����һ���ļ�������ʹ�����·�����ҵķ��������ҵ�·�����
		//��������д��Ľ�
		tempPathDeep = getPathDeepByChar(fileList[i].fileName);
		if(tempPathDeep < pathBaseDeep)
		{
			pathBaseDeep = tempPathDeep;
		}

		++ fileSum;
	}
	fclose(fp);
	//��������·��
	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	for(int i = 0;i < fileSum;++ i)
	{
		fprintf(fp,"%s\n",&(fileList[i].fileName[pathBaseDeep + 1]));
	}
	fclose(fp);

	XDELETE_ARRAY(fileList);

	return fileSum - 1;	//�����Լ��½���һ���ļ���������������Ҫ-1
}

int allBasicBoxSum = 0;	//װ��ʱʹ�õ������ӵ�����
int PNGProc(_XPicProcData * picData,int picSum)
{
	//Ѱ�����İ�Χ��
	int maxW = MAX_BOXSIZE;
	int maxH = MAX_BOXSIZE;
	for(int i = 0;i< picSum;++ i)
	{
		if(getMinWellSize2n(picData[i].picOutSize.x + 1) > maxW)
		{
			maxW = getMinWellSize2n(picData[i].picOutSize.x + 1);
		}
		if(getMinWellSize2n(picData[i].picOutSize.y + 1) > maxH)
		{
			maxH = getMinWellSize2n(picData[i].picOutSize.y + 1);
		}
	}
	//ƴ��ͼƬ
	_X2DPacker tempPacker;
	//���һ������û���Ż�
	tempPacker.m_basicBox = createArrayMem<_XBasicBox>(MAX_BASICBOXSUM);	//����100������
	for(int i = 0;i < MAX_BASICBOXSUM;i ++)
	{
		tempPacker.m_basicBox[i].m_beUsed = 0;
		tempPacker.m_basicBox[i].m_objectBoxesOrder = createArrayMem<int>(picSum);
		tempPacker.m_basicBox[i].m_order = i;
		tempPacker.m_basicBox[i].m_size.set(maxW,maxH);
		tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

		tempPacker.m_basicBox[i].m_mayPosition = createArrayMem<_XVector2>(4 * picSum);
		tempPacker.m_basicBox[i].m_mayPositionSum = 1;
		tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f,0.0f);

		tempPacker.m_basicBox[i].m_mayPositionX = createArrayMem<int>(2 * picSum);
		tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
		tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
		tempPacker.m_basicBox[i].m_mayPositionY = createArrayMem<int>(2 * picSum);
		tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
		tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
	}
	tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

	tempPacker.m_objectBox = createArrayMem<_XObjectBox>(picSum);	//������MAX_OBJECTSUM�����
	int truePicSum = 0;	//������ͬ����Դ�ᱻ�Ż���������������Ҫ����ͳ��ʵ��ʹ�õ�����Դ����
	for(int i = 0;i < picSum;i ++)
	{
		if(picData[i].sameTexOrder < 0)
		{
			tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
			tempPacker.m_objectBox[truePicSum].m_setOrder = i;
			tempPacker.m_objectBox[truePicSum].m_position.set(0.0f,0.0f);
			tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
			//����Ϊ�˷�ֹͼ��ü�������ɵ���ɫ����Ҫÿ��ͼƬ֮����һ������
		//	tempPacker.m_objectBox[truePicSum].m_size = picData[i].picOutSize;
			tempPacker.m_objectBox[truePicSum].m_size.set(picData[i].picOutSize.x + 1,picData[i].picOutSize.y + 1);
			tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
			truePicSum ++;
		}
	}
	//tempPacker.m_objectBoxSum = picSum;
	tempPacker.m_objectBoxSum = truePicSum;

	tempPacker.setOptimizeLevel(1);
	if(tempPacker.findResult() == 0)
	{
		printf("Picture optimize Error!\n");
		return 0;
	}
	//�������Ż������һ������
/*	int tryTimer = 0;
	int needBoxSumTemp[MAX_BASICBOXSUM];
	int finishedFlag = 0;
	while(true)
	{
		//��ʼ�����к��ӵ���Ϣ
		tempPacker.m_basicBox = createArrayMem<_XBasicBox>(MAX_BASICBOXSUM);	//����100������
		for(int i = 0;i < MAX_BASICBOXSUM;i ++)
		{
			tempPacker.m_basicBox[i].m_beUsed = 0;
			tempPacker.m_basicBox[i].m_objectBoxesOrder = createArrayMem<int>(picSum);
			tempPacker.m_basicBox[i].m_order = i;
			if(tryTimer == 0)
			{
				tempPacker.m_basicBox[i].m_size.set(maxW,maxH);
			}else
			{
				int w = maxW;
				int h = maxH;
				for(int j = 0;j < tryTimer;++ j)
				{
					if(i < needBoxSumTemp[j] - 1) break;
					if(maxW > maxH)
					{
						if(j % 2 == 0) w = w >> 1;
						else h = h >> 1;
					}else
					{
						if(j % 2 == 0) h = h >> 1;
						else w = w >> 1;
					}
				}
				tempPacker.m_basicBox[i].m_size.set(w,h);
				if(w <= 512 && h <= 512) finishedFlag = 1;
			}
			tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

			tempPacker.m_basicBox[i].m_mayPosition = createArrayMem<_XVector2>(4 * picSum);
			tempPacker.m_basicBox[i].m_mayPositionSum = 1;
			tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f,0.0f);

			tempPacker.m_basicBox[i].m_mayPositionX = createArrayMem<int>(2 * picSum);
			tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
			tempPacker.m_basicBox[i].m_mayPositionY = createArrayMem<int>(2 * picSum);
			tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
		}
		tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

		tempPacker.m_objectBox = createArrayMem<_XObjectBox>(picSum);	//������MAX_OBJECTSUM�����
		int truePicSum = 0;
		for(int i = 0;i < picSum;i ++)
		{//���������ͬ����ͼ������Ҫ�ظ�����
			if(picData[i].sameTexOrder < 0)
			{
				tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
				tempPacker.m_objectBox[truePicSum].m_setOrder = i;
				tempPacker.m_objectBox[truePicSum].m_position.set(0.0f,0.0f);
				tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
				//����Ϊ�˷�ֹͼ��ü�������ɵ���ɫ����Ҫÿ��ͼƬ֮����һ������
			//	tempPacker.m_objectBox[truePicSum].m_size = picData[i].picOutSize;
				tempPacker.m_objectBox[truePicSum].m_size.set(picData[i].picOutSize.x + 1,picData[i].picOutSize.y + 1);
				tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
				truePicSum ++;
			}
		}
		//tempPacker.m_objectBoxSum = picSum;
		tempPacker.m_objectBoxSum = truePicSum;

		tempPacker.setOptimizeLevel(1);
		//�����Ƕ��Ѱ�ң����ҵ����õĽ⣬�����޸�ΪֻѰ��һ�Ρ�
		if(tempPacker.findResult() == 0)
		{
			if(tryTimer == 0)
			{
				printf("Picture optimize Error!\n");
				for(int i = 0;i < MAX_BASICBOXSUM;i ++)
				{
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
				}
				XDELETE_ARRAY(tempPacker.m_basicBox);
				XDELETE_ARRAY(tempPacker.m_objectBox);
				return 0;
			}else
			{//������һ�ν��
				finishedFlag = 1;
				-- tryTimer;
				for(int i = 0;i < MAX_BASICBOXSUM;++ i)
				{
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
				}
				XDELETE_ARRAY(tempPacker.m_basicBox);
				XDELETE_ARRAY(tempPacker.m_objectBox);
			}
		}else
		{
			if(finishedFlag != 0)
			{//�ҵ����յĽ��
				break;
			}else
			{
				needBoxSumTemp[tryTimer] = tempPacker.getNeedBoxSum();
				++ tryTimer;
				for(int i = 0;i < MAX_BASICBOXSUM;++ i)
				{
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
					XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
				}
				XDELETE_ARRAY(tempPacker.m_basicBox);
				XDELETE_ARRAY(tempPacker.m_objectBox);
			}
		}
	}*/
	printf("All picture files optimization.!\n");
	
	//������һ���������������ģ���Ҫ�Ż�
	//ƴ�����֮��ʼ��ͼ
	char fileName[256];
	strcpy(fileName,TEXTURE_FILE_NAME);
	for(allBasicBoxSum = 0;allBasicBoxSum < MAX_BASICBOXSUM;++ allBasicBoxSum)
	{
		if(tempPacker.m_basicBox[allBasicBoxSum].m_beUsed <= 0) break;
		//��ͼ�ν��д���
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,tempPacker.m_basicBox[allBasicBoxSum].m_size.x,tempPacker.m_basicBox[allBasicBoxSum].m_size.y,
			32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
		SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,tempPacker.m_basicBox[allBasicBoxSum].m_size.x,tempPacker.m_basicBox[allBasicBoxSum].m_size.y,
			32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (allBasicBoxSum % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((allBasicBoxSum/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((allBasicBoxSum/100) % 10) + '0';

		unsigned int *pA = (unsigned int *)picArm->pixels;
		int objBoxOrder;
		int picOrder;
		unsigned int *p;
		int h;
		unsigned int *pATemp;
		unsigned int *pTemp;
		int sizeTemp;
		for(int i = 0;i < tempPacker.m_basicBox[allBasicBoxSum].m_beUsed;++ i)
		{
			objBoxOrder = tempPacker.m_basicBox[allBasicBoxSum].m_objectBoxesOrder[i];
			picOrder = tempPacker.m_objectBox[objBoxOrder].m_setOrder;
			p = (unsigned int *)picData[picOrder].pPic->pixels;

			pATemp = pA + (int)(tempPacker.m_objectBox[objBoxOrder].m_position.y) * picArm->w
				+ (int)(tempPacker.m_objectBox[objBoxOrder].m_position.x);
			pTemp = p + (int)(picData[picOrder].leftTopPoint.y) * picData[picOrder].pPic->w
				+ (int)(picData[picOrder].leftTopPoint.x);
			sizeTemp = (int)(picData[picOrder].picOutSize.x) << 2;

			for(h = 0;h < picData[picOrder].picOutSize.y;++ h)
			{
				memcpy(pATemp + h * picArm->w,pTemp + h * picData[picOrder].pPic->w,sizeTemp);
			}
		/*	for(h = tempPacker.m_objectBox[objBoxOrder].m_position.y;
				h < tempPacker.m_objectBox[objBoxOrder].m_position.y + picData[picOrder].picOutSize.y;
				++ h)
			{
				memcpy(pA + h * picArm->w + (int)(tempPacker.m_objectBox[objBoxOrder].m_position.x),
					p + ((int)(picData[picOrder].leftTopPoint.y) + (h - (int)(tempPacker.m_objectBox[objBoxOrder].m_position.y))) * picData[picOrder].pPic->w
					+ (int)(picData[picOrder].leftTopPoint.x),
					(int)(picData[picOrder].picOutSize.x) << 2);
			}*/
			strcpy(picData[picOrder].outFileName,fileName);
			picData[picOrder].textureOrder = allBasicBoxSum;
			picData[picOrder].picOffsetPosition = tempPacker.m_objectBox[objBoxOrder].m_position;
			//���ﲢ���ͷ���Դ��������Դ�����ⲿ���ͷ�
			//SDL_FreeSurface(picData[picOrder].pPic);
			//picData[picOrder].pPic = NULL;
		}
//		if(savePNG(fileName,picArm,Z_BEST_COMPRESSION) == 0) return 0;
		savePNG(fileName,picArm,Z_BEST_COMPRESSION);
		//�ͷ���Դ
		SDL_FreeSurface(picArm);
		printf("the %d registration completed!\n",allBasicBoxSum);
	}
	//���洦����ͬͼƬ��Դ����Ϣ
	for(int i = 0;i < picSum;++ i)
	{
		if(picData[i].sameTexOrder >= 0 && picData[i].sameTexOrder < picSum)
		{
			strcpy(picData[i].outFileName,picData[picData[i].sameTexOrder].outFileName);
			picData[i].textureOrder = picData[picData[i].sameTexOrder].textureOrder;
			picData[i].picOffsetPosition = picData[picData[i].sameTexOrder].picOffsetPosition;
		}
	}
	//���濪ʼ�ͷ���Դ
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
		XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
		XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
		XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
	}
	XDELETE_ARRAY(tempPacker.m_basicBox);
	XDELETE_ARRAY(tempPacker.m_objectBox);

	printf("All pictures and optimal output complete!\n");

	return allBasicBoxSum;
}
//����ֵ1���������������ͬ 0��������Ҫ�� 2���������ز�ͬ�����Ƿ���Ҫ��
int pixelCompare(const unsigned int *p0,const unsigned int *p1,int maxPixelsOffset)
{
	unsigned char *pc0 = (unsigned char *)p0;
	unsigned char *pc1 = (unsigned char *)p1;
	if(maxPixelsOffset != 0)
	{
		if(pc0[3] < pc1[3] - maxPixelsOffset || pc0[3] > pc1[3] + maxPixelsOffset) return 0;	//�ж�͸��ͨ���Ƿ����Ҫ��
		else
		{
			if(pc0[3] == pc1[3] && pc0[3] == 0) return 1;	//�����ȫ͸��������Ҫ����������ɫ�ķ���
			if(pc0[3] < maxPixelsOffset || pc1[3] < maxPixelsOffset) return 2;	//���͸��ͨ������͸��Ҳ������������ɫ�ķ���
		}

		if(pc0[0] < pc1[0] - maxPixelsOffset || pc0[0] > pc1[0] + maxPixelsOffset) return 0;	//������Ҫ��
		if(pc0[1] < pc1[1] - maxPixelsOffset || pc0[1] > pc1[1] + maxPixelsOffset) return 0;	//������Ҫ��
		if(pc0[2] < pc1[2] - maxPixelsOffset || pc0[2] > pc1[2] + maxPixelsOffset) return 0;	//������Ҫ��

		if(*p0 == *p1) return 1;
		return 2;
	}else
	{
		if(pc0[3] == 0 && pc0[3] == pc1[3]) return 1;	//͸��ͨ����ͬ��ȫ͸��������Ҫ�Ƚ�������ɫ
		if(* p0 == * p1) return 1;
		return 0;
	}
}
//����ֵ1���������������ͬ 0��������Ҫ�� 2���������ز�ͬ�����Ƿ���Ҫ��
int pixelCompare(const unsigned int *p,int maxPixelsOffset)
{
	unsigned char *pc = (unsigned char *)p;
	if(pc[3] == 0) return 1;
	if(pc[3] > maxPixelsOffset) return 0;	//������Ҫ��
	return 2;
}

int textureCompare(_XPicProcData *tex0,_XPicProcData *tex1,int maxPixelsOffset)
{
	if(tex0 == NULL || tex1 == NULL) return 0;
	//������ͼ����Ч�ߴ������ͬ
	if(tex0->picOutSize.x != tex0->picOutSize.x || tex0->picOutSize.y != tex0->picOutSize.y) return 0;
	//�Ƚ��м��5�������
	unsigned int *p0 = (unsigned int *)tex0->pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1->pPic->pixels;
	int x0 = tex0->leftTopPoint.x + tex0->picOutSize.x / 4.0f;
	int y0 = tex0->leftTopPoint.y + tex0->picOutSize.y / 4.0f;
	int x1 = tex1->leftTopPoint.x + tex1->picOutSize.x / 4.0f;
	int y1 = tex1->leftTopPoint.y + tex1->picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
	x0 = tex0->leftTopPoint.x + (tex0->picOutSize.x / 4.0f) * 3;
	//y0 = tex0->leftTopPoint.y + tex0->picOutSize.y / 4.0f;
	x1 = tex1->leftTopPoint.x + (tex1->picOutSize.x / 4.0f) * 3;
	//y1 = tex1->leftTopPoint.y + tex1->picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
	//x0 = tex0->leftTopPoint.x + (tex0->picOutSize.x / 4.0f) * 3;
	y0 = tex0->leftTopPoint.y + (tex0->picOutSize.y / 4.0f) * 3;
	//x1 = tex1->leftTopPoint.x + (tex1->picOutSize.x / 4.0f) * 3;
	y1 = tex1->leftTopPoint.y + (tex1->picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
	x0 = tex0->leftTopPoint.x + tex0->picOutSize.x / 4.0f;
	//y0 = tex0->leftTopPoint.y + (tex0->picOutSize.y / 4.0f) * 3;
	x1 = tex1->leftTopPoint.x + tex1->picOutSize.x / 4.0f;
	//y1 = tex1->leftTopPoint.y + (tex1->picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
	x0 = tex0->leftTopPoint.x + tex0->picOutSize.x / 2.0f;
	y0 = tex0->leftTopPoint.y + tex0->picOutSize.y / 2.0f;
	x1 = tex1->leftTopPoint.x + tex1->picOutSize.x / 2.0f;
	y1 = tex1->leftTopPoint.y + tex1->picOutSize.y / 2.0f;
	if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
	//��������5�������ȫ����ͬ����Ƚ�ȫ��������
	for(int i = 0;i < tex0->picOutSize.y;++ i)
	{
		y0 = i + tex0->leftTopPoint.y;
		y1 = i + tex1->leftTopPoint.y;
		for(int j = 0;j < tex0->picOutSize.x;++ j)
		{
			x0 = j + tex0->leftTopPoint.x;
			x1 = j + tex1->leftTopPoint.x;
			if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) return 0;
		}
	}
	//����������ͬ��������ͼƬ��ͬ
	return 1;
}

//ǿ�������ͼ�ȽϺ������������ݲ�
//maxOffset ��������ͼ��Ƚ�λ��ƫ������ֵԽ�󣬻��ѵ�ʱ��Խ��
//maxPixelsOffset �������ɫƫ������ֵԽ���������ɫƫ��Խ��
//maxDiffRate (0 - 1) �����ֵ�����������ɫƫ��ĵ������
int textureCompareEx(_XPicProcData *tex0,_XPicProcData *tex1,int maxOffset,int maxPixelsOffset,float maxDiffRate)
{//��δ��ɣ�����������Ҫ������ؾ����ƶ��ıȽϣ����ڴ���ͼƬ�ƶ�֮��ĳ������⣬��������ߵıȽ��鷳
	//Ŀǰ�������Ƚ����֮��ʹ����С�ļ��������������д�������Ϣ��
	if(tex0 == NULL || tex1 == NULL) return 0;
	if(maxDiffRate < 0) maxDiffRate = 0.0f;
	if(maxDiffRate > 1.0f) maxDiffRate = 1.0f;
	//������ͼ����Ч�ߴ����С�ڵ�����Сƫ��
	if(tex0->picOutSize.x < tex1->picOutSize.x - maxOffset
		|| tex0->picOutSize.x > tex1->picOutSize.x + maxOffset
		|| tex0->picOutSize.y < tex1->picOutSize.y - maxOffset
		|| tex0->picOutSize.y > tex1->picOutSize.y + maxOffset) return 0;
	//�Ƚ��м��5�������
	int maxX,maxY;	//��Ҫ�Ƚ�����ͼƬ��С���������Ǹ��ߴ�
	if(tex0->picOutSize.x < tex1->picOutSize.x) maxX = tex0->picOutSize.x;
	else maxX = tex1->picOutSize.x;
	if(tex0->picOutSize.y < tex1->picOutSize.y) maxY = tex0->picOutSize.y;
	else maxY = tex1->picOutSize.y;
	int dx = 0,dy = 0;
	int i = 0,j = 0,ret = 0;
	int x0 = 0,y0 = 0,x1 = 0,y1 = 0;
	float diffPixSum = 0;
	unsigned int *p0 = (unsigned int *)tex0->pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1->pPic->pixels;
	int maxDiffPixelsSum = tex0->picOutSize.x * tex0->picOutSize.y * maxDiffRate * maxDiffRate;
	for(dx = -maxOffset;dx < maxOffset;++ dx)
	{
		for(dy = -maxOffset;dy < maxOffset;++ dy)
		{
			x0 = tex0->leftTopPoint.x + dx + tex0->picOutSize.x / 4.0f;
			y0 = tex0->leftTopPoint.y + dy + tex0->picOutSize.y / 4.0f;
			x1 = tex1->leftTopPoint.x + tex0->picOutSize.x / 4.0f;
			y1 = tex1->leftTopPoint.y + tex0->picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0->leftTopPoint.x + dx + (tex0->picOutSize.x / 4.0f) * 3;
		//	y0 = tex0->leftTopPoint.y + dy + tex0->picOutSize.y / 4.0f;
			x1 = tex1->leftTopPoint.x + (tex0->picOutSize.x / 4.0f) * 3;
		//	y1 = tex1->leftTopPoint.y + tex0->picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;

		//	x0 = tex0->leftTopPoint.x + dx + (tex0->picOutSize.x / 4.0f) * 3;
			y0 = tex0->leftTopPoint.y + dy + (tex0->picOutSize.y / 4.0f) * 3;
		//	x1 = tex1->leftTopPoint.x + (tex0->picOutSize.x / 4.0f) * 3;
			y1 = tex1->leftTopPoint.y + (tex0->picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0->leftTopPoint.x + dx + tex0->picOutSize.x / 4.0f;
		//	y0 = tex0->leftTopPoint.y + dy + (tex0->picOutSize.y / 4.0f) * 3;
			x1 = tex1->leftTopPoint.x + tex0->picOutSize.x / 4.0f;
			//y1 = tex1->leftTopPoint.y + (tex0->picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0->leftTopPoint.x + dx + tex0->picOutSize.x / 2.0f;
			y0 = tex0->leftTopPoint.y + dy + tex0->picOutSize.y / 2.0f;
			x1 = tex1->leftTopPoint.x + tex0->picOutSize.x / 2.0f;
			y1 = tex1->leftTopPoint.y + tex0->picOutSize.y / 2.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
				&& pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset) == 0) continue;
			//��������5�������ȫ����ͬ����Ƚ�ȫ��������
			diffPixSum = 0;
			for(i = 0;i < maxY;++ i)
			{
				y0 = i + tex0->leftTopPoint.y + dy;
				y1 = i + tex1->leftTopPoint.y;
				for(j = 0;j < maxX;++ j)
				{
					x0 = j + tex0->leftTopPoint.x + dx;
					x1 = j + tex1->leftTopPoint.x;
					if(x0 < 0 || y0 < 0 || x0 >= tex0->pPic->w || y0 >= tex0->pPic->h)
					{//ͼƬ0������Χ
						if(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
						{//ͼƬ1Ҳ������Χ������Ϊ��ͬ
							ret = 1;
						}else
						{
							ret = pixelCompare(p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset);
						}
					}else
					if(x1 < 0 || y1 < 0 || x1 > tex1->pPic->w || y1 > tex1->pPic->h)
					{//ͼƬһ������Χ
						ret = pixelCompare(p0 + y0 * tex0->pPic->w + x0,maxPixelsOffset);
					}else
					{//��������������Χ
						ret = pixelCompare(p0 + y0 * tex0->pPic->w + x0,p1 + y1 * tex1->pPic->w + x1,maxPixelsOffset);
					}
					if(ret == 0) break;
					if(ret == 2) 
					{
						diffPixSum++;
						//������Ҫ�Ƚϲ�ͬ��Ȼ����������
						if(diffPixSum > maxDiffPixelsSum)
						{
							ret = 0;
							break;
						}
					}
				}
				if(ret == 0) break;
			}
			if(ret == 0) continue;
			//���бȽ����֮��ͼƬ������Ҫ���,������Ҫ����һЩͼ�����Ϣ��ȡ��С����ͼ������Ա�֤����Խ�硣
			if(dx == 0)
			{//x����û���ƶ�
				if(tex0->picOutSize.x < tex1->picOutSize.x)	//����ü�
				{//ȡС��	
					tex1->picOutSize.x = tex0->picOutSize.x;
					tex1->rightBottomPoint.x = tex1->pPic->w - tex1->leftTopPoint.x - tex1->picOutSize.x;
				}else
				if(tex0->picOutSize.x > tex1->picOutSize.x)	//����ü�
				{
					tex0->picOutSize.x = tex1->picOutSize.x;
					tex0->rightBottomPoint.x = tex0->pPic->w - tex0->leftTopPoint.x - tex0->picOutSize.x;
				}
			}else
			if(dx < 0)
			{//����ü�
				if(tex0->picOutSize.x - dx > tex1->picOutSize.x)
				{
					tex0->leftTopPoint.x += dx;
					tex0->picOutSize.x = tex1->picOutSize.x;
					tex0->rightBottomPoint.x = tex0->pPic->w - tex0->leftTopPoint.x - tex0->picOutSize.x;
				}else
				{
					tex0->leftTopPoint.x += dx;
					tex0->picOutSize.x -= dx;
					tex1->picOutSize.x = tex0->picOutSize.x;
					tex1->rightBottomPoint.x = tex1->pPic->w - tex1->leftTopPoint.x - tex1->picOutSize.x;
				}
			}else
			{//����ü�
				if(tex0->picOutSize.x + dx > tex1->picOutSize.x)
				{
					tex1->leftTopPoint.x += dx;
					tex1->picOutSize.x -= dx;
					tex0->picOutSize.x = tex1->picOutSize.x;
					tex0->rightBottomPoint.x = tex0->pPic->w - tex0->leftTopPoint.x - tex0->picOutSize.x;
				}else
				{
					tex1->leftTopPoint.x += dx;
					tex1->picOutSize.x -= tex0->picOutSize.x;
					tex1->rightBottomPoint.x = tex1->pPic->w - tex1->leftTopPoint.x - tex1->picOutSize.x;
				}
			}

			if(dy == 0)
			{//x����û���ƶ�
				if(tex0->picOutSize.y < tex1->picOutSize.y)	//����ü�
				{//ȡС��	
					tex1->picOutSize.y = tex0->picOutSize.y;
					tex1->rightBottomPoint.y = tex1->pPic->h - tex1->leftTopPoint.y - tex1->picOutSize.y;
				}else
				if(tex0->picOutSize.y > tex1->picOutSize.y)	//����ü�
				{
					tex0->picOutSize.y = tex1->picOutSize.y;
					tex0->rightBottomPoint.y = tex0->pPic->w - tex0->leftTopPoint.y - tex0->picOutSize.y;
				}
			}else
			if(dy < 0)
			{//����ü�
				if(tex0->picOutSize.y - dy > tex1->picOutSize.y)
				{
					tex0->leftTopPoint.y += dy;
					tex0->picOutSize.y = tex1->picOutSize.y;
					tex0->rightBottomPoint.y = tex0->pPic->w - tex0->leftTopPoint.y - tex0->picOutSize.y;
				}else
				{
					tex0->leftTopPoint.y += dy;
					tex0->picOutSize.y -= dy;
					tex1->picOutSize.y = tex0->picOutSize.y;
					tex1->rightBottomPoint.y = tex1->pPic->w - tex1->leftTopPoint.y - tex1->picOutSize.y;
				}
			}else
			{//����ü�
				if(tex0->picOutSize.y + dy > tex1->picOutSize.y)
				{
					tex1->leftTopPoint.y += dy;
					tex1->picOutSize.y -= dy;
					tex0->picOutSize.y = tex1->picOutSize.y;
					tex0->rightBottomPoint.y = tex0->pPic->w - tex0->leftTopPoint.y - tex0->picOutSize.y;
				}else
				{
					tex1->leftTopPoint.y += dy;
					tex1->picOutSize.y -= tex0->picOutSize.y;
					tex1->rightBottomPoint.y = tex1->pPic->w - tex1->leftTopPoint.y - tex1->picOutSize.y;
				}
			}
			return 1;	//ͼ����ͬ
		}
	}
	//���бȽ���ɶ�����������
	return 0;
}

//ʹ�ù��߹��̵ĺ�����װ�������ߵ�ϵͳ����
_XBool ResourceOptimize()
{
	int allPicSum = 0;		//�ܹ�����Ҫ���������֡������
	allPicSum = makePNGFileList();
	if(allPicSum <= 0) 
	{
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;	//û����Դ��Ҫ�Ż���ֱ�ӷ����Ż����
	}

	_XPicProcData *picData = createArrayMem<_XPicProcData>(allPicSum);
	if(picData == NULL) return XFalse;

	FILE *fp;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return XFalse;
	}

	//��ȡͼƬ���ƺ�·��,�Լ�����ͼƬ
	char tempFileName[MAX_FILE_NAME_LENGTH];
	char tempFileName1[MAX_FILE_NAME_LENGTH];
	int len = 0;
	int needProcFileSum = 0;
	for(int i = 0;i < allPicSum + 1;++ i)
	{
		fscanf(fp,"%s",tempFileName);
		//������Ҫ������ļ��У�pictureResource,��normalResource
		strcpy(tempFileName1,tempFileName);
		len = strlen(tempFileName1);
		for(int j = 0;j < len;++j)
		{
			if(tempFileName1[j] == '/' || tempFileName1[j] == '\\')
			{
				tempFileName1[j] = '\0';
			}
		}
		//if(fileNameCompare(tempFileName1,"pictureResource")) continue;		//pictureResouce�ڲ�����Դ���ᱻ����
		//if(fileNameCompare(tempFileName1,"normalResource")) continue;		//normalResource�ڲ�����Դ���ᱻ����
		if(!fileNameCompare(tempFileName1,"pic")) continue;		//ֻ��pic�ļ���������ļ��Żᾭ���Ż�����
		if(strcmp(tempFileName,"!!BasePath.png") != 0)
		{
			strcpy(picData[needProcFileSum].fileName,tempFileName);
			if(pictureProc(&(picData[needProcFileSum]),MAX_PIXELS_OFFSET) == 0)
			{
				printf("Picture resize error:%s!\n",tempFileName);
				fclose(fp);
				XDELETE_ARRAY(picData);
				return XFalse;
			}
			++ needProcFileSum;
		}
	}
	allPicSum = needProcFileSum;	
	printf("All files resizing completed! %d\n",allPicSum);
	if(allPicSum <= 0)
	{
		fclose(fp);
		XDELETE_ARRAY(picData);
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;
	}
	//���濪ʼ�Ƚ�ͼƬ�Ƿ�����ͬ��
	int sameTexSum = 0;
	for(int i = allPicSum - 1;i > 0;-- i)
	{
		picData[i].sameTexOrder = -1;	//��ʼ��Ϊô����ͬ����Դ
		for(int j = 0;j < i;++ j)
		{
			//ע�⣺textureCompareEx���޸�����ͼƬ����Ϣ������:��Ƚϵ�ͼƬ���ܻ��޸�֮ǰ�Ƚϵ�ͼƬ�ĳߴ��������
			//������������������⸴�ӻ���������������Ĵ��ڣ����Բ�ʹ�����ֱȽϷ�ʽ
			//if(textureCompareEx(&(picData[i]),&(picData[j]),5,10,1.0f) != 0)	//���ڿ���ͨ��������һ��������ͼƬ��Դ�н���ͼ�����Ϣ
			if(textureCompare(&(picData[i]),&(picData[j]),MAX_PIXELS_OFFSET) != 0)
			{//�ҵ���ͬ��ͼƬ��Դ
				++sameTexSum;
				printf("+:%s\n-:%s\n",picData[i].fileName,picData[j].fileName);
				picData[i].sameTexOrder = j;
				break;
			}
		}
	}
	printf("The same pictures sum:%d\n",sameTexSum);
	//���濪ʼƴ��ͼƬ
	int allOutputPicSum = PNGProc(picData,allPicSum);
	if(allOutputPicSum == 0)
	{
		fclose(fp);
		XDELETE_ARRAY(picData);
		return XFalse;
	}

	//�����е�����д�뵽����֡��Ϣ�ļ���
	printf("Writing file's infomation to infomationfile!\n");
	if(writeFileData(picData,allPicSum,allOutputPicSum) == 0)
	{
		fclose(fp);
		XDELETE_ARRAY(picData);
		return XFalse;
	}
	
	fclose(fp);
	//�ͷ���Դ
	for(int i = 0;i < allPicSum;++ i)
	{
		SDL_FreeSurface(picData[i].pPic);
		picData[i].pPic = NULL;
	}
	XDELETE_ARRAY(picData);
	return XTrue;
}
void releaseTempFile()
{
#ifdef UNICODE
//	char temp[512];
	DeleteFileA("!!BasePath.png");
	DeleteFileA(PNG_FILE_LIST_NAME);
	DeleteFileA(PNG_INFORMATION_FILE_NAME);

	char fileName[256];
	strcpy(fileName,TEXTURE_FILE_NAME);
	for(int i = 0;i < allBasicBoxSum;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';;
		DeleteFileA(fileName);
	}
#else
//	char temp[512];
	DeleteFile("!!BasePath.png");
	DeleteFile(PNG_FILE_LIST_NAME);
	DeleteFile(PNG_INFORMATION_FILE_NAME);

	char fileName[256];
	strcpy(fileName,TEXTURE_FILE_NAME);
	for(int i = 0;i < allBasicBoxSum;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';;
		DeleteFile(fileName);
	}
#endif
}
void clearTempFile()
{
#ifdef UNICODE
//	char temp[512];
	DeleteFileA("!!BasePath.png");
	DeleteFileA(PNG_FILE_LIST_NAME);
	DeleteFileA(PNG_INFORMATION_FILE_NAME);

	char fileName[256];
	strcpy(fileName,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';;
		DeleteFileA(fileName);
	}
#else
//	char temp[512];
	DeleteFile("!!BasePath.png");
	DeleteFile(PNG_FILE_LIST_NAME);
	DeleteFile(PNG_INFORMATION_FILE_NAME);

	char fileName[256];
	strcpy(fileName,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';;
		DeleteFile(fileName);
	}
#endif
}