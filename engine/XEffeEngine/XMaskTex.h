#ifndef _JIA_XMASKTEX_
#define _JIA_XMASKTEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.22
//--------------------------------
//����һ�����������ֵ���
//#include "XBasicClass.h"
#include "XMath\XVector2.h"
#include "XMath\XRect.h"
//#include "XOSDefine.h"
#include "XCommonDefine.h"
namespace XE{
class XMaskTex
{
private:
	XBool m_isInited;
	XVec2 m_sourcePixelSize;	//ԭʼ�����ش�С
	XVec2 m_curPixelSize;		//���ڵ����ش�С
	unsigned char * m_pixelData;		//�������ݣ������������һ�����ɾͲ����޸�

	XVec2 m_position;
	XVec2 m_scale;
	XRect m_rect;	//���κ��ӣ������������ж�
public:
	XBool init(const char *filename, XResPos resPos = RES_SYS_DEF);

	void setPosition(const XVec2& p)
	{
		m_position = p;
		m_rect.set(m_position, m_position + m_sourcePixelSize * m_scale);
	}
	//void setPosition(float x, float y){setPosition(XVec2(x,y));}
	//void setScale(float x,float y)
	//{
	//	setScale(XVec2(x,y));
	//}
	void setScale(const XVec2& scale)
	{
		m_scale = scale;
		m_rect.set(m_position, m_position + m_sourcePixelSize * m_scale);
	}
	//XBool getIsMask(float x,float y) const{getIsMask(XVec2(x,y));}
	XBool getIsMask(const XVec2& pos) const;
	//void draw();	//���ڲ���

	XMaskTex()
		:m_isInited(XFalse)
		, m_pixelData(NULL)
		, m_position(0.0f)
		, m_scale(1.0f)
	{}
	~XMaskTex() { release(); }
	void release()
	{
		if (!m_isInited) return;
		XMem::XDELETE_ARRAY(m_pixelData);

		m_isInited = XFalse;
	}
};
}
#endif	//_JIA_XMASKTEX_
