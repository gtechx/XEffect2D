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
	virtual ~XMusic();
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

	XBool isEnd();
	void pause();
	void resume();
	void rewind();
	XBool isPause();
	static bool getIsInvalid(XMusicHandle hangle){return hangle < 0;} 
};
}
#endif