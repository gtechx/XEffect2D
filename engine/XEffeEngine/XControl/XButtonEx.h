#ifndef _JIA_XBUTTONEX_
#define _JIA_XBUTTONEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.12
//--------------------------------
//����һ����XButton���ǿ��������֧�ָ�����״��button
#include "XButton.h"

class _XImageList;
class _XButtonEx:public _XControlBasic
{
	friend _XImageList;
private:
	_XBool m_isInited;					//��ť�Ƿ񱻳�ʼ��
	_XButtonState m_nowButtonState;		//��ǰ�İ�ť״̬

	_XFontUnicode m_caption;			//��ť�ı���

	const _XTextureData *m_buttonNormal;		//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const _XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonOn;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonDisable;		//��Ч״̬�µİ�ť��ͼ

	void (*m_funInit)(void *,int ID);			//�ؼ���ʼ����ʱ����õĺ���
	void (*m_funRelease)(void *,int ID);		//�ؼ���Դ�ͷŵ�ʱ����õĺ���
	void (*m_funMouseOn)(void *,int ID);		//���������ʱ����õĺ���
	void (*m_funMouseDown)(void *,int ID);		//��갴�µ�ʱ����õĺ���
	void (*m_funMouseUp)(void *,int ID);		//��굯���ʱ�����õĺ���
	void *m_pClass;				//�ص������Ĳ���

	_XSprite m_sprite;			//������ʾ��ͼ�ľ���
	_XVector2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
	_XVector2 m_upMousePoint;	//�ϴμ�¼�����λ��
	_XVector2 *m_pArea;			//������״�ĵ������
	_XVector2 *m_pNowArea;		//������״�ĵ������
	int m_areaPointSum;			//������״�ĵ������

	int m_hotKey;
	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
	float *m_colorRate;
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	_XBool init(const _XVector2& position,			//�ؼ���λ��
		const _XVector2 *area,int pointSum,		//����������ť����Ӧ��Χ�����е㣬�Լ��������
		const _XButtonTexture &tex,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,const _XVector2 &captionPosition,	//��ť����ʾ�����ֵ������Ϣ
		const _XFontUnicode &font);				//��ť��ʹ�õ�����
	_XBool initEx(const _XVector2& position,	//���Ƕ�����ӿڵļ�
		const _XButtonTexture &tex,				
		const char *caption,float captionSize,	
		const _XFontUnicode &font);
	_XBool initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//��ť��ʹ�õ�����
	_XBool initWithoutTex(const _XVector2 *area,int pointSum,const char *caption,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,const _XVector2 &captionPosition,float captionSize = 1.0f);
	_XBool initWithoutTex(const _XVector2 *area,int pointSum,const char *caption)	//��ť����ʵ�����ֵ������Ϣ
	{
		if(area == NULL || pointSum <= 0) return XFalse;
		_XVector2 tmp(0.0f,0.0f);
		for(int i = 0;i < pointSum;++ i)
		{
			tmp += area[i];
		}
		tmp /= (float)(pointSum);
		initWithoutTex(area,pointSum,caption,XEE::systemFont,tmp,1.0f);
	}

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);	//���ð�ť����ɫ
	void setAlpha(float a);	//���ð�ť����ɫ
protected:
	void draw();								//��水ť
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
	void setLostFocus();	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);
	void setMouseDownCB(void (* funMouseDown)(void *,int),void *pClass = NULL);
	void setMouseUpCB(void (* funMouseUp)(void *,int),void *pClass = NULL);
	void setTexture(const _XButtonTexture& tex);

	_XBool setACopy(const _XButtonEx &temp);

	_XButtonEx();
	~_XButtonEx(){release();}
	//������Ϊ������Ч�ʶ��������������
	void setCaptionPosition(const _XVector2& textPosition);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionPosition(float x,float y);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionSize(const _XVector2& size);						//���ð�ť�ı���ĳߴ�
	void setCaptionSize(float x,float y);						//���ð�ť�ı���ĳߴ�
	void setCaptionText(const char *caption);						//���ð�ť�ı��������
	const char *getCaptionString() const {return m_caption.getString();}
	void setHotKey(int hotKey);	//���ð������ȼ�
	int getHotKey() const;			//��ȡ��ǰ�������ȼ���Ϣ
	void setState(_XButtonState temp);		//���ð�ť��״̬
	_XButtonState getState() const; 
	void disable();
	void enable();
	void release();	//��Դ�ͷź���
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XButtonEx(const _XButtonEx &temp);
	_XButtonEx& operator = (const _XButtonEx& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԿؼ���̬֧�ֶ������������Ժͷ���
private:
	_XMoveData m_actionMoveData;	//��̬Ч���ı���
	_XVector2 m_oldPos;				//��������ʱ��λ��
	_XVector2 m_oldSize;			//��������ʱ�Ĵ�С
	_XVector2 m_centerPos;			//����ε��е�
	_XMoveData m_lightMD;
	//---------------------------------------------------------
};
#include "XButtonEx.inl"

#endif