//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XGem.h"
#include "XResourcePack.h"
namespace XE{
float gemBasicEnergy[8] = {15.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f};
float gemBasicTicket[MAX_GEM_TYPE - 1] = {0.005f,0.01f,0.02f, 0.02f,0.04f,0.08f, 0.03f,0.06f,0.12f, 0.04f,0.08f,0.16f,	//��ʯ
							0.05f,0.10f,0.20f, 0.06f,0.12f,0.24f, 0.07f,0.14f,0.28f, 0.08f,0.16f,0.32f,				//��ʯ
							0.00f,0.00f,0.00f,									//����
							0.00f,0.00f,0.00f,0.00f,0.00f,0.00f,0.00f,0.00f,0.00f};						//���� 
float gemE2GRate[8] = {0.5f,1.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.5f};
							
float demageGunToGem[6][8] = {	{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},		//��ͨ�ӵ�
								{1.5f,0.8f,0.9f,1.0f,1.0f,1.0f,1.0f,1.0f},		//ǿ���ӵ�
								{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},		//ɢ������ֱ�Ӽ���
								{2.5f,0.7f,0.8f,0.8f,0.8f,0.9f,0.9f,1.0f},		//�˵�����ֱ�Ӽ���
								{1.0f,2.0f,0.8f,0.7f,1.0f,1.0f,1.0f,1.5f},		//���ٵ�
								{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f}};		//������

void XGem::draw()
{
	if (!m_isEnable) return;
	if (m_type >= GEM_TYPE_08)
	{//����
	//	if(m_type >= GEM_TYPE_16 && m_type <= GEM_TYPE_20)
	//	{
	//		printf("xx\n");
	//	}
		m_gemSprite.draw(m_tex_00);
	}
	else
	{
		if (m_isGlare == 0)
		{
			if (m_type == GEM_TYPE_15)
			{
				m_gemSprite.draw(m_tex_00);
			}
			else
			{
				if (m_energy <= gemBasicEnergy[m_type] * m_size * 0.4f) m_gemSprite.draw(m_tex_02); else
					if (m_energy <= gemBasicEnergy[m_type] * m_size * 0.75f) m_gemSprite.draw(m_tex_01); else
						m_gemSprite.draw(m_tex_00);
			}
			if (m_gemStage == GEM_STAGE_DISPLAY && m_gemCounter >= 1)
			{
			}
			else if (m_showLight == 1)
			{
				m_lightFrame.draw();
			}
		}
		else
		{
			m_gemSprite.draw(m_tex_03);
		}
	}
}
void XGem::drawUp()
{
	if (!m_isEnable || m_isGlare != 0) return;
	if (m_gemStage == GEM_STAGE_DISPLAY && m_gemCounter >= 1)
	{
		if (m_type == GEM_TYPE_21)
		{
			XRender::drawLine(m_position.x + 64.0f, m_position.y + 64.0f, 0, 360, 3, XFColor(1.0f, sin(m_lineAlpha)));
		}
		m_boomFrame.draw();
	}
	else
	{
		m_runFrame.draw();
	}
}
void XGem::move(int interval)
{
	if (!m_isEnable) return;
	m_runFrame.move(interval);	//��תЧ��
	if (m_isGlare != 0)
	{
		m_isGlare -= interval;
		if (m_isGlare <= 0) m_isGlare = 0;
	}
	if (m_gemStage == GEM_STAGE_DISPLAY)
	{
		if (m_gemCounter == 0)
		{
			m_boomFrame.reset();
			m_gemCounter = 1;
			m_gemAlpha = 1;
			m_lineAlpha = 0.0f;
		}
		else
			if (m_gemCounter == 1)
			{
				m_boomFrame.move(interval);
				if (m_boomFrame.getIsEnd() != 0)
				{//����
					m_isEnable = false;
				}
				m_gemAlpha -= 0.002f * interval;
				if (m_gemAlpha <= 0.0f)
				{
					m_gemAlpha = 0.0f;
					m_gemCounter = 2;
				}
				m_gemSprite.setAlpha(m_gemAlpha);

				m_lineAlpha += 0.0075f * interval;
				if (m_lineAlpha >= PI) m_lineAlpha = PI;
			}
			else
				if (m_gemCounter == 2)
				{
					m_boomFrame.move(interval);
					if (m_boomFrame.getIsEnd() != 0)
					{//����
						m_isEnable = false;
					}
				}
	}
	else
		if (m_gemStage == GEM_STAGE_COMEIN)
		{
			m_gemCounter += interval;
			if (m_gemCounter > 600)
			{
				m_gemStage = GEM_STAGE_NORMAL;
				m_gemCounter = 0;
			}
		}
	if (m_showLight == 0/* && m_lightFrame.getIsEnd() == 1 */ && XRand::random(10000) < interval)
	{
		m_showLight = 1;
		m_lightFrame.reset();
	}
	if (m_showLight == 1)
	{
		m_lightFrame.move(interval);
		if (m_lightFrame.getIsEnd() == 1) m_showLight = 0;
	}
	if (m_gemStage == GEM_STAGE_COMEIN || m_gemStage == GEM_STAGE_NORMAL)
	{
		if (m_moveMode == 1)
		{//��ʯ���ƶ���δʵ��
			if (m_moveFlag == 0)
			{
				m_position.y += m_moveSpeed * interval;
			}
			else
			{
				m_position.y -= m_moveSpeed * interval;
			}
			if (m_moveSpeed > 0)
			{
				if (m_moveFlag == 0 && m_position.y > m_moveMax)
				{
					m_position.y = m_moveMax;
					m_moveFlag = 1;
				}
				if (m_moveFlag != 0 && m_position.y < m_moveMin)
				{
					m_position.y = m_moveMin;
					m_moveFlag = 0;
				}
			}
			else
			{
				if (m_moveFlag != 0 && m_position.y > m_moveMax)
				{
					m_position.y = m_moveMax;
					m_moveFlag = 0;
				}
				if (m_moveFlag == 0 && m_position.y < m_moveMin)
				{
					m_position.y = m_moveMin;
					m_moveFlag = 1;
				}
			}
			m_gemSprite.setPosition(m_position);
			if (m_type == GEM_TYPE_08)
			{
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y - 256 + 64);
			}
			else if (m_type == GEM_TYPE_09)
			{//ˮƽը��
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y + 64 - 16);
			}
			else if (m_type == GEM_TYPE_10)
			{//��ֱը��
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y + 64 - 16);
			}
			else
			{
				//	if(m_type >= GEM_TYPE_08 && m_type <= GEM_TYPE_21)
				//	{
				//		m_boomFrame.setPosition(m_position.x - 64.0f - 128.0f,m_position.y - 64.0f - 128.0f);
				//	}else
				if (m_type >= GEM_TYPE_16 && m_type <= GEM_TYPE_27)
				{
					m_boomFrame.setPosition(m_position.x - 64.0f - 128.0f, m_position.y - 64.0f - 128.0f);
				}
				else
				{
					m_boomFrame.setPosition(m_position.x - 64.0f, m_position.y - 64.0f);
				}
			}
			//m_boomFrame.setPosition(m_position);
			m_lightFrame.setPosition(m_position);
			m_runFrame.setPosition(m_position);
		}
		else if (m_moveMode == 2)
		{//��ʯ���ƶ���δʵ��
			if (m_moveFlag == 0)
			{
				m_position.x += m_moveSpeed * interval;
			}
			else
			{
				m_position.x -= m_moveSpeed * interval;
			}
			if (m_moveSpeed > 0)
			{
				if (m_moveFlag == 0 && m_position.x > m_moveMax)
				{
					m_position.x = m_moveMax;
					m_moveFlag = 1;
				}
				if (m_moveFlag != 0 && m_position.x < m_moveMin)
				{
					m_position.x = m_moveMin;
					m_moveFlag = 0;
				}
			}
			else
			{
				if (m_moveFlag != 0 && m_position.x > m_moveMax)
				{
					m_position.x = m_moveMax;
					m_moveFlag = 0;
				}
				if (m_moveFlag == 0 && m_position.x < m_moveMin)
				{
					m_position.x = m_moveMin;
					m_moveFlag = 1;
				}
			}
			m_gemSprite.setPosition(m_position);
			if (m_type == GEM_TYPE_08)
			{
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y - 256 + 64);
			}
			else if (m_type == GEM_TYPE_09)
			{//ˮƽը��
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y + 64 - 16);
			}
			else if (m_type == GEM_TYPE_10)
			{//��ֱը��
				m_boomFrame.setPosition(m_position.x - 256 + 64, m_position.y + 64 - 16);
			}
			else
			{
				//	if(m_type >= GEM_TYPE_08 && m_type <= GEM_TYPE_21)
				//	{
				//		m_boomFrame.setPosition(m_position.x - 64.0f - 128.0f,m_position.y - 64.0f - 128.0f);
				//	}else
				if (m_type >= GEM_TYPE_16 && m_type <= GEM_TYPE_27)
				{
					m_boomFrame.setPosition(m_position.x - 64.0f - 128.0f, m_position.y - 64.0f - 128.0f);
				}
				else
				{
					m_boomFrame.setPosition(m_position.x - 64.0f, m_position.y - 64.0f);
				}
			}
			m_lightFrame.setPosition(m_position);
			m_runFrame.setPosition(m_position);
			//	if(m_type >= GEM_TYPE_16 && m_type <= GEM_TYPE_20)
			//	{//���ߵĻ��������Ч
			//		if(m_position .x < -128.0f) m_isEnable = 0;
			//	}
		}
	}
}
void XGem::release()
{
	m_gemSprite.release();
	m_boomFrame.release();
	m_lightFrame.release();
	m_runFrame.release();
}
void XGem::setEnd()	//������ʧ�׶�
{
	if (m_gemStage != GEM_STAGE_NORMAL) return;
//	if(m_type >= GEM_TYPE_08)
//	{//����ֱ����ʧ
//		m_isEnable = false;
//	}else
	{//��ʯ��ҪЧ��
		m_gemStage = GEM_STAGE_DISPLAY;	//��ʼ������ʧ�׶�
		m_gemCounter = 0;
	}
}
int XGem::canCrash()
{
	if(m_isEnable != 0 && m_gemStage == GEM_STAGE_NORMAL) return 1;
	else return 0;
}
//���ѡ��ؿ�
void XGemMatrix::resetMatrix()
{
	int temp,order;
	int tempData[MAX_LEVEL_SUM];
	for(int i = 0;i < MAX_LEVEL_SUM;++ i)
	{
		tempData[i] = i + 10;
	}
	for(int i = 0;i < m_matrixSum - 10;++ i)
	{
		order = XRand::random(m_matrixSum - 10);
		if(order != i)
		{
			temp = tempData[i];
			tempData[i] = tempData[order];
			tempData[order] = temp;
		}
	}
	m_nowMatrix[0] = XRand::random(10);
//	m_nowMatrix[0] = 1;
	for(int i = 1;i < m_matrixSum - 10 + 1;++ i)
	{
		m_nowMatrix[i] = tempData[i - 1];
	}
	m_nowMatrix[1] = (m_nowMatrix[0] + 1 + XRand::random(9)) % 10;
	//�������
	for(int i = 0;i < m_matrixSum - 10 + 1;++ i)
	{
		if(m_nowMatrix[i] < 0 && m_nowMatrix[i] >= m_matrixSum)
		{
			printf("�ؿ�������ݴ���\n");
		}
	}
}
int XGemMatrix::init(XResPos resoursePosition)
{
	if(m_isInited != 0) return 1;
	//��ȡͼƬ��Դ
	if(m_gemTex[0][0].load("ResourcePack/pic/object/gem_00_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[0][1].load("ResourcePack/pic/object/gem_00_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[0][2].load("ResourcePack/pic/object/gem_00_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[0][3].load("ResourcePack/pic/object/gem_00_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[1][0].load("ResourcePack/pic/object/gem_01_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[1][1].load("ResourcePack/pic/object/gem_01_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[1][2].load("ResourcePack/pic/object/gem_01_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[1][3].load("ResourcePack/pic/object/gem_01_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[2][0].load("ResourcePack/pic/object/gem_02_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[2][1].load("ResourcePack/pic/object/gem_02_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[2][2].load("ResourcePack/pic/object/gem_02_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[2][3].load("ResourcePack/pic/object/gem_02_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[3][0].load("ResourcePack/pic/object/gem_03_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[3][1].load("ResourcePack/pic/object/gem_03_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[3][2].load("ResourcePack/pic/object/gem_03_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[3][3].load("ResourcePack/pic/object/gem_03_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[4][0].load("ResourcePack/pic/object/gem_04_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[4][1].load("ResourcePack/pic/object/gem_04_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[4][2].load("ResourcePack/pic/object/gem_04_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[4][3].load("ResourcePack/pic/object/gem_04_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[5][0].load("ResourcePack/pic/object/gem_05_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[5][1].load("ResourcePack/pic/object/gem_05_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[5][2].load("ResourcePack/pic/object/gem_05_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[5][3].load("ResourcePack/pic/object/gem_05_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[6][0].load("ResourcePack/pic/object/gem_06_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[6][1].load("ResourcePack/pic/object/gem_06_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[6][2].load("ResourcePack/pic/object/gem_06_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[6][3].load("ResourcePack/pic/object/gem_06_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[7][0].load("ResourcePack/pic/object/gem_07_00.png",resoursePosition) == 0) return 0;
	if(m_gemTex[7][1].load("ResourcePack/pic/object/gem_07_01.png",resoursePosition) == 0) return 0;
	if(m_gemTex[7][2].load("ResourcePack/pic/object/gem_07_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[7][3].load("ResourcePack/pic/object/gem_07_03.png",resoursePosition) == 0) return 0;

	if(m_gemTex[8][0].load("ResourcePack/pic/object/shield.png",resoursePosition) == 0) return 0;
//	if(m_gemTex[8][1].load("ResourcePack/pic/object/gem_07_01.png",resoursePosition) == 0) return 0;
//	if(m_gemTex[8][2].load("ResourcePack/pic/object/gem_07_02.png",resoursePosition) == 0) return 0;
	if(m_gemTex[8][3].load("ResourcePack/pic/object/shield_00.png",resoursePosition) == 0) return 0;

	if(m_gemTexTools[0].load("ResourcePack/pic/object/Boom.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[1].load("ResourcePack/pic/object/Light.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[2].load("ResourcePack/pic/object/ColorBoom.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[3].load("ResourcePack/pic/object/mirror.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[4].load("ResourcePack/pic/object/AddBullet.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[5].load("ResourcePack/pic/object/Tool_energy.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[6].load("ResourcePack/pic/object/Tool_gold.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[7].load("ResourcePack/pic/object/Tool_energyChange.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[8].load("ResourcePack/pic/object/Tool_10Free.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[9].load("ResourcePack/pic/object/Tool_time.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[10].load("ResourcePack/pic/object/black.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[11].load("ResourcePack/pic/object/mirror+.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[12].load("ResourcePack/pic/object/Light+.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[13].load("ResourcePack/pic/object/GoldBox.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[14].load("ResourcePack/pic/object/Buff_00.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[15].load("ResourcePack/pic/object/Buff_01.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[16].load("ResourcePack/pic/object/Buff_02.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[17].load("ResourcePack/pic/object/Buff_03.png",resoursePosition) == 0) return 0;
	if(m_gemTexTools[18].load("ResourcePack/pic/object/Buff_04.png",resoursePosition) == 0) return 0;

	if(m_boomFrame.init("ResourcePack/pic/UI/Boom_00/Boom_00_000.png",resoursePosition) == 0) return 0;
	if(m_boomFrame_00.init("ResourcePack/pic/UI/Boom_01/Boom_000.png",resoursePosition) == 0) return 0;
	if(m_boomFrame_01.init("ResourcePack/pic/UI/Boom_02/Boom_000.png",resoursePosition) == 0) return 0;
	if(m_boomFrame_02.init("ResourcePack/pic/UI/Effect_01/Effect_000.png",resoursePosition) == 0) return 0;
	if(m_boomFrame_03.init("ResourcePack/pic/UI/Effect_02/Effect_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[0].init("ResourcePack/pic/object/Light_00/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[1].init("ResourcePack/pic/object/Light_01/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[2].init("ResourcePack/pic/object/Light_02/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[3].init("ResourcePack/pic/object/Light_03/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[4].init("ResourcePack/pic/object/Light_04/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[5].init("ResourcePack/pic/object/Light_05/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[6].init("ResourcePack/pic/object/Light_06/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_lightFrame[7].init("ResourcePack/pic/object/Light_07/ligth_000.png",resoursePosition) == 0) return 0;
	if(m_runFrame.init("ResourcePack/pic/object/RoleLight/RoleLight_000.png",resoursePosition) == 0) return 0;

#if MAP_EDIT_MODE
	if(m_gemNum.init("ResourcePack/pic/UI/number_00.png",XVec2(25,32),XVec2(10,1),resoursePosition) == 0) return 0;
	m_gemNum.setAngle(90.0f);
	m_gemNum.setPosition(52.0f,138.0f);
	m_gemNum.setScale(0.5f,0.5f);
	m_gemNum.setDistance(-2.0f);
#endif
	//FILE *fp;
	//if((fp = fopen(GAME_DATA_FILE_NAME,"r")) == NULL)
	//{
	//	printf("Game data file open error!\n");
	//	return 0;
	//}
	////�����ȡ�ܵĹؿ���
	//fscanf(fp,"%d,",&m_matrixSum);
	//if(m_matrixSum <= 0) return 0;	//��ȡ�����ݷǷ�

	char * p = NULL;
	int len =XResPack.getFileLength(GAME_DATA_FILE_NAME);
	if(len <= 0) return 0;
	p = XMem::createArrayMem<char>(len + 1);
	if(p == NULL) return 0;
	XResPack.unpackResource(GAME_DATA_FILE_NAME,(unsigned char *)p);
	p[len] = '\0';
	sscanf(p,"%d,",&m_matrixSum);
	if(m_matrixSum <= 0) return 0;	//��ȡ�����ݷǷ�
	XMem::XDELETE_ARRAY(p);
	//�������ζ�ȡ���ؿ�����Ϣ
	m_matrixData = XMem::createArrayMem<XMatrixData>(m_matrixSum);
	if(m_matrixData == NULL) return 0;

	//�������ݶ�ȡ���
	m_isInited = 1;
	return 1;
}
int XGemMatrix::initInsideData()
{
	int len = 0;
	//���ζ�ȡÿһ�ص���Ϣ
#if MATRIX_DATA_TXT
	{//ʹ���ı��ķ�ʽ��ȡ
		char matrixFileName[] = GAME_DATA_MATRIX_FILE_NAME;
		int temp = strlen(matrixFileName);
		for(int i = 0;i < m_matrixSum; ++ i)
		{
			matrixFileName[temp - 5] = (i%10) + '0';
			matrixFileName[temp - 6] = ((i/10)%10) + '0';
			matrixFileName[temp - 7] = ((i/100)%10) + '0';
			len =XResPack->getFileLength(matrixFileName);
			if(len <= 0) return 0;
			try
			{
				m_matrixData[i].data = new char[len + 1];
				if(m_matrixData[i].data == NULL) return 0;
			}catch(...)
			{
				return 0;
			}
			XResPack->unpackResource(matrixFileName,(unsigned char *)(m_matrixData[i].data));
			m_matrixData[i].data[len] = '\0';
		}
	}
	for(int i = 0;i < m_matrixSum; ++ i)
	{
		matrixTXT2Data(i);
	}
#else
	{//����ʹ�ö����Ƶķ�ʽ��ȡ
		char matrixFileName[] = GAME_DATA_MATRIX_FILE_NAME;
		int temp = strlen(matrixFileName);
		matrixFileName[temp - 3] = 'd';
		matrixFileName[temp - 2] = 'a';
		matrixFileName[temp - 1] = 't';
		for(int i = 0;i < m_matrixSum; ++ i)
		{
			matrixFileName[temp - 5] = (i%10) + '0';
			matrixFileName[temp - 6] = ((i/10)%10) + '0';
			matrixFileName[temp - 7] = ((i/100)%10) + '0';
			len =XResPack.getFileLength(matrixFileName);
			if(len <= 0) return 0;
			try
			{
				m_matrixData[i].binData = new unsigned char[len + 1];
				if(m_matrixData[i].binData == NULL) return 0;
			}catch(...)
			{
				return 0;
			}
			XResPack.unpackResource(matrixFileName,(unsigned char *)(m_matrixData[i].binData));
		}
	}
#endif
	return 1;
}
struct _GEM_CHANGE_NEED_DATA
{
	int flag;		//�Ƿ���Ч
	XGEM_TYPE type;//����
	int size;		//��С
	int energy;		//����		
};
_GEM_CHANGE_NEED_DATA gemChangeData[MAX_GEM_TYPE] = {
	{0,GEM_TYPE_00,0,0},
	{1,GEM_TYPE_00,1,gemBasicEnergy[0]},{1,GEM_TYPE_00,2,gemBasicEnergy[0] * 2},{1,GEM_TYPE_00,4,gemBasicEnergy[0] * 4},
	{1,GEM_TYPE_01,1,gemBasicEnergy[1]},{1,GEM_TYPE_01,2,gemBasicEnergy[1] * 2},{1,GEM_TYPE_01,4,gemBasicEnergy[1] * 4},
	{1,GEM_TYPE_02,1,gemBasicEnergy[2]},{1,GEM_TYPE_02,2,gemBasicEnergy[2] * 2},{1,GEM_TYPE_02,4,gemBasicEnergy[2] * 4},
	{1,GEM_TYPE_03,1,gemBasicEnergy[3]},{1,GEM_TYPE_03,2,gemBasicEnergy[3] * 2},{1,GEM_TYPE_03,4,gemBasicEnergy[3] * 4},
	{1,GEM_TYPE_04,1,gemBasicEnergy[4]},{1,GEM_TYPE_04,2,gemBasicEnergy[4] * 2},{1,GEM_TYPE_04,4,gemBasicEnergy[4] * 4},
	{1,GEM_TYPE_05,1,gemBasicEnergy[5]},{1,GEM_TYPE_05,2,gemBasicEnergy[5] * 2},{1,GEM_TYPE_05,4,gemBasicEnergy[5] * 4},
	{1,GEM_TYPE_06,1,gemBasicEnergy[6]},{1,GEM_TYPE_06,2,gemBasicEnergy[6] * 2},{1,GEM_TYPE_06,4,gemBasicEnergy[6] * 4},
	{1,GEM_TYPE_07,1,gemBasicEnergy[7]},{1,GEM_TYPE_07,2,gemBasicEnergy[7] * 2},{1,GEM_TYPE_07,4,gemBasicEnergy[7] * 4},
	{1,GEM_TYPE_15,1,SHIELD_ENERGY},{1,GEM_TYPE_15,2,SHIELD_ENERGY},{1,GEM_TYPE_15,4,SHIELD_ENERGY},
	{1,GEM_TYPE_08,1,1},
	{1,GEM_TYPE_09,1,1},
	{1,GEM_TYPE_10,1,1},
	{1,GEM_TYPE_11,1,1},
	{1,GEM_TYPE_12,1,1},
	{1,GEM_TYPE_13,1,1},
	{1,GEM_TYPE_14,1,1},
	{1,GEM_TYPE_21,1,1},
	{1,GEM_TYPE_22,4,gemBasicEnergy[7] * 8},
	};

#if MAP_EDIT_MODE
int gemChangeMatrix[400] = {0};
int upGemChoose = 0;
#endif
void XGemMatrix::clearMatrix()
{
#if MAP_EDIT_MODE
	for(int i = 1;i < m_matrixAllGemSum;++ i)
	{
		gemChangeMatrix[i] = 0;
		(m_gem + i)->m_isEnable = gemChangeData[gemChangeMatrix[i]].flag;
		(m_gem + i)->m_type = gemChangeData[gemChangeMatrix[i]].type;
		(m_gem + i)->m_size = gemChangeData[gemChangeMatrix[i]].size;
		(m_gem + i)->m_energy = gemChangeData[gemChangeMatrix[i]].energy;
		setGemAttribute((m_gem + i));
	}
	m_matrixAllGemSum = 1;
	if((m_gem + 0)->m_type < GEM_TYPE_15) m_matrixAvailablyGemSum = 1;
	else m_matrixAvailablyGemSum = 0;
#endif
}
int XGemMatrix::changeGem(int x,int y,int flag)
{//���������Ҫ�������
#if MAP_EDIT_MODE
	if(m_isShowReseau != 0)
	{//������Ҫ��x,y����ȡ��
		int i = (948 + 64 - x + 16) / 32;
		int j = (y + 6 - 64 + 16) / 32;
		
		x = 1280 - 200 - 68 - i * 32;
		y = 58 + j * 32;
	} 
	if(m_nowMaxtrixGemSum <= 0) return 0;
	//���ҷ���Ҫ��Ľ�
	XVec2 tempPoint;
	tempPoint.set(x - 64.0f,y - 64.0f);
	for(int i = 0;i < m_matrixAllGemSum;++ i)
	{
		if(tempPoint.getLength((m_gem + i)->m_position) <= 16.0f)
		{//�ҵ�����Ҫ��Ľ�
			if(flag == 1)
			{
				gemChangeMatrix[i] = 0;
			}else
			if(flag == 0)
			{
				if(gemChangeMatrix[i] == 0 && upGemChoose != 0)
				{
					gemChangeMatrix[i] = upGemChoose;
				}else
				{
					gemChangeMatrix[i] ++;
					if(gemChangeMatrix[i] >= MAX_GEM_TYPE) gemChangeMatrix[i] = 0;
				}
			}else
			if(flag == 2)
			{
				gemChangeMatrix[i] --;
				if(gemChangeMatrix[i] < 0) gemChangeMatrix[i] = MAX_GEM_TYPE - 1;
			}
			upGemChoose = gemChangeMatrix[i];
			(m_gem + i)->m_isEnable = gemChangeData[gemChangeMatrix[i]].flag;
			(m_gem + i)->m_type = gemChangeData[gemChangeMatrix[i]].type;
			(m_gem + i)->m_size = gemChangeData[gemChangeMatrix[i]].size;
			(m_gem + i)->m_energy = gemChangeData[gemChangeMatrix[i]].energy;
			//(m_gem + m_nowMaxtrixGemSum)->m_position = tempPoint;
			setGemAttribute((m_gem + i));
			return 1;
		}
	}
	//û���ҵ��������
	if(m_matrixAllGemSum < 400)
	{
		if(flag == 1)
		{
			gemChangeMatrix[m_matrixAllGemSum] = 0;
		}else
		if(flag == 0)
		{
			if(gemChangeMatrix[m_matrixAllGemSum] == 0 && upGemChoose != 0)
			{
				gemChangeMatrix[m_matrixAllGemSum] = upGemChoose;
			}else
			{
				gemChangeMatrix[m_matrixAllGemSum] ++;
				if(gemChangeMatrix[m_matrixAllGemSum] >= MAX_GEM_TYPE) gemChangeMatrix[m_matrixAllGemSum] = 0;
			}
		}else
		if(flag == 2)
		{
			gemChangeMatrix[m_matrixAllGemSum] --;
			if(gemChangeMatrix[m_matrixAllGemSum] < 0) gemChangeMatrix[m_matrixAllGemSum] = MAX_GEM_TYPE - 1;
		}
		upGemChoose = gemChangeMatrix[m_matrixAllGemSum];
		(m_gem + m_matrixAllGemSum)->m_isEnable = gemChangeData[gemChangeMatrix[m_matrixAllGemSum]].flag;
		(m_gem + m_matrixAllGemSum)->m_type = gemChangeData[gemChangeMatrix[m_matrixAllGemSum]].type;
		(m_gem + m_matrixAllGemSum)->m_size = gemChangeData[gemChangeMatrix[m_matrixAllGemSum]].size;
		(m_gem + m_matrixAllGemSum)->m_energy = gemChangeData[gemChangeMatrix[m_matrixAllGemSum]].energy;
		(m_gem + m_matrixAllGemSum)->m_position = tempPoint;
		setGemAttribute((m_gem + m_matrixAllGemSum));
		//m_nowMaxtrixGemSum ++;
		if((m_gem + m_matrixAllGemSum)->m_type < GEM_TYPE_15) m_matrixAvailablyGemSum ++;
		m_matrixAllGemSum ++;
		return 1;
	}
	if(flag == 1)
	{//������ʯ
		int flag = 0;
		int flagTemp = 0;
		for(int i = 0;i < m_matrixAllGemSum;i ++)
		{
			if((m_gem + i)->m_isEnable == 0)
			{
				++flag;
				if((m_gem + i)->m_type < GEM_TYPE_15) flagTemp++;
			}
			if(flag != 0)
			{
				*(m_gem + i) = *(m_gem + i + flag);
			}
		}
		m_matrixAllGemSum -= flag;
		m_matrixAvailablyGemSum -= flagTemp;
	}
#endif
	return 0;
}
int XGemMatrix::writeMatrix(int matrixOrder)
{
	char matrixFileName[] = GAME_DATA_MATRIX_FILE_NAME;
	int temp = strlen(matrixFileName);
	matrixFileName[temp - 5] = (matrixOrder%10) + '0';
	matrixFileName[temp - 6] = ((matrixOrder/10)%10) + '0';
	matrixFileName[temp - 7] = ((matrixOrder/100)%10) + '0';
	//���㱦ʯ������
	int sum = 0;
	for(int i = 0;i < 20;++ i)
	{
		for(int j = 0;j < MAX_GEM_ROW;++ j)
		{
			if((m_gem + i * MAX_GEM_ROW + j)->m_isEnable != 0)
			{
				sum ++;
			}
		}
	}
	if(sum <= 0) return 0;	//û����Ч�ر�ʯ��ֱ���˳�
	FILE *fp;
	if((fp = fopen(matrixFileName,"w")) == NULL)
	{
		printf("Game data file open error!\n");
		return 0;
	}
	fprintf(fp,"%d:1:60000\n",sum);	//��ȡ��ʯ����

	//���濪ʼ��ȡ��ʯ�е���Ϣ
	XGem *tempGem;
	sum = 0;
	for(int i = 0;i < 20;++ i)
	{
		for(int j = 0;j < MAX_GEM_ROW;++ j)
		{
			tempGem = m_gem + i * MAX_GEM_ROW + j;
			if(tempGem->m_isEnable != 0)
			{
				fprintf(fp,"(%d:%.0f,%.0f:0,0,0,0:%d,%.0f,%.0f),",sum,tempGem->m_position.x,tempGem->m_position.y,tempGem->m_type,tempGem->m_size,tempGem->m_energy);
				sum++;
			//	if(j == MAX_GEM_ROW - 1)
			//	{
			//		fprintf(fp,"\n");
			//	}
			}
		}
	}
	fprintf(fp,"\n0:");
	fclose(fp);
	return 1;
}
void XGemMatrix::setGemAttribute(XGem *tempGem)
{
	if(tempGem->m_type == GEM_TYPE_00)
	{
		tempGem->m_tex_00 = &(m_gemTex[0][0]);
		tempGem->m_tex_01 = &(m_gemTex[0][1]);
		tempGem->m_tex_02 = &(m_gemTex[0][2]);
		tempGem->m_tex_03 = &(m_gemTex[0][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_01)
	{
		tempGem->m_tex_00 = &(m_gemTex[1][0]);
		tempGem->m_tex_01 = &(m_gemTex[1][1]);
		tempGem->m_tex_02 = &(m_gemTex[1][2]);
		tempGem->m_tex_03 = &(m_gemTex[1][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_02)
	{
		tempGem->m_tex_00 = &(m_gemTex[2][0]);
		tempGem->m_tex_01 = &(m_gemTex[2][1]);
		tempGem->m_tex_02 = &(m_gemTex[2][2]);
		tempGem->m_tex_03 = &(m_gemTex[2][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_03)
	{
		tempGem->m_tex_00 = &(m_gemTex[3][0]);
		tempGem->m_tex_01 = &(m_gemTex[3][1]);
		tempGem->m_tex_02 = &(m_gemTex[3][2]);
		tempGem->m_tex_03 = &(m_gemTex[3][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_04)
	{
		tempGem->m_tex_00 = &(m_gemTex[4][0]);
		tempGem->m_tex_01 = &(m_gemTex[4][1]);
		tempGem->m_tex_02 = &(m_gemTex[4][2]);
		tempGem->m_tex_03 = &(m_gemTex[4][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_05)
	{
		tempGem->m_tex_00 = &(m_gemTex[5][0]);
		tempGem->m_tex_01 = &(m_gemTex[5][1]);
		tempGem->m_tex_02 = &(m_gemTex[5][2]);
		tempGem->m_tex_03 = &(m_gemTex[5][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_06)
	{
		tempGem->m_tex_00 = &(m_gemTex[6][0]);
		tempGem->m_tex_01 = &(m_gemTex[6][1]);
		tempGem->m_tex_02 = &(m_gemTex[6][2]);
		tempGem->m_tex_03 = &(m_gemTex[6][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_07)
	{
		tempGem->m_tex_00 = &(m_gemTex[7][0]);
		tempGem->m_tex_01 = &(m_gemTex[7][1]);
		tempGem->m_tex_02 = &(m_gemTex[7][2]);
		tempGem->m_tex_03 = &(m_gemTex[7][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_15)
	{//��Զ�򲻻��ĵ���
		tempGem->m_tex_00 = &(m_gemTex[8][0]);
	//	tempGem->m_tex_01 = &(m_gemTex[8][1]);
	//	tempGem->m_tex_02 = &(m_gemTex[8][2]);
		tempGem->m_tex_03 = &(m_gemTex[8][3]);
	}else
	if(tempGem->m_type == GEM_TYPE_08)
	{//��ը��ը��
		tempGem->m_tex_00 = &(m_gemTexTools[0]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_00);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 256 + 64,tempGem->m_position.y - 256 + 64),0,0,0,0.05f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
	}else
	if(tempGem->m_type == GEM_TYPE_21)
	{//�۳������ĵ���
		tempGem->m_tex_00 = &(m_gemTexTools[10]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_03);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64.0f - 128.0f,tempGem->m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(0.5f,0.5f);
	}else
	if(tempGem->m_type == GEM_TYPE_22)
	{//����ĵ���
		tempGem->m_tex_00 = &(m_gemTexTools[13]);
		tempGem->m_boomFrame.setACopy(m_boomFrame);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64,tempGem->m_position.y - 64),0,0,0,0.05f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
	}else
	if(tempGem->m_type == GEM_TYPE_09)
	{//ˮƽը��
		tempGem->m_tex_00 = &(m_gemTexTools[12]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_01);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 256 + 64,tempGem->m_position.y + 64 - 16),0,0,0,0.05f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
	}else
	if(tempGem->m_type == GEM_TYPE_10)
	{//��ֱը��
		tempGem->m_tex_00 = &(m_gemTexTools[1]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_01);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 256 + 64,tempGem->m_position.y + 64 - 16),0,0,0,0.05f,1);
	}else
	if(tempGem->m_type == GEM_TYPE_11)
	{//��ɫը��
		tempGem->m_tex_00 = &(m_gemTexTools[2]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_03);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64.0f - 128.0f,tempGem->m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(0.5f,0.5f);
	}else
	if(tempGem->m_type == GEM_TYPE_12)
	{//��������
		tempGem->m_tex_00 = &(m_gemTexTools[11]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_03);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64.0f - 128.0f,tempGem->m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(0.5f,0.5f);
	}else
	if(tempGem->m_type == GEM_TYPE_13)
	{//��������
		tempGem->m_tex_00 = &(m_gemTexTools[3]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_03);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64.0f - 128.0f,tempGem->m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(0.5f,0.5f);
	}else
	if(tempGem->m_type == GEM_TYPE_14)
	{//һ���ӵ��ĵ���
		tempGem->m_tex_00 = &(m_gemTexTools[4]);
		tempGem->m_boomFrame.setACopy(m_boomFrame_03);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64.0f - 128.0f,tempGem->m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(0.5f,0.5f);
	}
	tempGem->m_gemSprite.init(m_gemTex[0][0].texture.m_w,m_gemTex[0][0].texture.m_h);
	if(tempGem->m_type >= GEM_TYPE_08)
	{//����
		tempGem->m_runFrame.setACopy(m_runFrame);
		tempGem->m_runFrame.setAttribute(XVec2(tempGem->m_position.x,tempGem->m_position.y),1,0,0,0.05f,1);
		tempGem->m_runFrame.setScale(tempGem->m_size * 0.25f,tempGem->m_size * 0.25f);
		tempGem->m_runFrame.setAngle(90.0f);

		if(tempGem->m_type == GEM_TYPE_08 
			|| tempGem->m_type == GEM_TYPE_21
			|| tempGem->m_type == GEM_TYPE_22
			|| tempGem->m_type == GEM_TYPE_09
			|| tempGem->m_type == GEM_TYPE_10
			|| tempGem->m_type == GEM_TYPE_11
			|| tempGem->m_type == GEM_TYPE_12
			|| tempGem->m_type == GEM_TYPE_13
			|| tempGem->m_type == GEM_TYPE_14)
		{
			tempGem->m_gemSprite.setAngle(90.0f);
		}else
		{
			tempGem->m_gemSprite.setAngle(0.0f);
		}
	//	tempGem->m_size = 1.0f;
	//	tempGem->m_energy = 1.0f;
	}else
	{
		tempGem->m_gemSprite.setAngle(90.0f);
		tempGem->m_boomFrame.setACopy(m_boomFrame);
		tempGem->m_boomFrame.setAttribute(XVec2(tempGem->m_position.x - 64,tempGem->m_position.y - 64),0,0,0,0.05f,1);
		tempGem->m_boomFrame.setAngle(90.0f);
		tempGem->m_boomFrame.setScale(tempGem->m_size * 0.5f,tempGem->m_size * 0.5f);
		if(tempGem->m_type <= GEM_TYPE_07)
		{
			tempGem->m_lightFrame.setACopy(m_lightFrame[tempGem->m_type]);
			tempGem->m_lightFrame.setAttribute(tempGem->m_position,0,0,0,0.05f,1);
			tempGem->m_lightFrame.setAngle(90.0f);
			tempGem->m_lightFrame.setScale(tempGem->m_size * 0.25f,tempGem->m_size * 0.25f);
		}
	}
	tempGem->m_gemSprite.setPosition(tempGem->m_position);
	tempGem->m_gemSprite.setScale(tempGem->m_size * 0.25f,tempGem->m_size * 0.25f);
	tempGem->m_isGlare = 0;
	tempGem->m_showLight = 0;
	tempGem->m_gemStage = GEM_STAGE_NORMAL;
}
int XGemMatrix::readMatrixFromTxt(int matrixOrder)
{
	char matrixFileName[] = GAME_DATA_MATRIX_FILE_NAME;
	int temp = strlen(matrixFileName);
	matrixFileName[temp - 5] = (matrixOrder%10) + '0';
	matrixFileName[temp - 6] = ((matrixOrder/10)%10) + '0';
	matrixFileName[temp - 7] = ((matrixOrder/100)%10) + '0';
	FILE *fp;
	if((fp = fopen(matrixFileName,"r")) == NULL)
	{
		printf("Game data file open error!\n");
		return 0;
	}
	fscanf(fp,"%d:%d:%d\n",&m_nowMaxtrixGemSum,&m_timerMode,&m_timerSum);	//��ȡ��ʯ����
	if(m_nowMaxtrixGemSum <= 0) return 0;
	if(m_timerMode != 0) m_timerMode = 1;
	if(m_timerMode != 0 && m_timerSum <= 0) return 0;
	m_matrixAllGemSum = m_nowMaxtrixGemSum;
	m_matrixAvailablyGemSum = 0;
#if MAP_EDIT_MODE
	m_nowMaxtrixGemSum = 400;
#else
	m_nowMaxtrixGemSum += 10;	//������� 10����Ϊ�ƶ�����ʹ��
#endif
	try
	{
		m_gem = new XGem[m_nowMaxtrixGemSum];
		if(m_gem == NULL)
		{
			fclose(fp);
			return 0;
		}
	}catch(...)
	{
		fclose(fp);
		return 0;
	}

	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		(m_gem + i)->m_isEnable = 0;
		(m_gem + i)->m_moveFlag = 0;
	}
	//���濪ʼ��ȡ��ʯ�е���Ϣ
	XGem *tempGem;
	for(int i = 0;i < m_matrixAllGemSum;++ i)
	{
		tempGem = m_gem + i;
		tempGem->m_isEnable = 1;
		fscanf(fp,"(%d:%f,%f:%d,%f,%f,%f:%d,%f,%f),",&temp,&(tempGem->m_position.x),&(tempGem->m_position.y),
			&(tempGem->m_moveMode),&(tempGem->m_moveSpeed),&(tempGem->m_moveMin),&(tempGem->m_moveMax),
			&(tempGem->m_type),&(tempGem->m_size),&(tempGem->m_energy));	//��ȡ��ʯ����
		if(tempGem->m_moveMode == 1)
		{
			tempGem->m_moveMin += tempGem->m_position.y;
			tempGem->m_moveMax += tempGem->m_position.y;
		}else
		if(tempGem->m_moveMode == 2)
		{
			tempGem->m_moveMin += tempGem->m_position.x;
			tempGem->m_moveMax += tempGem->m_position.x;
		}
		//������ݵĺ�����
		if(tempGem->m_moveMode < 0) tempGem->m_moveMode = 0;
		if(tempGem->m_moveMode > 2) tempGem->m_moveMode = 0;
		if(tempGem->m_energy <= 0) tempGem->m_energy = 1;
		if(tempGem->m_size <= 0) tempGem->m_size = 1.0f;
		if(tempGem->m_type < GEM_TYPE_15) m_matrixAvailablyGemSum ++;
		setGemAttribute(tempGem);
	}
	//��ȡ��ʾ����Ϣ
	fscanf(fp,"\n%d:",&m_helpSum);
	if(m_helpSum > 0)
	{
		try
		{
			m_helpData = new XHelpData[m_helpSum];
			if(m_helpData == NULL)
			{
				fclose(fp);
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			return 0;
		}
		for(int i = 0;i < m_helpSum;++ i)
		{
			fscanf(fp,"(%d,%d),",&m_helpData[i].gemOrder,&m_helpData[i].otherGem);
		}
	}
	fclose(fp);
	m_nowMatrixOrder = matrixOrder;
	m_matrixCanDraw = 1;
	m_nowHelpPoint = 0;
	return 1;
}
int XGemMatrix::readMatrix(int matrixOrder)
{
	if(matrixOrder < 0 || matrixOrder >= m_matrixSum) return 0;
	releaseMatrix();	//�ͷ����е���Դ

#if MATRIX_DATA_TXT
	int temp = 0;
	int offset = 0;
	char *p = m_matrixData[m_nowMatrix[matrixOrder]].data;
	sscanf(p + offset,"%d:%d:%d\n",&m_nowMaxtrixGemSum,&m_timerMode,&m_timerSum);
	offset += getCharPosition((char *)(p + offset),'\n') + 1;
	if(m_nowMaxtrixGemSum <= 0) return 0;
	if(m_timerMode != 0) m_timerMode = 1;
	if(m_timerMode != 0 && m_timerSum <= 0) return 0;
	m_matrixAllGemSum = m_nowMaxtrixGemSum;
	m_matrixAvailablyGemSum = 0;
#if MAP_EDIT_MODE
	m_nowMaxtrixGemSum = 400;
#else
	m_nowMaxtrixGemSum += 10;	//������� 10����Ϊ�ƶ�����ʹ��
#endif
	try
	{
		m_gem = new XGem[m_nowMaxtrixGemSum];
		if(m_gem == NULL)
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}

	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		(m_gem + i)->m_isEnable = 0;
		(m_gem + i)->m_moveFlag = 0;
	}
	//���濪ʼ��ȡ��ʯ�е���Ϣ
	XGem *tempGem;
	for(int i = 0;i < m_matrixAllGemSum;++ i)
	{
		tempGem = m_gem + i;
		tempGem->m_isEnable = 1;
		sscanf(p + offset,"(%d:%f,%f:%d,%f,%f,%f:%d,%f,%f),",&temp,&(tempGem->m_position.x),&(tempGem->m_position.y),
			&(tempGem->m_moveMode),&(tempGem->m_moveSpeed),&(tempGem->m_moveMin),&(tempGem->m_moveMax),
			&(tempGem->m_type),&(tempGem->m_size),&(tempGem->m_energy));
		if(tempGem->m_moveMode == 1)
		{
			tempGem->m_moveMin += tempGem->m_position.y;
			tempGem->m_moveMax += tempGem->m_position.y;
		}else
		if(tempGem->m_moveMode == 2)
		{
			tempGem->m_moveMin += tempGem->m_position.x;
			tempGem->m_moveMax += tempGem->m_position.x;
		}
		offset += getCharPosition((char *)(p + offset),')') + 2;

		//������ݵĺ�����
		if(tempGem->m_moveMode < 0) tempGem->m_moveMode = 0;
		if(tempGem->m_moveMode > 2) tempGem->m_moveMode = 0;
		if(tempGem->m_energy <= 0) tempGem->m_energy = 1;
		if(tempGem->m_size <= 0) tempGem->m_size = 1.0f;
		if(tempGem->m_type < GEM_TYPE_15) m_matrixAvailablyGemSum ++;
		setGemAttribute(tempGem);
	}
	//��ȡ��ʾ����Ϣ
	offset += 1;
	sscanf(p + offset,"\n%d:",&m_helpSum);
	offset += getCharPosition((char *)(p + offset),':') + 1;
	if(m_helpSum > 0)
	{
		try
		{
			m_helpData = new XHelpData[m_helpSum];
			if(m_helpData == NULL)
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		for(int i = 0;i < m_helpSum;++ i)
		{
			sscanf(p + offset,"(%d,%d),",&m_helpData[i].gemOrder,&m_helpData[i].otherGem);
			offset += getCharPosition((char *)(p + offset),')') + 2;
		}
	}
#else
	int offset = 0;
	unsigned char *p = m_matrixData[m_nowMatrix[matrixOrder]].binData;
	//��ȡ�ܹ��ı�ʯ����
	m_nowMaxtrixGemSum = (p[offset + 1] << 8) + p[offset];
	offset += 2;
	m_timerMode = p[offset];
	offset += 1;
	m_timerSum = p[offset];
	m_timerSum *= 1000;
	offset += 1;

	if(m_nowMaxtrixGemSum <= 0) return 0;
	if(m_timerMode != 0) m_timerMode = 1;
	if(m_timerMode != 0 && m_timerSum <= 0) return 0;
	m_matrixAllGemSum = m_nowMaxtrixGemSum;
	m_matrixAvailablyGemSum = 0;
#if MAP_EDIT_MODE
	m_nowMaxtrixGemSum = 400;
#else
	m_nowMaxtrixGemSum += 10;	//������� 10����Ϊ�ƶ�����ʹ��
#endif
	try
	{
		m_gem = new XGem[m_nowMaxtrixGemSum];
		if(m_gem == NULL)
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}

	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		(m_gem + i)->m_isEnable = 0;
		(m_gem + i)->m_moveFlag = 0;
	}
	//���濪ʼ��ȡ��ʯ�е���Ϣ
	XGem *tempGem;
	for(int i = 0;i < m_matrixAllGemSum;++ i)
	{
		tempGem = m_gem + i;
		tempGem->m_isEnable = 1;
		tempGem->m_position.x = (p[offset + 1] << 8) + p[offset];
		offset += 2;
		if(tempGem->m_position.x > 10000) tempGem->m_position.x = 10000 - tempGem->m_position.x;
		tempGem->m_position.y = (p[offset + 1] << 8) + p[offset];
		offset += 2;
		if(tempGem->m_position.y > 10000) tempGem->m_position.y = 10000 - tempGem->m_position.y;

		tempGem->m_type = (XGEM_TYPE)(p[offset] >> 3);
		tempGem->m_size = p[offset] & 0x7;
		offset += 1;
		tempGem->m_energy = p[offset];
		offset += 1;

		tempGem->m_moveMode = p[offset];
		offset += 1;
		if(tempGem->m_moveMode != 0)
		{
			tempGem->m_moveSpeed = ((p[offset + 1] << 8) + p[offset]);
			offset += 2;
			if(tempGem->m_moveSpeed > 10000) tempGem->m_moveSpeed = 10000 - tempGem->m_moveSpeed;
			tempGem->m_moveSpeed *= 0.001f;

			tempGem->m_moveMin = (p[offset + 1] << 8) + p[offset];
			offset += 2;
			if(tempGem->m_moveMin > 10000) tempGem->m_moveMin = 10000 - tempGem->m_moveMin;

			tempGem->m_moveMax = (p[offset + 1] << 8) + p[offset];
			offset += 2;
			if(tempGem->m_moveMax > 10000) tempGem->m_moveMax = 10000 - tempGem->m_moveMax;
		}else
		{
			tempGem->m_moveSpeed = 0.0f;
			tempGem->m_moveMin = 0.0f;
			tempGem->m_moveMax = 0.0f;
		}
		if(tempGem->m_moveMode == 1)
		{
			tempGem->m_moveMin += tempGem->m_position.y;
			tempGem->m_moveMax += tempGem->m_position.y;
		}else
		if(tempGem->m_moveMode == 2)
		{
			tempGem->m_moveMin += tempGem->m_position.x;
			tempGem->m_moveMax += tempGem->m_position.x;
		}

		//������ݵĺ�����
		if(tempGem->m_moveMode < 0) tempGem->m_moveMode = 0;
		if(tempGem->m_moveMode > 2) tempGem->m_moveMode = 0;
		if(tempGem->m_energy <= 0) tempGem->m_energy = 1;
		if(tempGem->m_size <= 0) tempGem->m_size = 1.0f;
		if(tempGem->m_type < GEM_TYPE_15) m_matrixAvailablyGemSum ++;
		setGemAttribute(tempGem);
	}
	//��ȡ��ʾ����Ϣ
	m_helpSum = (p[offset + 1] << 8) + p[offset];
	offset += 2;

	if(m_helpSum > 0)
	{
		try
		{
			m_helpData = new XHelpData[m_helpSum];
			if(m_helpData == NULL)
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		for(int i = 0;i < m_helpSum;++ i)
		{
			m_helpData[i].gemOrder = (p[offset + 1] << 8) + p[offset];
			offset += 2;
			m_helpData[i].otherGem = (p[offset + 1] << 8) + p[offset];
			offset += 2;
			if(m_helpData[i].otherGem >= 10000) m_helpData[i].otherGem = -1;
		}
	}
#endif
	m_nowMatrixOrder = matrixOrder;
	m_matrixCanDraw = 1;
	m_nowHelpPoint = 0;
	return 1;
}
int XGemMatrix::matrixTXT2Data(int matrixOrder)
{
#if MATRIX_DATA_TXT
	if(matrixOrder < 0 || matrixOrder >= m_matrixSum) return 0;
	int temp;
	int mextrixSum;
	int timerMode;
	int timerSum;

	int offset = 0;
	char *p = m_matrixData[matrixOrder].data;
	sscanf(p + offset,"%d:%d:%d\n",&mextrixSum,&timerMode,&timerSum);
	offset += getCharPosition((char *)(p + offset),'\n') + 1;
	if(mextrixSum <= 0) return 0;
	if(timerMode != 0) timerMode = 1;
	if(timerMode != 0 && timerSum <= 0) return 0;

	XGem *gem = NULL;
	try
	{
		gem = new XGem[mextrixSum];
		if(gem == NULL)
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}

	for(int i = 0;i < mextrixSum;++ i)
	{
		(gem + i)->m_isEnable = 0;
		(gem + i)->m_moveFlag = 0;
	}
	//���濪ʼ��ȡ��ʯ�е���Ϣ
	XGem *tempGem;
	for(int i = 0;i < mextrixSum;++ i)
	{
		tempGem = gem + i;
		tempGem->m_isEnable = 1;
		sscanf(p + offset,"(%d:%f,%f:%d,%f,%f,%f:%d,%f,%f),",&temp,&(tempGem->m_position.x),&(tempGem->m_position.y),
			&(tempGem->m_moveMode),&(tempGem->m_moveSpeed),&(tempGem->m_moveMin),&(tempGem->m_moveMax),
			&(tempGem->m_type),&(tempGem->m_size),&(tempGem->m_energy));
		offset += getCharPosition((char *)(p + offset),')') + 2;

		//������ݵĺ�����
		if(matrixOrder < 10 && tempGem->m_type <= GEM_TYPE_06) tempGem->m_type = (XGEM_TYPE)(tempGem->m_type + 1);
		if(tempGem->m_moveMode < 0) tempGem->m_moveMode = 0;
		if(tempGem->m_moveMode > 2) tempGem->m_moveMode = 0;
		if(tempGem->m_energy <= 0) tempGem->m_energy = 1;
		if(tempGem->m_size <= 0) tempGem->m_size = 1.0f;
		if(tempGem->m_type <= GEM_TYPE_07) tempGem->m_energy = gemBasicEnergy[tempGem->m_type] * tempGem->m_size;	
		if(tempGem->m_type == GEM_TYPE_15) tempGem->m_energy = SHIELD_ENERGY;	
	}
	//��ȡ��ʾ����Ϣ
	int helpSum;
	offset += 1;
	sscanf(p + offset,"\n%d:",&helpSum);
	offset += getCharPosition((char *)(p + offset),':') + 1;

	XHelpData *helpData = NULL;
	if(helpSum > 0)
	{
		try
		{
			helpData = new XHelpData[helpSum];
			if(helpData == NULL)
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		for(int i = 0;i < helpSum;++ i)
		{
			sscanf(p + offset,"(%d,%d),",&helpData[i].gemOrder,&helpData[i].otherGem);
			offset += getCharPosition((char *)(p + offset),')') + 2;
		}
	}
	//�������ݶ�ȡ��ɣ����潫��������д��Ķ������ļ���
	{
		char matrixFileName[] = GAME_DATA_MATRIX_FILE_NAME;
		temp = strlen(matrixFileName);
		matrixFileName[temp - 1] = 't';
		matrixFileName[temp - 2] = 'a';
		matrixFileName[temp - 3] = 'd';
		matrixFileName[temp - 5] = (matrixOrder%10) + '0';
		matrixFileName[temp - 6] = ((matrixOrder/10)%10) + '0';
		matrixFileName[temp - 7] = ((matrixOrder/100)%10) + '0';
		FILE *fp;
		if((fp = fopen(matrixFileName,"wb")) == NULL)
		{
			printf("Game data file open error!\n");
			return 0;
		}
		unsigned char tempData[10];
		tempData[0] = mextrixSum % 256;
		tempData[1] = (mextrixSum>>8) % 256;
		fwrite(tempData,2,1,fp);
		if(timerMode != 0) tempData[0] = 1;
		else tempData[0] = 0;
		fwrite(tempData,1,1,fp);
		tempData[0] = (timerSum/1000) % 256;	//�������ļ��е�ʱ�䵥λΪs���ı��ļ���ʱ�䵥λΪ����
		fwrite(tempData,1,1,fp);
		//����д�뱦ʯ����Ϣ
		for(int i = 0;i < mextrixSum;++ i)
		{
			//д��λ��
			if(gem[i].m_position.x < 0) gem[i].m_position.x = 10000 - gem[i].m_position.x;
			tempData[0] = ((int)gem[i].m_position.x) % 256;
			tempData[1] = (((int)gem[i].m_position.x)>>8) % 256;
			fwrite(tempData,2,1,fp);
			if(gem[i].m_position.y < 0) gem[i].m_position.y = 10000 - gem[i].m_position.y;
			tempData[0] = ((int)gem[i].m_position.y) % 256;
			tempData[1] = (((int)gem[i].m_position.y)>>8) % 256;
			fwrite(tempData,2,1,fp);
			//���濪ʼ��ʯ��Ϣ
			//д�뱦ʯ���ͺͳߴ�
			tempData[0] = (((((int)(gem[i].m_type)) % 256) << 3) + (((int)(gem[i].m_size)) % 256)) % 256;
			fwrite(tempData,1,1,fp);
			//д�뱦ʯ����
			tempData[0] = ((int)(gem[i].m_energy)) % 256;
			fwrite(tempData,1,1,fp);
			//����д���˶���Ϣ
			//�˶�ģʽ
			tempData[0] = gem[i].m_moveMode;
			fwrite(tempData,1,1,fp);
			if(gem[i].m_moveMode != 0)
			{
				//�˶��ٶ�(10 - 0.001)
				temp = gem[i].m_moveSpeed * 1000;
				if(temp < 0) temp = 10000 - temp;
				tempData[0] = temp % 256;
				tempData[1] = (temp>>8) % 256;
				fwrite(tempData,2,1,fp);
				//�˶���Χ
				temp = gem[i].m_moveMin;
				if(temp < 0) temp = 10000 - temp;
				tempData[0] = temp % 256;
				tempData[1] = (temp>>8) % 256;
				fwrite(tempData,2,1,fp);
				temp = gem[i].m_moveMax;
				if(temp < 0) temp = 10000 - temp;
				tempData[0] = temp % 256;
				tempData[1] = (temp>>8) % 256;
				fwrite(tempData,2,1,fp);
			}
			////д�뱦ʯ����
			//tempData[0] = ((int)(gem[i].m_type)) % 256;
			//fwrite(tempData,1,1,fp);
			////д�뱦ʯ�ߴ�
			//tempData[0] = ((int)(gem[i].m_size)) % 256;
			//fwrite(tempData,1,1,fp);
			////д�뱦ʯ����
			//tempData[0] = ((int)(gem[i].m_energy)) % 256;
			//fwrite(tempData,1,1,fp);
		}
		//����д�������Ϣ
		tempData[0] = helpSum % 256;
		tempData[1] = (helpSum>>8) % 256;
		fwrite(tempData,2,1,fp);
		for(int i = 0;i < helpSum;++ i)
		{
			tempData[0] = helpData[i].gemOrder % 256;
			tempData[1] = (helpData[i].gemOrder>>8) % 256;
			fwrite(tempData,2,1,fp);
			if(helpData[i].otherGem < 0)
			{
				tempData[0] = 10000 % 256;
				tempData[1] = (10000>>8) % 256;
				fwrite(tempData,2,1,fp);
			}else
			{
				tempData[0] = helpData[i].otherGem % 256;
				tempData[1] = (helpData[i].otherGem>>8) % 256;
				fwrite(tempData,2,1,fp);
			}
		}
		fclose(fp);
	}
	//д�����֮������ڴ�ռ�
	XDELETE_ARRAY(gem);
	XDELETE_ARRAY(helpData);
#endif
	return 1;
}
void XGemMatrix::draw()
{
	if(m_isInited == 0) return;
	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		m_gem[i].draw();
		//drawLine(m_gem[i].m_position.x + 64.0f,m_gem[i].m_position.y + 64.0f,m_gem[i].m_position.x + 3 + 64.0f,m_gem[i].m_position.y + 3 + 64.0f,3,0,0,0,1);
	}
#if MAP_EDIT_MODE
	if(m_isShowReseau != 0)
	{
		for(int i = 0;i < 21;i ++)
		{
			drawLine(948 + 80,			-6 + 32 * i + 48,	948 - 640 + 80,		-6 + 32 * i + 48,	1,0,0,1,1);	
			drawLine(948 - 32 * i + 80,	-6 + 48,			948 - 32 * i + 80,	-6 + 640 + 48,		1,0,0,1,1);	
		}
	}
#endif
}
void XGemMatrix::drawUp()
{
	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		m_gem[i].drawUp();
	}
#if MAP_EDIT_MODE
	int AllSum = 0;
	float AllEnergy = 0.0f;
	float AllTicket = 0.0f;
	int GemSum = 0;
	int temp = 0;
	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		if(m_isShowReseau != 0 && m_gem[i].m_isEnable != 0)
		{
			AllSum ++;
			AllEnergy += m_gem[i].m_energy;
			if(m_gem[i].m_type < GEM_TYPE_15) GemSum ++;
			if(m_gem[i].m_type <= GEM_TYPE_15)
			{
				if(m_gem[i].m_size <= 1.5f) temp = m_gem[i].m_type * 3; else
				if(m_gem[i].m_size <= 2.5f) temp = m_gem[i].m_type * 3 + 1; else
				temp = m_gem[i].m_type * 3 + 2;
			}else
			{
				temp = 26 + m_gem[i].m_type - GEM_TYPE_15;
			}
			AllTicket += gemBasicTicket[temp];
			m_gemNum.setPosition(m_gem[i].m_position.x + 64.0f,m_gem[i].m_position.y + 64.0f);
			m_gemNum.setNumber(i);
			m_gemNum.draw();
		}
	}
	m_gemNum.setPosition(1100,100);
	m_gemNum.setNumber(AllSum);
	m_gemNum.draw();
	m_gemNum.setPosition(1100,164);
	m_gemNum.setNumber((int)AllEnergy);
	m_gemNum.draw();
	m_gemNum.setPosition(1100,228);
	m_gemNum.setNumber((int)AllTicket);
	m_gemNum.draw();
	m_gemNum.setPosition(1100,292);
	m_gemNum.setNumber((int)GemSum);
	m_gemNum.draw();
	m_gemNum.setPosition(1100,356);
	m_gemNum.setNumber((int)m_nowMatrixOrder);
	m_gemNum.draw();
#endif
}
void XGemMatrix::move(int interval)
{
	if(m_isInited == 0) return;
//	if(m_nowMaxtrixGemSum > 10000) m_nowMaxtrixGemSum = 0;
	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		m_gem[i].move(interval);
	}
}
void XGemMatrix::getReleaseGemSum()
{
	if(m_isInited == 0) return;
	m_matrixReleaseGemSum = 0;
	for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
	{
		if(m_gem[i].m_isEnable != 0 && m_gem[i].m_gemStage == GEM_STAGE_NORMAL && m_gem[i].m_type < GEM_TYPE_15) m_matrixReleaseGemSum ++;
	}
}
XGemMatrix::XGemMatrix()
	:m_isInited(0)
	, m_matrixSum(0)
	, m_nowMaxtrixGemSum(0)
	, m_gem(NULL)
	, m_matrixCanDraw(0)
#if MAP_EDIT_MODE
	, m_isShowReseau(0)
#endif
	, m_helpSum(0)
	, m_nowHelpPoint(0)
	, m_helpData(NULL)
{}
void XGemMatrix::releaseMatrix()
{
	m_matrixCanDraw = 0;
	m_matrixAllGemSum = 0;
	m_matrixAvailablyGemSum = 0;
	if(m_gem != NULL)
	{
		for(int i = 0;i < m_nowMaxtrixGemSum;++ i)
		{
			m_gem[i].release();
		}
		XMem::XDELETE_ARRAY(m_gem);
	}
	m_nowMaxtrixGemSum = 0;
	m_helpSum = 0;
	m_nowHelpPoint = 0;
	XMem::XDELETE_ARRAY(m_helpData);
}

int XGemMatrix::addToolsGem(XVec2 position,XGEM_TYPE type)
{
	//Ѱ�ҵ�һ���հ׵ı�ʯ
	for(int i = m_nowMaxtrixGemSum - 1;i >= 0;-- i)
	{
		if(m_gem[i].m_isEnable == 0)
		{
			position.x -= 64.0f;
			position.y -= 64.0f;
			m_gem[i].release();
			//���³�ʼ��
			m_gem[i].m_isEnable = 1;
			m_gem[i].m_type = type;
			m_gem[i].m_position = position;
			m_gem[i].m_moveMode = 2;	//��ֱ�˶�
			if(type == GEM_TYPE_16) m_gem[i].m_moveSpeed = -0.18f; else	//����
			if(type == GEM_TYPE_17) m_gem[i].m_moveSpeed = -0.1f; else	//���
			if(type == GEM_TYPE_18) m_gem[i].m_moveSpeed = -0.15f; else	//���ת����
			if(type == GEM_TYPE_19) m_gem[i].m_moveSpeed = -0.21f; else	//10s������
			if(type == GEM_TYPE_20) m_gem[i].m_moveSpeed = -0.24f; else	//ʱ��

			if(type == GEM_TYPE_23) m_gem[i].m_moveSpeed = -0.24f; else	//��Ҽӱ�����
			if(type == GEM_TYPE_24) m_gem[i].m_moveSpeed = -0.24f; else	//ʱ���������
			if(type == GEM_TYPE_25) m_gem[i].m_moveSpeed = -0.24f; else	//�����ӱ�����
			if(type == GEM_TYPE_26) m_gem[i].m_moveSpeed = -0.24f; else	//���ټӿ�
			if(type == GEM_TYPE_27) m_gem[i].m_moveSpeed = -0.24f; 	//���ļ���
			m_gem[i].m_moveMin = - 300;
			m_gem[i].m_moveMax = 1280;
			m_gem[i].m_size = 2.0f;
			m_gem[i].m_energy = 1.0f;

			if(type == GEM_TYPE_16) m_gem[i].m_tex_00 = &(m_gemTexTools[5]); else
			if(type == GEM_TYPE_17) m_gem[i].m_tex_00 = &(m_gemTexTools[6]); else
			if(type == GEM_TYPE_18) m_gem[i].m_tex_00 = &(m_gemTexTools[7]); else
			if(type == GEM_TYPE_19) m_gem[i].m_tex_00 = &(m_gemTexTools[8]); else
			if(type == GEM_TYPE_20) m_gem[i].m_tex_00 = &(m_gemTexTools[9]); else

			if(type == GEM_TYPE_23) m_gem[i].m_tex_00 = &(m_gemTexTools[14]);
			if(type == GEM_TYPE_24) m_gem[i].m_tex_00 = &(m_gemTexTools[15]);
			if(type == GEM_TYPE_25) m_gem[i].m_tex_00 = &(m_gemTexTools[16]);
			if(type == GEM_TYPE_26) m_gem[i].m_tex_00 = &(m_gemTexTools[17]);
			if(type == GEM_TYPE_27) m_gem[i].m_tex_00 = &(m_gemTexTools[18]);

		//	if(type >= GEM_TYPE_08 && type <= GEM_TYPE_21)
		//	{
		//		m_gem[i].m_boomFrame.setACopy(m_boomFrame_03);
		//		m_gem[i].m_boomFrame.setAttribute(XVec2(m_gem[i].m_position.x - 64.0f - 128.0f,m_gem[i].m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
		//		m_gem[i].m_boomFrame.setScale(m_gem[i].m_size * 0.5f,m_gem[i].m_size * 0.5f);
		//		m_gem[i].m_boomFrame.setAngle(90.0f);
		//	}else
			if(type >= GEM_TYPE_16 && type <= GEM_TYPE_27)
			{
				m_gem[i].m_boomFrame.setACopy(m_boomFrame_02);
				m_gem[i].m_boomFrame.setAttribute(XVec2(m_gem[i].m_position.x - 64.0f - 128.0f,m_gem[i].m_position.y - 64.0f - 128.0f),0,0,0,0.025f,1);
				m_gem[i].m_boomFrame.setScale(m_gem[i].m_size * 0.5f,m_gem[i].m_size * 0.5f);
				m_gem[i].m_boomFrame.setAngle(90.0f);
			}else
			{
				m_gem[i].m_boomFrame.setACopy(m_boomFrame);
				m_gem[i].m_boomFrame.setAttribute(XVec2(m_gem[i].m_position.x - 64.0f,m_gem[i].m_position.y - 64.0f),0,0,0,0.05f,1);
				m_gem[i].m_boomFrame.setScale(m_gem[i].m_size * 0.5f,m_gem[i].m_size * 0.5f);
				m_gem[i].m_boomFrame.setAngle(90.0f);
			}

			m_gem[i].m_gemSprite.init(m_gemTex[0][0].texture.m_w,m_gemTex[0][0].texture.m_h);
			m_gem[i].m_gemSprite.setPosition(m_gem[i].m_position);
			m_gem[i].m_gemSprite.setScale(m_gem[i].m_size * 0.25f,m_gem[i].m_size * 0.25f);
			m_gem[i].m_gemSprite.setAngle(90.0f);
			m_gem[i].m_gemSprite.setAlpha(1.0f);
			m_gem[i].m_isGlare = 0;
			m_gem[i].m_showLight = 0;
			m_gem[i].m_gemStage = GEM_STAGE_COMEIN;
			m_gem[i].m_gemCounter = 0;
			return 1;
		}
	}
	return 0;
}
}