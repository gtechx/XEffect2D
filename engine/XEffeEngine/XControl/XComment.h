#ifndef _JIA_XCOMMENT_
#define _JIA_XCOMMENT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.9.1
//--------------------------------
#include "../XFontUnicode.h"

//����һ��˵�����࣬�������������ָ���ط�ʱ��˵��������ʾ
class _XComment
{
private:
	_XBool m_isInited;		//�Ƿ��ʼ��
	_XFontUnicode m_font;
	_XRect m_bgRect;		//��������ʾ
	_XBool m_haveString;	//�Ƿ���˵��
	_XBool m_visible;		//�Ƿ�ɼ�
	_XBool m_needShow;		//�Ƿ���Ҫ��ʾ

	int m_timer;
public:
	_XBool init(const _XFontUnicode&font)
	{
		if(m_isInited) return XFalse;
		m_font.setACopy(font);
		m_font.setColor(0.0f,0.0f,0.0f,1.0f);
		m_font.setSize(0.5f,0.5f);
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		//m_font.setMaxStrLen(128);

		m_isInited = XTrue;
		return XTrue;
	}
	_XBool init();
	void update(int stepTime)
	{
		if(!m_needShow) return;
		m_timer += stepTime;
		if(m_timer >= 3000) m_needShow = false;
	}
	_XBool setACopy(const _XComment & temp)
	{
		m_isInited = temp.m_isInited;		//�Ƿ��ʼ��
		m_font.setACopy(temp.m_font);
		m_bgRect = temp.m_bgRect;		//��������ʾ
		m_haveString = temp.m_haveString;	//�Ƿ���˵��
		m_visible = temp.m_visible;		//�Ƿ�ɼ�
		m_needShow = temp.m_needShow;		//�Ƿ���Ҫ��ʾ

		m_timer = temp.m_timer;
		return XTrue;
	}
	void setPosition(float x,float y)
	{
		int w = m_bgRect.getWidth();
		int h = m_bgRect.getHeight();
		m_bgRect.set(x,y,x + w,y + h);
		m_font.setPosition(x + 2.0f,y + 2.0f);
	}
	void draw()
	{
		if(m_isInited && m_haveString && m_visible && m_needShow)
		{
			drawFillBoxA(_XVector2(m_bgRect.left,m_bgRect.top),_XVector2(m_bgRect.getWidth(),m_bgRect.getHeight()),0.75f,0.75f,0.75f,1.0f,true);
			m_font.draw();
		}
	}
	void setString(const char *str)
	{
		if(!m_isInited) return;
		if(str == NULL) m_haveString = XFalse;
		else
		{
			m_haveString = true;
			m_font.setString(str);
			int w = m_font.getMaxPixelWidth();
			int h = m_font.getMaxPixelHeight();
			m_bgRect.right = m_bgRect.left + w + 4;
			m_bgRect.bottom = m_bgRect.top + h + 4;
		}
	}
	void setVisible(){m_visible = XTrue;}
	void disVisible()
	{
		m_visible = XFalse;
		m_needShow = XFalse;
	}
	void setShow()
	{
		m_needShow = XTrue;
		m_timer = 0;
	}
	void disShow(){m_needShow = XFalse;}
	_XBool getIsShow(){return m_needShow;}
	_XComment()
		:m_isInited(XFalse)
		,m_haveString(XFalse)
		,m_visible(XTrue)
		,m_needShow(XFalse)
	{}
	~_XComment()
	{}
};
#include "XComment.inl"
#endif