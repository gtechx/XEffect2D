#ifndef _JIA_XSIMPLELINE_
#define _JIA_XSIMPLELINE_
//����Ƿָ��߿ؼ�����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"
namespace XE{
enum XSimpleLineType
{
	SIMPLELINE_TYPE_VERTICAL,		//��ֱ��
	SIMPLELINE_TYPE_HORIZONTAL,		//ˮƽ��
};
class XSimpleLine:public XControlBasic
{
private:
	XBool m_isInited;
	float m_lineLen;
	float m_lineWidth;
	XSimpleLineType m_type;
public:
	XBool init(float lineLen,float lineWidth = 0.5f,
		XSimpleLineType type = SIMPLELINE_TYPE_HORIZONTAL);

	void setLineLen(float lineLen){m_lineLen = lineLen;}
	float getLineLen() const {return m_lineLen;}
	void setLineWidth(float lineWidth){m_lineWidth = lineWidth;}
	float getLineWidth() const {return m_lineWidth;}
	void setLineType(XSimpleLineType type){m_type = type;}
	XSimpleLineType getLineType() const {return m_type;}

	XSimpleLine()
		:m_isInited(XFalse)
		,m_lineLen(100.0f)
		,m_lineWidth(1.0f)
		,m_type(SIMPLELINE_TYPE_HORIZONTAL)
	{
		m_ctrlType = CTRL_OBJ_SIMPLELINE;
	}
	~XSimpleLine(){release();}
	void release();
	//������ͨ�ýӿ�
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor&){;}	//�����������ɫ
	XFColor getTextColor() const {return XFColor::white;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
protected:
	void draw();								//��水ť
	void drawUp(){};							//do nothing
	XBool mouseProc(float,float,XMouseState){return XFalse;}		//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(float,float){return XFalse;}				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
	XBool canBeChoose(){return XFalse;}
public:
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XSimpleLine(const XSimpleLine &temp);
	XSimpleLine& operator = (const XSimpleLine& temp);
};
#if WITH_INLINE_FILE
#include "XSimpleLine.inl"
#endif
}
#endif