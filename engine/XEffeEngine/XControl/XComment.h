#ifndef _JIA_XCOMMENT_
#define _JIA_XCOMMENT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.9.1
//--------------------------------
#include "../XFont/XFontUnicode.h"
#include "XCtrlColorStyle.h"
#include "../XMath/XMoveData.h"
namespace XE{
//����һ��˵�����࣬�������������ָ���ط�ʱ��˵��������ʾ
class XComment
{
private:
	static const int m_appearTime = 1000;
	static const int m_disappear = 4000;
	static const int m_maxTime = 5000;
private:
	XBool m_isInited;		//�Ƿ��ʼ��
	XFontUnicode m_font;
	XRect m_bgRect;		//��������ʾ
	XBool m_haveString;	//�Ƿ���˵��
	XBool m_visible;		//�Ƿ�ɼ�
	XBool m_needShow;		//�Ƿ���Ҫ��ʾ

	int m_timer;
	float m_alpha;
	XMoveData m_alphaMD;	//͸���ȵĶ�̬�仯
public:
	XBool init(const XFontUnicode& font);
	XBool init();
	void update(float stepTime);
	XBool setACopy(const XComment & temp);
	void setPosition(float x,float y);
	void draw();
	void setString(const char *str);
	void setVisible(){m_visible = XTrue;}
	void disVisible();
	void setShow();
	void disShow()
	{
		if(!m_needShow) return;
		if(m_timer <= m_appearTime)
		{
			m_needShow = XFalse;	//��δ���֣���ֱ����ʧ
			return;
		}
		if(m_timer >= m_disappear) return;//������ʧ����ʲôҲ����
		//������ʧ�׶�
		m_timer = m_disappear - 1;
	}
	void setAlpha(float a)
	{
		m_alpha = a;
		m_font.setAlpha(a);
	}
	XBool getIsShow(){return m_needShow;}
	XComment()
		:m_isInited(XFalse)
		,m_haveString(XFalse)
		,m_visible(XTrue)
		,m_needShow(XFalse)
		,m_alpha(1.0f)
	{}
	~XComment(){}
private:
	XComment(const XComment &rhs );		//��ֹʹ�ÿ������캯��
	XComment &operator=( XComment );	//��ֹʹ�ø�ֵ������
};
#if WITH_INLINE_FILE
#include "XComment.inl"
#endif
}
#endif