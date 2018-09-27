#ifndef _JIA_XACTION_
#define _JIA_XACTION_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XBasicFun.h"
#include "stdlib.h"
#include "string.h"
#include <vector>
#include "XCommonDefine.h"
#include "XMemory.h"
namespace XE{
//������ͨ���ص���������ʽ��ʵ�ֶ���������ʹ��C���﷨���򲻷���C++����ƹ���

enum XActionSimpleState
{
	ACTION_STATE_NULL,		//����״̬δ����
	ACTION_STATE_ACTION,	//����������
	ACTION_STATE_PAUSE,	//������ͣ
	ACTION_STATE_END,		//��������
};

//ʹ�ûص���������ʽ��ʾ����
enum XActionSimpleType
{
	ACTION_TYPE_COMPLEX,	//���и��ϵĶ���,�ɺܶ�ԭ�Ӷ������������̵Ķ���
	ACTION_TYPE_ATOMIC,		//ԭ�ӵĲ���(��)ϸ�ֵĶ���
};
class XActionSimpleEx
{
private:
	//static const int m_maxActionNameLength = 256;
	XResPos m_resoursePosition;	//��Դ���ڵ�λ��
	XBool m_isInited;	//�Ƿ��ʼ��
	XActionSimpleType m_type;
	XActionSimpleState m_actionState;	//������״̬
	XBool m_actionIsLoop;			//�����Ƿ�ѭ��
	//char m_actionName[m_maxActionNameLength];				//���������ƣ����ڰ�������
	std::string m_actionName;

	void (*m_funResetData)(void *);			//������������
	void (*m_funMove)(float stepTime,void *);	//����
	void (*m_funDraw)(void *);				//���
	void (*m_funAtEnd)(void *);				//��������֮�����
	XBool (*m_funIsEnd)(void *);				//�ж��Ƿ�������
	//������Դ����
	int (*m_funInit)(int,void*);	//��Դ��ʼ��
	void (*m_funRelease)(void*);	//��Դ�ͷź���

	void * m_pClass;
public:
	virtual XBool isName(const char *name);
	void setStop();	//��ֹ��������δʵ��
	virtual void setLoop(XBool isLoop) {m_actionIsLoop = isLoop;}	//���ö����Ƿ�ѭ��
	virtual XBool getIsLoop() const {return m_actionIsLoop;}		//��ȡ�����Ƿ�ѭ��
	virtual void setStart();			//���ö�����ʼ����
	virtual void setPause();			//���ö�����ͣ
	virtual void setResume();			//���ö����ָ�
	virtual void move(float stepTime = 1.0f);
	XActionSimpleState getActionState() const {return m_actionState;}
	XBool getIsEnd() const {return m_actionState == ACTION_STATE_END;}

	XActionSimpleEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_funResetData(NULL)		//������������
		,m_funMove(NULL)		//����
		,m_funDraw(NULL)		//���
		,m_funAtEnd(NULL)		//��������֮�����
		,m_funInit(NULL)		//��ʼ����Դ����
		,m_funRelease(NULL)		//��Դ�ͷ�
		,m_pClass(NULL)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
		,m_actionName("")
	{}
	XBool init(XResPos resPos,
		const char *actionName,
		XBool (*funIsEnd)(void *),
		void (*funMove)(float,void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *) = NULL,	
		int (*funInit)(int,void *) = NULL,
		void (*funRelease)(void *) = NULL,
		void * pClass = NULL);	
	void draw();

	~XActionSimpleEx(){release();}
	void release();
//�����Ǹ��϶����Ķ���
private:
	XActionSimpleEx **m_pAction;	//�Ӷ���
	int m_actionSum;		//�Ӷ�����������
	int m_curActionIndex;
public:
	//���洫��Ĳ����Ǹ��ϵ��Ӷ�����ָ��
	XBool initComplex(XResPos resPos,const char *actionName,int actionSum,...);	//���϶����ĳ�ʼ��
};

//�����������
class XActionSimpleManager		//����ಢ��������Դ�ͷţ���Ҫ�Լ����ⲿ�ͷ���Դ
{
public:
	XActionSimpleManager()
		:m_curActionHandle(0)
	{}
	virtual ~XActionSimpleManager() {m_pActions.clear();} 
protected:
	XActionSimpleManager(const XActionSimpleManager&);
	XActionSimpleManager &operator= (const XActionSimpleManager&);
private:
	int m_curActionHandle;		//��ǰ�Ķ������
	std::vector<XActionSimpleEx*> m_pActions;
	int getActionIndex(const char *actionName);	//ͨ�����ֻ�ȡ�������
	int getActionIndex(const XActionSimpleEx *action);	//ͨ�����ֻ�ȡ�������
public:
	bool pushAction(XActionSimpleEx *action);	//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����

	XBool popAction(const char *actionName);				//�ӱ���ɾ��һ������
	XBool popAction(XActionSimpleEx *action);
	XBool releaseAction(const char *actionName);	//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
	XBool releaseAction(XActionSimpleEx *action);
	XBool setStartAction(const char *actionName);			//���ÿ�ʼ����һ������
	XActionSimpleEx *getPAction(const char *actionName);

	XBool getIsActionEnd(const char *actionName);		//��ȡ��������Ƿ񲥷����

	void move(float stepTime);		//�������ƽ����еĶ���
	void draw();					//������������еĶ���

	//����Ľӿ���δʵ��
	void setAllActionStop();	//ֹͣ���еĶ���
	void setAllActionPause();	//��ͣ���еĶ���
	void setAllActionResume();	//�ָ����еĶ���
	void popAllAction();		//�������еĶ���
};
#if WITH_INLINE_FILE
#include "XActionSimple.inl"
#endif
}
#endif