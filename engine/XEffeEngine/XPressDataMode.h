#ifndef _JIA_PRESSDATAMODE_
#define _JIA_PRESSDATAMODE_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.10
//--------------------------------
//����һ��ѹ������ģ�ͣ����ǵ�ѹ��ʱ���ݿ�ʼ�仯�����ͷ�ʱ���ݿ�ʼ�ָ�
//���Է�Ϊ����ѹ��ģ�ͺ�˫��ѹ��ģ��
#include "XOSDefine.h"
#include "XInputEventCore.h"

enum _XPressState
{
//	PRESS_STATE_LEFT,	//����ѹ��	
//	PRESS_STATE_RIGHT,	//����ѹ��
	PRESS_STATE_PRESS,	//���ڵ���ѹ��ģ��
	PRESS_STATE_NORMAL,	//�ͷ�
};
class _XPressDataSingle	//����ѹ��ģ��
{
private:
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_nowDeformData;		//��ǰ�ı�������
	float m_nowData;			//��ǰ������
	_XPressState m_pressState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	//_XBool m_canRecover;		//�Ƿ��Զ��ָ�
	int m_sensitivity;			//������	
	int m_nowPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	//_XBool m_isRecover;		//�����Ƿ���ȫ�ָ�
	float m_dataFlag;		//���ݱ任�ķ���
public:
	_XPressDataSingle()
		:m_startChangeData(1.0f)
		,m_minChangeData(0.01f)
		,m_maxChangeData(0.1f)
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
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const {return m_pressState != PRESS_STATE_NORMAL;}
	//�������������δ������Ȳ���
	void setNowData(float data)
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
		m_nowData = data;
	}
};
//ѹ�����ݵĻظ�ģʽ
enum _XPressDataRecoverMode
{
	MODE_NO_RECOVER,	//����ظ��α�
	MODE_SLOW_RECOVER,	//�����ظ�
	MODE_IMM_RECOVER,	//�����ظ�
};
class _XPressDataDouble	//˫��ѹ��ģ��
{
private:
	float m_minDeformData;		//��С��������
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_nowDeformData;		//��ǰ�ı�������
	float m_nowData;			//��ǰ������
	_XPressState m_pressMinState;	//ѹ��״̬
	_XPressState m_pressMaxState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	_XPressDataRecoverMode m_recoverMode;		//�Ƿ��Զ��ָ�0:���ظ���1:�����ָ���2�����ָ�
	int m_sensitivity;			//������	
	int m_nowPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	_XBool m_isRecover;		//�����Ƿ���ȫ�ָ�

	_XKeyValue m_minKey;
	_XKeyValue m_maxKey;
public:
	//normalValue:��ͨ״̬��ֵ
	//maxDeformValue:���ε����ֵ
	//minDeformValue:���ε���Сֵ
	//sensitivity;����ϵ������λ ms
	void init(float normalValue,float maxDeformValue,float minDeformValue,
		int sensitivity,_XPressDataRecoverMode recoverMode = MODE_SLOW_RECOVER);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const {return (m_pressMinState != PRESS_STATE_NORMAL || 
		m_pressMaxState != PRESS_STATE_NORMAL);}
	//�������������δ������Ȳ���
	void setNowData(float data)
	{
		if(data < m_minDeformData) data = m_minDeformData; 
		if(data > m_maxDeformData) data = m_maxDeformData; 
		m_nowData = data;
		m_isRecover = XFalse;
	}
	_XPressDataDouble()
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
	void setKeyValue(_XKeyValue min,_XKeyValue max)
	{
		m_minKey = min;
		m_maxKey = max;
	}
	void updateState(const _XInputEvent &e)
	{
		if(e.type == EVENT_KEYBOARD)
		{
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
	}
};

class _XKeyPressModel
{
private:
	bool m_isInited;
	int m_sensitivity;			//������,��ʼ�ۻ���ʱ��,��λms
	int m_pressSpeed;			//������Ч��ʱ����λms
	_XKeyValue m_keyValue;		//��Ӧ�İ���ֵ
	bool m_isPress;			//�Ƿ���
	bool m_isKeyDown;
	int m_timer;
public:
	_XKeyPressModel()
		:m_isInited(false)
		,m_sensitivity(500)
		,m_pressSpeed(100)
		,m_keyValue(XKEY_LEFT)
	{}
	bool init(_XKeyValue key,int sensitivity = 500,int pressSpeed = 100)
	{
		m_keyValue = key;
		m_sensitivity = sensitivity; 
		m_pressSpeed = pressSpeed;

		m_isPress = false;
		m_isKeyDown = false;
		m_timer = 0;
		m_isInited = true;
		return true;
	}
	bool move(int stepTime)	//�����Ƿ񴥷�����ʱ��
	{
		if(!m_isInited) return false;
		if(m_isKeyDown)
		{
			m_timer += stepTime;
			if(m_timer >= m_sensitivity)
			{//���������������ʱ��
				m_isPress = true;
				m_timer -= m_pressSpeed;
			}
		}
		if(m_isPress)
		{
			m_isPress = false;
			return true;
		}
		return false;
	}
	void updateState(const _XInputEvent &e)
	{
		if(!m_isInited) return;
		if(e.type == EVENT_KEYBOARD && e.keyValue == m_keyValue)
		{
			if(e.keyState == KEY_STATE_DOWN)
			{
				m_isPress = true;
				m_isKeyDown = true; 
				m_timer = 0;
			}else
			if(e.keyState == KEY_STATE_UP)
			{
				m_isPress = false;
				m_isKeyDown = false; 
			}
		}
	}
};
#endif