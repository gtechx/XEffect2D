#ifndef _JIA_XNODELINE_
#define _JIA_XNODELINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XObjectBasic.h"
#include "XMath/XBezierSpline.h"
namespace XE{
#define NODELINE_DATA_FILE_NAME "line/line00000.dat"
#pragma pack(push)
#pragma pack(1)
struct XNode
{
	XVector2 myPoint;		//������λ��
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
enum XLeadMode
{
//	LEAD_MODE_NO_ROTATE = 0x01,		//û�нǶȱ仯
//	LEAD_MODE_ROTATE = 0x02,			//�������ߵĽǶȱ仯���仯
	LEAD_MODE_FOREWORD = 0x04,		//˳���ƶ�����
	LEAD_MODE_BACKWORD = 0x08			//�����ƶ�����
};
//���������е������
struct XNodeSpecialPoint
{
	XBool isEnable;			//������Ƿ���Ч
	XBool isEnd;				//������Ƿ��Ѿ��������ߵ��յ�
	float curLength;		//������߹��ľ���
	XVector2 position;		//������λ��
	float angle;			//�����ĽǶ�
	float upNodeLength;		//��һ���ڵ�����
	int upNodeOrder;		//��һ���ڵ�ı��
	XVector2 divergePoint;	//������ƫ��λ��
	float speed;			//�������ٶ�(pixel per second)
	XNodeSpecialPoint()
		:isEnable(XFalse)
		,isEnd(XTrue)
	{}
};
//����չ��ʱ����ʹ��
//typedef int XNodeSpecialPointHandle	//�����ڵ��е������ľ��

//ע�⣺����һЩ������Ҫ��1������������Ҫ�����㣬2�����в��ܳ��������غϵĵ㡣�������㻹û�н����
//���ܷ������ӱ�������ֵ��ʹ�����߸���Բ��
//�����Ҫʹ�ñ�������ֵ�Ļ��ֶβ���̫��
class XSCounter;
//�ڵ��������ߵ���
class XNodeLine:public XObjectBasic
{
private:
	static const int m_maxNodeSum = 64;//�ڵ������нڵ��������������
	XVector2 *m_bezierPoint;		//���ڱ�������ֵ�Ĺؼ���
	XBool m_needUpdateData;			//�Ƿ���Ҫ���±�������ֵ�ĵ�
	XBool m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���
	XBool m_isACopy;					//�Ƿ���һ������
	XBool m_isVisible;
	void updateData();				//ˢ�±�������ֵ�Ĺؼ���
	XBezierSpline m_bezierSpline;	//���ڱ�������ֵ����

	XSCounter *m_cp;				//���ü�����
	int m_myOrder;					//���������ߵı��
	XNode *m_node;					//�������еĽڵ�
	XLeadMode m_leadMode;					//���ߵ�����ģʽ

	XVector2 m_offsetPosition;		//�������ߵ�ƫ��λ��
	XVector2 m_scale;				//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	float m_lineLength;				//���ߵ��ܳ���
	int m_nodeSum;					//���������е������ڵ������
public:
	void setVisible() {m_isVisible = XTrue;}					//��������ɼ�
	void disVisible() {m_isVisible = XFalse;}					//����������ɼ�
	XBool getVisible() const {return m_isVisible;}			//��ȡ����Ƿ�ɼ���״̬ 
	//���ڲ�����������
	void setOneNode(const XVector2& node,int nodeOrder);	//�ı�����ĳ�����λ��
	void setOneNode(float x,float y,int nodeOrder);	//�ı�����ĳ�����λ��
	void addOneNode(const XVector2& node);		//������ͷ�����һ����
	void addOneNode(float x,float y);	//������ͷ�����һ����
	void decreaseOneNode();				//������ͷ���޳�һ����
	void addOneNode(const XVector2& node,int nodeOrder);		//�������м����һ����
	void addOneNode(float x,float y,int nodeOrder);		//�������м����һ����
	void decreaseOneNode(int nodeOrder);	//�������м��޳�һ����
	XBool saveNodeLine();					//�����������ߣ�ʹ�ú����ļ�������ʽ�����д�����
	int getOrder() const;						//��ȡ�����ŵ���Ϣ
	void setMyOrder(int temp);			//�������ߵı�ţ�ÿ�����߶���һ����ţ��ڱ�������ʱ��������������
	void clearUp();						//������еĵ�
	void setLoop();						//�������߱պ�
	int getNodeOrder(float x,float y,float distance) const	//����ָ������Ͼ���Ҫ�������ĵ�ı�ţ����û���򷵻�-1
	{
		if(distance < 0) return -1;
		XVector2 temp(x,y);
		for(int i = 0;i < m_nodeSum;i ++)
		{
			if(temp.getLength(m_node[i].myPoint) <= distance) return i;
		}
		return -1;
	}

