#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "glew.h"
#include "gl.h"
#include "XBasicFun.h"
#include "XMath/XVector2.h"
#include "XMath/XRect.h"
#include "XMath/XFColor.h"
#include "XBasicClass.h"
#include "XMemory.h"
#ifdef _WIN64
#pragma comment(lib, "gl/glew64s.lib") 
#else
#pragma comment(lib, "gl/glew32s.lib") 
#endif
//#pragma comment(lib, "gl/opengl.lib") 
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
//	enum XBlendType
//	{
//		BLEND_TWO_DATA,		//�������ӵĻ��
//		BLEND_FOUR_DATA,	//�ĸ����ӵĻ��
//	};
	enum XBlendModel
	{
		BLEND_NULL,
		BLEND_ALPHA,	//45
		BLEND_ADD,		//41
		BLEND_MULTIPLY,
		BLEND_SCREEN,
		BLEND_SUBTRACT,
		BLEND_MODEL0,	//4515
		BLEND_MODEL1,	//15
		BLEND_MODEL2,	//11
		BLEND_MODEL3,	//25
	};
//	extern const int srcBlendMode[9];
//	extern const int dstBlendMode[8];
	enum XShaderResult
	{
		CMP_SUCCESS,
		CMP_WARNING,
		CMP_ERROR,
	};
	//�����Ƿ�ɹ���0�ɹ���1�о��棬2�д���
	extern XShaderResult printShaderInfoLog(GLuint obj);						//���shader����Ϣ�ĺ���
	extern int printOglError(const char *file, int line);					//��ʾOpenGL�Ĵ�����Ϣ
	#define printOpenGLError() printOglError(__FILE__, __LINE__)	//��ӡ����ִ�д���ĺ���
	//�����Ƿ�ɹ���0�ɹ���1�о��棬2�д���
	extern XShaderResult printProgramInfoLog(GLuint obj);					//��ӡ������Ϣ�ĺ���

	extern XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,
							XResPos resPos = RES_SYS_DEF);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
	extern XBool setShaderFromStr(const char* vStr,const char *fStr,const char *gStr,XShaderHandle &pHandle);	//ֱ�Ӵ��ַ����м���shader
	extern XBool setShader(const char* vertFile,const char* fragFile,const char* geomFile,XShaderHandle &pHandle,
							XResPos resPos = RES_SYS_DEF);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
	//extern void changeSize(int h, int w);							//SDL�д��ڸı��¼�����Ӧ����
	//extern XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
	//					   XResPos resPos = RES_SYS_DEF);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
	extern XBool TextureLoadEx(GLuint &texture, const char *filename, int * w = NULL, int * h = NULL,
		XResPos resPos = RES_SYS_DEF, bool isInThread = false);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
	//����һ��û�н��и�����ĺ����ӿڣ�����֮ǰȷ����Դ���ϸ���Լ��
	extern XBool TextureLoadQuarter(GLuint &texture,const char *filename,XResPos resPos = RES_SYS_DEF);	//���ķ�֮һ�ߴ��ȡ��Դ
	//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ,�������ڲ����ݸ�ʽʱΪĬ�ϸ�ʽ
	extern GLuint emptyTexture(int x, int y, XColorMode mode,int &internalFormat,int filter = GL_LINEAR,int MSSum = 0);	//ʹ��RGBA�ĸ�ʽ����һ��ָ����С�Ŀհ���ͼ
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
	inline void SetBlendFunc(int src, int dst)
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
		glBlendFunc(src, dst);
	}
	inline void setBlendAlpha()
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	inline void setBlendAdd()	//41
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	inline void setBlendMultiply()
	{
		EnableBlend();
		SetBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
	}
	inline void setBlendScreen()
	{
		EnableBlend();
		SetBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
	}
	inline void setBlendSubtract()
	{
		EnableBlend();
		SetBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}	
	inline void setBlendInverse()
	{//�뱳����ɫ
		EnableBlend();
		XGL::SetBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
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
	inline void setBlendModel(XBlendModel m)
	{
		switch (m)
		{
		case BLEND_NULL:DisableBlend(); break;
		case BLEND_ALPHA:setBlendAlpha(); break;
		case BLEND_ADD:setBlendAdd(); break;
		case BLEND_MULTIPLY:setBlendMultiply(); break;
		case BLEND_SCREEN:setBlendScreen();	break;
		case BLEND_SUBTRACT:setBlendSubtract();break;
		case BLEND_MODEL0:
			EnableBlend();
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_MODEL1:
			EnableBlend();
			SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_MODEL2:
			EnableBlend();
			SetBlendFunc(GL_ONE, GL_ONE);
			break;
		case BLEND_MODEL3:
			EnableBlend();
			SetBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		default:
			printf("Blend error!");
			//LogStr("Blend error!");
			//aseert()
			break;
		}
	}
//	inline int GetBlendSrc(){return blendSrc;}
//	inline int GetBlendDst(){return blendDst;}
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
	inline void BindTexture2DMS(const unsigned int &t,const int &index = 0)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,t);
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
	//����ķ�ʽ��һЩʵ���Եķ���
	// read back pixel values
	extern void readFrame(void* backbuffer, int width, int height);
	//����ĺ궨�����ڼ��GL�ڲ��Ĵ��󣬽��������е��õ�GL�ӿڵĵط���������Ա����ų�GL�ڲ�����
