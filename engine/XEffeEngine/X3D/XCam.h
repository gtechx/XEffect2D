#ifndef _JIA_XCAM_
#define _JIA_XCAM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//#include "XBasicFun.h"
//#include "XBasicClass.h"
#include "glu.h"
#include "../XMath/XVector3.h"
#include "../XMath/XMatrix4x4.h"
#define WITH_LEFT_MULT	//����OpenCV�ı궨���ʱ��Ҫȡ�������
namespace XE{
//fovΪ�������ֱ������ӽǿ��ǻ�����ˮƽ�ʹ�ֱ�����н�С��һ��������ȷ�ϡ�
class XBasicCam
{
public:
	virtual void useCam() = 0;
	virtual ~XBasicCam() {}
};
class XCam :public XBasicCam	//��ͨ�����
{
private:
	XVec3 m_position;			//�������λ��
	XVec3 m_lookAtPosition;		//�����Ŀ����λ��
	XVec3 m_direction;			//������ķ���
	//XVec3 m_angle;				//������ĽǶ�
public:
	void init(const XVec3& pos, const XVec3& lookAt, const XVec3& direction)
	{
		m_position = pos;
		m_lookAtPosition = lookAt;
		m_direction = direction;
	}
	void setPosition(float x, float y, float z) { m_position.set(x, y, z); }
	void setPosition(const XVec3& p) { m_position = p; }
	void setLookAt(float x, float y, float z) { m_lookAtPosition.set(x, y, z); }
	void setLookAt(const XVec3& l) { m_lookAtPosition = l; }
	void setDirection(float x, float y, float z) { m_direction.set(x, y, z); }
	void setDirection(const XVec3& d) { m_direction = d; }
	void useCam()	//ʹ�������Ч
	{
		gluLookAt(m_position.x, m_position.y, m_position.z,
			m_lookAtPosition.x, m_lookAtPosition.y, m_lookAtPosition.z,
			m_direction.x, m_direction.y, m_direction.z);
	}
	XCam()
		:m_position(0.0f, 0.0f, -100.0f)
		, m_lookAtPosition(0.0f, 0.0f, 0.0f)
		, m_direction(0.0f, 1.0f, 0.0f)
	{}
};
//�������ε������
class XCamRoam :public XBasicCam
{
private:
	XVec3 m_position;			//�������λ��
	float m_len;				//��������Ӿ�
	//XVec3 m_angle;			//������ĽǶ�[0 - 360]
	XMat3 m_rotateMat;			//��ת����

	XVec3 m_lookAtPosition;		//�����Ŀ����λ��
	XVec3 m_direction;			//������ķ���
	XMat4 m_projectMatrix;	//��������������
	XMat4 m_viewMatrix;		//������Ĺ۲����
	XMat4 m_projXView;		//m_projectMatrix * m_viewMatrix

