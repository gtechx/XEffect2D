inline int hexCharToInt(char p)	//16�����ַ�תint����ֵ
{
	if(p >= '0' && p <= '9') return p - '0';
	if(p >= 'a' && p <= 'f') return p - 'a' + 10;
	if(p >= 'A' && p <= 'F') return p - 'A' + 10;
	return '0';
}
inline _XBool hexToBin(const char *in,unsigned char *out)
{
	if(in == NULL || out == NULL) return XFalse;
	int len = 0;
	if((len = strlen(in)) %2 != 0) return XFalse;
	int tmp0;
	int tmp1;
	for(int i = 0;i < len;i += 2)
	{
		tmp0 = hexCharToInt(in[i]);
		if(tmp0 == '0') return XFalse;
		tmp1 = hexCharToInt(in[i + 1]);
		if(tmp1 == '0') return XFalse;
		out[i >> 1] = (tmp0 << 4) + tmp1;
	}
	return XTrue;
}
inline std::string intToStr(int temp,int radix)
{
	static char str[128];
	itoa(temp,str,radix);
	return str;
}
inline std::string floatToStr(float temp,int len)
{
	static char str[128];
	gcvt(temp,len,str);
	return str;
}
inline std::string getWorkPath()
{//��ȡ��ǰ����·��
	static char path[512];
	GetCurrentDirectory(512,path);
	return path;
}
inline std::string getFileExtension(const char * filename)
{
	if(filename == NULL) return " ";
	int len = getCharPosition(filename,'.',1);
	if(len < 0) return " ";
	return filename + len + 1;
}
inline void setWorkPath(const std::string &path)
{//���õ�ǰ����·��
	SetCurrentDirectory(path.c_str());
}
inline std::string getCurrentExeFileFullPath()
{//��ȡ��ǰexe�ļ���ȫ·���ļ���
	static char lpFileName[MAX_PATH];
	GetModuleFileName(NULL,lpFileName,MAX_PATH);
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{//���·��ת���ɾ���·��
	//����1���Լ������������·��������ʱ������������
	//static char currentPath[MAX_PATH + 1];
	//GetCurrentDirectory(MAX_PATH,currentPath);	//��¼��ǰ��·��
	//int len = getCharPosition(filename.c_str(),'\\',1);
	//int len1 = getCharPosition(filename.c_str(),'/',1);
	//if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	//
	//static char path[MAX_PATH + 1];
	//static char path1[MAX_PATH + 1];
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
	static char currentPath[MAX_PATH + 1];
	if(!GetFullPathName(filename.c_str(),MAX_PATH + 1,currentPath,NULL)) return "";
	return currentPath;
}
inline _XBool isAbsolutePath(const char * path)
{
	if(path == NULL) return XTrue;
	if(strlen(path) > 2 && path[1] == ':') return XTrue;
	return XFalse;
}
inline std::string mergeFilename(const std::string &base,const std::string &offset)
{//�������base��offset���ǺϷ���·��
	if(isAbsolutePath(offset.c_str())) return offset;
	int depth,lastPos;
	calPathDepth(offset.c_str(),depth,lastPos);
	std::string tmp = getBackPath(base.c_str(),depth);
	return tmp + (offset.c_str() + lastPos);
}
inline void setCapsLockState(_XBool state)
{
	BYTE btKeyState[256];
	if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
	if((!(btKeyState[VK_CAPITAL] & 1) && state) ||
		((btKeyState[VK_CAPITAL] & 1) && !state))
	{//�������û������������Ϊ��
		keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}
inline void setNumLockState(_XBool state)
{
	BYTE btKeyState[256];
	if(!GetKeyboardState((LPBYTE)&btKeyState)) return;
	 
	if((!(btKeyState[VK_NUMLOCK] & 1) && state) ||
		((btKeyState[VK_NUMLOCK] & 1) && !state))
	{//�������û������������Ϊ��
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}
inline std::string getChooseFilename(char * defName,char * title,bool isOpen)
{
	char OFN_Direct[MAX_PATH];	//ȫ·��
	char OFN_Name[MAX_PATH];	//�ļ���
	OFN_Name[0] ='\0';
	//strcpy(OFN_Name,"c:config.xml");
	if(defName == NULL) OFN_Direct[0] ='\0';
	else strcpy(OFN_Direct,defName);
	if(!popFileOpenDlg(NULL,OFN_Direct,OFN_Name,title,isOpen) || strlen(OFN_Name) <= 0) return "";
	return OFN_Direct;
}
inline std::string getWindowsSysFontPath()
{
	std::string fontsDir = getenv ("windir");
	fontsDir += "\\Fonts\\";
	return fontsDir;
}
inline std::string getFilenameFormPath(const char *path)
{
	if(path == NULL) return "";
	int ret = getPathDeepByChar(path);
	if(path[ret] == '/' || path[ret] == '\\') return path + ret + 1;
	return path + ret;
}
inline wchar_t oneCharToWchar(const char* src)
{
	if(src == NULL) return NULL;
	wchar_t ret;
	MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
	return ret;
}
inline std::string Wchar2ANSI(const wchar_t* src)
{
	if(src == NULL) return NULL;
    int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
 	std::string ret;
	ret.resize(nRetLen + 1);
    WideCharToMultiByte(CP_ACP, 0, src, -1, &ret[0], nRetLen, NULL, NULL);
    return ret;
}