#ifndef _JIA_XSIMPLETRACKER_
#define _JIA_XSIMPLETRACKER_
#include "XMath\XVector2.h"
namespace XE {
//��׷�ٵ��������Ϣ
enum XTObjState
{
	STATE_NULL,	//��Ч��״̬
	STATE_BORN,	//���ڳ���
	STATE_LIVE,	//���ڴ��
	STATE_DIE,	//��������
};
struct XTObjInfo
{
	int typeID;			//�����ID
	int id;				//�Ǳ�������ID
	XVec2 pos;			//�����λ��
	XVec2 speed;		//������ٶ�
	float life;			//���������
	float bornTimer;	//������ʱ
	float dieTimer;		//������ʱ
	float upSetTime;	//�ϴθ������ݵ�ʱ���
	XTObjState state;	//�����״̬
	XTObjState upState;	//��һ�ε�״̬
	//�����Ǽ�����м����
	float steptime;		//�������ݸ���֮���ʱ���
	XVec2 estimatedPos;	//Ԥ����λ��
	bool isTracked;		//�Ƿ��������ټ���
	float maxDis;		//�����ƶ���������
	XTObjInfo()
		:id(-1)
		, life(0.0f)
		, bornTimer(0.0f)
		, upSetTime(0.0f)
		, state(STATE_NULL)
	{}
};
class XSimpleTracker
{
private:
	int m_curObjID;
	float m_minBornTime;	//����ʱ�����ֵ(��)
	float m_minDieTime;		//����ʱ�����ֵ(��)
	float m_maxSpeed;		//�����˶�������ٶ�(��)
	std::vector<XTObjInfo> m_curObjs;	//��ǰ��������Ϣ
	std::vector<XTObjInfo> m_oldObjs;	//ԭ����������Ϣ
	void bornAObj(int typeID, XVec2 pos);
public:
	int getCurObjID()const { return m_curObjID; }
	//��ҪԤ�Ƚ������״̬����ΪSTATE_NULL
	void tracker(std::vector<XTObjInfo>& objs);
	//��ҪԤ�Ƚ������״̬����ΪSTATE_NULL
	void trackerByID(std::vector<XTObjInfo>& objs);
	void update(float steptime);
	const std::vector<XTObjInfo>& getCurObjs()const { return m_curObjs; }

	//�����ǲ����Ĳ���
	float getMinBornTime()const { return m_minBornTime; }
	float getMinDieTime()const { return m_minDieTime; }
	float getMaxSpeed()const { return m_maxSpeed; }
	void setMinBornTime(float s) { m_minBornTime = s; }
	void setMinDieTime(float s) { m_minDieTime = s; }
	void setMaxSpeed(float s) { m_maxSpeed = s; }
	XSimpleTracker()
		:m_minBornTime(0.25f)
		, m_minDieTime(0.25f)
		, m_maxSpeed(512.0f)
		, m_curObjID(0)
	{}
};
}
#endif