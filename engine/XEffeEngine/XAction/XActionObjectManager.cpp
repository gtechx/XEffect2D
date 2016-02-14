#include "XStdHead.h"
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
namespace XE{
char * XActionObjectDescription::getStrFromData()
{//������ת�����ַ���
	if(!m_isEnable) return NULL;
	char * str = NULL;
	str = XMem::createArrayMem<char>(MAX_ACTION_OBJECT_DES_LEN);
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
			char *paramStr = ((XSpriteParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XMem::XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XMem::XDELETE_ARRAY(paramStr);
		}
		break;
	case OBJ_FRAMEEX:
		{
			char *paramStr = ((XFrameExParamData *)m_paramData)->getStrFromData();
			if(paramStr == NULL)
			{
				XMem::XDELETE_ARRAY(str);
				return NULL;
			}
			sprintf(str + offset,"%s",paramStr);
			XMem::XDELETE_ARRAY(paramStr);
		}
		break;
		break;
	}
	return str;
}
void XActionObjectDescription::releaseParamData()
{
	if(m_paramData == NULL) return;
	switch(m_objectType)
	{
	case OBJ_NULL:
		XMem::XDELETE(m_paramData);
		break;
	case OBJ_SPRITE:
		{
			XSpriteParamData *tmp = (XSpriteParamData *)m_paramData;
			XMem::XDELETE(tmp);
			m_paramData = NULL;
		}
		break;
	case OBJ_FRAMEEX:
		{
			XFrameExParamData *tmp = (XFrameExParamData *)m_paramData;
			XMem::XDELETE(tmp);
			m_paramData = NULL;
		}
		break;
	}
}
int XActionObjectDescription::getDataFromStr(const char * str)
{//���ַ�����ȡ����
	if(m_isEnable ||
		str == NULL) return 0;
	//���濪ʼ��ӡ����
	int offset = 0;
	if(sscanf(str + offset,"ID:%d,\n",&m_objectID) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	if(m_objectFilename == NULL) m_objectFilename = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return 0;
	if(sscanf(str + offset,"Name:%s\n",m_objectFilename) != 1) return offset;	//�ļ����в���ʹ��','��
	offset += XString::getCharPosition(str + offset,',') + 1;	//�������⣡��������@@@@@@@@@@@@@@@
	XObjectType tmpType;
	if(sscanf(str + offset,"Type:%d,\n",&tmpType) != 1) return offset;
	offset += XString::getCharPosition(str + offset,',') + 1;
	switch(tmpType)
	{
	case OBJ_NULL:
		releaseParamData();
		break;		//do nothing
	case OBJ_SPRITE:
		{
			releaseParamData();
			XSpriteParamData * paramData = XMem::createMem<XSpriteParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return 0;//��ȡʧ��
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			releaseParamData();
			XFrameExParamData * paramData = XMem::createMem<XFrameExParamData>();
			int len = paramData->getDataFromStr(str + offset);
			if(len == 0) 
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return 0;//��ȡʧ��
			}
			offset += len;
			m_paramData = paramData;
		}
		break;
	}
	m_objectType = tmpType;
	m_isEnable = XTrue;
	return offset;
}
XBool XActionObjectDescription::getDataFromFile(FILE *fp)
{
	if(m_isEnable ||
		fp == NULL) return XFalse;
	if(fscanf(fp,"ID:%d,\n",&m_objectID) != 1) return XFalse;
	if(m_objectFilename == NULL) m_objectFilename = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	if(m_objectFilename == NULL) return XFalse;
	if(fscanf(fp,"Name:%s\n",m_objectFilename) != 1) return XFalse;
	if(fscanf(fp,"Type:%d,\n",&m_objectType) != 1) return XFalse;
	switch(m_objectType)
	{
	case OBJ_NULL:
		releaseParamData();
		break;		//do nothing
	case OBJ_SPRITE:
		{
			releaseParamData();
			XSpriteParamData * paramData = XMem::createMem<XSpriteParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	case OBJ_FRAMEEX:
		{
			releaseParamData();
			XFrameExParamData * paramData = XMem::createMem<XFrameExParamData>();
			if(!paramData->getDataFromFile(fp))
			{
				XMem::XDELETE_ARRAY(m_objectFilename);
				return XFalse;
			}
			m_paramData = paramData;
		}
		break;
	}
	m_isEnable = XTrue;
	return XTrue;
}
XObjectBasic * XActionObjectDescription::createAObject()
{//����������������½���һ������
	if(!m_isEnable) return NULL;
	switch(m_objectType)
	{
	case OBJ_NULL: return NULL;break;
	case OBJ_SPRITE: 
		{
			XSprite *temp = XMem::createMem<XSprite>();
			if(temp == NULL || m_paramData == NULL) return NULL;
			XSpriteParamData * tempParamData = (XSpriteParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition,tempParamData->changePoint))
			{
				XMem::XDELETE(temp);
				return NULL;
			}	//Ŀǰ��ʹ��Ĭ�ϲ�������Ϊ�������治������������Ҫ����
			//ע�����
			XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	case OBJ_FRAMEEX: 
		{
			XFrameEx *temp = XMem::createMem<XFrameEx>();
			if(temp == NULL || m_paramData == NULL) return NULL;
			XFrameExParamData * tempParamData = (XFrameExParamData *) m_paramData;
			if(!temp->init(m_objectFilename,tempParamData->resourcePosition))
			{
				XMem::XDELETE(temp);
				return NULL;
			}	//Ŀǰ��ʹ��Ĭ�ϲ�������Ϊ�������治������������Ҫ����
			//��ʼ������Ĳ���
			temp->setAttribute(tempParamData->pos,tempParamData->loop,tempParamData->endImmediately,tempParamData->startFrame,
				tempParamData->actionSpeed,tempParamData->disappearAtEnd,tempParamData->isOverturn);
			//ע�����
			XActionObjectManager::GetInstance().addAObject(temp,this);
			return temp;
		}
		break;
	}
	return NULL;
}
}