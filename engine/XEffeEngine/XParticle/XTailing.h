#ifndef _JIA_XTAILING_
#define _JIA_XTAILING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
//����һ����βЧ���Ļ���
#include "..\XMath\XVector2.h"
//#include "..\XMath\XRect.h"
#include "..\XMath\XFColor.h"
//#include "..\XShaderGLSL.h"
//#include "..\XMath\XRandomFun.h"
namespace XE{
#define WITH_EX_EFFECT	//�Ƿ������õ�Ч��
struct XTailingPoint
{
	XVector2 position;	//λ��
	float life;			//����
	XTailingPoint()
		:life(0.0f)
	{}
};	//��β��һ���ڵ�
class XTailing	//��βЧ�����߼���
{
protected:
	static const int m_maxPointSum = 64;
	bool m_isEnd;	//�Ƿ����
	int m_headPoint;	//ͷ�����
	int m_endPoint;		//β�����
	XTailingPoint m_points[m_maxPointSum];
	float m_maxLife;
public:
	void AddPoint(const XVector2 &pos);	//�����µĵ�
	void Update(const float &stepTime);			//����״̬����λΪ��
	int GetHeadIndex() const {return m_headPoint;}
	int GetEndIndex() const {return m_endPoint;}
	bool GetIsEnd() const {return m_isEnd;}
	void SetMaxLife(const float &life){m_maxLife = life;}
	float GetMaxLife()const{return m_maxLife;}
	XTailing()
		:m_isEnd(true)
		,m_headPoint(0)
		,m_endPoint(0)
		,m_maxLife(1.0f)
	{}
};
class XTailingD:public XTailing	//��βЧ���������
{
protected:
	XFColor m_color;
	float m_size;
public:
	XTailingD()
		:m_size(1.0f)
		,m_color(1.0f,1.0f,1.0f,1.0f)
	{}
	virtual void Draw();	//��溯��
	void SetColor(const XFColor &c){m_color = c;}
	void SetSize(const float &size){m_size = size;}
	virtual void DrawBegin() = 0;
	virtual void DrawEnd();
	virtual void DrawBody() = 0;
	virtual int UpdateData(float *v,float *c) = 0;	//For Test
};
class XTailingDPoint:public XTailingD
{
public:
	virtual void DrawBegin();
	virtual void DrawBody();
	virtual int UpdateData(float *,float *) {return 0;}	//For Test
};
class XTailingDLine:public XTailingD
{
public:
	virtual void Draw();	//��溯��
	virtual void DrawBegin();
	virtual void DrawBody();
	virtual void DrawEnd();
	virtual int UpdateData(float *v,float *c);	//For Test
};
class XTailingObject
{
protected:
	XVector2 m_position;
	XVector2 m_speed;
	bool m_isStop;
	XTailingD *m_tailing;
public:
	void Update(const float &stepTime,const XRect &rect);//�������˶�
	//+++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˲��Զ�����Ľӿ�
	int UpdateData(float *v,float *c){if(m_tailing != NULL) return m_tailing->UpdateData(v,c);return 0;}
	//---------------------------------------------------
	void Draw(){if(m_tailing != NULL) m_tailing->Draw();}
	void DrawBegin(){if(m_tailing != NULL) m_tailing->DrawBegin();}
	void DrawBody(){if(m_tailing != NULL) m_tailing->DrawBody();}
	void DrawEnd(){if(m_tailing != NULL) m_tailing->DrawEnd();}
	void Reset(const XVector2 &pos,const XVector2 &speed,XTailingD *t);
	bool GetIsEnd()const{return m_isStop && (m_tailing == NULL || m_tailing->GetIsEnd());}
	void SetSpeed(const XVector2 &speed){m_speed = speed;}
	XTailingObject()
		:m_isStop(true)
		,m_tailing(NULL)
	{}
	XVector2 GetPosition()const{return m_position;}
	XVector2 GetSpeed()const{return m_speed;}
};
#if WITH_INLINE_FILE
#include "XTailing.inl"
#endif
}
#endif