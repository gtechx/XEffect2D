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
#define NODELINE_DATA_FILE_NAME "line/line%05d.dat"
#pragma pack(push)
#pragma pack(1)
struct XNode
{
	XVec2 myPoint;		//������λ��
	//������û����ƽ���Ĳ���
	float toNextAngle;		//����һ����ĽǶ�
	float toNextSin;		//����һ����ĽǶȵ�sinֵ
	float toNextCos;		//����һ����ĽǶȵ�cosֵ
	float toNextLength;		//����һ����ľ���

	float toPreviousAngle;	//��ǰһ����ĽǶ�
	float toPreviousSin;	//��ǰһ����ĽǶȵ�sinֵ
	float toPreviousCos;	//��ǰһ����ĽǶȵ�cosֵ
	float toPreviousLength;	//��ǰһ����ľ���
	//�����Ǿ���������ƽ���Ĳ���
	float toBezNextAngle;		//����һ����ĽǶ�
	float toBezNextLength;		//����һ����ľ���
	float toBezPreviousAngle;	//��ǰһ����ĽǶ�
	float toBezPreviousLength;	//��ǰһ����ľ���

	XNode(float x = 0.0f, float y = 0.0f)
		:myPoint(x, y)
		, toNextAngle(0.0f)
		, toNextSin(0.0f)
		, toNextCos(0.0f)
		, toNextLength(0.0f)
		, toPreviousAngle(0.0f)
		, toPreviousSin(0.0f)
		, toPreviousCos(0.0f)
		, toPreviousLength(0.0f)
	{}
	XNode(const XVec2& p)
		:myPoint(p)
		, toNextAngle(0.0f)
		, toNextSin(0.0f)
		, toNextCos(0.0f)
		, toNextLength(0.0f)
		, toPreviousAngle(0.0f)
		, toPreviousSin(0.0f)
		, toPreviousCos(0.0f)
		, toPreviousLength(0.0f)
	{}
	void resetToNext()
	{
		toNextAngle = 0.0f;
		toNextSin = 0.0f;
		toNextCos = 0.0f;
		toNextLength = 0.0f;
	}
	void resetToPreviouse()
	{
		toPreviousAngle = 0.0f;
		toPreviousSin = 0.0f;
		toPreviousCos = 0.0f;
		toPreviousLength = 0.0f;
	}
	void setNext(const XVec2& pos)
	{
		toNextAngle = myPoint.getAngle(pos);
		toNextCos = cos(toNextAngle);
		toNextSin = sin(toNextAngle);
		toNextLength = myPoint.getLength(pos);
	}
	void setNext(XNode &node)
	{
		setNext(node.myPoint);
		node.toPreviousAngle = toNextAngle + PI;
		if (node.toPreviousAngle >= PI2) node.toPreviousAngle -= PI2;
		node.toPreviousCos = cos(node.toPreviousAngle);
		node.toPreviousSin = sin(node.toPreviousAngle);
		node.toPreviousLength = toNextLength;
	}
	void setPrevious(const XVec2& pos)
	{
		toPreviousAngle = myPoint.getAngle(pos);
		toPreviousCos = cos(toPreviousAngle);
		toPreviousSin = sin(toPreviousAngle);
		toPreviousLength = myPoint.getLength(pos);
	}
	void setPrevious(XNode &node)
	{
		setPrevious(node.myPoint);
		node.toNextAngle = toPreviousAngle + PI;
		if (node.toNextAngle >= PI2) node.toNextAngle -= PI2;
		node.toNextCos = cos(node.toNextAngle);
		node.toNextSin = sin(node.toNextAngle);
		node.toNextLength = toPreviousLength;
	}
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
	XBool isEnd;			//������Ƿ��Ѿ��������ߵ��յ�
	float curLength;		//������߹��ľ���
	XVec2 position;		//������λ��
	float angle;			//�����ĽǶ�
	float upNodeLength;		//��һ���ڵ�����
	int upNodeOrder;		//��һ���ڵ�ı��
	XVec2 divergePoint;	//������ƫ��λ��
	float speed;			//�������ٶ�(pixel per second)
	XNodeSpecialPoint()
		:isEnable(XFalse)
		, isEnd(XTrue)
	{}
};
struct XNodeCrossInfo
{
	XVec2 pos;	//������λ��
	int indexA;		//˳��λ��
	int indexB;
	float angleA;	//�Ƕ�
	float angleB;
};
struct XNodeMatchInfo
{//�����ڵ�����ƥ�����Ϣ
	int matchLen;	//ƥ��ĳ���
	int indexA;		//ƥ��A��ʼ��ı��
	int indexB;		//ƥ��B��ʼ��ı��
	bool isOrder;	//�Ƿ���˳��ƥ��
	XNodeMatchInfo()
		:matchLen(0)
	{}
};
struct XNodePointInfo
{
	XVec2 pos;	//��ֵ���λ��
	float angle;	//��ֵ��ĽǶ�
	bool isEnd;		//�Ƿ����
	int upIndex;	//��������ĵ�ı��
	XNodePointInfo()
		:upIndex(0)
		, isEnd(true)
		, angle(0.0f)
	{}
};
//����չ��ʱ����ʹ��
//typedef int XNodeSpecialPointHandle	//�����ڵ��е������ľ��

//ע�⣺����һЩ������Ҫ��1������������Ҫ�����㣬2�����в��ܳ��������غϵĵ㡣�������㻹û�н����
//���ڱ��������Ż���ȡ�����߶ε�ƽ��������Ϊ��׼���ڳ������ȵ��߽��жԲ�ֱ�����г��ȶ�С�ڻ����ƽ������Ϊֹ
//���ܷ������ӱ�������ֵ��ʹ�����߸���Բ��
//�����Ҫʹ�ñ�������ֵ�Ļ��ֶβ���̫�������Գ�������Ż���
class XSCounter;
//�ڵ��������ߵ���
class XNodeLine :public XObjectBasic
{
private:
	//	static const int m_maxNodeSum = 512;//�ڵ������нڵ��������������
	//	XVec2 *m_bezierPoint;		//���ڱ�������ֵ�Ĺؼ���
	XBool m_needUpdateData;			//�Ƿ���Ҫ���±�������ֵ�ĵ�
	XBool m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���
	XBool m_isVisible;
public:
	void updateData();				//ˢ�±�������ֵ�Ĺؼ���
private:
	std::vector<XVec2> m_bezierPoint;
	XBezierSpline m_bezierSpline;	//���ڱ�������ֵ����

//	XSCounter *m_cp;				//���ü�����
	int m_myOrder;					//���������ߵı��
//	XNode *m_node;					//�������еĽڵ�
	std::vector<XNode> m_node;
	XLeadMode m_leadMode;					//���ߵ�����ģʽ

