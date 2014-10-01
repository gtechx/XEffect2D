#ifndef _JIA_XRESOURCEMANAGER_
#define _JIA_XRESOURCEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//����һ����Դ���������࣬��������Դ�����ϲ������
//#include <vector>
#include <list>
#include "XBasicFun.h"
#include "XSoundCommon.h"
//��Դ������
enum _XResourceType
{
	RESOURCE_TYPE_NULL,			//��Դ������
	RESOURCE_TYPE_TEXTURE,		//��ͼ����Դ
	RESOURCE_TYPE_SOUND,		//��Ч����Դ
	RESOURCE_TYPE_MUSIC,		//��������Դ(��δʵ��)
	//�ؼ�����ԴҲ��Ҫ����Դ����������
	RESOURCE_TYPE_XBUTTON_TEX,	//��ť����Դ
	RESOURCE_TYPE_XCHECK_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XEDIT_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XSLIDER_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XPROGRESS_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XMULTILIST_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XCOMBO_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XDIRLIST_TEX,	//����ѡ�����Դ
	RESOURCE_TYPE_XPASSWORDPAD_TEX,	//������������Դ
};
class _XResourceTex
{
public:
	unsigned int m_texID;
	int m_width;
	int m_height;
	_XResourceTex()
		:m_texID(0)
	{}
	~_XResourceTex() {release();}
	void release();
};
class _XResourceSound
{
public:
	void *m_handle;		//��Ч��Դ��ID
	_XResourceSound()
		:m_handle(NULL)
	{}
	~_XResourceSound(){release();}
	void release()
	{
		if(m_handle != NULL)
		{
			_XSoundHandle.clearSound(m_handle);
			m_handle = NULL;
		}
	}
};
//������ָ�벻��ֱ����ָ�븳ֵ����Ҫ������Դ�������Ľӿ�����ֵ
class _XResourceInfo
{
private:
	_XBool m_isInited;		//��Դ�Ƿ��Ѿ�����
	void release();			//��Դ���ͷź���
public:
	int m_ID;				//��Դ��ID
	char *m_name;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
	_XResourceType m_type;	//��Դ������
	int m_counter;			//��Դ�����ü�����

	void *m_pointer;		//��Դʵ���ָ��

	_XBool load(_XResourcePosition resoursePosition);			//��Դ�����뺯��
	_XBool isLoaded() const {return m_isInited;}
	_XResourceInfo()
		:m_isInited(XFalse)
		,m_name(NULL)
		,m_pointer(NULL)
		,m_counter(0)
		,m_type(RESOURCE_TYPE_NULL)
	{
		static int index = 0;
		m_ID = index ++;
	}
	//���ظ�ֵ���������������캯������������
	_XResourceInfo(_XResourceInfo &temp)
	{
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//��Դ��ID
		m_name = temp.m_name;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
		m_type = temp.m_type;	//��Դ������
		++ m_counter;			//��Դ�����ü�����
		m_pointer = temp.m_pointer;		//��Դʵ���ָ��
	}
	~_XResourceInfo()
	{
		if(m_counter <= 0) release();
	}
	_XResourceInfo &operator= (const _XResourceInfo & temp)
	{
		if(this == & temp) return * this;
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//��Դ��ID
		m_name = temp.m_name;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
		m_type = temp.m_type;	//��Դ������
		++ m_counter;			//��Դ�����ü�����
		m_pointer = temp.m_pointer;		//��Դʵ���ָ��
		return * this;
	}
};
//��Ҫ��Ϊ�˽��ͬ������Դ�ظ���������⣬Ŀǰֻ���ͼ���
class _XResourceManager	
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XResourceManager()
		:m_resourceSum(0)
	{
	}
	_XResourceManager(const _XResourceManager&);
	_XResourceManager &operator= (const _XResourceManager&);
	virtual ~_XResourceManager()
	{
		release();
	}
public:
	static _XResourceManager& GetInstance()
	{
		static _XResourceManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	//std::vector<_XResourceInfo *> m_resourceBuff;	//��Դ�Ķ�ջ
	std::list<_XResourceInfo *> m_resourceBuff;
	int m_resourceSum;			//Ŀǰ��Դ������
	void release();				//�ͷ����е���Դ
private:
	_XBool isLoaded(const char * name);	//�ж�ָ����Դ�Ƿ��Ѿ�����
	_XResourceInfo * isLoad(const char * name);
public:
	_XResourceInfo *loadResource(const char * name,_XResourceType type,_XResourcePosition resoursePosition);	//����ָ����Դ
	_XBool releaseResource(const _XResourceInfo *p);	//�ͷ�һ����Դ
	_XResourceInfo *copyResource(_XResourceInfo *p)
	{
		if(p != NULL) ++ p->m_counter;
		return p;
	}
};

#define _XResourceMng _XResourceManager::GetInstance()

#endif