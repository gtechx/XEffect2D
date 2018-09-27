#include "XStdHead.h"
#include "XGraph.h"
namespace XE {
void XGraph::addAElementAndConnect(const XVec2& pos)
{
	XGraphElement *tmp = XMem::createMem<XGraphElement>();
	if (tmp == NULL) return;
	tmp->m_position = pos;
	int index = getCloseElement(pos);
	m_graphElements.push_back(tmp);
	if (index >= 0)
		createEdge(tmp, m_graphElements[index]);
}
void XGraph::draw()
{
	//���ͼԪ�еĵ�
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		XRender::drawPoint((*it)->m_position, 3, XFColor::red);
	}
	//���ͼԪ�еı�
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		for (std::vector<XGraphElement *>::iterator jt = (*it)->m_neighbourElement.begin(); jt != (*it)->m_neighbourElement.end(); ++jt)
		{
			if ((*it)->m_id >= (*jt)->m_id)
				continue;	//�����ظ���棬��������һ��·���������
			XRender::drawLine((*it)->m_position, (*jt)->m_position);
		}
	}
}
void XGraph::clearAllEdge()	//������еı�
{
	for (std::vector<XGraphElement *>::iterator it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		(*it)->m_neighbourElement.clear();
	}
	m_edges.clear();
}
void XGraph::clearAllElement()	//������е�Ԫ
{
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		XMem::XDELETE(m_graphElements[i]);
	}
	m_graphElements.clear();
	m_edges.clear();
	m_roads.clear();
}
void XGraph::clearOneElement(int index)	//���ָ����Ԫ
{
	if (index < 0 || index >= m_graphElements.size()) return;
	//������б�����õ㹹�ɵı�
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		if (i == index) continue;
		for (int j = 0; j < m_graphElements[i]->m_neighbourElement.size(); ++j)
		{
			//����1���������еıߣ������ͬ�������
			//if(m_graphElements[i]->m_neighbourElement[j] == m_graphElements[index])
			//{
			//	m_graphElements[i]->m_neighbourElement.erase(m_graphElements[i]->m_neighbourElement.begin() + j);
			//	-- j;
			//}
			//����2�����ǲ����ظ�������ֻ��Ҫ���һ��
			if (m_graphElements[i]->m_neighbourElement[j] != m_graphElements[index]) continue;
			m_graphElements[i]->m_neighbourElement.erase(m_graphElements[i]->m_neighbourElement.begin() + j);
			break;
		}
	}
	XMem::XDELETE(m_graphElements[index]);
	m_graphElements.erase(m_graphElements.begin() + index);
}
int XGraph::getCloseElement(const XVec2& pos)	//��ȡ��ָ��λ�������Ԫ������
{
	if (m_graphElements.size() == 0) return -1;
	int index = 0;
	float dis = m_graphElements[0]->m_position.getLengthSqure(pos);
	float tmpDis;
	for (int i = 1; i < m_graphElements.size(); ++i)
	{
		tmpDis = m_graphElements[i]->m_position.getLengthSqure(pos);
		if (tmpDis < dis)
		{
			dis = tmpDis;
			index = i;
		}
	}
	return index;
}
//��������㷨�����Ż������Խ�������Ȼ�����������������Ż�
bool XGraph::getIslandDis(const std::vector<int> &A,
	const std::vector<int> &B,
	int &indexA, int &indexB, float &dis)
{
	if (A.size() <= 0 || B.size() <= 0) return false;
	float tmpDis;
	//	bool isFirst = true;
	indexA = A[0];
	indexB = B[0];
	dis = m_graphElements[indexA]->m_distance[indexB];

	for (std::vector<int>::const_iterator it = A.begin(); it != A.end(); ++it)
	{
		for (std::vector<int>::const_iterator jt = B.begin(); jt != B.end(); ++jt)
		{
			tmpDis = m_graphElements[*it]->m_distance[*jt];
			if (tmpDis < dis)
			{
				indexA = *it;
				indexB = *jt;
				dis = tmpDis;
			}
		}
	}
	return true;
}
void XGraph::calculateEdge()
{//���㷨��ʵ�֣����������ܣ�����2��ÿ���㶼ѡ�����·��������ֱ�����еĵ㶼������Ϊֹ����δʵ�֣���Ҫ��֤�����γ�һ��ͼ��
	clearAllEdge();
	if (m_graphElements.size() <= 0) return;	//���ݲ�����ֱ�ӷ���
	std::vector<std::vector<int>> islands;
	islands.resize(m_graphElements.size());
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		islands[i].push_back(i);
	}
	//ʹ�ÿռ任ʱ��ķ�ʽ���㷨�����Ż�����׼������
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		m_graphElements[i]->m_distance.clear();
		for (int j = 0; j < m_graphElements.size(); ++j)
		{
			if (j == i)
			{
				m_graphElements[i]->m_distance.push_back(0.0f);
			}
			else
				if (j < i)
				{
					m_graphElements[i]->m_distance.push_back(m_graphElements[j]->m_distance[i]);
				}
				else
				{
					m_graphElements[i]->m_distance.push_back(m_graphElements[i]->m_position.getLengthSqure(m_graphElements[j]->m_position));
				}
		}
	}

	int indexA, indexB;
	float dis;
	int minIndexA = 0, minIndexB = 0;
	float minDis = 0.0f;
	int minIslandIndexA = 0, minIslandIndexB = 0;
	while (islands.size() > 1)
	{
		//Ѱ�ҵ������������
		for (int i = 0; i < int(islands.size()) - 1; ++i)
		{
			for (int j = i + 1; j < islands.size(); ++j)
			{
				if (i == 0 && j == 1)
				{
					getIslandDis(islands[i], islands[j], minIndexA, minIndexB, minDis);
					minIslandIndexA = i;
					minIslandIndexB = j;
				}
				else
				{
					getIslandDis(islands[i], islands[j], indexA, indexB, dis);
					if (dis < minDis)
					{
						minDis = dis;
						minIndexA = indexA;
						minIndexB = indexB;
						minIslandIndexA = i;
						minIslandIndexB = j;
					}
				}
			}
		}
		//������,�����������
		createEdge(m_graphElements[minIndexA], m_graphElements[minIndexB]);
		if (m_graphElements[minIndexA]->m_id >= m_graphElements[minIndexB]->m_id)
			m_edges.push_back(XGraphEdge(m_graphElements[minIndexB], m_graphElements[minIndexA]));
		else
			m_edges.push_back(XGraphEdge(m_graphElements[minIndexA], m_graphElements[minIndexB]));
		//�ϲ�������
		islands[minIslandIndexA].insert(islands[minIslandIndexA].end(),
			islands[minIslandIndexB].begin(), islands[minIslandIndexB].end());
		islands.erase(islands.begin() + minIslandIndexB);
	}
}
void XGraph::roadPlanning(bool enableCross, bool withEx)
{
	m_roads.clear();
	m_roadsMaxLen = 0;
	if (m_graphElements.size() <= 1)//û���㹻������ֱ�ӷ���
		return;
	m_tmpPlanRoads.clear();
	if (withEx) addEdgeToRoadsEx(0, enableCross);	//���ߵķ�ʽ����
	else addElementToRoads(0, m_tmpPlanRoads, enableCross);	//����û�п����ߺϲ����㷨����ɽ�������ţ��Ľ�����Ϊʹ�ñ����ߵķ�ʽ����δʵ�֣�
}
void XGraph::addElementToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{//���波�Բ����
	for (int i = 0; i < m_graphElements[index]->m_neighbourElement.size(); ++i)
	{
		if (isEdgeInRoads(m_graphElements[index], m_graphElements[index]->m_neighbourElement[i], roads))
		{
			//	if(index < int(m_graphElements.size()) - 1)
			//		addElementToRoads(index + 1,roads,enableCross);
			continue;
		}
		//�����½���һ��·��
		std::deque<XGraphElement *> tmp;
		tmp.push_back(m_graphElements[index]);
		tmp.push_back(m_graphElements[index]->m_neighbourElement[i]);
		roads.push_back(tmp);
		if (isAllCover(roads))
		{//������б��Ѿ�ȫ���ǣ���ʼ�����Ƿ�Ϊ���ŵ�·
			updateRoadPlanning(roads, enableCross);
		}
		else
		{//��������Ѿ�������ǰ���Ž⣬�Ͳ��ü�����
			//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
			if (index < int(m_graphElements.size()) - 1)
				addElementToRoads(index + 1, roads, enableCross);
		}
		roads.pop_back();	//��ԭ����
		//���Խ��õ���뵽����������
		for (int j = 0; j < roads.size(); ++j)
		{
			if (roads[j][0] == m_graphElements[index]->m_neighbourElement[i])
			{//���뵽ͷ��ǰ	
				if (isAngleOK(roads[j][1], roads[j][0], m_graphElements[index]) &&
					(enableCross || isInMiddle(m_graphElements[index]->m_neighbourElement[i], roads)))
				{//���Ƕ��Ƿ����
					roads[j].push_front(m_graphElements[index]);
					if (isAllCover(roads))
					{//������б��Ѿ�ȫ���ǣ���ʼ�����Ƿ�Ϊ���ŵ�·
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//��������Ѿ�������ǰ���Ž⣬�Ͳ��ü�����
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_front();	//��ԭ����
				}
			}
			if (*(roads[j].end() - 1) == m_graphElements[index]->m_neighbourElement[i])
			{//���뵽β�Ժ�
				if (isAngleOK(*(roads[j].end() - 2), *(roads[j].end() - 1), m_graphElements[index]) &&
					(enableCross || isInMiddle(m_graphElements[index]->m_neighbourElement[i], roads)))
				{//���Ƕ��Ƿ����
					roads[j].push_back(m_graphElements[index]);
					if (isAllCover(roads))
					{//������б��Ѿ�ȫ���ǣ���ʼ�����Ƿ�Ϊ���ŵ�·
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//��������Ѿ�������ǰ���Ž⣬�Ͳ��ü�����
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_back();	//��ԭ����
				}
			}
			if (roads[j][0] == m_graphElements[index])
			{//���뵽ͷ��ǰ	
				if (isAngleOK(roads[j][1], roads[j][0], m_graphElements[index]->m_neighbourElement[i]) &&
					(enableCross || isInMiddle(m_graphElements[index], roads)))	//60��
				{//���Ƕ��Ƿ����
					roads[j].push_front(m_graphElements[index]->m_neighbourElement[i]);
					if (isAllCover(roads))
					{//������б��Ѿ�ȫ���ǣ���ʼ�����Ƿ�Ϊ���ŵ�·
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//��������Ѿ�������ǰ���Ž⣬�Ͳ��ü�����
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_front();	//��ԭ����						
				}
			}
			if (*(roads[j].end() - 1) == m_graphElements[index])
			{//���뵽β�Ժ�
				if (isAngleOK(*(roads[j].end() - 2), *(roads[j].end() - 1), 
					m_graphElements[index]->m_neighbourElement[i]) &&
					(enableCross || isInMiddle(m_graphElements[index], roads)))
				{//���Ƕ��Ƿ����
					roads[j].push_back(m_graphElements[index]->m_neighbourElement[i]);
					if (isAllCover(roads))
					{//������б��Ѿ�ȫ���ǣ���ʼ�����Ƿ�Ϊ���ŵ�·
						updateRoadPlanning(roads, enableCross);
					}
					else
					{//��������Ѿ�������ǰ���Ž⣬�Ͳ��ü�����
						//if((roads.size() < m_roads.size() || m_roads.size() == 0) && index < int(m_graphElements.size()) - 1)
						if (index < int(m_graphElements.size()) - 1)
							addElementToRoads(index + 1, roads, enableCross);
					}
					roads[j].pop_back();	//��ԭ����
				}
			}
		}
	}
}
bool XGraph::isAllCover(const std::vector<std::deque<XGraphElement *>> &roads)
{//���б߶��Ѿ���·��
	for (auto it = m_graphElements.begin(); it != m_graphElements.end(); ++it)
	{
		for (auto jt = (*it)->m_neighbourElement.begin(); jt != (*it)->m_neighbourElement.end(); ++jt)
		{
			if ((*it)->m_id >= (*jt)->m_id)
				continue;	//�����ظ���棬��������һ��·���������
			if (!isEdgeInRoads((*it), (*jt), roads)) return false;
		}
	}
	return true;
}
bool XGraph::isEdgeInRoads(const XGraphElement *A, const XGraphElement *B,
	const std::vector<std::deque<XGraphElement *>> &roads)
{
	for (auto it = roads.begin(); it != roads.end(); ++it)
	{
		if (it->size() == 0) continue;
		for (auto jt = it->begin(); jt != it->end() - 1; ++jt)
		{
			if ((*jt == A && *(jt + 1) == B) ||
				(*jt == B && *(jt + 1) == A))
				return true;
		}
	}
	return false;
}
bool XGraph::isInMiddle(const XGraphElement *A, const std::vector<std::deque<XGraphElement *>> &roads)
{
	for (auto it = roads.begin(); it != roads.end(); ++it)
	{
		if (it->size() < 2) continue;
		for (auto jt = it->begin() + 1; jt != it->end() - 1; ++jt)
		{
			if (*jt == A) return true;
		}
	}
	return false;
}
void XGraph::optimizationRoad(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)	//�Ż�·��
{//�������ӵ�·��������
	for (int i = 0; i < int(roads.size()) - 1; ++i)
	{
		for (int j = i + 1; j < roads.size(); ++j)
		{
			if (roads[i][0] == roads[j][0] && isAngleOK(roads[i][1], roads[i][0], roads[j][1]) &&
				(enableCross || !isInMiddle(roads[i][0], roads)))
			{//ͷͷ
			//	for(int k = 1;k < roads[j].size();++ k)
			//	{
			//		roads[i].push_front(roads[j][k]);
			//	}
				if (roads[j].size() > 0)
				{
					for (auto it = roads[j].begin() + 1; it != roads[j].end(); ++it)
					{
						m_tmpPlanRoads[i].push_front(*it);
					}
				}
				roads.erase(roads.begin() + j);
				i = -1;
				break;
			}
			else
				if (*(roads[i].end() - 1) == roads[j][0] &&
					isAngleOK(roads[j][1], roads[j][0], *(roads[i].end() - 2)) &&
					(enableCross || !isInMiddle(roads[j][0], roads)))
				{//βͷ
					roads[i].insert(roads[i].end(), roads[j].begin() + 1, roads[j].end());
					roads.erase(roads.begin() + j);
					i = -1;
					break;
				}
				else
					if (roads[i][0] == *(roads[j].end() - 1) &&
						isAngleOK(roads[i][1], roads[i][0], *(roads[j].end() - 2)) &&
						(enableCross || !isInMiddle(roads[i][0], roads)))
					{//ͷβ
						roads[i].insert(roads[i].begin(), roads[j].begin(), roads[j].end() - 1);
						roads.erase(roads.begin() + j);
						i = -1;
						break;
					}
					else
						if (*(roads[i].end() - 1) == *(roads[j].end() - 1) &&
							isAngleOK(*(roads[i].end() - 2),
								*(roads[i].end() - 1), *(roads[j].end() - 2)) &&
								(enableCross || !isInMiddle(*(roads[i].end() - 1), roads)))
						{//ββ
						//	for(int k = int(roads[j].size()) - 2;k >= 0;-- k)
						//	{
						//		roads[i].push_back(roads[j][k]);
						//	}
							if (roads[j].size() >= 2)
							{
								for (auto it = roads[j].end() - 2;; --it)
								{
									roads[i].push_back(*it);
									if (it == roads[j].begin()) break;
								}
							}
							roads.erase(roads.begin() + j);
							i = -1;
							break;
						}
		}
	}
}
int XGraph::getMaxRoadLen(const std::vector<std::deque<XGraphElement *>> &roads)
{
	int ret = roads[0].size();
	for (int i = 1; i < roads.size(); ++i)
	{
		if (roads[i].size() > ret) ret = roads[i].size();
	}
	return ret;
}
void XGraph::updateRoadPlanning(std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{
	m_tmpRoads = roads;
	optimizationRoad(m_tmpRoads, enableCross);
	if (m_tmpRoads.size() < m_roads.size() || m_roads.size() == 0)
	{//��������
		m_roadsMaxLen = getMaxRoadLen(m_tmpRoads);
		m_roads = m_tmpRoads;	//��¼���ŵĽ��
	}
	else
		if (m_tmpRoads.size() == m_roads.size())
		{//�ȳ�ʱ����󳤶�Ϊ��׼
			int maxLen = getMaxRoadLen(m_tmpRoads);
			if (maxLen > m_roadsMaxLen)
			{
				m_roadsMaxLen = getMaxRoadLen(m_tmpRoads);
				m_roads = m_tmpRoads;	//��¼���ŵĽ��
			}
		}
}
void XGraph::addEdgeToRoads(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{//�ݹ�������п��ܵ�������ҵ��������������Ž⣬�����Ż�
	//���������е�������
	for (int j = 0; j < roads.size(); ++j)
	{
		if (roads[j][0] == m_edges[index].m_a)
		{//���뵽ͷ��ǰ	
			if (isAngleOK(roads[j][1], roads[j][0], m_edges[index].m_b) &&
				(enableCross || isInMiddle(m_edges[index].m_a, roads)))
			{//���Ƕ��Ƿ����
				roads[j].push_front(m_edges[index].m_b);
				if (index >= int(m_edges.size()) - 1)//�������
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_front();	//��ԭ����
			}
		}
		if (*(roads[j].end() - 1) == m_edges[index].m_a)
		{//���뵽β�Ժ�
			if (isAngleOK(*(roads[j].end() - 2),
				*(roads[j].end() - 1), m_edges[index].m_b) &&
				(enableCross || isInMiddle(m_edges[index].m_a, roads)))
			{//���Ƕ��Ƿ����
				roads[j].push_back(m_edges[index].m_b);
				if (index >= int(m_edges.size()) - 1)//�������
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_back();	//��ԭ����
			}
		}
		if (roads[j][0] == m_edges[index].m_b)
		{//���뵽ͷ��ǰ	
			if (isAngleOK(roads[j][1], roads[j][0], m_edges[index].m_a) &&
				(enableCross || isInMiddle(m_edges[index].m_b, roads)))	//60��
			{//���Ƕ��Ƿ����
				roads[j].push_front(m_edges[index].m_a);
				if (index >= int(m_edges.size()) - 1)//�������
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_front();	//��ԭ����						
			}
		}
		if (*(roads[j].end() - 1) == m_edges[index].m_b)
		{//���뵽β�Ժ�
			if (isAngleOK(*(roads[j].end() - 2),
				*(roads[j].end() - 1), m_edges[index].m_a) &&
				(enableCross || isInMiddle(m_edges[index].m_b, roads)))
			{//���Ƕ��Ƿ����
				roads[j].push_back(m_edges[index].m_a);
				if (index >= int(m_edges.size()) - 1)//�������
					updateRoadPlanning(roads, enableCross);
				else
					addEdgeToRoads(index + 1, roads, enableCross);
				roads[j].pop_back();	//��ԭ����
			}
		}
	}
	//����һ����	
	std::deque<XGraphElement *> tmp;
	tmp.push_back(m_edges[index].m_a);
	tmp.push_back(m_edges[index].m_b);
	roads.push_back(tmp);
	if (index >= int(m_edges.size()) - 1)//�������
		updateRoadPlanning(roads, enableCross);
	else
		addEdgeToRoads(index + 1, roads, enableCross);
	roads.pop_back();
}
void XGraph::addEdgeToRoadsEx(int index, bool enableCross)
{//ʹ�õ����ϲ��ķ�ʽ
	m_tmpPlanRoads.resize(m_edges.size());
	for (int i = 0; i < m_tmpPlanRoads.size(); ++i)
	{
		m_tmpPlanRoads[i].push_back(m_edges[i].m_a);
		m_tmpPlanRoads[i].push_back(m_edges[i].m_b);
	}
	//����Ȼ�ϲ���·�ߺϲ���1��ֻ��һ��·�ҿ��Խ�����Ȼ��ϣ�2���ж���·����ֻ��һ��·���Խ�����Ȼ��ϣ���δʵ�֣�
	for (int i = 0; i < m_graphElements.size(); ++i)
	{
		if (m_graphElements[i]->m_neighbourElement.size() == 2 &&
			isAngleOK(m_graphElements[i]->m_neighbourElement[0], m_graphElements[i], m_graphElements[i]->m_neighbourElement[1]))
		{//����õ�ֻ��һ��ͨ·����������ͨ·�ĽǶȷ���Ҫ����������·�ϲ�
			int indexA = -1, indexB = -1;
			bool isHeadA = false, isHeadB = false;
			for (int j = 0; j < m_tmpPlanRoads.size(); ++j)
			{
				if (indexA < 0)
				{
					if (m_tmpPlanRoads[j][0] == m_graphElements[i])
					{
						indexA = j;
						isHeadA = true;
					}
					else
						if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i])
						{
							indexA = j;
							isHeadA = false;
						}
				}
				else
					if (indexB < 0)
					{
						if (m_tmpPlanRoads[j][0] == m_graphElements[i])
						{
							indexB = j;
							isHeadB = true;
							break;
						}
						else
							if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i])
							{
								indexB = j;
								isHeadB = false;
								break;
							}
					}
			}
			//�ϲ�������·
			assert(indexA >= 0 && indexB >= 0);
			if (isHeadA)
			{
				if (isHeadB)
				{//ͷͷ
					if (m_tmpPlanRoads[indexB].size() > 0)
					{
						for (auto it = m_tmpPlanRoads[indexB].begin() + 1; it != m_tmpPlanRoads[indexB].end(); ++it)
						{
							m_tmpPlanRoads[indexA].push_front(*it);
						}
					}
				}
				else
				{//ͷβ
					m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].begin(),
						m_tmpPlanRoads[indexB].begin(), m_tmpPlanRoads[indexB].end() - 1);
				}
			}
			else
			{
				if (isHeadB)
				{//βͷ
					m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].end(),
						m_tmpPlanRoads[indexB].begin() + 1, m_tmpPlanRoads[indexB].end());
				}
				else
				{//ββ
					if (m_tmpPlanRoads[indexB].size() >= 2)
					{
						for (std::deque<XGraphElement *>::iterator it = m_tmpPlanRoads[indexB].end() - 2;; --it)
						{
							m_tmpPlanRoads[indexA].push_back(*it);
							if (it == m_tmpPlanRoads[indexB].begin()) break;
						}
					}
				}
			}
			m_tmpPlanRoads.erase(m_tmpPlanRoads.begin() + indexB);
		}
		else
			if (m_graphElements[i]->m_neighbourElement.size() > 2)
			{//������ڶ��ͨ·������ֻ��һ�����е�ͨ·���������
				int sum = 0, indexNA = 0, indexNB = 0;
				for (int j = 0; j < int(m_graphElements[i]->m_neighbourElement.size()) - 1; ++j)
				{
					for (int k = j + 1; k < m_graphElements[i]->m_neighbourElement.size(); ++k)
					{
						if (!isAngleOK(m_graphElements[i]->m_neighbourElement[j],
							m_graphElements[i], m_graphElements[i]->m_neighbourElement[k])) continue;
						indexNA = j;
						indexNB = k;
						++sum;
						if (sum > 1) break;
					}
					if (sum > 1) break;
				}
				if (sum == 1)
				{//ֻ��һ�����е�ͨ·�����Ȼ��ͨ
					int indexA = -1, indexB = -1;
					bool isHeadA = false, isHeadB = false;
					for (int j = 0; j < m_tmpPlanRoads.size(); ++j)
					{
						if (m_tmpPlanRoads[j][0] == m_graphElements[i] &&
							(m_tmpPlanRoads[j][1] == m_graphElements[i]->m_neighbourElement[indexNA] ||
								m_tmpPlanRoads[j][1] == m_graphElements[i]->m_neighbourElement[indexNB]))
						{
							if (indexA < 0)
							{
								indexA = j;
								isHeadA = true;
							}
							else
							{
								indexB = j;
								isHeadB = true;
								break;
							}
						}
						else
							if (*(m_tmpPlanRoads[j].end() - 1) == m_graphElements[i] &&
								(*(m_tmpPlanRoads[j].end() - 2) == m_graphElements[i]->m_neighbourElement[indexNA] ||
									*(m_tmpPlanRoads[j].end() - 2) == m_graphElements[i]->m_neighbourElement[indexNB]))
							{
								if (indexA < 0)
								{
									indexA = j;
									isHeadA = false;
								}
								else
								{
									indexB = j;
									isHeadB = false;
									break;
								}
							}
					}
					//�ϲ�������·
					assert(indexA >= 0 && indexB >= 0);
					if (isHeadA)
					{
						if (isHeadB)
						{//ͷͷ
							if (m_tmpPlanRoads[indexB].size() > 0)
							{
								for (auto it = m_tmpPlanRoads[indexB].begin() + 1; it != m_tmpPlanRoads[indexB].end(); ++it)
								{
									m_tmpPlanRoads[indexA].push_front(*it);
								}
							}
						}
						else
						{//ͷβ
							m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].begin(),
								m_tmpPlanRoads[indexB].begin(), m_tmpPlanRoads[indexB].end() - 1);
						}
					}
					else
					{
						if (isHeadB)
						{//βͷ
							m_tmpPlanRoads[indexA].insert(m_tmpPlanRoads[indexA].end(),
								m_tmpPlanRoads[indexB].begin() + 1, m_tmpPlanRoads[indexB].end());
						}
						else
						{//ββ
							if (m_tmpPlanRoads[indexB].size() >= 2)
							{
								for (std::deque<XGraphElement *>::iterator it = m_tmpPlanRoads[indexB].end() - 2;; --it)
								{
									m_tmpPlanRoads[indexA].push_back(*it);
									if (it == m_tmpPlanRoads[indexB].begin()) break;
								}
							}
						}
					}
					m_tmpPlanRoads.erase(m_tmpPlanRoads.begin() + indexB);
				}
			}
	}
	//����������зǱ�Ȼ�ϲ�����·
	tryMergeRoad(0, m_tmpPlanRoads, enableCross);
}
void XGraph::tryMergeRoad(int index, std::vector<std::deque<XGraphElement *>> &roads, bool enableCross)
{
	bool canMerge = false;
	//std::vector<std::deque<XGraphElement *>> tmpRoads;
	for (int i = index; i < int(roads.size()) - 1; ++i)
	{
		XGraphElement *end = *(roads[i].end() - 1);
		XGraphElement *end1 = *(roads[i].end() - 2);
		for (int j = i + 1; j < roads.size(); ++j)
		{
			if (roads[i][0] == roads[j][0] && isAngleOK(roads[i][1], roads[i][0], roads[j][1]) &&
				(enableCross || !isInMiddle(roads[i][0], roads)))
			{//ͷͷ
				std::deque<XGraphElement *> tmp = roads[j];	//���������ڴ渴�ƵĿ����������Ż���
				if (tmp.size() > 0)
				{
					for (auto it = tmp.begin() + 1; it != tmp.end(); it++)
					{
						roads[i].push_front(*it);
					}
				}
				roads.erase(roads.begin() + j);
				tryMergeRoad(index, roads, enableCross);
				roads[i].erase(roads[i].begin(), roads[i].begin() + (tmp.size() - 1));
				roads.insert(roads.begin() + j, tmp);

				canMerge = true;
			}
			else
				if (end == roads[j][0] &&
					isAngleOK(roads[j][1], roads[j][0], end1) &&
					(enableCross || !isInMiddle(roads[j][0], roads)))
				{//βͷ
					std::deque<XGraphElement *> tmp = roads[j];	//���������ڴ渴�ƵĿ����������Ż���
					roads[i].insert(roads[i].end(), tmp.begin() + 1, tmp.end());
					roads.erase(roads.begin() + j);
					tryMergeRoad(index, roads, enableCross);
					//��ԭ�ֳ�
					roads[i].erase(roads[i].begin() + (roads[i].size() - tmp.size() + 1), roads[i].end());
					roads.insert(roads.begin() + j, tmp);

					canMerge = true;
				}
				else
					if (roads[i][0] == *(roads[j].end() - 1) &&
						isAngleOK(roads[i][1], roads[i][0], *(roads[j].end() - 2)) &&
						(enableCross || !isInMiddle(roads[i][0], roads)))
					{//ͷβ
						std::deque<XGraphElement *> tmp = roads[j];	//���������ڴ渴�ƵĿ����������Ż���
						roads[i].insert(roads[i].begin(), tmp.begin(), tmp.end() - 1);
						roads.erase(roads.begin() + j);
						tryMergeRoad(index, roads, enableCross);
						roads[i].erase(roads[i].begin(), roads[i].begin() + tmp.size() - 1);
						roads.insert(roads.begin() + j, tmp);

						canMerge = true;
					}
					else
						if (end == *(roads[j].end() - 1) &&
							isAngleOK(end1, end, *roads[j].end() - 2) &&
							(enableCross || !isInMiddle(end, roads)))
						{//ββ
							std::deque<XGraphElement *> tmp = roads[j];	//���������ڴ渴�ƵĿ����������Ż���
							if (tmp.size() >= 2)
							{
								for (auto it = tmp.end() - 2;; --it)
								{
									roads[i].push_back(*it);
									if (it == tmp.begin()) break;
								}
							}
							roads.erase(roads.begin() + j);
							tryMergeRoad(index, roads, enableCross);
							roads[i].erase(roads[i].begin() + (roads[i].size() - tmp.size() + 1), roads[i].end());
							roads.insert(roads.begin() + j, tmp);
							canMerge = true;
						}
		}
		if (canMerge) return;	//����ܽ�϶����������Բ������Ž�
	}
	//���û�п��Ժϲ��ģ��������жϽ���Ƿ�Ϊ����
	if (canMerge) return;
	if (roads.size() < m_roads.size() || m_roads.size() == 0)
	{//��������
		m_roadsMaxLen = getMaxRoadLen(roads);
		m_roads = roads;	//��¼���ŵĽ��
	}
	else
		if (roads.size() == m_roads.size())
		{//�ȳ�ʱ����󳤶�Ϊ��׼
			int maxLen = getMaxRoadLen(roads);
			if (maxLen > m_roadsMaxLen)
			{
				m_roadsMaxLen = maxLen;
				m_roads = roads;	//��¼���ŵĽ��
			}
		}
}
}