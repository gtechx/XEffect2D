#ifndef _JIA_XSHADOWMAP_
#define _JIA_XSHADOWMAP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "XShaderGLSL.h"
namespace XE{
//����һ������������Ӱ���࣬ʹ�õķ�����Shadowmap����������ٶȿ죬
//����ֻ�ܶ�����ƱȽ���Ч�����ڵ��Դ�����ã�����Ҫ�����Ե�������

#define SHADOW_MAP_TEXTURE_SIZE (1024)

class XShadowMap
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��
	XFBO m_fbo;	//��ʱ��ʹ��FBO
public:
	unsigned int getShadowMapTexture(){return m_fbo.getTexture(0);}
	XBool init();
	void updateShadowMap();	//���ݲ���ˢ��shadowmap������
	//void draw();

	void release();

	XShadowMap()
		:m_isInited(XFalse)
	{}
	~XShadowMap(){release();}
};
}
#endif