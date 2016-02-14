#ifndef _JIA_XSOUNDCORE_
#define _JIA_XSOUNDCORE_
//����һ�����ĵ������Ļ��࣬���е��������඼��Ҫ����������м̳У��Ӷ��ﵽ��ܼ̳е�Ŀ��
//������Ƶ�Ŀ����Ϊ���ܸ�������滻һ���ײ�ģ��(��δʵ��),���������¼ܹ�ʱ���롣
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.16
//--------------------------------
#include <string>
#include <vector>
namespace XE{
enum XSoundState
{
	SOUND_STATE_NORMAL,		//��ͨ״̬
	SOUND_STATE_FADEIN,		//����
	SOUND_STATE_FADEOUT,	//����
};
struct XSoundFadeData
{
	XSoundState state;
	int channel;	//ͨ�����
	int soundTimer;
	int soundFadeTime;
	int curSoundVolume;	//��ǰ������
	XSoundFadeData()
		:state(SOUND_STATE_NORMAL)
	{}
};

typedef void (* SND_CALLBACK_FUN)(void *,unsigned char *,int);

class XSoundCore
{
public:
	XSoundCore(){}
	virtual ~XSoundCore() {}
public:
	virtual bool init() = 0;
	virtual void close() = 0;

	//music����
	virtual bool loadMusic(const std::string &filename,void *&p) = 0;
	virtual void clearMusic(void *p) = 0;
	virtual int setMusicVolume(int v) = 0;
	virtual int getMusicVolume() = 0;
	virtual int playMusic(void *p,int loop) = 0;
	virtual int musicFadeIn(void * p,int loop,int ms) = 0;
	virtual int musicFadeOut(int ms) = 0;

	virtual void pauseMusic() = 0;
	virtual void resumeMusic() = 0;
	virtual void rewindMusic() = 0;
	virtual bool isMusicPause() = 0;
	virtual bool isMusicPlaying() = 0;
	virtual int haltMusic() = 0;
	//sound����
	virtual bool loadSound(const std::string &filename,void *&p) = 0;	//���ļ��ж�ȡ������Դ
	virtual bool loadSound(void *data,int len,void *&p) = 0;			//���ڴ��ж�ȡ������Դ
	virtual void clearSound(void * p) = 0;
	virtual int setVolume(int c,int v) = 0;
	virtual int getVolume(int c) = 0;
	virtual int playSound(void *p,int loop) = 0;

	virtual int soundFadeIn(void * p,int loop,int ms) = 0;
	virtual int soundFadeOut(int c,int ms) = 0;
	virtual void pauseSound(int c) = 0;
	virtual void resumeSound(int c) = 0;
	virtual bool isSoundPause(int c) = 0;
	virtual bool isSoundPlaying(int c) = 0;
	virtual int haltSound() = 0;
	virtual int haltSound(int c) = 0;

	//�����ǹ��ڻص���������
	virtual void setCallBack(SND_CALLBACK_FUN fun,void *) = 0;
	//Ϊ���滻fadein��fadeout �ӿ����涨��
	virtual void update(float stepTime) = 0;

	virtual unsigned char * getData(void *p) = 0;
	virtual int getDataLen(void *p) = 0;
	virtual void setData(void *p,unsigned char * d) = 0;
	virtual void setDataLen(void *p,int len) = 0;
};
}
#endif