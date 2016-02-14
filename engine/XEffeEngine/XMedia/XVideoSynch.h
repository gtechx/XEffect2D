#ifndef _JIA_XVIDEOSYNCH_
#define _JIA_XVIDEOSYNCH_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2015.4.3
//--------------------------------
#include "XCritical.h"
#include "XThread.h"
#include "XNetWork\XNetCommon.h"
#include "XMedia\XMovieFfmpeg.h"
namespace XE{
enum XVideoSynchRole
{
	VS_ROLE_SERVER,	//ͬ���ķ�����
	VS_ROLE_CLIENT,	//ͬ���Ŀͻ���
};
//���ݴ���Э��
//ͷ	nameLen		name	videoNameLen	videoName	frameIndex	check	β
//0xcc	4Bytes		name	4Bytes			name		4Bytes		1Byte	0xdd
//0xccͷ��0xddβ��0xeeת��
class XBroadcastUdp
{
private:
	bool m_isInited;			//�Ƿ���ɳ�ʼ��
	std::string m_name;			//ͬ��������
	XVideoSynchRole m_role;	//��ɫ
	int m_port;					//���ڹ㲥�Ķ˿�

	//����������ͨѶ������
	int m_sendSocket;	//���͵��׽���
	sockaddr_in m_addrUDP;
	unsigned char *m_data;
	unsigned char *m_dataSend;
	int m_dataLen;	//����buff�Ĵ�С
	int m_sendLen;	//��Ҫ���͵����ݴ�С
	int m_recvSocket;	//���յ��׽���

	int m_curFrameIndex;
	std::string m_curVideoName;

	bool readFromCFG(const std::string &filename);
	bool sendInfo();	//�����緢��ͬ������
	void addSpecialChar();	//����ת���ַ�
	unsigned char getCheck();	//����У���ַ�
	void decSpecialChar(const unsigned char * inData,int inLen,unsigned char * outData,int &outLen);	//��ȥת���ַ�

	XThreadState m_recvThreadState;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳� 
	bool checkData(const unsigned char * p,int len);

	//������ͬ��ʱ��
	XTimer m_timer;	//�߾��ȼ�ʱ��
	int m_delayTimer;	//��ʱ�����ӳ�
	XCritical m_mutex;

	void *m_pClass;
	void (*m_commandFun)(void *,int);	//�յ���������ʱ�Ļص�����
public:
	void setCallbackFun(void *pClass,void (*commandFun)(void *,int))
	{
		m_pClass = pClass;
		m_commandFun = commandFun;
	}
	int getTimer();	//��ȡ���뼶��ʱ��
	XBroadcastUdp()
		:m_isInited(false)
		,m_curVideoName("")
		,m_curFrameIndex(0)
		,m_dataLen(0)
		,m_data(NULL)
		,m_dataSend(NULL)
		,m_pClass(NULL)
		,m_commandFun(NULL)
	{}
	~XBroadcastUdp(){release();}
	void release();
	bool init(const std::string &cfgFileName);
	void setCurFrame(int frameIndex);
	void setCurVideo(const std::string &filename);
	std::string getCurVideoName() const {return m_curVideoName;}
	int getCurFrameIndex() const {return m_curFrameIndex;}
	XVideoSynchRole getRole() const {return m_role;}
};
enum XVideoSynchState
{
	STATE_NORMAL = 0,		//��ͨ״̬
	STATE_PLAYING_END = -1,	//���Ž���
	STATE_CLOSE_CLIP = -2,	//�ر�ӰƬ
	STATE_LOAD = -3,			//Ԥ����ӰƬ
	STATE_PLAY = -4,			//��ʼ����
	STATE_WAITING = -128,		//�ȴ�״̬
};
//�����Ǽ���Ƶͬ������
class XSimpleVideoSynch:public XMovieCore
{
private:
	bool m_isInited;
	XBroadcastUdp m_udp;
	XMovieFfmpeg m_movie;
	XVector2 m_offset;
	XVector2 m_size;
	std::string m_curVideoName;
	static void commandFun(void *,int);
	XVideoSynchState m_commandIndex;
	bool m_isReadyToPlay;	//�Ƿ�׼�����ţ����ú�������ʱ�����򲢲������̿�ʼ���ţ�������Ҫ�ȴ��ͻ���������ܿ�ʼ����
	float m_playTimer;
public:
	virtual XBool getIsLoad()const{return m_movie.getIsLoad();}
	XBool init(const std::string &filename);
	void setOffset(const XVector2 &offset) {m_offset = offset;}
	void setSize(const XVector2 &size) {m_size = size;}

