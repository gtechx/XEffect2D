#ifndef _JIA_XGUN_
#define _JIA_XGUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------

//�㵽ֱ�ߵľ��빫ʽ
//ֱ�ߣ�a * x + b * y + c = 0;
//�㣺x0,y0
//���빫ʽ��abs(a * x0 + b * y0 + c)/sqrt(a * a + b * b)
//y=kx-kx1+y1 -> kx - y - kx1 + y1 = 0; k = tan(B)
// tan(B) * x - y - tan(B) * x1 + y1 = 0;
//abs(tan(B) * x0 + (-1) * y0 + (y1 - tan(B) * x1)) /sqrt(tan(b) * tan(b) + 1)

#include "XEffeEngine.h"
#include "XGem.h"
//#include "XCameraX.h"
#include "XMedia\XCamera.h"
#include "XBasicFun.h"
#include "XMath\XMoveNumber.h"
#include "XShaderGLSL.h"
#include "XMedia\XSoundCommon.h"

#define PC_FREE_VERSION (0) //�����������Ѱ汾
namespace XE{
extern XMusicHandle BGMusicHandle[6];	//�����ľ��
//extern XMusic BGmusic;					//���ű������ֵ���

//#define GOLD_RATE (2)			//�ݻٱ�ʯ֮��ʯ�������ý�ҵĶһ���ϵ
#define LINE_BOOM_ENERGY (8)	//���Ե���
#define AREA_BOOM_ENERGY (10)	//��Χ��ը���ߵ�����
#define AREA_BULLET_ENERGY (6)	//��Χ��ը����������
#define ENERGY_ADD_TOOLS (100)	//��������ӵ�������
#define COLOR_BOOM_ENERGY (8)	//��ɫ�ӵ�������
#define TOOLS_DECREASE_ENERGY (200) //���������ĵ��߼��ٵ�����
#define TOOLS_ADD_ENERGY (400)	//���������ĵ������ӵ�����
#define TOOLS_ADD_GOLD (800)	//���ӽ�ҵĵ������ӵĽ��
#define TOOLS_FREE_TIMER (10000)	//������������ʱ��
#define TOOLS_ADD_TIMER (15000) //����ʱ��ĵ������ӵ�ʱ��
#define TOOLS_ENERGY_CHANGE (0.125f) //����ת�����ߵ�ת������
#define GUN_ANGLE_MAX (150.0f)
#define GUN_ANGLE_MIN (30.0f)

//�ӵ��������ٶ�
#define GUN_SHOOT_SPEED_00 (150)
#define GUN_SHOOT_SPEED_01 (240)
#define GUN_SHOOT_SPEED_02 (450)
#define GUN_SHOOT_SPEED_03 (450)
#define GUN_SHOOT_SPEED_04 (300)
#define GUN_SHOOT_SPEED_05 (700)
//�ӵ�������
#define GUN_SHOOT_ENERGY_00 (1)
#define GUN_SHOOT_ENERGY_01 (4)
#define GUN_SHOOT_ENERGY_02 (20)
#define GUN_SHOOT_ENERGY_03 (30)
#define GUN_SHOOT_ENERGY_04 (8)
#define GUN_SHOOT_ENERGY_05 (200)	
#define GUN_SHOOT_ENERGY_X0 (1)	//�ʵ�����
#define MAX_BULLET_SUM (256)
#define LINE_BULLET_DESTROY (20)

#define LEVEL_OVER_ADD_SCORE (2000) //�������ӵķ���

#define ENERGY_WARNNING (100)

#define SAVE_FILE_NAME ("databasic.dat")		//�������ݵĶ�ȡ�ʹ�Ŷ���D��
#define SAVE_FILE_NAME_HEAD ("dataHead.dat")	//�������ݵĶ�ȡ�ʹ�Ŷ���D��
#define SAVE_FILE_NAME_USER ("dataUser.dat")	//�������ݵĶ�ȡ�ʹ�Ŷ���D��

#define USER_HEAD_SUM (20)

#define USE_FBO (0)		//�Ƿ�ʹ��FBO��������ը��Ч���Լ���βЧ��
#define USE_SHADER (0)	//�Ƿ�ʹ��shader���������β��ɵĲ�Ӱ

enum XBULLET_TYPE
{
	BULLET_TYPE_00,		//��ǹɨ���ӵ�
	BULLET_TYPE_01,		//��ھ�����
	BULLET_TYPE_02,		//����
	BULLET_TYPE_03,		//��������
	BULLET_TYPE_04,		//�����
	BULLET_TYPE_05,		//������
	BULLET_TYPE_06,		//x
	BULLET_TYPE_07,		//x
	BULLET_TYPE_08,		//x
	BULLET_TYPE_X0,		//ɫ�ʵ�
	BULLET_TYPE_X1,		//������ǿ���ӵ�
};

//�ߴ� 32 x 64
class XBulletTexture
{
private:
	char m_isInited;
public:
	XTextureData m_bulletTex_00;
	XTextureData m_bulletTex_01;
	XTextureData m_bulletTex_02;
	XTextureData m_bulletTex_03;
	XTextureData m_bulletTex_04;
	XTextureData m_bulletTex_05;
	XTextureData m_bulletTex_X0;	//ɫ�ʵ����ӵ�
	XTextureData m_bulletTex_X1;	//������ǿ���ӵ�
	XFrameEx m_bulletFrame;	//�ӵ��Ķ���
	XFrameEx m_bulletBoom;	//�ӵ��Ķ���
	int init(XResPos resoursePosition = RES_LOCAL_FOLDER);
	XBulletTexture();
};

#pragma pack(push)
#pragma pack(1)
struct XConfigData
{
	char isTicketNeed;	//�Ƿ���Ҫ����Ʊ
	char ticketSum;		//�١�һ�㡢��
	char isAutoShoot;	//�Ƿ��Զ����
	char coinToCredit;	//���ٱ���һ��
	int oneCreditEnergy;	//һ�����ܵ���
	int volume;
	int allInCoin;	//��Ͷ����
	int allOutTicket;	//�ܳ���Ʊ��
	char levelSum;	//������Ĺؿ���,6,7,8,9,������
};
#pragma pack(pop)

class XBullet
{
private:
public:
	XVec2 m_position;	//�ӵ����ڵ�λ��
	XVec2 m_speed;		//�ӵ����ٶ�
	XMoveData m_nowSpeedData;	//���ڸ��ٵ��ļ��ٹ���

