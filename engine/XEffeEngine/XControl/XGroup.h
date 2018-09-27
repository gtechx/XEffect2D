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
	bool m_withRect;
//	XVec2 m_position;	//�ؼ���λ��
//	XVec2 m_scale;		//��С
	XRect m_rect;			//�ؼ��ķ�Χ
	XRect m_drawRect;		//��ǰ�ռ�ķ�Χ

	XFontUnicode m_caption;
	XVec2 m_textSize;

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
	XBool init(const XVec2& position,
		const XRect& rect,
		const char *caption,const XFontUnicode& font,float captionSize = 1.0f);
	XBool initWithoutSkin(const XVec2& position,
		const XRect& rect,
		const char *caption) {return init(position,rect,caption,getDefaultFont(),1.0f);}
	XBool initWithoutSkin(const XVec2& position,
		const XVec2& pixelSize,
		const char *caption) 
	{
		return init(position,XRect(XVec2::zero,pixelSize),
			caption,getDefaultFont(),1.0f);
	}
	void setState(XGroupState state);
	XGroupState getState(){return m_state;}	//��ȡ�ؼ���״̬
//	void setCallbackFun(void (* funStateChange)(void *,int),
//		void *pClass = NULL);
	void resetSize(const XVec2& size);
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);
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
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//do nothing
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//do nothing
	XBool canGetFocus(const XVec2&){return XFalse;}	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	XBool canLostFocus(const XVec2&){return XTrue;}	//do nothing
public:
	void drawBG()//���һ��ǳɫ�ı���
	{
		XRender::drawFillRectA(m_drawRect.getLT(), m_drawRect.getSize(), m_color * 0.5f, true);
	}
	void setLostFocus() 
	{
		if (!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return;		//�����Ч��ֱ���˳�

		m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
	}							//do nothing
	void disable()
	{
		m_isEnable = XFalse;
		m_isBeChoose = XFalse;
	}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	XBool setACopy(const XGroup & temp);

	void setVisible();
	void disVisible();	//���ÿؼ����ɼ�

	XGroup()
		:m_isInited(XFalse)
//		,m_position(0.0f)
//		,m_scale(1.0f)
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