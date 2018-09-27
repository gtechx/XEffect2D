#ifndef _JIA_XCAMERAFACTORY
#define _JIA_XCAMERAFACTORY
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"
namespace XE{
class XPixelsInputFactory
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	XPixelsInputFactory(){}
	XPixelsInputFactory(const XPixelsInputFactory&);
	XPixelsInputFactory &operator= (const XPixelsInputFactory&);
	virtual ~XPixelsInputFactory(){} 
public:
	static XPixelsInputFactory& GetInstance()
	{
		static XPixelsInputFactory m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	template<typename T>
	T * create(XPixelsInputInfo & data)
	{
		T *pPixelsInput = NULL;
		pPixelsInput = XMem::createMem<T>();
		if(pPixelsInput != NULL && !pPixelsInput->init(data))
			XMem::XDELETE(pPixelsInput);//��ʼ��ʧ��
		return pPixelsInput;
	}
};
}
#endif //_JIA_XCAMERAFACTORY
