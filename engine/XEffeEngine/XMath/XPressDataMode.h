#ifndef _JIA_PRESSDATAMODE_
#define _JIA_PRESSDATAMODE_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.10
//--------------------------------
//����һ��ѹ������ģ�ͣ����ǵ�ѹ��ʱ���ݿ�ʼ�仯�����ͷ�ʱ���ݿ�ʼ�ָ�
//���Է�Ϊ����ѹ��ģ�ͺ�˫��ѹ��ģ��
#include "../XCommonDefine.h"
#include "XInputEventCore.h"
namespace XE{
enum XPressState
{
//	PRESS_STATE_LEFT,	//����ѹ��	
//	PRESS_STATE_RIGHT,	//����ѹ��
	PRESS_STATE_PRESS,	//���ڵ���ѹ��ģ��
	PRESS_STATE_NORMAL,	//�ͷ�
};
class XPressDataSingle	//����ѹ��ģ��
{
private:
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_curDeformData;		//��ǰ�ı�������
	float m_curData;			//��ǰ������
	XPressState m_pressState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	//XBool m_canRecover;		//�Ƿ��Զ��ָ�
	int m_sensitivity;			//������	
	int m_curPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	//XBool m_isRecover;		//�����Ƿ���ȫ�ָ�
	float m_dataFlag;		//���ݱ任�ķ���

	XKeyValue m_pressKey;
public:
	XPressDataSingle()
		:m_startChangeData(1.0f)
		,m_minChangeData(0.01f)
		,m_maxChangeData(0.1f)
		,m_pressKey(XKEY_LEFT)
	{}
	//normalValue:��ͨ״̬�µ�ֵ
	//maxDeformValue:������״̬�µ�ֵ
	//sensitivity:������(��λms),����ʱ��֮��ʼ����
	void init(float normalValue,float maxDeformValue,int sensitivity);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;	//��һ�ΰ���ʱ��Ծ��ֵ
		m_minChangeData = minChange;	//�仯�������ٶ�
		m_maxChangeData = maxChange;	//�仯������ٶ�
	}
	void setPress();	//���ô��ڰ�ѹ״̬
	void setRelease(){m_pressState = PRESS_STATE_NORMAL;}	//���ô����ͷ�״̬
	void move(float stepTime);
	float getCurData() const {return m_curData;}
	XBool isPress() const {return m_pressState != PRESS_STATE_NORMAL;}
	//�������������δ������Ȳ���
	void setCurData(float data)
	{//��Χ���
		if(m_normalData > m_maxDeformData)
		{
			if(data < m_maxDeformData) data = m_maxDeformData; 
			if(data > m_normalData) data = m_normalData; 
		}else
		{
			if(data > m_maxDeformData) data = m_maxDeformData; 
			if(data < m_normalData) data = m_normalData; 
		}
		m_curData = data;
	}
	//����ģʽ2����������ģʽ
	void setKeyValue(XKeyValue pressKey){m_pressKey = pressKey;}
	void updateState(const XInputEvent &e)
	{
		if(e.type != EVENT_KEYBOARD) return;
		if(e.keyState == KEY_STATE_DOWN)
		{
			if(e.keyValue == m_pressKey) setPress();
		}else
		if(e.keyState == KEY_STATE_UP)
		{
			if(e.keyValue == m_pressKey) setRelease();
		}
	}
};
//ѹ�����ݵĻظ�ģʽ
enum XPressDataRecoverMode
{
	MODE_NO_RECOVER,	//����ظ��α�
	MODE_SLOW_RECOVER,	//�����ظ�
	MODE_IMM_RECOVER,	//�����ظ�
};
class XPressDataDouble	//˫��ѹ��ģ��
{
private:
	float m_minDeformData;		//��С��������
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_curDeformData;		//��ǰ�ı�������
	float m_curData;			//��ǰ������
	XPressState m_pressMinState;	//ѹ��״̬
	XPressState m_pressMaxState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	XPressDataRecoverMode m_recoverMode;		//�Ƿ��Զ��ָ�0:���ظ���1:�����ָ���2�����ָ�
	int m_sensitivity;			//������	
	int m_curPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	XBool m_isRecover;		//�����Ƿ���ȫ�ָ�

	XKeyValue m_minKey;
	XKeyValue m_maxKey;
public:
	//normalValue:��ͨ״̬��ֵ
	//maxDeformValue:���ε����ֵ
	//minDeformValue:���ε���Сֵ
	//sensitivity;����ϵ������λ ms
	void init(float normalValue,float maxDeformValue,float minDeformValue,
		int sensitivity,XPressDataRecoverMode recoverMode = MODE_SLOW_RECOVER);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void move(float stepTime);
	float getCurData() const {return m_curData;}
	XBool isPress() const {return (m_pressMinState != PRESS_STATE_NORMAL || 
		m_pressMaxState != PRESS_STATE_NORMAL);}
	//�������������δ������Ȳ���
	void setCurData(float data)
	{
		if(data < m_minDeformData) data = m_minDeformData; 
		if(data > m_maxDeformData) data = m_maxDeformData; 
		m_curData = data;
		m_isRecover = XFalse;
	}
	XPressDataDouble()
		:m_startChangeData(1.0f)
		,m_minChangeData(0.01f)	//�����ֵ��Ҫ�ɱ�
		,m_maxChangeData(0.1f)		//�����ֵ��Ҫ�ɱ�
		,m_minKey(XKEY_LEFT)
		,m_maxKey(XKEY_RIGHT)
	{}
	//����ģʽ1���ֶ�ģʽ
	void setPressMax();
	void setPressMin();
	void setReleaseMin(){m_pressMinState = PRESS_STATE_NORMAL;}
	void setReleaseMax(){m_pressMaxState = PRESS_STATE_NORMAL;}
	//����ģʽ2����������ģʽ
	void setKeyValue(XKeyValue min,XKeyValue max)
	{
		m_minKey = min;
		m_maxKey = max;
	}
	void updateState(const XInputEvent &e)
	{
		if(e.type != EVENT_KEYBOARD) return;
		if(e.keyState == KEY_STATE_DOWN)
		{
			if(e.keyValue == m_minKey) setPressMin(); else
			if(e.keyValue == m_maxKey) setPressMax();
		}else
		if(e.keyState == KEY_STATE_UP)
		{
			if(e.keyValue == m_minKey) setReleaseMin(); else
			if(e.keyValue == m_maxKey) setReleaseMax();
		}
	}
};
class XKeyPressModel
{
private:
	bool m_isInited;
	bool m_isPress;			//�Ƿ���
	bool m_isKeyDown;
	int m_sensitivity;			//������,��ʼ�ۻ���ʱ��,��λms
	int m_pressSpeed;			//������Ч��ʱ����λms
	XKeyValue m_keyValue;		//��Ӧ�İ���ֵ
	int m_timer;
public:
	XKeyPressModel()
		:m_isInited(false)
		, m_sensitivity(500)
		, m_pressSpeed(100)
		, m_keyValue(XKEY_LEFT)
	{}
	void resetState()
	{//ǿ�ư�������
		m_isPress = false;
		m_isKeyDown = false; 
	}
	bool init(XKeyValue key,int sensitivity = 500,int pressSpeed = 100);
	bool move(float stepTime);	//�����Ƿ񴥷������¼�
	//���������������Ҫ�ֶ�����
	void updateState(const XInputEvent& e);
};
}
#endif