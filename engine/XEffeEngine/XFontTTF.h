#ifndef _JIA_XFONTTTF_
#define _JIA_XFONTTTF_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.22
//--------------------------------
//����һ����װ��SDL_TTF���࣬��ϵ���£�_XFontTTF��װ��SDL_TTF,SDL_TTF��װ��Freetype
#include "XEffeEngine.h"
#include "../SDL_ttf/SDL_ttf.h"


extern unsigned int loadTextureFormSDL(SDL_Surface *surface,_XVector2& size);	//��sdlת����OpenGL
//extern int power_of_two(int input);

#define MAX_TTF_FONT_FILE_SUM (128)	//���������������������

#pragma comment(lib, "../../engine/lib/FreeType/freetype253.lib")

enum _XTTFRenderType
{
	RENDER_LATIN1,
	RENDER_UTF8,
	RENDER_UNICODE
};
struct _XFontTTFInfo
{
	_XBool isEnable;							//�Ƿ���Ч
	char filename[MAX_FILE_NAME_LENGTH];	//�ļ���
	TTF_Font *font;
	int fontSize;							//�ߴ�
	int widthOfSpace;						//�ո���Ŀ�ȣ�����Ҳ��������
	_XTTFRenderType type;								//���������
	SDL_Color color;						//�������ɫ

	void setType(_XTTFRenderType t)
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
	void setColor(SDL_Color c){color = c;}
	_XFontTTFInfo()
		:isEnable(XFalse)
		,font(NULL)
		,type(RENDER_UTF8)
	{
		color.r = 0xFF;
		color.g = 0xFF;
		color.b = 0xFF;
		color.unused = 0x00;
	}
};
class _XFontTTF
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XFontTTF();
	_XFontTTF(const _XFontTTF&);
	_XFontTTF &operator= (const _XFontTTF&);
	virtual ~_XFontTTF(); 
public:
	static _XFontTTF& GetInstance();
	//-------------------------------------------
private:
	_XBool m_isInited;

	_XFontTTFInfo m_fontInfo[MAX_TTF_FONT_FILE_SUM];
	//TTF_Font *m_font[MAX_TTF_FONT_FILE_SUM];	//������ָ��
	int m_nowLoadTTFFontFileSum;				//��ǰ���������������
	
	//�����һЩ��������
	//_XTTFRenderType m_renderType;	//�������Ⱦ����
	//SDL_Color m_forecol;	//�������ɫ

	SDL_Surface * getSurface(int index,const char * str);
public:
	int getTTFFileOrder(const char * filename,int ptsize);	//��ȡ����Ҫ���ttf������ļ��ı��
	int loadTTFFile(const char * filename,int ptsize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		//����������ļ���-1��ʾ����ʧ�ܣ��ɹ����ر��
	int releaseTTFFile(int order)	//�ͷ�ĳ��������ļ�
	{
		if(order < 0 || order >= MAX_TTF_FONT_FILE_SUM) return 0;
		if(!m_fontInfo[order].isEnable) return 0;

		TTF_CloseFont(m_fontInfo[order].font);
		m_fontInfo[order].isEnable = XFalse;
		m_fontInfo[order].font = NULL;
		-- m_nowLoadTTFFontFileSum;
		return 1;
	}
	void setAttitude(int fontOrder,_XTTFRenderType type,SDL_Color color)	//�����������Ⱦ����
	{//������û�����߼��ϵĲ�����
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			m_fontInfo[fontOrder].setType(type);
			m_fontInfo[fontOrder].setColor(color);
		}
	//	m_renderType = type;
	//	m_forecol = color;
	}
	void setOutline(int fontOrder,int outLine)
	{
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			TTF_SetFontOutline(m_fontInfo[fontOrder].font,outLine);
		}
	}
	void setStyle(int fontOrder,int style)
	{
		if(fontOrder >= 0 && fontOrder < MAX_TTF_FONT_FILE_SUM && m_fontInfo[fontOrder].isEnable) 
		{
			TTF_SetFontStyle(m_fontInfo[fontOrder].font,style);
		}
	}
	unsigned int getTexture(int fontOrder,const char * str,_XVector2& size);
	void release()	//�ͷ���Դ
	{
		for(int i = 0;i < MAX_TTF_FONT_FILE_SUM;++ i)
		{
			if(m_fontInfo[i].isEnable) 
			{
				TTF_CloseFont(m_fontInfo[i].font);
				m_fontInfo[i].isEnable = XFalse;
				m_fontInfo[i].font = NULL;
			}
		}
		m_nowLoadTTFFontFileSum = 0;
	}
	//������Ϊ��֧��_XNumber,_XFont,_XFontUnicode������Ľӿ�
	_XBool getTextureNumber(int fontOrder,unsigned int &tex,_XVector2 &layout);
	_XBool getTextureFont(int fontOrder,unsigned int &tex,_XVector2 &layout);
	_XBool getTextureFontUnicode(int fontOrder,unsigned int *tex,int& texSum,_XVector2 &layout,_XBool withBlackOutLine = XFalse);
};
#endif