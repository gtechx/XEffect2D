#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ�����ڽ��������࣬������ʹ��������ͼ�Ľ⹹�����Ǳ����㣬�ƶ�������ֻ��߹ⷴ��㣬�����ƶ����Ǳ���Ҫ�еġ�

#include "XControlBasic.h"
#include "../XNumber.h"
#include "../XSprite.h"

class _XProgressTexture
{
private:
	_XBool m_isInited;
public:
	_XTextureData *progressBackGround;			//�������ı�����ͼ
	_XTextureData *progressMove;				//���������ƶ��仯����ͼ
	_XTextureData *progressUpon;				//���������ϲ�����ֻ��������Ч

	_XRect m_mouseRect;			//������Ӧ��Χ
	_XVector2 m_fontPosition;	//�������ֵ�λ��

	_XProgressTexture();
	~_XProgressTexture();
	_XBool init(const char *backgound,const char *move,const char *upon,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

//����������ֵ�趨��ΧΪ0.0 �� 100.0
class _XProgress:public _XControlBasic
{
private:
	float m_nowValue;					//�������ĵ�ǰֵ

	_XBool m_isInited;					//�������Ƿ񱻳�ʼ��
	char m_mode;	//0��ͨģʽ 1����ģʽ
	_XNumber m_caption;					//�������ı���

	const _XTextureData *m_progressBackGround;	//�������ı�����ͼ
	const _XTextureData *m_progressMove;			//���������ƶ��仯����ͼ
	const _XTextureData *m_progressUpon;			//���������ϲ�����ֻ��������Ч

	_XSprite m_spriteBackground;	//������ʾ�����ľ���
	_XSprite m_spriteMove;			//������ʾ��ͼ�ľ���
//	_XVector2 m_movePosition;			//����������ڱ�����λ��
	_XSprite m_spriteUpon;			//������ʾ�ϲ����ֵľ���
//	_XVector2 m_uponPosition;			//��������ڱ�����λ��
	_XVector2 m_textPosition;			//������ʾ��λ��
	_XVector2 m_textSize;				//���ֵĳߴ�
	_XFColor m_textColor;		//���ֵ���ɫ

	_XBool m_isShowFont;				//�Ƿ���ʾ��������
	_XBool m_isACopy;			//���ʵ���Ƿ���һ������

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	_XBool init(const _XVector2& position,//�ؼ����ڵ�λ��
		const _XRect &Area,	//����������ķ�Χ
		const _XProgressTexture &tex,	//�ؼ�����ͼ
		_XNumber* font,float captionSize,const _XVector2& textPosition,	//�ؼ�������
		int mode = 0);
	_XBool initEx(const _XVector2& position,//�ؼ����ڵ�λ��
		const _XProgressTexture &tex,	//�ؼ�����ͼ
		_XNumber* font,float captionSize,	//�ؼ�������
		int mode = 0);
	_XBool initPlus(const char * path,
		_XNumber* font,float captionSize,	//�ؼ�������
		int mode = 0,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect &area,
		_XNumber* font,float captionSize,	//�ؼ�������
		const _XVector2& textPosition,int mode = 0);

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		m_spriteBackground.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_spriteUpon.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_spriteBackground.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_spriteUpon.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ

protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState){return XTrue;}	//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XTrue;}	//do nothing
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	void setValue(float temp);
	float getValue() const;

	_XBool setACopy(const _XProgress &temp);		//����һ������
	_XProgress();
	~_XProgress();
	void release();
	//��������������
	void enable();
	void disable();
	//Ϊ��֧����������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XProgress(const _XProgress &temp);
	_XProgress& operator = (const _XProgress& temp);

	virtual void justForTest() {}
};
inline float _XProgress::getValue() const
{
	return m_nowValue;
}
inline void _XProgress::enable()
{
	m_isEnable = XTrue;
}
inline void _XProgress::disable()
{
	m_isEnable = XFalse;
}

#endif