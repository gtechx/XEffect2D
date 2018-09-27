#ifndef _JIA_XTEMPSAVE_
#define _JIA_XTEMPSAVE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "..\XCommonDefine.h"
#include <deque>
namespace XE{
//��������ڽ����ʱ����Ĺ���
class XStepSave
{
private:
	bool m_isInited;
	int m_coreSize;		//һ���洢�ռ�Ĵ�С
	int m_coreSum;		//�洢�ռ������
	unsigned char **m_cores;	//�洢�ռ�
	std::deque<unsigned char *> m_freeBuff;	//δʹ�õĿռ�
	std::deque<unsigned char *> m_usedBuff;	//ʹ�õĿռ�
	//std::deque<unsigned char *>::iterator m_curIndex;	//������deque��������������ܲ��Ǻܺ�(��Ҫ�Ż�)
	int m_curIndex;	//������deque��������������ܲ��Ǻܺ�(��Ҫ�Ż�)
public:
	XStepSave()
		:m_isInited(false)
		,m_cores(NULL)
		,m_coreSum(0)
	{}
	~XStepSave(){release();}
	bool init(int size,int sum);	//size:һ��core�Ĵ�С��sum��core������
	bool pushACore(const void *p);	//�����������һ��״̬
	unsigned char *getPrevCore() const;		//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
	void moveToPrev()
	{
		if(m_curIndex > 0) 
			--m_curIndex;
		//forTest();
	}
	unsigned char *getNextCore() const;		//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
	void moveToNext()
	{
		if(m_curIndex >= 0 && m_curIndex + 1 < m_usedBuff.size()) 
			++ m_curIndex;
		//forTest();
	}
	unsigned char *getCurCore() const;
	void release();
	void forTest();
	void reset() 
	{ 
		m_curIndex = -1;
		for (auto it = m_usedBuff.begin(); it != m_usedBuff.end(); ++it)
		{
			m_freeBuff.push_back(*it);
		}
		m_usedBuff.clear();
	}
	bool haveAction()const { return m_curIndex >= 0; }
};
#if WITH_INLINE_FILE
#include "XStepSave.inl"
#endif
}
#endif