#ifndef _JIA_XACTIONMANAGER_
#define _JIA_XACTIONMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XActionDescription.h"
#include <map>
namespace XE{
//��ָ��Ŀ¼�µ�����AD�ļ�ȫ�����룬������AD���Ʊ���
//����Դ�ļ��ж�ȡ���ݻ�������⣬�����޸�
class XDir;
class XActionMananger
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	XActionMananger()
		:m_isInited(XFalse)
	{
	}
	XActionMananger(const XActionMananger&);
	XActionMananger &operator= (const XActionMananger&);
	virtual ~XActionMananger() {release();}
public:
	static XActionMananger& GetInstance()
	{
		static XActionMananger m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	XBool m_isInited;	//�Ƿ��Ѿ�����
	std::map<int,XActionDescription *> m_actionDescriptions;	//���еĶ���������
	std::map<std::string,int> m_actionDesMap;
	std::vector<int> m_actionIDList;							//AD��ID�б�

	XBool isActionDescriptionFile(const char * filename);
	//���ļ����ж�ȡ���������ļ�
	void loadActionDesFromFold(const XDir *dir);
public:
	XBool loadAllActionDes(const char * path = NULL);	//��ָ��Ŀ¼�µĶ���ȫ������ //�ڲ���Դ��ȡ���������
	XBool saveAllActionDes();	//�����ж������浽Ĭ��Ŀ¼��

	XBool pushAActionDescription(XActionDescription * tempAD)
	{//���һ��AD������
		if(tempAD == NULL) return XFalse;
		if(getActionDes(tempAD->getName()) != NULL) return XFalse;	//ͬ��
		if(getActionDes(tempAD->getID()) != NULL) return XFalse;		//ͬID
		//����һ��
		m_actionDescriptions.insert(std::pair<int,XActionDescription *>(tempAD->getID(),tempAD));
		m_actionDesMap.insert(std::pair<std::string,int>(tempAD->getName(),tempAD->getID()));
		m_actionIDList.push_back(tempAD->getID());
		return XTrue;
	}
	XActionDescription *getActionDes(int ID)		//����ID����
	{
		if(m_actionDescriptions.count(ID) == 0) return NULL;
		return m_actionDescriptions[ID];
	}
	XActionDescription *getActionDes(const char * name)	//�������ֵ���
	{
		if(name == NULL) return NULL;
		if(m_actionDesMap.count(name) == 0) return NULL;
		int id = m_actionDesMap[name];
		return getActionDes(id);
	}
	XActionDescription *getActionDesByIndex(int index)	//��Ҫ��Ϊ�˱���
	{
		int sum = m_actionIDList.size();
		if(index < 0 || index >= sum) return NULL;
		return getActionDes(m_actionIDList[index]);
	}
	XBool getIsReferenced(XActionDescription * AD);
	int getADSum()
	{
		if(!m_isInited) return 0;
		return m_actionDesMap.size();
	}
//++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�AD�Ŀ��ƽӿ�(��δʵ��)
	//void play(int ID);		//���Ŷ���
	//void stop(int ID);		//ֹͣ��������
	//void pause(int ID);		//��ͣ����
	//void setShow(int ID);	//��ʾ����
	//void disShow(int ID);	//���ض���
	//XBool getIsEnd(int ID);	//�ж϶����Ƿ񲥷����

	//void play(const char * name);
	////....����Ķ�Ӧ�ӿ�
	//void draw();
	//void move(float stepTime);
//--------------------------------------------------

	void release();	//��Դ�ͷ�	
	void deleteAActionDesNoReference(XActionDescription * AD);
	XBool setADName(XActionDescription * AD,const char *name);
};
}
#endif