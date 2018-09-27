#ifndef _JIA_XMUTITEXT_
#define _JIA_XMUTITEXT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//����һ��������ʾ�����ı����࣬�����мƻ�����༭���ܣ�����̫���Ӿ�û���������ĳ�����

#include "XEdit.h"
#include "XSlider.h"
#include <deque>
#include "XString.h"
#if WITH_LOG
#else
#include "XCritical.h"
#endif
namespace XE{
//sliderĿ¼��Ϊ:/SliderH��/SliderV
typedef XEditSkin XMultiTextSkin;

//���е��ַ���
struct XMultiTextLineString
{
	XBool isEnable;				//��һ���Ƿ�Ҫ��ʾ���Ƿ���Ч
	//std::string showString;		//��һ����������Ҫ��ʾ���ַ���
//	int showStart;				//�����ַ�����ʾ���ֵ�ͷ�ڵ�ǰ�е�ƫ��
//	int showLength;				//��ʾ�ַ����ĳ���
	XFontUnicode curText;		//������ʾ��һ���ַ���������
//	int lineHeadorder;			//��һ���ַ���ȫ���е�ͷλ��
//	int lineEndOrder;			//��һ���ַ���ȫ���е�βλ��
	//int lineLength;				//��һ���ַ����ĳ���
	//char isEnterEnd;			//�Ƿ���Ϊ���ж�������ʾ1����������Ϊ��ʾ�ַ����ĳ������ƶ�����0
	XSprite spriteSelect;		//�ַ�����ʾѡ�񱳾���ɫ�ľ���
	XBool haveAllSelect;			//�������Ƿ�汻����ѡ��

	int lineWidthPixel;			//�е����ؿ��

	XMultiTextLineString()
	//	:showString("")
	{}
};

#define MAX_LINE_STRING_LENGTH (256)	//���п�����ʾ���ַ�������󳤶�
#define MAX_LINE_SUM (256)				//����������������ַ���
//#define MAX_STRING_LENGTH (65536)		//�������������ַ����ĳ���
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//Ĭ�ϵĻ��������
#endif //DEFAULT_SLIDER_WIDTH
enum XMultiTextActionType
{//�����б�Ķ�������
	MLTTXT_ACTION_TYPE_IN,		//ѡ�����
	MLTTXT_ACTION_TYPE_MOVE,	//ѡ���ƶ�
	MLTTXT_ACTION_TYPE_DCLICK,	//˫��
	MLTTXT_ACTION_TYPE_OUT,		//ȡ��ѡ��
};
class XMultiText:public XControlBasic
{
private:
	XBool m_isInited;				//�Ƿ��ʼ��
	XBool m_haveSelect;				//�Ƿ����ַ�������ѡ��

	XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	XSlider m_verticalSlider;		//��ֱ������
	XSlider m_horizontalSlider;	//ˮƽ������

	std::deque<std::string*> m_curStr;	//��ǰʹ�õ��ַ���
	std::deque<std::string*> m_freeStr;	//��ǰ��δʹ�õ��ַ���

	const XTextureData *m_multiEditNormal;	//�������ͨ״̬
	const XTextureData *m_multiEditDisable;	//�������Ч״̬
	const XTextureData *m_multiEditSelect;	//�����Ƭѡ��ɫ
	const XTextureData *m_multiEditUpon;	//�����Ƭѡ��ɫ

	XSprite m_spriteBackGround;	//������ʾ�����ı�����ͼ
	//XVec2 m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	XVec2 m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	XFColor m_textColor;			//���ֵ���ɫ
	float m_textWidth;	//����Ŀ��
	float m_curTextHeight;	//����ĸ߶�

	int m_selectLineOrder;		//��ǰ�ĵ��б�ѡ�������һ��

	XMultiTextLineString *m_lineStr;
	int m_showStartLine;			//��ǰ��ʾ�ĵ�һ�����ܵĵڼ���
	int m_canShowLineSum;		//��ǰ������ʾ������
//	int m_curShowLineWidth;		//��ǰһ�п�����ʾ�Ŀ��
	int m_curMaxLineSum;		//��ǰ�ַ�����ʵ�ʵ�����
//	int m_curMaxLineLength;		//��ǰ�ַ�����ʵ�ʵ��п�
	void updateLineStr();		//���µ�ǰ������ı���ÿһ����

	//������Ϊ��ʵ�������ƶ���������±���
	int m_curStartPixels;			//��ʼ������λ��
	int m_curMaxLineWidthPixels;	//��ǰ�������ؿ��
	int m_maxLineWidthPixels;		//�ܵ�������ؿ��

//	void (*m_funInputChenge)(void *,int);		//�������ݷ����ı��ʱ�����
//	void (*m_funInputOver)(void *,int);		//ȷ���������֮���ʱ�����
//	void *m_pClass;
	
