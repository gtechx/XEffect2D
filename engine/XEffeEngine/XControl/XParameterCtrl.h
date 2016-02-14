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
namespace XE{
enum XParamType
{
	TYPE_LINE,		//�̱߳仯
	TYPE_EXPONENT,	//ָ���仯
};
class XParameterCtrl:public XControlBasic
{
private:
	bool m_isInited;
	XVector2 m_pixelSize;	//�ߴ�

	XFontUnicode m_font;

	XButton m_btn[2];
	XFColor m_textColor;

	XParamType m_paramType;
	float m_increaseRate;	//�Ŵ��ϵ��
	float m_reduceRate;		//��С��ϵ��
	float m_curData;
	static void ctrlProc(void*,int,int);
	void increaseData();
	void reduceData();
public:
	enum XParamCtrlEvent
	{
		PC_VALUE_CHANGE,
	};
	void setCurValue(float value)
	{
		m_curData = value;
		m_font.setString(XString::toString(m_curData).c_str());
	}
	float getCurValue()const{return m_curData;}
	bool initWithoutSkin(float curValue,const XVector2 &pixelSize,const XFontUnicode &font);
	bool initWithoutSkin(float curValue,float width)
	{
		return initWithoutSkin(curValue,XVector2(width,MIN_FONT_CTRL_SIZE),getDefaultFont());
	}
	void setParamType(XParamType type,float iRate,float rRate)
	{
		m_paramType = type;
		m_reduceRate = rRate;
		m_increaseRate = iRate;
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_btn[0].setPosition(m_position);
		m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y,m_position.y);
		m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//�ַ������м�
		m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_pixelSize.x * m_scale.x,m_position.y + m_pixelSize.y * m_scale.y);
	}

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y)
	{
		m_scale.set(x,y);
		m_btn[0].setScale(m_scale);
		m_btn[0].setPosition(m_position);
		m_btn[1].setScale(m_scale);
		m_btn[1].setPosition(m_position.x + m_pixelSize.x * m_scale.x - m_pixelSize.y * m_scale.y,m_position.y);
		m_font.setScale(m_scale);
		m_font.setPosition(m_position + m_pixelSize * m_scale * 0.5f);	//�ַ������м�
		m_curMouseRect.set(m_position.x,m_position.y,m_position.x + m_pixelSize.x * m_scale.x,m_position.y + m_pixelSize.y * m_scale.y);
	}
	void setTextColor(const XFColor& color){m_textColor = color;}	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
		m_font.setColor(m_textColor * m_color);
		m_btn[0].setColor(r,g,b,a);
		m_btn[1].setColor(r,g,b,a);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		m_font.setColor(m_textColor * m_color);
		m_btn[0].setColor(m_color);
		m_btn[1].setColor(m_color);
		updateChildColor();
	}

protected:
	void draw()
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isVisible) return;	//������ɼ�ֱ���˳�
		//����м�ı���
		XRender::drawFillBoxA(m_position,m_pixelSize * m_scale,XFColor::gray,true);
		m_font.draw();
		m_btn[0].draw();
		m_btn[1].draw();
	}
	void drawUp()
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isVisible) return;	//������ɼ�ֱ���˳�
		m_btn[0].drawUp();
		m_btn[1].drawUp();
	}
	void update(float stepTime)
	{
		if(!m_isInited) return;
		m_btn[0].update(stepTime);
		m_btn[1].update(stepTime);
	}
	XBool mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
		if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
		XBool ret = m_btn[0].mouseProc(x,y,mouseState);
		XBool ret1 = m_btn[1].mouseProc(x,y,mouseState);
		return (ret || ret1);
	}
	XBool keyboardProc(int keyOrder,XKeyState keyState)			//�����Ƿ񴥷���������
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||	//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
		XBool ret = m_btn[0].keyboardProc(keyOrder,keyState);
		XBool ret1 = m_btn[1].keyboardProc(keyOrder,keyState);
		return (ret || ret1);
	}
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return m_curMouseRect.isInRect(x,y);
	}
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus()	//����ʧȥ����
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return;		//�����Ч��ֱ���˳�

		m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
	}
public:
	XParameterCtrl()
		:m_isInited(false)
		,m_paramType(TYPE_EXPONENT)
		,m_increaseRate(1.1f)
		,m_reduceRate(0.9f)
		,m_curData(1.0f)
	{
		m_ctrlType = CTRL_OBJ_PARAMCTRL;
	}
	~XParameterCtrl(){release();}
	void release();
	XBool setACopy(const XParameterCtrl &temp){return XFalse;}	//��δʵ��
	XBool isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	{
		if(!m_isInited) return XFalse;
		return m_curMouseRect.isInRect(x,y);
	}
	XVector2 getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	{
		if(!m_isInited) return XVector2::zero;
		switch(order)
		{
		case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
		case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
		case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
		case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
		}
		return XVector2::zero;
	}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XParameterCtrl(const XParameterCtrl &temp);
	XParameterCtrl& operator = (const XParameterCtrl& temp);
};
}
#endif