#ifndef _JIA_XSLIDER_
#define _JIA_XSLIDER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
#include "../XOprateDes.h"
#include "../XXml.h"
namespace XE{
enum XSliderType
{
	SLIDER_TYPE_VERTICAL,		//��ֱ��
	SLIDER_TYPE_HORIZONTAL		//ˮƽ��
};
enum XSliderState
{
	SLIDER_STATE_NORMAL,		//��ͨ״̬
	SLIDER_STATE_DOWN,			//��갴��
	SLIDER_STATE_ON,			//�������
	SLIDER_STATE_DISABLE		//��Ч״̬
};
class XSliderSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *sliderLineNormal;			//�������ߵ���ͨ״̬
	XTextureData *sliderLineDown;				//�������ߵİ���״̬
	XTextureData *sliderLineOn;				//�������ߵ�����״̬
	XTextureData *sliderLineDisable;			//�������ߵ���Ч״̬
	XTextureData *sliderButtonNormal;			//��������ť����ͨ״̬
	XTextureData *sliderButtonDown;			//��������ť�İ���״̬
	XTextureData *sliderButtonOn;				//��������ť������״̬
	XTextureData *sliderButtonDisable;			//��������ť����Ч״̬

	XRect m_mouseRect;			//������Ӧ��Χ
	XRect m_mouseRectButton;			//������Ӧ��Χ
	XVector2 m_fontPosition;	//�������ֵ�λ��

	XSliderSkin();
	~XSliderSkin(){release();}
	XBool init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
private:
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResourcePosition resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResourcePosition resPos);		//����ҳ�ж�ȡ��Դ
};
class XMultiList;
class XMultiListBasic;
class XMultiText;
class XDirectoryList;
class XSliderEx;
class XImageList;
class XPropertyBox;
class XSlider:public XControlBasic,public XBasicOprate
{
	friend XMultiList;
	friend XMultiListBasic;
	friend XMultiText;
	friend XDirectoryList;
	friend XSliderEx;
	friend XImageList;
	friend XPropertyBox;
private:
	XBool m_isInited;
	XSliderState m_curSliderState;			//��������״̬

	XSliderType m_typeVorH;						//������������

	const XTextureData *m_sliderLineNormal;			//�������ߵ���ͨ״̬
	const XTextureData *m_sliderLineDown;			//�������ߵİ���״̬
	const XTextureData *m_sliderLineOn;				//�������ߵ�����״̬
	const XTextureData *m_sliderLineDisable;			//�������ߵ���Ч״̬
	const XTextureData *m_sliderButtonNormal;		//��������ť����ͨ״̬
	const XTextureData *m_sliderButtonDown;			//��������ť�İ���״̬
	const XTextureData *m_sliderButtonOn;			//��������ť������״̬
	const XTextureData *m_sliderButtonDisable;		//��������ť����Ч״̬
public:
	enum XSliderEvent
	{
		SLD_INIT,
		SLD_RELEASE,
		SLD_MOUSE_ON,
		SLD_MOUSE_DOWN,
		SLD_MOUSE_UP,
		SLD_MOUSE_MOVE,
		SLD_VALUE_CHANGE,
	};
private:
	XRect m_buttonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
	//XRect m_curButtonArea;		//��ǰ������Ӧ��Χ
	XSprite m_buttonSprite;	//������ʾ��ͼ�ľ���
	XSprite m_lineSprite;		//������ʾ��ͼ�ľ���

	XBool m_needFont;						//�Ƿ���Ҫ����
	XFontUnicode m_caption;				//����
	std::string m_fontString;//��ʽ���ַ���
	XVector2 m_textPosition;				//���ֵ����λ��
	XVector2 m_textSize;					//���ֵĳߴ�
	XFColor m_textColor;					//���ֵ���ɫ
	//�����ǹ��ڹ��������Ľӿ�
	float *m_pVariable;	//�����ı���
public:
	void setConnectVar(float * p) 
	{
		m_pVariable = p;
		setCurValue(*p);
	}	//��������
	void disConnectVar() {m_pVariable = NULL;}			//ȡ����������
private:
	XVector2 m_upMousePoint;		//�ϴ�����ڹ�������λ��
	XVector2 m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ
	XVector2 m_curButtonPosition;//��ǰ���鰴ť��λ��
	float m_minValue;			//�����������ֵ
	float m_maxValue;			//����������Сֵ
	float m_curValue;			//�������ĵ�ǰֵ
	float m_upValue;			//����������һ��ֵ(�������������Ŀ����������϶����鳬����Χ֮����Իָ����϶�֮ǰ��֪������Ŀǰ���ڼ򵥵Ŀ��ǣ������������)
	XBool m_dataChanged;			//����������ֵ�Ƿ��б��޸�
	float m_keyOneValue;		//����һ�ΰ����Ļ�����(ע�����ֵ����Ҫ>=0,���򽫻���ɴ���)
	void updateButtonData();	//����ʵʱ���ݵı仯���»������Ϣ

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ

