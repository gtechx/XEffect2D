#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XXml.h"
namespace XE{
class XPointCtrl:public XControlBasic
{
private:
	static const int m_pointCtrlLineSum = 10;//����һ������ԭ�ؼ��㣬������Ӧ����λ�ò���
	XBool m_isInited;		//�Ƿ��ʼ��
//	XVector2 m_position;	//�ؼ���λ��
//	XVector2 m_scale;		//��С
	XVector2 m_pixelSize;	//���ش�С
	XVector2 m_truePixelSize;	//��ʵ�����سߴ�
	XFontUnicode m_font;
	char m_textStr[64];		//��ʾ���ַ���

	bool m_withFont;
	bool m_isDown;		//�Ƿ����ʰȡ
	bool m_withRange;	//��Χ�����Ƿ���Ч
	bool m_withMap;		//�Ƿ����ӳ��
	XRect m_range;		//�����ƶ��ķ�Χ
	XRect m_mapRange;	//ӳ��ķ�Χ
	XVector2 m_mapValue;
	bool m_autoString;	//�Զ�������ʾ������

	void updateData();
public:
	enum XPointCtrlEvent
	{
		PITCTRL_DATA_CHANGE,
	};
private:
	XCtrlMode m_ctrlMode;		//�ؼ���ģʽ
public:
	void setCtrlMode(XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float l,float t,float r,float b);
	void disRange(){m_withRange = false;}
	XVector2 getValue();
	void setValue(const XVector2 &value);
	void setMapRange(float l,float t,float r,float b);
	void disMapRange(){m_withMap = false;}
public:
	XBool init(const XVector2& position,const XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}

	XBool mouseProc(float x,float y,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState);		//do nothing
	XBool canGetFocus(float x,float y);	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	XBool canLostFocus(float,float){return XTrue;}	//do nothing

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

//	void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XBool setACopy(const XPointCtrl & temp);

	XBool setFontString(const char *str);

	//virtual void justForTest() {;}

	XPointCtrl()
		:m_isInited(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_scale(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_range(0.0f,0.0f,100.0f,100.0f)
		,m_withRange(false)
		,m_mapRange(0.0f,0.0f,100.0f,100.0f)
		,m_withMap(false)
		,m_mapValue(0.0f,0.0f)
//		,m_funDataChange(NULL)
//		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
		,m_autoString(true)
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
		XVector2 tmp;
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