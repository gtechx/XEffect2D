#ifndef _JIA_XACTIONDESCRIPTION_
#define _JIA_XACTIONDESCRIPTION_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "../XObjectBasic.h"
#include "../XFont/XNumberEx.h"
#include "../XMath/XMoveData.h"
//#include "../XBasicFun.h"
#include <vector>
#include "XActionObjectManager.h"
namespace XE{
#define ACTION_DES_PATH "ResourcePack/ActionDescription/"
enum XActionType
{//����������
	ACTION_TYPE_POSITION,	//λ�õı仯
	ACTION_TYPE_POSITIONX,	//λ�õı仯
	ACTION_TYPE_POSITION_Y,	//λ�õı仯
	ACTION_TYPE_COLOR,		//��ɫ�ı仯
	ACTION_TYPE_R,
	ACTION_TYPE_G,
	ACTION_TYPE_B,
	ACTION_TYPE_ALPHA,		//͸���ȵı仯
	ACTION_TYPE_ANGLE,		//�Ƕȱ仯
	ACTION_TYPE_SIZE,		//�ߴ�ı仯
	ACTION_TYPE_SIZE_EX,	//�ߴ�ı仯	//x��y�ȱ�������
	ACTION_TYPE_SIZEX,		//�ߴ�ı仯	
	ACTION_TYPE_SIZE_Y,		//�ߴ�ı仯
	ACTION_TYPE_STAY,		//ͣ��״̬
	ACTION_TYPE_MAX,
};
extern std::string actionTypeStr[];
struct XActionState
{//������ֵ�״̬
	XBool isEnable;
	XVector2 position;
	XFColor color;
	float angle;	//[0-360]
	XVector2 size;
	XActionState()
		:angle(0.0f)
		,isEnable(XFalse)
	{}
};
//״̬����
enum XActionStateType
{
	ACTION_STATE_TYPE_POSX,
	ACTION_STATE_TYPE_POS_Y,
	ACTION_STATE_TYPE_COLOR_R,
	ACTION_STATE_TYPE_COLOR_G,
	ACTION_STATE_TYPE_COLOR_B,
	ACTION_STATE_TYPE_COLOR_A,
	ACTION_STATE_TYPE_ANGLE,
	ACTION_STATE_TYPE_SIZEX,
	ACTION_STATE_TYPE_SIZE_Y,
	ACTION_STATE_TYPE_MAX,
};
struct XActionStateDescription
{
	int time;	//״̬������ʱ��㣬���ֻ��AD_TYPE_STATE_LINEʱ��Ч
	XBool isValidPosX;
	XBool isValidPosY;
	XBool isValidColorR;
	XBool isValidColorG;
	XBool isValidColorB;
	XBool isValidColorA;
	XBool isValidAngle;
	XBool isValidSizeX;
	XBool isValidSizeY;

