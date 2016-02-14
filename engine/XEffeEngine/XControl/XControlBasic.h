#ifndef _JIA_XCONTROLBASIC_
#define _JIA_XCONTROLBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "../XObjectBasic.h" 
#include "XComment.h"
#include "../XFrameWork.h"
class TiXmlNode;
namespace XE{
#define MIN_FONT_CTRL_SIZE (XEG.m_windowData.systemFontSize + 2.0f)

//�ؼ���ģʽ
enum XCtrlMode
{
	CTRL_MODE_SIMPLE,	//��ģʽ��ֻ����Ҫ�Ĳ���
	CTRL_MODE_NORMAL,	//��׼ģʽ����泣�õ���Ϣ
	CTRL_MODE_COMPLETE,	//���ģʽ��������е���Ϣ
};
enum XCtrlObjType
{
	CTRL_OBJ_NULL,		//��Ч�����
	CTRL_OBJ_BUTTON,	//��ť���
	CTRL_OBJ_BUTTONEX,	//��ť���
	CTRL_OBJ_CHECK,		//��ѡ������
	CTRL_OBJ_EDIT,		//���������
	CTRL_OBJ_MOUSERIGHTBUTTONMENU,	//����Ҽ��˵������
	CTRL_OBJ_SLIDER,	//�����������
	CTRL_OBJ_RADIOS,	//��ѡ������
	CTRL_OBJ_PROGRESS,	//�����������
	CTRL_OBJ_MUTITEXT,	//�����ı������
	CTRL_OBJ_MUTILIST,	//�����б��
	CTRL_OBJ_COMBO,		//�����б������
	CTRL_OBJ_DIRECTORYLIST,	//·���б��
	CTRL_OBJ_GROUP,		//Ⱥ���
	CTRL_OBJ_POINTCTRL,		//��ԭ
	CTRL_OBJ_LINECTRL,		//��ԭ
	CTRL_OBJ_SLIDEREX,	//�����������
	CTRL_OBJ_PASSWORDPAD,	//���������
	CTRL_OBJ_IMAGELIST,		//ͼ����п�
	CTRL_OBJ_CHART,		//ͼ���
	CTRL_OBJ_SIMPLECHART,		//������ͼ��
	CTRL_OBJ_TAB,			//��ǩ�ؼ�
	CTRL_OBJ_TEXT,			//���ֿؼ�
	CTRL_OBJ_SIMPLELINE,	//�ָ��߿ؼ�
	CTRL_OBJ_PROGRESSRING,	//�����������
	CTRL_OBJ_CALENDAR,		//�����ؼ�
	CTRL_OBJ_MENU,			//�˵���Ŀؼ�
	CTRL_OBJ_IMAGE,		//ͼƬ�Ŀؼ�
	CTRL_OBJ_BTNBAR,		//��ť���Ŀؼ�
	CTRL_OBJ_TOOLBAR,		//�������Ŀؼ�
	CTRL_OBJ_PROPERTYBOX,	//���Կ�Ŀؼ�
	CTRL_OBJ_COLORCHOOSE,	//��ɫѡ��Ŀؼ�
	CTRL_OBJ_SUBWINDOW,	//�Ӵ��ڿؼ�
	CTRL_OBJ_SOFTBOARD,	//����̿ؼ�
	CTRL_OBJ_PARAMCTRL,	//�����ؼ�
	CTRL_OBJ_FUNCTION,	//��ͼ����
};
enum XCtrlSpecialType
{
	CTRL_TYPE_NORMAL,	//��ͨ�ؼ�
	CTRL_TYPE_KYBOARD,	//���м����¼������ȴ�����
	CTRL_TYPE_MOUSE,	//��������¼������ȴ�����
};
//ͨ���ؼ����ͷ��ؿؼ��������磺CTRL_OBJ_BUTTON����XButton
namespace XCtrl
{
	inline std::string getCtrlNameByType(XCtrlObjType type);
	inline std::string getCtrlTypeByString(XCtrlObjType type);
	inline XCtrlObjType getCtrlTypeByString(const std::string &name);
}
//˵����������Ԫ��Ŀ����Ϊ���ÿؼ����������Է��ʿؼ��Ļ�ͼ��������Ϣ��������
//���ǿؼ���ʵ�屻����֮�󣬶���ĳ���Ա�����Լ�������Щ��������ֹ�ظ���ͼ����
//������Ϣ����������Ԫ�������������ṹ��Ƶ���϶Ⱥ͸��Ӷȣ�������Ҫ˼����
//�õĽ����ʽ
class XCtrlManagerBase;
class XToolBar;
class XTab;
class XPropertyLine;
class XSubWindow;
//�ؼ���Ļ���
class XControlBasic:public XObjectBasic
{
	friend XCtrlManagerBase;
	friend XToolBar;
	friend XTab;
	friend XPropertyLine;
	friend XSubWindow;
protected:
	XCtrlSpecialType m_ctrlSpecialType;
	XCtrlObjType m_ctrlType;
	XRect m_mouseRect;		//�ؼ��������Ӧ��Χ
	XRect m_curMouseRect;	//��ǰ������Ӧ��Χ�������Χ�ܵ�λ�úʹ�С�ı仯���仯	(���ֵ��û�������еĿؼ���ʹ�ã����Ч���½���2014.4.30�����������)
	XVector2 m_scale;		//�ؼ��Ĵ�С
	XVector2 m_position;	//�ؼ���λ��
	XFColor m_color;		//�ؼ�����ɫ

