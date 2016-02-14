#ifndef _JIA_XENGINELOGO_
#define _JIA_XENGINELOGO_
//����һ����ʾ����Logo����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XOSDefine.h"
#include "XSprite.h"
namespace XE{
class XEngineLogo
{
private:
	XResourcePosition m_resoursePosition;
	XBool m_isInited;
	XBool m_mode;	//0��ͨģʽ��1����ģʽ
	XVector2 m_position;
	XSprite m_back;	
	XSprite m_logoBack;
	XSprite m_logoLight;
	XSprite m_logoMiddle;
	XSprite m_logoTextX;
	XSprite m_logoTextE;
	XSprite m_logoTextEC;
	XSprite m_logoTextRE;
	//XSprite m_logoTextECopy[30];

	int m_logoStage;
	float m_xAlpha;
	float m_xSize;
	float m_dxLight;
	float m_eAlpha;
	
	char m_stageFlag[8];	//���������׶α�ǵı���
	//����������ڲ����õķ���
	void setPosition(const XVector2 &position);	//����logo��ʾ��λ��

public:
	XBool init(const XVector2 &position,XResourcePosition resoursePosition,XBool mode = true);		//��ʼ��
	void move(float timeDelay);			//��ʾlogo�Ķ���
	void reset();						//����
	void draw();						//���logo
	XEngineLogo();						//���캯��
	~XEngineLogo();						//���캯��
	void setBackSize(const XVector2 &size);		//���ñ���ͼ�ĳߴ�
	void setBackSize(float x,float y);		//���ñ���ͼ�ĳߴ�

	//����������������δʵ��
	void setSize(const XVector2 &size);			//����logo��ʾ�ĳߴ�
	XBool release();	//�ͷ�logo����Դ
	XBool getIsEnd();
};

inline void XEngineLogo::setPosition(const XVector2 &position)	//����logo��ʾ��λ��
{
	m_position = position;
}
inline XBool XEngineLogo::getIsEnd()
{
	if(!m_isInited) return XFalse;
	return m_logoStage == -2;
}
}
#endif