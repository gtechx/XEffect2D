#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XSprite.h"

//���������Ļ����࣬������������������̳ж���

enum _XFontAlignmentModeX
{
	FONT_ALIGNMENT_MODE_X_LEFT,	//������
	FONT_ALIGNMENT_MODE_X_MIDDLE,	//���ж���
	FONT_ALIGNMENT_MODE_X_RIGHT,	//�Ҳ����
};
enum _XFontAlignmentModeY
{
	FONT_ALIGNMENT_MODE_Y_UP,		//�϶���
	FONT_ALIGNMENT_MODE_Y_MIDDLE,	//���ж���
	FONT_ALIGNMENT_MODE_Y_DOWN,	//�¶���
};

#define STRING_MAX_SIZE (64)	//Ĭ�ϵ�����ַ�������

class _XFontBasic:public _XObjectBasic
{
//private:
protected:
	_XBool m_isInited;		//�Ƿ��Ѿ���ʼ��
	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	_XSprite m_sprite;		//��ͼ
	char *m_string;			//��Ҫ��ʾ���ַ���
	int m_lineSum;			//��ʾ�ַ���������
	int m_needShowTextSum;		//��Ҫ��ʾ���ַ�������

	_XBool m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����

	int m_maxStringLen;		//֧�ֵ�����ַ�������

	_XFontAlignmentModeX m_alignmentModeX;	//�����ˮƽ���뷽ʽ
	_XFontAlignmentModeY m_alignmentModeY;	//����Ĵ�ֱ���뷽ʽ

	//Ϊ���������ﶨ�����FBO�Ķ���
	_XBool m_withFBO;		//�Ƿ�ʹ��FBO
	_XBool m_needUpdateFBO;	//�Ƿ���Ҫ����FBO
	_XFBO *m_pFbo;			//ʹ�õ�FBO
	_XSprite m_fboSprite;	//��ͼ
public:
	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a)
	{
		m_sprite.setColor(r,g,b,a);
		updateChildColor();
	}
	void setAlpha(float a) 
	{
		m_sprite.setAlpha(a);
		updateChildAlpha();
	}
	_XFColor getColor() const {return m_sprite.getColor();}
protected:
	_XBool m_isVisible;
public:
	void setVisible() 
	{
		m_isVisible = XTrue;
		updateChildVisible();
	}					//��������ɼ�
	void disVisible() 
	{
		m_isVisible = XFalse;
		updateChildVisible();
	}					//����������ɼ�
	_XBool getVisible() const {return m_isVisible;}			//��ȡ����Ƿ�ɼ���״̬ 
protected:
	//+++++++++++++++++++++++++++
	//���������Ӳü�����
	_XRect m_clipRect;	//����ü��ľ�����������������Ͻǵ�һ����Ծ���
	_XBool m_isCliped;
public:
	void setClipRect(const _XRect &temp){setClipRect(temp.left,temp.top,temp.right,temp.bottom);}
	void setClipRect(float left,float top,float right,float bottom)
	{
		m_clipRect.set(left,top,right,bottom);
		m_isCliped = XTrue;
		m_needUpdateData = XTrue;
	}
	void disClip()
	{
		if(m_isCliped) 
		{
			m_isCliped = XFalse;
			m_needUpdateData = XTrue;
		}
	}
	//---------------------------

	void setAlignmentModeX(_XFontAlignmentModeX mode)
	{
		if(mode == m_alignmentModeX) return;
		m_alignmentModeX = mode;
		m_needUpdateData = XTrue;
	}
	void setAlignmentModeY(_XFontAlignmentModeY mode)
	{
		if(mode == m_alignmentModeY) return;
		m_alignmentModeY = mode;
		m_needUpdateData = XTrue;
	}
protected:
	float m_angle;			//����ĽǶ�
	float m_angleSin;		//����Ƕȵ�����	����������������Ϊ�˽�ʡ����ʱ��
	float m_angleCos;		//����Ƕȵ�����

	float m_distance;		//�ַ�֮��ľ���	
	float m_distanceY;		//��֮��ľ���

	_XVector2 m_position;		//�����λ��
	_XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	_XVector2 m_rotateBasicPoint;	//������ת�����ĵ�

	_XVector2 m_size;		//���ֵĳߴ磨���ش�С��
	_XVector2 m_showSize;	//��ʾ����������Ŵ�С
public:

	void setDistance(float distance);			//����������֮��ľ���
	void setDistance(float x,float y);			//����������֮��ľ���
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const {return m_angle;}
	virtual const char * getString() const {return m_string;}

	using _XObjectBasic::setSize;
	void setSize(float x,float y);			//�����������ʾ��С
	using _XObjectBasic::setPosition;
	void setPosition(float x,float y);			//����������ʾ��λ��
	void setRotateBasePoint(float x,float y);

	_XVector2 getTextSize() const {return m_size;}
	_XVector2 getSize() const {return m_showSize;}
	_XVector2 getPosition() const {	return _XVector2(m_position.x,m_position.y);}
