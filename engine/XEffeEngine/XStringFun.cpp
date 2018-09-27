#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XStringFun.h"
#include "XCommonDefine.h"
#include "XNetWork\XWinSockCommon.h"

namespace XE{
const char *XStringConverter::ANSI2UTF8(const char*src)
{
	if (src == NULL) return NULL;
	int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	if (nRetLen == 0) return NULL;
	if (m_tmpArray.size() < nRetLen + 1) m_tmpArray.resize(nRetLen + 1);	//�����ڴ�ռ�

	MultiByteToWideChar(CP_ACP, 0, src, -1, &(m_tmpArray[0]), nRetLen);
	nRetLen = WideCharToMultiByte(CP_UTF8, 0, &(m_tmpArray[0]), -1, NULL, 0, NULL, NULL);
	if (m_result.size() < nRetLen + 1) m_result.resize(nRetLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, &(m_tmpArray[0]), -1, &m_result[0], nRetLen, NULL, NULL);

	return m_result.c_str();
}
namespace XString
{
	XBool binToHex(const unsigned char *in,int len,unsigned char *out)
	{//��δ��������
		if(in == NULL || out == NULL || len <= 0) return false;
		char *pTmp = (char *)out;
		for(int i = 0;i < len;++ i)
		{
			sprintf(pTmp + i * 2,"%02x",in[i]);
		}
		return true;
	}
	XBool getIsNumber(const char * p)
	{
		//����1
		//000.000������ʽû���ж�
		if(p == NULL) return XFalse;
		int index = 0;
		//��һ���ַ�����Ϊ'-' '+'
		//if((p[index] < '0' || p[index] > '9') && p[index] != '+' && p[index] != '-') return XFalse;
		if(p[index] == '+' || p[index] == '-') ++ index;
		int len = strlen(p);
		if(len <= index) return XFalse;
		XBool dotFlag = XFalse;
		for(;index < len;++ index)
		{
			if(p[index] >= '0' && p[index] <= '9') continue;
			if(p[index] == '.' && !dotFlag)
			{
				dotFlag = XTrue;
				continue;
			}
			return XFalse;
		}
		return XTrue;
		//����2,����ǿ���ٶ���
		//if(p == NULL) return XFalse;
		//char *ok = NULL;
		//strtod(p,&ok);
		//return !isspace(*p) && strlen(ok) == 0;
	}
	XBool getIsUnsignedInt(const char * p)	//�ж��ַ����Ƿ�Ϊ�޷���������(��δʵ��)
	{
		if(p == NULL) return XFalse;
		int len = strlen(p);
		for(int index = 0;index < len;++ index)
		{
			if(p[index] < '0' || p[index] > '9') return XFalse;
		}
		return XTrue;
	}
	XBool getIsInt(const char * p)			//�ж��ַ����Ƿ�Ϊ������(��δʵ��)
	{
		if(p == NULL) return XFalse;
		int len = strlen(p);
		int index = 0;
		//��һ���ַ�����Ϊ'-' '+'
		if(p[index] == '+' || p[index] == '-') ++ index;
		for(;index < len;++ index)
		{
			if(p[index] < '0' || p[index] > '9') return XFalse;
		}
		return XTrue;
	}
	XBool getIsValidIp(const char *ip)
	{
		//����1��
		return inet_addr(ip) != INADDR_NONE;
		//����3��
		//struct sockaddr_in sa;
		//int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
		//return result != 0;
		//����2��
		//int p[4] ={0,0,0,0};
		//int len = strlen(ip);
		//int j = 0;
		//for(int i = 0; i < len;++ i)
		//{
		//	if(ip[i] == '.' || i == len -1)
		//	{/*�ָ��*/
		//		if(p[j] >= 0 && p[j] <= 255) ++ j;
		//		else return XFalse;
		//	}else
		//	{
		//		int d = ip[i] - '0';
		//		if(d > 9 || d < 0) return XFalse;
		//		else p[j] = p[j] * 10 + d;
		//	}
		//}
		//return true;
	}
	XBool getIsLocalhost(const char *p)
	{
		if(p == NULL) return XFalse;
		std::string tmp = p;
		for(unsigned int i = 0;i < tmp.size();++ i)
			if(tmp[i] >= 'A' && tmp[i] <= 'Z') tmp[i] -= 'A' - 'a';
		return tmp == "localhost";
	}
	int getIsHexNumber(const char * p)
	{
		if(p == NULL) return -1;
		int len = strlen(p);
		if(len <= 0) return -1;
		if(p[1] == 'x' || p[1] == 'X')
		{
			if(p[0] != '0') return -1;
			return getIsHexNumber(p + 2);
		}
		if(len <= 8)
		{//��unsigned int�ķ�Χ��
			int ret = 0;
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//�Ϸ�������
					if(p[i] >= '0' && p[i] <= '9') ret = (ret << 4) + p[i] - '0'; else
					if(p[i] >= 'A' && p[i] <= 'F') ret = (ret << 4) + p[i] - 'A' + 10; else
					if(p[i] >= 'a' && p[i] <= 'f') ret = (ret << 4) + p[i] - 'a' + 10; 
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return ret;
		}else
		{//������Χ
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//�Ϸ�������
					continue;
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return 1;
		}
	}
	long long getIsHexNumber64(const char * p)
	{
		if(p == NULL) return -1;
		int len = strlen(p);
		if(len <= 0) return -1;
		if(p[1] == 'x' || p[1] == 'X')
		{
			if(p[0] != '0') return -1;
			return getIsHexNumber64(p + 2);
		}
		if(len <= 16)
		{//��long long�ķ�Χ��
			long long ret = 0;
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//�Ϸ�������
					if(p[i] >= '0' && p[i] <= '9') ret = (ret << 4) + p[i] - '0'; else
					if(p[i] >= 'A' && p[i] <= 'F') ret = (ret << 4) + p[i] - 'A' + 10; else
					if(p[i] >= 'a' && p[i] <= 'f') ret = (ret << 4) + p[i] - 'a' + 10; 
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return ret;
		}else
		{//������Χ
			for(int i = 0;i < len;++ i)
			{
				if((p[i] >= '0' && p[i] <= '9') ||
					(p[i] >= 'A' && p[i] <= 'F') ||
					(p[i] >= 'a' && p[i] <= 'f'))
				{//�Ϸ�������
					continue;
				}else
				if(p[i] == '\0') break; 
				else
				{
					return -1;
				}
			}
			return 1;
		}
	}
	int getCharSum(const char *p,char temp)
	{
		if(p == NULL) return 0;
		int ret = 0;
		for(;*p != '\0';++ p)
		{
			if((* p) == temp) ++ ret;
		}
		//for(int i = 0;;++ i)
		//{
		//	if(p[i] == '\0') break;
		//	if(p[i] == temp) ++ ret;
		//}
		return ret;
	}
	//����ĺ������Կ���ʹ��memchr��strchr�����Ż�
	int getCharPosition(const char *p,char temp,int mode)
	{
		if(p == NULL) return -1;
		const char *p1 = p;
		if(mode == 0)
		{//�������
			for(;*p != '\0';++ p)
			{
				if(*p < 0)
				{//����
					++ p;
					continue;
				}
				if(*p == temp) return (p - p1);
			}
		}else
		{//�������
			for(int i = strlen(p) - 1;i >= 0;-- i)
			{
				if(i - 1 >= 0 && p[i - 1] < 0)
				{//����
					-- i;
					continue;
				}
				if(p[i] == temp) return i;
			}
		}
		return -1;
	}
	int getCharPositionEx(const char *p,char temp,int index,int mode)
	{
		if(p == NULL) return -1;
		if(index <= 0) return -1;
		int sum = 0;
		const char *p1 = p;
		if(mode == 0)
		{//�������
			for(;*p != '\0';++ p)
			{
				if(*p < 0)
				{//����
					++ p;
					continue;
				}
				if(*p == temp)
				{
					++ sum;
					if(sum == index)
						return (p - p1);
				}
			}
		}else
		{//�������
			for(int i = strlen(p) - 1;i >= 0;-- i)
			{
				if(i - 1 >= 0 && p[i - 1] < 0)
				{//����
					-- i;
					continue;
				}
				if(p[i] == temp)
				{
					++ sum;
					if(sum == index)
						return i;
				}
			}
		}
		return -1;
	}
	XBool getFirstWord(const char *p,int &s,int &e)
	{
		if(p == NULL) return XFalse;
		s = -1;
		e = -1;
		int len = strlen(p);
		if(len <= 0) return XFalse;
		for(int i = 0;i < len;++ i)
		{
			if((p[i] <= 'Z' && p[i] >= 'A')
				|| (p[i] <= 'z' && p[i] >= 'a')
				|| p[i] == '_')
			{
				if(s == -1) s = i;
			}else
			{
				if(s != -1) 
				{
					e = i - 1;
					return XTrue;
				}
			}
		}
		if(s != -1) e = len - 1;
		return (s >= 0 && e >= 0 && e > s);
	}
	XBool getFirtCanShowString(const char *p,int &s,int &e)
	{
		if(p == NULL) return XFalse;
		s = -1;
		e = -1;
		int len = strlen(p);
		if(len <= 0) return XFalse;
		for(int i = 0;i < len;++ i)
		{
			if(p[i] >= 32 && p[i] <= 126)
			{//����ʾ��ASCII��
				if(s == -1) s = i;
			}else
			{
				if(p[i] < 0)
				{//����
					if(s == -1) s = i;
					++ i;
				}else
				{//������ʾ���ַ�
					if(s != -1)
					{//����
						e = i - 1;
						return XTrue;
					}
				}
			}
		}
		if(s != -1) e = len - 1;
		return (s >= 0 && e >= 0 && e > s);
	}
	std::string getCanShowString(const char *str,int canShowLen)
	{
		if(str == NULL || canShowLen <= 0) return XString::gNullStr;
		int len = strlen(str);
		if(len == 0) return XString::gNullStr;
		if(len <= canShowLen) return str;
		if(canShowLen == 1) return ".";
		if(canShowLen == 2) return "..";
		std::string ret = str;
		//������Ҫ�����ַ����Ա������
		ret.resize(canShowLen);
		if(isAtUnicodeEnd(str,canShowLen - 2))
		{
			ret.at(canShowLen - 2) = '.';
			ret.at(canShowLen - 1) = '.';
		}else
		{
			ret.at(canShowLen - 1) = '.';
		}
		return ret;
	}
	int getANSIStrPosLen(const char * str,int index)
	{
		if(str == NULL || index <= 0) return 0;
		int len = strlen(str);
		int ret = 0;
		int tmp = 0;
		for(int i = 0;i < len;++ i)
		{
			if(str[i] < 0)
			{
				ret += 2;
				++ i;
				++ tmp;
			}else
			{
				++ ret;
				++ tmp;
			}
			if(tmp == index) return ret;
		}
		return len;
	}
	int strToInt(const std::string& temp,int radix)
	{
		int ret = 0;
		if(radix == 16) if(sscanf(temp.c_str(),"%x",&ret) != 1) return  0;else
		if(radix == 10) return atoi(temp.c_str()); else
		if(radix == 2)
		{
			int size = temp.size();
			const char * tempStr = temp.c_str();
			for(int i = 0;i < size;++ i)
			{
				ret = ret << 1;
				if(tempStr[i] == '1') ret += 1;
			}
		}else
		if(radix == 8)
		{
			int size = temp.size();
			const char * tempStr = temp.c_str();
			for(int i = 0;i < size;++ i)
			{
				ret = ret << 3;
				ret += tempStr[i] - '0';
			}
		}

		return ret;
	}
	std::string ANSI2UTF8(const char * src)
	{
		if(src == NULL) return XString::gNullStr;
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return XString::gNullStr;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return XString::gNullStr;
		MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
		nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		std::string ret;
		ret.resize(nRetLen + 1);
		WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL);
		XMem::XDELETE_ARRAY(unicode_buf);

