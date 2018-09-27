#ifndef _JIA_XTAB_
#define _JIA_XTAB_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.11
//--------------------------------
//����Ǳ�ǩ�ؼ�(��δʵ��)
#include "XButton.h"
#include "XCtrlManagerBase.h"
namespace XE{
#define TAB_TITLE_HEIGHT (MIN_FONT_CTRL_SIZE)	//����ĸ߶�

class XTabObject
{
public:
	XBool isEnable;					//�Ƿ���Ч
	XBool isVisible;					//�Ƿ�ɼ�
	std::string srcTitle;				//ԭʼ����û�����Ƴ��ȵ�
	std::string title;					//����
	std::vector<XControlBasic *> objs;	//���
	XFontUnicode *pFont;

	XBool needDraw;
	int len;	//��ʾ�ĳ���
	int offset;	//ƫ��
	std::vector<XVec2> pos;		//��������λ��
	std::vector<XVec2> scale;	//�������Գߴ�
	XTabObject()
		:isEnable(XTrue)
		,isVisible(XTrue)
		,pFont(NULL)
	{}
	virtual ~XTabObject(){}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};

//����
//1�����ñ�ǩ�ɼ�		x	//����Ҫ����
//2�����ñ�ǩ�Ƿ��ѡ	x	//����Ҫ����
//3��������ǩ			x
//4�����̲�����ǩ		x

class XTab:public XControlBasic
{
private:
	static const int m_tabTitleDistance = 3;	//����֮��ļ��//ʹ�����ַ�ʽ�����
	static const int m_tabBtnWidth = 20;
//	XRect m_mouseRect;		//��Χ
//	XRect m_curMouseRect;	//��Χ
//	XVec2 m_position;	//λ��
//	XVec2 m_scale;		//��С
	XRect m_titleRect;

	XBool m_isInited;
	XBool m_withoutTex;	//�Ƿ�ӵ����ͼ

	XButton m_leftBtn;
	XButton m_rightBtn;
	XFColor m_textColor;		//���ֵ���ɫ

	std::vector<XTabObject> m_tabObjects;	//���е����

	int m_curChooseTabIndex;	//��ǰѡ��ı�ǩ���
	int m_curStartTabIndex;		//��ǰ��ʾ�ĵ�һ����ǩ�ı��
	int m_maxTitleLen;			//��������ǩ
	float m_curTextWidth;		//����Ŀ��
	XFontUnicode m_texFont;

	void updateTitle();	//���±������ʾ
	void updateBtnState();	//���°�ť��״̬
	void updateObjState(bool flag = true);
	static void ctrlProc(void*,int,int);

	//std::string m_curChooseTabStr;
public:
	enum XTabEvent
	{
		TAB_CHOOSE,
	};
	const std::string& getCurChooseTabStr()const {return m_tabObjects[m_curChooseTabIndex].srcTitle;}
private:
//	void (*m_funChooseTab)(void *,int,const std::string& );
//	void *m_pClass;				//�ص������Ĳ���
	void(*m_funUpDraw)(void *);
	void* m_pClass;				//�ص������Ĳ���
public:
	XBool initWithoutSkin(const XRect& rect,const XFontUnicode& font);
	XBool initWithoutSkin(const XRect& rect) {return initWithoutSkin(rect,getDefaultFont());}
	XBool initWithoutSkin(const XVec2& pixelSize) 
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),getDefaultFont());
	}
//	void setChooseTabCB(void (* funChooseTab)(void *,int,const std::string& ),void *pClass = NULL);
	void setUpDrawCB(void(*funUpDraw)(void *), void *pClass = NULL);
protected:
	void draw();
	void drawUp();
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int,XKeyState);		//�����Ƿ񴥷���������
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����

public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	XBool setTabStr(const char *str,unsigned int index);
	XBool setTabsStr(const char *str);		//;��Ϊ�ָ���
	const std::string& getTabStr(unsigned int index)const;
	std::string getTabsStr();
	int getTabIndexByTitle(const std::string& title) const;
	void addATab(const std::string& title);
	void addATabs(const std::string& title);
	void delATab(const std::string& title);
	void clearAllObj();	//������еĴ�����ϵ
	void addObjToTab(XControlBasic *obj,unsigned int index);	//��һ����ǩ��������
	void addObjToTab(XControlBasic *obj,const std::string& title);	//��һ����ǩ��������
	void setTabVisible(bool flag,int index);	//���ñ�ǩ�ɼ�
	void setTabVisible(bool flag,const std::string& title);	//���ñ�ǩ�ɼ�
	XBool getTabVisible(unsigned int index) const;
	XBool getTabVisible(const std::string& title) const;
	void setTabEnable(bool flag,unsigned int index);
	void setTabEnable(bool flag,const std::string& title);
	XBool getTabEnable(unsigned int index) const;
	XBool getTabEnable(const std::string& title) const;

	void delObjFromTab(XControlBasic *obj,int index);
	void delObjFromTab(XControlBasic *obj,const std::string& title);
	//����Ľӿ���δʵ��
	void clearObjFromTab(int index);
	void clearObjFromTab(const std::string& title);

	XTab()
		:m_isInited(XFalse)
		, m_withoutTex(XTrue)
		, m_curChooseTabIndex(0)
		, m_curStartTabIndex(0)
//		,m_funChooseTab(NULL)
//		,m_pClass(NULL)
		, m_funUpDraw(nullptr)
		, m_pClass(nullptr)
	{
		m_ctrlType = CTRL_OBJ_TAB;
	}
	~XTab(){release();}
	void release();

	void disable();
	void enable();
	void setVisible();	//������ʾ
	void disVisible();	//���ò���ʾ
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XTab(const XTab &temp);
	XTab& operator = (const XTab& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMoveData m_actionMoveData;	//��̬Ч���ı���
	int m_oldChooseTabIndex;
	void startAction();
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʹ�ÿؼ��������ķ�ʽ�����tab
	XCtrlManagerBase m_ctrlManager;	//����һ���ؼ�������
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XTab.inl"
#endif
}
#endif