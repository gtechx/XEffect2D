#ifndef _JIA_XBUTTON_
#define _JIA_XBUTTON_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XControlBasic.h"
#include "XResourceManager.h"
//#include "../XMath/XMoveData.h"

//����һ����ť���࣬������Ӧ��ť�¼���
namespace XE{
enum XButtonState
{
	BUTTON_STATE_NORMAL,		//��ť״̬��ͨ״̬
	BUTTON_STATE_DOWN,		//��ť���µ�״̬
	BUTTON_STATE_ON,			//��ť��괦��������״̬
	BUTTON_STATE_DISABLE		//��ť��Ч��״̬
};
enum XButtonStyle
{
	BTN_STYLE_NORMAL,	//��ͨ��ʽ��ť
	BTN_STYLE_CHECK,	//check��ʽ�İ�ť
};
enum XButtonSymbol
{
	BTN_SYMBOL_NULL,		//û��
	BTN_SYMBOL_LINE,		//����
	BTN_SYMBOL_CIRCLE,		//Բ��
	BTN_SYMBOL_RECT,		//����
	BTN_SYMBOL_CROSS,		//ʮ�ּ�
	BTN_SYMBOL_TRIANGLE,	//������
	BTN_SYMBOL_MASK_RIGHT,	//�Ժ�
	BTN_SYMBOL_MASK_WRONG,	//���
	BTN_SYMBOL_LEFT,		//��
	BTN_SYMBOL_RIGHT,		//��
	BTN_SYMBOL_UP,			//��
	BTN_SYMBOL_DOWN,		//��

	BTN_SYMBOL_SAVE,		//����
	BTN_SYMBOL_LOAD,		//��ȡ
	BTN_SYMBOL_DEF,			//Ĭ��
	BTN_SYMBOL_DOWNLOAD,	//����
	BTN_SYMBOL_UPDATE,		//�ϴ�
};
//�ؼ��¼�����Ӧʹ�ûص������ķ�ʽ
//�����¼�	��ʱ�Ȳ���
//ע���¼�	��ʱ�Ȳ���
//�����¼�
//�����¼�
//�����¼�
class XButtonSkin
{
private:
	XBool m_isInited;
	//XSCounter *m_cp;		//���ü�����,��δ���
	void releaseTex();
public:
	XTextureData *buttonNormal;			//��ͨ״̬�µİ�ť��ͼ
	XTextureData *buttonDown;				//����״̬�µİ�ť��ͼ
	XTextureData *buttonOn;				//����״̬�µİ�ť��ͼ
	XTextureData *buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	XRect m_mouseRect;			//������Ӧ��Χ
	XVec2 m_fontPosition;	//�������ֵ�λ��
	int m_areaPointSum;			//����������
	XVec2 *m_pArea;			//��������Ϣ

	XButtonSkin();
	~XButtonSkin(){release();}
	XBool init(const char *normal,const char *down,const char *on,const char *disable,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};

//���ڵ����⣺����û����ʾ����
class XCombo;
class XMouseRightButtonMenu;
class XDirectoryList;
class XGroup;
class XSliderEx;
class XSliderInfinite;
class XPasswordPad;
class XTab;
class XCalendar;
class XButtonBar;
class XSubWindow;
class XSubWindowEx;
class XParameterCtrl;
class XButton:public XControlBasic
{
	friend XCombo;
	friend XMouseRightButtonMenu;
	friend XDirectoryList;
	friend XGroup;
	friend XSliderEx;
	friend XSliderInfinite;
	friend XPasswordPad;
	friend XTab;
	friend XCalendar;
	friend XButtonBar;
	friend XSubWindow;
	friend XSubWindowEx;
	friend XParameterCtrl;
private:
	XBool m_isInited;					//��ť�Ƿ񱻳�ʼ��
	XButtonState m_curButtonState;		//��ǰ�İ�ť״̬

	XFontUnicode m_caption;			//��ť�ı���

	const XTextureData *m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const XTextureData *m_buttonOn;				//����״̬�µİ�ť��ͼ
	const XTextureData *m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ
	XSprite m_sprite;	//������ʾ��ͼ�ľ���
	XVec2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	XVec2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	XFColor m_textColor;		//���ֵ���ɫ
	XVec2 m_upMousePoint;	//�ϴμ�¼�����λ��

