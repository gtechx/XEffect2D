#ifndef _JIA_XACTIONEX_
#define _JIA_XACTIONEX_

#include "XActionSimple.h"
#include "XInputEventCore.h"
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.5.20
//--------------------------------
//����Ƕ�XActionSimple�������һ���Ĺ���
//������ͨ���̳���ʵ�ֵĶ�������
class _XActionEx
{
protected:
	_XResourcePosition m_resoursePosition;	//��Դ���ڵ�λ��
	_XBool m_isInited;						//�Ƿ��ʼ��
	_XActionSimpleType m_type;
	_XActionSimpleState m_actionState;		//������״̬
	_XBool m_actionIsLoop;					//�����Ƿ�ѭ��
	std::string m_actionName;				//���������ƣ����ڰ�������
public:
	_XBool isName(const char *name);
	void setStop();	//��ֹ��������δʵ��
	void setLoop(_XBool isLoop) {m_actionIsLoop = isLoop;}	//���ö����Ƿ�ѭ��
	_XBool getIsLoop() {return m_actionIsLoop;}		//��ȡ�����Ƿ�ѭ��
	void setStart();		//���ö�����ʼ����
	void setPause();		//���ö�����ͣ
	void setResume();		//���ö����ָ�
	void move(int stepTime = 1);
	_XActionSimpleState getActionState(){return m_actionState;}
	_XBool getIsEnd() {return m_actionState == ACTION_STATE_END;}
	std::string getName() {return m_actionName;}

	_XActionEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
		,m_actionName(" ")
	{}
	_XBool init(_XResourcePosition resoursePosition,
		const char *actionName);	
	void draw();
	void keepDraw();	//�����Ƿ�������ᱣ�ֻ��Ƶĺ���
	void input(const _XInputEvent &inputEvent);

	~_XActionEx(){release();}
	void release();
//�����Ǹ��϶����Ķ���
private:
	_XActionEx **m_pAction;	//�Ӷ���
	int m_actionSum;		//�Ӷ�����������
	int m_nowActionIndex;
public:
	_XBool init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//���϶����ĳ�ʼ��
public://��������Ҫ��ͬ�Ķ���ʵ�ֵĶ�������
	virtual void funResetData(){}				//������������
	virtual void funMove(int stepTime){}		//����
	virtual void funDraw(){}					//���
	virtual void funKeepDraw(){}				//������棬��Щͼ��ʵ�ڶ������֮��Ҳ��Ҫ���Ƶģ���ô������������л��ơ�
	virtual void funAtEnd(){}					//��������֮�����
	virtual _XBool funIsEnd(){return XTrue;}	//�ж��Ƿ�������
	//������Դ����
	virtual _XBool funInit(_XResourcePosition res){return XTrue;}	//��Դ��ʼ��
	virtual void funRelease(){}	//��Դ�ͷź���
	//������δ�����������
	virtual void funInput(const _XInputEvent &inputEvent){}
	//void * m_pClass;
};

#include <vector>
//�����������
typedef int _XActionExHandle;		//�����ľ��
struct _XActionExData
{
	_XActionExHandle actionHandle;	//�����ı��
	_XBool actionIsEnable;			//�����Ƿ���Ч
	_XActionEx *pAction;			//������ָ��

	_XActionExData()
		:actionIsEnable(XFalse)
		,pAction(NULL)
	{}
};
class _XActionExManager	//����ಢ��������Դ�ͷţ���Ҫ�Լ����ⲿ�ͷ���Դ
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XActionExManager()
		:m_nowActionHandle(0)
	{}
	_XActionExManager(const _XActionExManager&);
	_XActionExManager &operator= (const _XActionExManager&);
	virtual ~_XActionExManager() {;} 
public:
	static _XActionExManager& GetInstance()
	{
		static _XActionExManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_nowActionHandle;						//��ǰ�Ķ������
	std::vector<_XActionExData> m_pActions;		//���ж����ı�
	std::vector<int> m_actionOrder;		//�������������Ĳ����ϵ��������һ��ӳ��(��δ��Ч)
	std::vector<int> m_actionOrderX;		//�������������Ĳ����ϵ��������һ��ӳ��(��δ��Ч)
public:
	_XActionExHandle pushAction(_XActionEx *action);			//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����

	_XActionExHandle getActionHandle(const char *actionName);	//ͨ�����ֻ�ȡ�������
	_XActionExHandle getActionHandle(const _XActionEx *action);	//ͨ�����ֻ�ȡ�������
	_XBool popAction(const char *actionName);					//�ӱ���ɾ��һ������
	_XBool popAction(_XActionEx *action);
	_XBool popAction(_XActionExHandle actionHandle);	
	_XBool releaseAction(const char *actionName);			//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
	_XBool releaseAction(_XActionEx *action);
	_XBool releaseAction(_XActionExHandle actionHandle);
	_XBool setStartAction(const char *actionName);			//���ÿ�ʼ����һ������
	_XBool setStartAction(_XActionExHandle actionHandle);	//���ÿ�ʼ����һ������
	_XActionEx *getPAction(const char *actionName);
	_XActionEx *getPAction(_XActionExHandle actionHandle);

	_XBool getIsActionEnd(const char *actionName);			//��ȡ��������Ƿ񲥷����
	_XBool getIsActionEnd(_XActionExHandle actionHandle);	//��ȡ��������Ƿ񲥷����

	void move(int stepTime);		//�������ƽ����еĶ���
	void draw();					//������������еĶ���
	//������δ�����������
	void input(const _XInputEvent &inputEvent);

	//����Ľӿ�(��δʵ��)
	void setAllActionStop();	//ֹͣ���еĶ���
	void setAllActionPause();	//��ͣ���еĶ���
	void setAllActionResume();	//�ָ����еĶ���
	void popAllAction();		//�������еĶ���
	//�����ǲ����ϵ�Ľӿ�(��δʵ��)
	//˵��������ᱻ�����ڵ�
	_XBool moveToBottom(const char *actionName);			//�ƶ���������
	_XBool moveToBottom(_XActionExHandle actionHandle);
	_XBool moveToTop(const char *actionName);				//�ƶ���������
	_XBool moveToTop(_XActionExHandle actionHandle);
	_XBool moveUp(const char *actionName);					//�����ƶ�һ��,����һ����н���
	_XBool moveUp(_XActionExHandle actionHandle);
	_XBool moveDown(const char *actionName);					//�����ƶ�һ��
	_XBool moveDown(_XActionExHandle actionHandle);
	_XBool moveUpTo(const char *s,const char *d);			//s�ƶ���d����
	_XBool moveUpTo(_XActionExHandle s,_XActionExHandle d);
	_XBool moveDownTo(const char *s,const char *d);			//s�ƶ���d����
	_XBool moveDownTo(_XActionExHandle s,_XActionExHandle d);
	int getLayerIndex(const char *actionName);			//��ȡ��ǰ������һ��
	int getLayerIndex(_XActionExHandle actionHandle);
};
#define _XAExManager _XActionExManager::GetInstance()

#include "XActionEx.inl"

#endif