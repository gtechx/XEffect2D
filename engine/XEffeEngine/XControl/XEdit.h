#ifndef _JIA_XEDIT_
#define _JIA_XEDIT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//����һ�������������
//��Ҫ֧�ֵĹ��ܵ�
//1�������ı�����
//2���ı��м����
//3�����ѡ���ı�(�����������ѡ��״̬�ᷴɫ�������ʹ��CPUͼ�����������Ҫʹ��������ɫ��������ʵ��)
//4���ı��滻�����С����ơ�ɾ��
//5�����̿��Ƽ���λ�õĸı�
//6�������ʽ���ı�����

//�ؼ���״̬
//1����Ч״̬
//2��������ʾ״̬
//3������״̬
//4�����ѡ��״̬
//5������Ҽ��ĵ����˵����˵����ݣ����С����ơ�ճ������������Ҫʵ��һ���Ҽ��˵�����

//ע�⣺����ֻ֧��Ӣ������

//С��������	x
//���̲���		x
//������		x
//���Ƭѡ		x
//�Ҽ��˵�		x
//������ճ��	x
//����������ʾ��Χ�Ĵ���	x
//shift + ���������Ƭѡ	x
//�������Լ������Ҽ��˵�	x
//ctrl+c��ctrl+v�Ĳ���		x
//����ΧƬѡ				x
//shift + home				x
//shift + end				x
//����ģʽ					x
//���˫��ȫѡ

#include "XMouseRightButtonMenu.h"
#include "../XXml.h"
namespace XE{
class XEditSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *editNormal;			//�������ͨ״̬
	XTextureData *editDisable;			//�������Ч״̬
	XTextureData *editSelect;			//�����Ƭѡ��ɫ
	XTextureData *editInsert;			//����������
	XTextureData *editUpon;			//�ϲ����ɫ

	XRect m_mouseRect;			//������Ӧ��Χ

	XEditSkin();
	~XEditSkin(){release();}
	XBool init(const char *normal,const char *disable,const char *select,const char *insert,
		const char *upon = NULL,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};

class XDirectoryList;
class XPropertyLine;
class XEdit:public XControlBasic
{
	friend XDirectoryList;
	friend XPropertyLine;
private:
	static const int m_maxInputStringLength = 256;	//������������ַ����ĳ���
	static const int m_textEdgeWidth = 5;			//��������ı߽���
	static char m_copyString[m_maxInputStringLength]; //��һ������Edit�ڲ����õ��ڴ�ռ䣬��Ҫ������ʵ��֮��ĸ���ճ��ʱ���ڴ��������
	XBool m_isInited;	//�Ƿ��ʼ��
	XBool m_haveSelect;		//�Ƿ����ַ�������ѡ��
	XBool m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
	bool m_isPassword;	//�Ƿ�Ϊ��������
	XMouseRightButtonMenu *m_mouseRightButtonMenu;	//����Ҽ��˵�

	XFontUnicode m_caption;			//��ʾ��ǰ������ַ���
//	XFontUnicode m_selectText;			//��ʾ��ѡ�е��ַ���(Ŀǰ���û�б�ʹ�õ�����������)

	char *m_curString;		//��ǰ������ַ���
	char *m_tempCurString;	//����ַ�����Ҫ���ڸ���ʹ��
//	char *m_insertString;	//������ַ���
//	char *m_selectString;	//ѡȡ���ַ���
	int m_curInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	int m_curStringLength;	//��ǰ�����ַ����ĳ���
	int m_selectStart;		//ѡ�����ʼλ��
	int m_selectEnd;		//ѡ��Ľ���λ��

	const XTextureData *m_editNormal;		//�������ͨ״̬
	const XTextureData *m_editDisable;		//�������Ч״̬
	const XTextureData *m_editSelect;		//�����Ƭѡ��ɫ
	const XTextureData *m_editInsert;		//����������
	const XTextureData *m_editUpon;

