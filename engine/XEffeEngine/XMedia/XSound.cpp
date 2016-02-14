#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSound.h"
//#include "SDL.h"

#include "XResourcePack.h"
#include "XTimer.h"
namespace XE{
void channelFinished(int channel)
{//��ĳ��ͨ������Ч������ɵ�ʱ�������������
	printf("%d finished!\n",channel);
}
//void effectFunc(int chan,void *stream,int len,void *udata)
void effectFunc(int,void *stream,int len,void *)
{//�������������ת��(�����������ʵ��һ�������ŵĹ���):��crash������֪����ôŪ�ء�
	if (gFrameworkData.pFrameWork == NULL) return;
	SDL_AudioCVT  wav_cvt;
	int ret = SDL_BuildAudioCVT(&wav_cvt,
		gFrameworkData.pFrameWork->getAudioFormat(), gFrameworkData.pFrameWork->getAudioChannelSum(), 
		gFrameworkData.pFrameWork->getAudioSampleRate(),
		gFrameworkData.pFrameWork->getAudioFormat(), gFrameworkData.pFrameWork->getAudioChannelSum(), 
		gFrameworkData.pFrameWork->getAudioSampleRate() * 2);
	if(ret == -1) printf("Couldn't build converter!\n");
	wav_cvt.buf = (Uint8 *)malloc(len * wav_cvt.len_mult);
	if(wav_cvt.buf == NULL) return;
	wav_cvt.len = len;
	memcpy(wav_cvt.buf,stream,wav_cvt.len);
	SDL_ConvertAudio(&wav_cvt);
	SDL_MixAudio((Uint8 *)stream,wav_cvt.buf, wav_cvt.len_cvt,128);
	free(wav_cvt.buf);
}
XSound::XSound()
	:m_firstAddSoundFlag(XFalse)
	,m_resoursePosition(RESOURCE_AUTO)
{
	m_sound.clear();
//	Mix_ChannelFinished(channelFinished);
}
XSound::~XSound()
{
	clearUp();
}
XSndHandle XSound::addSound(const char* filename,XResourcePosition resoursePosition)
{
	if(!m_firstAddSoundFlag)
	{//�����൱���ǳ�ʼ��
		m_soundVolume = XCurSndCore.getVolume(0);
		m_firstAddSoundFlag = XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	m_resoursePosition = resoursePosition;
	XSoundInfo *temp = XMem::createMem<XSoundInfo>();
	if(temp == NULL) return -1;
	temp->resInfo = XResManager.loadResource(filename,RESOURCE_TYPE_SOUND,m_resoursePosition);
	if(temp->resInfo == NULL)
	{
		XMem::XDELETE(temp);
		return -1;
	}
	temp->sound = ((XResourceSound *)temp->resInfo->m_pointer)->m_handle;
	m_sound.push_back(temp);
	return (int)(m_sound.size()) - 1;
}
void XSound::clearUp()
{//��Դ����Դ����������֮���������ͷ���Դ
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] != NULL && m_sound[i]->resInfo != NULL)
		{
			XResManager.releaseResource(m_sound[i]->resInfo);
			m_sound[i]->resInfo = NULL;
			XMem::XDELETE(m_sound[i]);
		}
	}
	m_sound.clear();
}
void XSound::clearOneSound(XSndHandle soundHandle)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL) return;
	XSoundInfo *temp = m_sound[soundHandle];
	if(temp->resInfo != NULL)
	{
		XResManager.releaseResource(temp->resInfo);	//��δʵ��
		temp->resInfo = NULL;
		XMem::XDELETE(m_sound[soundHandle]);
	}
}
XBool XSound::play(XSndHandle soundHandle,int loops)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL || m_sound[soundHandle]->sound == NULL) return XFalse;
	XSoundInfo *temp = m_sound[soundHandle];
	if(temp->soundTime == -1 || XTime::getCurrentTicks() - temp->soundTime >= MIN_SOUND_DELAY)
	{
		temp->soundTime = XTime::getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",soundHandle);
#endif
		int channel = XCurSndCore.playSound(temp->sound,loops);
		XCurSndCore.setVolume(channel,m_soundVolume);
		temp->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}
