#ifndef _JIA_XMOUSERIGHTBUTTONMENU_
#define _JIA_XMOUSERIGHTBUTTONMENU_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XButton.h"

//����Ҽ��˵����࣬��Ҫ������ʾ����Ҽ��˵�
//�Ҽ��˵����������İ�ť��ɣ���Ŧ����ʹ�ò�ͬ��ͼ���Ӷ�����һ����ɵ�����
//����˵��ǲ���ͨ����������̬�ı�λ�õģ�����λ��������Ҽ�ȷ����Ϊ�˼�С�Ѷȣ�Ŀǰ���У�
typedef _XButtonTexture _XMouseRightButtonMenuTexture;
class _XEdit;
class _XMouseRightButtonMenu:public _XControlBasic
{
	friend _XEdit;
private:
	_XBool m_isInited;
	_XButton *m_menu;		//��ť��ָ��
	int m_menuSum;		//��ť������
	int m_nowChoose;		//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	int m_lastChoose;		//����ѡ���ֵ

	void (*m_funChooseChange)(void *,int);	//�ı�ѡֵʱ����
	void (*m_funChooseOver)(void *,int);	//����ȷ��ѡֵʱ����
	void *m_pClass;

	_XRect m_allArea;		//�����ռ����Ӧ��Χ
	_XVector2 m_upMousePoint;	//�ϴμ�¼������ڷ�Χ�ڵ�λ��

	_XResourceInfo *m_resInfo;
public:
	//�����AreaΪ�����˵���������Ӧ����Ĭ�����в˵��һ��
	_XBool init(int menuSum,	//�˵��е��������
		const _XVector2& position,	//�˵���λ��
		const _XRect& Area,	//�˵���������Ӧ��Χ
		const _XMouseRightButtonMenuTexture &tex,	//�˵�����ͼ
		const _XFontUnicode &font,float captionSize,const _XVector2& textPosition);		//�˵�������
	_XBool initEx(int menuSum,	//������ӿڵļ�
		const _XVector2& position,	
		const _XMouseRightButtonMenuTexture &tex,	
		const _XFontUnicode &font,float captionSize = 1.0f);
	_XBool initPlus(const char * path,int menuSum,	//�˵��е��������
		const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(int menuSum,const _XRect& area,
		const _XFontUnicode &font,float captionSize, const _XVector2& textPosition);
	_XBool initWithoutTex(int menuSum,const _XRect& area,const _XVector2& textPosition)
	{
		return initWithoutTex(menuSum,area,XEE::systemFont,1.0f,textPosition);
	}
	_XBool initWithoutTex(int menuSum,const _XVector2& pixelSize,const _XVector2& textPosition)
	{
		return initWithoutTex(menuSum,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,textPosition);
	}
protected:
	void draw();//���˵�
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//����ʱ�����Ӧ����ʹ��������¼�ѡ��menu��
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void setNowChoose(int){;}								//���õ�ǰѡ��Ĳ˵���
	_XBool setACopy(const _XMouseRightButtonMenu &temp);		//����һ����Ŀ��ʵ�干����Դ��ʵ��
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);	//���ò˵���λ��

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	_XMouseRightButtonMenu();
	~_XMouseRightButtonMenu(){release();}
	//����������������
	int getLastChoose() const;										//��������ѡ���ֵ
	void setCallbackFun(void (* funChooseChange)(void *,int),
		void (* funChooseOver)(void *,int),
		void *pClass = NULL);	//���ò˵��Ļص�����
	void setHotKey(int hotKey,int order);							//���ò˵���ĳһ����ȼ�
	void setText(const char *temp,int order);								//�ı�˵���ĳһ���ֵ
	void setTexture(const _XMouseRightButtonMenuTexture &tex,int order);	//�ı�˵���ĳһ�����ͼ
	void release();	//�ͷ���Դ
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//����Ľӿ���δʵ��
	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//virtual void justForTest() {;}
private:	//����Ϊ�˷�ֹ������÷����Ĵ��󣬶����ظ�ֵ���캯���͸�ֵ������
	_XMouseRightButtonMenu(const _XMouseRightButtonMenu &temp);
	_XMouseRightButtonMenu& operator = (const _XMouseRightButtonMenu& temp);
};
#include "XMouseRightButtonMenu.inl"

#endif