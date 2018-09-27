#ifndef _JIA_XRECT_
#define _JIA_XRECT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "..\XOSDefine.h"
#include "..\XCommonDefine.h"
#include "XVector2.h"
namespace XE{
class XRectEx;
//�����Ⱦ���
class XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
	void setAndOrder(float xl, float yt, float xr, float yb)
	{//���ò�����������Ļ����ϵ��Ҫ��
		if (xl > xr) XMath::swapTwoSum(xl, xr);
		if (yt > yb) XMath::swapTwoSum(yt, yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
	void set(float xl, float yt, float xr, float yb)
	{
		//if(xl > xr) XMath::swapTwoSum(&xl,&xr);
		//if(yt > yb) XMath::swapTwoSum(&yt,&yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
    void set(float ltD, float rbD)
	{
		//if(xl > xr) XMath::swapTwoSum(&xl,&xr);
		//if(yt > yb) XMath::swapTwoSum(&yt,&yb);
		left = ltD;
		top = ltD;
		right = rbD;
		bottom = rbD;
	}
	void set(const XVec2& leftTop, const XVec2& rightBottom);
	//���Ͻ�+ȫ�ߴ�
	void set(float _left, float _top, const XVec2& size)
	{
		left = _left;
		top = _top;
		right = left + size.x;
		bottom = top + size.y;
	}
    XBool setLeft(float xl);
    XBool setRight(float xr);
    XBool setTop(float yt);
    XBool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
	XRect()
		:left(0.0f),top(0.0f),right(0.0f),bottom(0.0f)
	{}
	//������
	XRect(float xl,float yt,float xr,float yb)
		:left(xl),top(yt),right(xr),bottom(yb)
	{}
	//������ʽ�ľ���
	XRect(float ltD,float rbD)
		:left(ltD),top(ltD),right(rbD),bottom(rbD)
	{}
	//���Ͻ�+���½�
	XRect(const XVec2& leftTop,const XVec2& rightBottom)
		:left(leftTop.x),top(leftTop.y),right(rightBottom.x),bottom(rightBottom.y)
	{}
	//���Ͻ�+ȫ�ߴ�
	XRect(float _left,float _top,const XVec2& size)
		:left(_left),top(_top),right(_left + size.x),bottom(_top + size.y)
	{}
	//���ĵ�+ȫ�ߴ�
	XRect(const XVec2& c,float w,float h)
		:left(c.x - w * 0.5f),top(c.y - h * 0.5f),right(c.x + w * 0.5f),bottom(c.y + h * 0.5f)
	{}
	XRect(const XRectEx& r);
	//ע������ĺ���ʽ����ڲ�����ת�ľ��ε���ײ�жϣ��������ת����Ҫ��һ���޸Ĵ���
    XBool isCrash(const XRect& R0) const;
    XBool isCrash(const XVec2& leftTop,const XVec2& rightBottom) const;
    XBool isCrash(float xl,float yt,float xr,float yb) const;
    XBool isInRect(const XVec2& p0) const;
    XBool isInRect(float x,float y) const;
    float getArea() const;
    XVec2 getCenter() const;
    XVec2 getSize() const;
	//��ȡ���Ͻǵ�
	XVec2 getLT() const { return XVec2(left, top); }
	//��ȡ���½ǵ�
	XVec2 getRB() const { return XVec2(right, bottom); }
	//��ȡ���½ǵ�
	XVec2 getLB() const { return XVec2(left, bottom); }
	//��ȡ���Ͻǵ�
	XVec2 getRT() const { return XVec2(right, top); }
	float getXCenter() const { return (left + right) * 0.5f; }
	float getYCenter() const { return (top + bottom) * 0.5f; }
    float getWidth() const;
    float getHeight() const;
//    void setCenter(float x,float y);
    void setCenter(const XVec2& p);
	void scaleCenter(float scale)	//������Ϊ��׼���������
	{
		XVec2 c = getCenter();
		XVec2 s(getWidth() * scale * 0.5f, getHeight() * scale * 0.5f);
		left = c.x - s.x;
		right = c.x + s.x;
		top = c.y - s.y;
		bottom = c.y + s.y;
	}
	void scaleLeftTop(float scale)	//�����Ͻ�Ϊ��׼���������
	{
		right = (right - left) * scale + left;
		bottom = (bottom - top) * scale + top;
	}
};
//������ʹ�����ĵ�ʹ�С�����һ�����ε���
class XRectEx
{
private:
public:
	XRectEx()
		:x(0), y(0), width(0), height(0)
	{}
	XRectEx(float posx, float posy, float w, float h)
		:x(posx), y(posy), width(w), height(h)
	{}
	XRectEx(const XRect& r)
		:x(r.getXCenter()), y(r.getYCenter()), width(r.getWidth()), height(r.getHeight())
	{}
	float x;
	float y;
	float width;
	float height;
	XVec2 getCenter()const { return XVec2(x + width * 0.5f, y + height * 0.5f); }
	float getLeft()const {return x - width * 0.5f; }
	float getRight()const { return x + width * 0.5f; }
	float getTop()const { return y - height * 0.5f; }
	float getBottom()const { return y + height * 0.5f; }
};
#if WITH_INLINE_FILE
#include "XRect.inl"
#endif
}
#endif