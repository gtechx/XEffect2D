#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XVector2.h"
namespace XE{
const XVector2 XVector2::zero(0.0f,0.0f);
#if !WITH_INLINE_FILE
#include "XVector2.inl"
#endif
//������2D��
//XVector2& XVector2::operator = (const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//	return *this;
//}
//XVector2::XVector2(const XVector2I & temp)
//{
//	x = temp.x;
//	y = temp.y;
//}
}