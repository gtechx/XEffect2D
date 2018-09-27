#ifndef _JIA_XCUBEMAP_
#define _JIA_XCUBEMAP_
#include "XPixelCommon.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.1
//--------------------------------
namespace XE{
class XCubeMap
{
private:
	bool m_isInited;	//�Ƿ��ʼ��
	unsigned int m_texture;

	bool loadPic(const std::string& name,int index,XResPos resPos);
	void release();

	bool loadFromFolder(const std::string& filename);	//���ļ�����������Դ
	bool loadFromPacker(const std::string& filename);	//��ѹ������������Դ
	bool loadFromWeb(const std::string& filename);		//����ҳ�ж�ȡ��Դ
public:
	bool init(const std::string& path,XResPos resPos = RES_SYS_DEF);
	void draw();
	unsigned int getTexture() const {return m_texture;}

	XCubeMap()
		:m_isInited(false)
		,m_texture(0)
	{}
	~XCubeMap(){release();}
};
}
#endif