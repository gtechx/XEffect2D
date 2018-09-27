#ifndef _JIA_XGRAPH_
#define _JIA_XGRAPH_
#include "XGraphElement.h"
#include <deque>
//ע��:
//1�������Ż�ȫ�����㷨����������
//2�������Ż�Ϊ�����ߵķ�ʽ���·�Դﵽ�ϴ����������
//3����·����ʱ�ԽǶ�С�����ȣ�����ʹ�ý������
namespace XE{
class XGraph
{
private:
	std::vector<XGraphElement *> m_graphElements;	//ͼԪ�ļ���
	std::vector<XGraphEdge> m_edges;	//���еı�
	void createEdge(XGraphElement *A, XGraphElement * B)	//������Ԫ�ؽ�����
	{
		A->m_neighbourElement.push_back(B);
		B->m_neighbourElement.push_back(A);
	}
	//Ѱ�ҵ�A��B������������㣬�����ؾ���
	bool getIslandDis(const std::vector<int> &A, const std::vector<int> &B, 
		int &indexA, int &indexB, float &dis);
	std::vector<std::deque<XGraphElement *>> m_tmpPlanRoads;	//��ʱ��·�������м���
public:
	int getElementsSum()const { return m_graphElements.size(); }
	const XGraphElement *getElement(int index)const
	{
		if (index >= 0 && index < m_graphElements.size()) return m_graphElements[index];
		return NULL;
	}
	XVec2 getElementsPos(int index)const
	{
		if (index >= 0 && index < m_graphElements.size()) return m_graphElements[index]->m_position;
		return XVec2::zero;
	}
	bool getIsRoadEndpoint(int index)const
	{//�жϽڵ��Ƿ�Ϊ·��Ķ˵�
		if (index < 0 || index >= m_graphElements.size()) return false;
		for (auto it = m_roads.begin(); it != m_roads.end(); ++it)
		{
			if (*(it->begin()) == m_graphElements[index] ||
				*(it->end() - 1) == m_graphElements[index]) return true;
		}
		return false;
	}
public:
	const std::vector<XGraphEdge> &getEdges()const { return m_edges; }
	bool getIsExist(const XGraphElement *A) const
	{//�����ж��������Ƿ����
		for (int i = 0; i < m_graphElements.size(); ++i)
		{
			if (m_graphElements[i] == A) return true;
		}
		return false;
	}
	static bool getIsEdge(const XGraphElement *A, const XGraphElement *B)	//�����Ƿ����A��B�ı�
	{
		//����жϱ��Ƿ����
		if (A->m_neighbourElement.size() <= B->m_neighbourElement.size())
		{
			for (std::vector<XGraphElement *>::const_iterator it = A->m_neighbourElement.begin(); it != A->m_neighbourElement.end(); ++it)
			{
				if (*it == B) return true;
			}
		}
		else
		{
			for (std::vector<XGraphElement *>::const_iterator it = B->m_neighbourElement.begin(); it != B->m_neighbourElement.end(); ++it)
			{
				if (*it == A) return true;
			}
		}
		return false;
	}
	void addAElement(const XVec2& pos)		//��ͼ������һԪ�أ�ֻ������һ��Ԫ�ز���������Ӧ�ı�
	{
		XGraphElement *tmp = XMem::createMem<XGraphElement>();
		if (tmp == NULL) return;
		tmp->m_position = pos;
		m_graphElements.push_back(tmp);
	}
	void changeElement(int index, const XVec2& pos)	//�ı�ĳһ��Ԫ�����ڵ�λ��(��δ���)
	{
		if (index < 0 || index >= m_graphElements.size()) return;
		m_graphElements[index]->m_position = pos;
	}
	void addAElementAndConnect(const XVec2& pos);	//��ͼ������һԪ�ز���ԭ����Ԫ�ؽ����������
	void draw();

	void clearAllEdge();	//������еı�
	void clearAllElement();	//������е�Ԫ
	void clearOneElement(int index);	//���ָ����Ԫ
	int getCloseElement(const XVec2& pos);	//��ȡ��ָ��λ�������Ԫ������

	void calculateEdge();	//���ݵ����ߣ�ʹ�����е�����һ��·������̣������Ż�����
	//ԭ��1����ȫ����
	//2�����ظ��ı�
	//3��·������������
	//4��·�Ĺս�С��ָ��ֵ�����ڴ�ǰ���¾���С
	//enableCross �Ƿ�������
	//withEx���Ƿ�ʹ���Ż����㷨���Ż����㷨���������о�������
	void roadPlanning(bool enableCross, bool withEx);	//��ͼ���Ϊ·���㷨
	std::vector<std::deque<XGraphElement *>> getRoads()const { return m_roads; }
	const std::vector<std::deque<XGraphElement *>> &getRoads() { return m_roads; }
private:
	int m_roadsMaxLen;	//·�����·�ĳ���
	std::vector<std::deque<XGraphElement *>> m_roads;	//���·֮�����յĽ��
	std::vector<std::deque<XGraphElement *>> m_tmpRoads;	//���·֮�����յĽ��
	//���ݵ�order�п��ܽ��ڵ�index���뵽roads�Ķ����У������Ƿ�ɹ�
	void addElementToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//����·�Ƿ��Ѿ�ȫ����
	bool isAllCover(const std::vector<std::deque<XGraphElement *>> &roads);
	//�жϽڵ�index�Ƿ��Ѿ���·��
	bool isInRoads(int index, const std::vector<std::deque<XGraphElement *>> &roads)
	{
		return isInMiddle(m_graphElements[index], roads);
	}
	//�жϱ�A-B�Ƿ���·�д���
	bool isEdgeInRoads(const XGraphElement *A, const XGraphElement *B, const std::vector<std::deque<XGraphElement *>> &roads);
	void optimizationRoad(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);	//�Ż�·��
	bool isAngleOK(const XGraphElement *A,
		const XGraphElement *B, const XGraphElement *C)	//���ߵĽǶ��Ƿ����Ҫ��
	{
		float angle = XMath::angleAbbreviation(A->m_position.getAngle(B->m_position) -
			B->m_position.getAngle(C->m_position));
		if (angle > PI) angle = PI2 - angle;
		return (angle < PI * 0.3333f);	//60��
	}
	bool isInMiddle(const XGraphElement *A, const std::vector<std::deque<XGraphElement *>> &roads);	//�жϵ�A�Ƿ����������·���м䣬�����ж��Ƿ�����·�����ཻ
	int getMaxRoadLen(const std::vector<std::deque<XGraphElement *>> &roads);
	void updateRoadPlanning(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//������ǿ����ʵ��
	void addEdgeToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
	//������㷨�Ƕ������㷨���Ż�,�µ��㷨��ȫ���ھɵ��㷨
	void addEdgeToRoadsEx(int index, bool enableCross);
	void tryMergeRoad(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross);
};
}
#endif