#include "XStdHead.h"
#include "XSoundSDLMixer.h"

namespace XE{
bool XSoundSDLMixer::init()
{
//��ʼ�������豸(Music Sound ��Movie�࣬��߶��������豸���ڳ�ͻ������Ҫ��ռ)
	if (gFrameworkData.pFrameWork == NULL) return false;
//	gFrameworkData.pFrameWork->audioSampleRate = MIX_DEFAULT_FREQUENCY;
	gFrameworkData.pFrameWork->setAudioSampleRate(44100);
	gFrameworkData.pFrameWork->setAudioFormat(MIX_DEFAULT_FORMAT);
	gFrameworkData.pFrameWork->setAudioChannelSum(2);
	const int TMP_CHUNK_SIZE = 2048;// + 192000 * 3 /2;

	Mix_OpenAudio(gFrameworkData.pFrameWork->getAudioSampleRate(), 
		gFrameworkData.pFrameWork->getAudioFormat(), 
		gFrameworkData.pFrameWork->getAudioChannelSum(), TMP_CHUNK_SIZE);
	Mix_AllocateChannels(128);
	return true;
}
}