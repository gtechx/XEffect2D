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
#if AUDIO_MATHOD == 0
	int lateChannel;	//��¼������ŵ�ǰ��Ч���������Ա��ڿ��Կ��������Ч��ֹͣ��
#endif
#if AUDIO_MATHOD == 1
	void* lateChannel;	//��¼������ŵ�ǰ��Ч���������Ա��ڿ��Կ��������Ч��ֹͣ��
#endif
	XSoundInfo()
		:sound(NULL)
		,resInfo(NULL)
		,isSlowDownInit(XFalse)
		,isEnableSlowDown()
		,slowDownOldData(NULL)
		,slowDownNewData(NULL)
		,soundTime(-1)
#if AUDIO_MATHOD == 0
		,lateChannel(-1)
#endif
#if AUDIO_MATHOD == 1
		,lateChannel(nullptr)
#endif
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
	XResPos m_resoursePosition;	//��Դλ��
private:
	std::vector<XSoundInfo *> m_sound;

	int m_soundVolume;
	XBool m_firstAddSoundFlag;
public:
	XSndHandle addSound(const char* filename,XResPos resPos = RES_SYS_DEF);					//����һ����Ч�����������Ч�ľ�������ڲ���
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

	void pause(XSndHandle s);
	void resume(XSndHandle s);
	XBool isPause(XSndHandle s);
	void stop(XSndHandle s);
	XBool isEnd(XSndHandle s);
	XBool fadeInSound(XSndHandle s,int loops,int ms);
	void fadeOutSound(XSndHandle s, int ms);
	void release();	//ע����������಻���Զ�������������������ͷź���������һ��Ҫ�ǵ����������ͷź������Զ����õĻ���Ҫ�����ظ�����
	static bool getIsInvalid(XSndHandle hangle){return hangle < 0;} 
};
inline void XSound::setSoundVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
}
}
#endif