	virtual XBool load(const char *filename,XColorMode mode = COLOR_RGB,bool withVoice = true);
	virtual XBool updateFrame();			//������Ƶ�����Ƿ����
	virtual void draw(){m_movie.draw();}
	virtual void updateTex(XTexture &tex) {m_movie.updateTex(tex);}
	virtual unsigned int * getTexture() {return m_movie.getTexture();}
	//virtual void setPauseOrPlay(void) = 0;
	virtual void pause() {m_movie.pause();}
	virtual void play();
	virtual XBool isPlaying() const {return m_movie.isPlaying();}
	virtual void closeClip(void);
	virtual void setLoop(XBool isLoop) {m_movie.setLoop(isLoop);}	//�����Ƿ�ѭ��
	virtual XBool getIsLoop()const {return m_movie.getIsLoop();}	//��ȡ�Ƿ�ѭ��
	virtual XBool getIsEnd()const {return m_movie.getIsEnd();}	//��ȡ�Ƿ񲥷����
	virtual double getCurrentPlayingTime() const {return m_movie.getCurrentPlayingTime();}

	XSimpleVideoSynch()
		:m_isInited(false)
		,m_commandIndex(STATE_NORMAL)
		,m_isReadyToPlay(false)
	{}
	virtual ~XSimpleVideoSynch()
	{}
};
//�����ǹ���ý�岥������������ݽṹ�Ķ���

//�б�����
//����
//ƫ��
//�ߴ�
//ѭ���������˳��
//����ָ���ͣ�����š�������һ��������ָ���Ӧ��osc����
//�б�ʼ�����Ƿ���oscָ���Լ�ָ������
//�б���������Ƿ���oscָ���Լ�ָ������

//�б�������
//ͷ��ͣʱ��
//ѭ��ͷ��ͣʱ��
//ӰƬ����
//ѭ��βͣ��ʱ��
//β��ͣʱ��
//ѭ��������-1������ѭ����0��ѭ����1ѭ��һ�Ρ���
//�Ƿ�ͣ�ڽ�β
//��ʼ����ʱ�Ƿ���oscָ���Լ�ָ������
//��������ʱ�Ƿ���oscָ���Լ�ָ������
enum XOscDataType
{
	TYPE_INT32,
	TYPE_INT64,
	TYPE_FLOAT,
	TYPE_STIRNG,
};
struct XOscCommand
{//oscָ��Ľṹ
	bool isEnable;		//�Ƿ���Ч
	std::string ip;		//ͨѶ��IP
	int port;			//ͨѶ�Ķ˿ں�
	std::string addr;	//ͨѶ�ĵ�ַ
	XOscDataType type;	//����
	int valueI;
	float valueF;
	std::string valueS;
	XOscCommand()
		:isEnable(false)
	{}
};
struct XListItem
{//�б���Ľṹ
	int beginDelay;			//ͷ�ӳٵ�λms
	int loopBeginDelay;		//ѭ��ͷ�ӳ�
	std::string filename;	//�ļ���
	int loopEndDelay;		//ѭ��β�ӳ�
	int endDelay;			//����βʱ��
	int loopTimes;			//ѭ������
	bool stayAtEnd;			//�������֮��ֹͣ
	XOscCommand cmdAtBegin;	//����ͷ���͵�osc����
	XOscCommand cmdAtEnd;		//����β���͵�osc����
	XListItem()
		:beginDelay(0)
		,loopBeginDelay(0)
		,loopEndDelay(0)
		,endDelay(0)
		,loopTimes(0)
		,stayAtEnd(false)
	{}
};
enum XVideoListMode
{
	MODE_ORDER,	//˳�򲥷�
	MODE_RAND,	//�������
};
struct XVideoList
{//�����б�Ľṹ
	bool isEnable;
	std::string name;		//�б�����
	XVector2 offset;		//��Ƶƫ����������
	XVector2 pixelSize;	//ʹ�õ���Ƶ�����ش�С���ü�����Ч�ߴ�
	bool isLoop;			//�Ƿ�ѭ��
	XVideoListMode mode;	//ѭ��ģʽ
	XOscCommand cmdStop;	//ֹͣ���Ž��յ�oscָ��
	XOscCommand cmdPlay;	//��ʼ���Ž��յ�oscָ��
	XOscCommand cmdNext;	//��һ�����յ�oscָ��
	XOscCommand cmdAtBegin;	//�б�ʼ���ŷ��͵�oscָ��
	XOscCommand cmdAtEnd;		//�б�����ɷ��͵�oscָ��
	std::vector<XListItem> items;
	XVideoList()
		:isEnable(false)
	{}
};
class XMediaPlayer
{//ý�岥��������(��δ���)
private:
public:
};
#if WITH_INLINE_FILE
#include "XVideoSynch.inl"
#endif
}
#endif