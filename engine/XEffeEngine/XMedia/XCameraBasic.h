#ifndef _JIA_XCAMERABASIC_
#define _JIA_XCAMERABASIC_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
//����һ������ͷ��Ƶ������ȡ�Ļ��࣬�������е���Ƶ���ݲɼ����඼�̳��������
#define WITH_GL_TEXTRUE		//�Ƿ��ṩ��OpenGL��ͼ��֧�֣����ֻ�Ǵ���Ļ�ȡ���ݵĻ�����û�б�Ҫ����ͼ����֧�ֵ�
#include "../XOSDefine.h"
#ifdef WITH_GL_TEXTRUE
#include "../XBasicOpenGL.h"
#include "../XSprite.h"
#else
#include "XCommonDefine.h"
#endif
#include "../XBabyBase.h"
namespace XE{
#define SUPPORT_FOR_HIK (1)	//�Ƿ�֧�ֺ���������ͷ

enum XPixelsInputType
{
	TYPE_CAM_NORMAL,	//��ͨ����ͷ
#if SUPPORT_FOR_HIK	
	TYPE_CAM_HIK,		//����������ͷ
#endif
	TYPE_CAM_AVER,		//Բ�յ�����ͷ
	//����Ĺ�������δ��ɣ�
	TYPE_VIDEO,		//��Ƶ����
	TYPE_PICTURE,	//ͼƬ����

	TYPE_KINECT_COLOR,	//kinect �Ĳ�ɫ����
	TYPE_KINECT_DEPTH,	//kinect ���������
	TYPE_KINECT2_COLOR,	//kinect2�Ĳ�ɫ����
	TYPE_KINECT2_DEPTH,	//kinect2���������
};
//���������ز���
struct XPixelsInputInfo	//ԭ����XCameraData
{
	XPixelsInputInfo()
		:isSilence(false)
		,withDefaultFormat(true)
	{}
	std::string filename;	//�ļ����ƺ�·��
	int w;				//���ؿ�
	int h;				//���ظ�
	int fps;			//֡��
	int deviceOrder;	//�豸���
	//��������������Ϊ��videoInput�������
	bool isSilence;		//�Ƿ�����
	bool withDefaultFormat;	//�Ƿ�ʹ��Ĭ�ϵ���Ƶ��ʽ
	
	XPixelsInputType pixelsInputType;	//����ͷ���ͺ�
	bool needReset;	//�Ƿ��ʼ����ͼΪ��ɫ

	void setNormal(int width,int height,int order = 0,int setfps = 30)
	{
		w = width;
		h = height;
		deviceOrder = order;
		fps = setfps;
		pixelsInputType = TYPE_CAM_NORMAL;
		needReset = false;
	}
#if SUPPORT_FOR_HIK
	char ipAddress[16];	//��������ͷ����ַ
	char userName[64];	//�û���
	char pwd[16];		//����
	int port;			//�˿ں�

	void setHIK(int width,int height,const char *add,const char *usern,
		const char * passward,int p = 8000)
	{
		if(add == NULL || usern == NULL || passward == NULL) return;
		w = width;
		h = height;
		strcpy_s(ipAddress,16,add);
		strcpy_s(userName,64,usern);
		strcpy_s(pwd,16,passward);
		port = p;
		pixelsInputType = TYPE_CAM_HIK;
		needReset = false;
	}
#endif
};
class XPixelsInputBasic:public XBabyBase
{
private:
protected:
	XBool m_isInited;		//�Ƿ���г�ʼ��

	int m_pixelsWidth;		//����������ؿ�
	int m_pixelsHeight;		//����������ظ�
#ifdef WITH_GL_TEXTRUE
//	int m_cameraTexWidth;	//��ͼ�����ؿ�
//	int m_cameraTexHeight;	//��ͼ�����ظ߶�
#endif
	int m_buffSize;			//��������buff�ĳߴ�

	XBool m_isWork;			//�����豸�Ĺ���״̬
	XColorMode m_colorMode;
public:
	XColorMode getColorMode()const{return m_colorMode;}
	int getWidth() const {return m_pixelsWidth;}
	int getHeight() const {return m_pixelsHeight;}
#ifdef WITH_GL_TEXTRUE
//	int getTexWidth() const {return m_cameraTexWidth;}
//	int getTexHeight() const {return m_cameraTexHeight;}
	XTexture m_pixelsTex;	//��ͼ
	unsigned int getPixelsTex() const {return m_pixelsTex.m_texture;}
	XSprite m_pixelsSprite;//���ڻ�ͼ�ľ���
#endif

	void setWork() {m_isWork = XTrue;}
	void setStop() {m_isWork = XFalse;}
	XBool getWorkState() const {return m_isWork;}

	virtual XBool init(XPixelsInputInfo &info) = 0;	//��ʼ�������豸
	virtual void release() = 0;					//�ͷ��豸�����ͷ���Դ
	virtual XBool updateFrame() = 0;				//����֡����

	virtual int getBuffSize() const{return m_buffSize;}	//���������ݵ�buff�ߴ�
	virtual void getData(unsigned char * p) const = 0;	//������������
	virtual unsigned char * getDataP() const = 0;				//��ȡ���ݵ�ָ��

	virtual void draw()	//�����������
	{
#ifdef WITH_GL_TEXTRUE
		if(!m_isInited) return;
		m_pixelsSprite.draw(&m_pixelsTex.m_texture);
#endif
	}

	XPixelsInputBasic()
		:m_isInited(XFalse)
		,m_isWork(XTrue)
	{}
	virtual ~XPixelsInputBasic(){}
};
}
#endif