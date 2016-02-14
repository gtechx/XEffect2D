#ifndef _JIA_XSTRINGFUN_
#define _JIA_XSTRINGFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//���ﶨ�嶼�����ַ���������ط���
#include "XOSDefine.h"
#include <string>
namespace XE{
enum XStringCodeType
{
	STR_TYPE_ANSI,
	STR_TYPE_UTF8,
	STR_TYPE_UTF16,
	STR_TYPE_UNKNOWN,	//δ֪��
};

//���ַ������ҵ������һ������Ҫ����ַ���λ��,ע����������������һ��û�н��������ַ������ᵼ�����ش���
//����-1��ʾ�Ҳ�������ַ�
namespace XString
{
	extern XBool getIsNumber(const char * p);			//����ַ����Ƿ�Ϊʮ������ֵ
	extern XBool getIsUnsignedInt(const char * p);		//�ж��ַ����Ƿ�Ϊ�޷���������(��δʵ��)
	extern XBool getIsInt(const char * p);				//�ж��ַ����Ƿ�Ϊ������(��δʵ��)
	extern XBool getIsValidIp(const char *p);			//�ж��ַ����Ƿ�Ϊ�Ϸ���IP��ַ
	extern XBool getIsLocalhost(const char *p);		//�ж��ַ����Ƿ���localhost��ʽ��IP(�ܹ��칦�ܵĺ�����û�����ô�����Ϻ�)
	extern int getIsHexNumber(const char * p);			//����ַ����Ƿ�Ϊ16������������ǵĻ�ֱ�ӷ�����ֵ��ע��ȡֵ��Χ���⣬-1����Ƿ�
	extern int getCharSum(const char *p,char temp);		//ͳ���ַ������ض��ַ�������
	extern int getCharPosition(const char *p,char temp,int mode = 0);				//���ַ���p�в����ַ�temp��λ�ã�-1Ϊû������ַ���mode 0:������ң�1:�������
	extern int getCharPositionEx(const char *p,char temp,int index,int mode = 0);	//��ȡ�ַ�����ָ���ַ���index�γ��ֵ�λ��
	extern XBool getFirstWord(const char *p,int &s,int &e);						//��ȡ�ַ����еĵ�һ������ e.g:"  xia jia ... " return "xia"��Ŀǰ��֧�����ģ�����û�е��ʵ���ʽ����
	extern XBool getFirtCanShowString(const char *p,int &s,int &e);				//��ȡ��һ�ο�����ʾ���ַ���,�����ַ�������Ҫ��ANSI��ʽ
	//canShowLen������ʾ���ַ����Ŀ��
	extern std::string getCanShowString(const char *str,int canShowLen);
	//��ȡANSI�ַ����е�index���ַ����ֽ�λ�ã��磺"��ʤ��xiajia"��index = 1ʱ����2��index = 2ʱ����4��index = 4�Ƿ���7
	extern int getANSIStrPosLen(const char * str,int index);
	//�����������Ϣ�ͳ��ȷ��ؿ�����ʾ���ַ���������
	//len:��ʾ�Ŀ��
	//charWidth:Ӣ���ַ��Ŀ��
	inline std::string getCanShowString(const char *str,int len,int charWidth)
	{
		if(charWidth <= 0) return "";
		return getCanShowString(str,len / charWidth);
	}
	//ANSI�����ʽ�����ַ����е�ǰ��λ���Ƿ�Ϊ�ַ��Ľ���(XTrue)���������ַ����м�(XFalse)������ռ�������ֽڣ��жϵ�ǰλ���Ƿ��������ֽڵ��м���߽���
	extern XBool isAtUnicodeEnd(const char *p,int pos);
	//��16���Ƶ�pת����10���ƣ��Ƿ����ݷ���'0'
	inline int hexCharToInt(char p);	//16�����ַ�תint����ֵ
	//16����ת����2���ƣ���"FFEECC",ת����{0xff,0xee,0xcc}
	//ע���������������ȷ�ԣ�1���������Ϊ2�ı�����2�����������������Ҫһ��
	inline XBool hexToBin(const char *in,unsigned char *out);
	inline XBool binToHex(const char *in,unsigned char *out);	//��δʵ��
	//��tempת����radix�����ַ���������֧��2���ƣ�8���ƣ�10���ƣ�16����
	inline std::string intToStr(int temp,int radix = 16);
	//��������ת�����ַ�������toString����Ч�ʸ�
	inline std::string floatToStr(float temp,int len = 6);
	extern std::string getTextFromClipboard();		//��ճ�����ж�ȡ����
	extern void setTextToClipboard(const std::string & str);		//������д�뵽ճ����
	//���ַ���tempװ���ɶ�Ӧ������������֧��2���ƣ�8���ƣ�10���ƣ�16����
	//ע���������û�п�����Խ�����⣬���ַ�����ʾ�����ݳ���int���ܱ�ʾ�����Χʱ�������������ִ���
	//���������ַ������ƶ��������ݲ���ʱ���ú���û�н��м�飬������ɴ���
	extern int strToInt(const std::string &temp,int radix = 10);
	//�ַ�ת���ķ���(windows��UTF16��wchar��unicode�ȼۣ����ڱ��ϵͳ�²��ȼ���Ҫ���鿼��)
	extern char* ANSIToUTF8(const char* src);			//ע�⣺��Ҫ�ֶ��ͷŷ��ص��ַ�����Դ
	extern std::string ANSI2UTF8(const char * src);
	inline std::string ANSI2UTF8(const std::string & str){return ANSI2UTF8(str.c_str());}
	extern char* UTF8ToANSI(const char* src);			//ע�⣺��Ҫ�ֶ��ͷŷ��ص��ַ�����Դ
	extern std::string UTF82ANSI(const char * src);
	inline std::string UTF82ANSI(const std::string &str){return UTF82ANSI(str.c_str());}
	extern wchar_t* ANSIToWchar(const char* src);	//ע�⣺��Ҫ�ֶ��ͷŷ��ص��ַ�����Դ
	extern char* WcharToANSI(const wchar_t* src);			//ע�⣺��Ҫ�ֶ��ͷŷ��ص��ַ�����Դ
	inline std::string Wchar2ANSI(const wchar_t* src);
	inline wchar_t oneCharToWchar(const char* src);
	extern bool isUTF8(const char * str,int length);	//��������߼��ϲ����Ͻ�
	inline bool isUTF8(const std::string &str){return isUTF8(str.c_str(),str.size());}
	extern bool isUTF16(const char * str,int length);	//��δʵ��
	extern bool isANSI(const char * str,int length);	//��������߼��ϲ����Ͻ�
	//��ȡ�ַ����ı����ʽ
	extern XStringCodeType getStrCodeType(const char * str,int length);	//��δʵ��
	//��һ������ת�����ַ���
	template <typename T>
	std::string toString(const T& value)
	{
		std::ostringstream out;
	//	out.str("");
		out << value;
		return out.str();
	}
	//��һ���ַ���ת��Ϊ����
	template <typename T>
	const T toValue(const std::string& str)
	{
		T val;
		std::stringstream ss;
		ss << str;
		ss >> val;
		if(ss.fail())
				throw std::runtime_error((std::string)typeid(T).name() + "type wanted:" + str);
		return val;
	}
	//��ȡ�ַ����б����ַ��ĳ��ȣ����getStrLen("���") = 2,getStrLen("abc���") = 5;
	extern int getStrLen(const char * p);
}
////windows�´�ϵͳע����л�ȡϵͳ�����ӳ���ϵ
//#include <map>
//bool initWindows()
//{
//	//������Թ���ϵͳ�������ط���
//	std::map<string, string> fontsMap;	//ϵͳ������·����ӳ���
//	const wchar_t *Fonts = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";	//ע���������ļ�ֵ
//	HKEY key_ft;
//	LONG l_ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, Fonts, 0, KEY_QUERY_VALUE, &key_ft);	//��ע����л�ȡ����ļ�ֵ
//	if (l_ret != ERROR_SUCCESS)
//	{
//		LogStr("initWindows(): couldn't find fonts registery key");	
//		return false;	//��ע����л�ȡϵͳ�����ֵʧ��
//	}
//	
//	DWORD value_count;
//	DWORD max_data_len;
//	wchar_t value_name[2048];
//	BYTE *value_data;
//	// get font_file_name -> font_face mapping from the "Fonts" registry key
//	l_ret = RegQueryInfoKeyW(key_ft, NULL, NULL, NULL, NULL, NULL, NULL, &value_count, NULL, &max_data_len, NULL, NULL);
//	if(l_ret != ERROR_SUCCESS)
//	{
//	    LogStr("initWindows(): couldn't query registery for fonts");
//        return false;
//	}
//	// no font installed
//	if(value_count == 0)
//	{
//	    LogStr("initWindows(): couldn't find any fonts in registery");
//        return false;
//	}
//	// max_data_len is in BYTE
//	value_data = static_cast<BYTE *>(HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, max_data_len));
//	if(value_data == NULL) return false;
//
//	char value_name_char[2048];
//	char value_data_char[2048];
//
//    string fontsDir = getenv ("windir");
//    fontsDir += "\\Fonts\\";
//	for (DWORD i = 0; i < value_count; ++i)
//	{
//		DWORD name_len = 2048;
//		DWORD data_len = max_data_len;
//
//		l_ret = RegEnumValueW(key_ft, i, value_name, &name_len, NULL, NULL, value_data, &data_len);
//		if(l_ret != ERROR_SUCCESS)
//		{
//			LogStr("initWindows(): couldn't read registry key for font type");
//			continue;
//		}
//
//		wcstombs(value_name_char,value_name,2048);
//		wcstombs(value_data_char,reinterpret_cast<wchar_t *>(value_data),2048);
//		string curr_face = value_name_char;
//		string font_file = value_data_char;
//		curr_face = curr_face.substr(0, curr_face.find('(') - 1);
//		fontsMap[curr_face] = fontsDir + font_file;
//	}
//	HeapFree(GetProcessHeap(), 0, value_data);
//	l_ret = RegCloseKey(key_ft);
//}
#if WITH_INLINE_FILE
#include "XStringFun.inl"
#endif
}
#endif