protected:
	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�

	virtual void updateData() = 0;	//����һ�����麯������ʵ�ֲο���������
public:
	int getMaxPixelWidth()
	{
		if(m_needUpdateData)
		{
			updateData();
			m_needUpdateData = XFalse;
		}
		return m_maxPixelWidth;
	}
	int getMaxPixelHeight()
	{
		if(m_needUpdateData)
		{
			updateData();
			m_needUpdateData = XFalse;
		}
		return m_maxPixelHeight;
	}

public:
	//���ظ�ֵ������
	_XFontBasic& operator = (const _XFontBasic& temp);
	_XBool setACopy(const _XFontBasic &temp);
protected:
	_XBool m_isPassword;
public:
	void setIsPassWord(_XBool temp)
	{
		if((m_isPassword && !temp) || (!m_isPassword && temp))
		{
			m_isPassword = temp;
			m_needUpdateData = XTrue;
		}
	}
	_XBool getIsPassword() const {return m_isPassword;}
protected:
	_XFontBasic(int maxStrLen)
		:m_needUpdateData(XFalse)
		,m_alignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(XFalse)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
		,m_isVisible(XTrue)
		,m_isCliped(XFalse)
		,m_clipRect(0,0,0,0)
		,m_isPassword(XFalse)
		,m_withFBO(XFalse)
		//,m_withFBO(XTrue)
		,m_needUpdateFBO(XFalse)
		,m_pFbo(NULL)
	{
		if(maxStrLen < 2) m_maxStringLen = 2;
		else m_maxStringLen = maxStrLen;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	_XFontBasic()
		:m_needUpdateData(XFalse)
		,m_alignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(XFalse)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
		,m_isVisible(XTrue)
		,m_isCliped(XFalse)
		,m_clipRect(0,0,0,0)
		,m_isPassword(XFalse)
		,m_withFBO(XFalse)
		//,m_withFBO(XTrue)
		,m_needUpdateFBO(XFalse)
		,m_pFbo(NULL)
	{
		m_maxStringLen = STRING_MAX_SIZE;
		m_string = createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	//���ؿ������캯��
	_XFontBasic(const _XFontBasic& temp);
public:
	_XBool setString(const char *p);	//������ʾ���ַ���

	virtual ~_XFontBasic()	//������麯����Ϊ�˷�ֹͨ������ָ��ɾ�����������ʱ��ɵ��ڴ�й¶
	{
		XDELETE_ARRAY(m_string);
		if(m_pFbo != NULL) XDELETE(m_pFbo);
	}
	virtual void draw() = 0;
	virtual _XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,_XBool withFBO = XFalse) = 0;	
	_XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void setMaxStrLen(int maxStrLen) = 0;

	//virtual void justForTest() {;}
};
inline _XBool _XFontBasic::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(x,y,getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XBool _XFontBasic::setString(const char *p)	//������ʾ���ַ���
{
	if(p == NULL) return XFalse;
	if(strcmp(p,m_string) == 0) return XTrue;	//�������ͬ���ַ���������
	if(strlen(p) < m_maxStringLen)
	{
		strcpy(m_string,p);
	}else
	{
		memcpy(m_string,p,m_maxStringLen);
		if(isAtUnicodeEnd(m_string,m_maxStringLen - 3))
		{
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '.';
			m_string[m_maxStringLen - 1] = '\0';
		}else
		{
			m_string[m_maxStringLen - 4] = '.';
			m_string[m_maxStringLen - 3] = '.';
			m_string[m_maxStringLen - 2] = '\0';
		}
		//strlen(p) >= m_maxStringLen
	}
	m_needUpdateData = XTrue;
	return XTrue;
}
inline void _XFontBasic::setPosition(float x,float y)			//����������ʾ��λ��
{
	m_setPosition.set(x,y);
	updateChildPos();
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setDistance(float distance)			//����������֮��ľ���
{
	m_distance = distance;
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setAngle(float angle)	//����������ʾ�ĽǶ�
{
	m_angle = angle;
	updateChildAngle();
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * DEGREE2RADIAN);
	m_angleCos = cos(angle * DEGREE2RADIAN);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setSize(float x,float y)	//�����������ʾ��С
{
	m_showSize.set(x,y);
	updateChildSize();
	m_sprite.setSize(x,y);
	m_needUpdateData = XTrue;
}
inline void _XFontBasic::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}
#endif