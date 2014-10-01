#ifndef _JIA_XTOOLBAR_
#define _JIA_XTOOLBAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.31
//--------------------------------
#include "XControlBasic.h"
//����һ�����������࣬Ŀǰ��δʵ�ֹ���
//�������ǿؼ��ļ���
struct _XToolBarUnit
{
	_XBool enable;	//�Ƿ���Ч
	_XControlBasic *obj;
	_XVector2 size;	//ԭʼ��С
	int width;
	_XToolBarUnit()
		:obj(NULL)
		,enable(XTrue)
		,size(1.0f,1.0f)
		,width(0.0f)
	{}
};
class _XToolBar:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��Ѿ���ʼ��
//	_XVector2 m_position;
//	_XVector2 m_size;
	float m_insertPoint;	//������λ��

	int m_height;
	std::vector<_XToolBarUnit> m_objects;

	_XBool m_isMouseDown;	//����Ƿ���
	_XVector2 m_oldMousePosition;	//��갴�µ�λ��
public:
	_XBool initWithoutTex(int height = 32);
protected:
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	//void setLostFocus(){return XTrue;}	//����ʧȥ����
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
	}

	//����಻�����Դ���й������Բ����Զ��ͷ���Դ�������Ҫ�ⲿû��
	//�����ͷ���Դ�Ļ���Ҫ�ֶ�������Դ�ͷź���
	void addAObject(_XControlBasic *obj);
	_XControlBasic *getObjByIndex(int index);
	int getIndexByP(const _XControlBasic *obj);
	void releaseMem();
	_XToolBar()
		:m_isInited(XFalse)
		,m_insertPoint(10.0f)
	{
		m_ctrlType = CTRL_OBJ_XTOOLBAR;
	}
	~_XToolBar(){release();}
	void release();

	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	void setVisible();
	void disVisible();	//���ÿؼ����ɼ�
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XToolBar(const _XToolBar &temp);
	_XToolBar& operator = (const _XToolBar& temp);

};
#include "XToolBar.inl"
#endif