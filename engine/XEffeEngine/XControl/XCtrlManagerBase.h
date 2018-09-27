#ifndef _JIA_XCTRLMANAGERBASE_
#define _JIA_XCTRLMANAGERBASE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.9
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"
namespace XE{
//����һ���ؼ�����Ļ��࣬���д��пؼ������������඼Ҫ�̳������������ǰ��������ĳ�Ա
struct XCtrlObjetInfo
{
	XControlBasic *pObject;	//�ؼ���ָ��
	void (*pFunction)(void);	//���ƺ�����ָ��
	XBool needDraw;			//�Ƿ�ӵ�л���Ȩ��
	XBool needInput;			//�Ƿ�ӵ�����봦��Ȩ��
	XCtrlObjetInfo()
		:pObject(NULL)
		,pFunction(NULL)
		,needDraw(XFalse)
		,needInput(XFalse)
	{}
};
class XGameBasic;
class XCtrlManagerBase
{
public:
	XCtrlManagerBase()
		:m_isAutoDraw(1)
		,m_focusOrder(-1)
		,m_isShiftDown(false)
		,m_pGame(NULL)
		,m_withWireframe(true)
	{}
	XCtrlManagerBase(const XCtrlManagerBase&);
	virtual ~XCtrlManagerBase(){;}
protected:
	std::vector<XControlBasic *> m_specialCtrls;	//����Ŀؼ���Ŀǰ�ù�����δʵ��
	std::vector<XCtrlObjetInfo> m_ctrlObjInfos;
	std::vector<int> m_ctrlDrawOrderMap;	//���������ʹ�õ�ʱ��������⣡��
	char m_isAutoDraw;	//�Ƿ��Լ�����ť�Ļ���
	bool m_isShiftDown;
	bool m_withWireframe;	//�Ƿ��ڿؼ��������һ���߿�
	int m_focusOrder;	//��ǰ��ý���Ŀؼ��ı�ţ�ֻ�л�ý���Ŀؼ����ܶ��������¼�������Ӧ,���Ϊԭʼ��ţ���ӳ�����
public:
	int findSpecialObjectID(const void * object) const;	//Ѱ����������

	unsigned int getObjSum() const {return m_ctrlObjInfos.size();}
	XBool addACtrl(XControlBasic * object);			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	XBool addAFunction(void (* function)());			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	void setNeedDraw(XBool flag,unsigned int objectID);			//����ĳ������Ƿ��й�������ʾ
	void setNeedDraw(XBool flag,const void *object);	//����ĳ������Ƿ��й�������ʾ
	void setNeedInput(XBool flag,unsigned int objectID);			//����ĳ������Ƿ��й�������ʾ
	void setNeedInput(XBool flag,const void *object);	//����ĳ������Ƿ��й�������ʾ
	void setNeedDrawAndInput(bool needDraw,bool needInput,const void *object)
	{
		int id = findObjectID(object);
		setNeedDraw(needDraw,id);
		setNeedInput(needInput,id);
	}
	void setNeedDrawAndInput(bool needDraw,bool needInput,unsigned int objectID)
	{
		setNeedDraw(needDraw,objectID);
		setNeedInput(needInput,objectID);
	}
	void decreaseAObject(int objectID);					//ȥ��һ�������ע��,�����������˳��
	void decreaseAObject(const void * object);			//ȥ��һ�������ע��
	void decreaseAObject(void (* function)());			//ȥ��һ�������ע��
	void clearAllObject();								//������е����
	int findObjectID(const void * object) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	int findObjectID(void (* function)()) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	XCtrlObjType getObjectType(unsigned int objectID) const;	//��ȡ���������
	void *getObject(unsigned int objectID) const;				//��ȡ�����ָ��
	//Ϊ�˴ﵽ��ͼ˳���޸��Լ���Ϣ����˳���޸ĵ�Ŀ�ģ������ṩһ�麯�����ڵ����ؼ���ע��˳����δ�������ԣ�
	void moveAndInsert(int objectID,int insertObjectID);	//�����objectID���뵽insertID��λ�ã���������֮������ȫ��˳���ƶ�
	void moveToFirst(int objectID){moveAndInsert(objectID,m_ctrlDrawOrderMap[0]);}
	void moveToEnd(int objectID){moveAndInsert(objectID,m_ctrlDrawOrderMap[(int)(m_ctrlDrawOrderMap.size()) - 1]);}
	void changeTowObject(unsigned int object1,unsigned int object2);				//��object1��object2����������Ե�
	void setIsAutoDraw(XBool isAutoDraw)
	{
		if(isAutoDraw) m_isAutoDraw = 1;
		else m_isAutoDraw = 0;
	}
public:
	void setWithWireframe(bool flag){m_withWireframe = flag;}
	void mouseProc(const XVec2& p,XMouseState mouseState);		//����¼��ĺ���
	void keyProc(int keyOrder,XKeyState keyState);			//�����¼��ĺ���
	void insertChar(const char *ch,int len);
	void draw();
	void drawUp();
	void update(float stepTime);

