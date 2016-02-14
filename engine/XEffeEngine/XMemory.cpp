#include "XStdHead.h"
#include "XMemory.h"
namespace XE{
namespace XMem
{
	unsigned char *spliceData(unsigned char * baseBuff, int &baseLen, int &offset, const unsigned char * addBuff, int len)
	{
		if (addBuff == NULL) return baseBuff;
		if (offset + len > baseLen || baseBuff == NULL)
		{//���ݳ�������
			if (baseLen == 0) baseLen = 1;
			while (true)
			{
				baseLen = baseLen << 1;
				if (offset + len <= baseLen) break;
			}
			unsigned char *temp = XMem::createArrayMem<unsigned char>(baseLen);
			if (baseBuff != NULL) memcpy(temp, baseBuff, offset);
			XMem::XDELETE_ARRAY(baseBuff);
			memcpy(temp + offset, addBuff, len);
			offset += len;
			return temp;
		}
		else
		{//����û�г�������
			memcpy(baseBuff + offset, addBuff, len);
			offset += len;
			return baseBuff;
		}
	}
}
	bool XBuffer::pushData(const unsigned char *data,int len)	//��������������
	{
		if(data == NULL || len <= 0) return false;
		//���潫���ݼ���
		if(m_buffSize - m_usage < len)
		{//�ڴ�ռ䲻����������Ҫ�����ڴ�ռ�
			if(!XMem::resizeMem(m_pData,m_usage,len + m_usage)) return false;
			m_buffSize = len + m_usage;
		}
		memcpy(m_pData + m_usage,data,len);
		m_usage += len;
		return true;
	}
	bool XBuffer::pushAData(int data)
	{
		//���潫���ݼ���
		if(m_buffSize - m_usage < 1)
		{//�ڴ�ռ䲻����������Ҫ�����ڴ�ռ�
			if(!XMem::resizeMem(m_pData,m_usage,1 + m_usage)) return false;
			m_buffSize = 1 + m_usage;
		}
		m_pData[m_usage] = data;
		//memcpy(m_pData + m_usage,data,len);
		++ m_usage;
		return true;
	}
	bool XBuffer::resize(int size)
	{
		if(size <= 0) return false;
		if(size == m_buffSize) return true;
		unsigned char *tmp = m_pData;
		m_pData = XMem::createArrayMem<unsigned char>(size);
		if(m_pData == NULL)
		{//�ڴ����ʧ��
			m_pData = tmp;
			return false;
		}
		//Ŀǰ�Ĳ��ԣ�������ǰ�����ݣ��ȽϺ��ʵĲ����Ǿ����ܶ�ı���ԭ�е�����(��δʵ��)
		m_usage = 0;
		XMem::XDELETE_ARRAY(tmp);
		m_buffSize = size;
		return true;
	}
	bool XBuffer::decreaseData(int len)
	{
		if(len <= 0 || len > m_usage) return false;	//�Ƿ�������
		if(len == m_usage)
		{
			m_usage = 0;
			return true;
		}
		memmove(m_pData,m_pData + len,m_usage - len);
		m_usage -= len;
		return true;
	}
}