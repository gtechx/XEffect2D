#ifndef _JIA_XCOMBO_
#define _JIA_XCOMBO_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
#include "../XXml.h"
namespace XE{
//�����˵��������ͼ
class XComboSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *comboInputNormal;			//�����˵���������ͨ״̬
	XTextureData *comboInputDisable;			//��Ч״̬�µİ�ť��ͼ
	XButtonSkin downButtonTex;
	XButtonSkin downMenuUpButtonTex;
	XButtonSkin downMenuButtonTex;
	XButtonSkin downMenuDownButtonTex;

	XRect m_mouseRect;

	XComboSkin();
	~XComboSkin() {release();}
	XBool init(const char *inputNormal,const char *inputDisable,
		const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};

#define DEFAULT_COMBO_BT_SIZE (MIN_FONT_CTRL_SIZE)
#define DEFAULT_COMBO_MN_HEIGHT (MIN_FONT_CTRL_SIZE)
#define DEFAULT_COMBO_UD_HEIGHT (MIN_FONT_CTRL_SIZE * 0.75f)

class XPropertyLine;
class XCombo:public XControlBasic
{
	friend XPropertyLine;
private:
	//static const int m_comboMaxMenuLength = 4096;	//������Ҫ�Ż���
	static const int m_comboLeftDistance = 5;
	static const int m_comboTopDistance = 0;
	//static const int m_defaultComboBtSize = MIN_FONT_CTRL_SIZE;
	//static const int m_defaultComboMNHeight = MIN_FONT_CTRL_SIZE;
	XBool m_isInited;
	XBool m_isDrawDownMenu;		//�Ƿ���ʾ�����˵�
	XBool m_withoutTex;	//û����ͼ����ʽ
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ�ֿؼ���̬������ı���
	bool m_isDraw;	//�Ƿ��ǲ˵�����
	XMoveData m_actionMoveData;
	//-----------------------------------------------------

	const XTextureData *m_comboInputNormal;			//�����˵���������ͨ״̬
	const XTextureData *m_comboInputDisable;			//��Ч״̬�µİ�ť��ͼ

	int m_curMenuSum;			//��ǰ���е�ѡ������(��δʵ�֣���δ��Ч)
	int m_menuSum;				//�����˵��е��ܲ˵�������
	int m_menuDrawSum;			//�����˵�����ʾ�Ĳ˵�������
	int m_menuStartDrawOrder;	//�����˵��п�ʼ��ʾ�ĵ�һ���˵���ı��
	int m_menuTextWidth;		//�����˵�����ʾ�����ֵ����ֿ��
	int m_captionTextWidth;		//��������ʾ���ַ����Ŀ��

	int m_curChooseOrder;		//��ǰѡ��Ĳ˵���ı��

	XSprite m_sprite;			//������ʾ��ͼ�ľ���
	XFontUnicode m_caption;	//���ֵ�����
	XVec2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	XFColor m_textColor;		//���ֵ���ɫ
		
	std::string m_menuData;
	//char *m_menuData;			//�����˵��ľ�������
	XButton *m_buttom;			//�����˵��еİ�ť��3����ťΪ���ܰ�ť������Ϊ�˵���ť

	void updateString();				//�����ַ���������Ĳ˵�����
	void changeDownMenuState();		//�ı������˵���״̬

	static void funCtrlProc(void*,int,int);

	XRect m_inputArea;			//��������Ӧ��Χ
	XRect m_downButtonArea;	//������ť����Ӧ��Χ
	XRect m_downMenuUpArea;		//�Ϸ�ҳ����Ӧ��Χ
	XRect m_downMenuArea;		//ѡ�������Ӧ��Χ
	XRect m_downMenuDownArea;	//�·�ҳ����Ӧ��Χ
	XRect m_allArea;			//�����ռ����Ӧ��Χ

