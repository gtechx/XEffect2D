#ifndef _JIA_XBUTTONEX_
#define _JIA_XBUTTONEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.12
//--------------------------------
//����һ����XButton���ǿ��������֧�ָ�����״��button
#include "XButton.h"
namespace XE{
class XImageList;
class XButtonEx:public XControlBasic
{
	friend XImageList;
private:
	XBool m_isInited;					//��ť�Ƿ񱻳�ʼ��
	XButtonState m_curButtonState;		//��ǰ�İ�ť״̬

	XFontUnicode m_caption;			//��ť�ı���

	const XTextureData *m_buttonNormal;		//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const XTextureData *m_buttonOn;			//����״̬�µİ�ť��ͼ
	const XTextureData *m_buttonDisable;		//��Ч״̬�µİ�ť��ͼ

	XSprite m_sprite;			//������ʾ��ͼ�ľ���
	XVector2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	XFColor m_textColor;		//���ֵ���ɫ
	XVector2 m_upMousePoint;	//�ϴμ�¼�����λ��
	XVector2 *m_pArea;			//������״�ĵ������
	XVector2 *m_pCurArea;		//������״�ĵ������
	int m_areaPointSum;			//������״�ĵ������

	int m_hotKey;
	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
	float *m_colorRate;
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	XBool init(const XVector2& position,			//�ؼ���λ��
		const XVector2 *area,int pointSum,		//����������ť����Ӧ��Χ�����е㣬�Լ��������
		const XButtonSkin &tex,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,const XVector2 &captionPosition,	//��ť����ʾ�����ֵ������Ϣ
		const XFontUnicode &font);				//��ť��ʹ�õ�����
	XBool initEx(const XVector2& position,	//���Ƕ�����ӿڵļ�
		const XButtonSkin &tex,				
		const char *caption,float captionSize,	
		const XFontUnicode &font);
	XBool initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const XFontUnicode &font,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//��ť��ʹ�õ�����
	XBool initWithoutSkin(const XVector2 *area,int pointSum,const char *caption,	//��ť����ʵ�����ֵ������Ϣ
		const XFontUnicode &font,const XVector2 &captionPosition,float captionSize = 1.0f);
	XBool initWithoutSkin(const XVector2 *area,int pointSum,const char *caption)	//��ť����ʵ�����ֵ������Ϣ
	{
		if(area == NULL || pointSum <= 0) return XFalse;
		XVector2 tmp(0.0f,0.0f);
		for(int i = 0;i < pointSum;++ i)
		{
			tmp += area[i];
		}
		tmp /= (float)(pointSum);
		initWithoutSkin(area,pointSum,caption,getDefaultFont(),tmp,1.0f);
	}

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ
protected:
	void draw();								//��水ť
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
	void setLostFocus();	//����ʧȥ����
public:
	void setTexture(const XButtonSkin& tex);

	XBool setACopy(const XButtonEx &temp);

	XButtonEx();
	~XButtonEx(){release();}
	//������Ϊ������Ч�ʶ��������������
	void setCaptionPosition(const XVector2& textPosition);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionPosition(float x,float y);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionSize(const XVector2& size);						//���ð�ť�ı���ĳߴ�
	void setCaptionSize(float x,float y);						//���ð�ť�ı���ĳߴ�
	void setCaptionText(const char *caption);						//���ð�ť�ı��������
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//���ð������ȼ�
	int getHotKey() const;			//��ȡ��ǰ�������ȼ���Ϣ
	void setState(XButtonState temp);		//���ð�ť��״̬
	XButtonState getState() const; 
	void disable();
	void enable();
	void release();	//��Դ�ͷź���
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XButtonEx(const XButtonEx &temp);
	XButtonEx& operator = (const XButtonEx& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	XMoveData m_actionMoveData;	//��̬Ч���ı���
	XVector2 m_oldPos;				//��������ʱ��λ��
	XVector2 m_oldSize;			//��������ʱ�Ĵ�С
	XVector2 m_centerPos;			//����ε��е�
	XMoveData m_lightMD;
	//---------------------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���������µ��¼�����ʽ������ؼ����¼�
public:
	enum XButtonExEvent
	{
		BTNEX_INIT,
		BTNEX_RELEASE,
		BTNEX_MOUSE_ON,
		BTNEX_MOUSE_DOWN,
		BTNEX_MOUSE_UP,
	};
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XButtonEx.inl"
#endif
}
#endif