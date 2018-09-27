#ifndef _JIA_XALLTEXTURE_
#define _JIA_XALLTEXTURE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#include "XCommonDefine.h"
#include "XMath\XRect.h"
#include "XBasicOpenGL.h"
#include "XPixelCommon.h"
#include <vector>
namespace XE {
#define TEX_INFO_DEBUG (0)

class XTextureData	//��ͼ����,ֻ������ӿڶ���
{
private:
	int m_ID;
	XBool m_isInited;
public:
	void setIsThreadEvn(bool flag) { texture.setIsThreadEvn(flag); }
	bool getIsThreadEvn() const { return texture.getIsThreadEvn(); }
	unsigned int getTexGLID()const { return texture.m_texture; }
	char isEnableInsideClip;	//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	XRect clipInsideRect;		//�ڲ����е�����

	void setNoInited() { m_isInited = XFalse; }//�������û�г�ʼ��,��������Դ�Ͳ��ᱻ�ͷŵ�
	XTexture texture;		//ָ����ͼ��ָ��
	XVec2 textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	XVec2 textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	XVec2 textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�

	int getOpWidth()const
	{
		if (isEnableInsideClip != 0)
			return clipInsideRect.getWidth();
		else
			return texture.m_w;
	}	//��ȡ�Ż�֮ǰ�Ŀ��
	int getOpHeight()const
	{
		if (isEnableInsideClip != 0)
			return clipInsideRect.getHeight();
		else
			return texture.m_h;
	}	//��ȡ�Ż�֮ǰ�ĸ߶�
	//++++++++++++++++++++++++++++++++++++++
	//������ͼ��������ڲ��ü������⣬�����������һ��ת�����������ڲ��ü���ɵ�����
	XVec2 getCoord(const XVec2& p)//pΪ��ͼ��Ч�����ڵĵ�,�������ڲ��ü���Ĺ�һ����ͼ����
	{
		if (isEnableInsideClip != 0)
		{
			return XVec2((p.x + clipInsideRect.left) / texture.m_w,
				(p.y + clipInsideRect.top) / texture.m_h);
		}
		else
			return p / textureSize;
	}
	XVec2 getCoordNormalized(const XVec2& p)//pΪ��ͼ��Ч�����ڵĹ�һ���ĵ�,�������ڲ��ü���Ĺ�һ����ͼ����
	{
		if (isEnableInsideClip != 0)
		{
			return XVec2((p.x * (textureSize.x - 1.0f) + clipInsideRect.left + 1.0f) / texture.m_w,
				(p.y * (textureSize.y - 1.0f) + clipInsideRect.top + 1.0f) / texture.m_h);
		}
		else
			return p;
	}
	//--------------------------------------
	void updateTexture(const XPBO &pbo) { texture.updateTexture(pbo); }
	void updateTextureR2B(const XPBO &pbo) { texture.updateTextureR2B(pbo); }
	void updateTexture(const void * p);
	void updateTextureR2B(const void * p);
	void updateTextureEx(const void * p);		//������ʹ���������������ĳЩ�����������ܷ����Ϻ�
	void updateTextureR2BEx(const void * p);	//������ʹ���������������ĳЩ�����������ܷ����Ϻ�
	XTextureData()
		:textureMove(0.0f)
		//,pTexture(NULL)
		, textureMove2(0.0f)
		, isEnableInsideClip(-1)
		, clipInsideRect(0, 0, 1, 1)
		, m_isInited(XFalse)
		, textureSize(0.0f)
	{
		static int id = 0;
		m_ID = id++;
	}
	//���������ظ��ͷŵ�����
	~XTextureData() { release(); }
	int getID() const { return m_ID; }	//�����ж��Ƿ�ͬһĿ��
	XBool load(const char * filename,
		XResPos resPos = RES_SYS_DEF)
	{
		if (m_isInited) return XTrue;
		if (!reload(filename, resPos)) return false;
		m_isInited = XTrue;
		return XTrue;
	}
	XBool reload(const char * filename,
		XResPos resPos = RES_SYS_DEF);
	XBool loadEmpty();
	XBool createWithTexture(int w, int h, XColorMode colorMode, unsigned int tex);	//colorMode�Ĳ����ο�texture������ʱ��Ĳ���˵��
	//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ,�������ڲ����ݸ�ʽʱ��ΪĬ�ϸ�ʽ
	XBool createTexture(int w, int h, XColorMode colorMode, int internalFormat = -1);
	XBool createFromPixels(const XPixelCore &p)
	{
		if (!p.getIsInited()) return false;
		if (p.getIsFit4())
		{
			if (!createTexture(p.getWidth(), p.getHeight(), p.getColorMode())) return false;
			updateTexture(p.getPixelC());
		}
		else
		{//����OpenGL��Ҫ���ֽӶ��룬����������ڲ��������ֽӶ���ģ��������ת��
			int w = 4 - (p.getWidth() % 4);
			w += p.getWidth();
			int h = p.getHeight();
			unsigned char *tmpPixels = XMem::createArrayMem<unsigned char>(w * h * p.getBytePerPixel());
			if (tmpPixels == NULL) return false;
			memset(tmpPixels, 0, w * h * p.getBytePerPixel());
			//�����������
			int w1 = w * p.getBytePerPixel();
			int w2 = p.getWidth() * p.getBytePerPixel();
			for (int i = 0, index1 = 0, index2 = 0; i < h; ++i, index1 += w1, index2 += w2)
			{
				memcpy(tmpPixels + index1, p.getPixelC() + index2, w1);
			}
			if (!createTexture(w, h, p.getColorMode()))
			{
				XMem::XDELETE_ARRAY(tmpPixels);
				return false;
			}
			updateTexture(tmpPixels);
			XMem::XDELETE_ARRAY(tmpPixels);
		}
		return true;
	}
	bool getIsInited()const { return m_isInited; }
	XBool setACopy(const XTextureData& temp);
	XTextureData& operator = (const XTextureData& temp);
	void bind() { texture.bind(); }
	XBool reset() { return texture.reset(); }//��ʼ����ͼ
private:
	void release();
	bool loadFromFolder(const char *filename, XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename) { return loadFromFolder(filename, RES_WEB);}	//����ҳ�ж�ȡ��Դ
//��������ò���Ѿ���ʱ
//	XTextureData(const XTextureData& temp);	//��ֹ��Ϊ���ÿ������캯������ɴ���
};
inline XTextureData * createATextureData(const char * filename, XResPos resPos = RES_SYS_DEF)
{
	XTextureData *ret = XMem::createMem<XTextureData>();
	if (ret == NULL) return NULL;
	if (!ret->load(filename, resPos))
	{
		XMem::XDELETE(ret);
		return NULL;
	}
	return ret;
}
#if TEX_INFO_DEBUG
extern int texInfoSum;
#endif
class XTextureInfo	//��������ӿ���XTextureData�ӿںϲ�������ӿ�ֻ����
{
private:
	XSCounter *m_cp;
public:
	char *textureName;			//��Դ�ļ�������
	//char *targetFileName;		//Ŀ���ļ�������
	//int targetOrder;			//Ŀ����ļ����
	XTexture texture;		//ָ����ͼ��ָ��
	XVec2 textureOffset;	//��Դ��Ŀ���ļ��е�ƫ��λ��
	XVec2 textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	XVec2 textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	XVec2 textureSize;		//��ͼ�ĳߴ�

