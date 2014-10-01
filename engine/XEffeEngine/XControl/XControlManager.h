#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"

//#define MAX_CTRL_OBJ_SUM (512)	//�ؼ�������������ע����ܿؼ�����

struct _XCtrlObjetInfo
{
	_XControlBasic *pObject;	//�ؼ���ָ��
	void (*pFunction)(void);	//���ƺ�����ָ��
//	_XCtrlObjType type;			//�ؼ�������
	_XBool needDraw;			//�Ƿ�ӵ�л���Ȩ��
	_XBool neadInput;			//�Ƿ�ӵ�����봦��Ȩ��
	_XCtrlObjetInfo()
		:pObject(NULL)
		,pFunction(NULL)
//		,type(CTRL_OBJ_NULL)
		,needDraw(XFalse)
		,neadInput(XFalse)
	{}
};
//�������Ҫʹ�õ�����ϵͳ
class _XControlManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//���ֵ���ģʽ����Ʒ�ʽ��֮ǰ��Ҫ��
protected:
	_XControlManager()
		:m_isAutoDraw(1)
		,m_focusOrder(-1)
		,m_isShiftDown(false)
	{
		//for(int i = 0;i < MAX_CTRL_OBJ_SUM;++ i)
		//{
		//	m_pObject[i].pObject = NULL;
		//	m_pObject[i].pFunction = NULL;
		//	m_objectType[i] = CTRL_OBJ_NULL;
		//	m_objectDrawOrderMap[i] = 0;
		//}
	}
	_XControlManager(const _XControlManager&);
	_XControlManager &operator= (const _XControlManager&);
	virtual ~_XControlManager(){;}

public:
	static _XControlManager& GetInstance()
	{
		static _XControlManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	std::vector<_XCtrlObjetInfo> m_ctrlObjInfos;
	std::vector<int> m_ctrlDrawOrderMap;
//	_XCtrlObjetP m_pObject[MAX_CTRL_OBJ_SUM];		//���ע�������
//	_XCtrlObjType m_objectType[MAX_CTRL_OBJ_SUM];	//���������
//	int m_objectSum;	//���������
//	int m_objectDrawOrderMap[MAX_CTRL_OBJ_SUM];		//������˳���ӳ���
	char m_isAutoDraw;	//�Ƿ��Լ�����ť�Ļ���
	int m_focusOrder;	//��ǰ��ý���Ŀؼ��ı�ţ�ֻ�л�ý���Ŀؼ����ܶ��������¼�������Ӧ
	bool m_isShiftDown;

public:
	int getObjSum() const {return m_ctrlObjInfos.size();}
	_XBool addACtrl(_XControlBasic * object);			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	_XBool addAFunction(void (* function)());			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	void setNeedDraw(_XBool flag,int objectID);			//����ĳ������Ƿ��й�������ʾ
	void setNeedDraw(_XBool flag,const void *object);	//����ĳ������Ƿ��й�������ʾ
	void setNeedInput(_XBool flag,int objectID);			//����ĳ������Ƿ��й�������ʾ
	void setNeedInput(_XBool flag,const void *object);	//����ĳ������Ƿ��й�������ʾ
	void decreaseAObject(int objectID);					//ȥ��һ�������ע��,�����������˳��
	void decreaseAObject(const void * object);			//ȥ��һ�������ע��
	void decreaseAObject(void (* function)());			//ȥ��һ�������ע��
	int findObjectID(const void * object) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	int findObjectID(void (* function)()) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	_XCtrlObjType getObjectType(int objectID) const;	//��ȡ���������
	void *getObject(int objectID) const;				//��ȡ�����ָ��
	//Ϊ�˴ﵽ��ͼ˳���޸��Լ���Ϣ����˳���޸ĵ�Ŀ�ģ������ṩһ�麯�����ڵ����ؼ���ע��˳����δ�������ԣ�
	void moveAndInsert(int objectID,int insertObjectID);	//�����objectID���뵽insertID��λ�ã���������֮������ȫ��˳���ƶ�
	void changeTowObject(int object1,int object2);				//��object1��object2����������Ե�
	void setIsAutoDraw(_XBool isAutoDraw)
	{
		if(isAutoDraw) m_isAutoDraw = 1;
		else m_isAutoDraw = 0;
	}
public:
	void mouseProc(int x,int y,_XMouseState mouseState);		//����¼��ĺ���
	void keyProc(int keyOrder,_XKeyState keyState);			//�����¼��ĺ���
	void insertChar(const char *ch,int len);
	void draw();
	void update(int stepTime);

	void draw(const void * obj);	//�ֶ����ĳ������
};
#define _XCtrlManger _XControlManager::GetInstance()
inline void _XControlManager::setNeedDraw(_XBool flag,int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//��������Ƿ�
	m_ctrlObjInfos[objectID].needDraw = flag;
}
inline void _XControlManager::setNeedDraw(_XBool flag,const void *object)
{
	setNeedDraw(flag,findObjectID(object));
}
inline void _XControlManager::setNeedInput(_XBool flag,int objectID)
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return;	//��������Ƿ�
	m_ctrlObjInfos[objectID].neadInput = flag;
}
inline void _XControlManager::setNeedInput(_XBool flag,const void *object)
{
	setNeedInput(flag,findObjectID(object));
}
inline void _XControlManager::decreaseAObject(const void * object)
{
	decreaseAObject(findObjectID(object));
}
inline void _XControlManager::decreaseAObject(void (* function)())
{
	decreaseAObject(findObjectID(function));
}
inline _XCtrlObjType _XControlManager::getObjectType(int objectID) const
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return CTRL_OBJ_NULL;	//��������Ƿ�
	if(m_ctrlObjInfos[objectID].pObject == NULL) return CTRL_OBJ_FUNCTION;
	else return m_ctrlObjInfos[objectID].pObject->getCtrlType();
}
inline void *_XControlManager::getObject(int objectID) const
{
	if(objectID < 0 || objectID >= m_ctrlObjInfos.size()) return NULL;	//��������Ƿ�
	return m_ctrlObjInfos[objectID].pObject;
}
inline void _XControlManager::changeTowObject(int object1,int object2)				//��object1��object2����������Ե�
{
	if(object1 < 0 || object1 >= m_ctrlObjInfos.size() ||
		object2 < 0 || object2 >= m_ctrlObjInfos.size() ||
		object1 == object2) return;	//����Ĳ���
	xChangeTwoSum(m_ctrlDrawOrderMap[object1],m_ctrlDrawOrderMap[object2]);
}
inline void _XControlManager::insertChar(const char *ch,int len)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlDrawOrderMap.size()) return;
	int index = m_ctrlDrawOrderMap[m_focusOrder];
	if(m_ctrlObjInfos[index].pObject != NULL) 
			m_ctrlObjInfos[index].pObject->insertChar(ch,len);
}
inline void _XControlManager::draw(const void * obj)	//�ֶ����ĳ������
{
	if(m_isAutoDraw != 0 ||
		findObjectID(obj) < 0) return;
	((_XControlBasic *)obj)->draw();
	((_XControlBasic *)obj)->drawUp();
}
#endif