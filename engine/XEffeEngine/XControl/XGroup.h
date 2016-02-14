#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XButton.h"
namespace XE{
//Ŀǰ��δʵ�֣����ǽ�����ӵĹ���
//1������������֮�����ڵĳ�Ա״̬���ᱻ�ı�
//2�����ˣ����˵���һ�β�������ֵ��������ܿ��Ի���һֱ�������״̬
//3�����̲��������е����ݶ�����ͨ�����̲��������

//����һ��Ⱥ��Ŀռ䣬�Ƚϼٵ���һ���ؼ�
class XGroup:public XControlBasic
{
public:
	enum XGroupState
	{
		STATE_NORMAL,		//����״̬
		STATE_MINISIZE,		//��С��״̬
	};
private:
	static const int m_groupStateBtnSize = 32;
	XBool m_isInited;
//	XVector2 m_position;	//�ؼ���λ��
//	XVector2 m_scale;		//��С
	XRect m_rect;			//�ؼ��ķ�Χ
	XRect m_drawRect;		//��ǰ�ռ�ķ�Χ
	bool m_withRect;

	XFontUnicode m_caption;
	XVector2 m_textSize;

	XGroupState m_state;
	XButton m_stateBotton;
public:
	enum XGroupEvent
	{
		GRP_STATE_CHANGE,
	};
	void setWithRect(bool flag){m_withRect = flag;}
	bool getWithRect()const{return m_withRect;}
private:
//	void (*m_funStateChange)(void *,int ID);	//״̬�����ı�ʱ���õĺ���
//	void *m_pClass;

	static void ctrlProc(void *,int,int);
	void updateData();		//�����ڲ�����
public:
	XBool init(const XVector2& position,
		const XRect &rect,
		const char *caption,const XFontUnicode &font,float captionSize = 1.0f);
	XBool initWithoutSkin(const XVector2& position,
		const XRect &rect,
		const char *caption) {return init(position,rect,caption,getDefaultFont(),1.0f);}
	XBool initWithoutSkin(const XVector2& position,
		const XVector2 &pixelSize,
		const char *caption) 
	{
		return init(position,XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			caption,getDefaultFont(),1.0f);
	}
	void setState(XGroupState state);
	XGroupState getState(){return m_state;}	//��ȡ�ؼ���״̬
//	void setCallbackFun(void (* funStateChange)(void *,int),
//		void *pClass = NULL);
	void resetSize(const XVector2 &size);
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing
	void setCaptionText(const char *caption);					//���ð�ť�ı��������
protected:
	void draw();
	void drawUp()
	{
	//	if(!m_isInited ||
	//		!m_isVisible) return ;	//���û�г�ʼ��ֱ���˳�
	//	m_stateBotton.drawUp();
	}
	void update(float)
	{
		//m_stateBotton.update(stepTime);
	}
	XBool mouseProc(float x,float y,XMouseState mouseState);		//do nothing
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//do nothing
	XBool canGetFocus(float,float){return XFalse;}	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	XBool canLostFocus(float,float){return XTrue;}	//do nothing
public:
	void drawBG()//���һ��ǳɫ�ı���
	{
		XRender::drawFillBoxA(XVector2(m_drawRect.left,m_drawRect.top),XVector2(m_drawRect.getWidth(),m_drawRect.getHeight()),
			XFColor(m_color.fR * 0.5f,m_color.fG * 0.5f,m_color.fB * 0.5f,m_color.fA * 0.5f),true);
	}
	void setLostFocus() {}							//do nothing
	void disable()
	{
		m_isEnable = XFalse;
		m_isBeChoose = XFalse;
	}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XBool setACopy(const XGroup & temp);

	void setVisible();
	void disVisible();	//���ÿؼ����ɼ�

	XGroup()
		:m_isInited(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_scale(1.0f,1.0f)
		,m_state(STATE_NORMAL)
//		,m_funStateChange(NULL)
//		,m_pClass(NULL)
		,m_withRect(true)
	{
		m_ctrlType = CTRL_OBJ_GROUP;
	}
	~XGroup(){release();}
	//virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XGroup(const XGroup &temp);
	XGroup& operator = (const XGroup& temp);
};
#if WITH_INLINE_FILE
#include "XGroup.inl"
#endif
}
#endif