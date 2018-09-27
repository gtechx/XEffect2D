#ifndef _JIA_XCOLORCHOOSE_
#define _JIA_XCOLORCHOOSE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.9
//--------------------------------
#include "XControlBasic.h"
#include "../XXml.h"
namespace XE{
//����һ������ѡ����ɫ�Ŀؼ�
class XColorChoose:public XControlBasic
{
public:
	enum XColorChooseType
	{
		TYPE_WITH_TEXT,
		TYPE_NO_TEXT,
	};
	enum XColorChooseEvent
	{
		CLC_COLOR_CHANGE,	//ѡ�����ɫ�����˸ı�
	};
private:
	XBool m_isInited;
	XFColor m_curColor;	//��ǰ����ɫ
	XVec2 m_area;		//��Χ

	XFontUnicode m_font;
	XColorChooseType m_type;
	XBool m_needChooseColor;
public:
	void setType(XColorChooseType type){m_type = type;}
	XBool initWithoutSkin(const XVec2& size,const XCColor &color);
protected:
	void draw();
	void drawUp(){}						
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		if(m_isSilent) return XFalse;
		return m_curMouseRect.isInRect(p);
	}
	XBool canLostFocus(const XVec2&){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	//void setLostFocus();	//����ʧȥ����
public:
	void update(float stepTime);
	XColorChoose()
		:m_isInited(XFalse)
		,m_type(TYPE_WITH_TEXT)
		,m_needChooseColor(XFalse)
	{
		m_ctrlType = CTRL_OBJ_COLORCHOOSE;
	}
	~XColorChoose(){release();}	//do nothing
	void release();
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XColorChoose(const XColorChoose &temp);
	XColorChoose& operator = (const XColorChoose& temp);
private:
	void updateColor();
public:
	XBool isInRect(const XVec2& p)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	{
		if(!m_isInited) return XFalse;
		return m_curMouseRect.isInRect(p);
	}
	XVec2 getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	{
		if(!m_isInited) return XVec2::zero;
		switch(order)
		{
		case 0:return m_curMouseRect.getLT();
		case 1:return m_curMouseRect.getRT();
		case 2:return m_curMouseRect.getRB();
		case 3:return m_curMouseRect.getLB();
		}
		return XVec2::zero;
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p)
	{
		m_position = p;
		m_font.setPosition(m_position + XVec2(2, m_area.y * 0.5f) * m_scale);
		m_curMouseRect.set(m_position, m_position + m_area * m_scale);
	}

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s)
	{
		m_scale = s;
		m_font.setPosition(m_position + XVec2(2, m_area.y * 0.5f) * m_scale);
		m_font.setScale(m_scale);
		m_curMouseRect.set(m_position, m_position + m_area * m_scale);
	}

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c){m_color = c;}
	void setAlpha(float a){m_color.setA(a);}

	XBool setACopy(const XColorChoose &temp);

	XFColor getCurColor() const {return m_curColor;}
	void setCurColor(const XFColor& c) 
	{
		m_curColor = c;
		updateColor();
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		XFColor tmp = getCurColor();
		if(!XXml::addLeafNode(e,(m_ctrlName + "R").c_str(),XString::toString(tmp.r))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "G").c_str(),XString::toString(tmp.g))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "B").c_str(),XString::toString(tmp.b))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "A").c_str(),XString::toString(tmp.a))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		XFColor tmp;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "R").c_str(),tmp.r) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "G").c_str(),tmp.g) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "B").c_str(),tmp.b) == NULL) return XFalse;
		if(XXml::getNextXmlAsFloat(e,(m_ctrlName + "A").c_str(),tmp.a) == NULL) return XFalse;
		setCurColor(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
}
#endif