	void draw(const void * obj);	//�ֶ����ĳ������
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ���Ż����ؼ��Ļص�����������ƶ�����ı���
	XGameBasic *m_pGame;
	void eventProc(int id,int eventID);
	void setGame(XGameBasic *pGame){m_pGame = pGame;}
	//---------------------------------------------------------
};
inline void XCtrlManagerBase::setNeedDraw(XBool flag,unsigned int objectID)
{
	if(objectID >= m_ctrlObjInfos.size()) return;	//��������Ƿ�
	m_ctrlObjInfos[objectID].needDraw = flag;
}
inline void XCtrlManagerBase::setNeedDraw(XBool flag,const void *object)
{
	setNeedDraw(flag,findObjectID(object));
}
inline void XCtrlManagerBase::setNeedInput(XBool flag,unsigned int objectID)
{
	if(objectID >= m_ctrlObjInfos.size()) return;	//��������Ƿ�
	m_ctrlObjInfos[objectID].needInput = flag;
}
inline void XCtrlManagerBase::setNeedInput(XBool flag,const void *object)
{
	setNeedInput(flag,findObjectID(object));
}
inline void XCtrlManagerBase::decreaseAObject(const void * object)
{
	int ret = findObjectID(object);
	if(ret >= 0) decreaseAObject(ret);
	else
	{
		ret = findSpecialObjectID(object);
		if(ret < 0) return;
		m_specialCtrls.erase(m_specialCtrls.begin() + ret);
	}
}
inline void XCtrlManagerBase::decreaseAObject(void (* function)())
{
	decreaseAObject(findObjectID(function));
}
inline XCtrlObjType XCtrlManagerBase::getObjectType(unsigned int objectID) const
{
	if(objectID >= m_ctrlObjInfos.size()) return CTRL_OBJ_NULL;	//��������Ƿ�
	if(m_ctrlObjInfos[objectID].pObject == NULL) return CTRL_OBJ_FUNCTION;
	else return m_ctrlObjInfos[objectID].pObject->getCtrlType();
}
inline void *XCtrlManagerBase::getObject(unsigned int objectID) const
{
	if(objectID >= m_ctrlObjInfos.size()) return NULL;	//��������Ƿ�
	return m_ctrlObjInfos[objectID].pObject;
}
inline void XCtrlManagerBase::changeTowObject(unsigned int object1,unsigned int object2)				//��object1��object2����������Ե�
{
	if(object1 >= m_ctrlObjInfos.size() ||
		object2 >= m_ctrlObjInfos.size() ||
		object1 == object2) return;	//����Ĳ���
	int index1 = -1;
	int index2 = -1;
	for(unsigned int i = 0;i < m_ctrlDrawOrderMap.size();++ i)
	{
		if(m_ctrlDrawOrderMap[i] < 0) continue;
		if((unsigned int)m_ctrlDrawOrderMap[i] == object1) index1 = i;
		if((unsigned int)m_ctrlDrawOrderMap[i] == object2) index2 = i;
		if(index1 >= 0 && index2 >= 0) break;
	}
	XMath::swapTwoSum(m_ctrlDrawOrderMap[index1],m_ctrlDrawOrderMap[index2]);
	//XMath::swapTwoSum(m_ctrlDrawOrderMap[object1],m_ctrlDrawOrderMap[object2]);
}
inline void XCtrlManagerBase::insertChar(const char *ch,int len)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_ctrlDrawOrderMap.size()) return;
	if(m_ctrlObjInfos[m_focusOrder].pObject != NULL) 
			m_ctrlObjInfos[m_focusOrder].pObject->insertChar(ch,len);
}
inline void XCtrlManagerBase::draw(const void * obj)	//�ֶ����ĳ������
{
//	if(m_isAutoDraw != 0 ||
//		findObjectID(obj) < 0) return;
	if(obj == NULL) return;
	((XControlBasic *)obj)->draw();
	((XControlBasic *)obj)->drawUp();
}
}
#endif