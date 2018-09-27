#ifndef _JIA_XSLIDERRING_
#define _JIA_XSLIDERRING_
#include "XSlider.h"
namespace XE {
enum XSliderRingMode
{
	SLDR_MODE_VERTICAL,		//��ֱ��
	SLDR_MODE_HORIZONTAL,	//ˮƽ��
	SLDR_MODE_RING,			//���ε�
};
enum XSliderRingState
{
	SLDR_STATE_NORMAL,		//��ͨ״̬
	SLDR_STATE_DOWN,			//��갴��
	SLDR_STATE_ON,			//�������
	SLDR_STATE_DISABLE		//��Ч״̬
};
class XSliderRing :public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_size;
	float m_curValue;
	float m_maxValue;
	float m_minValue;
	XSliderRingMode m_mode;
	int m_isClockwise;	//�Ƿ�Ϊ˳ʱ�뷽��-1:δ֪��0:��ʱ�뷽��1:˳ʱ�뷽��
	//�Ƿ��Ǿ�̬ģʽ����̬ģʽʱֵ�Ǿ��Եģ���̬ģʽ��ֵ��ÿ���ƶ�����
	//bool m_isStaticMode;	//(��δʵ��)
	XVec2 m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ

	XVec2 m_oldMousePos;
	float m_upValue;			//����������һ��ֵ(�������������Ŀ����������϶����鳬����Χ֮����Իָ����϶�֮ǰ��֪������Ŀǰ���ڼ򵥵Ŀ��ǣ������������)
	bool m_dataChanged;
	float m_keyOneValue;		//����һ�ΰ����Ļ�����(ע�����ֵ����Ҫ>=0,���򽫻���ɴ���)

	XFColor m_textColor;	//���ֵ���ɫ
	bool m_withFont;		//�Ƿ�������
	std::string m_fontString;	//��������
	XFontUnicode m_caption;	//��ʾ������
	void updateFontStr();	//���ݵ�ǰֵ�����ַ���������

	XSliderRingState m_curSliderState;
	void updateCurValue(const XVec2& pos);
	float m_trimValue;	//΢������
public:
	enum XSliderRingEvent
	{
		SLDR_MOUSE_DOWN,
		SLDR_MOUSE_UP,
		SLDR_MOUSE_MOVE,
		SLDR_VALUE_TRIM,		//΢���¼�����������ʱ�ᴥ��΢���¼������ڿ���΢С���ݵı仯
		SLDR_VALUE_CHANGE,
	};
public:
	bool initWithoutSkin(const XVec2& size,float maxValue = 100.0f,float minValue = 0.0f,
		XSliderRingMode mode = SLDR_MODE_HORIZONTAL);
	XBool setFont(const char* caption, const XFontUnicode& font);
	XBool setFont(const char* caption)
	{
		return setFont(caption, getDefaultFont());
	}

	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);
	const XFColor& getTextColor() const { return m_textColor; }

	using XObjectBasic::setColor;
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();
	void drawUp() {}
	XBool mouseProc(const XVec2& p, XMouseState mouseState);
	XBool keyboardProc(int keyOrder, XKeyState keyState);
	void insertChar(const char*, int) { ; }
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return  !(m_curSliderState == SLDR_STATE_DOWN);}
	void setLostFocus();	//����ʧȥ����
public:
	XSliderRing();
	~XSliderRing() { release(); }
	void release();
	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	float getCurValue()const { return m_curValue; }
	//�������α仯�Ƕ�֮��Ĳ�ֵ
	float getDistanceValue()const;
	float getTrimValue()const { return m_trimValue; }

	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XSliderRing(const XSliderRing &temp);
	XSliderRing& operator = (const XSliderRing& temp);
protected:
	void update(float stepTime) {}
};
#if WITH_INLINE_FILE
#include "XSliderRing.inl"
#endif
}
#endif