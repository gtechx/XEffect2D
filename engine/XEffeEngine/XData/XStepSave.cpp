#include "XStdHead.h"
#include "XStepSave.h"
namespace XE{
bool XStepSave::init(int size,int sum)
{
	if(m_isInited) return false;
	if(size <= 0 || sum <= 0) return false;
	m_coreSum = sum;
	m_coreSize = size;
	m_cores = XMem::createArrayMem<unsigned char *>(sum);
	if(m_cores == NULL) return false;
	for(int i = 0;i < m_coreSum;++ i)
	{
		m_cores[i] = XMem::createArrayMem<unsigned char>(m_coreSize);
		if(m_cores[i] == NULL) return false;
		m_freeBuff.push_back(m_cores[i]);
	}
	m_usedBuff.clear();
	//��������ڴ�ռ�ĳ�ʼ��(��δ���)
	m_curIndex = -1;	//��ʶΪû������

	m_isInited = true;
	return true;
}
bool XStepSave::pushACore(const void *p)	//�����������һ��״̬
{//��������
	if(p == NULL || !m_isInited) return false;
	if(m_curIndex == -1 || m_curIndex + 1 == (int)(m_usedBuff.size()))
	{//�¿�ʼ�����߱�ǩ��β��
		if(m_freeBuff.size() <= 0)
		{//��Դ�Ѿ�����
			unsigned char *tmp = m_usedBuff[0];
			m_usedBuff.pop_front();
			memcpy(tmp,p,m_coreSize);
			m_usedBuff.push_back(tmp);
			m_curIndex = (int)(m_usedBuff.size()) - 1;
		}else
		{//��Դ��δ����
			unsigned char *tmp = m_freeBuff[0];
			m_freeBuff.pop_front();
			memcpy(tmp,p,m_coreSize);
			m_usedBuff.push_back(tmp);
			++ m_curIndex;
		}
	}else
	{//��ǩ���м�
		while(m_curIndex + 1 != (int)(m_usedBuff.size()))
		{
			assert((int)(m_usedBuff.size()) - 1 >= 0);
			unsigned char *tmp = *(m_usedBuff.end() - 1);
			m_usedBuff.pop_back();
			m_freeBuff.push_back(tmp);
		}
		pushACore(p);
	}
	//forTest();
	return true;
}
void XStepSave::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_coreSum;++ i)
	{
		XMem::XDELETE_ARRAY(m_cores[i]);
	}
	XMem::XDELETE_ARRAY(m_cores);
	m_usedBuff.clear();
	m_freeBuff.clear();
	m_curIndex = -1;
	m_isInited = false;
}
void XStepSave::forTest()
{
	int i = 0;
	for (auto it = m_usedBuff.begin(); it != m_usedBuff.end(); ++it,++ i)
	{
		if (m_curIndex == i) printf("+%s ", *it);
		else printf("%s ", *it);
	}
	printf("\n");
}
#if !WITH_INLINE_FILE
#include "XStepSave.inl"
#endif
}