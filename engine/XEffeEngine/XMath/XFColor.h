#ifndef _JIA_XFCOLOR_
#define _JIA_XFCOLOR_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//#include "XBasicFun.h"
#include "XRandomFun.h"
namespace XE{
class XCColor;
//����������ɫ 0.0 - 1.0
class XFColor
{
//private:
public:
    float r;    //0 - 1
    float g;
    float b;
    float a;
	float w;	//����
public:
	XFColor(const XCColor &temp);
	XFColor& operator = (const XCColor& temp);

    XFColor()
        :r(0.0f),g(0.0f),b(0.0f),a(0.0f)
    {}
	void calculateW(){w = (std::max)(r,(std::max)(g,b));}	//�������ȣ��ڱ�Ҫ��ʱ���������
	//XFColor(float r,float g,float b,float a)
	//{
	//	if(r < 0.0f) r = 0.0f;
	//	if(r > 1.0f) r = 1.0f;
	//	if(g < 0.0f) g = 0.0f;
	//	if(g > 1.0f) g = 1.0f;
	//	if(b < 0.0f) b = 0.0f;
	//	if(b > 1.0f) b = 1.0f;
	//	if(a < 0.0f) a = 0.0f;
	//	if(a > 1.0f) a = 1.0f;
	//	fR = r;fG = g;fB = b;fA = a;
	//}
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
	XFColor(float _r,float _g,float _b,float _a = 1.0f)
		:r(_r),g(_g),b(_b),a(_a)
	{}
	XFColor(const XFColor& c,float _a)
		:r(c.r),g(c.g),b(c.b),a(_a)
	{}
	XFColor(float _l,float _a)
		:r(_l),g(_l),b(_l),a(_a)
	{}
	XFColor(unsigned int color)
	{
		r = (color >> 24) / 255.0f;
		g = ((color >> 16) % 256) / 255.0f;
		b = ((color >> 8) % 256) / 255.0f;
		a = (color % 256) / 255.0f;
	}
	XFColor(double rate)	//rate [0 - 1]
	{
		int color = 0xffffff * rate;
		r = (color >> 16) / 255.0f;
		g = ((color >> 8) % 256) / 255.0f;
		b = (color % 256) / 255.0f;
		a = 1.0f;
	}
    void set(float _r,float _g,float _b,float _a);
    void set(float _l,float _a = 1.0f);
	XFColor operator * (const XFColor& temp) const;
	XFColor operator * (const float& temp) const;
	void setR(float _r);
	void setG(float _g);
	void setB(float _b);
	void setA(float _a);
	float getR() const;
	float getG() const;
	float getB() const;
	float getA() const;
	void randColor();
	float limit() const {return 1.0f;}
	XFColor anti(){return XFColor(1.0f - r,1.0f - g,1.0f - b,a);}
	static const XFColor zero,white, gray, black, red, green, blue, cyan, magenta,
        yellow,aliceBlue,antiqueWhite,aqua,aquamarine,azure,beige,bisque,blanchedAlmond,
        blueViolet,brown,burlyWood,cadetBlue,chartreuse,chocolate,coral,cornflowerBlue,cornsilk,
        crimson,darkBlue,darkCyan,darkGoldenRod,darkGray,darkGrey,darkGreen,darkKhaki,
        darkMagenta,darkOliveGreen,darkorange,darkOrchid,darkRed,darkSalmon,darkSeaGreen,
        darkSlateBlue,darkSlateGray,darkSlateGrey,darkTurquoise,darkViolet,deepPink,
        deepSkyBlue,dimGray,dimGrey,dodgerBlue,fireBrick,floralWhite,forestGreen,fuchsia,
        gainsboro,ghostWhite,gold,goldenRod,grey,greenYellow,honeyDew,hotPink,indianRed,indigo,
        ivory,khaki,lavender,lavenderBlush,lawnGreen,lemonChiffon,lightBlue,lightCoral,
        lightCyan,lightGoldenRodYellow,lightGray,lightGrey,lightGreen,lightPink,lightSalmon,
        lightSeaGreen,lightSkyBlue,lightSlateGray,lightSlateGrey,lightSteelBlue,lightYellow,
        lime,limeGreen,linen,maroon,mediumAquaMarine,mediumBlue,mediumOrchid,mediumPurple,
        mediumSeaGreen,mediumSlateBlue,mediumSpringGreen,mediumTurquoise,mediumVioletRed,
        midnightBlue,mintCream,mistyRose,moccasin,navajoWhite,navy,oldLace,olive,oliveDrab,
        orange,orangeRed,orchid,paleGoldenRod,paleGreen,paleTurquoise,paleVioletRed,papayaWhip,
        peachPuff,peru,pink,plum,powderBlue,purple,rosyBrown,royalBlue,saddleBrown,salmon,
        sandyBrown,seaGreen,seaShell,sienna,silver,skyBlue,slateBlue,slateGray,slateGrey,snow,
        springGreen,steelBlue,tan,teal,thistle,tomato,turquoise,violet,wheat,whiteSmoke,
        yellowGreen;
};
#if WITH_INLINE_FILE
#include "XFColor.inl"
#endif
namespace XColor{
//������ɫ֮����в�ֵ��rateΪ��ֵ��λ��(0.0 - 1.0)֮��
inline XFColor getInterpolationColor(const XFColor& color1,const XFColor& color2,float rate)	//rateΪ0.0f - 1.0f
{
	return XFColor(color1.r + (color2.r - color1.r) * rate,
		color1.g + (color2.g - color1.g) * rate,
		color1.b + (color2.b - color1.b) * rate,
		color1.a + (color2.a - color1.a) * rate);
}
//������ɫ��������ϵļ��㹫ʽ��rate1 + rate = 1
extern XFColor colorMix(const XFColor& color1,const XFColor& color2,float rate1,float rate2);
//������ɫ��������ϵļ��㹫ʽ
extern XFColor colorMix(const XFColor& color1,const XFColor& color2,const XFColor& color3,const XFColor& color4,
						 float rate1,float rate2,float rate3,float rate4);
//������ɫ��ϣ���������������
extern XFColor colorMixEx(const XFColor& color1,const XFColor& color2,const XFColor& color3,const XFColor& color4,
						 float rate1,float rate2,float rate3,float rate4);
//������ɫ��ϣ�Ŀǰ��δʵ��
extern XFColor colorMix(const XFColor *color,const float *rate,int mixSum);	//��Ҫע�������ƥ������
extern XFColor colorMixEx(const XFColor *color,const float *rate,int mixSum);	//��Ҫע�������ƥ������
//ʹ��ɫ�ֶԽǵķ�ʽ���л��
extern XFColor colorMixEx(const XFColor& color1,const XFColor& color2);
}
}
#endif