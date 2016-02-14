#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSoundCommon.h"
//#include "XBasicFun.h"
#include "XResourceManager.h"
//�޸Ŀ��Դ���Դ��������ȡ��Դ
#include <vector>
namespace XE{
//#define MAX_SOUND_SUM (128)	//�����Ч����
#define MIN_SOUND_DELAY (50)	//��ͬ��Ч���ŵ���Сʱ��������λΪ����

struct XSoundInfo
{
	void * sound;	//Mix_Chunk
	XResourceInfo *resInfo;
	int soundTime;						//�����Ա���ڿ�����Ч���ŵļ������ֹͬһ����Ч�ڶ�ʱ���ڲ��ϵز���

	XBool isSlowDownInit;
	XBool isEnableSlowDown;
	unsigned char *slowDownOldData;
	int slowDownOldLen;
	unsigned char *slowDownNewData;
	int slowDownNewLen;
	int slowDownRate;
	int lateChannel;	//��¼������ŵ�ǰ��Ч���������Ա��ڿ��Կ��������Ч��ֹͣ��

	XSoundInfo()
		:sound(NULL)
		,resInfo(NULL)
		,isSlowDownInit(XFalse)
		,isEnableSlowDown()
		,slowDownOldData(NULL)
		,slowDownNewData(NULL)
		,soundTime(-1)
		,lateChannel(-1)
	{}
};

typedef int XSndHandle;
//������Ϊ����ϵͳ
class XSound
{
public:
	XSound();
	virtual ~XSound();
protected:
	XSound(const XSound&);
	XSound &operator= (const XSound&);
private:
	XResourcePosition m_resoursePosition;	//��Դλ��
private:
	std::vector<XSoundInfo *> m_sound;

	int m_soundVolume;
	XBool m_firstAddSoundFlag;
public:
	XSndHandle addSound(const char* filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//����һ����Ч�����������Ч�ľ�������ڲ���
	void clearUp();											//��������������������Դ
	void clearOneSound(XSndHandle soundHandle);			//�����ָ������Ч��Դ
	XBool play(XSndHandle soundHandle,int loops = 0);	//-1Ϊ����ѭ��
	void setSoundVolume(int volume);						//���õ�ǰ��Ч���ŵ�����
	void setAllVolume(int volume);							//���������������ŵ�����

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǹ��������ӳ��ķ�����ֻ��SDL_Mixer����С
	XBool slowDownInit(int rate,XSndHandle handle);
	void enableSlowDown(XSndHandle handle);	//ʹ���ӳ�
	void disableSlowDown(XSndHandle handle);	//ȡ���ӳ�
	void slowDownRelease(XSndHandle handle);	//�ͷ���Դ
	XBool slowDownAllInit(int rate);
	void enableAllSlowDown();	//ʹ���ӳ�
	void disableAllSlowDown();	//ȡ���ӳ�
	void slowDownAllRelease();	//�ͷ���Դ
	//-------------------------------------------------------

	void pause(XSndHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		XCurSndCore.pauseSound(m_sound[s]->lateChannel);
	}
	void resume(XSndHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		XCurSndCore.resumeSound(m_sound[s]->lateChannel);
	}
	XBool isPause(XSndHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XFalse;
		return XCurSndCore.isSoundPause(m_sound[s]->lateChannel);
	}
	void stop(XSndHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		XCurSndCore.soundFadeOut(m_sound[s]->lateChannel,100);
	}
	XBool isEnd(XSndHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XTrue;
		return !XCurSndCore.isSoundPlaying(m_sound[s]->lateChannel);
	}
	XBool fadeInSound(XSndHandle s,int loops,int ms);
	void fadeOutSound(XSndHandle s,int ms)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		XCurSndCore.soundFadeOut(m_sound[s]->lateChannel,ms);
	}
	void release()	//ע����������಻���Զ�������������������ͷź���������һ��Ҫ�ǵ����������ͷź������Զ����õĻ���Ҫ�����ظ�����
	{
		XCurSndCore.haltSound();//�ر����е�channel
		clearUp();
	}
};
inline void XSound::setSoundVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
}
inline void XSound::setAllVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
	XCurSndCore.setVolume(-1,m_soundVolume);
}
}
#endif