	float m_energy;			//�ӵ�������
	float m_angle;			//�ӵ��ĽǶ�

	XBULLET_TYPE m_bulletType;	//�ӵ�������

	int m_bulletStage;		//�ӵ��Ľ׶� 0:��̬�ս�׶�
	float m_bulletAlpha;	//�ӵ���alpha
	int m_bulletAlphaStage;	//�ӵ�alpha�Ľ׶�

	XBulletTexture *m_tex;

	XSprite m_bulletSprite_00;
	XSprite m_bulletSprite_01;
	XFrameEx m_bulletFrame;

	float m_armAngle;		//Ŀ��Ƕ�
	int m_armAngleFlag;	//Ŀ��Ƕ��Ƿ���Ч
public:
	int init(XVec2 position,XVec2 speed,float energy,float angle,XBulletTexture *tex,XBULLET_TYPE type);
	void draw();
	int move(int interval);	//�����Ƿ����߽���ײ��1�ϱ߽���ײ��2�±߽���ײ
	int getIsEnd();			//�ӵ��Ƿ��Ѿ���Ч
	int getCanCrash();		//�ӵ��Ƿ�����˺�����
	void setOver()
	{
		m_energy = 0.0f;
		if(m_bulletType == BULLET_TYPE_00 || m_bulletType == BULLET_TYPE_01 || m_bulletType == BULLET_TYPE_03 || m_bulletType == BULLET_TYPE_04)
		{
			m_bulletStage = 2;
		}else
		{
			m_bulletStage = 0;
		}
	}

	XBullet();
};

enum XGUN_TYPE
{
	GUN_TYPE_NULL,	
	GUN_TYPE_00,	//����ǹ
	GUN_TYPE_01,	//��ھ�����
	GUN_TYPE_02,	//����
	GUN_TYPE_03,	//��������
	GUN_TYPE_04,	//�����
	GUN_TYPE_05,	//������
};

enum TOOLS_TYPE
{
	TOOLS_TYPE_00,	//�������ĵ���
	TOOLS_TYPE_01,	//�ӽ�ҵĵ���
	TOOLS_TYPE_02,	//���ת�����ĵ���
	TOOLS_TYPE_03,	//10s��������ĵ���
	TOOLS_TYPE_04,	//��ʱ��ĵ���

	TOOLS_TYPE_05,	//��ҷ���
	TOOLS_TYPE_06,	//ʱ�����
	TOOLS_TYPE_07,	//�����ӱ�
	TOOLS_TYPE_08,	//���ټӿ�
	TOOLS_TYPE_09,	//���ļ���
};

enum GAME_STAGE
{
	GAME_STAGE_GAMELOGO,	//��ϷDEMO����
	GAME_STAGE_GAMEHEAD,	//ÿ�ؿ�ʼ�ļ�¼
	GAME_STAGE_GAMEPLAY,	//��Ϸ����
	GAME_STAGE_GAMEWIN,		//��Ϸʤ�����
	GAME_STAGE_GAMELOST,	//��Ϸʧ�ܽ���
	GAME_STAGE_GAMEINPUTNAME,	//�����������
	GAME_STAGE_GAMESHOWORDER,	//��ʾ���а�
	GAME_STAGE_GAMEALLOVER,	//��Ϸͨ��
	GAME_STAGE_INSTRUCTION,	//��Ϸ���ܽ���
};
#define SMALL_STAR_SUM (256)

#define CAMERA_WIDTH (320)	//����ͷ�Ŀ��
#define CAMERA_HIGHT (240)	//����ͷ�ĸ߶�
#define CAMERA_TEX_WIDTH (512)	//����ͷ�Ŀ��
#define CAMERA_TEX_HIGHT (256)	//����ͷ�ĸ߶�
#define MAX_GAME_ORDER_SUM (10)
#define MAX_GAME_ORDER_NAME_LENGTH (7)	//��Ҫ����15
#define MAX_GAME_ORDER_CAMERA_DATA (CAMERA_WIDTH * CAMERA_HIGHT * 3)	//ͷ������ݴ�С(10����λ)
enum INPUT_NAME_KEY
{
	INPUT_KEY_LEFT,
	INPUT_KEY_RIGHT,
	INPUT_KEY_ENTER,
};

enum CONFIG_KEY
{
	CONFIG_KEY_LEFT,
	CONFIG_KEY_RIGHT,
	CONFIG_KEY_UP,
	CONFIG_KEY_DOWN,
	CONFIG_KEY_SHOOT,
};

class XGun
{
private:
	XSprite m_cameraLogo;	//����ͷ��ͼ��
	XSprite m_cameraNotSupply;	//����ͷ��֧�ֵ�ͼ��
	float m_notSupplyAlpha;

	//XCameraX m_camera;
	XCamera m_camera;
	XSprite m_cameraBG;
	//XSprite m_cameraHead[MAX_GAME_ORDER_SUM];	//10��ͷ��
	XSprite m_cameraHead;	//10��ͷ��
	GLuint m_cameraRecordTex[MAX_GAME_ORDER_SUM];		//10��ͷ�����ͼ
	GLuint m_userHeadTex[USER_HEAD_SUM];		//10��ͷ�����ͼ
	XSprite m_takePhotoAttention;				//������ʾ
	char m_takePhotoNeedShow;					//�Ƿ���Ҫ��ʾ������ʾ
	char m_takePhotoCanPressKey;				//�Ƿ���ܰ�����Ϣ	0:�����ܰ��� 1���ܰ��� 2�Ѿ��������������ܰ���
	char m_takePhotoKeyFlag;					//������ʾʱ�İ�������
	char m_takePhotoPoint;						//��ǰ���յĲ����
	char m_isCameraSupply;						//�Ƿ�������ͷ֧��

	XSprite m_userHeadSprite[4];				//��̬��ʾ�����ͼ��sprite
	XFont  m_userheadName[4];
	char m_nowUserHeadOrder;					//��ǰ��ʾ����ҵ�ͷ��ı��
	float m_nowUserHeadData;					//��ǰ��ʾ�����ͷ��Ĳ���

	void userHeadMove(int delay,int h);
	void userHeadDraw();
public:
	void pressShoot()
	{
		if(m_isCameraSupply == 0) return;
		if(m_takePhotoCanPressKey != 0) 
		{
			m_takePhotoCanPressKey = 2;
			m_takePhotoKeyFlag = 1;
		}
	}
	void pressChangeGun()
	{
		if(m_isCameraSupply == 0) return;
		if(m_takePhotoCanPressKey != 0) 
		{
			m_takePhotoCanPressKey = 2;
			m_takePhotoKeyFlag = 2;
		}
	}
private:
	void updataHeadTex(int order);
	void updataUserHeadTex(int order);
	//����������ʱ�Ķ���Ч��
	XSprite m_takePhotoTex00;
	XSprite m_takePhotoTex01;
	XSprite m_takePhotoTex02;
	XSprite m_takePhotoTex03;
	float m_takePhotoChange;

	//��Ʊ�Ľ�����
	XSprite m_ticketBar;
	XSprite m_ticketBarDown;
	XFrameEx m_toolsBoom;	//�����߱�ըЧ������Ч
	char m_toolsBoomFlag;

	XResPos m_resoursePosition;
	int m_BGPicOrder;	//Ӧ����ʾ�ı������
	XSprite m_backGround_00;
	XSprite m_backGround_01;
	XSprite m_backGround_02;
	XSprite m_backGround_03;
	XSprite m_backGround_04;
//	char m_backGroundColor;
//	int m_backGroundTimer;
	float m_backGroungPosition;
	XSprite m_star[13];
	char m_starFlag[13];
	XVec2 m_starSpeed[13];
	XVec2 m_starPosition[13];

	XSprite m_BGSider[2];
	XMoveData m_BGSiderMoveData[2];

//+++++++++++++++++++++++++++++++++++++++
//�����ǹ���С���ǵ�һЩ����
	XSprite m_smallStar[6];
	//float m_smallStarM;	//С���ǵ��ܶȣ���������С���ǵĳ��ִ���
	char m_smallStarType[SMALL_STAR_SUM];
	char m_smallSrarStage[SMALL_STAR_SUM];	//��ǰС���ǵĽ׶�
	XVec2 m_smallStarPosition[SMALL_STAR_SUM];	//��ǰС���ǵ�λ��
	XVec2 m_smallStarSize[SMALL_STAR_SUM];		//��ǰС���ǵĴ�С
	XVec2 m_smallStarSpeed[SMALL_STAR_SUM];		//��ǰС���ǵ��ٶ�
	XMoveData m_smallStarMoveData;

	int smallStarInit(XResPos resoursePosition);
	void smallStarMove(int delay);
	void smallStarDraw();
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//�����ǹ�������ͷ��Ķ���
	XSprite m_girlFace[6];		//����ͷ��
	XSprite m_faceBG;			//����˵���ı���
	XFontUnicode m_faceText;	//����Ի�������
	int m_faceStage;			//��ʾ����Ի��Ľ׶��Ա�־
	XMoveData m_faceMoveData;
	int m_nowFaceOrder;
	int m_faceTextSum;
	int m_faceTextTimer;		//��ʾ���ֵļ�ʱ��
	int m_faceGameTimer;		//��Ϸ��ʱ
	char m_faceTextRandom;

	int faceInit(XResPos resoursePosition);
	void faceMove(int delay);
	void faceDraw();
//public:
	void setFaceStart()
	{
		if(m_faceStage == 0) m_faceStage = 1;
	}
//private:
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//�����ǹ��ڲ�ӰЧ����һЩ����
#if USE_FBO
	XFBO m_fbo;
	XSprite m_fboSprite;
	XSprite m_fboShake;
	int m_fboOrder;
	XMoveData m_fboShakeData;
#if USE_SHADER
	void initShaderFromText();	//���ļ��г�ʼ��sheder�Ĵ���
#endif
public:
	void useFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fbo.useFBO();
			m_fbo.attachTex(2);
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	void removeFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fbo.removeFBO();
		}
	}
	void drawFBO()
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			GLuint tempTex = m_fbo.getTexture(2);
			m_fboShake.draw(&tempTex);
		}
	}
	void moveFBO(int delay)
	{
		if(m_fboShakeData.getIsEnd() == 0)
		{
			m_fboShakeData.move(delay);
			float randomA = XRand::random(360) * DEGREE2RADIAN;
			m_fboShake.setPosition(m_fboShakeData.getCurData() * sin(randomA),m_fboShakeData.getCurData() * cos(randomA));
		}
	}
private:
#endif
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//�����ǹ������״̬�ı���
	XSprite m_userBuffLogo[6];	//���״̬�ı�־�������ǿ��ʱ�������������ǿ�����ټӿ죬���ļ��룬����ȡ��
	char m_userBuffFlag[6];	//���״̬�ı�־	//״̬һ�ζ�Ӧ���¼����5��״̬����	
	int m_userBuffReleaseTime[6];	//״̬��ʣ��ʱ��
	XMoveData m_userBuffData[6];

	int m_userBuffTimer;	//������ǿʱ����Ч��ʱ

	int userBuffInit(XResPos resoursePosition);
	void userBuffMove(int delay);
	void userBuffDraw();
	void clearAllBuff()
	{
		for(int i = 0;i < 6;++ i)
		{
			if(m_userBuffFlag[i] > 0 && m_userBuffFlag[i] < 6) m_userBuffFlag[i] = 6;
			else m_userBuffFlag[i] = 0;
			m_userBuffReleaseTime[i] = 0;
		}
	}
public:
	float buffTimerRate()
	{
		if(m_userBuffFlag[1] != 0) return 0.5f;
		else return 1.0f;
	}
	float buffShootTimer()
	{
		if(m_userBuffFlag[3] != 0) return 0.33f;
		else return 1.0f;
	}
	float buffEnergeRate()
	{
		if(m_userBuffFlag[4] != 0) return 0.5f;
		else return 1.0f;
	}
	float buffBulletPowerRate()
	{
		if(m_userBuffFlag[2] != 0) return 2.0f;
		else return 1.0f;
	}
	float buffGoldRate()
	{
		if(m_userBuffFlag[0] != 0) return 2.0f;
		else return 1.0f;
	}
private:
//---------------------------------------
//+++++++++++++++++++++++++++++++++++++++
//�����ǹ��ڵ���Ч���Ķ���
	XSprite m_toolsEffect[6];	//��������ң�������ʱ�䣬����������
	int  m_toolsEffectStage[6];
	XMoveData m_toolsEffectData[6];

	int toolsEffectInit(XResPos resoursePosition);
	void toolsEffectMove(int delay);
	void toolsEffectDraw();
