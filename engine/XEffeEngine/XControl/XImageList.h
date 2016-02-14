#ifndef _JIA_XIMAGELIST_
#define _JIA_XIMAGELIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.5
//--------------------------------
//����һ��ͼ�γ��еĿؼ�
#include "XButtonEx.h"
#include "XSlider.h"
#include <deque>
namespace XE{
class XImageList:public XControlBasic
{
private:
	XBool m_isInited;			
public:
	enum XImageListEvent
	{
		IMGLST_INIT,
		IMGLST_RELEASE,
		IMGLST_SELECT_CHANGE,
	};
//	void (*m_funInit)(void *,int ID);
//	void (*m_funRelease)(void *,int ID);
//	void (*m_funSelectChange)(void *,int ID);
//	void *m_pClass;				//�ص������Ĳ���
protected:
	void draw();	
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);				//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState){return XFalse;}	//�����Ƿ񴥷���������
private:
	float m_buttonWidth;	//���Ұ�ť�Ŀ��
	XVector2 m_imageSize;	//�б��е�ͼƬ���
	int m_showImageSum;
	int m_curSelectImageIndex;	//��ǰѡ���ͼƬ���
	int m_curShowImageIndex;	//��ǰ��ʾ��ͼƬ����ʼ���

	XButtonEx m_leftBtn;	//��ߵİ�ť
	XButtonEx m_rightBtn;	//�ұߵİ�ť
	XSlider m_imageSld;	//������

	std::deque<XSprite *> m_imageList;	//ͼƬ������
	void updateState(bool flag = true);	//����ʵ�ʵ����ݸ��½�������

	//static void imageListCtrCB(void * pClass,int id);
	static void ctrlProc(void *,int,int);
public:
	XBool initWithoutSkin(float buttonWidth,	//���Ұ�ť�Ŀ��
		const XVector2 &imageSize,	//ͼƬ�ĳߴ�
		int showImageSum);	//��ʾͼƬ������
	XBool addImage(const char * filename);
	XBool addImage(const XTextureData &tex);	//��δʵ��
	XBool addImage(const XSprite &sprite);	//��δʵ��

	XImageList()
		:m_isInited(XFalse)
//		,m_funInit(NULL)
//		,m_funRelease(NULL)
//		,m_funSelectChange(NULL)
//		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGELIST;
	}
	~XImageList() {release();}
	void release();
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	void setTextColor(const XFColor&) {;}	//�����������ɫ
	XFColor getTextColor() const {return XFColor::white;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass = NULL);
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//����ʧȥ����
	void disable();
	void enable();
	void setVisible();
	void disVisible();
	XBool setACopy(const XImageList &)			//����һ������
	{//��δʵ��
		return XFalse;
	}

	//virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XImageList(const XImageList &temp);
	XImageList& operator = (const XImageList& temp);
};
#if WITH_INLINE_FILE
#include "XImageList.inl"
#endif
}
#endif