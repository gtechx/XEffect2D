#ifndef _JIA_XMUTITEXT_
#define _JIA_XMUTITEXT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ��������ʾ�����ı����࣬�����мƻ�����༭���ܣ�����̫���Ӿ�û���������ĳ�����

#include "XEdit.h"
#include "XSlider.h"

typedef _XEditTexture _XMultiTextTexture;

//���е��ַ���
struct _XMultiTextLineString
{
	char isEnable;				//��һ���Ƿ�Ҫ��ʾ���Ƿ���Ч
	char *showString;			//��һ����������Ҫ��ʾ���ַ���
	int showStart;				//�����ַ�����ʾ���ֵ�ͷ�ڵ�ǰ�е�ƫ��
	int showLength;				//��ʾ�ַ����ĳ���
	_XFontUnicode nowText;		//������ʾ��һ���ַ���������
	int lineHeadorder;			//��һ���ַ���ȫ���е�ͷλ��
	int lineEndOrder;			//��һ���ַ���ȫ���е�βλ��
	int lineLength;				//��һ���ַ����ĳ���
	char isEnterEnd;			//�Ƿ���Ϊ���ж�������ʾ1����������Ϊ��ʾ�ַ����ĳ������ƶ�����0
	_XSprite spriteSelect;		//�ַ�����ʾѡ�񱳾���ɫ�ľ���
	char haveAllSelect;			//�������Ƿ�汻����ѡ��
	_XMultiTextLineString()
		:showString(NULL)
	{
	}
};

#define MAX_LINE_STRING_LENGTH 256	//���п�����ʾ���ַ�������󳤶�
#define MAX_LINE_SUM 256				//����������������ַ���
#define MAX_STRING_LENGTH 65536		//�������������ַ����ĳ���

class _XMultiText:public _XControlBasic
{
private:
	char m_isInited;				//�Ƿ��ʼ��

	char m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	char m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������

	char *m_nowString;				//��ǰ������ַ���������

	const _XTextureData *m_mutiEditNormal;	//�������ͨ״̬
	const _XTextureData *m_mutiEditDisable;	//�������Ч״̬
	const _XTextureData *m_mutiEditSelect;	//�����Ƭѡ��ɫ
	const _XTextureData *m_mutiEditUpon;	//�����Ƭѡ��ɫ

	_XSprite m_spriteBackGround;	//������ʾ�����ı�����ͼ
	//_XVector2 m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_textWidth;	//����Ŀ���
	float m_textHeight;	//����ĸ߶�

	char m_haveSelect;				//�Ƿ����ַ�������ѡ��
	int m_nowSelectLineOrder;		//��ǰ�ĵ��б�ѡ�������һ��

	_XMultiTextLineString *m_lineStr;
	int m_nowStartLine;			//��ǰ��ʾ�ĵ�һ�����ܵĵڼ���
	int m_nowShowLineSum;		//��ǰ������ʾ������
	int m_nowShowLineWidth;		//��ǰһ�п�����ʾ�Ŀ���
	int m_nowMaxLineSum;		//��ǰ�ַ�����ʵ�ʵ�����
	int m_nowMaxLineLength;		//��ǰ�ַ�����ʵ�ʵ��п�
	void updateLineStr();		//���µ�ǰ������ı���ÿһ����

//	void (*m_funInputChenge)(void *,int);		//�������ݷ����ı��ʱ�����
//	void (*m_funInputOver)(void *,int);		//ȷ���������֮���ʱ�����
//	void *m_pClass;

	char m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ

	friend void funMutiTextValueChangeV(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiTextValueChangeH(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveH(void *pClass,int objectID);

	void setStartLine(int temp);	//���ô�����ʾ��ͷ�е��к�
	void checkStr();							//����ַ����ĵ��п����Ƿ��������,���������Ҫ�����ڽ��ַ�������Ĭ�ϵĿ��Ȳ��Ƴ���Ҫ����ʽ,�Ա����Ժ���ַ�������

public:
	int init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,	//�ؼ��ķ�Χ
		const _XMultiTextTexture& tex,	//�ؼ�����ͼ
		const char *str,const _XFontUnicode& font, float strSize,		//�ؼ������弰�����Ϣ
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider);	//�ؼ����������������ؼ�
protected:
	void draw();
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState);							//�����Ƿ������������
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	void setPosition(const _XVector2& position);	//�ı�ռ��λ��
	void setPosition(float x,float y);	//�ı�ռ��λ��
	_XVector2 getPosition() const {return m_position;}
	void setSize(const _XVector2& size);			//�ı�ؼ��ĳߴ�
	void setSize(float x,float y);		//�ı�ؼ��ĳߴ�
	_XVector2 getSize() const {return m_size;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor);
		}
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor * m_color);
		}
		m_verticalSlider.setColor(m_color);
		m_horizontalSlider.setColor(m_color);
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ

	void setString(const char *str);			//�ı���ʾ���ַ���
	void addString(const char *str);			//���ַ����������ַ���
	void addALine(const char *str);			//���ַ����������ַ���

	int setACopy(const _XMultiText &temp);
	
	int exportData(const char *fileName = NULL);		//���ݵ���
	int importData(const char *fileName = NULL);		//���ݵ���
	int deleteSelectLine();						//ɾ����ǰѡ���һ��
	int moveUpSelectLine();						//��ѡ���һ������
	int moveUpLine(int order);					//��ĳһ������
	int moveDownSelectLine();					//��ѡ���һ������
	int moveDownLine(int order);				//��ĳһ������
	int insertALine(const char *str,int lineOrder);	//���ı���ָ���в���һ��

	_XMultiText();
	~_XMultiText();
	//��������������
	void enable();		//ʹ����Ч
	void disable();		//ʹ����Ч
	void release();
	int getSelectLine() const;			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
	//Ϊ��֧����������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XMultiText(const _XMultiText &temp);
	_XMultiText& operator = (const _XMultiText& temp);
};

inline void _XMultiText::enable()		//ʹ����Ч
{
	m_isEnable = 1;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}

inline void _XMultiText::disable()		//ʹ����Ч
{
	m_isEnable = 0;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}

inline int _XMultiText::getSelectLine() const			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
{
	if(m_isInited == 0) return -1;
	if(m_haveSelect == 0) return -1;
	else return m_nowSelectLineOrder;
}

inline void _XMultiText::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XMultiText::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}
#endif