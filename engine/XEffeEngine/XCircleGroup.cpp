#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XCircleGroup.h"
namespace XE{
template<class type>
XBool XCircleGroup<type>::init(int buffsize,int mode)    //��ʼ�����������ڴ�ռ�
{
	if(m_isInited ||    //����Ѿ���ʼ���������ظ���ʼ��
		buffsize <= 0) return XFalse;        //0�ߴ��ǲ��е�
	m_insertPoint = 0;                //��ǰ�Ĳ����
	m_popPoint = 0;                    //��ǰȡ����
	m_buffSize = buffsize;            //����buff�Ĵ�С
	m_pElement = NULL;                //Ԫ�ض��е�ָ��
	m_mode = mode;
	m_pElement = XMem::createArrayMem<XElement>(m_buffSize);
	if(m_pElement == NULL) return XFalse;    //�ڴ����ʧ��
	//��ʼ����������
	for(int i = 0;i < m_buffSize;++ i)
	{
		m_pElement[i].isEnable = XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
template<class type>
int XCircleGroup<type>::insertOneElement(type &element)    //����һ��Ԫ��
{
	if(!m_isInited) return 0;
	pthread_mutex_lock(m_mutex);
	if(!m_pElement[m_insertPoint].isEnable)
	{//�����ݿ���ȡ������ȡ�����ݣ�������Ӧ�Ĵ���
		m_pElement[m_insertPoint].elementData = element;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
		m_pElement[m_insertPoint].isEnable = XTrue;
		++ m_insertPoint;
		if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
		pthread_mutex_unlock(m_mutex);
		return 1;
	}else
	{
//        printf("buff is full!");
		if(m_mode == 0)
		{
//            printf(" some data will be demp!\n");
			//��buff�������ݶ���
			m_pElement[m_insertPoint].elementData = element;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
			m_pElement[m_insertPoint].isEnable = XTrue;
			++ m_insertPoint;
			if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
			++ m_popPoint;
			if(m_popPoint >= m_buffSize) m_popPoint = 0;
		}
//        printf("\n");
		pthread_mutex_unlock(m_mutex);
		return 2;
	}
}
template<class type>
XBool XCircleGroup<type>::popOneElement(type &element)    //ȡ��һ��Ԫ��
{
	if(!m_isInited) return XFalse;
	pthread_mutex_lock(m_mutex);
	if(m_pElement[m_popPoint].isEnable != 0)
	{//�����ݿ���ȡ������ȡ�����ݣ�������Ӧ�Ĵ���
		element = m_pElement[m_popPoint].elementData;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
		m_pElement[m_popPoint].isEnable = XFalse;
		++ m_popPoint;
		if(m_popPoint >= m_buffSize) m_popPoint = 0;
		pthread_mutex_unlock(m_mutex);
		return XTrue;
	}
	pthread_mutex_unlock(m_mutex);
	return XFalse;
}
}