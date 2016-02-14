#ifndef _JIA_XCAMERAFACTORY
#define _JIA_XCAMERAFACTORY
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"
namespace XE{
class XCameraFactory
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	XCameraFactory(){}
	XCameraFactory(const XCameraFactory&);
	XCameraFactory &operator= (const XCameraFactory&);
	virtual ~XCameraFactory(){} 
public:
	static XCameraFactory& GetInstance()
	{
		static XCameraFactory m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	template<typename T>
	T * create(XCameraInfo & data)
	{
		T *pCamera = NULL;
		pCamera = XMem::createMem<T>();
		if(pCamera != NULL) 
		{
			if(!pCamera->init(data)) XMem::XDELETE(pCamera);//��ʼ��ʧ��
		}
		return pCamera;
	}
};
}
#endif //_JIA_XCAMERAFACTORY
