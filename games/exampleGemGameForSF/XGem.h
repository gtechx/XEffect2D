#ifndef _JIA_XGEM_
#define _JIA_XGEM_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XEffeEngine.h"
namespace XE{
#define MAP_EDIT_MODE (0)		//�Ƿ�����ͼ�༭ģʽ

#define MAX_GEM_ROW (20)		//��ʯ���20��
#define MATRIX_DATA_TXT (0)		//�Ƿ�ʹ���ı��ķ�ʽ��ȡ����

#define MAX_LEVEL_SUM (1024)		//�ؿ������ֵ

#define MAX_GEM_TYPE (37)

extern float demageGunToGem[6][8];	//�ӵ��Ա�ʯ���ƻ�ϵ��

extern float gemBasicEnergy[8];
extern float gemBasicTicket[MAX_GEM_TYPE - 1];
extern float gemE2GRate[8];
#define GOLD_BOX_TICKET_MIN (5)
#define GOLD_BOX_TICKET_MAX (100)
#define GOLD_BOX_SCORES_MIN (300)
#define GOLD_BOX_SCORES_MAX (1000)
#define SHIELD_ENERGY (250)	//���Ƶ�����

enum XGEM_TYPE
{
	GEM_TYPE_00,							
	GEM_TYPE_01,
	GEM_TYPE_02,
	GEM_TYPE_03,
	GEM_TYPE_04,
	GEM_TYPE_05,
	GEM_TYPE_06,
	GEM_TYPE_07,
	GEM_TYPE_15,	//��Զ�򲻻��ĵ���
	GEM_TYPE_08,	//��ը�ĵ���
	GEM_TYPE_09,	//ˮƽֱ�߹����ĵ���
	GEM_TYPE_10,	//��ֱֱ�߹����ĵ���
	GEM_TYPE_11,	//ɫ�ʹ����ĵ���
	GEM_TYPE_12,	//���淴�乥���ĵ���1
	GEM_TYPE_13,	//���淴�乥���ĵ���2
	GEM_TYPE_14,	//����һ���ӵ��ĵ���
	GEM_TYPE_21,	//�۳������ĵ���
	GEM_TYPE_22,	//����ĵ���

	GEM_TYPE_16,	//�������ĵ���
	GEM_TYPE_17,	//�ӽ�ҵĵ���
	GEM_TYPE_18,	//���ת��Ϊ�����ĵ���
	GEM_TYPE_19,	//10s�������ĵ���
	GEM_TYPE_20,	//��ʱ��ĵ���

	//������״̬����
	GEM_TYPE_23,	//��ý��x2
	GEM_TYPE_24,	//ʱ�����
	GEM_TYPE_25,	//������ǿ
	GEM_TYPE_26,	//���ټӿ�
	GEM_TYPE_27,	//���Ľ���
};

enum
{
	GEM_STAGE_NORMAL,	//����״̬
	GEM_STAGE_DISPLAY,	//��ʧ״̬
	GEM_STAGE_COMEIN,	//����״̬
};

class XGem
{
private:
public:
	XGEM_TYPE m_type;
	bool m_isEnable;
	float m_size;
	XVec2 m_position;
	float m_energy;

	float m_lineAlpha;

	XSprite m_gemSprite;
	XTextureData *m_tex_00;
	XTextureData *m_tex_01;
	XTextureData *m_tex_02;
	XTextureData *m_tex_03;

	XFrameEx m_boomFrame;	//��ʯ���ݻٵ�Ч��
	XFrameEx m_lightFrame;	//��ʯ�������Ч��
	char m_showLight;
	XFrameEx m_runFrame;	//��ʯ��Ȧ����

