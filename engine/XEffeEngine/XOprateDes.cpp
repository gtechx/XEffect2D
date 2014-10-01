#include "XOprateDes.h"
void _XOperateManager::clearAll()
{
	m_nowStateIndex = -1;
	m_operateIndex.clear();
	for(int i = 0;i < m_operateDes.size();++ i)
	{
		XDELETE(m_operateDes[i].pData);
	}
	m_operateDes.clear();
}
void _XOperateManager::undo()
{
	if(!canUndo()) return;	//������ܽ��г�����������ֱ�ӷ���
	for(int i = m_operateIndex[m_nowStateIndex];
		i > m_operateIndex[m_nowStateIndex - 1];-- i)
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
	m_nowStateIndex = m_nowStateIndex - 1;	//��ǳ�������һ������
	//printf("��ǰ����!%d\n",m_nowStateIndex);
}
void _XOperateManager::redo()
{
	if(!canRedo()) return;	//������ܽ���������������ֱ�ӷ���
	for(int i = m_operateIndex[m_nowStateIndex] + 1;
		i <= m_operateIndex[m_nowStateIndex + 1];++ i)
	{//�������еķ���״̬�仯���������Щ�����״̬����Ϊ��ǰ״̬
		m_operateDes[i].pObj->setOprateState(m_operateDes[i].pData);
	}
	m_nowStateIndex = m_nowStateIndex + 1;
	//printf("��ǰ����!%d\n",m_nowStateIndex);
}
void _XOperateManager::addAOperate(_XBasicOprate *obj)
{//���һ������
	if(obj == NULL) return;
	//�����ǰ״̬ʱ����"����"�����ģ�Ҳ���ǵ�ǰ״̬�������µ�״̬ʱ��������Ҫ��״̬�����Ͷ�����������
	if(m_nowStateIndex >= 0 && m_nowStateIndex != m_operateIndex.size() - 1)
	{
		int size = m_operateIndex.size() - 1 - m_nowStateIndex;
		for(int i = 0;i < size;++ i)
			m_operateIndex.pop_back();
		if(m_operateIndex[m_nowStateIndex] != m_operateDes.size() - 1)
		{
			size = m_operateDes.size() - 1 - m_operateIndex[m_nowStateIndex];
			for(int i = 0;i < size;++ i)
			{
				XDELETE(m_operateDes[m_operateDes.size() - 1].pData);	//ɾ����������������
				m_operateDes.pop_back();
			}
		}
	}
	//�����ǰ�����״̬����һ�������ظ��������ظ�����
	for(int i = m_operateDes.size() - 1;i >= 0;-- i)
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
	_XOperateDes tmp;
	tmp.pObj = obj;
	tmp.pData = obj->getOprateState();
	m_operateDes.push_back(tmp);
	//printf("״̬!%d\n",m_operateDes.size());
}
void _XOperateManager::setOperateOver()
{//���ö������
	//��������в�û���µĶ�����������ֱ�ӷ���
	if(m_operateDes.size() <= 0 || (m_operateIndex.size() > 0 && 
		m_operateDes.size() - 1 <= m_operateIndex[m_operateIndex.size() - 1])) return;
//	if(!((m_nowStateIndex < 0 && m_operateDes.size() > 0)
//		|| (m_operateIndex.size() > 0 && m_operateDes.size() - 1 > m_operateIndex[m_operateIndex.size() - 1]))) return;
	//������Ҫ���ظ���״̬����������޳�,(��һ������������ͬһ��������ڶ�����������������û�б�Ҫ�ģ�ֻ��Ҫ�������һ��״̬����)
	if(m_nowStateIndex < 0)
	{//��һ��
		for(int i = 0;i < m_operateDes.size() - 1;++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//һ�����������ڴ���ͬһ������Ķ��״̬������ֻ�������һ��
					_XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
					for(int k = j;k < m_operateDes.size() - 1;++ k)
					{
						m_operateDes[k] = m_operateDes[k + 1];
					}
					m_operateDes.pop_back();
					XDELETE(tmp.pData);
					-- j;
					//printf("�����޳�!\n");
				}
			}
		}
	}else
	{//���ǵ�һ��
		for(int i = m_operateIndex[m_nowStateIndex] + 1;i < m_operateDes.size();++ i)
		{
			for(int j = i + 1;j < m_operateDes.size();++ j)
			{
				if(m_operateDes[i].pObj == m_operateDes[j].pObj)
				{//һ�����������ڴ���ͬһ������Ķ��״̬������ֻ�������һ��
					_XOperateDes tmp = m_operateDes[i];
					m_operateDes[i] = m_operateDes[j];
					for(int k = j;k < m_operateDes.size() - 1;++ k)
					{
						m_operateDes[k] = m_operateDes[k + 1];
					}
					m_operateDes.pop_back();
					XDELETE(tmp.pData);
					-- j;
					//printf("�����޳�!\n");
				}
			}
		}
	}
	//��¼�µĶ�������
	++m_nowStateIndex;
	m_operateIndex.push_back(m_operateDes.size() - 1);
	//printf("����!%d\n",m_operateIndex.size()); 
}
void _XOperateManager::decreaseObj(_XBasicOprate *obj)
{//(��������û������)
	if(obj == NULL) return;
	for(int i = m_operateDes.size() - 1;i >= 0;-- i)
	{
		if(m_operateDes[i].pObj == obj)
		{//��Ҫ��������
			//�������ֵ
			XDELETE(m_operateDes[i].pData);
			for(int j = i;j < m_operateDes.size() - 1;++ j)
			{
				m_operateDes[j] = m_operateDes[j + 1];
			}
			m_operateDes.pop_back();
			//������������
			for(int j = 0;j < m_operateIndex.size();++ j)
			{//���������ݼ������������ظ�
				if(m_operateIndex[j] >= i) --m_operateIndex[j];
			}
			//�����޳��ظ�
			for(int j = 0;j < m_operateIndex.size();++ j)
			{
				if((j < m_operateIndex.size() - 1 && m_operateIndex[j] == m_operateIndex[j + 1]) || 
					m_operateIndex[j] < 0)
				{//�����ظ�����������һ������
					if(m_nowStateIndex > j) -- m_nowStateIndex;
					for(int k = j + 1;k < m_operateIndex.size() - 1;++ k)
					{
						m_operateIndex[k] = m_operateIndex[k + 1];
					}
					m_operateIndex.pop_back();
				}
			}
			if(m_operateIndex.size() <= 0) m_nowStateIndex = -1;
		}
	}
}