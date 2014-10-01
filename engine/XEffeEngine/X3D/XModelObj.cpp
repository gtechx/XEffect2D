//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"
#include "XModelObj.h"
#include "X3DWorld.h"

void removeRedundantSpace(char *p)	//ɾ�������Ļ�����β�Ŀո��
{
	int len = strlen(p);
	for(int i = 0;i < len;++ i)
	{
		if(i < len - 2 && p[i] == '/' && p[i + 1] == ' ')
		{//����'/'�Ŀո�ҲҪ����
			strcpy(p + i + 1,p + i + 2);
			-- i;
		}else
		if(p[i] == ' ' && p[i + 1] == '/')
		{//����'/'�Ŀո�ҲҪ����
			strcpy(p + i,p + i + 1);
			-- i;
		}else
		if(i == 0 && p[i] == ' ' && p[i + 1] != ' ')
		{//ͷ�Ŀո�Ҳ����
			strcpy(p + i,p + i + 1);
			-- i;
		}else
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == ' ')
		{//�����Ŀո�
			strcpy(p + i,p + i + 1);
			-- i;
		}else 
		if(i < len - 1 && p[i] == '\\' && p[i + 1] == '\n')
		{//����ֱ��ת���ɿո�
			p[i + 1] = ' ';
			strcpy(p + i,p + i + 1);
			-- i;
		}else 
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == '\0')
		{//�����Ŀո���
			p[i] = '\0';
			break;
		}else 
		if(i < len - 1 && p[i] == ' ' && p[i + 1] == '\n')
		{//�����Ŀո���
			strcpy(p + i,p + i + 1);
			-- i;
		}
		if(p[i] == '\0') break;
	}
}
_XBool _XModelObj::save(const char *filename)
{
	if(!m_isInited ||
		filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return XFalse;	//�ļ���ʧ��
	fprintf(fp,"# xiajia_1981@163.com\n");
	//����д������ģ������
	for(int i = 0;i < m_materialFilename.size();++ i)
	{
		fprintf(fp,"mtllib %s\n",m_materialFilename[i].c_str());
	}
	if(!saveMaterial()) 
	{
		fclose(fp);
		return XFalse;
	}
	for(int i = 0;i < m_point.size();++ i)
	{//����д�붥������
		fprintf(fp,"v %f %f %f\n",m_point[i].x,m_point[i].y,m_point[i].z);
	}
	for(int i = 0;i < m_texturePoint.size();++ i)
	{//����д����ͼ����
		fprintf(fp,"vt %f %f\n",m_texturePoint[i].x,1.0f - m_texturePoint[i].y);
	}
	for(int i = 0;i < m_normal.size();++ i)
	{//����д�뷨������
		fprintf(fp,"vn %f %f %f\n",m_normal[i].x,m_normal[i].y,m_normal[i].z);
	}
	for(int i = 0;i < m_group.size();++ i)
	{//����д��������
		if(m_group[i] == NULL) continue;
		//д������
		if(m_group[i]->groupName.length() > 0)
			fprintf(fp,"g %s\n",m_group[i]->groupName.c_str());
		//д���������
		if(m_group[i]->materialName.length() > 0)
			fprintf(fp,"usemtl %s\n",m_group[i]->materialName.c_str());

		if(m_group[i]->withTexInfo != 1 && m_group[i]->withNormalInfo != 1)
		{
			for(int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d %d %d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].vIndex.y,m_group[i]->face[j].vIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo == 1 && m_group[i]->withNormalInfo != 1)
		{
			for(int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d/%d %d/%d %d/%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].tIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].tIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].tIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo != 1 && m_group[i]->withNormalInfo == 1)
		{
			for(int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d//%d %d//%d %d//%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].nIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].nIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].nIndex.z);
			}
		}else
		if(m_group[i]->withTexInfo == 1 && m_group[i]->withNormalInfo == 1)
		{
			for(int j = 0;j < m_group[i]->face.size();++ j)
			{
				fprintf(fp,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",m_group[i]->face[j].vIndex.x,m_group[i]->face[j].tIndex.x,m_group[i]->face[j].nIndex.x,
					m_group[i]->face[j].vIndex.y,m_group[i]->face[j].tIndex.y,m_group[i]->face[j].nIndex.y,
					m_group[i]->face[j].vIndex.z,m_group[i]->face[j].tIndex.z,m_group[i]->face[j].nIndex.z);
			}
		}
	}
	fclose(fp);
	return XTrue;
}
std::string getObjInfo0FromStr(const char * p)
{
	if(p == NULL) return "";
	static char tempLineData[1024];	//ע�������1024���������
	std::string tmp;
	int len = strlen(p);
	if(isUTF8(p,len)) tmp = UTF82ANSI(p);
	else tmp = p;
	int s,e,b;
	if(!getFirtCanShowString(tmp.c_str(),s,e))
	{
		printf("���������:%s\n",tmp.c_str());
	}else
	{
		b = getCharPosition(tmp.c_str() + s,' ');
		memcpy(tempLineData,tmp.c_str() + s + b + 1,e - s - b);
		tempLineData[e - s - b] = '\0';
	}
	return tempLineData;
}
_XBool _XModelObj::load(const char *filename)	//���ļ�����������
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;	//�ļ���ʧ��
	char lineData[256];
	char tempLineData[256];
	//���濪ʼ�����ļ�
	_XVector3 tempData;
	_XVector2 tempData2;
	_XFaceInfo tempFaceInfo;
	_XGroupInfo *tempGroupInfo = NULL;
	int len = 0;
	while(true)
	{
		if(fgets(lineData,256,fp) == NULL) break;
		while(true)
		{
			len = strlen(lineData);
			if(len > 2 && lineData[len - 2] == '\\' && lineData[len - 1] == '\n')
			{//�ж������β�ǻ��з��ţ��������һ��
				if(fgets(tempLineData,256,fp) == NULL) break;
				strcat(lineData,tempLineData);
			}else
				break;
		}
		if(lineData[0] == '#') continue;	//ע��
		if(lineData[0] == 'v' && lineData[1] == ' ')
		{//��������
			sscanf(lineData,"v %f %f %f",&tempData.x,&tempData.y,&tempData.z);
			//tempData.y = -tempData.y;
			m_point.push_back(tempData);
		}else
		if(lineData[0] == 'v' && lineData[1] == 't')
		{//��ͼ����
			if(sscanf(lineData,"vt %f %f %f",&tempData.x,&tempData.y,&tempData.z) == 3)
			{
				if(tempData.x >= 0.0f) tempData2.x = tempData.x;
				else tempData2.x = 1.0f - tempData.x;
				if(tempData.y >= 0.0f) tempData2.y = 1.0f - tempData.y;
				else tempData2.y = -tempData.y;
				m_texturePoint.push_back(tempData2);
			}else
			if(sscanf(lineData,"vt %f %f",&tempData2.x,&tempData2.y) == 2)
			{
				if(tempData2.x >= 0.0f) tempData2.x = tempData2.x;
				else tempData2.x = 1.0f - tempData2.x;
				if(tempData2.y >= 0.0f) tempData2.y = 1.0f - tempData2.y;
				else tempData2.y = -tempData2.y;
				m_texturePoint.push_back(tempData2);
			}else
			{//��ʽ��֧��
				fclose(fp);
				return XFalse;
			}
		}else
		if(lineData[0] == 'v' && lineData[1] == 'n')
		{//��������
			sscanf(lineData,"vn %f %f %f",&tempData.x,&tempData.y,&tempData.z);
		//	tempData.x = -tempData.x;
		//	tempData.y = -tempData.y;
		//	tempData.z = -tempData.z;
			m_normal.push_back(tempData);
		}else
		if(lineData[0] == 'f' && lineData[1] == ' ')
		{//������
			if(tempGroupInfo == NULL)
			{
				tempGroupInfo = createMem<_XGroupInfo>();
				if(tempGroupInfo == NULL)
				{
					fclose(fp);
					return XFalse;
				}
				m_group.push_back(tempGroupInfo);
			}
			if(!readFaceInfo(tempGroupInfo,lineData))
			{
				fclose(fp);
				return XFalse;
			}
		}else
		if(lineData[0] == 's' && lineData[1] == ' ')
		{
		}else
		if(lineData[0] == 'g' && lineData[1] == ' ')
		{//��
			//��ȡ����
			std::string tmp = getObjInfo0FromStr(lineData);
			tempGroupInfo = getGroup(tmp.c_str());
			if(tempGroupInfo == NULL)
			{
				tempGroupInfo = createMem<_XGroupInfo>();
				if(tempGroupInfo == NULL)
				{
					fclose(fp);
					return XFalse;
				}
				m_group.push_back(tempGroupInfo);
				tempGroupInfo->groupName = tmp;
			}
		}else
		{
			len = getCharPosition(lineData,' ');
			if(len > 0)
			{
				memcpy(tempLineData,lineData,len);
				tempLineData[len] = '\0';
				if(strcmp(tempLineData,"usemtl") == 0)
				{//usemtl
					if(tempGroupInfo == NULL)
					{
						tempGroupInfo = createMem<_XGroupInfo>();
						if(tempGroupInfo == NULL)
						{
							fclose(fp);
							return XFalse;
						}
						m_group.push_back(tempGroupInfo);
					}
					//��ȡ������
					tempGroupInfo->materialName = getObjInfo0FromStr(lineData);
				}else
				if(strcmp(tempLineData,"mtllib") == 0)
				{//mtllib�����ļ�
					std::string tmp = getObjInfo0FromStr(lineData);
					if(tmp == "")
					{
						printf("���������:%s\n",lineData);
						continue;
					}
					m_materialFilename.push_back(tmp);
					tmp = mergeFilename(filename,tmp);
					if(loadMaterial(tmp.c_str()))
					{
						m_materialIndex.push_back(m_material.size());
					}else
					{
						printf("�����ļ���ʧ:%s\n",tmp.c_str());
					}
				}
			}
		}
		if(feof(fp)) break;	//�ļ���ȡ���֮��ֱ���˳�
	}

	fclose(fp);
	if(!checkData()) return XFalse;
	//��������������ײ����(��δ���)
	bool first = true;
	float xMin = 0.0f,xMax = 0.0f,yMin = 0.0f,yMax = 0.0f,zMin = 0.0f,zMax = 0.0f;
	_XVector3 tmpV;
	for(int i = 0;i < m_group.size();++ i)
	{
		for(int j = 0;j < m_group[i]->face.size();++ j)
		{
			tmpV = m_point[m_group[i]->face[j].vIndex.x - 1];
			if(first)
			{
				xMin = tmpV.x;xMax = tmpV.x;
				yMin = tmpV.y;yMax = tmpV.y;
				zMin = tmpV.z;zMax = tmpV.z;
				first = false;
			}else
			{
				if(tmpV.x < xMin) xMin = tmpV.x;
				if(tmpV.x > xMax) xMax = tmpV.x;
				if(tmpV.y < yMin) yMin = tmpV.y;
				if(tmpV.y > yMax) yMax = tmpV.y;
				if(tmpV.z < zMin) zMin = tmpV.z;
				if(tmpV.z > zMax) zMax = tmpV.z;
			}
			tmpV = m_point[m_group[i]->face[j].vIndex.y - 1];
			if(tmpV.x < xMin) xMin = tmpV.x;
			if(tmpV.x > xMax) xMax = tmpV.x;
			if(tmpV.y < yMin) yMin = tmpV.y;
			if(tmpV.y > yMax) yMax = tmpV.y;
			if(tmpV.z < zMin) zMin = tmpV.z;
			if(tmpV.z > zMax) zMax = tmpV.z;
			tmpV = m_point[m_group[i]->face[j].vIndex.z - 1];
			if(tmpV.x < xMin) xMin = tmpV.x;
			if(tmpV.x > xMax) xMax = tmpV.x;
			if(tmpV.y < yMin) yMin = tmpV.y;
			if(tmpV.y > yMax) yMax = tmpV.y;
			if(tmpV.z < zMin) zMin = tmpV.z;
			if(tmpV.z > zMax) zMax = tmpV.z;
		}
	}
	m_bandBox[0].set(xMin,yMin,zMin);
	m_bandBox[1].set(xMax,yMin,zMin);
	m_bandBox[2].set(xMax,yMax,zMin);
	m_bandBox[3].set(xMin,yMax,zMin);
	m_bandBox[4].set(xMin,yMin,zMax);
	m_bandBox[5].set(xMax,yMin,zMax);
	m_bandBox[6].set(xMax,yMax,zMax);
	m_bandBox[7].set(xMin,yMax,zMax);
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XModelObj::readFaceInfo(_XGroupInfo *group,char *lineData)
{
	/*{//���ļ��Ĳ���
		group->withTexInfo = 0;
		group->withNormalInfo = 1;
		int v[3],n[3];
		sscanf(lineData,"f %d//%d %d//%d %d//%d",&v[0],&n[0],&v[1],&n[1],&v[2],&n[2]);
		_XFaceInfo tempxxx;
		tempxxx.vIndex.x = v[0];
		tempxxx.vIndex.y = v[1];
		tempxxx.vIndex.z = v[2];
		tempxxx.nIndex.x = n[0];
		tempxxx.nIndex.y = n[1];
		tempxxx.nIndex.z = n[2];
		group->face.push_back(tempxxx);
		return XTrue;
	}*/
	removeRedundantSpace(lineData);
	int sumSlash = getCharSum(lineData,'/');	//���������
	int sumSpace = getCharSum(lineData,' ');	//���ݵ�����
	if(sumSpace < 3) return XFalse;
	_XFaceInfo tempFaceInfo;
	std::vector<_XVectorIndex3> tempData;
	int offset = getCharPosition(lineData,' ') + 1;
	_XVectorIndex3 tempIndex;
	if(sumSlash == 0)
	{//v
		if(group->withTexInfo == 1) return XFalse;
		if(group->withNormalInfo == 1) return XFalse;
		group->withTexInfo = 0;
		group->withNormalInfo = 0;

		for(int i = 0;i < sumSpace;++ i)
		{
			if(sscanf(lineData + offset,"%d ",&tempIndex.x) != 1) return XFalse;
			offset += getCharPosition(lineData + offset,' ') + 1;
			tempData.push_back(tempIndex);
		}
		for(int i = 0;i < sumSpace - 2;++ i)
		{
			tempFaceInfo.vIndex.x = tempData[0].x;
			tempFaceInfo.vIndex.y = tempData[i + 1].x;
			tempFaceInfo.vIndex.z = tempData[i + 2].x;
			group->face.push_back(tempFaceInfo);
		}
	}else
	if(sumSlash == sumSpace)
	{//v/t
		if(group->withTexInfo == 0) return XFalse;
		if(group->withNormalInfo == 1) return XFalse;
		group->withTexInfo = 1;
		group->withNormalInfo = 0;

		for(int i = 0;i < sumSpace;++ i)
		{
			if(sscanf(lineData + offset,"%d/%d ",&tempIndex.x,&tempIndex.y) != 2) return XFalse;
			offset += getCharPosition(lineData + offset,' ') + 1;
			tempData.push_back(tempIndex);
		}
		for(int i = 0;i < sumSpace - 2;++ i)
		{
			tempFaceInfo.vIndex.x = tempData[0].x;
			tempFaceInfo.vIndex.y = tempData[i + 1].x;
			tempFaceInfo.vIndex.z = tempData[i + 2].x;

			tempFaceInfo.tIndex.x = tempData[0].y;
			tempFaceInfo.tIndex.y = tempData[i + 1].y;
			tempFaceInfo.tIndex.z = tempData[i + 2].y;
			group->face.push_back(tempFaceInfo);
		}
	}else
	{// 
		tempIndex.x = getCharPosition(lineData,'/');
		if(lineData[tempIndex.x + 1] == '/')
		{//v//n
			if(group->withTexInfo == 1) return XFalse;
			if(group->withNormalInfo == 0) return XFalse;
			group->withTexInfo = 0;
			group->withNormalInfo = 1;
			for(int i = 0;i < sumSpace;++ i)
			{
				if(sscanf(lineData + offset,"%d//%d ",&tempIndex.x,&tempIndex.z) != 2) return XFalse;
				offset += getCharPosition(lineData + offset,' ') + 1;
				tempData.push_back(tempIndex);
			}
			for(int i = 0;i < sumSpace - 2;++ i)
			{
				tempFaceInfo.vIndex.x = tempData[0].x;
				tempFaceInfo.vIndex.y = tempData[i + 1].x;
				tempFaceInfo.vIndex.z = tempData[i + 2].x;
				tempFaceInfo.nIndex.x = tempData[0].z;
				tempFaceInfo.nIndex.y = tempData[i + 1].z;
				tempFaceInfo.nIndex.z = tempData[i + 2].z;
				group->face.push_back(tempFaceInfo);
			}
		}else
		{//v/t/n 
			if(group->withTexInfo == 0) return XFalse;
			if(group->withNormalInfo == 0) return XFalse;
			group->withTexInfo = 1;
			group->withNormalInfo = 1;
			for(int i = 0;i < sumSpace;++ i)
			{
				if(sscanf(lineData + offset,"%d/%d/%d ",&tempIndex.x,&tempIndex.y,&tempIndex.z) != 3) return XFalse;
				offset += getCharPosition(lineData + offset,' ') + 1;
				tempData.push_back(tempIndex);
			}
			for(int i = 0;i < sumSpace - 2;++ i)
			{
				tempFaceInfo.vIndex.x = tempData[0].x;
				tempFaceInfo.vIndex.y = tempData[i + 1].x;
				tempFaceInfo.vIndex.z = tempData[i + 2].x;
				tempFaceInfo.tIndex.x = tempData[0].y;
				tempFaceInfo.tIndex.y = tempData[i + 1].y;
				tempFaceInfo.tIndex.z = tempData[i + 2].y;
				tempFaceInfo.nIndex.x = tempData[0].z;
				tempFaceInfo.nIndex.y = tempData[i + 1].z;
				tempFaceInfo.nIndex.z = tempData[i + 2].z;
				group->face.push_back(tempFaceInfo);
			}
		}
	}
	return XTrue;
}
_XBool _XModelObj::loadMaterial(const char *filename)
{
	if(filename == NULL) return XFalse;
	//�������ͼ�ļ�����ȡ�����Ϣ
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;
	char lineData[256];
	char tempLineData[256];
	int startIndex = 0;
	int endIndex = 0;
	_XMaterialInfo * tempMaterialInfo = NULL;
	while(true)
	{
		if(fgets(lineData,256,fp) == NULL) break;
		if(lineData[0] == '#') continue;	//ע��
		if(!getFirstWord(lineData,startIndex,endIndex)) continue;
		memcpy(tempLineData,lineData + startIndex,endIndex - startIndex + 1);
		tempLineData[endIndex - startIndex + 1] = '\0';
		if(strcmp(tempLineData,"newmtl") == 0)
		{
			tempMaterialInfo = createMem<_XMaterialInfo>();
			if(tempMaterialInfo == NULL)
			{
				fclose(fp);
				return XFalse;
			}
			//sscanf(lineData,"newmtl %s",tempLineData);
			tempMaterialInfo->matetialName = getObjInfo0FromStr(lineData);
			m_material.push_back(tempMaterialInfo);
		}else
		if(strcmp(tempLineData,"map_Kd") == 0)
		{//��ͨ��ͼ
			if(tempMaterialInfo == NULL) return XFalse;
			//�����ȡ��Դ
			std::string tmp = getObjInfo0FromStr(lineData);
			if(tmp == "")
			{
				printf("���������:%s\n",lineData);
				tempMaterialInfo->withTexture = XFalse;
				continue;
			}
			tempMaterialInfo->mapKdName = tmp;
			//·������
			tmp = mergeFilename(filename,tmp);
			if(!tempMaterialInfo->textureData.load(tmp.c_str()))
			{
				printf("��ͼ�ļ���ȡʧ��:%s\n",tmp.c_str());
				tempMaterialInfo->withTexture = XFalse;
			}else
				tempMaterialInfo->withTexture = XTrue;
		}else
		if(strcmp(tempLineData,"map_bump") == 0)
		{//������ͼ
			if(tempMaterialInfo == NULL) return XFalse;
			//�����ȡ��Դ
			std::string tmp = getObjInfo0FromStr(lineData);
			if(tmp == "")
			{
				printf("���������:%s\n",lineData);
				tempMaterialInfo->withDumpTex = XFalse;
				continue;
			}
			tempMaterialInfo->mapDumpName = tmp;
			//·������
			tmp = mergeFilename(filename,tmp);
			if(!tempMaterialInfo->dumpTexData.load(tmp.c_str()))
			{
				printf("��ͼ�ļ���ȡʧ��:%s\n",tmp.c_str());
				tempMaterialInfo->withDumpTex = XFalse;
			}else
				tempMaterialInfo->withDumpTex = XTrue;
			//����shader
		//	if(tempMaterialInfo->normalShader == NULL)
		//	{
		//		tempMaterialInfo->normalShader = createMem<_XShaderGLSL>();
		//		if(tempMaterialInfo->normalShader == NULL) return XFalse;
		//		* tempMaterialInfo->normalShader = _X3DWorld::GetInstance().m_dumpMapShader;
		//		tempMaterialInfo->normalShader->connectTexture("Texture_00",NULL);
		//		tempMaterialInfo->normalShader->connectTexture("normalTexture",&tempMaterialInfo->dumpTexData.texture.m_texture);
		//	}
		}else
		if(strcmp(tempLineData,"illum") == 0)
		{//illum(��δʵ��)
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %d",&tempMaterialInfo->illum) != 1) return XFalse;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'a')
		{//ka
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ka.x,
				&tempMaterialInfo->ka.y,&tempMaterialInfo->ka.z) != 3) return XFalse;
			tempMaterialInfo->ka.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'd')
		{//kd
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->kd.x,
				&tempMaterialInfo->kd.y,&tempMaterialInfo->kd.z) != 3) return XFalse;
			tempMaterialInfo->kd.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 's')
		{//ks
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ks.x,
				&tempMaterialInfo->ks.y,&tempMaterialInfo->ks.z) != 3) return XFalse;
			tempMaterialInfo->ks.w = 1.0f;
		}else
		if(tempLineData[0] == 'K' && tempLineData[1] == 'e')
		{//ke
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->ke.x,
				&tempMaterialInfo->ke.y,&tempMaterialInfo->ke.z) != 3) return XFalse;
			tempMaterialInfo->ke.w = 0.0f;
		}else
		if(tempLineData[0] == 'N' && tempLineData[1] == 'i')
		{//Ni(��δ��ʽ)
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f",&tempMaterialInfo->Ni) != 1) return XFalse;
		}else
		if(tempLineData[0] == 'N' && tempLineData[1] == 's')
		{//Ns
			if(tempMaterialInfo == NULL) return XFalse;
			float tempNs = 0;
			if(sscanf(lineData + endIndex + 1," %f",&tempNs) != 1) return XFalse;
			tempMaterialInfo->shininess = tempNs * 0.001f * 128.0f;
		}else
		if(tempLineData[0] == 'T' && tempLineData[1] == 'r')
		{//Tr
			if(tempMaterialInfo == NULL) return XFalse;
			float tempAlpha = 0;
			if(sscanf(lineData + endIndex + 1," %f",&tempAlpha) != 1) return XFalse;
			tempMaterialInfo->alpha = 1.0f - tempAlpha;
		}else
		if(tempLineData[0] == 'T' && tempLineData[1] == 'f')
		{//Tf(��δʵ��)
			if(sscanf(lineData + endIndex + 1," %f %f %f",&tempMaterialInfo->Tf.x,
				&tempMaterialInfo->Tf.y,&tempMaterialInfo->Tf.z) != 3) return XFalse;
		}else
		if(tempLineData[0] == 'd')
		{//d
			if(tempMaterialInfo == NULL) return XFalse;
			if(sscanf(lineData + endIndex + 1," %f",&tempMaterialInfo->alpha) != 1) return XFalse;
		}
		if(feof(fp)) break;	//�ļ���ȡ���֮��ֱ���˳�
	}

	fclose(fp);
	return XTrue;
}
_XBool _XModelObj::saveMaterial()
{
	int sum = m_materialFilename.size();
	if(sum == 0) return XTrue;	//���û�в����ļ�ֱ�ӷ���
	int mSum = m_material.size();
	FILE * fp = NULL;
	for(int i = 0;i < sum;++ i)
	{
		if((fp = fopen(m_materialFilename[i].c_str(),"w")) == NULL) return XFalse;	//�ļ���ʧ��
		if(i == sum - 1)
		{//���һ��
			for(int j = m_materialIndex[i];j < mSum;++ j)
			{//д��mtl����
				fprintf(fp,"newmtl %s\n",m_material[j]->matetialName.c_str());	//д���������
				fprintf(fp,"\tNs %f\n",m_material[j]->shininess * 1000.0f);
				fprintf(fp,"\tNi %f\n",m_material[j]->Ni);
				fprintf(fp,"\td %f\n",m_material[j]->alpha);
				fprintf(fp,"\tTr %f\n",1.0f - m_material[j]->alpha);
				fprintf(fp,"\tTf %f %f %f\n",m_material[j]->Tf.x,m_material[j]->Tf.y,m_material[j]->Tf.z);
				fprintf(fp,"\tillum %d\n",m_material[j]->illum);
				fprintf(fp,"\tKa %f %f %f\n",m_material[j]->ka.x,m_material[j]->ka.y,m_material[j]->ka.z);
				fprintf(fp,"\tKd %f %f %f\n",m_material[j]->kd.x,m_material[j]->kd.y,m_material[j]->kd.z);
				fprintf(fp,"\tKs %f %f %f\n",m_material[j]->ks.x,m_material[j]->ks.y,m_material[j]->ks.z);
				fprintf(fp,"\tKe %f %f %f\n",m_material[j]->ke.x,m_material[j]->ke.y,m_material[j]->ke.z);
				fprintf(fp,"\tmap_Kd %s\n",m_material[j]->mapKdName.c_str());
				if(m_material[j]->withDumpTex) fprintf(fp,"\tmap_bump %s\n",m_material[j]->mapDumpName.c_str());
			}
		}else
		{
			for(int j = m_materialIndex[i];j < m_materialIndex[i + 1];++ j)
			{
				fprintf(fp,"newmtl %s\n",m_material[j]->matetialName.c_str());	//д���������
				fprintf(fp,"\tNs %f\n",m_material[j]->shininess / 128.0f * 1000.0f);
				fprintf(fp,"\tNi %f\n",m_material[j]->Ni);
				fprintf(fp,"\td %f\n",m_material[j]->alpha);
				fprintf(fp,"\tTr %f\n",1.0f - m_material[j]->alpha);
				fprintf(fp,"\tTf %f %f %f\n",m_material[j]->Tf.x,m_material[j]->Tf.y,m_material[j]->Tf.z);
				fprintf(fp,"\tillum %d\n",m_material[j]->illum);
				fprintf(fp,"\tKa %f %f %f\n",m_material[j]->ka.x,m_material[j]->ka.y,m_material[j]->ka.z);
				fprintf(fp,"\tKd %f %f %f\n",m_material[j]->kd.x,m_material[j]->kd.y,m_material[j]->kd.z);
				fprintf(fp,"\tKs %f %f %f\n",m_material[j]->ks.x,m_material[j]->ks.y,m_material[j]->ks.z);
				fprintf(fp,"\tKe %f %f %f\n",m_material[j]->ke.x,m_material[j]->ke.y,m_material[j]->ke.z);
				fprintf(fp,"\tmap_Kd %s\n",m_material[j]->mapKdName.c_str());
				if(m_material[j]->withDumpTex) fprintf(fp,"\tmap_bump %s\n",m_material[j]->mapDumpName.c_str());
			}
		}
		fclose(fp);
	}
	return XTrue;
}
_XBool _XModelObj::checkData()
{
	int vSum = m_point.size();
	int tSum = m_texturePoint.size();
	int nSum = m_normal.size();
	_XGroupInfo * tempGroup = NULL;
	float *vPointers = NULL;
	float *tPointers = NULL;
	float *nPointers = NULL;
	for(int i = 0;i < m_group.size();++ i)
	{
		tempGroup = m_group[i];
		//��ȡ��ͼ�ı��
		if(tempGroup->withTexInfo == 1)
		{
			tempGroup->materialIndex = getMaterialIndex(tempGroup->materialName);
			if(tempGroup->materialIndex == -1)
			{
				printf("ȱʧ����˵����%s\n",tempGroup->groupName.c_str());
				tempGroup->withTexInfo = 0;
			}
		}
		//������ͼ����
		if(vPointers != NULL) XDELETE_ARRAY(vPointers);
		vPointers = createArrayMem<float>(tempGroup->face.size() * 9);
		if(vPointers == NULL) return XFalse;
		if(tempGroup->withTexInfo == 1)
		{
			if(tPointers != NULL) XDELETE_ARRAY(tPointers);
			tPointers = createArrayMem<float>(tempGroup->face.size() * 6);
			if(tPointers == NULL) return XFalse;
		}
		if(tempGroup->withNormalInfo == 1)
		{
			if(nPointers != NULL) XDELETE_ARRAY(nPointers);
			nPointers = createArrayMem<float>(tempGroup->face.size() * 9);
			if(nPointers == NULL) return XFalse;
		}
		//������ݵ���ȷ��
		for(int j = 0;j < tempGroup->face.size();++ j)
		{
			if(tempGroup->face[j].vIndex.x < 0) tempGroup->face[j].vIndex.x = vSum + tempGroup->face[j].vIndex.x + 1;
			if(tempGroup->face[j].vIndex.y < 0) tempGroup->face[j].vIndex.y = vSum + tempGroup->face[j].vIndex.y + 1;
			if(tempGroup->face[j].vIndex.z < 0) tempGroup->face[j].vIndex.z = vSum + tempGroup->face[j].vIndex.z + 1;

			if(tempGroup->face[j].vIndex.x <= 0 || tempGroup->face[j].vIndex.x > vSum) return XFalse;
			if(tempGroup->face[j].vIndex.y <= 0 || tempGroup->face[j].vIndex.y > vSum) return XFalse;
			if(tempGroup->face[j].vIndex.z <= 0 || tempGroup->face[j].vIndex.z > vSum) return XFalse;

			vPointers[j * 9 + 0] = m_point[tempGroup->face[j].vIndex.x - 1].x;
			vPointers[j * 9 + 1] = m_point[tempGroup->face[j].vIndex.x - 1].y;
			vPointers[j * 9 + 2] = m_point[tempGroup->face[j].vIndex.x - 1].z;
			vPointers[j * 9 + 3] = m_point[tempGroup->face[j].vIndex.y - 1].x;
			vPointers[j * 9 + 4] = m_point[tempGroup->face[j].vIndex.y - 1].y;
			vPointers[j * 9 + 5] = m_point[tempGroup->face[j].vIndex.y - 1].z;
			vPointers[j * 9 + 6] = m_point[tempGroup->face[j].vIndex.z - 1].x;
			vPointers[j * 9 + 7] = m_point[tempGroup->face[j].vIndex.z - 1].y;
			vPointers[j * 9 + 8] = m_point[tempGroup->face[j].vIndex.z - 1].z;

			if(tempGroup->withTexInfo == 1)
			{
				if(tempGroup->face[j].tIndex.x < 0) tempGroup->face[j].tIndex.x = tSum - tempGroup->face[j].tIndex.x + 1;
				if(tempGroup->face[j].tIndex.y < 0) tempGroup->face[j].tIndex.y = tSum - tempGroup->face[j].tIndex.y + 1;
				if(tempGroup->face[j].tIndex.z < 0) tempGroup->face[j].tIndex.z = tSum - tempGroup->face[j].tIndex.z + 1;

				if(tempGroup->face[j].tIndex.x <= 0 || tempGroup->face[j].tIndex.x > tSum) return XFalse;
				if(tempGroup->face[j].tIndex.y <= 0 || tempGroup->face[j].tIndex.y > tSum) return XFalse;
				if(tempGroup->face[j].tIndex.z <= 0 || tempGroup->face[j].tIndex.z > tSum) return XFalse;

				tPointers[j * 6 + 0] = m_texturePoint[tempGroup->face[j].tIndex.x - 1].x;
				tPointers[j * 6 + 1] = m_texturePoint[tempGroup->face[j].tIndex.x - 1].y;
				tPointers[j * 6 + 2] = m_texturePoint[tempGroup->face[j].tIndex.y - 1].x;
				tPointers[j * 6 + 3] = m_texturePoint[tempGroup->face[j].tIndex.y - 1].y;
				tPointers[j * 6 + 4] = m_texturePoint[tempGroup->face[j].tIndex.z - 1].x;
				tPointers[j * 6 + 5] = m_texturePoint[tempGroup->face[j].tIndex.z - 1].y;
			}
			if(tempGroup->withNormalInfo == 1)
			{
				if(tempGroup->face[j].nIndex.x < 0) tempGroup->face[j].nIndex.x = nSum + tempGroup->face[j].nIndex.x + 1;
				if(tempGroup->face[j].nIndex.y < 0) tempGroup->face[j].nIndex.y = nSum + tempGroup->face[j].nIndex.y + 1;
				if(tempGroup->face[j].nIndex.z < 0) tempGroup->face[j].nIndex.z = nSum + tempGroup->face[j].nIndex.z + 1;

				if(tempGroup->face[j].nIndex.x <= 0 || tempGroup->face[j].nIndex.x > nSum) return XFalse;
				if(tempGroup->face[j].nIndex.y <= 0 || tempGroup->face[j].nIndex.y > nSum) return XFalse;
				if(tempGroup->face[j].nIndex.z <= 0 || tempGroup->face[j].nIndex.z > nSum) return XFalse;

				nPointers[j * 9 + 0] = m_normal[tempGroup->face[j].nIndex.x - 1].x;
				nPointers[j * 9 + 1] = m_normal[tempGroup->face[j].nIndex.x - 1].y;
				nPointers[j * 9 + 2] = m_normal[tempGroup->face[j].nIndex.x - 1].z;
				nPointers[j * 9 + 3] = m_normal[tempGroup->face[j].nIndex.y - 1].x;
				nPointers[j * 9 + 4] = m_normal[tempGroup->face[j].nIndex.y - 1].y;
				nPointers[j * 9 + 5] = m_normal[tempGroup->face[j].nIndex.y - 1].z;
				nPointers[j * 9 + 6] = m_normal[tempGroup->face[j].nIndex.z - 1].x;
				nPointers[j * 9 + 7] = m_normal[tempGroup->face[j].nIndex.z - 1].y;
				nPointers[j * 9 + 8] = m_normal[tempGroup->face[j].nIndex.z - 1].z;
			}
		}
		if(!tempGroup->vbo.init(tempGroup->face.size() * 3,vPointers,tempGroup->withTexInfo == 1?tPointers:NULL,
			tempGroup->withNormalInfo == 1?nPointers:NULL,NULL)) 
		{
			XDELETE_ARRAY(vPointers);
			XDELETE_ARRAY(tPointers);
			XDELETE_ARRAY(nPointers);
			return XFalse;
		}
		XDELETE_ARRAY(vPointers);
		XDELETE_ARRAY(tPointers);
		XDELETE_ARRAY(nPointers);
	}
	//����ʹ�ò���������Ч
	for(int i = 0;i < m_material.size(); ++ i)
	{
		m_material[i]->setData();
	}
	return XTrue;
}
int _XModelObj::getMaterialIndex(const string &str)
{
	for(int i = 0;i < m_material.size();++ i)
	{
		if(m_material[i]->matetialName == str) return i;
	}
	return -1;
}
void _XModelObj::draw(_XBool withTex,const _XBasic3DObject *base)
{
	if(!m_isInited) return;
	updateMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();

	if(base != NULL) glMultMatrixf(base->getMatrix());
	glMultMatrixf(m_matrix);
	//glTranslatef(m_position.x,m_position.y,m_position.z);
	//glRotatef(m_angle.x,1,0,0);
	//glRotatef(m_angle.y,0,1,0);
	//glRotatef(m_angle.z,0,0,1);
	//glScalef(m_size.x,m_size.y,m_size.z);
	//drawOrigin();
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	_XGroupInfo * tempGroup = NULL;
	for(int i = 0;i < m_group.size();++ i)
	{
		tempGroup = m_group[i];
		_XBool needTexture = tempGroup->withTexInfo == 1 && withTex && m_material[tempGroup->materialIndex]->withTexture;
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		if(tempGroup->withTexInfo == 1 && withTex) 
		{//���������ͼ
			m_material[tempGroup->materialIndex]->material.usetMaterial();
			if(m_material[tempGroup->materialIndex]->withTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,
					m_material[tempGroup->materialIndex]->textureData.texture.m_texture);
				if(m_material[tempGroup->materialIndex]->withDumpTex)
					_X3DWorld::GetInstance().useShadow(needTexture,SHADER_DUMP_AND_SHADOW,m_material[tempGroup->materialIndex]->dumpTexData.texture.m_texture);	//ʹ��dumpshader������dump��ͼ
				else
					_X3DWorld::GetInstance().useShadow(needTexture,SHADER_SHADOW);
				//glColor4f(m_color.fR,m_color.fG,m_color.fB,m_color.fA * m_material[tempGroup->materialIndex]->alpha);
			}else
			{//�в�������ͼ
				glColor4fv(m_color);
				_X3DWorld::GetInstance().useShadow(needTexture,SHADER_SHADOW);
			}
		}else
		{//�����������ͼ
			_X3DWorld::GetInstance().m_worldMaterial.usetMaterial();	//���û�в�����Ϣ��ʹ��Ĭ�ϵĲ���
			glColor4fv(m_color);
			_X3DWorld::GetInstance().useShadow(needTexture,SHADER_SHADOW);
		}

		tempGroup->vbo.drawByArray(GL_TRIANGLES,withTex);

		_X3DWorld::GetInstance().removeShadow();
		
		glPopAttrib();
	}

	//glEnable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}