	int m_isGlare;
	int m_gemStage;	//�׶�
	int m_gemCounter;
	float m_gemAlpha;
	//�����ӵĲ���
	int m_moveMode;	//0���˶� 1��ˮƽ�˶� 2����ֱ�˶�
	float m_moveSpeed;	//�˶�����
	float m_moveMin;	//�˶��ķ�Χ
	float m_moveMax;	
	char m_moveFlag;
public:
	void draw();
	void drawUp();
	void move(int interval);
	void release();
	void setEnd();	//������ʧ�׶�
	int canCrash();	//�����ʯ�Ƿ��ܱ���
};

#define GAME_DATA_FILE_NAME "ResourcePack/gameData.txt"
#define GAME_DATA_MATRIX_FILE_NAME "ResourcePack/gemMatrix_000.txt"

struct XHelpData
{
	int gemOrder;	//��ʾ��ʯ�ı��
	int otherGem;	//��ر�ʯ�ı�ţ���������ر�ʯ��ʧ�����������ʯ��ʧ������ʾ������
};

struct XMatrixData
{
#if MATRIX_DATA_TXT
	char *data;	//�ı�����
	XMatrixData()
	:data(NULL)
	{}
#else
	unsigned char *binData;	//����������
	XMatrixData()
	:binData(NULL)
	{}
#endif
};

class XGemMatrix
{
private:
public:
	XGem *m_gem;
	XFrameEx m_boomFrame;		//��ʯ���ݻٵ�Ч��
	XFrameEx m_boomFrame_00;	//ը�����߱�ը��Ч��
	XFrameEx m_boomFrame_01;	//��ը�����߱�ը��Ч��
	XFrameEx m_boomFrame_02;	//������߱��򻵵�Ч��
	XFrameEx m_boomFrame_03;	//���߱��򻵵�Ч��
	XFrameEx m_lightFrame[8];		//��ʯ���ݻٵ�Ч��
	XFrameEx m_runFrame;		//��ʯ���ߵ�Ч��

	XTextureData m_gemTex[9][4];
	XTextureData m_gemTexTools[19];

	char m_isInited;	//�Ƿ��ʼ��
	int m_matrixSum;	//�ؿ�����
	int m_nowMatrixOrder;	//��ǰ�ؿ����
	int m_nowMaxtrixGemSum;	//��ǰ�ؿ��ı�ʯ����
	char m_matrixCanDraw;	//�����Ƿ�������
	int m_matrixAllGemSum;	//�ؿ����趨�ı�ʯ����	
	int m_matrixReleaseGemSum;	//��ǰʣ�±�ʯ������
	int m_matrixAvailablyGemSum;	//��������Ч�ر�ʯ����

	int m_timerMode;	//ʱ��ģʽ
	int m_timerSum;		//ʱ�����Ƶ���ֵ����λΪms

	//�����ǹ�����ʾ�������Ϣ
	int m_helpSum;
	int m_nowHelpPoint;	//��ǰ��ʾ������һ����
	XHelpData *m_helpData;
	int getHelpIsEnd()
	{
		if(m_nowHelpPoint >= m_helpSum) return 1;
		else return 0;
	}
	int upDateHelp()
	{
		if(m_nowHelpPoint >= m_helpSum || m_nowHelpPoint < 0) return 0;
		if(m_helpData[m_nowHelpPoint].gemOrder >= 0 && m_helpData[m_nowHelpPoint].gemOrder < m_nowMaxtrixGemSum
			&& m_gem[m_helpData[m_nowHelpPoint].gemOrder].canCrash() == 0)
		{
			m_nowHelpPoint ++;
			return 1;
		}else
		if(m_helpData[m_nowHelpPoint].otherGem >= 0 && m_helpData[m_nowHelpPoint].otherGem < m_nowMaxtrixGemSum
			&& m_gem[m_helpData[m_nowHelpPoint].otherGem].canCrash() == 0)
		{
			m_nowHelpPoint ++;
			return 1;
		}
		return 0;
	}

	void releaseMatrix();
	void setGemAttribute(XGem *tempGem);
#if MAP_EDIT_MODE
	int m_isShowReseau;	//�Ƿ���ʾ����
	XNumber m_gemNum;
#endif

	//������������ؿ��ĸ���
	XMatrixData *m_matrixData;	//���йؿ�������
	int m_nowMatrix[MAX_LEVEL_SUM];		//��ǰ���ѡ�е�����

public:
	void resetMatrix();	//ÿ���µ���Ҷ���������ѡ�������6��
	int init(XResPos resoursePosition = RES_LOCAL_FOLDER);
	int initInsideData();
	int readMatrixFromTxt(int matrixOrder);
	int readMatrix(int matrixOrder);
	int writeMatrix(int matrixOrder);	
	int changeGem(int x,int y,int flag = 0);
	void clearMatrix();
	void draw();
	void drawUp();
	void move(int interval);
	void getReleaseGemSum();

	int matrixTXT2Data(int matricOrder);	//��ʯ�����������ı���2��������ת��

	//����һ�����ߵĺ���
	int addToolsGem(XVec2 position,XGEM_TYPE type);

	XGemMatrix();
};
}
#endif