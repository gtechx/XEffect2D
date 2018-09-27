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
	bool m_onlyWithTexture;
	bool m_connectWithTex;	//�Ƿ�����ͼ���ӣ�����֮��ֻ��ʾָ������ͼ
	XTextureData *m_pTexture;	//��ͼ����
	XSprite m_sprite;	//������ʾ
	int m_pixelSum;	//�������
	int m_w;
	int m_h;
	unsigned int m_texID;
	unsigned int m_connectTex;
public:
	void setTextureID(unsigned int id){m_texID = id;}
	void imageDraw(const XVec2&scale,const XVec2&pos)
	{
		if(!m_isInited) return;
		XVec2 tmpScale = m_sprite.getScale();
		XVec2 tmpPos = m_sprite.getPosition();
		m_sprite.setScale(scale);
		m_sprite.setPosition(pos);
		m_sprite.draw(m_pTexture->texture.m_texture);
		m_sprite.setScale(tmpScale);
		m_sprite.setPosition(tmpPos);
	}
	bool getIsConnected()const{return m_connectWithTex;}	//��ȡ�Ƿ�����
	void connectTex(unsigned int tex)	//��ָ����ͼ����
	{
		m_connectWithTex = true;
		m_connectTex = tex;
	}
	void disConnected(){m_connectWithTex = false;}	//ȡ������
	XBool init(const char *filename);			//���ļ��ж�ȡͼƬ
	XBool init(int w,int h,XColorMode mode);	//�����յ�ͼƬ
	XBool init(int w,int h,unsigned int tex,bool needResize = false);	//ֻͨ����ͼ��Ž���
	void updateTxt(const void *data)	//������ͼ����
	{
		if(!m_onlyWithTexture && m_pTexture != NULL)
			m_pTexture->updateTexture(data);
	}
	void updateTxtR2B(const void *data)	//������ͼ����
	{
		if(!m_onlyWithTexture && m_pTexture != NULL)
			m_pTexture->updateTextureR2B(data);
	}
	int getWidth() const //��ȡͼƬ�Ŀ��
	{
		if(m_onlyWithTexture) return m_w;
		else return (int)(m_pTexture->textureMove2.x - m_pTexture->textureMove.x);
	}		
	int getHeight() const //��ȡͼƬ�ĸ߶�
	{
		if(m_onlyWithTexture) return m_h;
		else return (int)(m_pTexture->textureMove2.y - m_pTexture->textureMove.y);
	}	
	void disable() { m_isEnable = XFalse; }					//ʹ��Ч
	void enable() { m_isEnable = XTrue; }					//ʹ��Ч
protected:
	void draw();
	void drawUp() {;}

	XBool mouseProc(const XVec2&,XMouseState){return XFalse;}
	XBool keyboardProc(int,XKeyState){return XFalse;}
	void insertChar(const char *,int) {;}
	XBool canGetFocus(const XVec2& p);
	XBool canLostFocus(const XVec2&){return XTrue;}
public:
	void bind(){if(m_pTexture != NULL) m_pTexture->bind();}	//����ͼ
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);
	void fitScale(float x,float y);	//�Զ���Ӧָ���Ĵ�С
	XImageCtrl()
		:m_isInited(XFalse)
		,m_connectWithTex(false)
		,m_pTexture(NULL)
	{
		m_ctrlType = CTRL_OBJ_IMAGE;
	}
	~XImageCtrl(){release();}
	void release();
	XBool isInRect(const XVec2& p) {return m_sprite.isInRect(p);}
	XVec2 getBox(int order) {return m_sprite.getBox(order);}
	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);
};
#if WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}
#endif