#ifndef _JIA_XERRORREPORTER_
#define _JIA_XERRORREPORTER_
//����һ�����Ի����쳣��������࣬�����������Ĵ���һ�㶼�Ƿǳ����صĴ���
//ϵͳ��鵽��������յ��׳����쳣ʱ����������࣬����ʾ�������ش����Լ������������ڵ��ļ����У�����ʾ�û��˳�����������Ļ��ͼ����־�ļ���
//���ɴ��󱨸棬�����ͼ�λ�����û�г�ʼ����ǰ���£�ֱ�����ɴ��󱨸棬���˳���
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:      2014.3.8
//--------------------------------
//ע�⣺�����Ŀǰ���ڶ��̰߳�ȫ�Ե����⣬��ʹ��ʱ��Ҫ�����ڶ��߳��е��ã���Ҫע����̰߳�ȫ���⡣

#include <string>
#include "XControl/XControls.h"

#define ERROR_REPORTER_TXT_FILENAME ("ErrorReport.txt")	//���󱨸���ļ���
#define ERROR_REPORTER_PIC_FILENAME ("ErrorReport.png")	//���󱨸���ļ���
class _XErrorReporter
{
protected:
    _XErrorReporter()
		:m_isInited(false)
		,m_isBtnChecked(false)
		,m_needShow(false)
		,m_isMainThreadProc(false)
	{}
    _XErrorReporter(const _XErrorReporter&);
	_XErrorReporter &operator= (const _XErrorReporter&);
    virtual ~_XErrorReporter(){}
public:
    static _XErrorReporter& GetInstance()
	{
		static _XErrorReporter m_instance;
		return m_instance;
	}
private:
	bool m_isInited;	//ͼ�λ������Ƿ��ʼ��
	bool m_isBtnChecked;

	_XSprite m_screenShot;
	_XSprite m_background;
	_XButton m_button;
	_XFontUnicode m_font;
	_XFontUnicode m_fontDescrip;
	_XEdit m_edit;
	bool reportFile(const std::string &errorDescription,const std::string &filename,int line,const std::string &funName);
	int inputEvent();
	friend void errorReporterBtnProc(void *pClass,int id);

	//������Ϊ�˶��̰߳�ȫ�������
	bool m_needShow;	//�Ƿ���Ҫ��ʾ���󱨸����
	std::string m_errDescription;	//��������
	std::string m_errFilename;		//���������ļ���
	std::string m_errFunname;		//�������ĺ�����
	int m_errLine;					//���������к�
	bool m_isMainThreadProc;		//�Ƿ����߳̽������¼�����
public:
	bool init(_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��ʼ��
	void draw()
	{
		m_screenShot.draw();
		m_background.draw();
		m_font.draw();
		m_fontDescrip.draw();
	}
	//__DATE__ __TIME__ __FUNCDNAME__ __TIMESTAMP__
	void reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName);
	void errorProc();
	bool isErrored()const{return m_needShow;}
	void setMainProc(){m_isMainThreadProc = true;}
	bool getIsMainProc()const{return m_isMainThreadProc;}
};
inline void reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	_XErrorReporter::GetInstance().reportError(errorDescription,filename,line,funName);
}
#define REPORT_ERROR(p) reportError(p,__FILE__,__LINE__,__FUNCDNAME__)
#endif