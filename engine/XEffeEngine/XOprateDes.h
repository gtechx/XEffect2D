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
namespace XE{
class XBasicOprate;
//����ṹ��������һ������
struct XOperateDes
{
	XBasicOprate *pObj;	//����������ʵ��ָ��
	void *pData;			//�����������״̬�����ݣ���ͬ��ʵ�����ݽṹ�����ݲ�ͬ
	XOperateDes()
		:pObj(NULL)
		, pData(NULL)
	{}
	~XOperateDes()
	{}
	void release();	//������ܴ������⣬��pObj��ʵ�屻�ͷ�ʱ����������crash��
};
//����Ƕ��������������������������ԭ��Ϊ�������
//״̬���У�0��1��2��3��4��5��6��7��8��9����	�������ܹ��������������ʱ���Ⱥ�״̬�仯�����������
//�����������У�5��7��9����						��һ������������һϵ�е�״̬�仯��ɣ�����ȷһ���������ʱ����״̬���еı�������������
//��ǰ�����������ţ�0��							�����ڼ�¼��ǰ�����ĸ�������ͨ���ı����ֵ��ʵ�ֶ����Ļ���
class XOperateManager
{
public:
	XOperateManager()
		:m_curStateIndex(-1)
	{}
	virtual~XOperateManager() { clearAll(); }
protected:
	XOperateManager(const XOperateManager&);
	XOperateManager &operator= (const XOperateManager&);
public:
	std::vector<XOperateDes> m_operateDes;			//״̬�����Ķ���
	std::vector<int> m_operateIndex;				//������Ӧ��״̬��ŵĶ���
	int m_curStateIndex;							//��ǰ�����ı��
public:
	void clearAll();	//������еĶ�������
	void undo();		//��������
	void redo();		//��������
	XBool canUndo() { return (m_curStateIndex > 0 && m_operateIndex.size() > 0); }	//�ж��Ƿ��������������
	XBool canRedo() { return (m_curStateIndex < (int)(m_operateIndex.size()) - 1); }		//�ж��Ƿ��������������
	void addAOperate(XBasicOprate *obj);	//��״̬���������һ��״̬�����֮ǰ��������������ᶪ������ǰ״̬֮���״̬
	void setOperateOver();		//���ö�����ɣ���һϵ�е�״̬�仯�����һ������
	void decreaseObj(XBasicOprate *obj);	//�������н�ָ����������в��������
};
class XBasicOprate	//������Ҫ����undo��redo���඼��Ҫ�̳��������
{
protected:
	bool m_withUndoAndRedo;				//��ʾ�������ڱ�Ƕ����Ƿ���Ҫ��������������������������Ļ��������������붯��������
	void *m_pStateClass;				//�ص������е�ָ��ָ��ص�������ʵ��
	void(*m_funStateChange)(void *);	//״̬�����仯�ǵ��õĻص�����
public:
	virtual void setWithUndo(bool flag) { m_withUndoAndRedo = flag; }	//�������߹رն�������
	virtual void setOprateState(void * data) = 0;		//��������ĵ�ǰ״̬��dataΪ������ǰ״̬�����ݣ���ͬ��ʵ��data�Ľṹ�����ݲ�ͬ
	virtual void *getOprateState() const = 0;			//��ȡ����������ǰ״̬�����ݣ��������ݵ�ָ�룬������ݵ������ɶ�������������
	virtual void releaseOprateStateData(void *) = 0;	//�ͷ�getOprateState���ɵ��ڴ�ռ䣬����ֻ�з���ĺ���֪�����ͣ�Ϊ���ܹ�����������������������
	virtual void stateChange() = 0;			//״̬�����仯ʱ��Ҫ���ö������붯���������ǵ��ã��������𽫵�ǰ�������浽������������������״̬�仯�Ļص�����
//	{//��bug
//		if(m_withUndoAndRedo) XOpManager.addAOperate(this);	//�����Ҫ��¼��ǰ״̬�򽫵��ö�������������غ���
//		if(m_funStateChange != NULL) m_funStateChange(m_pStateClass);	//������صĻص�����
//	}
	virtual void setStateChangeCB(void(*funStateChange)(void *), void * p)	//����״̬�仯ʱ���õĻص�����
	{
		m_funStateChange = funStateChange;
		m_pStateClass = p;
	}
	virtual bool isSameState(void * data) = 0;	//�ж�data��������״̬�͵�ǰ״̬�Ƿ�һ��	//Ϊ�˿���չ��Ӧ�øĳ��ж����������Ƿ�һ��(��δ����)

	XBasicOprate()
		:m_withUndoAndRedo(false)
		, m_pStateClass(NULL)
		, m_funStateChange(NULL)
	{}
	//���ע����ʱ����Ҫ�������������б���صĶ���ע�����Ա�֤�����ĺ�����
	virtual ~XBasicOprate() {}
//	{//��bug
//		if(gFrameworkData.pOperateManager != NULL)
//			gFrameworkData.pOperateManager->decreaseObj(this);
//	}
};
#if WITH_INLINE_FILE
#include "XOprateDes.inl"
#endif
}
#endif