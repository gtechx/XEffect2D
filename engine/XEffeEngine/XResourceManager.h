#ifndef _JIA_XRESOURCEMANAGER_
#define _JIA_XRESOURCEMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//����һ����Դ���������࣬��������Դ�����ϲ������
#include <list>
#include "XCritical.h"

namespace XE{
//��Դ������
enum XResourceType
{
	RESOURCE_TYPE_NULL,			//��Դ������
	RESOURCE_TYPE_TEXTURE,		//��ͼ����Դ
	RESOURCE_TYPE_SOUND,		//��Ч����Դ
	RESOURCE_TYPE_MUSIC,		//��������Դ(��δʵ��)
	//�ؼ�����ԴҲ��Ҫ����Դ����������
	RESOURCE_TYPEXBUTTON_TEX,	//��ť����Դ
	RESOURCE_TYPEXCHECK_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXEDIT_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXSLIDER_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXPROGRESS_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXMULTILIST_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXCOMBO_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXDIRLIST_TEX,	//����ѡ�����Դ
	RESOURCE_TYPEXPASSWORDPAD_TEX,	//������������Դ
};
class XResourceTex
{
public:
	unsigned int m_texID;
	int m_width;
	int m_height;
	XResourceTex()
		:m_texID(0)
	{}
	~XResourceTex() {release();}
	void release();
};
class XResourceSound
{
public:
	void *m_handle;		//��Ч��Դ��ID
	XResourceSound()
		:m_handle(NULL)
	{}
	~XResourceSound(){release();}
	void release();
};
//������ָ�벻��ֱ����ָ�븳ֵ����Ҫ������Դ�������Ľӿ�����ֵ
class XResourceInfo
{
private:
	XBool m_isInited;		//��Դ�Ƿ��Ѿ�����
	void release();			//��Դ���ͷź���
public:
	int m_ID;				//��Դ��ID
	std::string m_resName;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
	XResourceType m_type;	//��Դ������
	int m_counter;			//��Դ�����ü�����

	void *m_pointer;		//��Դʵ���ָ��

	XBool load(XResPos resPos, bool isInThread = false);			//��Դ�����뺯��
	bool update(XResPos resPos, bool isInThread = false);	//������Դ
	XBool isLoaded() const { return m_isInited; }
	XResourceInfo()
		:m_isInited(XFalse)
		, m_pointer(NULL)
		, m_counter(0)
		, m_type(RESOURCE_TYPE_NULL)
	{
		static int index = 0;
		m_ID = index++;
	}
	//���ظ�ֵ���������������캯������������
	XResourceInfo(XResourceInfo &temp)
	{
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//��Դ��ID
		m_resName = temp.m_resName;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
		m_type = temp.m_type;	//��Դ������
		++m_counter;			//��Դ�����ü�����
		m_pointer = temp.m_pointer;		//��Դʵ���ָ��
	}
	~XResourceInfo()
	{
		if (m_counter <= 0) release();
	}
	XResourceInfo &operator= (const XResourceInfo & temp)
	{
		if (this == &temp) return *this;
		m_isInited = temp.m_isInited;
		m_ID = temp.m_ID;				//��Դ��ID
		m_resName = temp.m_resName;			//��Դ�����֣������ִ�Сд����Դ�ļ��Լ�·����
		m_type = temp.m_type;	//��Դ������
		++m_counter;			//��Դ�����ü�����
		m_pointer = temp.m_pointer;		//��Դʵ���ָ��
		return *this;
	}
};
//��Ҫ��Ϊ�˽��ͬ������Դ�ظ���������⣬Ŀǰֻ���ͼ���
class XResourceManager
{
public:
	XResourceManager()
		:m_resourceSum(0)
		, m_canReuse(true)
	{}
	virtual ~XResourceManager() { release(); }
protected:
	XResourceManager(const XResourceManager&);
	XResourceManager &operator= (const XResourceManager&);
private:
	XCritical m_mutex;	//ȷ�����̰߳�ȫ
	//std::vector<XResourceInfo *> m_resourceBuff;	//��Դ�Ķ�ջ
	std::list<XResourceInfo *> m_resourceBuff;
	int m_resourceSum;			//Ŀǰ��Դ������
	void release();				//�ͷ����е���Դ
	bool m_canReuse;		//�Ƿ�������Դ����
private:
	XBool isLoaded(const char * name);	//�ж�ָ����Դ�Ƿ��Ѿ�����
	XResourceInfo * isLoad(const char * name);
public:
	//�Ƿ�������Դ���ã���Դ����ʱ�����߳���Դ���±���Ҫ���⣬���ܽϵ�
	//�������Ҫ��Դ���ã�����Դ����Ч�ʽϸ�
	void setCanReuse(bool flag) { m_canReuse = flag; }
	XResourceInfo *loadResource(const char* name, XResourceType type,
		XResPos resPos, bool isInThread = false);	//����ָ����Դ
	bool updateRes(XResourceInfo* &res, const char* name,
		XResPos resPos, bool isInThread = false);
	XBool releaseResource(const XResourceInfo *p);	//�ͷ�һ����Դ
	XResourceInfo *copyResource(XResourceInfo *p)
	{
		if (!m_canReuse || p == nullptr) return nullptr;
		++p->m_counter;
		return p;
	}
};
}
#endif