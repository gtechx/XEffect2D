#ifndef _JIA_XDATAGUESS_
#define _JIA_XDATAGUESS_
#include <deque>
#include "XMath\XVector2.h"
//#define WITH_DEBUG_INFO //�Ƿ���ʾ������Ϣ
namespace XE{
//�������ڶ��������(x,y)�����ݽ��з����Ա��ڶ�δ������x�������Ӧ��yʹ����������ƽ��
enum XGuessModle
{
	GUESS_MODLE_LINE,		//���Բ²�
	GUESS_MODLE_SECOND,		//�����ٶȵĲ²�
	GUESS_MODLE_LINE_FIT,	//���Բ²�������ݽ�������	//��������Ϊ����
	GUESS_MODLE_SECOND_FIT,	//�����ٶȵĲ²�
	//��������ڡ���е���۾�����Ŀ����ӵĹ���
	GUESS_MODLE_PERIODIC,	//�������ظ������ݣ����ģʽ����ʹ���ظ�������Խ���ã�������׼ȷ
};
struct XCycleInfo
{
	float m_maxValue;	//���ڵ����ֵ
	int m_maxValueSum;	
	float m_minValue;	//���ڵ���Сֵ
	int m_minValueSum;
	float m_upTime;		//���ڵ�����ʱ��
	int m_upTimeSum;
	float m_downTime;	//���ڵ��½�ʱ��
	int m_downTimeSum;
	float m_cycleTime;	//ƽ������	��δʵ��
	float m_upDownRate;	//�����ȣ������׶����½��׶���ռʱ��ı��� ��δ�Զ�����
	XCycleInfo()
		:m_minValue(0.0f)
		, m_minValueSum(0)
		, m_maxValue(0.0f)
		, m_maxValueSum(0)
		, m_upTime(0.0f)
		, m_upTimeSum(0)
		, m_downTime(0.0f)
		, m_downTimeSum(0)
		, m_cycleTime(0.0f)
		, m_upDownRate(0.71f)
	{}
	bool getIsEnable()const{return m_maxValueSum > 0;}
	void addAMaxValue(float maxV);
	void addAMinValue(float minV);
	void addAUpTime(float upTime);
	void addADownTime(float upTime);
	void reset();
	float getAmplitude()const{return m_maxValue - m_minValue;}
};
enum XCycleState
{
	CYC_STA_NULL,	//δ֪��״̬
	CYC_STA_UP,		//�����׶�
	CYC_STA_DOWN,	//�½��׶�
};
class XCycleData
{
private:
	bool m_haveStart;	//�Ƿ��ҵ��������
	bool m_haveEnd;		//�Ƿ��ҵ������յ�
	bool m_haveTop;		//�Ƿ��ҵ����ڶ���
	float m_startTime;	//��ʼ��ʱ��
	float m_topTime;	//�����ʱ��
	float m_endTime;	//������ʱ��
	XCycleState m_state;
	float m_maxValue;	//�����ڵ����ֵ
	float m_minValue;	//�����ڵ���Сֵ
	float m_upTime;		//������ʱ��
	float m_downTime;	//�½���ʱ��

	//����������������������㷨Ŀǰ�������ص�����
	bool calStart(const XCycleInfo &info);	//���㿪ͷ
	bool calEnd();		//�������
	bool calTop();

	int m_upDatasSum;	//�������ݵĴ���
	int m_downDatasSum;	//�½����ݵĴ���
	float m_startValue;	//��ʼ����ֵ
	float m_endValue;	//��������ֵ
	//Ĭ��Ϊ1����ֵԽ����Եֿ������������
	int m_turnThreshold;	//�жϽ��������ǿ�ʼ������״̬��ֵ
	int m_startEndIndex;	//��ʼλ�ý����ı�ţ�����Ϊֹ��Ҫ�����λ��֮��ʼ�ж�
	int m_startIndex;
	int m_topIndex;
	int m_endIndex;
public:
	XCycleData()
		:m_haveStart(false)
		, m_haveEnd(false)
		, m_haveTop(false)
		, m_startTime(0.0f)
		, m_endTime(0.0f)
		, m_state(CYC_STA_NULL)
		, m_upDatasSum(0)
		, m_downDatasSum(0)
		, m_turnThreshold(1)
	{}
	void setTurnThreshold(int t)
	{
		if(t >= 1) m_turnThreshold = t;
	}
	std::deque<XVec2> m_datas;
	void pushAData(const XE::XVec2& data,const XCycleInfo &info);
	bool getIsComplete()const{return m_haveEnd;} 
	bool getHaveHead()const{return m_haveStart;}
	float getMinValue()const{return m_minValue;}
	float getMaxValue()const{return m_maxValue;}
	float getUpTime()const{return m_upTime;}
	float getDownTime()const{return m_downTime;}
	int getUpDatasSum()const{return m_upDatasSum;}
	int getDownDatasSum()const{return m_downDatasSum;}
	float getStartValue()const{return m_startValue;}
	float getEndValue()const{return m_endValue;}
	void clear();
	//xΪ�����������ʼ��ʱ��
	bool getValue(float x,float &y)const;
	float getStartTime()const{return m_startTime;}	//��ȡ������ʼ��ʱ��
};
class XDataGuess
{
private:
	std::deque<XVec2> m_datas;
	XGuessModle m_mode;
	//���������ԵĲ²⣬��������ڡ���е���۾�����Ŀ����ӵĹ���
	XCycleData m_cycDatas;	//���е�����
	XCycleData m_cycDatasTmp;
	std::deque<XCycleData> m_oldCycData;
	XDataGuess *m_pDG;
	bool m_isInited;
	XCycleInfo m_cycleInfo;
	bool m_isCycDataUpdate;
	//������һ���ǳ����Ի��ĺ���������ͨ���ص��ķ�ʽ���ⲿʵ�����߼�
	//bSimple //�Ƿ�Ϊ��ʼ�ؼ�������
	bool getIsInvalid(const XCycleData &data,bool bSimple = false)const;	//���������Ƿ���Ч 
public:
	XDataGuess()
		:m_mode(GUESS_MODLE_LINE)
		, m_pDG(NULL)
		, m_isInited(false)
	{}
	~XDataGuess(){release();}
	//������������ڳ�ʼ����ʱ�����
	bool init(XGuessModle mode);
	bool getIsCycDataUpdated();
	const std::deque<XCycleData> &getOldCycDatas()const{return m_oldCycData;}
	//��Ҫע����������ݱ���Ҫ����x�ĺ�˳���
	void pushAData(const XVec2& data);
	float guessData(float x)const;
	bool getCanGuess()const;	//�����Ƿ���Խ��й���
	void release();
	void setTurnThreshold(int t)
	{
		m_cycDatas.setTurnThreshold(t);
		m_cycDatasTmp.setTurnThreshold(t);
	}
};
#if WITH_INLINE_FILE
#include "XDataGuess.inl"
#endif
}
#endif