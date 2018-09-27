#ifndef _JIA_XGRAPHELEMENT_
#define _JIA_XGRAPHELEMENT_
#include "XMath\XVector2.h"
#include <vector>
namespace XE{
extern int gXGraphIndex;
//Ԫ����Ϣ
class XGraphElement
{
private:
public:
	int m_id;
	XVec2 m_position;	//��ǰͼԪ���ڵ�λ��
	std::vector<XGraphElement *> m_neighbourElement;	//�ٽ���ͼԪ
	XGraphElement()
	{
		m_id = gXGraphIndex;
		++ gXGraphIndex;
	}
	XGraphElement(const XVec2& pos)
		:m_position(pos)
	{
		m_id = gXGraphIndex;
		++ gXGraphIndex;
	}
	//XGraphElement(float x,float y)
	//	:m_position(x,y)
	//{
	//	m_id = gXGraphIndex;
	//	++ gXGraphIndex;
	//}
	std::vector<float> m_distance;	//����������Ԫ�صľ���
};
//�ߵ���Ϣ
class XGraphEdge
{
private:
public:
	XGraphElement *m_a;		//�ߵ�һ����
	XGraphElement *m_b;		//�ߵ���һ����
	int m_counter;	//������
	XGraphEdge()
		:m_a(NULL)
		, m_b(NULL)
		, m_counter(0)
	{}
	XGraphEdge(XGraphElement *A,XGraphElement *B)
		:m_a(A)
		,m_b(B)
		,m_counter(0)
	{}
	bool isTheSame(const XGraphElement *A,const XGraphElement *B)const
	{
		return (m_a == A && m_b == B) || (m_b == A && m_a == B);
	}
};
}
#endif