	XSprite m_spriteBackGround;	//������ʾ�����ı�����ͼ
	XSprite m_spriteSelect;		//������ʾѡ�����ݵı�����ɫ
	XSprite m_spriteInsert;		//������ʾ�������
	XVec2 m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	XVec2 m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	XFColor m_textColor;		//���ֵ���ɫ
	float m_curTextWidth;		//��ǰ����Ŀ�ȣ������ֵ������ĳߴ�ı�ʱ��Ҫ����
	float m_curTextHeight;		//��ǰ����ĸ߶�
public:
	enum XEditEvent
	{
		EDT_INPUT_CHANGE,		//�������ݷ����仯��ʱ�����
		EDT_INPUT_OVER,			//�������ݽ�����ʱ�����
	};
	enum XEditType
	{
		TYPE_STRING,		//�ַ���
		TYPE_UNSIGNED_INT,	//�޷���������
		TYPE_INT,			//������
		TYPE_HEX,			//16������
		TYPE_FLOAT,			//������
	};
	void setEditType(XEditType type){m_editType = type;}
	XEditType getEditType(){return m_editType;}
private:
	XEditType m_editType;	//����������	(��ع�����δʵ�֣���Ҫ�޸�insertChar��keyProc�������ж��Ƿ��ܽ�����ز���)
	XBool isStringPassable()	//�жϵ�ǰ�ַ����Ƿ����Ҫ��
	{
		if(m_curString[0] == '\0') return XTrue;	//���ַ����������
		switch(m_editType)
		{
		case TYPE_STRING:return XTrue;		//�ַ���
		case TYPE_UNSIGNED_INT:return XString::getIsUnsignedInt(m_curString);	//�޷���������
		case TYPE_INT:return XString::getIsInt(m_curString);			//������
		case TYPE_HEX:return XString::getIsHexNumber(m_curString) >= 0;			//16������
		case TYPE_FLOAT:return XString::getIsNumber(m_curString);			//������
		}
		return XFalse;
	}
//	void (*m_funInputChenge)(void *,int);		//�������ݷ����ı��ʱ�����
//	void (*m_funInputOver)(void *,int);		//ȷ���������֮���ʱ�����
//	void *m_pClass;

	int m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	char m_keyShiftState;		//shift������״̬	0����1����
	char m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	char m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	char m_keyCtrlState;		//Ctrl������״̬ 0����1����
	
	int m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	int m_curShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��
	char *m_curShowString;		//��ǰ��ʾ���ַ��� 
	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
	XRect m_selectRect;	//ѡ��ķ�Χ
	int keyJudgement(int keyOrder);	//�����ǰ����жϴ���ĺ��������ڲ�����
public:
	void upDataShowStr();	//������ʾ���Ⱥ͹���λ�ø�����ʾ������
	void upDateInsertShowPosition();	//����ʵ��������¹����ʾ��λ��
	void changeInsertPoint(int sum);	//�ƶ����
	void upDataSelectShow();			//����Ƭѡ����ʾ��Ϣ
	void mouseRightMenuProc();			//�Ҽ��˵��Ĵ���
	void deleteSelectStr();			//ɾ��ѡ�е��ַ������ƶ����

public:
	XBool init(const XVec2& position,		//�ؼ���λ��
		const XRect& Area,					//�ؼ��������Ӧ����
		const XEditSkin &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const XFontUnicode& font,			//�ؼ�������
		float strSize = 1.0f,	//�ؼ���������Ϣ
		XMouseRightButtonMenu * mouseMenu = NULL);		//�ؼ����Ҽ��˵�
	XBool initEx(const XVec2& position,	//����ӿڵļ򻯰汾
		const XEditSkin &tex,			
		const char *str,					
		const XFontUnicode& font,			
		float strSize = 1.0f,	
		XMouseRightButtonMenu * mouseMenu = NULL);
	XBool initPlus(const char * path,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const XFontUnicode& font,			//�ؼ�������
		float strSize = 1.0f,	//�ؼ���������Ϣ
		XMouseRightButtonMenu * mouseMenu = NULL,//�ؼ����Ҽ��˵�
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const XFontUnicode& font,			//�ؼ�������
		float strSize = 1.0f,	//�ؼ���������Ϣ
		XMouseRightButtonMenu * mouseMenu = NULL);
	XBool initWithoutSkin(const XRect& area,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(area,str,getDefaultFont(),1.0f,mouseMenu);
	}
	XBool initWithoutSkin(const XVec2& pixelSize,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),str,getDefaultFont(),1.0f,mouseMenu);
	}
	XBool initWithoutSkin(float width,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		XMouseRightButtonMenu * mouseMenu = NULL)
	{
		return initWithoutSkin(XRect(0.0f, 0.0f, width, MIN_FONT_CTRL_SIZE), str, getDefaultFont(), 1.0f, mouseMenu);
	}
