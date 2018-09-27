#ifndef _JIA_XMUTILIST_
#define _JIA_XMUTILIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//������һ�������б�ؼ�����
//ʵ�ֻ�����			x
//ʵ�����ı�����		x
//ʵ����ѡ				x
//ʵ�����ݵ��뵼��		x
//ʵ�ּ����У�������	x
//ʵ�ֲ����У�������	x
//ʵ��ɾ���У�ɾ����	x
//ʵ���ƶ��У��ƶ���	x
//ʵ��ˮƽ���������������ƶ�
//����϶��е��ƶ�		x
#include "XSlider.h"
//#include "XMouseRightButtonMenu.h"
namespace XE{
//sliderĿ¼��Ϊ:/SliderH��/SliderV
//�����Ƕ����б�ؼ�����ͼ
class XMultiListSkin
{
private:
	XBool m_isInited;
	void releaseTex();
public:
	XTextureData *mutiListNormal;		//�����б����ͨ״̬
	XTextureData *mutiListDisable;		//�����б����Ч״̬
	XTextureData *mutiListSelect;		//�����б��б�ѡ���еı���
	XTextureData *mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	XTextureData *mutiListTitle;		//�����б�ı��ⱳ��
	XTextureData *mutiListTitleEnd;	//�����б�ı���ָ���

	XRect m_mouseRect;			//������Ӧ��Χ

	XMultiListSkin();
	~XMultiListSkin(){release();}
	XBool init(const char *normal,const char *disable,const char *select,const char *move,
		const char *title,const char *titleEnd,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};
//���е�һ��Ԫ��
struct XMultiListOneBox
{
	XBool isEnable;				//���Ԫ���Ƿ���Ч
	XBool isShow;				//���Ԫ���Ƿ���Ҫ��ʾ
	XFontUnicode text;			//������ʾ������

	XVec2 order;				//���Ԫ�����ڵ�λ��(Ҳ������һ�У���һ��)
//	char *string;				//���Ԫ���е��ַ���
	std::string textStr;
	int stringLength;			//�ַ����ĳ���
	XMultiListOneBox *nextBox;	//��һ��Ԫ�ص�ָ��
	XMultiListOneBox()
		:nextBox(NULL)
//		,string(NULL)
	{}
};
//���е�һ��
class XMultiListOneRow
{
public:
	XBool isEnable;					//��һ���Ƿ���Ч
	char isShow;					//��һ�����Ƿ���Ҫ��ʾ	0:����ʾ 1:������ʾ 2:����ʾ�ָ��
	char needChip;					//�Ƿ���Ҫ�и0������Ҫ�и1��ǰ���и2�������и�
	float left;						//��ߵ��и�λ��
	float right;					//�ұߵ��и�λ��
	XFontUnicode text;				//������ʾ������

	int order;						//��һ�еı��
	int stringShowWidth;			//��һ�п�����ʾ���ַ��Ŀ��
	XVec2 position;				//��һ�е���Կؼ����Ͻǵ�����λ��
	XVec2 pixSize;				//����һ����λ�����سߴ�,������ش�С��û�о������ŵ�
	int posX;
	int pixLen;						//�еĿ�ȣ������ü���
//	char *title;					//��һ�еı���
	std::string title;
	XMultiListOneRow *nextRow;		//��һ�е�ָ��
	XSprite m_spriteTitleEnd;		//ÿ�н����ı�Ƿ�
	XSprite m_spriteTitle;			//ÿ�еı���
	XMultiListOneRow()
		:nextRow(NULL)
//		,title(NULL)
	{}
};

#define MUTILIST_MIN_WIDTH (64)		//�����б����С���
#define MUTILIST_MAX_ROW_SUM (256)	//����ж�����
#define MUTILIST_TITLE_EXPAND_LENGTH (100)	//���һ��������չ�Ŀ��
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//Ĭ�ϵĻ��������
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (MIN_FONT_CTRL_SIZE)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH

enum XMultiListActionType
{//�����б�Ķ�������
	MLTLST_ACTION_TYPE_IN,		//ѡ�����
	MLTLST_ACTION_TYPE_MOVE,	//ѡ���ƶ�
	MLTLST_ACTION_TYPE_DCLICK,	//˫��
	MLTLST_ACTION_TYPE_OUT,		//ȡ��ѡ��
};
class XMultiList:public XControlBasic
{
private:
	XBool m_isInited;				//�Ƿ��ʼ��

