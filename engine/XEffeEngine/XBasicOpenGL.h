#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//#include "gl.h"
//#include "glew.h"
//#include "stdlib.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//�����ڲ���ʽ			��ɫ�ɷ�	��ɫ�ɷ�	��ɫ�ɷ�	alpha�ɷ�
//GL_ALPHA				���ı�		���ı�		���ı�		At
//GL_LUMINANCE			Lt			Lt			Lt			���ı�
//GL_LUMINANCE_ALPHA	Lt			Lt			Lt			At
//GL_INTENSITY			It			It			It			It
//GL_RGB				Rt			Gt			Bt			���ı�
//GL_RGBA				Rt			Gt			Bt			At
enum _XBlendType
{
	BLEND_TWO_DATA,		//�������ӵĻ��
	BLEND_FOUR_DATA,		//�ĸ����ӵĻ��
};
extern unsigned int srcBlendMode[9];
extern unsigned int dstBlendMode[8];

extern void printShaderInfoLog(GLuint obj);						//���shader����Ϣ�ĺ���
extern int printOglError(const char *file, int line);					//��ʾOpenGL�Ĵ�����Ϣ
#define printOpenGLError() printOglError(__FILE__, __LINE__)	//��ӡ����ִ�д���ĺ���
extern void printProgramInfoLog(GLuint obj);					//��ӡ������Ϣ�ĺ���

