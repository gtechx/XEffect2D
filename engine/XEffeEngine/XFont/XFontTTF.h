#ifndef _JIA_XFONTTTF_
#define _JIA_XFONTTTF_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.22
//--------------------------------
//����һ����װ��SDL_TTF���࣬��ϵ���£�XFontTTF��װ��SDL_TTF,SDL_TTF��װ��Freetype
#include "../SDL_ttf/SDL_ttf.h"
#include "../XMath/XVector2.h"
#include <vector>
namespace XE{
extern unsigned int loadTextureFromSDL(SDL_Surface *surface,XVec2& size);	//��sdlת����OpenGL
extern unsigned int loadTextureFromSDLSimple(SDL_Surface *surface);	//��sdlת����OpenGL,Ĭ�ϴ������ͼ�Ѿ����ϸ�ʽ�ͳߴ��Ҫ��
//extern int power_of_two(int input);

#define MAX_TTF_FONT_FILE_SUM (16)	//���������������������
#ifdef _WIN64
//#pragma comment(lib, "FreeType/libfreetype64.lib")
#pragma comment(lib, "FreeType/freetype253_64.lib")
#else
#pragma comment(lib, "FreeType/freetype253.lib")
//#pragma comment(lib, "FreeType/libfreetype32.lib")
#endif

enum XTTFRenderType
{
	RENDER_LATIN1,
	RENDER_UTF8,
	RENDER_UNICODE
};
struct XFontTTFInfo
{
	XBool isEnable;							//�Ƿ���Ч
	//char filename[MAX_FILE_NAME_LENGTH];	//�ļ���
	std::string filename;
	TTF_Font *font;
	int fontSize;							//��������ش���
	int fontDistance;						//�������еľ���
	int widthOfSpace;						//�ո���Ŀ�ȣ�����Ҳ��������
	XTTFRenderType type;								//���������
	SDL_Color color;						//�������ɫ

	void setType(XTTFRenderType t);
	void setColor(SDL_Color c){color = c;}
	XFontTTFInfo()
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
class XFontTTF
{
public:
	XFontTTF();
	virtual ~XFontTTF(); 
protected:
	XFontTTF(const XFontTTF&);
	XFontTTF &operator= (const XFontTTF&);
private:
	XBool m_isInited;

	XFontTTFInfo m_fontInfo[MAX_TTF_FONT_FILE_SUM];
	//TTF_Font *m_font[MAX_TTF_FONT_FILE_SUM];	//������ָ��
	int m_curLoadTTFFontFileSum;				//��ǰ���������������
	
	//�����һЩ��������
	//XTTFRenderType m_renderType;	//�������Ⱦ����
	//SDL_Color m_forecol;	//�������ɫ

	SDL_Surface * getSurface(int index,const char * str);
	SDL_Surface * getSurfaceEx(int index, const char * str, SDL_Surface * dst);

	bool loadFromFolder(int i,const char *filename,int ptsize);	//���ļ�����������Դ
	bool loadFromPacker(int i,const char *filename,int ptsize);	//��ѹ������������Դ
	bool loadFromWeb(int i,const char *filename,int ptsize);		//����ҳ�ж�ȡ��Դ
public:
	int getTTFFileOrder(const char * filename,int ptsize);	//��ȡ����Ҫ���ttf������ļ��ı��
	int loadTTFFile(const char * filename,int ptsize,int fontDis,
		XResPos resPos = RES_SYS_DEF);		//����������ļ���-1��ʾ����ʧ�ܣ��ɹ����ر��
	int releaseTTFFile(int order);	//�ͷ�ĳ��������ļ�
	void setAttitude(int fontOrder,XTTFRenderType type,SDL_Color color);	//�����������Ⱦ����
	void setOutline(int fontOrder,int outLine);
	void setStyle(int fontOrder,int style);
	unsigned int getTexture(int fontOrder,const char * str,XVec2& size);
	void release();	//�ͷ���Դ
	//������Ϊ��֧��XNumber,XFont,XFontUnicode������Ľӿ�
	XBool getTextureNumber(int fontOrder,unsigned int &tex,XVec2& layout);
	XBool getTextureFont(int fontOrder,unsigned int &tex,XVec2& layout);
	//Ŀǰ���������������̫���Ҫ�ص��Ż�
	//withOpr,�Ƿ�ʹ���Ż������ʹ���Ż���������Ч�ʵ��ǿ�����������ִ�λ
	XBool getTextureFontUnicode(int fontOrder,std::vector<unsigned int> &tex,
		XVec2& layout,XBool withBlackOutLine = XFalse,bool withOpr = false);
private:
	//һ���Բ���һ�����ַ���
	bool addALineStr(int fontOrder, int lineIndex, char *str, int strLen,
		XStringConverter &cver, SDL_Surface *image, SDL_Surface *&tmpFace);
	//һ���ַ�һ���ַ��Ĳ���һ���ַ���
	bool addALineStrByChar(int fontOrder, int lineIndex, char *str, int strLen,
		XStringConverter &cver, SDL_Surface *image, SDL_Surface *&tmpFace);
	//�Ƴ��ַ����е�ռλ�ո�
	int removePlaceholder(char *str, int len);
};
#if WITH_INLINE_FILE
#include "XFontTTF.inl"
#endif
}
#endif