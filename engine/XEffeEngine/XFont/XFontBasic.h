#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.1
//--------------------------------
#include "XSprite.h"
namespace XE{
class XFBO;
//���������Ļ����࣬������������������̳ж���
 enum XFontAlignmentModeX
{
	FONT_ALIGNMENT_MODE_X_LEFT,	//������
	FONT_ALIGNMENT_MODE_X_MIDDLE,	//���ж���
	FONT_ALIGNMENT_MODE_X_RIGHT,	//�Ҳ����
};
enum XFontAlignmentModeY
{
	FONT_ALIGNMENT_MODE_Y_UP,		//�϶���
	FONT_ALIGNMENT_MODE_Y_MIDDLE,	//���ж���
	FONT_ALIGNMENT_MODE_Y_DOWN,	//�¶���
};

class XFontBasic:public XObjectBasic
{
private:
	static const int m_stringMaxLength = 64;	//Ĭ�ϵ�����ַ�������(������Ҫ�Ż���)
protected:
	XBool m_isInited;		//�Ƿ��Ѿ���ʼ��
	XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	XSprite m_sprite;		//��ͼ
	char *m_string;			//��Ҫ��ʾ���ַ���
	int m_lineSum;			//��ʾ�ַ���������
	int m_needShowTextSum;		//��Ҫ��ʾ���ַ�������

	XBool m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����

	int m_maxStringLen;		//֧�ֵ�����ַ�������

	XFontAlignmentModeX m_alignmentModeX;	//�����ˮƽ���뷽ʽ
	XFontAlignmentModeY m_alignmentModeY;	//����Ĵ�ֱ���뷽ʽ

	//Ϊ���������ﶨ�����FBO�Ķ���
	XBool m_withFBO;		//�Ƿ�ʹ��FBO
	XBool m_needUpdateFBO;	//�Ƿ���Ҫ����FBO
	XFBO *m_pFbo;			//ʹ�õ�FBO
	XSprite m_fboSprite;	//��ͼ
public:
	using XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);
	XFColor getColor() const {return m_sprite.getColor();}
protected:
	XBool m_isVisible;
public:
	void setVisible(); //��������ɼ�
	void disVisible(); //����������ɼ�
	XBool getVisible() const {return m_isVisible;}			//��ȡ����Ƿ�ɼ���״̬ 
protected:
	//+++++++++++++++++++++++++++
	//���������Ӳü�����
	XRect m_clipRect;	//����ü��ľ�����������������Ͻǵ�һ����Ծ���
	XBool m_isCliped;
public:
	void setClipRect(const XRect &temp){setClipRect(temp.left,temp.top,temp.right,temp.bottom);}
	void setClipRect(float left,float top,float right,float bottom);
	void disClip();
	//---------------------------

	void setAlignmentModeX(XFontAlignmentModeX mode);
	void setAlignmentModeY(XFontAlignmentModeY mode);
protected:
	float m_angle;			//����ĽǶ�
	float m_angleSin;		//����Ƕȵ�����	����������������Ϊ�˽�ʡ����ʱ��
	float m_angleCos;		//����Ƕȵ�����

	float m_distance;		//�ַ�֮��ľ���	
	float m_distanceY;		//��֮��ľ���

	XVector2 m_position;		//�����λ��
	XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	XVector2 m_rotateBasicPoint;	//������ת�����ĵ�

	XVector2 m_size;		//���ֵĳߴ磨���ش�С��
	XVector2 m_scale;	//��ʾ����������Ŵ�С
public:

	void setDistance(float distance);			//����������֮��ľ���
	void setDistance(float x,float y);			//����������֮��ľ���
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const {return m_angle;}
	virtual const char * getString() const {return m_string;}

	using XObjectBasic::setScale;
	void setScale(float x,float y);			//�����������ʾ��С
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);			//����������ʾ��λ��
	void setRotateBasePoint(float x,float y);

	XVector2 getTextSize() const {return m_size;}
	XVector2 getScale() const {return m_scale;}
	XVector2 getPosition() const {	return XVector2(m_position.x,m_position.y);}
protected:
	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�

	virtual void updateData() = 0;	//����һ�����麯������ʵ�ֲο���������
public:
	int getMaxPixelWidth();
	int getMaxPixelHeight();

public:
	//���ظ�ֵ������
	XFontBasic& operator = (const XFontBasic& temp);
	XBool setACopy(const XFontBasic &temp);
protected:
	XBool m_isPassword;
public:
	void setIsPassWord(XBool temp);
	XBool getIsPassword() const {return m_isPassword;}
protected:
	XFontBasic(int maxStrLen)
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
		,m_scale(1.0f,1.0f)
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
		m_string = XMem::createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	XFontBasic()
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
		,m_scale(1.0f,1.0f)
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
		m_maxStringLen = m_stringMaxLength;
		m_string = XMem::createArrayMem<char>(m_maxStringLen);
		m_string[0] = '\0';
	}
	//���ؿ������캯��
	XFontBasic(const XFontBasic& temp);
public:
	XBool setString(const char *p);	//������ʾ���ַ���

	virtual ~XFontBasic();	//������麯����Ϊ�˷�ֹͨ������ָ��ɾ�����������ʱ��ɵ��ڴ�й¶
	virtual void draw() = 0;
	virtual XBool initEx(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,XBool withFBO = XFalse) = 0;	
	XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void setMaxStrLen(int maxStrLen) = 0;

	//virtual void justForTest() {;}
};
#if WITH_INLINE_FILE
#include "XFontBasic.inl"
#endif
}
#endif