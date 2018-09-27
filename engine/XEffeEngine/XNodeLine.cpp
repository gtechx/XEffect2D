#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNodeLine.h"
#include "XResourcePack.h"
#include "XObjectManager.h"
namespace XE{
XNodeLine::XNodeLine()
	:m_myOrder(0)
	,m_leadMode(LEAD_MODE_FOREWORD)	//���ߵ�����ģʽ
	,m_lineLength(0.0f)					//���ߵ��ܳ���
	,m_bezLineLength(0.0f)
	//,m_nodeSum(0)						//���������е������ڵ������
	,m_offsetPosition(0.0f)
	,m_scale(1.0f)
	,m_isLoop(XFalse)
	,m_isVisible(XTrue)
	,m_withAngleSmooth(true)
	,m_withBezier(false)
{
//	m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
//	if(m_node == NULL) printf("Memory error!\n");
//	m_bezierPoint = XMem::createArrayMem<XVec2>(m_maxNodeSum * 2);
//	if(m_bezierPoint == NULL) printf("Memory error!\n");
//	try
//	{
//		m_cp = new XSCounter;
//		if(m_cp == NULL) printf("Memory error!\n");
//	}catch(...)
//	{
//		printf("Memory error!\n");
//	}

	m_specialPoint.isEnable = XFalse;
	clearUp();
	m_objType = OBJ_NODELINE;
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
}
XNodeLine::~XNodeLine()
{
//	if(m_cp != NULL && -- m_cp->m_counter <= 0)
//	{//����û��������Ҫ�ͷ�
//		XMem::XDELETE_ARRAY(m_node);
//		XMem::XDELETE_ARRAY(m_bezierPoint);
//		XMem::XDELETE(m_cp);
//	}
	release();
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
}
void XNodeLine::addOneNode(const XVec2& p)		//������ͷ�����һ����
{
	int prevNodeNumber;
	XNode tmp(p);
	if(m_node.size() > 0)
	{//��д��һ��������������Ϣ
		if(tmp.myPoint.getLengthSqure(m_node[int(m_node.size()) - 1].myPoint) <= 0.0f)
			return;	//��̫��������ʱ���ܲ���
		prevNodeNumber = m_node.size() - 1;
		m_node[prevNodeNumber].setNext(tmp);
		m_lineLength += tmp.toPreviousLength;
		
		if(!m_isLoop)
		{
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			tmp.toNextAngle = m_node[prevNodeNumber].toNextAngle;
			tmp.toNextCos = m_node[prevNodeNumber].toNextCos;
			tmp.toNextSin = m_node[prevNodeNumber].toNextSin;
			if(m_node.size() == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = tmp.toPreviousAngle;
				m_node[0].toPreviousCos = tmp.toPreviousCos;
				m_node[0].toPreviousSin = tmp.toPreviousSin;
			}
		}else
		{
			m_lineLength -= m_node[0].toPreviousLength;
			tmp.setNext(m_node[0]);
			m_lineLength += m_node[0].toPreviousLength;
		}
	}
	m_node.push_back(tmp);
	m_needUpdateData = XTrue;
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd || m_node.size() < 2) return;	//����Ҫ����
	//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
	//	if(m_isLoop)
	//	{//ѭ��������Ҫ����
	//	}else
	//	{//��ѭ��������Ҫ����
	//	}
	}else
	{//����
		if(m_isLoop)
		{//ѭ��
			m_specialPoint.curLength += m_node[int(m_node.size()) - 1].toPreviousLength;
			m_specialPoint.upNodeLength += m_node[int(m_node.size()) - 1].toPreviousLength;
		}else
		{//��ѭ��
			m_specialPoint.curLength += m_node[int(m_node.size()) - 1].toPreviousLength;
			m_specialPoint.upNodeLength += m_node[int(m_node.size()) - 1].toPreviousLength;
		}
	}
}
void XNodeLine::setOneNode(const XVec2& p,int nodeOrder)
{
	if(nodeOrder < 0 || nodeOrder >= m_node.size()) return;
	float tmpOldLineLength = m_lineLength;
	float tmpOldLoopLen = m_node[0].toPreviousLength;
	if(nodeOrder == 0)
	{//ͷ
		//������Ҫ�ж�0�ڵ������ڽڵ��ľ����Ƿ�̫С��������������(��δ���)
		m_node[0].myPoint = p;
		if(m_node.size() > 1)
		{
			m_lineLength -= m_node[0].toNextLength;
			m_node[0].setNext(m_node[1]);
			m_lineLength += m_node[0].toNextLength;
			if(!m_isLoop)
			{
				if(m_node.size() > 1)
				{//ͷ�ĵ㱣�ֺ�һ����ķ���
					m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
					m_node[0].toPreviousCos = m_node[1].toPreviousCos;
					m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				}
				if(m_node.size() == 2)
				{
					m_node[1].toNextAngle = m_node[0].toNextAngle;
					m_node[1].toNextCos = m_node[0].toNextCos;
					m_node[1].toNextSin = m_node[0].toNextSin;
				}
			}else
			{
				m_lineLength -= m_node[0].toPreviousLength;
				m_node[int(m_node.size()) - 1].setNext(m_node[0]);
				m_lineLength += m_node[0].toPreviousLength;
			}
		}
	}else
	if(nodeOrder == int(m_node.size()) - 1)
	{//β
		//������Ҫ�ж�β�ڵ������ڽڵ��ľ����Ƿ�̫С��������������(��δ���)
		m_node[nodeOrder].myPoint = p;		//��¼��ǰ�ĵ����Ϣ
		if(m_node.size() > 1)
		{//��д��һ��������������Ϣ
			m_lineLength -= m_node[nodeOrder].toPreviousLength;
			m_node[nodeOrder - 1].setNext(m_node[nodeOrder]);
			m_lineLength += m_node[nodeOrder].toPreviousLength;
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			if(!m_isLoop)
			{
				m_node[nodeOrder].toNextAngle = m_node[nodeOrder - 1].toNextAngle;
				m_node[nodeOrder].toNextCos = m_node[nodeOrder - 1].toNextCos;
				m_node[nodeOrder].toNextSin = m_node[nodeOrder - 1].toNextSin;
				if(m_node.size() == 2)
				{//ͷ�ĵ㱣�ֺ�һ����ķ���
					m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
					m_node[0].toPreviousCos = m_node[1].toPreviousCos;
					m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				}
			}else
			{
				m_lineLength -= m_node[nodeOrder].toNextLength;
				m_node[nodeOrder].setNext(m_node[0]);
				m_lineLength += m_node[nodeOrder].toNextLength;
			}
		}
		if(m_isLoop) setLoop(m_isLoop);	//�������߱պ�
	}else
	{//�м�
		//������Ҫ�жϸýڵ������ڽڵ��ľ����Ƿ�̫С��������������(��δ���)
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].myPoint = p;
		m_node[nodeOrder].setNext(m_node[nodeOrder + 1]);
		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder - 1].setNext(m_node[nodeOrder]);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		if(!m_isLoop)
		{
			if(nodeOrder == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == int(m_node.size()) - 2)
			{//���һ����Ҫ����ǰһ����ķ���
				m_node[nodeOrder + 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[nodeOrder + 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[nodeOrder + 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}
	}
	m_needUpdateData = XTrue;	//�²���㣬��Ҫ���¼����ֵ��
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd || m_node.size() < 2) return;	//����Ҫ����
	//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		if(m_isLoop)
		{//ѭ��
			if(nodeOrder <= m_specialPoint.upNodeOrder)
			{
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.curLength += tmpOldLoopLen - m_node[0].toPreviousLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength += tmpOldLoopLen - m_node[0].toPreviousLength;
			}
		}else
		{//��ѭ��
			if(nodeOrder <= m_specialPoint.upNodeOrder)
			{
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
			}
		}
	}else
	{//����
		if(m_isLoop)
		{//ѭ��
			if(nodeOrder >= m_specialPoint.upNodeOrder)
			{
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.curLength += tmpOldLoopLen - m_node[0].toPreviousLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength += tmpOldLoopLen - m_node[0].toPreviousLength;
			}
		}else
		{//��ѭ��
			if(nodeOrder >= m_specialPoint.upNodeOrder)
			{
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
			}
		}
	}
}
void XNodeLine::decreaseOneNode()
{
	if(m_node.size() <= 0) return;
	float tmpOldLineLength = m_lineLength;
	float tmpOldLoopLen = m_node[0].toPreviousLength;
	if(m_isLoop)
	{
		m_lineLength -= m_node[m_node.size() - 1].toPreviousLength;
		m_lineLength -= m_node[m_node.size() - 1].toNextLength;
		m_node.pop_back();
		if(m_node.size() <= 0) return;
		if(m_node.size() == 1)
		{
			m_node[m_node.size() - 1].resetToNext();
			m_node[m_node.size() - 1].resetToPreviouse();
		}else
		{
			m_node[m_node.size() - 1].setNext(m_node[0]);
			m_lineLength += m_node[0].toPreviousLength; //�����µĳ���
		}
	}else
	{
		m_lineLength -= m_node[m_node.size() - 1].toPreviousLength;
		m_node.pop_back();
		if(m_node.size() <= 0) return;
		if(m_node.size() == 1)
		{
			m_node[m_node.size() - 1].resetToNext();
			m_node[m_node.size() - 1].resetToPreviouse();
		}else
		{
			m_node[m_node.size() - 1].toNextLength = 0.0f;
			m_node[m_node.size() - 1].toNextAngle = m_node[m_node.size() - 2].toNextAngle;
			m_node[m_node.size() - 1].toNextSin = m_node[m_node.size() - 2].toNextSin;
			m_node[m_node.size() - 1].toNextCos = m_node[m_node.size() - 2].toNextCos;
		}
	}
	m_needUpdateData = XTrue;
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd || m_node.size() < 2) return;	//����Ҫ����
	//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		if(m_isLoop)
		{//ѭ��
			if(m_specialPoint.upNodeOrder == (int)(m_node.size()))
			{//����պϵ�
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.curLength += tmpOldLoopLen - m_node[0].toPreviousLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength += tmpOldLoopLen - m_node[0].toPreviousLength;
			}
		}else
		{//��ѭ��,����Ҫ�ı�
		}
	}else
	{//����
		if(m_isLoop)
		{//ѭ���������ʱ�ƶ��ſ�ʼ��ɾ�����һ����������Ծ
			if(m_specialPoint.upNodeOrder == (int)(m_node.size()))
			{
				m_specialPoint.curLength += m_lineLength - m_node[0].toPreviousLength;
				m_specialPoint.upNodeLength = m_lineLength - m_node[0].toPreviousLength;
				m_specialPoint.upNodeOrder = 0;
			}else
			{
				m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.curLength += tmpOldLoopLen - m_node[0].toPreviousLength;
				m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
				m_specialPoint.upNodeLength += tmpOldLoopLen - m_node[0].toPreviousLength;
			}
		}else
		{//��ѭ��
			m_specialPoint.curLength -= tmpOldLineLength - m_lineLength;
			m_specialPoint.upNodeLength -= tmpOldLineLength - m_lineLength;
		}
	}
}
void XNodeLine::addOneNode(const XVec2& node, int nodeOrder)		//�������м����һ����
{
	if(nodeOrder < 0 || nodeOrder > m_node.size()) return;	//�����¼�ĵ��Ѿ�������ֱ�ӷ���
	if(nodeOrder == 0)
	{//ͷ
		if(m_node[0].myPoint.getLengthSqure(node) <= 0.0f)
			return;	//��̫��������ʱ���ܲ���
		m_node.push_back(XNode(0.0f,0.0f));
		for(int i = m_node.size() - 1;i > 0;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_node[0].myPoint = node;
		m_node[0].setNext(m_node[1]);
		m_lineLength += m_node[0].toNextLength;
		if(!m_isLoop)
		{
			if(m_node.size() > 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_node.size() == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}else
		{
			m_lineLength -= m_node[m_node.size() - 1].toNextLength;
			m_node[m_node.size() - 1].setNext(m_node[0]);
			m_lineLength += m_node[m_node.size() - 1].toNextLength;
		}
		if(m_specialPoint.isEnable && !m_specialPoint.isEnd && m_node.size() >= 2)
		{
			//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
			if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
			{//˳��
				if(m_isLoop)
				{//ѭ��
					++m_specialPoint.upNodeOrder;
					m_specialPoint.curLength += m_node[0].toNextLength;
					m_specialPoint.upNodeLength += m_node[0].toNextLength;
				}else
				{//��ѭ��
					++m_specialPoint.upNodeOrder;
					m_specialPoint.curLength += m_node[0].toNextLength;
					m_specialPoint.upNodeLength += m_node[0].toNextLength;
				}
			}else
			{//����
				if(m_isLoop)
				{//ѭ��
					++m_specialPoint.upNodeOrder;
				}else
				{//��ѭ��
					++m_specialPoint.upNodeOrder;
				}
			}
		}
	}else
	if(nodeOrder == (int)(m_node.size()))
	{//β
		addOneNode(node);
	}else
	{//�м�
		if(m_node[nodeOrder].myPoint.getLengthSqure(node) <= 0.0f ||
			m_node[nodeOrder - 1].myPoint.getLengthSqure(node) <= 0.0f)
			return;	//��̫��������ʱ���ܲ���
		float tmpOldLineLength = m_lineLength;
		float tmpOldLoopLen = m_node[0].toPreviousLength;
		m_node.push_back(XNode(0.0f,0.0f));
		for(int i = m_node.size() - 1;i > nodeOrder;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint = node;
		m_node[nodeOrder].setNext(m_node[nodeOrder + 1]);
		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].setPrevious(m_node[nodeOrder - 1]);
		m_lineLength += m_node[nodeOrder].toPreviousLength;
		if(!m_isLoop)
		{
			if(nodeOrder == 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == (int)(m_node.size()) - 2)
			{//���һ������ǰһ���㱣��һ��
				m_node[m_node.size() - 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[m_node.size() - 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[m_node.size() - 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}
		if(m_specialPoint.isEnable && !m_specialPoint.isEnd && m_node.size() >= 2)
		{//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
			if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
			{//˳��
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						++ m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}
				}else
				{//��ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						++ m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
					}
				}
			}else
			{//����
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						++ m_specialPoint.upNodeOrder;
					}else
					{
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}
				}else
				{//��ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						++ m_specialPoint.upNodeOrder;
					}else
					{
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
					}
				}
			}
		}
	}
	m_needUpdateData = XTrue;
}
void XNodeLine::decreaseOneNode(int nodeOrder)
{
	//������ݵĺ�����
	if(nodeOrder < 0 || nodeOrder >= m_node.size()) return;
	if(nodeOrder == 0)
	{//ͷ
		float tmpOldLineLength = m_lineLength;
		float tmpOldLoopLen = m_node[0].toPreviousLength;
		if(m_isLoop)
		{
			m_lineLength -= m_node[0].toNextLength;
			m_lineLength -= m_node[0].toPreviousLength;
			for(int i = 0;i < int(m_node.size()) - 1;++ i)
			{
				m_node[i] = m_node[i + 1];
			}
			m_node.pop_back();
			if(m_node.size() == 1)
			{
				m_node[0].resetToNext();
				m_node[0].resetToPreviouse();
			}
			if(m_node.size() > 1)
			{
				m_node[m_node.size() - 1].setNext(m_node[0]);
				m_lineLength += m_node[0].toPreviousLength;
			}
		}else
		{
			m_lineLength -= m_node[0].toNextLength;
			for(int i = 0;i < int(m_node.size()) - 1;++ i)
			{
				m_node[i] = m_node[i + 1];
			}
			m_node.pop_back();
			if(m_node.size() == 1)
			{
				m_node[0].resetToNext();
				m_node[0].resetToPreviouse();
			}
			if(m_node.size() > 1)
			{
				m_node[0].toPreviousLength = 0.0f;
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
			}
		}
		if(m_specialPoint.isEnable && !m_specialPoint.isEnd && m_node.size() >= 2)
		{//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
			if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
			{//˳��
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder > 0)
					{
						-- m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}else
					{
						m_specialPoint.upNodeOrder = m_node.size() - 1;
						m_specialPoint.curLength += m_lineLength - m_node[0].toPreviousLength;
						m_specialPoint.upNodeLength = m_lineLength - m_node[0].toPreviousLength;
					}
				}else
				{//��ѭ��
					if(m_specialPoint.upNodeOrder > 0)
					{
						-- m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
					}else
					{
						resetSpecialPoint();
					}
				}
			}else
			{//����
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder == 0)
					{
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}else
					{
						-- m_specialPoint.upNodeOrder;
					}
				}else
				{//��ѭ��
					-- m_specialPoint.upNodeOrder;
				}
			}
		}
	}else
	if(nodeOrder == (int)(m_node.size()) - 1)
	{//β
		decreaseOneNode();
	}else
	{//�м�
		float tmpOldLineLength = m_lineLength;
		float tmpOldLoopLen = m_node[0].toPreviousLength;
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder + 1].toPreviousLength;
		for(int i = nodeOrder;i < int(m_node.size()) - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_node[nodeOrder - 1].setNext(m_node[nodeOrder]);
		m_lineLength += m_node[nodeOrder - 1].toNextLength;
		if(!m_isLoop)
		{
			if(nodeOrder == 1)
			{
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == (int)(m_node.size()) - 2)
			{
				m_node[m_node.size() - 2].toNextAngle = m_node[m_node.size() - 3].toNextAngle;
				m_node[m_node.size() - 2].toNextCos = m_node[m_node.size() - 3].toNextCos;
				m_node[m_node.size() - 2].toNextSin = m_node[m_node.size() - 3].toNextSin;
			}
		}
		m_node.pop_back();
		if(m_specialPoint.isEnable && !m_specialPoint.isEnd && m_node.size() >= 2)
		{//������Ҫ���ƶ�������ݽ��и����Ա��ھ����ı�֤�ƶ�����ȶ�
			if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
			{//˳��
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						--m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}
				}else
				{//��ѭ��
					if(m_specialPoint.upNodeOrder >= nodeOrder)
					{
						--m_specialPoint.upNodeOrder;
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
					}
				}
			}else
			{//����
				if(m_isLoop)
				{//ѭ��
					if(m_specialPoint.upNodeOrder > nodeOrder)
					{
						-- m_specialPoint.upNodeOrder;
					}else
					{
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.curLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength -= m_node[0].toPreviousLength - tmpOldLoopLen;
					}
				}else
				{//��ѭ��
					if(m_specialPoint.upNodeOrder > nodeOrder)
					{
						-- m_specialPoint.upNodeOrder;
					}else
					{
						m_specialPoint.curLength += m_lineLength - tmpOldLineLength;
						m_specialPoint.upNodeLength += m_lineLength - tmpOldLineLength;
					}
				}
			}
		}
	}
	m_needUpdateData = XTrue;
}
void XNodeLine::clearUp()
{//ֻ��Ҫ�����һ�������Ϣ����
	m_node.clear();
	m_lineLength = 0.0f;
	m_bezLineLength = 0.0f;
	m_specialPoint.isEnd = true;
}
XBool XNodeLine::saveNodeLine()
{
	if(m_node.size() <= 1) return XFalse;
	FILE *fp;
	char fileName[MAX_FILE_NAME_LENGTH];
	sprintf_s(fileName,MAX_FILE_NAME_LENGTH,NODELINE_DATA_FILE_NAME,m_myOrder);
	//���ļ�
	if((fp = fopen(fileName,"wb")) == NULL)
	{
		printf("Line file open error!\n");
		return XFalse;
	}
	//д������
	int sum = m_node.size();
	fwrite(&sum,sizeof(int),1,fp);			//�ڵ�����
	fwrite(&m_lineLength,sizeof(float),1,fp);		//�����߳���
	fwrite(&m_isLoop,sizeof(m_isLoop),1,fp);		//�����߳���
	//����д��ڵ���Ϣ
	for(int i = 0;i < m_node.size();++ i)
	{
		fwrite(&m_node[i],sizeof(XNode),1,fp);	
	}
	//д�����֮��ر��ļ�
	fclose(fp);
	return XTrue;
}
bool XNodeLine::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"rb")) == NULL)
	{//���ļ�
		printf("Line file open error!\n");
		return XFalse;
	}
	//��ȡ����
	int sum;
	fread(&sum,sizeof(int),1,fp);		//�ڵ�����
	if(sum <= 1)
	{
		fclose(fp);
		return false;
	}
	m_node.resize(sum);
	fread(&m_lineLength,sizeof(float),1,fp);		//�����߳���
	fread(&m_isLoop,sizeof(m_isLoop),1,fp);		//�����߳���
	//���ζ�ȡ�ڵ���Ϣ
	for(int i = 0;i < m_node.size();++ i)
	{
		fread(&m_node[i],sizeof(XNode),1,fp);	
	}
	//��ȡ���֮��ر��ļ�
	fclose(fp);
	return true;
}
bool XNodeLine::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	//����Դ�ļ�����ȡ��Դ
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL)
	{
		printf("Load NodeLine Data Error!\n");
		return XFalse;
	}
	//����ȡ�������ͷŵ�ָ����λ��
	int offset = 0;
	memcpy(&offset,p,sizeof(int));
	m_node.resize(offset);
	offset = sizeof(int);
	memcpy(&m_lineLength,p + offset,sizeof(float));
	offset += sizeof(float);
	memcpy(&m_isLoop,p + offset,sizeof(m_isLoop));
	offset += sizeof(m_isLoop);
	//����д��ڵ���Ϣ
	for(int i = 0;i < m_node.size();++ i)
	{
		memcpy(&m_node[i],p + offset,sizeof(XNode));
		offset += sizeof(XNode);
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XNodeLine::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XNodeLine::getNodeLine(XResPos resPos)
{
	char fileName[MAX_FILE_NAME_LENGTH] = "";
	sprintf_s(fileName,MAX_FILE_NAME_LENGTH,NODELINE_DATA_FILE_NAME,m_myOrder);
	strcpy_s(fileName,MAX_FILE_NAME_LENGTH,(std::string(BASE_RESOURCE_PATH) + fileName).c_str());
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(fileName)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(fileName)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(fileName)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(fileName) && !loadFromFolder(fileName) &&
			!loadFromWeb(fileName)) return false;
		break;
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
void XNodeLine::updateData()
{//Ŀǰ����û�д���ѭ���벻ѭ�������Դ�������
	if(!m_needUpdateData) return;
	m_needUpdateData = XFalse;	//�����������
	if(m_node.size() < 2) return;
	//���濪ʼ�����ֵ�ؼ���(ÿ����������������)
	XVec2 tempPoint0;
	XVec2 tempPoint1;
	XVec2 tempPoint2;
	m_bezierPoint.resize(m_node.size() << 1);
	//��һ����Ҫ�ֿ�����
	if(m_isLoop)
	{
		tempPoint0 = (m_node[0].myPoint + m_node[m_node.size() - 1].myPoint) * 0.5f;	//��ǰһ������е�
		tempPoint1 = (m_node[0].myPoint + m_node[1].myPoint) * 0.5f;	//���һ������е�
		tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;	//���е���е�
		m_bezierPoint[0] = tempPoint0 + m_node[0].myPoint - tempPoint2;	//���е����ߵ�ƽ��
		m_bezierPoint[1] = tempPoint1 + m_node[0].myPoint - tempPoint2;
		//���һ����Ҫ�ֿ�����
		//tempPoint0 = (m_node[0].myPoint + m_node[m_nodeSum - 1].myPoint) * 0.5f;
		tempPoint1 = (m_node[m_node.size() - 1].myPoint + m_node[m_node.size() - 2].myPoint) * 0.5f;
		tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
		m_bezierPoint[(m_node.size() - 1) << 1] = tempPoint1 + m_node[m_node.size() - 1].myPoint - tempPoint2;
		m_bezierPoint[((m_node.size() - 1) << 1) + 1] = tempPoint0 + m_node[m_node.size() - 1].myPoint - tempPoint2;
	}else
	{
		tempPoint0 = (m_node[1].myPoint - m_node[0].myPoint) * 0.3333f;
		m_bezierPoint[0] = m_node[0].myPoint - tempPoint0;	//���е����ߵ�ƽ��
		m_bezierPoint[1] = m_node[0].myPoint + tempPoint0;

		//���һ����Ҫ�ֿ�����
		tempPoint0 = (m_node[m_node.size() - 1].myPoint - m_node[m_node.size() - 2].myPoint) * 0.3333f;
		m_bezierPoint[(m_node.size() - 1) << 1] = m_node[m_node.size() - 1].myPoint - tempPoint0;
		m_bezierPoint[((m_node.size() - 1) << 1) + 1] = m_node[m_node.size() - 1].myPoint + tempPoint0;
	}
	for(int i = 1;i < m_node.size() - 1;++ i)
	{
		tempPoint0 = (m_node[i].myPoint + m_node[i - 1].myPoint) * 0.5f;
		tempPoint1 = (m_node[i].myPoint + m_node[i + 1].myPoint) * 0.5f;
		tempPoint2 = (tempPoint0 + tempPoint1) * 0.5f;
		m_bezierPoint[i << 1] = tempPoint0 + m_node[i].myPoint - tempPoint2;
		m_bezierPoint[(i << 1) + 1] = tempPoint1 + m_node[i].myPoint - tempPoint2;
	}
	//������Ҫ�������е�ı�������Ϣ
	XBezierSpline tmpBz;
	m_bezLineLength = 0.0f;
	if(m_isLoop)
	{
		for(int i = 0;i < m_node.size();++ i)
		{
			setBezierSplinePar(tmpBz,i);
			m_node[i].toBezNextLength = tmpBz.getLength();
			m_node[i].toBezNextAngle = tmpBz.getBezierSplineAngle(0.0f);
			m_node[(i + 1) % m_node.size()].toBezPreviousLength = tmpBz.getLength();
			m_node[(i + 1) % m_node.size()].toBezPreviousAngle = tmpBz.getBezierSplineAngle(1.0f);
			m_bezLineLength += m_node[i].toBezNextLength;
		}
	}else
	{
		m_node[0].toBezPreviousLength = 0.0f;
		m_node[0].toBezPreviousAngle = m_node[0].toPreviousAngle;
		m_node[m_node.size() - 1].toBezNextLength = 0.0f;
		m_node[m_node.size() - 1].toBezNextAngle = m_node[m_node.size() - 1].toNextAngle;
		for(int i = 0;i < m_node.size() - 1;++ i)
		{
			setBezierSplinePar(tmpBz,i);
			m_node[i].toBezNextLength = tmpBz.getLength();
			m_node[i].toBezNextAngle = tmpBz.getBezierSplineAngle(0.0f);
			m_node[i + 1].toBezPreviousLength = tmpBz.getLength();
			m_node[i + 1].toBezPreviousAngle = tmpBz.getBezierSplineAngle(1.0f);
			m_bezLineLength += m_node[i].toBezNextLength;
		}
	}
}
void XNodeLine::updateBezierInfo()
{
	if(m_node.size() <= 2) return;
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd) return;	//����Ҫ����
	setBezierSplinePar(m_bezierSpline,m_specialPoint.upNodeOrder,!((m_leadMode & LEAD_MODE_FOREWORD) != 0));
	//if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	//{//˳��
	//	//��������������仯ʱ������Ҫ����������ݱ�֤���ݵĺ�����
	//	if(!m_isLoop)
	//	{//��ѭ��
	//		m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//			m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
	//			m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
	//			m_node[m_specialPoint.upNodeOrder + 1].myPoint);
	//	}else
	//	{
	//		if(m_specialPoint.upNodeOrder >= m_node.size() - 1)
	//		{//���һ����
	//			m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//				m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
	//				m_bezierPoint[0],
	//				m_node[0].myPoint);
	//		}else
	//		{
	//			m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//				m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
	//				m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
	//				m_node[m_specialPoint.upNodeOrder + 1].myPoint);
	//		}
	//	}
	//}else
	//{//����
	//	if(!m_isLoop)
	//	{//��ѭ��
	//		m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//			m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
	//			m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
	//			m_node[m_specialPoint.upNodeOrder - 1].myPoint);
	//	}else
	//	{
	//		if(m_specialPoint.upNodeOrder == 0)
	//		{
	//			m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//				m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
	//				m_bezierPoint[((m_node.size() - 1) << 1) + 1],
	//				m_node[m_node.size() - 1].myPoint);
	//		}else
	//		{
	//			m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
	//				m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
	//				m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
	//				m_node[m_specialPoint.upNodeOrder - 1].myPoint);
	//		}
	//	}
	//}
}
//ע��������Ҫ��2����
void XNodeLine::moveSpecialPoint(float timeDelay)
{
	if(m_node.size() <= 1) return;
	XBool needBezier = m_withBezier;
	if(m_node.size() <= 2 && needBezier) needBezier = false;	//С���������ʱ��������Ч 
	bool needUpdate = m_needUpdateData;
	updateData();	//����б�Ҫ���²�ֵ����
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd) return;	//����Ҫ����
	if(!m_isLoop)
	{//�����ѭ�������ǵ���ѭ��·�ϣ�����Ҫ���¼���
		if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
		{//˳��
			if(m_specialPoint.upNodeOrder == (int)(m_node.size()) - 1) 
			{
				m_specialPoint.isEnd = XTrue;
				m_specialPoint.angle = m_node[m_node.size() - 1].toNextAngle;
				m_specialPoint.position = m_node[m_node.size() - 1].myPoint;
				m_specialPoint.curLength = 0.0f;
				m_specialPoint.upNodeOrder = 0;
				m_specialPoint.upNodeLength = 0.0f;
				return;
			}
		}else
		{//����
			if(m_specialPoint.upNodeOrder == 0)
			{
				m_specialPoint.isEnd = XTrue;
				m_specialPoint.angle = m_node[0].toPreviousAngle;
				m_specialPoint.position = m_node[0].myPoint;
				m_specialPoint.curLength = 0.0f;
				m_specialPoint.upNodeOrder = 0;
				m_specialPoint.upNodeLength = 0.0f;
				return;
			}
		}
	}
	if(needUpdate) //������ݷ����˸��£�������Ҫ������صı���������
	{
		if(needBezier)
			updateBezierInfo();
		else
		{//����Բ����Ϸ��Խ��д���
			if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
			{//˳��
				if(!m_isLoop)
				{//��ѭ��
					if(m_specialPoint.upNodeOrder >= m_node.size() - 1)
					{//�Ѿ������յ�
						m_specialPoint.isEnd = XTrue;
						m_specialPoint.angle = m_node[m_node.size() - 2].toNextAngle;
						m_specialPoint.curLength = m_lineLength;
						m_specialPoint.position = m_node[m_node.size() - 1].myPoint;
						m_specialPoint.upNodeOrder = m_node.size() - 2;
						m_specialPoint.upNodeLength = m_lineLength - m_node[m_node.size() - 1].toPreviousLength;
					}
				}else
				{
					if(m_specialPoint.upNodeOrder > m_node.size() - 1)
					{//���µ������
						m_specialPoint.isEnd = XTrue;
						m_specialPoint.angle = m_node[0].toNextAngle;
						m_specialPoint.position = m_node[0].myPoint;
						m_specialPoint.curLength = 0.0f;
						m_specialPoint.upNodeOrder = 0;
						m_specialPoint.upNodeLength = 0.0f;
					}
				}
			}else
			{//����
				if(!m_isLoop)
				{//��ѭ��
					if(m_specialPoint.upNodeOrder > (int)(m_node.size()) - 1)
						resetSpecialPoint();
				}else
				{
					if(m_specialPoint.upNodeOrder > (int)(m_node.size()) - 1)
						resetSpecialPoint();
				}
			}
		}
	}
	m_specialPoint.curLength += timeDelay * m_specialPoint.speed;
	float tmpAngle;
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		while(true)
		{
			if(m_specialPoint.curLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toNextLength)
			{//�Ѿ�Խ������һ����
				if(!m_isLoop)
				{//��ѭ��
					if(m_specialPoint.upNodeOrder + 1 == (int)(m_node.size()) - 1)
					{//�Ѿ������յ�
						m_specialPoint.isEnd = XTrue;
						m_specialPoint.angle = m_node[m_node.size() - 2].toNextAngle;
						m_specialPoint.curLength = m_lineLength;
						m_specialPoint.position = m_node[m_node.size() - 1].myPoint;
						m_specialPoint.upNodeOrder = m_node.size() - 2;
						m_specialPoint.upNodeLength = m_lineLength - m_node[m_node.size() - 1].toPreviousLength;
						break;
					}else
					{
						++ m_specialPoint.upNodeOrder;
						m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							setBezierSplinePar(m_bezierSpline,m_specialPoint.upNodeOrder);
							//m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//	m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
							//	m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
							//	m_node[m_specialPoint.upNodeOrder + 1].myPoint);
						}
					}
				}else
				{//ѭ��
					if(m_specialPoint.upNodeOrder == (int)(m_node.size()) - 1)
					{//���µ������
						m_specialPoint.isEnd = XTrue;
						if(needBezier)
						{
							setBezierSplinePar(m_bezierSpline,0);
							//m_bezierSpline.init(m_node[0].myPoint,m_bezierPoint[1],m_bezierPoint[2],m_node[1].myPoint);
						}else
						{
							m_specialPoint.angle = m_node[0].toNextAngle;
							m_specialPoint.position = m_node[0].myPoint;
						}
						m_specialPoint.curLength = 0.0f;
						m_specialPoint.upNodeOrder = 0;
						m_specialPoint.upNodeLength = 0.0f;
						break;
					}else
					{
						++ m_specialPoint.upNodeOrder;
						m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							setBezierSplinePar(m_bezierSpline,m_specialPoint.upNodeOrder);
							//if(m_specialPoint.upNodeOrder == m_node.size() - 1)
							//{//���һ����
							//	m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//		m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
							//		m_bezierPoint[0],
							//		m_node[0].myPoint);
							//}else
							//{
							//	m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//		m_bezierPoint[(m_specialPoint.upNodeOrder << 1) + 1],
							//		m_bezierPoint[(m_specialPoint.upNodeOrder + 1) << 1],
							//		m_node[m_specialPoint.upNodeOrder + 1].myPoint);
							//}
						}
					}
				}
			}else
			{//û��Խ������һ����
				if(m_specialPoint.upNodeOrder == (int)(m_node.size()) - 1)
				{//�������һ���㣬���������
					if(needBezier)
					{//��������Ҫ���ӵĲ���
						float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
						tempPos = m_bezierSpline.getT(tempPos);
						m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
						m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
					}else
					{
						m_specialPoint.position = m_node[m_specialPoint.upNodeOrder].myPoint 
							+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * 
							XVec2(m_node[m_specialPoint.upNodeOrder].toNextCos,m_node[m_specialPoint.upNodeOrder].toNextSin);
						if(m_withAngleSmooth)
						{
							tmpAngle = m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 - tmpAngle);
							}else
							if(tmpAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + tmpAngle);
							}
						}else
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle;
						}
					}
				}else
				{//�����ĵ�
					if(needBezier)
					{//��������Ҫ���ӵĲ���
						float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength;
						tempPos = m_bezierSpline.getT(tempPos);
						m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
						m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
					}else
					{
						m_specialPoint.position = m_node[m_specialPoint.upNodeOrder].myPoint
							+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * 
							XVec2(m_node[m_specialPoint.upNodeOrder].toNextCos,m_node[m_specialPoint.upNodeOrder].toNextSin);
						if(m_withAngleSmooth)
						{
							tmpAngle = m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) / m_node[m_specialPoint.upNodeOrder].toNextLength 
									* tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) / m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) / m_node[m_specialPoint.upNodeOrder].toNextLength 
									* (PI2 + tmpAngle);
							}	
						}else
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle ;
						}
					}
				}
				break;
			}
		}
	}else
	{//����
		while(true)
		{
			if(m_specialPoint.curLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toPreviousLength)
			{//�Լ�Խ������һ����
				if(!m_isLoop)
				{//��ѭ��
					if(m_specialPoint.upNodeOrder == 1)
					{//�Ѿ������յ�
						m_specialPoint.isEnd = XTrue;
						m_specialPoint.angle = m_node[1].toPreviousAngle;
						m_specialPoint.curLength = m_lineLength;
						m_specialPoint.position = m_node[0].myPoint;
						m_specialPoint.upNodeOrder = 1;
						m_specialPoint.upNodeLength = m_lineLength - m_node[0].toNextLength;
						break;
					}else
					{
						-- m_specialPoint.upNodeOrder;
						m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							setBezierSplinePar(m_bezierSpline,m_specialPoint.upNodeOrder,true);
							//m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//	m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
							//	m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
							//	m_node[m_specialPoint.upNodeOrder - 1].myPoint);
						}
					}
				}else
				{//ѭ��
					if(m_specialPoint.upNodeOrder == 0)
					{//�Ѿ������յ�
						m_specialPoint.isEnd = XTrue;
						if(needBezier)
						{
							setBezierSplinePar(m_bezierSpline,m_node.size() - 1,true);
							//m_bezierSpline.init(m_node[m_node.size() - 1].myPoint,
							//	m_bezierPoint[(m_node.size() - 1) << 1],
							//	m_bezierPoint[((m_node.size() - 2) << 1) + 1],
							//	m_node[m_node.size() - 2].myPoint);
						}else
						{
							//m_specialPoint.isEnd = XTrue;
							m_specialPoint.angle = m_node[m_node.size() - 1].toPreviousAngle;
							m_specialPoint.position = m_node[m_node.size() - 1].myPoint;
						}
						m_specialPoint.curLength = 0.0f;
						m_specialPoint.upNodeOrder = m_node.size() - 1;
						m_specialPoint.upNodeLength = 0.0f;
						break;
					}else
					{
						-- m_specialPoint.upNodeOrder;
						m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
						if(needBezier)
						{//��������Ҫ���ӵĲ���
							setBezierSplinePar(m_bezierSpline,m_specialPoint.upNodeOrder,true);
							//if(m_specialPoint.upNodeOrder == 0)
							//{
							//	m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//		m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
							//		m_bezierPoint[((m_node.size() - 1) << 1) + 1],
							//		m_node[m_node.size() - 1].myPoint);
							//}else
							//{
							//	m_bezierSpline.init(m_node[m_specialPoint.upNodeOrder].myPoint,
							//		m_bezierPoint[(m_specialPoint.upNodeOrder << 1)],
							//		m_bezierPoint[((m_specialPoint.upNodeOrder - 1) << 1) + 1],
							//		m_node[m_specialPoint.upNodeOrder - 1].myPoint);
							//}
						}
					}
				}
			}else
			{//û��Խ������һ����
				if(m_specialPoint.upNodeOrder == 0)
				{//�������е����
					if(needBezier)
					{//��������Ҫ���ӵĲ���
						float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength) / m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						tempPos = m_bezierSpline.getT(tempPos);
						m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
						m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
					}else
					{
						m_specialPoint.position = m_node[m_specialPoint.upNodeOrder].myPoint 
							+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * 
							XVec2(m_node[m_specialPoint.upNodeOrder].toPreviousCos,m_node[m_specialPoint.upNodeOrder].toPreviousSin);
						if(m_withAngleSmooth)
						{
							tmpAngle = m_node[m_node.size() - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + tmpAngle);
							}
						}else
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle;
						}
					}
				}else
				{//��ͨ�ĵ�
					if(needBezier)
					{//��������Ҫ���ӵĲ���
						float tempPos = (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						tempPos = m_bezierSpline.getT(tempPos);
						m_specialPoint.position = m_bezierSpline.getBezierSplineValue(tempPos);
						m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(tempPos);
					}else
					{
						m_specialPoint.position = m_node[m_specialPoint.upNodeOrder].myPoint 
							+ (m_specialPoint.curLength - m_specialPoint.upNodeLength) * 
							XVec2(m_node[m_specialPoint.upNodeOrder].toPreviousCos,m_node[m_specialPoint.upNodeOrder].toPreviousSin);
						if(m_withAngleSmooth)
						{
							tmpAngle = m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle;
							if(tmpAngle <= PI && tmpAngle >=  -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
									+ (m_specialPoint.curLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
									* (PI2 + tmpAngle);
							}
						}else
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle;
						}
					}
				}
				break;
			}
		}
	}
}
void XNodeLine::resetSpecialPt(int index,float distance)
{
	if(index < 0 || index >= m_node.size()) return;
	if(distance < 0.0f) distance = 0.0f;
	if(m_node.size() <= 1) return;
	XBool needBezier = m_withBezier;
	if(m_node.size() <= 2 && needBezier) needBezier = false;
	updateData();	//����б�Ҫ���²�ֵ����
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.position = m_node[index].myPoint;
		m_specialPoint.angle = m_node[index].toNextAngle;
		m_specialPoint.upNodeOrder = index;
		m_specialPoint.curLength = 0.0f;
		for(int i = 0;i < index;++ i)
		{
			m_specialPoint.curLength += m_node[i].toNextLength;
		}
		m_specialPoint.curLength += distance;
		m_specialPoint.upNodeLength = m_specialPoint.curLength;
		if(m_isLoop)
		{//ѭ��
			if(needBezier)
			{
				setBezierSplinePar(m_bezierSpline,index);
				//if(index < m_node.size() - 1)
				//	m_bezierSpline.init(m_node[index].myPoint,m_bezierPoint[(index << 1) + 1],	
				//		m_bezierPoint[(index << 1) + 2],m_node[index + 1].myPoint);
				//else
				//	m_bezierSpline.init(m_node[index].myPoint,m_bezierPoint[(index << 1) + 1],	
				//		m_bezierPoint[0],m_node[0].myPoint);
			}
		}else
		{//��ѭ��
			if(needBezier && index < (int)(m_node.size()) - 1)
				setBezierSplinePar(m_bezierSpline,index);
				//m_bezierSpline.init(m_node[index].myPoint,m_bezierPoint[(index << 1) + 1],	
				//	m_bezierPoint[(index << 1) + 2],m_node[index + 1].myPoint);
		}
		if(needBezier)
			m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(0.0f);	
		else
			m_specialPoint.angle = m_node[index].toNextAngle;
	}else
	{//����
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.position = m_node[index].myPoint;
		m_specialPoint.upNodeOrder = index;
		m_specialPoint.curLength = 0.0f;
		for(int i = m_node.size() - 1;i > index;-- i)
		{
			m_specialPoint.curLength += m_node[i].toPreviousLength;
		}
		m_specialPoint.curLength += distance;
		m_specialPoint.upNodeLength = m_specialPoint.curLength;
		if(m_isLoop)
		{//ѭ��
			if(needBezier)
			{
				setBezierSplinePar(m_bezierSpline,index,true);
				//if(index > 0)
				//	m_bezierSpline.init(m_node[index].myPoint,
				//		m_bezierPoint[index << 1],
				//		m_bezierPoint[(index << 1) - 1],
				//		m_node[index - 1].myPoint);
				//else
				//	m_bezierSpline.init(m_node[0].myPoint,
				//		m_bezierPoint[0],
				//		m_bezierPoint[(m_node.size() << 1) - 1],
				//		m_node[m_node.size() - 1].myPoint);
			}
		}else
		{//��ѭ��
			if(needBezier && index > 0)
				setBezierSplinePar(m_bezierSpline,index,true);
				//m_bezierSpline.init(m_node[index].myPoint,
				//	m_bezierPoint[index << 1],
				//	m_bezierPoint[(index << 1) - 1],
				//	m_node[index - 1].myPoint);
		}
		if(needBezier)
			m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(0.0f);	
		else
			m_specialPoint.angle = m_node[index].toPreviousAngle;
	}
}
void XNodeLine::resetSpecialPoint()
{
	if(m_node.size() <= 1) return;
	XBool needBezier = m_withBezier;
	if(m_node.size() <= 2 && needBezier) needBezier = false;
	updateData();	//����б�Ҫ���²�ֵ����
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.curLength = 0.0f;
		m_specialPoint.position = m_node[0].myPoint;
		m_specialPoint.angle = m_node[0].toNextAngle;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = 0;
		if(needBezier)
		{
			setBezierSplinePar(m_bezierSpline,0);
			//m_bezierSpline.init(m_node[0].myPoint,m_bezierPoint[1],m_bezierPoint[2],m_node[1].myPoint);
			m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(0.0f);
		}else
		{
			m_specialPoint.angle = m_node[0].toNextAngle;
		}
	}else
	{//����
		m_specialPoint.isEnable = XTrue;
		m_specialPoint.isEnd = XFalse;
		m_specialPoint.curLength = 0.0f;
		m_specialPoint.position = m_node[m_node.size() - 1].myPoint;
		m_specialPoint.upNodeLength = 0.0f;
		m_specialPoint.upNodeOrder = m_node.size() - 1;
		if(needBezier)
		{
			setBezierSplinePar(m_bezierSpline,m_node.size() - 1,true);
			//m_bezierSpline.init(m_node[m_node.size() - 1].myPoint,m_bezierPoint[(m_node.size() - 1) << 1],
			//	m_bezierPoint[((m_node.size() - 2) << 1) + 1],m_node[m_node.size() - 2].myPoint);
			m_specialPoint.angle = m_bezierSpline.getBezierSplineAngle(0.0f);
		}else
		{
			m_specialPoint.angle = m_node[m_node.size() - 1].toPreviousAngle;
		}
	}
}
void XNodeLine::setLoop(bool flag)
{
	if(m_node.size() < 2) 	//С���������ǲ���ѭ����
	{
		m_isLoop = flag;
		return;
	}
	//����һ��������һ������������
	if(flag)
	{//����Ϊѭ��
		if(m_isLoop)//���������ԭ�����Ǳպϵģ�������Ҫ��ȥԭ���ĳ���
			m_lineLength -= m_node[0].toPreviousLength;
		m_node[m_node.size() - 1].setNext(m_node[0]);
		m_lineLength += m_node[0].toPreviousLength; //�����µĳ���
		m_isLoop = XTrue;
	}else
	{//ȡ��ѭ��
		if(m_isLoop)//���������ԭ�����Ǳպϵģ�������Ҫ��ȥԭ���ĳ���
			m_lineLength -= m_node[0].toPreviousLength;
		m_node[0].resetToPreviouse();
		m_node[m_node.size() - 1].resetToNext();
		if(m_node.size() > 1)
		{
			m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
			m_node[0].toPreviousCos = m_node[1].toPreviousCos;
			m_node[0].toPreviousSin = m_node[1].toPreviousSin;

			m_node[m_node.size() - 1].toNextAngle = m_node[m_node.size() - 2].toNextAngle;
			m_node[m_node.size() - 1].toNextCos = m_node[m_node.size() - 2].toNextCos;
			m_node[m_node.size() - 1].toNextSin = m_node[m_node.size() - 2].toNextSin;
		}
		m_isLoop = XFalse;
	}
	m_needUpdateData = true;
}
XBool XNodeLine::setACopy(const XNodeLine &temp)
{
	if(this == &temp) return XFalse;		//��ֹ��������

//	if(m_isACopy == 0)
//	{//��������Ǹ�������Ҫ�ͷ���Դ
//		XMem::XDELETE_ARRAY(m_node);
//		m_isACopy = 1;
//	}
	//��������
	m_needUpdateData = temp.m_needUpdateData;						//�������еĽڵ�
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_bezierPoint = temp.m_bezierPoint;	
	m_bezierSpline = temp.m_bezierSpline;						//�������еĽڵ�

	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_node = temp.m_node;						//�������еĽڵ�
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_bezLineLength = temp.m_bezLineLength;			//���ߵ��ܳ���
	m_withAngleSmooth = temp.m_withAngleSmooth;	

	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_withBezier = temp.m_withBezier;	
	return XTrue;
}
XNodeLine::XNodeLine(const XNodeLine &temp)
{
	//m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
	//��������
	m_needUpdateData = temp.m_needUpdateData;						//�������еĽڵ�
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_bezierPoint = temp.m_bezierPoint;	
	m_bezierSpline = temp.m_bezierSpline;						//�������еĽڵ�

	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_node = temp.m_node;						//�������еĽڵ�
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_bezLineLength = temp.m_bezLineLength;			//���ߵ��ܳ���
	m_withAngleSmooth = temp.m_withAngleSmooth;	

	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_withBezier = temp.m_withBezier;	
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
}
XNodeLine& XNodeLine::operator = (const XNodeLine& temp)
{
	if(&temp == this) return *this;
//	if(m_node == NULL || m_isACopy != 0)
//	{//��Դ�Ѿ����ͷţ�������Դ�������Լ������������·����ڴ�ռ�
//		m_node = XMem::createArrayMem<XNode>(m_maxNodeSum);
//	}
	m_needUpdateData = temp.m_needUpdateData;						//�������еĽڵ�
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���
	m_isVisible = temp.m_isVisible;					//���������ߵı��
	m_bezierPoint = temp.m_bezierPoint;	
	m_bezierSpline = temp.m_bezierSpline;						//�������еĽڵ�

	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_node = temp.m_node;						//�������еĽڵ�
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_scale = temp.m_scale;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_bezLineLength = temp.m_bezLineLength;			//���ߵ��ܳ���
	m_withAngleSmooth = temp.m_withAngleSmooth;	

	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_withBezier = temp.m_withBezier;	

//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
	return *this;
}
XBool XNodeLine::isInRect(const XVec2& p)
{
//	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
	XVec2 pointS,pointE;
	if(m_node.size() == 1)
	{//ֻ��һ����Ľڵ����ߣ��ж������ľ���
		if(p.getLength(m_node[0].myPoint * m_scale + m_offsetPosition) <= 5) return XTrue;
		else return XFalse;
	}else
	if(m_node.size() >= 1)
	{
		for(int i = 0;i < (int)(m_node.size()) - 1;++ i)
		{
			pointS.set(m_node[i].myPoint * m_scale + m_offsetPosition);
			pointE.set(m_node[i + 1].myPoint * m_scale + m_offsetPosition);
			if(XMath::minDistancePointToLine(p,pointS,pointE) <= 5) return XTrue;
		}
	}
	return XFalse;
}
XVec2 XNodeLine::getBox(int order)
{//������Ҫ���ذ�Χ��
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_node.size() > 0)
	{//����ڵ��������нڵ㣬���������ڵ�İ�Χ��
		left = m_node[0].myPoint.x;
		right = m_node[0].myPoint.x;
		top = m_node[0].myPoint.y;
		bottom = m_node[0].myPoint.y;
		//���濪ʼ�������еĵ㣬Ѱ�Ұ�Χ����
		for(auto it = m_node.begin() + 1;it != m_node.end();++ it)
		{
			if(it->myPoint.x > right) right = it->myPoint.x;else
			if(it->myPoint.x < left) left = it->myPoint.x;
			if(it->myPoint.y > bottom) bottom = it->myPoint.y;else
			if(it->myPoint.y < top) top = it->myPoint.y;
		}
	}
	//����԰�Χ���ӽ�����չ����ֹ������ߵ��ߵ�����
	left -= 5;
	right += 5;
	top -= 5;
	bottom += 5;
	switch(order)
	{
	case 0:return XVec2(left,	top) * m_scale + m_offsetPosition;
	case 1:return XVec2(right,	top) * m_scale + m_offsetPosition;
	case 2:return XVec2(right,	bottom) * m_scale + m_offsetPosition;
	case 3:return XVec2(left,	bottom) * m_scale + m_offsetPosition;
	}
	return XVec2::zero;
}
void XNodeLine::draw()
{
	updateData();
	if(m_node.size() <= 0) return;
	for(unsigned int j = 0;j < m_node.size() - 1;++ j)
	{
		//��������
		if(j == 0)
		{//��һ��������ɫ
			XRender::drawPoint(m_node[j].myPoint * m_scale + m_offsetPosition,4,XFColor::green);
		}else
		{//�������ú�ɫ
			XRender::drawPoint(m_node[j].myPoint * m_scale + m_offsetPosition,4,XFColor::red);
		}
		XRender::drawLine(m_node[j].myPoint * m_scale + m_offsetPosition,m_node[j + 1].myPoint * m_scale + m_offsetPosition);
	}
	XRender::drawPoint(m_node[m_node.size() - 1].myPoint * m_scale + m_offsetPosition,4,XFColor::red);
	if(m_isLoop)
	{
		XRender::drawLine(m_node[m_node.size() - 1].myPoint * m_scale + m_offsetPosition,m_node[0].myPoint * m_scale + m_offsetPosition);
	}
	XVec2 tmpVec(m_specialPoint.position.x * m_scale.x,m_specialPoint.position.y * m_scale.y);
	XRender::drawLine(tmpVec,m_specialPoint.angle * RADIAN2DEGREE,20.0f);
	//�ڵ�ǰ���ڵ�λ�û�һ��ʮ��
	XRender::drawCross(tmpVec,10.0f * m_scale.x);
	if(m_withBezier && m_node.size() >= 2)
	{
		m_bezierSpline.draw();
		XBezierSpline tmpBz;
		if(m_isLoop)
		{
			for(int i = 0;i < m_node.size();++ i)
			{
				setBezierSplinePar(tmpBz,i);
				tmpBz.draw();
			}
		}else
		{
			for(int i = 0;i < ((int)m_node.size()) - 1;++ i)
			{
				setBezierSplinePar(tmpBz,i);
				tmpBz.draw();
			}
		}
	}
}
XVec2 XNodeLine::getRatePosition(float rate,int *pIndex,bool needBezier)
{//Ŀǰ�в�֧��needBezier,���������û���ϸ���Թ�
	if(m_node.size() <= 0) return XVec2::zero;
	if(rate < 0.0f)
	{
		if(pIndex != NULL) *pIndex = 0;
		return m_offsetPosition + m_node[0].myPoint * m_scale;
	}
	if(rate > 1.0f)
	{
		if(m_isLoop)
		{
			if(pIndex != NULL) *pIndex = 0;
			return m_offsetPosition + m_node[0].myPoint * m_scale;
		}else
		{
			if(pIndex != NULL) *pIndex = m_node.size() - 1;
			return m_offsetPosition + m_node[m_node.size() - 1].myPoint * m_scale;
		}
	}
//	float tmpLine = m_lineLength * rate;
//	float tmpLen = 0.0f;
	if(m_isLoop)
	{
		if(rate > 1.0f) return m_offsetPosition + m_node[0].myPoint * m_scale;
		float tmpLine = m_lineLength * rate;
		float tmpLen = 0.0f;
		for(int i = 0;i < m_node.size();++ i)
		{
			if(tmpLen + m_node[i].toNextLength >= tmpLine)
			{//Ŀ������������֮��
				if(pIndex != NULL) *pIndex = i;
				if(i != (int)(m_node.size()) - 1)
					return m_offsetPosition + 
						XMath::lineSlerp(m_node[i].myPoint,m_node[i + 1].myPoint,(tmpLine - tmpLen) / m_node[i].toNextLength) * m_scale;
				else
					return m_offsetPosition + 
						XMath::lineSlerp(m_node[i].myPoint,m_node[0].myPoint,(tmpLine - tmpLen) / m_node[i].toNextLength) * m_scale;
			}else
			{
				tmpLen += m_node[i].toNextLength;
			}
		}
	}else
	{
		if(rate > 1.0f) return m_offsetPosition + m_node[m_node.size() - 1].myPoint * m_scale;
		float tmpLine = m_lineLength * rate;
		float tmpLen = 0.0f;
		for(int i = 0;i < (int)(m_node.size()) - 1;++ i)
		{
			if(tmpLen + m_node[i].toNextLength >= tmpLine)
			{//Ŀ������������֮��
				if(pIndex != NULL) *pIndex = i;
				return m_offsetPosition +
					XMath::lineSlerp(m_node[i].myPoint,m_node[i + 1].myPoint,(tmpLine - tmpLen) / m_node[i].toNextLength) * m_scale;
			}else
			{
				tmpLen += m_node[i].toNextLength;
			}
		}
	}
	return XVec2::zero;
}
XVec2 XNodeLine::getRate(const XVec2& pos,int *pIndex,bool needBezier)
{
	if(m_node.size() <= 0) return XVec2(-1.0f);
	double a,b,c;	//ֱ�߲���
	XVec2 tmp;
	float distance = -1.0f;
	XVec2 tmpPos = (pos - m_offsetPosition) / m_scale;
	float tmpDis;
	int curIndex = -1;
	XVec2 curPoint;	//����Ĵ����λ��
	if(m_isLoop)
	{
		for(int i = 0;i < m_node.size();++ i)
		{
			XMath::twoPointLine(m_node[i].myPoint,m_node[(i + 1) % m_node.size()].myPoint,&a,&b,&c);
			tmp = XMath::curtatePointToLine(tmpPos,a,b,c);
			if(XMath::isInArea(tmp.x,m_node[i].myPoint.x,m_node[(i + 1) % m_node.size()].myPoint.x) &&
				XMath::isInArea(tmp.y,m_node[i].myPoint.y,m_node[(i + 1) % m_node.size()].myPoint.y))
			{
				tmpDis = tmpPos.getLengthSqure(tmp);
				if(distance < 0.0f || tmpDis < distance)
				{
					curPoint = tmp;
					distance = tmpDis;
					curIndex = i;
				}
			}
		}
	}else
	{
		for(int i = 0;i < (int)(m_node.size()) - 1;++ i)
		{
			XMath::twoPointLine(m_node[i].myPoint,m_node[i + 1].myPoint,&a,&b,&c);
			tmp = XMath::curtatePointToLine(tmpPos,a,b,c);
			if(XMath::isInArea(tmp.x,m_node[i].myPoint.x,m_node[i + 1].myPoint.x) &&
				XMath::isInArea(tmp.y,m_node[i].myPoint.y,m_node[i + 1].myPoint.y))
			{
				tmpDis = tmpPos.getLengthSqure(tmp);
				if(distance < 0.0f || tmpDis < distance)
				{
					curPoint = tmp;
					distance = tmpDis;
					curIndex = i;
				}
			}
		}
	}
	//�����괹��֮����Ҫ���㶥�㣬����붥��ľ������������Ϊ����
	for(int i = 0;i < m_node.size();++ i)
	{
		tmpDis = tmpPos.getLengthSqure(m_node[i].myPoint);
		if(distance < 0.0f || tmpDis < distance)
		{
			curPoint = m_node[i].myPoint;
			distance = tmpDis;
			curIndex = i;
		}
	}
	if(pIndex != NULL) *pIndex = curIndex;
	if(curIndex >= 0)
	{
		XVec2 ret;
		ret.x = 0.0f;
		for(int i = 0;i < curIndex;++ i)
		{
			ret.x += m_node[i].toNextLength;
		}
		ret.x += m_node[curIndex].myPoint.getLength(curPoint);
		ret.x = ret.x / m_lineLength;
		ret.y = sqrt(distance);
		//printf("%f,%f\n",ret.x,ret.y);
		return ret;
	}
	return XVec2(-1.0f);
}
void XNodeLine::setLeadMode(XLeadMode mode)
{
	if(m_leadMode == mode) return;	//���û�з����仯����ֱ���˳�
	switch (mode)
	{
	case LEAD_MODE_FOREWORD:m_leadMode = LEAD_MODE_FOREWORD;break;
	case LEAD_MODE_BACKWORD:m_leadMode = LEAD_MODE_BACKWORD;break;
	}
	if(!m_specialPoint.isEnable || m_specialPoint.isEnd) return;	//����Ҫ����
	//������µ������
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		if(m_isLoop)
		{
			m_specialPoint.curLength = m_lineLength - m_node[0].toPreviousLength - m_specialPoint.curLength;
			if(m_specialPoint.curLength < 0.0f) m_specialPoint.curLength += m_lineLength;
			m_specialPoint.upNodeOrder = m_specialPoint.upNodeOrder - 1;
			if(m_specialPoint.upNodeOrder < 0) m_specialPoint.upNodeOrder += m_node.size();
			//������Ҫ���㳤��
			m_specialPoint.upNodeLength = 0.0f;
			for(int i = 1;i <= m_specialPoint.upNodeOrder;++ i)
				m_specialPoint.upNodeLength += m_node[i].toPreviousLength;
		}else
		{
			m_specialPoint.curLength = m_lineLength - m_specialPoint.curLength;
			m_specialPoint.upNodeOrder = m_specialPoint.upNodeOrder - 1;
			if(m_specialPoint.upNodeOrder < 0) m_specialPoint.upNodeOrder += m_node.size();
			//������Ҫ���㳤��
			m_specialPoint.upNodeLength = 0.0f;
			for(int i = 0;i <= m_specialPoint.upNodeOrder;++ i)
				m_specialPoint.upNodeLength += m_node[i].toPreviousLength;
		}
		updateBezierInfo();
	}else
	{//����
		if(m_isLoop)
		{
			m_specialPoint.curLength = m_lineLength - m_node[0].toPreviousLength - m_specialPoint.curLength;
			if(m_specialPoint.curLength < 0.0f) m_specialPoint.curLength += m_lineLength;
			m_specialPoint.upNodeOrder = (m_specialPoint.upNodeOrder + 1) % m_node.size();
			//������Ҫ���㳤��
			m_specialPoint.upNodeLength = 0.0f;
			for(int i = m_node.size() - 2;i >= m_specialPoint.upNodeOrder;-- i)
				m_specialPoint.upNodeLength += m_node[i].toNextLength;
			if(m_specialPoint.upNodeLength >= m_lineLength) m_specialPoint.upNodeLength -= m_lineLength;
		}else
		{
			m_specialPoint.curLength = m_lineLength - m_specialPoint.curLength;
			m_specialPoint.upNodeOrder = (m_specialPoint.upNodeOrder + 1) % m_node.size();
			//������Ҫ���㳤��
			m_specialPoint.upNodeLength = 0.0f;
			for(int i = m_node.size() - 1;i >= m_specialPoint.upNodeOrder;-- i)
				m_specialPoint.upNodeLength += m_node[i].toNextLength;
			if(m_specialPoint.upNodeLength >= m_lineLength) m_specialPoint.upNodeLength -= m_lineLength;
		}
		updateBezierInfo();
	}
}
void XNodeLine::setWithBezier(bool flag)
{
	if(!m_withBezier && flag)
		updateBezierInfo();
	m_withBezier = flag;
}
void XNodeLine::bezierOptimization()
{
	updateData();
	float average = 0.0f;
	if(m_node.size() > 2 && m_withBezier)
	{
		if(m_isLoop)
		{
			average = m_lineLength / m_node.size();
			for(int i = 0;i < m_node.size();++ i)
			{
				if(m_node[i].toNextLength > average)
				{
					if(i < m_node.size() - 1)
					{
						addOneNode((m_node[i].myPoint + m_node[i + 1].myPoint) * 0.5f,i + 1);
						-- i;
					}else
					{
						addOneNode((m_node[i].myPoint + m_node[0].myPoint) * 0.5f,0);
						i = 0;
					}
				}
			}
		}else
		{
			average = m_lineLength / (m_node.size() - 1.0f);
			for(int i = 0;i < (int)(m_node.size()) - 1;++ i)
			{
				if(m_node[i].toNextLength > average)
				{
					addOneNode((m_node[i].myPoint + m_node[i + 1].myPoint) * 0.5f,i + 1);
				}
			}
		}
	}
	updateData();
}
void XNodeLine::getCrossInfo(const XNodeLine &line,std::vector<XNodeCrossInfo> &ret)
{//Ŀǰ�������ཻ������ļ��㲢û�������ϵ���ѧ��ʽ������Ŀǰ����Ϊ�Ǳ���������
	ret.clear();
	if(m_node.size() <= 0 || line.getNodeSum() <= 0) return;	//û���㹻������
	if(m_node.size() == 1) return;//ֻ��һ������������(Ŀǰ��δ����)
	if(line.getNodeSum() == 1) return;//ֻ��һ����������ο�(Ŀǰ��δ����)
	int endA = m_node.size() - 1;
	if(m_isLoop) endA = m_node.size();
	int endB = line.getNodeSum() - 1;
	if(line.getIsLoop()) endB = line.getNodeSum();
	if(m_withBezier || line.getWithBezier())
	{//�����һ������֧�ֱ�������ֵ������Ҫ���б�������ֵ����
		std::vector<XVec2> intersectPoints;
		XBezierSpline bsA,bsB;
		if(m_withBezier && line.getWithBezier())
		{//�����ڱ�����ƽ�����������μ������б��������ߵĽ���
			for(int i = 0;i < endA; ++ i)
			{
				setBezierSplinePar(bsA,i);
				for(int j = 0;j < endB;++ j)
				{
					line.setBezierSplinePar(bsB,j);
					intersectPoints.clear();
					XMath::getBezIntersectPointsEx(bsA,bsB,intersectPoints);
					for(auto kt = intersectPoints.begin();kt != intersectPoints.end();++ kt)
					{
						XNodeCrossInfo tmp;
						tmp.pos = bsA.getBezierSplineValue(kt->x);
						tmp.indexA = i;
						tmp.indexB = j;
						tmp.angleA = bsA.getBezierSplineAngle(kt->x);
						tmp.angleB = bsB.getBezierSplineAngle(kt->y);
						ret.push_back(tmp);
					}
				}
			}
		}else
		if(m_withBezier)
		{//ֻ��������ƽ��
			XVec2 p0,p1;
			for(int i = 0;i < endA; ++ i)
			{
				setBezierSplinePar(bsA,i);
				for(int j = 0;j < endB;++ j)
				{
					p0 = line.getNode(j);
					p1 = line.getNode((j + 1) % line.getNodeSum());
					bsB.init(p0,(p0 + p1) * 0.333333f,(p0 + p1) * 0.666667f,p1);
					intersectPoints.clear();
					XMath::getBezIntersectPointsEx(bsA,bsB,intersectPoints);
					for (auto kt = intersectPoints.begin(); kt != intersectPoints.end(); ++kt)
					{
						XNodeCrossInfo tmp;
						tmp.pos = bsA.getBezierSplineValue(kt->x);
						tmp.indexA = i;
						tmp.indexB = j;
						tmp.angleA = bsA.getBezierSplineAngle(kt->x);
						tmp.angleB = bsB.getBezierSplineAngle(kt->y);
						ret.push_back(tmp);
					}
				}
			}
		}else
		{//ֻ�жԷ���ƽ��
			XVec2 p0,p1;
			for(int i = 0;i < endA; ++ i)
			{
				p0 = getNode(i);
				p1 = getNode((i + 1) % getNodeSum());
				bsA.init(p0,(p0 + p1) * 0.333333f,(p0 + p1) * 0.666667f,p1);
				for(int j = 0;j < endB;++ j)
				{
					setBezierSplinePar(bsB,j);
					intersectPoints.clear();
					XMath::getBezIntersectPointsEx(bsA,bsB,intersectPoints);
					for (auto kt = intersectPoints.begin(); kt != intersectPoints.end(); ++kt)
					{
						XNodeCrossInfo tmp;
						tmp.pos = bsA.getBezierSplineValue(kt->x);
						tmp.indexA = i;
						tmp.indexB = j;
						tmp.angleA = bsA.getBezierSplineAngle(kt->x);
						tmp.angleB = bsB.getBezierSplineAngle(kt->y);
						ret.push_back(tmp);
					}
				}
			}
		}
	}else
	{
		for(int i = 0;i < endA; ++ i)
		{
			for(int j = 0;j < endB;++ j)
			{
				//����2�����ж��Ƿ��ཻ���ټ��㽻���λ��
				if(XMath::isLineIntersect(m_node[i].myPoint,m_node[(i + 1) % m_node.size()].myPoint,
					line.getNode(j),line.getNode((j + 1) % line.getNodeSum())))
				{
					XNodeCrossInfo tmp;
					tmp.pos = XMath::crossPointLineToLine(m_node[i].myPoint,m_node[(i + 1) % m_node.size()].myPoint,
						line.getNode(j),line.getNode((j + 1) % line.getNodeSum()));
					tmp.indexA = i;
					tmp.indexB = j;
					tmp.angleA = m_node[i].toNextAngle;
					tmp.angleB = line.getNodeInfo(j).toNextAngle;
					ret.push_back(tmp);
				}
				//����1���ȼ��㽻�㣬���жϽ����Ƿ��ڷ�Χ��
				//XVec2 pos = XMath::crossPointLineToLine(m_node[i].myPoint,m_node[(i + 1) % m_node.size()].myPoint,
				//	line.getNode(j),line.getNode((j + 1) % line.getNodeSum()));
				////�жϽ����Ƿ��ڷ�Χ��
				//if(XMath::isInArea(pos.x,m_node[i].myPoint.x,m_node[(i + 1) % m_node.size()].myPoint.x) && 
				//	XMath::isInArea(pos.y,m_node[i].myPoint.y,m_node[(i + 1) % m_node.size()].myPoint.y) &&
				//	XMath::isInArea(pos.x,line.getNode(j).x,line.getNode((j + 1) % line.getNodeSum()).x) &&
				//	XMath::isInArea(pos.y,line.getNode(j).y,line.getNode((j + 1) % line.getNodeSum()).y))
				//{//������ڷ�Χ�ڣ�������Ч�ĵ�
				//	XNodeCrossInfo tmp;
				//	tmp.pos = pos;
				//	tmp.indexA = i;
				//	tmp.indexB = j;
				//	tmp.angleA = m_node[i].toNextAngle;
				//	tmp.angleB = line.getNodeInfo(j).toNextAngle;
				//	ret.push_back(tmp);
				//}
			}
		}
	}
}
XNodeMatchInfo XNodeLine::simpleMatch(const XNodeLine &line)
{//��δʵ��
	XNodeMatchInfo ret;
	return ret;	
}
XNodePointInfo XNodeLine::getNodePointInfo(float distance)	//������뿪ʼ�����Ϊdistance�ĵ����Ϣ
{//ע�⣺���������δ��������
	updateData();
	XNodePointInfo ret;
	if(m_node.size() <= 0) return ret;	//���ݷǷ�
	if(m_withBezier)
	{
		if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
		{//˳��
			if(m_isLoop)
			{//ѭ��
				if(distance >= m_bezLineLength || distance <= 0.0f)
				{//���
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,0);
						ret.angle = tmpBS.getBezierSplineAngle(0.0f);
					}else
					{//ֱ��
						ret.angle = m_node[0].toNextAngle;
					}
					ret.isEnd = (distance > 0.0f);
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;

					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = 0;i < m_node.size(); ++ i)
					{
						tmpDis += m_node[i].toBezNextLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toBezNextLength;
						break;
					}
					ret.isEnd = false;
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,ret.upIndex);
						ret.pos = tmpBS.getBezierSplineValueEx(tmpDis);
						ret.angle = tmpBS.getBezierSplineAngleEx(tmpDis);
					}else
					{//ֱ��
						ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[(ret.upIndex + 1) % m_node.size()].myPoint,tmpDis);
						if(m_withAngleSmooth)
						{
							float tmpAngle = m_node[(ret.upIndex + 1) % m_node.size()].toNextAngle - m_node[ret.upIndex].toNextAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (PI2 + tmpAngle);
							}
						}else ret.angle = m_node[ret.upIndex].toNextAngle;
					}
					return ret;
				}
			}else
			{//��ѭ��
				if(distance >= m_bezLineLength)
				{//���
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,m_node.size() - 2);
						ret.angle = tmpBS.getBezierSplineAngle(1.0f);
					}else
					{//ֱ��
						ret.angle = m_node[m_node.size() - 1].toNextAngle;
					}
					ret.isEnd = true;
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				if(distance <= 0.0f)
				{//��δ��ʼ
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,0);
						ret.angle = tmpBS.getBezierSplineAngle(0.0f);
					}else
					{//ֱ��
						ret.angle = m_node[0].toNextAngle;
					}
					ret.isEnd = false;
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = 0;i < m_node.size(); ++ i)
					{
						tmpDis += m_node[i].toBezNextLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toBezNextLength;
						break;
					}
					ret.isEnd = false;
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,ret.upIndex);
						ret.pos = tmpBS.getBezierSplineValueEx(tmpDis);
						ret.angle = tmpBS.getBezierSplineAngleEx(tmpDis);
					}else
					{//ֱ��
						ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex + 1].myPoint,tmpDis);
						if(m_withAngleSmooth)
						{
							float tmpAngle = m_node[ret.upIndex + 1].toNextAngle - m_node[ret.upIndex].toNextAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (PI2 + tmpAngle);
							}
						}else ret.angle = m_node[ret.upIndex].toNextAngle;
					}
					return ret;
				}
			}
		}else
		{//����
			if(m_isLoop)
			{//ѭ��
				if(distance >= m_bezLineLength || distance <= 0.0f)
				{//���
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,m_node.size() - 1,true);
						ret.angle = tmpBS.getBezierSplineAngle(0.0f);
					}else
					{//ֱ��
						ret.angle = m_node[m_node.size() - 1].toPreviousAngle;
					}
					ret.isEnd = (distance > 0.0f);
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = m_node.size() - 1;i >= 0;-- i)
					{
						tmpDis += m_node[i].toBezPreviousLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toBezPreviousLength;
						break;
					}
					ret.isEnd = false;
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,ret.upIndex,true);
						ret.pos = tmpBS.getBezierSplineValueEx(tmpDis);
						ret.angle = tmpBS.getBezierSplineAngleEx(tmpDis);
					}else
					{//ֱ��
						if(ret.upIndex == 0)
							ret.pos = XMath::lineSlerp(m_node[0].myPoint,m_node[m_node.size() - 1].myPoint,tmpDis);
						else
							ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex - 1].myPoint,tmpDis);
						if(m_withAngleSmooth)
						{
							float tmpAngle;
							if(ret.upIndex == 0)
								tmpAngle = m_node[m_node.size() - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
							else
								tmpAngle = m_node[ret.upIndex - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (PI2 + tmpAngle);
							}
						}else ret.angle = m_node[ret.upIndex].toPreviousAngle;
					}
					return ret;
				}
			}else
			{//��ѭ��
				if(distance >= m_bezLineLength)
				{//���
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,1,true);
						ret.angle = tmpBS.getBezierSplineAngle(1.0f);
					}else
					{//ֱ��
						ret.angle = m_node[0].toPreviousAngle;
					}
					ret.isEnd = true;
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;
					return ret;
				}else
				if(distance <= 0.0f)
				{//��δ��ʼ
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,m_node.size() - 1,true);
						ret.angle = tmpBS.getBezierSplineAngle(0.0f);
					}else
					{//ֱ��
						ret.angle = m_node[m_node.size() - 1].toPreviousAngle;
					}
					ret.isEnd = false;
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = m_node.size() - 1;i >= 0;-- i)
					{
						tmpDis += m_node[i].toBezPreviousLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toBezPreviousLength;
						break;
					}
					ret.isEnd = false;
					if(m_node.size() > 2)
					{
						XBezierSpline tmpBS;
						setBezierSplinePar(tmpBS,ret.upIndex,true);
						ret.pos = tmpBS.getBezierSplineValueEx(tmpDis);
						ret.angle = tmpBS.getBezierSplineAngleEx(tmpDis);
					}else
					{//ֱ��
						ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex - 1].myPoint,tmpDis);
						if(m_withAngleSmooth)
						{
							float tmpAngle = m_node[ret.upIndex - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
							if(tmpAngle <= PI && tmpAngle >= -PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * tmpAngle;
							}else
							if(tmpAngle > PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (tmpAngle - PI2);
							}else
							if(tmpAngle < -PI)
							{
								ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (PI2 + tmpAngle);
							}
						}else ret.angle = m_node[ret.upIndex].toPreviousAngle;
					}
					return ret;
				}
			}
		}	
	}else
	{
		if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
		{//˳��
			if(m_isLoop)
			{//ѭ��
				if(distance >= m_lineLength || distance <= 0.0f)
				{//���
					ret.angle = m_node[0].toNextAngle;
					ret.isEnd = (distance > 0.0f);
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;

					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = 0;i < m_node.size(); ++ i)
					{
						tmpDis += m_node[i].toNextLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toNextLength;
						break;
					}
					ret.isEnd = false;
					ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[(ret.upIndex + 1) % m_node.size()].myPoint,tmpDis);
					if(m_withAngleSmooth)
					{
						float tmpAngle = m_node[(ret.upIndex + 1) % m_node.size()].toNextAngle - m_node[ret.upIndex].toNextAngle;
						if(tmpAngle <= PI && tmpAngle >= -PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * tmpAngle;
						}else
						if(tmpAngle > PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (tmpAngle - PI2);
						}else
						if(tmpAngle < -PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (PI2 + tmpAngle);
						}
					}else ret.angle = m_node[ret.upIndex].toNextAngle;
					return ret;
				}
			}else
			{//��ѭ��
				if(distance >= m_lineLength)
				{//���
					ret.angle = m_node[m_node.size() - 1].toNextAngle;
					ret.isEnd = true;
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				if(distance <= 0.0f)
				{//��δ��ʼ
					ret.angle = m_node[0].toNextAngle;
					ret.isEnd = false;
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = 0;i < m_node.size(); ++ i)
					{
						tmpDis += m_node[i].toNextLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toNextLength;
						break;
					}
					ret.isEnd = false;
					ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex + 1].myPoint,tmpDis);
					if(m_withAngleSmooth)
					{
						float tmpAngle = m_node[ret.upIndex + 1].toNextAngle - m_node[ret.upIndex].toNextAngle;
						if(tmpAngle <= PI && tmpAngle >= -PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * tmpAngle;
						}else
						if(tmpAngle > PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (tmpAngle - PI2);
						}else
						if(tmpAngle < -PI)
						{
							ret.angle = m_node[ret.upIndex].toNextAngle + tmpDis * (PI2 + tmpAngle);
						}
					}else ret.angle = m_node[ret.upIndex].toNextAngle;
				}
				return ret;
			}
		}else
		{//����
			if(m_isLoop)
			{//ѭ��
				if(distance >= m_lineLength || distance <= 0.0f)
				{//���
					ret.angle = m_node[m_node.size() - 1].toPreviousAngle;
					ret.isEnd = (distance > 0.0f);
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = m_node.size() - 1;i >= 0;-- i)
					{
						tmpDis += m_node[i].toPreviousLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toPreviousLength;
						break;
					}
					ret.isEnd = false;

					if(ret.upIndex == 0)
						ret.pos = XMath::lineSlerp(m_node[0].myPoint,m_node[m_node.size() - 1].myPoint,tmpDis);
					else
						ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex - 1].myPoint,tmpDis);
					if(m_withAngleSmooth)
					{
						float tmpAngle;
						if(ret.upIndex == 0)
							tmpAngle = m_node[m_node.size() - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
						else
							tmpAngle = m_node[ret.upIndex - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
						if(tmpAngle <= PI && tmpAngle >= -PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * tmpAngle;
						}else
						if(tmpAngle > PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (tmpAngle - PI2);
						}else
						if(tmpAngle < -PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (PI2 + tmpAngle);
						}
					}else ret.angle = m_node[ret.upIndex].toPreviousAngle;

					return ret;
				}
			}else
			{//��ѭ��
				if(distance >= m_lineLength)
				{//���
					ret.angle = m_node[0].toPreviousAngle;
					ret.isEnd = true;
					ret.pos = m_node[0].myPoint;
					ret.upIndex = 0;
					return ret;
				}else
				if(distance <= 0.0f)
				{//��δ��ʼ
					ret.angle = m_node[m_node.size() - 1].toPreviousAngle;
					ret.isEnd = false;
					ret.pos = m_node[m_node.size() - 1].myPoint;
					ret.upIndex = m_node.size() - 1;
					return ret;
				}else
				{//û���յ�
					float tmpDis = 0.0f;
					for(int i = m_node.size() - 1;i >= 0;-- i)
					{
						tmpDis += m_node[i].toPreviousLength;
						if(tmpDis <= distance) continue;
						ret.upIndex = i;
						tmpDis = 1.0f - (tmpDis - distance) / m_node[i].toPreviousLength;
						break;
					}
					ret.isEnd = false;
					ret.pos = XMath::lineSlerp(m_node[ret.upIndex].myPoint,m_node[ret.upIndex - 1].myPoint,tmpDis);
					if(m_withAngleSmooth)
					{
						float tmpAngle = m_node[ret.upIndex - 1].toPreviousAngle - m_node[ret.upIndex].toPreviousAngle;
						if(tmpAngle <= PI && tmpAngle >= -PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * tmpAngle;
						}else
						if(tmpAngle > PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (tmpAngle - PI2);
						}else
						if(tmpAngle < -PI)
						{
							ret.angle = m_node[ret.upIndex].toPreviousAngle + tmpDis * (PI2 + tmpAngle);
						}
					}else ret.angle = m_node[ret.upIndex].toPreviousAngle;
					return ret;
				}
			}
		}
	}
}
float XNodeLine::getDistanceFromStart(int index)				//����ĳ���ڵ������ʼ��֮��ľ���
{
	float ret = 0.0f;
	if(m_node.size() <= 0) return ret;
	if((m_leadMode & LEAD_MODE_FOREWORD) != 0)
	{//˳��
		if(index <= 0) return ret;
		if(index >= (int)(m_node.size()) - 1) index = (int)(m_node.size()) - 1;

		if(m_withBezier)
		{
			for(int i = 0;i < index;++ i)
			{
				ret += m_node[i].toBezNextLength;
			}
		}else
		{
			for(int i = 0;i < index;++ i)
			{
				ret += m_node[i].toNextLength;
			}
		}
		return ret;
	}else
	{
		if(index <= 0) index = 0;
		if(index >= (int)(m_node.size()) - 1) return ret;

		if(m_withBezier)
		{
			for(int i = (int)(m_node.size()) - 1;i > index;-- i)
			{
				ret += m_node[i].toBezPreviousLength;
			}
		}else
		{
			for(int i = (int)(m_node.size()) - 1;i > index;-- i)
			{
				ret += m_node[i].toPreviousLength;
			}
		}
		return ret;
	}
}
void XNodeLine::setBezierSplinePar(XBezierSpline &sp,int index,bool isBack)const	//���õ�index����ı��������ߵĲ���
{
	if(index < 0 || index >= m_node.size()) return;	//�Ƿ�������
	if(isBack)	
	{
		if(index > 0)
			sp.init(m_node[index].myPoint,m_bezierPoint[index << 1],m_bezierPoint[(index << 1) - 1],m_node[index - 1].myPoint);
		else
			sp.init(m_node[0].myPoint,m_bezierPoint[0],	m_bezierPoint[(m_node.size() << 1) - 1],m_node[m_node.size() - 1].myPoint);
	}else
	{
		if(index < (int)(m_node.size()) - 1)	//�������һ����
			sp.init(m_node[index].myPoint,m_bezierPoint[(index << 1) + 1],m_bezierPoint[(index << 1) + 2],m_node[index + 1].myPoint);
		else
			sp.init(m_node[index].myPoint,m_bezierPoint[(index << 1) + 1],m_bezierPoint[0],m_node[0].myPoint);
	}
}
//��ȡ����ϵ��������ֵԽ�����ϵ��Խ��
float XNodeLine::getDeformityRate()const
{//��󳤶�����С���ȵı�ֵ
	if(m_node.size() <= 1) return 1.0f;
	float minLen = 0.0f;
	float maxLen = 0.0f;
	if(m_withBezier)
	{
		minLen = m_node[0].toBezNextLength;
		maxLen = m_node[0].toBezNextLength;
		if(m_isLoop)
		{
			for(std::vector<XNode>::const_iterator it = m_node.begin() + 1;it != m_node.end();++ it)
			{
				if(it->toBezNextLength > maxLen)
					maxLen = it->toBezNextLength;
				if(it->toBezNextLength < minLen)
					minLen = it->toBezNextLength;
			}
		}else
		{
			for(std::vector<XNode>::const_iterator it = m_node.begin() + 1;it != m_node.end() - 1;++ it)
			{
				if(it->toBezNextLength > maxLen)
					maxLen = it->toBezNextLength;
				if(it->toBezNextLength < minLen)
					minLen = it->toBezNextLength;
			}
		}
	}else
	{
		minLen = m_node[0].toNextLength;
		maxLen = m_node[0].toNextLength;
		if(m_isLoop)
		{
			for(std::vector<XNode>::const_iterator it = m_node.begin() + 1;it != m_node.end();++ it)
			{
				if(it->toNextLength > maxLen)
					maxLen = it->toNextLength;
				if(it->toNextLength < minLen)
					minLen = it->toNextLength;
			}
		}else
		{
			for(std::vector<XNode>::const_iterator it = m_node.begin() + 1;it != m_node.end() - 1;++ it)
			{
				if(it->toNextLength > maxLen)
					maxLen = it->toNextLength;
				if(it->toNextLength < minLen)
					minLen = it->toNextLength;
			}
		}
	}
	if(minLen <= 0.0f) return FLT_MAX;
	return maxLen / minLen;
}
#if !WITH_INLINE_FILE
#include "XNodeLine.inl"
#endif
}