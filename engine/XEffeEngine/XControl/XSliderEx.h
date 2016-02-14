#ifndef _JIA_XSLIDEREX_
#define _JIA_XSLIDEREX_
//������ɲ��ֲ���
//����һ����΢�����ص�slider���࣬����һ�������ࡣ
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.6
//--------------------------------
#include "XSlider.h"
namespace XE{
class XSliderEx:public XControlBasic,public XBasicOprate
{
private:
	XBool m_isInited;
	XSlider m_mainSld;			//����Ļ�����
	XSlider m_secondarySld;	//�����Ļ�����
	XButton m_chooseBtn;		//�л������������İ�ť

	int m_timer;	//��ʱ���������жϸ��ְ���״̬�Ƿ���Ҫ��ʾ
public:
	enum XSliderExEvent
	{
		SLDEX_INIT,
		SLDEX_RELEASE,
		SLDEX_MOUSE_ON,
		SLDEX_MOUSE_DOWN,
		SLDEX_MOUSE_UP,
		SLDEX_MOUSE_MOVE,
		SLDEX_VALUE_CHANGE,
	};
private:
	static void stateChangeProc(void *);
	static void ctrlProc(void*,int,int);
protected:
	void update(float stepTime);
	void draw();//��滬����
	void drawUp();						//do nothing
	XBool mouseProc(float x,float y,XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
	void setLostFocus(){m_mainSld.setLostFocus();}	//����ʧȥ����
public:
	XBool initWithoutSkin(const XRect& area,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVector2 &fontPosition = XVector2(0.0f,16.0f));
	XBool initWithoutSkin(const XVector2& pixelSize,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVector2 &fontPosition = XVector2(0.0f,16.0f))
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),max,min,type,fontPosition);
	}
	XSliderEx()
		:m_isInited(XFalse)
		,m_timer(0)
	{
		m_ctrlType = CTRL_OBJ_SLIDEREX;
	}
	virtual ~XSliderEx(){release();}

	void release();
	//������һЩ��������
	XBool setFont(const char *caption,const XFontUnicode &font,float captionSize = 1.0f,const XVector2 &fontPosition = XVector2::zero);
	XBool setFont(const char *caption,const XVector2 &fontPosition)
	{
		return setFont(caption,getDefaultFont(),1.0f,fontPosition);
	}
	XBool setFont(const char *caption)
	{
		return m_mainSld.setFont(caption);
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor& color){m_mainSld.setTextColor(color);}	//�����������ɫ
	XFColor getTextColor() const {return m_mainSld.getTextColor();}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//�����������ɫ
	void setAlpha(float a);

	XBool setACopy(const XSliderEx &temp);			//����һ������

	void setCurValue(float temp);			//���û��鵱ǰ��ֵ
	void setRange(float max,float min);

	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	void setVisible();
	void disVisible();
	float getCurValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const; 
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y); //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XSliderEx(const XSliderEx &temp);
	XSliderEx& operator = (const XSliderEx& temp);
	//�����ǹ��ڹ��������Ľӿ�
public:
	void setConnectVar(float * p) {m_mainSld.setConnectVar(p);}	//��������
	void disConnectVar() {m_mainSld.disConnectVar();}			//ȡ����������
public:
	void setOprateState(void * data);
	void *getOprateState() const;
	void releaseOprateStateData(void *p);
	virtual bool isSameState(void * data);
};
#if WITH_INLINE_FILE
#include "XSliderEx.inl"
#endif
}
#endif