#include "XStdHead.h"
#include "XCheatModule.h"
namespace XE{
bool XCheatModule::init()
{
	if(m_isInited) return false;

	m_cheatEdt.initWithoutSkin(512.0f,"");
	m_cheatEdt.setPosition(0.0f,getSceneHeight() - 16.0f);
	m_cheatEdt.setEventProc(ctrlProc,this); 
	m_cheatEdt.disVisible();
	m_cheatEdt.setScale(0.5f);
	m_cheatMltTxt.initWithoutSkin(XVector2(512.0f,1024.0f),"Msg");
	m_cheatMltTxt.setPosition(0.0f,getSceneHeight() - 16.0f - 512.0f);
	m_cheatMltTxt.disVisible();
	m_cheatMltTxt.setScale(0.5f);

	m_visible = false;

	m_isInited = true;
	return true;
}
void XCheatModule::ctrlProc(void*pClass,int id,int eventID)
{
	if(eventID != XEdit::EDT_INPUT_OVER) return;
	XCheatModule &pPar = *(XCheatModule *)pClass;
	if(id != pPar.m_cheatEdt.getControlID()) return;
	//�����������
	pPar.commandProc(pPar.m_cheatEdt.getString());
}
void XCheatModule::commandProc(const char *command)
{
	if(command == NULL || command[0] != '-') 
	{
		if(command == NULL || command[0] == '\0') return;	//���ַ���
		if(command != NULL)
			m_cheatMltTxt.addALine(command);
		m_cheatMltTxt.addALine("������߷Ƿ�������!");
		return;	//�Ƿ�������
	}
	//���濪ʼ��������
	//����������Ĭ�ϵĹ��ò���
	//if(strcmp(command,"-windowWidth") == 0)
	//{//��ֻ��һ��ʾ�������ӣ�����Ĺ��ܿ�����չ
	//	m_cheatMltTxt.addALine(command);
	//	m_cheatMltTxt.addALine(XString::toString(XEE::windowWidth).c_str());
	//	m_cheatEdt.setString("");
	//	return;
	//}
	//���������û��Զ���Ĳ���
	m_cheatMltTxt.addALine(command);
	std::string retStr;
	bool isFlag = false;
	for(int i = 0;i < m_cmdProcs.size();++ i)
	{
		if(m_cmdProcs[i].eventProc != NULL && m_cmdProcs[i].eventProc(m_cmdProcs[i].pClass,command,retStr))
		{
			//m_cheatMltTxt.addALine(retStr.c_str());	
			m_cheatMltTxt.addString(retStr.c_str());	//����֧�ֶ���д��
			//m_cheatEdt.setString("");
			//return;	//������Դ�͸������Ψһ�ĳ��ع�ϵ
			isFlag = true;
		}
	}
	if(isFlag) m_cheatEdt.setString("");
}
XBool XCheatModule::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited) return false;
	switch(keyOrder)
	{
	case XKEY_LCTRL:
	case XKEY_RCTRL:
		if(keyState == KEY_STATE_DOWN)
			m_ctrlBtnDown = true;
		else 
			m_ctrlBtnDown = false;
		break;
	case XKEY_LALT:
	case XKEY_RALT:
		if(keyState == KEY_STATE_DOWN)
			m_altBtnDown = true;
		else 
			m_altBtnDown = false;
		break;
	case XKEY_X:
		if(m_ctrlBtnDown && m_altBtnDown && keyState == KEY_STATE_DOWN)
		{
			m_visible = !m_visible;
			if(m_visible)
			{
				m_cheatEdt.setVisible();
				m_cheatMltTxt.setVisible();
			}else
			{
				m_cheatEdt.disVisible();
				m_cheatMltTxt.disVisible();
			}
		}
		break;
	}
	return XFalse;
}
}