	const XTextureData *m_mutiListNormal;	//�����б����ͨ״̬
	const XTextureData *m_mutiListDisable;	//�����б����Ч״̬
	const XTextureData *m_mutiListSelect;	//�����б��б�ѡ���еı���
	const XTextureData *m_mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	const XTextureData *m_mutiListTitle;		//�����б�ı��ⱳ��
	const XTextureData *m_mutiListTitleEnd;	//�����б�ı���ָ���

	XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	XSlider m_verticalSlider;			//��ֱ������
	XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	XSlider m_horizontalSlider;			//ˮƽ������
	
	int m_tableRowSum;				//����е�����
	XMultiListOneRow *m_tableRow;	//�е�����
	int m_tableLineSum;				//����е�����
	XMultiListOneBox *m_tableBox;	//����Ԫ�ص�����

	XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_showStartRow;				//��ʾ�ӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������

	XBool m_needShowMove;			//�Ƿ���Ҫ��ʾ�ƶ�������

	XSprite m_spriteBackGround;	//�ؼ��Ĵ󱳾�
	XSprite m_spriteSelect;		//�ؼ���ѡ��(��ʱֻ�ܵ���ѡ��)
	XSprite m_spriteMove;			//����ƶ��Ķ�����

	static void ctrlProc(void *,int,int);

	//������Ϊ��ʵ���������ı�������������ʱ����
	XBool m_mouseLeftButtonDown;		//�������Ƿ���
	int m_startX;					//��꿪ʼ�϶������
	int m_changeRowOrder;			//�϶��еı��
	XMultiListOneRow * m_changeRow;	//�϶��е�ָ��
//	int m_rowDx;					//�϶��е���ʾƫ��λ��

	//������Ϊ��ʵ������϶��ƶ��ж�����ı���
	XBool m_mouseMoveDown;	//����Ƿ��а����϶�
	int m_oldLineOrder;		//��갴�����ڵ��У����ڼ�����Ҫ����ƶ�

public:
	int getLineSum() const {return m_tableLineSum;}
	int getRowSum() const {return m_tableRowSum;}
	void setTitleStr(const char *str);	//ÿ��ʹ��;�ֿ�
	std::string getTitleStr();	//��ȡ����������ַ���
	const std::string& getTitleStr(int order)const;	//��ȡָ��������ַ���
	void setTitleStr(const char *str,int order);		//����ĳ�����������
	void setBoxStr(const char *str,int line,int row);	//����ĳһ����Ԫ�������
	const char *getBoxStr(int line,int row);		//��ȡĳ����Ԫ���е�����
	XBool setRowWidth(int width,int order);		//��������һ�еĿ��
	int getSelectIndex();
	void disSelectIndex()	//ȡ����ѡ
	{
		if (!m_haveSelect) return;
		setAction(MLTLST_ACTION_TYPE_OUT, m_selectLineOrder);
		m_haveSelect = XFalse;
		updateSelectLine();
	}
	//��������Ϊ��׼
	void setSelectIndex(int index)
	{
		if (index < 0 || index >= m_tableLineSum) return;
		m_haveSelect = XTrue;
		m_selectLineOrder = index;
		updateSelectLine();
	}
	enum XMultiListEvent
	{
		MLTLST_SELECT,
		MLTLST_DCLICK,
		MLTLST_LINE_CHANGE,	//�����˻����¼�
	};
private:
//	void (*m_funSelectFun)(void *,int ID);
//	void (*m_funDClick)(void *,int ID);	//���˫��ʱ����Ӧ�ĺ���
///	void * m_pClass;
	XBool m_withMouseDrag;	//�Ƿ�֧������϶�
	int m_mDragA;	//���뻻�е������к�
	int m_mDragB;
public:
	int getMDragA()const { return m_mDragA; }
	int getMDragB()const { return m_mDragB; }
//	void setSelectFun(void (* funSelectFun)(void *,int),void * pClass);
//	void setDClickFun(void (* funDClick)(void *,int),void * pClass);
	void setWithMouseDrag(XBool flag) {m_withMouseDrag = flag;}
private:
	void updateShowChange();					//�����б��ı仯��Ϣ�����б���еı�����ʾ���
	int m_showPixWidth;							//��ʾ��������ؿ��
	int m_showPixHight;							//��ʾ��������ظ߶�
	void updateSliderState();					//���ݱ�����������»�������״̬
	void updateSelectLine();					//����ѡ���е�״̬
	void initANewRowData(XMultiListOneRow * upRow,int i);	//��ʼ��һ���µı���
	void initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j);	//��ʼ��һ���б�Ԫ�ص�����

