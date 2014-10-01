//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XActionObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"
//��action�е���Դ���й������
#include "string.h"

char * _XActionObjectDescription::getStrFromData()
{//������ת�����ַ���
	if(!m_isEnable) return NULL;
	char * str = NULL;
	str = createArrayMem<char>(MAX_ACTION_OBJECT_DES_LEN);
	if(str == NULL) return NULL;
	//���濪ʼ��ӡ����
	int offset = 0;
	sprintf(str + offset,"ID:%d,\n",m_objectID);
	offset = strlen(str);
	sprintf(str + offset,"Name:%s\n",m_objectFilename);
	offset = strlen(str);
	sprintf(str + offset,"Type:%d,\n",m_objectType);
	offset = strlen(str);
	switch(m_objectType)
	{
	case OBJ_NULL:break;		//do nothing
	case OBJ_SPRITE:
		{
			char *paramStr = ((_XSpriteParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XDELETE_ARRAY(paramStr);
		}
		break;
	case OBJ_FRAMEEX:
		{
			char *paramStr = ((_XFrameExParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XDELETE_ARRAY(paramStr);
		}
		break;
		break;
	}
	return str;
}
int _XActionObjectDescription::getDataFromStr(const char * str)
{//���ַ�����ȡ����
	if(m_isEnable) return 0;
	if(str == NULL) return 0;
	//���濪ʼ��ӡ����
	int offset = 0;
	if(sscanf(str + offset,"ID:%d,\n",&m_objectID) != 1) return offset;
	offset += getCharPosition(str + offset,',') + 1;
	if(m_objectFilename == NULL) m_objectFilename = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return 0;
	if(sscanf(str + offset,"Name:%s\n",m_objectFilename) != 1) return offset;	//�ļ����в���ʹ��','��
	offset += getCharPosition(str + offset,',') + 1;	//�������⣡��������@@@@@@@@@@@@@@@
	if(sscanf(str + offset,"Type:%d,\n",&m_objectType) != 1) return offset;
	offset += getCharPosition(str + offset,',') + 1;
	switch(m_objectType)
	{
	case OBJ_NULL:
		if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
		break;		//do nothing
	case OBJ_SPRITE:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
			_XSpriteParamData * paramData = createMem<_XSpriteParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XDELETE_ARRAY(m_objectFilename);
				return 0;//��ȡʧ��
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
			_XFrameExParamData * paramData = createMem<_XFrameExParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XDELETE_ARRAY(m_objectFilename);
				return 0;//��ȡʧ��
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return offset;
}
_XBool _XActionObjectDescription::getDataFromFile(FILE *fp)
{
	if(m_isEnable) return XFalse;
	if(fp == NULL) return XFalse;
	if(fscanf(fp,"ID:%d,\n",&m_objectID) != 1) return XFalse;
	if(m_objectFilename == NULL) m_objectFilename = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return XFalse;
	if(fscanf(fp,"Name:%s\n",m_objectFilename) != 1) return XFalse;
	if(fscanf(fp,"Type:%d,\n",&m_objectType) != 1) return XFalse;
	switch(m_objectType)
	{
	case OBJ_NULL:
		if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
		break;		//do nothing
	case OBJ_SPRITE:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
			_XSpriteParamData * paramData = createMem<_XSpriteParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			if(m_paramData != NULL) XDELETE(m_paramData);//�ͷŲ���Ҫ������
			_XFrameExParamData * paramData = createMem<_XFrameExParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return XTrue;
}
_XObjectBasic * _XActionObjectDescription::createAObject()
{//����������������½���һ������
	if(!m_isEnable) return NULL;
	switch(m_objectType)
	{
	case OBJ_NULL: return NULL;break;
	case OBJ_SPRITE: 
		{
			_XSprite *temp = createMem<_XSprite>();
			if(temp == NULL) return NULL;
			if(m_paramData == NULL) return NULL;
			_XSpriteParamData * tempParamData = (_XSpriteParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition,tempParamData->changePoint))
			{
				XDELETE(temp);
				return NULL;
			}	//Ŀǰ��ʹ��Ĭ�ϲ�������Ϊ�������治������������Ҫ����
			//ע�����
			_XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	case OBJ_FRAMEEX: 
		{
			_XFrameEx *temp = createMem<_XFrameEx>();
			if(temp == NULL) return NULL;
			if(m_paramData == NULL) return NULL;
			_XFrameExParamData * tempParamData = (_XFrameExParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition))
			{
				XDELETE(temp);
				return NULL;
			}	//Ŀǰ��ʹ��Ĭ�ϲ�������Ϊ�������治������������Ҫ����
			//��ʼ������Ĳ���
			temp->setAttribute(tempParamData->pos,tempParamData->loop,tempParamData->endImmediately,tempParamData->startFrame,
				tempParamData->actionSpeed,tempParamData->disappearAtEnd,tempParamData->isOverturn);
			//ע�����
			_XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	}
	return NULL;
}