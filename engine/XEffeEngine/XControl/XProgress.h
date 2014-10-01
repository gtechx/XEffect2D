#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ�����ڽ��������࣬������ʹ��������ͼ�Ľ⹹�����Ǳ����㣬�ƶ�������ֻ��߹ⷴ��㣬�����ƶ����Ǳ���Ҫ�еġ�

#include "XControlBasic.h"
#include "XResourcePack.h"
#include "XResourceManager.h"
#include "../XNumber.h"

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
	~_XProgressTexture(){release();}
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
	_XBool initEx(const _XVector2& position,//������ӿڵļ�
		const _XProgressTexture &tex,	
		_XNumber* font,float captionSize = 1.0f,
		int mode = 0);
	_XBool initPlus(const char * path,
		_XNumber* font,float captionSize = 1.0f,	//�ؼ�������
		int mode = 0,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect &area,
		_XNumber* font = NULL,float captionSize = 1.0f,	//�ؼ�������
		const _XVector2& textPosition = _XVector2(0.0f,0.0f),int mode = 0);
	_XBool initWithoutTex(const _XVector2 &pixelSize,
		_XNumber* font = NULL,float captionSize = 1.0f,	//�ؼ�������
		const _XVector2& textPosition = _XVector2(0.0f,0.0f),int mode = 0)
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			font,captionSize,textPosition,mode);
	}

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float,float,_XMouseState){return XTrue;}	//do nothing
	_XBool keyboardProc(int,_XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float,float){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
public:
	void setValue(float temp);
	float getValue() const;

	_XBool setACopy(const _XProgress &temp);		//����һ������
	_XProgress();
	~_XProgress(){release();}
	void release();
	//��������������
	void enable();
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XProgress(const _XProgress &temp);
	_XProgress& operator = (const _XProgress& temp);

	//virtual void justForTest() {}
};
#include "XProgress.inl"

#endif