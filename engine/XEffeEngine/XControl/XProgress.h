#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//����һ�����ڽ��������࣬������ʹ��������ͼ�Ľ⹹�����Ǳ����㣬�ƶ�������ֻ��߹ⷴ��㣬�����ƶ����Ǳ���Ҫ�еġ�
#include "XControlBasic.h"
#include "XResourceManager.h"
#include "../XFont/XNumber.h"
namespace XE{
class XProgressSkin
{
private:
	XBool m_isInited;
public:
	XTextureData *progressBackGround;			//�������ı�����ͼ
	XTextureData *progressMove;				//���������ƶ��仯����ͼ
	XTextureData *progressUpon;				//���������ϲ�����ֻ��������Ч

	XRect m_mouseRect;			//������Ӧ��Χ
	XVector2 m_fontPosition;	//�������ֵ�λ��

	XProgressSkin();
	~XProgressSkin(){release();}
	XBool init(const char *backgound,const char *move,const char *upon,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
private:
	bool loadFromFolder(const char *filename,XResourcePosition resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResourcePosition resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResourcePosition resPos);		//����ҳ�ж�ȡ��Դ
};

//����������ֵ�趨��ΧΪ0.0 �� 100.0
class XProgress:public XControlBasic
{
private:
	float m_curValue;					//�������ĵ�ǰֵ
	XNumber m_caption;					//�������ı���

	const XTextureData *m_progressBackGround;	//�������ı�����ͼ
	const XTextureData *m_progressMove;			//���������ƶ��仯����ͼ
	const XTextureData *m_progressUpon;			//���������ϲ�����ֻ��������Ч

	XSprite m_spriteBackground;	//������ʾ�����ľ���
	XSprite m_spriteMove;			//������ʾ��ͼ�ľ���
//	XVector2 m_movePosition;			//����������ڱ�����λ��
	XSprite m_spriteUpon;			//������ʾ�ϲ����ֵľ���
//	XVector2 m_uponPosition;			//��������ڱ�����λ��
	XVector2 m_textPosition;			//������ʾ��λ��
	XVector2 m_textSize;				//���ֵĳߴ�
	XFColor m_textColor;		//���ֵ���ɫ

	XBool m_isShowFont;				//�Ƿ���ʾ��������
	XBool m_isACopy;			//���ʵ���Ƿ���һ������
	XBool m_isInited;					//�������Ƿ񱻳�ʼ��
	XBool m_withoutTex;	//û����ͼ����ʽ
	char m_mode;	//0��ͨģʽ 1����ģʽ
	XResourceInfo *m_resInfo;
public:
	XBool init(const XVector2& position,//�ؼ����ڵ�λ��
		const XRect &Area,	//����������ķ�Χ
		const XProgressSkin &tex,	//�ؼ�����ͼ
		XNumber* font,float captionSize,const XVector2& textPosition,	//�ؼ�������
		int mode = 0);
	XBool initEx(const XVector2& position,//������ӿڵļ�
		const XProgressSkin &tex,	
		XNumber* font,float captionSize = 1.0f,
		int mode = 0);
	XBool initPlus(const char * path,
		XNumber* font,float captionSize = 1.0f,	//�ؼ�������
		int mode = 0,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initWithoutSkin(const XRect &area,
		XNumber* font = NULL,float captionSize = 1.0f,	//�ؼ�������
		const XVector2& textPosition = XVector2(0.0f,0.0f),int mode = 0);
	XBool initWithoutSkin(const XVector2 &pixelSize,
		XNumber* font = NULL,float captionSize = 1.0f,	//�ؼ�������
		const XVector2& textPosition = XVector2(0.0f,0.0f),int mode = 0)
	{
		return initWithoutSkin(XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			font,captionSize,textPosition,mode);
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

protected:
	void draw();
	void drawUp(){};						//do nothing
	XBool mouseProc(float,float,XMouseState){return XTrue;}	//do nothing
	XBool keyboardProc(int,XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	XBool canGetFocus(float,float){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
public:
	void setValue(float temp);
	float getValue() const;

	XBool setACopy(const XProgress &temp);		//����һ������
	XProgress();
	~XProgress(){release();}
	void release();
	//��������������
	void enable();
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XProgress(const XProgress &temp);
	XProgress& operator = (const XProgress& temp);

	//virtual void justForTest() {}
};
#if WITH_INLINE_FILE
#include "XProgress.inl"
#endif
}
#endif