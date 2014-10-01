#ifndef _JIA_XARCBALL_
#define _JIA_XARCBALL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "XBasicClass.h"

class _XArcBall
{
protected:
	bool m_isInited;	//�Ƿ��ʼ��
	_XVector3 m_clickVector;
	_XVector3 m_dragVector;
	float m_sizeWRate;	//�ߴ��ת����
	float m_sizeHRate;

	void mapToSphere(const _XVector2 &point,_XVector3& vec) const;	//��2D����ӳ���3D����
public:
	_XArcBall(float width,float height)
		:m_isInited(false)
	{
		setSize(width,height);
	}
	_XArcBall()
		:m_isInited(false)
	{}
	~_XArcBall() {};
	bool setSize(float w,float h)
	{
		if(w <= 1.0f || h <= 1.0f) return false;	//���ݲ��Ϸ�
		m_sizeWRate = 2.0f / (w - 1.0f);
		m_sizeHRate = 2.0f / (h - 1.0f);
		m_isInited = true;
		return true;
	}
	void click(const _XVector2 &point){mapToSphere(point,m_clickVector);}	//��갴�¼�¼����״̬
	void drag(const _XVector2 &point,_XVector4& rotate); //���������϶�������ת����Ԫ��
};
#endif