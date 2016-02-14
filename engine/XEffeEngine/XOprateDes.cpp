#include "XStdHead.h"
#include "XOprateDes.h"
namespace XE{
void XOperateDes::release()
{//������ܴ������⣬��pObj��ʵ�屻�ͷ�ʱ����������crash��
	if(pObj == NULL) return;
	pObj->releaseOprateStateData(pData);
	pData = NULL;
}
void XOperateManager::clearAll()
{
	m_curStateIndex = -1;
	m_operateIndex.clear();
	for(unsigned int i = 0;i < m_operateDes.size();++ i)
	{
		m_operateDes[i].release();
		//XMem::XDELETE(m_operateDes[i].pData);
	}
	m_operateDes.clear();
}
void XOperateManager::undo()
{
	if(!canUndo()) return;	//������ܽ��г�����������ֱ�ӷ���
	for(int i = m_operateIndex[m_curStateIndex];
		i > m_operateIndex[m_curStateIndex - 1];-- i)
	{//�������з���״̬�仯�����������Щ�����״̬���û���һ����Ч��״̬
		for(int j = i - 1;j >= 0;-- j)
		{
			if(m_operateDes[i].pObj == m_operateDes[j].pObj)
			{
				m_operateDes[j].pObj->setOprateState(m_operateDes[j].pData);
				break;
			}
		}
	}
	m_curStateIndex = m_curStateIndex - 1;	//��ǳ�������һ������
	//printf("��ǰ����!%d\n",m_curStateIndex);
}
void XOperateManager::redo()
{
	if(!canRedo()) return;	//������ܽ���������������ֱ�ӷ���
	for(int i = m_operateIndex[m_curStateIndex] + 1;
		i <= m_operateIndex[m_curStateIndex + 1];++ i)
	{//�������еķ���״̬�仯���������Щ�����״̬����Ϊ��ǰ״̬
		m_operateDes[i].pObj->setOprateState(m_operateDes[i].pData);
	}
	m_curStateIndex = m_curStateIndex + 1;
	//printf("��ǰ����!%d\n",m_curStateIndex);
}
void XOperateManager::addAOperate(XBasicOprate *obj)
{//���һ������
	if(obj == NULL) return;
	//�����ǰ״̬ʱ����"����"�����ģ�Ҳ���ǵ�ǰ״̬�������µ�״̬ʱ��������Ҫ��״̬�����Ͷ�����������
	if(m_curStateIndex >= 0 && m_curStateIndex != (int)(m_operateIndex.size()) - 1)
	{
		int size = (int)(m_operateIndex.size()) - 1 - m_curStateIndex;
		for(int i = 0;i < size;++ i)
			m_operateIndex.pop_back();
		//if(size > 0)
		//	m_operateIndex.erase(m_operateIndex.begin() + m_operateIndex.size() - size,m_operateIndex.end());
		if(m_operateIndex[m_curStateIndex] != (int)(m_operateDes.size()) - 1)
		{
			size = (int)(m_operateDes.size()) - 1 - m_operateIndex[m_curStateIndex];
			for(int i = 0;i < size;++ i)
			{
				m_operateDes[(int)(m_operateDes.size()) - 1].release();
				//XMem::XDELETE(m_operateDes[(int)(m_operateDes.size()) - 1].pData);	//ɾ����������������
				m_operateDes.pop_back();
			}
		}
	}
	//�����ǰ�����״̬����һ�������ظ��������ظ�����
	for(int i = (int)(m_operateDes.size()) - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{
			 if(obj->isSameState(m_operateDes[i].pData))
			 {
				 //printf("����ظ�����,����\n");
				 return;
			 }
			 break;
		}
	}
	//�������״̬�������뵽״̬����������
	XOperateDes tmp;
	tmp.pObj = obj;
	tmp.pData = obj->getOprateState();
	m_operateDes.push_back(tmp);
	//printf("״̬!%d\n",m_operateDes.size());
}
void XOperateManager::setOperateOver()
{//���ö������
	//��������в�û���µĶ�����������ֱ�ӷ���
	if(m_operateDes.size() <= 0 || (m_operateIndex.size() > 0 && 
		(int)(m_operateDes.size()) - 1 <= m_operateIndex[(int)(m_operateIndex.size()) - 1])) return;
//	if(!((m_curStateIndex < 0 && m_operateDes.size() > 0)
//		|| (m_operateIndex.size() > 0 && (int)(m_operateDes.size()) - 1 > m_operateIndex[(int)(m_operateIndex.size()) - 1]))) return;
	//������Ҫ���ظ���״̬����������޳�,(��һ������������ͬһ��������ڶ�����������������û�б�Ҫ�ģ�ֻ��Ҫ�������һ��״̬����)
	if(m_curStateIndex < 0)
	{//��һ��
		for(int i = 0;i < (int)(m_operateDes.size()) - 1;++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//һ�����������ڴ���ͬһ������Ķ��״̬������ֻ�������һ��
					XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
				//	for(int k = j;k < (int)(m_operateDes.size()) - 1;++ k)
				//	{
				//		m_operateDes[k] = m_operateDes[k + 1];
				//	}
				//	m_operateDes.pop_back();
					m_operateDes.erase(m_operateDes.begin() + j);
					//XMem::XDELETE(tmp.pData);
					tmp.release();
					-- j;
					//printf("�����޳�!\n");
				}
			}
		}
	}else
	{//���ǵ�һ��
		for(int i = m_operateIndex[m_curStateIndex] + 1;i < m_operateDes.size();++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//һ�����������ڴ���ͬһ������Ķ��״̬������ֻ�������һ��
					XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
				//	for(int k = j;k < (int)(m_operateDes.size()) - 1;++ k)
				//	{
				//		m_operateDes[k] = m_operateDes[k + 1];
				//	}
				//	m_operateDes.pop_back();
					m_operateDes.erase(m_operateDes.begin() + j);
					//XMem::XDELETE(tmp.pData);
					tmp.release();
					-- j;
					//printf("�����޳�!\n");
				}
			}
		}
	}
	//��¼�µĶ�������
	++m_curStateIndex;
	m_operateIndex.push_back((int)(m_operateDes.size()) - 1);
	//printf("����!%d\n",m_operateIndex.size()); 
}
void XOperateManager::decreaseObj(XBasicOprate *obj)
{//(��������û������)
	if(obj == NULL) return;
	for(int i = (int)(m_operateDes.size()) - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{//��Ҫ��������
			//�������ֵ
			m_operateDes[i].release();
			//XMem::XDELETE(m_operateDes[i].pData);
		//	for(int j = i;j < (int)(m_operateDes.size()) - 1;++ j)
		//	{
		//		m_operateDes[j] = m_operateDes[j + 1];
		//	}
		//	m_operateDes.pop_back();
			m_operateDes.erase(m_operateDes.begin() + i);
			//������������
			for(unsigned int j = 0;j < m_operateIndex.size();++ j)
			{//���������ݼ������������ظ�
				if(m_operateIndex[j] >= i) --m_operateIndex[j];
			}
			//�����޳��ظ�
			for(unsigned int j = 0;j < m_operateIndex.size();++ j)
			{
				if((j < (int)(m_operateIndex.size()) - 1 && m_operateIndex[j] == m_operateIndex[j + 1]) || 
					m_operateIndex[j] < 0)
				{//�����ظ�����������һ������
					if(m_curStateIndex > j) -- m_curStateIndex;
				//	for(int k = j + 1;k < (int)(m_operateIndex.size()) - 1;++ k)
				//	{
				//		m_operateIndex[k] = m_operateIndex[k + 1];
				//	}
				//	m_operateIndex.pop_back();
					m_operateIndex.erase(m_operateIndex.begin() + j + 1);
				}
			}
			if(m_operateIndex.size() <= 0) m_curStateIndex = -1;
		}
	}
}
#if !WITH_INLINE_FILE
#include "XOprateDes.inl"
#endif
}