	XFontUnicode m_caption;
	XVec2 m_fontSize;
	XFColor m_textColor;			//���ֵ���ɫ
	float m_curTextWidth;			//��ǰ��������
	float m_curTextHeight;			//��ǰ������߶�

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
public:
	XBool init(const XVec2& position,		//�ռ����ڵ�λ��
		const XRect& Area,					//�ؼ���ʵ����ʾ����
		const XMultiListSkin &tex,		//�ؼ�����ͼ
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	//��ֱ������
		const XSlider &hSlider);	//ˮƽ������
	XBool initEx(const XVec2& position,		//������ӿڵļ�
		const XMultiListSkin &tex,		
		const XFontUnicode& font,			
		float strSize,						
		int rowSum,					
		int lineSum,				
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	
		const XSlider &hSlider);
	XBool initPlus(const char * path,		//�ؼ�����ͼ
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum);
	XBool initWithoutSkin(const XRect& area,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutSkin(area,getDefaultFont(),1.0f,rowSum,lineSum);
	}
	XBool initWithoutSkin(const XVec2& pixelSize,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),
			getDefaultFont(),1.0f,rowSum,lineSum);
	}
protected:
	void draw();					//��溯��
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& p);
	void setLostFocus();
public:
	XBool exportData(const char *fileName = NULL);			//���ݵ���
	XBool importData(const char *fileName = NULL);			//���ݵ���
	XBool setRowSum(int rowSum);		//��������������������հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	XBool setLineSum(int lineSum);	//���������������Ĳ�����հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	XBool deleteLine(int order);		//ɾ��ĳһ��
	XBool deleteRow(int order);		//ɾ��ĳһ��
	XBool insertALine(int order);		//��order������һ��
	XBool insertARow(int order);		//��order������һ��
	XBool moveDownLine(int order);	//��order������
	XBool moveRightRow(int order);	//��order������

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);			//���óߴ�

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	XMultiList();
	~XMultiList(){release();}
	void release();
private:
	void releaseTempMemory();	//�ͷŲ��������ڴ�ռ�
public:
	//���涨�������������
	XBool addALine();					//��ĩβ����һ��
	XBool addARow();					//��ĩβ����һ��
	XBool deleteSelectLine();			//ɾ��ѡȡ��һ��
	XBool moveUpLine(int order);		//��order������
	XBool moveLeftRow(int order);		//��order������
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XMultiList(const XMultiList &temp);
	XMultiList& operator = (const XMultiList& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMultiListActionType m_actionType;
	XMoveData m_actionMD;
	int m_actionPosition;	//��ǰ������λ��
	void setAction(XMultiListActionType type,int index);
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XMultiList.inl"
#endif
}
#endif