#include "XStdHead.h"
#include "XCalendar.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XCalendar::ctrlProc(void*pClass,int ID,int eventID)
{
	if(eventID != XButton::BTN_MOUSE_DOWN) return;
	XCalendar &pPar = *(XCalendar *)pClass;
	if(ID == pPar.m_yearAddBtn.getControlID())
	{
		++pPar.m_curShowData.year;
		pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_yearDecBtn.getControlID())
	{
		--pPar.m_curShowData.year;
		pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_monthAddBtn.getControlID())
	{
		++pPar.m_curShowData.month;
		if(pPar.m_curShowData.month > 12)
		{
			pPar.m_curShowData.month = 1;
			++pPar.m_curShowData.year;
			pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(XString::toString(pPar.m_curShowData.month).c_str());
		pPar.updateCurDate();
		return;
	}
	if(ID == pPar.m_monthDecBtn.getControlID())
	{
		--pPar.m_curShowData.month;
		if(pPar.m_curShowData.month <= 0)
		{
			pPar.m_curShowData.month = 12;
			--pPar.m_curShowData.year;
			pPar.m_yearTxt.setString(XString::toString(pPar.m_curShowData.year).c_str());
		}
		pPar.m_monthTxt.setString(XString::toString(pPar.m_curShowData.month).c_str());
		pPar.updateCurDate();
		return;
	}
}
XBool XCalendar::initWithoutSkin(const XFontUnicode& font)
{
	if(m_isInited) return XFalse;
	m_position.reset();
	m_scale.set(1.0f);
	m_rect.set(0.0f,0.0f,350.0f,280.0f);
	XTime::getTimeMs(m_todayDate);	//��ȡ���ڵ�ʱ��
	m_curShowData.year = m_todayDate.year;
	m_curShowData.month = m_todayDate.month;
	m_curShowData.day = 1;
	m_yearAddBtn.initWithoutSkin("<<",font,XRect(0.0f,32.0f));
	m_yearAddBtn.setPosition(m_position + XVec2(10.0f,2.0f) * m_scale);
	m_yearAddBtn.setScale(m_scale);
	m_yearAddBtn.setEventProc(ctrlProc,this);
	m_yearAddBtn.setWithAction(XFalse);
	m_yearDecBtn.initWithoutSkin(">>",font,XRect(0.0f,32.0f));
	m_yearDecBtn.setPosition(m_position + XVec2(112.0f,2.0f) * m_scale);
	m_yearDecBtn.setScale(m_scale);
	m_yearDecBtn.setEventProc(ctrlProc,this);
	m_yearDecBtn.setWithAction(XFalse);
	if(!m_yearTxt.setACopy(font)) return XFalse;
	m_yearTxt.setColor(XFColor::black);
	m_yearTxt.setPosition(m_position + XVec2(77.0f,18.0f) * m_scale);
	m_yearTxt.setScale(m_scale);
	m_yearTxt.setString(XString::toString(m_todayDate.year).c_str());
	//����Щ����������������ע����
	XCtrlManager.decreaseAObject(&m_yearAddBtn);	//ע��������
	XCtrlManager.decreaseAObject(&m_yearDecBtn);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_yearAddBtn);
	XObjManager.decreaseAObject(&m_yearDecBtn);
	XObjManager.decreaseAObject(&m_yearTxt);
#endif

	m_monthAddBtn.initWithoutSkin("<<",font,XRect(0.0f,32.0f));
	m_monthAddBtn.setPosition(m_position + XVec2(240.0f,2.0f) * m_scale);
	m_monthAddBtn.setScale(m_scale);
	m_monthAddBtn.setEventProc(ctrlProc,this);
	m_monthAddBtn.setWithAction(XFalse);
	m_monthDecBtn.initWithoutSkin(">>",font,XRect(0.0f,32.0f));
	m_monthDecBtn.setPosition(m_position + XVec2(312.0f,2.0f) * m_scale);
	m_monthDecBtn.setScale(m_scale);
	m_monthDecBtn.setEventProc(ctrlProc,this);
	m_monthDecBtn.setWithAction(XFalse);
	if(!m_monthTxt.setACopy(font)) return XFalse;
	m_monthTxt.setColor(XFColor::black);
	m_monthTxt.setPosition(m_position + XVec2(292.0f,18.0f) * m_scale);
	m_monthTxt.setScale(m_scale);
	m_monthTxt.setString(XString::toString(m_todayDate.month).c_str());
	//����Щ����������������ע����
	XCtrlManager.decreaseAObject(&m_monthAddBtn);	//ע��������
	XCtrlManager.decreaseAObject(&m_monthDecBtn);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_monthAddBtn);
	XObjManager.decreaseAObject(&m_monthDecBtn);
	XObjManager.decreaseAObject(&m_monthTxt);
