#ifndef _JIA_XBMP_
#define _JIA_XBMP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XCommonDefine.h"
namespace XE{
#pragma pack(push)
#pragma pack(1)
struct XBMPFileHeader
{
    unsigned short bfType;			//λͼ�ļ������ͣ�����ΪBM
    int bfSize;						//λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
    unsigned short bfReserved1;		//λͼ�ļ������֣�����Ϊ0
    unsigned short bfReserved2;		//λͼ�ļ������֣�����Ϊ0
    int bfOffBits;					//λͼ���ݵ���ʼλ�ã��������λͼ
    // �ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
};
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
struct XBMPInfoHeader
{
    int biSize;                    // ���ṹ��ռ���ֽ���
    int biWidth;                // λͼ�Ŀ�ȣ�������Ϊ��λ
    int biHeight;                // λͼ�ĸ߶ȣ�������Ϊ��λ
    unsigned short biPlanes;    // Ŀ���豸�ļ��𣬱���Ϊ1
    unsigned short biBitCount;    // ÿ�����������λ����������1(˫ɫ),4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
    int biCompression;            // λͼѹ�����ͣ������� 0(��ѹ��),
    // 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
    int biSizeImage;            // λͼ�Ĵ�С�����ֽ�Ϊ��λ
    int biXPelsPerMeter;        // λͼˮƽ�ֱ��ʣ�ÿ��������
    int biYPelsPerMeter;        // λͼ��ֱ�ֱ��ʣ�ÿ��������
    int biClrUsed;                // λͼʵ��ʹ�õ���ɫ���е���ɫ��
    int biClrImportant;            // λͼ��ʾ��������Ҫ����ɫ��
};
#pragma pack(pop)

//���ﱣ���BMPͼ��ʹ��24bits RGB_888�ĸ�ʽ����ʱ����������չ��
class XBMP
{
private:
	static const int m_bmpColorBytes = 3;
	XBool m_isInited;
public:
	int m_width;
	int m_height;
	unsigned char *m_data;
public:
	XBool init(const char *filename, int needAlpha = 0);    //��һ���ļ��ж�ȡͼƬ����
	XBool init(int width, int height);                        //����һ���յ�ͼƬ
	XBool writeToFile(const char *filename);                //��ͼƬ���浽BMP�ļ���
	XBool release();                                        //�ͷ����е���Դ
	XBMP()
		:m_isInited(XFalse)
		, m_width(0)
		, m_height(0)
		, m_data(NULL)
	{}
};
}
#endif