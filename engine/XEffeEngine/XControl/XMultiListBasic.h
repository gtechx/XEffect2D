#ifndef _JIA_XMUTILISTBASIC_
#define _JIA_XMUTILISTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.26
//--------------------------------
//����һ�������Ķ����б��ֻʵ����һЩ�����Ĺ���

//#include "XSlider.h"
//#include "XMouseRightButtonMenu.h"
#include "XMultiList.h"
#include "XCheck.h"
namespace XE{
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//Ĭ�ϵĻ��������
#endif //DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_TITLE_HEIGHT
#define DEFAULT_TITLE_HEIGHT (MIN_FONT_CTRL_SIZE)
#endif	//DEFAULT_TITLE_HEIGHT
#ifndef DEFAULT_END_WIDTH
#define DEFAULT_END_WIDTH (8.0f)
#endif	//DEFAULT_END_WIDTH
#ifndef DEFAULT_CHECK_SIZE
#define DEFAULT_CHECK_SIZE (MIN_FONT_CTRL_SIZE)
#endif	//DEFAULT_CHECK_SIZE
class XMultiListBasic:public XControlBasic
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
	XSlider m_verticalSlider;		//��ֱ������
	XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	XSlider m_horizontalSlider;	//ˮƽ������
	
	int m_tableRowSum;				//����е�����(������Ҫ����һ����Ч��������������ĸ���)
	XMultiListOneRow *m_tableRow;	//�е�����
	int m_tableLineSum;				//����е�����(������Ҫ����һ����Ч��������������ĸ���)
	XMultiListOneBox *m_tableBox;	//����Ԫ�ص�����

	XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	XRect m_selectRect;			//ѡ����е��׿�
	XBool *m_haveSelectFlag;			//���ڱ�����е������Ƿ�ѡ��
	XKeyState m_stateOfShiftKey;			//shift������״̬������Ƭѡ

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
	XBool getHaveSelectState(int i);
	int getTableLineSum();
private:
	//--------------------------------------------------------
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	float m_showStartRow;			//��ʾ��ͷһ�е����ؿ��
	int m_canShowLineSum;			//�ؼ�������ʾ������

	XBool m_needShowMove;			//�Ƿ���Ҫ��ʾ�ƶ�������

	XSprite m_spriteBackGround;	//�ؼ��Ĵ󱳾�
	XSprite m_spriteSelect;		//�ؼ���ѡ��(��ʱֻ�ܵ���ѡ��)
	XSprite m_spriteMove;			//����ƶ��Ķ�����

	//static void funMutiListBasicStateChange(void *pClass,int objectID);
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
	enum XMultiListBasicEvent
	{
		MLTLST_SELECT,
		MLTLSTB_CHK_STATE_CHANGE,
	};
//	void (*m_funCheckStateChange)(void *,int ID);	//״̬ѡ�����仯��ʱ����õĺ���
//	void (*m_funSelectChange)(void *,int ID);		//ѡ�����仯ʱ����
//	void *m_pClass;				//�ص������Ĳ���

public:
	int getSelectIndex() const;	//����ѡ��������к�
//	void setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass);
	void clearAllSelect();	//������е�ѡ��

	int getLineSum() const {return m_tableLineSum;}
	int getRowSum() const {return m_tableRowSum;}
	void setTitleStr(const char *str,int order);		//����ĳ�����������
	void setBoxStr(const char *str,int line,int row);	//����ĳһ����Ԫ�������
	XBool setRowWidth(int temp,int order);		//��������һ�еĿ��
private:
	void updateShowChange();					//�����б��ı仯��Ϣ�����б���еı�����ʾ���
	int m_showPixWidth;							//��ʾ��������ؿ��
	int m_showPixHight;							//��ʾ��������ظ߶�
	void updateSliderState();					//���ݱ�����������»�������״̬
	void updateSelectLine();					//����ѡ���е�״̬
	void initANewRowData(XMultiListOneRow * upRow,int i);	//��ʼ��һ���µı���
	void initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j);	//��ʼ��һ���б�Ԫ�ص�����

	XFontUnicode m_caption;
	XVector2 m_fontSize;
	XFColor m_textColor;			//���ֵ���ɫ
	float m_curTextWidth;			//��ǰ��������
	float m_curTextHeight;			//��ǰ������߶�

	XCheck *m_check0;		//�Ƿ�ɼ���״̬ѡ���İ�ť
	XCheck *m_check1;		//�Ƿ�ɱ༭��״̬ѡ���İ�ť
	XBool *m_check0State;		//�Ƿ�ɼ���״̬
	XBool *m_check1State;		//�Ƿ�ɱ༭��״̬
public:
	XBool getCheckState(int order,int lineOrder);
	void setCheckState(int order,int lineOrder,XBool state);

	XBool init(const XVector2& position,		//�ռ����ڵ�λ��
		const XRect& Area,					//�ؼ���ʵ����ʾ����
		const XMultiListSkin &tex,		//�ؼ�����ͼ
		const XCheckSkin &checktex0,		//��ѡ�����ͼ
		const XCheckSkin &checktex1,		//��ѡ�����ͼ
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	//��ֱ������
		const XSlider &hSlider);	//ˮƽ������
	XBool initEx(const XVector2& position,		//����ӿڵļ򻯰汾
		const XMultiListSkin &tex,		
		const XCheckSkin &checktex0,		
		const XCheckSkin &checktex1,		
		const XFontUnicode &font,			
		float strSize,						
		int rowSum,				
		int lineSum,			
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	
		const XSlider &hSlider);
	XBool initPlus(const char *path,		//��ѡ�����ͼ
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	XBool initWithoutSkin(const XRect& area,
		const XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum);
	XBool initWithoutSkin(const XRect& area,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutSkin(area,getDefaultFont(),1.0f,rowSum,lineSum);
	}
	XBool initWithoutSkin(const XVector2& pixelSize,
		int rowSum,					//�ؼ��е�����
		int lineSum)
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			getDefaultFont(),1.0f,rowSum,lineSum);
	}
protected:
	void draw();					//��溯��
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);					//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float x,float y);
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
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);			//���óߴ�

	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

	XMultiListBasic();
	~XMultiListBasic(){release();}
	void release();
private:
	void releaseTempMemory();	//�ͷŲ��������ڴ�ռ�
public:
	//���涨�������������
	XBool addALine();					//��ĩβ����һ��
	XBool addARow();					//��ĩβ����һ��
	XBool deleteCurSelectLine();		//ɾ��ѡȡ��һ��
	XBool deleteSelectLines();		//ɾ�����б�ѡ�е�����
	XBool moveUpLine(int order);		//��order������
	XBool moveLeftRow(int order);		//��order������
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	//virtual void justForTest() {;}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XMultiListBasic(const XMultiListBasic &temp);
	XMultiListBasic& operator = (const XMultiListBasic& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XMultiListBasic.inl"
#endif
}
#endif