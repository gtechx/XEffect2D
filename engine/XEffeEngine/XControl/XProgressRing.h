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
namespace XE {
class XProgressRing :public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	float m_r0;	//�⾶
	float m_r1;	//�ھ�
	XBool m_withBG;
	XBool m_withText;
	XFontUnicode m_caption;
	XFColor m_textColor;		//���ֵ���ɫ

//	XVec2 m_position;
	float m_curValue;					//�������ĵ�ǰֵ
	bool m_withVirtual;			//�Ƿ�ʹ����������ݽ����м����
	XMoveData m_valueMD;
public:
	//r0:�⾶
	//r1:�ھ�
	XBool initWithoutSkin(float r0, float r1, const XFontUnicode& font, 
		bool withBG = false, bool withText = true);
	XBool initWithoutSkin(float r0, float r1, bool withBG = false, bool withText = true)
	{
		return initWithoutSkin(r0, r1, getDefaultFont(), withBG, withText);
	}

	XProgressRing()
		:m_isInited(XFalse)
		, m_curValue(0.0f)
		, m_withText(XTrue)
		, m_withBG(XTrue)
		, m_textColor(1.0f, 1.0f)
		, m_withVirtual(false)
	{
		m_ctrlType = CTRL_OBJ_PROGRESSRING;
	}
	~XProgressRing() { release(); }
	void release();

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const { return m_textColor; }	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void setValue(float temp);	//0 - 100֮��
	float getValue() const { return m_curValue; }

	//�Ƿ���ʾ�м����
	bool getWithVirtual()const { return m_withVirtual; }
	void setWithVirtual(bool flag) { m_withVirtual = flag; }
protected:
	void update(float stepTime) { m_valueMD.move(stepTime); }
	void draw();
	void drawUp() {};						//do nothing
	XBool mouseProc(const XVec2&, XMouseState) { return XTrue; }	//do nothing
	XBool keyboardProc(int, XKeyState) { return XTrue; }	//do nothing
	void insertChar(const char *, int) { ; }
	XBool canGetFocus(const XVec2&) { return XFalse; }	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return XTrue; }
public:
	XBool setACopy(const XProgressRing &temp);		//����һ������(��δ���)
	//��������������
	void enable() { m_isEnable = XTrue; }
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XProgressRing(const XProgressRing &temp);
	XProgressRing& operator = (const XProgressRing& temp);
};
#if WITH_INLINE_FILE
#include "XProgressRing.inl"
#endif
}
#endif