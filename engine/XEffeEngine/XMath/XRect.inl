//inline �����Ķ���
inline void _XRect::set(const _XVector2& leftTop,const _XVector2& rightBottom)
{
    set(leftTop.x,leftTop.y,rightBottom.x,rightBottom.y);
}
inline _XBool _XRect::setLeft(float xl)
{
    left = xl;
    return (xl <= right);
}
inline _XBool _XRect::setRight(float xr)
{
    right = xr;
    return (xr >= left);
}
inline _XBool _XRect::setTop(float yt)
{
    top = yt;
    return (yt <= bottom);
}
inline _XBool _XRect::setBottom(float yb)
{
    bottom = yb;
    return (yb >= top);
}
inline float _XRect::getLeft() const
{
    return left;
}
inline float _XRect::getRight() const
{
    return right;
}
inline float _XRect::getTop() const
{
    return top;
}
inline float _XRect::getBottom() const
{
    return bottom;
}
inline _XBool _XRect::isCrash(const _XRect& R0) const
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
inline _XBool _XRect::isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const
{
	//_XRect R0(leftTop,rightBottom);
	return isCrash(_XRect(leftTop,rightBottom));
}
inline _XBool _XRect::isCrash(float xl,float yt,float xr,float yb) const
{
	//_XRect R0(xl,yt,xr,yb);
	return isCrash(_XRect(xl,yt,xr,yb));
}
inline _XBool _XRect::isInRect(const _XVector2& p0) const
{
    return (p0.x >= left && p0.x <= right && p0.y >= top && p0.y <= bottom);
}
inline _XBool _XRect::isInRect(float x,float y) const
{
//    if(x >= left && x <= right && y >= top && y <= bottom)
    return (x > left && x < right && y > top && y < bottom);
}
inline float _XRect::getArea() const
{
    float area = (right - left) * (bottom - top);
    if(area < 0) return -area;
    else return area;
}
inline _XVector2 _XRect::getCenter() const
{
    //_XVector2 temp;
    //temp.set((float)((left + right)/ 2.0), (float)((top + bottom)/ 2.0));
    //return temp;
    return _XVector2((left + right) * 0.5f,(top + bottom) * 0.5f);
}
inline float _XRect::getWidth() const
{
	return abs(right - left);
	//float temp = right - left;
	//if(temp < 0) return -temp;
	//else return temp;
}
inline float _XRect::getHeight() const
{
	return abs(bottom - top);
	//float temp = bottom - top;
	//if(temp < 0) return -temp;
	//else return temp;
}
inline void _XRect::setCenter(float x,float y)
{
    _XVector2 temp = getCenter();
    temp.set(x - temp.x,y - temp.y);
    set(left + temp.x,top + temp.y,right + temp.x,bottom + temp.y);
}
inline void _XRect::setCenter(const _XVector2& p)
{
    setCenter(p.x,p.y);
}