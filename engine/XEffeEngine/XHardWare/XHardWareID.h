#ifndef _JIA_XHARDWAREID_
#define _JIA_XHARDWAREID_
//������������ݷǳ��㷺��Ŀǰֻ���򵥵Ĵ���
#include <string>
struct IWbemLocator;
struct IWbemServices;
namespace XE{
namespace XHardware
{
extern std::string getCPUVID();		//12����Чλ
extern std::string getCPUBrand();	//48����Чλ
//addr�������6���ֽڵĿռ�
extern bool getMACAddress(unsigned char *addr);	//��ȡ����Ӳ��ַ
};
//ͨ��WMI����ȡӲ����Ϣ����
class XHardwareInfo
{
private:
	bool m_isInited;
	IWbemLocator *m_pLoc;
	IWbemServices *m_pSvc;
public:
	bool init();	//��ʼ��
	void release();
	std::string getHardDiskID();	//��ȡӲ��ID
	std::string getMotherboardProduct();	//��ȡ�����ͺ�

	XHardwareInfo()
		:m_isInited(false)
		,m_pLoc(NULL)
		,m_pSvc(NULL)
	{}
	~XHardwareInfo(){release();}
};
}
#endif