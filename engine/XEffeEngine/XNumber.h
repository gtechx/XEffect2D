#ifndef _JIA_XNUMBER_
#define _JIA_XNUMBER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSprite.h"

#ifndef TEXT_MODE
#define TEXT_MODE ("0123456789+-*/%.")
#endif

#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH (256)
#endif

//���ֵĶ��뷽ʽ
enum _XNumberAlignmentMode
{
	NUMBER_ALIGNMENT_MODE_LEFT,	//������
	NUMBER_ALIGNMENT_MODE_MIDDLE,	//���ж���
	NUMBER_ALIGNMENT_MODE_RIGHT,	//�Ҳ����
};
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class _XNumber:public _XObjectBasic
{
private:
	_XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�

	_XSprite m_sprite;					//��ͼ
	_XBool m_isInited;
	_XBool m_isVisible;

	float m_angle;			//����ĽǶ�
	float m_angleSin;	//�Ƕȵ�sinֵ
	float m_angleCos;

	_XNumberAlignmentMode m_alignmentMode;
public:
	void setAlignmentMode(_XNumberAlignmentMode mode);
	void setVisible();					//��������ɼ�
	void disVisible();						//����������ɼ�
	_XBool getVisible() const {return m_isVisible;}					//��ȡ����Ƿ�ɼ���״̬ 
private:
	char *m_string;

	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�
public:
	int getMaxPixelWidth();
	int getMaxPixelHeight();
private:
	_XBool m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����
	void updateData();		//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	int m_needShowTextSum;	//��Ҫ��ʾ���ַ�������

	_XVector2 m_position;	//�����λ�ã������������õ�λ��
	_XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	float m_alpha;			//͸��ͨ��
	float m_distance;		//����֮��ľ���	
	_XVector2 m_size;		//���ֵĳߴ磨���ش�С��
	_XVector2 m_layout;		//����ͼƬ�Ĳ���
	_XVector2 m_showSize;
	_XVector2 m_rotateBasicPoint;	//������ת�����ĵ�
public:
	_XBool init(const char *fileName,	//����ͼƬ������
		const _XVector2 &size,			//��������ش�С
		const _XVector2 &layout,			//����ͼƬ�Ĳ���
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		
	_XBool initEx(const char *fileName,	//����ͼƬ������
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	_XBool initFromTTF(const char *filename,	//ttf����������
		int fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��TTF�ļ��ж�ȡ��Դ(��δʵ��)

	void draw();

	using _XObjectBasic::setSize;
	void setSize(float x,float y);	//�����������ʾ��С
	_XVector2 getSize() const {return m_showSize;}
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const {return m_angle;}
	
	using _XObjectBasic::setPosition;
	void setPosition(float x,float y);	//����������ʾ��λ��
	void setPositionX(float x);
	void setPositionY(float y);
	_XVector2 getPosition() const {return m_setPosition;}
	void setRotateBasePoint(float x,float y);

	_XBool setNumber(int temp);								//����������
	_XBool setNumber(float temp,int decimalPartLength);		//���ô�С�������
	_XBool setNumber(const char *temp);		
	//����һ�κ�����ַ���
	_XNumber& operator = (const _XNumber& temp);
	_XBool setACopy(const _XNumber &temp);

	using _XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);	//������ɫ
	_XFColor getColor() const {return m_sprite.getColor();}
	void setAlpha(float a);
	float getAlpha() const {return m_alpha;}
	_XBool release();

	void setDistance(float distance) {m_distance = distance;}
	float getDistance() const {return m_distance;}
	//Ϊ��ʵ����������������Ӽ����ӿ�
	_XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	_XNumber();
	~_XNumber();

	//virtual void justForTest() {;}
};

#include "XNumber.inl"

#endif
