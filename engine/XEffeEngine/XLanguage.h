#ifndef _JIA_XLANGUAGE_
#define _JIA_XLANGUAGE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
#include "XOSDefine.h"
#include <string>
namespace XE{
enum XLanguage
{
	XLGG_CN,	//����
	XLGG_EN,	//Ӣ��
};
class XLanguageBasic
{
protected:
	XBool m_isInited;
	XLanguage m_currentLanguage;			//��ǰ������
	virtual XBool loadFromFile(const std::string &filename) = 0;	//���ļ��ж�ȡ�ַ���
	virtual XBool saveToFile(const std::string &filename) = 0;	//���ļ��ж�ȡ�ַ���
public:
	virtual XBool init(XLanguage language,const std::string &filename)
	{
		if(!setCurrentLanguage(language,filename)) return XFalse;
		m_isInited = XTrue;
		return XTrue;
	}
	virtual XBool setCurrentLanguage(XLanguage language,const std::string &filename) = 0;		//���õ�ǰ������
	virtual XLanguage getCurrentLanguage() const {return m_currentLanguage;}

	XLanguageBasic()
		:m_isInited(XFalse)
		,m_currentLanguage(XLGG_CN)
	{}
	virtual ~XLanguageBasic(){}
};
}
#endif