	XTextureInfo()
		:textureOffset(0.0f)
		, textureMove(0.0f)
		, textureMove2(0.0f)
		, textureSize(0.0f)
		//,targetOrder(0)
	{
		m_cp = new XSCounter;
		textureName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		textureName[0] = '\0';
		//	targetFileName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		//	targetFileName[0] = '\0';
#if TEX_INFO_DEBUG
		++ texInfoSum;
		printf("%d-\n", texInfoSum);
#endif
	}
	~XTextureInfo()
	{
		if (m_cp != NULL && --m_cp->m_counter <= 0)
		{
			XMem::XDELETE(m_cp);
			XMem::XDELETE_ARRAY(textureName);
			//	XMem::XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n", texInfoSum);
#endif
		}
	}
	//�ض��忽�����캯��
	XTextureInfo(const XTextureInfo & temp);
	//�ض��帳ֵ������
	XTextureInfo& operator = (const XTextureInfo& temp);
};
//��Դ������Ϣ
struct XTxtureResPackInfo
{
	std::string m_resName;	//��Դ������
	std::vector<XTextureInfo> m_srcTexInfo;		//ԭʼͼƬ����Ϣ
	std::vector<XTexture> m_dstTexInfo;			//Ŀ��ͼƬ����Ϣ
};
//�����������һ������ϵͳ�����Բ������ظ�ֵ���������߸��ƹ��캯��
class XTextureResourceManager
{
public:
	XTextureResourceManager();
	virtual ~XTextureResourceManager() { release(); }
protected:
	XTextureResourceManager(const XTextureResourceManager&);
	XTextureResourceManager &operator= (const XTextureResourceManager&);
private:
	//XResPos m_resoursePosition;				//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	XBool m_isInited;	//�Ƿ�����˳�ʼ��
	XBool m_isEnable;	//�Ƿ���Ч
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�����������
	std::vector<XTxtureResPackInfo> m_texResInfos;	//���е���ͼ��Դ
	//-------------------------------------------------
	bool loadFromFolder(const char *filename, XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename, XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename) { return false; }	//����ҳ�ж�ȡ��Դ

public:
	XBool init(XResPos resPos = RES_SYS_DEF);	//��ʼ������
	XBool getTexture(const char *temp, XTextureInfo &texInfo);		//��ȡ��ͼ����Ϣ
	XBool addATexResPack(const char *filename, XResPos resPos = RES_SYS_DEF);	//�������������һ����Դ��
	XBool releaseATexResPack(const char *filename);	//�ͷ�һ����Դ��
	XBool isLoaded(const char *filename);	//�ж���Դ�Ƕ��Ѿ�����

	void release();
};
//��ԭ��ͼ��p���Ż�֮�����ͼ�ļ������ݣ�length��p�����ݳ��ȣ�texInfo���Ż�֮ǰ���ļ���Ϣ�Լ��Ż�������
extern XBool reductionTexture(const void *p, int length, const XTextureInfo &texInfo);
#if WITH_INLINE_FILE
#include "XTextureInformation.inl"
#endif
}
#endif