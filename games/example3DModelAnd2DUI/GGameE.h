#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "X3D/X3DWorld.h"

class GGame:public XE::XGameBasic3D
{
public:
	GGame(){}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:����2D�ĺ���
	void draw3D(XE::XBool withTex);				//TODO:����3D�ĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void ctrlEventProc(int id,int eventID);

	XE::XSprite tempSprite;
	XE::XModelObj tempModel;
	XE::XSlider tmpXSld;
	XE::XSlider tmpYSld;
	XE::XSlider tmpZSld;
	XE::XVector3 angle;

	//˵�������VBOЧ����Ҫ��releaseģʽ�²��ܿ�����Ŀǰԭ����
	XE::XVBO tmpVBO;
	float *v;
	float *t;
	float *n;
	unsigned int *index;
	XE::XVector2 mouseVector;
};

#endif