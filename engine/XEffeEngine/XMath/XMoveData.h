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
	MOVE_DATA_MODE_NULL,		//��Ч������
	MOVE_DATA_MODE_LINE,		//���Ա仯
	MOVE_DATA_MODE_SIN,			//sin���߱仯,	�ȿ����		��������:��
	MOVE_DATA_MODE_SIN_MULT,	//sin���߱仯,	�ȿ����		��������:С
	MOVE_DATA_MODE_SQRT2,		//4�ο���,		�ȿ����		��������:��
	MOVE_DATA_MODE_SQRT2_MULT,	//ʹ�ó˷�ʵ��,	�ȿ����		��������:��
	MOVE_DATA_MODE_COS,			//cos���߱仯,	�������		��������:��
	MOVE_DATA_MODE_COS_MULT,	//cos���߱仯,	�������		��������:С
	MOVE_DATA_MODE_DSIN,		//sin���߱仯,	�ȿ�����ٿ�	��������:��	
	MOVE_DATA_MODE_DCOS,		//cos���߱仯,	�����������	��������:��
	MOVE_DATA_MODE_DSIN_MULT,	//sin���߱仯,	�ȿ�����ٿ�	��������:С	
	MOVE_DATA_MODE_DCOS_MULT,	//cos���߱仯,	�����������	��������:С
	MOVE_DATA_MODE_SHAKE,		//˥�������ߣ��ȿ����(��δʵ��)
	MOVE_DATA_MODE_MAX,
};

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
public:
	float getStartData() const {return m_startData;}
	float getEndData() const {return m_endData;}
	float getSpeed() const {return m_speed;}
	int getLoopTimer() const {return m_isLoop;}
	XMoveDataMode getMoveMode() const {return m_mode;}
public:
	//isLoop:0,��ѭ��,>0ѭ���Ĵ���,<0����ѭ��
	void set(float start,float end,float speed,XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0);
	//timeΪ�����ĳ���ʱ��
	void setEx(float start,float end,float time,XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0)
	{
		set(start,end,1.0f / time * 0.001f,mode,isLoop);
	}
	void reset();	//�������¿�ʼ
	void move(float delay);
	float getValue(float time);	//����timeʱ�̵���ֵ:��λms
	float getCurData() const {return m_curData;}
	float getCurTimer() const {return m_timer;}
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd();
	XMoveData()
		:m_startData(0.0f)
		,m_endData(0.0f)
		,m_curData(0.0f)
		,m_isEnd(XTrue)
		,m_mode(MOVE_DATA_MODE_SIN)
		,m_isLoop(0)
	{}
	void getParamStr(char * str);
};
#if WITH_INLINE_FILE
#include "XMoveData.inl"
#endif
}
#endif