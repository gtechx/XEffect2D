#ifndef _JIA_XMOVENUMBER_
#define _JIA_XMOVENUMBER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.3
//--------------------------------
#include "../XOSDefine.h"
namespace XE{
//������һ������ʵ�ֶ�̬���ֱ仯����,���ǿ�������һ�����ֹ���
class XMoveNumber
{
private:
	int m_curNumber;	//��ǰ����ֵ
	float m_armNumber;	//Ŀ����Ҫ�ﵽ����ֵ
public:
	void addArmNumber(float number){m_armNumber += number;}	//����Ŀ����ֵ������
	void setArmNumber(float number){m_armNumber = number;}	//����Ŀ����ֵ
	int getCurNumber(){return m_curNumber;}				//���ص�ǰ����ֵ
	int getArmNumber(){return (int)m_armNumber;}
	XBool move(float delay);			//���������Ƿ����仯0��û�б仯��1�������˱仯
	void reset()
	{
		m_curNumber = 0;
		m_armNumber = 0.0f;
	}
	XMoveNumber()
	:m_curNumber(0)
	,m_armNumber(0.0f)
	{}
};
}
#endif