	XBool m_isEnable;		//�ؼ��Ƿ���Ч����Ч��������Կ���������ȴ��ʾΪ���ܲ�����ģʽ
	XBool m_isVisible;		//�ؼ��Ƿ�ɼ����ɼ�������Ż���ʾ����
	XBool m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�,ͬʱ���������屻����
	XBool m_isBeChoose;		//�ؼ��Ƿ�ѡ�У����ռ��̲�����������table��ѡ��(��ʱ��δʵ��)������ͬʱֻ��һ������ܳ�Ϊ����
	int m_objectID;			//�ؼ���ID
	XBool m_withAction;	//�Ƿ�ʹ�ÿؼ��Ķ�̬Ч��
	XBool m_isInAction;	//�Ƿ����ڴ��ڶ���������

	XComment m_comment;	//ע��
	XBool m_isMouseInRect;	//����Ƿ��ڷ�Χ��
public:
	XVector2 getPixelSize() const {return XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight());}
	XCtrlObjType getCtrlType()const{return m_ctrlType;}
	void setWithAction(XBool flag){m_withAction = flag;}
	XBool getWithAction()const{return m_withAction;}
	XBool getIsInAction()const{return m_isInAction;}
	XRect getMouseRect() const {return m_mouseRect;}
	float getMouseRectWidth() const {return m_mouseRect.getWidth();}
	float getMouseRectHeight() const {return m_mouseRect.getHeight();}
	int getControlID() const {return m_objectID;}	//��ȡ�ؼ���ID
	void setActive(){m_isActive = XTrue;}
	void disActive(){m_isActive = XFalse;}		//���ÿؼ������տ�����Ϣ
	XBool getActive() const {return m_isActive;}
	void setVisible()
	{
		m_isVisible = XTrue;
		m_comment.setVisible();
		updateChildVisible();
	}
	void disVisible()
	{
		m_isVisible = XFalse;
		m_isBeChoose = XFalse;
		m_comment.disVisible();
		updateChildVisible();
	}	//���ÿؼ����ɼ�
	XBool getVisible() const {return m_isVisible;}
	XBool getEnable() const {return m_isEnable;}
	XBool setACopy(const XControlBasic & temp);
	//++++++++++++++++++++++++++++++++++++++++++++++++
	//������ע����صĽӿ�
	void setComment(const char *str) {m_comment.setString(str);}
	void disComment() {m_comment.setString(NULL);}
	void setCommentPos(float x,float y){m_comment.setPosition(x,y);}
	//------------------------------------------------
protected:	//���Ϊ����ĺ������������ⲿ����
	virtual void update(float){;}	//Ĭ��ʲôҲ����
	virtual void draw() = 0;
	virtual void drawUp() = 0;	//���������һ������Ķ������ؼ����������棬�ײ��Ư���㣬Ư����Ҳ�����ϲ��У������Ҽ��˵��������˵���
	virtual XBool mouseProc(float x,float y,XMouseState mouseState) = 0;	//�����Ӧ����,������������ķ���ֵ�����壬�����ռ���Ķ����в�ͳһ���з��ؿؼ��Ƿ���״̬����ģ��ȵ�
	virtual XBool keyboardProc(int keyOrder,XKeyState keyState) = 0;		//������Ӧ����
	virtual void insertChar(const char * ch,int len) = 0;

