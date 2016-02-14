#ifndef _JIA_XMODELSTL_
#define _JIA_XMODELSTL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "X3DWorld.h"
#include "XShaderGLSL.h"
namespace XE{
//����һ��Stlģ�Ͳ�������
/*
������STL�ļ��ù̶����ֽ���������������Ƭ�ļ�����Ϣ���ļ���ʼ��80���ֽ����ļ�ͷ��
���ڴ����ļ�������������4���ֽڵ�����������ģ�͵�������Ƭ�����������������ÿ����
����Ƭ�ļ�����Ϣ��ÿ��������Ƭռ�ù̶���50���ֽڣ�������3��4�ֽڸ�����(����Ƭ�ķ�
ʸ��)3��4�ֽڸ�����(1�����������)3��4�ֽڸ�����(2�����������)3��4�ֽڸ�����(3��
���������)��������Ƭ�����2���ֽ���������������Ƭ��������Ϣ��һ������������STL��
���Ĵ�СΪ��������Ƭ������50�ټ���84���ֽڣ��ܹ�134���ֽڡ�

�ļ���		80Bytes
faceSum		4Bytes
faceInfo	50Bytes
	normal	3 * 4Bytes
	vector	3 * 4 * 3Bytes	
	����	2Byts
	����
*/

class XModelStl:public XBasic3DObject
{
private:
	bool m_isInited;

	unsigned char m_modelName[80];	//ģ�͵�����
	int m_faceSum;
	float *m_normalData;	//��������
	float *m_vectorData;	//��������
	XVBO vbo;
public:
	bool load(const char * filename);
	void draw();
	void release()
	{
		if(!m_isInited) return;
		XMem::XDELETE_ARRAY(m_normalData);
		XMem::XDELETE_ARRAY(m_vectorData);
		vbo.release();
		m_isInited = false;
	}
	XModelStl()
		:m_isInited(false)
		,m_normalData(NULL)
		,m_vectorData(NULL)
	{}
	~XModelStl(){release();}
};
}
#endif