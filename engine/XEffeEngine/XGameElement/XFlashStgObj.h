#ifndef _JIA_XFLASHSTGOBJ_
#define _JIA_XFLASHSTGOBJ_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//#define WITH_FLASH	//�����Ҫ֧�������Ļ�����Ҫ�����������
#ifdef WITH_FLASH
#include "XStageObject.h"
#include "XFlashSprite.h"
namespace XE{
//������ĳ������
class XFlashStgObj:public XStageObject
{
private:
	bool m_isInited;	//�Ƿ���ɳ�ʼ��
	XFlashSprite *m_pFSprite;
public:
	XFlashStgObj()
		:m_isInited(false)
		,m_pFSprite(NULL)
	{}
	bool init(XFlashSprite *pSprite,const XVector2 &originOffset)
	{
		if(m_isInited || pSprite == NULL) return false;

		m_pFSprite = pSprite;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//���������ӿ��Ǳ��ڲ����õģ��ⲿ���ܵ���
	virtual void setPosition(float x,float y)
	{
		if(!m_isInited) return;
		m_pFSprite->setPosition(x,y);
	}
	virtual void setScale(float x,float y)
	{
		if(!m_isInited) return;
		m_pFSprite->setScale(x,y);
	}
	virtual void draw()
	{
		if(!m_isInited || !m_visible) return;
		m_pFSprite->draw();
	}
	virtual void update(float stepTime)
	{
		if(!m_isInited) return;
		m_pFSprite->update();
	}
};
}
#endif
#endif