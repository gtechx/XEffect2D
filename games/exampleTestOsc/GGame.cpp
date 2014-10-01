#include "GGame.h"
#include "systemHook.h"
#pragma comment(lib, "systemHook.lib")

void ctrlFunX(void *pClass,int id)
{
	_GGame &pPar = *(_GGame *)pClass;
	if(id == pPar.m_dataLst.getControlID())
	{//���˫���¼�
		pPar.sendButtonDown();
	}else
	if(id == pPar.m_dataKMLst.getControlID())
	{//���˫���¼�������ָ������
		pPar.sendSelectData();
	}
}
void ctrlFun(void *pClass,int id)
{
	_GGame &pPar = *(_GGame *)pClass;
	if(id == pPar.m_openBtn.getControlID())
	{//�򿪷���
		if(!pPar.m_isSenderOpen)
		{
			pPar.m_sender.setup(pPar.m_textIPEdt.getString(),atoi(pPar.m_textPortEdt.getString()));
			pPar.m_sender.setSendAsMessage(true);
			pPar.m_isSenderOpen = true;
			pPar.m_allMsgText.setString("�������,�Ƿ�ɹ����Լ�ȷ��.");
		//	pPar.m_openBtn.disable();
		//	pPar.m_closeBtn.enable();
			pPar.m_openBtn.setCaptionText("�ر�");
			pPar.m_textIPEdt.disable();
			pPar.m_textPortEdt.disable();
			if(pPar.m_keyMapOscList.size() > 0) startHook();//����hook
			pPar.m_mouseX = -1;
			pPar.m_mouseY = -1;
		}else
		{
			pPar.m_sender.setup("",0);
			pPar.m_isSenderOpen = false;
			pPar.m_allMsgText.setString("�ر����.");
		//	pPar.m_openBtn.enable();
		//	pPar.m_closeBtn.disable();
			pPar.m_openBtn.setCaptionText("��");
			pPar.m_textIPEdt.enable();
			pPar.m_textPortEdt.enable();
			if(pPar.m_keyMapOscList.size() > 0) stopHook();//�ر�hook
		}
	}else
	//if(id == pPar.m_closeBtn.getControlID())
	//{//�رշ���
	//	if(pPar.m_isSenderOpen)
	//	{
	//		pPar.m_sender.setup("",0);
	//		pPar.m_isSenderOpen = false;
	//		pPar.m_allMsgText.setString("�ر����.");
	//		pPar.m_openBtn.enable();
	//		pPar.m_closeBtn.disable();
	//		pPar.m_textIPEdt.enable();
	//		pPar.m_textPortEdt.enable();
	//		if(pPar.m_keyMapOscList.size() > 0) stopHook();//�ر�hook
	//	}else
	//	{
	//		pPar.m_allMsgText.setString("��δ����.");
	//	}
	//}else
	if(id == pPar.m_addBtn.getControlID())
	{//���
		if(!pPar.checkNowData())
		{
			pPar.m_allMsgText.setString("����������ʵ�ʵ����ݲ���.");
			return;
		}
		//�������
		_GOscData *tmp = createMem<_GOscData>();
		tmp->data = pPar.m_textDataEdt.getString();
		tmp->address = pPar.m_textAddressEdt.getString();
		tmp->dataType = pPar.m_textDataCmb.getNowChooseOrder();
		pPar.m_datasVector.push_back(tmp);
		//�����б�
		pPar.addALineData(tmp);
		pPar.m_allMsgText.setString("������ӳɹ�.");
	}else
	if(id == pPar.m_delBtn.getControlID())
	{//ɾ��
		//ɾ������
		int lineSum = pPar.m_dataLst.getSelectIndex();
		if(lineSum >= 0)
		{
			_GOscData *tmp = pPar.m_datasVector[lineSum];
			for(int i = lineSum;i < pPar.m_datasVector.size() - 1;++ i)
			{
				pPar.m_datasVector[i] = pPar.m_datasVector[i + 1];
			}
			pPar.m_datasVector.pop_back();
			XDELETE(tmp);
			pPar.m_dataLst.deleteLine(lineSum);
			pPar.m_allMsgText.setString("ɾ���ɹ�.");
		}else
		{
			pPar.m_allMsgText.setString("��ѡ��Ҫɾ��������.");
		}
	}else
	if(id == pPar.m_sendBtn.getControlID())
	{//����
		if(pPar.m_isSendState) pPar.sendOverProc();
		else pPar.sendButtonDown();
	}else
//	if(id == pPar.m_stopBtn.getControlID())
//	{//ֹͣ����
//		if(pPar.m_isSendState) pPar.sendOverProc();
//		else pPar.m_allMsgText.setString("û�з�������.");
//	}else
	if(id == pPar.m_openRBtn.getControlID())
	{//��������
		if(pPar.m_recvState)
		{
			pPar.m_recvState = false;
			pPar.m_receiver.setup(0);
			pPar.m_allMsgText.setString("�ѹرս���.");
		//	pPar.m_openRBtn.enable();
		//	pPar.m_closeRBtn.disable();
			pPar.m_openRBtn.setCaptionText("��ʼ����");
			pPar.m_textRPortEdt.enable();
			pPar.m_supportACKMode.enable();
		}else
		{
			pPar.m_recvState = true;
			pPar.m_receiver.setup(atoi(pPar.m_textRPortEdt.getString()));
			pPar.m_allMsgText.setString("�Ѿ���������,�Ƿ�ɹ��������ж�.");
		//	pPar.m_openRBtn.disable();
		//	pPar.m_closeRBtn.enable();
			pPar.m_openRBtn.setCaptionText("ֹͣ����");
			pPar.m_textRPortEdt.disable();
			pPar.m_supportACKMode.disable();
		}
	}else
	//if(id == pPar.m_closeRBtn.getControlID())
	//{//�رս���
	//	if(pPar.m_recvState)
	//	{
	//		pPar.m_recvState = false;
	//		pPar.m_receiver.setup(0);
	//		pPar.m_allMsgText.setString("�ѹرս���.");
	//		pPar.m_openRBtn.enable();
	//		pPar.m_closeRBtn.disable();
	//		pPar.m_textRPortEdt.enable();
	//		pPar.m_supportACKMode.enable();
	//	}else
	//	{
	//		pPar.m_allMsgText.setString("������δ����.");
	//	}
	//}else
	if(id == pPar.m_clearRecvBtn.getControlID())
	{
		pPar.m_recvText.setString("Recv:");
		pPar.m_allMsgText.setString("���տ������Ѿ����.");
	}else
	if(id == pPar.m_saveBtn.getControlID())
	{//����
		if(!pPar.saveToFile(getChooseFilename(CFG_FILENAME,"����",false).c_str()))
		{
			pPar.m_allMsgText.setString("д�������ļ�ʧ��.");
		}
	}else
	if(id == pPar.m_readBtn.getControlID()) 
	{//��ȡ
		if(pPar.m_recvState || pPar.m_isSendState)
		{
			pPar.m_allMsgText.setString("���ڽ��ܻ��߷���״̬���ܶ�ȡ�����ļ�.");
			return;
		}
		if(!pPar.readFromFile(getChooseFilename(CFG_FILENAME,"��ȡ",true).c_str()))
		{
			pPar.m_allMsgText.setString("��ȡ�����ļ�ʧ��.");
		}
	}else
	if(id == pPar.m_dataLst.getControlID())
	{//ѡ���������
		int index = pPar.m_dataLst.getSelectIndex();
		if(index >= 0)
		{
			pPar.m_textDataEdt.setString(pPar.m_datasVector[index]->data.c_str());
			pPar.m_textDataCmb.setNowChooseOrder(pPar.m_datasVector[index]->dataType);
			pPar.m_textAddressEdt.setString(pPar.m_datasVector[index]->address.c_str());
		}
	}else
	if(id == pPar.m_modifyBtn.getControlID())
	{//�޸�
		int index = pPar.m_dataLst.getSelectIndex();
		if(index >= 0 && pPar.checkNowData())
		{
			pPar.m_datasVector[index]->data = pPar.m_textDataEdt.getString();
			pPar.m_datasVector[index]->dataType = pPar.m_textDataCmb.getNowChooseOrder();
			pPar.m_datasVector[index]->address = pPar.m_textAddressEdt.getString();
			pPar.setLineData(pPar.m_datasVector[index],index);
			pPar.m_allMsgText.setString("�޸����.");
		}else
		{
			pPar.m_allMsgText.setString("��ѡ����Ҫ�޸ĵ�����.");
		}
	}else
	if(id == pPar.m_clearBtn.getControlID())
	{//���
		if(pPar.m_datasVector.size() <= 0)
		{
			pPar.m_allMsgText.setString("�б���û������.");
			return;
		}
		pPar.m_dataLst.setLineSum(0);
		for(int i = 0;i < pPar.m_datasVector.size();++ i)
		{
			XDELETE(pPar.m_datasVector[i]);
		}
		pPar.m_datasVector.clear();
		pPar.m_allMsgText.setString("������.");
	}else
	if(id == pPar.m_addKMBtn.getControlID())
	{//���
		if(!pPar.checkKMData())
		{
			pPar.m_allMsgText.setString("�������ݲ��Ϸ������Ѿ�����.");
			return;
		}
		//�������
		_GKMOscData *tmp = createMem<_GKMOscData>();
		tmp->keyValue = pPar.m_keySelectCmb.getNowChooseOrder();
		tmp->keyState = pPar.m_keyStateCmb.getNowChooseOrder();
		if(tmp->keyValue == 39 || tmp->keyValue == 40)
		{
			tmp->data = "0 - 255";
			tmp->address = pPar.m_oscAddressEdt.getString();
			tmp->dataType = 0;
		}else
		{
			tmp->data = pPar.m_oscDataEdt.getString();
			tmp->address = pPar.m_oscAddressEdt.getString();
			tmp->dataType = pPar.m_oscDataCmb.getNowChooseOrder();
		}
		pPar.m_keyMapOscList.push_back(tmp);
		//�����б�
		pPar.addALineData(tmp);
		pPar.m_allMsgText.setString("������ӳɹ�.");
	}else
	if(id == pPar.m_delKMBtn.getControlID())
	{//ɾ��
		//ɾ������
		int lineSum = pPar.m_dataKMLst.getSelectIndex();
		if(lineSum >= 0)
		{
			_GKMOscData *tmp = pPar.m_keyMapOscList[lineSum];
			for(int i = lineSum;i < pPar.m_keyMapOscList.size() - 1;++ i)
			{
				pPar.m_keyMapOscList[i] = pPar.m_keyMapOscList[i + 1];
			}
			pPar.m_keyMapOscList.pop_back();
			XDELETE(tmp);
			pPar.m_dataKMLst.deleteLine(lineSum);
			pPar.m_allMsgText.setString("ɾ���ɹ�.");
		}else
		{
			pPar.m_allMsgText.setString("��ѡ��Ҫɾ��������.");
		}
	}else
	if(id == pPar.m_clearKMBtn.getControlID())
	{//���
		if(pPar.m_keyMapOscList.size() <= 0)
		{
			pPar.m_allMsgText.setString("�б���û������.");
			return;
		}
		pPar.m_dataKMLst.setLineSum(0);
		for(int i = 0;i < pPar.m_keyMapOscList.size();++ i)
		{
			XDELETE(pPar.m_keyMapOscList[i]);
		}
		pPar.m_keyMapOscList.clear();
		pPar.m_allMsgText.setString("������.");
	}else
	if(id == pPar.m_modifyKMBtn.getControlID())
	{//�޸�
		int index = pPar.m_dataKMLst.getSelectIndex();
		if(index >= 0 && pPar.checkKMData(index))
		{
			pPar.m_keyMapOscList[index]->keyValue = pPar.m_keySelectCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->keyState = pPar.m_keyStateCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->data = pPar.m_oscDataEdt.getString();
			pPar.m_keyMapOscList[index]->dataType = pPar.m_oscDataCmb.getNowChooseOrder();
			pPar.m_keyMapOscList[index]->address = pPar.m_oscAddressEdt.getString();
			pPar.setLineData(pPar.m_keyMapOscList[index],index);
			pPar.m_allMsgText.setString("�޸����.");
		}else
		{
			pPar.m_allMsgText.setString("��ѡ����Ҫ�޸ĵ�����.");
		}
	}else
	if(id == pPar.m_dataKMLst.getControlID())
	{//ѡ���������
		int index = pPar.m_dataKMLst.getSelectIndex();
		if(index >= 0)
		{
			pPar.m_keySelectCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->keyValue);
			pPar.m_keyStateCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->keyState);
			pPar.m_oscDataEdt.setString(pPar.m_keyMapOscList[index]->data.c_str());
			pPar.m_oscDataCmb.setNowChooseOrder(pPar.m_keyMapOscList[index]->dataType);
			pPar.m_oscAddressEdt.setString(pPar.m_keyMapOscList[index]->address.c_str());
		}
	}
}
_XBool _GGame::checkNowData()
{
	switch(m_textDataCmb.getNowChooseOrder())
	{
	case 0:	//int32
	case 1:	//int64
	case 2:	//float
		if(!getIsNumber(m_textDataEdt.getString())) return XFalse;
		break;
	case 3:	//string
		break;
	case 4:	//blob	//��δ֧��
		break;
	}
	return XTrue;
}
_XBool _GGame::checkKMData(int index)
{
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		if(m_keyMapOscList[i]->keyValue == m_keySelectCmb.getNowChooseOrder() &&
			m_keyMapOscList[i]->keyState == m_keyStateCmb.getNowChooseOrder() && 
			i != index) return XFalse;	//��ͬ���趨���ܴ���
	}
	if(m_keySelectCmb.getNowChooseOrder() != 39 &&
		m_keySelectCmb.getNowChooseOrder() != 40)
	{
		switch(m_oscDataCmb.getNowChooseOrder())
		{
		case 0:	//int32
		case 1:	//int64
		case 2:	//float
			if(!getIsNumber(m_oscDataEdt.getString())) return XFalse;
			break;
		case 3:	//string
			break;
		case 4:	//blob	//��δ֧��
			break;
		}
	}
	return XTrue;
}
void _GGame::sendOverProc()
{
	m_isSendState = false;
	m_allMsgText.setString("��ֹͣ.");
	//���ｫ��صĿؼ�ʹ��(��δ���)
	m_textIPEdt.enable();
	m_textPortEdt.enable();
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
	m_sendBtn.setCaptionText("��ʼ����");
//	m_stopBtn.enable();	//�����ǳ��ϸ���߼�
	m_sendAsMsg.enable();
	m_sendModeCmb.enable();
}
void _GGame::sendSelectData()
{
	if(!m_isSenderOpen)
	{
		m_allMsgText.setString("������δ��,���ȴ�����.");
		return;
	}
	//���淢��ѡ������
	int index = m_dataKMLst.getSelectIndex();
	if(index >= 0) sendKMData(index);
}
void _GGame::sendKMData(int index)
{
	ofxOscMessage m;
	m.setAddress(m_keyMapOscList[index]->address);
	LogNull("����һ������-��ַ:%s %d",m_keyMapOscList[index]->address.c_str(),index);
	if(m_keyMapOscList[index]->keyValue == 39)
	{
		m.addIntArg(m_mouseX);
	}else
	if(m_keyMapOscList[index]->keyValue == 40)
	{
		m.addIntArg(m_mouseY);
	}else
	{
		switch(m_keyMapOscList[index]->dataType)
		{
		case 0:	//int32
			m.addIntArg(atoi(m_keyMapOscList[index]->data.c_str()));
			break;
		case 1:	//int64
			m.addInt64Arg(atoi(m_keyMapOscList[index]->data.c_str()));
			break;
		case 2:	//float
			m.addFloatArg(atof(m_keyMapOscList[index]->data.c_str()));
			break;
		case 3:	//string
			m.addStringArg(m_keyMapOscList[index]->data);
			break;
		case 4:	//blob����֧��
			break;
		}
	}
	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
}
void _GGame::sendButtonDown()
{
	if(!m_isSenderOpen)
	{
		m_allMsgText.setString("������δ��,���ȴ�����.");
		return;
	}
	if(m_isSendState)
	{
		m_allMsgText.setString("���ڷ�������,���Ե�.");
	}else
	{//���濪ʼ��������
		if(m_sendModeCmb.getNowChooseOrder() == 0
			|| m_sendModeCmb.getNowChooseOrder() == 1)
		{
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				m_allMsgText.setString("��ѡ��һ����Ҫ���͵�OSC��ַ.");
				return;
			}
			m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//��ȡ��Ҫ���͵�OSC��ַ
		}
		if(m_sendModeCmb.getNowChooseOrder() == 2)
		{//���Ϳ�����
			int tmp = m_dataLst.getSelectIndex();
			if(tmp < 0)
			{
				if(m_textAddressEdt.getString() != NULL && strlen(m_textAddressEdt.getString()) > 0)
				{
					m_nowSendAddr = m_textAddressEdt.getString();
				}else
				{
					m_allMsgText.setString("��ѡ�����������Ҫ���͵�OSC��ַ.");
					return;
				}
			}else
			{
				m_nowSendAddr = m_dataLst.getBoxStr(tmp,0);	//��ȡ��Ҫ���͵�OSC��ַ
			}
		}
		if(m_sendModeCmb.getNowChooseOrder() == 3 && !checkNowData())
		{//����Ƿ��͵�ǰ���ݵĻ�����鵱ǰ�����Ƿ�Ϸ�
			m_allMsgText.setString("��ǰ���ݲ��Ϸ�,��ȷ���������ͺ�����ƥ��.");
			return;
		}
		m_setTimer = atoi(m_textSpacingEdt.getString());
		m_setTimes = atoi(m_textTimesEdt.getString());
		if(m_setTimer <= 0) m_setTimer = 0;
		if(m_setTimes <= 0) m_setTimes = 0;
		m_isSendState = true;
		m_timer = 0;
		m_times = 0;
		m_allMsgText.setString("�Ѿ���ʼ��������.");
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
		m_sendBtn.setCaptionText("ֹͣ����");
		m_sendAsMsg.disable();
		m_sendModeCmb.disable();
	}
}
void windowExitFun(void *p)
{
	_GGame &pPar = *(_GGame *)p;
	if(XEE::windowData.withFrame == 0)
	{//û�б߿��ʱ������ⲽ����
		pPar.m_moveData.set(1.0f,0.0f,0.002f);
		pPar.m_moveData.reSet();
		pPar.m_isExit = true;
		getWindowPos(pPar.m_windowPosX,pPar.m_windowPosY);
		getWindowSize(pPar.m_windowSizeW,pPar.m_windowSizeH);
	}else
	{
		exitWindow();
	}
}
bool _GGame::init()
{
	setVSync(false);
	getWindowPos(m_windowPosX,m_windowPosY);
	getWindowSize(m_windowSizeW,m_windowSizeH);
	m_moveData.set(0.0f,1.0f,0.002f);
	XEE::customWinTitle.setCallbackFun(windowExitFun,NULL,this);
	m_isFirst = true;
	m_isExit = false;

	m_tab.initWithoutTex(_XVector2(1014.0f,725.0f));
	m_tab.addATab("���Ͷ˿�����");
	m_tab.addATab("������������");
	m_tab.addATab("�������ģ��");
	m_tab.addATab("���ն�����");
	m_tab.addATab("ϵͳ����");
	m_tab.setPosition(5.0f,5.0f);

	int h = 5 + 40;
	m_textIP.init();
	m_textIP.setPosition(10.0f,h);
	m_textIP.setString("IP:");
	m_textIPEdt.initWithoutTex(_XVector2(256.0f,32.0f),"192.168.0.1");
	m_textIPEdt.setPosition(60.0f,h);
	m_textIPEdt.setComment("����OSC���Ͷ˵�IP��ַ��һ��Ϊ����IP��Ҳ��������127.0.0.1");
	m_tab.addObjToTab(&m_textIP,"���Ͷ˿�����");
	m_tab.addObjToTab(&m_textIPEdt,"���Ͷ˿�����");

	m_textPort.init();
	m_textPort.setPosition(320.0f,h);
	m_textPort.setString("Port:");
	m_textPortEdt.initWithoutTex(_XVector2(128.0f,32.0f),"12345");
	m_textPortEdt.setPosition(400.0f,h);
	m_textPortEdt.setComment("����OSC���Ͷ˵Ķ˿ںţ��˿ں���Ҫ����ն˵Ķ˿ں�һ�£�Ĭ��Ϊ12345");
	m_tab.addObjToTab(&m_textPort,"���Ͷ˿�����");
	m_tab.addObjToTab(&m_textPortEdt,"���Ͷ˿�����");

	m_openBtn.initWithoutTex("��",_XVector2(128.0f,32.0f));
	m_openBtn.setPosition(532.0f,h);
	m_openBtn.setMouseDownCB(ctrlFun,this);
	m_openBtn.enable();
	m_openBtn.setWithAction(XTrue);
	m_openBtn.setComment("�򿪻��߹ر�OSC��������ȷ��IP��ַ�Ͷ˿���ȷ�����򲻼������ȷ��");
//	m_closeBtn.initWithoutTex("�ر�",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_closeBtn.setPosition(664.0f,h);
//	m_closeBtn.setMouseDownCB(ctrlFun,this);
//	m_closeBtn.disable();
//	m_closeBtn.setWithAction(XTrue);
	m_tab.addObjToTab(&m_openBtn,"���Ͷ˿�����");
//	m_tab.addObjToTab(&m_closeBtn,"���Ͷ˿�����");

	h = 5 + 40;
	m_addBtn.initWithoutTex("���",_XVector2(70.0f,32.0f));
	m_addBtn.setPosition(10.0f,h);
	m_addBtn.setMouseDownCB(ctrlFun,this);
	m_addBtn.setComment("����߱༭�õ�OSC������Ϣ��ӵ��ұߵ��б����Ա���֮�����");
	m_delBtn.initWithoutTex("ɾ��",_XVector2(70.0f,32.0f));
	m_delBtn.setPosition(84.0f,h);
	m_delBtn.setMouseDownCB(ctrlFun,this);
	m_delBtn.setComment("ɾ������б���ѡ�е�����");
	m_clearBtn.initWithoutTex("���",_XVector2(70.0f,32.0f));
	m_clearBtn.setPosition(158.0f,h);
	m_clearBtn.setMouseDownCB(ctrlFun,this);
	m_clearBtn.setComment("����ұ��б���е���������");
	m_modifyBtn.initWithoutTex("�޸�",_XVector2(70.0f,32.0f));
	m_modifyBtn.setPosition(232.0f,h);
	m_modifyBtn.setMouseDownCB(ctrlFun,this);
	m_modifyBtn.setComment("���ұ��б���ѡ��һ�����ݲ��޸ģ����¸ð�ť���޸ĺ�����ݸ��µ��б���");
	m_tab.addObjToTab(&m_addBtn,"������������");
	m_tab.addObjToTab(&m_delBtn,"������������");
	m_tab.addObjToTab(&m_clearBtn,"������������");
	m_tab.addObjToTab(&m_modifyBtn,"������������");

	m_dataLst.initWithoutTex(_XVector2(709.0f,670.0f),3,0);
	m_dataLst.setPosition(306.0f,h);
	m_dataLst.setTitleStr("Address",0);
	m_dataLst.setRowWidth(250,0);
	m_dataLst.setTitleStr("Data",1);
	m_dataLst.setRowWidth(250,1);
	m_dataLst.setTitleStr("Type",2);
	m_dataLst.setRowWidth(250,2);
	m_dataLst.setWithMouseDrag(false);
	m_dataLst.setSelectFun(ctrlFun,this);
	m_dataLst.setDClickFun(ctrlFunX,this);
	m_tab.addObjToTab(&m_dataLst,"������������");

	h += 35;
	m_textData.init();
	m_textData.setPosition(10.0f,h);
	m_textData.setString("Data:");
	m_textDataEdt.initWithoutTex(_XVector2(204.0f,32.0f),"1234567");
	m_textDataEdt.setPosition(98.0f,h);
	m_textDataEdt.setComment("OSC��Ϣ�ж�Ӧ�����ݣ���ȷ��������������������������ƥ��");
	m_tab.addObjToTab(&m_textData,"������������");
	m_tab.addObjToTab(&m_textDataEdt,"������������");

	h += 35;
	m_textType.init();
	m_textType.setPosition(10.0f,h);
	m_textType.setString("Type:");
	m_textDataCmb.initWithoutTex(170,5,3);
	m_textDataCmb.setMenuStr("Int32",0);
	m_textDataCmb.setMenuStr("Int64",1);
	m_textDataCmb.setMenuStr("Float",2);
	m_textDataCmb.setMenuStr("String",3);
	m_textDataCmb.setMenuStr("Blob",4);
	m_textDataCmb.setPosition(98.0f,h);
	m_textDataCmb.setComment("OSC��Ϣ�����ݵ��������ͣ���ȷ����������������ƥ��");
	m_tab.addObjToTab(&m_textType,"������������");
	m_tab.addObjToTab(&m_textDataCmb,"������������");
	h += 37;
	m_textAddress.init();
	m_textAddress.setPosition(10.0f,h);
	m_textAddress.setString("Address:");
	m_textAddressEdt.initWithoutTex(_XVector2(162.0f,32.0f),"/data");
	m_textAddressEdt.setPosition(140.0f,h);
	m_textAddressEdt.setComment("OSC��Ϣ�е�ַ");
	m_tab.addObjToTab(&m_textAddress,"������������");
	m_tab.addObjToTab(&m_textAddressEdt,"������������");
	h += 35;
	m_textTimes.init();
	m_textTimes.setPosition(10.0f,h);
	m_textTimes.setString("Times:");
	m_textTimesEdt.initWithoutTex(_XVector2(162.0f,32.0f),"1");
	m_textTimesEdt.setPosition(140.0f,h);
	m_textTimesEdt.setComment("OSC��Ϣ��Ҫѭ�����͵Ĵ������������ͽ�����������ֵ��ѭ������");
	m_tab.addObjToTab(&m_textTimes,"������������");
	m_tab.addObjToTab(&m_textTimesEdt,"������������");
	h += 35;
	m_textSpacing.init();
	m_textSpacing.setPosition(10.0f,h);
	m_textSpacing.setString("Spacing:");
	m_textSpacingEdt.initWithoutTex(_XVector2(162.0f,32.0f),"1");
	m_textSpacingEdt.setPosition(140.0f,h);
	m_textSpacingEdt.setComment("���η���֮��ļ��ʱ�䣬��λΪ����(ms)��1����Ϊǧ��֮һ��");
	m_tab.addObjToTab(&m_textSpacing,"������������");
	m_tab.addObjToTab(&m_textSpacingEdt,"������������");
	h += 35;
	m_sendModeCmb.initWithoutTex(258,4,3);
	m_sendModeCmb.setMenuStr("����ѡ�е�ַ",0);
	m_sendModeCmb.setMenuStr("����ѡ������",1);
	m_sendModeCmb.setMenuStr("���Ϳ�����",2);
	m_sendModeCmb.setMenuStr("���͵�ǰ����",3);
	m_sendModeCmb.setPosition(10.0f,h);
	m_sendModeCmb.setNowChooseOrder(1);	//Ĭ��Ϊ����ѡ������
	m_sendModeCmb.setComment("ѡ����Ҫ�ķ���ģʽ���������ַ���ģʽ���������ʣ������Ķ�˵���ĵ�");
	m_tab.addObjToTab(&m_sendModeCmb,"������������");
	h += 37;
	m_sendAsMsg.initWithoutTex("��Ϊ��Ϣ����",_XVector2(32.0f,32.0f));
	m_sendAsMsg.setPosition(10.0f,h);
	m_sendAsMsg.setComment("��ѡʱ��Ҫ���͵�������Ϊһ����Ϣ���ͣ�������Ϊһ�����ݰ�����");
	m_tab.addObjToTab(&m_sendAsMsg,"������������");
	h += 35;
	m_sendBtn.initWithoutTex("��ʼ����",_XVector2(292.0f,32.0f));
	m_sendBtn.setPosition(10.0f,h);
	m_sendBtn.setMouseDownCB(ctrlFun,this);
	m_sendBtn.setComment("�����趨�������ݻ�ֹͣ���ͣ�����ǰ����ȷ��OSC�������Ѿ���ȷ��");
//	m_stopBtn.initWithoutTex("ֹͣ",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_stopBtn.setPosition(142.0f,h);
//	m_stopBtn.setMouseDownCB(ctrlFun,this);
	m_tab.addObjToTab(&m_sendBtn,"������������");
//	m_tab.addObjToTab(&m_stopBtn,"������������");
	h = 5 + 40;
	m_recvText.initWithoutTex(_XVector2(740.0f,670.0f),"Recv:");
	m_recvText.setPosition(275.0f,h);
//	m_textRIP.setACopy(m_textIP);
//	m_textRIP.setPosition(10.0f,h.0f);
//	m_textRIP.setString("IP:");
//	m_textRIPEdt.initWithoutTex(_XRect(0.0f,0.0f,256.0f,32.0f),"192.168.0.1",XEE::systemFont);
//	m_textRIPEdt.setPosition(100.0f,h.0f);
	m_textRPort.init();
	m_textRPort.setPosition(10.0f,h);
	m_textRPort.setString("Port:");
	m_textRPortEdt.initWithoutTex(_XVector2(172.0f,32.0f),"12345");
	m_textRPortEdt.setPosition(98.0f,h);
	m_textRPortEdt.setComment("OSC�������Ķ˿ںţ���������Ӧ�������Ķ˿ں�һ�²�����ȷ���գ�Ĭ��Ϊ12345");
	m_tab.addObjToTab(&m_recvText,"���ն�����");
	m_tab.addObjToTab(&m_textRPort,"���ն�����");
	m_tab.addObjToTab(&m_textRPortEdt,"���ն�����");
	h += 35;
	m_openRBtn.initWithoutTex("��ʼ����",_XVector2(260.0f,32.0f));
	m_openRBtn.setPosition(10.0f,h);
	m_openRBtn.setMouseDownCB(ctrlFun,this);
	m_openRBtn.enable();
	m_openRBtn.setComment("�򿪻�ر�OSC����������ȷ���������˿ں�������ȷ");
//	m_closeRBtn.initWithoutTex("�ر�",XEE::systemFont,_XRect(0.0f,0.0f,128.0f,32.0f));
//	m_closeRBtn.setPosition(142.0f,h);
//	m_closeRBtn.setMouseDownCB(ctrlFun,this);
//	m_closeRBtn.disable();
	m_tab.addObjToTab(&m_openRBtn,"���ն�����");
//	m_tab.addObjToTab(&m_closeRBtn,"���ն�����");
	h += 35;
	m_supportACKMode.initWithoutTex("�Ƿ��ʴ����",_XVector2(32.0f,32.0f));
	m_supportACKMode.setPosition(10.0f,h);
	m_supportACKMode.setComment("Ӧ��ģʽ������ʱ�������յ�ĳ��ַ�Ŀ�����ʱ�����Զ����ظõ�ַ������");
	m_tab.addObjToTab(&m_supportACKMode,"���ն�����");
	h += 35;
	m_clearRecvBtn.initWithoutTex("������տ�",_XVector2(260.0f,32.0f));
	m_clearRecvBtn.setPosition(10.0f,h);
	m_clearRecvBtn.setMouseDownCB(ctrlFun,this);
	m_clearRecvBtn.setComment("���¸ð�ť��ս����б������б��е��������ݽ������");
	m_tab.addObjToTab(&m_clearRecvBtn,"���ն�����");

	h = 5 + 40;
	m_saveBtn.initWithoutTex("����",_XVector2(128.0f,32.0f));
	m_saveBtn.setPosition(10.0f,h);
	m_saveBtn.setMouseDownCB(ctrlFun,this);
	m_saveBtn.setComment("����ǰ�����ϵ��������ݱ��浽�ļ���");
	m_readBtn.initWithoutTex("��ȡ",_XVector2(128.0f,32.0f));
	m_readBtn.setPosition(142.0f,h);
	m_readBtn.setMouseDownCB(ctrlFun,this);
	m_readBtn.setComment("�������ļ��ж�ȡ��һ�α������������");
	m_tab.addObjToTab(&m_saveBtn,"ϵͳ����");
	m_tab.addObjToTab(&m_readBtn,"ϵͳ����");
	//�����Ǽ������ӳ��
	h = 5 + 40;
	m_dataKMLst.initWithoutTex(_XVector2(709.0f,670.0f),5,0);
	m_dataKMLst.setPosition(306.0f,h);
	m_dataKMLst.setTitleStr("Key",0);
	m_dataKMLst.setRowWidth(100,0);
	m_dataKMLst.setTitleStr("State",1);
	m_dataKMLst.setRowWidth(100,1);
	m_dataKMLst.setTitleStr("Address",2);
	m_dataKMLst.setRowWidth(200,2);
	m_dataKMLst.setTitleStr("Data",3);
	m_dataKMLst.setRowWidth(200,3);
	m_dataKMLst.setTitleStr("Type",4);
	m_dataKMLst.setRowWidth(200,4);
	m_dataKMLst.setWithMouseDrag(false);
	m_dataKMLst.setSelectFun(ctrlFun,this);
	m_dataKMLst.setDClickFun(ctrlFunX,this);
	m_tab.addObjToTab(&m_dataKMLst,"�������ģ��");
	m_addKMBtn.initWithoutTex("���",_XVector2(70.0f,32.0f));
	m_addKMBtn.setPosition(10.0f,h);
	m_addKMBtn.setMouseDownCB(ctrlFun,this);
	m_addKMBtn.setComment("����߱༭�õ���������OSC��ӳ����ӵ�ӳ���б���");
	m_delKMBtn.initWithoutTex("ɾ��",_XVector2(70.0f,32.0f));
	m_delKMBtn.setPosition(84.0f,h);
	m_delKMBtn.setMouseDownCB(ctrlFun,this);
	m_delKMBtn.setComment("ɾ��ӳ���б���ѡ�����");
	m_clearKMBtn.initWithoutTex("���",_XVector2(70.0f,32.0f));
	m_clearKMBtn.setPosition(158.0f,h);
	m_clearKMBtn.setMouseDownCB(ctrlFun,this);
	m_clearKMBtn.setComment("�����ߵ�ӳ���б�");
	m_modifyKMBtn.initWithoutTex("�޸�",_XVector2(70.0f,32.0f));
	m_modifyKMBtn.setPosition(232.0f,h);
	m_modifyKMBtn.setMouseDownCB(ctrlFun,this);
	m_modifyKMBtn.setComment("����߱༭�õ�ֵ���µ�ӳ���б���ѡ�е���");
	m_tab.addObjToTab(&m_addKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_delKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_clearKMBtn,"�������ģ��");
	m_tab.addObjToTab(&m_modifyKMBtn,"�������ģ��");
	h += 35;
	m_keySelectCmb.initWithoutTex(115,41,10);
	m_keySelectCmb.setMenuStr("Key_0",0);
	m_keySelectCmb.setMenuStr("Key_1",1);
	m_keySelectCmb.setMenuStr("Key_2",2);
	m_keySelectCmb.setMenuStr("Key_3",3);
	m_keySelectCmb.setMenuStr("Key_4",4);
	m_keySelectCmb.setMenuStr("Key_5",5);
	m_keySelectCmb.setMenuStr("Key_6",6);
	m_keySelectCmb.setMenuStr("Key_7",7);
	m_keySelectCmb.setMenuStr("Key_8",8);
	m_keySelectCmb.setMenuStr("Key_9",9);
	m_keySelectCmb.setMenuStr("Key_A",10);
	m_keySelectCmb.setMenuStr("Key_B",11);
	m_keySelectCmb.setMenuStr("Key_C",12);
	m_keySelectCmb.setMenuStr("Key_D",13);
	m_keySelectCmb.setMenuStr("Key_E",14);
	m_keySelectCmb.setMenuStr("Key_F",15);
	m_keySelectCmb.setMenuStr("Key_G",16);
	m_keySelectCmb.setMenuStr("Key_H",17);
	m_keySelectCmb.setMenuStr("Key_I",18);
	m_keySelectCmb.setMenuStr("Key_J",19);
	m_keySelectCmb.setMenuStr("Key_K",20);
	m_keySelectCmb.setMenuStr("Key_L",21);
	m_keySelectCmb.setMenuStr("Key_M",22);
	m_keySelectCmb.setMenuStr("Key_N",23);
	m_keySelectCmb.setMenuStr("Key_O",24);
	m_keySelectCmb.setMenuStr("Key_P",25);
	m_keySelectCmb.setMenuStr("Key_Q",26);
	m_keySelectCmb.setMenuStr("Key_R",27);
	m_keySelectCmb.setMenuStr("Key_S",28);
	m_keySelectCmb.setMenuStr("Key_T",29);
	m_keySelectCmb.setMenuStr("Key_U",30);
	m_keySelectCmb.setMenuStr("Key_V",31);
	m_keySelectCmb.setMenuStr("Key_W",32);
	m_keySelectCmb.setMenuStr("Key_X",33);
	m_keySelectCmb.setMenuStr("Key_Y",34);
	m_keySelectCmb.setMenuStr("Key_Z",35);
	m_keySelectCmb.setMenuStr("Mus_L",36);
	m_keySelectCmb.setMenuStr("Mus_M",37);
	m_keySelectCmb.setMenuStr("Mus_R",38);
	m_keySelectCmb.setMenuStr("Mus_X",39);
	m_keySelectCmb.setMenuStr("Mus_Y",40);
	m_keySelectCmb.setPosition(10.0f,h);
	m_keySelectCmb.setComment("��Ӧ�������߼��̰���");
	m_keyStateCmb.initWithoutTex(105,2,2);
	m_keyStateCmb.setMenuStr("Down",0);
	m_keyStateCmb.setMenuStr("Up",1);
	m_keyStateCmb.setPosition(163.0f,h);
	m_keyStateCmb.setComment("������״̬");
	m_tab.addObjToTab(&m_keySelectCmb,"�������ģ��");
	m_tab.addObjToTab(&m_keyStateCmb,"�������ģ��");
	h += 37;
	m_oscDataTxt.init();
	m_oscDataTxt.setPosition(10.0f,h);
	m_oscDataTxt.setString("Data:");
	m_oscDataEdt.initWithoutTex(_XVector2(204.0f,32.0f),"1234567");
	m_oscDataEdt.setPosition(98.0f,h);
	m_oscDataEdt.setComment("OSC����");
	m_tab.addObjToTab(&m_oscDataTxt,"�������ģ��");
	m_tab.addObjToTab(&m_oscDataEdt,"�������ģ��");
	h += 35;
	m_oscType.init();
	m_oscType.setPosition(10.0f,h);
	m_oscType.setString("Type:");
	m_oscDataCmb.initWithoutTex(170,5,3);
	m_oscDataCmb.setMenuStr("Int32",0);
	m_oscDataCmb.setMenuStr("Int64",1);
	m_oscDataCmb.setMenuStr("Float",2);
	m_oscDataCmb.setMenuStr("String",3);
	m_oscDataCmb.setMenuStr("Blob",4);
	m_oscDataCmb.setPosition(98.0f,h);
	m_oscDataCmb.setComment("OSC��������");
	m_tab.addObjToTab(&m_oscType,"�������ģ��");
	m_tab.addObjToTab(&m_oscDataCmb,"�������ģ��");
	h += 37;
	m_oscAddress.init();
	m_oscAddress.setPosition(10.0f,h);
	m_oscAddress.setString("Address:");
	m_oscAddressEdt.initWithoutTex(_XVector2(162.0f,32.0f),"/data");
	m_oscAddressEdt.setPosition(140.0f,h);
	m_oscAddressEdt.setComment("OSC��ַ");
	m_tab.addObjToTab(&m_oscAddress,"�������ģ��");
	m_tab.addObjToTab(&m_oscAddressEdt,"�������ģ��");

	m_allMsgText.setACopy(XEE::systemFont);
	m_allMsgText.setString("ϵͳ��Ϣ");
	m_allMsgText.setPosition(512.0f,748.0f);
	m_allMsgText.setMaxStrLen(512);

	readFromFile();
	LogStr("��ʼ�����");
	return true;
}
void _GGame::sendNowData(int mode)
{//�����Ƿ���,������Ҫ�޸�Ϊ����ָ����ַ������
	ofxOscMessage m;
	if(mode == 0)
	{//����ָ����ַ������
		m.setAddress(m_nowSendAddr);
		LogNull("����һ������-��ַ:%s",m_nowSendAddr.c_str());
		for(int i = 0;i < m_datasVector.size();++ i)
		{
			if(m_datasVector[i]->address == m_nowSendAddr)
			{//���η���ָ����ַ������
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
					m.addStringArg(m_datasVector[i]->data);
					break;
				case 4:	//blob����֧��
					break;
				}
			}
		}
	}else
	if(mode == 1)
	{//���͵���ѡ������
		m.setAddress(m_nowSendAddr);
		int i = m_dataLst.getSelectIndex();
		LogNull("����һ������-��ַ:%s %d",m_nowSendAddr.c_str(),i);
		if(i >= 0)
		{
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
				m.addStringArg(m_datasVector[i]->data);
				break;
			case 4:	//blob����֧��
				break;
			}
		}
	}else
	if(mode == 2)
	{//���Ϳ�����
		m.setAddress(m_nowSendAddr);
		LogNull("����һ��������-��ַ:%s",m_nowSendAddr.c_str());
	}else
	{//���͵�ǰ����
		m.setAddress(m_textAddressEdt.getString());
		switch(m_textDataCmb.getNowChooseOrder())
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
			m.addStringArg(m_textDataEdt.getString());
			break;
		case 4:	//blob����֧��
			break;
		}
	}

	if(m_sendAsMsg.getState()) m_sender.sendMessage(m);
	else
	{
		ofxOscBundle b;
		b.addMessage(m);
		m_sender.sendBundle(b);
	}
}
void _GGame::move(int stepTime)
{
	if(!m_moveData.getIsEnd())
	{
		if(m_isFirst) m_moveData.move(0);
		else m_moveData.move(stepTime);
		float tmpData = m_moveData.getNowData();
		if(XEE::windowData.withFrame == 0)
		{
			setWindowPos(m_windowPosX,
				m_windowPosY + m_windowSizeH * 0.5f * (1.0f - tmpData));
			setWindowSize(m_windowSizeW,m_windowSizeH * tmpData);
			if(m_isExit && m_moveData.getIsEnd()) exitWindow();
		}
	}
	if(m_isFirst) m_isFirst = false;
	ctrlStateManager();
	if(m_recvState)
	{//��������
		while(m_receiver.hasWaitingMessages())
		{
			ofxOscMessage m;
			m_receiver.getNextMessage(&m);
			//����Ϣ���뵽��ʾ��
			++m_recvSum;
			std::string recvData = "";
			char tmpStr[1024];
			if(isUTF8(m.getAddress()))
			{
				//printf("UTF8\n");
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,UTF82ANSI(m.getAddress().c_str()).c_str());
			}else
			{
				sprintf(tmpStr,"%d-Addr:%s ",m_recvSum,m.getAddress().c_str());
			}
			recvData += tmpStr;
			LogNull("���յ�һ������-��ַ:%s",tmpStr);
			for(int i = 0; i < m.getNumArgs(); i++)
			{//get the argument type
				recvData += "|Type:" + m.getArgTypeName(i);
				recvData += ":";
				switch(m.getArgType(i))
				{
				case OFXOSC_TYPE_INT32:
					recvData += toString(m.getArgAsInt32(i));
					break;
				case OFXOSC_TYPE_INT64:
					recvData += toString(m.getArgAsInt64(i));
					break;
				case OFXOSC_TYPE_FLOAT:
					recvData += toString(m.getArgAsFloat(i));
					break;
				case OFXOSC_TYPE_STRING:
					if(isUTF8(m.getArgAsString(i)))
					{
						//printf("UTF8\n");
						recvData += toString(UTF82ANSI(m.getArgAsString(i).c_str()));
					}else
					{
						recvData += toString(m.getArgAsString(i));
					}
					break;
				default:
					recvData += "unknown";
					break;
				}
			}
			m_recvText.addALine(recvData.c_str());
			if(m_supportACKMode.getState())
			{//���֧���ʴ���ƵĻ���������Ҫ����Ƿ���Ҫ�ʴ�
				if(strcmp(m.getAddress().c_str(),m_textAddressEdt.getString()) == 0
					&& m.getNumArgs() == 0)
				{//��ַ�Ƕ�Ӧ��
					sendNowData(0);
				}
			}
		}
	}
	if(m_isSendState)
	{//��������
		m_timer += stepTime;
		if(m_timer > m_setTimer)
		{//����һ��
			m_timer = 0;
			sendNowData(m_sendModeCmb.getNowChooseOrder());

			++m_times;
			if(m_times >= m_setTimes) sendOverProc();//�������
		}
	}
	if(m_mouseOnTimer < 10000) m_mouseOnTimer += stepTime;
	ctrlMsgProc();
	//�������hook�����ݷ�������
	if(m_isSenderOpen && m_keyMapOscList.size() > 0)
	{
		_XInputEvent e;
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
int _GGame::keyValueToIndex(_XKeyValue k)
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
void _GGame::ctrlMsgProc()
{
	if(!m_isShowMsg && m_mouseOnTimer > 2000)
	{
		if(m_textIPEdt.getVisible() && m_textIPEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//IP
			m_allMsgText.setString("����OSC���Ͷ˵�IP��ַ��һ��Ϊ����IP��Ҳ��������127.0.0.1");
		}else
		if(m_textPortEdt.getVisible() && m_textPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//Port
			m_allMsgText.setString("����OSC���Ͷ˵Ķ˿ںţ��˿ں���Ҫ����ն˵Ķ˿ں�һ�£�Ĭ��Ϊ12345");
		}else
		if(m_openBtn.getVisible() && m_openBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�򿪰�ť
			m_allMsgText.setString("�򿪻��߹ر�OSC��������ȷ��IP��ַ�Ͷ˿���ȷ�����򲻼������ȷ��");
		}else
	//	if(m_closeBtn.getVisible() && m_closeBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	//	{//�رհ�ť
	//		m_allMsgText.setString("�ر�OSC�������������ȴ򿪲��ܹر�");
	//	}else
		if(m_sendAsMsg.getVisible() && m_sendAsMsg.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//��Ϊ��Ϣ����
			m_allMsgText.setString("��ѡʱ��Ҫ���͵�������Ϊһ����Ϣ���ͣ�������Ϊһ�����ݰ�����.");
		}else
		if(m_addBtn.getVisible() && m_addBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//���
			m_allMsgText.setString("����߱༭�õ�OSC������Ϣ��ӵ��ұߵ��б����Ա���֮�������");
		}else
		if(m_delBtn.getVisible() && m_delBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//ɾ��
			m_allMsgText.setString("ɾ������б���ѡ�е����ݡ�");
		}else
		if(m_clearBtn.getVisible() && m_clearBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//���
			m_allMsgText.setString("����ұ��б���е��������ݡ�");
		}else
		if(m_modifyBtn.getVisible() && m_modifyBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�޸�
			m_allMsgText.setString("���ұ��б���ѡ��һ�����ݲ��޸ģ����¸ð�ť���޸ĺ�����ݸ��µ��б��С�");
		}else
		if(m_textDataEdt.getVisible() && m_textDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//data
			m_allMsgText.setString("OSC��Ϣ�ж�Ӧ�����ݣ���ȷ��������������������������ƥ�䡣");
		}else
		if(m_textDataCmb.getVisible() && m_textDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//type
			m_allMsgText.setString("OSC��Ϣ�����ݵ��������ͣ���ȷ����������������ƥ�䡣");
		}else
		if(m_textAddressEdt.getVisible() && m_textAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//addr
			m_allMsgText.setString("OSC��Ϣ�е�ַ��");
		}else
		if(m_textTimesEdt.getVisible() && m_textTimesEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//Times
			m_allMsgText.setString("OSC��Ϣ��Ҫѭ�����͵Ĵ������������ͽ�����������ֵ��ѭ�����͡�");
		}else
		if(m_textSpacingEdt.getVisible() && m_textSpacingEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//space
			m_allMsgText.setString("���η���֮��ļ��ʱ�䣬��λΪ����(ms)��1����Ϊǧ��֮һ�롣");
		}else
		if(m_sendBtn.getVisible() && m_sendBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//send
			m_allMsgText.setString("�����趨�������ݻ�ֹͣ���ͣ�����ǰ����ȷ��OSC�������Ѿ���ȷ�򿪡�");
		}else
//		if(m_stopBtn.getVisible() && m_stopBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//stop
//			m_allMsgText.setString("���¸ð�ťֹͣ���͡�");
//		}else
		if(m_sendModeCmb.getVisible() && m_sendModeCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//����ģʽ
			m_allMsgText.setString("ѡ����Ҫ�ķ���ģʽ���������ַ���ģʽ���������ʣ������Ķ�˵���ĵ���");
		}else
		if(m_dataLst.getVisible() && m_dataLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�����б�
			m_allMsgText.setString("�����б�����˫����Ӧ�����н��з��ͣ���ȷ���Ѿ���ȷ��OSC��������");
		}else
		if(m_supportACKMode.getVisible() && m_supportACKMode.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//Ӧ��ģʽ
			m_allMsgText.setString("Ӧ��ģʽ������ʱ�������յ�ĳ��ַ�Ŀ�����ʱ�����Զ����ظõ�ַ�����ݡ�");
		}else
		if(m_clearRecvBtn.getVisible() && m_clearRecvBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//����б�
			m_allMsgText.setString("���¸ð�ť��ս����б������б��е��������ݽ�����ա�");
		}else
		if(m_textRPortEdt.getVisible() && m_textRPortEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//���ն˿�
			m_allMsgText.setString("OSC�������Ķ˿ںţ���������Ӧ�������Ķ˿ں�һ�²�����ȷ���գ�Ĭ��Ϊ12345");
		}else
		if(m_openRBtn.getVisible() && m_openRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//��
			m_allMsgText.setString("�򿪻�ر�OSC����������ȷ���������˿ں�������ȷ��");
		}else
//		if(m_closeRBtn.getVisible() && m_closeRBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
//		{//�ر�
//			m_allMsgText.setString("�ر�OSC��������");
//		}else
		if(m_saveBtn.getVisible() && m_saveBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//����
			m_allMsgText.setString("����ǰ�����ϵ��������ݱ��浽config.xml�ļ��С�");
		}else
		if(m_readBtn.getVisible() && m_readBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//��ȡ
			m_allMsgText.setString("�������ļ�config.xml�ж�ȡ��һ�α�����������ݡ�");
		}else
		if(m_recvText.getVisible() && m_recvText.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�����б��
			m_allMsgText.setString("���յ����ݽ���ʾ�ڴˣ�ѡ����ĩ�п��Ա��ֹ����ĩ�С�");
		}else
		if(m_addKMBtn.getVisible() && m_addKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//���
			m_allMsgText.setString("����߱༭�õ���������OSC��ӳ����ӵ�ӳ���б��С�");
		}else
		if(m_delKMBtn.getVisible() && m_delKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//ɾ��
			m_allMsgText.setString("ɾ��ӳ���б���ѡ����С�");
		}else
		if(m_clearKMBtn.getVisible() && m_clearKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//���
			m_allMsgText.setString("�����ߵ�ӳ���б�");
		}else
		if(m_modifyKMBtn.getVisible() && m_modifyKMBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�޸�
			m_allMsgText.setString("����߱༭�õ�ֵ���µ�ӳ���б���ѡ�е��С�");
		}else
		if(m_keySelectCmb.getVisible() && m_keySelectCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//��ֵ
			m_allMsgText.setString("��Ӧ�������߼��̰�����");
		}else
		if(m_keyStateCmb.getVisible() && m_keyStateCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//����״̬
			m_allMsgText.setString("������״̬��");
		}else
		if(m_oscDataEdt.getVisible() && m_oscDataEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc���ݱ༭
			m_allMsgText.setString("OSC���ݡ�");
		}else
		if(m_oscDataCmb.getVisible() && m_oscDataCmb.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc���ݱ༭
			m_allMsgText.setString("OSC�������͡�");
		}else
		if(m_oscAddressEdt.getVisible() && m_oscAddressEdt.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//osc��ַ
			m_allMsgText.setString("OSC��ַ��");
		}else
		if(m_dataKMLst.getVisible() && m_dataKMLst.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
		{//�����̺�osc��ӳ���б�
			m_allMsgText.setString("�����̺�osc��ӳ���б�");
		}
	}
}
void _GGame::draw()
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
void _GGame::input(const _XInputEvent &event)
{
	if(event.type == EVENT_MOUSE)
	{
		m_mouseOnTimer = 0;
		m_isShowMsg = false;
	}
	if(event.type == EVENT_KEYBOARD && 
		event.keyState == KEY_STATE_DOWN &&
		event.keyValue == XKEY_ESCAPE)
	{
		windowExitFun(this);
	}
}
void _GGame::release()
{
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XDELETE(m_keyMapOscList[i]);
	}
	stopHook();
	m_keyMapOscList.clear();
}
bool _GGame::saveToFile(const char *filename)
{
	//TiXmlDocument doc(CFG_FILENAME);
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlDocument doc;
//	if(filename == NULL) doc.crea(CFG_FILENAME);
//	else doc.LoadFile(filename);
	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");

	if(!addLeafNode(elmRoot,"sendIP",m_textIPEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendPort",m_textPortEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendAsMessage",m_sendAsMsg.getState())) return false;
	if(!addLeafNode(elmRoot,"dataE",m_textDataEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"typeE",toString(m_textDataCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"addressE",m_textAddressEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"timeE",m_textTimesEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"spaceE",m_textSpacingEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"sendMode",toString(m_sendModeCmb.getNowChooseOrder()))) return false;

	for(int i = 0;i < m_datasVector.size();++ i)
	{
		TiXmlElement elmNode("data"); 
		if(!addLeafNode(elmNode,"addr",m_datasVector[i]->address)) return false;
		if(!addLeafNode(elmNode,"datas",m_datasVector[i]->data)) return false;
		if(!addLeafNode(elmNode,"type",toString(m_datasVector[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!addLeafNode(elmRoot,"keyValueE",toString(m_keySelectCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"keyStateE",toString(m_keyStateCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"oscDataE",m_oscDataEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"oscTypeE",toString(m_oscDataCmb.getNowChooseOrder()))) return false;
	if(!addLeafNode(elmRoot,"oscAddrE",m_oscAddressEdt.getString())) return false;
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		TiXmlElement elmNode("map"); 
		if(!addLeafNode(elmNode,"keyValue",toString(m_keyMapOscList[i]->keyValue))) return false;
		if(!addLeafNode(elmNode,"keyState",toString(m_keyMapOscList[i]->keyState))) return false;
		if(!addLeafNode(elmNode,"addr",m_keyMapOscList[i]->address)) return false;
		if(!addLeafNode(elmNode,"datas",m_keyMapOscList[i]->data)) return false;
		if(!addLeafNode(elmNode,"type",toString(m_keyMapOscList[i]->dataType))) return false;
		if(elmRoot.InsertEndChild(elmNode) == NULL) return false;  
	}
	if(!addLeafNode(elmRoot,"recvPort",m_textRPortEdt.getString())) return false;
	if(!addLeafNode(elmRoot,"ACKMode",m_supportACKMode.getState())) return false;

	if(doc.InsertEndChild(elmRoot) == NULL) return false;
	bool ret;
	if(filename == NULL) ret = doc.SaveFile(CFG_FILENAME);
	else ret = doc.SaveFile(filename);
	if(!ret) return ret;
	//doc.SaveFile();
	m_allMsgText.setString("�ļ��������!");
	return true;
}
bool _GGame::readFromFile(const char *filename)
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
	TiXmlNode *rootNode = NULL;
	rootNode = doc.FirstChild("root");
	if(rootNode == NULL) return false;
	if(getXmlAsString(rootNode,"sendIP",tmp))
		m_textIPEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"sendPort",tmp))
		m_textPortEdt.setString(tmp.c_str());
	if(getXmlAsBool(rootNode,"sendAsMessage",tmpB))
		m_sendAsMsg.setState(tmpB);
	if(getXmlAsString(rootNode,"dataE",tmp))
		m_textDataEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"typeE",tmpI))
		m_textDataCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"addressE",tmp))
		m_textAddressEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"timeE",tmp))
		m_textTimesEdt.setString(tmp.c_str());
	if(getXmlAsString(rootNode,"spaceE",tmp))
		m_textSpacingEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"sendMode",tmpI))
		m_sendModeCmb.setNowChooseOrder(tmpI);
	//�����ȡ����
	for(int i = 0;i < m_datasVector.size();++ i)
	{
		XDELETE(m_datasVector[i]);
	}
	m_datasVector.clear();
	m_dataLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("data");
	while(1)
	{
		if(keyNode == NULL) break;
		_GOscData *tmp = createMem<_GOscData>();
		getXmlAsString(keyNode,"addr",tmp->address);
		getXmlAsString(keyNode,"datas",tmp->data);
		getXmlAsInt(keyNode,"type",tmp->dataType);
		m_datasVector.push_back(tmp);
		addALineData(tmp);

		keyNode = keyNode->NextSiblingElement("data");
	}

	if(getXmlAsInt(rootNode,"keyValueE",tmpI))
		m_keySelectCmb.setNowChooseOrder(tmpI);
	if(getXmlAsInt(rootNode,"keyStateE",tmpI))
		m_keyStateCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"oscDataE",tmp))
		m_oscDataEdt.setString(tmp.c_str());
	if(getXmlAsInt(rootNode,"oscTypeE",tmpI))
		m_oscDataCmb.setNowChooseOrder(tmpI);
	if(getXmlAsString(rootNode,"oscAddrE",tmp))
		m_oscAddressEdt.setString(tmp.c_str());
	for(int i = 0;i < m_keyMapOscList.size();++ i)
	{
		XDELETE(m_keyMapOscList[i]);
	}
	m_keyMapOscList.clear();
	m_dataKMLst.setLineSum(0);
	keyNode = rootNode->FirstChildElement("map");
	while(1)
	{
		if(keyNode == NULL) break;
		_GKMOscData *tmp = createMem<_GKMOscData>();
		getXmlAsInt(keyNode,"keyValue",tmp->keyValue);
		getXmlAsInt(keyNode,"keyState",tmp->keyState);
		getXmlAsString(keyNode,"addr",tmp->address);
		getXmlAsString(keyNode,"datas",tmp->data);
		getXmlAsInt(keyNode,"type",tmp->dataType);
		m_keyMapOscList.push_back(tmp);
		addALineData(tmp);
		keyNode = keyNode->NextSiblingElement("map");
	}
	//��ȡ��������
	if(getXmlAsString(rootNode,"recvPort",tmp))
		m_textRPortEdt.setString(tmp.c_str());
	if(getXmlAsBool(rootNode,"ACKMode",tmpB))
		m_supportACKMode.setState(tmpB);
	return true;
}
void _GGame::addALineData(const _GOscData *tmp)
{
	int lineSum = m_dataLst.getLineSum();
	m_dataLst.addALine();
	setLineData(tmp,lineSum);
}
void _GGame::setLineData(const _GOscData *tmp,int index)
{
	m_dataLst.setBoxStr(tmp->address.c_str(),index,0);
	m_dataLst.setBoxStr(tmp->data.c_str(),index,1);
	m_dataLst.setBoxStr(oscDataTypeToStr(tmp->dataType).c_str(),index,2);
}
void _GGame::addALineData(const _GKMOscData *tmp)
{
	int lineSum = m_dataKMLst.getLineSum();
	m_dataKMLst.addALine();
	setLineData(tmp,lineSum);
}
void _GGame::setLineData(const _GKMOscData *tmp,int index)
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
std::string _GGame::keyValueToStr(int keyValue)
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
	case 39:return "Mouse_X";
	case 40:return "Mouse_Y";
	default:
		return "";
	}
}
std::string _GGame::oscDataTypeToStr(int type)
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
void _GGame::ctrlStateManager()
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