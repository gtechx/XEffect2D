#ifndef _JIA_MOVIECORE_
#define _JIA_MOVIECORE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.20
//--------------------------------
#include "../XCommonDefine.h"
#include "XCommonDefine.h"
namespace XE{
//����ӰƬ������Ļ��࣬���е�ӰƬ�����඼�Ǽ̳����������
class XTexture;
class XMovieCore
{
protected:
public:
	virtual ~XMovieCore() {}
	virtual XBool load(const char *filename,XColorMode mode = COLOR_RGB,bool withVoice = true) = 0;	//����ָ��ӰƬ
	virtual XBool updateFrame() = 0;			//������Ƶ�����Ƿ����
	virtual void draw() = 0;					//���
	virtual void updateTex(XTexture &tex) = 0;
	virtual unsigned int * getTexture() = 0;
	//virtual void setPauseOrPlay(void) = 0;
	virtual void pause() = 0;
	virtual void play() = 0;
	virtual XBool isPlaying()const = 0;
	virtual void closeClip(void) = 0;
	virtual void setLoop(XBool isLoop) = 0;	//�����Ƿ�ѭ��
	virtual XBool getIsLoop()const = 0;	//��ȡ�Ƿ�ѭ��
	virtual XBool getIsEnd()const = 0;	//��ȡ�Ƿ񲥷����
	virtual XBool getIsLoad()const = 0;	//�Ƿ�����
	virtual double getCurrentPlayingTime() const = 0;
};
template<typename T>
class XMovie	//�������Ҫʹ������ָ��
{
private:
	XMovieCore *m_pObj;
public:
	XMovie(){m_pObj = XMem::createMem<T>();}
	virtual ~XMovie(){XMem::XDELETE(m_pObj);}
	XBool load(const char *filename) {return m_pObj->load(filename);}
	XBool updateFrame() {return m_pObj->updateFrame();}
	void draw() {m_pObj->draw();}
	void updateTex(XTexture &tex) {m_pObj->updateTex(tex);}
	unsigned int* getTexture() {return m_pObj->getTexture();}
//	void setPauseOrPlay(void) {m_pObj->setPauseOrPlay();}
	virtual void pause(){m_pObj->pause();}
	virtual void play(){m_pObj->play();}
	virtual XBool isPlaying(){return m_pObj->isPlaying();}
	void closeClip(void) {m_pObj->closeClip();}
	void setLoop(XBool isLoop) {m_pObj->setLoop(isLoop);}
	XBool getIsLoop() {return m_pObj->getIsLoop(isLoop);}	//��ȡ�Ƿ�ѭ��
	XBool getIsEnd() {return m_pObj->getIsEnd(isLoop);};	//��ȡ�Ƿ񲥷����
	double getCurrentPlayingTime() const {return m_pObj->getCurrentPlayingTime();};
private:
	XMovie(const XMovie &temp);
	XMovie& operator = (const XMovie& temp);
};
}
#endif