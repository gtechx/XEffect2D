#ifndef _JIA_XLIGHT_
#define _JIA_XLIGHT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//����һ��3D�����еĵƹ�
#include "XBasic3D.h"
namespace XE{
class XLight
{
private:
	XVec4 m_ambient;	//�Ƶ���ɫ
	XVec4 m_diffuse;	//ɢ�����ɫ��
	XVec4 m_position;	//�ƹ��λ��
	XVec4 m_specular;	//Ŀǰ�в�֪������Ǹ�ɶ��
	int m_index;			//�ƹ�ı�� [1 - n]

	//��Ƶķ���
	XVec3 m_lookAtPosition;		//���Ŀ����λ��
	XVec3 m_direction;			//��Ƶķ���
	XMat4 m_projectMatrix;	//��Ƶ��������
	XMat4 m_viewMatrix;		//��ƵĹ۲����
	XVec3 m_angle;
	float m_len;
	XBool m_needCalculate;			//�Ƿ���Ҫ����
public:
	XMat4 getProjectMatrix() {return m_projectMatrix;}
	XMat4 getViewMatrix() {return m_viewMatrix;}
	XLight();
	void calculate();
	void setLength(float len) 
	{
		m_len = len;
		m_needCalculate = XTrue;
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needCalculate = XTrue;
	}
	void setIndex(int index) {m_index = index;}
	void setPosition(float x,float y,float z,float w)
	{
		m_position.set(x,y,z,w);
		//m_needCalculate = XTrue;
	}
	void setPosition(const XVec4& p) {m_position = p;}
	XVec3 getPosition() const {return XVec3(m_position.x,m_position.y,m_position.z);}
	void setAmblient(float x,float y,float z,float w)
	{
		m_ambient.set(x,y,z,w);
	}
	void setAmblient(const XVec4& a) {m_ambient = a;}
	void setDiffuse(float x,float y,float z,float w)
	{
		m_diffuse.set(x,y,z,w);
	}
	void setDiffuse(const XVec4& d) {m_diffuse = d;}
	void setSpecular(float x,float y,float z,float w)
	{
		m_specular.set(x,y,z,w);
	}
	void setSpecular(const XVec4& s) {m_specular = s;}
	void useLight();	//ʹ�õƹ�
	void disLight();
	static void disAllLight()
	{
		glDisable(GL_LIGHTING);
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SINGLE_COLOR_EXT);
	}
};
}
#endif //_JIA_XLIGHT_
