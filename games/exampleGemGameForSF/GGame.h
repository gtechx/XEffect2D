#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "XResourcePack.h"
#include "XLoading.h"
#include "XBasicFun.h"

#include "XGun.h"
#include "XGem.h"

#define WINDOWS_WIDTH (1280)
#define WINDOWS_HIGHT (720)
#define MAX_GUN_ROTATE_TIMER (100)	//ǹ��ת���������ٶ�
#define MIN_GUN_ROTATE_TIMER (7)	//ǹ��ת��������ٶ�

class GGame:public XE::XGameBasic
{
public:
	GGame()
		:keyShootDownState(0)
		, nowGunRotateTimer(MAX_GUN_ROTATE_TIMER)
		, keyMoveUpDownState(0)
		, keyMoveDownDownState(0)
		, mouseMoveStateUD(0)
		, mouseMoveStateLR(0)
		, mouseMoveStateLRC(0)
		, g_pJoy00(NULL)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
//	static GGame& GetInstance()
//	{
//		static GGame m_instance;
//		return m_instance;
//	}
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void ctrlEventProc(int id,int eventID){}//TODO:�ؼ�����Ϣ������

	XE::XGun gameGun;
	XE::XGemMatrix gameGemMatrix;
	XE::XLoading Loading;

	char keyShootDownState;
	int keyShootTimer;
	float nowGunRotateTimer;
	char keyMoveUpDownState;
	int keyMoveUpTimer;
	char keyMoveDownDownState;
	int keyMoveDownTimer;
	char mouseMoveStateUD;
	char mouseMoveStateLR;
	char mouseMoveStateLRC;
	void keyProc(int interval);
#if MAP_EDIT_MODE
	XE::XVector2 mousePoint;
#endif
#if PC_FREE_VERSION		//��������İ汾����Щ���ܱ��ر�
	GLuint m_gameAllTex;
#endif
	SDL_Joystick *g_pJoy00;
};

#define _GGameMain GGame::GetInstance()

#endif