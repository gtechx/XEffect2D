#ifndef _JIA_XMOUSERIGHTBUTTONMENU_
#define _JIA_XMOUSERIGHTBUTTONMENU_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XButton.h"
namespace XE{
//����Ҽ��˵����࣬��Ҫ������ʾ����Ҽ��˵�
//�Ҽ��˵����������İ�ť��ɣ���Ŧ����ʹ�ò�ͬ��ͼ���Ӷ�����һ����ɵ�����
//����˵��ǲ���ͨ����������̬�ı�λ�õģ�����λ��������Ҽ�ȷ����Ϊ�˼�С�Ѷȣ�Ŀǰ���У�
typedef XButtonSkin XMouseRightButtonMenuSkin;
class XEdit;
class XMouseRightButtonMenu:public XControlBasic
{
	friend XEdit;
private:
	XBool m_isInited;
	XButton *m_menu;		//��ť��ָ��
	int m_menuSum;		//��ť������
	int m_curChoose;		//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	int m_lastChoose;		//����ѡ���ֵ
public:
	enum XMouseRightButtonMenuEvent
	{
		MRBM_CHOOSE_CHANGE,
		MRBM_CHOOSE_OVER,
	};
//	void (*m_funChooseChange)(void *,int);	//�ı�ѡֵʱ����
//	void (*m_funChooseOver)(void *,int);	//����ȷ��ѡֵʱ����
//	void *m_pClass;
private:
	XRect m_allArea;		//�����ռ����Ӧ��Χ
	XVec2 m_upMousePoint;	//�ϴμ�¼������ڷ�Χ�ڵ�λ��

	XResourceInfo *m_resInfo;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ�ֿؼ���̬������ı���
	bool m_isDraw;			//�Ƿ��ǲ˵�����
	XMoveData m_actionMoveData;
	void playOutAction();	//�����˳��Ķ���
	//-----------------------------------------------------
public:
	//�����AreaΪ�����˵���������Ӧ����Ĭ�����в˵��һ��
	XBool init(int menuSum,	//�˵��е��������
		const XVec2& position,	//�˵���λ��
		const XRect& Area,	//�˵���������Ӧ��Χ
		const XMouseRightButtonMenuSkin &tex,	//�˵�����ͼ
		const XFontUnicode& font,float captionSize,const XVec2& textPosition);		//�˵�������
	XBool initEx(int menuSum,	//������ӿڵļ�
		const XVec2& position,	
		const XMouseRightButtonMenuSkin &tex,	
		const XFontUnicode& font,float captionSize = 1.0f);
	XBool initPlus(const char * path,int menuSum,	//�˵��е��������
		const XFontUnicode& font,float captionSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(int menuSum,const XRect& area,
		const XFontUnicode& font,float captionSize, const XVec2& textPosition);
	XBool initWithoutSkin(int menuSum,const XRect& area,const XVec2& textPosition)
	{
		return initWithoutSkin(menuSum,area,getDefaultFont(),1.0f,textPosition);
	}
	XBool initWithoutSkin(int menuSum,const XVec2& pixelSize,const XVec2& textPosition)
	{
		return initWithoutSkin(menuSum,XRect(XVec2::zero,pixelSize),
			getDefaultFont(),1.0f,textPosition);
	}
protected:
	void draw();//���˵�
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//����ʱ�����Ӧ����ʹ��������¼�ѡ��menu��
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	void setCurChoose(int){;}								//���õ�ǰѡ��Ĳ˵���
	XBool setACopy(const XMouseRightButtonMenu &temp);		//����һ����Ŀ��ʵ�干����Դ��ʵ��
	void setMnuState(bool state,int index)//���ð���״̬
	{
		if(index < 0 || index >= m_menuSum) return;
		if(state) m_menu[index].enable();
		else m_menu[index].disable();
	}
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);	//���ò˵���λ��

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	XMouseRightButtonMenu();
	~XMouseRightButtonMenu(){release();}
	//����������������
	int getLastChoose() const;										//��������ѡ���ֵ
	void setCallbackFun(void (* funChooseChange)(void *,int),
		void (* funChooseOver)(void *,int),
		void *pClass = NULL);	//���ò˵��Ļص�����
	void setHotKey(int hotKey,int order);							//���ò˵���ĳһ����ȼ�
	void setText(const char *temp,int order);								//�ı�˵���ĳһ���ֵ
	void setTexture(const XMouseRightButtonMenuSkin &tex,int order);	//�ı�˵���ĳһ�����ͼ
	void release();	//�ͷ���Դ
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//����Ľӿ���δʵ��
	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	//virtual void justForTest() {;}
private:	//����Ϊ�˷�ֹ������÷����Ĵ��󣬶����ظ�ֵ���캯���͸�ֵ������
	XMouseRightButtonMenu(const XMouseRightButtonMenu &temp);
	XMouseRightButtonMenu& operator = (const XMouseRightButtonMenu& temp);
};
#if WITH_INLINE_FILE
#include "XMouseRightButtonMenu.inl"
#endif
}
#endif