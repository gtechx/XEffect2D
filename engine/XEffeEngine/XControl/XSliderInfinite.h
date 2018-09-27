#ifndef _JIA_XSLIDERINFINITE_
#define _JIA_XSLIDERINFINITE_
#include "XControl\XSlider.h"

namespace XE
{
class XSliderInfinite :public XControlBasic, public XBasicOprate
{
private:
	bool m_isInited;
	XVec2 m_size;	//���ش�С
	XSlider m_mainSld;	//���Ļ�����
	XButton m_addMinBtn;
	XButton m_desMinBtn;
	XButton m_addMaxBtn;
	XButton m_desMaxBtn;

	float m_timer;

	static void ctrlProc(void*, int, int);
	enum XChangeRangeType
	{
		TYPE_ADD_MIN,
		TYPE_DES_MIN,
		TYPE_ADD_MAX,
		TYPE_DES_MAX,
	};
	void changeRange(XChangeRangeType type);
public:
	enum XSliderInfiniteEvent
	{
		SLDI_MOUSE_ON,
		SLDI_MOUSE_DOWN,
		SLDI_MOUSE_UP,
		SLDI_MOUSE_MOVE,
		SLDI_VALUE_CHANGE,
	};
protected:
	void update(float stepTime);
	void draw();//��滬����
	void drawUp();						//do nothing
	XBool mouseProc(const XVec2& p, XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder, XKeyState keyState);
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return XTrue; }
	void setLostFocus() { m_mainSld.setLostFocus(); }	//����ʧȥ����
public:
	XBool initWithoutSkin(const XVec2& size, float maxValue = 100.0f,
		float minValue = 0.0f, XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XSliderInfinite();
	~XSliderInfinite() { release(); }
	XBool setFont(const char *caption) { return m_mainSld.setFont(caption);}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color) { m_mainSld.setTextColor(color); }	//�����������ɫ
	const XFColor& getTextColor() const { return m_mainSld.getTextColor(); }	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);//�����������ɫ
	void setAlpha(float a);

	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	void setVisible();
	void disVisible();
	float getCurValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const;
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p); //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	void setCurValue(float temp, bool withEvent = false);			//���û��鵱ǰ��ֵ
	void setRange(float max, float min);

	void release();
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XSliderInfinite(const XSliderInfinite &temp);
	XSliderInfinite& operator = (const XSliderInfinite& temp);
public:
	void setConnectVar(float * p) { m_mainSld.setConnectVar(p); }	//��������
	void disConnectVar() { m_mainSld.disConnectVar(); }			//ȡ����������
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual void releaseOprateStateData(void *p);
	virtual void stateChange()
	{
		if (m_withUndoAndRedo) XOpManager.addAOperate(this);	//�����Ҫ��¼��ǰ״̬�򽫵��ö�������������غ���
		if (m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//������صĻص�����
	}
	virtual bool isSameState(void * data);
	void setIsInteger(bool flag) { m_mainSld.setIsInteger(flag); }
	bool getIsInteger()const { return m_mainSld.getIsInteger(); }
};
#if WITH_INLINE_FILE
#include "XSliderInfinite.inl"
#endif
}
#endif