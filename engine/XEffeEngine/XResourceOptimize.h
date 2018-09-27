#ifndef _JIA_XRESOURCEOPTIMIZE_
#define _JIA_XRESOURCEOPTIMIZE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "XTextureInformation.h"
#include "XCommonDefine.h"
#include <vector>
namespace XE{
#define PNG_FILE_LIST_NAME "pngFileList.txt"
#define PNG_INFORMATION_FILE_NAME "textureInformation.txt"

//��Դ�Ż��ʹ��ͳһ����֮������Ż������һ��ͼ�����Ż��������Ҫ���洦��
#define MAX_PIXELS_OFFSET (0)	//ͼƬ�Ż�ʱ�����ͼƬ���ص��ݲ�

struct XPicProcData;
class XResourceOptimize
{
public:
	XResourceOptimize()
		:m_isOptimizeInMem(true)
	{}
	virtual ~XResourceOptimize() {/*clearTempFile();*/ }
protected:
	XResourceOptimize(const XResourceOptimize&);
	XResourceOptimize &operator= (const XResourceOptimize&);
public:
	//�������ⲿ���õĽӿ�
	//optInMem�Ƿ����ڴ��н����Ż�,�����ٶȽϿ�,���������Դ�϶�ʱ������ڴ�ľ�
	//withOpti�Ƿ�ʹ��ͳһ����ͼ��С�������ʹ��ͳһ����ͼ��С���Ż�Ч������
	bool optimize(bool optInMem = false, bool withOpti = false);		//�Ż�
	void clearFolderSet()		//ɾ�������ļ���ɸѡ�趨
	{
		m_needOptimizeFolder.clear();
		m_needNotOptimizeFolder.clear();
	}
	void addNeedFolder(const std::string& folder);		//������Ҫ������ļ���
	void addNeedNotFolder(const std::string& folder);	//���Ӳ���Ҫ������ļ���
	void clearTempFile();	//�����ʱ�ļ�
private:
	bool m_isOptimizeInMem;	//�Ƿ����ڴ��н����Ż�����ͼƬ��������ʱ������ٶȺܿ죬��ͼƬ�����϶�ʱ�����������ڴ治����˳�	(δ��Ч)
	//m_needOptimizeFolder��m_needNotOptimizeFolderֻȡ��һ����m_needOptimizeFolder�нϸ����ȼ�	
	std::vector<std::string> m_needOptimizeFolder;		//��Ҫ������ļ��У�ֻ����������ļ���	(δ��Ч)
	std::vector<std::string> m_needNotOptimizeFolder;	//����Ҫ������ļ��У�ȫ������������ָ�����ļ���	(δ��Ч)

	bool writeFileData(std::vector<XPicProcData> &picData, int allOutputSum);	//���Ż�֮�������д�뵽�����ļ���
	int makePNGFileList();
	bool getPictureRect(XPicProcData& result, int maxPixelsOffset);				//�����ݲ����ͼƬ��Ч����ķ�Χ
	int imagePack(std::vector<XPicProcData>& picData, bool withOpti = false);							//��ͼƬ���ݽ���ƴ��
	bool imageCompare(XPicProcData& tex0, XPicProcData& tex1, int maxPixelsOffset);	//�Ƚ�������Ƭ�Ƿ���ͬ���߲����Ƿ���maxPixelsOffsetԼ�����ݲΧ��
	bool imageCompareEx(XPicProcData& tex0, XPicProcData& tex1, int maxOffset, 
		int maxPixelsOffset, float maxDiffRate);
};
}
#endif