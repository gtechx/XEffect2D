#include "GGame.h"
#include "XFile.h"
#include "systemHook.h"
#pragma comment(lib, "systemHook.lib")
using namespace XE;
void GGame::ctrlEventProc(int id,int eventID)
{
	if(eventID == XMultiList::MLTLST_DCLICK)
	{
		if(id == m_dataLst.getControlID())
		{//���˫���¼�
			sendButtonDown();
			return;
		}
		if(id == m_dataKMLst.getControlID())
		{//���˫���¼�������ָ������
			sendSelectData();
			return;
		}
	}
	if(id == m_openBtn.getControlID())
	{//�򿪷���
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!XString::getIsUnsignedInt(m_textPortEdt.getString()))
		{
			setMassage("�˿ںű���Ϊ������.");
			return;
		}
		if(!XString::getIsValidIp(m_textIPEdt.getString())
			&& !XString::getIsLocalhost(m_textIPEdt.getString()))
		{
			setMassage("�Ƿ���IP��ַ!");
			return;
		}
		if(!m_isSenderOpen)
		{
			try
			{
				m_sender.setup(m_textIPEdt.getString(),atoi(m_textPortEdt.getString()));
			}catch(...)
			{
				if(getLanguageIndex() == INDEX_ENGLISH)
					setMassage("OSC open fail,Please check whether the port has been occupied.");
				else setMassage("osc��ʧ��,����osc�˿��Ƿ��Ѿ���ռ��.");
				return;
			}
#ifdef WITH_OFXOSC
			m_sender.setSendAsMessage(true);
#endif
			m_isSenderOpen = true;
			if(getLanguageIndex() == INDEX_ENGLISH)
				setMassage("Opened,determine whether successful by yourself.");
			else setMassage("�������,�Ƿ�ɹ����Լ�ȷ��.");
		//	pPar.m_openBtn.disable();
		//	pPar.m_closeBtn.enable();
			if(getLanguageIndex() == 0) m_openBtn.setCaptionText("Close");
			else m_openBtn.setCaptionText("�ر�");
			m_textIPEdt.disable();
			m_textPortEdt.disable();
			if(m_keyMapOscList.size() > 0) startHook();//����hook
			m_mouseX = -1;
			m_mouseY = -1;
		}else
		{
#ifdef WITH_OFXOSC
			m_sender.setup("",0);
#else
			m_sender.release();
#endif
			m_isSenderOpen = false;
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Chosed.");
			else setMassage("�ر����.");
		//	pPar.m_openBtn.enable();
		//	pPar.m_closeBtn.disable();
			if(getLanguageIndex() == INDEX_ENGLISH) m_openBtn.setCaptionText("Open");
			else m_openBtn.setCaptionText("��");
			m_textIPEdt.enable();
			m_textPortEdt.enable();
			if(m_keyMapOscList.size() > 0) stopHook();//�ر�hook
		}
		return;
	}
	if(id == m_addBtn.getControlID())
	{//���
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!checkNowData())
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is not correct type.");
			else setMassage("����������ʵ�ʵ����ݲ���.");
			return;
		}
		//�������
		GOscData *tmp = XMem::createMem<GOscData>();
		tmp->data = m_textDataEdt.getString();
		tmp->address = m_textAddressEdt.getString();
		tmp->dataType = m_textDataCmb.getCurChooseOrder();
		m_datasVector.push_back(tmp);
		//�����б�
		addALineData(tmp);
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data added.");
		else setMassage("������ӳɹ�.");
		return;
	}
	if(id == m_delBtn.getControlID())
	{//ɾ��
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		//ɾ������
		int lineSum = m_dataLst.getSelectIndex();
		if(lineSum >= 0)
		{
			GOscData *tmp = m_datasVector[lineSum];
			for(int i = lineSum;i < (int)(m_datasVector.size()) - 1;++ i)
			{
				m_datasVector[i] = m_datasVector[i + 1];
			}
			m_datasVector.pop_back();
			XMem::XDELETE(tmp);
			m_dataLst.deleteLine(lineSum);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data deleted.");
			else setMassage("ɾ���ɹ�.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("please choose the data to delete.");
			else setMassage("��ѡ��Ҫɾ��������.");
		}
		return;
	}
	if(id == m_sendBtn.getControlID())
	{//����
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_isSendState) sendOverProc();
		else sendButtonDown();
		return;
	}
	if(id == m_openRBtn.getControlID())
	{//��������
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_recvState)
		{
			m_recvState = false;
#ifdef WITH_OFXOSC
			m_receiver.setup(0);
#else
			m_receiver.release();
#endif
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Closed receiver.");
			else setMassage("�ѹرս���.");
		//	m_openRBtn.enable();
		//	m_closeRBtn.disable();
			m_openRBtn.setCaptionText(m_language.m_openRBtnCloseStr.c_str());
			m_textRPortEdt.enable();
			m_supportACKMode.enable();
		}else
		{
			try
			{
				m_receiver.setup(m_textRPortEdt.getAsInt());
			}catch(...)
			{
				if(getLanguageIndex() == INDEX_ENGLISH) setMassage("OSC open fail,����osc�˿��Ƿ��Ѿ���ռ��.");
				else setMassage("osc���մ�ʧ��,����osc�˿��Ƿ��Ѿ���ռ��.");
				return;
			}
			m_recvState = true;
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Receiver opened,�Ƿ�ɹ��������ж�.");
			else setMassage("�Ѿ���������,�Ƿ�ɹ��������ж�.");
		//	m_openRBtn.disable();
		//	m_closeRBtn.enable();
			m_openRBtn.setCaptionText(m_language.m_openRBtnOpenStr.c_str());
			m_textRPortEdt.disable();
			m_supportACKMode.disable();
		}
		return;
	}
	if(id == m_clearRecvBtn.getControlID())
	{
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		m_recvText.setString("Recv:");
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Received data have cleared.");
		else setMassage("���տ������Ѿ����.");
		return;
	}
	if(id == m_saveBtn.getControlID())
	{//����
		if(eventID != XButton::BTN_MOUSE_UP) return;
		if(getLanguageIndex() == INDEX_ENGLISH) 
		{
			if(!saveToFile(XFile::getChooseFilename(CFG_FILENAME,"Save",false).c_str()))
				setMassage("Save failed.");
		}else
		{
			if(!saveToFile(XFile::getChooseFilename(CFG_FILENAME,"����",false).c_str()))
				setMassage("д�������ļ�ʧ��.");
		}
		return;
	}
	if(id == m_readBtn.getControlID()) 
	{//��ȡ
		if(eventID != XButton::BTN_MOUSE_UP) return;
		if(m_recvState || m_isSendState)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("It is forbidden to read when sender open or receiver open.");
			else setMassage("���ڽ��ܻ��߷���״̬���ܶ�ȡ�����ļ�.");
			return;
		}
		if(getLanguageIndex() == INDEX_ENGLISH) 
		{
			if(!readFromFile(XFile::getChooseFilename(CFG_FILENAME,"Read",true).c_str()))
				setMassage("Read file failed.");
		}else
		{
			if(!readFromFile(XFile::getChooseFilename(CFG_FILENAME,"��ȡ",true).c_str()))
				setMassage("��ȡ�����ļ�ʧ��.");
		}
		return;
	}
	if(id == m_dataLst.getControlID())
	{//ѡ���������
		if(eventID != XMultiList::MLTLST_SELECT) return;
		int index = m_dataLst.getSelectIndex();
		if(index >= 0)
		{
			m_textDataEdt.setString(m_datasVector[index]->data.c_str());
			m_textDataCmb.setCurChooseOrder(m_datasVector[index]->dataType);
			m_textAddressEdt.setString(m_datasVector[index]->address.c_str());
		}
		return;
	}
	if(id == m_modifyBtn.getControlID())
	{//�޸�
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		int index = m_dataLst.getSelectIndex();
		if(index >= 0 && checkNowData())
		{
			m_datasVector[index]->data = m_textDataEdt.getString();
			m_datasVector[index]->dataType = m_textDataCmb.getCurChooseOrder();
			m_datasVector[index]->address = m_textAddressEdt.getString();
			setLineData(m_datasVector[index],index);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Modifayed.");
			else setMassage("�޸����.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("please choose the data to modify.");
			else setMassage("��ѡ����Ҫ�޸ĵ�����.");
		}
		return;
	}
	if(id == m_clearBtn.getControlID())
	{//���
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_datasVector.size() <= 0)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("There is no data to clear.");
			else setMassage("�б���û������.");
			return;
		}
		m_dataLst.setLineSum(0);
		for(int i = 0;i < m_datasVector.size();++ i)
		{
			XMem::XDELETE(m_datasVector[i]);
		}
		m_datasVector.clear();
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Cleared.");
		else setMassage("������.");
		return;
	}
	if(id == m_addKMBtn.getControlID())
	{//���
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(!checkKMData())
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Dta is invaid or already exist.");
			else setMassage("���ݲ��Ϸ������Ѿ�����.");
			return;
		}
		//�������
		GKMOscData *tmp = XMem::createMem<GKMOscData>();
		tmp->keyValue = m_keySelectCmb.getCurChooseOrder();
		tmp->keyState = m_keyStateCmb.getCurChooseOrder();
		if(tmp->keyValue == 39 || tmp->keyValue == 40)
		{
			tmp->data = "0 - 255";
			tmp->address = m_oscAddressEdt.getString();
			tmp->dataType = 0;
		}else
		{
			tmp->data = m_oscDataEdt.getString();
			tmp->address = m_oscAddressEdt.getString();
			tmp->dataType = m_oscDataCmb.getCurChooseOrder();
		}
		m_keyMapOscList.push_back(tmp);
		//�����б�
		addALineData(tmp);
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data added.");
		else setMassage("������ӳɹ�.");
		return;
	}
	if(id == m_delKMBtn.getControlID())
	{//ɾ��
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		//ɾ������
		int lineSum = m_dataKMLst.getSelectIndex();
		if(lineSum >= 0)
		{
			GKMOscData *tmp = m_keyMapOscList[lineSum];
			for(int i = lineSum;i < (int)(m_keyMapOscList.size()) - 1;++ i)
			{
				m_keyMapOscList[i] = m_keyMapOscList[i + 1];
			}
			m_keyMapOscList.pop_back();
			XMem::XDELETE(tmp);
			m_dataKMLst.deleteLine(lineSum);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Deleted.");
			else setMassage("ɾ���ɹ�.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose the data to clear.");
			else setMassage("��ѡ��Ҫɾ��������.");
		}
		return;
	}
	if(id == m_clearKMBtn.getControlID())
	{//���
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		if(m_keyMapOscList.size() <= 0)
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("There is no data to clear.");
			else setMassage("�б���û������.");
			return;
		}
		m_dataKMLst.setLineSum(0);
		for(int i = 0;i < m_keyMapOscList.size();++ i)
		{
			XMem::XDELETE(m_keyMapOscList[i]);
		}
		m_keyMapOscList.clear();
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data cleared.");
		else setMassage("������.");
		return;
	}
	if(id == m_modifyKMBtn.getControlID())
	{//�޸�
		if(eventID != XButton::BTN_MOUSE_DOWN) return;
		int index = m_dataKMLst.getSelectIndex();
		if(index >= 0 && checkKMData(index))
		{
			m_keyMapOscList[index]->keyValue = m_keySelectCmb.getCurChooseOrder();
			m_keyMapOscList[index]->keyState = m_keyStateCmb.getCurChooseOrder();
			m_keyMapOscList[index]->data = m_oscDataEdt.getString();
			m_keyMapOscList[index]->dataType = m_oscDataCmb.getCurChooseOrder();
			m_keyMapOscList[index]->address = m_oscAddressEdt.getString();
			setLineData(m_keyMapOscList[index],index);
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data modifyed.");
			else setMassage("�޸����.");
		}else
		{
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose the data to modify.");
			else setMassage("��ѡ����Ҫ�޸ĵ�����.");
		}
		return;
	}
	if(id == m_dataKMLst.getControlID())
	{//ѡ���������
		if(eventID != XMultiList::MLTLST_SELECT) return;
		int index = m_dataKMLst.getSelectIndex();
		if(index >= 0)
		{
			m_keySelectCmb.setCurChooseOrder(m_keyMapOscList[index]->keyValue);
			m_keyStateCmb.setCurChooseOrder(m_keyMapOscList[index]->keyState);
			m_oscDataEdt.setString(m_keyMapOscList[index]->data.c_str());
			m_oscDataCmb.setCurChooseOrder(m_keyMapOscList[index]->dataType);
			m_oscAddressEdt.setString(m_keyMapOscList[index]->address.c_str());
		}
		return;
	}
	if(id == m_languageCmb.getControlID())
	{
		if(eventID == XCombo::CMB_DATA_CHANGE) updateLanguage();
		return;
	}
}
XBool GGame::checkNowData()
{
	switch(m_textDataCmb.getCurChooseOrder())
	{
	case 0:	//int32
	case 1:	//int64
	case 2:	//float
		if(!XString::getIsNumber(m_textDataEdt.getString())) return XFalse;
		break;
	case 3:	//string
		break;
	case 4:	//blob	//��δ֧��
		break;
	}
	return XTrue;
}
XBool GGame::checkKMData(int index)
{
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		if(m_keyMapOscList[i]->keyValue == m_keySelectCmb.getCurChooseOrder() &&
			m_keyMapOscList[i]->keyState == m_keyStateCmb.getCurChooseOrder() && 
			i != index) return XFalse;	//��ͬ���趨���ܴ���
	}
	if(m_keySelectCmb.getCurChooseOrder() != 39 &&
		m_keySelectCmb.getCurChooseOrder() != 40)
	{
		switch(m_oscDataCmb.getCurChooseOrder())
		{
		case 0:	//int32
		case 1:	//int64
		case 2:	//float
			if(!XString::getIsNumber(m_oscDataEdt.getString())) return XFalse;
			break;
		case 3:	//string
			break;
		case 4:	//blob	//��δ֧��
			break;
		}
	}
	return XTrue;
}
void GGame::sendOverProc()
{
	m_textTimesEdt.setString(XString::toString(m_setTimes).c_str());
	m_isSendState = false;
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Stoped.");
	else setMassage("��ֹͣ.");
	//���ｫ��صĿؼ�ʹ��(��δ���)
	//m_textIPEdt.enable();
	//m_textPortEdt.enable();
	m_openBtn.enable();
//	m_closeBtn.enable();
	m_addBtn.enable();
	m_delBtn.enable();
	m_clearBtn.enable();
	m_modifyBtn.enable();
	m_textDataEdt.enable();
	m_textDataCmb.enable();
	m_textAddressEdt.enable();
	m_textTimesEdt.enable();
	m_textSpacingEdt.enable();

	m_addKMBtn.enable();
	m_delKMBtn.enable();
	m_clearKMBtn.enable();
	m_modifyKMBtn.enable();
	m_keySelectCmb.enable();
	m_keyStateCmb.enable();
	m_oscDataEdt.enable();
	m_oscDataCmb.enable();
	m_oscAddressEdt.enable();

	m_sendBtn.enable();
	m_sendBtn.setCaptionText(m_language.m_sendBtnOpenStr.c_str());
//	m_stopBtn.enable();	//�����ǳ��ϸ���߼�
	m_sendAsMsg.enable();
	m_sendModeCmb.enable();
}
void GGame::sendSelectData()
{
	if(!m_isSenderOpen)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please open sender at first.");
		else setMassage("������δ��,���ȴ�����.");
		return;
	}
	//���淢��ѡ������
	int index = m_dataKMLst.getSelectIndex();
	if(index >= 0) sendKMData(index);
}
void GGame::sendKMData(int index)
{
#ifdef WITH_OFXOSC
	ofxOscMessage m;
#else
	oscpkt::Message m;
#endif
	if(m_codeModeCmb.getCurChooseOrder() == 1)
	{
#ifdef WITH_OFXOSC
		m.setAddress(XString::ANSI2UTF8(m_keyMapOscList[index]->address));
#else
		m.init(XString::ANSI2UTF8(m_keyMapOscList[index]->address));
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) 
			LogNull("Send a data - address:%s %d",XString::ANSI2UTF8(m_keyMapOscList[index]->address).c_str(),index);
		else 
			LogNull("����һ������-��ַ:%s %d",XString::ANSI2UTF8(m_keyMapOscList[index]->address).c_str(),index);
	}else
	{
#ifdef WITH_OFXOSC
		m.setAddress(m_keyMapOscList[index]->address);
#else
		m.init(m_keyMapOscList[index]->address);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) 
			LogNull("Send a data - address:%s %d",m_keyMapOscList[index]->address.c_str(),index);
		else 
			LogNull("����һ������-��ַ:%s %d",m_keyMapOscList[index]->address.c_str(),index);
	}
	if(m_keyMapOscList[index]->keyValue == 39)
	{
#ifdef WITH_OFXOSC
		m.addIntArg(m_mouseX);
#else
		m.pushInt32(m_mouseX);
#endif
	}else
	if(m_keyMapOscList[index]->keyValue == 40)
	{
#ifdef WITH_OFXOSC
		m.addIntArg(m_mouseY);
#else
		m.pushInt32(m_mouseY);
#endif
	}else
	{
		switch(m_keyMapOscList[index]->dataType)
		{
		case 0:	//int32
#ifdef WITH_OFXOSC
			m.addIntArg(atoi(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushInt32(atoi(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 1:	//int64
#ifdef WITH_OFXOSC
			m.addInt64Arg(atoi(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushInt64(atoi(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 2:	//float
#ifdef WITH_OFXOSC
			m.addFloatArg(atof(m_keyMapOscList[index]->data.c_str()));
#else
			m.pushFloat(atof(m_keyMapOscList[index]->data.c_str()));
#endif
			break;
		case 3:	//string
#ifdef WITH_OFXOSC
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.addStringArg(XString::ANSI2UTF8(m_keyMapOscList[index]->data));
			else
				m.addStringArg(m_keyMapOscList[index]->data);
#else
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.pushStr(XString::ANSI2UTF8(m_keyMapOscList[index]->data));
			else
				m.pushStr(m_keyMapOscList[index]->data);
#endif
			break;
		case 4:	//blob����֧��
			break;
		}
	}
#ifdef WITH_OFXOSC
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
#else
	m_sender.sendMessage(m);
#endif
}
void GGame::sendButtonDown()
{
	if(!m_isSenderOpen)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please open sender at first.");
		else setMassage("������δ��,���ȴ�����.");
		return;
	}
	if(m_isSendState)
	{
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is sending,please waitting.");
		else setMassage("���ڷ�������,���Ե�.");
	}else
	{//���濪ʼ��������
		if(m_sendModeCmb.getCurChooseOrder() == CHOOSE_ADDR
			|| m_sendModeCmb.getCurChooseOrder() == CHOOSE_DATA)
		{
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose a data to sender.");
				else setMassage("��ѡ��һ����Ҫ���͵�OSC��ַ.");
				return;
			}
			m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//��ȡ��Ҫ���͵�OSC��ַ
		}
		if(m_sendModeCmb.getCurChooseOrder() == NULL_DATA)
		{//���Ϳ�����
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(m_textAddressEdt.getString() != NULL && strlen(m_textAddressEdt.getString()) > 0)
				{
					m_nowSendAddr = m_textAddressEdt.getString();
				}else
				{
					if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Please choose a OSC adress or input a one.");
					else setMassage("��ѡ�����������Ҫ���͵�OSC��ַ.");
					return;
				}
			}else
			{
				m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//��ȡ��Ҫ���͵�OSC��ַ
			}
		}
		if(m_sendModeCmb.getCurChooseOrder() == CURRENT_DATA && !checkNowData())
		{//����Ƿ��͵�ǰ���ݵĻ�����鵱ǰ�����Ƿ�Ϸ�
			if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data is invalid.");
			else setMassage("��ǰ���ݲ��Ϸ�,��ȷ���������ͺ�����ƥ��.");
			return;
		}
		m_setTimer = m_textSpacingEdt.getAsInt();
		m_setTimes = m_textTimesEdt.getAsInt();
		if(m_setTimer <= 0) m_setTimer = 0;
		if(m_setTimes <= 0) 
		{
			m_setTimes = 1;
			m_textTimesEdt.setString("1");
		}
		m_isSendState = true;
		m_timer = 0;
		m_times = 0;
		if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Started to send data.");
		else setMassage("�Ѿ���ʼ��������.");
		//���ｫ��صĿؼ���Ч(��δ���)
		m_textIPEdt.disable();
		m_textPortEdt.disable();
		m_openBtn.disable();
//		m_closeBtn.disable();
		m_addBtn.disable();
		m_delBtn.disable();
		m_clearBtn.disable();
		m_modifyBtn.disable();
		m_textDataEdt.disable();
		m_textDataCmb.disable();
		m_textAddressEdt.disable();
		m_textTimesEdt.disable();
		m_textSpacingEdt.disable();

		m_addKMBtn.disable();
		m_delKMBtn.disable();
		m_clearKMBtn.disable();
		m_modifyKMBtn.disable();
		m_keySelectCmb.disable();
		m_keyStateCmb.disable();
		m_oscDataEdt.disable();
		m_oscDataCmb.disable();
		m_oscAddressEdt.disable();

//		m_sendBtn.disable();
//		m_stopBtn.enable();
		m_sendBtn.setCaptionText(m_language.m_sendBtnCloseStr.c_str());
		m_sendAsMsg.disable();
		m_sendModeCmb.disable();
	}
}
void windowExitFun(void *p)
{
	GGame &pPar = *(GGame *)p;
	if(XEG.m_windowData.withFrame == 0)
	{//û�б߿��ʱ������ⲽ����
		pPar.m_moveData.set(1.0f,0.0f,0.002f);
		pPar.m_moveData.reset();
		pPar.m_isExit = true;
		XEG.getWindowPos(pPar.m_windowPosX,pPar.m_windowPosY);
		XEG.getWindowSize(pPar.m_windowSizeW,pPar.m_windowSizeH);
	}else
	{
		XEG.setGameExit();
	}
}
void GGame::createUI()
{
	m_tab.initWithoutSkin(XVec2(1014.0f,725.0f));
	m_tab.addATab("���Ͷ˿�����");
	m_tab.addATab("������������");
	m_tab.addATab("�������ģ��");
	m_tab.addATab("���ն�����");
	m_tab.addATab("ϵͳ����");
	m_tab.setPosition(5.0f,5.0f);
	m_mouseRBtnMnu.initWithoutSkin(4,XRect(0,0,200,50),*XEG.m_systemFont,1.0f,XVec2(100,25)); 
	m_mouseRBtnMnu.setText("����(T)",0);
	m_mouseRBtnMnu.setText("����(C)",1);
	m_mouseRBtnMnu.setText("ճ��(V)",2);
	m_mouseRBtnMnu.setText("����(S)",3);
	m_mouseRBtnMnu.setHotKey(XKEY_T,0);
	m_mouseRBtnMnu.setHotKey(XKEY_C,1);
	m_mouseRBtnMnu.setHotKey(XKEY_V,2);
	m_mouseRBtnMnu.setHotKey(XKEY_S,3);
	m_mouseRBtnMnu.setScale(0.5f);

	int h = 5 + 40;
	m_textIP.init("IP:");
	m_textIP.setPosition(10.0f,h);
	m_textIPEdt.initWithoutSkin(256.0f,"192.168.0.1",&m_mouseRBtnMnu);
	m_textIPEdt.setPosition(60.0f,h);
	m_tab.addObjToTab(&m_textIP,"���Ͷ˿�����");
	m_tab.addObjToTab(&m_textIPEdt,"���Ͷ˿�����");

	m_textPort.init("Port:");
	m_textPort.setPosition(320.0f,h);
	m_textPortEdt.initWithoutSkin(128.0f,"12345",&m_mouseRBtnMnu);
	m_textPortEdt.setPosition(400.0f,h);
	m_tab.addObjToTab(&m_textPort,"���Ͷ˿�����");
	m_tab.addObjToTab(&m_textPortEdt,"���Ͷ˿�����");

	m_openBtn.initWithoutSkin("��",128.0f);
	m_openBtn.setPosition(532.0f,h);
	m_openBtn.enable();
	m_openBtn.setWithAction(XTrue);
	m_tab.addObjToTab(&m_openBtn,"���Ͷ˿�����");

	h = 5 + 40;
	m_addBtn.initWithoutSkin("���",70.0f);
	m_addBtn.setPosition(10.0f,h);
	m_delBtn.initWithoutSkin("ɾ��",70.0f);
	m_delBtn.setPosition(84.0f,h);
	m_clearBtn.initWithoutSkin("���",70.0f);
	m_clearBtn.setPosition(158.0f,h);
	m_modifyBtn.initWithoutSkin("�޸�",70.0f);
	m_modifyBtn.setPosition(232.0f,h);
	m_tab.addObjToTab(&m_addBtn,"������������");
	m_tab.addObjToTab(&m_delBtn,"������������");
	m_tab.addObjToTab(&m_clearBtn,"������������");
	m_tab.addObjToTab(&m_modifyBtn,"������������");

	m_dataLst.initWithoutSkin(XVec2(709.0f,670.0f),3,0);
	m_dataLst.setPosition(306.0f,h);
	m_dataLst.setTitleStr("Address;Data;Type;");
	m_dataLst.setRowWidth(250,0);
	m_dataLst.setRowWidth(250,1);
	m_dataLst.setRowWidth(250,2);
	m_dataLst.setWithMouseDrag(false);
	m_tab.addObjToTab(&m_dataLst,"������������");

	h += 35;
	m_textData.init("Data:");
	m_textData.setPosition(10.0f,h);
	m_textDataEdt.initWithoutSkin(204.0f,"1234567",&m_mouseRBtnMnu);
	m_textDataEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_textData,"������������");
	m_tab.addObjToTab(&m_textDataEdt,"������������");

	h += 35;
	m_textType.init("Type:");
	m_textType.setPosition(10.0f,h);
	m_textDataCmb.initWithoutSkin(170,5,3);
	m_textDataCmb.setMenuStr("Int32;Int64;Float;String;Blob;");
	m_textDataCmb.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_textType,"������������");
	m_tab.addObjToTab(&m_textDataCmb,"������������");
	h += 37;
	m_textAddress.init("Address:");
	m_textAddress.setPosition(10.0f,h);
	m_textAddressEdt.initWithoutSkin(162.0f,"/data",&m_mouseRBtnMnu);
	m_textAddressEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textAddress,"������������");
	m_tab.addObjToTab(&m_textAddressEdt,"������������");
	h += 35;
	m_textTimes.init("Times:");
	m_textTimes.setPosition(10.0f,h);
	m_textTimesEdt.initWithoutSkin(162.0f,"1",&m_mouseRBtnMnu);
	m_textTimesEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textTimes,"������������");
	m_tab.addObjToTab(&m_textTimesEdt,"������������");
	h += 35;
	m_textSpacing.init("Spacing:");
	m_textSpacing.setPosition(10.0f,h);
	m_textSpacingEdt.initWithoutSkin(162.0f,"1",&m_mouseRBtnMnu);
	m_textSpacingEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_textSpacing,"������������");
	m_tab.addObjToTab(&m_textSpacingEdt,"������������");
	h += 35;
	m_sendModeCmb.initWithoutSkin(258,5,3);
	m_sendModeCmb.setMenuStr("����ѡ�е�ַ;����ѡ������;���Ϳ�����;���͵�ǰ����;������������;");
	m_sendModeCmb.setPosition(10.0f,h);
	m_sendModeCmb.setCurChooseOrder(1);	//Ĭ��Ϊ����ѡ������
	m_tab.addObjToTab(&m_sendModeCmb,"������������");
	h += 37;
	m_sendAsMsg.initWithoutSkin("��Ϊ��Ϣ����");
	m_sendAsMsg.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_sendAsMsg,"������������");
	h += 35;
	m_sendBtn.initWithoutSkin("��ʼ����",292.0f);
	m_sendBtn.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_sendBtn,"������������");
	h = 5 + 40;
	m_recvText.initWithoutSkin(XVec2(740.0f,670.0f),"Recv:");
	m_recvText.setPosition(275.0f,h);
	m_textRPort.init("Port:");
	m_textRPort.setPosition(10.0f,h);
	m_textRPortEdt.initWithoutSkin(172.0f,"12345",&m_mouseRBtnMnu);
	m_textRPortEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_recvText,"���ն�����");
	m_tab.addObjToTab(&m_textRPort,"���ն�����");
	m_tab.addObjToTab(&m_textRPortEdt,"���ն�����");
	h += 35;
	m_openRBtn.initWithoutSkin("��ʼ����",260.0f);
	m_openRBtn.setPosition(10.0f,h);
	m_openRBtn.enable();
	m_tab.addObjToTab(&m_openRBtn,"���ն�����");
	h += 35;
	m_supportACKMode.initWithoutSkin("�Ƿ��ʴ����");
	m_supportACKMode.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_supportACKMode,"���ն�����");
	h += 35;
	m_clearRecvBtn.initWithoutSkin("������տ�",260.0f);
	m_clearRecvBtn.setPosition(10.0f,h);
	m_tab.addObjToTab(&m_clearRecvBtn,"���ն�����");

	h = 5 + 40;
	m_languageCmb.initWithoutSkin(150,2,2);
	m_languageCmb.setPosition(10.0f,h);
	m_languageCmb.setMenuStr("english;����;");
	m_tab.addObjToTab(&m_languageCmb,"ϵͳ����");
	h += 35;
	m_codeModeCmb.initWithoutSkin(150,2,2);
	m_codeModeCmb.setPosition(10.0f,h);
	m_codeModeCmb.setMenuStr("GDK;UTF8;");
	m_tab.addObjToTab(&m_codeModeCmb,"ϵͳ����");
	h += 35;
	m_saveBtn.initWithoutSkin("����",128.0f);
	m_saveBtn.setPosition(10.0f,h);
	m_readBtn.initWithoutSkin("��ȡ",128.0f);
	m_readBtn.setPosition(142.0f,h);
	m_tab.addObjToTab(&m_saveBtn,"ϵͳ����");
	m_tab.addObjToTab(&m_readBtn,"ϵͳ����");
	//�����Ǽ������ӳ��
	h = 5 + 40;
	m_dataKMLst.initWithoutSkin(XVec2(709.0f,670.0f),5,0);
	m_dataKMLst.setPosition(306.0f,h);
	m_dataKMLst.setTitleStr("Key;State;Address;Data;Type;");
	m_dataKMLst.setRowWidth(100,0);
	m_dataKMLst.setRowWidth(100,1);
	m_dataKMLst.setRowWidth(200,2);
	m_dataKMLst.setRowWidth(200,3);
	m_dataKMLst.setRowWidth(200,4);
	m_dataKMLst.setWithMouseDrag(false);
	m_tab.addObjToTab(&m_dataKMLst,"�������ģ��");
	m_addKMBtn.initWithoutSkin("���",70.0f);
	m_addKMBtn.setPosition(10.0f,h);
	m_delKMBtn.initWithoutSkin("ɾ��",70.0f);
	m_delKMBtn.setPosition(84.0f,h);
	m_clearKMBtn.initWithoutSkin("���",70.0f);
	m_clearKMBtn.setPosition(158.0f,h);
	m_modifyKMBtn.initWithoutSkin("�޸�",70.0f);
	m_modifyKMBtn.setPosition(232.0f,h);
	m_tab.addObjToTab(&m_addKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_delKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_clearKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_modifyKMBtn,"�������ģ��");
	h += 35;
	m_keySelectCmb.initWithoutSkin(115,41,10);
	m_keySelectCmb.setMenuStr("Key_0;Key_1;Key_2;Key_3;Key_4;Key_5;Key_6;Key_7;Key_8;Key_9;Key_A;Key_B;Key_C;Key_D;Key_E;Key_F;Key_G;\
Key_H;Key_I;Key_J;Key_K;Key_L;Key_M;Key_N;Key_O;Key_P;Key_Q;Key_R;Key_S;Key_T;Key_U;Key_V;Key_W;Key_X;Key_Y;Key_Z;Mus_L;Mus_M;Mus_R;Mus_X;Mus_Y;");	//40
	m_keySelectCmb.setPosition(10.0f,h);
	m_keyStateCmb.initWithoutSkin(105,2,2);
	m_keyStateCmb.setMenuStr("Down;Up;");
	m_keyStateCmb.setPosition(163.0f,h);
	m_tab.addObjToTab(&m_keySelectCmb,"�������ģ��");
	m_tab.addObjToTab(&m_keyStateCmb,"�������ģ��");
	h += 37;
	m_oscDataTxt.init("Data:");
	m_oscDataTxt.setPosition(10.0f,h);
	m_oscDataEdt.initWithoutSkin(204.0f,"1234567",&m_mouseRBtnMnu);
	m_oscDataEdt.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_oscDataTxt,"�������ģ��");
	m_tab.addObjToTab(&m_oscDataEdt,"�������ģ��");
	h += 35;
	m_oscType.init("Type:");
	m_oscType.setPosition(10.0f,h);
	m_oscDataCmb.initWithoutSkin(170,5,3);
	m_oscDataCmb.setMenuStr("Int32;Int64;Float;String;Blob;");
	m_oscDataCmb.setPosition(98.0f,h);
	m_tab.addObjToTab(&m_oscType,"�������ģ��");
	m_tab.addObjToTab(&m_oscDataCmb,"�������ģ��");
	h += 37;
	m_oscAddress.init("Address:");
	m_oscAddress.setPosition(10.0f,h);
	m_oscAddressEdt.initWithoutSkin(162.0f,"/data",&m_mouseRBtnMnu);
	m_oscAddressEdt.setPosition(140.0f,h);
	m_tab.addObjToTab(&m_oscAddress,"�������ģ��");
	m_tab.addObjToTab(&m_oscAddressEdt,"�������ģ��");

	m_allMsgText.setACopy(*XEG.m_systemFont);
	m_allMsgText.setPosition(512.0f,748.0f);
	m_allMsgText.setMaxStrLen(512);
}
bool GGame::init()
{
//	XGL::showGLInfoToLog();	//��ʾGL����ذ汾��Ϣ
//	XEE::setStyle(XEE::COLOR_STYLE_LIGHT);
//	XEE::setVSync(false);
	XEG.getWindowPos(m_windowPosX,m_windowPosY);
	XEG.getWindowSize(m_windowSizeW,m_windowSizeH);
	if(XEG.m_windowData.withFrame == 0)
	{
		XEG.setWindowPos(m_windowPosX,
			m_windowPosY + m_windowSizeH * 0.5f);
		XEG.setWindowSize(m_windowSizeW,0.0f);
	}
	m_moveData.set(0.0f,1.0f,0.002f);//,MOVE_DATA_MODE_SHAKE);
	XEG.m_customWinTitle->setCallbackFun(windowExitFun,NULL,this);
	m_isFirst = true;
	m_isExit = false;

	createUI();
	readFromFile();

	updateLanguage();	//ʹ�����������Ч
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("System Infomation.");
	else setMassage("ϵͳ��Ϣ");
	LogStr("��ʼ�����");
	return true;
}
void GGame::sendNowData(int mode)
{//�����Ƿ���,������Ҫ�޸�Ϊ����ָ����ַ������
#ifdef WITH_OFXOSC
	ofxOscMessage m;
#else
	oscpkt::Message m;
#endif
	switch(mode)
	{
	case CHOOSE_ADDR://����ָ����ַ������
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.setAddress(m_nowSendAddr);
#else
			m.init(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.init(m_nowSendAddr);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a data - Address:%s",m_nowSendAddr.c_str());
		else LogNull("����һ������-��ַ:%s",m_nowSendAddr.c_str());
		for(unsigned int i = 0;i < m_datasVector.size();++ i)
		{
			if(m_datasVector[i]->address == m_nowSendAddr)
			{//���η���ָ����ַ������
#ifdef WITH_OFXOSC
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.addIntArg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.addFloatArg(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob����֧��
					break;
				}
#else
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.pushInt32(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.pushInt64(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.pushFloat(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.pushStr(m_datasVector[i]->data);
					break;
				case 4:	//blob����֧��
					break;
				}
#endif
			}
		}
		break;
	case CHOOSE_DATA:
		{//���͵���ѡ������
			if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
				m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
			else
				m.setAddress(m_nowSendAddr);
#else
				m.init(XString::ANSI2UTF8(m_nowSendAddr));
			else
				m.init(m_nowSendAddr);
#endif
			int i = m_dataLst.getSelectIndex();
			if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a data - Address:%s %d",m_nowSendAddr.c_str(),i);
			else LogNull("����һ������-��ַ:%s %d",m_nowSendAddr.c_str(),i);
			if(i >= 0)
			{
#ifdef WITH_OFXOSC
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.addIntArg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.addFloatArg(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob����֧��
					break;
				}
#else
				switch(m_datasVector[i]->dataType)
				{
				case 0:	//int32
					m.pushInt32(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 1:	//int64
					m.pushInt64(atoi(m_datasVector[i]->data.c_str()));
					break;
				case 2:	//float
					m.pushFloat(atof(m_datasVector[i]->data.c_str()));
					break;
				case 3:	//string
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						m.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
					else
						m.pushStr(m_datasVector[i]->data);
					break;
				case 4:	//blob����֧��
					break;
				}
#endif
			}
		}
		break;
	case NULL_DATA://���Ϳ�����
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.setAddress(m_nowSendAddr);
#else
			m.init(XString::ANSI2UTF8(m_nowSendAddr));
		else
			m.init(m_nowSendAddr);
#endif
		if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Send a blank data - Address:%s",m_nowSendAddr.c_str());
		else LogNull("����һ��������-��ַ:%s",m_nowSendAddr.c_str());
		break;
	case CURRENT_DATA://���͵�ǰ����
		if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
			m.setAddress(XString::ANSI2UTF8(m_textAddressEdt.getString()));
		else
			m.setAddress(m_textAddressEdt.getString());
#else
			m.init(XString::ANSI2UTF8(m_textAddressEdt.getString()));
		else
			m.init(m_textAddressEdt.getString());
#endif
#ifdef WITH_OFXOSC
		switch(m_textDataCmb.getCurChooseOrder())
		{
		case 0:	//int32
			m.addIntArg(atoi(m_textDataEdt.getString()));
			break;
		case 1:	//int64
			m.addInt64Arg(atoi(m_textDataEdt.getString()));
			break;
		case 2:	//float
			m.addFloatArg(atof(m_textDataEdt.getString()));
			break;
		case 3:	//string
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.addStringArg(XString::ANSI2UTF8(m_textDataEdt.getString()));
			else
				m.addStringArg(m_textDataEdt.getString());
			break;
		case 4:	//blob����֧��
			break;
		}
#else
		switch(m_textDataCmb.getCurChooseOrder())
		{
		case 0:	//int32
			m.pushInt32(atoi(m_textDataEdt.getString()));
			break;
		case 1:	//int64
			m.pushInt64(atoi(m_textDataEdt.getString()));
			break;
		case 2:	//float
			m.pushFloat(atof(m_textDataEdt.getString()));
			break;
		case 3:	//string
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				m.pushStr(XString::ANSI2UTF8(m_textDataEdt.getString()));
			else
				m.pushStr(m_textDataEdt.getString());
			break;
		case 4:	//blob����֧��
			break;
		}
#endif
		break;
	default://�����б��е���������
		for(unsigned int i = 0;i < m_datasVector.size();++ i)
		{
#ifdef WITH_OFXOSC
			ofxOscMessage tmpM;
#else
			oscpkt::Message tmpM;
#endif
			if(m_codeModeCmb.getCurChooseOrder() == 1)
#ifdef WITH_OFXOSC
				tmpM.setAddress(XString::ANSI2UTF8(m_datasVector[i]->address));
			else
				tmpM.setAddress(m_datasVector[i]->address);
#else
				tmpM.init(XString::ANSI2UTF8(m_datasVector[i]->address));
			else
				tmpM.init(m_datasVector[i]->address);
#endif
			//���η���ָ����ַ������
#ifdef WITH_OFXOSC
			switch(m_datasVector[i]->dataType)
			{
			case 0:	//int32
				tmpM.addIntArg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 1:	//int64
				tmpM.addInt64Arg(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 2:	//float
				tmpM.addFloatArg(atof(m_datasVector[i]->data.c_str()));
				break;
			case 3:	//string
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					tmpM.addStringArg(XString::ANSI2UTF8(m_datasVector[i]->data));
				else
					tmpM.addStringArg(m_datasVector[i]->data);
				break;
			case 4:	//blob����֧��
				break;
			}
#else
			switch(m_datasVector[i]->dataType)
			{
			case 0:	//int32
				tmpM.pushInt32(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 1:	//int64
				tmpM.pushInt64(atoi(m_datasVector[i]->data.c_str()));
				break;
			case 2:	//float
				tmpM.pushFloat(atof(m_datasVector[i]->data.c_str()));
				break;
			case 3:	//string
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					tmpM.pushStr(XString::ANSI2UTF8(m_datasVector[i]->data));
				else
					tmpM.pushStr(m_datasVector[i]->data);
				break;
			case 4:	//blob����֧��
				break;
			}
#endif
#ifdef WITH_OFXOSC
			if(m_sendAsMsg.getState()) m_sender.sendMessage(tmpM);
			else
			{
				ofxOscBundle b;
				b.addMessage(tmpM);
				m_sender.sendBundle(b);
			}
#else
			m_sender.sendMessage(tmpM);
#endif
		}
		return;	//�������֮��ֱ�ӷ���
	}

#ifdef WITH_OFXOSC
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
#else
	m_sender.sendMessage(m);
#endif
}
void GGame::move(float stepTime)
{
	if(!m_moveData.getIsEnd())
	{
		if(m_isFirst) m_moveData.move(0);
		else m_moveData.move(stepTime);
		float tmpData = m_moveData.getCurData();
		if(XEG.m_windowData.withFrame == 0)
		{
			XEG.setWindowPos(m_windowPosX,
				m_windowPosY + m_windowSizeH * 0.5f * (1.0f - tmpData));
			XEG.setWindowSize(m_windowSizeW,m_windowSizeH * tmpData);
			if(m_isExit && m_moveData.getIsEnd()) XEG.setGameExit();
		}
	}
	if(m_isFirst) m_isFirst = false;
	ctrlStateManager();
	if(m_recvState)
	{//��������
#ifdef WITH_OFXOSC
		while(m_receiver.hasWaitingMessages())
#else
		while(m_receiver.haveMessage())
#endif
		{
#ifdef WITH_OFXOSC
			ofxOscMessage m;
			m_receiver.getNextMessage(&m);
#else
			oscpkt::Message tmp;
			m_receiver.popAMessage(tmp);
			XOscMessage m(tmp);
#endif
			//����Ϣ���뵽��ʾ��
			++m_recvSum;
			std::string recvData = "";
			char tmpStr[1024];
			//if(XString::isUTF8(m.getAddress()))
#ifdef WITH_OFXOSC
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,XString::UTF82ANSI(m.getAddress()).c_str());
			else
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.getAddress().c_str());
#else
			if(m_codeModeCmb.getCurChooseOrder() == 1)
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,XString::UTF82ANSI(m.m_address).c_str());
			else
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.m_address.c_str());
#endif
			recvData += tmpStr;
			if(getLanguageIndex() == INDEX_ENGLISH) LogNull("Receiver a data - Address:%s",tmpStr);
			else LogNull("���յ�һ������-��ַ:%s",tmpStr);
#ifdef WITH_OFXOSC
			for(int i = 0; i < m.getNumArgs(); i++)
			{//get the argument type
				recvData += "|Type:" + m.getArgTypeName(i);
				recvData += ":";
				switch(m.getArgType(i))
				{
				case OFXOSC_TYPE_INT32:
					recvData += XString::toString(m.getArgAsInt32(i));
					break;
				case OFXOSC_TYPE_INT64:
					recvData += XString::toString(m.getArgAsInt64(i));
					break;
				case OFXOSC_TYPE_FLOAT:
					recvData += XString::toString(m.getArgAsFloat(i));
					break;
				case OFXOSC_TYPE_STRING:
					//if(XString::isUTF8(m.getArgAsString(i)))
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						recvData += XString::UTF82ANSI(m.getArgAsString(i));
					else
						recvData += m.getArgAsString(i);
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
#else
			for(int i = 0; i < m.m_args.size();++ i)
			{//get the argument type
				recvData += "|Type:" + m.m_args[i].getTypeName();
				recvData += ":";
				switch(m.m_args[i].type)
				{
				case OSC_TYPE_INT32:
					recvData += XString::toString(m.m_args[i].dataI);
					break;
				case OSC_TYPE_INT64:
					recvData += XString::toString(m.m_args[i].dataL);
					break;
				case OSC_TYPE_FLOAT:
					recvData += XString::toString(m.m_args[i].dataF);
					break;
				case OSC_TYPE_STRING:
					//if(XString::isUTF8(m.getArgAsString(i)))
					if(m_codeModeCmb.getCurChooseOrder() == 1)
						recvData += XString::UTF82ANSI(m.m_args[i].dataS);
					else
						recvData += m.m_args[i].dataS;
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
#endif
			m_recvText.addALine(recvData.c_str());
#ifdef WITH_OFXOSC
			if(m_supportACKMode.getState() && m.getNumArgs() == 0)
			{//���֧���ʴ���ƵĻ���������Ҫ����Ƿ���Ҫ�ʴ�,�������ں������Ż��������Ʋ�ͳһ
				std::string tmp = m_nowSendAddr;
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					m_nowSendAddr = XString::UTF82ANSI(m.getAddress());
				else 
					m_nowSendAddr = m.getAddress();
				sendNowData(0);
				m_nowSendAddr = tmp;
			}
#else
			if(m_supportACKMode.getState() && m.m_args.size() == 0)
			{//���֧���ʴ���ƵĻ���������Ҫ����Ƿ���Ҫ�ʴ�,�������ں������Ż��������Ʋ�ͳһ
				std::string tmp = m_nowSendAddr;
				if(m_codeModeCmb.getCurChooseOrder() == 1)
					m_nowSendAddr = XString::UTF82ANSI(m.m_address);
				else 
					m_nowSendAddr = m.m_address;
				sendNowData(0);
				m_nowSendAddr = tmp;
			}
#endif
		}
	}
	if(m_isSendState)
	{//��������
		if(m_timer == 0)
		{
			sendNowData(m_sendModeCmb.getCurChooseOrder());

			++m_times;
			if(m_times >= m_setTimes)
			{
				sendOverProc();//�������
			}else
				m_textTimesEdt.setString(XString::toString(m_setTimes - m_times).c_str());
		}
		m_timer += stepTime;
		if(m_timer > m_setTimer) m_timer = 0;
	}
//	if(m_mouseOnTimer < 10000) m_mouseOnTimer += stepTime;
//	ctrlMsgProc();
	//�������hook�����ݷ�������
	if(m_isSenderOpen && m_keyMapOscList.size() > 0)
	{
		XInputEvent e;
		if(popAInputEvent(e))
		{
			switch(e.type)
			{
			case EVENT_KEYBOARD:
				{
					int index = keyValueToIndex(e.keyValue);
					if(index >= 100) break;
					int state = 0;
					if(e.keyState == KEY_STATE_UP) state = 1;
					for(int i = 0;i < m_keyMapOscList.size();++ i)
					{
						if(m_keyMapOscList[i]->keyValue == index && 
							m_keyMapOscList[i]->keyState == state)
						{//������������
							sendKMData(i);
						}
					}
				}
				break;
			case EVENT_MOUSE:
				{
					int index = 100;
					int state = 0;
					switch(e.mouseState)
					{
					case MOUSE_LEFT_BUTTON_DOWN:
						index = 36;state = 0;
						break;
					case MOUSE_LEFT_BUTTON_UP:
						index = 36;state = 1;
						break;
					case MOUSE_MIDDLE_BUTTON_DOWN:
						index = 37;state = 0;
						break;
					case MOUSE_MIDDLE_BUTTON_UP:
						index = 37;state = 1;
						break;
					case MOUSE_RIGHT_BUTTON_DOWN:
						index = 38;state = 0;
						break;
					case MOUSE_RIGHT_BUTTON_UP:
						index = 38;state = 1;
						break;
					}
					if(index >= 100 && e.mouseState != MOUSE_MOVE) break;
					for(int i = 0;i < m_keyMapOscList.size();++ i)
					{
						if(m_keyMapOscList[i]->keyValue == index && 
							m_keyMapOscList[i]->keyState == state)
						{//������������
							sendKMData(i);
						}else
						if(m_keyMapOscList[i]->keyValue == 39 && m_mouseX != e.mouseX && e.mouseX <= 512
							&& m_mouseX != (e.mouseX >> 1))
						{
							m_mouseX = (e.mouseX >> 1);
							sendKMData(i);
						}else
						if(m_keyMapOscList[i]->keyValue == 40 && m_mouseY != e.mouseY && e.mouseY <= 512
							&& m_mouseY != (e.mouseY >> 1))
						{
							m_mouseY = (e.mouseY >> 1);
							sendKMData(i);
						}
					}
				}
				break;
			}
		}
	}
}
int GGame::keyValueToIndex(XKeyValue k)
{
	switch(k)
	{
	case XKEY_0: return 0;
	case XKEY_1: return 1;
	case XKEY_2: return 2;
	case XKEY_3: return 3;
	case XKEY_4: return 4;
	case XKEY_5: return 5;
	case XKEY_6: return 6;
	case XKEY_7: return 7;
	case XKEY_8: return 8;
	case XKEY_9: return 9;
	case XKEY_A: return 10;
	case XKEY_B: return 11;
	case XKEY_C: return 12;
	case XKEY_D: return 13;
	case XKEY_E: return 14;
	case XKEY_F: return 15;
	case XKEY_G: return 16;
	case XKEY_H: return 17;
	case XKEY_I: return 18;
	case XKEY_J: return 19;
	case XKEY_K: return 20;
	case XKEY_L: return 21;
	case XKEY_M: return 22;
	case XKEY_N: return 23;
	case XKEY_O: return 24;
	case XKEY_P: return 25;
	case XKEY_Q: return 26;
	case XKEY_R: return 27;
	case XKEY_S: return 28;
	case XKEY_T: return 29;
	case XKEY_U: return 30;
	case XKEY_V: return 31;
	case XKEY_W: return 32;
	case XKEY_X: return 33;
	case XKEY_Y: return 34;
	case XKEY_Z: return 35;
	default:
		return 100;
	}
}
//void GGame::ctrlMsgProc()
//{
//	if(!m_isShowMsg && m_mouseOnTimer > 2000)
//	{
//		if(m_textIPEdt.getVisible() && m_textIPEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//IP
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Enter the OSC sender IP,the local IP 127.0.0.1.");
//			else m_allMsgText.setString("����OSC���Ͷ˵�IP��ַ������IP��������127.0.0.1");
//		}else
//		if(m_textPortEdt.getVisible() && m_textPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//Port
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Enter the OSC sender port number,the default is 12345.");
//			else m_allMsgText.setString("����OSC���Ͷ˵Ķ˿ںţ��˿ں���Ҫ����ն˵Ķ˿ں�һ�£�Ĭ��Ϊ12345");
//		}else
//		if(m_openBtn.getVisible() && m_openBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�򿪰�ť
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC transmitter turned on or off , make sure the correct IP and port.");
//			else m_allMsgText.setString("�򿪻��߹ر�OSC��������ȷ��IP��ַ�Ͷ˿���ȷ�����򲻼������ȷ��");
//		}else
//		if(m_sendAsMsg.getVisible() && m_sendAsMsg.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//��Ϊ��Ϣ����
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Checked, the data to be sent as a message, otherwise it is sent as a bundle.");
//			else m_allMsgText.setString("��ѡʱ��Ҫ���͵�������Ϊһ����Ϣ���ͣ�������Ϊһ�����ݰ�����.");
//		}else
//		if(m_addBtn.getVisible() && m_addBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//���
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Add the OSC data to list on the right.");
//			else m_allMsgText.setString("����߱༭�õ�OSC������Ϣ��ӵ��ұߵ��б����Ա���֮�������");
//		}else
//		if(m_delBtn.getVisible() && m_delBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//ɾ��
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Delete the selected data on the left.");
//			else m_allMsgText.setString("ɾ������б���ѡ�е����ݡ�");
//		}else
//		if(m_clearBtn.getVisible() && m_clearBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//���
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear all datas of the right list.");
//			else m_allMsgText.setString("����ұ��б���е��������ݡ�");
//		}else
//		if(m_modifyBtn.getVisible() && m_modifyBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�޸�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Update data to the right of the selected row.");
//			else m_allMsgText.setString("���ұ��б���ѡ��һ�����ݲ��޸ģ����¸ð�ť���޸ĺ�����ݸ��µ��б��С�");
//		}else
//		if(m_textDataEdt.getVisible() && m_textDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//data
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Osc data,make sure that the data and the type match.");
//			else m_allMsgText.setString("OSC��Ϣ�ж�Ӧ�����ݣ���ȷ��������������������������ƥ�䡣");
//		}else
//		if(m_textDataCmb.getVisible() && m_textDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//type
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The osc data type.");
//			else m_allMsgText.setString("OSC��Ϣ�����ݵ��������ͣ���ȷ����������������ƥ�䡣");
//		}else
//		if(m_textAddressEdt.getVisible() && m_textAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//addr
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The osc address.");
//			else m_allMsgText.setString("OSC��Ϣ�е�ַ��");
//		}else
//		if(m_textTimesEdt.getVisible() && m_textTimesEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//Times
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The times of OSC massages sent cycles.");
//			else m_allMsgText.setString("OSC��Ϣ��Ҫѭ�����͵Ĵ������������ͽ�����������ֵ��ѭ�����͡�");
//		}else
//		if(m_textSpacingEdt.getVisible() && m_textSpacingEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//space
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The interval between the two sent in milliseconds (ms).");
//			else m_allMsgText.setString("���η���֮��ļ��ʱ�䣬��λΪ����(ms)��1����Ϊǧ��֮һ�롣");
//		}else
//		if(m_sendBtn.getVisible() && m_sendBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//send
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Send or stop sending,please make sure the OSC sender has been opened.");
//			else m_allMsgText.setString("�����趨�������ݻ�ֹͣ���ͣ�����ǰ����ȷ��OSC�������Ѿ���ȷ�򿪡�");
//		}else
//		if(m_sendModeCmb.getVisible() && m_sendModeCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//����ģʽ
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Select the desired transmission mode ,there are four transmission modes.");
//			else m_allMsgText.setString("ѡ����Ҫ�ķ���ģʽ���������ַ���ģʽ���������ʣ������Ķ�˵���ĵ���");
//		}else
//		if(m_dataLst.getVisible() && m_dataLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�����б�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Data list,double-click on the data line to send.");
//			else m_allMsgText.setString("�����б�����˫����Ӧ�����н��з��ͣ���ȷ���Ѿ���ȷ��OSC��������");
//		}else
//		if(m_supportACKMode.getVisible() && m_supportACKMode.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//Ӧ��ģʽ
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("ACK mode,receives data in the address blank will return data that address.");
//			else m_allMsgText.setString("Ӧ��ģʽ������ʱ�������յ�ĳ��ַ�Ŀ�����ʱ�����Զ����ظõ�ַ�����ݡ�");
//		}else
//		if(m_clearRecvBtn.getVisible() && m_clearRecvBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//����б�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear the reception list.");
//			else m_allMsgText.setString("���¸ð�ť��ս����б������б��е��������ݽ�����ա�");
//		}else
//		if(m_textRPortEdt.getVisible() && m_textRPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//���ն˿�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("The OSC receiver port number,the default is 12345.");
//			else m_allMsgText.setString("OSC�������Ķ˿ںţ���������Ӧ�������Ķ˿ں�һ�²�����ȷ���գ�Ĭ��Ϊ12345");
//		}else
//		if(m_openRBtn.getVisible() && m_openRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//��
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC receiver on or off.");
//			else m_allMsgText.setString("�򿪻�ر�OSC����������ȷ���������˿ں�������ȷ��");
//		}else
//		if(m_saveBtn.getVisible() && m_saveBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//����
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Save all datas to \"config.xml\".");
//			else m_allMsgText.setString("����ǰ�����ϵ��������ݱ��浽config.xml�ļ��С�");
//		}else
//		if(m_readBtn.getVisible() && m_readBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//��ȡ
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Read the last saved configuration data from \"config.xml\".");
//			else m_allMsgText.setString("�������ļ�config.xml�ж�ȡ��һ�α�����������ݡ�");
//		}else
//		if(m_recvText.getVisible() && m_recvText.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�����б��
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Received data will be displayed here.");
//			else m_allMsgText.setString("���յ����ݽ���ʾ�ڴˣ�ѡ����ĩ�п��Ա��ֹ����ĩ�С�");
//		}else
//		if(m_addKMBtn.getVisible() && m_addKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//���
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Add the map to the map list.");
//			else m_allMsgText.setString("����߱༭�õ���������OSC��ӳ����ӵ�ӳ���б��С�");
//		}else
//		if(m_delKMBtn.getVisible() && m_delKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//ɾ��
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Delete selected row in the map list.");
//			else m_allMsgText.setString("ɾ��ӳ���б���ѡ����С�");
//		}else
//		if(m_clearKMBtn.getVisible() && m_clearKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//���
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Clear map list on the left.");
//			else m_allMsgText.setString("�����ߵ�ӳ���б�");
//		}else
//		if(m_modifyKMBtn.getVisible() && m_modifyKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�޸�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Update data to the left row selected in the list.");
//			else m_allMsgText.setString("����߱༭�õ�ֵ���µ�ӳ���б���ѡ�е��С�");
//		}else
//		if(m_keySelectCmb.getVisible() && m_keySelectCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//��ֵ
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Key or mouse button.");
//			else m_allMsgText.setString("��Ӧ�������߼��̰�����");
//		}else
//		if(m_keyStateCmb.getVisible() && m_keyStateCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//����״̬
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Key or button state.");
//			else m_allMsgText.setString("������״̬��");
//		}else
//		if(m_oscDataEdt.getVisible() && m_oscDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc���ݱ༭
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC data.");
//			else m_allMsgText.setString("OSC���ݡ�");
//		}else
//		if(m_oscDataCmb.getVisible() && m_oscDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc���ݱ༭
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC data type.");
//			else m_allMsgText.setString("OSC�������͡�");
//		}else
//		if(m_oscAddressEdt.getVisible() && m_oscAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//osc��ַ
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("OSC address.");
//			else m_allMsgText.setString("OSC��ַ��");
//		}else
//		if(m_dataKMLst.getVisible() && m_dataKMLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�����̺�osc��ӳ���б�
//			if(getLanguageIndex() == INDEX_ENGLISH) m_allMsgText.setString("Mapping list mouse, keyboard and osc.");
//			else m_allMsgText.setString("�����̺�osc��ӳ���б�");
//		}
//	}
//}
void GGame::draw()
{
//	m_textIP.draw();
//	m_textPort.draw();
//	m_textData.draw();
//	m_textType.draw();
//	m_textAddress.draw();
//	m_textTimes.draw();
//	m_textSpacing.draw();
//	drawLine(10,375,1016,375);
//	m_textRIP.draw();
//	m_textRPort.draw();
	m_allMsgText.draw();
}
void GGame::input(const XInputEvent &event)
{
//	if(event.type == EVENT_MOUSE)
//	{
//		m_mouseOnTimer = 0;
//		m_isShowMsg = false;
//	}
	if(event.isKeyBoardDown())
	{
		switch(event.keyValue)
		{
		case XKEY_ESCAPE:
			windowExitFun(this);
			break;
//		case XKEY_A:
//			XEE::setWindowAlpha(0.5f);
//			//XEE::setBGColor(XFColor::white);
//			break;
		}
	}
}
void GGame::release()
{
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XMem::XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XMem::XDELETE(m_keyMapOscList[i]);
	}
	stopHook();
	m_keyMapOscList.clear();
}
bool GGame::saveToFile(const char *filename)
{
	//TiXmlDocument doc(CFG_FILENAME);
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlDocument doc;
//	if(filename == NULL) doc.crea(CFG_FILENAME);
//	else doc.LoadFile(filename);
	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	if(!XXml::addLeafNode(elmRoot,"sendIP",m_textIPEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendPort",m_textPortEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendAsMessage",m_sendAsMsg.getState())) return false;
	if(!XXml::addLeafNode(elmRoot,"dataE",m_textDataEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"typeE",XString::toString(m_textDataCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"addressE",m_textAddressEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"timeE",m_textTimesEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"spaceE",m_textSpacingEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"sendMode",XString::toString(m_sendModeCmb.getCurChooseOrder()))) return false;

	for(int i = 0;i < m_datasVector.size();++ i)
	{
		TiXmlElement elmNode("data"); 
		if(!XXml::addLeafNode(elmNode,"addr",m_datasVector[i]->address)) return false;
		if(!XXml::addLeafNode(elmNode,"datas",m_datasVector[i]->data)) return false;
		if(!XXml::addLeafNode(elmNode,"type",XString::toString(m_datasVector[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!XXml::addLeafNode(elmRoot,"keyValueE",XString::toString(m_keySelectCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"keyStateE",XString::toString(m_keyStateCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"oscDataE",m_oscDataEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"oscTypeE",XString::toString(m_oscDataCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"oscAddrE",m_oscAddressEdt.getString())) return false;
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		TiXmlElement elmNode("map"); 
		if(!XXml::addLeafNode(elmNode,"keyValue",XString::toString(m_keyMapOscList[i]->keyValue))) return false;
		if(!XXml::addLeafNode(elmNode,"keyState",XString::toString(m_keyMapOscList[i]->keyState))) return false;
		if(!XXml::addLeafNode(elmNode,"addr",m_keyMapOscList[i]->address)) return false;
		if(!XXml::addLeafNode(elmNode,"datas",m_keyMapOscList[i]->data)) return false;
		if(!XXml::addLeafNode(elmNode,"type",XString::toString(m_keyMapOscList[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!XXml::addLeafNode(elmRoot,"recvPort",m_textRPortEdt.getString())) return false;
	if(!XXml::addLeafNode(elmRoot,"ACKMode",m_supportACKMode.getState())) return false;
	if(!XXml::addLeafNode(elmRoot,"language",XString::toString(m_languageCmb.getCurChooseOrder()))) return false;
	if(!XXml::addLeafNode(elmRoot,"codeMode",XString::toString(m_codeModeCmb.getCurChooseOrder()))) return false;

	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	bool ret;
	if(filename == NULL) ret = doc.SaveFile(CFG_FILENAME);
	else ret = doc.SaveFile(filename);
	if(!ret) return ret;
	//doc.SaveFile();
	if(getLanguageIndex() == INDEX_ENGLISH) setMassage("Data saved!"); 
	else setMassage("�ļ��������!");
	//����
	//{
	//	TiXmlDocument doc;
	//	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	//	doc.InsertEndChild(declaration);
	//	TiXmlElement elmRoot("root");
	//	m_tab.saveState(elmRoot);
	//	doc.InsertEndChild(elmRoot);
	//	doc.SaveFile("Test.xml");
	//}
	return true;
}
bool GGame::readFromFile(const char *filename)
{
	TiXmlBase::SetCondenseWhiteSpace(false);
	if(filename != NULL && filename[0] == '\0') return false;
	std::string tmp;
	bool tmpB;
	int tmpI;
	TiXmlNode *keyNode = NULL;
	//TiXmlDocument doc(CFG_FILENAME);
	TiXmlDocument doc;
	if(filename == NULL) doc.LoadFile(CFG_FILENAME);
	else doc.LoadFile(filename);

	if(!doc.LoadFile()) return false;
	TiXmlNode *rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	if(XXml::getXmlAsString(rootNode,"sendIP",tmp))
		m_textIPEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"sendPort",tmp))
		m_textPortEdt.setString(tmp.c_str());
	if(XXml::getXmlAsBool(rootNode,"sendAsMessage",tmpB))
		m_sendAsMsg.setState(tmpB);
	if(XXml::getXmlAsString(rootNode,"dataE",tmp))
		m_textDataEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"typeE",tmpI))
		m_textDataCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"addressE",tmp))
		m_textAddressEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"timeE",tmp))
		m_textTimesEdt.setString(tmp.c_str());
	if(XXml::getXmlAsString(rootNode,"spaceE",tmp))
		m_textSpacingEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"sendMode",tmpI))
		m_sendModeCmb.setCurChooseOrder(tmpI);
	//�����ȡ����
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XMem::XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	m_dataLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("data");
	while(keyNode != NULL)
	{
		GOscData *tmp = XMem::createMem<GOscData>();
		XXml::getXmlAsString(keyNode,"addr",tmp->address);
		XXml::getXmlAsString(keyNode,"datas",tmp->data);
		XXml::getXmlAsInt(keyNode,"type",tmp->dataType);
		m_datasVector.push_back(tmp);
		addALineData(tmp);

		keyNode = keyNode->NextSiblingElement("data");
	}

	if(XXml::getXmlAsInt(rootNode,"keyValueE",tmpI))
		m_keySelectCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsInt(rootNode,"keyStateE",tmpI))
		m_keyStateCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"oscDataE",tmp))
		m_oscDataEdt.setString(tmp.c_str());
	if(XXml::getXmlAsInt(rootNode,"oscTypeE",tmpI))
		m_oscDataCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsString(rootNode,"oscAddrE",tmp))
		m_oscAddressEdt.setString(tmp.c_str());
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XMem::XDELETE(m_keyMapOscList[i]);
	}
	m_keyMapOscList.clear();
	m_dataKMLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("map");
	while(keyNode != NULL)
	{
		GKMOscData *tmp = XMem::createMem<GKMOscData>();
		XXml::getXmlAsInt(keyNode,"keyValue",tmp->keyValue);
		XXml::getXmlAsInt(keyNode,"keyState",tmp->keyState);
		XXml::getXmlAsString(keyNode,"addr",tmp->address);
		XXml::getXmlAsString(keyNode,"datas",tmp->data);
		XXml::getXmlAsInt(keyNode,"type",tmp->dataType);
		m_keyMapOscList.push_back(tmp);
		addALineData(tmp);
		keyNode = keyNode->NextSiblingElement("map");
	}
	//��ȡ��������
	if(XXml::getXmlAsString(rootNode,"recvPort",tmp))
		m_textRPortEdt.setString(tmp.c_str());
	if(XXml::getXmlAsBool(rootNode,"ACKMode",tmpB))
		m_supportACKMode.setState(tmpB);
	if(XXml::getXmlAsInt(rootNode,"language",tmpI))
		m_languageCmb.setCurChooseOrder(tmpI);
	if(XXml::getXmlAsInt(rootNode,"codeMode",tmpI))
		m_codeModeCmb.setCurChooseOrder(tmpI);
	//����
	//TiXmlDocument doc;
	//doc.LoadFile("Test.xml");

	//if(!doc.LoadFile()) return false;
	//TiXmlNode *rootNode = doc.FirstChild("root");
	//if(rootNode == NULL) return false;
	//m_tab.loadState(rootNode);

	return true;
}
void GGame::addALineData(const GOscData *tmp)
{
	int lineSum = m_dataLst.getLineSum();
	m_dataLst.addALine();
	setLineData(tmp,lineSum);
}
void GGame::setLineData(const GOscData *tmp,int index)
{
	m_dataLst.setBoxStr(tmp->address.c_str(),index,0);
	m_dataLst.setBoxStr(tmp->data.c_str(),index,1);
	m_dataLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,2);
}
void GGame::addALineData(const GKMOscData *tmp)
{
	int lineSum = m_dataKMLst.getLineSum();
	m_dataKMLst.addALine();
	setLineData(tmp,lineSum);
}
void GGame::setLineData(const GKMOscData *tmp,int index)
{
	m_dataKMLst.setBoxStr(keyValueToStr(tmp->keyValue).c_str(),index,0);
	switch(tmp->keyState)
	{
	case 0:
		m_dataKMLst.setBoxStr("Down",index,1);
		break;
	default:
		m_dataKMLst.setBoxStr("Up",index,1);
		break;
	}
	m_dataKMLst.setBoxStr(tmp->address.c_str(),index,2);
	if(tmp->keyValue == 39 || tmp->keyValue == 40) 
		m_dataKMLst.setBoxStr("0 - 255",index,3); 
	else
		m_dataKMLst.setBoxStr(tmp->data.c_str(),index,3);
	m_dataKMLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,4);
}
std::string GGame::keyValueToStr(int keyValue)
{
	switch(keyValue)
	{
	case 0:return "KEY_0";
	case 1:return "KEY_1";
	case 2:return "KEY_2";
	case 3:return "KEY_3";
	case 4:return "KEY_4";
	case 5:return "KEY_5";
	case 6:return "KEY_6";
	case 7:return "KEY_7";
	case 8:return "KEY_8";
	case 9:return "KEY_9";
	case 10:return "KEY_A";
	case 11:return "KEY_B";
	case 12:return "KEY_C";
	case 13:return "KEY_D";
	case 14:return "KEY_E";
	case 15:return "KEY_F";
	case 16:return "KEY_G";
	case 17:return "KEY_H";
	case 18:return "KEY_I";
	case 19:return "KEY_J";
	case 20:return "KEY_K";
	case 21:return "KEY_L";
	case 22:return "KEY_M";
	case 23:return "KEY_N";
	case 24:return "KEY_O";
	case 25:return "KEY_P";
	case 26:return "KEY_Q";
	case 27:return "KEY_R";
	case 28:return "KEY_S";
	case 29:return "KEY_T";
	case 30:return "KEY_U";
	case 31:return "KEY_V";
	case 32:return "KEY_W";
	case 33:return "KEY_X";
	case 34:return "KEY_Y";
	case 35:return "KEY_Z";
	case 36:return "Mouse_LeftBtn";
	case 37:return "Mouse_MiddleBtn";
	case 38:return "Mouse_RightBtn";
	case 39:return "MouseX";
	case 40:return "Mouse_Y";
	default:
		return "";
	}
}
std::string GGame::oscDataTypeToStr(int type)
{
	switch(type)
	{
	case 0: return "Int32";
	case 1: return "Int64";
	case 2: return "Float";
	case 3: return "String";
	case 4: return "Blob";
	default:return "";
	}
}
void GGame::ctrlStateManager()
{
	if(m_recvText.getLineSum() > 1) m_clearRecvBtn.enable();
	else m_clearRecvBtn.disable();
	if(m_sendBtn.getEnable())
	{
		if(m_dataLst.getLineSum() <= 0) m_clearBtn.disable();
		else m_clearBtn.enable();
		if(m_dataLst.getSelectIndex() < 0)
		{
			m_delBtn.disable();
			m_modifyBtn.disable();
		}else
		{
			m_delBtn.enable();
			m_modifyBtn.enable();
		}
		if(m_dataKMLst.getLineSum() <= 0) m_clearKMBtn.disable();
		else m_clearKMBtn.enable();
		if(m_dataKMLst.getSelectIndex() < 0)
		{
			m_delKMBtn.disable();
			m_modifyKMBtn.disable();
		}else
		{
			m_delKMBtn.enable();
			m_modifyKMBtn.enable();
		}
	}
}
//#include <psapi.h>
//��ȡָ�����Ƶ��ڴ�ʹ��������λΪKB
//#define MAX_BUF_SIZE (512)
//int getProcMemoryUsage( const char* pProcessName )
//{
//	WCHAR chBuf[MAX_BUF_SIZE];
//	ZeroMemory( chBuf , MAX_BUF_SIZE );
//	DWORD dwProcs[1024*2];
//	DWORD dwNeeded;
//	if( !EnumProcesses(dwProcs , sizeof(dwProcs) , &dwNeeded  ))
//	{//���������Ϣ
//		printf("EnumProcesses Failed (%d).\n",GetLastError());
//		return -1;
//	}
//	//�����ж��ٸ�����ID
//	DWORD dwProcCount =  dwNeeded / sizeof( DWORD );
//
//	HMODULE hMod;
//	DWORD arraySize;
//	char processName[MAX_BUF_SIZE];
//	for( int i=0; i<dwProcCount ; i++ )
//	{
//		DWORD m_processid = dwProcs[i];
//		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_processid); 
//		if(hProcess)
//		{
//			if (EnumProcessModules(hProcess,&hMod,sizeof(HMODULE),&arraySize))
//			{
//				GetModuleBaseName(hProcess,hMod,processName,sizeof(processName));
//				if( strcmp(processName , pProcessName )!=0 )
//				{
//					PROCESS_MEMORY_COUNTERS pmc;
//					GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));
// 
//					printf("%d \n %d\n %d \n %d\n" ,pmc.PeakWorkingSetSize , pmc.PagefileUsage , pmc.QuotaPagedPoolUsage , pmc.QuotaPeakPagedPoolUsage);
//					return pmc.PagefileUsage/1024;
//				}
//			}
//		}
//	}
//	return -1;
//}
void GGame::updateLanguage()
{
	switch(m_languageCmb.getCurChooseOrder())
	{
	case 0:	//Ӣ��
		if(!m_language.setCurrentLanguage(XLGG_EN,"languageEN.xml")) return;
		setMassage("Set to English.");
		break;
	case 1:
		if(!m_language.setCurrentLanguage(XLGG_EN,"languageCN.xml")) return;
		setMassage("����Ϊ����");
		break;
	}
	m_tab.setTabsStr(m_language.m_tabTitileStr.c_str());
	//�����ǹ��ڽ���Ԫ�صĶ���
	//m_textIP;
	//m_textIPEdt;
	//m_textPort;
	//m_textPortEdt;
	if(m_isSenderOpen) m_openBtn.setCaptionText(m_language.m_openBtnCloseStr.c_str());
	else m_openBtn.setCaptionText(m_language.m_openBtnOpenStr.c_str());
	//m_openBtn;
	////m_closeBtn;
	//++++++++++++++++++++++++++++++
	//������Ϊ�����Ӽ���ģ�������ı���
	m_addKMBtn.setCaptionText(m_language.m_addKMBtnStr.c_str());
	m_delKMBtn.setCaptionText(m_language.m_delKMBtnStr.c_str());
	m_clearKMBtn.setCaptionText(m_language.m_clearKMBtnStr.c_str());	//���
	m_modifyKMBtn.setCaptionText(m_language.m_modifyKMBtnStr.c_str());	//�޸İ�ť
	//m_keySelectCmb;	//������ѡ��
	//m_keyStateCmb;	//����״̬��ѡ��
	m_oscDataTxt.setString(m_language.m_oscDataTxtStr.c_str());
	//m_oscDataEdt;
	m_oscType.setString(m_language.m_oscTypeStr.c_str());
	//m_oscDataCmb;
	m_oscAddress.setString(m_language.m_oscAddressStr.c_str());
	//m_oscAddressEdt;
	//m_dataKMLst;
	//------------------------------
	m_addBtn.setCaptionText(m_language.m_addBtnStr.c_str());
	m_delBtn.setCaptionText(m_language.m_delBtnStr.c_str());
	m_clearBtn.setCaptionText(m_language.m_clearBtnStr.c_str());	//���
	m_modifyBtn.setCaptionText(m_language.m_modifyBtnStr.c_str());	//�޸İ�ť
	m_textData.setString(m_language.m_textDataStr.c_str());
	//m_textDataEdt;
	m_textType.setString(m_language.m_textTypeStr.c_str());
	//m_textDataCmb;
	m_textAddress.setString(m_language.m_textAddressStr.c_str());
	//m_textAddressEdt;
	m_textTimes.setString(m_language.m_textTimesStr.c_str());
	//m_textTimesEdt;
	m_textSpacing.setString(m_language.m_textSpacingStr.c_str());
	//m_textSpacingEdt;
	if(m_isSendState) m_sendBtn.setCaptionText(m_language.m_sendBtnCloseStr.c_str());
	else m_sendBtn.setCaptionText(m_language.m_sendBtnOpenStr.c_str());
	////XButton m_stopBtn;
	m_sendModeCmb.setMenuStr(m_language.m_sendModeCmbStr.c_str());
	//XMultiList m_dataLst;
	m_sendAsMsg.setCaptionText(m_language.m_sendAsMsgStr.c_str());
	m_supportACKMode.setCaptionText(m_language.m_supportACKModeStr.c_str());
	m_clearRecvBtn.setCaptionText(m_language.m_clearRecvBtnStr.c_str());	//������տ�İ�ť

	//m_languageCmb;		//����ѡ��

	////m_textRIP;
	////m_textRIPEdt;
	//m_textRPort;
	//m_textRPortEdt;
	if(m_recvState) m_openRBtn.setCaptionText(m_language.m_openRBtnOpenStr.c_str());
	else m_openRBtn.setCaptionText(m_language.m_openRBtnCloseStr.c_str());
	////m_closeRBtn;
	m_saveBtn.setCaptionText(m_language.m_saveBtnStr.c_str());		//���水ť
	m_readBtn.setCaptionText(m_language.m_readBtnStr.c_str());		//��ȡ�����ļ��İ�ť
		
	m_textIPEdt.setComment(m_language.m_textIPEdtCommentStr.c_str());
	m_textPortEdt.setComment(m_language.m_textPortEdtCommentStr.c_str());
	m_openBtn.setComment(m_language.m_openBtnCommentStr.c_str());
	m_addBtn.setComment(m_language.m_addBtnCommentStr.c_str());
	m_delBtn.setComment(m_language.m_delBtnCommentStr.c_str());
	m_clearBtn.setComment(m_language.m_clearBtnCommentStr.c_str());
	m_modifyBtn.setComment(m_language.m_modifyBtnCommentStr.c_str());
	m_textDataEdt.setComment(m_language.m_textDataEdtCommentStr.c_str());
	m_textDataCmb.setComment(m_language.m_textDataCmbCommentStr.c_str());
	m_textAddressEdt.setComment(m_language.m_textAddressEdtCommentStr.c_str());
	m_textTimesEdt.setComment(m_language.m_textTimesEdtCommentStr.c_str());
	m_textSpacingEdt.setComment(m_language.m_textSpacingEdtCommentStr.c_str());
	m_sendModeCmb.setComment(m_language.m_sendModeCmbCommentStr.c_str());
	m_sendAsMsg.setComment(m_language.m_sendAsMsgCommentStr.c_str());
	m_sendBtn.setComment(m_language.m_sendBtnCommentStr.c_str());
	m_textRPortEdt.setComment(m_language.m_textRPortEdtCommentStr.c_str());
	m_openRBtn.setComment(m_language.m_openRBtnCommentStr.c_str());
	m_supportACKMode.setComment(m_language.m_supportACKModeCommentStr.c_str());
	m_clearRecvBtn.setComment(m_language.m_clearRecvBtnCommentStr.c_str());
	m_languageCmb.setComment(m_language.m_languageCmbCommentStr.c_str());
	m_codeModeCmb.setComment(m_language.m_codeModeCmbCommentStr.c_str());
	m_saveBtn.setComment(m_language.m_saveBtnCommentStr.c_str());
	m_readBtn.setComment(m_language.m_readBtnCommentStr.c_str());
	m_addKMBtn.setComment(m_language.m_addKMBtnCommentStr.c_str());
	m_delKMBtn.setComment(m_language.m_delKMBtnCommentStr.c_str());
	m_clearKMBtn.setComment(m_language.m_clearKMBtnCommentStr.c_str());
	m_modifyKMBtn.setComment(m_language.m_modifyKMBtnCommentStr.c_str());
	m_keySelectCmb.setComment(m_language.m_keySelectCmbCommentStr.c_str());
	m_keyStateCmb.setComment(m_language.m_keyStateCmbCommentStr.c_str());
	m_oscDataEdt.setComment(m_language.m_oscDataEdtCommentStr.c_str());
	m_oscDataCmb.setComment(m_language.m_oscDataCmbCommentStr.c_str());
	m_oscAddressEdt.setComment(m_language.m_oscAddressEdtCommentStr.c_str());
}