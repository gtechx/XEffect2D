#ifndef _JIA_XMEMORY_
#define _JIA_XMEMORY_
//�����װ���й����ڴ��������غ���
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "string.h"
namespace XE {
	//#define XMem::XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
	//#define XMem::XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
	//#define FILE_ERROR(p) {fclose(p);return XFalse;}
	//�Լ�����new��delete
	//A* a = (A*)malloc(sizeof(A));
	//new (a) A();	//����������
	//
	//a->~A();
	//free(a);
	namespace XMem
	{//�ڴ�������ط���
		template<typename T>
		inline void XDELETE(T *&p)
		{
			if (p == NULL) return;
			delete p;
			p = NULL;
		}
		template<typename T>
		inline void XDELETE_ARRAY(T *&p)
		{
			if (p == NULL) return;
			delete[] p;
			p = NULL;
		}
		template<typename T>
		inline T * createMem()
		{
			try
			{
				return new T;
			}
			catch (...)
			{
				return NULL;
			}
		}
		template<typename T>
		inline T * createArrayMem(int size)
		{
			if (size <= 0) return NULL;
			try
			{
				return new T[size];
			}
			catch (...)
			{
				return NULL;
			}
		}

		//��addBuff�����ݽӵ�baseBuff֮������ڴ�ռ䲻��������㹻���ڴ�ռ�
		extern unsigned char *spliceData(unsigned char * baseBuff, int &baseLen, int &offset,
			const void * addBuff, int len);
		//��p���ڴ��С�ĳ�size��������usage������
		template<typename T>
		bool resizeMem(T *&p, int usage, int size)
		{
			if (size <= 0 || usage > size) return false;	//��Ч������
			T * tmp = XMem::createArrayMem<T>(size);
			if (tmp == NULL) return false;	//�ڴ����ʧ�ܣ�ʲôҲ���ı�
			if (usage > 0)
				memcpy(tmp, p, usage * sizeof(T));
			XMem::XDELETE_ARRAY(p);
			p = tmp;
			return true;
		}
		extern void randomMem(void *mem, int size);	//����һ���ڴ��е�����
	}
	//���ڴ洢�����ݽṹ
	class XBuffer
	{
	private:
		unsigned char* m_pData;	//�洢�ռ��ָ��
		int m_buffSize;			//�洢�Ĵ�С
		int m_usage;			//�ռ��ʹ����
		//�������Ҫ��ֹ�������캯���͸��Ʋ�����
		XBuffer(const XBuffer &temp);
		XBuffer& operator = (const XBuffer& temp);
	public:
		XBuffer()
			:m_pData(NULL)
			, m_buffSize(0)
			, m_usage(0)
		{}
		~XBuffer() { release(); }
		int getBuffSize()const { return m_buffSize; }
		int getUsage()const { return m_usage; }
		//���������Ҫ�����ã�һ��Ҫ����ȷ������ͼʱ�ŵ��ã���������ڲ����ݴ���
		void setUsage(int u)
		{
			if (u > m_buffSize) m_usage = m_buffSize;
			else m_usage = u;
		}
		//�����������ָ������
		bool pushData(const void *data, int len);	//��������������
		bool pushAData(unsigned char data);
		//ֻ�����С���������ݿ���
		bool resize(int size);
		//����ԭ������
		bool resizeEx(int size);
		void release()
		{
			XMem::XDELETE_ARRAY(m_pData);
			m_buffSize = 0;
			m_usage = 0;
		}
		//ֻ�����С���������ݿ���
		void fitSize(int size)	//��������������Ѿ�ʹ�õ��ڴ�ռ䣬������ֱ�Ӷ���
		{
			if (size <= 0 || m_buffSize >= size) return;	//�����Ȼ���㹻���ڴ�ռ�����Ҫ�������
			resize(size);
		}
		//����ԭ������
		void fitSizeEx(int size)	//��������������Ѿ�ʹ�õ��ڴ�ռ䣬������ֱ�Ӷ���
		{
			if (size <= 0 || m_buffSize >= size) return;	//�����Ȼ���㹻���ڴ�ռ�����Ҫ�������
			resizeEx(size);
		}
		//�Ӷ�����ȡ��ָ������
		bool popData(unsigned char *data, int len)
		{
			if (data == NULL || len <= 0) return false;
			if (len > m_usage) return false;
			memcpy(data, m_pData, len);
			return decreaseData(len);
		}
		//��������һ�ֵ��÷�ʽ
		const unsigned char *getBuffer()const { return m_pData; }
		unsigned char *getBuffer() { return m_pData; }	//����һ�ֱȽ�Σ�յ�ʹ�÷����������ʹ������ӿ�
		bool decreaseData(int len);
		void clear() { m_usage = 0; }
	};
}
#endif