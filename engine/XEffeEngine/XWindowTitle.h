#ifndef _JIA_XWINDOWTITLE_
#define _JIA_XWINDOWTITLE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.19
//--------------------------------
//����һ�����ڱ��������࣬���ڶ������洰�ڱ�����

#include "XControl/XControls.h"

class _XWindowTitle
{
private:
	bool m_isInited;
	friend void ctrProc(void *,int);
	_XButton m_minSizeBtn;
	_XButton m_exitBtn;
	_XButton m_iconBtn;
	_XFontUnicode m_titleFont;
	bool m_isMouseBtnDown;
	_XRect m_titleRect;
	//_XVector2 m_oldMousePos;
	int m_oldPosX;
	int m_oldPosY;
	bool m_withText;	//�Ƿ���ʾ����

	friend void winTitleCtrProc(void *,int);

	void (*m_winExitFun)(void *);	//�˳�ʱ���õĻص�����
	void (*m_winMinFun)(void *);	//��С��ʱ���õĻص�����
	void * m_pClass;
public:
	bool init();
	void move(int stepTime){;}	
	void draw()
	{
		//drawFillBox(_XVector2(0.0f,-32.0f),_XVector2(XEE::windowData.sceneW - 129.0f,32.0f),0.5f,0.5f,0.5f);
		drawFillBox(_XVector2(0.0f,-32.0f),_XFColor(0.35f,0.35f,0.35f,1.0f),
			_XVector2(0.0f + XEE::windowData.sceneW - 129.0f,-32.0f),_XFColor(0.55f,0.55f,0.55f,1.0f),
			_XVector2(0.0f + XEE::windowData.sceneW - 129.0f,0.0f),_XFColor(0.55f,0.55f,0.55f,1.0f),
			_XVector2(0.0f,0.0f),_XFColor(0.35f,0.35f,0.35f,1.0f));
		m_titleFont.draw();
	}
	void setCallbackFun(void (*winExitFun)(void *),void (*winMinFun)(void *),void * pClass)
	{
		m_winExitFun = winExitFun;
		m_winMinFun = winMinFun;
		m_pClass = pClass;
	}
	void setWithText(bool b)
	{
		if(b) m_titleFont.setVisible();
		else m_titleFont.disVisible();
	}
	void mouseProc(int x,int y,_XMouseState mouseState);
	void release(){;}				

	_XWindowTitle()
		:m_isInited(false)
		,m_withText(true)
		,m_winExitFun(NULL)
		,m_winMinFun(NULL)
	{}
	~_XWindowTitle(){release();}
};
#include "XWindowTitle.inl"
#endif