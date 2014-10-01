#ifndef _JIA_XBASIC3D_
#define _JIA_XBASIC3D_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//������һЩ������3D������ԭ����
#include "XBasicClass.h"
#include "XCam.h"
#include "XBasicOpenGl.h"

enum _X3DDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_LINE,
	DRAW_MODE_POINT,
};

extern void drawLine(const _XVector3 &ps,const _XVector3 &pe,const _XFColor & color = _XFColor::white);							//��һ��ָ���߶�
extern void drawLine(const _XVector3 &ps,const _XVector3 &n,float len,const _XFColor & color = _XFColor::white);				//��һ��ָ���߶�
extern void drawTriangle(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,const _XFColor & color = _XFColor::white);	//���һ��ָ����������
extern void drawTriangle(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,const _XVector3 &n,const _XFColor & color = _XFColor::white);	//���һ��ָ����������
extern void drawBox(const _XVector3 &center,const _XVector3 &size,const _XVector3 &angle,const _XFColor & color = _XFColor::white);	//���һ������
extern void drawBox(const _XVector3 &center,const _XVector3 &size,const _XVector3 &angle,unsigned int tex);	//���һ������ͼ�ĺ���

extern void drawBall(const _XVector3 &center,float r,const _XVector3 &angle,const _XFColor & color);
extern void drawArray(const float *p,int w,int h);
extern void drawArray(const float *p,int w,int h,unsigned int tex);
extern void drawBox(GLfloat size);
extern void drawPlane(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2,const _XVector3 &p3,
					  const _XVector3 &n,const _XFColor & color = _XFColor::white);
extern void drawCuboid(const _XVector3 &pos,	//λ��
	const _XVector3 &size,	//������ĳߴ磬�����
	const _XVector3 &angle,//������ĽǶ�
	const _XFColor &color);	//��泤����
//gluLookAt
extern _XMatrix4x4 calLookAtMatrix(const _XVector3 &eyePos,const _XVector3 &LookAtPos,const _XVector3 &up);
//gluPerspective
extern _XMatrix4x4 calPerspectiveMatrix(float fovy,float rat,float zNear,float zFar);
//glOrtho
extern _XMatrix4x4 calOrthoMatrix(float left,float right,float bottom,float top,float zNear,float zFar);
//glOrtho2D
extern _XMatrix4x4 calOrtho2DMatrix(float left,float right,float bottom,float top);
//glFrustum
extern _XMatrix4x4 calFrustumMatrix(float left,float right,float bottom,float top,float zNear,float zFar);

extern bool getOrtho(const _XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar);
extern bool getFrustum(const _XMatrix4x4& mtrx,float& left,float& right,float& bottom,float& top,float& zNear,float& zFar);
extern bool getPerspective(const _XMatrix4x4& mtrx,float& fovy,float& aspectRatio,float& zNear, float& zFar);
extern void getLookAt(const _XMatrix4x4& mtrx,_XVector3& eye,_XVector3& center,_XVector3& up,float lookDistance = 1.0f);
//��v������תangle����ת����
extern _XMatrix4x4 getRotate(float angle,_XVector3 v);	// ��ĳ������ת
extern _XMatrix4x4 getRotate(const _XVector3 &a);
extern _XMatrix4x4 getRotateX(float angle);	
extern _XMatrix4x4 getRotateY(float angle);	
extern _XMatrix4x4 getRotateZ(float angle);	
extern _XMatrix4x4 getTranslate(const _XVector3 &p);
extern _XMatrix4x4 getScale(const _XVector3 &s);
extern void drawOrigin();
//���ݷ���������������ת�Ƕȣ���׼��(1,0,0);Ҳ���ǽ�v(1,0,0)ת��d�ķ�������Ҫ�ĽǶ�
inline _XVector3 getAngleFromDirection(const _XVector3 & d)
{
	float z = atan2(d.y,sqrt(d.x * d.x + d.z * d.z));
	float y = -atan2(d.z,d.x);
	return _XVector3(0.0f,y,z);
}

class _XBasic3DObject
{
protected:
	_XVector3 m_position;
	_XVector3 m_size;
	_XVector3 m_angle;
	_XFColor m_color;
	bool m_needUpdateMatrix;
	_XMatrix4x4 m_matrix;
	_XMatrix4x4 m_multMatrix;

	_XVector3 m_areaPoint[8];	//��ײ���ӵ�8���㣬���ڲü�����
public:
	virtual _XVector3 getAreaPoint(int index) const
	{
		if(index < 0 || index >= 8) return _XVector3::zero;
		return m_areaPoint[index];
	}
	virtual _XMatrix4x4 getMatrix()const{return m_matrix;}
	virtual void setPosition(const _XVector3 &pos) 
	{
		m_position = pos;
		m_needUpdateMatrix = true;
	}
	virtual void setPosition(float x,float y,float z)
	{
		m_position.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual _XVector3 getPosition()const{return m_position;}
	virtual void setSize(const _XVector3 &size) 
	{
		m_size = size;
		m_needUpdateMatrix = true;
	}
	virtual void setSize(float x,float y,float z)
	{
		m_size.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual _XVector3 getSize()const{return m_size;}
	virtual void setAngle(const _XVector3 &angle) 
	{
		m_angle = angle;
		m_needUpdateMatrix = true;
	}
	virtual void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needUpdateMatrix = true;
	}
	virtual void setMultRotate(const _XMatrix4x4 & mtrx) 
	{
		m_multMatrix = mtrx;
		m_needUpdateMatrix = true;
	}
	virtual _XVector3 getAngle()const{return m_angle;}
	virtual void setColor(const _XFColor &color) {m_color = color;}
	virtual void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	virtual _XFColor getColor()const{return m_color;}
	virtual void updateMatrix()
	{
		if(!m_needUpdateMatrix) return;
		m_matrix = getTranslate(m_position);
		m_matrix = m_matrix * getRotate(m_angle);
		m_matrix = m_matrix * m_multMatrix;
		m_matrix = m_matrix * getScale(m_size);
		m_needUpdateMatrix = false;
	}

	_XBasic3DObject()
		:m_position(0.0f,0.0f,0.0f)
		,m_size(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_color(1.0f,1.0f,1.0f,1.0f)
		,m_needUpdateMatrix(false)
	{
		m_matrix.loadIdentity();
		m_multMatrix.loadIdentity();
	}
};
//near:���ص㣬far:Զ�ص㣬v:������Ķ������ݣ�n:������ķ������ݣ�out:����,���ؽ�����Զ���ص��еĹ�һ�����룬< 0 Ϊû�н���
extern float canPickingByRay(const _XVector3 &nearP,const _XVector3 &farP,_XVector3 *v,const _XVector3 &n,_XVector3 &out);
//gluUnProject()
extern int getUnProject(float x,float y,float z,const _XMatrix4x4& modelview,const _XMatrix4x4 &projection,
				  const _XVector4 &viewport,_XVector3 &out);
//������Ļ���ֱ꣬�ӷ���Զ����������
extern int getUnProject(const _XVector2 &pos,_XVector3 &nearP,_XVector3 &farP);
#endif