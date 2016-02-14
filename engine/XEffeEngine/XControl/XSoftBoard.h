#ifndef _JIA_XSOFTBOARD_
#define _JIA_XSOFTBOARD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XButton.h"
#include "XAutoShrinkBase.h"
#include "XDragObjectBase.h"
#include "XControlManager.h"
namespace XE{
enum XSoftBoardType
{//����̵�����
	TYPE_NORMAL,
	TYPE_MINI,
};
class XSoftBoard:public XControlBasic,public XDragObjectBase,public XAutoShrinkBase
{
public:
	static const int m_ctrlGap = 4;	//�ؼ�֮��ļ�϶
	static const int m_numberBtnSum = 10;
	static const int m_letterBtnSum = 26;
	static const int m_symbolBtnSum = 11;
	static const int m_specialBtnSum = 28;
	static const std::string m_numberBtnStrs[m_numberBtnSum];	
	static const std::string m_numberBtnUpStrs[m_numberBtnSum];	
	static const std::string m_letterBtnStrs[m_letterBtnSum];	
	static const std::string m_letterBtnUpStrs[m_letterBtnSum];	
	static const std::string m_symbolBtnStrs[m_symbolBtnSum];	
	static const std::string m_symbolBtnUpStrs[m_symbolBtnSum];	
	static const std::string m_specialBtnStrs[m_specialBtnSum];	
private:
	bool m_isInited;
	bool m_isShiftDown;		//�Ƿ�shift����
	bool m_isCapsDown;		//�Ƿ�caps����
	XSoftBoardType m_softBoardType;

	XE::XButton m_numberBtn[m_numberBtnSum];	
	XE::XButton m_letterBtn[m_letterBtnSum];	
	XE::XButton m_symbolBtn[m_symbolBtnSum];	
	XE::XButton m_specialBtn[m_specialBtnSum];	

	static void btnEventProc(void *,int,int);	//����״̬�İ�ť
	void pressCapsBtn();	//caps����������
	void pressShiftBtn();	//shift����������

	//�Լ������Լ��Ŀؼ�
	XCtrlManagerBase m_ctrlManager;	//����һ���ؼ�������
	void addACtrl(XControlBasic *obj)
	{
		XCtrlManager.decreaseAObject(obj);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(obj);
#endif
		m_ctrlManager.addACtrl(obj);
	}
	void resetKeyState();//�˳�ʱ��Ҫ��ԭ��Щ�Ѿ����µİ���
public:
	void setVisible()
	{
		pressShiftBtn();
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
		resetKeyState();
	}
	bool initWithoutSkin();
protected:
	void draw();
	void drawUp(){}
	void update(float stepTime)
	{
		updateShrinkState(stepTime);
		m_ctrlManager.update(stepTime);
		if(m_isInited && m_isVisible && ((XEE::getCapsLockState() && !m_isCapsDown) || (!XEE::getCapsLockState() && m_isCapsDown)))	//XEE::getCapsLockState() != m_isCapsDown
			pressCapsBtn();
	}
	XBool mouseProc(float x,float y,XMouseState mouseState)		//������궯������Ӧ����
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
		//if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
		m_curMousePosition.set(x,y);
		mouseEventProc(x,y,mouseState);
		m_ctrlManager.mouseProc(x,y,mouseState);
		return isInRect(x,y);
		//return XFalse;
	}
	XBool keyboardProc(int keyOrder,XKeyState keyState)			//�����Ƿ񴥷���������
	{
		return XFalse;
	//	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
	//		!m_isActive ||		//û�м���Ŀؼ������տ���
	//		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	//	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	//	m_ctrlManager.keyProc(keyOrder,keyState);
	//	return XFalse;
	}
	void insertChar(const char *ch,int len){m_ctrlManager.insertChar(ch,len);}
	XBool canGetFocus(float x,float y)				//�����жϵ�ǰ����Ƿ���Ի�ý���
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return m_curMouseRect.isInRect(x,y);
	}
	XBool canLostFocus(float,float){return true;}	//Ӧ���ǿ�����ʱʧȥ�����
