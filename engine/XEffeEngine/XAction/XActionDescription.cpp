#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionDescription.h"

#include "XActionManager.h"
#include "XFrameEx.h"
namespace XE{
std::string actionTypeStr[] = {	
	"ACTION_TYPE_POSITION",	//λ�õı仯
	"ACTION_TYPE_POSITIONX",	//λ�õı仯
	"ACTION_TYPE_POSITION_Y",	//λ�õı仯
	"ACTION_TYPE_COLOR",		//��ɫ�ı仯
	"ACTION_TYPE_R",
	"ACTION_TYPE_G",
	"ACTION_TYPE_B",
	"ACTION_TYPE_ALPHA",		//͸���ȵı仯
	"ACTION_TYPE_ANGLE",		//�Ƕȱ仯
	"ACTION_TYPE_SIZE",		//�ߴ�ı仯
	"ACTION_TYPE_SIZE_EX",	//�ߴ�ı仯	//x��y�ȱ�������
	"ACTION_TYPE_SIZEX",		//�ߴ�ı仯	
	"ACTION_TYPE_SIZE_Y",		//�ߴ�ı仯
	"ACTION_TYPE_STAY",		//ͣ��״̬
	"ACTION_TYPE_MAX",
};

void XActionCore::move(float delay)
{
	if(m_isEnd || !m_isEnable) return;
	//if(m_isStart)
	{
		m_timeLine += delay;
		if(m_actionType == ACTION_TYPE_STAY)
		{
			if(m_timeLine >= m_endTime) m_isEnd = XTrue;
			return;
		}
		if(m_timeLine >= m_startTime)
		{//��ʼ���Ŷ���
			if(!m_isActionStart)
			{//��ʼ������
				//for(unsigned int i = 0;i < m_moveData.size();++ i)
				//{
				//	m_moveData[i]->reset();
				//	m_moveData[i]->move(m_timeLine - m_startTime);
				//}
				for(int i = 0;i < m_moveDataSum;++ i)
				{
					m_moveData[i].reset();
					m_moveData[i].move(m_timeLine - m_startTime);
				}
				
				m_isActionStart = XTrue;
			}else
			{
				//for(unsigned int i = 0;i < m_moveData.size();++ i)
				//{
				//	m_moveData[i]->move(delay);
				//}
				for(int i = 0;i < m_moveDataSum;++ i)
				{
					m_moveData[i].move(delay);
				}
			}
			//�ж��Ƿ�������
			XBool flag = XTrue;
		//	for(unsigned int i = 0;i < m_moveData.size();++ i)
		//	{
		//		if(!m_moveData[i]->getIsEnd())
		//		{
		//			flag = XFalse;
		//			break;
		//		}
		//	}
			for(int i = 0;i < m_moveDataSum;++ i)
			{
				if(!m_moveData[i].getIsEnd())
				{
					flag = XFalse;
					break;
				}
			}
			if(flag) m_isEnd = XTrue;	//��������
			//���߶�����������
			if(m_object == NULL) return;	//�������ĺϷ���
			switch(m_actionType)
			{
			case ACTION_TYPE_POSITION:
				m_object->setPosition(m_moveData[0].getCurData(),m_moveData[1].getCurData());
				break;
			case ACTION_TYPE_POSITIONX:
				m_object->setPosition(m_moveData[0].getCurData(),m_object->getPosition().y);
				break;
			case ACTION_TYPE_POSITION_Y:
				m_object->setPosition(m_object->getPosition().x,m_moveData[0].getCurData());
				break;
			case ACTION_TYPE_COLOR:
				m_object->setColor(m_moveData[0].getCurData(),
					m_moveData[1].getCurData(),
					m_moveData[2].getCurData(),
					m_moveData[3].getCurData());//(m_object->getPosition().x,m_moveData[0]->getCurData());
				break;
			case ACTION_TYPE_R:
				{
					XFColor tempColor = m_object->getColor();
					m_object->setColor(m_moveData[0].getCurData(),tempColor.g,tempColor.b,tempColor.a);
				}
				break;
			case ACTION_TYPE_G:
				{
					XFColor tempColor = m_object->getColor();
					m_object->setColor(tempColor.g,m_moveData[0].getCurData(),tempColor.b,tempColor.a);
				}
				break;
			case ACTION_TYPE_B:
				{
					XFColor tempColor = m_object->getColor();
					m_object->setColor(tempColor.r,tempColor.g,m_moveData[0].getCurData(),tempColor.a);
				}
				break;
			case ACTION_TYPE_ALPHA:
				m_object->setAlpha(m_moveData[0].getCurData());
				break;
			case ACTION_TYPE_ANGLE:
				m_object->setAngle(m_moveData[0].getCurData());
				break;
			case ACTION_TYPE_SIZE:
				m_object->setScale(m_moveData[0].getCurData(),m_moveData[1].getCurData());
				break;
			case ACTION_TYPE_SIZE_EX:
				m_object->setScale(m_moveData[0].getCurData(),m_moveData[0].getCurData());
				break;
			case ACTION_TYPE_STAY:
				break;
			}
		}
	}
}
XBool XActionCore::set(float startTime,float endTime,XActionType type,XMoveData *md,XActionState *state)
{
	if(m_isEnable) return XFalse;	//��ֹ�ظ�����

	m_object = NULL;
	m_startTime = startTime;
	m_endTime = endTime;
	m_actionType = type;
	//m_moveData.clear();
	m_moveDataSum = 0;
	switch(m_actionType)
	{
	case ACTION_TYPE_POSITION:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(&md[0]);
		//m_moveData.push_back(&md[1]);
		m_moveData = md;
		m_moveDataSum = 2;
		break;
	case ACTION_TYPE_POSITIONX:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(md);
		m_moveData = md;
		m_moveDataSum = 1;
		break;
	case ACTION_TYPE_POSITION_Y:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(md);
		m_moveData = md;
		m_moveDataSum = 1;
		break;
	case ACTION_TYPE_COLOR:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(&md[0]);
		//m_moveData.push_back(&md[1]);
		//m_moveData.push_back(&md[2]);
		//m_moveData.push_back(&md[3]);
		m_moveData = md;
		m_moveDataSum = 4;
		break;
	case ACTION_TYPE_R:
	case ACTION_TYPE_G:
	case ACTION_TYPE_B:
	case ACTION_TYPE_ALPHA:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(md);
		m_moveData = md;
		m_moveDataSum = 1;
		break;
	case ACTION_TYPE_ANGLE:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(md);
		m_moveData = md;
		m_moveDataSum = 1;
		break;
	case ACTION_TYPE_SIZE:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(&md[0]);
		//m_moveData.push_back(&md[1]);
		m_moveData = md;
		m_moveDataSum = 2;
		break;
	case ACTION_TYPE_SIZE_EX:
		if(md == NULL) return XFalse;
		//m_moveData.push_back(md);
		m_moveData = md;
		m_moveDataSum = 1;
		break;
	case ACTION_TYPE_STAY:
		if(state == NULL) return XFalse;
		m_stayState = * state;
		break;
	}

	m_isEnable = XTrue;
	return XTrue;
}
void XActionDescription::move(float delay)
{
	if(m_isEnd || !m_isEnable) return;
	//if(!m_isStart) return;
	m_timeLine += delay;
	//printf("%f,%d\n",m_timeLine,delay);
	if(m_timeLine >= m_startTime)
	{
		if(!m_isActionStart)
		{
			for(unsigned int i = 0;i < m_otherAction.size();++ i)
			{
				m_otherAction[i]->setStart();
				m_otherAction[i]->move(m_timeLine - m_startTime);
			}
			for(unsigned int i = 0;i < m_actionCore.size();++ i)
			{
				m_actionCore[i]->setStart();
				m_actionCore[i]->move(m_timeLine - m_startTime);
			}
			//�ж�����(��������Ż�������ÿ�ζ���ʱ��ȡ�������������δ�������Ż�)
			if(m_object != NULL)
			{
				switch(m_object->getObjectType())
				{
				case OBJ_FRAMEEX:
					((XFrameEx *)m_object)->reset();
					((XFrameEx *)m_object)->move(m_timeLine - m_startTime);
					break;
				}
			}
			m_isActionStart = XTrue;
		}else
		{
			for(unsigned int i = 0;i < m_otherAction.size();++ i)
			{
				m_otherAction[i]->move(delay);
			}
			for(unsigned int i = 0;i < m_actionCore.size();++ i)
			{
				m_actionCore[i]->move(delay);
			}
			//�ж�����(��������Ż�������ÿ�ζ���ʱ��ȡ�������������δ�������Ż�)
			if(m_object != NULL)
			{
				switch(m_object->getObjectType())
				{
				case OBJ_FRAMEEX:
					((XFrameEx *)m_object)->move(delay);
					break;
				}
			}
		}
		//�ж��Ƿ����
		XBool flag = XTrue;
		for(unsigned int i = 0;i < m_otherAction.size();++ i)
		{
			if(!m_otherAction[i]->getIsEnd())
			{
				flag = XFalse;
				break;
			}
		}
		if(flag)
		{
			for(unsigned int i = 0;i < m_actionCore.size();++ i)
			{
				if(!m_actionCore[i]->getIsEnd())
				{
					flag = XFalse;
					break;
				}
			}
		}
		if(flag) m_isEnd = XTrue;
	}
}
bool XActionDescription::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	char tempStr[MAX_FILE_NAME_LENGTH];
//	if(filename != NULL)
//	{
//		sprintf_s(tempStr,MAX_FILE_NAME_LENGTH,"%s%s",ACTION_DES_PATH,filename);
//	}else
//	{
//		sprintf_s(tempStr,MAX_FILE_NAME_LENGTH,"%s%s.acd",ACTION_DES_PATH,m_actionName);
//	}
//	if((fp = fopen(tempStr,"r")) == NULL) return XFalse;	//�ļ���ʧ��
	if((fp = fopen(filename,"r")) == NULL) return XFalse;	//�ļ���ʧ��
	int tempSize;
		
