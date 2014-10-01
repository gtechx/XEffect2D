#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSoundCommon.h"
#include "XLogBook.h"
#include <vector>

//Ŀǰ���񲢲�֧�ִ��ڴ�������MP3��ʽ���鿴sdl_mixer�е�Mix_LoadMUS_RW˵���к�����˼��ֻ֧��Ogg��MikMod
//#define MAX_MUSIC_SUM 128	//��������
typedef int XMusicHandle;
class _XMusic
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XMusic()
		:m_firstAddMusicFlag(XFalse)
	{
		m_music.clear();
	}
	_XMusic(const _XMusic&);
	_XMusic &operator= (const _XMusic&);
	virtual ~_XMusic()
	{
		_XSoundHandle.haltMusic();
		clearUp();
	}
public:
	static _XMusic& GetInstance()
	{
		static _XMusic m_instance;
		return m_instance;
	}
	//-------------------------------------------
//public:
//	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	std::vector<void *> m_music;	//Mix_Music
	//Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	_XBool m_firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();												//��������������������Դ
	void clearOneMusic(XMusicHandle musicHandle);				//�����ָ������Ч��Դ
	_XBool playMusic(XMusicHandle musicHandle,int loop = 0);	//-1����ѭ��
	void setMusicVolume(int volume);							//���õ�ǰ��Ч���ŵ�����(0 - 128)
	void fadeOutMusic(int ms);
	_XBool fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);

	_XBool isEnd() {return !_XSoundHandle.isMusicPlaying();}
	void pause(){_XSoundHandle.pauseMusic();}
	void resume(){_XSoundHandle.resumeMusic();}
	void rewind(){_XSoundHandle.rewindMusic();}
	_XBool isPause() {return _XSoundHandle.isMusicPause();}
};
inline _XBool _XMusic::fadeInMusic(XMusicHandle musicHandle,int loop,int ms)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
	if(_XSoundHandle.musicFadeIn(m_music[musicHandle],loop,ms) == -1) return XFalse;
	return XTrue;
}
inline void _XMusic::fadeOutMusic(int ms)
{
	_XSoundHandle.musicFadeOut(ms);
}
inline void _XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return;
	if(m_music[musicHandle] != NULL)
	{
		_XSoundHandle.clearMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}
inline _XBool _XMusic::playMusic(XMusicHandle musicHandle,int loop)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
	if(_XSoundHandle.playMusic(m_music[musicHandle],loop) == -1)
	{
		LogStr("Sound play error!");
		return XFalse;
	}
	return XTrue;
}
inline void _XMusic::setMusicVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_musicVolume = volume;
	_XSoundHandle.setMusicVolume(m_musicVolume);
}
#endif