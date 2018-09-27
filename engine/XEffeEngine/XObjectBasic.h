#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XCommonDefine.h"
#include <string>
#include <vector>
#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
namespace XE{
enum XObjectType
{
	OBJ_NULL,			//��Ч�����
	OBJ_SPRITE,			//��������
	OBJ_FRAME,			//����֡�����
	OBJ_FRAMEEX,		//����֡�����
	OBJ_NUMBER,			//�����ַ��������
	OBJ_FONTUNICODE,	//��ʾ�ַ������
	OBJ_FONTX,			//��ʾ�ַ������
	OBJ_NODELINE,		//�ڵ����ߵ����
	OBJ_TURNBOARD,		//�����ӵĿؼ�
	OBJ_CONTROL,		//�ؼ������
};
extern std::string objectTypeStr[];
//������ܱ�������������������Ļ��࣬�ܱ�����������������������������
class XObjectManager;
class XObjectBasic
{
	//private:
	friend XObjectManager;
protected:
	XObjectType m_objType;
public:
	virtual XObjectType getObjectType() const { return m_objType; }
	virtual XBool isInRect(const XVec2&) = 0;		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	virtual XVec2 getBox(int order) = 0;			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual XVec2 getRectSize() { return getBox(2) - getBox(0); }
	virtual float getRectWidth() { return getBox(2).y - getBox(0).y; }
	virtual float getRectHeight() { return getBox(2).x - getBox(0).x; }

	virtual void setPosition(float x, float y) { setPosition(XVec2(x, y)); }		//���������λ��	+Child����
	virtual void setPosition(const XVec2& pos) = 0;
	virtual const XVec2& getPosition() const = 0;				//��ȡ����ĳߴ�

	virtual void setScale(float x, float y) { setScale(XVec2(x, y)); }		//��������ĳߴ�	+Child����
	virtual void setScale(const XVec2& scale) = 0;
	virtual void setScale(float scale) { setScale(XVec2(scale)); }
	virtual const XVec2& getScale() const = 0;					//��ȡ����ĳߴ�

	virtual void setAngle(float angle) = 0;					//��������ĽǶ�	+Child����
	virtual float getAngle() const = 0;						//��ȡ����ĽǶ�
	//virtual ~XObjectBasic();
	virtual void setVisible() = 0;					//��������ɼ�		+Child����
	virtual void disVisible() = 0;					//����������ɼ�	+Child����
	virtual XBool getVisible() const = 0;					//��ȡ����Ƿ�ɼ���״̬ 
	//�¼���Ľӿ�
	virtual void draw() = 0;

	virtual void setColor(float r, float g, float b, float a) { setColor(XFColor(r, g, b, a)); }		//+Child����
	virtual void setColor(const XFColor& color) = 0;
	virtual const XFColor& getColor() const = 0;
	virtual void setAlpha(float a) = 0;								//+Child����

	XObjectBasic()
		:m_parent(NULL)
		, m_objType(OBJ_NULL)
	{}
	virtual ~XObjectBasic()
	{
		if (m_parent == NULL) return;
		//�����ĸ�������ע���Լ�
		m_parent->popChild(this);
		m_parent = NULL;
		clearAllChild();	//ɾ����������������
	}
	//�������Ӷ����������ϵ�Ŀ���
protected:
	XObjectBasic *m_parent;	//������
	std::vector<XObjectBasic *> m_childList;	//�����б�,�κ�һ������ֻ����һ�������κ�һ����������ж��������
	std::vector<XVec2> m_childRelativePos;	//���λ��
	std::vector<XVec2> m_childRelativeSize;	//��Դ�С
	std::vector<float> m_childRelativeAngle;	//��ԽǶ�
												//Ϊ����Ƶļ򵥣�������ʱʹ����ɫ��͸���ȵĲ�����Ա仯����ͳһ�仯
	virtual void updateChildPos();
	virtual void updateChildScale();
	virtual void updateChildAngle();
	virtual void updateChildAlpha();
	virtual void updateChildColor();
	virtual void updateChildVisible();
public:
	virtual void clearAllChild();	//ɾ�����е�������
	virtual void pushChild(XObjectBasic * child);	//����
	virtual void popChild(XObjectBasic * child);	//�Ƴ�
	virtual XBool getIsChild(XObjectBasic * child);	//���Ŀ���Ƿ�Ϊ�Լ����ӳ�Ա
	//���������δ��ɣ�������һ�л�������
	//virtual void justForTest() = 0;	//��һ�е�Ŀ����Ϊ��ͨ��������һ���Ա���ͨ��������������Ҫ�޸ĵĵط�
};
}
#endif