extern char *textFileRead(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��filename���ļ��ж�ȡshader����	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
extern int textFileWrite(const char *filename, const char *s);					//��shader����sд�뵽filename���ļ���
extern _XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
						_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
extern _XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,_XShaderHandle &pHandle,
						_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ

extern void changeSize(int h, int w);							//SDL�д��ڸı��¼�����Ӧ����

//extern _XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
//					   _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
extern _XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
						 _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
//����һ��û�н��и�����ĺ����ӿڣ�����֮ǰȷ����Դ���ϸ���Լ��
extern _XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//���ķ�֮һ�ߴ��ȡ��Դ

extern void initWindowMatrix();	//��ʼ�����ڵľ���
extern _XBool initOpenGL3D();	//3D���ڵĳ�ʼ��һЩopenGL����
extern _XBool initOpenGL2D();	//�����������ת�Ĵ��ڵĳ�ʼ��һЩopenGL����
//��򵥵Ļ��ߺ���
extern void drawLine(float ax,float ay,float bx,float by,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//��ָ��λ����ָ��λ��֮�仮һ��ֱ�� type 0:ʵ�ߣ�1:����
extern void drawLine(float ax,float ay,float bx,float by,float width,float r,float g,float b,float a,int type = 0);
inline void drawLine(const _XVector2& p0,const _XVector2& p1,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0)
{
	drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
}
inline void drawLine(const _XVector2& p,float angle,float length)	//angle[0 - 360]
{
	drawLine(p,_XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
}
//ͨ��VBO��������
extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b);	
extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a);	
inline void drawRectS(const _XRect &rect)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(rect.left,rect.top);
	glVertex2f(rect.right,rect.top);
	glVertex2f(rect.right,rect.bottom);
	glVertex2f(rect.left,rect.bottom);
	glVertex2f(rect.left,rect.top);
	glEnd();
}
//���߿�ʵ�߻�������
extern void drawRect(const _XRect& rect,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
//���߿��뱳����ɫ
extern void drawRectAntiColor(const _XRect& rect,float width = 1.0f,int type = 0);
//���߿�
inline void drawBoxS(float x,float y,float sizeX,float sizeY)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(x - sizeX,y + sizeY);
	glVertex2f(x - sizeX,y - sizeY);
	glVertex2f(x + sizeX,y - sizeY);
	glVertex2f(x + sizeX,y + sizeY);
	glVertex2f(x - sizeX,y + sizeY);
	glEnd();
}
inline void drawBoxS(const _XVector2 &p1,const _XVector2 &p2,const _XVector2 &p3,const _XVector2 &p4)
{
	glBegin(GL_LINE_STRIP);
	glVertex2fv(p1);
	glVertex2fv(p2);
	glVertex2fv(p4);
	glVertex2fv(p3);
	glVertex2fv(p1);
	glEnd();
}
inline void drawBox(const _XVector2 &pos,const _XVector2 &size)
{
	glBegin(GL_LINE_STRIP);
		glVertex2fv(pos);
		glVertex2f(pos.x + size.x,pos.y);
		glVertex2f(pos.x + size.x,pos.y + size.y);
		glVertex2f(pos.x,pos.y + size.y);
		glVertex2fv(pos);
	glEnd();
}
inline void drawFillBoxS(float x,float y,float w,float h)
{
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x + w,y);
	glVertex2f(x + w,y + h);
	glVertex2f(x,y + h);
	glEnd();
}
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a);
inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f)
{
	glDisable(GL_TEXTURE_2D);
	glColor3f(r,g,b);
	glLineWidth(w);
	drawBoxS(x,y,sizeX,sizeY);
	glEnable(GL_TEXTURE_2D);
}
inline void drawBox(float x,float y,float sizeX,float sizeY)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glLineWidth(1);
	drawBoxS(x,y,sizeX,sizeY);
	glEnable(GL_TEXTURE_2D);
}
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,float width,float r,float g,float b,float a);
extern void drawTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,float width = 0.5f,float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���������
//����
extern void drawPoint(float x,float y,float r,float g,float b,float a);
inline void drawPoint(const _XVector2& p,float r,float g,float b,float a)
{
	drawPoint(p.x,p.y,r,g,b,a);
}
extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
inline void drawPoint(const _XVector2& p,float size,float r,float g,float b,float a)
{
	drawPoint(p.x,p.y,size,r,g,b,a);
}
//Vbo����
extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b);	
extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a);	
//��ʮ��
extern void drawCross(const _XVector2& p,float size = 10.0f,float w = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//��Բ��
//px,px:λ��
//sr:�뾶
//an:������
extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//����Բ
extern void drawCircleLine(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//���Բ��
//x,yԲ�ĵ�λ�ã�
//r0,r1����뾶��
//angleS,angleE��ֹ�Ƕ�[0 - 360]
//an:������
extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//���Բ������͸������
extern void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//��ʵ�ľ���
class _XFColor;
extern void drawFillBox(const _XVector2 &p0,const _XFColor &c0,		//��һ�����Լ���ɫ
						const _XVector2 &p1,const _XFColor &c1,		//�ڶ������Լ���ɫ
						const _XVector2 &p2,const _XFColor &c2,		//���������Լ���ɫ
						const _XVector2 &p3,const _XFColor &c3);	//���ĸ����Լ���ɫ
extern void drawFillBox(float x,float y,float w,float h,
						float r,float g,float b,float a);	//���ʵ�ľ���
extern void drawFillBox(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,bool withLine = false);	//���ʵ�ľ���
extern void drawFillBoxA(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false);	//���ʵ�ľ���
extern void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
//�����û�������Ǹ�ǿ
extern void drawFillBoxExAS(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
//�и�ǿ�������
//extern void drawFillBoxExA(const _XVector2& pos,const _XVector2& size,	
//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
//						bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
inline void drawFillBoxExA(const _XVector2& pos,const _XVector2& size,	
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						bool withLine = false,bool withRoundCorner = false,bool down = false)	//���ʵ�ľ���
{
	drawFillBoxExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
}

//cornerStyle:����Բ�ǵ���ʽÿ2bits����һ���ǣ�����Ϊ���½ǡ����½ǡ����Ͻǡ����Ͻ�
//0Ϊֱ�ǡ�1Ϊ��Բ�ǣ�2��Ϊ��Բ�ǣ�3��Ϊ��һ���������Բ��
//lineStyle:�ߵ���ʽ��4��bits�ֱ�����4�����Ƿ���Ҫ��Ҫ���
extern void drawFillBoxExEx(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//���ʵ�ľ���
extern void drawFillBoxExExA(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
						unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//���ʵ�ľ���
extern void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ���ı���
extern void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false);	//���ʵ���ı���
extern void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize);	//���ڰ�ɫ�ļ��
extern void drawFillTriangle(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ��������
extern void drawFillTriangleEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ��������
//��ʵ�Ķ����
extern void drawFillPolygon(const _XVector2 *p,int sum,float r,float g,float b);
extern void drawFillPolygon(const _XVector2 *p,int sum,const _XVector2 &pos,float angle,float r,float g,float b);
extern void drawFillPolygonEx(const _XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine = false);	//���ӽ�����ʸ�
extern void drawFillPolygonExA(const _XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine = false);	//���ӽ�����ʸ�
//����ĺ����������һ����ͼ
class _XTexture;
extern void drawTexture(_XTexture &tex,const _XVector2 &pos,int cW,int cH,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH);	//ʹ��WITH_XSPRITE_EX��ʱ����Ҫ������ͼ��һ��
extern void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend = false);
extern void drawTextureEx(unsigned int tex,const _XVector2 &pos,const _XVector2 &size,_XVector2 *vArray,
						  _XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ
extern GLuint emptyTexture(int x,int y,_XColorMode mode = COLOR_RGBA);	//ʹ��RGBA�ĸ�ʽ����һ��ָ����С�Ŀհ���ͼ
//withThread���Ƿ�ʹ�ö��߳�
//����Ļ�ӵ�x��y��ʼ��w��h�����ڵ�ͼ�񱣴浽png�ļ�
extern _XBool gameShot(const char *fileName,int w,int h,bool withThread = true,float x = 0.0f,float y = 0.0f);
inline void setVSync(bool sync)	//�����͹رմ�ֱͬ�����رյĻ������Էſ�֡�ʣ������Ļ�������֡��
{	
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString( GL_EXTENSIONS );

	if(strstr(extensions,"WGL_EXT_swap_control") == 0) return;
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
		if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
	}
	//˵��
	//In Linux / GLX
	//Use the GLX_EXT_swap_control extension to control swap interval. Check the GLX-specific extensions string via glXQueryExtensionsString() to verify that the extension is actually present.
	//The extension provides glXSwapIntervalEXT(), which also directly specifies the swap interval. glXSwapIntervalEXT(1) is used to enable vsync; glXSwapIntervalEXT(0) to disable vsync.
}
#include "XBasicFun.h"
#include "string.h"
class _XTexture;
class _XTextureInfo;
class _XFrame;			//�򵥲���
class _XFrameEx;
class _XNodeLine;		//�򵥲���
class _XFontUnicode;	//�򵥲���
class _XSCounter	//���ܼ���������
{
private:
	friend _XTexture;
	friend _XTextureInfo;
	friend _XFrame;
	friend _XFrameEx;
	friend _XNodeLine;
	friend _XFontUnicode;

	int m_counter;
	_XSCounter()
		:m_counter(1)
	{}
};
#define TEX_DEBUG (0)
#define TEX_DEBUG1 (0)
#if TEX_DEBUG
extern int XTSum;
#endif
#if TEX_DEBUG1
extern int CPSum;
#endif

class _XResourceInfo;
class _XPBO;
class _XTexture	//����ӿڽ���ֻ���ڣ�����������û�вü����ⲿ�ӿ�
{
private:
	_XSCounter *m_cp;	//���ü�����
	void release();	//�ͷ�ͼƬ��Դ
	_XBool m_isLoaded;	//�Ƿ�����
	//char * m_fileName;	//��ͼ��Դ���ļ���
	_XResourceInfo * m_resInfo;	//��Դ��ָ��
public:
	GLuint m_texture;	//��ͼ�ı��:0��Ч����Լ0��Ч
	int m_w;			//��ͼ�����ؿ��
	int m_h;			//��ͼ�����ظ߶�
	_XBool getIsEnable()	//��鵱ǰ��ͼ�Ƿ���Ч
	{
		return m_isLoaded;
	}
	_XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
	}
	_XBool reset();
public:
	//resoursePosition��ʾ��Դ��λ�ã�0��ʾ�ⲿ��Դ��1����ʾ�ڲ���Դ��������Դ�ļ����ж�ȡ��Դ	
	_XBool load(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//��ָ���ļ�������ͼƬ��Դ
	//���������ӿ���δʵ��
	_XBool createTexture(int w,int h,_XColorMode mode);			//����һ��ָ����С�Ŀհ���ͼ
	_XBool createWithTexture(int w,int h,_XColorMode mode,unsigned int tex);	//ֱ��ʹ���Ѿ����ڵ���ͼ
	void updateTexture(const _XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const _XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTexture(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);	//����������ͬ����ͬ���ں���ɫ��������
	//void updateTextureGray(const unsigned char *data);

	_XTexture()
	:m_texture(0)
	,m_isLoaded(XFalse)
	,m_cp(NULL)
	//,m_fileName(NULL)
	,m_mode(COLOR_RGBA)
	,m_resInfo(NULL)
	{}

	~_XTexture()
	{
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
		//	XDELETE_ARRAY(m_fileName);
		}
	}
	_XTexture(const _XTexture& temp);	//�������캯��
	_XTexture& operator = (const _XTexture &temp);
};
#endif
