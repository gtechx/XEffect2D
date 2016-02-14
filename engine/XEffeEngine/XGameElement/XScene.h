#ifndef _JIA_XSCENE_
#define _JIA_XSCENE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.5.21
//--------------------------------
#include "stdio.h"
#include <string>
#include "XInputEventCore.h"
namespace XE{
class XScene
{
public:
	XScene()
		:m_sceneName("x")
	{ printf("XScene Constructor \n"); }	
	virtual ~XScene(){printf("XScene Destructor::%s\n", m_sceneName.c_str());}

	virtual void setup(){printf("XScene setup::%s\n", m_sceneName.c_str());}
		
	virtual void update(float){}
	virtual void draw(){}

	//�����ļ������ܺ����ӿ�
	virtual void willEnter(){}		//������Ҫ����ʱ����
	virtual void entered(){}		//����������ɵ���
	virtual void willExit(){}		//������Ҫ�˳�ʱ����
	virtual void exited(){}			//�����˳���ɵ���
	
	virtual void input(const XInputEvent &) {}
	
	std::string getSceneName()const{return m_sceneName;}
	void setSceneName(const std::string & name){m_sceneName = name;}
private:
	std::string m_sceneName;
};
}
#endif