	XVec2 m_offsetPosition;		//�������ߵ�ƫ��λ��
	XVec2 m_scale;				//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	float m_lineLength;				//���ߵ��ܳ��ȣ�ֱ�߳��Ⱥ�
	float m_bezLineLength;			//���߾���������ƽ��֮��ĳ��Ⱥ�
//	int m_nodeSum;					//���������е������ڵ������
	bool m_withAngleSmooth;			//�Ƿ�ʹ�ýǶ�ƽ��
public:
	bool getWithAngleSmooth()const { return m_withAngleSmooth; }
	void setWithAngleSmooth(bool flag) { m_withAngleSmooth = flag; }
	void setVisible() { m_isVisible = XTrue; }					//��������ɼ�
	void disVisible() { m_isVisible = XFalse; }					//����������ɼ�
	XBool getVisible() const { return m_isVisible; }			//��ȡ����Ƿ�ɼ���״̬ 
	//���ڲ�����������
	void setOneNode(const XVec2& node, int nodeOrder);	//�ı�����ĳ�����λ��
//	void setOneNode(float x,float y,int nodeOrder);	//�ı�����ĳ�����λ��
	void addOneNode(const XVec2& node);		//������ͷ�����һ����
//	void addOneNode(float x,float y);	//������ͷ�����һ����
	void decreaseOneNode();				//������β���޳�һ����
	void addOneNode(const XVec2& node, int nodeOrder);		//�������м����һ����
//	void addOneNode(float x,float y,int nodeOrder);		//�������м����һ����
	void decreaseOneNode(int nodeOrder);	//�������м��޳�һ����
	void decreaseNode(int nodeS, int nodeE);	//ɾ���ڵ�S���ڵ�E�еĵ㣬������S��E(��δʵ��)
	XBool saveNodeLine();					//�����������ߣ�ʹ�ú����ļ�������ʽ�����д�����
	int getOrder() const;						//��ȡ�����ŵ���Ϣ
	void setMyOrder(int temp);			//�������ߵı�ţ�ÿ�����߶���һ����ţ��ڱ�������ʱ��������������
	void clearUp();						//������еĵ�
	void setLoop(bool flag);						//�������߱պ�
	bool getIsLoop()const { return m_isLoop; }
	//	int getNodeOrder(float x,float y,float distance) const	//����ָ������Ͼ���Ҫ�������ĵ�ı�ţ����û���򷵻�-1
	//	{
	//		return getNodeOrder(XVec2(x,y), distance);
	//	}
	int getNodeOrder(const XVec2& p, float distance) const	//����ָ������Ͼ���Ҫ�������ĵ�ı�ţ����û���򷵻�-1
	{
		if (distance < 0) return -1;
		distance *= distance;
		for (auto it = m_node.begin(); it != m_node.end(); ++it)
		{
			if (p.getLengthSqure(it->myPoint) <= distance) return it - m_node.begin();
		}
		return -1;
	}

	int getNodeSum() const { return m_node.size(); }//���ؽڵ������еĽڵ�����
	XVec2 getNode(int order) const	//��ȡ�ڵ�������ĳ���ڵ��λ��
	{
		if (order < 0 || order >= m_node.size()) return XVec2::zero;
		return m_node[order].myPoint;
	}
	XNode getNodeInfo(int order) const
	{
		if (order < 0 || order >= m_node.size()) return XNode();
		return m_node[order];
	}
	//int setSize(float size);			//�������ߵĳߴ�
	using XObjectBasic::setPosition;
	void setPosition(const XVec2& p);	//�������ߵ�ƫ��λ��
	const XVec2& getPosition() const { return m_offsetPosition; }
	const XVec2& getScale() const { return m_scale; }
	using XObjectBasic::setScale;
	void setScale(const XVec2& s) { m_scale = s; }

	float getAngle() const { return 0.0f; }	//�������ӿڶ��������û��ʵ�ʵ�����
	float getNodeLineLength() const
	{
		if (m_withBezier) return m_bezLineLength;
		else return m_lineLength;
	}//��ȡ�ڵ����ߵ��ܳ���
	void setAngle(float) {}	//�������ӿڶ��������û��ʵ�ʵ�����
	//Ϊ����������ʵ������Ľӿ�
	XBool isInRect(const XVec2& p);
	XVec2 getBox(int order);

private:
	//�������ߵ�ʹ��
	XNodeSpecialPoint m_specialPoint;	//�����е������
	void release();		//�ͷ���Դ
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
public:
	XBool getNodeLine(XResPos resPos = RES_SYS_DEF);					//���ļ��ж�ȡ��������
	void moveSpecialPoint(float timeDelay);	//�ƶ������е����ֵ㣬���ӹ����Ƿ�ʹ�ñ�������ֵ
	void resetSpecialPoint();				//����������
	void resetSpecialPt(int index, float distance = 0.0f);		//�����������ĳһ�㿪ʼ�ƶ���������һ�ξ���
	void updateBezierInfo();	//���±�����������
private:
	bool m_withBezier;	//�Ƿ�ʹ�ñ�������ֵ
public:
	bool getWithBezier()const { return m_withBezier; }
	void setWithBezier(bool flag);
	//Ŀǰ����㷨��������
	void bezierOptimization();	//����ڱ����������߽����Ż��Ա������߸��ӱ��ֺ���
	void bezierOptimizationEx()	//����ڱ����������߽����Ż��Ա������߸��ӱ��ֺ���
	{
		while (getDeformityRate() >= 3.0f)
		{
			bezierOptimization();
		}
	}

