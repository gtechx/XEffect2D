#ifndef _JIA_XTAB_
#define _JIA_XTAB_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.11
//--------------------------------
//����Ǳ�ǩ�ؼ�(��δʵ��)
#include "XButton.h"

#define TAB_MODE (1)

#define TAB_TITLE_HEIGHT (32)	//����ĸ߶�
#define TAB_TITLE_DISTANCE (3)	//����֮��ļ��
#define TAB_BTN_WIDTH (20)

struct _XTabObject
{
	_XBool isEnable;					//�Ƿ���Ч
	_XBool isVisible;					//�Ƿ�ɼ�
	std::string srcTitle;				//ԭʼ����û�����Ƴ��ȵ�
	std::string title;					//����
	std::vector<_XControlBasic *> objs;	//���
	_XFontUnicode *pFont;

	_XBool needDraw;
	int len;	//��ʾ�ĳ���
	int offset;	//ƫ��
	std::vector<_XVector2> pos;		//��������λ��
	std::vector<_XVector2> size;	//�������Գߴ�
	_XTabObject()
		:isEnable(XTrue)
		,isVisible(XTrue)
		,pFont(NULL)
	{}
};

//����
//1�����ñ�ǩ�ɼ�		x	//����Ҫ����
//2�����ñ�ǩ�Ƿ��ѡ	x	//����Ҫ����
//3��������ǩ			x
//4�����̲�����ǩ		x

class _XTab:public _XControlBasic
{
private:
//	_XRect m_mouseRect;		//��Χ
//	_XRect m_nowMouseRect;	//��Χ
//	_XVector2 m_position;	//λ��
//	_XVector2 m_size;		//��С
	_XRect m_titleRect;

	_XBool m_isInited;
	_XBool m_withoutTex;	//�Ƿ�ӵ����ͼ

	_XButton m_leftBtn;
	_XButton m_rightBtn;
	_XFColor m_textColor;		//���ֵ���ɫ

	std::vector<_XTabObject> m_tabObjects;	//���е����

	int m_nowChooseTabIndex;	//��ǰѡ��ı�ǩ���
	int m_nowStartTabIndex;		//��ǰ��ʾ�ĵ�һ����ǩ�ı��
	int m_maxTitleLen;			//��������ǩ
	float m_nowTextWidth;		//����Ŀ��
	_XFontUnicode m_texFont;

	void updateTitle();	//���±������ʾ
	void updateBtnState();	//���°�ť��״̬
	void updateObjState(bool flag = true);
	friend void tabBtnProc(void*,int);

	void (*m_funChooseTab)(void *,int,const std::string &);
	void *m_pClass;				//�ص������Ĳ���
public:
	_XBool initWithoutTex(const _XRect &rect,const _XFontUnicode &font);
	_XBool initWithoutTex(const _XRect &rect) {return initWithoutTex(rect,XEE::systemFont);}
	_XBool initWithoutTex(const _XVector2 &pixelSize) 
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),XEE::systemFont);
	}
	void setChooseTabCB(void (* funChooseTab)(void *,int,const std::string &),void *pClass = NULL);
protected:
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	
	_XBool keyboardProc(int,_XKeyState);		//�����Ƿ񴥷���������

	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����

public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	_XBool setTabStr(const char *str,int index);
	_XBool setTabsStr(const char *str);		//;��Ϊ�ָ���
	std::string getTabStr(int index);
	std::string getTabsStr();
	int getTabIndexByTitle(const std::string &title) const;
	void addATab(const std::string &title);
	void clearAllObj();	//������еĴ�����ϵ
	void addObjToTab(_XControlBasic *obj,int index);	//��һ����ǩ��������
	void addObjToTab(_XControlBasic *obj,const std::string &title);	//��һ����ǩ��������
	void setTabVisible(bool flag,int index);	//���ñ�ǩ�ɼ�
	void setTabVisible(bool flag,const std::string &title);	//���ñ�ǩ�ɼ�
	_XBool getTabVisible(int index) const;
	_XBool getTabVisible(const std::string &title) const;
	void setTabEnable(bool flag,int index);
	void setTabEnable(bool flag,const std::string &title);
	_XBool getTabEnable(int index) const;
	_XBool getTabEnable(const std::string &title) const;
	//����Ľӿ���δʵ��
	void delObjFromTab(_XControlBasic *obj,int index);
	void delObjFromTab(_XControlBasic *obj,const std::string &title);
	void clearObjFromTab(int index);
	void clearObjFromTab(const std::string &title);

	_XTab()
		:m_isInited(XFalse)
		,m_withoutTex(XTrue)
		,m_nowChooseTabIndex(0)
		,m_nowStartTabIndex(0)
		,m_funChooseTab(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_XTAB;
	}
	~_XTab(){release();}
	void release();

	void disable();
	void enable();
	void setVisible();	//������ʾ
	void disVisible();	//���ò���ʾ
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XTab(const _XTab &temp);
	_XTab& operator = (const _XTab& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	_XMoveData m_actionMoveData;	//��̬Ч���ı���
	int m_oldChooseTabIndex;
	void startAction();
	void update(int stepTime);
	//---------------------------------------------------------
};

#include "XTab.inl"

#endif