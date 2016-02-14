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
class XTempSave
{
private:
	bool m_isInited;
	int m_coreSize;		//һ���洢�ռ�Ĵ�С
	int m_coreSum;		//�洢�ռ������
	unsigned char **m_cores;	//�洢�ռ�
	std::deque<unsigned char *> m_freeBuff;	//δʹ�õĿռ�
	std::deque<unsigned char *> m_usedBuff;	//ʹ�õĿռ�
	int m_curIndex;
public:
	XTempSave()
		:m_isInited(false)
		,m_cores(NULL)
		,m_coreSum(0)
	{}
	~XTempSave(){release();}
	bool init(int size,int sum);	//size:һ��core�Ĵ�С��sum��core������
	bool pushACore(const unsigned char *p);	//�����������һ��״̬
	unsigned char *getPrevCore() const;		//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
	void moveToPrev(){if(m_curIndex > 0) --m_curIndex;/*forTest();*/}
	unsigned char *getNextCore() const;		//��ȡ��һ��״̬�����û����һ��״̬�򷵻�NULL
	void moveToNext(){if(m_curIndex >= 0 && m_curIndex + 1 < m_usedBuff.size()) ++ m_curIndex;/*forTest();*/}
	unsigned char *getCurCore() const;
	void release();
	void forTest();
};
#if WITH_INLINE_FILE
#include "XTempSave.inl"
#endif
}
#endif