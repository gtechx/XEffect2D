#ifndef _JIA_XCAMERA_
#define _JIA_XCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
//����һ����ͨ����ͷ��Ƶ������ȡ���࣬����videoInput
#include "XCameraBasic.h"
#include "videoInput.h"
//ע���ڰ���opencv��ʱ����Ҫ�ٰ�������⣬���������ظ���������Ϊopencv���Ѿ����������
//#ifdef _DEBUG
//#ifdef _WIN64
//#pragma comment(lib, "videoInputD64.lib")
//#else
//#pragma comment(lib, "videoInputD.lib")
//#endif
//#else
//#ifdef _WIN64
//#pragma comment(lib, "videoInput64.lib")	//��opencv_videoio300.lib�г�ͻ
//#else
//#pragma comment(lib, "videoInput.lib")	//��opencv_videoio300.lib�г�ͻ
//#endif
//#endif

#pragma comment(linker, "/NODEFAULTLIB:atlthunk.lib")
#include <atlbase.h>

#if (_ATL_VER < 0x0700)
namespace ATL
{
  inline void * __stdcall __AllocStdCallThunk()
  {
  return ::HeapAlloc(::GetProcessHeap(), 0, sizeof(_stdcallthunk));
  }

  inline void __stdcall __FreeStdCallThunk(void *p)
  {
  ::HeapFree(::GetProcessHeap(), 0, p);
  }
};
#endif //(_ATL_VER < 0x0700)
namespace XE{
class XCamera :public XPixelsInputBasic
{
private:
	videoInput m_VI;		//����ͷ���豸ָ��
	int m_deviceSum;		//����ͷ�豸������
	int m_deviceOrder;		//����ͷ�豸�ı��
	unsigned char *m_frameDataBuff1;//������������ҷ�ת������
	unsigned char *m_frameDataBuff;	//����ͷÿ֡������
	unsigned char *m_texDataBuff;	//��ͼ������
	int m_px;
	int m_py;
	XBool m_isUp2Down;		//�Ƿ����·�ת
	XBool m_isLeft2Right;	//�Ƿ����ҷ�ת
	XBool m_r2b;			//�Ƿ���Ҫ����ɫ��ת�����ȷ�ϲ���Ҫʶ����ɫ�Ļ������Բ���Ҫ��ô��
public:
	void setR2B(XBool f) { m_r2b = f; }
	bool getNeedR2B()const { return m_r2b; }
	void setUp2Down(XBool flag) { m_isUp2Down = flag; }
	void setLeft2Right(XBool flag) { m_isLeft2Right = flag; }

	void getData(unsigned char * p) const
	{
		if (!m_isInited) return;
		memcpy(p, m_frameDataBuff, m_buffSize);
	}
	unsigned char * getDataP() const
	{
		if (!m_isInited) return NULL;
		return m_frameDataBuff;
	}
	//int getBuffSize() const {return m_buffSize;}
	XBool init(XPixelsInputInfo &data);
	XBool updateFrame();
	bool resart()
	{
		if (!m_isInited) return false;
		return m_VI.restartDevice(m_deviceOrder);
	}
	//void draw();
	XCamera()
		:m_frameDataBuff1(NULL)
		, m_frameDataBuff(NULL)
		, m_texDataBuff(NULL)
		, m_isUp2Down(XFalse)
		, m_isLeft2Right(XFalse)
		, m_updateTimer(0.0f)
		, m_needReset(false)
		, m_px(0)
		, m_py(0)
		, m_r2b(XTrue)
	{}
	~XCamera(){release();}
	void release();
	void showDeviceSetting()
	{
		if(m_isInited) m_VI.showSettingsWindow(m_deviceOrder);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǹ��ڱ������������ط���
	bool reset();
	bool needReset()
	{
		if(!m_needReset) return false;
		return m_updateTimer > 1.0f;	//�����1s���ݶ�û�и�������Ϊ�豸���ֹ���
	}
private:
	float m_updateTimer;	//���µ�ʱ��
	XPixelsInputInfo m_curInfo;
	bool m_needReset;	//�Ƿ���Ҫ��������
	//-----------------------------------------------
};

//class XCameraFactory:public XCameraBaiscFactory
//{
//	//+++++++++++++++++++++++++++++++++++++++++++
//	//������Ҫ�������ΪSingletonģʽ
//protected:
//	XCameraFactory(){}
//	XCameraFactory(const XCameraFactory&);
//	XCameraFactory &operator= (const XCameraFactory&);
//	virtual ~XCameraFactory(){} 
//public:
//	static XCameraFactory& GetInstance()
//	{
//		static XCameraFactory m_instance;
//		return m_instance;
//	}
//	//-------------------------------------------
//public:
//	XCameraBasic * create(XCameraInfo & data)
//	{
//		XCameraBasic *pCamera = NULL;
//		if(data.cameraType == CAMERA_TYPE_NORMAL)
//		{
//			pCamera = XMem::createMem<XCamera>();
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
#endif