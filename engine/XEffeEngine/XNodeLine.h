#ifndef _JIA_XNODELINE_
#define _JIA_XNODELINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "stdlib.h"
#include "XBasicClass.h"
#include "XBasicFun.h"
#include "XObjectBasic.h"
#include "XBezierSpline.h"

#define NODELINE_DATA_FILE_NAME ("line/line00000.dat")

#pragma pack(push)
#pragma pack(1)
struct _XNode
{
	_XVector2 myPoint;		//������λ��
	float toNextAngle;		//����һ����ĽǶ�
	float toNextSin;		//����һ����ĽǶȵ�sinֵ
	float toNextCos;		//����һ����ĽǶȵ�cosֵ
	float toNextLength;		//����һ����ľ���

	float toPreviousAngle;	//��ǰһ����ĽǶ�
	float toPreviousSin;	//��ǰһ����ĽǶȵ�sinֵ
	float toPreviousCos;	//��ǰһ����ĽǶȵ�cosֵ
	float toPreviousLength;	//��ǰһ����ľ���
};
#pragma pack(pop)

//�����ߵ�����ģʽ
enum _XLeadMode
{
//	LEAD_MODE_NO_ROTATE = 0x01,		//û�нǶȱ仯
//	LEAD_MODE_ROTATE = 0x02,			//�������ߵĽǶȱ仯���仯
	LEAD_MODE_FOREWORD = 0x04,		//˳���ƶ�����
	LEAD_MODE_BACKWORD = 0x08			//�����ƶ�����
};
//���������е������
struct _XNodeSpecialPoint
{
	_XBool isEnable;			//������Ƿ���Ч
	_XBool isEnd;				//������Ƿ��Ѿ��������ߵ��յ�
	float nowLength;		//������߹��ľ���
	_XVector2 position;		//������λ��
	float angle;			//�����ĽǶ�
	float upNodeLength;		//��һ���ڵ�����
	int upNodeOrder;		//��һ���ڵ�ı��
	_XVector2 divergePoint;	//������ƫ��λ��
	float speed;			//�������ٶ�(pixel per second)
	_XNodeSpecialPoint()
		:isEnable(XFalse)
		,isEnd(XTrue)
	{}
};
//�ڵ������нڵ��������������
#define MAX_NODE_SUM 64
//����չ��ʱ����ʹ��
//typedef int XNodeSpecialPointHandle	//�����ڵ��е������ľ��

//ע�⣺����һЩ������Ҫ��1������������Ҫ�����㣬2�����в��ܳ��������غϵĵ㡣�������㻹û�н����
//���ܷ������ӱ�������ֵ��ʹ�����߸���Բ��
//�����Ҫʹ�ñ�������ֵ�Ļ��ֶβ���̫��
class _XSCounter;
//�ڵ��������ߵ���
class _XNodeLine:public _XObjectBasic
{
private:
	_XVector2 *m_bezierPoint;		//���ڱ�������ֵ�Ĺؼ���
	_XBool m_needUpdateData;			//�Ƿ���Ҫ���±�������ֵ�ĵ�
	void updateData();				//ˢ�±�������ֵ�Ĺؼ���
	_XBezierSpline m_bezierSpline;	//���ڱ�������ֵ����
private:
	_XSCounter *m_cp;				//���ü�����

	int m_myOrder;					//���������ߵı��
	_XNode *m_node;					//�������еĽڵ�
	_XLeadMode m_leadMode;					//���ߵ�����ģʽ
	_XBool m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	_XVector2 m_offsetPosition;		//�������ߵ�ƫ��λ��
	_XVector2 m_size;				//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
	_XBool m_isACopy;					//�Ƿ���һ������
	_XBool m_isVisible;

