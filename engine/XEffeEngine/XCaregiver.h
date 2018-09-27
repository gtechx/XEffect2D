#ifndef _JIA_XCAREGIVER_
#define _JIA_XCAREGIVER_
#include "XBabyBase.h"
#include <vector>
//��һ���������ö��࣬��ΪbabyӦ���������Լ��
//��������Ҫ��Ϊ�˽����������豸��֮�������ⲿԭ�����һ��ʱ���豸����֮��
//���������´��豸�������Ӳ��Ա�������Ӧ����ʾ
#include "XTimer.h"
namespace XE{
struct XBabyInfo
{
	XBabyBase *pBaby;
	XSimpleTimer timer;
};
class XCaregiver
{
private:
	std::vector<XBabyInfo> m_babys;	//���Ա�����������б�
public:
	bool addABaby(XBabyBase *baby, int resetTimer = 1000)
	{
		if (getBabyIndex(baby) >= 0) return false;
		XBabyInfo tmp;
		tmp.pBaby = baby;
		tmp.timer.setStepTime(resetTimer);
		m_babys.push_back(tmp);
		return true;
	}
	int getBabyIndex(XBabyBase *baby)	//��ȡָ��baby�ı�ţ�����������򷵻�-1
	{
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby == baby) return i;
		}
		return -1;
	}
	void releaseAllBaby() { m_babys.clear(); }		//�ͷ����е��չ˶���
	bool freeABaby(XBabyBase *baby)	//�ͷ�һ��baby�Ĺ���
	{
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby == baby)
			{
				m_babys.erase(m_babys.begin() + i);
				return true;
			}
		}
		return false;
	}
	void update(float stepTime)		//��������baby��״̬
	{//������ݱ��չ˶����״̬���չ�baby
		if (m_babys.size() <= 0) return;
		for (int i = 0; i < m_babys.size(); ++i)
		{
			if (m_babys[i].pBaby->needReset() && m_babys[i].timer.update(stepTime))
			{
				m_babys[i].pBaby->reset();
				m_babys[i].timer.reset();
			}
		}
	}
};
}
#endif