//---------------------------------------

	XTextureData m_gunTex_00;	//��ͨ����
	XTextureData m_gunTex_01;	//��ھ�����
	XTextureData m_gunTex_02;	//����
	XTextureData m_gunTex_03;	//��������
	XTextureData m_gunTex_04;	//�����
	XTextureData m_gunTex_05;	//������

	XTextureData m_gunFireTex_00;
	XTextureData m_gunFireTex_01;
	XTextureData m_gunFireTex_02;
	XTextureData m_gunFireTex_03;
	XTextureData m_gunFireTex_04;
	XTextureData m_gunFireTex_05;

	char m_gunIsFlash;
	int m_gunFlashTime;
	XSprite m_gunSprite;
	XSprite m_gunFireSprite_00;
	float m_gunAngle;
	XGUN_TYPE m_canUserGunType;	//ͨ����������������ʹ�õ�ǹ
	XGUN_TYPE m_gunType;

	void checkGunType();	//�����ҵ�ǹ�Ƿ����㹻������֧��

	char m_isInited;

	XBullet *m_bullet;	//�ӵ���ָ��
	XBulletTexture m_bulletTex;
	float m_gunFireAlpha;
	int m_gunFireAlphaStage;
	XGUN_TYPE m_gunFireType;
	void gunFireGlare(int interval);	//����ʱǹ����˸��Ч��

	//������Ϊ����ʾ�����������һЩ��Ա
	XSprite m_creditText;
	//XSprite m_creditSprite;
	XSprite m_goldSprite;
	XSprite m_energySprite;
	//XSprite m_scoresSprite;
	XSprite m_scoresText;

	int m_creditSum;	//����Ҫ����Ч��

	int m_scoresSum;
	int m_scoresArmSum;
	int m_energySum;	//��Ҫ����Ч��
	int m_energyArmSum;	//����ʵ�ֹ���Ч��
	int m_goldSum;
	int m_goldArmSum;	//����ʵ�ֹ���Ч��
	XNumber m_creditNum;
	XNumber m_scoresNum;
	XNumber m_energyNum;
	XNumber m_goldNum;
	void numberChange(int interval);	//���ֹ����Ķ���

	XNumber m_energyAddNum;
	float m_energyAddAlpha;
	int m_energyAddStage;
	void energyAddDraw();
	void energyAddMove(int interval);
	void energyAdd(int temp,int mode = 0);	//�ı���������ֵ��������ʾ
	XSprite m_lowEnergy;
	XSprite m_energyBar;
	XSprite m_energyBarDown;
	char m_isEnergyGlare;
	float m_energyAlpha;
	char m_energyAlphaStage;
	void numberOfEnergyGlare(int interval);	//�������ֽ�Сʱ��˸�Ķ���
	int UIBoardInit(XResPos resoursePosition);

	//������ʾÿ�����ӵķ�������ֵ
	XNumber m_goldAddNum[MAX_BULLET_SUM];
	float m_goldAddAlpha[MAX_BULLET_SUM];
	float m_goldAddX[MAX_BULLET_SUM];
	char m_goldAddFlag[MAX_BULLET_SUM];
	int m_goldAddBasicX[MAX_BULLET_SUM];
	int m_goldAddBasicY[MAX_BULLET_SUM];
	void goldAddMove(int interval);
	//����������ڱ�������ӵ���Դ
	XNumber m_goldBoxNum[MAX_BULLET_SUM];
	XSprite m_goldBoxEnergyLogo;
	XSprite m_goldBoxTicketLogo;
	XSprite m_goldBoxSprite[MAX_BULLET_SUM];
	float m_goldBoxAlpha[MAX_BULLET_SUM];
	float m_goldBoxSize[MAX_BULLET_SUM];
	char m_goledBoxEffectFlag[MAX_BULLET_SUM];
	char m_goldBoxTex[MAX_BULLET_SUM];
	int goldBoxEffectInit(XResPos resoursePosition);
	void goldBoxEffectmove(int interval);
	void goldBoxEffectDraw();
	void findAGoldBoxEffect(int mode,XVec2 position,int sum);
	
	//������ʾ�ɶ��Ľ��
	XFrameEx m_goldFly[MAX_BULLET_SUM * 10];
	float m_goldFlyAlpha[MAX_BULLET_SUM * 10];
	float m_goldFlyParam[MAX_BULLET_SUM * 10];
	XVec2 m_goldFlyPosition[MAX_BULLET_SUM * 10];	//��ʼ��λ��
	char m_goldFlyFlag[MAX_BULLET_SUM * 10];

	void findGoldFly(int level,XVec2 position);
	void goldFlyMove(int interval);
	int goldAddAndFlyInit(XResPos resoursePosition);

	//ǹ��״̬
	char m_isGunColorBoom;	//ǹ�Ƿ�ӵ��ɫ��ը����Ч��
	XFrameEx m_colorBoomFrame;	//��ɫը��ʱ��ǹ����Ч

	//��Ϸ״̬��һЩ����
	//XProgress m_nowLevelProcess;
	XSprite m_nowLevelProcessB;
	XSprite m_nowLevelProcess;
	XSprite m_nowLevelProcessU;
	//XNumber m_nowLevelProcessN;
	XSprite m_nowLevelProcessLight;
	XSprite m_nowLevelProcessLage;
	XFrameEx m_levelProcessBoom;
	int m_levelFlagStage;
	float m_levelFlagSize;
	float m_PrograssAlpha;
	float m_PrograssAlphaTemp;
	float m_levelFiniedSum;
	float m_levelFiniedArmSum;
	int processInit(XResPos resoursePosition);
	void processMove(int interval);

	//�����ǹ���������ߵ�һЩ����
	XTextureData m_tools_00;	//�漴���ߵ���ͼ
	XTextureData m_tools_01;	//�漴���ߵ���ͼ
	XTextureData m_tools_02;	//�漴���ߵ���ͼ
	XTextureData m_tools_03;	//�漴���ߵ���ͼ
	XTextureData m_tools_04;	//�漴���ߵ���ͼ
	XTextureData m_tools_05;	//�漴���ߵ���ͼ
	XTextureData m_tools_06;	//�漴���ߵ���ͼ
	XTextureData m_tools_07;	//�漴���ߵ���ͼ
	XTextureData m_tools_08;	//�漴���ߵ���ͼ
	XTextureData m_tools_09;	//�漴���ߵ���ͼ
	XSprite m_toolsSprite;	//������ߵľ���
	float m_toolsSize;		//������ߵĳߴ�
	char m_toolsFlag;		//������ߵı�־
	float m_toolsAlpha;		//������ߵ�͸����
	//int m_toolsTimer;		//��ʱ���ߵļ�ʱ����
	TOOLS_TYPE m_toolsType;	//������ߵ�����
