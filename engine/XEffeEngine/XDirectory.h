#ifndef _JIA_XDIRECTORY_
#define _JIA_XDIRECTORY_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.13
//--------------------------------
//����һ��Ŀǰֻ֧��win32�Ļ���ϵͳ�����Ŀ¼������࣬�����Լ���չ�Զ�����
//�Ż���1������ָ����˳������
//2������ָ���ĺ�׺��
//3���Ƿ�������ļ��������ÿ���
//#include "windows.h"
#include <vector>
#include "XOSDefine.h"
namespace XE{
class XDir;
class XFileInfo
{
public:
	XBool isEnable;	//�Ƿ���Ч
//	char * allPath;		//ȫ·����
//	char * filename;	//�ļ���
	std::string allPath;
	std::string filename;
	XBool isDirectory;	//�Ƿ���Ŀ¼
	XFileInfo()
		:isEnable(XFalse)
		,directory(NULL)
//		,allPath(NULL)
//		,filename(NULL)
	{}
	XDir * directory;	//�����Ŀ¼�Ļ�����������һ��Ŀ¼����Ϣ
	void release();
	//���������
	long size;		//�ļ���С
	FILETIME time;	//�ļ��޸ĵ�ʱ��
};
class XDir
{
public:
	XBool isEnable;				//Ŀ¼�Ƿ���Ч
//	char * directoryName;		//Ŀ¼
	std::string directoryName;
	std::vector<XFileInfo *> files;		//Ŀ¼�µ��ļ�(��)
	XDir()
		:isEnable(XFalse)
		,level(0)
//		,directoryName(NULL)
	{}
	void release();
	int level;			//·���㼶,��ͼ��������ı���[0 - n]
};
class XDirectory
{
private:
	XBool m_isInited;		//�Ƿ��ʼ��
	XBool m_withChildFolder;	//�Ƿ���������ļ��� 
public:
	XDir m_curDirectory;	//��ǰ��Ŀ¼��Ϣ
	XBool init(const char *directoryName,bool withChildFolder = true);	//ʹ�þ���·����·����Ҫ�ԡ�/�������ǡ�\������
	XBool initEx(const char *directoryName,bool withChildFolder = true);	//ʹ�����·��������ڹ���·����·����Ҫ�ԡ�/�������ǡ�\������

	XDirectory()
		:m_isInited(XFalse)
		,m_withChildFolder(XTrue)
	{}
	~XDirectory(){release();}
	void release()
	{//�ͷ����е���Դ
		if(!m_isInited) return;
		m_curDirectory.release();
		m_isInited = XFalse;
	}
	std::vector<XFileInfo *> m_sortResult;	//������
	enum XSortMode
	{
		SORT_BY_SYSTEM,	//ϵͳ����
		SORT_BY_SIZE,	//����С����
		SORT_BY_TIME,	//��ʱ������
	};
	void sortFiles(XSortMode mode = SORT_BY_SYSTEM);	//���ȹ����ļ��ٽ�������(��δ��������)
	void filterFiles()	//�����ļ�(��δ��������)
	{
		m_curSortMode = SORT_BY_SYSTEM;
		m_sortResult.clear();
		listFiles(m_curDirectory);
	}
	void setFilterName(const std::string &name);	//���ù��˵��ļ���չ��
	void clearFilter(){m_filterNames.clear();}	//��չ�����
private:
	XSortMode m_curSortMode;
	std::vector<std::string> m_filterNames;	//�ļ����˵�����
	void listFiles(const XDir&dir);	//�г�·���µ������ļ�����ͨ�����������й���
};
namespace XFile
{
	extern XBool infoConvert(WIN32_FIND_DATA &fileInfo,XFileInfo &file);
	extern XBool enumerationFiles(const char *path,XDir &dir,bool witchCilder = true);		//ʹ�þ���·��,����Ŀ¼�µ��ļ���Ϣ��������Ŀ¼
	inline XBool enumerationFiles(const std::string &path,XDir &dir,bool witchCilder = true){return enumerationFiles(path.c_str(),dir,witchCilder);}
	extern XBool enumerationFilesEx(const char *path,XDir &dir,bool witchCilder = true);	//ʹ�����·��,����Ŀ¼�µ��ļ���Ϣ��������Ŀ¼
	inline XBool enumerationFilesEx(const std::string &path,XDir &dir,bool witchCilder = true){return enumerationFilesEx(path.c_str(),dir,witchCilder);}
}
}
#endif