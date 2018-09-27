#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceOptimize.h"

#include "XFile.h"
//#include "png.h"
#include "XSavePNG.h"
#include "SDL_image.h"
namespace XE{
#if TEX_INFO_DEBUG
int texInfoSum = 0;
#endif

XTextureResourceManager::XTextureResourceManager()
	:m_isInited(XFalse)
	,m_isEnable(XFalse)
{}
XBool XTextureResourceManager::init(XResPos resPos)	//�����ȡ����ѹ������Դ�ļ�
{
	if(m_isInited) return XTrue;

	m_isEnable = XTrue;
	m_isInited = XTrue;
	//Ĭ��������Դ��
	if(!addATexResPack(PNG_INFORMATION_FILE_NAME,resPos))
		LogStr("Ĭ����Դ������ʧ��!");
	return XTrue;
}
XBool XTextureResourceManager::getTexture(const char *temp,XTextureInfo &texInfo)
{
	if(!m_isInited ||
		!m_isEnable ||
		temp == NULL) return XFalse;
	for(auto it = m_texResInfos.begin();it != m_texResInfos.end();++ it)
	{
		for(auto jt = it->m_srcTexInfo.begin();jt != it->m_srcTexInfo.end();++ jt)
		{
			if(XFile::fileNameCompare(temp, jt->textureName))
			{
				texInfo = *jt;
				return true;
			}
		}
	}
	return XFalse;
}
XBool XTextureResourceManager::isLoaded(const char *filename)
{
	if(filename == NULL) return false;
	for(auto it = m_texResInfos.begin();it != m_texResInfos.end();++ it)
	{
		if(it->m_resName == filename) return true;
	}
	return false;
}
XBool XTextureResourceManager::releaseATexResPack(const char *filename)	//�ͷ�һ����Դ��
{
	if(filename == NULL) return true;
	for(auto it = m_texResInfos.begin();it < m_texResInfos.end();++ it)
	{
		if(it->m_resName == filename)
		{//������Ҫ����Դ�����ͷ�
			m_texResInfos.erase(it);
			return true;
		}
	}
	return false;
}
bool XTextureResourceManager::loadFromFolder(const char *filename,XResPos resPos)	//���ļ�����������Դ
{
	FILE *fp;
	//���ļ�
	if((fp = fopen(filename,"r")) == NULL)//PNG_INFORMATION_FILE_NAME
	{
//		m_isEnable = XFalse;
		return XFalse;
	}
	//��ȡ�ļ�����
	int textureSum;
	int targetTextureSum;
	if(fscanf(fp,"%d,",&textureSum) != 1) {fclose(fp);return XFalse;}
	if(fscanf(fp,"%d;",&targetTextureSum) != 1) {fclose(fp);return XFalse;}
	if(textureSum <= 0 || targetTextureSum <= 0)
	{
		fclose(fp);
//			m_isEnable = XFalse;
		return XFalse;
	}
	//������ȡ�������Դ�������Ҫ����Դ����
	char tempChar;
	int tempFlag;
	char *targetTextureFlag = NULL;
	char targetFileName[MAX_FILE_NAME_LENGTH] = "";
	std::string tmpPath = XFile::getFilePath(filename) + "/";
	targetTextureFlag = XMem::createArrayMem<char>(targetTextureSum);
	if(targetTextureFlag == NULL) 
	{
		fclose(fp);
//			m_isEnable = XFalse;
		return XFalse;
	}
	memset(targetTextureFlag,0,targetTextureSum);
	XTxtureResPackInfo tmp;
	tmp.m_resName = filename;
	tmp.m_srcTexInfo.resize(textureSum);
	tmp.m_dstTexInfo.resize(targetTextureSum);
	for(int i = 0;i < textureSum;++ i)
	{
		//������һ���س�λ
		if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}

		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}
			if(tempChar == ':')
			{
				targetFileName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				targetFileName[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//�ļ����ݷ�����������ֱ�ӷ���
			fclose(fp);
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		sprintf_s(tmp.m_srcTexInfo[i].textureName,MAX_FILE_NAME_LENGTH,"%s%s",tmpPath.c_str(),targetFileName);
		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(fscanf(fp,"%c",&tempChar) != 1) {fclose(fp);return XFalse;}
			if(tempChar == ':')
			{
				targetFileName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				targetFileName[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//�ļ����ݷ�����������ֱ�ӷ���
			fclose(fp);
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		if(fscanf(fp,"%d:",&tempFlag) != 1) {fclose(fp);return XFalse;}
		if(tempFlag < 0 || tempFlag >= targetTextureSum)
		{//�ļ����ݷ�����������ֱ�ӷ���
			fclose(fp);
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		if(tempFlag >= targetTextureSum) return XFalse;
		if(targetTextureFlag[tempFlag] == 0)
		{
			if(!tmp.m_dstTexInfo[tempFlag].load((tmpPath + targetFileName).c_str(),resPos))
			{
				fclose(fp);
//					m_isEnable = XFalse;
				XMem::XDELETE_ARRAY(targetTextureFlag);
				return XFalse;
			}
			targetTextureFlag[tempFlag] = 1;
		}
		tmp.m_srcTexInfo[i].texture = tmp.m_dstTexInfo[tempFlag];
		//��ȡʣ�µ���Ϣ
		if(fscanf(fp,"%f,",&tmp.m_srcTexInfo[i].textureOffset.x) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f:",&tmp.m_srcTexInfo[i].textureOffset.y) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f,",&tmp.m_srcTexInfo[i].textureMove.x) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f,",&tmp.m_srcTexInfo[i].textureMove.y) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f,",&tmp.m_srcTexInfo[i].textureMove2.x) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f:",&tmp.m_srcTexInfo[i].textureMove2.y) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f,",&tmp.m_srcTexInfo[i].textureSize.x) != 1) {fclose(fp);return XFalse;}
		if(fscanf(fp,"%f;",&tmp.m_srcTexInfo[i].textureSize.y) != 1) {fclose(fp);return XFalse;}
	}
	XMem::XDELETE_ARRAY(targetTextureFlag);
	//�������ݶ�ȡ���
	fclose(fp);
	m_texResInfos.push_back(tmp);
	return true;
}
bool XTextureResourceManager::loadFromPacker(const char *filename,XResPos resPos)	//��ѹ������������Դ
{
	//����Դ������ȡ����Դ�ļ�������
	char tempFilename[MAX_FILE_NAME_LENGTH] = "";
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s%s",BASE_RESOURCE_PATH,filename);//PNG_INFORMATION_FILE_NAME
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) 
	{//û���Ż�����Դ�ļ�,ֱ�ӷ���
//			m_isEnable = XTrue;
//			m_isInited = XTrue;
		return false;
	}
	//����һ������
	int textureSum;
	int targetTextureSum;
	int offset = 0;
	if(sscanf((char *)(p + offset),"%d,",&textureSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),',') + 1;
	if(sscanf((char *)(p + offset),"%d;",&targetTextureSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),';') + 1;
	if(textureSum <= 0 || targetTextureSum <= 0)
	{
		XMem::XDELETE_ARRAY(p);
//			m_isEnable = XFalse;
		return XFalse;
	}
	//������ȡ�������Դ�������Ҫ����Դ����
	char tempChar = ' ';
	int tempFlag = 0;
	char *targetTextureFlag = NULL;
	char targetFileName[MAX_FILE_NAME_LENGTH] = "";
	targetTextureFlag = XMem::createArrayMem<char>(targetTextureSum);
	if(targetTextureFlag == NULL) 
	{
		XMem::XDELETE_ARRAY(p);
//			m_isEnable = XFalse;
		return XFalse;
	}
	memset(targetTextureFlag,0,targetTextureSum);
	XTxtureResPackInfo tmp;
	tmp.m_resName = tempFilename;
	tmp.m_srcTexInfo.resize(textureSum);
	tmp.m_dstTexInfo.resize(targetTextureSum);
	for(int i = 0;i < textureSum;++ i)
	{//������һ���س�λ
		if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		++ offset;//��ȡ�ı��Ļس�λ
		if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		++ offset;//��ȡ�ı��Ļس�λ

		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
			++ offset;
			if(tempChar == ':')
			{
				tempFilename[j] = '\0';
				sprintf_s(tmp.m_srcTexInfo[i].textureName,MAX_FILE_NAME_LENGTH,"%s%s",BASE_RESOURCE_PATH,tempFilename);	//Ϊ�˷�װǰ���·��ͳһ�����޸����·��
				//m_texInfo[i].textureName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				//m_texInfo[i].textureName[j] = tempChar;
				tempFilename[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//�ļ����ݷ�����������ֱ�ӷ���
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(p);
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		tempFlag = 0;
		for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
		{
			if(sscanf((char *)(p + offset),"%c",&tempChar) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
			++ offset;
			if(tempChar == ':')
			{
				targetFileName[j] = '\0';
				tempFlag = 1;
				break;
			}else
			{
				targetFileName[j] = tempChar;
			}
		}
		if(tempFlag == 0)
		{//�ļ����ݷ�����������ֱ�ӷ���
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(p);
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%d:",&tempFlag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),':') + 1;
		if(tempFlag < 0 || tempFlag >= targetTextureSum)
		{//�ļ����ݷ�����������ֱ�ӷ���
//				m_isEnable = XFalse;
			XMem::XDELETE_ARRAY(p);
			XMem::XDELETE_ARRAY(targetTextureFlag);
			return XFalse;
		}
		if(targetTextureFlag[tempFlag] == 0)
		{//������ֵ�ǰ��Դ��δ���룬������������Դ
			sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s%s",BASE_RESOURCE_PATH,targetFileName);
			if(!tmp.m_dstTexInfo[tempFlag].load(tempFilename,resPos))
			{
//					m_isEnable = XFalse;
				XMem::XDELETE_ARRAY(p);
				XMem::XDELETE_ARRAY(targetTextureFlag);
				return XFalse;
			}
			targetTextureFlag[tempFlag] = 1;
		}
		tmp.m_srcTexInfo[i].texture = tmp.m_dstTexInfo[tempFlag];
		//��ȡʣ�µ���Ϣ
		if(sscanf((char *)(p + offset),"%f,",&tmp.m_srcTexInfo[i].textureOffset.x) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f:",&tmp.m_srcTexInfo[i].textureOffset.y) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),':') + 1;

		if(sscanf((char *)(p + offset),"%f,",&tmp.m_srcTexInfo[i].textureMove.x) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f,",&tmp.m_srcTexInfo[i].textureMove.y) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f,",&tmp.m_srcTexInfo[i].textureMove2.x) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f:",&tmp.m_srcTexInfo[i].textureMove2.y) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),':') + 1;
		if(sscanf((char *)(p + offset),"%f,",&tmp.m_srcTexInfo[i].textureSize.x) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),',') + 1;
		if(sscanf((char *)(p + offset),"%f;",&tmp.m_srcTexInfo[i].textureSize.y) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),';') + 1;
	}
	XMem::XDELETE_ARRAY(targetTextureFlag);
	//�������ݶ�ȡ���
	XMem::XDELETE_ARRAY(p);
	m_texResInfos.push_back(tmp);
	return true;
}
XBool XTextureResourceManager::addATexResPack(const char *filename,XResPos resPos)
{
	if(!m_isInited || !m_isEnable) return false;
	if(filename == NULL) return false;
	if(isLoaded(filename)) return false;	//��ֹ��Դ�ظ�����
	//������Դ��λ��
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename,resPos) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename)) return false;
		break;
	}
	return true;
}
bool XTextureData::loadFromFolder(const char *filename,XResPos resPos)	//���ļ�����������Դ
{
	if(!texture.load(filename,resPos)) return XFalse;
	//���ڳ����ڲ�������ݽ���2��n�η���չ����������Ҫ�ν����ݡ�
	if(XEG.getTexNeedFit2N() && !XMath::isNPOT(texture.m_w,texture.m_h))
	{//����2��n�η�û�н�����չ
		textureSize.set(texture.m_w,texture.m_h);
		isEnableInsideClip = 0;
	}else
	{//���ڽ����˿���չ������Ҫ���вü�
		int w = XMath::getMinWellSize2n(texture.m_w);
		int h = XMath::getMinWellSize2n(texture.m_h);
		textureMove.set(0.0f);
		textureMove2.set(0.0f);
		clipInsideRect.left = (w - texture.m_w) * 0.5f;
		clipInsideRect.top = (h - texture.m_h) * 0.5f;
		clipInsideRect.right = clipInsideRect.left + texture.m_w;
		clipInsideRect.bottom = clipInsideRect.top + texture.m_h;
		textureSize.set(texture.m_w,texture.m_h);
		texture.m_w = w;
		texture.m_h = h;

		isEnableInsideClip = 1;
	}
	return true;
}
bool XTextureData::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	XTextureInfo texInfo;
	if(!XTexManager.getTexture(filename,texInfo)) return false;
	texture = texInfo.texture;
	textureMove = texInfo.textureMove;
	textureMove2 = texInfo.textureMove2;
	clipInsideRect.set(texInfo.textureOffset,
		texInfo.textureOffset + texInfo.textureSize);
	textureSize = clipInsideRect.getSize() + textureMove + textureMove2;
	isEnableInsideClip = 1;
	return true;
}
XBool XTextureData::reload(const char * filename, XResPos resPos)
{
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();

 	switch(resPos)
	{
	case RES_LOCAL_PACK://���Ż����ж�ȡ���ݣ���ȡʧ��֮�����Դ���ж�ȡ
		if(!loadFromPacker(filename) && !loadFromFolder(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename)) return false;
		break;
	}
	return true;
}
XBool XTextureData::setACopy(const XTextureData& temp)
{
	if(& temp == this) return XFalse;	//��ֹ����ֵ
	m_isInited = temp.m_isInited;

	m_ID = temp.m_ID;
	texture = temp.texture;		//ָ����ͼ��ָ��
	textureMove = temp.textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	textureMove2 = temp.textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	isEnableInsideClip = temp.isEnableInsideClip;			//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	clipInsideRect = temp.clipInsideRect;		//�ڲ����е�����
	textureSize = temp.textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�

	return XTrue;
}
XTextureData& XTextureData::operator = (const XTextureData& temp)
{
	if(& temp == this) return *this;	//��ֹ����ֵ
	m_isInited = temp.m_isInited;

	m_ID = temp.m_ID;
	texture = temp.texture;		//ָ����ͼ��ָ��
	textureMove = temp.textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	textureMove2 = temp.textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	isEnableInsideClip = temp.isEnableInsideClip;			//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	clipInsideRect = temp.clipInsideRect;		//�ڲ����е�����
	textureSize = temp.textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�

	return * this;
}
//����Դ��ԭ
XBool reductionTexture(const void *p,int length,const XTextureInfo &texInfo)
{
	if(p == NULL ||
		length <= 0) return XFalse;
//	SDL_Surface *temp = NULL;
//	SDL_Surface *temp_back = NULL;
	SDL_RWops *fileData = SDL_RWFromMem((void *)p,length);
	if(fileData == NULL) return XFalse;
	SDL_Surface *temp_back = IMG_Load_RW(fileData,1);
	if(temp_back == NULL) return XFalse;
//	SDL_SetAlpha(temp, SDL_RLEACCEL | SDL_SRCALPHA, 255);
//	temp_back = SDL_DisplayFormatAlpha(temp);
	//������Ҫ���½�����ͼ
	int texW = texInfo.textureSize.x + texInfo.textureMove.x + texInfo.textureMove2.x;
	int texH = texInfo.textureSize.y + texInfo.textureMove.y + texInfo.textureMove2.y;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
	//��ʼ����ͼ����
	unsigned char *pData = (unsigned char *)picArm->pixels;
	unsigned char *pData1 = (unsigned char *)temp_back->pixels;
	memset(pData,0,(texW * texH) << 2);
	//����Ҫ�����ݴ洢����ͼ����
	for(int i = 0;i < texInfo.textureSize.y;++ i)
	{
		memcpy(pData + (int)((texInfo.textureMove.y + i) * texW * 4.0f) + (int)(texInfo.textureMove.x * 4.0f),
			pData1 + (int)((texInfo.textureOffset.y + i) * temp_back->w * 4.0f) + (int)(texInfo.textureOffset.x * 4.0f),
			texInfo.textureSize.x * 4.0f);
	}
	//������Ҫ����ͼ����
	XPng::savePNG(texInfo.textureName,picArm,Z_BEST_COMPRESSION);
	//�ͷ���Դ
	SDL_FreeSurface(picArm);
	picArm = NULL;
//	SDL_FreeSurface(temp);
//	temp = NULL;
	SDL_FreeSurface(temp_back);
	temp_back = NULL;
	//SDL_FreeRW(fileData);
	return XTrue;
}
XTextureInfo::XTextureInfo(const XTextureInfo & temp)
{
	if(this == &temp) return;		//��ֹ��������
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_cp = temp.m_cp;
	texture = temp.texture;
	textureOffset = temp.textureOffset;
	textureMove = temp.textureMove;
	textureMove2 = temp.textureMove2;
	textureSize = temp.textureSize;
	textureName = temp.textureName;
//	targetFileName = temp.targetFileName;
//	targetOrder = temp.targetOrder;
}
XTextureInfo& XTextureInfo::operator = (const XTextureInfo& temp)
{
	if(& temp == this) return * this;	//��ֹ����ֵ
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{
		XMem::XDELETE(m_cp);
		XMem::XDELETE_ARRAY(textureName);
	//	XMem::XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
		-- texInfoSum;
		printf("%d-\n",texInfoSum);
#endif
	}
	m_cp = temp.m_cp;
	texture = temp.texture;
	textureOffset = temp.textureOffset;
	textureMove = temp.textureMove;
	textureMove2 = temp.textureMove2;
	textureSize = temp.textureSize;
	textureName = temp.textureName;
//	targetFileName = temp.targetFileName;
//	targetOrder = temp.targetOrder;
	return * this;
}
#if !WITH_INLINE_FILE
#include "XTextureInformation.inl"
#endif
}