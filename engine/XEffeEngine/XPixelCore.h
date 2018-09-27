#ifndef _JIA_XPIXELCORE_
#define _JIA_XPIXELCORE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include <string>
#include "XBasicFun.h"
namespace XE{
enum XColorMode;
class XPixelCore
{
protected:
	unsigned char * m_pPixels;	//��������
	int w;
	int h;
	XColorMode m_colorMode;
	bool m_isInited;
	int m_dataSize;	//�������ݵ�����
public:
	XPixelCore()
		:m_isInited(false)
		, m_pPixels(NULL)
		, m_dataSize(0)
	{}
	virtual ~XPixelCore() { release(); }
	virtual void release()
	{
		if (!m_isInited) return;
		XMem::XDELETE_ARRAY(m_pPixels);
		m_isInited = false;
	}
	virtual unsigned char *getPixel() { return m_pPixels; }
	virtual unsigned char *getPixelC()const { return m_pPixels; }
	virtual int getWidth() const { return w; }
	virtual int getHeight() const { return h; }
	virtual XColorMode getColorMode() const { return m_colorMode; }
	virtual bool setColorMode(XColorMode mode);	//�������ظ�ʽת��,����������ĺܴ������ʹ��
	virtual bool getIsInited() const { return m_isInited; }

	virtual bool load(const std::string& filename, XResPos resPos = RES_SYS_DEF) = 0;	//���ļ�������
	virtual bool load(const XBuffer &buff) = 0;	//���ڴ�����������
	virtual bool fitNPot() = 0;	//����ӦΪ2��n�η�������
	virtual bool create(int width, int height, XColorMode mode = COLOR_RGB) = 0;	//����һ���հ׵�����
	virtual int getBytePerPixel()const = 0;	//��ȡÿ�����ص��ֽ���
	virtual bool getIsFit4()const { return (w * getBytePerPixel()) % 4 == 0; }
};
template<typename T>
class XPixels
{
private:
	XPixelCore *m_pObj;
public:
	XPixels() { m_pObj = XMem::createMem<T>(); }
	~XPixels() { XMem::XDELETE(m_pObj); }
	virtual void release() { m_pObj->release(); }
	virtual unsigned char * getPixel() { return m_pObj->getPixel(); }
	virtual int getWidth() const { return m_pObj->getWidth(); }
	virtual int getHeight() const { return m_pObj->getHeight(); }
	virtual XColorMode getColorMode() const { return m_pObj->getColorMode(); }
	virtual bool getIsInited() const { return m_pObj->getIsInited(); }

	virtual bool load(const std::string& filename, XResPos resPos = RES_SYS_DEF)
	{
		return m_pObj->load(filename, resPos);
	}
	//virtual bool loaf(const unsigned char * data)
	//{
	//	return m_pObj->load(data);
	//}
	virtual bool fitNPot() { return m_pObj->fitNPot(); }	//����ӦΪ2��n�η�������
	virtual bool create(int width, int height, XColorMode mode = COLOR_RGB) { return m_pObj->create(width, height, mode); }
};
}
#endif