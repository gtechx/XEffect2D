#ifndef _JIA_XACTIONEX_
#define _JIA_XACTIONEX_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.5.20
//--------------------------------
#include "XActionSimple.h"
#include "XInputEventCore.h"
#include <vector>
namespace XE{
//����Ƕ�XActionSimple�������һ���Ĺ���
//������ͨ���̳���ʵ�ֵĶ�������
class XActionEx
{
protected:
	XResourcePosition m_resoursePosition;	//��Դ���ڵ�λ��
	XBool m_isInited;						//�Ƿ��ʼ��
	XActionSimpleType m_type;
	XActionSimpleState m_actionState;		//������״̬
	XBool m_actionIsLoop;					//�����Ƿ�ѭ��
	std::string m_actionName;				//���������ƣ����ڰ�������
public:
	XBool isName(const char *name);
	void setStop();	//��ֹ��������δʵ��
	void setLoop(XBool isLoop) {m_actionIsLoop = isLoop;}	//���ö����Ƿ�ѭ��
	XBool getIsLoop() const {return m_actionIsLoop;}		//��ȡ�����Ƿ�ѭ��
	void setStart();		//���ö�����ʼ����
	void setPause();		//���ö�����ͣ
	void setResume();		//���ö����ָ�
	void move(float stepTime = 1.0f);
	XActionSimpleState getActionState()const{return m_actionState;}
	XBool getIsEnd() const {return m_actionState == ACTION_STATE_END;}
	std::string getName() const {return m_actionName;}

	XActionEx()
		:m_isInited(XFalse)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
		,m_actionName(" ")
	{}
	XBool init(XResourcePosition resoursePosition,
		const char *actionName);	
	void draw();
	virtual void keepDraw();	//�����Ƿ�������ᱣ�ֻ��Ƶĺ���
	void input(const XInputEvent &inputEvent);

	~XActionEx(){release();}
	void release();
//�����Ǹ��϶����Ķ���
private:
	XActionEx **m_pAction;	//�Ӷ���
	int m_actionSum;		//�Ӷ�����������
	int m_curActionIndex;
public:
	XBool init(XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//���϶����ĳ�ʼ��
public://��������Ҫ��ͬ�Ķ���ʵ�ֵĶ�������
	virtual void funResetData(){}				//������������
	virtual void funMove(float stepTime){}		//����
	virtual void funDraw(){}					//���
	virtual void funKeepDraw(){}				//������棬��Щͼ��ʵ�ڶ������֮��Ҳ��Ҫ���Ƶģ���ô������������л��ơ�
	virtual void funAtEnd(){}					//��������֮�����
	virtual XBool funIsEnd(){return XTrue;}	//�ж��Ƿ�������
	//������Դ����
	virtual XBool funInit(XResourcePosition res){return XTrue;}	//��Դ��ʼ��
	virtual void funRelease(){}	//��Դ�ͷź���
	//������δ�����������
	virtual void funInput(const XInputEvent &inputEvent){}
	//void * m_pClass;
};

//�����������
typedef int XActionExHandle;		//�����ľ��
struct XActionExData
{
	XActionExHandle actionHandle;	//�����ı��
	XBool actionIsEnable;			//�����Ƿ���Ч
	XActionEx *pAction;			//������ָ��

	XActionExData()
		:actionIsEnable(XFalse)
		,pAction(NULL)
	{}
};
class XActionExManager	//����ಢ��������Դ�ͷţ���Ҫ�Լ����ⲿ�ͷ���Դ
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	XActionExManager()
		:m_curActionHandle(0)
	{}
	XActionExManager(const XActionExManager&);
	XActionExManager &operator= (const XActionExManager&);
	virtual ~XActionExManager() {;} 
public:
	static XActionExManager& GetInstance()
	{
		static XActionExManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_curActionHandle;						//��ǰ�Ķ������
	std::vector<XActionExData> m_pActions;		//���ж����ı�
	std::vector<int> m_actionOrder;		//�������������Ĳ����ϵ��������һ��ӳ��(��δ��Ч)
	std::vector<int> m_actionOrderX;		//�������������Ĳ����ϵ��������һ��ӳ��(��δ��Ч)
public:
	XActionExHandle pushAction(XActionEx *action);			//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����

	XActionExHandle getActionHandle(const char *actionName);	//ͨ�����ֻ�ȡ�������
	XActionExHandle getActionHandle(const XActionEx *action);	//ͨ�����ֻ�ȡ�������
	XBool popAction(const char *actionName);					//�ӱ���ɾ��һ������
	XBool popAction(XActionEx *action);
	XBool popAction(XActionExHandle actionHandle);	
	XBool releaseAction(const char *actionName);			//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
	XBool releaseAction(XActionEx *action);
	XBool releaseAction(XActionExHandle actionHandle);
	XBool setStartAction(const char *actionName);			//���ÿ�ʼ����һ������
	XBool setStartAction(XActionExHandle actionHandle);	//���ÿ�ʼ����һ������
	XActionEx *getPAction(const char *actionName);
	XActionEx *getPAction(XActionExHandle actionHandle);

	XBool getIsActionEnd(const char *actionName);			//��ȡ��������Ƿ񲥷����
	XBool getIsActionEnd(XActionExHandle actionHandle);	//��ȡ��������Ƿ񲥷����

	void move(float stepTime);		//�������ƽ����еĶ���
	void draw();					//������������еĶ���
	//������δ�����������
	void input(const XInputEvent &inputEvent);

	//����Ľӿ�(��δʵ��)
	void setAllActionStop();	//ֹͣ���еĶ���
	void setAllActionPause();	//��ͣ���еĶ���
	void setAllActionResume();	//�ָ����еĶ���
	void popAllAction();		//�������еĶ���
	//�����ǲ����ϵ�Ľӿ�(��δʵ��)
	//˵��������ᱻ�����ڵ�
	XBool moveToBottom(const char *actionName);			//�ƶ���������
	XBool moveToBottom(XActionExHandle actionHandle);
	XBool moveToTop(const char *actionName);				//�ƶ���������
	XBool moveToTop(XActionExHandle actionHandle);
	XBool moveUp(const char *actionName);					//�����ƶ�һ��,����һ����н���
	XBool moveUp(XActionExHandle actionHandle);
	XBool moveDown(const char *actionName);					//�����ƶ�һ��
	XBool moveDown(XActionExHandle actionHandle);
	XBool moveUpTo(const char *s,const char *d);			//s�ƶ���d����
	XBool moveUpTo(XActionExHandle s,XActionExHandle d);
	XBool moveDownTo(const char *s,const char *d);			//s�ƶ���d����
	XBool moveDownTo(XActionExHandle s,XActionExHandle d);
	int getLayerIndex(const char *actionName);			//��ȡ��ǰ������һ��
	int getLayerIndex(XActionExHandle actionHandle);
};
#define XAExManager XActionExManager::GetInstance()
#if WITH_INLINE_FILE
#include "XActionEx.inl"
#endif
}
#endif