	if(fscanf(fp,"ID:%d,\n",&m_ID) != 1) {fclose(fp);return XFalse;}	//д�붯��ID;
	if(fscanf(fp,"Name:%s\n",m_actionName) != 1) {fclose(fp);return XFalse;}//д�붯������;
	//fscanf(fp,"OBJ:%d,\n",&tempSize);//���������/ID;(�д�ȷ��)����ʱ�����������Ϣ
	if(fscanf(fp,"%d:\n",&tempSize) != 1) {fclose(fp);return XFalse;}
	if(tempSize != 0)
	{//������������������������������������������������ã����������������ͬʱ��ʹ�ã��������ﲻ�ܱ����ã��������Գ�ͻ��
		XActionObjectDescription *temp = XMem::createMem<XActionObjectDescription>();
		if(temp == NULL || !temp->getDataFromFile(fp))
		{
			fclose(fp);
			return XFalse;
		}
		m_object = temp->createAObject();
	}else
	{//���������
		m_object = NULL;
	}
	if(fscanf(fp,"Time:%f,%f\n",&m_startTime,&m_endTime) != 2) {fclose(fp);return XFalse;}	
	m_otherAction.clear();
	m_actionCore.clear();
	m_otherActionID.clear();

	if(fscanf(fp,"OAD Sum:%d,\n",&tempSize) != 1) {fclose(fp);return XFalse;}//������������;
	if(tempSize > 0)//����������ID������
	{
		for(int i = 0;i < tempSize;++ i)
		{//������ݶ�ȡû������,��Ҫ�й�����֮���������
			if(fscanf(fp,"%d,\n",&tempSize) != 1) {fclose(fp);return XFalse;}	//д�붯��ID;
			if(fscanf(fp,"%s\n",tempStr) != 1) {fclose(fp);return XFalse;}		//д�붯������;
			//��δ��ɣ�����AD������
			m_otherActionID.push_back(tempSize);
			//�����Ⱥ�˳������⣬����һ���ȡδ����ֱ�Ӷ�ȡ�����ж���������������Ҫ���������ȡ
		//	XActionDescription * temp = XActionMananger::GetInstance().getAD(tempSize);
		//	if(temp == NULL || !pushAActionDescription(temp))
		//	{//������ȡʧ�ܣ����ﲢû���ͷ���ʱ��Դ�������в��Ͻ�
		//		fclose(fp);
		//		return XFalse;
		//	}
		}
	}
	if(fscanf(fp,"AC Sum:%d,\n",&tempSize) != 1) {fclose(fp);return XFalse;}//Ԫ����������
	if(tempSize > 0)
	{
		XActionCore *temp;
		int type;
		float startTime,endTime;
		XMoveData * tempMD = NULL;
		int mdSum;
		//md����
		float startData,endData,speed;
		int mode,isLoop;
		XActionState tempState;
		for(int i = 0;i < tempSize;++ i)
		{
			if(fscanf(fp,"AC Type:%d,\n",&type) != 1) {fclose(fp);return XFalse;}	//Ԫ����������
			if(fscanf(fp,"Time:%f,%f,\n",&startTime,&endTime) != 2) {fclose(fp);return XFalse;}	//Ԫ����������
			if(fscanf(fp,"MD Sum:%d,\n",&mdSum) != 1) {fclose(fp);return XFalse;}
			tempMD = NULL;
			if(mdSum > 0)
			{
				if(mdSum > 1) 
				{
					tempMD = XMem::createArrayMem<XMoveData>(mdSum);
					for(int j = 0;j < mdSum;++ j)
					{
						//m_actionCore[i]->m_moveData[j]->getParamStr(tempStr);	//ע���ʽ��Ӧ����
						if(fscanf(fp,"%f,%f,%f,%d,%d,\n",&startData,&endData,&speed,&mode,&isLoop) != 5) {fclose(fp);return XFalse;}	//Ԫ�����Ĳ���
						tempMD[j].set(startData,endData,speed,(XMoveDataMode)(mode),isLoop);
					}
				}else 
				{
					tempMD = XMem::createMem<XMoveData>();
					if(fscanf(fp,"%f,%f,%f,%d,%d,\n",&startData,&endData,&speed,&mode,&isLoop) != 5) {fclose(fp);return XFalse;}	//Ԫ�����Ĳ���
					tempMD->set(startData,endData,speed,(XMoveDataMode)(mode),isLoop);
				}
			}
			if(fscanf(fp,"State:%f,%f,%f,%f,%f,%f,%f,%f,%f,\n",&tempState.position.x,&tempState.position.y,
				&tempState.angle,&tempState.size.x,&tempState.size.y,
				&tempState.color.r,&tempState.color.g,&tempState.color.b,&tempState.color.a) != 9) {fclose(fp);return XFalse;}
			temp = XMem::createMem<XActionCore>();
			temp->set(startTime,endTime,(XActionType)(type),tempMD,&tempState);
			if(!pushAActionCore(temp))
			{//������ȡʧ�ܣ����ﲢû���ͷ���ʱ��Դ�������в��Ͻ�
				fclose(fp);
				return XFalse;
			}
		}
	}
	//ASD�Ķ�ȡ
	if(fscanf(fp,"ASD Sum:%d,\n",&tempSize) != 1) {fclose(fp);return XFalse;}//Ԫ����������
	if(tempSize > 0)
	{//�������ζ�ȡASD������
		XActionStateDescription * tempASD = NULL;
		float tempValue = 0;
		int tempBool = 0;
		for(int i = 0;i < tempSize;++ i)
		{
			tempASD = XMem::createMem<XActionStateDescription>();
			if(tempASD == NULL) return XFalse;
			if(fscanf(fp,"time:%d,\n",&tempASD->time) != 1) {fclose(fp);return XFalse;}
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidPosX = XTrue;
			else tempASD->isValidPosX = XFalse;
			tempASD->position.x = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidPosY = XTrue;
			else tempASD->isValidPosY = XFalse;
			tempASD->position.y = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidColorR = XTrue;
			else tempASD->isValidColorR = XFalse;
			tempASD->color.r = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidColorG = XTrue;
			else tempASD->isValidColorG = XFalse;
			tempASD->color.g = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidColorB = XTrue;
			else tempASD->isValidColorB = XFalse;
			tempASD->color.b = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidColorA = XTrue;
			else tempASD->isValidColorA = XFalse;
			tempASD->color.a = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidAngle = XTrue;
			else tempASD->isValidAngle = XFalse;
			tempASD->angle = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidSizeX = XTrue;
			else tempASD->isValidSizeX = XFalse;
			tempASD->size.x = tempValue;
			if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
			if(tempBool == 1) tempASD->isValidSizeY = XTrue;
			else tempASD->isValidSizeY = XFalse;
			tempASD->size.y = tempValue;

			if(!insertASD(tempASD))
			{
				XMem::XDELETE(tempASD);
				return XFalse;
			}
		}
	}
	fclose(fp);
	return true;
}
bool XActionDescription::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	return false;
}
bool XActionDescription::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XActionDescription::loadAction(const char * filename,XResPos resPos)
{
	if(m_isEnable) return XFalse;
	if(filename == NULL) return XFalse;
	m_isEnable = XTrue;
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename) && !loadFromFolder(filename) &&
			!loadFromWeb(filename)) return false;
		break;
	}
	return XTrue;
}
XBool XActionDescription::saveAction(const char *filename)
{
	FILE *fp = NULL;
	char tempStr[MAX_FILE_NAME_LENGTH];
	if(filename != NULL)
	{
		//sprintf(tempStr,"%s%s",ACTION_DES_PATH,filename);
		strcpy_s(tempStr,MAX_FILE_NAME_LENGTH,filename);
	}else
	{//Ĭ�ϵı���·��
		sprintf_s(tempStr,MAX_FILE_NAME_LENGTH,"%s%s.acd",ACTION_DES_PATH,m_actionName);
	}
	if((fp = fopen(tempStr,"w")) == NULL) return XFalse;	//�ļ���ʧ��
	
	fprintf(fp,"ID:%d,\n",m_ID);			//д�붯��ID;
	fprintf(fp,"Name:%s\n",m_actionName);	//д�붯������;
	//fprintf(fp,"OBJ:%d,\n",m_object);		//���������/ID;(�д�ȷ��)����ʱ�����������Ϣ,���½�����
	if(m_object == NULL)
	{//û��Ŀ�����
		fprintf(fp,"0:\n");	
	}else
	{
		fprintf(fp,"1:\n");	
		XActionObjectDescription * objDes = XActionObjectManager::GetInstance().getObjDes(m_object);
		if(objDes == NULL)
		{
			fclose(fp);
			return XFalse;
		}
		char * objDesStr = objDes->getStrFromData();
		if(objDesStr == NULL)
		{
			fclose(fp);
			return XFalse;
		}
		fprintf(fp,"%s",objDesStr);	//��Ҫ����
		XMem::XDELETE_ARRAY(objDesStr);
	}

	fprintf(fp,"Time:%f,%f\n",m_startTime,m_endTime);	
#ifdef _WIN64
	fprintf(fp,"OAD Sum:%zd,\n",m_otherAction.size());//������������;
#else
	fprintf(fp,"OAD Sum:%d,\n",m_otherAction.size());//������������;
#endif
	if(m_otherAction.size() > 0)//����������ID������
	{
		for(unsigned int i = 0;i < m_otherAction.size();++ i)
		{
#ifdef _WIN64
			fprintf(fp,"%zd,\n",m_otherAction[i]->m_ID);	//д�붯��ID;
#else
			fprintf(fp,"%d,\n",m_otherAction[i]->m_ID);	//д�붯��ID;
#endif
			fprintf(fp,"%s\n",m_otherAction[i]->m_actionName);//д�붯������;
		}
	}
	fprintf(fp,"AC Sum:%d,\n",m_actionCore.size());//Ԫ����������
	if(m_actionCore.size() > 0)
	{
		for(unsigned int i = 0;i < m_actionCore.size();++ i)
		{
			fprintf(fp,"AC Type:%d,\n",m_actionCore[i]->m_actionType);	//Ԫ����������
			fprintf(fp,"Time:%f,%f,\n",m_actionCore[i]->m_startTime,m_actionCore[i]->m_endTime);	//Ԫ����������
			fprintf(fp,"MD Sum:%d,\n",m_actionCore[i]->m_moveDataSum);
			if(m_actionCore[i]->m_moveDataSum > 0)
			{
				for(int j = 0;j < m_actionCore[i]->m_moveDataSum;++ j)
				{
					m_actionCore[i]->m_moveData[j].getParamStr(tempStr);
					fprintf(fp,"%s\n",tempStr);	//Ԫ�����Ĳ���
				}
			}
			fprintf(fp,"State:%f,%f,%f,%f,%f,%f,%f,%f,%f,\n",m_actionCore[i]->m_stayState.position.x,m_actionCore[i]->m_stayState.position.y,
				m_actionCore[i]->m_stayState.angle,m_actionCore[i]->m_stayState.size.x,m_actionCore[i]->m_stayState.size.y,
				m_actionCore[i]->m_stayState.color.r,m_actionCore[i]->m_stayState.color.g,m_actionCore[i]->m_stayState.color.b,m_actionCore[i]->m_stayState.color.a);
		}
	}
	//ASD������
#ifdef _WIN64
	fprintf(fp,"ASD Sum:%zd,\n",m_stateTable.size());//Ԫ����������
#else
	fprintf(fp,"ASD Sum:%d,\n",m_stateTable.size());//Ԫ����������
#endif
	if(m_stateTable.size() > 0)
	{
		for(unsigned int i = 0;i < m_stateTable.size();++ i)
		{//����д����������
			fprintf(fp,"time:%d,\n",m_stateTable[i]->time);
			if(m_stateTable[i]->isValidPosX) fprintf(fp,"1,%f,\n",m_stateTable[i]->position.x);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidPosY) fprintf(fp,"1,%f,\n",m_stateTable[i]->position.y);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorR) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.r);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorG) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.g);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorB) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.b);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorA) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.a);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidAngle) fprintf(fp,"1,%f,\n",m_stateTable[i]->angle);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidSizeX) fprintf(fp,"1,%f,\n",m_stateTable[i]->size.x);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidSizeY) fprintf(fp,"1,%f,\n",m_stateTable[i]->size.y);
			else fprintf(fp,"0,0,\n");
		}
	}
	fclose(fp);
	return XTrue;
}
XActionDescription::XActionDescription()
:m_isEnable(XFalse)
//,m_isStart(XFalse)
,m_isEnd(XTrue)
,m_isActionStart(XFalse)
,m_timeLine(0.0f)
//,m_actionDesType(AD_TYPE_AC)
,m_minTime(-1)
,m_maxTime(-1)
{
	static int myID = 0;
	while(true)
	{//������ҪID��Ψһ��
		++ myID;
		if(XActionMananger::GetInstance().getActionDes(myID) == NULL) break;
	}
	m_ID = myID;
	sprintf_s(m_actionName,MAX_FILE_NAME_LENGTH,"ActionDescription%06d",m_ID);	//������Ҫע�����ֵ�Ψһ��
	//int len = strlen(m_actionName);
	//sprintf_s(m_actionName,MAX_FILE_NAME_LENGTH,"ActionDescription%6d",m_ID);	//������Ҫע�����ֵ�Ψһ��
	//int len = strlen(m_actionName);
	//for(int i = 0;i < len;++ i)
	//{
	//	if(m_actionName[i] == ' ') m_actionName[i] = '0';
	//}
}
void XActionDescription::draw()
{//������Ҫ����ͼ��ĸ�������ͼ�������ڸǵ�����
	if(!m_isEnable) return;
	for(unsigned int i = 0;i < m_otherAction.size();++ i)
	{
		m_otherAction[i]->draw();
	}
	if(m_object != NULL) m_object->draw();
}
void XActionDescription::setStart()
{
	//if(m_isEnd)
	{
		//m_isStart = XTrue;
		m_timeLine = 0.0f;
		m_isActionStart = XFalse;
		m_isEnd = XFalse;
		for(unsigned int i = 0;i < m_otherAction.size();++ i)
		{
			m_otherAction[i]->setStart();
		}
		for(unsigned int i = 0;i < m_actionCore.size();++ i)
		{
			m_actionCore[i]->setStart();
		}
	}
}
int XActionDescription::getADChildIndex(XActionDescription * p)	//����ƶ���AD�ڵ�ǰAD���б��еı��
{
	if(p == NULL) return -1;
	int sum = m_otherAction.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_otherAction[i] == p) return i;
	}
	return -1;
}
void XActionDescription::setObject(XObjectBasic *obj)
{
	if(!m_isEnable) return;
	XActionObjectManager::GetInstance().decreaseAObject(m_object);	//ע����һ�п��ܻ��������
	m_object = obj;
	//�����������AC��Ŀ�궼��Ҫ�ı�
	int sum = m_actionCore.size();
	for(int i = 0;i < sum;++ i)
	{
		m_actionCore[i]->m_object = m_object;
	}
}
XBool XActionDescription::pushAActionCore(XActionCore * p)
{
	if(!m_isEnable) return XFalse;
	if(p == NULL) return XFalse;
	//������Ҫ����ظ�����
	int sum = m_actionCore.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_actionCore[i] == p) return XFalse;
	}
	p->m_object = m_object;
	m_actionCore.push_back(p);
	return XTrue;
}
XBool XActionDescription::popAActionCore(XActionCore * p)	//��������ɾ��һ��Ԫ��
{
	if(!m_isEnable) return XFalse;
	if(p == NULL) return XFalse;
	int size = m_actionCore.size();
	for(int i = 0;i < size;++ i)
	{
		if(m_actionCore[i] == p)
		{//ɾ�����Ԫ��
			popAActionCore(i);
			break;
		}
	}
	return XFalse;
}
XBool XActionDescription::replaceAAction(int index,XActionCore * p)
{
	if(p == NULL) return XFalse;
	if(index < 0 || index >= m_actionCore.size()) return XFalse;
	XMem::XDELETE(m_actionCore[index]);
	p->m_object = m_object;
	m_actionCore[index] = p;
	return XTrue;
}
XBool XActionDescription::popAActionCore(int index)
{
	if(!m_isEnable) return XFalse;
	int size = m_actionCore.size();
	if(index < 0 || index >= size) return XFalse;
	XMem::XDELETE(m_actionCore[index]);
//	for(int j = index;j < size - 1;++ j)
//	{
//		m_actionCore[j] = m_actionCore[j + 1];
//	}
//	m_actionCore.pop_back();
	m_actionCore.erase(m_actionCore.begin() + index);
	return XTrue;
}
XBool XActionDescription::getIsChildActionDes(XActionDescription * p)
{
	if(!m_isEnable) return XFalse;
	if(p == NULL) return XFalse;
	int sum = m_otherAction.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_otherAction[i] == p) return XTrue;
		if(m_otherAction[i]->getIsChildActionDes(p)) return XTrue;
	}
	return XFalse;
}
XBool XActionDescription::pushAActionDescription(XActionDescription * p)
{
	if(!m_isEnable) return XFalse;
	if(p == NULL) return XFalse;
	//������Ҫ����ظ��������໥����
	if(getIsChildActionDes(p)) return XFalse;		//�ظ�����
	if(p->getIsChildActionDes(this)) return XFalse;	//�໥����
	m_otherAction.push_back(p);
	m_otherActionID.push_back(p->m_ID);
	return XTrue;
}
XBool XActionDescription::popAActionDescription(int index)
{//�Ӷ�����ɾ��ָ������AD
	if(!m_isEnable) return XFalse;
	int sum = m_otherAction.size();
	if(index < 0 || index >= sum) return XFalse;
//	for(int i = index;i < sum - 1;++ i)
//	{
//		m_otherAction[i] = m_otherAction[i + 1];
//		m_otherActionID[i] = m_otherActionID[i + 1];
//	}
//	m_otherAction.pop_back();
//	m_otherActionID.pop_back();
	m_otherAction.erase(m_otherAction.begin() + index);
	m_otherActionID.erase(m_otherActionID.begin() + index);
	return XTrue;
}
void XActionDescription::release()
{
	if(!m_isEnable) return;
	//�������Դ�ͷ���δʵ��
	for(unsigned int i = 0;i < m_actionCore.size();++ i)
	{
		XMem::XDELETE(m_actionCore[i]);
	}
	m_actionCore.clear();
	//m_actionCore.swap(std::vector<XActionCore *>());

	for(unsigned int i = 0;i < m_stateTable.size();++ i)
	{
		XMem::XDELETE(m_stateTable[i]);
	}
	m_stateTable.clear();
	//m_stateTable.swap(std::vector<XActionStateDes *>());
	m_isEnable = XFalse;
}
//+++++++++++++++++++++++++++++++++++++++
XBool XActionDescription::popASD(const XActionStateDes* ASD)
{
	if(ASD == NULL) return XFalse;
	int sum = m_stateTable.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_stateTable[i] == ASD)
		{//�ҵ��˶�Ӧ��Ŀ��
			m_stateTable.erase(m_stateTable.begin() + i);
			if(sum > 1)
			{
				m_minTime = m_stateTable[0]->time;
				m_maxTime = m_stateTable[sum - 1]->time;
			}else
			{
				m_minTime = -1;
				m_maxTime = -1;
			}
			XMem::XDELETE(ASD);
			return XTrue;
		}
	}
	return XFalse;
}
XBool XActionDescription::insertASD(XActionStateDes* ASD)
{
	if(ASD == NULL) return XFalse;
	int sum = m_stateTable.size();
	if(sum == 0)
	{
		m_stateTable.push_back(ASD);
	}else
	{
		for(int i = 0;i < sum;++ i)
		{
			if(m_stateTable[i]->time > ASD->time)
			{
				m_stateTable.insert(m_stateTable.begin() + i,ASD);	//����
			}
		}
	}
	if(ASD->time < m_minTime || m_minTime < 0) m_minTime = ASD->time;
	if(ASD->time > m_maxTime || m_maxTime < 0) m_maxTime = ASD->time;
	return XTrue;
}
XActionStateDes* XActionDescription::getPreviousASD(int time,XActionStateType type)
{
	if(time < m_minTime) return NULL;
	int sum = m_stateTable.size();
	for(int i = sum - 1;i >= 0;-- i)
	{
		if(m_stateTable[i]->time <= time)
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POSX: 
				if(m_stateTable[i]->isValidPosX)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_POS_Y: 
				if(m_stateTable[i]->isValidPosY)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_R: 
				if(m_stateTable[i]->isValidColorR)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_G: 
				if(m_stateTable[i]->isValidColorG)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_B: 
				if(m_stateTable[i]->isValidColorB)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_A: 
				if(m_stateTable[i]->isValidColorA)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_ANGLE: 
				if(m_stateTable[i]->isValidAngle)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_SIZEX: 
				if(m_stateTable[i]->isValidSizeX)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_SIZE_Y: 
				if(m_stateTable[i]->isValidSizeY)
					return m_stateTable[i];
				break;
			}
		}
	}
	return NULL;
}
XActionStateDes* XActionDescription::getNextASD(int time,XActionStateType type)
{
	if(time > m_maxTime) return NULL;
	int sum = m_stateTable.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_stateTable[i]->time >= time)
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POSX: 
				if(m_stateTable[i]->isValidPosX)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_POS_Y: 
				if(m_stateTable[i]->isValidPosY)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_R: 
				if(m_stateTable[i]->isValidColorR)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_G: 
				if(m_stateTable[i]->isValidColorG)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_B: 
				if(m_stateTable[i]->isValidColorB)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_COLOR_A: 
				if(m_stateTable[i]->isValidColorA)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_ANGLE: 
				if(m_stateTable[i]->isValidAngle)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_SIZEX: 
				if(m_stateTable[i]->isValidSizeX)
					return m_stateTable[i];
				break;
			case ACTION_STATE_TYPE_SIZE_Y: 
				if(m_stateTable[i]->isValidSizeY)
					return m_stateTable[i];
				break;
			}
		}
	}
	return NULL;
}
float XActionDescription::getASValue(int time,XActionStateType type)
{
	XActionStateDes * tempS = NULL;
	XActionStateDes * tempE = NULL;
	tempS = getPreviousASD(time,type);
	tempE = getNextASD(time,type);
	if((tempS == NULL || tempS == tempE)&& tempE != NULL) 
	{
		switch(type)
		{
		case ACTION_STATE_TYPE_POSX: return tempE->position.x; 
		case ACTION_STATE_TYPE_POS_Y: return tempE->position.y; 
		case ACTION_STATE_TYPE_COLOR_R: return tempE->color.r; 
		case ACTION_STATE_TYPE_COLOR_G: return tempE->color.g; 
		case ACTION_STATE_TYPE_COLOR_B: return tempE->color.b; 
		case ACTION_STATE_TYPE_COLOR_A: return tempE->color.a; 
		case ACTION_STATE_TYPE_ANGLE: return tempE->angle; 
		case ACTION_STATE_TYPE_SIZEX: return tempE->size.x; 
		case ACTION_STATE_TYPE_SIZE_Y: return tempE->size.y; 
		} 
	}else
	if(tempE == NULL && tempS != NULL) 
	{
		switch(type)
		{
		case ACTION_STATE_TYPE_POSX: return tempS->position.x; 
		case ACTION_STATE_TYPE_POS_Y: return tempS->position.y; 
		case ACTION_STATE_TYPE_COLOR_R: return tempS->color.r; 
		case ACTION_STATE_TYPE_COLOR_G: return tempS->color.g; 
		case ACTION_STATE_TYPE_COLOR_B: return tempS->color.b; 
		case ACTION_STATE_TYPE_COLOR_A: return tempS->color.a; 
		case ACTION_STATE_TYPE_ANGLE: return tempS->angle; 
		case ACTION_STATE_TYPE_SIZEX: return tempS->size.x; 
		case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y; 
		} 
	}else
	if(tempS != NULL && tempE != NULL) 
	{
		if(tempE->time - tempS->time == 0) 
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POSX: return tempS->position.x; 
			case ACTION_STATE_TYPE_POS_Y: return tempS->position.y; 
			case ACTION_STATE_TYPE_COLOR_R: return tempS->color.r; 
			case ACTION_STATE_TYPE_COLOR_G: return tempS->color.g; 
			case ACTION_STATE_TYPE_COLOR_B: return tempS->color.b; 
			case ACTION_STATE_TYPE_COLOR_A: return tempS->color.a; 
			case ACTION_STATE_TYPE_ANGLE: return tempS->angle; 
			case ACTION_STATE_TYPE_SIZEX: return tempS->size.x; 
			case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y; 
			} 
		}else
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POSX: return tempS->position.x + (tempE->position.x - tempS->position.x) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_POS_Y: return tempS->position.y + (tempE->position.y - tempS->position.y) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_R: return tempS->color.r + (tempE->color.r - tempS->color.r) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_G: return tempS->color.g + (tempE->color.g - tempS->color.g) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_B: return tempS->color.b + (tempE->color.b - tempS->color.b) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_A: return tempS->color.a + (tempE->color.a - tempS->color.a) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_ANGLE: return tempS->angle + (tempE->angle - tempS->angle) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_SIZEX: return tempS->size.x + (tempE->size.x - tempS->size.x) * (time - tempS->time)/(tempE->time - tempS->time);
			case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y + (tempE->size.y - tempS->size.y) * (time - tempS->time)/(tempE->time - tempS->time);
			} 
		}
	}
	return 0.0f;
}
XActionState XActionDescription::getActionState(int time)
{
	XActionState retActionState;
	if(time < m_minTime || time > m_maxTime || m_stateTable.size() < 2) return retActionState;

	retActionState.position.x = getASValue(time,ACTION_STATE_TYPE_POSX);
	retActionState.position.y = getASValue(time,ACTION_STATE_TYPE_POS_Y);
	retActionState.color.r = getASValue(time,ACTION_STATE_TYPE_COLOR_R);
	retActionState.color.g = getASValue(time,ACTION_STATE_TYPE_COLOR_G);
	retActionState.color.b = getASValue(time,ACTION_STATE_TYPE_COLOR_B);
	retActionState.color.a = getASValue(time,ACTION_STATE_TYPE_COLOR_A);
	retActionState.angle = getASValue(time,ACTION_STATE_TYPE_ANGLE);
	retActionState.size.x = getASValue(time,ACTION_STATE_TYPE_SIZEX);
	retActionState.size.y = getASValue(time,ACTION_STATE_TYPE_SIZE_Y);
	retActionState.isEnable = XTrue;

	return retActionState;
}
//-------------------------------------------

void XActionCore::setStart()	//���ÿ�ʼ
{
//	if(m_isEnd) 
	{
		//m_isStart = XTrue;
		m_timeLine = 0.0f;
		m_isActionStart = XFalse;
		m_isEnd = XFalse;
		if(m_actionType == ACTION_TYPE_STAY)
		{
			m_object->setPosition(m_stayState.position);
			m_object->setColor(m_stayState.color.r,m_stayState.color.g,
				m_stayState.color.b,m_stayState.color.a);
			m_object->setScale(m_stayState.size);
			m_object->setAngle(m_stayState.angle);
		}
	}
}
void XActionCore::release()
{
	if(!m_isEnable) return;
//	for(unsigned int i = 0;i < m_moveData.size();++ i)
//	{
//		XMem::XDELETE(m_moveData[i]);
//	}
//	m_moveData.clear();
//	m_moveData.swap(std::vector<XMoveData *>());
	if(m_moveDataSum == 1) XMem::XDELETE(m_moveData);
	else XMem::XDELETE_ARRAY(m_moveData);
	//XMem::XDELETE(m_object);	//�ͷ��������Դ,�����ͷ��������Դ
	m_moveDataSum = 0;

	m_isEnable = XFalse;
}
}