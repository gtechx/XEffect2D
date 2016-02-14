#ifndef _JIA_XGAMEBASIC_
#define _JIA_XGAMEBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.4.2
//--------------------------------
namespace XE{
//������Ϸ����Ļ���
class XGameBasic
{
private:
public:
	virtual bool init() = 0;							//TODO:���������Ϸ�ĳ�ʼ��
	virtual void move(float stepTime) = 0;				//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	virtual void draw() = 0;							//TODO:���Ƶĺ���
	virtual void input(const XInputEvent &event) = 0;	//TODO:���̻������������¼�
	virtual void release() = 0;							//TODO:�������ʱ����Դ�ͷ�
	virtual void ctrlEventProc(int id,int eventID) = 0;	//TODO����������������д���ؼ��¼�
};
class XGameBasic3D:public XGameBasic
{
private:
public:
	virtual void draw3D(XBool) = 0;							//TODO:���Ƶĺ���
};
}
#endif