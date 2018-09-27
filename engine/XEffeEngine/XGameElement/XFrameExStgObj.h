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
	virtual ~XFrameExStgObj(){}
	bool init(XFrameEx *pFrameEx,const XVec2& originOffset)
	{
		if(m_isInited || pFrameEx == NULL) return false;

		m_pFrameEx = pFrameEx;
		m_originOffset = originOffset;

		m_isInited = true;
		return true;
	}
//���������ӿ��Ǳ��ڲ����õģ��ⲿ���ܵ���
	virtual void setPosition(const XVec2& p)
	{
		if(!m_isInited) return;
		m_pFrameEx->setPosition(p);
	}
	virtual void setScale(const XVec2& s)
	{
		if(!m_isInited) return;
		m_pFrameEx->setScale(s);
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