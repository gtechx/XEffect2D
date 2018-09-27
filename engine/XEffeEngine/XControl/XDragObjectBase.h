#ifndef _JIA_XDRAGOBJECTBASE_
#define _JIA_XDRAGOBJECTBASE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
//����ǿ���קͼ����Ļ��࣬����������Ա�����϶�
#include "../XOSDefine.h"
//#include "../XMouseAndKeyBoardDefine.h"
#include "../XMath/XVector2.h"
namespace XE{
class XDragObjectBase
{
protected:
	XBool m_isMouseBtnDown;
	int m_oldPosX;
	int m_oldPosY;
public:
	XDragObjectBase()
		:m_isMouseBtnDown(false)
		,m_oldPosX(0)
		,m_oldPosY(0)
	{}
	virtual ~XDragObjectBase(){}
	virtual void setPosition(const XVec2& pos) = 0;
	virtual const XVec2& getPosition()const = 0;
protected:
	virtual XBool getIsInDragRect(const XVec2& p) = 0;	//�ж�λ���Ƿ�����ק������
	virtual XVec2 correctPos(const XVec2& pos) = 0;	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��
	void mouseEventProc(const XVec2& p,XMouseState mouseState);
};
}
#endif