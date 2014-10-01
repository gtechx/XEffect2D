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

class _XImageList:public _XControlBasic
{
private:
	_XBool m_isInited;			

	void (*m_funInit)(void *,int ID);
	void (*m_funRelease)(void *,int ID);
	void (*m_funSelectChange)(void *,int ID);
	void *m_pClass;				//�ص������Ĳ���
protected:
	void draw();	
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);				//������궯������Ӧ����
	_XBool keyboardProc(int,_XKeyState){return XFalse;}	//�����Ƿ񴥷���������
private:
	float m_buttonWidth;	//���Ұ�ť�Ŀ��
	_XVector2 m_imageSize;	//�б��е�ͼƬ���
	int m_showImageSum;
	int m_nowSelectImageIndex;	//��ǰѡ���ͼƬ���
	int m_nowShowImageIndex;	//��ǰ��ʾ��ͼƬ����ʼ���

	_XButtonEx m_leftBtn;	//��ߵİ�ť
	_XButtonEx m_rightBtn;	//�ұߵİ�ť
	_XSlider m_imageSld;	//������

	std::deque<_XSprite *> m_imageList;	//ͼƬ������
	void updateState(bool flag = true);	//����ʵ�ʵ����ݸ��½�������

	friend void imageListCtrCB(void * pClass,int id);
public:
	_XBool initWithoutTex(float buttonWidth,	//���Ұ�ť�Ŀ��
		const _XVector2 &imageSize,	//ͼƬ�ĳߴ�
		int showImageSum);	//��ʾͼƬ������
	_XBool addImage(const char * filename);
	_XBool addImage(const _XTextureData &tex);	//��δʵ��
	_XBool addImage(const _XSprite &sprite);	//��δʵ��

	_XImageList()
		:m_isInited(XFalse)
		,m_funInit(NULL)
		,m_funRelease(NULL)
		,m_funSelectChange(NULL)
		,m_pClass(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGELIST;
	}
	~_XImageList() {release();}
	void release();
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor&) {;}	//�����������ɫ
	_XFColor getTextColor() const {return _XFColor::white;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funSelectChange)(void *,int),
		void *pClass = NULL);
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
	//void setLostFocus();	//����ʧȥ����
	void disable();
	void enable();
	void setVisible();
	void disVisible();
	_XBool setACopy(const _XImageList &)			//����һ������
	{//��δʵ��
		return XFalse;
	}

	//virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XImageList(const _XImageList &temp);
	_XImageList& operator = (const _XImageList& temp);

};

#include "XImageList.inl"

#endif