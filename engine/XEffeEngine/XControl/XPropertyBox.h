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
#include "XCtrlManagerBase.h"
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
	XBool canGetFocus(const XVec2& p);	//�Ƿ���Ի�ý���
	void draw();
	void disable();
	void enable();
	XPropertyLine()
		:type(PROPERTY_BLANK)
		,pNameFont(NULL)
		,pCtrl(NULL)
	{}
	void setAlpha(float a);
	void setColor(const XFColor& c);
};
class XPropertyBox :public XControlBasic
{
private:
	XBool m_isInited;		//�Ƿ��Ѿ���ʼ��

	XBool m_haveTitle;		//�Ƿ�ӵ�б���
	std::string m_titleStr;	//��������
	XText m_titleTxt;		//������ʾ���������
	XFontUnicode m_font;	//ʹ�õ�������

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
	XBool initWithoutSkin(int width, int maxLineSum);
	void setNameWidth(int width);

protected:
	void draw();								//��水ť
	void drawUp();
	XBool mouseProc(const XVec2& p, XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder, XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *ch, int len) { m_ctrlManager.insertChar(ch, len); }
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&) { return XTrue; }	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:	//�����Կ�������һ��������
	XBool addAPropertyLine(const char * name, XPropertyLineType type, int menuSum = 0, int showMenuSum = 0);
	XBool setPropertyName(unsigned int index, const char * name);
	const std::string& getPropertyName(unsigned int index)const;
	XBool setPropertyCtrlPrama(unsigned int index, int prama);
	XBool setPropertyStr(unsigned int index, const char * str);
	std::string getPropertyStr(unsigned int index)const;
	XBool setPropertyValue(unsigned int index, float value);		//��δʵ��
	float getPropertyValue(unsigned int index);		//��δʵ��
	XBool setPropertyIndex(unsigned int index, int order);
	int getPropertyIndex(unsigned int index)const;
	XBool setPropertyState(unsigned int index, XBool state);
	XBool getPropertyState(unsigned int index)const;
	XBool setPropertyMenuStr(unsigned int index, int menuIndex, const char *str);
	XBool setPropertyMenuStr(unsigned int index, const char *str);
	void updateSliderState();	//���»�������״̬

	XPropertyBox()
		:m_isInited(XFalse)
		, m_haveTitle(XFalse)
		, m_curStartLineIndex(0)
	{
		m_ctrlType = CTRL_OBJ_PROPERTYBOX;
	}
	~XPropertyBox() { release(); }
	void release();

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	static void ctrlProc(void*, int, int);

	void disable();
	void enable();

	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XPropertyBox(const XPropertyBox &temp);
	XPropertyBox& operator = (const XPropertyBox& temp);
protected:
	void update(float stepTime);
	void updateRect();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(��δ���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		//�������еĿؼ���������״̬
		TiXmlElement elmNode(m_ctrlName.c_str());
		XBool flag = XTrue;
		for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
		{
			if (m_propertyLines[i].pCtrl != NULL &&
				!m_propertyLines[i].pCtrl->saveState(elmNode)) flag = XFalse;
		}
		if (e.InsertEndChild(elmNode) == NULL) flag = XFalse;
		return flag;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if (!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		//�������еĿռ䣬��ȡ��״̬
		TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
		if (keyNode == NULL) return XFalse;
		XBool flag = XTrue;
		for (unsigned int i = 0; i < m_propertyLines.size(); ++i)
		{
			if (m_propertyLines[i].pCtrl != NULL &&
				!m_propertyLines[i].pCtrl->loadState(keyNode)) flag = XFalse;
		}
		return flag;
	}
	//---------------------------------------------------------
private:
	XCtrlManagerBase m_ctrlManager;	//����һ���ؼ�������
};
enum XSerialInfoPpb
{
	S_PPB_PORT,
	S_PPB_BITRATE,
	S_PPB_MODE,
	S_PPB_WITHEVENT,
	S_PPB_WITHOLP,
	S_PPB_DATABIT,
	S_PPB_STOPBIT,
	S_PPB_DTR,
	S_PPB_RTS,
	S_PPB_WAIT_RECV,
};
extern void setAsSerialPortInfo(XPropertyBox &pb, const char* title);
struct XSerialPortInfo;
extern bool getSerialPortInfo(const XPropertyBox& pb, XSerialPortInfo& tmpInfo);
extern void setSerialPortInfo(XPropertyBox& pb, const XSerialPortInfo& tmpInfo);

#if WITH_INLINE_FILE
#include "XPropertyBox.inl"
#endif
}
#endif