XBool XSound::slowDownInit(int rate,XSndHandle handle)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return XFalse;
	XSoundInfo *temp = m_sound[handle];

	if(temp->isSlowDownInit) return XFalse;	//����Ѿ�����������Ҫ���·������ܲ���
	if(temp->sound == NULL) return XFalse;
	//��¼ԭ������
	temp->slowDownOldData = XCurSndCore.getData(temp->sound);
	temp->slowDownOldLen = XCurSndCore.getDataLen(temp->sound);
	//�����µ�����
	temp->slowDownNewLen = temp->slowDownOldLen * rate;
	temp->slowDownNewData = XMem::createArrayMem<unsigned char>(temp->slowDownNewLen);
	temp->slowDownRate = rate;
	//�����µ�����(��������������˫����,16bits��)
	for(int i = 0;i < temp->slowDownOldLen >> 2;++ i)
	{
		for(int j = 0;j < 4 * rate;++ j)
		{
			temp->slowDownNewData[i * 4 * rate + j] = temp->slowDownOldData[i * 4 + (j % 4)];
		}
	}
	//��¼״̬
	temp->isEnableSlowDown = XFalse;
	temp->isSlowDownInit = XTrue;
	return XTrue;
}
void XSound::enableSlowDown(XSndHandle handle)	//ʹ���ӳ�
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(!temp->isEnableSlowDown && temp->isSlowDownInit)
	{
		XCurSndCore.setData(temp->sound,temp->slowDownNewData);
		XCurSndCore.setDataLen(temp->sound,temp->slowDownNewLen);
		temp->isEnableSlowDown = XTrue;
	}
}
void XSound::disableSlowDown(XSndHandle handle)	//ȡ���ӳ�
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(temp->isEnableSlowDown)
	{
		XCurSndCore.setData(temp->sound,temp->slowDownOldData);
		XCurSndCore.setDataLen(temp->sound,temp->slowDownOldLen);
		temp->isEnableSlowDown = XFalse;
	}
}
void XSound::slowDownRelease(XSndHandle handle)	//�ͷ���Դ
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	XSoundInfo *temp = m_sound[handle];
	if(temp->isSlowDownInit)
	{
		if(temp->isEnableSlowDown) disableSlowDown(handle);
		XMem::XDELETE_ARRAY(temp->slowDownNewData);
		temp->isSlowDownInit = XFalse;
	}
}
XBool XSound::slowDownAllInit(int rate)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] == NULL) continue;
		if(m_sound[i]->isSlowDownInit)
		{
			if(m_sound[i]->slowDownRate != rate)
			{
				slowDownRelease(i);
				slowDownInit(rate,i);
			}
		}else
		{
			slowDownInit(rate,i);
		}
	}
	return XTrue;
}
void XSound::enableAllSlowDown()	//ʹ���ӳ�
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		enableSlowDown(i);
	}
}
void XSound::disableAllSlowDown()	//ȡ���ӳ�
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		disableSlowDown(i);
	}
}
void XSound::slowDownAllRelease()	//�ͷ���Դ
{
	for(unsigned int i = 0;i < m_sound.size();++ i)
	{
		slowDownRelease(i);
	}
}
XBool XSound::fadeInSound(XSndHandle s,int loops,int ms)
{
	if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL
		|| m_sound[s]->sound == NULL) return XFalse;

	if(m_sound[s]->soundTime == -1 || XTime::getCurrentTicks() - m_sound[s]->soundTime >= MIN_SOUND_DELAY)
	{
		m_sound[s]->soundTime = XTime::getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",s);
#endif
		int channel = XCurSndCore.soundFadeIn(m_sound[s]->sound,loops,ms);
		XCurSndCore.setVolume(channel,m_soundVolume);
		m_sound[s]->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}
}