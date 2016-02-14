#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
//#include "windows.h"
#include "XResourcePack.h"
#include "XNumber.h"
#include "XFont.h"
#include "XFontUnicode.h"
#include "XFile.h"
namespace XE{
unsigned int loadTextureFromSDL(SDL_Surface *surface,XVector2& size)
{
	/* Use the surface width and height expanded to powers of 2 */
	int w = XMath::getMinWellSize2n(surface->w);
	int h = XMath::getMinWellSize2n(surface->h);
	size.x = w;
	size.y = h;

	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if(image == NULL) return 0;

	/* Save the alpha blending attributes */
	Uint32 saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
#if SDL_VERSION_ATLEAST(1, 3, 0)
	SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
	SDL_SetSurfaceAlphaMod(surface, 0xFF);
#else
	Uint8 saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}
#endif

	/* Copy the surface into the GL texture image */
	SDL_Rect area;
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
#if SDL_VERSION_ATLEAST(1, 3, 0)
	SDL_SetSurfaceAlphaMod(surface, saved_alpha);
#else
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}
#endif

	/* Create an OpenGL texture for the image */
	GLuint texture;
	glGenTextures(1, &texture);
	XGL::BindTexture2D(texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#if WITHXSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	SDL_FreeSurface(image); /* No longer needed */
#if WITH_ENGINE_STATISTICS
	XEG.addStaticsticsTexInfo(texture);
#endif

	return texture;
}
void XFontTTFInfo::setType(XTTFRenderType t)
{
	type = t;
	if(font == NULL) return;
	switch(t)
	{
	case RENDER_LATIN1:
		TTF_SizeText(font," ",&(widthOfSpace),NULL);
		break;
	case RENDER_UTF8:
		TTF_SizeUTF8(font," ",&(widthOfSpace),NULL);
		break;
	case RENDER_UNICODE:
		{//������ܴ�������
			char tempStr[4] = "  ";
			tempStr[2] = '\0';
			tempStr[3] = '\0';
			TTF_SizeUNICODE(font,(Uint16 *)tempStr,&(widthOfSpace),NULL);
		}
		break;
	}
}
XFontTTF::XFontTTF()
	:m_isInited(XFalse)
	,m_curLoadTTFFontFileSum(0)
	//,m_renderType(RENDER_UTF8)
{
	if(TTF_Init() < 0) LogStr("Error:ttf init error!\n");
	else m_isInited = XTrue;
}
XFontTTF::~XFontTTF()
{
	if(!m_isInited) return;
	TTF_Quit();
	m_isInited = XFalse;
}
int XFontTTF::getTTFFileOrder(const char * filename,int ptsize)
{
	if(filename == NULL ||
		m_curLoadTTFFontFileSum <= 0) return -1;
	int sum = 0;
	for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
	{
		if(m_fontInfo[i].isEnable)
		{
			if(ptsize == m_fontInfo[i].fontSize && XFile::fileNameCompare(filename,m_fontInfo[i].filename.c_str())) return i;
			++ sum;
			if(sum >= MAX_TTF_FONT_FILE_SUM) return -1;
		}
	}
	return -1;
}
bool XFontTTF::loadFromFolder(int i,const char *filename,int ptsize)	//���ļ�����������Դ
{
	char * tmp = XString::ANSIToUTF8(filename);
	m_fontInfo[i].font = TTF_OpenFont(tmp, ptsize);
	if(m_fontInfo[i].font == NULL)
	{
		XMem::XDELETE_ARRAY(tmp);
		return false;
	}
	TTF_SetFontHinting(m_fontInfo[i].font,TTF_HINTING_LIGHT);
	TTF_SetFontKerning(m_fontInfo[i].font,0);
	XMem::XDELETE_ARRAY(tmp);
	return true;
}
bool XFontTTF::loadFromPacker(int i,const char *filename,int ptsize)	//��ѹ������������Դ
{
	int lengthTemp = XResPack.getFileLength(filename);
	unsigned char *p = XMem::createArrayMem<unsigned char>(lengthTemp + 1);
	if(p == NULL) return false;
	XResPack.unpackResource(filename,p);

	SDL_RWops *fileData = SDL_RWFromMem(p,lengthTemp);
	m_fontInfo[i].font = TTF_OpenFontRW(fileData,1,ptsize);
	if(m_fontInfo[i].font == NULL)
	{
		XMem::XDELETE_ARRAY(p);
		return false;
	}
	TTF_SetFontHinting(m_fontInfo[i].font,TTF_HINTING_LIGHT);
	TTF_SetFontKerning(m_fontInfo[i].font,0);
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFontTTF::loadFromWeb(int i,const char *filename,int ptsize)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
int XFontTTF::loadTTFFile(const char * filename,int ptsize,XResourcePosition resoursePosition)
{//��ʱ��֧�ִ������ȡ��Դ
	if(!m_isInited) return 0;
	if(filename == NULL) return -1;
	if(m_curLoadTTFFontFileSum >= MAX_TTF_FONT_FILE_SUM) return -1;
	int ret = getTTFFileOrder(filename,ptsize);
	if(ret >= 0) return ret;	//��ֹ�ظ�����
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();

	for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
	{
		if(!m_fontInfo[i].isEnable)
		{
			switch(resoursePosition)
			{
			case RESOURCE_LOCAL_PACK:
				loadFromPacker(i,filename,ptsize);
				break;
			case RESOURCE_LOCAL_FOLDER:
				loadFromFolder(i,filename,ptsize);
				break;
			case RESOURCE_WEB:
				loadFromWeb(i,filename,ptsize);
				break;
			case RESOURCE_AUTO:
				if(loadFromPacker(i,filename,ptsize) || loadFromFolder(i,filename,ptsize) ||
					loadFromWeb(i,filename,ptsize)) break;
				break;
			}
			if(m_fontInfo[i].font == NULL)
			{//��������ļ�����ʧ�ܵĻ�
				LogStr("ָ�������ļ�����ʧ��,���Դӵ�ǰĿ¼����!");
				//���波�Դӵ�ǰ�ļ����������
				m_fontInfo[i].font = TTF_OpenFont(XString::ANSI2UTF8(XFile::getFilenameFormPath(filename).c_str()).c_str(), ptsize);
				if(m_fontInfo[i].font == NULL)
				{//���ﳢ�Դ�ϵͳ����Ŀ¼�м��������ļ�
					LogStr("��ǰĿ¼����ʧ��,���Դ�ϵͳ����Ŀ¼����!");
					std::string tmpSysFontPath = XFile::getWindowsSysFontPath();
					tmpSysFontPath += XFile::getFilenameFormPath(filename);
					m_fontInfo[i].font = TTF_OpenFont(XString::ANSI2UTF8(tmpSysFontPath.c_str()).c_str(), ptsize);
					if(m_fontInfo[i].font == NULL)
					{
						LogNull("%s :�����ļ�����ʧ��,���Լ���Ĭ������!",tmpSysFontPath.c_str());
						//�����ٳ��Լ����������Ĭ������
						tmpSysFontPath = XFile::getWindowsSysFontPath() + "simfang.ttf";
						m_fontInfo[i].font = TTF_OpenFont(XString::ANSI2UTF8(tmpSysFontPath.c_str()).c_str(), ptsize);
						if(m_fontInfo[i].font == NULL)
						{
							LogNull("û�к��ʵ������ļ�!");
							return -1;
						}
					}
				}
			}
			m_fontInfo[i].isEnable = XTrue;
			m_fontInfo[i].fontSize = ptsize;
			m_fontInfo[i].setType(m_fontInfo[i].type);
			m_fontInfo[i].filename = filename;
			++ m_curLoadTTFFontFileSum;
			return i;
		}
	}

	return -1;
}
unsigned int XFontTTF::getTexture(int fontOrder,const char * str,XVector2& size)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return 0;
	if(!m_fontInfo[fontOrder].isEnable) return 0;
	SDL_Surface *surface = getSurface(fontOrder,str);
	if(surface == NULL) return 0;
	GLuint ret = loadTextureFromSDL(surface,size);
	SDL_FreeSurface(surface);
	//����Ƿ��д���
	GLenum gl_error = glGetError();
	if(gl_error != GL_NO_ERROR ) 
	{
		printf("Warning: Couldn't create texture: 0x%x\n",gl_error);
	}
	return ret;
}
SDL_Surface * XFontTTF::getSurface(int index,const char * str)
{
	if(str == NULL) return NULL;
	SDL_Surface * surface = NULL;
	switch(m_fontInfo[index].type) 
	{
	    case RENDER_LATIN1:
			surface = TTF_RenderText_Blended(m_fontInfo[index].font,str,m_fontInfo[index].color);
		break;
	    case RENDER_UTF8:
			surface = TTF_RenderUTF8_Blended(m_fontInfo[index].font,str,m_fontInfo[index].color);
		break;

	    case RENDER_UNICODE:
		{
			/* This doesn't actually work because you can't pass
			   UNICODE text in via command line, AFAIK, but...
			 */
			Uint16 unicode_text[BUFSIZ];
			for(int i = 0;(str[0] || str[1]); ++i ) 
			{
				unicode_text[i] = ((Uint8 *)str)[0];
				unicode_text[i] <<= 8;
				unicode_text[i] |= ((Uint8 *)str)[1];
				str += 2;
			}
			surface = TTF_RenderUNICODE_Blended(m_fontInfo[index].font,
				unicode_text,m_fontInfo[index].color);
		}
		break;
	    default:
		surface = NULL; /* This shouldn't happen */
		break;
	}
	return surface;
}
//��������Ų����Ի�����ŵĽ��
void getMinSize(int& x,int& y,int sum,int sizeX,int sizeY)	//��������ʵĳߴ�
{//ʹ�ñ������Եķ������Ƚ�ɵ����ʱ������
	if(sum <= 0) return;
	int minX = sum;
	int minSize = XMath::getMinWellSize2n(sizeY) * XMath::getMinWellSize2n(sizeX * sum);
	int curSize;
	for(int i = 1;i < sum;++ i)
	{
		if((sum % i) == 0)
		{
			int w = XMath::getMinWellSize2n(sizeX * i);
			int h = XMath::getMinWellSize2n(sizeY * (sum / i));
			curSize = w * h;
			if(curSize < minSize || (curSize == minSize && h < w))	//����Ż�������Ҫ��ϸ����
			{
				minSize = curSize;
				minX = i;
			}
		}
	}
	x = minX;
	y = sum / x;
}
//����ķ�����δʵ��
XBool XFontTTF::getTextureNumber(int fontOrder,unsigned int &tex,XVector2 &layout)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	int len = sizeof(TEXT_MODE) - 1;
	//������Ҫ������ô�����Ų�����õ�
	int x,y;
	getMinSize(x,y,len,(m_fontInfo[fontOrder].fontSize >> 1),m_fontInfo[fontOrder].fontSize);
	int w = XMath::getMinWellSize2n(x * (m_fontInfo[fontOrder].fontSize >> 1));
	int h = XMath::getMinWellSize2n(y * (m_fontInfo[fontOrder].fontSize));
	//������ʵ�ߴ����ͼ
	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
			0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
		       );
	if(image == NULL) return XFalse;
	//Ȼ��ʹ����ͼ
	char str[] = TEXT_MODE;
	char pStr[] = TEXT_MODE;
	for(int i = 0;i < y;++ i)
	{
		if(i * x + x <= (int)(strlen(str)))
		{
			memcpy(pStr,str + i * x,x);
			pStr[x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//ճ��
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
			SDL_FreeSurface(temp);
		}else
		{
			memcpy(pStr,str + i * x,strlen(str) - i * x);
			pStr[strlen(str) - i * x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//ճ��
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
			SDL_FreeSurface(temp);
		}
	}
	//ȡͼ��ϣ����濪ʼ�����������
	XVector2 texSize;
	tex = loadTextureFromSDL(image,texSize);
	SDL_FreeSurface(image);
	layout.set(x,y);
	return XTrue;
}
XBool XFontTTF::getTextureFont(int fontOrder,unsigned int &tex,XVector2 &layout)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	int len = sizeof(TEXT_FONT_MODE) - 1;
	//������Ҫ������ô�����Ų�����õ�
	int x,y;
	getMinSize(x,y,len,(m_fontInfo[fontOrder].fontSize >> 1),m_fontInfo[fontOrder].fontSize);
	int w = XMath::getMinWellSize2n(x * (m_fontInfo[fontOrder].fontSize >> 1));
	int h = XMath::getMinWellSize2n(y * (m_fontInfo[fontOrder].fontSize));
	//������ʵ�ߴ����ͼ
	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
			0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
		       );
	if(image == NULL) return XFalse;
	//Ȼ��ʹ����ͼ
	char str[] = TEXT_FONT_MODE;
	char pStr[] = TEXT_FONT_MODE;
	for(int i = 0;i < y;++ i)
	{
		if(i * x + x <= (int)(strlen(str)))
		{
			memcpy(pStr,str + i * x,x);
			pStr[x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//ճ��
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
			SDL_FreeSurface(temp);
		}else
		{
			memcpy(pStr,str + i * x,strlen(str) - i * x);
			pStr[strlen(str) - i * x] = '\0';
			SDL_Surface * temp = getSurface(fontOrder,pStr);
			if(temp == NULL) return XFalse;
			//ճ��
			SDL_Rect dst = {0,i * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
			//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
			SDL_SetAlpha(temp,0,255);
			SDL_SetAlpha(image,0,255);
			SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
			SDL_FreeSurface(temp);
			break;
		}
	}
	//ȡͼ��ϣ����濪ʼ�����������
	XVector2 texSize;
	tex = loadTextureFromSDL(image,texSize);
	SDL_FreeSurface(image);
	layout.set(x,y);
	return XTrue;
}
XBool XFontTTF::getTextureFontUnicode(int fontOrder,unsigned int *tex,int& texSum,XVector2 &layout,XBool withBlackOutLine)
{
	if(fontOrder < 0 || fontOrder >= MAX_TTF_FONT_FILE_SUM) return XFalse;
	if(!m_fontInfo[fontOrder].isEnable) return XFalse;
	//������Ҫ������ô�����Ų�����õ�
	int x = 1024 / m_fontInfo[fontOrder].fontSize;	//Ĭ��ʹ��1024*1024�ĳߴ�
	int y = 1024 / m_fontInfo[fontOrder].fontSize;
	int wByC = (x << 1);
	int hByC = (y << 1);
	int w = 1024;
	int h = 1024;
#if WITH_FULL_ALL_CHINESE == 0
	int len = XString::getStrLen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 2
	int len = XString::getStrLen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 1
	int len = XString::getStrLen(TEXT_FONT_UNICODE_MODE0);
	len += XString::getStrLen(TEXT_FONT_UNICODE_MODE1);
	len += XString::getStrLen(TEXT_FONT_UNICODE_MODE2);
	len += XString::getStrLen(TEXT_FONT_UNICODE_MODE3);
#endif
	texSum = len / (x * y);
	if(texSum * x * y < len) ++ texSum;
	//��չ����ַ�����ʹ�ÿ��Ա���ʹ��
#if WITH_FULL_ALL_CHINESE == 0
	int length = strlen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 2
	int length = strlen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 1
	int length = strlen(TEXT_FONT_UNICODE_MODE0);
	length += strlen(TEXT_FONT_UNICODE_MODE1);
	length += strlen(TEXT_FONT_UNICODE_MODE2);
	length += strlen(TEXT_FONT_UNICODE_MODE3);
#endif
	char *str = XMem::createArrayMem<char>(length + 128);
	if(str == NULL) return XFalse;
#if WITH_FULL_ALL_CHINESE == 0
	char tempStr[] = TEXT_FONT_UNICODE_MODE;
#endif
#if WITH_FULL_ALL_CHINESE == 2
	char tempStr[] = TEXT_FONT_UNICODE_MODE;
#endif
#if WITH_FULL_ALL_CHINESE == 1
	std::string tmp = TEXT_FONT_UNICODE_MODE0;
	tmp += TEXT_FONT_UNICODE_MODE1;
	tmp += TEXT_FONT_UNICODE_MODE2;
	tmp += TEXT_FONT_UNICODE_MODE3;
	const char *tempStr = tmp.c_str();
#endif
	int j = 0;
	for(int i = 0;i < length;)
	{
		if(tempStr[i] < 0)
		{
			str[j] = tempStr[i];
			str[j + 1] = tempStr[i + 1];
			j += 2;
			i += 2;
		}else
		{//ASCII
			str[j] = tempStr[i];
			str[j + 1] = ' ';
			j += 2;
			++ i;
		}
	}
	str[j] = '\0';
	str[j + 1] = '\0';
	//�������ν�����ͼ
	int maxLen = strlen(str);
#if WITH_FULL_ALL_CHINESE == 0
	char pStr[] = TEXT_FONT_UNICODE_MODE;
#endif
#if WITH_FULL_ALL_CHINESE == 2
	char pStr[] = TEXT_FONT_UNICODE_MODE;
#endif
#if WITH_FULL_ALL_CHINESE == 1
	char *pStr = XMem::createArrayMem<char>(maxLen);
	if(pStr == NULL) 
	{
		XMem::XDELETE_ARRAY(str);
		return XFalse;
	}
#endif
	char *pStr1;
	for(int i = 0;i < texSum;++ i)
	{//������ʵ�ߴ����ͼ
		SDL_Surface *image = SDL_CreateRGBSurface(
				SDL_SWSURFACE,
				w, h,32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
				0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
#else
				0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
#endif
				   );
		if(image == NULL)
		{
			XMem::XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE == 1
			XMem::XDELETE_ARRAY(pStr);
#endif
			return XFalse;
		}
		//Ȼ��ʹ����ͼ
		for(j = 0;j < y;++ j)
		{
			if(j * (wByC) + i * x * (hByC) + (wByC) <= maxLen)
			{
				memcpy(pStr,str + j * (wByC) + i * x * (hByC),wByC);
				pStr[wByC] = '\0';
				pStr[(wByC) + 1] = '\0';
				if(m_fontInfo[fontOrder].widthOfSpace == m_fontInfo[fontOrder].fontSize)
				{//ȥ������Ҫ�Ŀո�
					int removeSum = 0;
					for(int tw = 0;tw < x;++ tw)
					{
						if(pStr[(tw << 1) + 1] == ' ' && pStr[(tw << 1)] > 0)
						{//ASCII
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							++ removeSum;
						}else
						{
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							pStr[(tw << 1) - removeSum + 1] = pStr[(tw << 1) + 1];
						}
					}
					pStr[(wByC) - removeSum] = '\0';
					pStr[(wByC) - removeSum + 1] = '\0';
				}
				pStr1 = XString::ANSIToUTF8(pStr);
				SDL_Surface * temp = getSurface(fontOrder,pStr1);
				XMem::XDELETE_ARRAY(pStr1);
				if(temp == NULL)
				{
					XMem::XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE == 1
					XMem::XDELETE_ARRAY(pStr);
#endif
					return XFalse;
				}
				//if(withBlackOutLine)
				//{
				//	unsigned char * p = (unsigned char *)temp->pixels;
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 0;
				//		p[ti + 1] = 0;
				//		p[ti + 2] = 0;
				//	}
				//	SDL_Rect dst0 = {1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	SDL_SetAlpha(temp,0,255);
				//	SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst0);	//����ֱ�ӿ���
				//	SDL_SetAlpha(temp,SDL_SRCALPHA,0);
				//	SDL_Rect dst1 = {-1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	����SDL_SetAlpha(image,SDL_SRCALPHA,0);
				//	SDL_BlitSurface(temp,0,image,&dst1);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	SDL_Rect dst2 = {0,j * m_fontInfo[fontOrder].fontSize + 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst2);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	SDL_Rect dst3 = {0,j * m_fontInfo[fontOrder].fontSize - 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst3);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 255;
				//		p[ti + 1] = 255;
				//		p[ti + 2] = 255;
				//	}
				//}
				//ճ��
				SDL_Rect dst = {0,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
				//if(!withBlackOutLine)
				{
					SDL_SetAlpha(temp,0,255);
					SDL_SetAlpha(image,0,255);
				}
				SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
				SDL_FreeSurface(temp);
			}else
			{
				memcpy(pStr,str + j * (wByC) + i * x * (hByC),maxLen - (j * (wByC) + i * x * (hByC)));
				pStr[maxLen - (j * (wByC) + i * x * (hByC))] = '\0';
				pStr[maxLen - (j * (wByC) + i * x * (hByC)) + 1] = '\0';
				if(m_fontInfo[fontOrder].widthOfSpace == m_fontInfo[fontOrder].fontSize)
				{//ȥ������Ҫ�Ŀո�
					int removeSum = 0;
					for(int tw = 0;tw < x;++ tw)
					{
						if(pStr[(tw << 1) + 1] == ' ' && pStr[(tw << 1)] > 0)
						{//ASCII
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							++ removeSum;
						}else
						{
							pStr[(tw << 1) - removeSum] = pStr[(tw << 1)];
							pStr[(tw << 1) - removeSum + 1] = pStr[(tw << 1) + 1];
						}
					}
					pStr[(wByC) - removeSum] = '\0';
					pStr[(wByC) - removeSum + 1] = '\0';
				}
				pStr1 = XString::ANSIToUTF8(pStr);
				SDL_Surface * temp = getSurface(fontOrder,pStr1);
				XMem::XDELETE_ARRAY(pStr1);
				if(temp == NULL)
				{
					XMem::XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE == 1
					XMem::XDELETE_ARRAY(pStr);
#endif
					return XFalse;
				}
				//ճ��
				//if(withBlackOutLine)
				//{
				//	unsigned char * p = (unsigned char *)temp->pixels;
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 0;
				//		p[ti + 1] = 0;
				//		p[ti + 2] = 0;
				//	}
				//	SDL_Rect dst0 = {1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	SDL_SetAlpha(temp,0,255);
				//	SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst0);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	SDL_SetAlpha(temp,SDL_SRCALPHA,0);
				//	SDL_Rect dst1 = {-1,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//	//SDL_SetAlpha(image,0,255);
				//	SDL_BlitSurface(temp,0,image,&dst1);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	SDL_Rect dst2 = {0,j * m_fontInfo[fontOrder].fontSize - 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst2);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	SDL_Rect dst3 = {0,j * m_fontInfo[fontOrder].fontSize + 1,temp->w,temp->h};
				//	SDL_BlitSurface(temp,0,image,&dst3);	//��������alpha���(������Ҫ��ԭԭ�е����)
				//	for(int ti = 0;ti < temp->w*temp->h*4;ti += 4)
				//	{
				//		p[ti + 0] = 255;
				//		p[ti + 1] = 255;
				//		p[ti + 2] = 255;
				//	}
				//}
				SDL_Rect dst = {0,j * m_fontInfo[fontOrder].fontSize,temp->w,temp->h};
				//if(!withBlackOutLine)
				{
					//������alphaͨ��ʱ���������ǲ����л��ֱ�Ӹ���
					SDL_SetAlpha(temp,0,255);
					SDL_SetAlpha(image,0,255);
				}
				SDL_BlitSurface(temp,0,image,&dst);	//��������alpha���(������Ҫ��ԭԭ�е����)
				SDL_FreeSurface(temp);
				break;	//��������һ�У�����
			}
		}
		//ȡͼ��ϣ����濪ʼ�����������
		if(withBlackOutLine)
		{//���Ӻ�ɫ�����,���ڽ����Դ����Ӱ�������
			unsigned char * p = (unsigned char *)image->pixels;
			unsigned char * px = XMem::createArrayMem<unsigned char>(w * h * 4);
			if(px == NULL) 
			{
				XMem::XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE == 1
				XMem::XDELETE_ARRAY(pStr);
#endif
				return XFalse;
			}
			memcpy(px,p,w * h * 4);
			int index = 0;
			int sum,a;
			int tempW = w << 2;
			for(int th = 0;th < h;++ th)
			{
				index = th * tempW;
				for(int tw = 0;tw < w;++ tw,index += 4)
				{
					if(px[index + 3] < 127)
					{
						if((tw > 0 && px[index - 4 + 3] >= 127)
							|| (tw < w - 1 && px[index + 4 + 3] >= 127)
							|| (th > 0 && px[index - tempW + 3] >= 127)
							|| (th < h - 1 && px[index + tempW + 3] >= 127)
							|| (tw > 0 && th > 0 && px[index - 4 - tempW + 3] >= 127)
							|| (tw > 0 && th < h - 1 && px[index - 4 + tempW + 3] >= 127)
							|| (tw < w - 1 && th > 0 && px[index + 4 - tempW + 3] >= 127)
							|| (tw < w - 1 && th < h - 1 && px[index + 4 + tempW + 3] >= 127))
						{
							sum = 1;
							a = px[index + 3];
							if(tw > 0){a += px[index - 4 + 3];++sum;}
							if(tw < w - 1){a += px[index + 4 + 3];++sum;}
							if(th > 0){a += px[index - tempW + 3];++sum;}
							if(th < h - 1){a += px[index + tempW + 3];++sum;}
							if(tw > 0 && th > 0){a += px[index - 4 - tempW + 3];++sum;}
							if(tw > 0 && th < h - 1){a += px[index - 4 + tempW + 3];++sum;}
							if(tw < w - 1 && th > 0){a += px[index + 4 - tempW + 3];++sum;}
							if(tw < w - 1 && th < h - 1){a += px[index + 4 + tempW + 3];++sum;}
							a = a / sum;
							if(a > 127) a = 127;
							p[index + 3] = a << 1;	//Ŀǰ��Ϊ�����ֵ
							//p[index + 3] = 255;
							p[index + 0] = 0;
							p[index + 1] = 0;
							p[index + 2] = 0;
						}else
						{
							p[index + 0] = 0;
							p[index + 1] = 0;
							p[index + 2] = 0;
							//p[index + 3] = px[index + 3] << 1;
						}
					}else
					{
						p[index + 0] = p[index + 0] * px[index + 3] / 255;
						p[index + 1] = p[index + 1] * px[index + 3] / 255;
						p[index + 2] = p[index + 2] * px[index + 3] / 255;
						p[index + 3] = 255;
					}
				}
			}
			XMem::XDELETE_ARRAY(px);
		}
		XVector2 texSize;
		tex[i] = loadTextureFromSDL(image,texSize);
		SDL_FreeSurface(image);
	}
	layout.set(x,y);
	XMem::XDELETE_ARRAY(str);
#if WITH_FULL_ALL_CHINESE == 1
	XMem::XDELETE_ARRAY(pStr);
#endif
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XFontTTF.inl"
#endif
}