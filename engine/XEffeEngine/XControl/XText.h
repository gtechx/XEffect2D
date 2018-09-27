#ifndef _JIA_XTEXT_
#define _JIA_XTEXT_
//��������ֿؼ�����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
namespace XE{
class XPropertyBox;
class XText :public XControlBasic
{
	friend XPropertyBox;
private:
	XBool m_isInited;
	XFontUnicode m_font;
public:
	XBool init(const XFontUnicode& font);
	XBool init() { return init(getDefaultFont()); }
	XBool init(const char * p)
	{
		if (!init(getDefaultFont())) return XFalse;
		setString(p);
		return XTrue;
	}
	void setAlignmentMode(XFontAlignmentModeX x, XFontAlignmentModeY y);
	void setString(const char * str) { m_font.setString(str); }
	void setMaxStrLen(int len) { m_font.setMaxStrLen(len); }
	const char *getCaptionString() const { return m_font.getString(); }
	XText()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_TEXT;
	}
	~XText() { release(); }
	void release();

	//������ͨ�ýӿ�
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);
	void setTextColor(const XFColor& color) { m_font.setColor(color); }	//�����������ɫ
	const XFColor& getTextColor() const { return m_font.getColor(); }	//��ȡ�ؼ��������ɫ
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();								//��水ť
	void drawUp() {};							//do nothing
	XBool mouseProc(const XVec2&, XMouseState) { return XFalse; }		//������궯������Ӧ����
	XBool keyboardProc(int, XKeyState) { return XFalse; }			//�����Ƿ񴥷���������
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2&) { return XFalse; }				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return XTrue; }	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
	XBool canBeChoose() { return XFalse; }
public:
	XBool isInRect(const XVec2& p) { return m_font.isInRect(p); }		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order) { return m_font.getBox(order); }			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XText(const XText &temp);
	XText& operator = (const XText& temp);
};
#if WITH_INLINE_FILE
#include "XText.inl"
#endif
}
#endif