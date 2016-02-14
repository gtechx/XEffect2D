#ifndef _JIA_XPROGRESSRING_
#define _JIA_XPROGRESSRING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.18
//--------------------------------
//����һ�����ν�������Ŀǰ��δʵ��
#include "XControlBasic.h"
#include "XResourceManager.h"
namespace XE{
class XProgressRing:public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	float m_r0;	//�⾶
	float m_r1;	//�ھ�
	XBool m_withBG;
	XBool m_withText;
	XFontUnicode m_caption;
	XFColor m_textColor;		//���ֵ���ɫ

//	XVector2 m_position;
	float m_curValue;					//�������ĵ�ǰֵ
public:
	XBool initWithoutSkin(float r0,float r1,const XFontUnicode &font,bool withBG = false,bool withText = true);
	XBool initWithoutSkin(float r0,float r1,bool withBG = false,bool withText = true)
	{
		return initWithoutSkin(r0,r1,getDefaultFont(),withBG,withText);
	}

	XProgressRing()
		:m_isInited(XFalse)
		,m_curValue(0.0f)
		,m_withText(XTrue)
		,m_withBG(XTrue)
		,m_textColor(1.0f,1.0f,1.0f,1.0f)
	{
		m_ctrlType = CTRL_OBJ_PROGRESSRING;
	}
	~XProgressRing(){release();}
	void release();

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void setValue(float temp);
	float getValue() const{return m_curValue;}
protected:
	void draw();
	void drawUp(){};						//do nothing
	XBool mouseProc(float,float,XMouseState){return XTrue;}	//do nothing
	XBool keyboardProc(int,XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	XBool canGetFocus(float,float){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
public:
	XBool setACopy(const XProgressRing &temp);		//����һ������(��δ���)
	//��������������
	void enable(){m_isEnable = XTrue;}
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XProgressRing(const XProgressRing &temp);
	XProgressRing& operator = (const XProgressRing& temp);
};
#if WITH_INLINE_FILE
#include "XProgressRing.inl"
#endif
}
#endif