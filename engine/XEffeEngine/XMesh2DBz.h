#ifndef _JIA_XMESH2DBZ_
#define _JIA_XMESH2DBZ_
#include "XMesh2D.h"
//����һ��������������ֵ��2D����
#define OPTIMIZATION
namespace XE{
#define DEFAULT_BD_FILENAME "bdData"
class XMesh2DBz
{
private:
	bool m_isInited;
	XMesh2D m_mesh;
	int m_vSum;//���Ƶ�֮��Ķ�������
	int m_handSumX;				//���Ƶ������
	int m_handSumY;

	//bool handleFlag[HAND_SUM];	//���ڵ��״̬�Ƿ����仯,��Ǳ仯��Χ������ʱֻ���±仯��Χ�ڵ����ݣ�������Ŀǰ������δʵ�֣����ܿ�������һ����
	std::vector<XVec2> m_handlePoint;	//���ڵ��ڵĵ�
	std::vector<XVec2> m_tmpHandlePoint;

public:
	int getNearestPointIndex(const XVec2& pos, float distance = 10.0f)
	{
		int ret = -1;
		float nDis = 0.0f;
		float dis = distance * distance;
		float tmpDis;
		for (int i = 0; i < m_handSumX * m_handSumY; ++i)
		{
			tmpDis = m_handlePoint[i].getLengthSqure(pos);
			if (tmpDis > dis) continue;
			if (ret < 0 || tmpDis < nDis)
			{
				ret = i;
				nDis = tmpDis;
			}
		}
		return ret;
	}
	//	int getNearestPointIndex(float x,float y,float distance = 10.0f)
	//	{
	//		return getNearestPointIndex(XVec2(x, y), distance);
	//	} 
private:
	int m_w;
	int m_h;
	void release()
	{
		if (!m_isInited) return;
		m_isInited = false;
	}
public:
	XVec2 getPixelSize()const { return XVec2(m_w, m_h); }
	void updateMesh();
	void setHandlePos(const XVec2& pos, int index)
	{
		if (index < 0 || index >= m_handlePoint.size()) return;
		m_handlePoint[index] = pos;
	}
	XMesh2DBz()
		:m_isInited(false)
		, m_specialIndex(-1)
		, m_specialLine(-1)
		, m_specialRow(-1)
	{}
	~XMesh2DBz() { release(); }
	//w���ؿ�ȣ�h���ظ߶�
	//handX,ˮƽ����Ŀ��Ƶ������
	//handY,��ֱ����Ŀ��Ƶ������
	//vSum�����Ƶ�֮��Ķ�������
	bool init(int w, int h, int handX, int handY, int vSum);
	bool initFromFile(const char *filename = NULL);
	bool initFromFileB(const char *filename = NULL);
	void draw(unsigned int tex, XShaderGLSL *pShader = NULL)const
	{
		if (!m_isInited) return;
		m_mesh.drawTexture(tex, pShader);
	}
	void drawWireframeEx(bool special = false, bool onlyHandle = false)const;	//�������͵�
	void drawWireframeEx(const XVec2& base, float scale, bool withWireframe, bool withHandle)const;	//�������͵�
	//���ı��ķ�ʽ��������
	bool save(const char *filename = NULL)const;//�����ݱ��浽�ļ�
	bool load(const char *filename = NULL); //���ļ��ж�ȡ����
	//�Զ����Ƶķ�ʽ��������
	bool saveB(const char *filename = NULL)const;//�����ݱ��浽�ļ�
	bool loadB(const char *filename = NULL); //���ļ��ж�ȡ����

	void reset();	//�������е�

	int getHandleSumX()const { return m_handSumX; }
	int getHandleSumY()const { return m_handSumY; }
	int getHandleSum()const { return m_handSumX * m_handSumY; }
	XVec2 getHandlePos(int index)const
	{
		if (index < 0 || index >= m_handlePoint.size()) return XVec2::zero;
		return m_handlePoint[index];
	}
	std::vector<XVec2> getHandlePoses()const { return m_handlePoint; }
	void setTextureInfo(float sx, float sy, float w, float h)
	{
		if (!m_isInited) return;
		m_mesh.setTextureInfo(sx, sy, w, h);
	}
private:
	int m_specialIndex;	//�����ı��
	int m_specialLine;	//�������
	int m_specialRow;	//�������
	XVec2 m_position;
public:
	void setSpecialIndex(int index) { m_specialIndex = index; }
	int getSpecialIndex()const { return m_specialIndex; }
	void setSpecialLine(int index) { m_specialLine = index; }
	int getSpecialLine()const { return m_specialLine; }
	void setSpecialRow(int index) { m_specialRow = index; }
	int getSpecialRow()const { return m_specialRow; }

	bool mapVertex2Tex(const XVec2& pos, XVec2& out)const { return m_mesh.mapVertex2Tex(pos, out); }
	bool mapTex2Vertex(const XVec2& pos, XVec2& out)const { return m_mesh.mapTex2Vertex(pos, out); }
	void setColor(const XFColor& c) { m_mesh.setColor(c); }

	void setPosition(const XVec2& pos)
	{
		XVec2 offset = pos - m_position;
		m_position = pos;
		for (auto it = m_handlePoint.begin(); it != m_handlePoint.end(); ++it)
		{
			(*it) += offset;
		}
		updateMesh();
	}
};
}
#endif