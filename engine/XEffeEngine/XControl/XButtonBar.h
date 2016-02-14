#ifndef _JIA_XBUTTONBAR_
#define _JIA_XBUTTONBAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.9.6
//--------------------------------
#include "XButton.h"
namespace XE{
struct XButtonBarCore
{
	XBool isEnable;	//�Ƿ���Ч
	int width;
	std::string name;
	XButton *button;
	XButtonBarCore()
		:button(NULL)
	{}
	~XButtonBarCore(){}
};
class XButtonBar:public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	float m_insertPoint;	//���밴ť��λ��
	int m_barHeight;	//�߶�
	std::vector<XButtonBarCore> m_buttons;
public:
	XBool initWithoutSkin(int height = 32);
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);
	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const;	//��ȡ�ؼ��������ɫ
	
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	int getIndexByName(const std::string &name);
	XBool addAButton(const std::string &name,const std::string &caption = "");
	XBool addAButton(const std::string& name,int width,const std::string &caption = "");
	//+++++++++++++++++++++++++++++++++++++++
	//�����Ƕ԰�ť����ز����ķ�װ
	XBool setBtnEnable(const std::string &name);		//���ð�ťʹ��
	XBool setBtnDisable(const std::string &name);	//���ð�ťʧЧ
	XBool setBtnCaption(const std::string &name,const std::string &caption);	//���ð�ť������
	XBool setEventProc(const std::string &name,
		void (* eventProc)(void *,int,int),void *pClass = NULL);
	//��������icon��صĺ���
	XBool setBtnNormalIcon(const std::string &name,const char * filename,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool setBtnNormalIcon(const std::string &name,const XSprite &icon);
	XBool setBtnDisableIcon(const std::string &name,const char * filename,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool setBtnDisableIcon(const std::string &name,const XSprite &icon);
	XBool removeBtnIcon(const std::string &name);	//ȥ�����е�icon
	XBool setBtnIconPosition(const std::string &name,float x,float y);	//����icon��λ��
	XBool setBtnIconSize(const std::string &name,float x,float y);	//����icon�����Ŵ�С
	XBool setBtnIconSize(const std::string &name,float x){return setBtnIconSize(name,x,x);}
	//---------------------------------------
	int getBtnID(const std::string &name);	//��ȡָ����ť��ID
	XBool setBtnComment(const std::string &name,const std::string &comment);
	XBool setBtnHotKey(const std::string &name,int hotKey);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
public:
	void release();
	XButtonBar()
		:m_isInited(XFalse)
		,m_barHeight(32)
	{
		m_ctrlType = CTRL_OBJ_BTNBAR;
	}
	~XButtonBar() {release();}

	virtual XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	virtual XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	void disable();
	void enable();
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����

private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XButtonBar(const XButtonBar &temp);
	XButtonBar& operator = (const XButtonBar& temp);
};
#if WITH_INLINE_FILE
#include "XButtonBar.inl"
#endif
}
#endif