//	XNumber m_toolsTimerText;
//	XSprite m_toolsTextSprite;	//������ߵľ���
//	float m_toolsTextAlpha;

	int toolsInit(XResPos resoursePosition);
	void toolsMove(int interval);
	int canGetTools(XGemMatrix *gems,XVec2 position);	//�ж��Ƿ���Ի�õ���
	void toolsDoProc(XVec2 position,XGEM_TYPE type);	//���߲���Ч���ĺ���
	void toolsDraw();

	//�����ǹ�����Ϸ���̵�һЩ����
	GAME_STAGE m_gameStage;	//��Ϸ�׶εı�־
	GAME_STAGE m_upGameStage;	//��һ�����������
	int m_gameCounter;		//��Ϸ�׶εĸ����Բ���
	int m_nowLevel;			//��ǰ�ǵڼ��� 0��ʼ
	float m_lineStartX;
	int m_lineStartTime;
	//��Ϸͷ�׶ε�һЩ����
	XNumber m_gameHeadNum;
	XSprite m_gameHeadText;
	XSprite m_gameHeadBG;
	XSprite m_gameHeadBGLevelInfo;
	float m_gameHeadTextY;
	float m_gameHeadSize;
	float m_gameHeadAlpha;
	int gameHeadInit(XResPos resoursePosition);
	void gameHeadDraw();
	void gameHeadMove(int interval);
	//��������Ϸ�����е�������ʾ
	XSprite m_gamePlayText00;
	XSprite m_gamePlayText01;
	XSprite m_gamePlayText02;
	int m_gamePlayTicketSum;		//�������Ӳ�Ʊ����ֵ
	XNumber m_gamePlayNum;			//����
	XSprite m_gamePlayLogo00;		//ͼ��00
	XSprite m_gamePlayLogo01;		//ͼ��01

	float m_gamePlayAlpha;
	float m_gamePlaySize;
	float m_gamePlayTextY;
	int m_gamePlayOverTimer;
	int gamePlayInit(XResPos resoursePosition);
	void gamePlayDraw();
	void gamePlayMove(int interval);
	//�����ǹ�����Ϸʤ����һЩ����
	XSprite m_gameWinBG;
	XSprite m_gameWinBGUp;
	XSprite m_gameWinText00;	//��ɶ�
	XSprite m_gameWinText01;	//����
	XSprite m_gameWinText02;	//��ý��
	XSprite m_gameWinText03;	//��������
	XSprite m_gameWinText04;	//���⽱��
	XSprite m_gameWinText05;	//��ھ�����
	XSprite m_gameWinText06;	//����
	XSprite m_gameWinText07;	//��������
	XSprite m_gameWinText08;	//�����
	XSprite m_gameWinText09;	//������
	XNumber m_gameWinNum;
	XTextureData m_gameWinStar_00;	
	XTextureData m_gameWinStar_01;	
	XSprite m_gameWinStar;	
	float m_gameWinBGAlpha;
	float m_gameWinTextY;
	float m_gameWinNum_00_00;	//��ɶȵ�����
	float m_gameWinNum_00_01;	//��ɶȵ�����
	float m_gameWinNum_01_00;	//���ֵĶ���
	float m_gameWinNum_01_01;	//���ֵĶ���
	float m_gameWinNum_02_00;	//��ҵķ���
	float m_gameWinNum_02_01;	//��ҵķ���
	float m_gameWinNum_03_00;	//�����ķ���
	int m_gameWinNum_03_01;	//�����ķ���
	float m_gameWinNum_04_00;	//���⽱��������
	float m_gameWinNum_04_01;	//���⽱��������
	int gameWinInit(XResPos resoursePosition);
	void gameWinDraw();
	void gameWinMove(int interval);
	//��������������ı���
	XSprite m_gameLostBG;	
	XSprite m_gameLostText;	
	XNumber m_gameLostNum;
	float m_gameLostAlpha;
	float m_gameLostY;
	int m_gameLostTimer;
	int gameLostInit(XResPos resoursePosition);
	void gameLostDraw();
	void gameLostMove(int interval);
	//������demo����ı���
	XSprite m_gameDemoBG;	
	XSprite m_gameDemoText1;	
	XSprite m_gameDemoText2;
	XSprite m_gameDemoText3;
	float m_gameDemoText2Alpha;
	float m_gameDemoText1Size;
	float m_gameDemoBGX;
	int gameDemoInit(XResPos resoursePosition);
	void gameDemoDraw();
	void gameDemoMove(int interval);
//	void gameStartGame();
	//���涨��һЩ�������а�ı���,���а�ֻ����ǰ20������Ϣ
	int readRecordData();
	int writeRecordData();
	int readHeadData();
	int writeHeadData();
	int readUserData();
	int writeUserData();
public:
	int initRecordData();
	int initHeadData(int flagWrite = 0);
	int initUserData();
private:
	XDataBasic m_data00;	//�������ݱ���
	XDataBasic m_data01;	//�������ݱ���
	XDataBasic m_data02;	//�������ݱ���
	int m_gameOrderScores[MAX_GAME_ORDER_SUM];
	char m_gameOrderName[MAX_GAME_ORDER_SUM][MAX_GAME_ORDER_NAME_LENGTH];	//ÿ���˵�����ֻ����6���ֽ�
	unsigned char *m_gameOrderCameraData;	//ͷ������
	unsigned char *m_gameOrderUserHeadData;	
	char m_gameOrderUserHeadName[USER_HEAD_SUM][MAX_GAME_ORDER_NAME_LENGTH];
	unsigned char *m_cameraTexData;
