#ifndef _JIA_XOPRATEDES_
#define _JIA_XOPRATEDES_
//����������undo��redo������Ķ��������Ͷ���������࣬�������Ҫʹ��undo��redo����ֻ��Ҫ���ڸû��༴��
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.6.23
//--------------------------------
#include "XBasicClass.h"
#include <vector>

class _XBasicOprate;
//����ṹ��������һ������
struct _XOperateDes
{
	_XBasicOprate *pObj;	//����������ʵ��ָ��
	void *pData;			//�����������״̬�����ݣ���ͬ��ʵ�����ݽṹ�����ݲ�ͬ
	_XOperateDes()
		:pObj(NULL)
		,pData(NULL)
	{}
};
//����Ƕ��������������������������ԭ��Ϊ�������
//״̬���У�0��1��2��3��4��5��6��7��8��9����	�������ܹ��������������ʱ���Ⱥ�״̬�仯�����������
//�����������У�5��7��9����						��һ������������һϵ�е�״̬�仯��ɣ�����ȷһ���������ʱ����״̬���еı�������������
//��ǰ�����������ţ�0��							�����ڼ�¼��ǰ�����ĸ�������ͨ���ı����ֵ��ʵ�ֶ����Ļ���
class _XOperateManager
{
protected:
	_XOperateManager()
		:m_nowStateIndex(-1)
	{}
	virtual~_XOperateManager(){clearAll();}
	_XOperateManager(const _XOperateManager&);
	_XOperateManager &operator= (const _XOperateManager&);
public:
	static _XOperateManager& GetInstance()
	{
		static _XOperateManager m_instance;
		return m_instance;
	}
public:
	std::vector<_XOperateDes> m_operateDes;			//״̬�����Ķ���
	std::vector<int> m_operateIndex;				//������Ӧ��״̬��ŵĶ���
	int m_nowStateIndex;							//��ǰ�����ı��
public:
	void clearAll();	//������еĶ�������
	void undo();		//��������
	void redo();		//��������
	_XBool canUndo() {return (m_nowStateIndex > 0 && m_operateIndex.size() > 0);}	//�ж��Ƿ��������������
	_XBool canRedo() {return (m_nowStateIndex < m_operateIndex.size() - 1);}		//�ж��Ƿ��������������
	void addAOperate(_XBasicOprate *obj);	//��״̬���������һ��״̬�����֮ǰ��������������ᶪ������ǰ״̬֮���״̬
	void setOperateOver();		//���ö�����ɣ���һϵ�е�״̬�仯�����һ������
	void decreaseObj(_XBasicOprate *obj);	//�������н�ָ����������в��������
};

#define _XOpManager _XOperateManager::GetInstance()

class _XBasicOprate	//������Ҫ����undo��redo���඼��Ҫ�̳��������
{
protected:
	bool m_withUndoAndRedo;				//��ʾ�������ڱ�Ƕ����Ƿ���Ҫ��������������������������Ļ��������������붯��������
	void *m_pStateClass;				//�ص������е�ָ��ָ��ص�������ʵ��
	void (*m_funStateChange)(void *);	//״̬�����仯�ǵ��õĻص�����
public:
	virtual void setWithUndo(bool flag) {m_withUndoAndRedo = flag;}	//�������߹رն�������
	virtual void setOprateState(void * data) = 0;		//��������ĵ�ǰ״̬��dataΪ������ǰ״̬�����ݣ���ͬ��ʵ��data�Ľṹ�����ݲ�ͬ
	virtual void *getOprateState() const = 0;			//��ȡ����������ǰ״̬�����ݣ��������ݵ�ָ�룬������ݵ������ɶ�������������

	virtual void stateChange()			//״̬�����仯ʱ��Ҫ���ö������붯���������ǵ��ã��������𽫵�ǰ�������浽������������������״̬�仯�Ļص�����
	{
		if(m_withUndoAndRedo) _XOpManager.addAOperate(this);	//�����Ҫ��¼��ǰ״̬�򽫵��ö�������������غ���
		if(m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//������صĻص�����
	}

	virtual void setStateChangeCB(void (* funStateChange)(void *),void * p)	//����״̬�仯ʱ���õĻص�����
	{
		m_funStateChange = funStateChange;
		m_pStateClass = p;
	}
	virtual bool isSameState(void * data) = 0;	//�ж�data��������״̬�͵�ǰ״̬�Ƿ�һ��	//Ϊ�˿���չ��Ӧ�øĳ��ж����������Ƿ�һ��(��δ����)

	_XBasicOprate()
		:m_withUndoAndRedo(false)
		,m_pStateClass(NULL)
		,m_funStateChange(NULL)
	{}
	//���ע����ʱ����Ҫ�������������б���صĶ���ע�����Ա�֤�����ĺ�����
	virtual ~_XBasicOprate() {_XOpManager.decreaseObj(this);}
};

#include "XOprateDes.inl"

#endif