#ifndef _JIA_XBASIC3D_
#define _JIA_XBASIC3D_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//������һЩ������3D������ԭ����
//#include "XBasicOpenGL.h"
//#include "XBasicClass.h"
//#include "XCam.h"
#include "../XMath/XVector3.h"
#include "../XMath/XMatrix4x4.h"
//#include "../XMath/XFColor.h"
namespace XE {
enum X3DDrawMode
{
	DRAW_MODE_FILL,
	DRAW_MODE_LINE,
	DRAW_MODE_POINT,
};
//#define WITH_GLUT
namespace XRender
{
	extern void drawLine(const XVec3& ps, const XVec3& pe, const XFColor& color = XFColor::white, float w = 0.5f);							//��һ��ָ���߶�
	extern void drawLine(const XVec3& ps, const XVec3& n, float len, const XFColor& color = XFColor::white, float w = 0.5f);				//��һ��ָ���߶�
	extern void drawTriangle(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XFColor& color = XFColor::white);	//���һ��ָ����������
	extern void drawTriangle(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XVec3& n, const XFColor& color = XFColor::white);	//���һ��ָ����������
	//size:��ߴ�
	extern void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, const XFColor& color = XFColor::white);	//���һ������
	//����λ�ð�ߴ�
	extern void drawCuboid(const XVec3& center, const XVec3& size, const XVec3& angle, unsigned int tex);	//���һ������ͼ�ĺ���
#ifdef WITH_GLUT
//ע�⣺�������������ʹ��glut��ʵ�ֵģ�һ������£�������������ǲ����ŵģ���Ϊ���ŵĻ��������Ҫ����glut��dll����������������ּ
	extern void drawBall(const XVec3& center, float r, const XVec3& angle, const XFColor& color);
#endif
	//p��Ϊw*h�ĵ���
	//�������p���ɵ�ƽ��
	extern void drawArray(const float *p, int w, int h);
	extern void drawArray(const float *p, int w, int h, unsigned int tex);
	//λ����ԭ��
	//��浥λ�����壬�����Ϊ1
	extern void drawCuboidBase();
	//����ͼ�����������Ϊ1
	extern void drawCuboidWithTexBase();
	//��ʹ�þ������ĳ�����
	//size:��ߴ�
	extern void drawCuboidBase(const XVec3& size);
	inline void drawCuboidBase(GLfloat size) { drawCuboidBase(XVec3(size)); }
	//λ�ò���ԭ��
	//pos����λ�ã�size��ߴ磬��ʹ�þ�������
	extern void drawCuboidBase(const XVec3& center, const XVec3& size);
	extern void drawCuboid(const XVec3& center, const XVec3& size);
	//x���Ͻǣ�yz���ģ�ȫ�ߴ�
	extern void drawCuboidEx(const XVec3& pos,	//λ��
		const XVec3& size,		//������ĳߴ磬�����
		const XVec3& angle,		//������ĽǶ�
		const XFColor& color);	//��泤����
	extern void drawPlane(const XVec3& p0, const XVec3& p1, const XVec3& p2, const XVec3& p3,
		const XVec3& n, const XFColor& color = XFColor::white);
	extern void drawOrigin();	//���ԭ��
	extern void drawFloor(float distance = 2, float w = 0.5f);	//���0ƽ��(����)
}
namespace XMath
{
	//gluLookAt
	extern XMat4 calLookAtMatrix(const XVec3& eyePos, const XVec3& LookAtPos, const XVec3& up);
	//gluPerspective
	extern XMat4 calPerspectiveMatrix(float fovy, float rat, float zNear, float zFar);
	//glOrtho	//û�о�������
	inline XMat4 calOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return XMat4(2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, -2.0 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	//glOrtho2D //û�о�������
	inline XMat4 calOrtho2DMatrix(float left, float right, float bottom, float top) { return calOrthoMatrix(left, right, bottom, top, -1.0, 1.0); }
	//glFrustum
	inline XMat4 calFrustumMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		//�����΢��������㷨�������
		//return XMat4(	2.0*zNear/(right-left),	0.0f,					(right+left)/(right-left),		0.0f,
		//				0.0f,					2.0*zNear/(top-bottom), (top+bottom)/(top-bottom),		0.0f,
		//				0.0f,					0.0f,					-(zFar+zNear)/(zFar-zNear),		-2.0*zFar*zNear/(zFar-zNear),
		//				0.0f,					0.0f,					-1.0f,							0.0f);
		//������ʱ����ȷ���㷨
		return XMat4(2.0*zNear / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
			0.0f, 2.0*zNear / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
			0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -1.0f,
			0.0f, 0.0f, -2.0*zFar*zNear / (zFar - zNear), 0.0f);
	}