		return ret;
	}
	char* UTF8ToANSI(const char* src)
	{
		if(src == NULL) return NULL;
		//��һ��UTF8תUTF16(windows��UTF16��wchar��unicode�ȼۣ����ڱ��ϵͳ�²��ȼ�)
		int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return NULL;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen);
		//�ڶ���UTF16תANSI
		nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
		if(buf == NULL) 
		{
			XMem::XDELETE_ARRAY(unicode_buf);
			return NULL;
		}
		WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
		XMem::XDELETE_ARRAY(unicode_buf);
		return buf;
	}
	std::string UTF82ANSI(const char * src)
	{
		if(src == NULL) return XString::gNullStr;
		int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return XString::gNullStr;
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		if(MultiByteToWideChar(CP_UTF8, 0, src, -1, unicode_buf, nRetLen) == 0) return XString::gNullStr;
		nRetLen = WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, NULL, 0, NULL, NULL);

		std::string ret;
		ret.resize(nRetLen + 1);
		if(WideCharToMultiByte(CP_ACP, 0, unicode_buf, -1, &ret[0], nRetLen, NULL, NULL) == 0) return XString::gNullStr;
		XMem::XDELETE_ARRAY(unicode_buf);

		return ret;
	}
	//�ж��ַ����Ƿ�ΪUTF8��ʽ(������������⣬��"ֹͣ"�ַ������ж��ϳ���)
	bool isUTF8(const char * str,int length)  
	{
		if(str == NULL) return false;
		DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�  
		UCHAR chr;  
		bool bAllAscii = true; //���ȫ������ASCII, ˵������UTF-8  
		for(int i = 0;i < length;++ i)  
		{  
			chr = *(str+i);  
			if((chr&0x80) != 0) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx  
				bAllAscii = false;  
			if(nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���  
			{  
				if(chr >= 0x80)  
				{  
					if(chr >= 0xFC && chr <= 0xFD)  
						nBytes = 6;  
					else if(chr >= 0xF8)  
						nBytes = 5;  
					else if(chr >= 0xF0)  
						nBytes = 4;  
					else if(chr >= 0xE0)  
						nBytes = 3;  
					else if(chr >= 0xC0)  
						nBytes = 2;  
					else  
						return false;  
					-- nBytes;  
				}  
			}else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx  
			{  
				if((chr & 0xC0) != 0x80)  
					return false;  

				-- nBytes;  
			}  
		}  
		if(nBytes > 0 ||//Υ������  
			bAllAscii) return false;//���ȫ������ASCII, ˵������UTF-8  
		return true;  
	}
	bool isANSI(const char * str,int length)
	{
		if(str == NULL || length <= 0) return false;	//�Ƿ�������
		//unsigned char *p = (unsigned char *)str;
		for(int i = 0;i < length;)
		{
			if(str[i] < 0x80) ++i;
			else
			{
				if(length == i) return false;	//���Ȳ���
				if(str[i] >= 129 && str[i] <= 254
					&& str[i + 1] >= 64 && str[i + 1]<=254)
					i += 2;
				else
					return false;
			}
		}
		return true;
	}
	XBool isAtUnicodeEnd(const char *p,int pos)
	{
		int len = strlen(p);
		if(p == NULL || pos < 0 || pos > len) return XFalse;
		if(pos == 0 || pos == len) return XTrue;
		int index = 0;
		while(index < pos)
		{
			if(p[index] < 0) index += 2;
			else ++ index;
		}
		return (index == pos);
	}
	std::string getTextFromClipboard()
	{//��ճ�����ж�ȡ����
		if(!IsClipboardFormatAvailable(CF_TEXT)) return XString::gNullStr;
	//	HWND h = FindWindow(NULL,"Control");
	//	if(!OpenClipboard(h)) return XString::gNullStr;
		if(!OpenClipboard(NULL)) return XString::gNullStr;
		HGLOBAL hglb = GetClipboardData(CF_TEXT); 
		if(hglb == NULL) return XString::gNullStr;
		LPVOID lptstr = GlobalLock(hglb);
		GlobalUnlock(hglb); 
		CloseClipboard();
		if(lptstr != NULL) return (char *)lptstr;
		else return XString::gNullStr;
	}
	void setTextToClipboard(const std::string& str)
	{//������д�뵽ճ����
	//	HWND h = FindWindow(NULL,"Control");
	//	if(!OpenClipboard(h) || !EmptyClipboard()) return; 
		if(!OpenClipboard(NULL) || !EmptyClipboard()) return; 
		size_t size = sizeof(TCHAR)*(1 + str.length());
		HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE,size); 
		if(hResult == NULL)
		{	//�ڴ����ʧ��
			CloseClipboard();
			return;
		}
		LPSTR lpstrCopy = (LPSTR)GlobalLock(hResult);
		if(lpstrCopy == NULL)
		{
			GlobalFree(hResult);
			CloseClipboard();
			return;
		}
		memcpy(lpstrCopy,str.c_str(),size); 
	//	lpstrCopy[size] = (TCHAR)0;   
		GlobalUnlock(hResult); 
	#ifndef _UNICODE
		if(SetClipboardData(CF_TEXT,hResult) == NULL)
	#else
		if(SetClipboardData(CF_UNICODETEXT,hResult) == NULL)
	#endif
		{
			printf("Unable to set Clipboard data");
			GlobalFree(hResult); // Edit by Leo Davidson 16/Sep/2010: Free buffer if clipboard didn't take it.
		}

		CloseClipboard();
	}
	wchar_t* ANSIToWchar(const char* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
		return unicode_buf;
	}
	char* ANSIToUTF8(const char* src)
	{
		if(src == NULL) return NULL;
		// *function: MultiByteToWideChar (Maps a character string to a UTF-16 (wide character) string.)
		// - UINT CodePage (Code page to use in performing the conversion. )
		//                CP_ACP: The system default Windows ANSI code page.
		// - DWORD dwFlags (Flags indicating the conversion type)
		//                0:
		// - LPCSTR lpMultiByteStr (Pointer to the character string to convert.)
		//                src: the word that you want to conver
		// - int cbMultiByte (you want to process size of lpMultiByteStr)
		//                -1:  the function processes the entire input string, including the terminating null character. when the input string
		//                            not contains terminating null character, it will failure.
		// - LPWSTR lpWideCharStr (Pointer to a buffer that receives the converted string.)
		//                NULL: no receives WideChar.
		// - int cchWideChar (size of lpWideCharStr)
		//                0: set the paramter for the function returns the required buffer size.
		// * return value : because of cchWideChar is 0, so returns the required buffer size of lpWideCharStr
		int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
		if(nRetLen == 0) return NULL;
		//  allocate space for unicode_buf
		wchar_t* unicode_buf = XMem::createArrayMem<wchar_t>(nRetLen + 1);
		if(unicode_buf == NULL) return NULL;
		// covert the src to utf-8, and store in unicode_buf
		MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);

		// *function: WideCharToMultiByte (Maps a UTF-16 (wide character) string to a new character string. )
		// - UINT CodePage (Code page to use in performing the conversion. )
		//                CP_UTF8: With this value set, lpDefaultChar and lpUsedDefaultChar must be set to NULL.
		// - DWORD dwFlags (Flags indicating the conversion type. )
		//                0 :
		// - LPCWSTR lpWideCharStr (Pointer to the Unicode string to convert.)
		//                unicode_buf : the word that you want to conver
		// - int cchWideChar (you want to process size of lpWideCharStr)
		//                -1: the function processes the entire input string, including the terminating null character. when the input string
		//                        not contains terminating null character, it will failure.
		// - LPSTR lpMultiByteStr (Pointer to a buffer that receives the converted string.)
		//                NULL : no receives MultiByteStr.
		// - int cbMultiByte (size of lpMultiByteStr)
		//                0: set the paramter for the function returns the required buffer size.
		// - LPCSTR lpDefaultChar (Pointer to the character to use if a character cannot be represented in the specified code page. )
		//                NULL : For the CP_UTF7 and CP_UTF8 settings for CodePage, this parameter must be set to NULL.
		// - LPBOOL lpUsedDefaultChar (Pointer to a flag that indicates if the function has used a default character in the conversion.)
		//                NULL : For the CP_UTF7 and CP_UTF8 settings for CodePage, this parameter must be set to NULL.
		nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
		//  allocate space for buf
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
		if(buf == NULL) 
		{
			XMem::XDELETE_ARRAY(unicode_buf);
			return NULL;
		}
		WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
		// release space of unicode_buf
		XMem::XDELETE_ARRAY(unicode_buf);

		return buf;
	}
	char* WcharToANSI(const wchar_t* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
		char * buf = XMem::createArrayMem<char>(nRetLen + 1);
		if(buf == NULL) return NULL;
		WideCharToMultiByte(CP_ACP, 0, src, -1, buf, nRetLen, NULL, NULL);
		return buf;
	}
	int getStrLen(const char * p)	//����ַ����ı����ַ�����
	{
		int len = 0;
		int length = strlen(p);
		for(int i = 0;i < length;++ i)
		{
			if(p[i] < 0) 
				++ i;
			++ len;
		}
		return len;
	}
	int getStrLen(const char * p, int len)
	{
		int length = 0;
		for (int i = 0; i < len; ++i)
		{
			if(p[i] < 0) 
				++ i;
			++length;
		}
		return length;
	}

	std::string g_strDigit[] = {"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"};
	std::string g_strDigitEx[] = {"��", "һ", "��", "��", "��", "��", "��", "��", "��", "��"};
	std::string g_strUnit[] = {"Բ", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��","ʰ", "��", "Ǫ", "��", "ʰ", "��"};
	std::string g_strUnitEx[] = {"Ԫ", "ʮ", "��", "ǧ", "��", "ʮ", "��", "ǧ", "��","ʮ", "��", "ǧ", "��", "ʮ", "��"};
	std::string g_strUnit2[] = {"��", "��"};
	int replaceSubStr(std::string &strOrig,std::string strSub,const std::string &strReplace)
	{
		int pos = (int)strOrig.find(strSub);
		int length = (int)strSub.length();
    
		if (pos >= 0)
		{
			strOrig.replace(pos, length, strReplace);
			return 0;
		}
    
		return -1;
	}
	void replaceSubStrAtHead(std::string &strOrig,std::string strSub,const std::string &strReplace)
	{
		int pos = (int)strOrig.find(strSub);
		int length = (int)strSub.length();
    
		if (pos == 0)
			strOrig.replace(pos, length, strReplace);
	}
	std::string floatToRMB(float sum)
	{
		#define MAX_NUMBER_LEN 100
		int i               = 0;
		int ret             = 0;
		int length          = 0;
		char *p             = NULL;
		char *pcDecimal     = NULL; //����С�������ַ�
		char czNumber[MAX_NUMBER_LEN]  = {0};  //�����������ֲ����ַ�
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//�ж��Ƿ�ΪС��
		if (sum < 0)
		{
			strResult = "��֧�ֶ�����";   
			return strResult;   
		}

		//������תΪ�����ַ���������sprintf_s������ת��
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%.15lg", sum);
		//printf("[No.0]%s\n", czNumber); 

		//���������̫���̫С��������Ϊ�Ѿ�תΪ��ѧ���������ԻẬ��e�ַ�
		p = strchr(czNumber,'e');  
		if (NULL!=p) 
		{
			strResult = "��֧�ֶ�̫���̫С����";
			return strResult;
		}
    
		p = strchr(czNumber, '.');  
		if (NULL != p) 
		{       
			p[0] = 0;    
			pcDecimal = p + 1;   
		}    
		length = (int)strlen(czNumber);  
    
		for (i = 0; i<length; i++) 
		{        
			if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
			{
				strResult += g_strDigit[czNumber[i] - '0'];
			}else 
			{
				strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
			}   
		}
		//std::cout << "[No.1]������ֱ���滻Ϊ����: \n" << strResult << std::endl;

		//��strResult�е�����"����"�Ӵ��滻Ϊ"��"
		while (1)
		{
			ret = replaceSubStr(strResult, "����", "��");
			if (ret < 0)
			{
				break;
			}
		}
		//std::cout << "[No.2]�滻��������Ϊ��: \n" << strResult << std::endl;

		replaceSubStr(strResult, "����", "��");
		replaceSubStr(strResult, "����", "��");
		if (strResult != "��Բ")    //�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
		{
			replaceSubStr(strResult, "��Բ", "Բ");
		}
    
		//std::cout << "[No.3]ȥ�����ڡ�������Բǰ�����: \n" << strResult << std::endl;

		//С����ȷ����λ��������ȷ����λ��
		if (NULL != pcDecimal) 
		{
			//���С����������ֵ����������Ϊ0����ɾ���ַ����е���Ԫ
			if (strResult == "��Բ")
			{
				strResult.clear();
			}
			i = 0;
			while (1) 
			{           
				if (0 == pcDecimal[i] || i >= 2) 
					break;   
				strResult += g_strDigit[pcDecimal[i] - '0'] + g_strUnit2[i];
				i++;      
			}   
		}
		//std::cout << "[No.4]С����ȷ����λ��������ȷ����λ��: \n" << strResult << std::endl;
    
		return strResult;
	}
	std::string floatToChinese(float sum,bool smallStr)
	{
		#define MAX_NUMBER_LEN 100
		int length          = 0;
		char *p             = NULL;
		char *pcDecimal     = NULL; //����С�������ַ�
		char czNumber[MAX_NUMBER_LEN]  = {0};  //�����������ֲ����ַ�
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//�ж��Ƿ�ΪС��
		if (sum < 0)
		{
			strResult = "��֧�ֶ�����";   
			return strResult;   
		}

		//������תΪ�����ַ���������sprintf_s������ת��
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%.6f", sum);
		//printf("[No.0]%s\n", czNumber); 

		//���������̫���̫С��������Ϊ�Ѿ�תΪ��ѧ���������ԻẬ��e�ַ�
		p = strchr(czNumber,'e');  
		if (NULL!=p) 
		{
			strResult = "��֧�ֶ�̫���̫С����";
			return strResult;
		}
    
		p = strchr(czNumber, '.');  
		if (NULL != p) 
		{       
			p[0] = 0;    
			pcDecimal = p + 1;   
		}    
		length = (int)strlen(czNumber);  
    
		if(smallStr)
		{
			for (int i = 0; i<length;++ i) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
					strResult += g_strDigitEx[czNumber[i] - '0'];
				else 
					strResult += g_strDigitEx[czNumber[i] - '0'] + g_strUnitEx[length-1-i];
			}
		}else
		{
			for (int i = 0; i<length;++ i) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
					strResult += g_strDigit[czNumber[i] - '0'];
				else 
					strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
			}
		}
		//std::cout << "[No.1]������ֱ���滻Ϊ����: \n" << strResult << std::endl;

		//��strResult�е�����"����"�Ӵ��滻Ϊ"��"
		if(smallStr)
		{
			while(replaceSubStr(strResult, "����", "��") >= 0)
			{}
			//std::cout << "[No.2]�滻��������Ϊ��: \n" << strResult << std::endl;

			replaceSubStr(strResult, "����", "��");
			replaceSubStr(strResult, "����", "��");
			if (strResult != "��Ԫ")    //�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
				replaceSubStr(strResult, "��Ԫ", "Ԫ");
			//�����ǰ��������"һʮ"��Ӧ��ȥ��"һ"
			replaceSubStrAtHead(strResult,"һʮ","ʮ");
		}else
		{
			while(replaceSubStr(strResult, "����", "��") >= 0)
			{}
			//std::cout << "[No.2]�滻��������Ϊ��: \n" << strResult << std::endl;

			replaceSubStr(strResult, "����", "��");
			replaceSubStr(strResult, "����", "��");
			if (strResult != "��Բ")    //�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
				replaceSubStr(strResult, "��Բ", "Բ");
			//�����ǰ��������"һʮ"��Ӧ��ȥ��"һ"
			replaceSubStrAtHead(strResult,"Ҽʰ","ʰ");
		}
    
		//std::cout << "[No.3]ȥ�����ڡ�������Բǰ�����: \n" << strResult << std::endl;

		//С����ȷ����λ��������ȷ����λ��
		if (NULL != pcDecimal) 
		{
			int pcDecimalLen = 5;
			for(pcDecimalLen = 5;pcDecimalLen >= 0;-- pcDecimalLen)
			{
				if(pcDecimal[pcDecimalLen] != '0') break;
			}
			if(pcDecimalLen <= 0)
			{//���С������ȫ��Ϊ0��   
				if(smallStr)
					replaceSubStr(strResult, "Ԫ", "");
				else
					replaceSubStr(strResult, "Բ", "");
			}else
			{
				if(smallStr)
				{
					replaceSubStr(strResult, "Ԫ", "��");
					//����ת��С������
					for(int i = 0;i <= pcDecimalLen;++ i)
					{
						strResult += g_strDigitEx[pcDecimal[i] - '0'];
					}
				}else
				{
					replaceSubStr(strResult, "Բ", "��");
					//����ת��С������
					for(int i = 0;i <= pcDecimalLen;++ i)
					{
						strResult += g_strDigit[pcDecimal[i] - '0'];
					}
				}
			}
		}else
		{
			if(smallStr)
				replaceSubStr(strResult, "Ԫ", "");
			else
				replaceSubStr(strResult, "Բ", "");
		}
		//std::cout << "[No.4]С����ȷ����λ��������ȷ����λ��: \n" << strResult << std::endl;
    
		return strResult;
	}
	std::string intToChinese(int sum,bool smallStr)
	{
		#define MAX_NUMBER_LEN 100
		char czNumber[MAX_NUMBER_LEN]  = {0};  //�����������ֲ����ַ�
		std::string strResult;

		//std::cout << "======================================" << std::endl;
		//std::cout << sum << std::endl;

		//�ж��Ƿ�ΪС��
		if (sum < 0)
		{
			strResult = "��֧�ֶ�����";   
			return strResult;   
		}

		//������תΪ�����ַ���������sprintf_s������ת��
		sprintf_s(czNumber, MAX_NUMBER_LEN, "%d", sum);
		//printf("[No.0]%s\n", czNumber); 

		//���������̫���̫С��������Ϊ�Ѿ�תΪ��ѧ���������ԻẬ��e�ַ�
		char *p = strchr(czNumber,'e');  
		if (NULL != p) 
		{
			strResult = "��֧�ֶ�̫�����";
			return strResult;
		}  
		int length = (int)strlen(czNumber);  
    
		if(smallStr)
		{
			for (int i = 0; i<length; i++) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
				{
					strResult += g_strDigitEx[czNumber[i] - '0'];
				}else 
				{
					strResult += g_strDigitEx[czNumber[i] - '0'] + g_strUnitEx[length-1-i];
				}   
			}
		}else
		{
			for (int i = 0; i<length; i++) 
			{        
				if ('0' == czNumber[i] && 0 != ((length-1-i) % 4))
				{
					strResult += g_strDigit[czNumber[i] - '0'];
				}else 
				{
					strResult += g_strDigit[czNumber[i] - '0'] + g_strUnit[length-1-i];
				}   
			}
		}
		//std::cout << "[No.1]������ֱ���滻Ϊ����: \n" << strResult << std::endl;

		//��strResult�е�����"����"�Ӵ��滻Ϊ"��"
		if(smallStr)
		{
			while(replaceSubStr(strResult, "����", "��") >= 0)
			{}
			//std::cout << "[No.2]�滻��������Ϊ��: \n" << strResult << std::endl;

			replaceSubStr(strResult, "����", "��");
			replaceSubStr(strResult, "����", "��");
			if (strResult != "��Ԫ")    //�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
			{
				replaceSubStr(strResult, "��Ԫ", "Ԫ");
			}
			//�����ǰ��������"һʮ"��Ӧ��ȥ��"һ"
			replaceSubStrAtHead(strResult,"һʮ","ʮ");
		}else
		{
			while(replaceSubStr(strResult, "����", "��") >= 0)
			{}
			//std::cout << "[No.2]�滻��������Ϊ��: \n" << strResult << std::endl;

			replaceSubStr(strResult, "����", "��");
			replaceSubStr(strResult, "����", "��");
			if (strResult != "��Բ")    //�����������ȫΪ0����Ҫȥ��Ԫ��λǰ�����
			{
				replaceSubStr(strResult, "��Բ", "Բ");
			}
			//�����ǰ��������"һʮ"��Ӧ��ȥ��"һ"
			replaceSubStrAtHead(strResult,"Ҽʰ","ʰ");
		}
    
		//std::cout << "[No.3]ȥ�����ڡ�������Բǰ�����: \n" << strResult << std::endl;
		if(smallStr)
			replaceSubStr(strResult, "Ԫ", "");
		else
			replaceSubStr(strResult, "Բ", "");
		//std::cout << "[No.4]С����ȷ����λ��������ȷ����λ��: \n" << strResult << std::endl;
    
		return strResult;
	}
	wchar_t oneCharToWchar(const char* src)
	{
		if(src == NULL) return NULL;
		wchar_t ret = 0;
		MultiByteToWideChar(CP_ACP, 0, src, 2,&ret, 1);
		return ret;
	}
	std::string Wchar2ANSI(const wchar_t* src)
	{
		if(src == NULL) return NULL;
		int nRetLen = WideCharToMultiByte(CP_ACP, 0,src, -1, NULL, 0, NULL, NULL);
 		std::string ret;
		ret.resize(nRetLen + 1);
		WideCharToMultiByte(CP_ACP, 0, src, -1, &ret[0], nRetLen, NULL, NULL);
		return ret;
	}
	int removeAllSpace(char *str, int strlen)
	{//��δʵ��
		return 0;
	}
#if !WITH_INLINE_FILE
#include "XStringFun.inl"
#endif
}
}