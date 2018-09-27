#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionManager.h"
#include "../XDirectory.h"
namespace XE{
void XActionMananger::loadActionDesFromFold(const XDir * dir)
{
	if(dir == NULL) return;	//�յ��ļ���ֱ�ӷ���
	int sum = dir->files.size();
	if(sum <= 0) return;
	XFileInfo * tempFile = NULL;
	for(int i = 0;i < sum;++ i)
	{
		tempFile = dir->files[i];
		if(tempFile->isDirectory)
		{//�ļ���
			loadActionDesFromFold(tempFile->directory);
		}else
		{
			if(isActionDescriptionFile(tempFile->filename.c_str()))
			{//�Ϸ����ļ�
				XActionDescription * tempAD = XMem::createMem<XActionDescription>();
				if(tempAD == NULL)
				{
					printf("ERROR:Memory ERROR!\n");
					return;
				}
				if(!tempAD->loadAction(tempFile->allPath.c_str(),RES_LOCAL_FOLDER))	//��Դλ�õĲ�������Ҫ��ȶ
				{
					printf("ERROR:Action description load ERROR:%s\n",tempFile->allPath.c_str());
					return;
				}
				m_actionDescriptions.insert(std::pair<int,XActionDescription *>(tempAD->getID(),tempAD));
				m_actionDesMap.insert(std::pair<std::string,int>(tempAD->getName(),tempAD->getID()));
				m_actionIDList.push_back(tempAD->getID());
			}
		//	else
		//	{//��Ч���ļ�
		//	}
		}
	}
}
XBool XActionMananger::loadAllActionDes(const char * path)	//��ָ��Ŀ¼�µĶ���ȫ������ 
{//�ڲ���Դ��ȡ���������
	if(m_isInited) return XFalse;
	char tempStr[MAX_FILE_NAME_LENGTH];
	if(path == NULL) strcpy_s(tempStr,MAX_FILE_NAME_LENGTH,ACTION_DES_PATH);	//ʹ��Ĭ��·��
	else strcpy_s(tempStr,MAX_FILE_NAME_LENGTH,path);

	XDirectory tempDir;
	if(!tempDir.initEx(tempStr))
	{
		if(!tempDir.init(tempStr)) return XFalse;
	}
	loadActionDesFromFold(&tempDir.m_curDirectory);
	//�������еĶ���������������Ӧ�ù�ϵ�����
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	XActionDescription * tempAD = NULL;
	for(;it != m_actionDescriptions.end();++ it)
	{
		int sum = it->second->m_otherActionID.size();
		for(int i = 0;i < sum;++ i)
		{
			tempAD = getActionDes(it->second->m_otherActionID[i]);
			if(tempAD == NULL)
			{//�Ҳ�����Ҫ�����ݣ����������صĴ��󣬽��ᵼ�����ݲ����������ܽ��в���
				printf("ERROR:Action description data is incomplete!\n");
				return XFalse;
			}
			it->second->pushAActionDescription(tempAD);
		}
	}

	m_isInited = XTrue;
	return XTrue;
}
XBool XActionMananger::saveAllActionDes()
{
	if(!m_isInited) return XFalse;
	//�������еĶ���������
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second != NULL) it->second->saveAction();
	}
	return XTrue;
}
XBool XActionMananger::getIsReferenced(XActionDescription * AD)
{
	if(AD == NULL) return XFalse;
	//�������е�AD���������
	XActionDescription *tempAD = NULL;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		tempAD = it->second;
		int allADSum = tempAD->getOtherActionSum();
		if(allADSum <= 0) continue;
		for(int i = 0;i < allADSum;++ i)
		{
			if(tempAD->getOtherAD(i) == AD) return XTrue;	//��Ӧ��
		}
	}
	return XFalse;		//û�б�����
}
XBool XActionMananger::isActionDescriptionFile(const char * filename)
{
	if(filename == NULL) return XFalse;
	int len = strlen(filename);
	if( (filename[len - 1] == 'd' || filename[len - 1] == 'D') &&
		(filename[len - 2] == 'c' || filename[len - 2] == 'C') &&
		(filename[len - 3] == 'a' || filename[len - 3] == 'A') &&
		filename[len - 4] == '.')
	{
		return XTrue;
	}
	return XFalse;
}
void XActionMananger::release()	//��Դ�ͷ�
{//�ͷ��Ѿ��������Դ
	if(!m_isInited) return;
	//�������е�AD���ͷ�
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		XMem::XDELETE(it->second);
	}
	m_actionDescriptions.clear();

	m_actionDesMap.clear();
	m_actionIDList.clear();
	m_isInited = XFalse;
}
void XActionMananger::deleteAActionDesNoReference(XActionDescription * AD)
{//ɾ��ָ����AD
	if(AD == NULL) return;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second == AD)
		{
			std::map<std::string,int>::iterator itTemp = m_actionDesMap.find(AD->getName());	//ɾ��ָ����
			m_actionDesMap.erase(itTemp);

			int sum = m_actionIDList.size();
			for(int i = 0;i < sum;++ i)
			{
				if(m_actionIDList[i] == AD->getID())
				{
				//	for(int j = i;j < sum - 1;++ j)
				//	{
				//		m_actionIDList[j] = m_actionIDList[j + 1];
				//	}
				//	m_actionIDList.pop_back();
					m_actionIDList.erase(m_actionIDList.begin() + i);
					break;
				}
			}

			//û���ͷ����AD�������Դ
			XActionObjectManager::GetInstance().decreaseAObject(AD->getObject());
			XMem::XDELETE(it->second);
			m_actionDescriptions.erase(it);
			break;
		}
	}
}
XBool XActionMananger::setADName(XActionDescription * AD,const char *name)
{
	if(AD == NULL) return XFalse;
	if(name == NULL) return XFalse;
	std::map<int,XActionDescription *>::iterator it = m_actionDescriptions.begin();
	for(;it != m_actionDescriptions.end();++ it)
	{
		if(it->second == AD)
		{
			std::map<std::string,int>::iterator itTemp = m_actionDesMap.find(AD->getName());	//ɾ��ָ����
			if(AD->setName(name))
			{//�����ɹ����޸�������index�Ķ�Ӧ��ϵ
				m_actionDesMap.erase(itTemp);
				m_actionDesMap.insert(std::pair<std::string,int>(AD->getName(),AD->getID()));
				return XTrue;
			}
			break;
		}
	}
	return XFalse;
}
}