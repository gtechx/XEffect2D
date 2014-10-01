#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"

class _GGame
{
protected:
	_GGame(){}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(int stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const _XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
};

#define _GGameMain _GGame::GetInstance()

#endif