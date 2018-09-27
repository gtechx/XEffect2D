#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "SDL_image.h"
#include "XMath/X2DPacker.h"
#include "XSavePNG.h"
#include "XResourceOptimize.h"
#include "XFile.h"
namespace XE{
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
struct XPicProcData
{
	XVec2 leftTopPoint;			//ͼƬ�ü������Ͻǲü�����
	XVec2 rightBottomPoint;		//ͼƬ�ü������½ǲü�����
	XVec2 picTureSize;			//ͼƬ��ԭʼ�ߴ�
	XVec2 picOutSize;			//ͼƬ�ü�֮��ĳߴ�
	XVec2 picOffsetPosition;		//ͼƬ���õ�ƫ������ ����ͼƬ�е�λ��
	int textureOrder;				//ͼƬ���ܱ��
	int sameTexOrder;				//��ͬͼƬ�ı��
	SDL_Surface *pPic;				//ͼ���ָ��
	//char fileName[MAX_FILE_NAME_LENGTH];			//ͼƬ������
	//char outFileName[MAX_FILE_NAME_LENGTH];			//������ͼ������
	std::string fileName;
	std::string outFileName;
	
	XPicProcData()
		:leftTopPoint(0.0f)
		,rightBottomPoint(0.0f)
		,picTureSize(0.0f)
		,picOutSize(0.0f)
		,pPic(NULL)
		,picOffsetPosition(0.0f)
		,textureOrder(0)
		,sameTexOrder(-1)
	{}
	~XPicProcData()	{/*releaseImage();*/}	//���������ͷ���Դ�����������ڴ�����
	bool loadImage()
	{
		if(pPic != NULL) return true;	//�Ѿ����룬����������
		char *tmp = XString::ANSIToUTF8(fileName.c_str());
		pPic = IMG_Load(tmp);
		XMem::XDELETE_ARRAY(tmp);
		if(pPic == NULL) return false;
		return true;
	}
	void releaseImage()
	{
		if(pPic == NULL) return;
		SDL_FreeSurface(pPic);
		pPic = NULL;
	}
};

#define TEXTURE_FILE_NAME "Texture_000.png"
#define MAX_BASICBOXSUM (1024)	//���ʹ�õ�1024�ű�׼��ͼ��
#define MAX_BOXSIZE (1024)		//��׼��ͼ�ĳߴ�Ϊ1024
//�������������ҵ�ͼ���͸�����ֵ��ϱ߽�
int getPixelsTop(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)	//͸���ȴ���maxPixelsOffset�����ֵʱ��Ϊ�ǲ����и�Ĳ���
{
	if(pixels == NULL) return 0;
	int size = width * height * 4;
	for(int i = 3;i < size;i += 4)
	{
		if(pixels[i] > maxPixelsOffset)
			return i / (width << 2);
	}
	return 0;
}
//�������������ҵ�ͼ���͸�����ֵ��±߽�
int getPixelsBottom(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	int size = width * height * 4;
	for(int i = size - 1;i >= 0;i -= 4)
	{
		if(pixels[i] > maxPixelsOffset)
			return i / (width << 2);
	}
	return 0;
}
//�������������ҵ�ͼ���͸�����ֵ���߽�
int getPixelsLeft(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	bool flag = false;
	int left = 0;
	const unsigned char *tempC;
	int tmpWidth = width << 2;	//һ�����ص�����
	for(int w = 0;w < width;++ w)	//������Գ��Խ����ڴ�ĺ����Ż�
	{
		flag = false;
		tempC = pixels + (w << 2) + 3;
		for(int h = 0;h < height;++ h,tempC += tmpWidth)
		{
			if(* tempC > maxPixelsOffset)
			{
				flag = true;
				break;
			}
		}
		if(flag) break;
		else left = w + 1;
	}
	return left;
}
//�������������ҵ�ͼ���͸�����ֵ��ұ߽�
int getPixelsRight(const unsigned char *pixels,
				 int width,int height,int maxPixelsOffset)
{
	if(pixels == NULL) return 0;
	bool flag = false;
	int right = width;
	const unsigned char * tempC;
	int tmpWidth = width << 2;	//һ�����ص�����
	for(int w = width - 1;w >= 0;-- w)	//������Գ��Խ����ڴ�ĺ����Ż�	
	{
		flag = false;
		tempC = pixels + (w << 2) + 3;
		for(int h = 0;h < height;++ h,tempC += tmpWidth)
		{
			if(*tempC > maxPixelsOffset)
			{
				flag = true;
				break;
			}
		}
		if(flag) break;
		else right = w;
	}
	return right;
}
//ȷ��ͼƬ��Ч���ֵĴ�С
bool XResourceOptimize::getPictureRect(XPicProcData &result,int maxPixelsOffset)
{
	//��ȡͼƬ
	if(!result.loadImage()) return false;
	//Ѱ�Ҿ���
	int top = 0,left = 0,bottom = result.pPic->h - 1,right = result.pPic->w - 1;
	if(result.pPic->format->BytesPerPixel == 4)
	{
		top = getPixelsTop((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		left = getPixelsLeft((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		bottom = getPixelsBottom((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
		right = getPixelsRight((unsigned char *)result.pPic->pixels,result.pPic->w,result.pPic->h,maxPixelsOffset);
	}

//	if(top >= bottom) return 0;
//	if(left >= right) return 0;
	//��������������ǵ�Ϊ��ͼƬ��ʱ��Ҳ��������ͼƬ����͸����ʱ��������������
	//����Ĵ������Ϊ�˴������������д��
	if(top >= bottom)
	{
		top = 0;
		bottom = result.pPic->h;
	}
	if(left >= right)
	{
		left = 0;
		right = result.pPic->w;
	}

	result.leftTopPoint.set(left,top);
	result.picTureSize.set(result.pPic->w,result.pPic->h);
	result.picOutSize.set(right - left,bottom - top);
	result.rightBottomPoint.set(result.pPic->w - right,result.pPic->h - bottom);
	
	if(!m_isOptimizeInMem)
		result.releaseImage();
	return true;
}
int XResourceOptimize::imagePack(std::vector<XPicProcData> &picData,bool withOpti)
{
	int allBasicBoxSum = 0;	//װ��ʱʹ�õ������ӵ�����
	//Ѱ�����İ�Χ��
	int maxW = MAX_BOXSIZE;
	int maxH = MAX_BOXSIZE;
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		if(XMath::getMinWellSize2n(it->picOutSize.x + 1) > maxW)
		{
			maxW = XMath::getMinWellSize2n(it->picOutSize.x + 1);
		}
		if(XMath::getMinWellSize2n(it->picOutSize.y + 1) > maxH)
		{
			maxH = XMath::getMinWellSize2n(it->picOutSize.y + 1);
		}
	}
	//ƴ��ͼƬ
	X2DPacker tempPacker;
	//����ƴ�ϵĲ���
	//tempPacker.setOptimizeLevel(true);
	//tempPacker.setCanRotate(false);	//������ת
	if(!withOpti)
	{
		//���һ������û���Ż�
		tempPacker.m_basicBox = XMem::createArrayMem<XBasicBox>(MAX_BASICBOXSUM);	//����100������
		for(int i = 0;i < MAX_BASICBOXSUM;i ++)
		{
			tempPacker.m_basicBox[i].m_beUsed = 0;
			tempPacker.m_basicBox[i].m_objectBoxesOrder = XMem::createArrayMem<int>(picData.size());
			tempPacker.m_basicBox[i].m_order = i;
			tempPacker.m_basicBox[i].m_size.set(maxW,maxH);
			tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

			tempPacker.m_basicBox[i].m_mayPosition = XMem::createArrayMem<XVec2>(4 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionSum = 1;
			tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f);

			tempPacker.m_basicBox[i].m_mayPositionX = XMem::createArrayMem<int>(2 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
			tempPacker.m_basicBox[i].m_mayPositionY = XMem::createArrayMem<int>(2 * picData.size());
			tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
			tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
		}
		tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

		tempPacker.m_objectBox = XMem::createArrayMem<XObjectBox>(picData.size());	//������MAX_OBJECTSUM�����
		int truePicSum = 0;	//������ͬ����Դ�ᱻ�Ż���������������Ҫ����ͳ��ʵ��ʹ�õ�����Դ����
		for(auto it = picData.begin();it != picData.end();++ it)
		{
			if (it->sameTexOrder >= 0) continue;
			tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
			tempPacker.m_objectBox[truePicSum].m_setOrder = it - picData.begin();
			tempPacker.m_objectBox[truePicSum].m_position.reset();
			tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
			//����Ϊ�˷�ֹͼ��ü�������ɵ���ɫ����Ҫÿ��ͼƬ֮����һ������
		//	tempPacker.m_objectBox[truePicSum].m_size = it->picOutSize;
			tempPacker.m_objectBox[truePicSum].m_size.set(it->picOutSize.x + 1,it->picOutSize.y + 1);
			tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
			++ truePicSum;
		}
		//tempPacker.m_objectBoxSum = picSum;
		tempPacker.m_objectBoxSum = truePicSum;

		tempPacker.setOptimizeLevel(true);
		if(tempPacker.findResult() == 0)
		{
			printf("Picture optimize Error!\n");
			return 0;
		}
	}else
	{
		//�������Ż������һ������
		int tryTimer = 0;
		int needBoxSumTemp[MAX_BASICBOXSUM];
		int finishedFlag = 0;
		int curOrderMode = 0;	//����ķ�ʽ
		//����ı������ڼ�¼��õĽ��
		int bestTryTimer = -1;	//��Ч
		int bestTryMode = 0;
		int bestNeedBoxSum[MAX_BASICBOXSUM];
		float bestRate = 0.0f;
		while(true)
		{
			//��ʼ�����к��ӵ���Ϣ
			tempPacker.m_basicBox = XMem::createArrayMem<XBasicBox>(MAX_BASICBOXSUM);	//����100������
			for(int i = 0;i < MAX_BASICBOXSUM;++ i)
			{
				tempPacker.m_basicBox[i].m_beUsed = 0;
				tempPacker.m_basicBox[i].m_objectBoxesOrder = XMem::createArrayMem<int>(picData.size());
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
					//if(w <= 64 && h <= 64) 
					//{
					//	finishedFlag = 1;
					//	printf("Error!\n");
					//}
				}
				tempPacker.m_basicBox[i].m_releaseAcreage = tempPacker.m_basicBox[i].m_size.x * tempPacker.m_basicBox[i].m_size.y;

				tempPacker.m_basicBox[i].m_mayPosition = XMem::createArrayMem<XVec2>(4 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionSum = 1;
				tempPacker.m_basicBox[i].m_mayPosition[0].set(0.0f);

				tempPacker.m_basicBox[i].m_mayPositionX = XMem::createArrayMem<int>(2 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionXSum = 1;
				tempPacker.m_basicBox[i].m_mayPositionX[0] = 0;
				tempPacker.m_basicBox[i].m_mayPositionY = XMem::createArrayMem<int>(2 * picData.size());
				tempPacker.m_basicBox[i].m_mayPositionYSum = 1;
				tempPacker.m_basicBox[i].m_mayPositionY[0] = 0;
			}
			tempPacker.m_basicBoxSum = MAX_BASICBOXSUM;

			tempPacker.m_objectBox = XMem::createArrayMem<XObjectBox>(picData.size());	//������MAX_OBJECTSUM�����
			int truePicSum = 0;
			for(auto it = picData.begin();it != picData.end();++ it)
			{//���������ͬ����ͼ������Ҫ�ظ�����
				if(it->sameTexOrder >= 0) continue;
				tempPacker.m_objectBox[truePicSum].m_order = truePicSum;
				tempPacker.m_objectBox[truePicSum].m_setOrder = it - picData.begin();
				tempPacker.m_objectBox[truePicSum].m_position.reset();
				tempPacker.m_objectBox[truePicSum].m_basicBoxOrder = -1;
				//����Ϊ�˷�ֹͼ��ü�������ɵ���ɫ����Ҫÿ��ͼƬ֮����һ������
			//	tempPacker.m_objectBox[truePicSum].m_size = it->picOutSize;
				tempPacker.m_objectBox[truePicSum].m_size.set(it->picOutSize.x + 1, it->picOutSize.y + 1);
				tempPacker.m_objectBox[truePicSum].m_acreage = tempPacker.m_objectBox[truePicSum].m_size.x * tempPacker.m_objectBox[truePicSum].m_size.y;
				++ truePicSum;
			}
			//tempPacker.m_objectBoxSum = picSum;
			tempPacker.m_objectBoxSum = truePicSum;

			tempPacker.setOptimizeLevel(true);
			//�����Ƕ��Ѱ�ң����ҵ����õĽ⣬�����޸�ΪֻѰ��һ�Ρ�
			if(tempPacker.findResult(curOrderMode) == 0)
			{
				for(int i = 0;i < MAX_BASICBOXSUM;++ i)
				{
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
				}
				XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
				XMem::XDELETE_ARRAY(tempPacker.m_objectBox);
				++ curOrderMode;
				tryTimer = 0;
				if(curOrderMode == 3)
				{//���г������
					if(bestTryTimer < 0)
					{
						printf("Picture optimize Error!\n");
						return -1;
					}else
					{
						tryTimer = bestTryTimer;
						curOrderMode = bestTryMode;
						memcpy(needBoxSumTemp,bestNeedBoxSum,MAX_BASICBOXSUM * sizeof(int));
						finishedFlag = 1;	//���Ľ��
					}
				}
			}else
			{
				if(finishedFlag != 0)
				{//�ҵ����յĽ��
					break;
				}else
				{
					needBoxSumTemp[tryTimer] = tempPacker.getNeedBoxSum();
					float tmpRate = tempPacker.getUseRate();
					if(tmpRate > bestRate)
					{//���������ã����¼
						bestTryTimer = tryTimer;
						bestTryMode = curOrderMode;
						memcpy(bestNeedBoxSum,needBoxSumTemp,MAX_BASICBOXSUM * sizeof(int));
						bestRate = tmpRate;
						printf("T:%d,O:%d,R:%f\n",tryTimer,curOrderMode,tmpRate);
					}
					++ tryTimer;
					for(int i = 0;i < MAX_BASICBOXSUM;++ i)
					{
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
						XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
					}
					XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
					XMem::XDELETE_ARRAY(tempPacker.m_objectBox);
				}
			}
		}
	}
	printf("All picture files optimization.!\n");
	
	//������һ���������������ģ���Ҫ�Ż�
	//ƴ�����֮��ʼ��ͼ
	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
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
			
			if(!m_isOptimizeInMem)
				if(!picData[picOrder].loadImage()) return 0;
			//���PNG����͸��ͨ����������������
			if(picData[picOrder].pPic->format->BytesPerPixel == 4)
			{
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
			}else
			{
				printf("����͸��ͨ����PNGͼƬ��������⣬���޸ĺ��ٽ����Ż�!\n");
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
			picData[picOrder].outFileName = fileName;
			picData[picOrder].textureOrder = allBasicBoxSum;
			picData[picOrder].picOffsetPosition = tempPacker.m_objectBox[objBoxOrder].m_position;
			
			if(!m_isOptimizeInMem)
				picData[picOrder].releaseImage();
		}
//		if(savePNG(fileName,picArm,Z_BEST_COMPRESSION) == 0) return 0;
		XPng::savePNG(fileName,picArm,Z_BEST_COMPRESSION);
		//�ͷ���Դ
		SDL_FreeSurface(picArm);
		printf("the %d registration completed!\n",allBasicBoxSum);
	}
	//���洦����ͬͼƬ��Դ����Ϣ
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		if(it->sameTexOrder >= 0 && it->sameTexOrder < picData.size())
		{
			it->outFileName = picData[it->sameTexOrder].outFileName;
			it->textureOrder = picData[it->sameTexOrder].textureOrder;
			it->picOffsetPosition = picData[it->sameTexOrder].picOffsetPosition;
		}
	}
	//���濪ʼ�ͷ���Դ
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_objectBoxesOrder);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPosition);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionX);
		XMem::XDELETE_ARRAY(tempPacker.m_basicBox[i].m_mayPositionY);
	}
	XMem::XDELETE_ARRAY(tempPacker.m_basicBox);
	XMem::XDELETE_ARRAY(tempPacker.m_objectBox);

	printf("All pictures and optimal output complete!\n");

	return allBasicBoxSum;
}
//����ֵ1���������������ͬ 0��������Ҫ�� 2���������ز�ͬ�������ݲΧ��
inline int pixelCompare(const unsigned int *p0,const unsigned int *p1,int maxPixelsOffset)
{
	unsigned char *pc0 = (unsigned char *)p0;
	unsigned char *pc1 = (unsigned char *)p1;
	if(pc0[3] == 0 && pc0[3] == pc1[3]) return 1;	//͸��ͨ����ͬ��ȫ͸��������Ҫ�Ƚ�������ɫ
	if(* p0 == * p1) return 1;
	if(maxPixelsOffset != 0)
	{
		if(pc0[3] < pc1[3] - maxPixelsOffset || pc0[3] > pc1[3] + maxPixelsOffset) return 0;	//�ж�͸��ͨ���Ƿ����Ҫ��
		else if(pc0[3] < maxPixelsOffset || pc1[3] < maxPixelsOffset) return 2;	//���͸��ͨ������͸��Ҳ������������ɫ�ķ���

		if(pc0[0] < pc1[0] - maxPixelsOffset || pc0[0] > pc1[0] + maxPixelsOffset) return 0;	//������Ҫ��
		if(pc0[1] < pc1[1] - maxPixelsOffset || pc0[1] > pc1[1] + maxPixelsOffset) return 0;	//������Ҫ��
		if(pc0[2] < pc1[2] - maxPixelsOffset || pc0[2] > pc1[2] + maxPixelsOffset) return 0;	//������Ҫ��

		return 2;
	}
	return 0;
}
//����ֵ1���������������ͬ 0��������Ҫ�� 2���������ز�ͬ���������ݲΧ��
inline int pixelCompare(const unsigned int *p,int maxPixelsOffset)
{
	unsigned char *pc = (unsigned char *)p;
	if(pc[3] == 0) return 1;
	if(pc[3] > maxPixelsOffset) return 0;	//������Ҫ��
	return 2;
}
//maxPixelsOffsetΪ�ݲ���ݲΧ�ڵ�ͼƬ����Ϊ��ͬ
//��������ͼƬ�Ƿ���ͬ
bool XResourceOptimize::imageCompare(XPicProcData &tex0,XPicProcData &tex1,int maxPixelsOffset)
{
	//������ͼ����Ч�ߴ������ͬ
	if(tex0.picOutSize.x != tex0.picOutSize.x || tex0.picOutSize.y != tex0.picOutSize.y) return false;
	//�Ƚ��м��5�������
	if(!m_isOptimizeInMem)
		if(!tex1.loadImage()) return false;
	unsigned int *p0 = (unsigned int *)tex0.pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1.pPic->pixels;
	int x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
	int y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
	int x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 4.0f;
	int y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
	//y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
	x1 = tex1.leftTopPoint.x + (tex1.picOutSize.x / 4.0f) * 3;
	//y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 4.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	//x0 = tex0.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
	y0 = tex0.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
	//x1 = tex1.leftTopPoint.x + (tex1.picOutSize.x / 4.0f) * 3;
	y1 = tex1.leftTopPoint.y + (tex1.picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
	//y0 = tex0.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
	x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 4.0f;
	//y1 = tex1.leftTopPoint.y + (tex1.picOutSize.y / 4.0f) * 3;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,
		p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	x0 = tex0.leftTopPoint.x + tex0.picOutSize.x / 2.0f;
	y0 = tex0.leftTopPoint.y + tex0.picOutSize.y / 2.0f;
	x1 = tex1.leftTopPoint.x + tex1.picOutSize.x / 2.0f;
	y1 = tex1.leftTopPoint.y + tex1.picOutSize.y / 2.0f;
	if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,
		p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0)
	{
		if(!m_isOptimizeInMem)
			tex1.releaseImage();
		return false;
	}
	//��������5�������ȫ����ͬ����Ƚ�ȫ��������
	for(int i = 0;i < tex0.picOutSize.y;++ i)
	{
		y0 = (i + tex0.leftTopPoint.y) * tex0.pPic->w + tex0.leftTopPoint.x;
		y1 = (i + tex1.leftTopPoint.y) * tex1.pPic->w + tex1.leftTopPoint.x;
		for(int j = 0;j < tex0.picOutSize.x;++ j)
		{
			if(pixelCompare(p0 + y0 + j,
				p1 + y1 + j,maxPixelsOffset) == 0) 
			{
				if(!m_isOptimizeInMem)
					tex1.releaseImage();
				return false;
			}
		}
	}
	//����������ͬ��������ͼƬ��ͬ
	if(!m_isOptimizeInMem)
		tex1.releaseImage();
	return true;
}

//ǿ�������ͼ�ȽϺ������������ݲ�
//maxOffset ��������ͼ��Ƚ�λ��ƫ������ֵԽ�󣬻��ѵ�ʱ��Խ��
//maxPixelsOffset �������ɫƫ������ֵԽ���������ɫƫ��Խ��
//maxDiffRate (0 - 1) �����ֵ�����������ɫƫ��ĵ������
bool XResourceOptimize::imageCompareEx(XPicProcData &tex0,XPicProcData &tex1,int maxOffset,int maxPixelsOffset,float maxDiffRate)
{//��δ��ɣ�����������Ҫ������ؾ����ƶ��ıȽϣ����ڴ���ͼƬ�ƶ�֮��ĳ������⣬��������ߵıȽ��鷳
	//Ŀǰ�������Ƚ����֮��ʹ����С�ļ��������������д�������Ϣ��
	if(maxDiffRate < 0) maxDiffRate = 0.0f;
	if(maxDiffRate > 1.0f) maxDiffRate = 1.0f;
	//������ͼ����Ч�ߴ����С�ڵ�����Сƫ��
	if(tex0.picOutSize.x < tex1.picOutSize.x - maxOffset
		|| tex0.picOutSize.x > tex1.picOutSize.x + maxOffset
		|| tex0.picOutSize.y < tex1.picOutSize.y - maxOffset
		|| tex0.picOutSize.y > tex1.picOutSize.y + maxOffset) return false;
	//�Ƚ��м��5�������
	int maxX,maxY;	//��Ҫ�Ƚ�����ͼƬ��С���������Ǹ��ߴ�
	if(tex0.picOutSize.x < tex1.picOutSize.x) maxX = tex0.picOutSize.x;
	else maxX = tex1.picOutSize.x;
	if(tex0.picOutSize.y < tex1.picOutSize.y) maxY = tex0.picOutSize.y;
	else maxY = tex1.picOutSize.y;
	int dx = 0,dy = 0;
	int i = 0,j = 0,ret = 0;
	int x0 = 0,y0 = 0,x1 = 0,y1 = 0;
	float diffPixSum = 0;
	
	if(!m_isOptimizeInMem)
		if(!tex1.loadImage()) return false;

	unsigned int *p0 = (unsigned int *)tex0.pPic->pixels;
	unsigned int *p1 = (unsigned int *)tex1.pPic->pixels;
	int maxDiffPixelsSum = tex0.picOutSize.x * tex0.picOutSize.y * maxDiffRate * maxDiffRate;
	for(dx = -maxOffset;dx < maxOffset;++ dx)
	{
		for(dy = -maxOffset;dy < maxOffset;++ dy)
		{
			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 4.0f;
			y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 4.0f;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
			y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + (tex0.picOutSize.x / 4.0f) * 3;
		//	y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 4.0f;
			x1 = tex1.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
		//	y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 4.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

		//	x0 = tex0.leftTopPoint.x + dx + (tex0.picOutSize.x / 4.0f) * 3;
			y0 = tex0.leftTopPoint.y + dy + (tex0.picOutSize.y / 4.0f) * 3;
		//	x1 = tex1.leftTopPoint.x + (tex0.picOutSize.x / 4.0f) * 3;
			y1 = tex1.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 4.0f;
		//	y0 = tex0.leftTopPoint.y + dy + (tex0.picOutSize.y / 4.0f) * 3;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 4.0f;
			//y1 = tex1.leftTopPoint.y + (tex0.picOutSize.y / 4.0f) * 3;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;

			x0 = tex0.leftTopPoint.x + dx + tex0.picOutSize.x / 2.0f;
			y0 = tex0.leftTopPoint.y + dy + tex0.picOutSize.y / 2.0f;
			x1 = tex1.leftTopPoint.x + tex0.picOutSize.x / 2.0f;
			y1 = tex1.leftTopPoint.y + tex0.picOutSize.y / 2.0f;
			if((x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
				&& !(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			else if((x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
				&& pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset) == 0) continue;
			else if(pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset) == 0) continue;
			//��������5�������ȫ����ͬ����Ƚ�ȫ��������
			diffPixSum = 0;
			for(i = 0;i < maxY;++ i)
			{
				y0 = i + tex0.leftTopPoint.y + dy;
				y1 = i + tex1.leftTopPoint.y;
				for(j = 0;j < maxX;++ j)
				{
					x0 = j + tex0.leftTopPoint.x + dx;
					x1 = j + tex1.leftTopPoint.x;
					if(x0 < 0 || y0 < 0 || x0 >= tex0.pPic->w || y0 >= tex0.pPic->h)
					{//ͼƬ0������Χ
						if(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
						{//ͼƬ1Ҳ������Χ������Ϊ��ͬ
							ret = 1;
						}else
						{
							ret = pixelCompare(p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset);
						}
					}else
					if(x1 < 0 || y1 < 0 || x1 > tex1.pPic->w || y1 > tex1.pPic->h)
					{//ͼƬһ������Χ
						ret = pixelCompare(p0 + y0 * tex0.pPic->w + x0,maxPixelsOffset);
					}else
					{//��������������Χ
						ret = pixelCompare(p0 + y0 * tex0.pPic->w + x0,p1 + y1 * tex1.pPic->w + x1,maxPixelsOffset);
					}
					if(ret == 0) break;
					if(ret == 2) 
					{
						++ diffPixSum;
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
				if(tex0.picOutSize.x < tex1.picOutSize.x)	//����ü�
				{//ȡС��	
					tex1.picOutSize.x = tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}else
				if(tex0.picOutSize.x > tex1.picOutSize.x)	//����ü�
				{
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}
			}else
			if(dx < 0)
			{//����ü�
				if(tex0.picOutSize.x - dx > tex1.picOutSize.x)
				{
					tex0.leftTopPoint.x += dx;
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}else
				{
					tex0.leftTopPoint.x += dx;
					tex0.picOutSize.x -= dx;
					tex1.picOutSize.x = tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}
			}else
			{//����ü�
				if(tex0.picOutSize.x + dx > tex1.picOutSize.x)
				{
					tex1.leftTopPoint.x += dx;
					tex1.picOutSize.x -= dx;
					tex0.picOutSize.x = tex1.picOutSize.x;
					tex0.rightBottomPoint.x = tex0.pPic->w - tex0.leftTopPoint.x - tex0.picOutSize.x;
				}else
				{
					tex1.leftTopPoint.x += dx;
					tex1.picOutSize.x -= tex0.picOutSize.x;
					tex1.rightBottomPoint.x = tex1.pPic->w - tex1.leftTopPoint.x - tex1.picOutSize.x;
				}
			}

			if(dy == 0)
			{//x����û���ƶ�
				if(tex0.picOutSize.y < tex1.picOutSize.y)	//����ü�
				{//ȡС��	
					tex1.picOutSize.y = tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->h - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}else
				if(tex0.picOutSize.y > tex1.picOutSize.y)	//����ü�
				{
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}
			}else
			if(dy < 0)
			{//����ü�
				if(tex0.picOutSize.y - dy > tex1.picOutSize.y)
				{
					tex0.leftTopPoint.y += dy;
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}else
				{
					tex0.leftTopPoint.y += dy;
					tex0.picOutSize.y -= dy;
					tex1.picOutSize.y = tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->w - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}
			}else
			{//����ü�
				if(tex0.picOutSize.y + dy > tex1.picOutSize.y)
				{
					tex1.leftTopPoint.y += dy;
					tex1.picOutSize.y -= dy;
					tex0.picOutSize.y = tex1.picOutSize.y;
					tex0.rightBottomPoint.y = tex0.pPic->w - tex0.leftTopPoint.y - tex0.picOutSize.y;
				}else
				{
					tex1.leftTopPoint.y += dy;
					tex1.picOutSize.y -= tex0.picOutSize.y;
					tex1.rightBottomPoint.y = tex1.pPic->w - tex1.leftTopPoint.y - tex1.picOutSize.y;
				}
			}
			if(!m_isOptimizeInMem)
				tex1.releaseImage();
			return true;	//ͼ����ͬ
		}
	}
	//���бȽ���ɶ�����������
	if(!m_isOptimizeInMem)
		tex1.releaseImage();
	return false;
}
bool XResourceOptimize::optimize(bool optInMem,bool withOpti)		//�Ż�
{
	m_isOptimizeInMem = optInMem;
	clearTempFile();
	int allPicSum = makePNGFileList();//�ܹ�����Ҫ���������֡������
	if(allPicSum <= 0) 
	{
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;	//û����Դ��Ҫ�Ż���ֱ�ӷ����Ż����
	}

	std::vector<XPicProcData> picDatas;

	FILE *fp;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return XFalse;
	}

	//��ȡͼƬ���ƺ�·��,�Լ�����ͼƬ
	char tempFileName[MAX_FILE_NAME_LENGTH] = "";
	char tempFileName1[MAX_FILE_NAME_LENGTH] = "";
	int len = 0;
	for(int i = 0;i < allPicSum;++ i)
	{
		XPicProcData tmp;
		if(fscanf(fp,"%s",tempFileName) != 1) return XFalse;
		//������Ҫ������ļ��У�pictureResource,��normalResource
		strcpy_s(tempFileName1,MAX_FILE_NAME_LENGTH,tempFileName);
		len = strlen(tempFileName1);
		for(int j = 0;j < len;++j)
		{
			if(tempFileName1[j] == '/' || tempFileName1[j] == '\\')
			{
				tempFileName1[j] = '\0';
			}
		}
		if(m_needOptimizeFolder.size() > 0)
		{
			bool tmpFlag = false;
			for(auto kt = m_needOptimizeFolder.begin();kt != m_needOptimizeFolder.end();++ kt)
			{
				if (XFile::fileNameCompare(tempFileName1, (*kt).c_str())) continue;		//ֻ����ɸѡĿ¼���ļ���
				tmpFlag = true;
				break;
			}
			if(tmpFlag) continue;
		}else
		if(m_needNotOptimizeFolder.size() > 0)
		{
			bool tmpFlag = false;
			for (auto kt = m_needNotOptimizeFolder.begin(); kt != m_needNotOptimizeFolder.end(); ++kt)
			{
				if (!XFile::fileNameCompare(tempFileName1, (*kt).c_str())) continue;		//�������ɸѡĿ¼�µ�ͼƬ
				tmpFlag = true;
				break;
			}
			if(tmpFlag) continue;
		}
		tmp.fileName = tempFileName;
		if(!getPictureRect(tmp,MAX_PIXELS_OFFSET))
		{
			printf("Picture resize error:%s!\n",tempFileName);
			fclose(fp);
			return XFalse;
		}
		picDatas.push_back(tmp);
	}	
#ifdef _WIN64
	printf("All files resizing completed! %zd\n",picDatas.size());
#else
	printf("All files resizing completed! %d\n",picDatas.size());
#endif
	if(picDatas.size() <= 0)
	{
		fclose(fp);
#ifdef UNICODE
		DeleteFileA(PNG_FILE_LIST_NAME);
#else
		DeleteFile(PNG_FILE_LIST_NAME);
#endif
		return XTrue;
	}
	//���濪ʼ�Ƚ�ͼƬ�Ƿ�����ͬ��
	int sameTexSum = 0;
	for(int i = int(picDatas.size()) - 1;i > 0;-- i)
	{
		picDatas[i].sameTexOrder = -1;	//��ʼ��Ϊô����ͬ����Դ
		if(!m_isOptimizeInMem)
			if(!picDatas[i].loadImage()) return false;
		printf("%d / %d\n",i,int(picDatas.size()) - 1);
		for(int j = 0;j < i;++ j)
		{
			//ע�⣺imageCompareEx���޸�����ͼƬ����Ϣ������:��Ƚϵ�ͼƬ���ܻ��޸�֮ǰ�Ƚϵ�ͼƬ�ĳߴ��������
			//������������������⸴�ӻ���������������Ĵ��ڣ����Բ�ʹ�����ֱȽϷ�ʽ
			//if(imageCompareEx(&(picData[i]),&(picData[j]),5,10,1.0f))	//���ڿ���ͨ��������һ��������ͼƬ��Դ�н���ͼ�����Ϣ
			if(imageCompare(picDatas[i],picDatas[j],MAX_PIXELS_OFFSET))
			{//�ҵ���ͬ��ͼƬ��Դ
				++sameTexSum;
				printf("+:%s\n-:%s\n",picDatas[i].fileName.c_str(),picDatas[j].fileName.c_str());
				picDatas[i].sameTexOrder = j;
				break;
			}
		}
		if(!m_isOptimizeInMem)
			picDatas[i].releaseImage();
	}
	printf("The same pictures sum:%d\n",sameTexSum);
	//���濪ʼƴ��ͼƬ
	int allOutputPicSum = imagePack(picDatas,withOpti);
	if(allOutputPicSum == 0)
	{
		fclose(fp);
		return XFalse;
	}

	//�����е�����д�뵽����֡��Ϣ�ļ���
	printf("Writing file's infomation to infomationfile!\n");
	if(!writeFileData(picDatas,allOutputPicSum))
	{
		fclose(fp);
		return XFalse;
	}
	
	fclose(fp);
	//�ͷ���Դ
	for(auto it = picDatas.begin();it != picDatas.end();++ it)
	{
		it->releaseImage();
	}
	return XTrue;
}
void XResourceOptimize::clearTempFile()	//�����ʱ�ļ�
{
#ifdef UNICODE
	DeleteFileA(PNG_FILE_LIST_NAME);
	DeleteFileA(PNG_INFORMATION_FILE_NAME);

	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';
		DeleteFileA(fileName);
	}
#else
	DeleteFile(PNG_FILE_LIST_NAME);
	DeleteFile(PNG_INFORMATION_FILE_NAME);

	char fileName[MAX_FILE_NAME_LENGTH];
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,TEXTURE_FILE_NAME);
	for(int i = 0;i < MAX_BASICBOXSUM;++ i)
	{
		fileName[strlen(TEXTURE_FILE_NAME) - 5] = (i % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 6] = ((i/10) % 10) + '0';
		fileName[strlen(TEXTURE_FILE_NAME) - 7] = ((i/100) % 10) + '0';
		DeleteFile(fileName);
	}
#endif
}
//���Ż�����д�뵽�����ļ����Ա��ڵ���
bool XResourceOptimize::writeFileData(std::vector<XPicProcData> &picData,int allOutputSum)
{
	FILE *fp;
	if((fp = fopen(PNG_INFORMATION_FILE_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return false;
	}

	fprintf(fp,"%d,%d;\n",picData.size(),allOutputSum);
	//д������ĳһ֡����Ϣ
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		fprintf(fp,"%s:%s:%d:%d,%d:%d,%d,%d,%d:%d,%d;\n",it->fileName.c_str(), it->outFileName.c_str(),
			it->textureOrder,
			(int)(it->picOffsetPosition.x),(int)(it->picOffsetPosition.y),
			(int)(it->leftTopPoint.x),(int)(it->leftTopPoint.y),
			(int)(it->rightBottomPoint.x),(int)(it->rightBottomPoint.y)
			,(int)(it->picOutSize.x),(int)(it->picOutSize.y));
	}
	fclose(fp);

	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("frame.txt Open error!\n");
		return false;
	}
	//д������ĳһ֡����Ϣ
	for(auto it = picData.begin();it != picData.end();++ it)
	{
		fprintf(fp,"%s\n",it->fileName.c_str());
	}
	fclose(fp);
	return true;
}
//��Ŀ¼�Լ���Ŀ¼�µ�����PNG�ļ�ȫ��ö�ٳ������������ָ���ļ�
int XResourceOptimize::makePNGFileList()
{
	char temp[512];
	//��������png�ļ����б�
	sprintf_s(temp,512,"dir *.png /s/b>%s",PNG_FILE_LIST_NAME);
	system(temp);
	//��������ļ�Ϊ���·��
	//���ļ��б�
	FILE *fp = NULL;
	if((fp = fopen(PNG_FILE_LIST_NAME,"r")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	//����ļ��б����Ϣ
	int pathBaseDeep = XFile::getPathDeepByChar(XFile::getCurrentExeFileFullPath().c_str());
	char fileNameTemp[MAX_FILE_NAME_LENGTH] = "";
	std::vector<std::string> pngFiles;	//���е�png�ļ�
	while(!feof(fp))
	{
		if(fscanf(fp,"%s",fileNameTemp) != 1) break;
		if(strlen(fileNameTemp) < 4 || fileNameTemp[strlen(fileNameTemp) - 4]  != '.') continue;
		
		int len = (int)(strlen(fileNameTemp));
		for(int j = 0;j < len;++ j)
		{
			if(fileNameTemp[j] == '\\') fileNameTemp[j] = '/';
		}
		pngFiles.push_back(fileNameTemp);
	}
	fclose(fp);
	//��������·��
	if((fp = fopen(PNG_FILE_LIST_NAME,"w")) == NULL)
	{
		printf("Files list file open error!\n");
		return -1;
	}
	for(auto it = pngFiles.begin();it != pngFiles.end();++ it)
	{
		fprintf(fp,"%s\n",&((*it).c_str()[pathBaseDeep + 1]));
	}
	fclose(fp);

	return pngFiles.size();
}
void XResourceOptimize::addNeedFolder(const std::string& folder)		//������Ҫ������ļ���
{
	for(unsigned int i = 0;i < m_needOptimizeFolder.size(); ++ i)
	{
		if(XFile::fileNameCompare(m_needOptimizeFolder[i].c_str(),folder.c_str())) return;
	}
	m_needOptimizeFolder.push_back(folder);
}
void XResourceOptimize::addNeedNotFolder(const std::string& folder)	//���Ӳ���Ҫ������ļ���
{
	for(unsigned int i = 0;i < m_needNotOptimizeFolder.size(); ++ i)
	{
		if(XFile::fileNameCompare(m_needNotOptimizeFolder[i].c_str(),folder.c_str())) return;
	}
	m_needNotOptimizeFolder.push_back(folder);
}
}