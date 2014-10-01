#ifndef _JIA_XPROGRESSRING_
#define _JIA_XPROGRESSRING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.18
//--------------------------------
//����һ�����ν�������Ŀǰ��δʵ��
#include "XControlBasic.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

class _XProgressRing:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	float m_r0;	//�⾶
	float m_r1;	//�ھ�
	_XBool m_withBG;
	_XBool m_withText;
	_XFontUnicode m_caption;
	_XFColor m_textColor;		//���ֵ���ɫ

//	_XVector2 m_position;
	float m_nowValue;					//�������ĵ�ǰֵ
public:
	_XBool initWithoutTex(float r0,float r1,const _XFontUnicode &font,bool withBG = false,bool withText = true);
	_XBool initWithoutTex(float r0,float r1,bool withBG = false,bool withText = true)
	{
		return initWithoutTex(r0,r1,XEE::systemFont,withBG,withText);
	}

	_XProgressRing()
		:m_isInited(XFalse)
		,m_nowValue(0.0f)
		,m_withText(XTrue)
		,m_withBG(XTrue)
		,m_textColor(1.0f,1.0f,1.0f,1.0f)
	{
		m_ctrlType = CTRL_OBJ_PROGRESSRING;
	}
	~_XProgressRing(){release();}
	void release();

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void setValue(float temp);
	float getValue() const{return m_nowValue;}
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float,float,_XMouseState){return XTrue;}	//do nothing
	_XBool keyboardProc(int,_XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float,float){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
public:
	_XBool setACopy(const _XProgressRing &temp);		//����һ������(��δ���)
	//��������������
	void enable(){m_isEnable = XTrue;}
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XProgressRing(const _XProgressRing &temp);
	_XProgressRing& operator = (const _XProgressRing& temp);
};
#include "XProgressRing.inl"
#endif