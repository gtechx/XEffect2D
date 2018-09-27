#ifndef _JIA_XTOOLBAR_
#define _JIA_XTOOLBAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.31
//--------------------------------
#include "XControlBasic.h"
namespace XE{
//����һ�����������࣬Ŀǰ��δʵ�ֹ���
//�������ǿؼ��ļ���
struct XToolBarUnit
{
	XBool enable;	//�Ƿ���Ч
	XControlBasic *obj;
	XVec2 scale;	//ԭʼ��С
	int width;
	XToolBarUnit()
		:obj(NULL)
		,enable(XTrue)
		,scale(1.0f)
		,width(0)
	{}
};
class XToolBar:public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��Ѿ���ʼ��
//	XVec2 m_position;
//	XVec2 m_scale;
	float m_insertPoint;	//������λ��

	int m_height;
	std::vector<XToolBarUnit> m_objects;

	XBool m_isMouseDown;	//����Ƿ���
	XVec2 m_oldMousePosition;	//��갴�µ�λ��
public:
	XBool initWithoutSkin(int height = 32);
protected:
	void draw();
	void drawUp();
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	//void setLostFocus(){return XTrue;}	//����ʧȥ����
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c){m_color = c;}
	void setAlpha(float a){m_color.setA(a);}

	//����಻�����Դ���й������Բ����Զ��ͷ���Դ�������Ҫ�ⲿû��
	//�����ͷ���Դ�Ļ���Ҫ�ֶ�������Դ�ͷź���
	void addAObject(XControlBasic *obj);
	XControlBasic *getObjByIndex(unsigned int index);
	int getIndexByP(const XControlBasic *obj);
	void releaseMem();
	XToolBar()
		:m_isInited(XFalse)
		,m_insertPoint(10.0f)
	{
		m_ctrlType = CTRL_OBJ_TOOLBAR;
	}
	~XToolBar(){release();}
	void release();

	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	void setVisible();
	void disVisible();	//���ÿؼ����ɼ�
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XToolBar(const XToolBar &temp);
	XToolBar& operator = (const XToolBar& temp);

};
#if WITH_INLINE_FILE
#include "XToolBar.inl"
#endif
}
#endif