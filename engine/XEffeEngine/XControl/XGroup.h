#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XButton.h"

//Ŀǰ��δʵ�֣����ǽ�����ӵĹ���
//1������������֮�����ڵĳ�Ա״̬���ᱻ�ı�
//2�����ˣ����˵���һ�β�������ֵ��������ܿ��Ի���һֱ�������״̬
//3�����̲��������е����ݶ�����ͨ�����̲��������

enum _XGroupState
{
	STATE_NORMAL,		//����״̬
	STATE_MINISIZE,		//��С��״̬
};
#define XGROUP_STATE_BTN_SIZE (32.0f)
//����һ��Ⱥ��Ŀռ䣬�Ƚϼٵ���һ���ؼ�
class _XGroup:public _XControlBasic
{
private:
	_XBool m_isInited;
//	_XVector2 m_position;	//�ؼ���λ��
//	_XVector2 m_size;		//��С
	_XRect m_rect;			//�ؼ��ķ�Χ
	_XRect m_drawRect;		//��ǰ�ռ�ķ�Χ

	_XFontUnicode m_caption;
	_XVector2 m_textSize;

	_XGroupState m_state;
	_XButton m_stateBotton;

	void (*m_funStateChange)(void *,int ID);	//״̬�����ı�ʱ���õĺ���
	void *m_pClass;

	friend void funXGroupStateBtn(void *pClass,int objectID);
	void updateData();		//�����ڲ�����
public:
	_XBool init(const _XVector2& position,
		const _XRect &rect,
		const char *caption,const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initWithoutTex(const _XVector2& position,
		const _XRect &rect,
		const char *caption) {return init(position,rect,caption,XEE::systemFont,1.0f);}
	_XBool initWithoutTex(const _XVector2& position,
		const _XVector2 &pixelSize,
		const char *caption) 
	{
		return init(position,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			caption,XEE::systemFont,1.0f);
	}
	void setState(_XGroupState state);
	_XGroupState getState(){return m_state;}	//��ȡ�ؼ���״̬
	void setCallbackFun(void (* funStateChange)(void *,int),
		void *pClass = NULL);
	void resetSize(const _XVector2 &size);
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
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
	void update(int stepTime)
	{
		//m_stateBotton.update(stepTime);
	}
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//do nothing
	_XBool canGetFocus(float,float){return XFalse;}	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	_XBool canLostFocus(float,float){return XTrue;}	//do nothing
public:
	void setLostFocus() {;}							//do nothing
	void disable()
	{
		m_isEnable = XFalse;
		m_isBeChoose = XFalse;
	}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XBool setACopy(const _XGroup & temp);

	void setVisible();
	void disVisible();	//���ÿؼ����ɼ�

	_XGroup()
		:m_isInited(XFalse)
//		,m_position(0.0f,0.0f)
//		,m_size(1.0f,1.0f)
		,m_state(STATE_NORMAL)
		,m_funStateChange(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_GROUP;
	}
	~_XGroup(){release();}
	//virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XGroup(const _XGroup &temp);
	_XGroup& operator = (const _XGroup& temp);
};
#include "XGroup.inl"

#endif