#ifndef _JIA_XPROPERTYBOX_
#define _JIA_XPROPERTYBOX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XSlider.h"
#include "XText.h"
#include "XEdit.h"
#include "XCheck.h"
#include "XCombo.h"
namespace XE{
//���Կ�
enum XPropertyLineType
{// �����е�����
	PROPERTY_BLANK,	//�հ��У����У�ʲô��û��
	PROPERTY_TITLE,	//ֻ��ʾ���ֵ���
	PROPERTY_EDIT,	//�����
	PROPERTY_CHECK,	//ѡ����
	PROPERTY_COMBO,	//ѡ��
};
class XPropertyLine
{//������
public:
	std::string name;	//�����е�����
	XPropertyLineType type;
	XFontUnicode *pNameFont;
	XControlBasic *pCtrl;

	XBool getIsFocus();
	void setFocus(bool flag);
	XBool canLostFocus(float x,float y);
	XBool canGetFocus();	//�Ƿ���Ի�ý���
	void draw();
	void drawUp();
	XBool mouseProc(float x,float y,XMouseState mouseState);
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void update(float stepTime);
	void insertChar(const char *ch,int len);
	void disable();
	void enable();
	XPropertyLine()
		:type(PROPERTY_BLANK)
		,pNameFont(NULL)
		,pCtrl(NULL)
	{}
	void setAlpha(float a);
	void setColor(float r,float g,float b,float a);
};
class XPropertyBox:public XControlBasic
{
private:
	XBool m_isInited;		//�Ƿ��Ѿ���ʼ��

	XBool m_haveTitle;		//�Ƿ�ӵ�б���
	std::string m_titleStr;	//��������
	XText m_titleTxt;		//������ʾ���������
	XFontUnicode m_font;	//ʹ�õ�������

//	XVector2 m_position;
//	XVector2 m_scale;

	float m_width;		//�ܵĿ��
	float m_nameWidth;	//�������ƵĿ�ȣ�Ĭ���Ƕ԰뿪
	int m_maxLineSum;	//��ʾ�������������

	XBool m_needShowSlider;	//�Ƿ���Ҫ��ʾ������
	XSlider m_slider;			//�Ҳ�Ļ�����
	int m_curStartLineIndex;	//��ǰ����ʼ�б��

	std::vector<XPropertyLine> m_propertyLines;	//������

	void setCurStartLineIndex(unsigned int index);//���õ�ǰ��ʼ�еı��
public:
	void setTitle(const char * title);	//���ñ���
	void disTitle();					//ȡ������
	XBool initWithoutSkin(int width,int maxLineSum);
	void setNameWidth(int width);

protected:
	void draw();								//��水ť
	void drawUp();							
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *ch,int len);
	XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:	//�����Կ�������һ��������
	XBool addAPropertyLine(const char * name,XPropertyLineType type,int menuSum = 0,int showMenuSum = 0);
	XBool setPropertyName(unsigned int index,const char * name);
	std::string getPropertyName(unsigned int index);
	XBool setPropertyCtrlPrama(unsigned int index,int prama);
	XBool setPropertyStr(unsigned int index,const char * str);
	std::string getPropertyStr(unsigned int index);
	XBool setPropertyValue(unsigned int index,float value);		//��δʵ��
	float getPropertyValue(unsigned int index);		//��δʵ��
	XBool setPropertyIndex(unsigned int index,int order);
	int getPropertyIndex(unsigned int index);
	XBool setPropertyState(unsigned int index,XBool state);
	XBool getPropertyState(unsigned int index);
	XBool setPropertyMenuStr(unsigned int index,int menuIndex,const char *str);
	void updateSliderState();	//���»�������״̬

	XPropertyBox()
		:m_isInited(XFalse)
		,m_haveTitle(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_scale(1.0f,1.0f)
		,m_curStartLineIndex(0)
		,m_oldFocusIndex(-1)
//		,m_isCtrlDown(XFalse)
	{
		m_ctrlType = CTRL_OBJ_PROPERTYBOX;
	}
	~XPropertyBox(){release();}
	void release();

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	static void ctrlProc(void*,int,int);

	void disable();
	void enable();

	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XPropertyBox(const XPropertyBox &temp);
	XPropertyBox& operator = (const XPropertyBox& temp);
protected:
	void update(float stepTime);
	void updateRect();
	int m_oldFocusIndex;
	//XBool m_isCtrlDown;
	void moveFocus(bool flag);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(��δ���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		//�������еĿؼ���������״̬
		TiXmlElement elmNode(m_ctrlName.c_str());
		XBool flag = XTrue;
		for(unsigned int i = 0;i < m_propertyLines.size();++ i)
		{
			if(m_propertyLines[i].pCtrl != NULL && 
				!m_propertyLines[i].pCtrl->saveState(elmNode)) flag = XFalse;
		}
		if(e.InsertEndChild(elmNode) == NULL) flag = XFalse;
		return flag;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		//�������еĿռ䣬��ȡ��״̬
		TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
		if(keyNode == NULL) return XFalse;
		XBool flag = XTrue;
		for(unsigned int i = 0;i < m_propertyLines.size();++ i)
		{
			if(m_propertyLines[i].pCtrl != NULL && 
				!m_propertyLines[i].pCtrl->loadState(keyNode)) flag = XFalse;
		}
		return flag;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XPropertyBox.inl"
#endif
}
#endif