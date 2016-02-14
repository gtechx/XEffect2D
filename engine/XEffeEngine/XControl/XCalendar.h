#ifndef _JIA_XCALENDAR_
#define _JIA_XCALENDAR_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.30
//--------------------------------
#include "XButton.h"
#include "XTimer.h"
#include "../XXml.h"
namespace XE{
//����һ������ѡ�����
//ע��Ŀǰ�����Ĵ��������׸����Ҫ�����Ż�
//Ŀǰ����ռ�û����ƶ�����
class XCalendar:public XControlBasic
{
private:
	XBool m_isInited;
	bool m_haveChoose;
	bool m_needShowToday;	//�Ƿ���Ҫ��ʾ����
	bool m_needShowChoose;	//�Ƿ���Ҫ��ʾѡ�������
	XVector2 m_todayPos;	//�����λ��
	XVector2 m_choosePos;	//ѡ�����ڵ�λ��
	XRect m_rect;			//��Χ

	XSystemTime m_todayDate;		//���ڵ�����
	XSystemTime m_curSetDate;		//��ǰ���õ�ʱ��
	XSystemTime m_curShowData;		//��ǰ��ʾ����ʼʱ��

	XButton m_yearAddBtn;	//��ӵ�button
	XButton m_yearDecBtn;
	XFontUnicode m_yearTxt;
	XButton m_monthAddBtn;	//��ӵ�button
	XButton m_monthDecBtn;
	XFontUnicode m_monthTxt;
	//XFontUnicode m_titleTxt;
	XFontUnicode m_titleFont[7];
	XFontUnicode m_dateFont[42];
	static void ctrlProc(void*,int,int);
public:
	XBool initWithoutSkin(const XFontUnicode &font);
	XBool initWithoutSkin(){return initWithoutSkin(getDefaultFont());}
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);
	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(float x,float y);

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//������һЩ���ýӿ�
	XSystemTime getTodayDate() const {return m_todayDate;}
	void setTodayDate(const XSystemTime &t) {m_todayDate = t;}
	bool getHaveChoose() const{return m_haveChoose;}
	XSystemTime getCurChooseDate() const {return m_curSetDate;}
	void setCurChooseDate(const XSystemTime &t) {m_curSetDate = t;}
	XSystemTime getCurShowDate() const {return m_curShowData;}
	void setCurShowDate(const XSystemTime &t) {m_curShowData = t;}

protected:
	void updateCurDate();
	void draw();								//��水ť
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool keyboardProc(int,XKeyState){return XFalse;}			//�����Ƿ񴥷���������
	void insertChar(const char *,int){;}
	XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	XCalendar()
		:m_isInited(XFalse)
		,m_haveChoose(false)
		,m_needShowToday(false)
		,m_needShowChoose(false)
	{
		m_ctrlType = CTRL_OBJ_CALENDAR;
	}
	~XCalendar(){release();}
	void release();

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XCalendar(const XButton &temp);
	XCalendar& operator = (const XButton& temp);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����ʵ�ֽ���ؼ�������״̬�ı���(���)
public:
	virtual XBool saveState(TiXmlNode &e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		XSystemTime tmp = getCurChooseDate();
		if(!XXml::addLeafNode(e,(m_ctrlName + "Year").c_str(),XString::toString(tmp.year))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Month").c_str(),XString::toString(tmp.month))) return XFalse; 
		if(!XXml::addLeafNode(e,(m_ctrlName + "Day").c_str(),XString::toString(tmp.day))) return XFalse; 
		return XTrue;
	}
	virtual XBool loadState(TiXmlNode *e)
	{
		if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
		XSystemTime tmp;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Year").c_str(),tmp.year) == NULL) return XFalse;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Month").c_str(),tmp.month) == NULL) return XFalse;
		if(XXml::getXmlAsInt(e,(m_ctrlName + "Day").c_str(),tmp.day) == NULL) return XFalse;
		setCurChooseDate(tmp);
		return XTrue;
	}
	//---------------------------------------------------------
};
#if WITH_INLINE_FILE
#include "XCalendar.inl"
#endif
}
#endif