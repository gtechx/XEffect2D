#ifndef _JIA_XPARAMETERCTRL_
#define _JIA_XPARAMETERCTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.9
//--------------------------------
#include "XControl\XButton.h"
//����һ�����ڷ��̱߳仯�Ĳ������õĿؼ�
//��������û�з�Χ
namespace XE {
enum XParamType
{
	TYPE_LINE,		//�̱߳仯
	TYPE_EXPONENT,	//ָ���仯
};
class XParameterCtrl :public XControlBasic
{
private:
	bool m_isInited;
	XVec2 m_pixelSize;	//�ߴ�

	XFontUnicode m_font;

	XButton m_btn[2];
	XFColor m_textColor;

	XParamType m_paramType;
	float m_increaseRate;	//�Ŵ��ϵ��
	float m_reduceRate;		//��С��ϵ��
	float m_curData;
	static void ctrlProc(void*, int, int);
	void increaseData();
	void reduceData();
public:
	enum XParamCtrlEvent
	{
		PC_VALUE_CHANGE,
	};
	void setCurValue(float value);
	float getCurValue()const { return m_curData; }
	bool initWithoutSkin(float curValue, const XVec2& pixelSize, const XFontUnicode& font);
	bool initWithoutSkin(float curValue, float width)
	{
		return initWithoutSkin(curValue, XVec2(width, MIN_FONT_CTRL_SIZE), getDefaultFont());
	}
	void setParamType(XParamType type, float iRate, float rRate);

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);
	void setTextColor(const XFColor& color) { m_textColor = color; }	//�����������ɫ
	const XFColor& getTextColor() const { return m_textColor; }	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p, XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder, XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return XTrue; }	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	XParameterCtrl()
		:m_isInited(false)
		, m_paramType(TYPE_EXPONENT)
		, m_increaseRate(1.1f)
		, m_reduceRate(0.9f)
		, m_curData(1.0f)
	{
		m_ctrlType = CTRL_OBJ_PARAMCTRL;
	}
	~XParameterCtrl() { release(); }
	void release();
	XBool setACopy(const XParameterCtrl &temp) { return XFalse; }	//��δʵ��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XParameterCtrl(const XParameterCtrl &temp);
	XParameterCtrl& operator = (const XParameterCtrl& temp);
};
#if WITH_INLINE_FILE
#include "XParameterCtrl.inl"
#endif
}
#endif