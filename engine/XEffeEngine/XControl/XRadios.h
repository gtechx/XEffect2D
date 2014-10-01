#ifndef _JIA_XRADIOS_
#define _JIA_XRADIOS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ����ѡ����࣬������MFC��������ġ�
//1����ѡ�����Զ������Ҫ�ڳ�ʼ����ʱ���趨��һ�鵥ѡ���еĵ�ѡ������
//2�������ĵ�ѡ��ʵ�����Ǿ��ǵ����ĸ�ѡ��ʵ������û�������

//�ؼ��ļ��ʽ��Ҫ�޸�

//������ƶ������ţ�ͳһʹ���������ռ��������ƶ�������
#include "XCheck.h"

typedef _XCheckTexture _XRadiosTexture;

class _XRadios:public _XControlBasic,public _XBasicOprate
{
private:
	_XBool m_isInited;	//�ռ�ʱ���Ѿ���ʼ��

	int m_radioSum;		//ѡ�������
	int m_nowChoose;	//��ǰ��ѡ����ı��
	_XCheck *m_radio;	//���е�ѡ���ָ��
	_XVector2 *m_checkPosition;	//��ѡ������λ��
	_XVector2 m_distance;	//���õ�ÿ����ѡ��֮��ľ���

	void (*m_funStateChange)(void *,int);			//�ؼ�״̬�ı�ʱ����
	void *m_pClass;				//�ص������Ĳ���

	const _XCheckTexture *m_texture;

	_XFontUnicode m_caption;
	float m_captionSize;
	_XFColor m_textColor;	//��ʾ���������ɫ
	_XVector2 m_textPosition;

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	_XBool init(int radioSum,			//ѡ�������
		const _XVector2& distance,	//ÿ����ѡ��֮��ľ���
		const _XVector2& position,	//�ؼ���λ��
		const _XRect &Area,			//ѡ��ͼ��������Ӧ��Χ
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);		//��ѡ���ʼ��
	_XBool initEx(int radioSum,			//������ӿڵļ�
		const _XVector2& distance,	
		const _XVector2& position,	
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize = 1.0);
	_XBool initPlus(int radioSum,			//ѡ�������
		const _XVector2& distance,	//ÿ����ѡ��֮��ľ���
		const char *path,const _XFontUnicode &font,float captionSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XFontUnicode &font,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,area,font,1.0f,textPosition);
	}
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XRect &area,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,area,XEE::systemFont,1.0f,textPosition);
	}
	_XBool initWithoutTex(int radioSum,
		const _XVector2& distance,
		const _XVector2 &pixelSize,
		const _XVector2& textPosition)
	{
		return initWithoutTex(radioSum,distance,_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			XEE::systemFont,1.0f,textPosition);
	}
protected:
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//�����ڼ���״̬��ʱ�򣬿���ͨ����������4�������ı�ѡ���ֵ
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//���õ�ǰѡ��ĵ�ѡ��
	void setRadioPosition(const _XVector2& position,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	void setRadioPosition(float x,float y,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	
	void setDistance(const _XVector2& distance);
	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);				//�������ű���

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//����λ��

	void setTextColor(const _XFColor& color);//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//���ð�ť����ɫ
	void setAlpha(float a);//���ð�ť����ɫ

	_XBool setACopy(const _XRadios &temp);			//����һ������
	_XBool setRadioSum(int radioSum);

	_XRadios();
	~_XRadios(){release();}
    //��������������
	void release();	//�ͷŷ������Դ
	void setCallbackFun(void (* funStateChange)(void *,int),void *pClass = NULL);
	int getNowChoose() const;	//���ص�ǰ��ѡ����ѡ�����ı��
	void setRadioText(const char *temp,int order);	//���õ�ѡ����ĳһ�������
	void setRadiosText(const char * temp);			//���ö����ֵ��ÿ��֮����';'����,�����������ƥ�䣬���Զ�ƥ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XRadios(const _XRadios &temp);
	_XRadios& operator = (const _XRadios& temp);
public:
	void setOprateState(void * data)
	{
		int index = *(int *)data;
		setChoosed(index);
	}
	void *getOprateState() const
	{
		int *data = createMem<int>();
		*data =  getNowChoose();
		return data;
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(int*)data == getNowChoose());
	}
};
#include "XRadios.inl"
#endif