protected:
	void update(float stepTime);
	void draw();					//��ͼ����
	void drawUp();				//���С�˵�
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);							//�����Ƿ������������
	void insertChar(const char * ch,int len);
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& p);
	virtual void setLostFocus(); 
	void setFocus() 
	{//����Ϊ����
		//printf("����Ϊ����\n");
		m_isBeChoose = XTrue;
		setImmPos();
	}	
	void setImmPos();
private:
	//������Ϊ��ʵ�ְ�������֮����������������ı���
	XBool keyProc(int keyOrder);
	int m_curKey;
	int m_curKeyDownTimer;
	int m_curKeyRepTimer;
	XBool m_curKeyDown;
public:
	void release();					//��Դ�ͷź���
	void setString(const char *str);		//����������ַ���

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);		//���ÿؼ���λ��

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);				//���ÿؼ������ű���
	XBool getEdtIsNumber();	//����������Ƿ�Ϊ��ֵ
	int getAsInt();		//�Ż����飺�����޳�ͷ��β�Ŀո���߷Ƿ��ַ��������ݴ���
	long long getAsInt64();		//�Ż����飺�����޳�ͷ��β�Ŀո���߷Ƿ��ַ��������ݴ���
	//XBool getAsBool();
	float getAsFloat();	//�Ż����飺�����޳�ͷ��β�Ŀո���߷Ƿ��ַ��������ݴ���
	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ
	
	XBool setACopy(const XEdit &temp);				//����Ϊһ���������ò�����Դ		

	XEdit();
	~XEdit(){release();}
	//��������������
	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	char *getString() const;				//����������ַ���
	int getSelectLength() const;				//���ѡ���ĳ���
	int getSelectHead() const;				//���ѡȡ��ͷ
	int getSelectEnd() const;				//���ѡȡ��β
	//void setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass = NULL);		//���ûص�����
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}

	void setInputLen(int len);	//���������ĳ���
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ���캯���͸�ֵ������
	XEdit(const XEdit &temp);
	XEdit& operator = (const XEdit& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	//XMoveData m_actionMoveData;	//��̬Ч���ı���
	XVec2 m_oldPos;				//��������ʱ��λ��
	XVec2 m_oldSize;			//��������ʱ�Ĵ�С
	XMoveData m_lightMD;
	XRect m_lightRect;
	//������Ķ���
	XMoveData m_insertActionMD;	//������Ķ���Ч��
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����Ϊ������ṩ������ʾ����
	std::string m_promptStr;	//��ʾ����
	bool m_withPromptStr;
public:
	void setPromptStr(const std::string& str)
	{
		m_withPromptStr = true;
		m_promptStr = str;
	}
	void disPromptStr(){m_withPromptStr = false;}
	const std::string& getPromptStr(){return m_promptStr;}
	bool withPromptStr(){return m_withPromptStr;}
	//---------------------------------------------------------
public:
	void setIsPassword(bool flag);
	bool getIsPassword() const{return m_isPassword;}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),getString())) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		std::string tmp;
		if(XXml::getXmlAsString(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setString(tmp.c_str());
		return XTrue;
	}
	//---------------------------------------------------------
private:
	bool getCopyStrHaveEnd()	//�жϿ����ַ����Ƿ��н�����
	{
		for(int i = 0;i < m_maxInputStringLength;++ i)
		{
			if(m_copyString[i] == '\0')
				return true;
		}
		return false;
	}
	bool stringPlaster();	//�Ӽ��а���ճ������
	void stringCopy()		//�����ַ��������а�
	{//����ģʽʱ��������
		if(!m_haveSelect || m_isPassword) return;//����Ƭѡ��û��Ƭѡ��ʲôҲ����
			//����Ƭѡ������
#ifdef XEE_OS_WINDOWS
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
		XString::setTextToClipboard(m_copyString);
#endif
#ifdef XEE_OS_LINUX
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
#endif
	}
	void chooseAllStr()	//ȫѡ
	{
		m_haveSelect = XTrue;
		m_selectEnd = m_curStringLength;
		m_selectStart = 0;
		m_selectMouseDown = XFalse;
		upDataSelectShow();
	}
};
#if WITH_INLINE_FILE
#include "XEdit.inl"
#endif
}
#endif