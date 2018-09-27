#ifndef _JIA_XMOVEDATAPAD_
#define _JIA_XMOVEDATAPAD_

#include "XControlBasic.h"
namespace XE{
class XMoveDataPad:public XControlBasic
{
private:
	float m_pixelsSize;
	bool m_isInited;
	XMoveData m_md;
	XFontUnicode m_caption;	
	XMoveData m_p1MD;	//���ڲ������Ƶ�
	XMoveData m_p2MD;	//���ڲ������Ƶ�
	XMoveData m_p3MD;	//���ڲ������Ƶ�
	XMoveData m_p4MD;	//���ڲ������Ƶ�
	void initParam(XMoveDataMode mode);
public:
	bool initWithoutSkin(XMoveDataMode mode);
	XMoveDataPad()
		:m_pixelsSize(256.0f)
		,m_isInited(false)
	{
		m_ctrlType = CTRL_OBJ_MOVEDATAPAD;
	}
	~XMoveDataPad(){release();}
	void release();
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p)
	{
		m_position = p;
		m_curMouseRect.set(m_position,m_position + m_scale * m_pixelsSize);
		m_caption.setPosition(m_curMouseRect.getLB());
		m_caption.setString(m_md.getModeStr().c_str());
	}
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s)
	{
		m_scale = s;
		m_curMouseRect.set(m_position,m_position + m_scale * m_pixelsSize);
		m_caption.setPosition(m_curMouseRect.getLB());
		m_caption.setString(m_md.getModeStr().c_str());
		m_caption.setScale(m_scale);
	}

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c)
	{
		m_color = c;
		m_caption.setAlpha(c.a);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		m_caption.setAlpha(a);
		updateChildAlpha();
	}
protected:
	void draw();
	void drawUp(){}
	void update(float steptime);
	XBool mouseProc(const XVec2& ,XMouseState){return XFalse;}		//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p)				//�����жϵ�ǰ����Ƿ���Ի�ý���
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return m_curMouseRect.isInRect(p);
	}
	XBool canLostFocus(const XVec2& ){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus()	//����ʧȥ����
	{
		if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||		//������ɼ�ֱ���˳�
			!m_isEnable) return;		//�����Ч��ֱ���˳�
		m_isBeChoose = XFalse;	//�ؼ����ڽ���״̬
	}

private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XMoveDataPad(const XMoveDataPad &temp);
	XMoveDataPad& operator = (const XMoveDataPad& temp);
public:
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
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

	void setMode(XMoveDataMode mode)
	{
		if(!m_isInited) return;
		m_md.setEx(0.0f,1.0f,5.0f,mode,-1);
		initParam(mode);
		m_caption.setString(m_md.getModeStr().c_str());
	}
};
}
#endif