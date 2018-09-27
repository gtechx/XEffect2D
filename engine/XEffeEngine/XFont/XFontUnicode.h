#ifndef _JIA_XFONTUNICODE_
#define _JIA_XFONTUNICODE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XFontBasic.h"
//ע�⺺����Ҫ���򣬷��򽫲�����ȷ��ʾ��
namespace XE{
//���齫һЩ�������Ե����ݷ��������ı��ļ�����
//ע��ҳ��ֵ�ڴ����б��޶���಻�ܳ���100ҳ
//����ַ�����ʹ��һ����������ͼ
#define MAX_FONT_UNICODE_FILE_NAME_LENGTH (256)	//�ַ���ͼƬ���ļ�������󳤶�
#define UNICODE_FONT_PAGE_SUM (256)	//���ַ�����ҳ������(10)
#if WITH_FULL_ALL_CHINESE == 0
#define MAX_UNICODE_TEXT_SUM (8000)	//�����������Ԫ�ص���������(16384)
#endif
#if WITH_FULL_ALL_CHINESE == 1
#define MAX_UNICODE_TEXT_SUM (45000)	//�����������Ԫ�ص���������(16384)
#endif
#if WITH_FULL_ALL_CHINESE == 2
#define MAX_UNICODE_TEXT_SUM (256)	//�����������Ԫ�ص���������(16384)
#endif
#define UNICODE_BYTES_WIDTH (2)		//���ֽڵ��ֿ��Ϊ2

//���������Ӳü�����
class XFontUnicode:public XFontBasic
{
private:
	XSCounter *m_cp;	//���ü�����

	int m_pageW;			//����ҳ�����سߴ�
	int m_pageH;
	char *m_fontPageText;	//�����ҳ����������Ų��ı�
	int m_libFontSum;		//�������ʵ�ʵ�Ԫ������

	void updateData();		//������Ҫ�����ڲ�����
	std::vector<XVec2> m_textPosition;	//���ڱ���ÿ���ַ���λ��
	std::vector<XRect> m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	std::vector<int> m_textPageOrder;	//��ʾ���ַ�ͼƬ�����б��

	XTextureData *m_texture;		//�������ͼ
	int m_pageSum;
	XVec2 m_layout;		//����ͼƬ�Ĳ���
public:
	XBool init(const char *filenameIn,	//����ͼƬ���ļ���
		const XVec2& size,			//��������ش�С
		const XVec2& layout,		//����ͼƬ�Ĳ���
		int pageSum,			//������ҳ������
		XResPos resPos = RES_SYS_DEF,XBool withFBO = XFalse);	
	XBool initEx(const char *filenameIn,XResPos resPos = RES_SYS_DEF,XBool withFBO = XFalse);	//�򻯰汾������Դ�ж�ȡ�������
	XBool initFromTTF(const char *filenameIn,int fontSize,
		XResPos resPos = RES_SYS_DEF,
		bool withOutLine = false,XBool withFBO = XFalse,bool withOpr = true);	//��TTF�г�ʼ������δʵ�֣�

	int getStringLengthPix();	//��ȡ�ַ��������ؿ��(Ϊ�����¼��ݣ���Ȼ��������ӿڣ�ʵ�����Ѿ����µĽӿ������)
private:
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
	bool loadInfoFromFolder(const char *filename,char *target);	//���ļ�����������Դ
	bool loadInfoFromPacker(const char *filename,char *target);	//��ѹ������������Դ
	bool loadInfoFromWeb(const char *filename,char *target);		//����ҳ�ж�ȡ��Դ
	//char m_isAcopy;				//�Ƿ�Ϊһ������
	int getTextIndex(const char p[UNICODE_BYTES_WIDTH])const;	//�ҵ�ָ�����ַ����ڵ�λ��
public:
	bool getIsSupport(const std::string& str)const
	{
		char tmp[UNICODE_BYTES_WIDTH] = {' ',' '};
		int size = str.size() < UNICODE_BYTES_WIDTH ? str.size() : UNICODE_BYTES_WIDTH;
		memcpy(tmp, str.c_str(), size);
		return getTextIndex(tmp) >= 0;
	}
	//����temp��һ������������û����Դ�����õ�����Դ������temp��temp����Դ�ͷ�֮�󣬾ͻ����
	XFontUnicode& operator = (const XFontUnicode& temp);
	XBool setACopy(const XFontUnicode& temp);
	XFontUnicode(const XFontUnicode& temp);	//�������캯��

	void release(){}	//�������¼��ݣ���ʵʲôҲ����
	void setRotateBasePoint(const XVec2& r);
	using XFontBasic::draw;		//���⸲�ǵ�����
	void draw();
	XFontUnicode();
	XFontUnicode(int maxStrLen);
	virtual ~XFontUnicode();
	//�ı��ַ�������󳤶�
	void setMaxStrLen(int maxStrLen);
};
#if WITH_INLINE_FILE
#include "XFontUnicode.inl"
#endif
}
#endif