	XVector2 position;
	XFColor color;
	float angle;	//[0-360]
	XVector2 size;
	XActionStateDescription()
		:angle(0.0f)
	{}
};
typedef XActionStateDescription XActionStateDes;
//����һ���Զ�����������
class XActionCore
{
public:
	XBool m_isEnable;
	XActionType m_actionType;
	//std::vector<XMoveData *> m_moveData;
	XMoveData *m_moveData;
	int m_moveDataSum;
	XObjectBasic* m_object;	//�����Ķ���
	float m_startTime;				//������ʼʱ��,����������ʱ��
	float m_endTime;
	float m_timeLine;				//ʱ����
	XActionState m_stayState;		//��Ҫ���ֵ�״̬
	//XBool m_isStart;			//�Ƿ�ʼ
	XBool m_isActionStart;	//������ʼ�����ݿ�ʼ�仯
	XBool m_isEnd;
	XBool set(float startTime,float endTime,XActionType type,XMoveData *md = NULL,XActionState *state = NULL);
	void move(float delay);
	XBool getIsEnd() const {return m_isEnd;}
	void setEnd(){m_isEnd = XTrue;}		//���ý���
	//XBool getIsLoop();
	void setStart();	//���ÿ�ʼ
	XActionCore()
		:m_isEnable(XFalse)
		,m_isEnd(XTrue)
		,m_startTime(0.0f)
		,m_timeLine(0.0f)
		//,m_isStart(XFalse)
		,m_moveData(NULL)
		,m_moveDataSum(0)
	{}
	~XActionCore(){release();}
	void release();
};
//AD�жԶ�������������
//enum XActionDesType
//{
//	AD_TYPE_AC,				//ͨ��ActionCore�ķ�ʽ��������
//	AD_TYPE_STATE_TABLE,	//ͨ��״̬��ķ�ʽ��������
//};
class XActionMananger;
class XActionDescription
{
	friend XActionMananger;
private:
	int m_ID;
	XBool m_isEnable;
	std::vector<XActionDescription *> m_otherAction;	//��������ɲ���
	std::vector<int> m_otherActionID;	//������ڸ�����ʼ��
	XObjectBasic* m_object;			//�����Ķ���
	std::vector<XActionCore *> m_actionCore;	//����Ԫ
	char m_actionName[MAX_FILE_NAME_LENGTH];			//����������
	float m_startTime;				//������ʼʱ��
	float m_endTime;				//��������ʱ��(ò��û�б�Ҫ),����û����
	float m_timeLine;
	XBool m_isEnd;
	//XBool m_isStart;	//�Ƿ�ʼ
	XBool m_isActionStart;	//������ʼ�����ݿ�ʼ�仯
	//XActionDesType m_actionDesType;
	//+++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�AD_TYPE_STATE_LINEģʽ����������δʵ�֣�
	//˵�����ģʽ�£�AD�д��state���б�ʱ��֡��⵱ǰ����������state֮�䣬
	//Ȼ��ͨ�����Բ�ֵ(����չΪ��������ֵ)�ķ�ʽ���㵱ǰ��״̬
private:
	std::vector<XActionStateDes *> m_stateTable;	//״̬�б�,��ʱ������
	int m_minTime;
	int m_maxTime;
	float getASValue(int time,XActionStateType type);
public:
	int getMinTime() {return m_minTime;}	//�����С��ʱ���
	int getMaxTime() {return m_maxTime;}	//�������ʱ���
	int getActionStateDesSum() {return m_stateTable.size();}
	XActionStateDes* getPreviousASD(int time,XActionStateType type);	//��õ�ǰʱ������һ��״̬����
	XActionStateDes* getNextASD(int time,XActionStateType type);		//��õ�ǰʱ������һ��״̬����
	XActionStateDes* getASD(int index)
	{
		if(index < 0 || index >= m_stateTable.size()) return NULL;
		return m_stateTable[index];
	}
	XActionState getActionState(int time);		//��ȡָ��ʱ���״̬(ע������㷨��Ч���д��Ż�)
	XBool insertASD(XActionStateDes* ASD);		//����һ��״̬����(��ʱ������)
	XBool popASD(const XActionStateDes* ASD);	//����һ��״̬����,���ͷ���Դ
	//XBool checkStateTable();						//���״̬�б�ĺ�����,ȱ���������յ������������յ�Ĵ���
	//---------------------------------------------
public:
	float getEndTime() const {return m_endTime;}
	float getStartTime() const {return m_startTime;}
	float getCurTime() const {return m_timeLine;}
	XObjectBasic * getObject() {return m_object;}
	int getOtherActionSum(){return m_otherAction.size();}
	XActionDescription *getOtherAD(int index)
	{
		if(index < 0 || index >= m_otherAction.size()) return NULL;
		return m_otherAction[index];
	}
	int getActionCoreSum(){return m_actionCore.size();}
	XActionCore * getActionCore(int index)
	{
		if(!m_isEnable) return NULL;
		if(index < 0 || index >= m_actionCore.size()) return NULL;
		return m_actionCore[index];
	}

public:
	int getID() const{return m_ID;}
	const char * getName() const{return m_actionName;}
	XBool setName(const char * name)
	{//(��δʵ��)
		if(name == NULL) return XFalse;
		if(strlen(name) >= MAX_FILE_NAME_LENGTH) return XFalse;
		if(strcmp(name,m_actionName) == 0) return XTrue;	//������ͬ�����ظ�����
		//if(XActionMananger::GetInstance().getActionDes(name) != NULL) return XFalse; //����
		//������Ҫ�������(��δʵ��)
		strcpy(m_actionName,name);
		return XTrue;
	}
	void move(float delay);
	void draw();
	void setStart();
	XBool getIsEnd() const {return m_isEnd;}	//�Ƿ����
	void setLoop();							//��δ���
	XBool set(float startTime,float endTime,XObjectBasic *obj)
	{
		if(m_isEnable) return XFalse;
		m_startTime = startTime;
		m_endTime = endTime;
		m_object = obj;
		m_otherAction.clear();
		m_actionCore.clear();

		m_isEnable = XTrue;
		return XTrue;
	}
	int getADChildIndex(XActionDescription * p);	//����ƶ���AD�ڵ�ǰAD���б��еı��
	void setObject(XObjectBasic *obj);
	XBool loadAction(const char * filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//���涯�����ļ�,��׺".acd"
	XBool saveAction(const char *filename = NULL);	//���ļ���ȡ����,��׺".acd"
	XBool pushAActionCore(XActionCore * p);
	XBool popAActionCore(XActionCore * p);	//��������ɾ��һ��Ԫ��
	XBool replaceAAction(int index,XActionCore * p);
	XBool popAActionCore(int index);
	XBool getIsChildActionDes(XActionDescription * p);
	XBool pushAActionDescription(XActionDescription * p);
	XBool popAActionDescription(int index);
	XActionDescription();
	~XActionDescription(){release();}
	void release();
private:
	bool loadFromFolder(const char *filename);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename);		//����ҳ�ж�ȡ��Դ
};
}
#endif