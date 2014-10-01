#ifndef _JIA_XBUTTON_
#define _JIA_XBUTTON_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XControlBasic.h"
#include "XResourcePack.h"
#include "XResourceManager.h"
#include "../XBasicWindow.h"

//����һ����ť���࣬������Ӧ��ť�¼���

enum _XButtonState
{
	BUTTON_STATE_NORMAL,		//��ť״̬��ͨ״̬
	BUTTON_STATE_DOWN,		//��ť���µ�״̬
	BUTTON_STATE_ON,			//��ť��괦��������״̬
	BUTTON_STATE_DISABLE		//��ť��Ч��״̬
};
//�ؼ��¼�����Ӧʹ�ûص������ķ�ʽ
//�����¼�	��ʱ�Ȳ���
//ע���¼�	��ʱ�Ȳ���
//�����¼�
//�����¼�
//�����¼�
class _XButtonTexture
{
private:
	_XBool m_isInited;
	//_XSCounter *m_cp;		//���ü�����,��δ���
	void releaseTex();
public:
	_XTextureData *buttonNormal;			//��ͨ״̬�µİ�ť��ͼ
	_XTextureData *buttonDown;				//����״̬�µİ�ť��ͼ
	_XTextureData *buttonOn;				//����״̬�µİ�ť��ͼ
	_XTextureData *buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	_XRect m_mouseRect;			//������Ӧ��Χ
	_XVector2 m_fontPosition;	//�������ֵ�λ��
	int m_areaPointSum;			//����������
	_XVector2 *m_pArea;			//��������Ϣ

	_XButtonTexture();
	~_XButtonTexture(){release();}
	_XBool init(const char *normal,const char *down,const char *on,const char *disable,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//���ڵ����⣺����û����ʾ����
class _XCombo;
class _XMouseRightButtonMenu;
class _XDirectoryList;
class _XGroup;
class _XSliderEx;
class _XPasswordPad;
class _XTab;
class _XCalendar;
class _XButtonBar;
class _XButton:public _XControlBasic
{
	friend _XCombo;
	friend _XMouseRightButtonMenu;
	friend _XDirectoryList;
	friend _XGroup;
	friend _XSliderEx;
	friend _XPasswordPad;
	friend _XTab;
	friend _XCalendar;
	friend _XButtonBar;
private:
	_XBool m_isInited;					//��ť�Ƿ񱻳�ʼ��
	_XButtonState m_nowButtonState;		//��ǰ�İ�ť״̬

	_XFontUnicode m_caption;			//��ť�ı���

	const _XTextureData *m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const _XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonOn;				//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	void (*m_funInit)(void *,int ID);
	void (*m_funRelease)(void *,int ID);
	void (*m_funMouseOn)(void *,int ID);
	void (*m_funMouseDown)(void *,int ID);
	void (*m_funMouseUp)(void *,int ID);
	void *m_pClass;				//�ص������Ĳ���

	_XSprite m_sprite;	//������ʾ��ͼ�ľ���
	_XVector2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
	_XVector2 m_upMousePoint;	//�ϴμ�¼�����λ��

	int m_hotKey;

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
	_XBool initProc(const _XFontUnicode &font,const char *caption,float captionSize);	//�����ĳ�ʼ������
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	_XBool init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ������Ӧ�����䣬ʹ�õ��������ͼƬ���Ͻǵ�����
		const _XButtonTexture &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &textPosition);	//�ؼ�����������Ϣ
	_XBool initEx(const _XVector2& position,	//����һ���ӿڵļ򻯰汾
		const _XButtonTexture &tex,		
		const char *caption,const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initPlus(const char * path,const char *caption,const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//���Ǿ��������Ż��İ汾�������Ժ���ֻ��������汾
	_XBool initWithoutTex(const char *caption,const _XFontUnicode &font,
		float captionSize,const _XRect& area,const _XVector2 &textPosition);	//û����ͼ����ʽ,ֱ����д����ͼ������ͼ(��δʵ�֣�δ��ɹ���֮һ)
	_XBool initWithoutTex(const char *caption,const _XFontUnicode &font,const _XRect& area);	//����ӿ����ϸ��ӿڵļ򻯰汾
	_XBool initWithoutTex(const char *caption,const _XRect& area) {return initWithoutTex(caption,XEE::systemFont,area);}
	_XBool initWithoutTex(const char *caption,const _XVector2& pixelSize) 
	{
		return initWithoutTex(caption,XEE::systemFont,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y));
	}
	_XBool initWithoutTex(const char *caption,float width) 
	{
		return initWithoutTex(caption,XEE::systemFont,_XRect(0.0f,0.0f,width,32.0f));
	}

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
protected:
	void draw();								//��水ť
	void drawUp();							
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);
	void setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass = NULL);
	void setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass = NULL);
	void setTexture(const _XButtonTexture& tex);

	_XBool setACopy(const _XButton &temp);

	_XButton();
	~_XButton(){release();}
	void release();
	//������Ϊ������Ч�ʶ��������������
	void setCaptionPosition(const _XVector2& textPosition);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionPosition(float x,float y);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionSize(const _XVector2& size);						//���ð�ť�ı���ĳߴ�
	void setCaptionSize(float x,float y);						//���ð�ť�ı���ĳߴ�
	void setCaptionText(const char *caption);						//���ð�ť�ı��������
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//���ð������ȼ�
	int getHotKey() const;			//��ȡ��ǰ�������ȼ���Ϣ
	void setState(_XButtonState temp);		//���ð�ť��״̬
	_XButtonState getState() const; 
	void disable();
	void enable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XButton(const _XButton &temp);
	_XButton& operator = (const _XButton& temp);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˸�btn���icon������Ľӿڣ�icon������:��ͨicon����Чicon
private:
	_XBool m_withNormalIcon;		//�Ƿ�ӵ����ͨicon
	_XSprite m_normalIcon;
	_XBool m_withDisableIcon;		//�Ƿ�ӵ����Чicon
	_XSprite m_disableIcon;	
	_XVector2 m_iconSize;		//icon�ĳߴ�
	_XVector2 m_iconPosition;	//icon��λ��
public:
	void setNormalIcon(const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void setNormalIcon(const _XSprite &icon);
	void setDisableIcon(const char * filename,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void setDisableIcon(const _XSprite &icon);
	void removeIcon();	//ȥ�����е�icon
	void setIconPosition(float x,float y);	//����icon��λ��
	void setIconSize(float x,float y);	//����icon�����Ŵ�С
	void setIconSize(float x){setIconSize(x,x);}
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	_XMoveData m_actionMoveData;	//��̬Ч���ı���
	_XVector2 m_oldPos;				//��������ʱ��λ��
	_XVector2 m_oldSize;			//��������ʱ�Ĵ�С
	_XMoveData m_lightMD;
	_XRect m_lightRect;
public:
protected:
	void update(int stepTime);
	//---------------------------------------------------------
};
#include "XButton.inl"

#endif