////	XFontUnicode m_gameOrderFont;	//��ʾ���а����ʾ����
//	XNumber m_gameOrderNumber_00;
//	XNumber m_gameOrderNumber_01;
//	XSprite m_gameOrderText_00;	
//	XSprite m_gameOrderText_01;
//	char m_showGameOrderFlag;
	XSprite m_gameOrderProBG;
	XSprite m_gameOrderProMD;
	XSprite m_gameOrderProUP;
	XNumber m_gameOrderProNumber;
	XMoveNumber m_gameOrderProMoveNumber;

	XFontUnicode m_gameOrderFontInput;	//��ʾ���а����ʾ����
	int m_gameNowOrder;				//��ҵ�ǰ������
	int m_gameOrderNextNeedScores;	//�����¸�������Ҫ�Ķ����

	XSprite m_gameInputNameBG;	//�������ֵı���
	XFont m_inputNameFont;
	float m_inputNameY;
	float m_inputAlpha;
	int m_inputNameTimer;
	int gameOrderInit(XResPos resoursePosition);
	void gameOrderDraw();
	void gameOrderDrawUp();
	void gameOrderMove(int interval);
	int getNowOrder();
	void updateGameOrder();

	char m_tempName[MAX_GAME_ORDER_NAME_LENGTH];
	int m_inputNamePosition;
	char m_nowChoose;
	int m_inputGlare;	//����������˸
	float m_inputCharMove;
	char m_inputCharFlag;
	void inputNameDraw();
	void inPutNameMove(int interval);
	void drawChar(float alpha);
	//�����ǹ�����ʾ���а��һЩ����
	XSprite m_orderShowBG;	//�󱳾���С����
	XSprite m_orderShowBG1;
	XSprite m_orderShowBG2;
	XSprite m_orderShowText;
	XNumber m_orderShowNum;
	char m_orderShowFlag;	//���ڱ���Ƿ��Ǵ�idle�����н���� 1:�ǣ�0:��
	float m_orderShowBGY;
	int orderShowInit(XResPos resoursePosition);
	void orderShowDraw();
	void orderShowMove(int interval);
	void showOrder(int x,int y,int order,int flag);
	int m_soundShowOrderTemp;
	//��������ͨ�غ�Ľ���
	XSprite m_gameAllOverText;
	float m_gameAllOverY;
	int m_gameAllOverTimer;
	int gameAllOverInit(XResPos resoursePosition);
	void gameAllOverDraw();
	void gameAllOverMove(int interval);
	//�����ǹ���ʱ�����Ƶı���
	XSprite m_gameTimerBG;			//��ʾʱ��ı���
	XSprite m_gameTimerAutoShoot;	//��ʾ�Զ����
	XNumber m_gameTimerNum;		//��ʾʱ�������
	char m_isNeedTimer;				//�Ƿ���Ҫʱ������
	int m_gamePlayTimer;			//��Ϸ��ʱ��	//��λΪ���� //ע���������ʾʱ�������ʾ�Զ����
	int m_gameSetPlayerTimer;		//��һ�����õļ�ʱʱ��
	int m_gameTimerStage;			//ʱ�����ʾ�׶�
	float m_gameTiemrX;
	float m_gameTimerAlpha;
	int gameTimerInit(XResPos resoursePosition);
	void gameTimerDraw();
	void gameTimerMove(int interval);
	void setGameTimerReset(int mode = 0);	//���ü�ʱ���¿�ʼ
	void setGameTimerStart();				//���ÿ�ʼ��ʱ
	void setGameTimerEnd(int mode);			//���ý�����ʱ 0:ֹͣ 1:�˳�

	//������������ʾ�Ľ���Ԫ��
	XSprite m_weaponInfoBG_00;					//ѡ�еı���
	XSprite m_weaponInfoBG_10;					//ѡ�еı���
	XSprite m_weaponInfoBG_01;					//û��ѡ�л��߲���ѡ��ı���
	XSprite m_weaponInfoBG_02;					
	XSprite m_weaponInfoBG_03;					
	XSprite m_weaponInfoBG_04;					
	XSprite m_weaponInfoBG_05;					
	XSprite m_weaponInfoBG_06;					
	XSprite m_weaponInfoLG_00;					
	XSprite m_weaponInfoLG_01;					
	XSprite m_weaponInfoLG_02;					
	XSprite m_weaponInfoLG_03;					
	XSprite m_weaponInfoLG_04;					
	XSprite m_weaponInfoLG_05;					
	XSprite m_weaponInfoLK_00;					
	XSprite m_weaponInfoLK_01;					
	XSprite m_weaponInfoLK_02;					
	XSprite m_weaponInfoLK_03;					
	XSprite m_weaponInfoLK_04;					
	XSprite m_weaponInfoLK_05;	
	XGUN_TYPE m_upChooseGunType;		//��һ��ѡ���������ʲô
	XGUN_TYPE m_nowChooseGunType;		//���ѡ�������ı��
	int m_chooseStage;			//����ѡ�����Ľ׶��Ա�־
	float m_chooseSize;
	int m_upCanUseGunType;	//��ǰ��ѡ��������ǵڼ���
	int m_nowCanUseGunType;	//��ǰ��ѡ��������ǵڼ���
	int m_canUseGunChangeStage;
	float m_canUserGunY;
	float m_canUserGunAlpha;
	float m_canUserGunSize;
	int weaponInfoInit(XResPos resoursePosition);	//�����ʼ��
	void weaponInfoDraw();
	void waeponInfoMove(int interval);
//+++++++++++++++++++++++++++++++++++++++++++
//���������Ϊ��ʵ���µĽ����е�Ԫ�ر䶯������ı���
	XMoveNumber m_weaponInfoMoveData;
	float m_weaponInfoUpPos;
	float m_weaponInfoNowPos;
	int m_weaponInfoTimer;
	void updataWeaponInfo();
//-------------------------------------------
	//�������������͵�������ʾ�Լ�ʱ�䲻���������ʾ
	XSprite m_energyLowFace;
	XSprite m_energyLowText;
	XSprite m_timerLowFace;
	XSprite m_timerLowText;

	int m_upEnergySum;	//��һ�ε�����ֵ
	int m_energyLowStage;
	float m_energyLowY;
	int m_upTimerSum;	//��һ�ε�ʱ��
	int m_timerLowStage;
	float m_timerLowY;

	int warnningInit(XResPos resoursePosition);	//�����ʼ��
	void warnningDraw();
	void warnningMove(int interval);
	//�����ǹ��ڷ��ڵ�����Ч��
	float m_UIShakeData;	//�𶯵ĵĲ���
	float m_UIShakeTempData;	//Ϊ���Ż�sin������������ʱ����
	float m_UIShakeTempData1;	//Ϊ���Ż�sin������������ʱ����
	int m_UIShakeStage;		//�𶯵Ľ׶�
	void UIShakeMove(int interval);
	float m_gunShakeData;	//�𶯵ĵĲ���
	float m_gunShakeTempData;	//Ϊ���Ż�sin������������ʱ����
	float m_gunShakeTempData1;	//Ϊ���Ż�sin������������ʱ����
	int m_gunShakeStage;		//�𶯵Ľ׶�
	void gunShakeMove(int interval);
	//�����ǹ��ڰ�����һЩ����
	XSprite m_helpText;
	XSprite m_helpPic;
	float m_helpAlpha;
	float m_helpAngle;
	float m_helpTextAlpha;
	int m_helpStage;
	XVec2 m_helpPosition;

	XVec2 m_helpArmPosition;	//������Ŀ��λ��
	XVec2 m_helpOldPosition;	//������ԭʼλ��
	float m_helpMoveX;				//�ƶ��Ĳ���
	int m_helpMoveStage;			//�ƶ��ı�־

	void setHelpPosition(float x,float y);
	int helpInit(XResPos resoursePosition);	//�����ʼ��
	void helpDraw();
	void helpMove(int interval);
	void setHelpEnd()
	{
		if(m_helpStage > 0 && m_helpStage < 4) m_helpStage = 4;
	}
	//�����ǹ������ý����һЩ����
