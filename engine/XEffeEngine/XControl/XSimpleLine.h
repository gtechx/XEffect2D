#ifndef _JIA_XSIMPLELINE_
#define _JIA_XSIMPLELINE_
//����Ƿָ��߿ؼ�����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.14
//--------------------------------
#include "XControlBasic.h"

enum _XSimpleLineType
{
	SIMPLELINE_TYPE_VERTICAL,		//��ֱ��
	SIMPLELINE_TYPE_HORIZONTAL,		//ˮƽ��
};
class _XSimpleLine:public _XControlBasic
{
private:
	_XBool m_isInited;
	float m_lineLen;
	float m_lineWidth;
	_XSimpleLineType m_type;
public:
	_XBool init(float lineLen,float lineWidth = 0.5f,
		_XSimpleLineType type = SIMPLELINE_TYPE_HORIZONTAL);

	void setLineLen(float lineLen){m_lineLen = lineLen;}
	float getLineLen() const {return m_lineLen;}
	void setLineWidth(float lineWidth){m_lineWidth = lineWidth;}
	float getLineWidth() const {return m_lineWidth;}
	void setLineType(_XSimpleLineType type){m_type = type;}
	_XSimpleLineType getLineType() const {return m_type;}

	_XSimpleLine()
		:m_isInited(XFalse)
		,m_lineLen(100.0f)
		,m_lineWidth(1.0f)
		,m_type(SIMPLELINE_TYPE_HORIZONTAL)
	{
		m_ctrlType = CTRL_OBJ_XSIMPLELINE;
	}
	~_XSimpleLine(){release();}
	void release();
	//������ͨ�ýӿ�
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color){;}	//�����������ɫ
	_XFColor getTextColor() const {return _XFColor::white;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAlpha(float a){m_color.setA(a);}
protected:
	void draw();								//��水ť
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XFalse;}		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y){return XFalse;}				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
	_XBool canBeChoose(){return XFalse;}
public:
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XSimpleLine(const _XSimpleLine &temp);
	_XSimpleLine& operator = (const _XSimpleLine& temp);
};
#include "XSimpleLine.inl"
#endif