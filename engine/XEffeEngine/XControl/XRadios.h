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
namespace XE{
typedef XCheckSkin XRadiosSkin;

class XRadios:public XControlBasic,public XBasicOprate
{
private:
	XBool m_isInited;	//�ռ�ʱ���Ѿ���ʼ��

	int m_radioSum;		//ѡ�������
	int m_curChoose;	//��ǰ��ѡ����ı��
	XCheck *m_radio;	//���е�ѡ���ָ��
	XVector2 *m_checkPosition;	//��ѡ������λ��
	XVector2 m_distance;	//���õ�ÿ����ѡ��֮��ľ���
public:
	enum XRadiosEvent
	{
		RDS_STATE_CHANGE,
	};
private:
//	void (*m_funStateChange)(void *,int);			//�ؼ�״̬�ı�ʱ����
//	void *m_pClass;				//�ص������Ĳ���

	const XCheckSkin *m_texture;

	XFontUnicode m_caption;
	float m_captionSize;
	XFColor m_textColor;	//��ʾ���������ɫ
	XVector2 m_textPosition;

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ
public:
	XBool init(int radioSum,			//ѡ�������
		const XVector2& distance,	//ÿ����ѡ��֮��ľ���
		const XVector2& position,	//�ؼ���λ��
		const XRect &Area,			//ѡ��ͼ��������Ӧ��Χ
		const XRadiosSkin* tex,const XFontUnicode &font,float captionSize,
		const XVector2& textPosition);		//��ѡ���ʼ��
	XBool initEx(int radioSum,			//������ӿڵļ�
		const XVector2& distance,	
		const XVector2& position,	
		const XRadiosSkin* tex,const XFontUnicode &font,float captionSize = 1.0);
	XBool initPlus(int radioSum,			//ѡ�������
		const XVector2& distance,	//ÿ����ѡ��֮��ľ���
		const char *path,const XFontUnicode &font,float captionSize = 1.0f,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XFontUnicode &font,float captionSize,
		const XVector2& textPosition);
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XFontUnicode &font,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,area,font,1.0f,textPosition);
	}
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XRect &area,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,area,getDefaultFont(),1.0f,textPosition);
	}
	XBool initWithoutSkin(int radioSum,
		const XVector2& distance,
		const XVector2 &pixelSize,
		const XVector2& textPosition)
	{
		return initWithoutSkin(radioSum,distance,XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),
			getDefaultFont(),1.0f,textPosition);
	}
protected:
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);	//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);		//�����ڼ���״̬��ʱ�򣬿���ͨ����������4�������ı�ѡ���ֵ
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//���õ�ǰѡ��ĵ�ѡ��
	void setRadioPosition(const XVector2& position,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	void setRadioPosition(float x,float y,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	
	void setDistance(const XVector2& distance);
	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(float x,float y);				//�������ű���

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//����λ��

	void setTextColor(const XFColor& color);//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//���ð�ť����ɫ
	void setAlpha(float a);//���ð�ť����ɫ

	XBool setACopy(const XRadios &temp);			//����һ������
	XBool setRadioSum(int radioSum);

	XRadios();
	~XRadios(){release();}
    //��������������
	void release();	//�ͷŷ������Դ
	//void setCallbackFun(void (* funStateChange)(void *,int),void *pClass = NULL);
	int getCurChoose() const;	//���ص�ǰ��ѡ����ѡ�����ı��
	void setRadioText(const char *temp,int order);	//���õ�ѡ����ĳһ�������
	void setRadioState(bool state,int order);	//���õ���ѡ���Ƿ��ѡ
	void setRadiosText(const char * temp);			//���ö����ֵ��ÿ��֮����';'����,�����������ƥ�䣬���Զ�ƥ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	//virtual void justForTest() {}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	XRadios(const XRadios &temp);
	XRadios& operator = (const XRadios& temp);
public:
	void setOprateState(void * data)
	{
		int index = *(int *)data;
		setChoosed(index);
	}
	void *getOprateState() const
	{
		int *data = XMem::createMem<int>();
		*data =  getCurChoose();
		return data;
	}
	void releaseOprateStateData(void *p)
	{
		int *data = (int*)p;
		XMem::XDELETE(data);
	}
	virtual bool isSameState(void * data)
	{
		if(data == NULL) return false;
		return(*(int*)data == getCurChoose());
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		if(!XXml::addLeafNode(e,m_ctrlName.c_str(),XString::toString(getCurChoose()))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		int tmp;
		if(XXml::getXmlAsInt(e,m_ctrlName.c_str(),tmp) == NULL) return XFalse;
		setChoosed(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XRadios.inl"
#endif
}
#endif