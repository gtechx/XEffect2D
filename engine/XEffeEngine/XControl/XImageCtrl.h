#ifndef _JIA_XIMAGECTRL_
#define _JIA_XIMAGECTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//����һ���򵥵���ͼ�Ŀؼ���������ʾһ��ͼƬ
#include "XControlBasic.h"
namespace XE{
class XImageCtrl:public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	XTextureData m_texture;	//��ͼ����
	XSprite m_sprite;	//������ʾ
	int m_pixelSum;	//�������
public:
	XBool init(const char *filename);			//���ļ��ж�ȡͼƬ
	XBool init(int w,int h,XColorMode mode);	//�����յ�ͼƬ
	void updateTxt(unsigned char *data)	{m_texture.updateTexture(data);}//������ͼ����
	int getWidth() const {return (int)(m_texture.textureMove2.x - m_texture.textureMove.x);}		//��ȡͼƬ�Ŀ��
	int getHeight() const {return (int)(m_texture.textureMove2.y - m_texture.textureMove.y);}	//��ȡͼƬ�ĸ߶�
protected:
	void draw();
	void drawUp() {;}

	XBool mouseProc(float,float,XMouseState){return XFalse;}
	XBool keyboardProc(int,XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	XBool canGetFocus(float x,float y);
	XBool canLostFocus(float,float){return XTrue;}
public:
	void bind(){m_texture.bind();}	//����ͼ
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);
	XImageCtrl()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_IMAGE;
	}
	~XImageCtrl(){release();}
	void release();
	XBool isInRect(float x,float y) {return m_sprite.isInRect(x,y);}
	XVector2 getBox(int order) {return m_sprite.getBox(order);}
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}
#endif