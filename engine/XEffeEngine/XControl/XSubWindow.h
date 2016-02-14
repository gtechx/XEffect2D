#ifndef _JIA_XSUBWINDOW_
#define _JIA_XSUBWINDOW_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.13
//--------------------------------
#include "XButton.h"
#include "XCtrlManagerBase.h"
#include "XDragObjectBase.h"
#include "XAutoShrinkBase.h"
namespace XE{
//����һ���Ӵ��ڵ��࣬��δʵ�֣�ֻ��Ϊ�˽����������
//˵��
//1��������ʵ�ִ����ڱ߽��Զ������Ĺ���
//2��������ʵ�ִ���ʧȥ����ʱ�Զ��黯�Ĺ���
//3��������ʵ�ִ��ڲ���л��Ĺ���
struct XSubWindowObj
{
	XControlBasic *obj;	//�ؼ���ָ��
	XVector2 pos;
	XVector2 scale;
	//���滹��Ҫ������������(��δ���)
};
class XSubWindow:public XControlBasic,public XDragObjectBase,public XAutoShrinkBase
{
public:
	enum XSubWindowState
	{//�Ӵ��ڵ�״̬
		STATE_NORMAL,	//��ͨ״̬
		STATE_MINISIZE,	//��С��
	};
	enum XSubWindowType
	{
		TYPE_NORMAL,	//��ͨ����
		TYPE_NO_TITLE,	//�ޱ���Ĵ���
		TYPE_TOP,		//��˵Ĵ���
	};
	enum XSubWindowEvent
	{
		SWD_STATE_CHANGE,	//����״̬�����ı�
		SWD_LOSE_FOCUS,		//����ʧȥ����
		SWD_RELEASE,		//����ע��
	};
private:
	XBool m_isInited;			//�Ƿ���г�ʼ��
	XBool m_withTitle;			//�����Ƿ��б���
	XBool m_withBackGround;	//�Ƿ��д��ڱ���
	XSubWindowType m_windowType;
	XSubWindowState m_windowState;
	std::string m_titleString;	//���ڵı���
	XButton m_titleButton[3];	//���ڵ�������ť����С�����رպʹ���

	XRect m_area;	//���ڵķ�Χ
	XFontUnicode m_titleFont;

	std::vector<XSubWindowObj> m_objects;	//���������
	static void ctrlProc(void *,int,int);

	void setMiniSize();
	void setNormalSize();
	void updateMouseRect();

	XCtrlManagerBase m_ctrlManager;	//����һ���ؼ�������
public:
	XSubWindow()
		:m_isInited(XFalse)
		,m_withTitle(XTrue)
		,m_titleString("SubWindow")
		,m_withBackGround(XTrue)
		,m_windowType(TYPE_NORMAL)
		,m_windowState(STATE_NORMAL)
	{
		m_ctrlType = CTRL_OBJ_SUBWINDOW;
	}
	~XSubWindow() {release();}
	void release();
	XBool initWithoutSkin(const XVector2 &area,const char * title);	//��Ƥ���ĳ�ʼ��
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return true;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
	void setFocus();
public:
	XBool setACopy(const XSubWindow &){}	//������ܸ���
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	virtual XVector2 getPosition() const {return m_position;}

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void setTitle(const char * title);
	void changeStage(XSubWindowState state);

	XBool addACtrlObj(XControlBasic *obj);	//�򴰿������һ���ؼ�
	int getObjectIndex(XControlBasic *obj);
private:
	virtual XBool getIsInDragRect(float x,float y);	//�ж�λ���Ƿ�����ק������
	virtual XVector2 correctPos(const XVector2 &pos);	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��

	virtual XShrinkDirection calShrinkDir();	//���㵱ǰ���ܵ���������
	virtual XBool getIsInShrinkRect(){return m_curMouseRect.isInRect(m_curMousePosition);}	//�ж�λ���Ƿ�����ק������
	virtual void updateShrinkPosition();
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(��δ���)
public:
	virtual XBool saveState(TiXmlNode &e);
	virtual XBool loadState(TiXmlNode *e);
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XSubWindow.inl"
#endif
}
#endif