	float m_lineLength;				//���ߵ��ܳ���
	int m_nodeSum;					//���������е������ڵ������
public:
	void setVisible() {m_isVisible = XTrue;}					//��������ɼ�
	void disVisible() {m_isVisible = XFalse;}					//����������ɼ�
	_XBool getVisible() const {return m_isVisible;}			//��ȡ����Ƿ�ɼ���״̬ 
	//���ڲ�����������
	void setOneNode(const _XVector2& node,int nodeOrder);	//�ı�����ĳ�����λ��
	void setOneNode(float x,float y,int nodeOrder);	//�ı�����ĳ�����λ��
	void addOneNode(const _XVector2& node);		//������ͷ�����һ����
	void addOneNode(float x,float y);	//������ͷ�����һ����
	void decreaseOneNode();				//������ͷ���޳�һ����
	void addOneNode(const _XVector2& node,int nodeOrder);		//�������м����һ����
	void addOneNode(float x,float y,int nodeOrder);		//�������м����һ����
	void decreaseOneNode(int nodeOrder);	//�������м��޳�һ����
	_XBool saveNodeLine();					//�����������ߣ�ʹ�ú����ļ�������ʽ�����д�����
	int getOrder() const;						//��ȡ�����ŵ���Ϣ
	void setMyOrder(int temp);			//�������ߵı�ţ�ÿ�����߶���һ����ţ��ڱ�������ʱ��������������
	void clearUp();						//������еĵ�
	void setLoop();						//�������߱պ�
	int getNodeOrder(float x,float y,float distance) const	//����ָ������Ͼ���Ҫ�������ĵ�ı�ţ����û���򷵻�-1
	{
		if(distance < 0) return -1;
		_XVector2 temp(x,y);
		for(int i = 0;i < m_nodeSum;i ++)
		{
			if(temp.getLength(m_node[i].myPoint) <= distance) return i;
		}
		return -1;
	}

	int getNodeSum() const { return m_nodeSum;}//���ؽڵ������еĽڵ�����
	_XVector2 getNode(int order) const	//��ȡ�ڵ�������ĳ���ڵ��λ��
	{
		if(order < 0 || order >= m_nodeSum) return _XVector2::zero;
		return m_node[order].myPoint;
	}
	//int setSize(float size);			//�������ߵĳߴ�
	using _XObjectBasic::setPosition;
	void setPosition(float x,float y);	//�������ߵ�ƫ��λ��
	_XVector2 getPosition() const {return m_offsetPosition;}
	_XVector2 getSize() const {return m_size;}
	using _XObjectBasic::setSize;
	void setSize(float x,float y) {m_size.set(x,y);}

	float getAngle() const {return 0.0f;}	//�������ӿڶ��������û��ʵ�ʵ�����
	float getNodeLineLength() const {return m_lineLength;}//��ȡ�ڵ����ߵ��ܳ���
	void setAngle(float) {}	//�������ӿڶ��������û��ʵ�ʵ�����
	//Ϊ����������ʵ������Ľӿ�
	_XBool isInRect(float x,float y);
	_XVector2 getBox(int order);

private:
	//�������ߵ�ʹ��
	_XNodeSpecialPoint m_specialPoint;	//�����е������
	void release();		//�ͷ���Դ
public:
	_XBool getNodeLine(_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//���ļ��ж�ȡ��������
	void moveSpecialPoint(int timeDelay,int isLoop,_XBool needBezier = XFalse);	//�ƶ������е����ֵ㣬���ӹ����Ƿ�ʹ�ñ�������ֵ
	void resetSpecialPoint(_XBool needBezier = XFalse);				//����������

	void setLeadMode(_XLeadMode mode);				//�������ߵ����ͣ�˳�����е����ߣ������������е�����
	void setSpecialPointSpeed(float speed);	//���������еĵ���ٶ�
	_XBool getSpecialPointIsEnd() const;				//��ȡ�����еĵ��Ƿ��Ѿ��ִ��յ�
	_XVector2 getSpecialPointPosition() const;		//��ȡ�����еĵ��λ��
	float getSpecialPointAngle() const;			//��ȡ�����еĵ�ĵ�ǰ�Ƕ�[0 - PI]

	//setACopy�������������Ŀ�����Դ���������Ŀ�걻��ǰ�ͷŵ������������⣬������Ҫ����ʹ��
	_XBool setACopy(const _XNodeLine &temp);			//����һ������������ӵ�к�ԭ��һ������Դ��������ӵ���Լ��ĸ��ԣ���������Դ�ǲ����޸ĵ�
	_XNodeLine(const _XNodeLine &temp);				//���صĿ������캯������ȫ����������������ֻ������ֵ��ͬ����
	_XNodeLine& operator = (const _XNodeLine& temp);//���صĸ�ֵ����������ȫ����������������ֻ������ֵ��ͬ����

	virtual void setColor(float,float,float,float){}
	virtual _XFColor getColor() const {return _XFColor::white;}
	virtual void setAlpha(float){}

	void draw();

	_XNodeLine();
	~_XNodeLine();	//���������������ͷ���Դ

	//virtual void justForTest() {}
};

#include "XNodeLine.inl"

#endif