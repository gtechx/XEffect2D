#ifndef _JIA_XSOUNDFMOD_
#define _JIA_XSOUNDFMOD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.16
//--------------------------------

#include "XSoundCore.h"
#include "../XFrameWork.h"
extern "C" 
{
	#include "fmod.h"
//	#include "fmod_errors.h"
}
#pragma comment(lib, "../../engine/lib/fmodex/vs/fmodex_vc.lib")
#if WITH_ALL_WARNING == 0
#pragma warning(disable: 4312)    //'type cast' : conversion from 'int' to 'FMOD_CHANNEL *' of greater size
#pragma warning(disable: 4311)    //'type cast' : pointer truncation from 'FMOD_CHANNEL *' to 'int'
#endif
namespace XE{
class XSoundFmod:public XSoundCore
{
public:
	XSoundFmod() 
		:m_musicState(SOUND_STATE_NORMAL)
		,m_curMusic(NULL)
		,m_isCallBackOpen(false)
		,m_musicChannel(NULL)
	{}
	virtual ~XSoundFmod() {}
protected:
	XSoundFmod(const XSoundFmod&);
	XSoundFmod &operator= (const XSoundFmod&);
private:
	XSoundState m_musicState;
	int m_curMusicVolume;	//��ǰ���ֵ�����
	int m_musicTimer;
	int m_musicFadeTime;
	std::vector<XSoundFadeData> m_soundFadeList;	//����

	FMOD_CHANNELGROUP *m_channelGroup;
	FMOD_SYSTEM *m_sys;
	FMOD_CHANNEL *m_musicChannel;
	void *m_curMusic;
	bool m_isCallBackOpen;
	FMOD_SOUND *m_cbSound;
	FMOD_CHANNEL *m_cbChannel;
	SND_CALLBACK_FUN m_cbFun;
	void *m_cbData;
	friend FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND *,void *,unsigned int);
public:
	bool init();
	void close() 
	{
		if(m_isCallBackOpen)
		{
			FMOD_Channel_Stop(m_cbChannel);
			FMOD_Sound_Release(m_cbSound);
			m_isCallBackOpen = false;
		}
		FMOD_System_Close(m_sys);
		FMOD_System_Release(m_sys);
	}
	//music����
	bool loadMusic(const std::string &filename,void *&p)
	{
		int fmodFlags = FMOD_HARDWARE | FMOD_CREATESTREAM;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,filename.c_str(),fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}