	virtual XBool canBeChoose()
	{
		return m_isActive &&		//û�м���Ŀؼ������տ���
			m_isVisible &&		//������ɼ�ֱ���˳�
			m_isEnable;		//�����Ч��ֱ���˳�
	}
	virtual XBool canGetFocus(float x,float y) = 0;	//�Ƿ���Ի�ý���
	virtual XBool canLostFocus(float x,float y) = 0;	//��ǰ�ؼ��Ƿ����ʧȥ���㣬���ڴ�������������
	virtual void setLostFocus() {m_isBeChoose = XFalse;}	//����ʧȥ����
	virtual void setFocus() {m_isBeChoose = XTrue;}			//����Ϊ����
public:
	virtual XBool isFocus() {return m_isBeChoose;}		//�Ƿ��ڽ���
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
//	virtual void setPosition(const XVector2& position) {setPosition(position.x,position.y);}
//	virtual void setPosition(float x,float y) = 0;
	virtual XVector2 getPosition() const {return m_position;}

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	virtual XVector2 getScale() const {return m_scale;}

	using XObjectBasic::setColor;	//���⸲�ǵ�����
//	virtual void setColor(float r,float g,float b,float a) = 0;		//+Child����
//	virtual void setColor(const XFColor& color) {setColor(color.fR,color.fG,color.fB,color.fA);}
	virtual XFColor getColor() const {return m_color;}	//��ȡ�ؼ��������ɫ

	float getAngle() const {return 0.0f;}				//��ȡ����ĽǶ�
	void setAngle(float){updateChildAngle();}					//��������ĽǶ�

	XControlBasic();
	virtual ~XControlBasic() {};
private:	//Ϊ�˷�ֹ������õĴ����������ظ�ֵ�������͸�ֵ���캯��
	XControlBasic(const XControlBasic &temp);
	XControlBasic& operator = (const XControlBasic& temp);
protected:
	void *m_pClass;
	void (*m_eventProc)(void *,int ID,int eventID);
public:
	virtual void setEventProc(void (* eventProc)(void *,int,int),void *pClass = NULL)
	{
		m_eventProc = eventProc;
		if(pClass != NULL) m_pClass = pClass;
		else m_pClass = this;
	}
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı��棨���������δ��ȫʵ�֣�
protected:
	XBool m_needSaveAndLoad;	//�Ƿ���Ҫ������߶�ȡ
	std::string m_ctrlName;		//�ÿؼ�������
public:
	virtual void setNeedSaveAndLoad(bool flag) {m_needSaveAndLoad = flag;}
	virtual XBool getNeedSaveAndLoad() const {return m_needSaveAndLoad;}
	virtual void setCtrlName(const std::string& name){m_ctrlName = name;}
	virtual std::string getCtrlName() const {return m_ctrlName;}
	virtual XBool saveState(TiXmlNode &)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		return XTrue;
	}
	//---------------------------------------------------------
};
/*
//��Ҫʵ�ֵĿؼ���������һЩ
1����ť		Button			x
2�����������	Edit		x	(Ŀǰ�����ܽ���������������)
3��������	Progress		x
4��������	Slider			x
5�������б�� MutiList		x
6�������б�� MutiText		x
7����ѡ��ť	Radio			x
8����ѡ��ť Check			x
9�������˵�	Combo			x
10��Ⱥ���	Group			x
11��ҳ�棨��ǩ��ť�� Tab
12�����������	MutiEdit		(Ŀǰ�����ܽ���������������)
13���ؼ�������� ����ͳһ�������пռ�ֱ�ӵ�ID�Լ����ڻ����ϵ
*/
#if WITH_INLINE_FILE
#include "XControlBasic.inl"
#endif
}
#endif