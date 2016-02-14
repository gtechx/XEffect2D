#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XSoundCommon.h"
#include <vector>
namespace XE{
//Ŀǰ���񲢲�֧�ִ��ڴ�������MP3��ʽ���鿴sdl_mixer�е�Mix_LoadMUS_RW˵���к�����˼��ֻ֧��Ogg��MikMod
//#define MAX_MUSIC_SUM 128	//��������
typedef int XMusicHandle;
class XMusic
{
public:
	XMusic()
		:m_firstAddMusicFlag(XFalse)
	{
		m_music.clear();
	}
	virtual ~XMusic()
	{
		XCurSndCore.haltMusic();
		clearUp();
	}
protected:
	XMusic(const XMusic&);
	XMusic &operator= (const XMusic&);
//public:
//	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	std::vector<void *> m_music;	//Mix_Music
	//Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	XBool m_firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();												//��������������������Դ
	void clearOneMusic(XMusicHandle musicHandle);				//�����ָ������Ч��Դ
	XBool playMusic(XMusicHandle musicHandle,int loop = 0);	//-1����ѭ��
	void setMusicVolume(int volume);							//���õ�ǰ��Ч���ŵ�����(0 - 128)
	void fadeOutMusic(int ms);
	XBool fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);

	XBool isEnd() {return !XCurSndCore.isMusicPlaying();}
	void pause(){XCurSndCore.pauseMusic();}
	void resume(){XCurSndCore.resumeMusic();}
	void rewind(){XCurSndCore.rewindMusic();}
	XBool isPause() {return XCurSndCore.isMusicPause();}
};
inline XBool XMusic::fadeInMusic(XMusicHandle musicHandle,int loop,int ms)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return XFalse;
	if(XCurSndCore.musicFadeIn(m_music[musicHandle],loop,ms) == -1) return XFalse;
	return XTrue;
}
inline void XMusic::fadeOutMusic(int ms)
{
	XCurSndCore.musicFadeOut(ms);
}
inline void XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if(musicHandle < 0 || musicHandle >= m_music.size()
		|| m_music[musicHandle] == NULL) return;
	if(m_music[musicHandle] != NULL)
	{
		XCurSndCore.clearMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}
inline void XMusic::setMusicVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_musicVolume = volume;
	XCurSndCore.setMusicVolume(m_musicVolume);
}
}
#endif