#include "XStdHead.h"
#include "XMoveNumber.h"
namespace XE{
XBool XMoveNumber::move(float delay)
{
	float tempSum;
	if((int)(m_armNumber) != m_curNumber)
	{
		tempSum = m_armNumber - m_curNumber;
		if(0.0075f * delay < 1.0f)	//�仯�ı������ܳ���1������ͻ�Ŵ���
		{
			tempSum *= 0.0075f * delay;
		}
		if(tempSum < 1.0f && tempSum > -1.0f)	//��С�ı仯ֵ����̫С
		{
			if(tempSum < 0) tempSum = -1.0f;
			else tempSum = 1.0f;
		}
		m_curNumber += tempSum;
		return XTrue;
	}
	return XFalse;
}
}