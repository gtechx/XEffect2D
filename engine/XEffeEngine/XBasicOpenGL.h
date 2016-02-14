#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "glew.h"
#include "gl.h"
//#include "stdlib.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
#include "XMath/XFColor.h"
//#include "XBasicFun.h"
#include "XBasicClass.h"
//#include "string.h"
#include "XMemory.h"
#ifdef _WIN64
#pragma comment(lib, "glew64s.lib") 
#else
#pragma comment(lib, "glew32s.lib") 
#endif
#pragma comment(lib, "opengl32.lib") 
namespace XE{
namespace XGL
{
	//�����ڲ���ʽ			��ɫ�ɷ�	��ɫ�ɷ�	��ɫ�ɷ�	alpha�ɷ�
	//GL_ALPHA				���ı�		���ı�		���ı�		At
	//GL_LUMINANCE			Lt			Lt			Lt			���ı�
	//GL_LUMINANCE_ALPHA	Lt			Lt			Lt			At
	//GL_INTENSITY			It			It			It			It
	//GL_RGB				Rt			Gt			Bt			���ı�
	//GL_RGBA				Rt			Gt			Bt			At
	enum XBlendType
	{
		BLEND_TWO_DATA,		//�������ӵĻ��
		BLEND_FOUR_DATA,	//�ĸ����ӵĻ��
	};
	extern const int srcBlendMode[9];
	extern const int dstBlendMode[8];

	extern void printShaderInfoLog(GLuint obj);						//���shader����Ϣ�ĺ���
	extern int printOglError(const char *file, int line);					//��ʾOpenGL�Ĵ�����Ϣ
	#define printOpenGLError() printOglError(__FILE__, __LINE__)	//��ӡ����ִ�д���ĺ���
	extern void printProgramInfoLog(GLuint obj);					//��ӡ������Ϣ�ĺ���

	extern XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
							XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
	extern XBool setShaderFromStr(const char* vStr,const char *fStr,const char *gStr,XShaderHandle &pHandle);	//ֱ�Ӵ��ַ����м���shader
	extern XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,XShaderHandle &pHandle,
							XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
	//extern void changeSize(int h, int w);							//SDL�д��ڸı��¼�����Ӧ����
	//extern XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
	//					   XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
	extern XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
							 XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
	//����һ��û�н��и�����ĺ����ӿڣ�����֮ǰȷ����Դ���ϸ���Լ��
	extern XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//���ķ�֮һ�ߴ��ȡ��Դ
	//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ
	extern GLuint emptyTexture(int x, int y, XColorMode mode = COLOR_RGBA);	//ʹ��RGBA�ĸ�ʽ����һ��ָ����С�Ŀհ���ͼ
	extern void showGLInfoToLog();
	//������Ҫ��GL�����Ż�(��δ��ɣ��ж�״̬�Ƿ���Ҫ�ı��ٶ�״̬���иı�,Ŀǰ�в���ȷ��һ���Ĺ����ܴ������������Ż�)
	//cocos2d�����˸����Ż�,���Խ��.
	//˵����gl�Դ��к�������ȡgl��״̬
	//glGetIntegerv(GL_TEXTURE_BINDING_2D,&t);
	//glIsEnabled(GL_TEXTURE_2D);�ȣ�������Щ���������Ķ��Ƚϴ�
	//+++++++++++++++++++++++++++++++++
	//��Ҫ�Ż��ĺ�����
	//glEnable(GL_TEXTURE_2D);ϵ��
	//glBindTexture(GL_TEXTURE_2D,t);ϵ��
	//glEnable(Blend);ϵ��
	//����ϵ��
	//---------------------------------
	//˵������һ������ʱ��δ��Ч����Ϊû�д���glPopAttribϵ�к���������߼�����
//	extern bool blendFlag;					//blend��״̬
//	extern int blendSrc;					//blend�Ĳ���src
//	extern int blendDst;					//blend�Ĳ���sst
//	extern bool texture2DFlag;				//��ǰTexture2D��״̬
//	extern unsigned int curTextureID[32];	//��ǰ�󶨵���ͼ��ID
//	inline bool GetIsTexture2D(){return texture2DFlag;}
	inline void EnableTexture2D()
	{
//		if(texture2DFlag)
//		{//������
//			bool flag = glIsEnabled(GL_TEXTURE_2D);
//			if(texture2DFlag != flag) printf("EnableTexture2D error!\n");
//			return;
//		}
//		texture2DFlag = true;
		glEnable(GL_TEXTURE_2D);
	}
	inline void DisableTexture2D()
	{
//		if(!texture2DFlag)
//		{
//			bool flag = glIsEnabled(GL_TEXTURE_2D);
//			if(texture2DFlag != flag) printf("DisableTexture2D error!\n");
//			return;
//		}
//		texture2DFlag = false;
		glDisable(GL_TEXTURE_2D);
	}
//	inline bool GetIsBlend(){return blendFlag;}
	inline void EnableBlend()
	{
//		if(blendFlag)
//		{//û����
//			bool flag = glIsEnabled(GL_BLEND);
//			if(blendFlag != flag) printf("EnableBlend error!\n");
//			return;
//		}
//		blendFlag = true;
		glEnable(GL_BLEND);
	}
	inline void DisableBlend()
	{
//		if(!blendFlag)
//		{//û����
//			bool flag = glIsEnabled(GL_BLEND);
//			if(blendFlag != flag) printf("DisableBlend error!\n");
//			return;
//		}
//		blendFlag = false;
		glDisable(GL_BLEND);
	}
//	inline int GetBlendSrc(){return blendSrc;}
//	inline int GetBlendDst(){return blendDst;}
	inline void SetBlendFunc(int src,int dst)
	{//���������������ˮ��Ŀ�п��Լ����������￪���߲������в�ͬ
//		if(blendSrc == src && blendDst == dst)
//		{//������
//			int tmpS,tmpD;
//			glGetIntegerv(GL_BLEND_SRC,&tmpS);
//			glGetIntegerv(GL_BLEND_DST,&tmpD);
//			return;
//		}
//		blendSrc = src;
//		blendDst = dst;
		glBlendFunc(src,dst);
	}
	inline void BindTexture2D(const unsigned int &t,const int &index = 0)
	{
	//	if(curTextureID[index] == t)
	//	{
	//		if(texture2DFlag != flag) printf("BindTexture2D error!\n");
	//		return;
	//	}
	//	curTextureID[index] = t;
		glBindTexture(GL_TEXTURE_2D,t);
	}
//	inline unsigned int getCurBindTexture2D(const int &index = 0){return curTextureID[index];}
	inline void initOpenGLState()
	{
	//	texture2DFlag = glIsEnabled(GL_TEXTURE_2D);
	//	blendFlag = glIsEnabled(GL_BLEND);
	//	memset(curTextureID,0,sizeof(int) * 32);
	//	glGetIntegerv(GL_BLEND_SRC,&blendSrc);
	//	glGetIntegerv(GL_BLEND_DST,&blendDst);
		//glActiveTexture(GL_TEXTURE0);
	}
}
class XTexture;
namespace XRender
{
	//Solid;Dashes;Short Dashes;Dots;Sparse Dots;Dash Dot;Dash Dot Dot
	//ʵ��;����;������;����;ϡ����;�㻮��;��㻮�� 
	enum XLineStyle
	{
		LS_SOLID = 0xFFFF,
		LS_DASHES = 0x00FF,
		LS_SHORT_DASHES = 0x0F0F,
		LS_DOTS = 0x5555,
		LS_SPARSE_DOTS = 0x2222,
		LS_DASH_DOT = 0xFE10,
		LS_DASH_DOT_DOT = 0x24FF,
	};
	//��򵥵Ļ��ߺ���
	extern void drawLine(float ax,float ay,float bx,float by,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	//��ָ��λ����ָ��λ��֮�仮һ��ֱ�� type 0:ʵ�ߣ�1:����
	extern void drawLine(float ax, float ay, float bx, float by, float width, const XFColor &c, XLineStyle type = LS_SOLID);
	inline void drawLine(float ax, float ay, float bx, float by, float width, float r, float g, float b, float a, XLineStyle type = LS_SOLID)
	{
		drawLine(ax,ay,bx,by,width,XFColor(r,g,b,a),type);
	}
	inline void drawLine(const XVector2& p0, const XVector2& p1, float width, const XFColor & c, XLineStyle type = LS_SOLID)
	{
		drawLine(p0.x,p0.y,p1.x,p1.y,width,c.fR,c.fG,c.fB,c.fA,type);
	}
	inline void drawLine(const XVector2& p0, const XVector2& p1, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	{
		drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
	}
	inline void drawLine(const XVector2& p,float angle,float length)	//angle[0 - 360]
	{
		drawLine(p,XVector2(p.x + length * cos(angle * DEGREE2RADIAN),p.y + length * sin(angle * DEGREE2RADIAN)));
	}
	//����뱳����ɫ����
	extern void drawLineAntiColor(float ax,float ay,float bx,float by,float width = 1.0f,XLineStyle type = LS_SOLID);
	//ͨ��VBO��������
	extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b);	
	extern void drawLinesVbo(unsigned int v,int pointSum,float w,float r,float g,float b,float a);	
	inline void drawRectS(const XRect &rect)
	{
		//glBegin(GL_LINE_STRIP);
		glBegin(GL_LINE_LOOP);
		glVertex2f(rect.left,rect.top);
		glVertex2f(rect.right,rect.top);
		glVertex2f(rect.right,rect.bottom);
		glVertex2f(rect.left,rect.bottom);
		//glVertex2f(rect.left,rect.top);
		glEnd();
	}
	//���߿�ʵ�߻�������
	extern void drawRect(const XVector2& pos,const XVector2& size,float width, const XFColor &c);
	extern void drawRect(const XRect& rect, float width, const XFColor &c, XLineStyle type = LS_SOLID);
	inline void drawRect(const XRect& rect, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	{
		drawRect(rect,width,XFColor(r,g,b,a),type);
	}
	//���߿��뱳����ɫ
	extern void drawRectAntiColor(const XRect& rect, float width = 1.0f, XLineStyle type = LS_SOLID);
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
	inline void drawBoxS(const XVector2 &p1,const XVector2 &p2,const XVector2 &p3,const XVector2 &p4)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p4);
		glVertex2fv(p3);
		glVertex2fv(p1);
		glEnd();
	}
	inline void drawBox(const XVector2 &pos,const XVector2 &size)
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
	extern void drawBox(float x,float y,float sizeX,float sizeY,int w,const XFColor &c);
	inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a)
	{
		drawBox(x,y,sizeX,sizeY,w,XFColor(r,g,b,a));
	}
	inline void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f)
	{
		XGL::DisableTexture2D();
		glColor3f(r,g,b);
		glLineWidth(w);
		drawBoxS(x,y,sizeX,sizeY);
	}
	inline void drawBox(float x,float y,float sizeX,float sizeY)
	{
		XGL::DisableTexture2D();
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glLineWidth(1);
		drawBoxS(x,y,sizeX,sizeY);
	}
	extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
	extern void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	extern void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,const XFColor & c);
	inline void drawBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,float width,float r,float g,float b,float a)
	{
		drawBox(p0,p1,p2,p3,width,XFColor(r,g,b,a));
	}
	//���������
	extern void drawTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,float width = 0.5f,float r = 1.0f,float g = 1.0f,float b = 1.0f);	
	//�����������
	extern void drawTriangle(const XVector2& p,float size,float width,const XFColor &color);
	//����
	extern void drawPoint(float x,float y,float r,float g,float b,float a);
	inline void drawPoint(const XVector2& p,float r,float g,float b,float a)
	{
		drawPoint(p.x,p.y,r,g,b,a);
	}
	extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
	inline void drawPoint(const XVector2& p,float size,float r,float g,float b,float a)
	{
		drawPoint(p.x,p.y,size,r,g,b,a);
	}
	//Vbo����
	extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b);	
	extern void drawPointsVbo(unsigned int v,int pointSum,float s,float r,float g,float b,float a);	
	//��ʮ��
	extern void drawCross(const XVector2& p,float size = 10.0f,float w = 1.0f,float r = 1.0f,float g = 1.0f,float b = 1.0f);
	extern void drawCross(const XVector2& p,float size,float w,const XFColor& color);
	//���Ժ�
	extern void drawMaskRight(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//�����
	extern void drawMaskWrong(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//��������������ı�ǵ����
	extern void drawLeft(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawRight(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawUp(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	extern void drawDown(const XVector2& p,const XVector2& size,float width,const XFColor& color);
	//��Բ��
	//px,px:λ��
	//sr:�뾶
	//an:������
	extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawCircle(const XVector2&pos, float sr, int an,const XFColor &color)
	{
		drawCircle(pos.x,pos.y,sr,an,color.fR,color.fG,color.fB,color.fA);
	}
	extern void drawCircleEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawCircleEx(const XVector2&pos, float sr,const XFColor &color)
	{
		drawCircleEx(pos.x,pos.y,sr,color.fR,color.fG,color.fB,color.fA);
	}
	//����Բ
	extern void drawCircleLine(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleLine(const XVector2 &pos, float sr, int an,float w,const XFColor &color);
	extern void drawCircleLineEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleLineEx(const XVector2 &pos, float sr,float w,const XFColor &color);
	//���Բ��
	//x,yԲ�ĵ�λ�ã�
	//r0,r1����뾶��
	//angleS,angleE��ֹ�Ƕ�[0 - 360]
	//an:������
	extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	inline void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,const XFColor &c)
	{
		drawFillRing(x,y,r0,r1,angleS,angleE,an,c.fR,c.fG,c.fB,c.fA);
	}
	//���Բ������͸������
	extern void drawFillRingEx(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	//��ʵ�ľ���
	extern void drawFillBox(const XVector2 &p0,const XFColor &c0,		//��һ�����Լ���ɫ
							const XVector2 &p1,const XFColor &c1,		//�ڶ������Լ���ɫ
							const XVector2 &p2,const XFColor &c2,		//���������Լ���ɫ
							const XVector2 &p3,const XFColor &c3);	//���ĸ����Լ���ɫ
	extern void drawFillBox(float x,float y,float w,float h,
		const XFColor & c);	//���ʵ�ľ���
	inline void drawFillBox(float x,float y,float w,float h,
							float r,float g,float b,float a)	//���ʵ�ľ���
	{
		drawFillBox(x,y,w,h,XFColor(r,g,b,a));
	}
	extern void drawFillBox(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,bool withLine = false);	//���ʵ�ľ���
	extern void drawFillBoxA(const XVector2& pos,const XVector2& size,
		const XFColor &c,bool withLine = false);	//���ʵ�ľ���
	inline void drawFillBoxA(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false)	//���ʵ�ľ���
	{
		drawFillBoxA(pos,size,XFColor(r,g,b,a),withLine);
	}
	extern void drawFillBoxEx(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
	//�����û�������Ǹ�ǿ
	extern void drawFillBoxExAS(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
	inline void drawFillBoxExAS(const XVector2& pos,const XVector2& size,	
		const XFColor &c,bool withLine = false,bool withRoundCorner = false,bool down = false)	//���ʵ�ľ���
	{
		drawFillBoxExAS(pos,size,c.fR,c.fG,c.fB,c.fA,withLine,withRoundCorner,down);
	}
	//�и�ǿ�������
	extern void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							bool withLine = false,bool withRoundCorner = false,bool down = false);	//���ʵ�ľ���
	//����Ч
	//inline void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
	//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
	//						bool withLine = false,bool withRoundCorner = false,bool down = false)	//���ʵ�ľ���
	//{
	//	drawFillBoxExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
	//}
	inline void drawFillBoxExA(const XVector2& pos,const XVector2& size,	
							const XFColor &c,
							bool withLine = false,bool withRoundCorner = false,bool down = false)
	{
		drawFillBoxExA(pos,size,c.fR,c.fG,c.fB,c.fA,withLine,withRoundCorner,down);
	}
	//ָ��ĳһ��ļ�ͷ,angle��λ��
	inline void drawArrow(const XVector2& arm,float angle,float length)
	{
		drawLine(arm,angle,length);	//������
		drawLine(arm,angle + 30,10);
		drawLine(arm,angle - 30,10);
	}
	//����ͷ,��start��end�ļ�ͷ
	inline void drawArrow(const XVector2& start,const XVector2& end)
	{
		float len = start.getLength(end);
		float angle = end.getAngle(start) * RADIAN2DEGREE;
		//���ֲ�
		drawArrow(end,angle,len);
	}
	//cornerStyle:����Բ�ǵ���ʽÿ2bits����һ���ǣ�����Ϊ���½ǡ����½ǡ����Ͻǡ����Ͻ�
	//0Ϊֱ�ǡ�1Ϊ��Բ�ǣ�2��Ϊ��Բ�ǣ�3��Ϊ��һ���������Բ��
	//lineStyle:�ߵ���ʽ��4��bits�ֱ�����4�����Ƿ���Ҫ��Ҫ���
	extern void drawFillBoxExEx(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//���ʵ�ľ���
	extern void drawFillBoxExExA(const XVector2& pos,const XVector2& size,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
							unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false);	//���ʵ�ľ���
	inline void drawFillBoxExExA(const XVector2& pos,const XVector2& size,
		const XFColor &c,unsigned char lineStyle = 0,unsigned char cornerStyle = 0,bool down = false)	//���ʵ�ľ���
	{
		drawFillBoxExExA(pos,size,c.fR,c.fG,c.fB,c.fA,lineStyle,cornerStyle,down);
	}
	extern void drawFillBox(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ���ı���
	extern void drawFillBoxEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,const XVector2& p3,
							float r = 1.0f,float g = 1.0f,float b = 1.0f,
							bool withLine = false);	//���ʵ���ı���
	extern void drawFillBoxBW(const XVector2& pos,const XVector2& size,const XVector2& cSize);	//���ڰ�ɫ�ļ��
	extern void drawFillTriangle(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ��������
	extern void drawFillTriangleEx(const XVector2& p0,const XVector2& p1,const XVector2& p2,
							float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ��������
	//��ʵ�Ķ����
	extern void drawFillPolygon(const XVector2 *p,int sum,float r,float g,float b);
	extern void drawFillPolygon(const XVector2 *p,int sum,const XVector2 &pos,float angle,float r,float g,float b);
	extern void drawFillPolygonEx(const XVector2 *p,const float *c,int sum,float r,float g,float b,bool withLine = false);	//���ӽ�����ʸ�
	extern void drawFillPolygonExA(const XVector2 *p,const float *c,int sum,float r,float g,float b,float a,bool withLine = false);	//���ӽ�����ʸ�
	inline void drawFillPolygonExA(const XVector2 *p,const float *c,int sum,const XFColor &cl,bool withLine = false)	//���ӽ�����ʸ�
	{
		drawFillPolygonExA(p,c,sum,cl.fR,cl.fG,cl.fB,cl.fA,withLine);
	}
	//����ĺ����������һ����ͼ
	extern void drawTexture(XTexture &tex,const XVector2 &pos,int cW,int cH,XVector2 *vArray,XVector2 *uArray,int arrayW,int arrayH);	//ʹ��WITHXSPRITE_EX��ʱ����Ҫ������ͼ��һ��
	extern void drawTexture(unsigned int tex,const XVector2 &pos,int texW,int texH,bool blend = false);
	extern void drawTextureEx(unsigned int tex,const XVector2 &pos,const XVector2 &size,XVector2 *vArray,
							  XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
}

#define TEX_DEBUG (0)
#define TEX_DEBUG1 (0)
#if TEX_DEBUG
extern int XTSum;
#endif
#if TEX_DEBUG1
extern int CPSum;
#endif

class XResourceInfo;
class XPBO;
class XTexture	//����ӿڽ���ֻ���ڣ�����������û�вü����ⲿ�ӿ�
{
private:
	XSCounter *m_cp;	//���ü�����
	void release();	//�ͷ�ͼƬ��Դ
	XBool m_isLoaded;	//�Ƿ�����
	XResourceInfo * m_resInfo;	//��Դ��ָ��
public:
	GLuint m_texture;	//��ͼ�ı��:0��Ч����Լ0��Ч
	int m_w;			//��ͼ�����ؿ��
	int m_h;			//��ͼ�����ظ߶�
	XBool getIsEnable() {return m_isLoaded;}//��鵱ǰ��ͼ�Ƿ���Ч
	XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		XGL::EnableTexture2D();
		XGL::BindTexture2D(m_texture);
	}
	XBool reset();
public:
	//resoursePosition��ʾ��Դ��λ�ã�0��ʾ�ⲿ��Դ��1����ʾ�ڲ���Դ��������Դ�ļ����ж�ȡ��Դ	
	XBool load(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//��ָ���ļ�������ͼƬ��Դ
	//���������ӿ���δʵ��
	XBool createTexture(int w,int h,XColorMode mode);			//����һ��ָ����С�Ŀհ���ͼ
	XBool createWithTexture(int w,int h,XColorMode mode,unsigned int tex);	//ֱ��ʹ���Ѿ����ڵ���ͼ
	void updateTexture(const XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTexture(const void *data,int w = 0,int h = 0,int x = 0,int y = 0)		//ʹ��ָ�����ݸ�����ͼ
	{
		if(!m_isLoaded || data == NULL) return;	//PBO��ʱ�����Ϊ NULL
		updateTextureX(data,w,h,x,y);
	}
	void updateTextureR2B(const void *data,int w = 0,int h = 0,int x = 0,int y = 0)	//����������ͬ����ͬ���ں���ɫ��������
	{
		if(!m_isLoaded || data == NULL) return;	//PBO��ʱ�����Ϊ NULL
		updateTextureR2BX(data,w,h,x,y);
	}
	void updateTextureEx(const void *data);		//������ʹ��������������ǳ�������������ܻ����������ܺ�
	void updateTextureR2BEx(const void *data);	//������ʹ��������������ǳ�������������ܻ����������ܺ�
private:
	void updateTextureX(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2BX(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);	//����������ͬ����ͬ���ں���ɫ��������
public:
	XTexture()
		:m_texture(0)
		,m_isLoaded(XFalse)
		,m_cp(NULL)
		,m_mode(COLOR_RGBA)
		,m_resInfo(NULL)
		,m_w(0)
		,m_h(0)
	{}

	~XTexture()
	{
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			-- CPSum;
			printf("%d-\n",CPSum);
#endif
			XMem::XDELETE(m_cp);
		}
	}
	XTexture(const XTexture& temp);	//�������캯��
	XTexture& operator = (const XTexture &temp);
};
}
#endif