#if defined(NDEBUG)
#define glCheck(x) x
#else
#define glCheck(x) {x; glMsg(#x, __LINE__, __FILE__);}
	extern void glMsg(const char* msg, long line, const char* file);
#endif
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
	extern void drawLine(const XVec2&a, const XVec2&b, float width = 1.0f,
		const XFColor&c = XFColor::white);
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		float r, float g, float b)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, XFColor(r, g, b));
	}
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		const XFColor&c)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, c);
	}
	//��ָ��λ����ָ��λ��֮�仮һ��ֱ�� type 0:ʵ�ߣ�1:����
	extern void drawLine(const XVec2& p0, const XVec2& p1, float width,
		const XFColor& c, XLineStyle type);
	inline void drawLine(float ax, float ay, float bx, float by, float width,
		const XFColor& c, XLineStyle type)
	{
		drawLine(XVec2(ax, ay), XVec2(bx, by), width, c, type);
	}
	extern void drawLines(const std::vector<XVec2>&points, float width = 1.0f, const XFColor& c = XFColor::white);
	//	inline void drawLine(const XVec2& p0, const XVec2& p1, float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f, XLineStyle type = LS_SOLID)
	//	{
	//		drawLine(p0.x,p0.y,p1.x,p1.y,width,r,g,b,a,type);
	//	}
	inline void drawLine(const XVec2& p, float angle, float length, float w = 1.0f, const XFColor& c = XFColor::white)	//angle[0 - 360]
	{
		drawLine(p, p + XMath::getRotateRate(angle * DEGREE2RADIAN) * length, w, c);
	}
	//����뱳����ɫ����
	extern void drawLineAntiColor(const XVec2& a, const XVec2& b, float width = 1.0f, XLineStyle type = LS_SOLID);
	//ͨ��VBO��������
	extern void drawLinesVbo(unsigned int v, int pointSum, float w, float r, float g, float b);
	extern void drawLinesVbo(unsigned int v, int pointSum, float w, const XFColor& c);
	//���߿��뱳����ɫ
	extern void drawRectAntiColor(const XRect& rect, float width = 1.0f, XLineStyle type = LS_SOLID);
	//���߿�
	//��˳�����ϡ����ϡ����¡�����
	inline void drawQuadBase(const XVec2& p1, const XVec2& p2, const XVec2& p3, const XVec2& p4)
	{
		glBegin(GL_LINE_LOOP);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glVertex2fv(p4);
		glEnd();
	}
	//pos:���Ͻ����꣬size:ȫ�ߴ�
	inline void drawRect(const XVec2& pos, const XVec2& size)
	{
		drawQuadBase(pos, XVec2(pos.x + size.x, pos.y),
			pos + size, XVec2(pos.x, pos.y + size.y));
	}
	inline void drawQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float width = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f)
	{
		glColor3f(r, g, b);
		glLineWidth(width);
		XGL::DisableTexture2D();
		drawQuadBase(p0, p1, p2, p3);
	}
	extern void drawQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float width, const XFColor& c);
	inline void drawRectBase(const XRect& rect)
	{
		drawQuadBase(rect.getLT(), rect.getRT(), rect.getRB(), rect.getLB());
	}
	//���߿�ʵ�߻�������
	//pos:�������꣬size����ߴ�
	extern void drawRect(const XVec2& pos, const XVec2& size, float width, const XFColor& c);
	extern void drawRect(const XRect& rect, float width = 1.0f, const XFColor& c = XFColor::white, XLineStyle type = LS_SOLID);
	//p:���Ͻ����꣬s��ȫ�ߴ�
	inline void drawFillRectBase(const XVec2&p, const XVec2&s)
	{
		glBegin(GL_QUADS);
		glVertex2fv(p);
		glVertex2f(p.x + s.x, p.y);
		glVertex2fv(p + s);
		glVertex2f(p.x, p.y + s.y);
		glEnd();
	}
	//inline void drawFillBoxS(float x,float y,float w,float h)
	//{
	//	drawFillBoxS(XVec2(x,y),XVec2(w,h));
	//}
	//x,y���������꣬
	//sizeX,sizeY:��ߴ�
	extern void drawRectEx(float x, float y, float sizeX, float sizeY, float angle, const XFColor& c = XFColor::white);
	//���������
	extern void drawTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2, float width = 0.5f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	//�����������
	extern void drawTriangle(const XVec2& p, float size, float width, const XFColor& color);
	//����
