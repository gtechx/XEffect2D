#ifndef _JIA_XMESH2D_
#define _JIA_XMESH2D_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------

#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include <vector>
//�����Ƕ�2Dmesh�ķ�װ
//��ʱ�Ȳ�ʹ��VBO�����Ż���֮���ٿ����ⲽ�Ż�

//1����Ϸ�ʽ����
//2��֧��shader
namespace XE{
class XMesh2D
{
public:
	std::vector<XVector2> m_v;		//����������
	std::vector<XVector2> m_t;		//��ͼ������
	std::vector<XFColor> m_c;		//��ɫ������
private:
	bool m_isInited;
	bool m_withTexture;
	bool m_withColor;

	XVector2 m_position;	//��ǰ��λ��
	XVector2 m_scale;		//���ŵı���
	XVector2 m_center;		//��ת���ŵ����λ�ã������й�һ��
	XFColor m_color;
	float m_angle;			//��ת�Ƕ�
	int m_sumX;
	int m_sumY;
public:
	int getWidthSum()const{return m_sumX;}
	int getHeightSum()const{return m_sumY;}
	void setPosition(float x,float y){m_position.set(x,y);}
	void setPosition(const XVector2 &p){m_position = p;}
	void setScale(float x,float y){m_scale.set(x,y);}
	void setColor(float r,float g,float b,float a){m_color.setColor(r,g,b,a);}
	void setAngle(float a){m_angle = a;}	//0 - 360,��λ�Ƕ�
	//w:mesh�����ؿ��
	//h:mesh�����ظ߶�
	//sumX:mesh��ˮƽ�ܶ�
	//sumY:mesh�Ĵ�ֱ�ܶ�
	//withT:�Ƿ������ͼ����
	//withC:�Ƿ������ɫ����
	bool init(int w,int h,int sumX,int sumY,bool withT = false,bool withC = false);
	bool init(const char *filename)
	{
		if(m_isInited) return false;
		if(!loadFromFile(filename)) return false;
		m_isInited = true;
		return true;
	}

	bool saveToFile(const char *filename);		//�����ݱ��浽�ļ�������ʹ��OBJ�����ݸ�ʽ
	bool loadFromFile(const char *filename);	//���ļ��ж�ȡ����

	void drawWireframe();	//����߿�
	void drawTexture(unsigned int tex);

	XMesh2D()
		:m_isInited(false)
		,m_position(0.0f,0.0f)
		,m_scale(1.0f,1.0f)
		,m_center(64.0f,64.0f)
//		,m_center(0.0f,0.0f)
		,m_color(1.0f,1.0f,1.0f,1.0f)
		,m_angle(0.0f)
	{}
	//������Ϊ�˱��������ק���ṩ�Ľӿ�
	int getNearestVertex(const XVector2 &pos,float maxD = 10.0f);	//��ȡ��pos����Ķ���ı�ţ�����С��maxD
	void setVertexPos(const XVector2 &pos,int index);	//���ö����λ��
	void clearAllData()
	{
		m_v.clear();
		m_t.clear();
		m_c.clear();
	}
};
}
#endif