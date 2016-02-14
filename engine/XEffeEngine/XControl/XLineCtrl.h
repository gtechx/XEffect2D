#ifndef _JIA_XLINECTRL_
#define _JIA_XLINECTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//����һ������ԭ�ؼ���
#include "XControlBasic.h"
#include "../XXml.h"
namespace XE{
class XLineCtrl:public XControlBasic
{
public:
	enum XLineCtrlType
	{
		LINE_CTRL_TYPEX_D,
		LINE_CTRL_TYPEX_U,
		LINE_CTRL_TYPE_Y_R,
		LINE_CTRL_TYPE_Y_L,
	};
private:
	static const int m_lineCtrlLineSum = 10;
//	XVector2 m_position;	//�ؼ���λ��
//	XVector2 m_scale;		//��С
	XVector2 m_pixelSize;	//���ش�С
	XVector2 m_truePixelSize;	//��ʵ�����سߴ�
	XFontUnicode m_font;
	char m_textStr[64];		//��ʾ���ַ���

	XLineCtrlType m_type;
	XBool m_isInited;		//�Ƿ��ʼ��
	bool m_withFont;
	bool m_isDown;		//�Ƿ����ʰȡ
	bool m_withRange;	//��Χ�����Ƿ���Ч
	bool m_withMap;		//�Ƿ����ӳ��
	bool m_withString;			//�Ƿ����ַ���
	XVector2 m_range;	//�����ƶ��ķ�Χ
	XVector2 m_mapRange;	//ӳ��ķ�Χ
	float m_mapValue;
	std::string m_showString;		//��Ҫ��ʾ���ַ���
	XFontUnicode m_stringFont;	//�ַ���������

	void updateData();
public:
	enum XLineCtrlEvent
	{
		LNCTRL_DATA_CHANGE,
	};
//	void (*m_funDataChange)(void *,int ID);
//	void *m_pClass;	

private:
	XCtrlMode m_ctrlMode;		//�ؼ���ģʽ
public:
	void setString(const char * str);

	void setCtrlMode(XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float min,float max);
	void disRange(){m_withRange = false;}
	float getValue();
	void setValue(float value);
	void setMapRange(float min,float max);
	void disMapRange(){m_withMap = false;}
public:
	XBool init(const XVector2& position,XLineCtrlType type = LINE_CTRL_TYPEX_D,const XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
//	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL);

	XBool mouseProc(float x,float y,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState){return XTrue;}		//do nothing
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

	//void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XBool setACopy(const XLineCtrl & temp);

	//virtual void justForTest() {;}

	XLineCtrl()
		:m_isInited(XFalse)
	//	,m_position(0.0f,0.0f)
	//	,m_scale(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_type(LINE_CTRL_TYPEX_D)
		,m_range(0.0f,100.0f)
		,m_withRange(false)
		,m_withMap(false)		//�Ƿ����ӳ��
		,m_mapRange(0.0f,100.0f)	//ӳ��ķ�Χ
		,m_mapValue(0.0f)
//		,m_funDataChange(NULL)
//		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
		,m_withString(false)
	{
		m_ctrlType = CTRL_OBJ_LINECTRL;
	}
	~XLineCtrl(){release();}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(getValue()))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		float tmp;
		if(XXml::getXmlAsFloat(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setValue(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XLineCtrl.inl"
#endif
}
#endif