#ifndef _JIA_XFILE_
#define _JIA_XFILE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//�������ļ���ز����ĺ��������ڶ��ļ���Ϣ���д���
#include "XStringFun.h"

#include "XEngineCommon.h"

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
namespace XE{
class XCColor;
namespace XFile
{
	//��ȡ�ļ��Ĵ�С
	inline int getFileLen(FILE * fp)
	{
		if (fp == NULL) return 0;
		int tempOffset = ftell(fp);	//��¼��ǰ���ļ�λ��
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);	//��ȡ�ļ��ĳ���
		fseek(fp, tempOffset, SEEK_SET);	//�ָ�֮ǰ���ļ�λ��
		return len;
	}
	extern char *readFileToStr(const char *filename,XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��filename���ļ��ж�ȡshader����	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
	extern int writeStrToFile(const char *filename, const char *s);					//��shader����sд�뵽filename���ļ���
	extern bool getFileString(const char *filename,std::string &outStr);	//��ָ�����ļ��ж�ȡ����
	//���溯�������ӣ�xiajia.txt,5 ����� xiajia(5).txt
	extern std::string addIndexToFilename(const char *filename,int index);	//���ļ���filename�ĺ������index�ı�ţ���Ҫ�����ڷ�ֹ�ļ����ظ�
	extern XBool fileNameCompare(const char *p1, const char *p2);	//�Ƚ�����·�������ļ����Ƿ�һ�£������Ǵ�Сд��
	extern int getPathDeep(const char *p);			//��ȡ·������ȣ���ȡ·��Ŀ¼�Ĳ��� e.g.:"c:/x/y/xx.txt" return 3
	extern int getPathDeepByChar(const char *p);	//��ȡ·������ȣ���ȡ·��Ŀ¼���ַ������ e.g.:"c:/x/y/xx.txt" return 6
	//��ȡ��ǰ����·��
	inline std::string getWorkPath();
	//��ȡ�ļ���չ��
	inline std::string getFileExtension(const char * filename);
	//���õ�ǰ����·��
	inline XBool setWorkPath(const std::string &path);
	//��ȡ��ǰexe�ļ���ȫ·���ļ���
	inline std::string getCurrentExeFileFullPath();
	//���·��ת���ɾ���·��
	inline std::string getFullPath(const std::string &filename);
	//#include "Shlwapi.h"
	//./../../data/data.txt,depthΪ2��lastPosΪ8
	//depth�����˵�·��������
	//lastPos����ʽ·�����ַ����е�λ��
	extern void calPathDepth(const char *p, int &depth, int &lastPos);	//�������·���ǺϷ���
	//·��p����backSum֮���·��backSum�������0
	extern std::string getBackPath(const char *p, int backSum);
	//����exeĿ¼��Ϊ����Ŀ¼
	inline void setExeAsWorkPath()
	{
		std::string tmp = getBackPath(XE::XFile::getCurrentExeFileFullPath().c_str(),0);
		tmp[tmp.size() - 1] = '\0';
		setWorkPath(tmp);
	}
	//�ж�·���Ƿ�Ϊ����·��
	inline XBool isAbsolutePath(const char * path);
	//����·�������·�������µ��ļ�·���磺./data/1.txt��../x.txt�ϲ�֮��Ϊ./x.txt
	inline std::string mergeFilename(const std::string &base, const std::string &offset);//�������base��offset���ǺϷ���·��
	//�ж�·�����ַ�������
	extern int getCharsNumInPath(const char * path);
	extern bool isPathValid(const char * path);	//���·���Ƿ�Ϸ�����δ������֤(Ŀǰ�������⣺��./../data/a.dat)
	//�����ļ��򿪴��ڲ�����ѡ����ļ������·���;���·��
	extern BOOL popFileOpenDlg(HWND hwnd, char* pstrFileName, char* pstrTitleName, char* title, bool isOpen, const std::string&defExt = "xml");
	//ͨ��winAPI�����ļ��򿪴����Ա������Ի���ѡ���ļ���
	inline std::string getChooseFilename(char * defName, char * title, bool isOpen, const std::string&defExt = "xml");
	//ͨ��winAPI�����Ի���ѡ���ļ���
	extern std::string getChooseFolderName();
	//ͨ��winAPI����������ѡ����Ҫ����ɫ
	extern XCColor getChooseColor(const XCColor &color);
	//ͨ��winApi��ȡϵͳ�����·��
	inline std::string getWindowsSysFontPath();
	//��·���ַ����л�ȡ�ļ���
	inline std::string getFilenameFormPath(const char *path);
	//���ļ�ȫ·�����л�ȡ·������C:/nihao/x.dat����C:/nihao��ע������Ǿ���·��
	inline std::string getFilePath(const char *filename);
	//�жϾ���·��������ļ������ļ����Ƿ����
	inline XBool isExistFile(const char *path)		
	{
		if(path == NULL) return XFalse;
	#ifdef UNICODE
		return PathFileExistsA(path);
	#else
		return PathFileExists(path);
	#endif
	}
	//�ж����·��������ļ������ļ����Ƿ����
	extern XBool isExistFileEx(const char *path);
	//�ж�Ŀ���Ƿ�Ϊ�ļ���(����·��)
	inline XBool isFolder(const char * path)		
	{
		if(path == NULL) return XFalse;
	#ifdef UNICODE
		return (GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY);
	#else
		return (GetFileAttributes(path) & FILE_ATTRIBUTE_DIRECTORY);
	#endif
	}
	//�ж�Ŀ���Ƿ�Ϊ�ļ���(���·��)
	extern XBool isFolderEx(const char * path);	
	//ɾ��ָ���ļ�
	inline XBool deleteFile(const char *filename)
	{
		if(filename == NULL) return XFalse;
	#ifdef UNICODE
		return DeleteFileA(filename);
	#else
		return DeleteFile(filename);
	#endif
	}
#if WITH_INLINE_FILE
#include "XFile.inl"
#endif
}
}
#endif