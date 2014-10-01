#ifndef _JIA_XCALENDAR_
#define _JIA_XCALENDAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.30
//--------------------------------
#include "XButton.h"
#include "XTimer.h"
//����һ������ѡ�����
//ע��Ŀǰ�����Ĵ��������׸����Ҫ�����Ż�
//Ŀǰ����ռ�û����ƶ�����
class _XCalendar:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XRect m_rect;			//��Χ

	_XSystemTime m_todayDate;		//���ڵ�����
	_XSystemTime m_nowSetDate;		//��ǰ���õ�ʱ��
	_XSystemTime m_nowShowData;		//��ǰ��ʾ����ʼʱ��
	bool m_haveChoose;

	_XButton m_yearAddBtn;	//��ӵ�button
	_XButton m_yearDecBtn;
	_XFontUnicode m_yearTxt;
	_XButton m_monthAddBtn;	//��ӵ�button
	_XButton m_monthDecBtn;
	_XFontUnicode m_monthTxt;
	//_XFontUnicode m_titleTxt;
	_XFontUnicode m_titleFont[7];
	_XFontUnicode m_dateFont[42];
	friend void calendarCtrProc(void*,int);
	bool m_needShowToday;	//�Ƿ���Ҫ��ʾ����
	_XVector2 m_todayPos;	//�����λ��
	bool m_needShowChoose;	//�Ƿ���Ҫ��ʾѡ�������
	_XVector2 m_choosePos;	//ѡ�����ڵ�λ��
public:
	_XBool initWithoutTex(const _XFontUnicode &font);
	_XBool initWithoutTex(){return initWithoutTex(XEE::systemFont);}
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//������һЩ���ýӿ�
	_XSystemTime getTodayDate() const {return m_todayDate;}
	void setTodayDate(const _XSystemTime &t) {m_todayDate = t;}
	bool getHaveChoose() const{return m_haveChoose;}
	_XSystemTime getNowChooseDate() const {return m_nowSetDate;}
	void setNowChooseDate(const _XSystemTime &t) {m_nowSetDate = t;}
	_XSystemTime getNowShowDate() const {return m_nowShowData;}
	void setNowShowDate(const _XSystemTime &t) {m_nowShowData = t;}

protected:
	void updateNowDate();
	void draw();								//��水ť
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	_XCalendar()
		:m_isInited(XFalse)
		,m_haveChoose(false)
		,m_needShowToday(false)
		,m_needShowChoose(false)
	{
		m_ctrlType = CTRL_OBJ_CALENDAR;
	}
	~_XCalendar(){release();}
	void release();

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XCalendar(const _XButton &temp);
	_XCalendar& operator = (const _XButton& temp);

};
#include "XCalendar.inl"
#endif