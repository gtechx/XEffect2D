#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
class GGame:public XE::XGameBasic,public XE::XDragObjectBase
{
public:
	virtual ~GGame(){}
	GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void ctrlEventProc(int id,int eventID){}//TODO:�ؼ�����Ϣ������

	virtual void setPosition(const XE::XVec2& pos) { setPosition(pos.x, pos.y); }
	virtual void setPosition(float x,float y);
	virtual const XE::XVec2& getPosition()const { return m_winPos; }
	virtual XE::XBool getIsInDragRect(const XE::XVec2& pos) { return getIsInDragRect(pos.x, pos.y); }
	virtual XE::XBool getIsInDragRect(float x,float y);//�ж�λ���Ƿ�����ק������
	virtual XE::XVec2 correctPos(const XE::XVec2 &pos) {return pos;}	//���ߴ������קλ�ã�������λ�ã���λ�ó��ڷǷ�״̬ʱ�������ɺϷ���λ��

	XE::XSprite m_sprite;
	XE::XProgressRing m_progress;
	XE::XButton m_button;
	XE::XVec2 m_winPos;
};

#endif