//����ǻ��ڱ�������ƽ����ε���
#ifndef _JIA_XBEZIERDEFORM_
#define _JIA_XBEZIERDEFORM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.3
//--------------------------------
#include "../XMesh2DBz.h"
#include "../XFont/XFontUnicode.h"
#include "XMoveData.h"

//û�м��뵫�ǿ��Կ��Ǽ���ĵ���:���ܵ���(��ˮƽ���ߴ�ֱ��ĳ����Ϊ��׼��������Χ������)
//����Ⱦ����ߵ���
//�ĸ���λ�ñ��ε���

//���ַ�ʽ�ǽ�����ͼƬ��Ϊһ��һ����С��Ԫ��Ȼ��ÿ����Ԫ�ڲ����б��������Σ��Ӷ���ͼʵ��ͼƬ���������
//ʵ�������ַ����Ǵ���ģ����ַ��������ͼƬ��ÿ����������Ԫ���紦��ͼ����������ȷ������Ӧ���Ƕ�����
//ͼ�����ͳһ�ı��������Σ������ʵ����ο�Ex������

//������������Σ������Ѿ�����ʵ�֣��������ܺܲ�

namespace XE{
enum XBDChangeType
{
	BD_CHANGE_NULL,		//û�б��ζ���
	BD_CHANGE_MOVE,		//�ƶ�
	BD_CHANGE_SCALE,	//����
	BD_CHANGE_ROTATE,	//��ת
};
struct XInputEvent;
class XBezierDeform
{
private:
	bool m_isInited;
	bool m_withFilename;
	std::string m_filename;
	XMesh2DBz m_mesh;
	bool m_showHandle;
	bool m_onlyHandle;	//�Ƿ�ֻ��ʾ��
	int m_mouseTouchDistance;	//��괥����ľ���

	XVec2 m_mouseDownPos;	//��갴�µ�λ��
	XVec2 m_oldMousePos;
	std::vector<XVec2> m_changeOldPoint;

	bool m_isMouseDown;
	XBDChangeType m_changeType;	//�Ƿ��ڱ���״̬
	void release();
	//��������
	XVec2 m_mouseOffset;
	XVec2 m_mouseSacle;
public:
	void setMouseParam(const XVec2& offset, const XVec2& scale)	//�������Ĳ���
	{
		m_mouseOffset = offset;
		m_mouseSacle = scale;
	}
	XBezierDeform()
		:m_isInited(false)
		, m_isMouseDown(false)
		, m_mouseTouchDistance(20)
		, m_changeType(BD_CHANGE_NULL)
		, m_showHandle(false)
		, m_onlyHandle(false)
		, m_withScale(false)
		, m_mouseOffset(0.0f)
		, m_mouseSacle(1.0f)
		, m_withFilename(false)
	{}
	void setFilename(const std::string& filename)
	{
		m_withFilename = true;
		m_filename = filename;
	}
	~XBezierDeform() { release(); }
	//w���ؿ�ȣ�h���ظ߶�
	//handX,ˮƽ����Ŀ��Ƶ������
	//handY,��ֱ����Ŀ��Ƶ������
	//vSum�����Ƶ�֮��Ķ�������
	bool init(int w, int h, int handX = 4, int handY = 4, int vSum = 10);
	bool initFromFile(const char *filename = NULL);
	void draw(unsigned int tex,bool withWireframe = true)
	{
		if (!m_isInited) return;
		m_mesh.draw(tex);
		if (!m_showHandle) return;
		if (withWireframe)
		{
			if (m_withScale)
				m_mesh.drawWireframeEx(m_basePosition, m_scale, true, false);
			else
				m_mesh.drawWireframeEx();
		}
	}
	void drawHandle()
	{
		if (!m_isInited || !m_showHandle) return;
		if (m_withScale)
			m_mesh.drawWireframeEx(m_basePosition, m_scale, false, true);
		else
			m_mesh.drawWireframeEx(true, true);
	}
	void inputProc(const XInputEvent &event);
	void reset()	//�������е�
	{
		if (!m_isInited) return;
		m_mesh.reset();
	}
	void setOnlyHandle(bool flag) { m_onlyHandle = flag; }
	bool getIsShowHandle()const { return m_showHandle; }
	XVec2 mapWorld2Local(const XVec2& pos)	//����������ӳ�䵽�ֲ�����
	{
		XVec2 ret;
		if (m_mesh.mapVertex2Tex(pos, ret)) return ret * m_mesh.getPixelSize();
		return XVec2::zero;
	}
	XVec2 mapLocal2World(const XVec2& pos)	//����������ӳ�䵽�ֲ�����
	{
		XVec2 ret;
		if (m_mesh.mapTex2Vertex(pos / m_mesh.getPixelSize(), ret)) return ret;
		return XVec2::zero;
	}
private:
	bool m_isInputEnable;
	bool m_withScale;
	float m_scale;
	XVec2 m_basePosition;
public:
	bool getEnableInput()const { return m_isInputEnable; }
	void setEnableInput(bool flag) { m_isInputEnable = flag; }
	void setScale(const XVec2& base, float scale)
	{
		m_basePosition = base;
		m_scale = scale;
		m_withScale = true;
	}
	XVec2 getScalePos(const XVec2& pos)const	//��������֮ǰ�ı���
	{
		if (m_withScale) return (pos - m_basePosition) / m_scale + m_basePosition;
		else return pos;
	}
};
}
#endif