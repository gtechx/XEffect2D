#ifndef _JIA_XCIRCLEGROUP_
#define _JIA_XCIRCLEGROUP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
/****************************************
˵����
������ݽṹʹ�ù̶����ڴ��С
�������ڴ�ռ�
�Ƚ��ȳ�������˳��
�������������ۻ��������
���̰߳�ȫ
�Ż���
����������ַ����������������Ƶĸ��Ӷ���������Ҫ���ؿ��ǣ�
1��һ����������Դ��һ���������Դ�����ַ�ʽ������ʹ�û������������̰߳�ȫ
2�������������Դ������������Դ���������Ҫʹ�û��������ܱ�֤���̰߳�ȫ
���Խ����
1��һ����������Դ��һ���������Դ�����������ݣ����ַ�ʽ��ʹ�û������ǰ�ȫ��
2�������������Դ������������Դ���������Ҫʹ�û��������ܱ�֤���̰߳�ȫ
****************************************/
#include "XBasicClass.h"
#include "XOSDefine.h"
//#include "XThread.h"
#include "XCritical.h"
#ifdef XEE_OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#endif
namespace XE{
template<class Type> class XCircleGroup
{
private:
    XBool m_isInited;    //�Ƿ��ʼ��
    int m_insertPoint;    //��ǰ�Ĳ����
    int m_popPoint;        //��ǰȡ����
    int m_buffSize;        //����buff�Ĵ�С
    
    struct XElement    //Ԫ�صĽṹ
    {
        XBool isEnable;
        Type elementData;
    };
    XElement *m_pElement;    //Ԫ�ض��е�ָ��
	pthread_mutex_t m_mutex;  
    int m_mode;            //ӵ�´���ģʽ��0��ӵ���������ϵ����� 1��ӵ�µȴ�
private:
	XCircleGroup(const XCircleGroup & temp){}	//���ؿ������캯����ֹ����Ҫ�Ĵ���
public:
    XCircleGroup()        //���캯��
    :m_isInited(XFalse)
    ,m_insertPoint(0)    //��ǰ�Ĳ����
    ,m_popPoint(0)        //��ǰȡ����
    ,m_buffSize(0)        //����buff�Ĵ�С
    ,m_pElement(NULL)    //Ԫ�ض��е�ָ��
    ,m_mode(0)
    {}
	XBool init(int buffsize,int mode = 0);    //��ʼ�����������ڴ�ռ�
	int insertOneElement(Type &element);    //����һ��Ԫ��
	XBool popOneElement(Type &element);    //ȡ��һ��Ԫ��
    XBool isEmpty() const   //�Ƿ�Ϊ��
    {
        if(!m_isInited) return XTrue;
        return !m_pElement[m_popPoint].isEnable;    //û�п���ȡ����������
    }
	XBool isFull() const   //�Ƿ�Ϊ��
	{
		if(!m_isInited) return XFalse;
		return m_pElement[m_insertPoint].isEnable;    //û�п��Բ���Ŀռ���
	}
};
}
#endif

/********************************************
//for test

#include "stdafx.h"
//#include "windows.h"
#include "stdlib.h"
#include "math.h"
#include "XCircleGroup.h"

struct _data
{
    int data1[1024];
    _data& operator=(_data d) 
    {
        if ( &d == this ) return *this;
        for(int i = 0;i < 1024;++ i)
        {
            data1[i] = d.data1[i];
            Sleep(1);
        }
        return *this;
    }
};
XCircleGroup <_data> tempBuff;

DWORD WINAPI thread_d(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(true)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_e(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(true)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_f(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(true)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}

//���Խ��������Ҫ���̺߳͸����߳��ж����������ܱ�֤���������ķ�Χ����ת��
int main(int avg,char* arg)
{
    HANDLE temp;
    DWORD dw;
    tempBuff.init(100,1);
    temp = CreateThread(NULL,NULL,thread_d,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_e,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_f,NULL,0,&dw);
    _data data;
    int oldData = 0;
    while(true)
    {
        if(tempBuff.popOneElement(&data) == 1)
        {
            for(int i = 1;i < 1024;++ i)
            {
                if(data.data1[0] != data.data1[i])
                {
                    printf("error!\n");
                }
            }
            oldData = data.data1[0];
        }
        Sleep(1);
    }
    return 1;
}

*********************************************/