	extern bool getOrtho(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar);
	extern bool getFrustum(const XMat4& mtrx, float& left, float& right, float& bottom, float& top, float& zNear, float& zFar);
	extern bool getPerspective(const XMat4& mtrx, float& fovy, float& aspectRatio, float& zNear, float& zFar);
	extern void getLookAt(const XMat4& mtrx, XVec3& eye, XVec3& center, XVec3& up, float lookDistance = 1.0f);
	//��v������תangle����ת����
	extern XMat4 getRotate(float angle, XVec3 v);	// ��ĳ������ת
	//3D�ռ��о��������˳��ΪglTranslatef / glRotatef / glScalef
	inline XMat4 getRotateX(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosA, sinA, 0.0f,
			0.0f, -sinA, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotateY(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(cosA, 0.0f, -sinA, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinA, 0.0f, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotateZ(float angle)
	{
		float sinA = sin(angle * DEGREE2RADIAN);
		float cosA = cos(angle * DEGREE2RADIAN);
		return XMat4(cosA, sinA, 0.0f, 0.0f,
			-sinA, cosA, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	inline XMat4 getRotate(const XVec3& a)
	{
		XMat4 ret = getRotateX(a.x);
		ret = ret * getRotateY(a.y);
		ret = ret * getRotateZ(a.z);
		return ret;
	}
	inline XMat4 getTranslate(const XVec3& p)
	{
		return XMat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			p.x, p.y, p.z, 1.0f);
	}
	inline XMat4 getScale(const XVec3& s)
	{
		return XMat4(s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	//���ݷ���������������ת�Ƕȣ���׼��(1,0,0);Ҳ���ǽ�v(1,0,0)ת��d�ķ�������Ҫ�ĽǶ�
	inline XVec3 getAngleFromDirection(const XVec3&  d)
	{
		float z = atan2(d.y, _hypot(d.x, d.z));
		float y = -atan2(d.z, d.x);
		return XVec3(0.0f, y, z);
	}
	//near:���ص㣬far:Զ�ص㣬v:������Ķ������ݣ�n:������ķ������ݣ�out:����,���ؽ�����Զ���ص��еĹ�һ�����룬< 0 Ϊû�н���
	extern float canPickingByRay(const XVec3& nearP, const XVec3& farP, XVec3 *v, const XVec3& n, XVec3& out);
	extern XVec3 worldToScreen(const XVec3& pos, const XMat4& modelview, const XMat4 &projection,
		const XRect& view);
	//gluUnProject()
	extern bool getUnProject(float x, float y, float z, const XMat4& modelview, const XMat4 &projection,
		const XVec4& viewport, XVec3& out);
	//������Ļ���ֱ꣬�ӷ���Զ����������
	extern bool getUnProject(const XVec2& pos, XVec3& nearP, XVec3& farP);
}
class XBasic3DObject
{
protected:
	XVec3 m_position;
	XVec3 m_scale;
	XVec3 m_angle;
	XFColor m_color;
	bool m_needUpdateMatrix;
	XMat4 m_matrix;
	XMat4 m_multMatrix;

	XVec3 m_areaPoint[8];	//��ײ���ӵ�8���㣬���ڲü�����
public:
	virtual XVec3 getAreaPoint(int index) const
	{
		if (index < 0 || index >= 8) return XVec3::zero;
		return m_areaPoint[index];
	}
	virtual XMat4 getMatrix()const { return m_matrix; }
	virtual void setPosition(const XVec3& pos)
	{
		m_position = pos;
		m_needUpdateMatrix = true;
	}
	virtual void setPosition(float x, float y, float z)
	{
		m_position.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getPosition()const { return m_position; }
	virtual void setScale(const XVec3& scale)
	{
		m_scale = scale;
		m_needUpdateMatrix = true;
	}
	virtual void setScale(float x, float y, float z)
	{
		m_scale.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual void setScale(float s)
	{
		m_scale.set(s, s, s);
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getScale()const { return m_scale; }
	virtual void setAngle(const XVec3& angle)
	{
		m_angle = angle;
		m_needUpdateMatrix = true;
	}
	virtual void setAngle(float x, float y, float z)
	{
		m_angle.set(x, y, z);
		m_needUpdateMatrix = true;
	}
	virtual void setMultRotate(const XMat4 & mtrx)
	{
		m_multMatrix = mtrx;
		m_needUpdateMatrix = true;
	}
	virtual XVec3 getAngle()const { return m_angle; }
	virtual void setColor(const XFColor& color) { m_color = color; }
	virtual void setColor(float r, float g, float b, float a) { m_color.set(r, g, b, a); }
	virtual XFColor getColor()const { return m_color; }
	virtual void updateMatrix()
	{
		if (!m_needUpdateMatrix) return;
		m_matrix = XMath::getTranslate(m_position);
		m_matrix = m_matrix * XMath::getRotate(m_angle);
		m_matrix = m_matrix * m_multMatrix;
		m_matrix = m_matrix * XMath::getScale(m_scale);
		m_needUpdateMatrix = false;
	}

	XBasic3DObject()
		:m_position(0.0f, 0.0f, 0.0f)
		, m_scale(1.0f, 1.0f, 1.0f)
		, m_angle(0.0f, 0.0f, 0.0f)
		, m_color(1.0f)
		, m_needUpdateMatrix(false)
	{
		m_matrix.loadIdentity();
		m_multMatrix.loadIdentity();
	}
	virtual ~XBasic3DObject() {}
};
}
#endif