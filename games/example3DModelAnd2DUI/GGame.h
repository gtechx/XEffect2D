#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "X3D/X3DWorld.h"

class GGame:public XE::XGameBasic3D
{
public:
	GGame()
		:m_srcData(NULL)
		,m_dstData(NULL)
		,m_mapData(NULL)
		,m_isPlay(false)
		,m_nowTimer(0)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void draw3D(XE::XBool withTex);				//TODO:���Ƶĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void ctrlEventProc(int id,int eventID);

	XE::XText m_filenameTxt;	//�ļ�·��
	XE::XEdit m_filenameEdt;
	XE::XText m_radiusTxt;		//ģ���뾶
	XE::XEdit m_radiusEdt;
	XE::XButton m_updateBtn;	//���°�ť

	XE::XSprite m_srcSprite;
	XE::XSprite m_dstSprite; 
	XE::XTextureData m_srcPic;
	XE::XTextureData m_dstPic;
	unsigned char *m_srcData;
	unsigned char *m_dstData;
	void updateData();
	XE::XTerrain m_tmpTerrain;	//����
	float *m_mapData;	//��������

	XE::XSprite m_backSprite;
	XE::XShaderGLSL m_tmpShader;
	bool m_isPlay;
	int m_nowTimer;
};

#endif