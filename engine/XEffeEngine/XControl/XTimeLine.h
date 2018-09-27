#ifndef _JIA_XTIMELINE_
#define _JIA_XTIMELINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.4.2
//--------------------------------
#include "../XXml.h"
#include "../XFont/XNumberEx.h"
#include "../XMath/XVector2.h"
#include "../XMath/XMoveData.h"
#include "../XFont/XFontUnicode.h"
#include "XText.h"
#include "XEdit.h"
#include "XSlider.h"
namespace XE{
#define TIMELINE_HEIGHT (32.0f)	//ʱ���ߵĸ߶�
#define TIMELINE_SLD_WIDTH (32.0f)	//�������Ŀ��
#define TIMELINE_TITLE_LEN (128.0f)	//ʱ���ߵı��ⳤ��
#define TIMELINE_FRAME_WIDTH (10.0f)	//ʱ������һ֡�����ؿ��
#define DEFAULT_TIMELINE_NAME "timeLine.xml"

//���Կ��ǵ��Ż�
//1������϶��ؼ�֡
//2��ʱ�����в��ɱ༭������
//3������ʱ�������			*
//4��֡�Ĳ������������䣬ɾ������� 
//5��Ƭѡ֡��Ƭѡ��
//����
//1����ִ��֡�ʺܵ�ʱ������Խ����֡����δ���
enum XFrameType
{//֡������
	FRAME_TYPE_KEY,		//�ؼ�֡
	FRAME_TYPE_INTER,	//����֡
	FRAME_TYPE_BLANK,	//�հ�֡
	FRAME_TYPE_LAST,	//����֡
};
//ʱ������֧�ֵ�������Ҫ�������̳�
class XFrameDataBasic
{
protected:
	int type;
public:
	void * pData;
	XFrameDataBasic()
		:pData(NULL)
		,type(0)
	{}
	virtual ~XFrameDataBasic(){}

	virtual XBool writeToXml(TiXmlElement * node) = 0;
	virtual XBool readFromXml(TiXmlNode * node) = 0;
};
extern XFrameDataBasic * createFrameDataBasic(TiXmlNode * node);
struct XOneFrame
{//���֡ӵ�ж����ݵĴ���Ȩ�ޣ�֡��ɾ��ʱ����Ҳ�ᱻ�ͷŵ�
	int frameIndex;				//֡�����
	int time;					//���뵥λ��ʱ��δ֪
	int timeLen;				//֡�ĳ���ʱ��,��λ����
	XMoveDataMode interpolationMode;			//��ֵģʽ
	XFrameDataBasic *pData;					//��ǰ�ؼ�֡������
	bool overFlag;				//�Ƿ��Ѿ���ʱ���߿�Խ���ı�־
	XOneFrame()
		:time(0)
		,interpolationMode(MD_MODE_LINE)	//Ĭ��Ϊ���Բ�ֵ
		,pData(NULL)
		,overFlag(false)
		,timeLen(0)
	{}
};
class XOneTimeLine
{
public:
	XFontUnicode m_title;
	XVec2 m_position;
	XVec2 m_scale;
	int m_startIndex;	//��ʼ��ʱ��
	int m_width;		//��ʾ�Ŀ��
	float m_frameTimeWidth;	//ÿһ֡��ʱ���� ��λΪ����

	XBool m_isInited;
public:
	void setPosition(const XVec2& p);
	void setScale(const XVec2& s);
	void setCurStartIndex(int index){m_startIndex = index;}
	std::string m_name;	//ʱ���ߵ�����
	std::vector<XOneFrame> m_keyFrames;	//���е�֡������
	XFrameType getFrameType(int index);		//���㵱ǰʱ���֡����
	void setFps(int fps);
	//������Ҫ�ṩһ���������ڻ�ȡ��ǰ��״̬
	//float m_getCurFrameData(int time);	//���ǵ��������ͣ�������δ���Ǻ����ʵ��

	XBool init(const std::string& name,int width,const XVec2& position,const XVec2& scale);
	void draw();
	XOneTimeLine()
		:m_isInited(XFalse)
		,m_frameTimeWidth(33.33f)
		,m_width(256)
		,m_startIndex(0)
	{}
	void addAKeyFrame(int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);//����һ���ؼ�֡
	void delAKeyFrame(int time);	//ɾ��һ��ʱ���Ĺؼ�֡
	void moveKeyFrame(int fromTime,int toTime);	//��һ��ʱ����֡�ƶ���
	void clearAllFrame();	//������е�֡
	~XOneTimeLine(){clearAllFrame();}

	const XOneFrame *getCurKeyFrame(float time);	//��ǰ���ڵĹؼ�֡
	const XOneFrame *getNextKeyFrame(float time);	//��ȡ��һ���ؼ�֡��ָ��
	void resetAllFrameFlag();
	void setAllFrameFlag();
	void resetAllFrameFlag(int from,int to);	//����from��to֮��Ĺؼ�֡
	void setAllFrameFlag(int from,int to);
	void resetAllFrameFlagBefore(int time);		//����time֮ǰ������ʱ��֡
	void setAllFrameFlagBefore(int time);
	void resetAllFrameFlagAfter(int time);		//����time֮�������ʱ��֡
	void setAllFrameFlagAfter(int time);
};
class XTimeLines
{
private:
	int m_objectID;

