#ifndef _JIA_XIMAGECTRL_
#define _JIA_XIMAGECTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//����һ���򵥵���ͼ�Ŀؼ���������ʾһ��ͼƬ
#include "XControlBasic.h"

class _XImageCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	_XTextureData m_texture;	//��ͼ����
	_XSprite m_sprite;	//������ʾ
	int m_pixelSum;	//�������
public:
	_XBool init(const char *filename);			//���ļ��ж�ȡͼƬ
	_XBool init(int w,int h,_XColorMode mode);	//�����յ�ͼƬ
	void updateTxt(unsigned char *data)	//������ͼ����
	{
		m_texture.updateTexture(data);
	}
	int getWidth() const {return m_texture.textureMove2.x - m_texture.textureMove.x;}		//��ȡͼƬ�Ŀ��
	int getHeight() const {return m_texture.textureMove2.y - m_texture.textureMove.y;}	//��ȡͼƬ�ĸ߶�
protected:
	void draw()
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isVisible) return;	//������ɼ�ֱ���˳�
		m_sprite.draw();
	}
	void drawUp() {;}

	_XBool mouseProc(float,float,_XMouseState){return XFalse;}
	_XBool keyboardProc(int,_XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	_XBool canGetFocus(float x,float y)
	{
		if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
			!m_isActive ||		//û�м���Ŀؼ������տ���
			!m_isVisible ||	//������ɼ�ֱ���˳�
			!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return isInRect(x,y);
	}
	_XBool canLostFocus(float,float){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		m_sprite.setPosition(m_position);
	}
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y)
	{
		m_size.set(x,y);
		m_sprite.setSize(m_size);
	}
	_XImageCtrl()
		:m_isInited(XFalse)
	{
		m_ctrlType = CTRL_OBJ_XIMAGE;
	}
	~_XImageCtrl(){release();}
	void release();
	_XBool isInRect(float x,float y) {return m_sprite.isInRect(x,y);}
	_XVector2 getBox(int order) {return m_sprite.getBox(order);}
	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		return m_sprite.setColor(r,g,b,a);
	}
	void setAlpha(float a)
	{
		m_color.setA(a);
		return m_sprite.setAlpha(a);
	}
};
#include "XImageCtrl.inl"
#endif