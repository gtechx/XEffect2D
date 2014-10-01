#ifndef _JIA_XDIRECTORY_
#define _JIA_XDIRECTORY_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.13
//--------------------------------
//����һ��Ŀǰֻ֧��win32�Ļ���ϵͳ�����Ŀ¼������࣬�����Լ���չ�Զ�����
#include "windows.h"
#include <vector>
#include "XOSDefine.h"

class _XDir;
class _XFile
{
public:
	_XBool isEnable;		//�Ƿ���Ч
	char * allPath;		//ȫ·����
	char * filename;	//�ļ���
	_XBool isDirectory;	//�Ƿ���Ŀ¼
	_XFile()
		:isEnable(XFalse)
		,directory(NULL)
		,allPath(NULL)
		,filename(NULL)
	{}
	_XDir * directory;	//�����Ŀ¼�Ļ�����������һ��Ŀ¼����Ϣ
	void release();
};
class _XDir
{
public:
	_XBool isEnable;				//Ŀ¼�Ƿ���Ч
	char * directoryName;		//Ŀ¼
	std::vector<_XFile *> files;		//Ŀ¼�µ��ļ�(��)
	_XDir()
		:isEnable(XFalse)
		,level(0)
		,directoryName(NULL)
	{}
	void release();
	int level;			//·���㼶,��ͼ��������ı���[0 - n]
};
class _XDirectory
{
private:
	_XBool m_isInited;		//�Ƿ��ʼ��
public:
	_XDir m_nowDirectory;	//��ǰ��Ŀ¼��Ϣ
	_XBool init(const char *directoryName);	//ʹ�þ���·��
	_XBool initEx(const char *directoryName);	//ʹ�����·��������ڹ���·��

	_XDirectory()
		:m_isInited(XFalse)
	{}
	~_XDirectory(){release();}
	void release()
	{//�ͷ����е���Դ
		if(!m_isInited) return;
		m_nowDirectory.release();
		m_isInited = XFalse;
	}
};
extern void infoConvert(WIN32_FIND_DATA &fileInfo,_XFile * file);
extern _XBool enumerationFiles(const char *path,_XDir * dir);		//ʹ�þ���·��,����Ŀ¼�µ��ļ���Ϣ��������Ŀ¼
extern _XBool enumerationFilesEx(const char *path,_XDir * dir);	//ʹ�����·��,����Ŀ¼�µ��ļ���Ϣ��������Ŀ¼

#pragma comment(lib, "Shlwapi.lib")
extern _XBool isExistFile(const char *path);		//�жϾ���·��������ļ������ļ����Ƿ����
extern _XBool isExistFileEx(const char *path);	//�ж����·��������ļ������ļ����Ƿ����
extern _XBool isFolder(const char * path);		//�ж�Ŀ���Ƿ�Ϊ�ļ���
extern _XBool isFolderEx(const char * path);		//�ж�Ŀ���Ƿ�Ϊ�ļ���
extern _XBool deleteFile(const char *filename);	//ɾ��ָ���ļ�
#endif