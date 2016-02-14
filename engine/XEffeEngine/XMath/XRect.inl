//INLINE �����Ķ���
INLINE void XRect::set(const XVector2& leftTop,const XVector2& rightBottom)
{
    set(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
}
INLINE XBool XRect::setLeft(float xl)
{
    left = xl;
    return (xl <= right);
}
INLINE XBool XRect::setRight(float xr)
{
    right = xr;
    return (xr >= left);
}
INLINE XBool XRect::setTop(float yt)
{
    top = yt;
    return (yt <= bottom);
}
INLINE XBool XRect::setBottom(float yb)
{
    bottom = yb;
    return (yb >= top);
}
INLINE float XRect::getLeft() const
{
    return left;
}
INLINE float XRect::getRight() const
{
    return right;
}
INLINE float XRect::getTop() const
{
    return top;
}
INLINE float XRect::getBottom() const
{
    return bottom;
}
INLINE XBool XRect::isCrash(const XRect& R0) const
{
	//x,y˫��ͶӰ���н������й�������
//	if((((left >= R0.left && left < R0.right) || (right <= R0.right && right > R0.left))
//		|| (((R0.left >= left && R0.left < right) || (R0.right <= right && R0.right > left))))
//		&& (((top >= R0.top && top < R0.bottom) || (bottom <= R0.bottom && bottom > R0.top))
//		|| (((R0.top >= top && R0.top < bottom) || (R0.bottom <= bottom && R0.bottom > top))))) return XTrue;
//	return XFalse;
	//������Ľ��һ�£��Ż���һ��
	return !(right <= R0.left || left >= R0.right || bottom <= R0.top || top >= R0.bottom);
}
INLINE XBool XRect::isCrash(const XVector2& leftTop,const XVector2& rightBottom) const
{
	//XRect R0(leftTop,rightBottom);
	return isCrash(XRect(leftTop,rightBottom));
}
INLINE XBool XRect::isCrash(float xl,float yt,float xr,float yb) const
{
	//XRect R0(xl,yt,xr,yb);
	return isCrash(XRect(xl,yt,xr,yb));
}
INLINE XBool XRect::isInRect(const XVector2& p0) const
{
    return (p0.x >= left && p0.x <= right && p0.y >= top && p0.y <= bottom);
}
INLINE XBool XRect::isInRect(float x,float y) const
{
//    if(x >= left && x <= right && y >= top && y <= bottom)
    return (x > left && x < right && y > top && y < bottom);
}
INLINE float XRect::getArea() const
{
    float area = (right - left) * (bottom - top);
    if(area < 0) return -area;
    else return area;
}
INLINE XVector2 XRect::getCenter() const
{
    //XVector2 temp;
    //temp.set((float)((left + right)/ 2.0), (float)((top + bottom)/ 2.0));
    //return temp;
    return XVector2((left + right) * 0.5f,(top + bottom) * 0.5f);
}
INLINE XVector2 XRect::getSize() const
{
	return XVector2(getWidth(),getHeight());
}
INLINE float XRect::getWidth() const
{
	return abs(right - left);
	//float temp = right - left;
	//if(temp < 0) return -temp;
	//else return temp;
}
INLINE float XRect::getHeight() const
{
	return abs(bottom - top);
	//float temp = bottom - top;
	//if(temp < 0) return -temp;
	//else return temp;
}
INLINE void XRect::setCenter(float x,float y)
{
    XVector2 temp = getCenter();
    temp.set(x - temp.x,y - temp.y);
    set(left + temp.x,top + temp.y,right + temp.x,bottom + temp.y);
}
INLINE void XRect::setCenter(const XVector2& p)
{
    setCenter(p.x,p.y);
}