public:
	XConfigData m_configData;
private:
	void initConfigData()
	{
		m_configData.isTicketNeed = 1;
		m_configData.ticketSum = 1;
		m_configData.isAutoShoot = 1;
		m_configData.coinToCredit = 1;
		m_configData.oneCreditEnergy = 2; //400 800 1200 1600 2000
		m_configData.volume = 10;	//0,10,20,30,40,50,60,70,80,90,100
		m_configData.allInCoin = 0;
		m_configData.allOutTicket = 0;
		m_configData.levelSum = 4;
	}
//	int readConfigData();	//ֱ�ӵ��ü�¼��������
//	void writeConfigData();
	XSprite m_configBG;
	XSprite m_configText_00;
	XSprite m_configText_01;
	XSprite m_configText_02;
	XSprite m_configText_03;
	XSprite m_configText_04;
	XSprite m_configText_05;
	XSprite m_configText_06;
	XSprite m_configText_07;
	XSprite m_configText_08;
	XSprite m_configText_09;
	XSprite m_configText_10;
	XSprite m_configText_11;
	XSprite m_configChoose;
	XFontUnicode m_configFont;
	float m_configRealY;
	float m_configY;
	int m_configNowChoose;	//���ý����ѡ��ֵ
	char m_isInConfig;	//�Ƿ������ý����״̬��
	int m_configStage;	//���ý���Ľ׶�
	int m_configChooseStage;
	float m_configChooseSize;
	XConfigData m_configBackData;
public:
	void enterConfig()
	{
		if(m_isInConfig == 0 && m_gameStage == GAME_STAGE_GAMELOGO && m_gameCounter == 3)
		{
			m_isInConfig = 1;
			m_configStage = 1;
			m_configNowChoose = 0;
			m_configChooseStage = 0;
			m_configChoose.setScale(1.0f,1.0f);
			m_configBackData = m_configData;
		}
	}
	void configKeyProc(CONFIG_KEY key);
private:
	int configInit(XResPos resoursePosition);
	void configDraw();
	void configMove(int interval);
private:
	//���������ܽ��������
	XSprite m_instructionBG;			//˵������ı���
	XSprite m_instructionOBJ_00;		//˵������������ͼ
	XSprite m_instructionOBJ_01;
	XSprite m_instructionOBJ_02;
	XSprite m_instructionOBJ_03;
	XSprite m_instructionOBJ_04;
	XSprite m_instructionOBJ_05;
	XSprite m_instructionOBJ_06;
	XSprite m_instructionOBJ_07;
	XSprite m_instructionOBJ_08;
	XSprite m_instructionOBJ_09;
	XSprite m_instructionOBJ_10;
	XSprite m_instructionOBJ_11;
	XSprite m_instructionOBJ_12;
	XSprite m_instructionOBJ_13;
	XSprite m_instructionText_00;		//˵�����������ͼ
	XSprite m_instructionText_01;
	XSprite m_instructionText_02;
	XSprite m_instructionText_03;
	XSprite m_instructionText_04;
	XSprite m_instructionText_05;
	XSprite m_instructionText_06;
	XSprite m_instructionText_07;
	XSprite m_instructionText_08;
	XSprite m_instructionText_09;
	XSprite m_instructionText_10;
	XSprite m_instructionText_11;
	XSprite m_instructionText_12;
	XSprite m_instructionText_13;
	float m_instructionTemp;
	float m_instructionPosition;
	float m_instructionMode;
	int m_instructionTimer;
	int instructInit(XResPos resoursePosition);
	void instructDraw();
	void instructMove(int interval);
	//���������Ʊ��ص�����
	XSprite m_ticketLog;
	XSprite m_ticketLog0;
	XSprite m_ticketText00;
	XSprite m_ticketText01;
	XNumber m_ticketNumber;
	XNumber m_ticketAddNumber;
	int m_numberStage;
	float m_numberAlpha;
//	char m_isTicketNeed;	//�Ƿ���Ҫ����Ʊ
	int m_allOutTicket;		//�ܵĳ���Ʊ������
	int m_allArmOutTicket;
	int m_releaseTicket;	//ʣ��Ĳ�Ʊ����
	int m_releaseArmTicket;
	char m_isWrong;
	int m_ticketWrongStage;
	float m_ticketWrongY;
	int m_ticketJumpStage;	//��Ʊ��Ծ�Ķ����׶�
	float m_ticketJumpSize;		
	void addTicketInput(XGEM_TYPE type,float size)
	{
		if(type >= GEM_TYPE_16) return;
		int temp;
		if(type <= GEM_TYPE_15)
		{
			if(size <= 1.5f) temp = type * 3; else
			if(size <= 2.5f) temp = type * 3 + 1; else
			temp = type * 3 + 2;
			if(temp >= 0 && temp < MAX_GEM_TYPE - 1) addTicket(gemBasicTicket[temp]);
		}else
		{
			temp = 26 + type - GEM_TYPE_15;
			if(temp >= 0 && temp < MAX_GEM_TYPE - 1) addTicket(gemBasicTicket[temp]);
		}
	}
