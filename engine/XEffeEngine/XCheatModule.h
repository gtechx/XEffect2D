#ifndef _JIA_XCHEATMODULE_
#define _JIA_XCHEATMODULE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.2
//--------------------------------
//����һ�������������࣬���ڴ���һЩԤ���趨�õ�ϵͳ����
//#include "XOSDefine.h"
//#include "XControl\XEdit.h"
#include "XControl\XMultiText.h"
//Ctrl + Alt + x:��ʾ�����������׿�
namespace XE{
struct XCheatProc
{
	bool (* eventProc)(void *,const char *,std::string&);
	void *pClass;
	XCheatProc()
		:pClass(NULL)
		,eventProc(NULL)
	{}
};
class XCheatModule
{
private:
	bool m_isInited;
	XEdit m_cheatEdt;
	XMultiText m_cheatMltTxt;
	bool m_visible;
	bool m_ctrlBtnDown;	//�Ƿ�ctrl��������
	bool m_altBtnDown;	//�Ƿ�alt��������

	std::vector<XCheatProc> m_cmdProcs;	//���ڴ�������Ļص�����
public:
	bool init();
	bool addCmdProc(bool (* eventProc)(void *,const char *,std::string&),void *pClass)
	{
		XCheatProc tmp;
		tmp.eventProc = eventProc;
		tmp.pClass = pClass;
		m_cmdProcs.push_back(tmp);
		return true;
	}
	static void ctrlProc(void*,int,int);
	void commandProc(const char *command);	//����������Ĵ�����
	XBool keyboardProc(int keyOrder,XKeyState keyState);	//������ӦһЩ����İ�����Ϣ

	//����ʹ��ע��ص������ķ�ʽ��֧�ֽ��й�����չ

	XCheatModule()
		:m_isInited(false)
		,m_visible(false)
		,m_ctrlBtnDown(false)
		,m_altBtnDown(false)
	{}
};
}
#endif