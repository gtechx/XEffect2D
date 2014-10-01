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
#include "XMouseRightButtonMenu.h"
//sliderĿ¼��Ϊ:/SliderH��/SliderV
//�����Ƕ����б�ؼ�����ͼ
class _XMultiListTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *mutiListNormal;		//�����б����ͨ״̬
	_XTextureData *mutiListDisable;		//�����б����Ч״̬
	_XTextureData *mutiListSelect;		//�����б��б�ѡ���еı���
	_XTextureData *mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	_XTextureData *mutiListTitle;		//�����б�ı��ⱳ��
	_XTextureData *mutiListTitleEnd;	//�����б�ı���ָ���

	_XRect m_mouseRect;			//������Ӧ��Χ

	_XMultiListTexture();
	~_XMultiListTexture(){release();}
	_XBool init(const char *normal,const char *disable,const char *select,const char *move,
		const char *title,const char *titleEnd,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//���е�һ��Ԫ��
struct _XMultiListOneBox
{
	_XBool isEnable;				//���Ԫ���Ƿ���Ч
	_XBool isShow;				//���Ԫ���Ƿ���Ҫ��ʾ
	_XFontUnicode text;			//������ʾ������

	_XVector2 order;				//���Ԫ�����ڵ�λ��(Ҳ������һ�У���һ��)
	char *string;				//���Ԫ���е��ַ���
	int stringLength;			//�ַ����ĳ���
	_XMultiListOneBox *nextBox;	//��һ��Ԫ�ص�ָ��
	_XMultiListOneBox()
		:string(NULL)
		,nextBox(NULL)
	{}
};

//���е�һ��
struct _XMultiListOneRow
{
	_XBool isEnable;					//��һ���Ƿ���Ч
	char isShow;					//��һ�����Ƿ���Ҫ��ʾ	0:����ʾ 1:������ʾ 2:����ʾ�ָ��
	_XFontUnicode text;				//������ʾ������

	int order;						//��һ�еı��
	int stringShowWidth;			//��һ�п�����ʾ���ַ��Ŀ��
	_XVector2 position;				//��һ�е���Կؼ����Ͻǵ�����λ��
	_XVector2 pixSize;				//����һ����λ�����سߴ�,������ش�С��û�о������ŵ�
	int posX;
	int pixLen;						//�еĿ�ȣ������ü���
	char *title;					//��һ�еı���
	_XMultiListOneRow *nextRow;		//��һ�е�ָ��
	_XSprite m_spriteTitleEnd;		//ÿ�н����ı�Ƿ�
	_XSprite m_spriteTitle;			//ÿ�еı���
	char needChip;					//�Ƿ���Ҫ�и0������Ҫ�и1��ǰ���и2�������и�
	float left;						//��ߵ��и�λ��
	float right;					//�ұߵ��и�λ��
	_XMultiListOneRow()
		:title(NULL)
		,nextRow(NULL)
	{}
};

#define MUTILIST_MIN_WIDTH (80)		//�����б����С���
#define MUTILIST_MAX_ROW_SUM (256)	//����ж�����
#define MUTILIST_TITLE_EXPAND_LENGTH (100)	//���һ��������չ�Ŀ��
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//Ĭ�ϵĻ��������
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (32.0f)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH

enum _XMultiListActionType
{//�����б�Ķ�������
	MLTLST_ACTION_TYPE_IN,		//ѡ�����
	MLTLST_ACTION_TYPE_MOVE,	//ѡ���ƶ�
	MLTLST_ACTION_TYPE_DCLICK,	//˫��
	MLTLST_ACTION_TYPE_OUT,		//ȡ��ѡ��
};
class _XMultiList:public _XControlBasic
{
private:
	_XBool m_isInited;				//�Ƿ��ʼ��

	const _XTextureData *m_mutiListNormal;	//�����б����ͨ״̬
	const _XTextureData *m_mutiListDisable;	//�����б����Ч״̬
	const _XTextureData *m_mutiListSelect;	//�����б��б�ѡ���еı���
	const _XTextureData *m_mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	const _XTextureData *m_mutiListTitle;		//�����б�ı��ⱳ��
	const _XTextureData *m_mutiListTitleEnd;	//�����б�ı���ָ���

	_XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	_XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������
	
	int m_tableRowSum;				//����е�����
	_XMultiListOneRow *m_tableRow;	//�е�����
	int m_tableLineSum;				//����е�����
	_XMultiListOneBox *m_tableBox;	//����Ԫ�ص�����

	_XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_showStartRow;				//��ʾ�ӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������

	_XBool m_needShowMove;			//�Ƿ���Ҫ��ʾ�ƶ�������

	_XSprite m_spriteBackGround;	//�ؼ��Ĵ󱳾�
	_XSprite m_spriteSelect;		//�ؼ���ѡ��(��ʱֻ�ܵ���ѡ��)
	_XSprite m_spriteMove;			//����ƶ��Ķ�����

	friend void funMutiListValueChangeV(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListValueChangeH(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveH(void *pClass,int objectID);

	//������Ϊ��ʵ���������ı�������������ʱ����
	_XBool m_mouseLeftButtonDown;		//�������Ƿ���
	int m_startX;					//��꿪ʼ�϶������
	int m_changeRowOrder;			//�϶��еı��
	_XMultiListOneRow * m_changeRow;	//�϶��е�ָ��
//	int m_rowDx;					//�϶��е���ʾƫ��λ��

	//������Ϊ��ʵ������϶��ƶ��ж�����ı���
	_XBool m_mouseMoveDown;	//����Ƿ��а����϶�
	int m_oldLineOrder;		//��갴�����ڵ��У����ڼ�����Ҫ����ƶ�

public:
	int getLineSum() const {return m_tableLineSum;}
	int getRowSum() const {return m_tableRowSum;}
	void setTitleStr(const char *str);	//ÿ��ʹ��;�ֿ�
	std::string getTitleStr();	//��ȡ����������ַ���
	std::string getTitleStr(int order);	//��ȡָ��������ַ���
	void setTitleStr(const char *str,int order);		//����ĳ�����������
	void setBoxStr(const char *str,int line,int row);	//����ĳһ����Ԫ�������
	char *getBoxStr(int line,int row);		//��ȡĳ����Ԫ���е�����
	_XBool setRowWidth(int temp,int order);		//��������һ�еĿ��
	int getSelectIndex();
private:
	void (*m_funSelectFun)(void *,int ID);
	void (*m_funDClick)(void *,int ID);	//���˫��ʱ����Ӧ�ĺ���
	void * m_pClass;
	_XBool m_withMouseDrag;	//�Ƿ�֧������϶�
public:
	void setSelectFun(void (* funSelectFun)(void *,int),void * pClass);
	void setDClickFun(void (* funDClick)(void *,int),void * pClass);
	void setWithMouseDrag(_XBool flag) {m_withMouseDrag = flag;}
private:
	void updateShowChange();					//�����б��ı仯��Ϣ�����б���еı�����ʾ���
	int m_showPixWidth;							//��ʾ��������ؿ��
	int m_showPixHight;							//��ʾ��������ظ߶�
	void updateSliderState();					//���ݱ�����������»�������״̬
	void updateSelectLine();					//����ѡ���е�״̬
	void initANewRowData(_XMultiListOneRow * upRow,int i);	//��ʼ��һ���µı���
	void initANewBoxData(_XMultiListOneBox * nowBox,_XMultiListOneRow * nowRow,int i,int j);	//��ʼ��һ���б�Ԫ�ص�����

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_nowTextWidth;			//��ǰ��������
	float m_nowTextHeight;			//��ǰ������߶�

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	_XBool init(const _XVector2& position,		//�ռ����ڵ�λ��
		const _XRect& Area,					//�ؼ���ʵ����ʾ����
		const _XMultiListTexture &tex,		//�ؼ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider);	//ˮƽ������
	_XBool initEx(const _XVector2& position,		//������ӿڵļ�
		const _XMultiListTexture &tex,		
		const _XFontUnicode &font,			
		float strSize,						
		int rowSum,					
		int lineSum,				
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const _XSlider &vSlider,	
		const _XSlider &hSlider);
	_XBool initPlus(const char * path,		//�ؼ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum);
	_XBool initWithoutTex(const _XRect& area,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutTex(area,XEE::systemFont,1.0f,rowSum,lineSum);
	}
	_XBool initWithoutTex(const _XVector2& pixelSize,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,rowSum,lineSum);
	}
protected:
	void draw();					//��溯��
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y);
	void setLostFocus();
public:
	_XBool exportData(const char *fileName = NULL);			//���ݵ���
	_XBool importData(const char *fileName = NULL);			//���ݵ���
	_XBool setRowSum(int rowSum);		//��������������������հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	_XBool setLineSum(int lineSum);	//���������������Ĳ�����հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	_XBool deleteLine(int order);		//ɾ��ĳһ��
	_XBool deleteRow(int order);		//ɾ��ĳһ��
	_XBool insertALine(int order);		//��order������һ��
	_XBool insertARow(int order);		//��order������һ��
	_XBool moveDownLine(int order);	//��order������
	_XBool moveRightRow(int order);	//��order������

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);			//���óߴ�

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	_XMultiList();
	~_XMultiList(){release();}
	void release();
private:
	void releaseTempMemory();	//�ͷŲ��������ڴ�ռ�
public:
	//���涨�������������
	_XBool addALine();					//��ĩβ����һ��
	_XBool addARow();					//��ĩβ����һ��
	_XBool deleteSelectLine();			//ɾ��ѡȡ��һ��
	_XBool moveUpLine(int order);		//��order������
	_XBool moveLeftRow(int order);		//��order������
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XMultiList(const _XMultiList &temp);
	_XMultiList& operator = (const _XMultiList& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	_XMultiListActionType m_actionType;
	_XMoveData m_actionMD;
	int m_actionPosition;	//��ǰ������λ��
	void setAction(_XMultiListActionType type,int index);
	//---------------------------------------------------------
};
#include "XMultiList.inl"

#endif