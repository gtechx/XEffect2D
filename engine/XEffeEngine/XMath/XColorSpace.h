#ifndef _JIA_XCOLORSPACE_
#define _JIA_XCOLORSPACE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.15
//--------------------------------
//�����װ�˸�����ɫ�ռ��ת������
namespace XE{
namespace XColorSpace
{//��ɫ�ռ��ת��
	//http://www.easyrgb.com/index.php?X=MATH&H=09#text9
	//X(0 - 95.047)Y(0 - 100.0f)Z(0 - 108.883)
	extern void XYZ2RGB(unsigned char *color, float X, float Y, float Z);
	extern void RGB2XYZ(unsigned char *color, float &X, float &Y, float &Z);
	extern void XYZ2Yxy(float X, float Y, float Z,float &oY,float &ox,float &oy);
	extern void Yxy2XYZ(float &X, float &Y, float &Z, float iY, float ix, float iy);
	extern void XYZ2HLab(float X, float Y, float Z, float &l, float &a, float &b);
	extern void HLab2XYZ(float &X, float &Y, float &Z, float l, float a, float b);
	extern void XYZ2CLab(float X, float Y, float Z, float &l, float &a, float &b);
	extern void CLab2XYZ(float &X, float &Y, float &Z, float l, float a, float b);
	extern void CLCH2CLab(float L, float C, float H, float &l, float &a, float &b);
	extern void CLab2CLCH(float &L, float &C, float &H, float l, float a, float b);
	extern void XYZ2CLuv(float X, float Y, float Z, float &l, float &u, float &v);
	extern void CLuv2XYZ(float &X, float &Y, float &Z, float l, float u, float v);
	//RGB(0 - 255)
	extern void RGB2HSL(float R, float G, float B, float &H, float &S, float &L);
	extern void HSL2RGB(float &R, float &G, float &B, float H, float S, float L);
	extern void RGB2HSV(float R, float G, float B, float &H, float &S, float &V);
	extern void HSV2RGB(float &R, float &G, float &B, float H, float S, float V);
	extern void RGB2CMY(float R, float G, float B, float &C, float &M, float &Y);
	extern void CMY2RGB(float &R, float &G, float &B, float C, float M, float Y);
	extern void CMY2CMYK(float C, float M, float Y, float &c, float &m, float &y,float &k);
	extern void CMYK2CMY(float &C, float &M, float &Y, float c, float m, float y,float k);

	//HSV	H(0 - 6) S(0 - 1) V(0 - 1)
	extern void HSV2RGB(unsigned char *color,float H,float S,float V);
	extern void RGB2HSV(unsigned char *color,float& H,float& S,float&V);
	extern float HueToRGB (float n1,float n2,float hue);
	extern void YUV2RGB(unsigned char *color,unsigned char Y,unsigned char U,unsigned char V);
	extern void YUVToRGB(unsigned char *pYUV, unsigned char *pRGB,int width,int height);
	extern void YUV420P2RGB(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	//��δ���
	extern void RGB2YUV420(unsigned char *Y,unsigned char *U,unsigned char *V, unsigned char *pRGB,int width,int height);	
	//���ݽǶȼ���ɫ���ϵ���ɫ[0-360]
	extern XFColor getWheelColor(float angle);	//�Ƕȵĵ�λΪ�ȣ�ɫ�ֵ�0�ȷ���Ϊ��ɫ����ɫΪ��������˳�� 
	//rate(0 - 1)
	extern XFColor getWheelColor(float angle,float rate);	//�Ƕȵĵ�λΪ�ȣ�ɫ�ֵ�0�ȷ���Ϊ��ɫ����ɫΪ��������˳�� 

	extern double getColorDifferenceLab(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceHSL(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	//HSV = HSB
	extern double getColorDifferenceHSV(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceLuv(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceRGB(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	//extern void rgb2lab(int R, int G, int B, int &oL, int &oA, int &oB);
	extern double getColorDifferenceCiede2000(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceDeltaE1994(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceDeltaCMC(const XCColor &c0, const XCColor &c1);		//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��
	extern double getColorDifferenceDeltaE2000(const XCColor &c0, const XCColor &c1);	//�Ƚ�������ɫ�����𣬷��رȽ�ϵ��

	extern void RGB2Gray(const unsigned char *rgb,char unsigned *gray,int pixelsSum);
	extern float getColorLuminance(const XFColor& color);	//������ɫ������
}
}
#endif