	XResourceInfo *m_resInfo;
public:
	XBool init(const XComboSkin &tex,	//�ؼ�����ͼ
		const XVec2& position,		//�ؼ���λ��
		const XRect& inputArea,		//��������Ч��Χ
		const XRect& downButtonArea,	//������ť����Ӧ��Χ
		const XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
		const XRect& downMenuArea,		//ѡ�������Ӧ��Χ
		const XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const XFontUnicode& font,		//��ʾ����ʹ�õ�����
		float fontSize = 1.0f);				//����Ĵ�С
	XBool initEx(const XComboSkin &tex,	//����ӿڵļ򻯰汾
		const XVec2& position,		
		int menuSum,					
		int drawMenuSum,				
		const XFontUnicode& font,		
		float fontSize = 1.0f);
	XBool initPlus(const char * path,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const XFontUnicode& font,		//��ʾ����ʹ�õ�����
		float fontSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	//int initWithoutSkin(const XRect& inputArea,		//��������Ч��Χ
	//	const XRect& downButtonArea,	//������ť����Ӧ��Χ
	//	const XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
	//	const XRect& downMenuArea,		//ѡ�������Ӧ��Χ
	//	const XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
	//	int menuSum,					//�����˵��е�ѡ�������
	//	int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
	//	const XFontUnicode& font,		//��ʾ����ʹ�õ�����
	//	float fontSize);
	XBool initWithoutSkin(int inputLen,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const XFontUnicode& font,		//��ʾ����ʹ�õ�����
		float fontSize = 1.0f);
	XBool initWithoutSkin(int inputLen,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum)				//�����˵�����ʾ�Ĳ˵��������
	{
		return initWithoutSkin(inputLen,menuSum,drawMenuSum,getDefaultFont(),1.0f);
	}

protected:			
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& p);
	void setLostFocus();	//����ʧȥ����
public:
	int getCurChooseOrder()const {return m_curChooseOrder;}	//��ȡ��ǰѡ����
	const std::string getCurChooseMenu(); 	//��ȡ��ǰѡ��Ĳ˵�������
	XBool setCurChooseOrder(int index);	//���õ�ǰѡ����
	XBool setMenuStr(const char *str,int order);			//����ĳ�������˵�������
	XBool setMenuStr(const char *str);			//���ö����ֵ��ÿ��֮����';'����,�����������ƥ�䣬���Զ�ƥ��
	std::string getMenuStr() const;
	std::string getMenuStr(int order);
//	void setShowMenuSum(int sum);	//��̬�ı���ʾ��ѡ������(��δʵ��)
//	void setMenuSum(int sum);		//�����ܵĲ˵�����(��δʵ��)
	//�������������˵��Ĳ˵�����
	bool resetMenuSum(int menuSum,int showMenuSum);	//���������δ�����ϸ����

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2&s);			//���ÿؼ��ĳߴ�

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2&p);		//���ÿռ��λ��

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c); 	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	//����ӵ���Լ�����Ϊ,������ĸ��ʹ��ͬ������Դ,���ĸ����Դ���ı�,��������ԴҲ����Ӧ�ı��ı�
	XBool setACopy(const XCombo &temp);	//����һ������
	void release();						//�ͷ���Դ
	XCombo();
	~XCombo(){release();}

	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
	//�����������ֻ���ڲ�ʹ����ͼ��ʱ�����Ч
	void setInputLen(int len);
	int getInputLen()const{return m_inputArea.getWidth();}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XCombo(const XCombo &temp);
	XCombo& operator = (const XCombo& temp);
public:
	enum XComboEvent
	{
		CMB_DATA_CHANGE,
	};
	enum XComboStyle
	{
		CMB_STYLE_DOWN,	//���µĲ˵���ʽ
		CMB_STYLE_UP,	//���ϵĲ˵���ʽ
	};
private:
	XComboStyle m_curStyle;	//��ǰ����ʽ
	void updateMenuBtnPosition();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	void setStyle(XComboStyle style)
	{
		if(m_curStyle == style) return;
		m_curStyle = style;
		if(!m_isInAction)
			updateMenuBtnPosition();
	}
	XComboStyle getStyle()const{return m_curStyle;}
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		int tmp = getCurChooseOrder();
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(tmp))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		int tmp;
		if(XXml::getXmlAsInt(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setCurChooseOrder(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XCombo.inl"
#endif
}
#endif