#ifndef _JIA_XSMARTPOINTER_
#define _JIA_XSMARTPOINTER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.15
//--------------------------------
#include "stdio.h"
//#include "XBasicFun.h"
namespace XE{
//����������
template<class T> class XSmartP;

template<class T> class XBackP  
{  
private:
    friend XSmartP<T>;  
    T *m_p;			//ʵ�ʵ�ָ��
    size_t m_counter;			//���ü�����
    XBackP(T *p) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("XBackP constructor called !\n");  
    }  
public:
    ~XBackP()  
    {  
        XMem::XDELETE(m_p);  
	//	printf( "XBackP distructor called !\n");  
    }  
};    
template<class T> class XSmartP  
{  
public:  
    // ���캯����p��ָ���Ѿ���̬������T����ָ��  
    XSmartP(T *p) 
		:m_backP(new XBackP<T>(p))
    {  
	//	printf("XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    XSmartP() 
		:m_backP(new XBackP<T>(new T()))
    {  
	//	printf("XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // ���ƹ��캯�������Ƴ�Ա����ʹ�ü�����1  
    XSmartP(const XSmartP& temp) 
		:m_backP(temp.m_backP)
    {  
		++m_backP->m_counter;    
	//	printf("XSmartP copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ��ֵ������  
    XSmartP<T>& operator=(const XSmartP<T>&temp)
	{  
		if(this == &temp) return *this;		//��ֹ��������
		// �����Ҳ������е�ʹ�ü���  
		++temp.m_backP->m_counter;  
		// ��������������ʹ�ü�����1�����ö����ʹ�ü�������0����ɾ���ö���  
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // ����m_backPָ��  
		return *this;  
	}   
    // �����������������Ϊ0����ɾ��m_backP����  
    ~XSmartP()  
    {  
	//	printf("XSmartP distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
    }  
    // ��ȡ���ݳ�Ա  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// �޸����ݳ�Ա  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // ���ػ��޸Ķ���  
    T getVal() const  
    {  
        return *m_backP->m_p;  
    }  
    void setVal(T val)  
    {  
        *m_backP->m_p = val;  
    }  
private:  
    XBackP<T> *m_backP;   //ָ��ʹ�ü�����m_backP 
};  

//�����ǹ�������Ķ���
template<class T> class XSmartPArray;

template<class T> class XBackPArray  
{  
private:
    friend XSmartPArray<T>;  
    T *m_p;			//ʵ�ʵ�ָ��
    size_t m_counter;			//���ü�����
    XBackPArray(T p[]) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("XBackPArray constructor called !\n");  
    }  
public:
    ~XBackPArray()  
    {  
        XMem::XDELETE_ARRAY(m_p);  
	//	printf( "XBackPArray distructor called !\n");  
    }  
};  
  
template<class T> class XSmartPArray  
{  
public:  
	XSmartPArray()
		:m_backP(new XBackPArray<T>(new T[2]))
		,m_size(2)
	{
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
	}
    // ���캯����p��ָ���Ѿ���̬������int����ָ��  
    XSmartPArray(T p[],int size) 
		:m_backP(new XBackPArray<T>(p))
		,m_size(size)
    {  
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    XSmartPArray(int size) 
		:m_backP(new XBackPArray<T>(new T[size]))
		,m_size(size)
    {  
	//	printf("XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ���ƹ��캯�������Ƴ�Ա����ʹ�ü�����1  
    XSmartPArray(const XSmartPArray& temp) 
		:m_backP(temp.m_backP)
		,m_size(temp.m_size)
    {  
		++m_backP->m_counter;    
	//	printf("XSmartPArray copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ��ֵ������  
    XSmartPArray<T>& operator=(const XSmartPArray<T>&temp)
	{  
		if(this == &temp) return *this;		//��ֹ��������
		// �����Ҳ������е�ʹ�ü���  
		++temp.m_backP->m_counter;  
		// ��������������ʹ�ü�����1�����ö����ʹ�ü�������0����ɾ���ö���  
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // ����m_backPָ��  
		m_size = temp.m_size;
		return *this;  
	}   
 	T& operator[](int order)const
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
 	T& operator[](int order)
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
    // �����������������Ϊ0����ɾ��m_backP����  
    ~XSmartPArray()  
    {  
	//	printf("XSmartPArray distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XMem::XDELETE(m_backP);
		}
    }  
    // ��ȡ���ݳ�Ա  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// �޸����ݳ�Ա  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // ���ػ��޸Ķ���  
    T getVal(int order) const  
    {  
		if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];  
    }  
    void setVal(T val,int order)  
    {  
		if(order < 0 || order >= m_size) order = 0;
        m_backP->m_p[order] = val;  
    }  
private:  
    XBackPArray<T> *m_backP;   //ָ��ʹ�ü�����m_backP 
	int m_size;
}; 
}
#endif