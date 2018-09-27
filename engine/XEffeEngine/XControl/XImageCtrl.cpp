#include "XStdHead.h"
#include "XImageCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
XBool XImageCtrl::init(const char *filename)			//���ļ��ж�ȡͼƬ
{
	if(m_isInited ||
		filename == NULL) return false;
	m_pTexture = XMem::createMem<XTextureData>();
	if(m_pTexture == NULL ||
		!m_pTexture->load(filename)) return XFalse;
	m_sprite.init(*m_pTexture,POINT_LEFT_TOP);
	m_w = m_pTexture->textureSize.x;
	m_h = m_pTexture->textureSize.y;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_onlyWithTexture = false;
	m_isEnable = XFalse;
	m_isVisible = m_isActive = XTrue;
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XImageCtrl::init(int w,int h,unsigned int tex,bool needResize)
{
	if(m_isInited || w <= 0 || h <= 0) return XFalse;
	if(needResize) m_sprite.init(w,h,1,POINT_LEFT_TOP);
	else m_sprite.init(w,h,0,POINT_LEFT_TOP);
	m_w = w;
	m_h = h;
	m_texID = tex;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_onlyWithTexture = true;
	m_isEnable = XFalse;
	m_isVisible = m_isActive = XTrue;
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XImageCtrl::init(int w,int h,XColorMode mode)	//�����յ�ͼƬ
{
	if(m_isInited ||
		w <= 0 || h <= 0) return XFalse;
	m_pTexture = XMem::createMem<XTextureData>();
	if(m_pTexture == NULL ||
		!m_pTexture->createTexture(w,h,mode)) return XFalse;
	m_sprite.init(*m_pTexture,POINT_LEFT_TOP);
	m_pixelSum = w * h;
	m_w = w;
	m_h = h;
	//������Ҫ�����ݽ��г�ʼ��(��δ���)
	m_pTexture->reset();
	m_onlyWithTexture = false;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_isEnable = XFalse;
	m_isVisible = m_isActive = XTrue;
	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XImageCtrl::release()
{
	if(!m_isInited) return;
	m_sprite.release();
	XMem::XDELETE(m_pTexture);
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif	
	m_isInited = false;
}
#if !WITH_INLINE_FILE
#include "XImageCtrl.inl"
#endif
}