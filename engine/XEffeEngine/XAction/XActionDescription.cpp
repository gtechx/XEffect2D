//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionDescription.h"
#include "XBasicWindow.h"
#include "XActionManager.h"
#include "XFrameEx.h"

string actionTypeStr[] = {	
	"ACTION_TYPE_POSITION",	//λ�õı仯
	"ACTION_TYPE_POSITION_X",	//λ�õı仯
	"ACTION_TYPE_POSITION_Y",	//λ�õı仯
	"ACTION_TYPE_COLOR",		//��ɫ�ı仯
	"ACTION_TYPE_R",
	"ACTION_TYPE_G",
	"ACTION_TYPE_B",
	"ACTION_TYPE_ALPHA",		//͸���ȵı仯
	"ACTION_TYPE_ANGLE",		//�Ƕȱ仯
	"ACTION_TYPE_SIZE",		//�ߴ�ı仯
	"ACTION_TYPE_SIZE_EX",	//�ߴ�ı仯	//x��y�ȱ�������
	"ACTION_TYPE_SIZE_X",		//�ߴ�ı仯	
	"ACTION_TYPE_SIZE_Y",		//�ߴ�ı仯
	"ACTION_TYPE_STAY",		//ͣ��״̬
	"ACTION_TYPE_MAX",
};

