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
#include "XCommonDefine.h"
namespace XE{
#define ERROR_REPORTER_TXT_FILENAME "ErrorReport.txt"	//���󱨸���ļ���
#define ERROR_REPORTER_PIC_FILENAME "ErrorReport.png"	//���󱨸���ļ���
class XSprite;
class XButton;
class XFontUnicode;
class XEdit;
class XErrorReporter
{
public:
    XErrorReporter();
    virtual ~XErrorReporter();
protected:
    XErrorReporter(const XErrorReporter&);
	XErrorReporter &operator= (const XErrorReporter&);
private:
	bool m_isInited;	//ͼ�λ������Ƿ��ʼ��
	bool m_isBtnChecked;
	bool m_needShow;	//�Ƿ���Ҫ��ʾ���󱨸����
	bool m_isMainThreadProc;		//�Ƿ����߳̽������¼�����

	XSprite *m_screenShot;
	XSprite *m_background;
	XButton *m_button;
	XFontUnicode *m_font;
	XFontUnicode *m_fontDescrip;
	XEdit *m_edit;
	bool reportFile(const std::string &errorDescription,const std::string &filename,int line,const std::string &funName);
	int inputEvent();
	static void ctrlProc(void *,int,int);

	//������Ϊ�˶��̰߳�ȫ�������
	std::string m_errDescription;	//��������
	std::string m_errFilename;		//���������ļ���
	std::string m_errFunname;		//�������ĺ�����
	int m_errLine;					//���������к�
public:
	bool init(XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��ʼ��
	void draw();
	//__DATE__ __TIME__ __FUNCDNAME__ __TIMESTAMP__
	void reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName);
	void errorProc();
	bool isErrored()const{return m_needShow;}
	void setMainProc(){m_isMainThreadProc = true;}
	bool getIsMainProc()const{return m_isMainThreadProc;}
};
}
#endif