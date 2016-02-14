#ifndef _JIA_XSPRITESTGOBJ_
#define _JIA_XSPRITESTGOBJ_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XStageObject.h"
#include "XSprite.h"
namespace XE{
//������ĳ������
class XSpriteStgObj:public XStageObject
{
private:
	bool m_isInited;	//�Ƿ���ɳ�ʼ��
	XSprite *m_pSprite;
public:
	XSpriteStgObj()
		:m_isInited(false)
		,m_pSprite(NULL)
	{}
	bool init(XSprite *pSprite,const XVector2 &originOffset)
	{
		if(m_isInited || pSprite == NULL) return false;

		m_pSprite = pSprite;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//���������ӿ��Ǳ��ڲ����õģ��ⲿ���ܵ���
	virtual void setPosition(float x,float y)
	{
		if(!m_isInited) return;
		m_pSprite->setPosition(x,y);
	}
	virtual void setScale(float x,float y)
	{
		if(!m_isInited) return;
		m_pSprite->setScale(x,y);
	}
	virtual void draw()
	{
		if(!m_isInited || !m_visible) return;
		m_pSprite->draw();
	}
	virtual void update(float/* stepTime*/){}
};
}
#endif