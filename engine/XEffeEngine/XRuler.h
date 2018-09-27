#ifndef _JIA_XRULER_
#define _JIA_XRULER_
#include "XMath\XMath.h"
#include "XFont\XFontUnicode.h"
namespace XE {
struct XRulerStyle
{
	bool m_withCross;		//�Ƿ����ʮ������ϵ
	bool m_withEdgeLine;	//�Ƿ���ʾ����
	bool m_withEdgeRule;	//�Ƿ���ʾ�߱��
	bool m_withNet;			//�Ƿ���ʾ����
	bool m_withMousePos;	//�Ƿ���ʾ���λ����Ϣ
	bool m_withFont;		//�Ƿ���ʾ����

	XRulerStyle()
		:m_withCross(true)
		, m_withEdgeLine(true)
		, m_withEdgeRule(true)
		, m_withNet(true)
		, m_withMousePos(true)
		, m_withFont(true)
	{}
};
class XRuler
{
private:
	bool m_isInited;

	XVec2 m_position;	//���ĵ������λ��
	XVec2 m_size;		//ʵ�ʴ�С
	XVec2 m_scale;		//ʵ�ʵ����ص�����ϵ��
	XVec2 m_pixelSize;	//���������ش�С
	XVec2 m_posOffset;	//���ĵ��ƫ��λ��,Ϊʵ��ֵ����������ֵ

	XRulerStyle m_style;
	XFontUnicode m_font;
public:
	XVec2 getPosition()const { return m_position; }
	void setPosition(const XVec2& pos) { m_position = pos; }
	XVec2 getPixelsSize()const { return m_size * m_scale; }
	void setPosOffset(const XVec2& offset) { m_posOffset = offset; }
	XVec2 getPosOffset()const { return m_posOffset; }
	void setScale(float s)
	{
		if (!m_isInited) return;
		m_scale.set(s);
		m_size = m_pixelSize / m_scale;
	}
	void setScale(const XVec2& s)
	{
		if (!m_isInited) return;
		m_scale = s;
		m_size = m_pixelSize / m_scale;
	}
	bool init();
	//sizeΪ���ط�Χ
	bool init(const XVec2&pos, const XVec2&size);
	//sizeΪ���ط�Χ
	bool init(const XVec2&pos, const XVec2&size, const XVec2&scale);
	void update() {}
	void draw();

	XVec2 mapScreenToLocal(const XVec2& s)	//�������굽�ռ������ӳ��
	{
		return (s - m_position) / m_scale - m_posOffset;
	}

	XRuler()
		:m_isInited(false)
	{}
};
}
#endif