	void setTexture(const XSliderSkin &tex);
public:
	XBool init(const XVector2& position,	//�ؼ����ڵ�λ��
		const XRect& Area,				//�ؼ��������Ӧ��Χ
		const XRect& buttonArea,		//������������Ӧ��Χ(�������ʵ����û��������)
		const XSliderSkin &tex,float max = 100.0f,float min = 0.0f,XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XBool initEx(const XVector2& position,	//������ӿڵļ�
		const XSliderSkin &tex,float max = 100.0f,float min = 0.0f,XSliderType type = SLIDER_TYPE_HORIZONTAL);
	XBool initPlus(const char * path,float max = 100.0f,float min = 0.0f,XSliderType type = SLIDER_TYPE_HORIZONTAL,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	//narrowBtn�Ƿ񻬶���Ϊխ�ߣ�խ��ֻ����ͨ��һ���С
	//int initWithoutSkin(const XRect& area,const XRect& buttonArea,XSliderType type,float max,float min,const XVector2 &fontPosition);
	XBool initWithoutSkin(const XRect& area,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVector2 &fontPosition = XVector2(0.0f,16.0f),bool narrowBtn = false);
	XBool initWithoutSkin(const XVector2& pixelSize,float max = 100.0f,float min = 0.0f,
		XSliderType type = SLIDER_TYPE_HORIZONTAL,const XVector2 &fontPosition = XVector2(0.0f,16.0f),bool narrowBtn = false)
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),max,min,type,fontPosition,narrowBtn);
	}

	XBool setFont(const char *caption,const XFontUnicode &font,float captionSize = 1.0f,const XVector2 &fontPosition = XVector2(0.0f,16.0f));
	XBool setFont(const char *caption,const XVector2 &fontPosition)
	{
		return setFont(caption,getDefaultFont(),1.0f,fontPosition);
	}
	XBool setFont(const char *caption)
	{
		return setFont(caption,getDefaultFont(),1.0f,m_textPosition);
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
//	using XControlBasic::getPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//���û�������λ��

	using XObjectBasic::setScale;		//���⸲�ǵ�����
//	using XControlBasic::getSize;	//���⸲�ǵ�����
	void setScale(float x,float y);				//���û����������ű���

	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
//	using XControlBasic::getColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
protected:
	void draw();							//��滬����
	void drawUp();
	XBool mouseProc(float x,float y,XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return !(m_curSliderState == SLIDER_STATE_DOWN);}
	void setLostFocus();	//����ʧȥ����
	float getMapValue(float v);
public:
	void setCurValue(float temp,bool cbFun = true);			//���û��鵱ǰ��ֵ,cbFun�Ƿ񴥷���ֵ�仯�Ļص�����
	void setRange(float max,float min);

	XBool setACopy(const XSlider &temp);			//����һ������

	XSlider();
	~XSlider(){release();}
	void release();
	//��������
	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	float getCurValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const; 
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XSlider(const XSlider &temp);
	XSlider& operator = (const XSlider& temp);
public:
	virtual void setOprateState(void * data);
	virtual void *getOprateState() const;
	virtual void releaseOprateStateData(void *p);
	virtual bool isSameState(void * data);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMoveData m_actionMoveData;	//��̬Ч���ı���
	XVector2 m_oldPos;				//��������ʱ��λ��
	XVector2 m_oldSize;			//��������ʱ�Ĵ�С
	XMoveData m_lightMD;
	XRect m_lightRect;
protected:
	void update(float stepTime);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(getCurValue()))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		float tmp;
		if(XXml::getXmlAsFloat(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setCurValue(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XSlider.inl"
#endif
}
#endif