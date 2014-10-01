#ifndef _JIA_XNUMBEREX_
#define _JIA_XNUMBEREX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//�������Խ��н�һ�����Ż�����һ���������������е��ַ��Ƿ����
#include "XSprite.h"

#ifndef TEXT_MODE
#define TEXT_MODE "0123456789+-*/%."
#endif
#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH 256
#endif
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class _XNumberTexture
{
private:
	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	_XBool m_isInited;	//�Ƿ��ʼ��
public:
	//int m_textureSum;		//��ͼ������
	_XTextureData *m_texture00;
	_XTextureData *m_texture01;
	_XTextureData *m_texture02;
	_XTextureData *m_texture03;
	_XTextureData *m_texture04;
	_XTextureData *m_texture05;
	_XTextureData *m_texture06;
	_XTextureData *m_texture07;
	_XTextureData *m_texture08;
	_XTextureData *m_texture09;
	_XTextureData *m_texture0a;		//+
	_XTextureData *m_texture0b;		//-
	_XTextureData *m_texture0c;		//*
	_XTextureData *m_texture0d;		///
	_XTextureData *m_texture0e;		//%
	_XTextureData *m_texture0f;		//.

public:
	_XBool init(const char *fileName,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
	_XNumberTexture();
};

class _XNumberEx
{
private:
	const _XNumberTexture *m_numberTexture;

	_XSprite m_sprite;					//��ͼ
	_XBool m_isInited;

	float m_angleSin;	//�Ƕȵ�sinֵ
	float m_angleCos;
public:
	char *m_number;
	_XVector2 m_position;	//�����λ�ã������������õ�λ��
	_XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	float m_angle;			//����ĽǶ�
	float m_distance;		//����֮��ľ���	
	_XVector2 m_size;		//���ֵĳߴ磨���ش�С��
	_XVector2 m_showSize;
	_XVector2 m_rotateBasicPoint;	//������ת�����ĵ�
	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr >= 9 ) typeScr = 8;
		if(typeDst >= 8 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
		m_sprite.setBlendType(m_blendTypeScr,m_blendTypeDst);
	}

	_XBool init(const _XNumberTexture *numberTexture,const _XVector2 &size);		
	void draw();

	void setSize(const _XVector2& size);		//�����������ʾ��С
	void setSize(float x,float y);	//�����������ʾ��С
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	void setPosition(const _XVector2& position);	//����������ʾ��λ��
	void setPosition(float x,float y);	//����������ʾ��λ��
	void setRotateBasePoint(float x,float y);

	_XBool setNumber(int temp);								//����������
	_XBool setNumber(float temp,int decimalPartLength);		//���ô�С�������
	_XBool setNumber(const char *temp);								//����һ�κ�����ַ���
	_XNumberEx& operator = (const _XNumberEx& temp);
	_XBool setACopy(const _XNumberEx &temp);
	void setColor(float r,float g,float b,float a);	//������ɫ
	void setAlpha(float a);
	float getAlpha() const {return m_sprite.getAlpha();}
	_XBool release();

	void setDistance(float distance){m_distance = distance;}
	float getDistance() const {return m_distance;}
	_XNumberEx();
};

