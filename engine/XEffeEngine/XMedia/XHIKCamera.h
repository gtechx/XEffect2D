#ifndef _JIA_XHIKCAMERA_
#define _JIA_XHIKCAMERA_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"

#if SUPPORT_FOR_HIK
#include "XEffeEngine.h"
#include "windows.h"
#include "HIK/HCNetSDK.h"
#include "HIK/plaympeg4.h"

#pragma comment(lib, "../../engine/lib/HIK/PlayCtrl.lib")
#pragma comment(lib, "../../engine/lib/HIK/HCNetSDK.lib")

//�����������װ�ɵ���ϵͳ
class _XHIKCamera:public _XCameraBasic
{
private:
	LONG m_lUserID;
	NET_DVR_DEVICEINFO_V30 m_structDeviceInfo;
	LONG m_lRealPlayHandle;

	unsigned char *m_dataRGB;		//�����������ݵ�ָ��,������ݲ����⿪��
	_XBool m_haveNewFrame;			//�Ƿ����µ�֡
	_XBool isNewFrame()				//�Ƿ����µ�֡
	{
		_XBool ret = m_haveNewFrame;
		m_haveNewFrame = XFalse;
		return ret;
	}

	unsigned char *m_dataRGBOut;	//����Ƕ���Ľӿ�
public:
	_XBool init(_XCameraData &data);
	void getData(unsigned char * p) const
	{
		if(!m_isInited) return;
		memcpy(p,m_dataRGBOut,m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if(!m_isInited) return NULL;
		return m_dataRGBOut;
	}
	//int getBuffSize() const {return m_buffSize;}
	_XBool upDateFrame();
	//void draw();

	_XHIKCamera()
		:m_haveNewFrame(XFalse)
		,m_dataRGB(NULL)
	{}
	~_XHIKCamera() {release();}
	void release();
};

//class _XHIKCameraFactory:public _XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//������Ҫ�������ΪSingletonģʽ
//protected:
//	_XHIKCameraFactory(){}
//	_XHIKCameraFactory(const _XHIKCameraFactory&);
//	_XHIKCameraFactory &operator= (const _XHIKCameraFactory&);
//	virtual ~_XHIKCameraFactory(){} 
//public:
//	static _XHIKCameraFactory& GetInstance()
//	{
//		static _XHIKCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	_XCameraBasic * create(_XCameraData & data)
//	{
//		_XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_HIK)
//		{
//			pCamera = createMem<_XHIKCamera>();
//			if(pCamera != NULL) 
//			{
//				if(pCamera->init(data) == 0)
//				{//��ʼ��ʧ��
//					XDELETE(pCamera);
//				}
//			}
//		}
//		return pCamera;
//	}
//};

#endif	//SUPPORT_FOR_HIK

#endif //_JIA_XHIKCAMERA_