	int m_hotKey;

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
	XBool initProc(const XFontUnicode& font,const char *caption,float captionSize);	//�����ĳ�ʼ������
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	XBool init(const XVec2& position,	//�ؼ����ڵ�λ��
		const XRect& Area,				//�ؼ������Ӧ�����䣬ʹ�õ��������ͼƬ���Ͻǵ�����
		const XButtonSkin &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const XFontUnicode& font,float captionSize,const XVec2& textPosition);	//�ؼ�����������Ϣ
	XBool initEx(const XVec2& position,	//����һ���ӿڵļ򻯰汾
		const XButtonSkin &tex,		
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f);
	XBool initPlus(const char * path,const char *caption,const XFontUnicode& font,float captionSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);//���Ǿ��������Ż��İ汾�������Ժ���ֻ��������汾
	XBool initWithoutSkin(const char *caption,const XFontUnicode& font,
		float captionSize,const XRect& area,const XVec2& textPosition);	//û����ͼ����ʽ,ֱ����д����ͼ������ͼ(��δʵ�֣�δ��ɹ���֮һ)
	XBool initWithoutSkin(const char *caption,const XFontUnicode& font,const XRect& area);	//����ӿ����ϸ��ӿڵļ򻯰汾
	XBool initWithoutSkin(const char *caption,const XRect& area) {return initWithoutSkin(caption,getDefaultFont(),area);}
	XBool initWithoutSkin(const char *caption,const XVec2& pixelSize) 
	{
		return initWithoutSkin(caption,getDefaultFont(),XRect(XVec2::zero,pixelSize));
	}
	XBool initWithoutSkin(const char *caption,float width) 
	{
		return initWithoutSkin(caption,getDefaultFont(), XRect(0.0f, 0.0f, width, MIN_FONT_CTRL_SIZE));
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);
protected:
	void draw();								//��水ť
	void drawUp();							
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	void setTexture(const XButtonSkin& tex);

	XBool setACopy(const XButton &temp);

	XButton();
	~XButton(){release();}
	void release();
	//������Ϊ������Ч�ʶ��������������
	void setCaptionPosition(const XVec2& textPosition);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
//	void setCaptionPosition(float x,float y);						//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionSize(const XVec2& size);						//���ð�ť�ı���ĳߴ�
//	void setCaptionSize(float x,float y);							//���ð�ť�ı���ĳߴ�
	void setCaptionText(const char *caption);						//���ð�ť�ı��������
	void setCaptionAlignmentModeX(XFontAlignmentModeX x){m_caption.setAlignmentModeX(x);}			//��������Ķ��䷽ʽ
	void setCaptionAlignmentModeY(XFontAlignmentModeY y){m_caption.setAlignmentModeY(y);}	
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//���ð������ȼ�
	int getHotKey() const;			//��ȡ��ǰ�������ȼ���Ϣ
	void setState(XButtonState temp);		//���ð�ť��״̬
	XButtonState getState() const; 
	void disable();
	void enable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
	//�ڲ�ʹ����ͼ������²��ܵ����������
	//�����������֮�������Զ��޸�Ϊ���ж���
	void setWidth(int width);	//���ð�ť�Ŀ��
	int getWidth()const{return m_mouseRect.getWidth();}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XButton(const XButton &temp);
	XButton& operator = (const XButton& temp);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˸�btn���icon������Ľӿڣ�icon������:��ͨicon����Чicon
private:
	XBool m_withNormalIcon;		//�Ƿ�ӵ����ͨicon
	XSprite m_normalIcon;
	XBool m_withDisableIcon;		//�Ƿ�ӵ����Чicon
	XSprite m_disableIcon;	
	XVec2 m_iconSize;		//icon�ĳߴ�
	XVec2 m_iconPosition;	//icon��λ��
public:
	void setNormalIcon(const char * filename,
		XResPos resPos = RES_SYS_DEF);
	void setNormalIcon(const XSprite &icon);
	void setDisableIcon(const char * filename,
		XResPos resPos = RES_SYS_DEF);
	void setDisableIcon(const XSprite &icon);
	void removeIcon();	//ȥ�����е�icon
//	void setIconPosition(float x, float y) { setIconPosition(XVec2(x, y)); }	//����icon��λ��
	void setIconPosition(const XVec2& p);	//����icon��λ��
//	void setIconSize(float x,float y) { setIconSize(XVec2(x, y)); }		//����icon�����Ŵ�С
	void setIconSize(const XVec2& p);	//����icon�����Ŵ�С
	void setIconSize(float x) { setIconSize(XVec2(x)); }
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMoveData m_actionMoveData;	//��̬Ч���ı���
	XVec2 m_oldPos;				//��������ʱ��λ��
	XVec2 m_oldSize;			//��������ʱ�Ĵ�С
	XMoveData m_lightMD;
	XRect m_lightRect;
public:
protected:
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����������ʾ�������
private:
	XButtonSymbol m_symbolType;	//��ʾ�ķ���
public:
	void setSymbol(XButtonSymbol type){m_symbolType = type;}	//������ʾ�ķ���
	XButtonSymbol getSymbol() const {return m_symbolType;}		//��ȡ��ʾ�ķ���
	void disSymbol(){m_symbolType = BTN_SYMBOL_NULL;}			//ȡ��������ʾ
	//---------------------------------------------------------
public:
	enum XButtonEvent
	{
		BTN_INIT,
		BTN_RELEASE,
		BTN_MOUSE_ON,
		BTN_MOUSE_DOWN,
		BTN_MOUSE_UP,
	};
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��֧��check��ʽ�İ�ť�������
private:
	XButtonStyle m_buttonStyle;	//��ť����ʽ
	bool m_isCheck;	//�Ƿ���
public:
	void setStyle(XButtonStyle style){m_buttonStyle = style;}
	void setIsCheck(bool flag){m_isCheck = flag;}
	bool getIsCheck()const{return m_isCheck;}
	//--------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XButton.inl"
#endif
}
#endif