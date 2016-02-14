#include "XStdHead.h"
#include "XTempSave.h"
namespace XE{
bool XTempSave::init(int size,int sum)
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
bool XTempSave::pushACore(const unsigned char *p)	//�����������һ��״̬
{//��������
	if(p == NULL || !m_isInited) return false;
	if(m_curIndex == -1 || m_curIndex + 1 == m_usedBuff.size())
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
		while(m_curIndex + 1 != m_usedBuff.size())
		{
			assert((int)(m_usedBuff.size()) - 1 >= 0);
			unsigned char *tmp = m_usedBuff[(int)(m_usedBuff.size()) - 1];
			m_usedBuff.pop_back();
			m_freeBuff.push_back(tmp);
		}
		pushACore(p);
	}
	//forTest();
	return true;
}
void XTempSave::release()
{
	if(!m_isInited) return;
	for(int i = 0;i < m_coreSum;++ i)
	{
		XMem::XDELETE_ARRAY(m_cores[i]);
	}
	XMem::XDELETE_ARRAY(m_cores);
	m_isInited = false;
}
void XTempSave::forTest()
{
	for(unsigned int i = 0;i < m_usedBuff.size();++ i)
	{
		if(m_curIndex == i) printf("+%s ",m_usedBuff[i]);
		else printf("%s ",m_usedBuff[i]);
	}
	printf("\n");
}
#if !WITH_INLINE_FILE
#include "XTempSave.inl"
#endif
}