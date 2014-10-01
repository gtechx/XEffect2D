#ifndef _JIA_XFONT_
#define _JIA_XFONT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//�л�˵��������ÿ���ַ������ݷ����ı�ʱ����Ҫ���ַ������ڲ����ݽ��и��£��������������Ա�����һ�ε��ַ������ݣ��Ƚϲ�ͬ�Ĳ��ֲŽ��и��¡�
//Ҳ����ֱ�ӽ��ַ���ͨ��FBO����Ϊһ����ͼ��ֱ����ͨ������һ������ʾ�Ͳ���Ҫÿ�ζ����м��㣬
//���������������⣬1���ڴ����Ľϴ�2����ͼ�ĳߴ���������ֵ����ݶ������仯
#include "XFontBasic.h"

#ifndef TEXT_FONT_MODE
#define TEXT_FONT_MODE (" !\"#$%&\'()*+,-./0123456789:;<=>\?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~")
#endif

class _XFont:public _XFontBasic
{
private:
	void updateData();			//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;			//���ڱ���ÿ���ַ��ļ��к���
	_XVector2 m_layout;		//����ͼƬ�Ĳ���

public:
	_XBool init(const char *filename,	//����ͼƬ���ļ���
		const _XVector2& size,		//��������ش�С
		const _XVector2& layout,		//����ͼƬ�Ĳ���
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);		
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	//����ӿ�,����Դ�ж�ȡ����
	_XBool initFromTTF(const char * filename,int fontSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse);	//��TTF�ļ��г�ʼ��(��δʵ��)

	_XFont& operator = (const _XFont& temp);
	_XBool setACopy(const _XFont &temp);
	_XFont(const _XFont &temp);	//�������캯��

	void draw();
	_XFont();
	_XFont(int maxStrLen);
	~_XFont();
	void setMaxStrLen(int maxStrLen)
	{
		XDELETE_ARRAY(m_string);
		XDELETE_ARRAY(m_textPosition);
		XDELETE_ARRAY(m_textRect);

		if(maxStrLen < 2) m_maxStringLen = 2;
		else m_maxStringLen = maxStrLen;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
		m_textPosition = createArrayMem<_XVector2>(m_maxStringLen);
		m_textRect = createArrayMem<_XRect>(m_maxStringLen);

		m_needUpdateData = XTrue;
	}
};

#include "XFont.inl"

#endif