	XLeadMode getLeadMode()const { return m_leadMode; }
	void setLeadMode(XLeadMode mode);				//�������ߵ����ͣ�˳�����е����ߣ������������е�����
	void setSpecialPointSpeed(float speed);	//���������еĵ���ٶ�
	XBool getSpecialPointIsEnd() const;				//��ȡ�����еĵ��Ƿ��Ѿ��ִ��յ�
	XVec2 getSpecialPointPosition() const;		//��ȡ�����еĵ��λ��
	float getSpecialPointAngle() const;			//��ȡ�����еĵ�ĵ�ǰ�Ƕ�[0 - PI]
	XVec2 getRatePosition(float rate, int *index = NULL, bool needBezier = false);	//rate[0 - 1],���ذٷֱȴ�������
	XVec2 getRate(const XVec2& pos, int *index = NULL, bool needBezier = false);	//������pos��ӽ��ĵ�İٷֱȺ;���

	//setACopy�������������Ŀ�����Դ���������Ŀ�걻��ǰ�ͷŵ������������⣬������Ҫ����ʹ��
	XBool setACopy(const XNodeLine &temp);			//����һ������������ӵ�к�ԭ��һ������Դ��������ӵ���Լ��ĸ��ԣ���������Դ�ǲ����޸ĵ�
	XNodeLine(const XNodeLine &temp);				//���صĿ������캯������ȫ����������������ֻ������ֵ��ͬ����
	XNodeLine& operator = (const XNodeLine& temp);//���صĸ�ֵ����������ȫ����������������ֻ������ֵ��ͬ����

	virtual void setColor(const XFColor&) {}
	virtual const XFColor& getColor() const { return XFColor::white; }
	virtual void setAlpha(float) {}

	void draw();
	XNodeLine();
	~XNodeLine();	//���������������ͷ���Դ

	//virtual void justForTest() {}
	//���������ߵ��ཻ��Ϣ
	void getCrossInfo(const XNodeLine &line, std::vector<XNodeCrossInfo> &ret);
	std::vector<XNodeCrossInfo> getCrossInfo(const XNodeLine &line)
	{
		std::vector<XNodeCrossInfo> ret;
		getCrossInfo(line, ret);
		return ret;
	}
	XNodeMatchInfo simpleMatch(const XNodeLine &line);	//�Ƚ���������˳���ǺϵĽڵ��������÷�����δʵ�֣�

	XNodePointInfo getNodePointInfo(float distance);	//������뿪ʼ�����Ϊdistance�ĵ����Ϣ
	float getDistanceFromStart(int index);				//����ĳ���ڵ������ʼ��֮��ľ���
	XNodePointInfo getNodePointInfo(int index, float distance)	//����ڵ�index�ľ���Ϊdistance�ĵ����Ϣ
	{
		return getNodePointInfo(getDistanceFromStart(index) + distance);
	}
public:
	//isBack�Ƿ񷴷���
	void setBezierSplinePar(XBezierSpline &sp, int index, bool isBack = false)const;	//���õ�index����ı��������ߵĲ���
	//��ȡ����ϵ��������ֵԽ�����ϵ��Խ��
	float getDeformityRate()const;
};
#if WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}
#endif