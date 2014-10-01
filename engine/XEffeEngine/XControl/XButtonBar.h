#ifndef _JIA_XBUTTONBAR_
#define _JIA_XBUTTONBAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.9.6
//--------------------------------
#include "XButton.h"

struct _XButtonBarCore
{
	_XBool isEnable;	//�Ƿ���Ч
	int width;
	std::string name;
	_XButton *button;
	_XButtonBarCore()
		:button(NULL)
	{}
	~_XButtonBarCore(){}
};
class _XButtonBar:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	float m_insertPoint;	//���밴ť��λ��
	int m_barHeight;	//�߶�
	std::vector<_XButtonBarCore> m_buttons;
public:
	_XBool initWithoutTex(int height = 32);
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);
	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const;	//��ȡ�ؼ��������ɫ
	
	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	int getIndexByName(const std::string &name);
	_XBool addAButton(const std::string &name,const std::string &caption = "");
	_XBool addAButton(const std::string& name,int width,const std::string &caption = "");
	//+++++++++++++++++++++++++++++++++++++++
	//�����Ƕ԰�ť����ز����ķ�װ
	_XBool setBtnEnable(const std::string &name);		//���ð�ťʹ��
	_XBool setBtnDisable(const std::string &name);	//���ð�ťʧЧ
	_XBool setBtnCaption(const std::string &name,const std::string &caption);	//���ð�ť������
	_XBool setBtnCallbackFun(const std::string &name,
		void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);	//���ð�ť�Ļص�����
	_XBool setBtnMouseDownCB(const std::string &name,
		void (* funMouseDown)(void *,int),
		void *pClass = NULL);	//���ð�ť������ʱ�Ļص�����
	//��������icon��صĺ���
	_XBool setBtnNormalIcon(const std::string &name,const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool setBtnNormalIcon(const std::string &name,const _XSprite &icon);
	_XBool setBtnDisableIcon(const std::string &name,const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool setBtnDisableIcon(const std::string &name,const _XSprite &icon);
	_XBool removeBtnIcon(const std::string &name);	//ȥ�����е�icon
	_XBool setBtnIconPosition(const std::string &name,float x,float y);	//����icon��λ��
	_XBool setBtnIconSize(const std::string &name,float x,float y);	//����icon�����Ŵ�С
	_XBool setBtnIconSize(const std::string &name,float x){return setBtnIconSize(name,x,x);}
	//---------------------------------------
	int getBtnID(const std::string &name);	//��ȡָ����ť��ID
	_XBool setBtnComment(const std::string &name,const std::string &comment);
	_XBool setBtnHotKey(const std::string &name,int hotKey);
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
public:
	void release();
	_XButtonBar()
		:m_isInited(XFalse)
		,m_barHeight(32)
	{
		m_ctrlType = CTRL_OBJ_BTNBAR;
	}
	~_XButtonBar() {release();}

	virtual _XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	virtual _XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	void disable();
	void enable();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����

private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XButtonBar(const _XButtonBar &temp);
	_XButtonBar& operator = (const _XButtonBar& temp);
};
#include "XButtonBar.inl"
#endif