	void clearMusic(void *p){FMOD_Sound_Release((FMOD_SOUND *)p);}
	int setMusicVolume(int v) {return FMOD_Channel_SetVolume(m_musicChannel,v / 128.0f);} 
	int getMusicVolume()
	{
		float v;
		if(m_musicChannel == NULL) FMOD_ChannelGroup_GetVolume(m_channelGroup,&v);
		else FMOD_Channel_GetVolume(m_musicChannel,&v);	//��Χ���� 0 - 1.0f;
		return (int)(v * 128.0f);
	}
	int playMusic(void *p,int loop)
	{
		FMOD_RESULT result = FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE,(FMOD_SOUND *)p,false,&m_musicChannel);
		if(result != FMOD_OK) return -1;
		if(loop != 0)
		{
			FMOD_Channel_SetMode(m_musicChannel,FMOD_LOOP_NORMAL);	//���ﲻ�ܿ���ѭ������
			FMOD_Channel_SetLoopCount(m_musicChannel,loop);	//loopΪ-1��fmod���Ƿ�Ϊ����ѭ��
		}else FMOD_Channel_SetMode(m_musicChannel,FMOD_LOOP_OFF);
		m_curMusic = p;
		return (int)m_musicChannel;
	}
	int musicFadeIn(void * p,int loop,int ms);
	int musicFadeOut(int ms);

	void pauseMusic(){FMOD_Channel_SetPaused(m_musicChannel,true);}
	void resumeMusic() {FMOD_Channel_SetPaused(m_musicChannel,false);}
	void rewindMusic() 
	{//û�в��Թ�
		haltMusic();
		playMusic(m_curMusic,0);
	}
	bool isMusicPause() 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_GetPaused(m_musicChannel,&b);
		return b != 0;
	}
	bool isMusicPlaying() 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_IsPlaying(m_musicChannel,&b);
		return b != 0;
	}
	int haltMusic() 
	{
		m_musicState = SOUND_STATE_NORMAL;
		return FMOD_Channel_Stop(m_musicChannel);
	} 
	//sound����
	bool loadSound(const std::string &filename,void *&p)	//���ļ��ж�ȡ������Դ
	{
		int fmodFlags = FMOD_HARDWARE;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,filename.c_str(),fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}
	bool loadSound(void *data,int,void *&p)
	{
		int fmodFlags = FMOD_HARDWARE | FMOD_OPENMEMORY;
		FMOD_SOUND *sound = NULL;
		FMOD_RESULT result = FMOD_System_CreateSound(m_sys,(char *)data,fmodFlags,NULL,&sound);
		if(result != FMOD_OK) return false;

		//FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_PCM);
		p = sound;
		return true;
	}
	void clearSound(void * p) {FMOD_Sound_Release((FMOD_SOUND *)p);}
	int setVolume(int c,int v) {return FMOD_Channel_SetVolume((FMOD_CHANNEL *)c,v/128.0f);}
	int getVolume(int c) 
	{
		float v;
		if(c == NULL) FMOD_ChannelGroup_GetVolume(m_channelGroup,&v);
		else FMOD_Channel_GetVolume((FMOD_CHANNEL *)c,&v);	//��Χ����
		return (int)(v * 128.0f);
	}
	int playSound(void *p,int loop) 
	{
		FMOD_CHANNEL * c = NULL;
		FMOD_RESULT result = FMOD_System_PlaySound(m_sys, FMOD_CHANNEL_FREE,(FMOD_SOUND *)p,false,&c);
		if(result != FMOD_OK) return -1;
		if(loop != 0) FMOD_Channel_SetMode(c,FMOD_LOOP_NORMAL);	//���ﲻ�ܿ���ѭ������
		else FMOD_Channel_SetMode(c,FMOD_LOOP_OFF);
		return (int)c;
	}

	int soundFadeIn(void * p,int loop,int ms);
	int soundFadeOut(int c,int ms);
	void pauseSound(int c) {FMOD_Channel_SetPaused((FMOD_CHANNEL *)c,true);}
	void resumeSound(int c) {FMOD_Channel_SetPaused((FMOD_CHANNEL *)c,false);}
	bool isSoundPause(int c) 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_GetPaused((FMOD_CHANNEL *)c,&b);
		return b != 0;
	}
	bool isSoundPlaying(int c) 
	{
		FMOD_BOOL b = 0;
		FMOD_Channel_IsPlaying((FMOD_CHANNEL *)c,&b);
		return b != 0;
	}
	int haltSound() {return FMOD_ChannelGroup_Stop(m_channelGroup);}	//����������⣬��ر�music
	int haltSound(int c) 
	{
		return FMOD_Channel_Stop((FMOD_CHANNEL *)c);
	} 

	//�����ǹ��ڻص���������
	void setCallBack(SND_CALLBACK_FUN fun, void *arg);
	//Ϊ���滻fadein��fadeout �ӿ����涨��
	void update(float stepTime);

	//����Ľӿ��޷�ʵ��,��ȡĳ��sound�����ݺ����ݳ��ȣ�������
	unsigned char * getData(void *) {return NULL;}
	int getDataLen(void *) {return 0;}
	void setData(void *,unsigned char * ) {;}
	void setDataLen(void *,int) {;}

	//���Ի��Ľӿ�
	//�ı䲥�ŵ�λ��vol [-1 +1]
	void setPan(int c,float vol) {FMOD_Channel_SetPan((FMOD_CHANNEL *)c,vol);}
	//�ı䲥���ٶ�
	void setSpeed(int c,float spd) {FMOD_Channel_SetFrequency((FMOD_CHANNEL *)c, XEG.getAudioSampleRate() * spd);}
};
}
#endif