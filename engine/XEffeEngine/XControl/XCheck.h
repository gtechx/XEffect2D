#ifndef _JIA_XCHECK_
#define _JIA_XCHECK_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//����һ������ʵ�ָ�ѡ�����
#include "XControlBasic.h"
#include "XResourceManager.h"
#include "../XMath/XMoveData.h"
#include "../XOprateDes.h"
#include "../XXml.h"
namespace XE{
class XCheckSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *checkChoosed;				//ѡ��ťѡ�е���ͼ
	XTextureData *checkDischoose;				//ѡ��ťδѡ�е���ͼ
	XTextureData *checkDisableChoosed;			//��Ч״̬��ѡ��ťѡ�е���ͼ
	XTextureData *checkDisableDischoose;		//��Ч״̬��ѡ��ťδѡ�е���ͼ

	XRect m_mouseRect;			//������Ӧ��Χ
	XVec2 m_fontPosition;	//�������ֵ�λ��

	XCheckSkin();
	~XCheckSkin(){release();}
	XBool init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};
class XRadios;
class XMultiListBasic;
class XDirectoryList;
class XChart;
class XPropertyLine;
class XCheck:public XControlBasic,public XBasicOprate
{
	friend XRadios;
	friend XMultiListBasic;
	friend XDirectoryList;
	friend XChart;
	friend XPropertyLine;
private:
	//static const int m_checkLeftAddLength = 15;	//���ַ��������������س�����Ϊ��Ӧ��Χ��Ϊ��ʹ���Ƹ��Ӻ���
	XBool m_isInited;					//�������Ƿ񱻳�ʼ��
	XFontUnicode m_caption;			//�������ı���

	const XTextureData *m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	const XTextureData *m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	const XTextureData *m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	const XTextureData *m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	XSprite m_sprite;			//������ʾ��ͼ�ľ���
	XVec2 m_textPosition;		//���ֵ����λ��
	XVec2 m_textSize;			//���ֵĳߴ�
	XFColor m_textColor;		//���ֵ���ɫ

	XBool m_state;				//��ѡ���ѡ��״̬
	XRect m_mouseClickArea;	//����������Ӧ��Χ
	XBool m_withCaption;			//�Ƿ�ӵ������
		//�����ǹ��ڹ��������Ľӿ�
	XBool *m_pVariable;	//�����ı���

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
public:
	void setConnectVar(XBool * p) 
	{//��������
		m_pVariable = p;
		setState(*p, true);
	}	
	void disConnectVar() {m_pVariable = NULL;}			//ȡ����������

public:
	XBool getWithCaption() const {return m_withCaption;}
	void setWithCaption(XBool withCaption) {m_withCaption = withCaption;}
	XBool init(const XVec2& position,	//�ؼ���λ��
		const XRect& Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const XCheckSkin &tex,			//�ؼ�����ͼ
		const char *caption,const XFontUnicode& font,float captionSize,	//��ʾ������������Ϣ
		const XVec2& textPosition);		//�����λ��
	XBool initEx(const XVec2& position,	//������ӿڵļ�
		const XCheckSkin &tex,			
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f);
	XBool initPlus(const char * path,			//�ؼ�����ͼ���ļ���·��
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const char *caption,				//����
		const XFontUnicode& font,float captionSize,	//�����Լ��ߴ�
		const XRect& area,const XVec2& captionPosition);	//�ռ�Ĵ�С�Լ����ֵ����λ��
	XBool initWithoutSkin(const char *caption,			//�������һ���ӿڵļ򻯰汾
		const XFontUnicode& font,const XRect& area);
	XBool initWithoutSkin(const char *caption,const XRect& area) {return initWithoutSkin(caption,getDefaultFont(),area);}
	XBool initWithoutSkin(const char *caption,const XVec2& pixelSize) 
	{
		return initWithoutSkin(caption,getDefaultFont(),XRect(XVec2::zero,pixelSize));
	}
	XBool initWithoutSkin(const char *caption) 
	{
		return initWithoutSkin(caption, getDefaultFont(), XRect(XVec2::zero, XVec2(MIN_FONT_CTRL_SIZE)));
	}
protected:
	void draw();
	void drawUp();
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//������궯������Ӧ����(���ض����Ƿ���ɸı�)
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//�Լ��̶�������Ӧ
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);//���ÿؼ���λ��

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c); 	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	void setCaptionText(const char *temp);			//���ø�ѡ�������
	const char *getCaptionString() const {return m_caption.getString();}
	XBool setACopy(const XCheck &temp);			//����һ������
	XCheck();
	virtual ~XCheck()
	{
		release();
		if(gFrameworkData.pOperateManager != NULL)
		gFrameworkData.pOperateManager->decreaseObj(this);
	}
	void release();
	void disable();
	void enable();
	XBool getState() const;
	void setState(XBool temp,bool withEvent = false);
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}
private:	//�������ظ��ƹ��캯���͸�ֵ����������ֹ������õĴ���
	XCheck(const XCheck &temp);
	XCheck& operator = (const XCheck& temp);
