#ifndef _JIA_XSTRING_
#define _JIA_XSTRING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
namespace XE{
//�ַ���ƥ�����
class XSunday
{
private:
	static const int m_maxCharsSum = 256;
	XBool m_isInited;
	int *m_shift;
	int m_pattSize;
	char * m_patt;
public:
	XBool init(const char *patt);
	XBool search(const char *text, int &position);
	XSunday();
	~XSunday();
};
namespace XString
{
	//ʹ��sunday�㷨���ַ����в����ƶ����ַ���
	extern XBool sundaySearch(const char *text,const char *patt,int &position);
	//ʹ��kmp�㷨���ַ����в����ƶ����ַ���
	extern XBool kmpSearch(const char *text,const char *m,int &pos);
	static std::string gNullStr = "";
}
}
#endif