#endif

	m_curMouseRect.set(m_position + m_rect.getLT() * m_scale,
		m_position + m_rect.getRB() * m_scale);

	std::string tmpTitle[]={"��","һ","��","��","��","��","��"};
	for(int i = 0;i < 7;++ i)
	{
		if(!m_titleFont[i].setACopy(font)) return XFalse;
		if(i == 0) m_titleFont[i].setColor(XFColor::red);
		else m_titleFont[i].setColor(XFColor::black);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_titleFont[i]);
#endif
		m_titleFont[i].setPosition(m_position + XVec2(25.0f + i * 50.0f,53.0f) * m_scale);
		m_titleFont[i].setScale(m_scale);
		m_titleFont[i].setString(tmpTitle[i].c_str());
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode& tmpFont = m_dateFont[i * 7 + j];
			if(!tmpFont.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&tmpFont);
#endif
			tmpFont.setPosition(m_position + XVec2(25.0f + j * 50.0f,88.0f + 35.0f * i) * m_scale);
			tmpFont.setScale(m_scale);
		}
	}
	updateCurDate();

	m_isInited = m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	return XTrue;
}
void XCalendar::updateCurDate()
{
	int week = XTime::getDateWeek(m_curShowData);
	int day = XTime::getDateDaysInYear(m_curShowData) - week;
	XSystemTime tmpT;
	m_needShowToday = false;
	m_needShowChoose = false;
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode& tmpFont = m_dateFont[i * 7 + j];
			if(j == 0) tmpFont.setColor(XFColor::red);
			else tmpFont.setColor(XFColor::black);
			XTime::getDateData(m_curShowData.year,day + i * 7 + j,tmpT,week);
			if(tmpT.month != m_curShowData.month) 
			{
				if(j == 0) tmpFont.setColor(1.0f,0.5f,0.5f,1.0f);
				else tmpFont.setColor(XFColor::gray);
			}
			tmpFont.setString(XString::toString(tmpT.day).c_str());
			if(tmpT.year == m_todayDate.year && tmpT.month == m_todayDate.month
				&& tmpT.day == m_todayDate.day)
			{//������ʾһ����ɫ
				m_needShowToday = true;
				m_todayPos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillRectEx(XVec2(m_curMouseRect.left + 50.0f * j * m_scale.x,
			//		m_curMouseRect.top + (35.0f * i + 70.0f) * m_scale.y),
			//		XVec2(50 * m_scale.x,35 * m_scale.y),0.5f,0.5f,1.0f,true);//,true); 
			}
			if(m_haveChoose && tmpT.year == m_curSetDate.year && 
				tmpT.month == m_curSetDate.month && tmpT.day == m_curSetDate.day)
			{//��ѡ�������һ��
				m_needShowChoose = true;
				m_choosePos.set(50.0f * j,35.0f * i + 70.0f);
			//	drawFillRectEx(XVec2(m_curMouseRect.left + 50.0f * j * m_scale.x,
			//		m_curMouseRect.top + (35.0f * i + 70.0f) * m_scale.y),
			//		XVec2(50 * m_scale.x,35 * m_scale.y),0.5f,0.5f,0.5f,true);//,true); 
			}

		}
	}
}
void XCalendar::setPosition(const XVec2& p)
{
	m_position = p;
	m_yearAddBtn.setPosition(m_position + XVec2(10.0f,2.0f) * m_scale);
	m_yearDecBtn.setPosition(m_position + XVec2(112.0f,2.0f) * m_scale);
	m_yearTxt.setPosition(m_position + XVec2(77.0f,18.0f) * m_scale);
	m_monthAddBtn.setPosition(m_position + XVec2(240.0f,2.0f) * m_scale);
	m_monthDecBtn.setPosition(m_position + XVec2(312.0f,2.0f) * m_scale);
	m_monthTxt.setPosition(m_position + XVec2(292.0f,18.0f) * m_scale);
	m_curMouseRect.set(m_position + m_rect.getLT() * m_scale,
		m_position + m_rect.getRB() * m_scale);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + XVec2(25.0f + i * 50.0f,53.0f) * m_scale);
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode& tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + XVec2(25.0f + j * 50.0f,88.0f + 35.0f * i) * m_scale);
		}
	}
}
void XCalendar::setScale(const XVec2& s)
{
	if(s.x < 0.0f || s.y < 0.0f) return;
	if(s.x == m_scale.x && s.y == m_scale.y) return;
	m_scale = s;
	m_yearAddBtn.setPosition(m_position + XVec2(10.0f,2.0f) * m_scale);
	m_yearAddBtn.setScale(m_scale);
	m_yearDecBtn.setPosition(m_position + XVec2(112.0f,2.0f) * m_scale);
	m_yearDecBtn.setScale(m_scale);
	m_yearTxt.setPosition(m_position + XVec2(77.0f,18.0f) * m_scale);
	m_yearTxt.setScale(m_scale);
	m_monthAddBtn.setPosition(m_position+ XVec2(240.0f,2.0f) * m_scale);
	m_monthAddBtn.setScale(m_scale);
	m_monthDecBtn.setPosition(m_position + XVec2(312.0f,2.0f) * m_scale);
	m_monthDecBtn.setScale(m_scale);
	m_monthTxt.setPosition(m_position + XVec2(292.0f,18.0f) * m_scale);
	m_monthTxt.setScale(m_scale);
//	m_titleTxt.setSize(m_scale);
	m_curMouseRect.set(m_position + m_rect.getLT() * m_scale,
		m_position + m_rect.getRB() * m_scale);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setPosition(m_position + XVec2(25.0f + i * 50.0f,53.0f) * m_scale);
		m_titleFont[i].setScale(m_scale);
	}
	for(int i = 0;i < 6;++ i)
	{
		for(int j = 0;j < 7;++ j)
		{
			XFontUnicode& tmpFont = m_dateFont[i * 7 + j];
			tmpFont.setPosition(m_position + XVec2(25.0f + j * 50.0f,88.0f + 35.0f * i) * m_scale);
			tmpFont.setScale(m_scale);
		}
	}
}
void XCalendar::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	static const std::string tmpTitle[]={"��","һ","��","��","��","��","��"};
	//��ʾ����
	XRender::drawFillRectExA(m_curMouseRect.getLT(),m_curMouseRect.getSize(),
		XCCS::normalColor * m_color,true); 
	//��ʾѡ��
	m_yearTxt.draw();
	m_monthTxt.draw();
	//��ʾ����
	for(int i = 0;i < 7;++ i)
		m_titleFont[i].draw();
	//��ʾ����
	if(m_needShowToday)
	{
		XRender::drawFillRectExA(m_curMouseRect.getLT() + m_todayPos * m_scale,
			XVec2(50,35) * m_scale,XCCS::downColor * m_color,true);//,true); 
	}
	if(m_needShowChoose)
	{
		XRender::drawFillRectExA(m_curMouseRect.getLT() + m_choosePos * m_scale,
			XVec2(50,35) * m_scale,XCCS::downColor * m_color,true);//,true); 
	}
	//��ʾ����
	for(int i = 0;i < 42;++ i)
		m_dateFont[i].draw();
	m_yearAddBtn.draw();
	m_yearDecBtn.draw();
	m_monthAddBtn.draw();
	m_monthDecBtn.draw();
}
XBool XCalendar::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_isSilent) return XFalse;
	XRect mouseRect;
	mouseRect.set(m_position + XVec2(m_rect.left,m_rect.top + 70.0f) * m_scale,
		m_position + m_rect.getRB() * m_scale);
	m_yearAddBtn.mouseProc(p,mouseState);
	m_yearDecBtn.mouseProc(p,mouseState);
	m_monthAddBtn.mouseProc(p,mouseState);
	m_monthDecBtn.mouseProc(p,mouseState);

	if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
		mouseRect.isInRect(p))
	{
		m_haveChoose = true;
		int dayX = (p.x - mouseRect.left) / (50.0f * m_scale.x);
		int dayY = (p.y - mouseRect.top) / (35.0f * m_scale.y);
		dayY = XTime::getDateDaysInYear(m_curShowData) - XTime::getDateWeek(m_curShowData) + dayY * 7 + dayX;
		XTime::getDateData(m_curShowData.year,dayY,m_curSetDate,dayX);
		updateCurDate();
		return XTrue;
	}
	return XFalse;
}
void XCalendar::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	m_isInited = false;
}
void XCalendar::setAlpha(float a)
{
	m_color.setA(a);
	m_yearAddBtn.setAlpha(a);
	m_yearDecBtn.setAlpha(a);
	m_yearTxt.setAlpha(a);
	m_monthAddBtn.setAlpha(a);
	m_monthDecBtn.setAlpha(a);
	m_monthTxt.setAlpha(a);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setAlpha(a);
	}
	for(int i = 0;i < 42;++ i)
	{
		m_dateFont[i].setAlpha(a);
	}
}
void XCalendar::setColor(const XFColor& c)
{
	m_color = c;
	m_yearAddBtn.setColor(m_color);
	m_yearDecBtn.setColor(m_color);
	m_yearTxt.setColor(m_color);
	m_monthAddBtn.setColor(m_color);
	m_monthDecBtn.setColor(m_color);
	m_monthTxt.setColor(m_color);
	for(int i = 0;i < 7;++ i)
	{
		m_titleFont[i].setColor(m_color);
	}
	for(int i = 0;i < 42;++ i)
	{
		m_dateFont[i].setColor(m_color);
	}
}
#if !WITH_INLINE_FILE
#include "XCalendar.inl"
#endif
}