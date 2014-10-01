#ifndef _JIA_XENGINELOGO_
#define _JIA_XENGINELOGO_
//����һ����ʾ����Logo����
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XEffeEngine.h"

class _XEngineLogo
{
private:
	_XResourcePosition m_resoursePosition;
	char m_isInited;
	char m_mode;	//0��ͨģʽ��1����ģʽ
	_XVector2 m_position;
	_XSprite m_back;	
	_XSprite m_logoBack;
	_XSprite m_logoLight;
	_XSprite m_logoMiddle;
	_XSprite m_logoTextX;
	_XSprite m_logoTextE;
	_XSprite m_logoTextEC;
	_XSprite m_logoTextRE;
	//_XSprite m_logoTextECopy[30];

	int m_logoStage;
	float m_xAlpha;
	float m_xSize;
	float m_dxLight;
	float m_eAlpha;
	
	char m_stageFlag[8];	//���������׶α�ǵı���
	//����������ڲ����õķ���
	void setPosition(const _XVector2 &position);	//����logo��ʾ��λ��

public:
	int init(const _XVector2 &position,_XResourcePosition resoursePosition,int mode = 0);		//��ʼ��
	void move(int timeDelay);			//��ʾlogo�Ķ���
	void reset();						//����
	void draw();						//���logo
	_XEngineLogo();						//���캯��
	~_XEngineLogo();						//���캯��
	void setBackSize(const _XVector2 &size);		//���ñ���ͼ�ĳߴ�
	void setBackSize(float x,float y);		//���ñ���ͼ�ĳߴ�

	//����������������δʵ��
	void setSize(const _XVector2 &size);			//����logo��ʾ�ĳߴ�
	int release();	//�ͷ�logo����Դ
	int getIsEnd();
};

inline void _XEngineLogo::setPosition(const _XVector2 &position)	//����logo��ʾ��λ��
{
	m_position = position;
}
inline void _XEngineLogo::setBackSize(const _XVector2 &size)		//���ñ���ͼ�ĳߴ�
{
	m_back.setSize(size);
}
inline void _XEngineLogo::setBackSize(float x,float y)		//���ñ���ͼ�ĳߴ�
{
	m_back.setSize(x,y);
}
inline int _XEngineLogo::getIsEnd()
{
	if(m_isInited == 0) return 0;
	if(m_logoStage == -2) return 1;
	else return 0;
}

#endif