	XBool m_needCalculate;			//�Ƿ���Ҫ����
	XBool m_neadCalFrustum;		//�Ƿ���Ҫ������׶
	XBool m_neadCalProjXView;		//�Ƿ���Ҫ����pXv
	float m_near;					//������
	float m_far;					//Զ����
	float m_fov;					//����
	bool m_isSetFov;
public:
	void setNear(float n)
	{
		m_near = n;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getNear()const { return m_near; }
	void setFar(float f)
	{
		m_far = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getFar()const { return m_far; }
	void setFov(float f)
	{
		m_fov = f;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		m_isSetFov = false;
	}
	float getFov()const { return m_fov; }
	void setCamState(const XVec3& pos, const XVec3& lookAt, const XVec3& dir);
public:
	XMat4 getProjXView()
	{
		calculate();
		if (m_neadCalProjXView)
		{
			m_projXView = m_projectMatrix * m_viewMatrix;
			m_neadCalProjXView = XFalse;
		}
		return m_projXView;
	}
	XMat4 getProjectMatrix() { return m_projectMatrix; }
	XMat4 getViewMatrix() { return m_viewMatrix; }
	XBool calculate();
	void init(const XVec3& pos, float len, const XVec3& angle)
	{
		if (!m_isSetFov)
			m_fov = XEG.getViewAngle();
		m_position = pos;
		m_len = len;
		m_rotateMat = XMath::getRotate3D(angle);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
		calculate();
	}
	void forward(float len)
	{//��ǰ�ƶ������
#ifdef WITH_LEFT_MULT
		m_position += XMath::transform3x3(XVec3(0.0f, 0.0f, len), m_rotateMat);
#else
		m_position += m_rotateMat * XVec3(0.0f, 0.0f, len);
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void backward(float len)
	{//����ƶ������
#ifdef WITH_LEFT_MULT
		m_position -= XMath::transform3x3(XVec3(0.0f, 0.0f, len), m_rotateMat);
#else
		m_position -= m_rotateMat * XVec3(0.0f, 0.0f, len);
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnLeft(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(0.0f, angle, 0.0f));
#else
		m_rotateMat = XMath::getRotate3D(XVec3(0.0f, angle, 0.0f)) * m_rotateMat;
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnRight(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(0.0f, -angle, 0.0f));
#else
		m_rotateMat = XMath::getRotate3D(XVec3(0.0f, -angle, 0.0f)) * m_rotateMat;
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnUp(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = XMath::getRotate3D(XVec3(-angle, 0.0f, 0.0f)) * m_rotateMat;
#else
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(-angle, 0.0f, 0.0f));
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void turnDown(float angle)
	{
#ifdef WITH_LEFT_MULT
		m_rotateMat = XMath::getRotate3D(XVec3(angle, 0.0f, 0.0f)) * m_rotateMat;
#else
		m_rotateMat = m_rotateMat * XMath::getRotate3D(XVec3(angle, 0.0f, 0.0f));
#endif
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	XVec3 getPosition() const { return m_position; }
	XVec3 getLookAtPos() const { return m_lookAtPosition; }
	XVec3 getDirection() const { return m_direction; }
	void setPosition(float x, float y, float z)
	{
		m_position.set(x, y, z);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	XVec3 worldToScreen(const XVec3& pos, const XRect& view);
	void setPosition(const XVec3& pos)
	{
		m_position = pos;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	float getLength() const { return m_len; }
	void setLength(float len)
	{
		m_len = len;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 360]
	void setAngle(const XVec3& angle)
	{
		m_rotateMat = XMath::getRotate3D(angle);
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 360]
	void setAngle(float x, float y, float z)
	{
		m_rotateMat = XMath::getRotate3D(XVec3(x, y, z));
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	//[0 - PI2]
	XVec3 getAngle() const { return XMath::getEulerian(m_rotateMat); }
	XMat3 getRotateMat()const { return m_rotateMat; }
	void setRotateMat(const XMat3& mat3)
	{
		m_rotateMat = mat3;
		m_needCalculate = XTrue;
		m_neadCalFrustum = XTrue;
	}
	void useCam()	//ʹ�������Ч
	{
		calculate();
		gluLookAt(m_position.x, m_position.y, m_position.z,
			m_lookAtPosition.x, m_lookAtPosition.y, m_lookAtPosition.z,
			m_direction.x, m_direction.y, m_direction.z);
	}
	XCamRoam()
		:m_position(0.0f, 0.0f, -100.0f)
		, m_len(100.0f)
		, m_near(10.0f)
		, m_far(10000.0f)
		, m_fov(45.0f)
		, m_isSetFov(false)
	{}
	//��������δʵ�ֵĽӿ�
private:
	XVec4 m_face[6];	//��׶���6������
	XVec3 m_point[9];	//��׶���8������,���һ����Ϊ��������ڵ�λ��
public:
	//��Ҫ���ӵĽӿڣ������������ԣ�
	void calculateFrustumPlanes(); //������׶��
	bool isInFrustum(float x, float y, float z);  //�жϵ��Ƿ�����׶���ڲ� 
	void calculateFrustumPoint();
	void drawFrustum();	//�����׶��
};
}
#endif //_JIA_XCAM_