_XMaterialInfo * _XModelObj::getMaterialInfo(const string &MatName)
{
	for(int i = 0;i < m_material.size();++ i)
	{
		if(m_material[i]->matetialName == MatName) return m_material[i];
	}
	return NULL;
}
//_XBool _XModelObj::setNormalShader(const string &MatName,_XShaderGLSL *shader)
//{
//	_XMaterialInfo *tempMat = getMaterialInfo(MatName);
//	if(tempMat == NULL) return XFalse;
//	tempMat->normalShader = shader;
//	return XTrue;
//}
//void _XModelObj::setWithDumpMap(_XBool flag)
//{
//	if(flag)
//	{
//		for(int i = 0;i < m_material.size();++ i)
//		{
//			if(m_material[i]->normalShader != NULL) m_material[i]->withDumpTex = XTrue;
//		}
//	}else
//	{
//		for(int i = 0;i < m_material.size();++ i)
//		{
//			m_material[i]->withDumpTex = XFalse;
//		}
//	}
//}
_XGroupInfo *_XModelObj::getGroup(const char *name)
{
	if(name == NULL) return NULL;
	for(int i = 0;i < m_group.size();++ i)
	{
		if(strcmp(m_group[i]->groupName.c_str(),name) == 0) return m_group[i];
	}
	return NULL;
}
void _XModelObj::release()
{
	if(!m_isInited) return;
	m_point.clear();
	m_texturePoint.clear();
	m_normal.clear();
	for(int i = 0;i < m_material.size();++ i)
	{
		XDELETE(m_material[i]);
	}
	m_material.clear();
	m_materialFilename.clear();
	m_materialIndex.clear();
	for(int i = 0;i < m_group.size();++ i)
	{
		XDELETE(m_group[i]);
	}
	m_group.clear();
	m_isInited = XFalse;
}