inline std::string getWorkPath()
{//��ȡ��ǰ����·��
	static char path[512] = "";
#ifdef UNICODE
	if (GetCurrentDirectoryA(512, path) <= 0) return "";
#else
	if (GetCurrentDirectory(512, path) <= 0) return "";
#endif
	return path;
}
inline std::string getFileExtension(const char * filename)
{//��ȡ�ļ�����չ��
	if (filename == NULL) return " ";
	int len = XString::getCharPosition(filename, '.', 1);
	if (len < 0) return " ";
	return filename + len + 1;
}
inline XBool setWorkPath(const std::string &path)
{//���õ�ǰ����·��
#ifdef UNICODE
	return SetCurrentDirectoryA(path.c_str());
#else
	return SetCurrentDirectory(path.c_str());
#endif
}
inline std::string getCurrentExeFileFullPath()
{//��ȡ��ǰexe�ļ���ȫ·���ļ���
	char lpFileName[MAX_PATH];
#ifdef UNICODE
	if (GetModuleFileNameA(NULL, lpFileName, MAX_PATH) <= 0) return "";
#else
	if (GetModuleFileName(NULL, lpFileName, MAX_PATH) <= 0) return "";
#endif
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{//���·��ת���ɾ���·��
	//����1���Լ������������·��������ʱ������������
	//char currentPath[MAX_PATH + 1];
	//GetCurrentDirectory(MAX_PATH,currentPath);	//��¼��ǰ��·��
	//int len = XString::getCharPosition(filename.c_str(),'\\',1);
	//int len1 = XString::getCharPosition(filename.c_str(),'/',1);
	//if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	//
	//char path[MAX_PATH + 1];
	//char path1[MAX_PATH + 1];
	//strcpy(path,filename.c_str());
	//if(len >= len1) len = len + 1;
	//else len = len1 + 1;
	//path[len] = '\0';
	//SetCurrentDirectory(path);  //�õõ������·�����õ�ǰĿ¼
	//GetCurrentDirectory(MAX_PATH,path1);  //�õ���ǰĿ¼�ľ���·��
	//
	//SetCurrentDirectory(currentPath);	//�ָ���ǰ��·��
	//strcat(path1,"/");
	//strcpy(path,filename.c_str());
	//strcat(path1,path + len);
	//return  path1;
	//����2��ֱ�ӵ���ϵͳAPI�����׵�
	char currentPath[MAX_PATH + 1];
#ifdef UNICODE
	if (!GetFullPathNameA(filename.c_str(), MAX_PATH + 1, currentPath, NULL)) return "";
#else
	if (!GetFullPathName(filename.c_str(), MAX_PATH + 1, currentPath, NULL)) return "";
#endif
	return currentPath;
}
inline XBool isAbsolutePath(const char * path)	//�Ƿ�Ϊ����·��
{
	if (path == NULL) return XTrue;
	if (strlen(path) > 2 && path[1] == ':') return XTrue;
	return XFalse;
}
inline std::string mergeFilename(const std::string &base, const std::string &offset)
{//�������base��offset���ǺϷ���·��
	if (isAbsolutePath(offset.c_str())) return offset;
	int depth, lastPos;
	calPathDepth(offset.c_str(), depth, lastPos);
	std::string tmp = getBackPath(base.c_str(), depth);
	return tmp + (offset.c_str() + lastPos);
}
//defName:Ĭ�ϵ��ļ���
//title:��������
//isOpen:��ť�Ƿ�Ϊ����
//defExt:Ĭ����չ��
inline std::string getChooseFilename(char * defName, char * title, bool isOpen, const std::string&defExt)
{
	char OFN_Direct[MAX_PATH];	//ȫ·��
	char OFN_Name[MAX_PATH];	//�ļ���
	memset(OFN_Direct,0,MAX_PATH);
	memset(OFN_Name,0,MAX_PATH);
	//strcpy(OFN_Name,"c:config.xml");
	if (defName != NULL) strcpy(OFN_Direct, defName);
	if (!popFileOpenDlg(XEG.getHWND(), OFN_Direct, OFN_Name, title, isOpen, defExt) || strlen(OFN_Name) <= 0) return "";
	return OFN_Direct;
}
inline std::string getWindowsSysFontPath()
{
	std::string fontsDir = getenv("windir");
	fontsDir += "\\Fonts\\";
	return fontsDir;
}
inline std::string getFilenameFormPath(const char *path)
{
	if (path == NULL) return "";
	int ret = getPathDeepByChar(path);
	if (path[ret] == '/' || path[ret] == '\\') return path + ret + 1;
	return path + ret;
}
inline std::string getFilePath(const char *filename)
{
	if(filename == NULL) return "";
	char tmpFilename[MAX_PATH];
	strcpy(tmpFilename,filename);
	int index = getPathDeepByChar(tmpFilename);
	if(index == 0) return "";	//·�����Ǿ���·��
	tmpFilename[index] = '\0';
	return tmpFilename;
}