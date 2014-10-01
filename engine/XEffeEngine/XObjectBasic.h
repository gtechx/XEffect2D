#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XBasicClass.h"
#include "XOprateDes.h"
#include <vector>
enum _XObjectType
{
	OBJ_NULL,			//��Ч�����
	OBJ_SPRITE,			//��������
	OBJ_FRAME,			//����֡�����
	OBJ_FRAMEEX,		//����֡�����
	OBJ_NUMBER,			//�����ַ��������
	OBJ_FONTUNICODE,	//��ʾ�ַ������
	OBJ_FONTX,			//��ʾ�ַ������
	OBJ_NODELINE,		//�ڵ����ߵ����
	OBJ_CONTROL,		//�ؼ������
};
extern string objectTypeStr[];
//������ܱ�������������������Ļ��࣬�ܱ�����������������������������
class _XObjectManager;
class _XObjectBasic
{
//private:
	friend _XObjectManager;
protected:
	_XObjectType m_objType;
public:
	virtual _XObjectType getObjectType() const {return m_objType;}
	virtual _XBool isInRect(float x,float y) = 0;		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	virtual _XVector2 getBox(int order) = 0;			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void setPosition(float x,float y) = 0;		//���������λ��	+Child����
	virtual void setPosition(const _XVector2& pos) {setPosition(pos.x,pos.y);}
	virtual _XVector2 getPosition() const  = 0;				//��ȡ����ĳߴ�

	virtual void setSize(float x,float y) = 0;			//��������ĳߴ�	+Child����
	virtual void setSize(const _XVector2& size) {setSize(size.x,size.y);}
	virtual void setSize(float size) {setSize(size,size);}
	virtual _XVector2 getSize() const  = 0;					//��ȡ����ĳߴ�

	virtual void setAngle(float angle) = 0;					//��������ĽǶ�	+Child����
	virtual float getAngle() const = 0;						//��ȡ����ĽǶ�
	//virtual ~_XObjectBasic();
	virtual void setVisible() = 0;					//��������ɼ�		+Child����
	virtual void disVisible() = 0;					//����������ɼ�	+Child����
	virtual _XBool getVisible() const = 0;					//��ȡ����Ƿ�ɼ���״̬ 
	//�¼���Ľӿ�
	virtual void draw() = 0;

	virtual void setColor(float r,float g,float b,float a) = 0;		//+Child����
	virtual void setColor(const _XFColor& color) {setColor(color.fR,color.fG,color.fB,color.fA);}
	virtual _XFColor getColor() const = 0;				
	virtual void setAlpha(float a) = 0;								//+Child����

	_XObjectBasic()
		:m_parent(NULL)
		,m_objType(OBJ_NULL)
	{}
	virtual ~_XObjectBasic()
	{
		if(m_parent != NULL)
		{//�����ĸ�������ע���Լ�
			m_parent->popChild(this);
			m_parent = NULL;
			clearAllChild();	//ɾ����������������
		}
	}

	//�������Ӷ����������ϵ�Ŀ���
protected:
	_XObjectBasic *m_parent;	//������
	std::vector<_XObjectBasic *> m_childList;	//�����б�,�κ�һ������ֻ����һ�������κ�һ����������ж��������
	std::vector<_XVector2> m_childRelativePos;	//���λ��
	std::vector<_XVector2> m_childRelativeSize;	//��Դ�С
	std::vector<float> m_childRelativeAngle;	//��ԽǶ�
												//Ϊ����Ƶļ򵥣�������ʱʹ����ɫ��͸���ȵĲ�����Ա仯����ͳһ�仯
	virtual void updateChildPos();
	virtual void updateChildSize();
	virtual void updateChildAngle();
	virtual void updateChildAlpha();
	virtual void updateChildColor();
	virtual void updateChildVisible();
public:
	virtual void clearAllChild();	//ɾ�����е�������
	virtual void pushChild(_XObjectBasic * child);	//����
	virtual void popChild(_XObjectBasic * child);	//�Ƴ�
	virtual _XBool getIsChild(_XObjectBasic * child);	//���Ŀ���Ƿ�Ϊ�Լ����ӳ�Ա
	//���������δ��ɣ�������һ�л�������
	//virtual void justForTest() = 0;	//��һ�е�Ŀ����Ϊ��ͨ��������һ���Ա���ͨ��������������Ҫ�޸ĵĵط�
};

#endif
