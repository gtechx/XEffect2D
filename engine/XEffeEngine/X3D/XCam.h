#ifndef _JIA_XCAM_
#define _JIA_XCAM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#include "XBasicFun.h"
#include "XBasicClass.h"
#include "glu.h"
//fovΪ�������ֱ������ӽǿ��ǻ�����ˮƽ�ʹ�ֱ�����н�С��һ��������ȷ�ϡ�

class _XBasicCam
{
public:
	virtual void useCam() = 0;
	virtual ~_XBasicCam() {}
};

class _XCam:public _XBasicCam	//��ͨ�����
{
private:
	_XVector3 m_position;			//�������λ��
	_XVector3 m_lookAtPosition;		//�����Ŀ����λ��
	_XVector3 m_direction;			//������ķ���
	//_XVector3 m_angle;				//������ĽǶ�
public:
	void init(const _XVector3 &pos,const _XVector3 &lookAt,const _XVector3 &direction)
	{
		m_position = pos;
		m_lookAtPosition = lookAt;
		m_direction = direction;
	}
	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &p) {m_position = p;}
	void setLookAt(float x,float y,float z) {m_lookAtPosition.set(x,y,z);}
	void setLookAt(const _XVector3 &l) {m_lookAtPosition = l;}
	void setDirection(float x,float y,float z) {m_direction.set(x,y,z);}
	void setDirection(const _XVector3 &d) {m_direction = d;}
	void useCam()	//ʹ�������Ч
	{
		gluLookAt(m_position.x,m_position.y,m_position.z,
			m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
			m_direction.x,m_direction.y,m_direction.z);
	}
	_XCam()
		:m_position(0.0f,0.0f,-100.0f)
		,m_lookAtPosition(0.0f,0.0f,0.0f)
		,m_direction(0.0f,1.0f,0.0f)
	{}
};
//�������ε������
class _XCamRoam:public _XBasicCam
{
private:
	_XVector3 m_position;			//�������λ��
	float m_len;					//��������Ӿ�
	_XVector3 m_angle;				//������ĽǶ�[0 - 360]

	_XVector3 m_lookAtPosition;		//�����Ŀ����λ��
	_XVector3 m_direction;			//������ķ���
	_XMatrix4x4 m_projectMatrix;	//��������������
	_XMatrix4x4 m_viewMatrix;		//������Ĺ۲����
	_XMatrix4x4 m_projXView;		//m_projectMatrix * m_viewMatrix

	_XBool m_needCalculate;			//�Ƿ���Ҫ����
	_XBool m_neadCalFrustum;		//�Ƿ���Ҫ������׶
	_XBool m_neadCalProjXView;		//�Ƿ���Ҫ����pXv
	float m_near;					//������
	float m_far;					//Զ����
	float m_fov;					//����
public:
	void setNear(float n)
	{
		m_near = n;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getNear()const {return m_near;}
	void setFar(float f)
	{
		m_far = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFar()const {return m_far;}
	void setFov(float f)
	{
		m_fov = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFov()const {return m_fov;}
public:
	_XMatrix4x4 getProjXView()
	{
		calculate();
		if(m_neadCalProjXView)
		{
			m_projXView = m_projectMatrix * m_viewMatrix;
			m_neadCalProjXView = XFalse;
		}
		return m_projXView;
	}
	_XMatrix4x4 getProjectMatrix() {return m_projectMatrix;}
	_XMatrix4x4 getViewMatrix() {return m_viewMatrix;}
	_XBool calculate();
	void init(const _XVector3 &pos,float len,_XVector3 &angle)
	{
		m_position = pos;
		m_len = len;
		m_angle = angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		calculate();
	}
	void forward(float len)
	{//��ǰ�ƶ������
		m_position.z += len * cos(m_angle.x * DEGREE2RADIAN) * cos(m_angle.y * DEGREE2RADIAN);
		m_position.y += len * sin(m_angle.x * DEGREE2RADIAN);
		m_position.x += len * cos(m_angle.x * DEGREE2RADIAN) * sin(m_angle.y * DEGREE2RADIAN);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void backward(float len)
	{//����ƶ������
		m_position.z -= len * cos(m_angle.x * DEGREE2RADIAN) * cos(m_angle.y * DEGREE2RADIAN);
		m_position.y -= len * sin(m_angle.x * DEGREE2RADIAN);
		m_position.x -= len * cos(m_angle.x * DEGREE2RADIAN) * sin(m_angle.y * DEGREE2RADIAN);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnLeft(float angle)
	{
		m_angle.y += angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnRight(float angle)
	{
		m_angle.y -= angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnUp(float angle)
	{
		m_angle.x += angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		if(m_angle.x > 90.0f) m_angle.x = 90.0f;
	}
	void turnDown(float angle)
	{
		m_angle.x -= angle;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		if(m_angle.x < -90.0f) m_angle.x = -90.0f;
	}
	_XVector3 getPosition() const {return m_position;}
	_XVector3 getLookAtPos() const {return m_lookAtPosition;}
	void setPosition(float x,float y,float z)
	{
		m_position.set(x,y,z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void setPosition(const _XVector3 &pos)
	{
		m_position = pos;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getLength() const {return m_len;}
	void setLength(float len)
	{
		m_len = len;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	_XVector3 getAngle() const {return m_angle;}
	void useCam()	//ʹ�������Ч
	{
		calculate();
		gluLookAt(m_position.x,m_position.y,m_position.z,
			m_lookAtPosition.x,m_lookAtPosition.y,m_lookAtPosition.z,
			m_direction.x,m_direction.y,m_direction.z);
	}
	_XCamRoam()
		:m_position(0.0f,0.0f,-100.0f)
		,m_len(100.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_near(10.0f)		
		,m_far(10000.0f)				
		,m_fov(45.0f)	
	{}
//��������δʵ�ֵĽӿ�
private:
	_XVector4 m_face[6];	//��׶���6������
	_XVector3 m_point[9];	//��׶���8������,���һ����Ϊ��������ڵ�λ��
public:
	//��Ҫ���ӵĽӿڣ������������ԣ�
	void calculateFrustumPlanes(); //������׶��
	bool isInFrustum(float x,float y,float z);  //�жϵ��Ƿ�����׶���ڲ� 
	void calculateFrustumPoint();
	void drawFrustum();	//�����׶��
};
#endif //_JIA_XCAM_