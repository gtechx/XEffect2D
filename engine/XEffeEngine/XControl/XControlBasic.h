#ifndef _JIA_XCONTROLBASIC_
#define _JIA_XCONTROLBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "../XBasicClass.h"
#include "../XMouseAndKeyBoardDefine.h"
#include "../XObjectBasic.h" 
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "../XNumberEx.h"
#include "XComment.h"

//�ؼ���ģʽ
enum _XCtrlMode
{
	CTRL_MODE_SIMPLE,	//��ģʽ��ֻ����Ҫ�Ĳ���
	CTRL_MODE_NORMAL,	//��׼ģʽ����泣�õ���Ϣ
	CTRL_MODE_COMPLETE,	//���ģʽ��������е���Ϣ
};
enum _XCtrlObjType
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
	CTRL_OBJ_XCHART,		//ͼ���
	CTRL_OBJ_XTAB,			//��ǩ�ؼ�
	CTRL_OBJ_XTEXT,			//���ֿؼ�
	CTRL_OBJ_XSIMPLELINE,	//�ָ��߿ؼ�
	CTRL_OBJ_PROGRESSRING,	//�����������
	CTRL_OBJ_CALENDAR,		//�����ؼ�
	CTRL_OBJ_XMENU,			//�˵���Ŀؼ�
	CTRL_OBJ_XIMAGE,		//ͼƬ�Ŀؼ�
	CTRL_OBJ_BTNBAR,		//��ť���Ŀؼ�
	CTRL_OBJ_XTOOLBAR,		//�������Ŀؼ�
	CTRL_OBJ_FUNCTION,	//��ͼ����
};
//ͨ���ؼ����ͷ��ؿؼ��������磺CTRL_OBJ_BUTTON����_XButton
inline std::string getCtrlNameByType(_XCtrlObjType type);
inline std::string getCtrlTypeByString(_XCtrlObjType type);
inline _XCtrlObjType getCtrlTypeByString(const std::string &name);
//˵����������Ԫ��Ŀ����Ϊ���ÿؼ����������Է��ʿؼ��Ļ�ͼ��������Ϣ��������
//���ǿؼ���ʵ�屻����֮�󣬶���ĳ���Ա�����Լ�������Щ��������ֹ�ظ���ͼ����
//������Ϣ����������Ԫ�������������ṹ��Ƶ���϶Ⱥ͸��Ӷȣ�������Ҫ˼����
//�õĽ����ʽ
class _XControlManager;
class _XToolBar;
class _XTab;
//�ؼ���Ļ���
class _XControlBasic:public _XObjectBasic
{
	friend _XControlManager;
	friend _XToolBar;
	friend _XTab;
protected:
	_XCtrlObjType m_ctrlType;
	_XRect m_mouseRect;		//�ؼ��������Ӧ��Χ
	_XRect m_nowMouseRect;	//��ǰ������Ӧ��Χ�������Χ�ܵ�λ�úʹ�С�ı仯���仯	(���ֵ��û�������еĿؼ���ʹ�ã����Ч���½���2014.4.30�����������)
	_XVector2 m_size;		//�ؼ��Ĵ�С
	_XVector2 m_position;	//�ؼ���λ��
	_XFColor m_color;		//�ؼ�����ɫ

	_XBool m_isEnable;		//�ؼ��Ƿ���Ч����Ч��������Կ���������ȴ��ʾΪ���ܲ�����ģʽ
	_XBool m_isVisible;		//�ؼ��Ƿ�ɼ����ɼ�������Ż���ʾ����
	_XBool m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�,ͬʱ���������屻����
	_XBool m_isBeChoose;		//�ؼ��Ƿ�ѡ�У����ռ��̲�����������table��ѡ��(��ʱ��δʵ��)������ͬʱֻ��һ������ܳ�Ϊ����
	int m_objectID;			//�ؼ���ID
	_XBool m_withAction;	//�Ƿ�ʹ�ÿؼ��Ķ�̬Ч��
	_XBool m_isInAction;	//�Ƿ����ڴ��ڶ���������

	_XComment m_comment;	//ע��
	_XBool m_isMouseInRect;	//����Ƿ��ڷ�Χ��
public:
	_XVector2 getPixelSize() const {return _XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight());}
	_XCtrlObjType getCtrlType()const{return m_ctrlType;}
	void setWithAction(_XBool flag){m_withAction = flag;}
	_XBool getWithAction()const{return m_withAction;}
	_XBool getIsInAction()const{return m_isInAction;}
	_XRect getMouseRect() const {return m_mouseRect;}
	float getMouseRectWidth() const {return m_mouseRect.getWidth();}
	float getMouseRectHeight() const {return m_mouseRect.getHeight();}
	int getControlID() const {return m_objectID;}	//��ȡ�ؼ���ID
	void setActive(){m_isActive = XTrue;}
	void disActive(){m_isActive = XFalse;}		//���ÿؼ������տ�����Ϣ
	_XBool getActive() const {return m_isActive;}
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
	_XBool getVisible() const {return m_isVisible;}
	_XBool getEnable() const {return m_isEnable;}
	_XBool setACopy(const _XControlBasic & temp);
	//++++++++++++++++++++++++++++++++++++++++++++++++
	//������ע����صĽӿ�
	void setComment(const char *str) {m_comment.setString(str);}
	void disComment() {m_comment.setString(NULL);}
	void setCommentPos(float x,float y){m_comment.setPosition(x,y);}
	//------------------------------------------------
protected:	//���Ϊ����ĺ������������ⲿ����
	virtual void update(int){;}	//Ĭ��ʲôҲ����
	virtual void draw() = 0;
	virtual void drawUp() = 0;	//���������һ������Ķ������ؼ����������棬�ײ��Ư���㣬Ư����Ҳ�����ϲ��У������Ҽ��˵��������˵���
	virtual _XBool mouseProc(float x,float y,_XMouseState mouseState) = 0;	//�����Ӧ����,������������ķ���ֵ�����壬�����ռ���Ķ����в�ͳһ���з��ؿؼ��Ƿ���״̬����ģ��ȵ�
	virtual _XBool keyboardProc(int keyOrder,_XKeyState keyState) = 0;		//������Ӧ����
	virtual void insertChar(const char * ch,int len) = 0;

	virtual _XBool canBeChoose()
	{
		return m_isActive &&		//û�м���Ŀؼ������տ���
			m_isVisible &&		//������ɼ�ֱ���˳�
			m_isEnable;		//�����Ч��ֱ���˳�
	}
	virtual _XBool canGetFocus(float x,float y) = 0;	//�Ƿ���Ի�ý���
	virtual _XBool canLostFocus(float x,float y) = 0;	//��ǰ�ؼ��Ƿ����ʧȥ���㣬���ڴ�������������
	virtual void setLostFocus() {m_isBeChoose = XFalse;}	//����ʧȥ����
	virtual void setFocus() {m_isBeChoose = XTrue;}			//����Ϊ����
	virtual _XBool isFocus() {return m_isBeChoose;}		//�Ƿ��ڽ���
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
//	virtual void setPosition(const _XVector2& position) {setPosition(position.x,position.y);}
//	virtual void setPosition(float x,float y) = 0;
	virtual _XVector2 getPosition() const {return m_position;}

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
//	virtual void setSize(const _XVector2& size) {setSize(size.x,size.y);}
//	virtual void setSize(float x,float y) = 0;
//	virtual void setSize(float size) {setSize(size,size);}
	virtual _XVector2 getSize() const {return m_size;}

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
//	virtual void setColor(float r,float g,float b,float a) = 0;		//+Child����
//	virtual void setColor(const _XFColor& color) {setColor(color.fR,color.fG,color.fB,color.fA);}
	virtual _XFColor getColor() const {return m_color;}	//��ȡ�ؼ��������ɫ

	float getAngle() const {return 0.0f;}				//��ȡ����ĽǶ�
	void setAngle(float){updateChildAngle();}					//��������ĽǶ�

	_XControlBasic();
	virtual ~_XControlBasic() {};
private:	//Ϊ�˷�ֹ������õĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XControlBasic(const _XControlBasic &temp);
	_XControlBasic& operator = (const _XControlBasic& temp);
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
#include "XControlBasic.inl"
#endif