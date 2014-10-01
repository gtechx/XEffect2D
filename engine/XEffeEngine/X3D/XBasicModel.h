#ifndef _JIA_XBASICMODEL_
#define _JIA_XBASICMODEL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "X3DWorld.h"
//����һ������ģ�͵��࣬���Ǽ򵥵Ķ�������õ��࣬����֮�����V N C T
class _XBasicModel
{
private:
	bool m_isInited;	//�Ƿ��Ѿ���ʼ��
	int m_vSum;	//��������
	float *m_v;	//��������
	float *m_c;	//�ɫ

	int m_iSum;	//��������
	unsigned int *m_i;	//��������
	_XVector3 m_position;	//λ��
	_XVector3 m_size;		//�ߴ�
	_XVector3 m_angle;		//�Ƕ�

	_XVBO m_vbo;
public:
	//iSumΪ������������������Ϊ��ͼԪ������ * ÿ��Ԫ����Ҫ�Ķ�����
	bool init(int vSum,float *v,float *c,int iSum,int *i)
	{
		if(m_isInited) return false;
		if(vSum <= 0 || iSum <= 0) return false;
		m_vSum = vSum;
		m_iSum = iSum;

		m_v = createArrayMem<float>(m_vSum * 3);
		memcpy(m_v,v,m_vSum * 3 * sizeof(float));	//ÿ����������������
		m_c = createArrayMem<float>(m_vSum * 4);
		memcpy(m_c,c,m_vSum * 4 * sizeof(float));	//ÿ����������������
		m_i = createArrayMem<unsigned int>(m_iSum * 3);
		memcpy(m_i,i,m_iSum * sizeof(int));

		m_vbo.init(m_vSum,m_v,NULL,NULL,m_c,m_iSum,m_i);

		m_isInited = true;
		return true;
	}
	void draw();
	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &pos) {m_position = pos;}
	void setSize(float x,float y,float z) {m_size.set(x,y,z);}
	void setSize(const _XVector3 &size) {m_size = size;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const _XVector3 &angle) {m_angle = angle;}

	void updateV(float *v)	//���¶�������
	{
		if(!m_isInited) return;
		memcpy(m_v,v,m_vSum * 3 * sizeof(float));
		m_vbo.updateDataV(m_vSum,m_v);
	}
	void updateC(float *c)	//���¶�������
	{
		if(!m_isInited) return;
		memcpy(m_c,c,m_vSum * 4 * sizeof(float));
		m_vbo.updateDataC(m_vSum,m_c);
	}
	void updateI(int *i)	//������������
	{
		if(!m_isInited) return;
		memcpy(m_i,i,m_iSum * sizeof(int));
		m_vbo.updateDataI(m_iSum,m_i);
	}
	void release()
	{
		if(!m_isInited) return;
		XDELETE_ARRAY(m_v);
		XDELETE_ARRAY(m_i);
		m_isInited = false;
	}
	_XBasicModel()
		:m_isInited(false)
		,m_vSum(0)
		,m_v(NULL)
		,m_iSum(0)
		,m_i(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_size(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
	{
	}
	~_XBasicModel() {release();}
};
#endif