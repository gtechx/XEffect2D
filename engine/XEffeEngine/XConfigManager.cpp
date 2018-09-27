#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XConfigManager.h"
#include "XControl\XSliderEx.h"
#include "XControl\XCheck.h"
#include "XControl\XRadios.h"
#include "XControl\XCheck.h"
#include "XNetWork/XNetServer.h"
#include "XNetWork/XNetClient.h"
#include "XFile.h"
#include "XControl\XControlManager.h"
namespace XE{
bool XConfigManager::m_withConfigManager = false;
void XConfigManager::ctrlProc(void*pClass, int id, int eventID)
{
	XConfigManager &pPar = *(XConfigManager *)pClass;
	//���ܰ���
	if (id == pPar.m_saveBtn->getControlID())
	{//����
		if (eventID != XButton::BTN_MOUSE_DOWN) return;
		if (pPar.save()) XEG.setTopMsgStr("���ñ���ɹ�");
		else XEG.setTopMsgStr("���ñ���ʧ!��!");
		pPar.setOperateToServer(CFG_NET_OP_SAVE);
		XCtrlManager.eventProc(id, XButton::BTN_MOUSE_DOWN);
		return;
	}
	if (id == pPar.m_loadBtn->getControlID())
	{//��ȡ
		if (eventID != XButton::BTN_MOUSE_DOWN) return;
		switch (pPar.m_configMode)
		{
		case CFG_MODE_CLIENT:
			pPar.setOperateToServer(CFG_NET_OP_LOAD);
			break;
		case CFG_MODE_SERVER:
			if(pPar.load()) XEG.setTopMsgStr("���ö�ȡ�ɹ�");
			else XEG.setTopMsgStr("���ö�ȡʧ!��!");
			pPar.sendCFGInfo();
			break;
		default:
			if (pPar.load()) XEG.setTopMsgStr("���ö�ȡ�ɹ�");
			else XEG.setTopMsgStr("���ö�ȡʧ!��!");
			break;
		}
		XCtrlManager.eventProc(id, XButton::BTN_MOUSE_DOWN);
		return;
	}
	if (id == pPar.m_defaultBtn->getControlID())
	{//Ĭ��
		if (eventID != XButton::BTN_MOUSE_DOWN) return;
		switch (pPar.m_configMode)
		{
		case CFG_MODE_CLIENT:
			pPar.setOperateToServer(CFG_NET_OP_DEFAULT);
			break;
		case CFG_MODE_SERVER:
			pPar.setDefault();
			pPar.sendCFGInfo();
			break;
		default:
			pPar.setDefault();
			break;
		}
		XCtrlManager.eventProc(id, XButton::BTN_MOUSE_DOWN);
		return;
	}
	if (id == pPar.m_netUpdateBtn->getControlID())
	{//ͬ��
		if (eventID == XButton::BTN_MOUSE_DOWN)
			pPar.sendSynchToServer();
		return;
	}
	if (id == pPar.m_netInjectBtn->getControlID())
	{//ע��
		if (eventID == XButton::BTN_MOUSE_DOWN)
			pPar.sendInject();
		return;
	}
	if (id == pPar.m_undoBtn->getControlID())
	{//����
		if (eventID == XButton::BTN_MOUSE_DOWN)
			XOpManager.undo();
		return;
	}
	if (id == pPar.m_redoBtn->getControlID())
	{//����
		if (eventID == XButton::BTN_MOUSE_DOWN)
			XOpManager.redo();
		return;
	}
	for (unsigned int i = 0; i < pPar.m_pItems.size(); ++i)
	{
		if (pPar.m_pItems[i] == NULL || pPar.m_pItems[i]->m_pCtrl == NULL ||
			id != pPar.m_pItems[i]->m_pCtrl->getControlID()) continue;
		//���洦���¼�
		XConfigItem &pItem = *pPar.m_pItems[i];
		int tmp = 0;
		switch (pItem.m_type)
		{
		case CFG_DATA_INT:
			if (eventID != XSliderEx::SLDEX_MOUSE_MOVE && eventID != XSliderEx::SLDEX_VALUE_CHANGE) break;
			tmp = XMath::toInt(((XSliderEx *)(pItem.m_pCtrl))->getCurValue());
			if (pItem.m_curValue.valueI != tmp)
			{
				pItem.m_curValue.valueI = tmp;
				*(int *)pItem.m_pVariable = tmp;
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
			break;
		case CFG_DATA_CHAR:
			if (eventID != XSliderEx::SLDEX_MOUSE_MOVE && eventID != XSliderEx::SLDEX_VALUE_CHANGE) break;
			tmp = XMath::toInt(((XSliderEx *)(pItem.m_pCtrl))->getCurValue());
			if (pItem.m_curValue.valueI != tmp)
			{
				pItem.m_curValue.valueI = tmp;
				*(char *)pItem.m_pVariable = tmp;
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
		case CFG_DATA_UCHAR:
			if (eventID != XSliderEx::SLDEX_MOUSE_MOVE && eventID != XSliderEx::SLDEX_VALUE_CHANGE) break;
			tmp = XMath::toInt(((XSliderEx *)(pItem.m_pCtrl))->getCurValue());
			if (pItem.m_curValue.valueI != tmp)
			{
				pItem.m_curValue.valueI = tmp;
				*(unsigned char *)pItem.m_pVariable = tmp;
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
			break;
		case CFG_DATA_FLOAT:
			if (eventID != XSliderEx::SLDEX_MOUSE_MOVE && eventID != XSliderEx::SLDEX_VALUE_CHANGE) break;
			if (pItem.m_curValue.valueF != ((XSliderEx *)(pItem.m_pCtrl))->getCurValue())
			{
				pItem.m_curValue.valueF = ((XSliderEx *)(pItem.m_pCtrl))->getCurValue();
				*(float *)pItem.m_pVariable = ((XSliderEx *)(pItem.m_pCtrl))->getCurValue();
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
			break;
		case CFG_DATA_XBOOL:
			if (eventID != XCheck::CHK_STATE_CHANGE) break;
			if (pItem.m_curValue.valueB != ((XCheck *)(pItem.m_pCtrl))->getState())
			{
				pItem.m_curValue.valueB = ((XCheck *)(pItem.m_pCtrl))->getState();
				*(XBool *)pItem.m_pVariable = ((XCheck *)(pItem.m_pCtrl))->getState();
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
			break;
		case CFG_DATA_RADIOS:
			if (eventID != XRadios::RDS_STATE_CHANGE) break;
			if (pItem.m_curValue.valueI != ((XRadios *)(pItem.m_pCtrl))->getCurChoose())
			{
				pItem.m_curValue.valueI = ((XRadios *)(pItem.m_pCtrl))->getCurChoose();
				*(int *)pItem.m_pVariable = ((XRadios *)(pItem.m_pCtrl))->getCurChoose();
				if (pItem.m_changeProc != NULL)
					pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//����ֵ�仯֮��ŵ��ûص�����
				pPar.sendItemValue(pPar.m_pItems[i]);	//���������ģʽ����֪ͨ�����豸������Ӧ��ֵ
			}
			break;
		}
		return;
	}
	//�ж��Ƿ���Ⱥ��ؼ���״̬�����˱仯
	for (unsigned int i = 0; i < pPar.m_pGroups.size(); ++i)
	{
		if (id == pPar.m_pGroups[i]->m_group.getControlID())
		{//Ⱥ��ؼ���״̬�����仯��ʱ��ı����岼��
			if (eventID == XGroup::GRP_STATE_CHANGE) pPar.relayout();
			return;
		}
	}
}
bool XConfigManager::save()
{
	FILE *fp = NULL;
	if ((fp = fopen(m_saveAndLoadFilename.c_str(), "w")) == NULL) return false;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (!m_pItems[i]->m_isActive) continue;//������ֵ����Ҫ����
		switch (m_pItems[i]->m_type)
		{
		case CFG_DATA_INT:
		case CFG_DATA_CHAR:
		case CFG_DATA_UCHAR:
			fprintf(fp, "%s:%d,\n", m_pItems[i]->m_name.c_str(), m_pItems[i]->m_curValue.valueI);
			break;
		case CFG_DATA_FLOAT:
			fprintf(fp, "%s:%f,\n", m_pItems[i]->m_name.c_str(), m_pItems[i]->m_curValue.valueF);
			break;
		case CFG_DATA_XBOOL:
			if (m_pItems[i]->m_curValue.valueB) fprintf(fp, "%s:1,\n", m_pItems[i]->m_name.c_str());
			else fprintf(fp, "%s:0,\n", m_pItems[i]->m_name.c_str());
			break;
		case CFG_DATA_RADIOS:
			fprintf(fp, "%s:%d,\n", m_pItems[i]->m_name.c_str(), m_pItems[i]->m_curValue.valueI);
			break;
		case CFG_DATA_CUSTOM:
			m_pItems[i]->m_customIt->save(fp);
			break;
		}
	}
	fclose(fp);
	return true;
}
bool XConfigManager::getItemValueFromStr(XConfigItem *it, const char *str)
{//���ﲻ����������������Լ��
	int temp;
	switch (it->m_type)
	{
	case CFG_DATA_INT:
		if (sscanf(str, "%d,\n", &it->m_curValue.valueI) == 0) return false;
		*(int *)it->m_pVariable = it->m_curValue.valueI;
		((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		((XSliderEx *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_CHAR:
		if (sscanf(str, "%d,\n", &it->m_curValue.valueI) == 0)  return false;
		*(char *)it->m_pVariable = it->m_curValue.valueI;
		((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		((XSliderEx *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_UCHAR:
		if (sscanf(str, "%d,\n", &it->m_curValue.valueI) == 0) return false;
		*(unsigned char *)it->m_pVariable = it->m_curValue.valueI;
		((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		((XSliderEx *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_FLOAT:
		if (sscanf(str, "%f,\n", &it->m_curValue.valueF) == 0) return false;
		*(float *)it->m_pVariable = it->m_curValue.valueF;
		((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueF);
		((XSliderEx *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_XBOOL:
		if (sscanf(str, "%d,\n", &temp) == 0) return false;
		if (temp == 0) it->m_curValue.valueB = XFalse;
		else it->m_curValue.valueB = XTrue;
		*(XBool *)it->m_pVariable = it->m_curValue.valueB;
		((XCheck *)it->m_pCtrl)->setState(it->m_curValue.valueB);
		((XCheck *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_RADIOS:
		if (sscanf(str, "%d,\n", &it->m_curValue.valueI) == 0) return false;
		*(int *)it->m_pVariable = it->m_curValue.valueI;
		((XRadios *)it->m_pCtrl)->setChoosed(it->m_curValue.valueI);
		((XRadios *)it->m_pCtrl)->stateChange();
		break;
	case CFG_DATA_CUSTOM:
		break;
	}
	return true;
}
bool XConfigManager::load(const char *filename)
{
	FILE *fp = NULL;
	if (filename == NULL)
	{
		if ((fp = fopen(m_saveAndLoadFilename.c_str(), "r")) == NULL) return false;
	}
	else
	{
		if ((fp = fopen(filename, "r")) == NULL) return false;
	}
	char lineData[1024];
	int len = 0;
	bool ret = true;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		XConfigItem &pItem = *m_pItems[i];
		if (!pItem.m_isActive) continue;	//������ֵ�����ȡ
		if (feof(fp)) break;
		//��ȡһ������
		if (pItem.m_type == CFG_DATA_CUSTOM)
		{//������������
			int fpos = ftell(fp);
			if (pItem.m_customIt->load(fp))
			{//�����ȡ�ɹ���ֱ�Ӽ���
				continue;
			}
			else
			{
				ret = false;
				fseek(fp, fpos, SEEK_SET);	//�ָ�ԭ�е�λ��
			}
		}
		while (true)
		{
			if (feof(fp) || fgets(lineData, 1024, fp) == NULL)
			{//���ݶ�ȡʧ�ܻ����ļ�����
				fclose(fp);
				return ret;
			}
			//Ѱ�������������
			len = XString::getCharPosition(lineData, ':') + 1;
			if (len < 0)
			{//���ݴ��󣬼�����ȡ���������
				ret = false;
				continue;
			}
			else break;
		}
		lineData[len - 1] = '\0';
		if (pItem.m_type == CFG_DATA_CUSTOM || strcmp(lineData, pItem.m_name.c_str()) != 0)
		{//���ƱȽ�ʧ��
			XConfigItem *it = getItemByName(lineData, i + 1);
			if (it == NULL)
			{
				--i;
				continue;	//�Ҳ������������
			}
			if (getItemValueFromStr(it, lineData + len) &&			//���濪ʼ��ȡ�������ֵ
				it->m_changeProc != NULL)
			{
				if (it->m_type == CFG_DATA_CUSTOM)
					it->m_changeProc(it->m_customIt, it->m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
				else
					it->m_changeProc(it->m_pVariable, it->m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
			}
			ret = false;	//������ݲ���ȷ
			--i;
			continue;
		}
		else
		{
			if (getItemValueFromStr(m_pItems[i], lineData + len))
			{//���ݶ�ȡ�ɹ�
				if (pItem.m_changeProc != NULL)
				{
					if (pItem.m_type == CFG_DATA_CUSTOM)
						pItem.m_changeProc(pItem.m_customIt, pItem.m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
					else
						pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
				}
			}
			else
			{
				--i;
				continue;
			}
		}
	}
	fclose(fp);
	return ret;
}
void XConfigManager::setDefault()	//�ָ�Ĭ��ֵ
{
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		XConfigItem &pItem = *m_pItems[i];
		if (!pItem.m_isActive) continue;	//������ֵ���ܻظ�Ĭ������
		switch (pItem.m_type)
		{
		case CFG_DATA_INT:
			pItem.m_curValue.valueI = pItem.m_defaultValue.valueI;
			*(int *)pItem.m_pVariable = pItem.m_curValue.valueI;
			((XSliderEx *)pItem.m_pCtrl)->setCurValue(pItem.m_curValue.valueI);
			((XSliderEx *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_CHAR:
			pItem.m_curValue.valueI = pItem.m_defaultValue.valueI;
			*(char *)pItem.m_pVariable = pItem.m_curValue.valueI;
			((XSliderEx *)pItem.m_pCtrl)->setCurValue(pItem.m_curValue.valueI);
			((XSliderEx *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_UCHAR:
			pItem.m_curValue.valueI = pItem.m_defaultValue.valueI;
			*(unsigned char *)pItem.m_pVariable = pItem.m_curValue.valueI;
			((XSliderEx *)pItem.m_pCtrl)->setCurValue(pItem.m_curValue.valueI);
			((XSliderEx *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_FLOAT:
			pItem.m_curValue.valueF = pItem.m_defaultValue.valueF;
			*(float *)pItem.m_pVariable = pItem.m_curValue.valueF;
			((XSliderEx *)pItem.m_pCtrl)->setCurValue(pItem.m_curValue.valueF);
			((XSliderEx *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_XBOOL:
			pItem.m_curValue.valueB = pItem.m_defaultValue.valueB;
			*(XBool *)pItem.m_pVariable = pItem.m_curValue.valueB;
			((XCheck *)pItem.m_pCtrl)->setState(pItem.m_curValue.valueB);
			((XCheck *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_RADIOS:
			pItem.m_curValue.valueI = pItem.m_defaultValue.valueI;
			*(int *)pItem.m_pVariable = pItem.m_curValue.valueI;
			((XRadios *)pItem.m_pCtrl)->setChoosed(pItem.m_curValue.valueI);
			((XRadios *)pItem.m_pCtrl)->stateChange();
			break;
		case CFG_DATA_CUSTOM:
			pItem.m_customIt->defValue();
			break;
		}
		if (pItem.m_changeProc != NULL)
		{
			if (pItem.m_type == CFG_DATA_CUSTOM)
				pItem.m_changeProc(pItem.m_customIt, pItem.m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
			else
				pItem.m_changeProc(pItem.m_pVariable, pItem.m_pClass);	//fix bug:��ȡ���ò���ʱ��û�е����丽���ĺ���
		}
	}
}
bool XConfigManager::init(XConfigMode mode)
{
	if (m_isInited) return false;	//��ֹ�ظ���ʼ��
	m_configMode = mode;

	//	m_saveBtn->initWithoutSkin("����",XVec2(64.0f,32.0f));
	m_saveBtn->initWithoutSkin("", XVec2(32.0f));
	//m_saveBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/save.png").c_str());
	m_saveBtn->setSymbol(BTN_SYMBOL_SAVE);
	m_saveBtn->setEventProc(ctrlProc, this);
	m_saveBtn->setPosition(m_position.x + 32.0f * m_scale.x, m_position.y);
	m_saveBtn->setComment("���������ݱ��浽�ļ�");
	//	m_loadBtn->initWithoutSkin("��ȡ",XVec2(64.0f,32.0f));
	m_loadBtn->initWithoutSkin("", XVec2(32.0f));
	//m_loadBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/load.png").c_str());
	m_loadBtn->setSymbol(BTN_SYMBOL_LOAD);
	m_loadBtn->setEventProc(ctrlProc, this);
	m_loadBtn->setPosition(m_position.x + 64.0f * m_scale.x, m_position.y);
	m_loadBtn->setComment("���ļ��ж�ȡ��������");
	//	m_loadBtn->setPosition(m_position + XVec2(64.0f + CFG_MANAGER_W_SPACE,0.0f));
	//	m_defaultBtn->initWithoutSkin("Ĭ��",XVec2(64.0f,32.0f));
	m_defaultBtn->initWithoutSkin("", XVec2(32.0f));
	//m_defaultBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/default.png").c_str());
	m_defaultBtn->setSymbol(BTN_SYMBOL_DEF);
	m_defaultBtn->setEventProc(ctrlProc, this);
	m_defaultBtn->setPosition(m_position.x + 96.0f * m_scale.x, m_position.y);
	m_defaultBtn->setComment("�������ûָ�Ĭ��ֵ");
	//	m_defaultBtn->setPosition(m_position + XVec2(128.0f + 2.0f * CFG_MANAGER_W_SPACE,0.0f));
	//	m_undoBtn->initWithoutSkin("����",XVec2(64.0f,32.0f));
	m_undoBtn->initWithoutSkin("", XVec2(32.0f, 32.0f));
	//m_undoBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/undo.png").c_str());
	m_undoBtn->setSymbol(BTN_SYMBOL_LEFT);
	m_undoBtn->setEventProc(ctrlProc, this);
	m_undoBtn->setPosition(m_position.x + 128.0f * m_scale.x, m_position.y);
	m_undoBtn->setComment("������һ�β���");
	//	m_undoBtn->setPosition(m_position + XVec2(192.0f + 3.0f * CFG_MANAGER_W_SPACE,0.0f));
	//	m_redoBtn->initWithoutSkin("����",XVec2(64.0f,32.0f));
	m_redoBtn->initWithoutSkin("", XVec2(32.0f));
	//m_redoBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/redo.png").c_str());
	m_redoBtn->setSymbol(BTN_SYMBOL_RIGHT);
	m_redoBtn->setEventProc(ctrlProc, this);
	m_redoBtn->setPosition(m_position.x + 160.0f * m_scale.x, m_position.y);
	m_redoBtn->setComment("������һ�β���");
	//	m_redoBtn->setPosition(m_position + XVec2(256.0f + 4.0f * CFG_MANAGER_W_SPACE,0.0f));
	//	m_netUpdateBtn->initWithoutSkin("ͬ��",XVec2(64.0f,32.0f));
	m_netUpdateBtn->initWithoutSkin("", XVec2(32.0f));
	//m_netUpdateBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/download.png").c_str());
	m_netUpdateBtn->setSymbol(BTN_SYMBOL_DOWNLOAD);
	m_netUpdateBtn->setEventProc(ctrlProc, this);
	m_netUpdateBtn->setPosition(m_position.x + 128.0f * m_scale.x, m_position.y);
	m_netUpdateBtn->setComment("����������ͬ������");
	//	m_netUpdateBtn->setPosition(m_position + XVec2(192.0f + 3.0f * CFG_MANAGER_W_SPACE,0.0f));
	//	m_netInjectBtn->initWithoutSkin("ע��",XVec2(64.0f,32.0f));
	m_netInjectBtn->initWithoutSkin("", XVec2(32.0f));
	//m_netInjectBtn->setNormalIcon((getCommonResPos() + "ResourcePack/pic/CFGIcon/upload.png").c_str());
	m_netUpdateBtn->setSymbol(BTN_SYMBOL_UPDATE);
	m_netInjectBtn->setEventProc(ctrlProc, this);
	m_netInjectBtn->setPosition(m_position.x + 160.0f * m_scale.x, m_position.y);
	m_netInjectBtn->setComment("����������ͬ����ȥ");
	//	m_netInjectBtn->setPosition(m_position + XVec2(256.0f + 4.0f * CFG_MANAGER_W_SPACE,0.0f));
	if (m_configMode == CFG_MODE_CLIENT)
	{//�������������������ڷ������˻����Ƿ�����������ʹ�ã���Ϊ�ͻ��˻����½����ؼ�
		m_undoBtn->disVisible();
		m_redoBtn->disVisible();
	}
	else
	{
		m_netUpdateBtn->disVisible();
		m_netInjectBtn->disVisible();
	}
	m_curInsertPos = m_position + XVec2(0.0f, 32.0f + CFG_MANAGER_H_SPACE);
	m_maxHeight = getSceneHeight();	//Ĭ��ʹ��ȫ���߶�

	addGroup(CFG_DEFAULT_GROUPNAME);
	//XConfigGroup *defGroup = XMem::createMem<XConfigGroup>();
	//if(defGroup == NULL) return false;
	//defGroup->m_isEnable = true;
	//defGroup->m_position = m_position + XVec2(0.0f,32.0f + CFG_MANAGER_H_SPACE);
	//defGroup->m_group.init(defGroup->m_position,XRect(0.0f,0.0f,m_width,32.0f),defGroup->m_name.c_str(),getDefaultFont(),1.0f);
	//m_pGroups.push_back(defGroup);
	if (m_configMode == CFG_MODE_SERVER)
	{//����������
		std::string tempStr = XEG.m_windowData.windowTitle;
		tempStr = tempStr + "_Config";
		m_netServer->setProjectStr(tempStr.c_str());
		if (!m_netServer->createServer(6868, XTrue)) return false;
	}
	else
	if (m_configMode == CFG_MODE_CLIENT)
	{//�����ͻ���
		std::string tempStr = XEG.m_windowData.windowTitle;
		tempStr = tempStr + "_Config";
		m_netClient->setProjectStr(tempStr.c_str());
		if (!m_netClient->createClient()) return false;
	}

	m_isInited = true;
	return true;
}
void XConfigManager::draw()
{
	if (!m_isVisble || !m_isInited) return;
	//����Ϊ�����һ������
	if (m_withBackground)
	{
		for (unsigned int i = 0; i < m_pGroups.size(); ++i)
		{
			m_pGroups[i]->m_group.drawBG();
		}
	}

	XRender::drawFillRectEx(m_position, XVec2(32.0f) * m_scale, 0.75f, 0.75f, 0.75f, true, false, true);

	for (auto it = m_pItems.begin(); it != m_pItems.end(); ++it)
	{
		switch ((*it)->m_type)
		{
		case CFG_DATA_CUSTOM:(*it)->m_customIt->draw(); break;
		case CFG_DATA_XSPRITE:
			if ((*it)->m_pTitleFont != nullptr) (*it)->m_pTitleFont->draw();
			((XObjectBasic *)(*it)->m_pVariable)->draw();
			break;
		}
	}
}
void XConfigManager::relayoutGroup(int index, bool flag)
{
	m_pGroups[index]->m_scale = m_scale;
	m_pGroups[index]->m_position = m_curInsertPos;
	m_pGroups[index]->m_maxHeight = m_maxHeight;
	m_pGroups[index]->m_group.setPosition(m_pGroups[index]->m_position);
	m_pGroups[index]->m_group.setScale(m_scale);
	if (flag) m_pGroups[index]->relayout();
}
void XConfigManager::relayout()	//�����Զ�����
{
	m_saveBtn->setPosition(m_position.x + 32.0f * m_scale.x, m_position.y);
	m_saveBtn->setScale(m_scale);
	m_loadBtn->setPosition(m_position.x + 64.0f * m_scale.x, m_position.y);
	m_loadBtn->setScale(m_scale);
	m_defaultBtn->setPosition(m_position.x + 96.0f * m_scale.x, m_position.y);
	m_defaultBtn->setScale(m_scale);
	m_undoBtn->setPosition(m_position.x + 128.0f * m_scale.x, m_position.y);
	m_undoBtn->setScale(m_scale);
	m_redoBtn->setPosition(m_position.x + 160.0f * m_scale.x, m_position.y);
	m_redoBtn->setScale(m_scale);
	m_netUpdateBtn->setPosition(m_position.x + 128.0f * m_scale.x, m_position.y);
	m_netUpdateBtn->setScale(m_scale);
	m_netInjectBtn->setPosition(m_position.x + 160.0f * m_scale.x, m_position.y);
	m_netInjectBtn->setScale(m_scale);
	//���������λ��
	m_curInsertPos = m_position + XVec2(0.0f, (32.0f + CFG_MANAGER_H_SPACE) * m_scale.x);
	m_isNewRow = true;
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		if (m_pGroups[i]->m_group.getState() == XGroup::STATE_NORMAL)
		{//��鵱ǰ���Ƿ�������£�����ݲ�������
			if (m_maxHeight + m_position.y - m_curInsertPos.y < m_pGroups[i]->calculateMaxSize().y)
			{//����ݲ���
				if (m_isNewRow)
				{//����Ѿ����µ�һ�����򲻴���
					relayoutGroup(i);
					moveDown(m_pGroups[i]->m_maxSize.x, m_pGroups[i]->m_maxSize.y);
				}
				else
				{//����Ļ�ʹ���µ�һ��
					useANewRow();
					relayoutGroup(i);
					moveDown(m_pGroups[i]->m_maxSize.x, m_pGroups[i]->m_maxSize.y);
				}
			}
			else
			{
				relayoutGroup(i);
				moveDown(m_pGroups[i]->m_maxSize.x, m_pGroups[i]->m_maxSize.y);
			}
		}
		else
		{
			relayoutGroup(i, false);
			XVec2 tempSize = m_pGroups[i]->m_group.getBox(0) - m_pGroups[i]->m_group.getBox(2);
			if (tempSize.x < 0.0f) tempSize.x = -tempSize.x;
			if (tempSize.y < 0.0f) tempSize.y = -tempSize.y;
			moveDown(tempSize.x * 1.0f / m_scale.x, tempSize.y * 1.0f / m_scale.x);
		}
	}
	return;
}
void XConfigManager::setVisible()
{//������ʾ
	if (!m_isInited || m_isVisble) return;
	m_isVisble = true;
	m_saveBtn->setVisible();
	m_loadBtn->setVisible();
	m_defaultBtn->setVisible();
	if (m_configMode == CFG_MODE_CLIENT)
	{
		m_netUpdateBtn->setVisible();
		m_netInjectBtn->setVisible();
	}
	else
	{
		m_undoBtn->setVisible();
		m_redoBtn->setVisible();
	}
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		m_pGroups[i]->m_group.setVisible();
	}
	//�������ݵ�����
	if (m_configMode != CFG_MODE_CLIENT)
	{
		if (XOpManager.canUndo()) m_undoBtn->enable();
		else m_undoBtn->disable();
		if (XOpManager.canRedo()) m_redoBtn->enable();
		else m_redoBtn->disable();
	}
	for (auto it = m_pItems.begin(); it != m_pItems.end(); ++it)
	{
		//	if(!(*it)->m_isActive)
		{//������±����ؼ���״̬
			updateItemToCFG(*it);
		}
	}
}
void XConfigManager::disVisible()
{//���ò���ʾ
	if (!m_isInited || !m_isVisble) return;
	m_isVisble = false;
	m_saveBtn->disVisible();
	m_loadBtn->disVisible();
	m_defaultBtn->disVisible();
	if (m_configMode != CFG_MODE_CLIENT)
	{
		m_undoBtn->disVisible();
		m_redoBtn->disVisible();
	}
	if (m_configMode == CFG_MODE_CLIENT)
	{
		m_netUpdateBtn->disVisible();
		m_netInjectBtn->disVisible();
	}
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		m_pGroups[i]->m_group.disVisible();
	}
}
void XConfigManager::sendSynchToServer()
{
	if (m_configMode == CFG_MODE_CLIENT)
	{
		XNetData *tempSendData = XMem::createMem<XNetData>();
		tempSendData->dataLen = 2;
		tempSendData->data = XMem::createArrayMem<unsigned char>(tempSendData->dataLen);
		tempSendData->isEnable = XTrue;
		tempSendData->type = DATA_TYPE_CONFIG_INFO;
		m_netClient->sendData(tempSendData);
	}
}
//���ַ����ж�ȡItem��ص����ݣ������Ƿ���Ҫ����
bool XConfigManager::createAItemFromStr(const unsigned char * data, int &offset, unsigned char *groupName,
	std::vector<XConfigItem *> *itemsList, std::vector<int> *itemsIDListD)
{
	if (data == NULL) return false;
	int itemID, tempItemID = 0, len;
	XConfigDataType itemType;
	offset = 0;
	unsigned char tempNameI[4096];	//Ĭ����󳤶�Ϊ4096��������������

	memcpy(&itemID, data + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&itemType, data + offset, sizeof(itemType));
	offset += sizeof(itemType);
	XConfigValue rangeMin, rangeMax, defValue, curValue;
	switch (itemType)
	{
	case CFG_DATA_INT:
	case CFG_DATA_CHAR:
	case CFG_DATA_UCHAR:
	case CFG_DATA_RADIOS:
	case CFG_DATA_FLOAT:
	case CFG_DATA_XBOOL:
		memcpy(&rangeMin, data + offset, sizeof(rangeMin));
		offset += sizeof(rangeMin);
		memcpy(&rangeMax, data + offset, sizeof(rangeMax));
		offset += sizeof(rangeMax);
		memcpy(&defValue, data + offset, sizeof(defValue));
		offset += sizeof(defValue);
		memcpy(&curValue.valueI, data + offset, sizeof(curValue));
		offset += sizeof(curValue);
		break;
	case CFG_DATA_CUSTOM:	//����������
		{
			int tempItemsSum;
			memcpy(&tempItemsSum, data + offset, sizeof(tempItemsSum));
			offset += sizeof(tempItemsSum);
			for (int i = 0; i < tempItemsSum; ++i)
			{//�������Ƕ��
				createAItemFromStr(data + offset, len, groupName, itemsList, itemsIDListD);
				offset += len;
			}
		}
		return false;
	case CFG_DATA_XSPRITE:	//���粻֧��
	case CFG_DATA_NULL:
		return false;	//��������Ĳ���
	default:
		return false;
	}
	memcpy(&len, data + offset, sizeof(int));
	offset += sizeof(int);
	memcpy(tempNameI, data + offset, len);
	tempNameI[len] = '\0';
	offset += len;
	switch (itemType)
	{
	case CFG_DATA_INT:
		{
			int *temp = XMem::createMem<int>();
			*temp = curValue.valueI;
			tempItemID = addAItem<int>(temp, itemType, (char *)tempNameI, rangeMax.valueI, rangeMin.valueI, defValue.valueI, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_CHAR:
		{
			char *temp = XMem::createMem<char>();
			*temp = curValue.valueI;
			tempItemID = addAItem<char>(temp, itemType, (char *)tempNameI, rangeMax.valueI, rangeMin.valueI, defValue.valueI, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_UCHAR:
		{
			unsigned char *temp = XMem::createMem<unsigned char>();
			*temp = curValue.valueI;
			tempItemID = addAItem<unsigned char>(temp, itemType, (char *)tempNameI, rangeMax.valueI, rangeMin.valueI, defValue.valueI, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_RADIOS:
		{
			int *temp = XMem::createMem<int>();
			*temp = curValue.valueI;
			tempItemID = addAItem<int>(temp, itemType, (char *)tempNameI, rangeMax.valueI, rangeMin.valueI, defValue.valueI, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_FLOAT:
		{
			float *temp = XMem::createMem<float>();
			*temp = curValue.valueF;
			tempItemID = addAItem<float>(temp, itemType, (char *)tempNameI, rangeMax.valueF, rangeMin.valueF, defValue.valueF, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_XBOOL:
		{
			XBool *temp = XMem::createMem<XBool>();
			*temp = curValue.valueB;
			tempItemID = addAItem<XBool>(temp, itemType, (char *)tempNameI, rangeMax.valueB, rangeMin.valueB, defValue.valueB, NULL, (char *)groupName);
		}
		break;
	case CFG_DATA_CUSTOM:	//����������
	case CFG_DATA_XSPRITE:	//���粻֧��
	case CFG_DATA_NULL:	//���粻֧��
		break;
	default:
		break;
	}
	XConfigItem * it = getItemByID(tempItemID);
	if (it != NULL)
	{
		if (itemsList != NULL) itemsList->push_back(it);
		if (itemsIDListD != NULL) itemsIDListD->push_back(itemID);
	}
	return true;
}
void XConfigManager::updateInfo(unsigned char *data)
{
	if (data == NULL) return;
	int groupSum = 0;
	int itemsSum = 0;
	int offset = 0;
	int len = 0;
	memcpy(&groupSum, data + offset, sizeof(int));
	offset += sizeof(int);
	unsigned char tempName[4096];	//Ĭ����󳤶�Ϊ4096��������������
	std::vector<XConfigItem *> itemsList;
	std::vector<int> itemsIDListD;
	for (int i = 0; i < groupSum; ++i)
	{
		memcpy(&len, data + offset, sizeof(int));
		offset += sizeof(int);
		memcpy(tempName, data + offset, len);
		tempName[len] = '\0';
		offset += len;
		if (i == 0) renameGroup(CFG_DEFAULT_GROUPNAME, (char *)tempName);
		else addGroup((char *)tempName);
		memcpy(&itemsSum, data + offset, sizeof(int));
		offset += sizeof(int);
		for (int j = 0; j < itemsSum; ++j)
		{
			createAItemFromStr(data + offset, len, tempName, &itemsList, &itemsIDListD);
			offset += len;
		}
	}
	//ͳһ�ı�ؼ���ID
	for (unsigned int i = 0; i < itemsList.size(); ++i)
	{
		itemsList[i]->setID(itemsIDListD[i]);
	}
}
void XConfigManager::sendItemValue(const XConfigItem * it)
{
	switch (m_configMode)
	{
	case CFG_MODE_SERVER:
		if (it->m_type != CFG_DATA_CUSTOM && it->m_type != CFG_DATA_XSPRITE)
		{
			XNetData *tempSendData = XMem::createMem<XNetData>();
			unsigned char *temp = NULL;
			int size = 0;
			int offset = 0;
			int tempID = it->getID();
			temp = XMem::spliceData(temp, size, offset, (unsigned char *)&tempID, sizeof(int));
			temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_curValue, sizeof(it->m_curValue));
			tempSendData->data = temp;
			tempSendData->dataLen = offset;
			tempSendData->isEnable = XTrue;
			tempSendData->type = DATA_TYPE_CONFIG_ITEM;
			m_netServer->sendData(tempSendData);
			//printf("���������ݷ���!\n");
		}
		break;
	case CFG_MODE_CLIENT:
		if (it->m_type != CFG_DATA_CUSTOM && it->m_type != CFG_DATA_XSPRITE)
		{
			XNetData *tempSendData = XMem::createMem<XNetData>();
			unsigned char *temp = NULL;
			int size = 0;
			int offset = 0;
			int tempID = it->getID();
			temp = XMem::spliceData(temp, size, offset, (unsigned char *)&tempID, sizeof(int));
			temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_curValue, sizeof(it->m_curValue));
			tempSendData->data = temp;
			tempSendData->dataLen = offset;
			tempSendData->isEnable = XTrue;
			tempSendData->type = DATA_TYPE_CONFIG_ITEM;
			m_netClient->sendData(tempSendData);
			//printf("���������ݷ���!\n");
		}
		break;
	}
}
void XConfigManager::updateItemFromCFG(XConfigItem * it)
{
	if (it == NULL) return;
	switch (it->m_type)
	{
	case CFG_DATA_INT:
		if (*(int *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			*(int *)it->m_pVariable = it->m_curValue.valueI;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_CHAR:
		if (*(char *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			*(char *)it->m_pVariable = it->m_curValue.valueI;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_UCHAR:
		if (*(unsigned char *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			*(unsigned char *)it->m_pVariable = it->m_curValue.valueI;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_FLOAT:
		if (*(float *)it->m_pVariable != it->m_curValue.valueF)
		{//�����������
			*(float *)it->m_pVariable = it->m_curValue.valueF;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueF);
		}
		break;
	case CFG_DATA_XBOOL:
		if (((*(XBool *)it->m_pVariable) && !it->m_curValue.valueB)
			|| (!(*(XBool *)it->m_pVariable) && it->m_curValue.valueB))
		{//�����������
			*(XBool *)it->m_pVariable = it->m_curValue.valueB;
			((XCheck *)it->m_pCtrl)->setState(it->m_curValue.valueB);
		}
		break;
	case CFG_DATA_RADIOS:
		if (*(int *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			*(int *)it->m_pVariable = it->m_curValue.valueI;
			((XRadios *)it->m_pCtrl)->setChoosed(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_CUSTOM:	//ռλ
		it->m_customIt->update();
		break;
	}
	if (it->m_changeProc != NULL)
	{
		if (it->m_type == CFG_DATA_CUSTOM)
			it->m_changeProc(it->m_customIt, it->m_pClass);
		else
			it->m_changeProc(it->m_pVariable, it->m_pClass);
	}
}
void XConfigManager::setTextColor(const XFColor& color)
{
	m_textColor = color;
	//�������е�Ԫ�أ�����������ɫ
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		switch (m_pItems[i]->m_type)
		{
		case CFG_DATA_INT:
		case CFG_DATA_CHAR:
		case CFG_DATA_UCHAR:
		case CFG_DATA_FLOAT:
			((XSliderEx *)m_pItems[i]->m_pCtrl)->setTextColor(m_textColor);
			break;
		case CFG_DATA_XBOOL:
			((XCheck *)m_pItems[i]->m_pCtrl)->setTextColor(m_textColor);
			break;
		case CFG_DATA_RADIOS:
			((XRadios *)m_pItems[i]->m_pCtrl)->setTextColor(m_textColor);
			break;
		}
	}
}
void XConfigManager::updateItemToCFG(XConfigItem * it)
{
	if (it == NULL) return;
	switch (it->m_type)
	{
	case CFG_DATA_INT:
		if (*(int *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			it->m_curValue.valueI = *(int *)it->m_pVariable;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_CHAR:
		if (*(char *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			it->m_curValue.valueI = *(char *)it->m_pVariable;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_UCHAR:
		if (*(unsigned char *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			it->m_curValue.valueI = *(unsigned char *)it->m_pVariable;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_FLOAT:
		if (*(float *)it->m_pVariable != it->m_curValue.valueF)
		{//�����������
			it->m_curValue.valueF = *(float *)it->m_pVariable;
			((XSliderEx *)it->m_pCtrl)->setCurValue(it->m_curValue.valueF);
		}
		break;
	case CFG_DATA_XBOOL:
		if (((*(XBool *)it->m_pVariable) && !it->m_curValue.valueB)
			|| (!(*(XBool *)it->m_pVariable) && it->m_curValue.valueB))
		{//�����������
			it->m_curValue.valueB = *(XBool *)it->m_pVariable;
			((XCheck *)it->m_pCtrl)->setState(it->m_curValue.valueB);
		}
		break;
	case CFG_DATA_RADIOS:
		if (*(int *)it->m_pVariable != it->m_curValue.valueI)
		{//�����������
			it->m_curValue.valueI = *(int *)it->m_pVariable;
			((XRadios *)it->m_pCtrl)->setChoosed(it->m_curValue.valueI);
		}
		break;
	case CFG_DATA_CUSTOM:	//ռλ
		it->m_customIt->update();
		break;
	}
}
void XConfigManager::sendCFGInfo()
{
	//GroupSum
	//GroupNameLen|GroupName|
		//ItemsSum
		//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefault|ItemCurValue|ItemNameLen|ItemName|
	XNetData *tempSendData = XMem::createMem<XNetData>();
	tempSendData->data = getConfigInfo(tempSendData->dataLen);
	tempSendData->isEnable = XTrue;
	tempSendData->type = DATA_TYPE_CONFIG_INFO;
	m_netServer->sendData(tempSendData);
	//printf("����������ͬ����Ϣ!\n");
}
void XConfigManager::sendInject()	//�����������ע����Ϣ
{
	if (m_configMode != CFG_MODE_CLIENT) return;
	//��ȡ�����ļ����������ļ����͵�����ˣ�����˶�ȡ�����ļ������룬Ȼ��ͬ�����ͻ���
	FILE *fp = NULL;
	if ((fp = fopen(CFG_DEFAULT_FILENAME, "r")) == NULL) return;
	XNetData *tempSendData = XMem::createMem<XNetData>();
	tempSendData->dataLen = XFile::getFileLen(fp);
	tempSendData->data = XMem::createArrayMem<unsigned char>(tempSendData->dataLen);
	fread(tempSendData->data, 1, tempSendData->dataLen, fp);
	fclose(fp);
	tempSendData->isEnable = XTrue;
	tempSendData->type = DATA_TYPE_CONFIG_INJECT;
	m_netClient->sendData(tempSendData);
	//printf("��������ע������!\n");
}
void XConfigManager::updateNet()
{
	switch (m_configMode)
	{
	case CFG_MODE_NORMAL:
		break;
	case CFG_MODE_SERVER://������
	{
		XNetData *tempData = m_netServer->getData();
		if (tempData != NULL)
		{//���洦������
			switch (tempData->type)
			{
			case DATA_TYPE_CONFIG_INFO:		//�ͻ������������õĽṹ��Ϣ
				sendCFGInfo();
				break;
			case DATA_TYPE_CONFIG_ITEM:		//�ͻ��˷���ĳ���������ֵ
			{//ȡ����Ӧ��ֵ
				int itemID;
				XConfigValue curValue;
				memcpy(&itemID, tempData->data, sizeof(itemID));
				memcpy(&curValue, tempData->data + sizeof(int), sizeof(curValue));
				if (itemID > m_cfgMaxItemsSum)
				{//�û��Զ�����������ID
					itemID = itemID / m_cfgMaxItemsSum;
					XConfigItem *it = getItemByID(itemID);
					if (it != NULL && it->m_customIt != NULL)
					{
						it->m_customIt->setValueFromStr(tempData->data);	//��Ч��Ӧ��ֵ
					}
				}
				else
				{
					setItemValue(itemID, curValue);	//��Ч��Ӧ��ֵ
				}
				//printf("�յ��ͻ������������ݱ��!\n");
			}
			break;
			case DATA_TYPE_CONFIG_ITEMS:	//�ͻ��˷��������������ֵ�����ᷢ��
				break;
			case DATA_TYPE_CONFIG_INJECT:	//�ӿͻ����յ�����ע���������
			{
				FILE *fp = NULL;
				if ((fp = fopen(CFG_INJECT_FILENAME, "w")) == NULL) return;
				fwrite(tempData->data, 1, tempData->dataLen, fp);
				fclose(fp);
				load(CFG_INJECT_FILENAME);
				sendCFGInfo();
			}
			break;
			case DATA_TYPE_CONFIG_OPERATE:	//���ܲ���(���)
				switch (*(XConfigNetOperate*)tempData->data)
				{
				case CFG_NET_OP_SAVE:
					save();
					break;
				case CFG_NET_OP_LOAD:
					load();
					sendCFGInfo();
					break;
				case CFG_NET_OP_DEFAULT:
					setDefault();
					sendCFGInfo();
					break;
				}
				break;
			}
			XMem::XDELETE(tempData);	//�������֮����ɾ������
		}
		for (unsigned int i = 0; i < m_pItems.size(); ++i)
		{
			if (m_pItems[i]->m_type == CFG_DATA_CUSTOM && m_pItems[i]->m_customIt->needSendStr())
			{//����Զ�����������Ҫ�������ݣ����﷢������
				XNetData *tempSendData = XMem::createMem<XNetData>();
				tempSendData->data = m_pItems[i]->m_customIt->sendStr(tempSendData->dataLen, m_pItems[i]->getID());
				tempSendData->isEnable = XTrue;
				tempSendData->type = DATA_TYPE_CONFIG_ITEMS;
				m_netServer->sendData(tempSendData);
				//printf("��ͻ��˷������ݱ��!\n");
			}
		}
	}
	break;
	case CFG_MODE_CLIENT://�ͻ���
	{
		XNetData *tempData = m_netClient->getData();
		if (tempData == NULL) break;
		switch (tempData->type)
		{
		case DATA_TYPE_CONFIG_INFO:		//���������͵����ýṹ
			clear();					//�����ǰ���е�Ⱥ��
			updateInfo(tempData->data);				//����ͬ������Ϣ�����µ�Ⱥ���ϵ
			//printf("�ͻ����յ�ͬ����Ϣ!\n");
			break;
		case DATA_TYPE_CONFIG_ITEM:		//���������͵�ĳ���������ֵ
		{//ȡ����Ӧ��ֵ
			int itemID;
			XConfigValue curValue;
			memcpy(&itemID, tempData->data, sizeof(itemID));
			memcpy(&curValue, tempData->data + sizeof(int), sizeof(curValue));
			setItemValue(itemID, curValue);	//��Ч��Ӧ��ֵ
			//printf("�յ������������������ݱ��!\n");
		}
		break;
		case DATA_TYPE_CONFIG_ITEMS:	//���������͵Ķ���������ֵ
			//���������Զ�������Żᷢ�����ֵ(��δ���)
		{
			int sum = 0;
			int offset = 0;
			memcpy(&sum, tempData->data + offset, sizeof(sum));
			offset += sizeof(sum);
			int itemID;
			XConfigValue curValue;
			for (int i = 0; i < sum; ++i)
			{
				memcpy(&itemID, tempData->data + offset, sizeof(itemID));
				offset += sizeof(itemID);
				memcpy(&curValue, tempData->data + offset, sizeof(curValue));
				offset += sizeof(curValue);
				setItemValue(itemID, curValue);	//��Ч��Ӧ��ֵ
			}
			//printf("���յ������������ݱ��!\n");
		}
		break;
		case DATA_TYPE_CONFIG_OPERATE:	//���������ᷢ���������
			break;
		}
		XMem::XDELETE(tempData);	//�������֮����ɾ������
	}
	break;
	}
}
void XConfigManager::update()	//����״̬,����߼�̨�鷳��Ҫ�Ż�Ϊû�б仯�Ͳ���Ҫ����
{
	if (!m_isInited) return;
	updateNet();
	if (m_isVisble)
	{
		if (m_configMode != CFG_MODE_CLIENT)
		{//�������������������ڷ������˻����Ƿ�����������ʹ�ã���Ϊ�ͻ��˻����½����ؼ�
			if (XOpManager.canUndo()) m_undoBtn->enable();
			else m_undoBtn->disable();
			if (XOpManager.canRedo()) m_redoBtn->enable();
			else m_redoBtn->disable();
			XOpManager.setOperateOver();
		}
		for (auto it = m_pItems.begin(); it != m_pItems.end(); ++it)
		{
			//	if(!(*it)->m_isActive)
			{//������±����ؼ���״̬
				updateItemToCFG(*it);
			}
		}
	}
	else
	{
		if (m_configMode != CFG_MODE_CLIENT)
			XOpManager.setOperateOver();
	}
	//����1��ûtʱ����һ�����ݣ�������ݷ����仯���¼һ�Σ�����ʽ������ʹ������ʽ��
}
void XConfigManager::setItemActive(bool isActive, void * p)	//����ĳ���������Ƿ�Ϊ������ʽ(Ĭ��ȫ��Ϊ������ʽ)
{
	if (p == NULL) return;
	XConfigItem *it = getItemByVariable(p);
	if (it == NULL) return;
	it->m_isActive = isActive;
	if (isActive)
	{
		switch (it->m_type)
		{
		case CFG_DATA_INT:
		case CFG_DATA_CHAR:
		case CFG_DATA_UCHAR:
		case CFG_DATA_FLOAT:
			((XSliderEx *)it->m_pCtrl)->enable();
			break;
		case CFG_DATA_XBOOL:
			((XCheck *)it->m_pCtrl)->enable();
			break;
		case CFG_DATA_RADIOS:
			((XRadios *)it->m_pCtrl)->enable();
			break;
		case CFG_DATA_CUSTOM:	//ռλ
			it->m_customIt->enable();
			break;
		}
	}
	else
	{
		switch (it->m_type)
		{
		case CFG_DATA_INT:
		case CFG_DATA_CHAR:
		case CFG_DATA_UCHAR:
		case CFG_DATA_FLOAT:
			((XSliderEx *)it->m_pCtrl)->disable();
			break;
		case CFG_DATA_XBOOL:
			((XCheck *)it->m_pCtrl)->disable();
			break;
		case CFG_DATA_RADIOS:
			((XRadios *)it->m_pCtrl)->disable();
			break;
		case CFG_DATA_CUSTOM:	//ռλ
			it->m_customIt->disable();
			break;
		}
	}
}
bool XConfigManager::clear()
{
	if (m_configMode == CFG_MODE_CLIENT)
	{//������Ҫɾ������
		for (unsigned int i = 0; i < m_pItems.size(); ++i)
		{
			m_pItems[i]->release();
			XMem::XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	else
	{
		for (unsigned int i = 0; i < m_pItems.size(); ++i)
		{
			XMem::XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		XMem::XDELETE(m_pGroups[i]);
	}
	m_pGroups.clear();
	addGroup(CFG_DEFAULT_GROUPNAME);	//����һ��Ĭ����
	return true;
}
bool XConfigManager::decreaseAItem(void *p)	//����һ��������
{
	if (p == NULL) return false;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->m_pVariable == p)
		{
			XConfigItem *it = m_pItems[i];
			//	for(int j = i;j < (int)(m_pItems.size()) - 1;++ i)
			//	{
			//		m_pItems[j] = m_pItems[j + 1]; 
			//	}
			//	m_pItems.pop_back();
			m_pItems.erase(m_pItems.begin() + i);
			//����Ⱥ�����Ϣ
			bool flag = false;
			for (unsigned int j = 0; j < m_pGroups.size(); ++j)
			{
				for (unsigned int k = 0; k < m_pGroups[j]->m_items.size(); ++k)
				{
					if (m_pGroups[j]->m_items[k] == it)
					{
						//	for(int l = k;l < (int)(m_pGroups[j]->m_items.size()) - 1;++ l)
						//	{
						//		m_pGroups[j]->m_items[l] = m_pGroups[j]->m_items[l + 1]; 
						//	}
						//	m_pGroups[j]->m_items.pop_back();
						m_pGroups[j]->m_items.erase(m_pGroups[j]->m_items.begin() + k);
						flag = true;
						break;
					}
				}
				if (flag) break;
			}
			XMem::XDELETE(it);
			return true;
		}
	}
	return false;
}
void XConfigGroup::moveDownPretreatment(int/*pixelW*/, int pixelH)
{
	if (m_curInsertPos.y + (pixelH + CFG_MANAGER_H_SPACE) * m_scale.y >
		m_position.y + m_maxHeight && !m_isNewRow)
	{
		useANewRow();
		m_isMaxH = true;
	}
}
void XConfigGroup::moveDown(int pixelW, int pixelH)	//�����ƶ������
{
	m_curInsertPos.y += (pixelH + CFG_MANAGER_H_SPACE) * m_scale.y;
	if (m_isNewRow)
	{
		m_maxRowWidth = pixelW;
		m_maxSize.x += pixelW + CFG_MANAGER_W_SPACE;	//�������������
	}
	else
	if (pixelW > m_maxRowWidth)
	{
		m_maxSize.x += pixelW - m_maxRowWidth;	//�������������
		m_maxRowWidth = pixelW;
	}
	if (!m_isMaxH) m_maxSize.y += pixelH + CFG_MANAGER_H_SPACE;
	else
	if ((m_curInsertPos.y - m_position.y) / m_scale.y > m_maxSize.y)
	{//��������еĸ߶ȴ���֮ǰ�еĸ߶ȣ���ȥ���ֵ
		m_maxSize.y = (m_curInsertPos.y - m_position.y) / m_scale.y;
	}
	m_isNewRow = false;
	//�������չһ�У���������������һ�еĻ�������Ҫ��չһ��
	if (m_curInsertPos.y >= m_position.y + m_maxHeight)
	{//������Ҫ����
		useANewRow();
		m_isMaxH = true;
	}
}
void XConfigGroup::relayout()
{//��������������������λ��
	m_curInsertPos = m_position + XVec2(5.0f, 32.0f + CFG_MANAGER_H_SPACE) * m_scale;
	m_maxSize.set(5.0f + m_width + CFG_MANAGER_W_SPACE, 32.0f + CFG_MANAGER_H_SPACE);
	m_maxRowWidth = m_width;
	m_isNewRow = false;
	m_isMaxH = false;
	XVec2 size;
	for (unsigned int i = 0; i < m_items.size(); ++i)
	{
		XConfigItem *it = m_items[i];
		if (it == NULL) continue;
		switch (it->m_type)
		{
		case CFG_DATA_INT:
		case CFG_DATA_CHAR:
		case CFG_DATA_UCHAR:
		case CFG_DATA_FLOAT:
			moveDownPretreatment(m_width, CFG_MNG_H_FONT);
			((XSliderEx *)it->m_pCtrl)->setPosition(m_curInsertPos);
			moveDown(m_width, CFG_MNG_H_FONT);
			break;
		case CFG_DATA_XBOOL:
			moveDownPretreatment(m_width, CFG_MNG_H_FONT);
			it->m_pCtrl->setPosition(m_curInsertPos);
			moveDown(m_width, CFG_MNG_H_FONT);
			break;
		case CFG_DATA_RADIOS:
			size = ((XRadios *)it->m_pCtrl)->getRectSize() / m_scale;
			moveDownPretreatment(size);
			((XRadios *)it->m_pCtrl)->setPosition(m_curInsertPos);
			moveDown(size);// / m_scale.y
			break;
		case CFG_DATA_CUSTOM:
			size = it->m_customIt->getRectSize() / m_scale;
			moveDownPretreatment(size);
			it->m_customIt->setPosition(m_curInsertPos);
			moveDown(size);// / m_scale.y
			break;
		case CFG_DATA_XSPRITE:
			size = ((XObjectBasic *)it->m_pVariable)->getRectSize() / m_scale;
			if (it->m_pTitleFont == nullptr)
			{
				moveDownPretreatment(size);
				((XObjectBasic *)it->m_pVariable)->setPosition(m_curInsertPos);
				moveDown(size);
			}
			else
			{
				size.y += it->m_pTitleFont->getTextSize().y;
				moveDownPretreatment(size);
				it->m_pTitleFont->setPosition(m_curInsertPos);
				((XObjectBasic *)it->m_pVariable)->setPosition(m_curInsertPos + 
					XVec2(0.0f, it->m_pTitleFont->getTextSize().y * it->m_pTitleFont->getScale().y));
				moveDown(size);
			}
			break;
		}
	}
	//���¸ı�����ķ�Χ
	m_group.resetSize(m_maxSize);
}
XVec2 XConfigGroup::calculateMaxSize()
{
	XVec2 ret;
	if (m_group.getState() == XGroup::STATE_MINISIZE)
	{
		ret = m_group.getBox(2) - m_group.getBox(0);
		//	ret.x /= m_scale.x;	//��ԭ������
		//	ret.y /= m_scale.y;
	}
	else
	{
		float maxY = 0.0f;
		bool isMaxH = false;
		ret.set(10.0f + m_width + CFG_MANAGER_W_SPACE, 32.0f);
		for (unsigned int i = 0; i < m_items.size(); ++i)
		{
			XConfigItem *it = m_items[i];
			if (it == NULL) continue;
			switch (it->m_type)
			{
			case CFG_DATA_INT:
			case CFG_DATA_CHAR:
			case CFG_DATA_UCHAR:
			case CFG_DATA_FLOAT:
			case CFG_DATA_XBOOL:
				ret.y += (CFG_MNG_H_FONT + CFG_MANAGER_H_SPACE) * m_scale.y;
				if (ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_scale.x;
					isMaxH = true;
					if (maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_scale.x;
				}
				break;
			case CFG_DATA_RADIOS:
				ret.y += CFG_MANAGER_H_SPACE * m_scale.y + ((XRadios *)it->m_pCtrl)->getRectHeight();
				if (ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_scale.x;
					isMaxH = true;
					if (maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_scale.x;
				}
				break;
			case CFG_DATA_CUSTOM:	//ռλ
				ret.y += CFG_MANAGER_H_SPACE * m_scale.y + it->m_customIt->getRectHeight();
				if (ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_scale.x;
					isMaxH = true;
					if (maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_scale.x;
				}
				break;
			case CFG_DATA_XSPRITE:
				ret.y += CFG_MANAGER_H_SPACE * m_scale.y + ((XObjectBasic *)it->m_pVariable)->getRectHeight();
				if (it->m_pTitleFont != nullptr)
					ret.y += it->m_pTitleFont->getTextSize().y * it->m_pTitleFont->getScale().y;
				if (ret.y > m_maxHeight)
				{
					ret.x += (m_width + CFG_MANAGER_W_SPACE) * m_scale.x;
					isMaxH = true;
					if (maxY < ret.y) maxY = ret.y;
					ret.y = 32.0f * m_scale.x;
				}
				break;
			}
		}
		if (isMaxH) ret.y = maxY;
	}
	return ret;
}
bool XConfigManager::addGroup(const char * name)
{
	if (name == NULL ||
		isGroupExist(name)) return false;	//�鲻������
	XConfigGroup *defGroup = XMem::createMem<XConfigGroup>();
	if (defGroup == NULL) return false;
	defGroup->m_isEnable = true;
	defGroup->m_position = m_position + XVec2(0.0f, 64.0f + CFG_MANAGER_H_SPACE);
	defGroup->m_name = name;
	defGroup->m_group.init(defGroup->m_position, XRect(0.0f, 0.0f, m_width + CFG_MANAGER_W_SPACE * 0.5f, 32.0f),
		defGroup->m_name.c_str(), getDefaultFont(), 1.0f);
	defGroup->m_group.setEventProc(ctrlProc, this);
	defGroup->m_group.setState(XGroup::STATE_MINISIZE);	//��ʼ״̬Ϊ��С��
	defGroup->m_width = m_width;
	defGroup->m_maxHeight = m_maxHeight;
	if (m_withBackground) defGroup->m_group.setWithRect(false);
	m_pGroups.push_back(defGroup);
	relayout();
	return true;
}
int XConfigManager::addSpecialItem(void* it, XConfigDataType type, const char* groupName,
	const char* title)
{
	if (it == NULL || type <= CFG_DATA_CUSTOM ||
		isSpecialItemExist(it)) return -1;	//����Ѿ����������ظ�����
	XConfigItem *pItem = XMem::createMem<XConfigItem>();
	if (pItem == NULL) return -1;
	if (title != nullptr)
	{
		pItem->m_pTitleFont = XMem::createMem<XFontUnicode>();
		if (pItem->m_pTitleFont != nullptr)
		{
			pItem->m_title = title;
			pItem->m_pTitleFont->setACopy(getDefaultFont());
			pItem->m_pTitleFont->setString(title);
			pItem->m_pTitleFont->setScale(m_scale);
			pItem->m_pTitleFont->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
			pItem->m_pTitleFont->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		}
	}
	pItem->m_isEnable = true;
	pItem->m_pVariable = it;
	pItem->m_customIt = NULL;
	((XObjectBasic *)pItem->m_pVariable)->setScale(((XObjectBasic *)pItem->m_pVariable)->getScale() * m_scale);
	////�������֡�����������ϽǶ�������⣬����ʹ��һ�ֲ��ʺϵķ�ʽ��������֡�ĳߴ粻��֮�仯(ע�⣺����Ȩ��֮�ƣ�����ع��Ļ���Ҫȡ��������)
	//if(((XObjectBasic *)pItem->m_pVariable)->getObjectType() == OBJ_FRAMEEX)
	//{
	//	((XObjectBasic *)pItem->m_pVariable)->setSize(1.0f,1.0f);
	//}else
	//{
	//	((XObjectBasic *)pItem->m_pVariable)->setSize(((XObjectBasic *)pItem->m_pVariable)->getSize() * m_scale);
	//}
//	pItem->m_scale = m_scale;
	pItem->m_type = type;
	pItem->m_isActive = false;
	m_pItems.push_back(pItem);
	XConfigGroup* gp = nullptr;
	if (groupName == NULL) gp = m_pGroups[0];//ʹ��Ĭ����
	else gp = getGroup(groupName);
	if (gp != nullptr)
	{
		gp->m_items.push_back(pItem);
		gp->m_group.pushChild((XObjectBasic *)pItem->m_pVariable);
		if (pItem->m_pTitleFont != nullptr)
			gp->m_group.pushChild(pItem->m_pTitleFont);
		if (gp->m_group.getState() == XGroup::STATE_MINISIZE)
		{
			((XObjectBasic *)pItem->m_pVariable)->disVisible();
			if (pItem->m_pTitleFont != nullptr)
				pItem->m_pTitleFont->disVisible();
		}
		relayout();
	}
	return pItem->getID();
}
int XConfigManager::addCustomItem(XCFGItemBasic *it, const char * groupName)
{
	if (it == NULL || isCustomItemExist(it)) return -1;
	XConfigItem *pItem = XMem::createMem<XConfigItem>();
	if (pItem == NULL) return -1;
	pItem->m_isEnable = true;
	pItem->m_pVariable = NULL;
	pItem->m_customIt = it;
	pItem->m_customIt->setScale(m_scale);
//	pItem->m_scale = m_scale;
	pItem->m_type = CFG_DATA_CUSTOM;
	m_pItems.push_back(pItem);
	XConfigGroup* gp = nullptr;
	if (groupName == NULL) gp = m_pGroups[0];	//ʹ��Ĭ����
	else gp = getGroup(groupName);

	if (gp != nullptr)
	{
		gp->m_items.push_back(pItem);
		gp->m_group.pushChild(pItem->m_customIt);
		if (gp->m_group.getState() == XGroup::STATE_MINISIZE) 
			pItem->m_customIt->disVisible();
		relayout();
	}
	return pItem->getID();
}
bool XConfigManager::isItemExist(void * p)	//����������Ƿ��Ѿ�����
{
	if (p == NULL) return false;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->m_pVariable == p) return true;
	}
	return false;
}
bool XConfigManager::isCustomItemExist(XCFGItemBasic *it)
{
	if (it == NULL) return false;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->m_type == CFG_DATA_CUSTOM &&
			m_pItems[i]->m_customIt == it) return true;
	}
	return false;
}
XConfigItem *XConfigManager::getItemByID(int ID)			//ͨ��ID��ȡ�������ָ��
{
	if (ID < 0) return NULL;
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->getID() == ID) return m_pItems[i];
	}
	return NULL;
}
XConfigItem *XConfigManager::getItemByVariable(void *p)	//ͨ������ָ�����������ָ��
{
	for (unsigned int i = 0; i < m_pItems.size(); ++i)
	{
		if (m_pItems[i]->m_pVariable == p) return m_pItems[i];
	}
	return NULL;
}
XConfigItem *XConfigManager::getItemByName(const char *name, int start)
{
	if (name == NULL) return NULL;
	for (int i = start; i < m_pItems.size(); ++i)
	{
		if (strcmp(m_pItems[i]->m_name.c_str(), name) == 0) return m_pItems[i];
	}
	return NULL;
}
bool XConfigManager::isGroupExist(const char * name)	//�ж�����Ƿ����
{
	if (name == NULL) return false;
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		if (strcmp(m_pGroups[i]->m_name.c_str(), name) == 0) return true;
	}
	return false;
}
XConfigGroup *XConfigManager::getGroup(const char * name)
{
	if (name == NULL) return NULL;
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		if (strcmp(m_pGroups[i]->m_name.c_str(), name) == 0) return m_pGroups[i];
	}
	return NULL;
}
void XConfigManager::release()
{//��Դ�ͷ�
	if (!m_isInited) return;
	if (m_configMode == CFG_MODE_CLIENT)
	{//������Ҫɾ������
		for (unsigned int i = 0; i < m_pItems.size(); ++i)
		{
			m_pItems[i]->release();
			XMem::XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	else
	{
		for (unsigned int i = 0; i < m_pItems.size(); ++i)
		{
			XMem::XDELETE(m_pItems[i]);
		}
		m_pItems.clear();
	}
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		XMem::XDELETE(m_pGroups[i]);
	}
	m_pGroups.clear();
	m_isInited = false;
}
void XConfigManager::setOperateToServer(XConfigNetOperate op)
{
	if (!m_isInited ||
		m_configMode != CFG_MODE_CLIENT) return;
	XNetData *tempData = XMem::createMem<XNetData>();
	if (tempData == NULL) return;
	tempData->isEnable = XTrue;
	tempData->type = DATA_TYPE_CONFIG_OPERATE;
	tempData->dataLen = sizeof(op);
	tempData->data = XMem::createArrayMem<unsigned char>(tempData->dataLen);
	memcpy(tempData->data, &op, tempData->dataLen);
	m_netClient->sendData(tempData);
}
unsigned char *XConfigManager::getConfigInfo(int &slen)
{
	unsigned char *temp = NULL;
	int size = 0;
	int offset = 0;
	XConfigDataType nullType = CFG_DATA_NULL;
	int len = m_pGroups.size();
	temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));
	for (unsigned int i = 0; i < m_pGroups.size(); ++i)
	{
		len = m_pGroups[i]->m_name.size();
		temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));
		temp = XMem::spliceData(temp, size, offset, (unsigned char *)m_pGroups[i]->m_name.c_str(), len);
		len = m_pGroups[i]->m_items.size();
		temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));
		for (unsigned int j = 0; j < m_pGroups[i]->m_items.size(); ++j)
		{
			XConfigItem *it = m_pGroups[i]->m_items[j];
			switch (it->m_type)
			{
			case CFG_DATA_INT:
			case CFG_DATA_CHAR:
			case CFG_DATA_UCHAR:
			case CFG_DATA_RADIOS:
			case CFG_DATA_FLOAT:
			case CFG_DATA_XBOOL:
				len = it->getID();
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));	//ID
				if (!it->m_isActive)
				{
					temp = XMem::spliceData(temp, size, offset, (unsigned char *)&nullType, sizeof(it->m_type));	//TYPE
					break;
				}
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_type, sizeof(it->m_type));	//TYPE
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_rangeMin, sizeof(it->m_rangeMin));	//min
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_rangeMax, sizeof(it->m_rangeMax));	//max
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_defaultValue, sizeof(it->m_defaultValue));	//defualt
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_curValue, sizeof(it->m_curValue));	//curValue
				len = it->m_name.size();
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));	//name len
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)it->m_name.c_str(), len);	//name len
				break;
			case CFG_DATA_CUSTOM:	//����������
				//��չ
			{
				len = it->getID();
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));					//ID
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&it->m_type, sizeof(it->m_type));	//TYPE
				unsigned char *tempStr = it->m_customIt->getInfo(len, it->getID());
				temp = XMem::spliceData(temp, size, offset, tempStr, len);
				XMem::XDELETE_ARRAY(tempStr);
			}
			break;
			case CFG_DATA_XSPRITE:	//���粻֧��
			case CFG_DATA_NULL:	//���粻֧��
				len = it->getID();
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&len, sizeof(int));	//ID
				temp = XMem::spliceData(temp, size, offset, (unsigned char *)&nullType, sizeof(it->m_type));	//TYPE
				break;
			}
		}
	}
	slen = offset;
	return temp;
}
void XConfigItem::release()	//ע�⣺����������ܷ���������������Ϊ���������е��������Ҫ�ͷ������Դ
{
	switch (m_type)
	{
	case CFG_DATA_INT:
		{
			int *p = (int *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_CHAR:
		{
			char *p = (char *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_UCHAR:
		{
			unsigned char *p = (unsigned char *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_FLOAT:
		{
			float *p = (float *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_XBOOL:	//ע�������ӦXBool���ͣ�������bool���ͣ����򽫻��п�����ɴ���
		{
			XBool *p = (XBool *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_RADIOS:	//��ѡ��
		{
			int *p = (int *)m_pVariable;
			XMem::XDELETE(p);
			m_pVariable = NULL;
		}
		break;
	case CFG_DATA_CUSTOM:	//�Զ���		���粻֧��
	case CFG_DATA_XSPRITE:	//��������	���粻֧��
	case CFG_DATA_NULL:		//��Ч��������
		XMem::XDELETE(m_pVariable);
		break;
	}
}
XBool XConfigManager::mouseProc(const XVec2& p, XMouseState mouseState)		//������궯������Ӧ����
{//��δ���
	if (!m_isInited) return XFalse;
	switch (mouseState)
	{
	case MOUSE_MOVE:
		if (m_isMouseDown)
		{//���������ק�¼�
			setPosition(m_position + p - m_mousePos);
			m_mousePos = p;
			//if(!tmpRect.isInRect(x,y)) m_isMouseDown = false;
		}
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if (XRect(m_position, m_position + 32.0f * m_scale).isInRect(p))
		{
			m_isMouseDown = true;
			m_mousePos = p;
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		m_isMouseDown = false;
		break;
	default:
		break;
	}
	return XFalse;
}
XConfigManager::XConfigManager()
	:m_isInited(false)
	, m_isVisble(true)
	, m_position(0.0f)
	, m_scale(1.0)
	, m_maxHeight(300.0f)
	, m_width(256.0f)
	, m_curInsertPos(0.0f)
	, m_configMode(CFG_MODE_NORMAL)
	//	,m_minuteIndex(-1)
	//	,m_operateIndex(-1)	//û�ж���
	, m_textColor(0.0f, 1.0f)
	, m_isMouseDown(false)
	, m_withBackground(false)
	, m_saveAndLoadFilename(CFG_DEFAULT_FILENAME)
{
	m_withConfigManager = true;	//������ù������Ѿ���ʹ��
	m_saveBtn = XMem::createMem<XButton>();
	if (m_saveBtn == NULL) LogStr("Mem Error!");
	m_loadBtn = XMem::createMem<XButton>();
	if (m_loadBtn == NULL) LogStr("Mem Error!");
	m_defaultBtn = XMem::createMem<XButton>();
	if (m_defaultBtn == NULL) LogStr("Mem Error!");
	m_undoBtn = XMem::createMem<XButton>();
	if (m_undoBtn == NULL) LogStr("Mem Error!");
	m_redoBtn = XMem::createMem<XButton>();
	if (m_redoBtn == NULL) LogStr("Mem Error!");
	m_netUpdateBtn = XMem::createMem<XButton>();
	if (m_netUpdateBtn == NULL) LogStr("Mem Error!");
	m_netInjectBtn = XMem::createMem<XButton>();
	if (m_netInjectBtn == NULL) LogStr("Mem Error!");
	m_netClient = XMem::createMem<XNetClient>();
	if (m_netClient == NULL) LogStr("Mem Error!");
	m_netServer = XMem::createMem<XNetServer>();
	if (m_netServer == NULL) LogStr("Mem Error!");
}
XConfigManager::~XConfigManager()
{
	release();
	XMem::XDELETE(m_saveBtn);
	XMem::XDELETE(m_loadBtn);
	XMem::XDELETE(m_defaultBtn);
	XMem::XDELETE(m_undoBtn);
	XMem::XDELETE(m_redoBtn);
	XMem::XDELETE(m_netUpdateBtn);
	XMem::XDELETE(m_netInjectBtn);
	XMem::XDELETE(m_netClient);
	XMem::XDELETE(m_netServer);
}
template<typename T>
int XConfigManager::addAItem(T *p, XConfigDataType type, const char* name,
	T max, T min, T def,
	void(*changeProc)(void*, void*), const char* groupName, void* pClass)	//����ID��-1Ϊʧ��
{
	if (type >= CFG_DATA_CUSTOM || p == NULL) return -1;	//���ݲ��Ϸ�
	if (p != NULL && isItemExist(p)) return -1;
	XConfigItem *pItem = XMem::createMem<XConfigItem>();
	if (pItem == NULL ||
		type == CFG_DATA_CUSTOM) return -1;	//�Զ���ؼ�����ʹ�����ַ�ʽ���
	if (name == NULL) pItem->m_name = "";
	else pItem->m_name = name;
	pItem->m_changeProc = changeProc;
	pItem->m_pClass = pClass;
	char tempStr[1024];
	switch (type)
	{
	case CFG_DATA_INT:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_curValue.valueI = *(int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			XSliderEx *pCtrl = XMem::createMem<XSliderEx>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutSkin(XVec2(m_width, CFG_MNG_H_FONT),
				pItem->m_rangeMax.valueI, pItem->m_rangeMin.valueI);
			pCtrl->setScale(m_scale);
			pCtrl->setTextColor(m_textColor);
			if (name == NULL) pCtrl->setFont("%%.0f");
			else
			{
				sprintf_s(tempStr, 1024, "%s:%%.0f", name);
				pCtrl->setFont(tempStr);
			}

			pCtrl->setEventProc(ctrlProc, this);
			pCtrl->setCurValue(*(int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_CHAR:
		pItem->m_defaultValue.valueI = (char)def;
		pItem->m_curValue.valueI = *(char *)p;
		pItem->m_rangeMin.valueI = (char)min;
		pItem->m_rangeMax.valueI = (char)max;
		{
			XSliderEx *pCtrl = XMem::createMem<XSliderEx>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutSkin(XVec2(m_width, CFG_MNG_H_FONT),
				pItem->m_rangeMax.valueI, pItem->m_rangeMin.valueI);
			pCtrl->setScale(m_scale);
			pCtrl->setTextColor(m_textColor);
			if (name == NULL) pCtrl->setFont("%%.0f");
			else
			{
				sprintf_s(tempStr, 1024, "%s:%%.0f", name);
				pCtrl->setFont(tempStr);
			}

			pCtrl->setEventProc(ctrlProc, this);
			pCtrl->setCurValue(*(char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_UCHAR:
		pItem->m_defaultValue.valueI = (unsigned char)def;
		pItem->m_curValue.valueI = *(unsigned char *)p;
		pItem->m_rangeMin.valueI = (unsigned char)min;
		pItem->m_rangeMax.valueI = (unsigned char)max;
		{
			XSliderEx *pCtrl = XMem::createMem<XSliderEx>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutSkin(XVec2(m_width, CFG_MNG_H_FONT),
				pItem->m_rangeMax.valueI, pItem->m_rangeMin.valueI);
			pCtrl->setScale(m_scale);
			pCtrl->setTextColor(m_textColor);
			if (name == NULL) pCtrl->setFont("%%.0f");
			else
			{
				sprintf_s(tempStr, 1024, "%s:%%.0f", name);
				pCtrl->setFont(tempStr);
			}

			pCtrl->setEventProc(ctrlProc, this);
			pCtrl->setCurValue(*(unsigned char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_FLOAT:
		pItem->m_defaultValue.valueF = (float)def;
		pItem->m_curValue.valueF = *(float *)p;
		pItem->m_rangeMin.valueF = (float)min;
		pItem->m_rangeMax.valueF = (float)max;
		{
			XSliderEx *pCtrl = XMem::createMem<XSliderEx>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutSkin(XVec2(m_width, CFG_MNG_H_FONT),
				pItem->m_rangeMax.valueF, pItem->m_rangeMin.valueF);
			pCtrl->setScale(m_scale);
			pCtrl->setTextColor(m_textColor);
			if (name == NULL) pCtrl->setFont("%%.4f");
			else
			{
				sprintf_s(tempStr, 1024, "%s:%%.4f", name);
				pCtrl->setFont(tempStr);
			}

			pCtrl->setEventProc(ctrlProc, this);
			//���ﲻ���������ݣ�����������ݵĻ�����������ݱ仯��ʱ����ûص�������ʱ�������Ѿ��ı������ִ�лص������е���ش���
			//pCtrl->setConnectVar((float *)p);
			pCtrl->setCurValue(*(float *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_XBOOL:
		pItem->m_defaultValue.valueB = (XBool)def;
		pItem->m_curValue.valueB = *(XBool *)p;
		pItem->m_rangeMin.valueB = (XBool)min;
		pItem->m_rangeMax.valueB = (XBool)max;
		{
			XCheck *pCtrl = XMem::createMem<XCheck>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			if (name == NULL) pCtrl->initWithoutSkin(" ", getDefaultFont(), 1.0f, XRect(0.0f, CFG_MNG_H_FONT),
				XVec2(CFG_MNG_H_FONT, CFG_MNG_H_FONT * 0.5f));
			else pCtrl->initWithoutSkin(name, getDefaultFont(), 1.0f, XRect(0.0f, CFG_MNG_H_FONT),
				XVec2(CFG_MNG_H_FONT, CFG_MNG_H_FONT * 0.5f));
			pCtrl->setScale(m_scale);
			pCtrl->setEventProc(ctrlProc, this);
			pCtrl->setConnectVar((XBool *)p);
			pCtrl->setTextColor(m_textColor);
			if (*(XBool *)p) pCtrl->setState(XTrue);
			else pCtrl->setState(XFalse);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_RADIOS:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_curValue.valueI = *(int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			XRadios *pCtrl = XMem::createMem<XRadios>();
			if (pCtrl == NULL) return -1;
			if (m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutSkin(1, XVec2(0.0f, CFG_MNG_H_FONT + 2.0f), XRect(0.0f, CFG_MNG_H_FONT), getDefaultFont(), 1.0f,
				XVec2(CFG_MNG_H_FONT + 2.0f, CFG_MNG_H_FONT * 0.5f));
			pCtrl->setRadiosText(name);
			pCtrl->setScale(m_scale);
			pCtrl->setTextColor(m_textColor);
			pCtrl->setEventProc(ctrlProc, this);
			pCtrl->setChoosed(*(int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	}
	pItem->m_isEnable = true;
	pItem->m_pVariable = p;
	pItem->m_type = type;
	m_pItems.push_back(pItem);
	XConfigGroup* gp = nullptr;
	if (groupName == NULL) gp = m_pGroups[0];//ʹ��Ĭ����
	else gp = getGroup(groupName);
	if (gp != nullptr)
	{
		gp->m_items.push_back(pItem);
		if (pItem->m_pCtrl != NULL)
		{
			gp->m_group.pushChild(pItem->m_pCtrl);
			if (gp->m_group.getState() == XGroup::STATE_MINISIZE) 
				pItem->m_pCtrl->disVisible();
		}
		relayout();
	}
	return pItem->getID();
}
#if !WITH_INLINE_FILE
#include "XConfigManager.inl"
#endif
}