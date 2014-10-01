#ifndef _JIA_XFONTUNICODE_
#define _JIA_XFONTUNICODE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XFontBasic.h"
//ע�⺺����Ҫ���򣬷��򽫲�����ȷ��ʾ��

//���齫һЩ�������Ե����ݷ��������ı��ļ�����
//ע��ҳ��ֵ�ڴ����б��޶���಻�ܳ���100ҳ
//����ַ�����ʹ��һ����������ͼ
#define MAX_FONT_UNICODE_FILE_NAME_LENGTH (256)	//�ַ���ͼƬ���ļ�������󳤶�
#define UNICODE_FONT_PAGE_SUM (30)	//���ַ�����ҳ������(10)
#if !WITH_FULL_ALL_CHINESE
#define MAX_UNICODE_TEXT_SUM (8000)	//�����������Ԫ�ص���������(16384)
#else
#define MAX_UNICODE_TEXT_SUM (50000)	//�����������Ԫ�ص���������(16384)
#endif
#define UNICODE_BYTES_WIDTH (2)		//���ֽڵ��ֿ��Ϊ2

//���������Ӳü�����
class _XFontUnicode:public _XFontBasic
{
private:
	_XSCounter *m_cp;	//���ü�����

	int m_pageW;			//����ҳ�����سߴ�
	int m_pageH;
	char *m_fontPageText;	//�����ҳ����������Ų��ı�
	int m_libFontSum;		//�������ʵ�ʵ�Ԫ������

	void updateData();		//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	int *m_textPageOrder;	//��ʾ���ַ�ͼƬ�����б��

	_XTextureData *m_texture;		//�������ͼ
	int m_pageSum;
	_XVector2 m_layout;		//����ͼƬ�Ĳ���

public:
	_XBool init(const char *filenameIn,	//����ͼƬ���ļ���
		const _XVector2 &size,			//��������ش�С
		const _XVector2 &layout,		//����ͼƬ�Ĳ���
		int pageSum,			//������ҳ������
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	
	_XBool initEx(const char *filenameIn,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	//�򻯰汾������Դ�ж�ȡ�������
	_XBool initFromTTF(const char *filenameIn,int fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,bool withOutLine = false,_XBool withFBO = XFalse);	//��TTF�г�ʼ������δʵ�֣�

	int getStringLengthPix();	//��ȡ�ַ��������ؿ��(Ϊ�����¼��ݣ���Ȼ��������ӿڣ�ʵ�����Ѿ����µĽӿ������)
private:
	//char m_isAcopy;				//�Ƿ�Ϊһ������
	int getTextIndex(const char p[UNICODE_BYTES_WIDTH]);	//�ҵ�ָ�����ַ����ڵ�λ��
public:
	//����temp��һ������������û����Դ�����õ�����Դ������temp��temp����Դ�ͷ�֮�󣬾ͻ����
	_XFontUnicode& operator = (const _XFontUnicode& temp);
	_XBool setACopy(const _XFontUnicode & temp);
	_XFontUnicode(const _XFontUnicode & temp);	//�������캯��

	void release(){}	//�������¼��ݣ���ʵʲôҲ����
	void setRotateBasePoint(float x,float y);
	void draw();
	_XFontUnicode();
	_XFontUnicode(int maxStrLen);
	virtual ~_XFontUnicode();
	//�ı��ַ�������󳤶�
	void setMaxStrLen(int maxStrLen);
};

#include "XFontUnicode.inl"

#endif