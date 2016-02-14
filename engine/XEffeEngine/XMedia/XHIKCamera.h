#ifndef _JIA_XHIKCAMERA_
#define _JIA_XHIKCAMERA_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"
#include "../XCritical.h"
#if SUPPORT_FOR_HIK
#include "HIK/HCNetSDK.h"
#include "HIK/plaympeg4.h"
//����ʹ�õİ汾��
//#define VERSION_30
#define VERSION_40

#pragma comment(lib, "../../engine/lib/HIK/PlayCtrl.lib")
#pragma comment(lib, "../../engine/lib/HIK/HCNetSDK.lib")
#pragma comment(lib, "../../engine/lib/HIK/DsSdk.lib")
#pragma comment(lib, "../../engine/lib/HIK/GdiPlus.lib")
namespace XE{
#define HIK_WITH_LOECORE (1)	//��ϵͳ���ܲ���ʱ������������Ϊ1�Ա���ʹ��ģ������������
//�����������װ�ɵ���ϵͳ
class XHIKCamera:public XCameraBasic
{
private:
	bool m_isGetInitData;
	void setInitData(int w,int h);
	LONG m_lUserID;
	NET_DVR_DEVICEINFO_V30 m_structDeviceInfo;
	LONG m_lRealPlayHandle;
	LONG m_lPort;

	unsigned char *m_dataRGB;		//�����������ݵ�ָ��,������ݲ����⿪��
	XBool m_haveNewFrame;			//�Ƿ����µ�֡
	XBool isNewFrame()				//�Ƿ����µ�֡
	{
		XBool ret = m_haveNewFrame;
		m_haveNewFrame = XFalse;
		return ret;
	}

	//unsigned char *m_dataRGBOut;	//����Ƕ���Ľӿ�
	static void CALLBACK decCBFun(long,char *,long,FRAME_INFO *, long,long);
#ifdef VERSION_40
	static void CALLBACK realDataCB(LONG, DWORD, BYTE *,DWORD ,void* );
#endif
#ifdef VERSION_30
	static void CALLBACK realDataCB(LONG, DWORD, BYTE *,DWORD ,DWORD );
#endif
	static void CALLBACK exceptionCB(DWORD, LONG, LONG, void *);
#if HIK_WITH_LOECORE
	unsigned char *m_yuvData;	//������ʱ�洢YUV����
	XCritical m_mutex;			//�߳���
#endif
public:
	XBool init(XCameraInfo &data);
	void getData(unsigned char * p) const
	{
		if(!m_isInited) return;
		memcpy(p,m_dataRGB,m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if(!m_isInited) return NULL;
		return m_dataRGB;
	}
	//int getBuffSize() const {return m_buffSize;}
	XBool updateFrame();
	//void draw();

	XHIKCamera()
		:m_haveNewFrame(XFalse)
		,m_dataRGB(NULL)
		,m_isGetInitData(false)
#if HIK_WITH_LOECORE
		,m_yuvData(NULL)
#endif
	{}
	~XHIKCamera() {release();}
	void release();
};

//class XHIKCameraFactory:public XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//������Ҫ�������ΪSingletonģʽ
//protected:
//	XHIKCameraFactory(){}
//	XHIKCameraFactory(const XHIKCameraFactory&);
//	XHIKCameraFactory &operator= (const XHIKCameraFactory&);
//	virtual ~XHIKCameraFactory(){} 
//public:
//	static XHIKCameraFactory& GetInstance()
//	{
//		static XHIKCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	XCameraBasic * create(XCameraInfo & data)
//	{
//		XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_HIK)
//		{
//			pCamera = XMem::createMem<XHIKCamera>();
//			if(pCamera != NULL) 
//			{
//				if(pCamera->init(data) == 0)
//				{//��ʼ��ʧ��
//					XMem::XDELETE(pCamera);
//				}
//			}
//		}
//		return pCamera;
//	}
//};
}
#endif	//SUPPORT_FOR_HIK

#endif //_JIA_XHIKCAMERA_
