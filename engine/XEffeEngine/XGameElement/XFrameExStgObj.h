#ifndef _JIA_XFRAMEEXSTGOBJ_
#define _JIA_XFRAMEEXSTGOBJ_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XStageObject.h"
#include "../XFrameEx.h"
namespace XE{
//������ĳ������
class XFrameExStgObj:public XStageObject
{
private:
	bool m_isInited;	//�Ƿ���ɳ�ʼ��
	XFrameEx *m_pFrameEx;
public:
	XFrameExStgObj()
		:m_isInited(false)
		,m_pFrameEx(NULL)
	{}
	bool init(XFrameEx *pFrameEx,const XVector2 &originOffset)
	{
		if(m_isInited || pFrameEx == NULL) return false;

		m_pFrameEx = pFrameEx;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//���������ӿ��Ǳ��ڲ����õģ��ⲿ���ܵ���
	virtual void setPosition(float x,float y)
	{
		if(!m_isInited) return;
		m_pFrameEx->setPosition(x,y);
	}
	virtual void setScale(float x,float y)
	{
		if(!m_isInited) return;
		m_pFrameEx->setScale(x,y);
	}
	virtual void draw()
	{
		if(!m_isInited || !m_visible) return;
		m_pFrameEx->draw();
	}
	virtual void update(float stepTime)
	{
		if(!m_isInited) return;
		m_pFrameEx->move(stepTime);
	}
};
}
#endif