//������һ������ʵ�ֶ�̬���ֱ仯����,���ǿ�������һ�����ֹ���
class _XMoveNumber
{
private:
	int m_nowNumber;	//��ǰ����ֵ
	float m_armNumber;	//Ŀ����Ҫ�ﵽ����ֵ
public:
	void addArmNumber(float number);	//����Ŀ����ֵ������
	void setArmNumber(float number);	//����Ŀ����ֵ
	int getNowNumber();				//���ص�ǰ����ֵ
	int getArmNumber();
	_XBool move(int delay);			//���������Ƿ����仯0��û�б仯��1�������˱仯
	void reset();
	_XMoveNumber()
	:m_nowNumber(0)
	,m_armNumber(0.0f)
	{}
};
//������һ�����ݶ�̬�仯���࣬��������ʵ��һЩ��̬�������Լ���̬��Ч��
enum _XMoveDataMode
{
	MOVE_DATA_MODE_LINE,		//���Ա仯
	MOVE_DATA_MODE_SIN,			//sin���߱仯,	�ȿ����		��������:��
	MOVE_DATA_MODE_SIN_MULT,	//sin���߱仯,	�ȿ����		��������:С
	MOVE_DATA_MODE_SQRT2,		//4�ο���,		�ȿ����		��������:��
	MOVE_DATA_MODE_SQRT2_MULT,	//ʹ�ó˷�ʵ��,	�ȿ����		��������:��
	MOVE_DATA_MODE_COS,			//cos���߱仯,	�������		��������:��
	MOVE_DATA_MODE_COS_MULT,	//cos���߱仯,	�������		��������:С
	MOVE_DATA_MODE_DSIN,		//sin���߱仯,	�ȿ�����ٿ�	��������:��	
	MOVE_DATA_MODE_DCOS,		//cos���߱仯,	�����������	��������:��
	MOVE_DATA_MODE_DSIN_MULT,	//sin���߱仯,	�ȿ�����ٿ�	��������:С	
	MOVE_DATA_MODE_DCOS_MULT,	//cos���߱仯,	�����������	��������:С
	MOVE_DATA_MODE_SHAKE,		//˥�������ߣ��ȿ����(��δʵ��)
	MOVE_DATA_MODE_MAX,
};

class _XMoveData
{
private:
	float m_startData;	//��ʼ��ֵ
	float m_endData;	//��ֹ��ֵ
	float m_nowData;	//��ǰ����ֵ
	float m_speed;
	float m_timer;		//ʱ����
	int m_loopTimer;	//ѭ���Ĵ���
	
	_XBool m_isEnd;	//�˶��Ƿ����1:���� 0:�˶���
	int m_isLoop;	//�Ƿ�ѭ��,�����Ǿ���ѭ��,�Լ�ѭ������
	_XMoveDataMode m_mode;
public:
	float getStartData() const {return m_startData;}
	float getEndData() const {return m_endData;}
	float getSpeed() const {return m_speed;}
	int getLoopTimer() const 
	{
		//if(m_isLoop == 0) return 0;
		return m_isLoop;
	}
	_XMoveDataMode getMoveMode() const {return m_mode;}
public:
	//isLoop:0,��ѭ��,>0ѭ���Ĵ���,<0����ѭ��
	void set(float start,float end,float speed,_XMoveDataMode mode = MOVE_DATA_MODE_SIN_MULT,int isLoop = 0)
	{
		m_isEnd = XFalse;
		m_startData = start;
		m_endData = end;
		m_speed = speed;
		m_nowData = start;
		m_timer = 0.0f;
		m_mode = mode;
		m_isLoop = isLoop;
		if(isLoop >= 1) m_loopTimer = isLoop;
		else m_loopTimer = -1;	//����ѭ��
	}
	void reSet()	//�������¿�ʼ
	{
		m_nowData = m_startData;
		m_timer = 0.0f;
		m_isEnd = XFalse;
		if(m_isLoop >= 1) m_loopTimer = m_isLoop;
		else m_loopTimer = -1;	//����ѭ��
	}
	void move(int delay);
	float getValue(float time);	//����timeʱ�̵���ֵ:��λms
	float getNowData() const {return m_nowData;}
	float getNowTimer() const {return m_timer;}
	_XBool getIsEnd() const {return m_isEnd;}
	void setEnd()
	{
		m_isEnd = XTrue;
		m_nowData = m_endData;
	}
	_XMoveData()
	:m_startData(0.0f)
	,m_endData(0.0f)
	,m_nowData(0.0f)
	,m_isEnd(XTrue)
	,m_mode(MOVE_DATA_MODE_SIN)
	,m_isLoop(0)
	{}
	void getParamStr(char * str)
	{
		if(str == NULL) return;
		sprintf(str,"%f,%f,%f,%d,%d,",m_startData,m_endData,m_speed,m_mode,m_isLoop);
	}
};

#include "XNumberEx.inl"

#endif
