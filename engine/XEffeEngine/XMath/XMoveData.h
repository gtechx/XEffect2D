#ifndef _JIA_XMOVEDATA_
#define _JIA_XMOVEDATA_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.3
//--------------------------------
#include "../XOSDefine.h"
//#include "stdio.h"
//#include "XMath.h"
namespace XE{
//������һ�����ݶ�̬�仯���࣬��������ʵ��һЩ��̬�������Լ���̬��Ч��
enum XMoveDataMode
{
	MD_MODE_NULL,		//��Ч������
	MD_MODE_LINE,		//���Ա仯
	MD_MODE_SIN,		//sin���߱仯,	�ȿ����		��������:��
	MD_MODE_SIN_MULT,	//sin���߱仯,	�ȿ����		��������:С
	MD_MODE_SQRT2,		//4�ο���,		�ȿ����		��������:��
	MD_MODE_SQRT2_MULT,	//ʹ�ó˷�ʵ��,	�ȿ����		��������:��
	MD_MODE_COS,		//cos���߱仯,	�������		��������:��
	MD_MODE_COS_MULT,	//cos���߱仯,	�������		��������:С
	MD_MODE_DSIN,		//sin���߱仯,	�ȿ�����ٿ�	��������:��	
	MD_MODE_DCOS,		//cos���߱仯,	�����������	��������:��
	MD_MODE_DSIN_MULT,	//sin���߱仯,	�ȿ�����ٿ�	��������:С	
	MD_MODE_DCOS_MULT,	//cos���߱仯,	�����������	��������:С
	MD_MODE_SHAKE,		//˥�������ߣ��ȿ����
	MD_MODE_SHAKE_EX,	//˥�������ߣ��ȿ����
	//����ķ�ʽ�ӱ�ֱ������
	MD_MODE_EIEO,
	MD_MODE_EI,
	MD_MODE_EO,
	MD_MODE_EL,
	MD_MODE_LL,
	MD_MODE_VLL,
	MD_MODE_TANH,
	MD_MODE_SINH,
	MD_MODE_SQUARE,

	MD_MODE_B5,
	MD_MODE_B3,
	MD_MODE_B2,
	MD_MODE_BAF1,
	MD_MODE_BAF2,
	MD_MODE_BAF3,
	MD_MODE_LS,
	MD_MODE_ES,
	MD_MODE_LEIEO,
	MD_MODE_VLEIEO,
	MD_MODE_QEI,
	MD_MODE_QEO,
	MD_MODE_EQEO,
	MD_MODE_CEI,
	MD_MODE_CEO,
	MD_MODE_QUEI,
	MD_MODE_QUEO,
	MD_MODE_BOUNCY,
	//���涼�ǽϸ��ӵ�
	MD_MODE_QBP,
	MD_MODE_ESP,
	MD_MODE_CBP,
	MD_MODE_CB2P,
	MD_MODE_SG,
	MD_MODE_OD,
	MD_MODE_EIB,
	MD_MODE_EOB,
	MD_MODE_EOIB,
	MD_MODE_EIOB,

	MD_MODE_EIBo,
	MD_MODE_EOBo,
	MD_MODE_EIOBo,
	MD_MODE_EOIBo,

	MD_MODE_EIE,
	MD_MODE_EOE,
	MD_MODE_EIOE,
	MD_MODE_EOIE,

	MD_MODE_BIC,	//�ο�ofxAnimatable
	MD_MODE_BOC,	//�ο�ofxAnimatable

	MD_MODE_SS,
	MD_MODE_SES,

	MD_MODE_MAX,
};
struct XMoveDataParam
{
	float p1;
	float p2;
	float p3;
	float p4;
	XMoveDataParam()
		:p1(0.05f)
		, p2(0.0f)
		, p3(0.0f)
		, p4(0.0f)
	{}
};
#define BOUNCES_MAX	20
class XMoveData
{
private:
	float m_startData;	//��ʼ��ֵ
	float m_endData;	//��ֹ��ֵ
	float m_curData;	//��ǰ����ֵ
	float m_speed;
	float m_timer;		//ʱ����
	int m_loopTimer;	//ѭ���Ĵ���
	
	XBool m_isEnd;	//�˶��Ƿ����1:���� 0:�˶���
	int m_isLoop;	//�Ƿ�ѭ��,�����Ǿ���ѭ��,�Լ�ѭ������
	XMoveDataMode m_mode;
	void initParam();	//��ʼ��������
	float bounceElast;
	float bounceDuration[BOUNCES_MAX];
	float bounceVelocity[BOUNCES_MAX];
	void initCustomBounce();
public:
	void setBounceElast(float tmp)
	{
		bounceElast = tmp;
		initCustomBounce();
	}
public:
	XMoveDataParam m_param;	//���ߵĲ������������ֱ�ӱ�¶�Ա����ⲿ�޸�
	float getValueEx(float percentage);	//��Χ0.0f - 1.0f
	float getStartData() const {return m_startData;}
	float getEndData() const {return m_endData;}
	float getSpeed() const {return m_speed;}
	int getLoopTimer() const {return m_isLoop;}
	XMoveDataMode getMoveMode() const {return m_mode;}
public:
	//isLoop:0,��ѭ��,>0ѭ���Ĵ���,<0����ѭ��
	void set(float start,float end,float speed,XMoveDataMode mode = MD_MODE_SIN_MULT,int isLoop = 0);
	//timeΪ�����ĳ���ʱ��
	void setEx(float start,float end,float time,XMoveDataMode mode = MD_MODE_SIN_MULT,int isLoop = 0)
	{
		set(start,end,1.0f / time * 0.001f,mode,isLoop);
	}
	void reset();	//�������¿�ʼ
	void move(float delay);
	float getValue(float time)	//����timeʱ�̵���ֵ:��λms
	{
		if(time < 0.0f) time = 0.0f;
		float curTime = time * m_speed;
		if(m_isLoop == 0)
		{//�����ѭ��
			if(curTime > 1.0f) curTime = 1.0f;
		}else
		{//ѭ��
			curTime = fmod(curTime,2.0f);
			if(curTime > 1.0f)
				curTime = 2.0f - curTime;
		}
		return getValueEx(curTime);
	}
	float getCurData() const {return m_curData;}
	float getCurTimer() const {return m_timer;}
	void setCurTimer(float t);
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd();
	XMoveData()
		:m_startData(0.0f)
		, m_endData(0.0f)
		, m_curData(0.0f)
		, m_isEnd(XTrue)
		, m_mode(MD_MODE_SIN)
		, m_isLoop(0)
	{}
	void getParamStr(char * str);
	std::string getModeStr();
};
#if WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}
#endif