public:
	virtual void setOprateState(void * data)
	{
		XBool index = *(XBool *)data;
		setState(index);
	}
	virtual void *getOprateState() const
	{
		XBool *data = XMem::createMem<XBool>();
		*data =  getState();
		return data;
	}
	virtual void releaseOprateStateData(void *p)
	{
		XBool *data = (XBool*)p;
		XMem::XDELETE(data);
	}
	virtual void stateChange()
	{
		if(m_withUndoAndRedo) XOpManager.addAOperate(this);	//�����Ҫ��¼��ǰ״̬�򽫵��ö�������������غ���
		if(m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//������صĻص�����
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(XBool*)data == getState());
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	//XMoveData m_actionMoveData;	//��̬Ч���ı���
	XVec2 m_oldPos;				//��������ʱ��λ��
	XVec2 m_oldSize;			//��������ʱ�Ĵ�С
	XMoveData m_lightMD;
	XRect m_lightRect;
protected:
	void update(float stepTime);
	//---------------------------------------------------------
public:
	enum XCheckEvent
	{
		CHK_INIT,
		CHK_RELEASE,
		CHK_MOUSE_ON,
		CHK_MOUSE_DOWN,
		CHK_MOUSE_UP,
		CHK_STATE_CHANGE,
	};
	enum XCheckStyle
	{
		CHK_STYLE_NORMAL,	//��ģʽ
		CHK_STYLE_BOX,		//���ģʽ
		CHK_STYLE_MINI,		//���Բģʽ
	};
	XCheckStyle getStyle()const{return m_style;}
	void setStyle(XCheckStyle style){m_style = style;}
private:
	XCheckStyle m_style;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(getState())
			return XXml::addLeafNode(e,m_ctrlName.c_str(),true);
		else
			return XXml::addLeafNode(e,m_ctrlName.c_str(),false);
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		bool tmpB;
		if(XXml::getXmlAsBool(e,m_ctrlName.c_str(),tmpB) == NULL) return XFalse;
		setState(tmpB);
		return XTrue;
	}
	//---------------------------------------------------------
};
//namespace XXml
//{
//inline TiXmlNode * getXmlValue(TiXmlNode *node,const char * name,XCheck &chk)
//{//��������֮�������Ȼ��ü��ģ�����Ч�ʻ��ܵ�����
//	bool tmp;
//	TiXmlNode *ret = getXmlAsBool(node,name,tmp);
//	if(ret == NULL) return NULL;
//	chk.setState(tmp);
//	return ret;
//}
//inline XBool addLeafNode(TiXmlNode &pElmParent,const char* pszNode,const XCheck &chk)
//{//��������֮�������Ȼ��ü��ģ�����Ч�ʻ��ܵ�����
//	return addLeafNode(pElmParent,pszNode,(bool)chk.getState());
//}
//};
#if WITH_INLINE_FILE
#include "XCheck.inl"
#endif
}
#endif