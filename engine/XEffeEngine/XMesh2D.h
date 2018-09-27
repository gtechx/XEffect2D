#ifndef _JIA_XMESH2D_
#define _JIA_XMESH2D_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------

#include "XMath\XVector2.h"
#include "XMath\XFColor.h"
#include "XShaderGLSL.h"
#include <vector>
//�����Ƕ�2Dmesh�ķ�װ
//��ʱ�Ȳ�ʹ��VBO�����Ż���֮���ٿ����ⲽ�Ż�

//1����Ϸ�ʽ����
//2��֧��shader
namespace XE{
struct XMesh2DInfo
{
	int w;
	int h;
	int sumX;
	int sumY;
	bool withT;
	float tSx;	//��ͼ��x���
	float tSy;
	float tw;
	float th;
	bool withC;
	XMesh2DInfo()
		:w(0)
		, h(0)
		, sumX(0)
		, sumY(0)
		, withT(false)
		, withC(false)
		, tSx(0.0f)
		, tSy(0.0f)
		, tw(1.0f)
		, th(1.0f)
	{}
};
class XMesh2D
{
public:
	std::vector<XVec2> m_v;		//����������
	std::vector<XVec2> m_t;		//��ͼ������
	std::vector<XFColor> m_c;		//��ɫ������
private:
	bool m_isInited;
	bool m_withTexture;
	bool m_withColor;

	XVec2 m_position;	//��ǰ��λ��
	XVec2 m_scale;		//���ŵı���
	XVec2 m_center;		//��ת���ŵ����λ�ã������й�һ��
	XFColor m_color;
	float m_angle;			//��ת�Ƕ�
	int m_sumX;
	int m_sumY;
	//XTurnOverMode m_turnOverMode;	//��תģʽ
	XVec2 world2Local(const XVec2& pos)const;	//����ռ䵽����ռ��ת��
	XVec2 local2World(const XVec2& pos)const;	//����ռ䵽����ռ��ת��
public:
	bool getIsInited()const { return m_isInited; }
	int getWidthSum()const { return m_sumX; }
	int getHeightSum()const { return m_sumY; }
	//	void setPosition(float x, float y) { m_position.set(x, y); }
	void setPosition(const XVec2& p) { m_position = p; }
	void setScale(const XVec2& s) { m_scale = s; }
	//	void setColor(float r, float g, float b, float a) { m_color.set(r, g, b, a); }
	void setColor(const XFColor&color) { m_color = color; }
	void setAngle(float a) { m_angle = a; }	//0 - 360,��λ�Ƕ�
	//w:mesh�����ؿ��
	//h:mesh�����ظ߶�
	//sumX:mesh��ˮƽ�ܶ�
	//sumY:mesh�Ĵ�ֱ�ܶ�
	//withT:�Ƿ������ͼ����
	//withC:�Ƿ������ɫ����
	bool init(const XVec2& size, const XVec2& sum,
		bool withT = false, bool withC = false)
	{
		XMesh2DInfo info;
		info.w = size.x;
		info.h = size.y;
		info.sumX = sum.x;
		info.sumY = sum.y;
		info.withT = withT;
		info.withC = withC;
		return init(info);
	}
	bool init(int w, int h, int sumX, int sumY,
		bool withT = false, bool withC = false)
	{
		XMesh2DInfo info;
		info.w = w;
		info.h = h;
		info.sumX = sumX;
		info.sumY = sumY;
		info.withT = withT;
		info.withC = withC;
		return init(info);
	}
	bool init(const XMesh2DInfo &info);
	bool init(const char *filename)
	{
		if (m_isInited) return false;
		if (!loadFromFile(filename)) return false;
		m_isInited = true;
		return true;
	}
	bool initB(const char *filename)
	{
		if (m_isInited) return false;
		if (!loadFromFileB(filename)) return false;
		m_isInited = true;
		return true;
	}
	void setTextureInfo(float sx, float sy, float w, float h);

	//���ı��ķ�ʽ��������
	bool saveToFile(const char *filename)const;		//�����ݱ��浽�ļ�������ʹ��OBJ�����ݸ�ʽ
	bool loadFromFile(const char *filename);		//���ļ��ж�ȡ����
	//�Զ����Ƶķ�ʽ��������
	bool saveToFileB(const char *filename)const;		//�����ݱ��浽�ļ�������ʹ��OBJ�����ݸ�ʽ
	bool loadFromFileB(const char *filename);		//���ļ��ж�ȡ����

	void drawWireframe()const;	//����߿�
	void drawTexture(unsigned int tex, XShaderGLSL *pShader = NULL)const;
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ����������������һЩ�ӿ�
private:
	int m_specialIndex;	//�����ı��
	int m_specialLine;	//�������
	int m_specialRow;	//�������
public:
	void setSpecialIndex(int index) { m_specialIndex = index; }
	int getSpecialIndex()const { return m_specialIndex; }
	void setSpecialLine(int index) { m_specialLine = index; }
	int getSpecialLine()const { return m_specialLine; }
	void setSpecialRow(int index) { m_specialRow = index; }
	int getSpecialRow()const { return m_specialRow; }
	void drawWireframeEx(bool special = false, bool withPoint = true)const;	//����߿�,��������һЩ��Ϣ
	//����ǹ�һ���Ľ��
	bool mapVertex2Tex(const XVec2& pos, XVec2& out)const;	//ӳ�䶥�����굽��ͼ�ռ�
	bool mapTex2Vertex(const XVec2& pos, XVec2& out)const;	//ӳ�䶥�����굽��ͼ�ռ�
//-------------------------------------------------
	XMesh2D()
		:m_isInited(false)
		, m_position(0.0f)
		, m_scale(1.0f)
		, m_center(64.0f)
//		,m_center(0.0f)
		, m_color(1.0f, 1.0f)
		, m_angle(0.0f)
		, m_specialIndex(-1)
		, m_specialLine(-1)
		, m_specialRow(-1)
	{}
	//������Ϊ�˱��������ק���ṩ�Ľӿ�
	int getNearestVertex(const XVec2& pos, float maxD = 10.0f)const;	//��ȡ��pos����Ķ���ı�ţ�����С��maxD
	void setVertexPos(const XVec2& pos, int index);	//���ö����λ��
	XVec2 getVertexPos(int index)const;
	void setVertexColor(const XFColor& color, int index);
	void setVertexTex(const XVec2& t, int index);
	void clearAllData()
	{
		m_v.clear();
		m_t.clear();
		m_c.clear();
	}
	//��δʵ�ֵĽӿ�
	void addALine();	//����һ��
	void addARow();		//����һ��
};
}
#endif