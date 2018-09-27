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
#include "../XFont/XFontUnicode.h"

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
	XVec2 m_fontPosition;	//�������ֵ�λ��

	XProgressSkin();
	~XProgressSkin(){release();}
	XBool init(const char *backgound,const char *move,const char *upon,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
private:
	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
};

//����������ֵ�趨��ΧΪ0.0 �� 100.0
class XProgress:public XControlBasic
{
private:
	float m_curValue;					//�������ĵ�ǰֵ
	bool m_withNumber;					//����ʾ���ֻ�������
	XNumber m_captionN;					//�������ı���
	XFontUnicode m_caption;
public:
	void setFont(const XFontUnicode& font);	//�����ڳ�ʼ��֮���������ã����ұ����ڳ�ʼ��ʱû��ָ������
	void setNumber(const XNumber &number);	//�����ڳ�ʼ��֮���������ã����ұ����ڳ�ʼ��ʱû��ָ������
private:

	const XTextureData *m_progressBackGround;	//�������ı�����ͼ
	const XTextureData *m_progressMove;			//���������ƶ��仯����ͼ
	const XTextureData *m_progressUpon;			//���������ϲ�����ֻ��������Ч

	XSprite m_spriteBackground;	//������ʾ�����ľ���
	XSprite m_spriteMove;			//������ʾ��ͼ�ľ���
//	XVec2 m_movePosition;			//����������ڱ�����λ��
	XSprite m_spriteUpon;			//������ʾ�ϲ����ֵľ���
//	XVec2 m_uponPosition;			//��������ڱ�����λ��
	XVec2 m_textPosition;			//������ʾ��λ��
	XVec2 m_textSize;				//���ֵĳߴ�
	XFColor m_textColor;		//���ֵ���ɫ

	XBool m_isShowFont;				//�Ƿ���ʾ��������
	XBool m_isACopy;			//���ʵ���Ƿ���һ������
	XBool m_isInited;					//�������Ƿ񱻳�ʼ��
	XBool m_withoutTex;	//û����ͼ����ʽ
	char m_mode;	//0��ͨģʽ 1����ģʽ
	XResourceInfo *m_resInfo;

	bool m_withVirtual;			//�Ƿ�ʹ����������ݽ����м����
	XMoveData m_valueMD;
public:
	XBool init(const XVec2& position,//�ؼ����ڵ�λ��
		const XRect& Area,	//����������ķ�Χ
		const XProgressSkin &tex,	//�ؼ�����ͼ
		const XFontUnicode* font,float captionSize,const XVec2& textPosition,	//�ؼ�������
		int mode = 0);
	XBool initEx(const XVec2& position,//������ӿڵļ�
		const XProgressSkin &tex,	
		const XFontUnicode* font,float captionSize = 1.0f,
		int mode = 0);
	XBool initPlus(const char * path,
		const XFontUnicode* font,float captionSize = 1.0f,	//�ؼ�������
		const int mode = 0,XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,
		const XFontUnicode* font = NULL,float captionSize = 1.0f,	//�ؼ�������
		const XVec2& textPosition = XVec2::zero,int mode = 0);
	XBool initWithoutSkin(const XVec2& pixelSize,float captionSize = 1.0f,int mode = 0)
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),
			&getDefaultFont(),captionSize,pixelSize * 0.5f,mode);
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(const XFColor& c);//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ

protected:
	void update(float stepTime){m_valueMD.move(stepTime);}
	void draw();
	void drawUp(){};						//do nothing
	XBool mouseProc(const XVec2&,XMouseState){return XTrue;}	//do nothing
	XBool keyboardProc(int,XKeyState){return XTrue;}	//do nothing
	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2&){return XFalse;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}
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
	XBool isInRect(const XVec2& p);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	bool getWithVirtual()const{return m_withVirtual;}
	void setWithVirtual(bool flag){m_withVirtual = flag;}
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