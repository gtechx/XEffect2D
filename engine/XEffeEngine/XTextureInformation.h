#ifndef _JIA_XALLTEXTURE_
#define _JIA_XALLTEXTURE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XOSDefine.h"
#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"

#include "stdio.h"
#include <memory>
#include "string.h"
using namespace std;

#define TEX_INFO_DEBUG (0)

class _XTextureData	//��ͼ����,ֻ������ӿڶ���
{
private:
	_XBool m_isInited;
	int m_ID;
public:
	void setNoInited()		//�������û�г�ʼ��
	{//��������Դ�Ͳ��ᱻ�ͷŵ�
		m_isInited = XFalse;
	}
	_XTexture texture;		//ָ����ͼ��ָ��
	_XVector2 textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	_XVector2 textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	char isEnableInsideClip;			//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	_XRect clipInsideRect;		//�ڲ����е�����
	_XVector2 textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�
	void updateTexture(const _XPBO &pbo){texture.updateTexture(pbo);}
	void updateTextureR2B(const _XPBO &pbo){texture.updateTextureR2B(pbo);}
	void updateTexture(const void * p){texture.updateTexture(p);}
	void updateTextureR2B(const void * p){texture.updateTextureR2B(p);}
	_XTextureData()
		:textureMove(0.0f,0.0f)
		//,pTexture(NULL)
		,textureMove2(0.0f,0.0f)
		,isEnableInsideClip(-1)
		,clipInsideRect(0,0,1,1)
		,m_isInited(XFalse)
		,textureSize(0.0f,0.0f)
	{
		static int id = 0;
		m_ID = id ++;
	}
	//���������ظ��ͷŵ�����
	~_XTextureData(){release();}
	int getID() const {return m_ID;}	//�����ж��Ƿ�ͬһĿ��
	_XBool load(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool loadEmpty();
	_XBool createWithTexture(int w,int h,_XColorMode colorMode,unsigned int tex);	//colorMode�Ĳ����ο�texture������ʱ��Ĳ���˵��
	//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ
	_XBool createTexture(int w,int h,_XColorMode colorMode);
	void release();
	_XBool setACopy(const _XTextureData& temp);
	_XTextureData& operator = (const _XTextureData& temp);
	void bind() {texture.bind();}
	_XBool reset() {return texture.reset();}//��ʼ����ͼ
private:
	_XTextureData(const _XTextureData& temp);	//��ֹ��Ϊ���ÿ������캯������ɴ���
};
inline _XTextureData * createATextureData(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE)
{
	_XTextureData *ret = createMem<_XTextureData>();
	if(ret == NULL) return NULL;
	if(!ret->load(filename,resoursePosition))
	{
		XDELETE(ret);
		return NULL;
	}
	return ret;
}
#if TEX_INFO_DEBUG
extern int texInfoSum;
#endif
class _XTextureInfo	//��������ӿ���_XTextureData�ӿںϲ�������ӿ�ֻ����
{
private:
	_XSCounter *m_cp;
public:
	char *textureName;			//��Դ�ļ�������
	//char *targetFileName;		//Ŀ���ļ�������
	//int targetOrder;			//Ŀ����ļ����
	_XTexture texture;		//ָ����ͼ��ָ��
	_XVector2 textureOffset;	//��Դ��Ŀ���ļ��е�ƫ��λ��
	_XVector2 textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	_XVector2 textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	_XVector2 textureSize;		//��ͼ�ĳߴ�

	_XTextureInfo()
	:textureOffset(0.0f,0.0f)
	,textureMove(0.0f,0.0f)
	,textureMove2(0.0f,0.0f)
	,textureSize(0.0f,0.0f)
	//,targetOrder(0)
	{
		m_cp = new _XSCounter;
		textureName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		textureName[0] = '\0';
	//	targetFileName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//	targetFileName[0] = '\0';
#if TEX_INFO_DEBUG
		++ texInfoSum;
		printf("%d-\n",texInfoSum);
#endif
	}
	~_XTextureInfo()
	{
		if(m_cp != NULL && --m_cp->m_counter <= 0)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(textureName);
		//	XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n",texInfoSum);
#endif
		}
	}
	//�ض��忽�����캯��
	_XTextureInfo(const _XTextureInfo & temp)
	{
		if(this == &temp) return;		//��ֹ��������
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		m_cp = temp.m_cp;
		texture = temp.texture;
		textureOffset = temp.textureOffset;
		textureMove = temp.textureMove;
		textureMove2 = temp.textureMove2;
		textureSize = temp.textureSize;
		textureName = temp.textureName;
	//	targetFileName = temp.targetFileName;
	//	targetOrder = temp.targetOrder;
	}
	//�ض��帳ֵ������
	_XTextureInfo& operator = (const _XTextureInfo& temp)
	{
		if(& temp == this) return * this;	//��ֹ����ֵ
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			XDELETE(m_cp);
			XDELETE_ARRAY(textureName);
		//	XDELETE_ARRAY(targetFileName);
#if TEX_INFO_DEBUG
			-- texInfoSum;
			printf("%d-\n",texInfoSum);
#endif
		}
		m_cp = temp.m_cp;
		texture = temp.texture;
		textureOffset = temp.textureOffset;
		textureMove = temp.textureMove;
		textureMove2 = temp.textureMove2;
		textureSize = temp.textureSize;
		textureName = temp.textureName;
	//	targetFileName = temp.targetFileName;
	//	targetOrder = temp.targetOrder;
		return * this;
	}
};
//�����������һ������ϵͳ�����Բ������ظ�ֵ���������߸��ƹ��캯��
class _XAllTexture
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XAllTexture();
	_XAllTexture(const _XAllTexture&);
	_XAllTexture &operator= (const _XAllTexture&);
	virtual ~_XAllTexture(); 
public:
	static _XAllTexture& GetInstance();
	//-------------------------------------------
private:
	_XResourcePosition m_resoursePosition;				//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	_XBool m_isInited;	//�Ƿ�����˳�ʼ��
	_XBool m_isEnable;	//�Ƿ���Ч

	int m_textureSum;		//ԭʼ��ͼ������
	int m_targetTextureSum;	//��������֮���Ŀ����ͼ����

	_XTextureInfo *m_texInfo;	//����ԭʼ��ͼ��Ϣ
	_XTexture *m_pTexture;		//����Ŀ����ͼ����Ϣ��Ŀ����ͼ����ԭʼ��ͼ�����Ż�֮�����

public:
	_XBool init(_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��ʼ������
	_XBool getTexture(const char *temp,_XTextureInfo &texInfo);		//��ȡ��ͼ����Ϣ

	void release();
};

//��ԭ��ͼ��p���Ż�֮�����ͼ�ļ������ݣ�length��p�����ݳ��ȣ�texInfo���Ż�֮ǰ���ļ���Ϣ�Լ��Ż�������
extern _XBool reductionTexture(const unsigned char *p,int length,const _XTextureInfo &texInfo);

#endif