#ifndef _JIA_XMEDIALIST_
#define _JIA_XMEDIALIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.8.4
//--------------------------------
#include <vector>
namespace XE{
//���ǹ��ڲ����б��һЩ����

//�����б��ѭ��ģʽ
enum XMediaListPlayerMode
{
	PLAY_MODE_SERIAL,	//˳�򲥷�
	PLAY_MODE_RAND,		//�������
};

//�����б�ĵ�Ԫ���
//ͷͣ��ʱ�� ��λms
//ѭ������ -1����ѭ�� 0��ѭ�� 1ѭ��һ�� nѭ��n��
//ѭ��ͷͣ��ʱ�� ��λms
//���������ļ���·��
//ѭ��βͣ��ʱ�� ��λms
//βͣ��ʱ�� ��λms

struct  XMediaListItem
{
	int m_headDelayTime;	//ms
	int m_loopTimes;		//ѭ������
	int m_loopHeadDelayTime;	//ms
	std::string m_videoFilename;
	int m_loopEndDelayTime;	//ms
	int m_endDelayTime;		//����ͣ�ٵ�ʱ��
	bool m_stopAtEnd;		//�Ƿ��ڽ������֮��ͣ��
	XMediaListItem()
		:m_headDelayTime(0)
		,m_loopTimes(0)
		,m_loopHeadDelayTime(0)
		,m_videoFilename("")
		,m_loopEndDelayTime(0)
		,m_endDelayTime(0)
		,m_stopAtEnd(false)
	{}
};
#define DEFAULT_PLAY_LIST_FILENAME "playList.xml"
class XMediaList
{
private:
	std::vector<XMediaListItem> m_playList;
	XMediaListPlayerMode m_playMode;	//��δʵ��
	int m_loopTimes;					//��δʵ��
public:
	bool loadFromFile(const char * filename = NULL);	//���ļ��ж�ȡ�б�
	bool saveToFile(const char * filename = NULL);		//���б�д�뵽�ļ���
	bool checkList() { return true; }		//����б������Ƿ���ȷ
	void addAItem2List(const XMediaListItem & c) { m_playList.push_back(c); }
	XMediaListPlayerMode getPlayMode()const { return m_playMode; }
	int getLoopTimes()const { return m_loopTimes; }
	unsigned int getItemSum() { return m_playList.size(); }
	const XMediaListItem * getItem(int index)
	{
		if (index < 0 || index >= m_playList.size()) return NULL;
		return &m_playList[index];
	}

	XMediaList()
		:m_loopTimes(0)
		, m_playMode(PLAY_MODE_SERIAL)
	{}
};
}
#endif