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

class _XSliderEx:public _XControlBasic,public _XBasicOprate
{
private:
	_XBool m_isInited;
	_XSlider m_mainSld;			//����Ļ�����
	_XSlider m_secondarySld;	//�����Ļ�����
	_XButton m_chooseBtn;		//�л������������İ�ť

	int m_timer;	//��ʱ���������жϸ��ְ���״̬�Ƿ���Ҫ��ʾ

	void (*m_funInit)(void *,int);		//��ʼ��
	void (*m_funRelease)(void *,int);		//��Դ�ͷ�
	void (*m_funMouseOn)(void *,int);		//�������
	void (*m_funMouseDown)(void *,int);	//��갴��
	void (*m_funMouseUp)(void *,int);		//��굯��
	void (*m_funMouseMove)(void *,int);	//����϶�����ʱ����
	void (*m_funValueChange)(void *,int);	//����������ֵ�仯
	void *m_pClass;				//�ص������Ĳ���

	friend void sliderExBtnProc(void *pClass,int id);
	friend void sliderExSldProc(void *pClass,int id);
	friend void sliderExSldOnProc(void *pClass,int id);
	friend void sliderExSldUpProc(void *pClass,int id);
	friend void sliderExSldMoveProc(void *pClass,int id);
	friend void sliderExSldChangeProc(void *pClass,int id);
	friend void stateChangeProc(void *);
protected:
	void update(int stepTime);
	void draw();//��滬����
	void drawUp(){;}						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//����ʧȥ����
public:
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero);
	_XBool initWithoutTex(const _XVector2& pixelSize,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2::zero)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),max,min,type,fontPosition);
	}
	_XSliderEx()
		:m_isInited(XFalse)
		,m_timer(0)
		,m_funInit(NULL)		//��ʼ��
		,m_funRelease(NULL)		//��Դ�ͷ�
		,m_funMouseOn(NULL)		//�������
		,m_funMouseDown(NULL)	//��갴��
		,m_funMouseUp(NULL)		//��굯��
		,m_funMouseMove(NULL)	//����϶�����ʱ����
		,m_funValueChange(NULL)	//����������ֵ�仯
		,m_pClass(NULL)				//�ص������Ĳ���
	{
		m_ctrlType = CTRL_OBJ_SLIDEREX;
	}
	virtual ~_XSliderEx(){release();}

	void release();
	//������һЩ��������
	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color){m_mainSld.setTextColor(color);}	//�����������ɫ
	_XFColor getTextColor() const {return m_mainSld.getTextColor();}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//�����������ɫ
	void setAlpha(float a);

	_XBool setACopy(const _XSliderEx &temp);			//����һ������

	//��������
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setNowValue(float temp);			//���û��鵱ǰ��ֵ
	void setRange(float max,float min);

	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	void setVisible();
	void disVisible();
	float getNowValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const; 
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y); //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XSliderEx(const _XSliderEx &temp);
	_XSliderEx& operator = (const _XSliderEx& temp);
	//�����ǹ��ڹ��������Ľӿ�
public:
	void setConnectVar(float * p) {m_mainSld.setConnectVar(p);}	//��������
	void disConnectVar() {m_mainSld.disConnectVar();}			//ȡ����������
public:
	void setOprateState(void * data);
	void *getOprateState() const;
	virtual bool isSameState(void * data);
};
#include "XSliderEx.inl"

#endif