public:
	float m_ticketbarArmSum;
	float m_ticketbarSum;
	void addTicket(float temp)
	{
		if(m_configData.isTicketNeed == 0) return;
		if(temp > 0)
		{
			if(m_configData.ticketSum == 0) m_ticketbarArmSum += temp * 0.25f;
			else if(m_configData.ticketSum == 1) m_ticketbarArmSum += temp * 0.5f;
			else m_ticketbarArmSum += temp * 1.0f;
			if(m_ticketbarArmSum > 1)
			{
				temp = (int)m_ticketbarArmSum;
				m_ticketbarArmSum -= temp;
				m_ticketbarSum = 0.0f;

				m_allArmOutTicket += temp;
				m_releaseArmTicket += temp;
				m_configData.allOutTicket += temp;
				m_ticketJumpStage = 1;
				m_numberStage = 1;
				char ch[20];
				sprintf_s(ch,"+%d",(int)(temp));
				m_ticketAddNumber.setNumber(ch);
				writeRecordData();
			}
		}
	}
	void ticketbarSumMove(int interval)
	{
		if(m_configData.isTicketNeed == 0) return;
		float tempSum;
		if(abs(m_ticketbarArmSum - m_ticketbarSum) > 0.0001f)
		{
			tempSum = m_ticketbarArmSum - m_ticketbarSum;
			if(tempSum < 0.0001f && tempSum > -0.0001f)
			{
				if(tempSum < 0) tempSum = -0.0001f;
				else tempSum = 0.0001f;
			}
			if(0.0015f * interval < 1.0f)
			{
				tempSum *= 0.0015f * interval;
			}
			m_ticketbarSum += tempSum;
			m_ticketBar.setClipRect(0.0f,0.0f,64.0f,251.0f * m_ticketbarSum);
		}
	}
	void decreaseTicket(int temp)
	{
		if(m_configData.isTicketNeed == 0) return;
		if(m_releaseArmTicket >= temp) m_releaseArmTicket -= temp;
	}
	void setTicketWrong()
	{
		if(m_isWrong == 0)
		{
			m_isWrong = 1;
			m_ticketWrongStage = 1;
		}else
		{
			if(m_ticketWrongStage >= 4) m_ticketWrongStage = 1;
		}
	}
	void setTicketWell()
	{
		if(m_isWrong == 1)
		{
		//	m_isWrong = 0;
			m_ticketWrongStage = 4;
		}
	}
private:
	int ticketInit(XResPos resoursePosition);
	void ticketDraw();
	void ticketMove(int interval);

	char m_isLightShoot;	//�Ƿ��Ǽ����ڷ�����
public:
	void inputName(INPUT_NAME_KEY key);
private:
	XGemMatrix *m_gems;
public:
	void addCredit()
	{
		m_creditSum ++;
		m_configData.allInCoin ++;
		char temp[32];
		sprintf_s(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
		m_creditNum.setNumber(temp);
		XSoundPlayer.play(m_soundCoinIn);
		writeRecordData();
	}
	void addEnergy()
	{
		if(m_creditSum >= m_configData.coinToCredit)
		{
			m_creditSum -= m_configData.coinToCredit;
			char temp[32];
			sprintf_s(temp,"%3d+%d",m_creditSum,m_configData.coinToCredit);
			m_creditNum.setNumber(temp);
		//	m_energyArmSum += 1000;
			energyAdd((m_configData.oneCreditEnergy + 1) * 400);
			XSoundPlayer.play(m_soundAddEnergy);
		}
	}
private:
	int findAFreeBullet();	//Ѱ��һ����Ч���ӵ�
	void shootEnergyDecrease(int energy);
	void gunFireSpriteSet(XGUN_TYPE type);
	//void gunShootBulletInit(int order,XGUN_TYPE type);
	void gunShootBulletInit(int order);
	int bulletBoom(XVec2 position,XBULLET_TYPE type,XGemMatrix *gems);	//��ը���Ĵ���

	//�����ǹ����������ֵĶ���
	//XSound m_sound;
	XSndHandle m_soundGun_00;
	XSndHandle m_soundGun_01;
	XSndHandle m_soundGun_02;
	XSndHandle m_soundGun_03;
	XSndHandle m_soundGun_04;
	XSndHandle m_soundGun_05;
	XSndHandle m_soundGun_06;
	XSndHandle m_soundBoom_00;
	XSndHandle m_soundBoom_01;
	XSndHandle m_soundBoom_02;
	XSndHandle m_soundBoom_03;
	XSndHandle m_soundBoom_04;
	XSndHandle m_soundCoinIn;
	XSndHandle m_soundSumAdd_00;
	int m_soundSumAddTimer;
	XSndHandle m_soundTimerOver;
	int m_soundTimerOverOld;
	XSndHandle m_soundStarAdd;
	int m_soundStarSumOld;
	XSndHandle m_soundTextDown;
	XSndHandle m_soundTimer_00;
	int m_soundTimerOld;
	//XSndHandle m_soundTimer_01;
	//int m_soundTimerOld_01;
	XSndHandle m_soundAddEnergy;
	XSndHandle m_soundChangeGun;
	XSndHandle m_soundTextMove_00;
	XSndHandle m_soundTextMove_01;
	XSndHandle m_soundTypeIn;
	XSndHandle m_soundTypeMove;
	XSndHandle m_soundHitGem;
	XSndHandle m_soundHitShield;
	XSndHandle m_soundGemBreak;
	XSndHandle m_soundEnergyFree;
	XSndHandle m_soundHitSide;
	XSndHandle m_soundSJTools;
	XSndHandle m_soundFSTools;
	XSndHandle m_soundScreenDown;
	XSndHandle m_soundScreenUp;
	XSndHandle m_soundColorBoom;
	int soundInit(XResPos resoursePosition);
public:
	int init(XGemMatrix *gems,XResPos resoursePosition = RES_LOCAL_FOLDER);
	void drawDown();
	void draw();
	void drawUp();
	void move(int interval);
	void whirlGun(int direction);
	void upgradeGun();
	void setGunType(XGUN_TYPE type);

	void crashProc(XGemMatrix *gems);//�ӵ�����ײ����
	int crashToolsProc(XGemMatrix *gems,XGEM_TYPE type,XVec2 tempPoint1);//�ӵ�����ײ����
	int colorBulletProc(XGemMatrix *gems,XGEM_TYPE type);
	int gemBeCrashProc(XGemMatrix *gems,int gemOrder,XVec2 tempPoint,int energy);	//��ʯ���򵽵Ĵ������
	int lightBulletProc(XGemMatrix *gems,int order);	//�����ӵ��Ĵ������
	void followBulletProc(XGemMatrix *gems,int order);	//�Զ����ٵ����Ĵ���

	void shoot();	//���
	int m_gunShootSpeed;

//	char m_shootMode;	//���ģʽ0:�������������1:�����������

	XGun();
};
}
#endif