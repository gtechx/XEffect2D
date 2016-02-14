//����ǻ��ڱ�������ƽ����ε���
#ifndef _JIA_XBEZIERDEFORM_
#define _JIA_XBEZIERDEFORM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.3
//--------------------------------
#include "XMesh2D.h"

//û�м��뵫�ǿ��Կ��Ǽ���ĵ���:���ܵ���(��ˮƽ���ߴ�ֱ��ĳ����Ϊ��׼��������Χ������)
//����Ⱦ����ߵ���
//�ĸ���λ�ñ��ε���

//���ַ�ʽ�ǽ�����ͼƬ��Ϊһ��һ����С��Ԫ��Ȼ��ÿ����Ԫ�ڲ����б��������Σ��Ӷ���ͼʵ��ͼƬ���������
//ʵ�������ַ����Ǵ���ģ����ַ��������ͼƬ��ÿ����������Ԫ���紦��ͼ����������ȷ������Ӧ���Ƕ�����
//ͼ�����ͳһ�ı��������Σ������ʵ����ο�Ex������

//������������Σ������Ѿ�����ʵ�֣��������ܺܲ�
#define OPTIMIZATION

namespace XE{
#define DEFAULT_BD_FILENAME "bdData.xml"
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
	XMesh2D m_mesh;
	int m_vSum;//���Ƶ�֮��Ķ�������
	int m_handSumX;				//���Ƶ������
	int m_handSumY;
	int m_mouseTouchDistance;	//��괥����ľ���

	//bool handleFlag[HAND_SUM];	//���ڵ��״̬�Ƿ����仯,��Ǳ仯��Χ������ʱֻ���±仯��Χ�ڵ����ݣ�������Ŀǰ������δʵ�֣����ܿ�������һ����
	std::vector<XVector2> m_handlePoint;	//���ڵ��ڵĵ�
	std::vector<XVector2> m_tmpHandlePoint;
	int m_curMouseIndex;
	XVector2 m_mouseDownPos;	//��갴�µ�λ��
	std::vector<XVector2> m_changeOldPoint;

	bool m_isMouseDown;
	int getCurMouseChoose(float x,float y)
	{
		for(int i = 0;i < m_handSumX * m_handSumY;++ i)
		{
			if(m_handlePoint[i].getLengthSqure(x,y) < m_mouseTouchDistance * m_mouseTouchDistance) return i;	//�ж���Ч�ľ���
		}
		return -1;
	} 
	void updateNet();
	bool m_needDrawHandle;	//�Ƿ���Ҫ�����Ƶ�
	XBDChangeType m_changeType;	//�Ƿ��ڱ���״̬
	bool load(const char *filename = NULL); //���ļ��ж�ȡ����
public:
	XBezierDeform()
		:m_isInited(false)
		,m_handlePoint(NULL)
		,m_isMouseDown(false)
		,m_needDrawHandle(false)
		,m_curMouseIndex(-1)
		,m_mouseTouchDistance(10)
		,m_changeType(BD_CHANGE_NULL)
	{}
	//w���ؿ�ȣ�h���ظ߶�
	//handX,ˮƽ����Ŀ��Ƶ������
	//handY,��ֱ����Ŀ��Ƶ������
	//vSum�����Ƶ�֮��Ķ�������
	bool init(int w,int h,int handX,int handY,int vSum);
	bool initFromFile(int w,int h,const char *filename = NULL);
	void draw(unsigned int tex);
	bool save(const char *filename = NULL)const;//�����ݱ��浽�ļ�
	void inputProc(const XInputEvent &event);
};
}
#endif