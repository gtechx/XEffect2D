#ifndef _JIA_XLINECTRL_
#define _JIA_XLINECTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//����һ������ԭ�ؼ���
#include "XControlBasic.h"

#define XLINE_CTRL_LINE_SUM (10)

enum _XLineCtrlType
{
	LINE_CTRL_TYPE_X_D,
	LINE_CTRL_TYPE_X_U,
	LINE_CTRL_TYPE_Y_R,
	LINE_CTRL_TYPE_Y_L,
};
class _XLineCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;		//�Ƿ��ʼ��
//	_XVector2 m_position;	//�ؼ���λ��
//	_XVector2 m_size;		//��С
	_XVector2 m_pixelSize;	//���ش�С
	_XVector2 m_truePixelSize;	//��ʵ�����سߴ�
	_XFontUnicode m_font;
	char m_textStr[64];		//��ʾ���ַ���

	_XLineCtrlType m_type;
	bool m_withFont;
	bool m_isDown;		//�Ƿ����ʰȡ
	void updateData();

	void (*m_funDataChange)(void *,int ID);
	void *m_pClass;	

	bool m_withRange;	//��Χ�����Ƿ���Ч
	_XVector2 m_range;	//�����ƶ��ķ�Χ
	bool m_withMap;		//�Ƿ����ӳ��
	_XVector2 m_mapRange;	//ӳ��ķ�Χ
	float m_mapValue;

	_XCtrlMode m_ctrlMode;		//�ؼ���ģʽ
	
	bool m_withString;			//�Ƿ����ַ���
	string m_showString;		//��Ҫ��ʾ���ַ���
	_XFontUnicode m_stringFont;	//�ַ���������
public:
	void setString(const char * str);

	void setCtrlMode(_XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float min,float max);
	void disRange(){m_withRange = false;}
	float getValue();
	void setValue(float value);
	void setMapRange(float min,float max);
	void disMapRange(){m_withMap = false;}
public:
	_XBool init(const _XVector2& position,_XLineCtrlType type = LINE_CTRL_TYPE_X_D,const _XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL);

	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int,_XKeyState){return XTrue;}		//do nothing
	_XBool canGetFocus(float x,float y);	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	_XBool canLostFocus(float,float){return XTrue;}	//do nothing

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	void insertChar(const char *,int){;}			//do nothing

	//void setLostFocus() {m_isBeChoose = XFalse;}							//do nothing
	void disable();	
	void enable(){m_isEnable = XTrue;}
	void release();

	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XBool setACopy(const _XLineCtrl & temp);

	//virtual void justForTest() {;}

	_XLineCtrl()
		:m_isInited(XFalse)
	//	,m_position(0.0f,0.0f)
	//	,m_size(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_type(LINE_CTRL_TYPE_X_D)
		,m_range(0.0f,100.0f)
		,m_withRange(false)
		,m_withMap(false)		//�Ƿ����ӳ��
		,m_mapRange(0.0f,100.0f)	//ӳ��ķ�Χ
		,m_mapValue(0.0f)
		,m_funDataChange(NULL)
		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
		,m_withString(false)
	{
		m_ctrlType = CTRL_OBJ_LINECTRL;
	}
	~_XLineCtrl(){release();}
};
#include "XLineCtrl.inl"
#endif