	static void ctrlProc(void*,int,int);

	void setStartLine(int temp);	//���ô�����ʾ��ͷ�е��к�
	void checkStr();							//����ַ����ĵ��п���Ƿ��������,���������Ҫ�����ڽ��ַ�������Ĭ�ϵĿ�Ȳ��Ƴ���Ҫ����ʽ,�Ա����Ժ���ַ�������

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
	char m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ����
public:
	enum XMultiTextEvent
	{
		MLTTXT_SELECT,
	};
//	void (*m_funSelect)(void *,int ID);	//������ѡʱ����
//	void *m_pClass;
public:
	XBool init(const XVec2& position,	//�ؼ����ڵ�λ��
		const XRect& Area,	//�ؼ��ķ�Χ
		const XMultiTextSkin& tex,	//�ؼ�����ͼ
		const char *str,const XFontUnicode& font, float strSize,		//�ؼ������弰�����Ϣ
		//const XVec2& strPosition,
		//const XMouseRightButtonMenu &mouseMenu,
		const XSlider &vSlider,const XSlider &hSlider);	//�ؼ����������������ؼ�
	XBool initEx(const XVec2& position,	//������ӿڵļ�
		const XMultiTextSkin& tex,	
		const char *str,const XFontUnicode& font, float strSize,
		const XSlider &vSlider,const XSlider &hSlider);
	XBool initPlus(const char *path,
		const char *str,const XFontUnicode& font, float strSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const char *str,const XFontUnicode& font, float strSize = 1.0f);
	XBool initWithoutSkin(const XRect& area,const char *str)
	{
		return initWithoutSkin(area,str,getDefaultFont(),1.0f);
	}
	XBool initWithoutSkin(const XVec2& pixelSize,const char *str)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),
			str,getDefaultFont(),1.0f);
	}

//	void setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass);
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);					//�����Ƿ������������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& p);
	void setLostFocus();
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);	//�ı�ռ��λ��

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);		//�ı�ؼ��ĳߴ�

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	//���漸��������Ҫ��֤�ַ������ݵĺϷ���
private:
	void clearAllStrData();	//������е��ַ���
	bool addAStrData(const char * str);	//���һ������,�����Ƿ����˷���
	XCritical m_mutex;	//������Ϊ�˶��̰߳�ȫ���������
public:
	void setString(const char *str);	//�ı���ʾ���ַ���
	const std::string getString()const	//�����������ɽϴ����������
	{
		std::string tmpStr = "";
		for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
		{
			tmpStr += (*(*it)) + "\n";
		}
		return tmpStr;
	}
	//��ȡָ���е��ַ���
	const char *getALineString(int index)const
	{
		if(index < 0 || m_curStr.size() <= index) return XString::gNullStr.c_str();
		else return m_curStr[index]->c_str();
	}
	bool setALineString(const char *str,int index)
	{//���������δ�����ϸ����
		if(str == NULL) return false;
		if(index < 0 || m_curStr.size() <= index) return false;
		if(*m_curStr[index] == str) return true;	//��ͬ���ַ�������Ҫ�滻
		*m_curStr[index] = str;
		checkStr();
		updateLineStr();
		return true;
	}
	void addString(const char *str);			//���ַ���������ַ���
	void addALine(const char *str);			//���ַ���������ַ���

	XBool setACopy(const XMultiText &temp);
	
	XBool exportData(const char *fileName = NULL);		//���ݵ���
	XBool importData(const char *fileName = NULL);		//���ݵ���
	XBool deleteSelectLine();						//ɾ����ǰѡ���һ��
	XBool moveUpSelectLine();						//��ѡ���һ������
	XBool moveUpLine(int order);					//��ĳһ������
	XBool moveDownSelectLine();					//��ѡ���һ������
	XBool moveDownLine(int order);				//��ĳһ������
	XBool insertALine(const char *str,int lineOrder);	//���ı���ָ���в���һ��

	XMultiText();
	~XMultiText(){release();}
	//��������������
	void enable();		//ʹ����Ч
	void disable();		//ʹ����Ч
	void release();
	int getSelectLine() const;			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
	int getLineSum() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XMultiText(const XMultiText &temp);
	XMultiText& operator = (const XMultiText& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMultiTextActionType m_actionType;
	XMoveData m_actionMD;
	int m_actionPosition;	//��ǰ������λ��
	void setAction(XMultiTextActionType type,int index);
	//---------------------------------------------------------
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XMultiText.inl"
#endif
}
#endif