	int getNodeSum() const {return m_nodeSum;}//���ؽڵ������еĽڵ�����
	XVector2 getNode(int order) const	//��ȡ�ڵ�������ĳ���ڵ��λ��
	{
		if(order < 0 || order >= m_nodeSum) return XVector2::zero;
		return m_node[order].myPoint;
	}
	//int setSize(float size);			//�������ߵĳߴ�
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);	//�������ߵ�ƫ��λ��
	XVector2 getPosition() const {return m_offsetPosition;}
	XVector2 getScale() const {return m_scale;}
	using XObjectBasic::setScale;
	void setScale(float x,float y) {m_scale.set(x,y);}

	float getAngle() const {return 0.0f;}	//�������ӿڶ��������û��ʵ�ʵ�����
	float getNodeLineLength() const {return m_lineLength;}//��ȡ�ڵ����ߵ��ܳ���
	void setAngle(float) {}	//�������ӿڶ��������û��ʵ�ʵ�����
	//Ϊ����������ʵ������Ľӿ�
	XBool isInRect(float x,float y);
	XVector2 getBox(int order);

private:
	//�������ߵ�ʹ��
	XNodeSpecialPoint m_specialPoint;	//�����е������
	void release();		//�ͷ���Դ
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
public:
	XBool getNodeLine(XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//���ļ��ж�ȡ��������
	void moveSpecialPoint(int timeDelay,int isLoop,XBool needBezier = XFalse);	//�ƶ������е����ֵ㣬���ӹ����Ƿ�ʹ�ñ�������ֵ
	void resetSpecialPoint(XBool needBezier = XFalse);				//����������

	void setLeadMode(XLeadMode mode);				//�������ߵ����ͣ�˳�����е����ߣ������������е�����
	void setSpecialPointSpeed(float speed);	//���������еĵ���ٶ�
	XBool getSpecialPointIsEnd() const;				//��ȡ�����еĵ��Ƿ��Ѿ��ִ��յ�
	XVector2 getSpecialPointPosition() const;		//��ȡ�����еĵ��λ��
	float getSpecialPointAngle() const;			//��ȡ�����еĵ�ĵ�ǰ�Ƕ�[0 - PI]

	//setACopy�������������Ŀ�����Դ���������Ŀ�걻��ǰ�ͷŵ������������⣬������Ҫ����ʹ��
	XBool setACopy(const XNodeLine &temp);			//����һ������������ӵ�к�ԭ��һ������Դ��������ӵ���Լ��ĸ��ԣ���������Դ�ǲ����޸ĵ�
	XNodeLine(const XNodeLine &temp);				//���صĿ������캯������ȫ����������������ֻ������ֵ��ͬ����
	XNodeLine& operator = (const XNodeLine& temp);//���صĸ�ֵ����������ȫ����������������ֻ������ֵ��ͬ����

	virtual void setColor(float,float,float,float){}
	virtual XFColor getColor() const {return XFColor::white;}
	virtual void setAlpha(float){}

	void draw();

	XNodeLine();
	~XNodeLine();	//���������������ͷ���Դ

	//virtual void justForTest() {}
};
#if WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}
#endif