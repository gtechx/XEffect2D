#ifndef _JIA_XSAMPLING_
#define _JIA_XSAMPLING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//�����˲���һ�����У����ݶ����г��ֵ�������ͳ����ֵ�����������������г����Ա������ݵ��ȶ���
#include <deque>
#include <vector>
namespace XE{
class XSamplingFilter
{
private:
	int m_size;	//�������е����
	int m_threshold;	//���ٵ�ʹ�ܱ�׼
	std::deque<int> m_ids;	//����ʶ���ID�Ķ���
	std::vector<int> m_sampleIDs;	//Ŀǰӵ�е�ID
	std::vector<int> m_idSum;	//ID��ͳ������
	int m_curID;	//��ǰ��ID��Ĭ��-1Ϊ��Ч��ID
	bool m_isIgnoreInvalidID;	//�Ƿ������Ч��ID�����Ϊtrue������Ч��ID���������
	void updateID();	//����ID
	void popAIDn();
public:
	XSamplingFilter()
		:m_curID(-1)
		,m_size(10)
		,m_threshold(5)
	{}
	void clear()	//��յ�ǰ��ͳ������
	{
		m_ids.clear();
		m_sampleIDs.clear();
		m_idSum.clear();
	}
	void popAID()
	{
		popAIDn();
		updateID();
	}
	const std::deque<int> &getIds()const{return m_ids;}
	void pushAID(int id);	//����һ����������
	int getCurID()const{return m_curID;}	//��ȡ��ǰ��ͳ��ID
	bool init(int size,int threshold)
	{
		if(size <= 0) return false;
		if(threshold < 1) return false;
		setSize(size);
		setThreshold(threshold);
		return true;
	}
	void setSize(int size);
	void setThreshold(int threshold)
	{
		m_threshold = threshold;
		updateID();
	}
};
}
#endif