	XVec2 m_position;
	XVec2 m_scale;
	float m_width;

	std::vector<XOneTimeLine *> m_timeLines;	//ʱ����

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǹ������Ķ���
	XText m_curTimeTxt;
	XText m_fpsTxt;
	XEdit m_fpsEdt;
	XText m_allTimeTxt;	//��ʱ��
	XEdit m_allTimeEdt;
	XButton m_playOrStopBtn;	//���ź�ֹͣ��ť
	XButton m_headBtn;		//�ص�ͷ�İ�ť
	XButton m_endBtn;		//�ص�β�İ�ť
	XSlider m_timeSld;		//ʱ��֡�Ļ�����
	XSlider m_timeLineSld;	//ʱ���ߵ����򻬶���
	int m_showTimeLineSum;	//������ʾ��ʱ���ߵ�����
	int m_curStartTimeLineIndex;	//��ǰ��ʾ��������һ��ʱ���ߵı��
	//-------------------------------------------------------

	int m_curStartIndex;	//��ǰʱ���߿�ʼ��֡���
	int m_canShowFrameSum;	//������ʾ��ʱ��֡����

	int m_fps;
	float m_frameTime;	//һ֡��ʱ���ȣ���λΪms
	int m_allTime;	//�ܵ�ʱ�䳤�ȣ���λms
	int m_curTimer;	//��ǰ��ʱ�䣬��λΪms
	int m_curChooseIndex;	//��ǰѡ���������ʱ����

	XBool m_isInited;
	XBool m_visible;	//�Ƿ�ɼ�
	bool m_isPlaying;
	//++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ�������ק�ؼ�֡������ı���
	bool m_isMouseDrag;		//�Ƿ������ק
	bool m_isMouseMove;		//����Ƿ��ƶ�
	int m_mouseDragTLIndex;	//�����ק��ʱ���߱��
	int m_mouseDragKFTime;	//ѡ��Ĺؼ�֡��ʱ��
	//--------------------------------------
	float m_canShowTime;	//������ʾ����ʱ��

	struct XRuleInfo
	{//��ߵ���Ϣ
		int ruleData;	//��ߵ�����
		XVec2 rulePosition;
		float offset;
	};
	XFontUnicode m_ruleFont;	//��ߵ�����
	std::vector<XRuleInfo> m_ruleInfos;	//��ߵ���Ϣ

//	bool m_needShowRuleHead;	//�Ƿ���Ҫ��ʾͷ���ı��
//	bool m_needShowRuleMiddle;	//�Ƿ���Ҫ��ʾͷ���ı��
//	bool m_needShowRuleEnd;	//�Ƿ���Ҫ��ʾͷ���ı��
//	XFontUnicode m_ruleHead;	//ͷ���ı��
//	float m_ruleHeadOffset;
//	XFontUnicode m_ruleMiddle;	//�в��ı��
//	float m_ruleMiddleOffset;
//	XFontUnicode m_ruleEnd;	//β���ı��
//	float m_ruleEndOffset;

	static void ctrlProc(void*,int,int);
public:
	int getCurChooseIndex() const {return m_curChooseIndex;}
	void play();	//��ʼ����
	void stop();	//ֹͣ����
	bool getIsPlaying() const {return m_isPlaying;}
	void setCurTimer(int curTime) {m_curTimer = curTime;}
private:
	void updateCtrlsPosition();
	void updateRule();	//���ݵ�ǰ��ʵ��������µ�ǰ��ߵ�״̬