//	extern void drawPoint(float x,float y,float r,float g,float b,float a);
//	inline void drawPoint(const XVec2& p,float r,float g,float b,float a)
//	{
//		drawPoint(p.x,p.y,r,g,b,a);
//	}
	extern void drawPoint(const XVec2& p, const XFColor& color);
	extern void drawPoints(const std::vector<XVec2>& ps, float size, const XFColor& color);
	//	extern void drawPoint(float x,float y,float size,float r,float g,float b,float a);
	//	inline void drawPoint(const XVec2& p,float size,float r,float g,float b,float a)
	//	{
	//		drawPoint(p.x,p.y,size,r,g,b,a);
	//	}
	extern void drawPoint(const XVec2& p, float size, const XFColor& color);
	//Vbo����
	extern void drawPointsVbo(unsigned int v, int pointSum, float s, float r, float g, float b);
	extern void drawPointsVbo(unsigned int v, int pointSum, float s, const XFColor& c);
	//��ʮ��
	extern void drawCross(const XVec2& p, float size = 10.0f, float w = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);
	extern void drawCross(const XVec2& p, float size, float w, const XFColor& color);
	//���Ժ�
	extern void drawMaskRight(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	//�����
	extern void drawMaskWrong(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	//��������������ı�ǵ����
	extern void drawLeft(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawRight(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawUp(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDown(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawSave(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawLoad(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDefault(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawDownload(const XVec2& p, const XVec2& size, float width, const XFColor& color);
	extern void drawUpdate(const XVec2& p, const XVec2& size, float width, const XFColor& color);

	//��Բ��
	//px,px:λ��
	//sr:�뾶
	//an:������
//	extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircle(const XVec2&pos, float sr, int an, const XFColor& color);
	//	extern void drawCircleEx(float px, float py, float sr,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawCircleEx(const XVec2&pos, float sr, const XFColor& color);
	//����Բ
	extern void drawEllipseLine(const XVec2&pos, float ar, float br, int an, const XFColor&c = XFColor::white);
	extern void drawEllipseLineEx(const XVec2&pos, float ar, float br, int an, float angle, const XFColor&color);
	extern void drawFillEllipseEx(const XVec2&pos, float ar, float br, int an, float angle, const XFColor&color);
	extern void drawCircleLine(const XVec2& pos, float sr, int an, float w, const XFColor& color);
	inline void drawCircleLine(float px, float py, float sr, int an, const XFColor&c = XFColor::white)
	{
		drawCircleLine(XVec2(px, py), sr, an, 1.0f, c);
	}
	extern void drawCircleLineEx(const XVec2& pos, float sr, float w, const XFColor& color);
	inline void drawCircleLineEx(float px, float py, float sr, const XFColor&c = XFColor::white)
	{
		drawCircleLineEx(XVec2(px, py), sr, 1.0f, c);
	}
	//���һ������״��������
	extern void drawHexagonLine(const XVec2& pos, float sr, float w, const XFColor& color);
	//����������
	extern void drawFillHexagon(const XVec2& pos, float sr, const XFColor& color);
	//���Բ��
	//x,yԲ�ĵ�λ�ã�
	//r0,r1����뾶��
	//angleS,angleE��ֹ�Ƕ�[0 - 360]
	//an:������
	//extern void drawFillRing(float x,float y,float r0,float r1,float angleS,float angleE,int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
	extern void drawFillRing(const XVec2& pos, float r0, float r1, float angleS, float angleE, int an, const XFColor& c = XFColor::white);
	//���Բ������͸������
	extern void drawFillRingEx(const XVec2& pos, float r0, float r1, float angleS, float angleE,
		int an, const XFColor& c = XFColor::white);
	//�������
	extern void drawFillFan(const XVec2& pos, float r, float angleS, float angleE, int an, const XFColor& c = XFColor::white);
	//��ʵ���ı���
	extern void drawFillQuad(const XVec2& p0, const XFColor& c0,		//��һ�����Լ���ɫ
		const XVec2& p1, const XFColor& c1,		//�ڶ������Լ���ɫ
		const XVec2& p2, const XFColor& c2,		//���������Լ���ɫ
		const XVec2& p3, const XFColor& c3);	//���ĸ����Լ���ɫ
	extern void drawFillQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		const XFColor& c);	//���ĸ����Լ���ɫ
	//���Ͻ������ȫ�ߴ�
	inline void drawFillRect(const XVec2& pos, const XVec2& size,
		const XFColor& c)	//���ʵ�ľ���
	{
		XGL::setBlendAlpha();
		XGL::DisableTexture2D();
		glColor4fv(c);
		drawFillRectBase(pos, size);
		XGL::DisableBlend();
	}
	extern void drawFillRect(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, bool withLine = false);	//���ʵ�ľ���
	extern void drawFillRectA(const XVec2& pos, const XVec2& size,
		const XFColor& c, bool withLine = false);	//���ʵ�ľ���
//	inline void drawFillRectA(const XVec2& pos,const XVec2& size,
//							float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,bool withLine = false)	//���ʵ�ľ���
//	{
//		drawFillRectA(pos,size,XFColor(r,g,b,a),withLine);
//	}
	extern void drawFillRectEx(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//���ʵ�ľ���
//�����û�������Ǹ�ǿ
	extern void drawFillRectExAS(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//���ʵ�ľ���
	inline void drawFillRectExAS(const XVec2& pos, const XVec2& size,
		const XFColor& c, bool withLine = false, bool withRoundCorner = false, bool down = false)	//���ʵ�ľ���
	{
		drawFillRectExAS(pos, size, c.r, c.g, c.b, c.a, withLine, withRoundCorner, down);
	}
	//�и�ǿ�������
	extern void drawFillRectExA(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		bool withLine = false, bool withRoundCorner = false, bool down = false);	//���ʵ�ľ���
//����Ч
//inline void drawFillRectExA(const XVec2& pos,const XVec2& size,	
//						float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,
//						bool withLine = false,bool withRoundCorner = false,bool down = false)	//���ʵ�ľ���
//{
//	drawFillRectExAS(pos,size,r,g,b,a,withLine,withRoundCorner,down);
//}
	inline void drawFillRectExA(const XVec2& pos, const XVec2& size,
		const XFColor& c,
		bool withLine = false, bool withRoundCorner = false, bool down = false)
	{
		drawFillRectExA(pos, size, c.r, c.g, c.b, c.a, withLine, withRoundCorner, down);
	}
	//ָ��ĳһ��ļ�ͷ,angle��λ��
	inline void drawArrow(const XVec2& arm, float angle, float length)
	{
		drawLine(arm, angle, length);	//������
		drawLine(arm, angle + 30, 10);
		drawLine(arm, angle - 30, 10);
	}
	//��ĳһ������ļ�ͷ,angle��λ��
	inline void drawArrowOut(const XVec2& arm, float angle, float length)
	{
		drawArrow(arm + XMath::getRotateRate(angle * DEGREE2RADIAN) * length,
			angle + 180.0f, length);
	}
	//����ͷ,��start��end�ļ�ͷ
	inline void drawArrow(const XVec2& start, const XVec2& end)
	{
		//float len = start.getLength(end);
		//float angle = end.getAngle(start) * RADIAN2DEGREE;
		////���ֲ�
		//drawArrow(end,angle,len);
		drawArrow(end, end.getAngle(start) * RADIAN2DEGREE, start.getLength(end));
	}
	//���Բ��ͷ
	//pos������λ��
	//r�����İ뾶
	//start��������ʼ�Ƕȣ���λ�Ƕ�
	//end��������ֹ�Ƕȣ���λ�Ƕ�
	extern void drawArrow(const XVec2&pos, float r, float start, float end, const XFColor& color);
	//cornerStyle:����Բ�ǵ���ʽÿ2bits����һ���ǣ�����Ϊ���½ǡ����½ǡ����Ͻǡ����Ͻ�
	//0Ϊֱ�ǡ�1Ϊ��Բ�ǣ�2��Ϊ��Բ�ǣ�3��Ϊ��һ���������Բ��
	//lineStyle:�ߵ���ʽ��4��bits�ֱ�����4�����Ƿ���Ҫ��Ҫ���
	extern void drawFillRectExEx(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false);	//���ʵ�ľ���
	extern void drawFillRectExExA(const XVec2& pos, const XVec2& size,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f,
		unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false);	//���ʵ�ľ���
	inline void drawFillRectExExA(const XVec2& pos, const XVec2& size,
		const XFColor& c, unsigned char lineStyle = 0, unsigned char cornerStyle = 0, bool down = false)	//���ʵ�ľ���
	{
		drawFillRectExExA(pos, size, c.r, c.g, c.b, c.a, lineStyle, cornerStyle, down);
	}
	inline void drawFillQuad(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float r = 1.0f, float g = 1.0f, float b = 1.0f)	//���ʵ���ı���
	{
		glColor3f(r, g, b);
		XGL::DisableTexture2D();
		glBegin(GL_QUADS);
		glVertex2fv(p0);
		glVertex2fv(p1);
		glVertex2fv(p2);
		glVertex2fv(p3);
		glEnd();
	}
	extern void drawFillQuadEx(const XVec2& p0, const XVec2& p1, const XVec2& p2, const XVec2& p3,
		float r = 1.0f, float g = 1.0f, float b = 1.0f,
		bool withLine = false);	//���ʵ���ı���
//���ڰ�ɫ�ļ��,anti�Ƿ�ɫ
//size:�ܴ�С
//cSize:���������
	extern void drawFillRectBW(const XVec2& pos, const XVec2& size, const XVec2& cSize, bool anti = false);
	//cSize:��������Ĵ�С
	//sum:���������
	extern void drawFillRectBWEx(const XVec2& pos, const XVec2& cSize, const XVec2& sum, bool anti = false);
	extern void drawFillTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		float r = 1.0f, float g = 1.0f, float b = 1.0f);	//���ʵ��������
	extern void drawFillTriangle(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		const XFColor&c);	//���ʵ��������
	extern void drawFillTriangleEx(const XVec2& p0, const XVec2& p1, const XVec2& p2,
		float r = 1.0f, float g = 1.0f, float b = 1.0f);	//���ʵ��������
//��ʵ�Ķ����(����ĺ���ֻ������͹�����)
	extern void drawFillPolygon(const XVec2 *p, int sum, float r, float g, float b);
	extern void drawFillPolygon(const XVec2 *p, int sum, const XVec2& pos, float angle, float r, float g, float b);
	extern void drawFillPolygonEx(const XVec2 *p, const float *c, int sum, float r, float g, float b, bool withLine = false);	//���ӽ�����ʸ�
	extern void drawFillPolygonExA(const XVec2 *p, const float *c, int sum, float r, float g, float b, float a, bool withLine = false);	//���ӽ�����ʸ�
	inline void drawFillPolygonExA(const XVec2 *p, const float *c, int sum, const XFColor& cl, bool withLine = false)	//���ӽ�����ʸ�
	{
		drawFillPolygonExA(p, c, sum, cl.r, cl.g, cl.b, cl.a, withLine);
	}
	//����ĺ����������һ����ͼ
	extern void drawTexture(XTexture &tex, const XVec2& pos, int cW, int cH,
		const XVec2 *vArray, const XVec2 *uArray, int arrayW, int arrayH);	//ʹ��WITHXSPRITE_EX��ʱ����Ҫ������ͼ��һ��
	extern void drawTexture(unsigned int tex, const XVec2& pos, int texW, int texH, bool blend = false);
	//��Ϊһ�����ͼƬ
	extern void drawTexture(unsigned int tex, const XVec2* v, const XVec2* u, int pSum,
		bool blend = false, const XFColor& c = XFColor::white);
	//��Ϊһ�����ͼƬ�����Խ���ϸ�֣��������Ż�Ϊ���һ�������ͼ��������һ����
	extern void drawTextureEx(unsigned int tex, const XVec2* v, const XVec2* u, int pSum, 
		int subSum = 1, bool blend = false);
	//ͨ��UV�������һ����ͼ���������Ż�����ϸ�֣�
	extern void drawTextureEx(unsigned int tex, const XVec2& pos, const XVec2& size, XVec2 *vArray,
		XVec2 *uArray, int arrayW, int arrayH, bool blend = false);
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
	bool m_isThreadEvn;	//�Ƿ�Ϊ���̵߳ļ��ػ���
public:
	void setIsThreadEvn(bool flag) { m_isThreadEvn = flag; }
	bool getIsThreadEvn() const { return m_isThreadEvn; }
	GLuint m_texture;	//��ͼ�ı��:0��Ч����Լ0��Ч
	int m_w;			//��ͼ�����ؿ��
	int m_h;			//��ͼ�����ظ߶�
	int m_internalFormat;	//�ڲ����ݸ�ʽ
	XBool getIsEnable() { return m_isLoaded; }//��鵱ǰ��ͼ�Ƿ���Ч
	XColorMode m_mode;	//0:RGBA else RGB
	void bind()
	{
		XGL::EnableTexture2D();
		XGL::BindTexture2D(m_texture);
	}
	XBool reset();
	int m_MSSum;
public:
	//resPos��ʾ��Դ��λ�ã�0��ʾ�ⲿ��Դ��1����ʾ�ڲ���Դ��������Դ�ļ����ж�ȡ��Դ	
	XBool load(const char *filename, XResPos resPos = RES_SYS_DEF);//��ָ���ļ�������ͼƬ��Դ
	//���������ӿ���δʵ�֣��������ڲ����ݸ�ʽʱΪĬ�ϸ�ʽ
	XBool createTexture(int w, int h, XColorMode mode, int internalFormat = -1, int filter = GL_LINEAR, int MSSum = 0);			//����һ��ָ����С�Ŀհ���ͼ
	XBool createWithTexture(int w, int h, XColorMode mode, unsigned int tex);	//ֱ��ʹ���Ѿ����ڵ���ͼ
	void updateTexture(const XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTexture(const void *data, int w = 0, int h = 0, int x = 0, int y = 0)		//ʹ��ָ�����ݸ�����ͼ
	{
		if (!m_isLoaded || data == NULL) return;	//PBO��ʱ�����Ϊ NULL
		updateTextureX(data, w, h, x, y);
	}
	void updateTextureR2B(const void *data, int w = 0, int h = 0, int x = 0, int y = 0)	//����������ͬ����ͬ���ں���ɫ��������
	{
		if (!m_isLoaded || data == NULL) return;	//PBO��ʱ�����Ϊ NULL
		updateTextureR2BX(data, w, h, x, y);
	}
	void updateTextureEx(const void *data);		//������ʹ��������������ǳ�������������ܻ����������ܺ�
	void updateTextureR2BEx(const void *data);	//������ʹ��������������ǳ�������������ܻ����������ܺ�
private:
	void updateTextureX(const void *data, int w = 0, int h = 0, int x = 0, int y = 0);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2BX(const void *data, int w = 0, int h = 0, int x = 0, int y = 0);	//����������ͬ����ͬ���ں���ɫ��������
public:
	XTexture()
		:m_texture(0)
		, m_isLoaded(XFalse)
		, m_cp(NULL)
		, m_mode(COLOR_RGBA)
		, m_resInfo(NULL)
		, m_w(0)
		, m_h(0)
		, m_MSSum(0)
		, m_isThreadEvn(false)
	{}

	~XTexture()
	{
		if (m_cp != NULL && --m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			-- CPSum;
			printf("%d-\n", CPSum);
#endif
			XMem::XDELETE(m_cp);
		}
	}
	XTexture(const XTexture& temp);	//�������캯��
	XTexture& operator = (const XTexture &temp);
};
inline int getBytePerPixelByColorMode(XColorMode mode)
{
	switch (mode)
	{
	case COLOR_BGRA:
	case COLOR_RGBA: return 4;
	case COLOR_BGR:
	case COLOR_RGB: return 3;
	case COLOR_RED:
	case COLOR_GRAY: return 1;
	case COLOR_RGBA_F: return 32;
	case COLOR_RGB_F: return 24;
	case COLOR_GRAY_F: return 4;
	case COLOR_DEPTH: return 4;
	default:return 0;
	}
}
inline unsigned int colorMode2GLMode(XColorMode mode)
{
	switch (mode)
	{
	case COLOR_BGRA: return GL_BGRA;
	case COLOR_RGBA: return GL_RGBA;
	case COLOR_BGR: return GL_BGR;
	case COLOR_RGB: return GL_RGB;
	case COLOR_RED: return GL_RED;
	case COLOR_GRAY: return GL_LUMINANCE;
	case COLOR_RGBA_F: return GL_RGBA;
	case COLOR_RGB_F: return GL_RGB;
	case COLOR_GRAY_F: return GL_RED;
	default:return 0;
	}
}
}
#endif