public:
	void setSoftBoardType(XSoftBoardType type);	//���ü��̵�����
	XBool setACopy(const XSubWindow &){}	//������ܸ���
	XBool isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	{
		if(!m_isInited) return XFalse;
		return m_curMouseRect.isInRect(x,y);
	}
	XVector2 getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	{
		if(!m_isInited) return XVector2::zero;
		switch(order)
		{
		case 0:return XVector2(m_curMouseRect.left,m_curMouseRect.top);
		case 1:return XVector2(m_curMouseRect.right,m_curMouseRect.top);
		case 2:return XVector2(m_curMouseRect.right,m_curMouseRect.bottom);
		case 3:return XVector2(m_curMouseRect.left,m_curMouseRect.bottom);
		}
		return XVector2::zero;
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	XVector2 getPosition()const{return m_position;}

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
private:
	virtual XBool getIsInDragRect(float x,float y){return m_curMouseRect.isInRect(x,y);}	//�ж�λ���Ƿ�����ק������
	virtual XVector2 correctPos(const XVector2 &pos)	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��
	{
		XVector2 ret = pos;
		if(pos.x < 0.0f) ret.x = 0.0f;
		if(pos.x > getSceneWidth() - m_curMouseRect.getWidth()) ret.x = getSceneWidth() - m_curMouseRect.getWidth();
		if(pos.y < 0.0f) ret.y = 0.0f;
		if(pos.y > getSceneHeight() - m_curMouseRect.getHeight()) ret.y = getSceneHeight() - m_curMouseRect.getHeight();
		return ret;
	}
	virtual XShrinkDirection calShrinkDir()	//���㵱ǰ���ܵ���������
	{
		if(m_isInShrink) return DIR_NULL;	//����Ѵ�������״̬�������ٽ�������
		//������Ҫ���ݴ���λ�ü��㴰�ڿ��ܵ���������
		//if(m_position.x < 2.0f) return DIR_LEFT;
		//1����꿿��
		//2���ؼ�����
		if(m_position.x < 5.0f) return DIR_LEFT;
		if(m_position.x > getSceneWidth() - m_curMouseRect.getWidth() - 5.0f) return DIR_RIGHT;
		if(m_position.y < 5.0f) return DIR_UP;
		if(m_position.y > getSceneHeight() - m_curMouseRect.getHeight() - 5.0f) return DIR_DOWN;
		return DIR_NULL;
	}
	virtual XBool getIsInShrinkRect(){return m_curMouseRect.isInRect(m_curMousePosition);}	//�ж�λ���Ƿ�����ק������
	virtual void updateShrinkPosition()
	{
		switch(m_curShrinkDir)
		{
		case DIR_UP:		//��������
			setPosition(m_position.x,(5.0f - m_curMouseRect.getHeight()) * m_shrinkRate);
			break;
		case DIR_LEFT:		//��������
			setPosition((5.0f - m_curMouseRect.getWidth()) * m_shrinkRate,m_position.y);
			break;
		case DIR_RIGHT:		//��������
			setPosition(getSceneWidth() - 5.0f * m_shrinkRate - m_curMouseRect.getWidth() * (1.0f - m_shrinkRate),m_position.y);
			break;
		case DIR_DOWN:		//��������
			setPosition(m_position.x,getSceneHeight() - 5.0f * m_shrinkRate - m_curMouseRect.getHeight() * (1.0f - m_shrinkRate));
			break;
		}
	}
public:
	XSoftBoard()
		:m_isInited(false)
		,m_isShiftDown(false)
		,m_isCapsDown(false)
		,m_softBoardType(TYPE_MINI)
	{
		m_ctrlType = CTRL_OBJ_SOFTBOARD;
		m_ctrlSpecialType = CTRL_TYPE_MOUSE;
		m_withAutoShrink = true;
	}
	virtual ~XSoftBoard(){release();}
	void release()
	{
		resetKeyState();
		XCtrlManager.decreaseAObject(this);	//ע��������
	#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(this);
	#endif
		m_ctrlManager.clearAllObject();	//������Ҫ������е����
	}
};
}

#endif