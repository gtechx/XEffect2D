#ifndef _JIA_XTEXT_
#define _JIA_XTEXT_
//��������ֿؼ�����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
#include "../XBasicWindow.h"

class _XText:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XFontUnicode m_font;
public:
	_XBool init(const _XFontUnicode &font);
	_XBool init() {return init(XEE::systemFont);}
	_XBool init(const char * p)
	{
		if(!init(XEE::systemFont)) return XFalse;
		setString(p);
		return XTrue;
	}
	void setString(const char * str){m_font.setString(str);}
	const char *getCaptionString() const {return m_font.getString();}
	_XText()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_XTEXT;
	}
	~_XText(){release();}
	void release();

	//������ͨ�ýӿ�
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);
	void setTextColor(const _XFColor& color){m_font.setColor(color);}	//�����������ɫ
	_XFColor getTextColor() const {return m_font.getColor();}	//��ȡ�ؼ��������ɫ
	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
protected:
	void draw();								//��水ť
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XFalse;}		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y){return XFalse;}				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
	_XBool canBeChoose(){return XFalse;}
public:
	_XBool isInRect(float x,float y){return m_font.isInRect(x,y);}		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order){return m_font.getBox(order);}			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XText(const _XText &temp);
	_XText& operator = (const _XText& temp);
};
#include "XText.inl"
#endif