	void (*m_funTimeChange)(void *,int ID);	//��ǰʱ�䷢���仯ʱ����
	void (*m_funPlay)(void *,int ID);	//��ǰʱ�䷢���仯ʱ����
	void (*m_funStop)(void *,int ID);	//��ǰʱ�䷢���仯ʱ����
	void *m_pClass;
public:
	void setTimeChangeCB(void (* funTimeChange)(void *,int),
		void (* funPlay)(void *,int),
		void (* funStop)(void *,int),
		void *pClass = NULL)
	{
		m_funTimeChange = funTimeChange;
		m_funPlay = funPlay;
		m_funStop = funStop;
		m_pClass = pClass;
	}
	void setPosition(const XVec2& p);
	void setSize(const XVec2& s);
	void setCurTimeSlider(float p);	//���õ�ǰ�İٷֱ�λ��
	void setCurTimeLineSlider(float p);	//���õ�ǰ�İٷֱ�λ��
	void setToPrev();	//��ǰ��һҳ
	void setToNext();	//�����һҳ
	//�����ǹ���ʱ���ߵĲ���
	XBool addATimeLine(const std::string& name);
	//��ָ����ʱ���������ӹؼ�֡
	XBool addKeyFrame(const std::string& name,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);
	//��ָ����ʱ���������ӹؼ�֡
	XBool addKeyFrame(unsigned int index,int time,int timeLen,XFrameDataBasic * data,XMoveDataMode mode = MD_MODE_NULL);
	XBool delCurKeyFrame(const std::string& name);	//ɾ����ǰʱ��Ĺؼ�֡�������ǰʱ��û�йؼ�֡��ɾ��
	XBool delCurKeyFrame(unsigned int index);	//ɾ����ǰʱ��Ĺؼ�֡�������ǰʱ��û�йؼ�֡��ɾ��
	XBool clearAllFroms(const std::string& name);
	XBool clearAllFroms(unsigned int index);
	void clearAllFroms();
	//�������������ĺ���
	XBool mouseProc(const XVec2& p,XMouseState mouseState);		//������궯������Ӧ����
	int getTimeLineIndexByName(const std::string& name);
	void setFps(int fps);
	int getFps() const {return m_fps;}
	void setAllTime(int time);
	int getAllTime() const {return m_allTime;}
	XBool init(float width,int showLineSum,int fps = 30);
	void draw();
	void move(float stepTime);
	void release();
	XTimeLines()
		:m_isInited(XFalse)
		,m_fps(30)
		,m_curStartIndex(0)
		,m_allTime(6000)
		,m_curTimer(0)
		,m_isPlaying(false)
		,m_curChooseIndex(-1)
		,m_showTimeLineSum(3)
		,m_curStartTimeLineIndex(0)
//		,m_needShowRuleHead(false)
//		,m_needShowRuleMiddle(false)
//		,m_needShowRuleEnd(false)
//		,m_ruleHeadOffset(0.0f)
//		,m_ruleMiddleOffset(0.0f)
//		,m_ruleEndOffset(0.0f)
		,m_funTimeChange(NULL)
		,m_funPlay(NULL)
		,m_funStop(NULL)
		,m_pClass(NULL)
		,m_objectID(100)
		,m_isMouseDrag(false)
		,m_visible(true)
	{}
	~XTimeLines(){release();}
	int getControlID() const{return m_objectID;}

	//������Ԥ�Ƶ�һЩ�ӿ�
	XBool saveToFile(const char * filename = NULL);
	XBool readFromFile(const char * filename = NULL);
	int getCurTimer() const {return m_curTimer;}	//��ȡ��ǰ��ʱ��

	const XOneFrame *getCurKeyFrame(const std::string& name);	//��ȡ��ǰ���ڵĹؼ�֡
	const XOneFrame *getNextKeyFrame(const std::string& name);	//��ȡ��һ���ؼ�֡��ָ��
	const XOneFrame *getCurKeyFrame(unsigned int index);		//��ȡ��ǰ���ڵĹؼ�֡
	const XOneFrame *getNextKeyFrame(unsigned int index);		//��ȡ��һ���ؼ�֡��ָ��
	void resetAllTimeLineFrameFlag();					//�������е�ʱ��֡
	void setAllTimeLineFrameFlag();						//�������е�ʱ��֡
	void resetAllTimeLineFrameFlag(int index);			//����ָ����ʱ��֡
	void resetAllTimeLineFrameFlag(const std::string& name);	//����ָ����ʱ��֡
	void setAllTimeLineFrameFlag(int index);			//����ָ����ʱ��֡
	void setAllTimeLineFrameFlag(const std::string& name);		//����ָ����ʱ��֡

	void setVisible()
	{
		m_visible = XTrue;
		m_curTimeTxt.setVisible();
		m_fpsTxt.setVisible();
		m_fpsEdt.setVisible();
		m_allTimeTxt.setVisible();	//��ʱ��
		m_allTimeEdt.setVisible();
		m_playOrStopBtn.setVisible();	//���ź�ֹͣ��ť
		m_headBtn.setVisible();		//�ص�ͷ�İ�ť
		m_endBtn.setVisible();		//�ص�β�İ�ť
		m_timeSld.setVisible();		//ʱ��֡�Ļ�����
		m_timeLineSld.setVisible();	//ʱ���ߵ����򻬶���
	}
	void disVisible()
	{
		m_visible = XFalse;
		m_curTimeTxt.disVisible();
		m_fpsTxt.disVisible();
		m_fpsEdt.disVisible();
		m_allTimeTxt.disVisible();	//��ʱ��
		m_allTimeEdt.disVisible();
		m_playOrStopBtn.disVisible();	//���ź�ֹͣ��ť
		m_headBtn.disVisible();		//�ص�ͷ�İ�ť
		m_endBtn.disVisible();		//�ص�β�İ�ť
		m_timeSld.disVisible();		//ʱ��֡�Ļ�����
		m_timeLineSld.disVisible();	//ʱ���ߵ����򻬶���
	}
	XBool getVisible() const {return m_visible;}
};
#if WITH_INLINE_FILE
#include "XTimeLine.inl"
#endif
}
#endif