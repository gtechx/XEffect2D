#ifndef _JIA_XNUMBER_
#define _JIA_XNUMBER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSprite.h"
namespace XE{
#ifndef TEXT_MODE
#define TEXT_MODE "0123456789+-*/%."
#endif

#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH (256)
#endif

//���ֵĶ��뷽ʽ
enum XNumberAlignmentMode
{
	NUMBER_ALIGNMENT_MODE_LEFT,		//������
	NUMBER_ALIGNMENT_MODE_MIDDLE,	//���ж���
	NUMBER_ALIGNMENT_MODE_RIGHT,	//�Ҳ����
};
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class XNumber:public XObjectBasic
{
private:
	XResourcePosition m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�

	XSprite m_sprite;					//��ͼ
	XBool m_isInited;
	XBool m_isVisible;
	XBool m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����

	float m_angle;			//����ĽǶ�
	float m_angleSin;	//�Ƕȵ�sinֵ
	float m_angleCos;

	XNumberAlignmentMode m_alignmentMode;
public:
	void setAlignmentMode(XNumberAlignmentMode mode);
	void setVisible();					//��������ɼ�
	void disVisible();						//����������ɼ�
	XBool getVisible() const {return m_isVisible;}					//��ȡ����Ƿ�ɼ���״̬ 
private:
	char *m_string;

	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�
public:
	int getMaxPixelWidth();
	int getMaxPixelHeight();
private:
	void updateData();		//������Ҫ�����ڲ�����
	XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	int m_needShowTextSum;	//��Ҫ��ʾ���ַ�������

	XVector2 m_position;	//�����λ�ã������������õ�λ��
	XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	float m_alpha;			//͸��ͨ��
	float m_distance;		//����֮��ľ���	
	XVector2 m_size;		//���ֵĳߴ磨���ش�С��
	XVector2 m_layout;		//����ͼƬ�Ĳ���
	XVector2 m_scale;
	XVector2 m_rotateBasicPoint;	//������ת�����ĵ�

	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
public:
	XBool init(const char *fileName,	//����ͼƬ������
		const XVector2 &size,			//��������ش�С
		const XVector2 &layout,			//����ͼƬ�Ĳ���
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);		
	XBool initEx(const char *fileName,	//����ͼƬ������
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	XBool initFromTTF(const char *filename,	//ttf����������
		int fontSize,
		XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��TTF�ļ��ж�ȡ��Դ(��δʵ��)

	void draw();

	using XObjectBasic::setScale;
	void setScale(float x,float y);	//�����������ʾ��С
	XVector2 getScale() const {return m_scale;}
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const {return m_angle;}
	
	using XObjectBasic::setPosition;
	void setPosition(float x,float y);	//����������ʾ��λ��
	void setPositionX(float x);
	void setPositionY(float y);
	XVector2 getPosition() const {return m_setPosition;}
	void setRotateBasePoint(float x,float y);

	XBool setNumber(int temp);								//����������
	XBool setNumber(float temp,int decimalPartLength);		//���ô�С�������
	XBool setNumber(const char *temp);		
	//����һ�κ�����ַ���
	XNumber& operator = (const XNumber& temp);
	XBool setACopy(const XNumber &temp);

	using XObjectBasic::setColor;
	void setColor(float r,float g,float b,float a);	//������ɫ
	XFColor getColor() const {return m_sprite.getColor();}
	void setAlpha(float a);
	float getAlpha() const {return m_alpha;}
	XBool release();

	void setDistance(float distance) {m_distance = distance;}
	float getDistance() const {return m_distance;}
	//Ϊ��ʵ����������������Ӽ����ӿ�
	XBool isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	XNumber();
	~XNumber();

	//virtual void justForTest() {;}
};
#if WITH_INLINE_FILE
#include "XNumber.inl"
#endif
}
#endif
