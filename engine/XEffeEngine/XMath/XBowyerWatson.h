#ifndef _JIA_XBOWYERWATSON_
#define _JIA_XBOWYERWATSON_
//�ý�������Զ����˳�����
#include <list> 
#include <vector>
#include "XMath\XVector2.h"
namespace XE{
class XLineEx{
public:
	int m_p0;
	int m_p1;
	XLineEx()
		:m_p0(0)
		, m_p1(0)
	{}
	XLineEx(const int &p0, const int &p1)
		:m_p0(p0)
		, m_p1(p1)
	{}
};
class XTriangle{
public:
	int m_p0;
	int m_p1;
	int m_p2;
	XTriangle(const int &p0, const int &p1, const int &p2)
		:m_p0(p0)
		, m_p1(p1)
		, m_p2(p2)
	{}
	bool findCommonLine(const XTriangle &tg,int &index)	//�غϵ��ߵı��
	{
		if((m_p0 == tg.m_p0 || m_p0 == tg.m_p1 || m_p0 == tg.m_p2) && 
			(m_p1 == tg.m_p0 || m_p1 == tg.m_p1 || m_p1 == tg.m_p2))
		{
			index = 0;
			return true;
		}
		if((m_p1 == tg.m_p0 || m_p1 == tg.m_p1 || m_p1 == tg.m_p2) && 
			(m_p2 == tg.m_p0 || m_p2 == tg.m_p1 || m_p2 == tg.m_p2))
		{
			index = 1;
			return true;
		}
		if((m_p2 == tg.m_p0 || m_p2 == tg.m_p1 || m_p2 == tg.m_p2) && 
			(m_p0 == tg.m_p0 || m_p0 == tg.m_p1 || m_p0 == tg.m_p2))
		{
			index = 2;
			return true;
		}
		return false; 
	}
};
struct TInfo
{
	int p0;
	int p1;
	XVec2 m_center;	//���Բ���ĵ�
	float m_radiu;		//���Բ�뾶 r��ƽ����Ϊ�˽�ʡ������
	TInfo(int a, int b, float r, const XVec2& c)
		:p0(a)
		, p1(b)
		, m_radiu(r)
		, m_center(c)
	{}
	bool checkInCircle(const XVec2& p)const{return m_center.getLengthSqure(p) <= m_radiu;}
};
struct XPointInfo
{
	XVec2 m_position;
	std::list<int> m_lines;
	std::list<TInfo> m_triangles;
	XPointInfo(const XVec2& p)
		:m_position(p)
	{}
};
class XBowyerWatson{
private:
	void calTriangleInfo(int p0,int p1,int p2,float &r,XVec2& c);	//���������ε��������
public:
	XBowyerWatson(){clearBowyerWatson();}
	~XBowyerWatson(){clearBowyerWatson();}
	void clearBowyerWatson();

	void createHelperPoint(const XVec2& pt1, const XVec2& pt2, const XVec2& pt3, const XVec2& pt4);

	void addNewPoint(const XVec2& pt);
	void updateNewPoint();

	const std::vector<XPointInfo>& getPointInfo()const{return m_points;}
private:
	int addBowyerWatsonPoint(const XVec2& pt);	//���ص�ı��

	void addBowyerWatsonLine(int p0,int p1);
	void delBowyerWatsonLine(int p0,int p1);

	void addBowyerWatsonTriangle(int p0,int p1,int p2);
	void delBowyerWatsonTriangle(int p0,int p1,int p2);

	void processNewPoint(const XVec2& pt);
	bool checkTriangleLinesExist(const int &pt1,const int &pt2,const int &pt3);
	bool checkTriangleLinesExist(const int &pt0,const int &pt1);
private:
	//��������Ż�Ϊ�ռ任ʱ��ķ�ʽ��ʹ�õ���������б�����б����Ż�����
	std::vector<XPointInfo> m_points;	//���е����Ϣ
	std::vector<XTriangle> m_commonTriangleVector;	//���е����Ϣ

	std::vector<XVec2>	m_lstAddPointList;

	int mHelperPoints[4];
};
}
#endif