#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XXml.h"
#include "XMath\XPressDataMode.h"
#define XPCTRL_BY_KEYMOVE	//�Ƿ����ͨ�����������ƶ�
namespace XE{
enum XPointCtrlStrMode
{
	PCTRL_MODE_RB,	//���������½�
	PCTRL_MODE_RT,	//���������Ͻ�
	PCTRL_MODE_LB,	//���������½�
	PCTRL_MODE_LT,	//���������Ͻ�
};
class XPointCtrl:public XControlBasic
{
private:
	static const int m_pointCtrlLineSum = 10;//����һ������ԭ�ؼ��㣬������Ӧ����λ�ò���
//	XVec2 m_position;	//�ؼ���λ��
//	XVec2 m_scale;		//��С
	XVec2 m_pixelSize;	//���ش�С
	XVec2 m_truePixelSize;	//��ʵ�����سߴ�
	XFontUnicode m_font;
	char m_textStr[64];		//��ʾ���ַ���

	XBool m_isInited;		//�Ƿ��ʼ��
	bool m_withFont;
	bool m_isDown;		//�Ƿ����ʰȡ
	bool m_withRange;	//��Χ�����Ƿ���Ч
	bool m_withMap;		//�Ƿ����ӳ��
	bool m_autoString;	//�Զ�������ʾ������
	XRect m_range;		//�����ƶ��ķ�Χ
	XRect m_mapRange;	//ӳ��ķ�Χ
	XVec2 m_mapValue;
#ifdef XPCTRL_BY_KEYMOVE
	XKeyPressModel m_moveKeyBoard[4];	//ͨ����갴�����ƶ�
#endif
	void updateData();
public:
	enum XPointCtrlEvent
	{
		PITCTRL_DATA_CHANGE,
	};
private:
	XCtrlMode m_ctrlMode;		//�ؼ���ģʽ
	XPointCtrlStrMode m_PCtrlMode;
	bool m_withFitRect;
	XRect m_fitRect;
public:
	//����������rectָ���Ŀ���
	void setFitRect(const XRect& r)
	{
		m_withFitRect = true;
		m_fitRect = r;
		fitMode(m_fitRect);
	}
	void disFitRect() { m_withFitRect = false; }
	void fitMode(const XRect& rect);
	XPointCtrlStrMode getPCtrlMode()const { return m_PCtrlMode; }
	bool setPCtrlMode(XPointCtrlStrMode mode);
	void setCtrlMode(XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float l,float t,float r,float b);
	void disRange(){m_withRange = false;}
	const XVec2& getValue()const;
	void setValue(const XVec2& value);
	void setMapRange(float l,float t,float r,float b);
	void disMapRange(){m_withMap = false;}
public:
	XBool init(const XVec2& position,const XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	virtual void update(float steptime);

	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState);		//do nothing
	XBool canGetFocus(const XVec2& p);	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	XBool canLostFocus(const XVec2&){return XTrue;}	//do nothing
	virtual void setLostFocus() //����ʧȥ����
	{ 
		m_isDown = false;
		m_isBeChoose = XFalse; 
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

//	void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XBool setACopy(const XPointCtrl & temp);

	XBool setFontString(const char *str);

	//virtual void justForTest() {;}

	XPointCtrl()
		:m_isInited(XFalse)
		, m_withFont(false)
		, m_pixelSize(10.0f)
		, m_isDown(false)
		, m_range(0.0f, 0.0f, 100.0f, 100.0f)
		, m_withRange(false)
		, m_mapRange(0.0f, 0.0f, 100.0f, 100.0f)
		, m_withMap(false)
		, m_mapValue(0.0f)
//		,m_funDataChange(NULL)
//		,m_pClass(NULL)
		, m_ctrlMode(CTRL_MODE_SIMPLE)
		, m_autoString(true)
		, m_withFitRect(false)
	{
		m_ctrlType = CTRL_OBJ_POINTCTRL;
	}
	~XPointCtrl() {release();}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(!XXml::addLeafNode(e,(m_ctrlName + "X").c_str(),XString::toString(getValue().x))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Y").c_str(),XString::toString(getValue().y))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		XVec2 tmp;
		if(XXml::getXmlAsFloat(e,(m_ctrlName + "X").c_str(),tmp.x) == NULL) return XFalse;
		if(XXml::getXmlAsFloat(e,(m_ctrlName + "Y").c_str(),tmp.y) == NULL) return XFalse;
		setValue(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XPointCtrl.inl"
#endif
}
#endif