void _XActionCore::move(int delay)
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
				//for(int i = 0;i < m_moveData.size();++ i)
				//{
				//	m_moveData[i]->reSet();
				//	m_moveData[i]->move(m_timeLine - m_startTime);
				//}
				for(int i = 0;i < m_moveDataSum;++ i)
				{
					m_moveData[i].reSet();
					m_moveData[i].move(m_timeLine - m_startTime);
				}
				
				m_isActionStart = XTrue;
			}else
			{
				//for(int i = 0;i < m_moveData.size();++ i)
				//{
				//	m_moveData[i]->move(delay);
				//}
				for(int i = 0;i < m_moveDataSum;++ i)
				{
					m_moveData[i].move(delay);
				}
			}
			//�ж��Ƿ�������
			_XBool flag = XTrue;
		//	for(int i = 0;i < m_moveData.size();++ i)
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
				m_object->setPosition(m_moveData[0].getNowData(),m_moveData[1].getNowData());
				break;
			case ACTION_TYPE_POSITION_X:
				m_object->setPosition(m_moveData[0].getNowData(),m_object->getPosition().y);
				break;
			case ACTION_TYPE_POSITION_Y:
				m_object->setPosition(m_object->getPosition().x,m_moveData[0].getNowData());
				break;
			case ACTION_TYPE_COLOR:
				m_object->setColor(m_moveData[0].getNowData(),
					m_moveData[1].getNowData(),
					m_moveData[2].getNowData(),
					m_moveData[3].getNowData());//(m_object->getPosition().x,m_moveData[0]->getNowData());
				break;
			case ACTION_TYPE_R:
				{
					_XFColor tempColor = m_object->getColor();
					m_object->setColor(m_moveData[0].getNowData(),tempColor.fG,tempColor.fB,tempColor.fA);
				}
				break;
			case ACTION_TYPE_G:
				{
					_XFColor tempColor = m_object->getColor();
					m_object->setColor(tempColor.fG,m_moveData[0].getNowData(),tempColor.fB,tempColor.fA);
				}
				break;
			case ACTION_TYPE_B:
				{
					_XFColor tempColor = m_object->getColor();
					m_object->setColor(tempColor.fR,tempColor.fG,m_moveData[0].getNowData(),tempColor.fA);
				}
				break;
			case ACTION_TYPE_ALPHA:
				m_object->setAlpha(m_moveData[0].getNowData());
				break;
			case ACTION_TYPE_ANGLE:
				m_object->setAngle(m_moveData[0].getNowData());
				break;
			case ACTION_TYPE_SIZE:
				m_object->setSize(m_moveData[0].getNowData(),m_moveData[1].getNowData());
				break;
			case ACTION_TYPE_SIZE_EX:
				m_object->setSize(m_moveData[0].getNowData(),m_moveData[0].getNowData());
				break;
			case ACTION_TYPE_STAY:
				break;
			}
		}
	}
}
_XBool _XActionCore::set(float startTime,float endTime,_XActionType type,_XMoveData *md,_XActionState *state)
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
	case ACTION_TYPE_POSITION_X:
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
void _XActionDescription::move(int delay)
{
	if(m_isEnd || !m_isEnable) return;
	//if(!m_isStart) return;
	m_timeLine += delay;
	//printf("%f,%d\n",m_timeLine,delay);
	if(m_timeLine >= m_startTime)
	{
		if(!m_isActionStart)
		{
			for(int i = 0;i < m_otherAction.size();++ i)
			{
				m_otherAction[i]->setStart();
				m_otherAction[i]->move(m_timeLine - m_startTime);
			}
			for(int i = 0;i < m_actionCore.size();++ i)
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
					((_XFrameEx *)m_object)->reset();
					((_XFrameEx *)m_object)->move(m_timeLine - m_startTime);
					break;
				}
			}
			m_isActionStart = XTrue;
		}else
		{
			for(int i = 0;i < m_otherAction.size();++ i)
			{
				m_otherAction[i]->move(delay);
			}
			for(int i = 0;i < m_actionCore.size();++ i)
			{
				m_actionCore[i]->move(delay);
			}
			//�ж�����(��������Ż�������ÿ�ζ���ʱ��ȡ�������������δ�������Ż�)
			if(m_object != NULL)
			{
				switch(m_object->getObjectType())
				{
				case OBJ_FRAMEEX:
					((_XFrameEx *)m_object)->move(delay);
					break;
				}
			}
		}
		//�ж��Ƿ����
		_XBool flag = XTrue;
		for(int i = 0;i < m_otherAction.size();++ i)
		{
			if(!m_otherAction[i]->getIsEnd())
			{
				flag = XFalse;
				break;
			}
		}
		if(flag)
		{
			for(int i = 0;i < m_actionCore.size();++ i)
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
_XBool _XActionDescription::loadAction(const char * filename,_XResourcePosition resoursePosition)
{
	if(m_isEnable) return XFalse;
	if(filename == NULL) return XFalse;
	m_isEnable = XTrue;
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		char tempStr[MAX_FILE_NAME_LENGTH];
	//	if(filename != NULL)
	//	{
	//		sprintf(tempStr,"%s%s",ACTION_DES_PATH,filename);
	//	}else
	//	{
	//		sprintf(tempStr,"%s%s.acd",ACTION_DES_PATH,m_actionName);
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
			_XActionObjectDescription *temp = createMem<_XActionObjectDescription>();
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
			//	_XActionDescription * temp = _XActionMananger::GetInstance().getAD(tempSize);
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
			_XActionCore *temp;
			int type;
			float startTime,endTime;
			_XMoveData * tempMD = NULL;
			int mdSum;
			//md����
			float startData,endData,speed;
			int mode,isLoop;
			_XActionState tempState;
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
						tempMD = createArrayMem<_XMoveData>(mdSum);
						for(int j = 0;j < mdSum;++ j)
						{
							//m_actionCore[i]->m_moveData[j]->getParamStr(tempStr);	//ע���ʽ��Ӧ����
							if(fscanf(fp,"%f,%f,%f,%d,%d,\n",&startData,&endData,&speed,&mode,&isLoop) != 5) {fclose(fp);return XFalse;}	//Ԫ�����Ĳ���
							tempMD[j].set(startData,endData,speed,(_XMoveDataMode)(mode),isLoop);
						}
					}else 
					{
						tempMD = createMem<_XMoveData>();
						if(fscanf(fp,"%f,%f,%f,%d,%d,\n",&startData,&endData,&speed,&mode,&isLoop) != 5) {fclose(fp);return XFalse;}	//Ԫ�����Ĳ���
						tempMD->set(startData,endData,speed,(_XMoveDataMode)(mode),isLoop);
					}
				}
				if(fscanf(fp,"State:%f,%f,%f,%f,%f,%f,%f,%f,%f,\n",&tempState.position.x,&tempState.position.y,
					&tempState.angle,&tempState.size.x,&tempState.size.y,
					&tempState.color.fR,&tempState.color.fG,&tempState.color.fB,&tempState.color.fA) != 9) {fclose(fp);return XFalse;}
				temp = createMem<_XActionCore>();
				temp->set(startTime,endTime,(_XActionType)(type),tempMD,&tempState);
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
			_XActionStateDescription * tempASD = NULL;
			float tempValue = 0;
			int tempBool = 0;
			for(int i = 0;i < tempSize;++ i)
			{
				tempASD = createMem<_XActionStateDescription>();
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
				tempASD->color.fR = tempValue;
				if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
				if(tempBool == 1) tempASD->isValidColorG = XTrue;
				else tempASD->isValidColorG = XFalse;
				tempASD->color.fG = tempValue;
				if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
				if(tempBool == 1) tempASD->isValidColorB = XTrue;
				else tempASD->isValidColorB = XFalse;
				tempASD->color.fB = tempValue;
				if(fscanf(fp,"%d,%f,\n",&tempBool,&tempValue) != 2) {fclose(fp);return XFalse;}
				if(tempBool == 1) tempASD->isValidColorA = XTrue;
				else tempASD->isValidColorA = XFalse;
				tempASD->color.fA = tempValue;
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
					XDELETE(tempASD);
					return XFalse;
				}
			}
		}
		fclose(fp);
	}else
	{//����Դ���н�ѹ������(��δ���)
	}
	return XTrue;
}
_XBool _XActionDescription::saveAction(const char *filename)
{
	FILE *fp = NULL;
	char tempStr[MAX_FILE_NAME_LENGTH];
	if(filename != NULL)
	{
		//sprintf(tempStr,"%s%s",ACTION_DES_PATH,filename);
		strcpy(tempStr,filename);
	}else
	{//Ĭ�ϵı���·��
		sprintf(tempStr,"%s%s.acd",ACTION_DES_PATH,m_actionName);
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
		_XActionObjectDescription * objDes = _XActionObjectManager::GetInstance().getObjDes(m_object);
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
		XDELETE_ARRAY(objDesStr);
	}

	fprintf(fp,"Time:%f,%f\n",m_startTime,m_endTime);	
	fprintf(fp,"OAD Sum:%d,\n",m_otherAction.size());//������������;
	if(m_otherAction.size() > 0)//����������ID������
	{
		for(int i = 0;i < m_otherAction.size();++ i)
		{
			fprintf(fp,"%d,\n",m_otherAction[i]->m_ID);	//д�붯��ID;
			fprintf(fp,"%s\n",m_otherAction[i]->m_actionName);//д�붯������;
		}
	}
	fprintf(fp,"AC Sum:%d,\n",m_actionCore.size());//Ԫ����������
	if(m_actionCore.size() > 0)
	{
		for(int i = 0;i < m_actionCore.size();++ i)
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
				m_actionCore[i]->m_stayState.color.fR,m_actionCore[i]->m_stayState.color.fG,m_actionCore[i]->m_stayState.color.fB,m_actionCore[i]->m_stayState.color.fA);
		}
	}
	//ASD������
	fprintf(fp,"ASD Sum:%d,\n",m_stateTable.size());//Ԫ����������
	if(m_stateTable.size() > 0)
	{
		for(int i = 0;i < m_stateTable.size();++ i)
		{//����д����������
			fprintf(fp,"time:%d,\n",m_stateTable[i]->time);
			if(m_stateTable[i]->isValidPosX) fprintf(fp,"1,%f,\n",m_stateTable[i]->position.x);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidPosY) fprintf(fp,"1,%f,\n",m_stateTable[i]->position.y);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorR) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.fR);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorG) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.fG);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorB) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.fB);
			else fprintf(fp,"0,0,\n");
			if(m_stateTable[i]->isValidColorA) fprintf(fp,"1,%f,\n",m_stateTable[i]->color.fA);
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
_XActionDescription::_XActionDescription()
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
		if(_XActionMananger::GetInstance().getActionDes(myID) == NULL) break;
	}
	m_ID = myID;
	sprintf(m_actionName,"ActionDescription%06d",m_ID);	//������Ҫע�����ֵ�Ψһ��
	//int len = strlen(m_actionName);
	//sprintf(m_actionName,"ActionDescription%6d",m_ID);	//������Ҫע�����ֵ�Ψһ��
	//int len = strlen(m_actionName);
	//for(int i = 0;i < len;++ i)
	//{
	//	if(m_actionName[i] == ' ') m_actionName[i] = '0';
	//}
}
void _XActionDescription::draw()
{//������Ҫ����ͼ��ĸ�������ͼ�������ڸǵ�����
	if(!m_isEnable) return;
	for(int i = 0;i < m_otherAction.size();++ i)
	{
		m_otherAction[i]->draw();
	}
	if(m_object != NULL) m_object->draw();
}
void _XActionDescription::setStart()
{
	//if(m_isEnd)
	{
		//m_isStart = XTrue;
		m_timeLine = 0.0f;
		m_isActionStart = XFalse;
		m_isEnd = XFalse;
		for(int i = 0;i < m_otherAction.size();++ i)
		{
			m_otherAction[i]->setStart();
		}
		for(int i = 0;i < m_actionCore.size();++ i)
		{
			m_actionCore[i]->setStart();
		}
	}
}
int _XActionDescription::getADChildIndex(_XActionDescription * p)	//����ƶ���AD�ڵ�ǰAD���б��еı��
{
	if(p == NULL) return -1;
	int sum = m_otherAction.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_otherAction[i] == p) return i;
	}
	return -1;
}
void _XActionDescription::setObject(_XObjectBasic *obj)
{
	if(!m_isEnable) return;
	_XActionObjectManager::GetInstance().decreaseAObject(m_object);	//ע����һ�п��ܻ��������
	m_object = obj;
	//�����������AC��Ŀ�궼��Ҫ�ı�
	int sum = m_actionCore.size();
	for(int i = 0;i < sum;++ i)
	{
		m_actionCore[i]->m_object = m_object;
	}
}
_XBool _XActionDescription::pushAActionCore(_XActionCore * p)
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
_XBool _XActionDescription::popAActionCore(_XActionCore * p)	//��������ɾ��һ��Ԫ��
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
_XBool _XActionDescription::replaceAAction(int index,_XActionCore * p)
{
	if(p == NULL) return XFalse;
	if(index < 0 || index >= m_actionCore.size()) return XFalse;
	XDELETE(m_actionCore[index]);
	p->m_object = m_object;
	m_actionCore[index] = p;
	return XTrue;
}
_XBool _XActionDescription::popAActionCore(int index)
{
	if(!m_isEnable) return XFalse;
	int size = m_actionCore.size();
	if(index < 0 || index >= size) return XFalse;
	XDELETE(m_actionCore[index]);
//	for(int j = index;j < size - 1;++ j)
//	{
//		m_actionCore[j] = m_actionCore[j + 1];
//	}
//	m_actionCore.pop_back();
	m_actionCore.erase(m_actionCore.begin() + index);
	return XTrue;
}
_XBool _XActionDescription::getIsChildActionDes(_XActionDescription * p)
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
_XBool _XActionDescription::pushAActionDescription(_XActionDescription * p)
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
_XBool _XActionDescription::popAActionDescription(int index)
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
void _XActionDescription::release()
{
	if(!m_isEnable) return;
	//�������Դ�ͷ���δʵ��
	for(int i = 0;i < m_actionCore.size();++ i)
	{
		XDELETE(m_actionCore[i]);
	}
	m_actionCore.clear();
	m_actionCore.swap(std::vector<_XActionCore *>());

	for(int i = 0;i < m_stateTable.size();++ i)
	{
		XDELETE(m_stateTable[i]);
	}
	m_stateTable.clear();
	m_stateTable.swap(std::vector<_XActionStateDes *>());
	m_isEnable = XFalse;
}
//+++++++++++++++++++++++++++++++++++++++
_XBool _XActionDescription::popASD(const _XActionStateDes* ASD)
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
			XDELETE(ASD);
			return XTrue;
		}
	}
	return XFalse;
}
_XBool _XActionDescription::insertASD(_XActionStateDes* ASD)
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
_XActionStateDes* _XActionDescription::getPreviousASD(int time,_XActionStateType type)
{
	if(time < m_minTime) return NULL;
	int sum = m_stateTable.size();
	for(int i = sum - 1;i >= 0;-- i)
	{
		if(m_stateTable[i]->time <= time)
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POS_X: 
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
			case ACTION_STATE_TYPE_SIZE_X: 
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
_XActionStateDes* _XActionDescription::getNextASD(int time,_XActionStateType type)
{
	if(time > m_maxTime) return NULL;
	int sum = m_stateTable.size();
	for(int i = 0;i < sum;++ i)
	{
		if(m_stateTable[i]->time >= time)
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POS_X: 
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
			case ACTION_STATE_TYPE_SIZE_X: 
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
float _XActionDescription::getASValue(int time,_XActionStateType type)
{
	_XActionStateDes * tempS = NULL;
	_XActionStateDes * tempE = NULL;
	tempS = getPreviousASD(time,type);
	tempE = getNextASD(time,type);
	if((tempS == NULL || tempS == tempE)&& tempE != NULL) 
	{
		switch(type)
		{
		case ACTION_STATE_TYPE_POS_X: return tempE->position.x; 
		case ACTION_STATE_TYPE_POS_Y: return tempE->position.y; 
		case ACTION_STATE_TYPE_COLOR_R: return tempE->color.fR; 
		case ACTION_STATE_TYPE_COLOR_G: return tempE->color.fG; 
		case ACTION_STATE_TYPE_COLOR_B: return tempE->color.fB; 
		case ACTION_STATE_TYPE_COLOR_A: return tempE->color.fA; 
		case ACTION_STATE_TYPE_ANGLE: return tempE->angle; 
		case ACTION_STATE_TYPE_SIZE_X: return tempE->size.x; 
		case ACTION_STATE_TYPE_SIZE_Y: return tempE->size.y; 
		} 
	}else
	if(tempE == NULL && tempS != NULL) 
	{
		switch(type)
		{
		case ACTION_STATE_TYPE_POS_X: return tempS->position.x; 
		case ACTION_STATE_TYPE_POS_Y: return tempS->position.y; 
		case ACTION_STATE_TYPE_COLOR_R: return tempS->color.fR; 
		case ACTION_STATE_TYPE_COLOR_G: return tempS->color.fG; 
		case ACTION_STATE_TYPE_COLOR_B: return tempS->color.fB; 
		case ACTION_STATE_TYPE_COLOR_A: return tempS->color.fA; 
		case ACTION_STATE_TYPE_ANGLE: return tempS->angle; 
		case ACTION_STATE_TYPE_SIZE_X: return tempS->size.x; 
		case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y; 
		} 
	}else
	if(tempS != NULL && tempE != NULL) 
	{
		if(tempE->time - tempS->time == 0) 
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POS_X: return tempS->position.x; 
			case ACTION_STATE_TYPE_POS_Y: return tempS->position.y; 
			case ACTION_STATE_TYPE_COLOR_R: return tempS->color.fR; 
			case ACTION_STATE_TYPE_COLOR_G: return tempS->color.fG; 
			case ACTION_STATE_TYPE_COLOR_B: return tempS->color.fB; 
			case ACTION_STATE_TYPE_COLOR_A: return tempS->color.fA; 
			case ACTION_STATE_TYPE_ANGLE: return tempS->angle; 
			case ACTION_STATE_TYPE_SIZE_X: return tempS->size.x; 
			case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y; 
			} 
		}else
		{
			switch(type)
			{
			case ACTION_STATE_TYPE_POS_X: return tempS->position.x + (tempE->position.x - tempS->position.x) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_POS_Y: return tempS->position.y + (tempE->position.y - tempS->position.y) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_R: return tempS->color.fR + (tempE->color.fR - tempS->color.fR) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_G: return tempS->color.fG + (tempE->color.fG - tempS->color.fG) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_B: return tempS->color.fB + (tempE->color.fB - tempS->color.fB) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_COLOR_A: return tempS->color.fA + (tempE->color.fA - tempS->color.fA) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_ANGLE: return tempS->angle + (tempE->angle - tempS->angle) * (time - tempS->time)/(tempE->time - tempS->time); 
			case ACTION_STATE_TYPE_SIZE_X: return tempS->size.x + (tempE->size.x - tempS->size.x) * (time - tempS->time)/(tempE->time - tempS->time);
			case ACTION_STATE_TYPE_SIZE_Y: return tempS->size.y + (tempE->size.y - tempS->size.y) * (time - tempS->time)/(tempE->time - tempS->time);
			} 
		}
	}
	return 0.0f;
}
_XActionState _XActionDescription::getActionState(int time)
{
	_XActionState retActionState;
	if(time < m_minTime || time > m_maxTime || m_stateTable.size() < 2) return retActionState;

	retActionState.position.x = getASValue(time,ACTION_STATE_TYPE_POS_X);
	retActionState.position.y = getASValue(time,ACTION_STATE_TYPE_POS_Y);
	retActionState.color.fR = getASValue(time,ACTION_STATE_TYPE_COLOR_R);
	retActionState.color.fG = getASValue(time,ACTION_STATE_TYPE_COLOR_G);
	retActionState.color.fB = getASValue(time,ACTION_STATE_TYPE_COLOR_B);
	retActionState.color.fA = getASValue(time,ACTION_STATE_TYPE_COLOR_A);
	retActionState.angle = getASValue(time,ACTION_STATE_TYPE_ANGLE);
	retActionState.size.x = getASValue(time,ACTION_STATE_TYPE_SIZE_X);
	retActionState.size.y = getASValue(time,ACTION_STATE_TYPE_SIZE_Y);
	retActionState.isEnable = XTrue;

	return retActionState;
}
//-------------------------------------------

void _XActionCore::setStart()	//���ÿ�ʼ
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
			m_object->setColor(m_stayState.color.fR,m_stayState.color.fG,
				m_stayState.color.fB,m_stayState.color.fA);
			m_object->setSize(m_stayState.size);
			m_object->setAngle(m_stayState.angle);
		}
	}
}
void _XActionCore::release()
{
	if(!m_isEnable) return;
//	for(int i = 0;i < m_moveData.size();++ i)
//	{
//		XDELETE(m_moveData[i]);
//	}
//	m_moveData.clear();
//	m_moveData.swap(std::vector<_XMoveData *>());
	if(m_moveDataSum == 1) XDELETE(m_moveData);
	else XDELETE_ARRAY(m_moveData);
	//XDELETE(m_object);	//�ͷ��������Դ,�����ͷ��������Դ
	m_moveDataSum = 0;

	m_isEnable = XFalse;
}