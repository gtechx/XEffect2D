#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTerrain.h"
#include "X3DWorld.h"

#include "XResourcePack.h"
#include "XPixelCommon.h"
#include "XMath/XBezierInterpolation.h"
#include "XPixelCommon.h"

namespace XE{
bool XTerrain::loadFromFolder(const char *filename,unsigned char *data)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"rb")) == NULL) return XFalse;	//�������ļ�ʧ��

	fread(data,1,m_width * m_height,fp);
	if(ferror(fp))
	{
		fclose(fp);
		XMem::XDELETE_ARRAY(data);
		return XFalse;
	}
	fclose(fp);
	return true;
}
bool XTerrain::loadFromPacker(const char *filename,unsigned char *data)	//��ѹ������������Դ
{
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL) return XFalse;
	memcpy(data,p,m_width * m_height);

	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XTerrain::loadFromWeb(const char *filename,unsigned char *data)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XTerrain::init(const char *mapFilename,const char *texFilename,int w,int h,
					   XBool withColor,
					   XResourcePosition resourcePosition)
{
	if(m_isInited) return XTrue;
	if(mapFilename == NULL) return XFalse;
	if(w <= 0 || h <= 0) return XFalse;

	m_width = w;
	m_height = h;
	m_withColor = withColor;
	unsigned char *tempTerrainData = XMem::createArrayMem<unsigned char>(m_width * m_height);
	if(tempTerrainData == NULL) return XFalse;
	if(resourcePosition == RESOURCE_SYSTEM_DEFINE) resourcePosition = getDefResPos();
	switch(resourcePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(mapFilename,tempTerrainData)) return false;
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(mapFilename,tempTerrainData)) return false;
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(mapFilename,tempTerrainData)) return false;
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(mapFilename,tempTerrainData) && !loadFromFolder(mapFilename,tempTerrainData) &&
			!loadFromWeb(mapFilename,tempTerrainData)) return false;
		break;
	}
//	//д������ For Test
//	if((fp = fopen("Test.dat","wb")) == NULL) return XFalse;	//�������ļ�ʧ��
//	for(int i = 0;i < h;i += 2)
//	{
//		for(int j = 0;j < w;j += 2)
//		{
//			fwrite(&tempTerrainData[i * h + j],1,sizeof(char),fp);
//		}
//	}
//	fclose(fp);
	m_terrainData = XMem::createArrayMem<float>(m_width * m_height);
	if(m_terrainData == NULL)
	{
		XMem::XDELETE_ARRAY(m_terrainData);
		return XFalse;
	}
	//��������ת��
	for(int i = 0;i < m_width * m_height;++ i)
	{
		m_terrainData[i] = tempTerrainData[i];
	}
	XMem::XDELETE_ARRAY(tempTerrainData);	//ɾ����ʱ����
	//������ͼ����
	if(texFilename == NULL) m_withTexture = XFalse;
	else 
	{
		m_withTexture = XTrue;
		if(!m_texture.load(texFilename,resourcePosition)) 
		{
			XMem::XDELETE_ARRAY(m_terrainData);
			return XFalse;
		}
		m_textureName = texFilename;
	}

	m_position.set(0.0f,0.0f,0.0f);
	m_scale.set(1.0f,1.0f,1.0f);
	m_angle.set(0.0f,0.0f,0.0f);
	//���潨��VBO
	m_allFaceSum = (m_width - 1) * (m_height - 1) * 2;
	m_allPointSum = m_width * m_height;
	m_index = XMem::createArrayMem<unsigned int>(m_allFaceSum * 3);
	if(m_index == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(withColor)
	{
		m_c = XMem::createArrayMem<float>(m_allPointSum * 4);
		if(m_c == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_v = XMem::createArrayMem<float>(m_allPointSum * 3);	//����
	m_n = XMem::createArrayMem<float>(m_allPointSum * 3);	//����
	m_faceN = XMem::createArrayMem<XVector3>(m_allFaceSum);	//�淨��
	if(m_n == NULL || m_v == NULL || m_faceN == NULL)// || m_pointN == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(m_withTexture)
	{
		m_t = XMem::createArrayMem<float>(m_allPointSum * 2);
		if(m_t == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_coordOffsetX = 1.0f / m_width;
	m_coordOffsetY = 1.0f / m_height;
//	int tempIndex = 0;
	int indexT = 0;
	int indexV = 0;
	int indexC = 0;
	float curCoordX = 0.0f;
	float curCoordY = 0.0f;
	int i,j;
	
	int faceIndex = 0;
	int indexI = 0;
	for(j = 0;j < m_height;++ j,curCoordY += m_coordOffsetY)
	{
		indexT = j * m_width * 2;
		indexV = j * m_width * 3;
		indexC = j * m_width * 4;
		indexI = j * m_width;
		faceIndex = j * (m_width - 1) * 2 * 3;
		for(i = 0,curCoordX = 0.0f;i < m_width;++i)
		{
			if(m_withTexture)
			{
				m_t[indexT] = curCoordX;
				m_t[indexT + 1] = curCoordY;
				indexT += 2;
				curCoordX += m_coordOffsetX;
			}
			m_v[indexV] = i;
			m_v[indexV + 2] = j;
			indexV += 3;
			if(m_withColor)
			{
				m_c[indexC] = 1.0f;
				m_c[indexC + 1] = 1.0f;
				m_c[indexC + 2] = 1.0f;
				m_c[indexC + 3] = m_reflectRate;
				indexC += 4;
			}
		}
		if(j < m_height - 1)
		{
			for(i = 0;i < m_width - 1;++i)
			{
				m_index[faceIndex] = indexI;
				m_index[faceIndex + 1] = indexI + m_width;
				m_index[faceIndex + 2] = indexI + 1;
				m_index[faceIndex + 3] = indexI + 1;
				m_index[faceIndex + 4] = indexI + m_width;
				m_index[faceIndex + 5] = indexI + 1 + m_width;

				faceIndex += 2 * 3;
				++indexI;
			}
		}
	}
	
	m_isInited = XTrue;
	if(!updateHeight(m_terrainData))
	{
		releaseMemory();
		m_isInited = XFalse;
		return XFalse;
	}

	return XTrue;
}
XBool XTerrain::initEx(const char *picFilename,const char * texFilename,
						 XBool withColor,XResourcePosition resourcePosition)
{
	if(m_isInited) return XTrue;
	XCurPixel tmpPixel;
	if(!tmpPixel.load(picFilename,resourcePosition)) return XFalse;
	float *tmpData = XMem::createArrayMem<float>(tmpPixel.getWidth() * tmpPixel.getHeight());
	if(tmpData == NULL) return XFalse;
	unsigned char * p = tmpPixel.getPixel();
	switch(tmpPixel.getColorMode())
	{
	case COLOR_RGBA:
		for(int i = 0;i < tmpPixel.getWidth() * tmpPixel.getHeight();++ i)
		{
			tmpData[i] = p[i << 2];
		}
		break;
	case COLOR_RGB:
		for(int i = 0;i < tmpPixel.getWidth() * tmpPixel.getHeight();++ i)
		{
			tmpData[i] = p[i * 3];
		}
		break;
	case COLOR_GRAY:
		for(int i = 0;i < tmpPixel.getWidth() * tmpPixel.getHeight();++ i)
		{
			tmpData[i] = p[i];
		}
		break;
	default:
		XMem::XDELETE_ARRAY(tmpData);
		return 0;
		break;
	}
	if(!init(tmpData,texFilename,tmpPixel.getWidth(),tmpPixel.getHeight(),withColor,resourcePosition))
	{
		XMem::XDELETE_ARRAY(tmpData);
		return XFalse;
	}
	XMem::XDELETE_ARRAY(tmpData);
	return XTrue;
}
XBool XTerrain::init(const float *mapData,const char * texFilename,int w,int h,
		XBool withColor,XResourcePosition resourcePosition)	//ֱ�Ӵ������ж�ȡ����
{
	if(m_isInited) return XTrue;
	if(mapData == NULL ||
		w <= 0 || h <= 0) return XFalse;

	m_width = w;
	m_height = h;
	m_withColor = withColor;
	if(resourcePosition == RESOURCE_SYSTEM_DEFINE) resourcePosition = getDefResPos();
//	//д������ For Test
//	if((fp = fopen("Test.dat","wb")) == NULL) return XFalse;	//�������ļ�ʧ��
//	for(int i = 0;i < h;i += 2)
//	{
//		for(int j = 0;j < w;j += 2)
//		{
//			fwrite(&tempTerrainData[i * h + j],1,sizeof(char),fp);
//		}
//	}
//	fclose(fp);
	m_terrainData = XMem::createArrayMem<float>(m_width * m_height);
	if(m_terrainData == NULL)
	{
		XMem::XDELETE_ARRAY(m_terrainData);
		return XFalse;
	}
	memcpy(m_terrainData,mapData,sizeof(float) * m_width * m_height);
	//������ͼ����
	if(texFilename == NULL) m_withTexture = XFalse;
	else 
	{
		m_withTexture = XTrue;
		if(!m_texture.load(texFilename,resourcePosition)) 
		{
			XMem::XDELETE_ARRAY(m_terrainData);
			return XFalse;
		}
		m_textureName = texFilename;
	}

	m_position.set(0.0f,0.0f,0.0f);
	m_scale.set(1.0f,1.0f,1.0f);
	m_angle.set(0.0f,0.0f,0.0f);
	//���潨��VBO
	m_allFaceSum = (m_width - 1) * (m_height - 1) * 2;
	m_allPointSum = m_width * m_height;
	m_index = XMem::createArrayMem<unsigned int>(m_allFaceSum * 3);
	if(m_index == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(withColor)
	{
		m_c = XMem::createArrayMem<float>(m_allPointSum * 4);
		if(m_c == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_v = XMem::createArrayMem<float>(m_allPointSum * 3);	//����
	m_n = XMem::createArrayMem<float>(m_allPointSum * 3);	//����
	m_faceN = XMem::createArrayMem<XVector3>(m_allFaceSum);	//�淨��
	if(m_n == NULL || m_v == NULL || m_faceN == NULL)// || m_pointN == NULL)
	{
		releaseMemory();
		return XFalse;
	}
	if(m_withTexture)
	{
		m_t = XMem::createArrayMem<float>(m_allPointSum * 2);
		if(m_t == NULL)
		{
			releaseMemory();
			return XFalse;
		}
	}
	m_coordOffsetX = 1.0f / m_width;
	m_coordOffsetY = 1.0f / m_height;
//	int tempIndex = 0;
	int indexT = 0;
	int indexV = 0;
	int indexC = 0;
	float curCoordX = 0.0f;
	float curCoordY = 0.0f;
	int i,j;

	int faceIndex = 0;
	int indexI = 0;
	for(j = 0;j < m_height;++ j,curCoordY += m_coordOffsetY)
	{
		indexT = j * m_width * 2;
		indexV = j * m_width * 3;
		indexC = j * m_width * 4;
		indexI = j * m_width;
		faceIndex = j * (m_width - 1) * 2 * 3;
		for(i = 0,curCoordX = 0.0f;i < m_width;++i)
		{
			if(m_withTexture)
			{
				m_t[indexT] = curCoordX;
				m_t[indexT + 1] = curCoordY;
				indexT += 2;
				curCoordX += m_coordOffsetX;
			}
			m_v[indexV] = i;
			m_v[indexV + 2] = j;
			indexV += 3;
			if(m_withColor)
			{
				m_c[indexC] = 1.0f;
				m_c[indexC + 1] = 1.0f;
				m_c[indexC + 2] = 1.0f;
				m_c[indexC + 3] = m_reflectRate;
				indexC += 4;
			}
		}
		if(j < m_height - 1)
		{
			for(i = 0;i < m_width - 1;++i)
			{
				m_index[faceIndex] = indexI;
				m_index[faceIndex + 1] = indexI + m_width;
				m_index[faceIndex + 2] = indexI + 1;
				m_index[faceIndex + 3] = indexI + 1;
				m_index[faceIndex + 4] = indexI + m_width;
				m_index[faceIndex + 5] = indexI + 1 + m_width;

				faceIndex += 2 * 3;
				++indexI;
			}
		}
	}
	m_isInited = XTrue;
	if(!updateHeight(m_terrainData))
	{
		releaseMemory();
		m_isInited = XFalse;
		return XFalse;
	}

	return XTrue;
}
void XTerrain::draw(XBool withTex)
{
	if(!m_isInited ||
		m_faceSum <= 0) return;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);
	if(m_withTexGen)
	{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(m_position.x,m_position.y,m_position.z);
    glRotatef(m_angle.x,1,0,0);
    glRotatef(m_angle.y,0,1,0);
    glRotatef(m_angle.z,0,0,1);
	glScalef(m_scale.x,m_scale.y,m_scale.z);

	if(gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->m_worldMaterial.usetMaterial();
	if(m_withTexture && withTex)
	{
		glActiveTexture(GL_TEXTURE0);
		XGL::EnableTexture2D();
	//	XGL::DisableBlend();
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);	//������������л��
		XGL::BindTexture2D(m_texture.texture.m_texture);
	}else
	{
	//	XGL::DisableTexture2D();
	//	XGL::DisableBlend();
	}
	if(gFrameworkData.p3DWorld != NULL)
		gFrameworkData.p3DWorld->useShadow(m_withTexture && withTex,SHADER_SHADOW);
	if(m_withTexGen)
	{
		XGL::EnableBlend();
		XGL::SetBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
		glColor4f(1.0f,1.0f,1.0f,m_reflectRate);
	}
	if(m_pShader != NULL) m_pShader->useShader();

	m_vbo.drawByIndex(GL_TRIANGLES,m_withTexture && withTex);

	if(m_pShader != NULL) m_pShader->disShader();
	glPopMatrix();
	if(m_withTexGen)
	{
		XGL::DisableBlend();
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
	glPopAttrib();
//	XGL::EnableBlend();
//	XGL::DisableTexture2D();
}
XBool XTerrain::updateVBO(bool v,bool n,bool c,bool t,bool i)
{
	if(!m_isInited) return XFalse;

	int sum;
	sum = m_allPointSum;
	if(v) m_vbo.updateDataV(sum,m_v);
	if(n) m_vbo.updateDataN(sum,m_n);
	if(c) m_vbo.updateDataC(sum,m_c);
	if(t) m_vbo.updateDataT(sum,m_t);
	if(i) m_vbo.updateDataI(sum * 3,m_index);

	return XTrue;
}
XBool XTerrain::updateByHeightAndNormal(const float *heights,const float *normal)
{
	if(!m_isInited ||
		heights == NULL || normal == NULL) return XFalse;
	for(int i = 0,j = 1;i < m_allPointSum;++ i,j += 3)
	{
		m_v[j] = heights[i];
	}

	memcpy(m_n,normal,m_height * m_width * 3 * sizeof(float));
	m_faceSum = m_allFaceSum;
	//m_newH = heights;
	updateVBO(true,true);//�������ƣ����Կ������Σ���3D��ˮ��Ŀ��,���Ĵ�
	//memcpy(m_terrainData,heights,sizeof(float) * m_width * m_height);
	return XTrue;
}
XBool XTerrain::updateByFlag(const bool *flags,const float * w)
{
	if(!m_isInited ||
		flags ==NULL ||
		w == NULL) return XFalse;
	int cIndex = 0;
	int j,i;
	int hIndex,index;
	m_faceSum = 0;
	for(j = 0,index = 0;j < m_height - 1;++ j)
	{
		hIndex = j * m_width;
		cIndex = (hIndex << 2) + 3;
		for(i = 0;i < m_width - 1;++ i,++ hIndex,cIndex += 4)
		{
			if(flags[hIndex + 1] || flags[hIndex + m_width])
			{//�ϰ����
				m_index[index] = hIndex; 
				m_index[index + 1] = hIndex + 1; 
				m_index[index + 2] = hIndex + m_width; 
				m_index[index + 3] = hIndex + 1; 
				m_index[index + 4] = hIndex + m_width; 
				m_index[index + 5] = hIndex + 1 + m_width;
				if(flags[hIndex]) m_c[cIndex] = w[hIndex];//m_reflectRate * w[hIndex];
				else m_c[cIndex] = 0.0f;
				if(flags[hIndex + 1]) m_c[cIndex + 4] = w[hIndex + 1];//m_reflectRate * w[hIndex + 1];
				else m_c[cIndex + 4] = 0.0f;
				if(flags[hIndex + m_width]) m_c[cIndex + (m_width << 2)] = w[hIndex + m_width];//m_reflectRate * w[hIndex + m_width];
				else m_c[cIndex + (m_width << 2)] = 0.0f;
				if(flags[hIndex + 1 + m_width]) m_c[cIndex + 4 + (m_width << 2)] = w[hIndex + 1 + m_width];//m_reflectRate * w[hIndex + 1 + m_width];
				else m_c[cIndex + 4 + (m_width << 2)] = 0.0f;
				m_faceSum += 2;
				index += 6;
			}else
			if(flags[hIndex])
			{
				m_index[index] = hIndex; 
				m_index[index + 1] = hIndex + 1; 
				m_index[index + 2] = hIndex + m_width;
				m_c[cIndex] = w[hIndex];//m_reflectRate * w[hIndex];
				m_c[cIndex + 4] = 0.0f;
				m_c[cIndex + (m_width << 2)] = 0.0f;
				++ m_faceSum;
				index += 3;
			}else
			if(flags[hIndex + m_width + 1])
			{//�°����
				m_index[index] = hIndex + 1; 
				m_index[index + 1] = hIndex + m_width; 
				m_index[index + 2] = hIndex + 1 + m_width; 
				m_c[cIndex + 4] = 0.0f;
				m_c[cIndex + (m_width << 2)] = 0.0f;
				m_c[cIndex + 4 + (m_width << 2)] = w[hIndex + 1 + m_width];//m_reflectRate * w[hIndex + 1 + m_width];
				++ m_faceSum;
				index += 3;
			}
		}
	}
	m_vbo.updateDataC(m_allPointSum,m_c);

	return XTrue;
}
XBool XTerrain::updateByStep(int step)
{
	if(!m_isInited ||
		step <= 0) return XFalse;
	int i,j,index,hIndex;
	m_faceSum = 0;
	int w = step * m_width;
	for(j = 0,index = 0;j < m_height - step;j += step)
	{
		hIndex = j * m_width;
		for(i = 0;i < m_width - step;i += step,hIndex += step)
		{
			m_index[index] = hIndex; 
			m_index[index + 1] = hIndex + step; 
			m_index[index + 2] = hIndex + w; 
			m_index[index + 3] = hIndex + step; 
			m_index[index + 4] = hIndex + w; 
			m_index[index + 5] = hIndex + step + w;
			m_faceSum += 2;
			index += 6;
		}
	}
	m_vbo.updateDataI(m_faceSum * 3,m_index);
	return XTrue;
}
XBool XTerrain::updateHeight(const float *heights,const bool *flags)
{//���ݸ߶���Ϣ��������
	if(!m_isInited) return XFalse;
	//�������ʵ�ʵ���ֵ�������
	//XVector3 tempNormal;
	int i,j;
	if(flags == NULL)
	{
		if(m_terrainData == heights)
		{
			float const *pH = heights;
			float *pV = m_v + 1;
			for(j = 0;j < m_allPointSum;++ j)
			{
				*pV = *(pH ++);
				pV += 3;
			}
			//��������淨��
			for(i = 0,j = 0;i < m_allFaceSum;++ i,j += 3)
			{
				m_faceN[i] = XMath::getNormal(m_v + m_index[j] * 3,
					m_v + m_index[j + 1] * 3,
					m_v + m_index[j + 2] * 3);
			}
			m_faceSum = m_allFaceSum;
			updateNormal(heights);//���￼�ǶԷ������ݽ���ƽ������
			//updateVBO(true,true,true,true);
			
			if(m_withTexture) m_vbo.updateDate(m_allPointSum,m_v,m_t,m_n,m_c,m_faceSum * 3,m_index);
			else m_vbo.updateDate(m_allPointSum,m_v,NULL,m_n,m_c,m_faceSum * 3,m_index);
		}else
		{
			float const *pH = heights;
			float *pV = m_v + 1;
			for(j = 0;j < m_allPointSum;++ j)
			{
				*pV = *(pH ++);
				pV += 3;
			}
			m_newH = heights;
			int index = 0;
			int lIndex = 0;
			for(j = 0;j < m_height - 1;++ j)
			{
				lIndex = j * m_width;
				index = (lIndex - j) * 2;
				for(i = 0;i < m_width - 1;++ i,++lIndex,index += 2)
				{
					if(m_terrainData[lIndex + 1] != heights[lIndex + 1]
						|| m_terrainData[lIndex + m_width] != heights[lIndex + m_width])
					{
						m_faceN[index] = getMyNormal(lIndex,true);
						m_faceN[index + 1] = getMyNormal(lIndex,false);
					}else
					if(m_terrainData[lIndex] != heights[lIndex])
					{
						m_faceN[index] = getMyNormal(lIndex,true);
					}else
					if(m_terrainData[lIndex + m_width + 1] != heights[lIndex + m_width + 1])
					{
						m_faceN[index] = getMyNormal(lIndex,false);
					}
				}
			}

			m_faceSum = m_allFaceSum;
			updateNormal(heights);//���￼�ǶԷ������ݽ���ƽ������,���Ĵ�
			updateVBO(true,true);	//�������ƣ����Կ������Σ���3D��ˮ��Ŀ��,���Ĵ�
		}
		memcpy(m_terrainData,heights,sizeof(float) * m_allPointSum);	//Ŀǰ������ݿ��Բ��ø���
	}else
	{
		//m_faceSum = 0;
//		int tIndex = 0;
		if(m_withColor)
		{
			float const *pH = heights;
			float *pV = m_v + 1;
			bool const *pf = flags; 
			float *pC = m_c + 3;
			for(j = 0;j < m_allPointSum;++ j)
			{
				*pV = *(pH ++);
				if(*(pf ++)) *pC = m_reflectRate;
				else *pC = 0;
				pC += 4;
				pV += 3;
			}
		}else
		{
			float const *pH = heights;
			float *pV = m_v + 1;
			for(j = 0;j < m_allPointSum;++ j)
			{
				*pV = *(pH ++);
				pV += 3;
			}		
		}
		//��������淨��
		unsigned int p0,p1,p2;
		unsigned int *pI = m_index;
		for(i = 0;i < m_allFaceSum;++ i)
		{
			p0 = *(pI ++);p1 = *(pI ++);p2 = *(pI ++);	//�����ֵ�����˱仯
			if((flags[p0] || flags[p1] || flags[p2])	//��Ҫ�ı䲢����ֵ�����仯
				&& (m_terrainData[p0] != heights[p0] ||
				m_terrainData[p1] != heights[p1] ||
				m_terrainData[p2] != heights[p2]))
			{
				m_faceN[i] = XMath::getNormal(m_v + p0 * 3,m_v + p1 * 3,m_v + p2 * 3);
				//m_faceN[i].set(0,1,0);
			}
		}
		//for(j = 0,index = 0;j < m_height - 1;++ j)
		//{
		//	hIndex = j * m_width;
		//	for(i = 0;i < m_width - 1;++ i,++ hIndex)
		//	{
		//		if(flags[hIndex] || flags[hIndex + 1] || flags[hIndex + m_width])
		//		{//�ϰ����
		//			m_index[index] = hIndex; 
		//			m_index[index + 1] = hIndex + 1; 
		//			m_index[index + 2] = hIndex + m_width; 

		//			++ m_faceSum;
		//			index += 3;
		//		}
		//		if(flags[hIndex + 1] || flags[hIndex + m_width] || flags[hIndex + m_width + 1])
		//		{//�°����
		//			m_index[index] = hIndex + 1; 
		//			m_index[index + 1] = hIndex + m_width; 
		//			m_index[index + 2] = hIndex + 1 + m_width; 

		//			++ m_faceSum;
		//			index += 3;
		//		}
		//	}
		//}
		updateNormal(heights);
		if(m_withColor) updateVBO(true,true,true);
		else updateVBO(true,true);
		
		memcpy(m_terrainData,heights,sizeof(float) * m_width * m_height);	//Ŀǰ������ݿ��Բ��ø���
	}
	return XTrue;
}
void XTerrain::updateNormal(const float *heights)
{
	int faceSumW = (m_width - 1) * 2;
	XVector3 tempPointN;
	int upIndex = 0;
	int index = 0,i,j;
	int lIndex = 0;
	float *pN = m_n;
	if(heights == m_terrainData)
	{
	/*	//����1:(δ�ϸ�����)
		//�ĸ���
			//�� 1	����Ҫ����
		m_n[0] = m_faceN[0].x;
		m_n[1] = m_faceN[0].y;
		m_n[2] = m_faceN[0].z;
			//�� 2
		index = (m_width - 1) * 3;
		upIndex = faceSumW;
		tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex - 2]) * 0.5f;
		m_n[index] = tempPointN.x;
		m_n[index + 1] = tempPointN.y;
		m_n[index + 2] = tempPointN.z;
			//��3
		index = (m_height - 1) * m_width * 3;
		upIndex = (m_height - 2) * faceSumW;
		tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5f;
		m_n[index] = tempPointN.x;
		m_n[index + 1] = tempPointN.y;
		m_n[index + 2] = tempPointN.z;
			//��4
		index = (m_height * m_width - 1) * 3;
		upIndex = (m_height - 1) * faceSumW;
		tempPointN = m_faceN[upIndex - 1];
		m_n[index] = tempPointN.x;
		m_n[index + 1] = tempPointN.y;
		m_n[index + 2] = tempPointN.z;
		//������
		for(int i = 1;i < m_width - 1;++ i)
		{
			index = i * 3;
			upIndex = i * 2;
			tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex - 2]) * 0.33333333333f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;

			index = (i + (m_height - 1) * m_width) * 3;
			upIndex = (i + (m_height - 2) * (m_width - 1)) * 2;
			tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[upIndex + 2]) * 0.33333333333f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;
		}
		for(int i = 1;i < m_height - 1;++ i)
		{
			index = i * m_width * 3;
			upIndex = i * faceSumW;
			tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - faceSumW] + m_faceN[upIndex - faceSumW + 1]) * 0.33333333333f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;

			index = ((i + 1) * m_width - 1) * 3;
			upIndex = (i + 1) * faceSumW;
			tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex - 2] + m_faceN[upIndex - faceSumW - 1]) * 0.33333333333f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;
		}
		//����
		for(int i = 1;i < m_height - 1;++ i)
		{
			index = (i * m_width + 1) * 3;
			upIndex = i * faceSumW + 2;
			for(int j = 1;j < m_width - 1;++ j,index += 3,upIndex += 2,lIndex += 2)
			{
				tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex - 2] +
					m_faceN[upIndex - faceSumW + 1] + m_faceN[upIndex - faceSumW] + m_faceN[upIndex - faceSumW - 1]) * 0.16666666666f;
				m_n[index] = tempPointN.x;
				m_n[index + 1] = tempPointN.y;
				m_n[index + 2] = tempPointN.z;
			}
		}*/
		//����2��
		for(j = 0;j < m_height;++ j)
		{
			index = (j * m_width - j) << 1;
			upIndex = index - faceSumW;
			for(i = 0;i < m_width;++ i,index += 2,upIndex += 2)
			{
				if(i == 0)
				{
					if(j == 0)
					{//�� 1	����Ҫ����
						tempPointN = m_faceN[index];
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 2
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[index]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}else
				if(i == m_width - 1)
				{
					if(j == 0)
					{//�� 2
						tempPointN = (m_faceN[index - 1] + m_faceN[index - 2]) * 0.5f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 1
						tempPointN = m_faceN[upIndex - 1];
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 3
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}else
				{
					if(j == 0)
					{//�� 3
						tempPointN = (m_faceN[index] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex + 1]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 6
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex] + m_faceN[upIndex + 1] +
							m_faceN[index - 2] + m_faceN[index - 1] + m_faceN[index]) * 0.16666666666f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}
			}
		}
	}else
	{
/*		//����1:(δ�ϸ�����)
		//�ĸ���
			//�� 1	����Ҫ����
		if(heights[0] != m_terrainData[0])
		{
			m_n[0] = m_faceN[0].x;
			m_n[1] = m_faceN[0].y;
			m_n[2] = m_faceN[0].z;
		}
			//�� 2
		index = (m_width - 1) * 3;
		upIndex = faceSumW;
		if(heights[m_width - 1] != m_terrainData[m_width - 1])
		{
			tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex - 2]) * 0.5f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;
		}
			//��3
		index = (m_height - 1) * m_width * 3;
		upIndex = (m_height - 2) * faceSumW;
		if(heights[(m_height - 1) * m_width] != m_terrainData[(m_height - 1) * m_width])
		{
			tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5f;
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;
		}
			//��4
		index = (m_height * m_width - 1) * 3;
		upIndex = (m_height - 1) * faceSumW;
		if(heights[m_height * m_width - 1] != m_terrainData[m_height * m_width - 1])
		{
			tempPointN = m_faceN[upIndex - 1];
			m_n[index] = tempPointN.x;
			m_n[index + 1] = tempPointN.y;
			m_n[index + 2] = tempPointN.z;
		}
		//������
		for(int i = 1;i < m_width - 1;++ i)
		{
			index = i * 3;
			if(heights[i] != m_terrainData[i])
			{
				upIndex = i * 2;
				tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex - 2]) * 0.33333333333f;
				m_n[index] = tempPointN.x;
				m_n[index + 1] = tempPointN.y;
				m_n[index + 2] = tempPointN.z;
			}

			index = (i + (m_height - 1) * m_width) * 3;
			if(heights[i + (m_height - 1) * m_width] != m_terrainData[i + (m_height - 1) * m_width])
			{
				upIndex = (i + (m_height - 2) * (m_width - 1)) * 2;
				tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[upIndex + 2]) * 0.33333333333f;
				m_n[index] = tempPointN.x;
				m_n[index + 1] = tempPointN.y;
				m_n[index + 2] = tempPointN.z;
			}
		}
		for(int i = 1;i < m_height - 1;++ i)
		{
			index = i * m_width * 3;
			if(heights[i * m_width] != m_terrainData[i * m_width])
			{
				upIndex = i * faceSumW;
				tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - faceSumW] + m_faceN[upIndex - faceSumW + 1]) * 0.33333333333f;
				m_n[index] = tempPointN.x;
				m_n[index + 1] = tempPointN.y;
				m_n[index + 2] = tempPointN.z;
			}

			index = ((i + 1) * m_width - 1) * 3;
			if(heights[(i + 1) * m_width - 1] != m_terrainData[(i + 1) * m_width - 1])
			{
				upIndex = (i + 1) * faceSumW;
				tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex - 2] + m_faceN[upIndex - faceSumW - 1]) * 0.33333333333f;
				m_n[index] = tempPointN.x;
				m_n[index + 1] = tempPointN.y;
				m_n[index + 2] = tempPointN.z;
			}
		}
		//����
		for(int i = 1;i < m_height - 1;++ i)
		{
			lIndex = i * m_width + 1;
			index = lIndex * 3;
			upIndex = i * faceSumW + 2;
			for(int j = 1;j < m_width - 1;++ j,index += 3,upIndex += 2,lIndex += 2,lIndex += 1)
			{
				if(heights[lIndex] != m_terrainData[lIndex])
				{
					tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex - 2] +
						m_faceN[upIndex - faceSumW + 1] + m_faceN[upIndex - faceSumW] + m_faceN[upIndex - faceSumW - 1]) * 0.16666666666f;
					m_n[index] = tempPointN.x;
					m_n[index + 1] = tempPointN.y;
					m_n[index + 2] = tempPointN.z;
				}
			}
		}*/
		//����2
		for(j = 0;j < m_height;++ j)
		{
			lIndex = j * m_width;
			index = (lIndex - j) << 1;
			upIndex = index - faceSumW;
			for(i = 0;i < m_width;++ i,++lIndex,index += 2,upIndex += 2)
			{
				if(heights[lIndex] == m_terrainData[lIndex])
				{
					pN += 3;
					continue;	//����û�з����仯��ֱ�ӷ���
				}
				if(i == 0)
				{
					if(j == 0)
					{//�� 1	����Ҫ����
						tempPointN = m_faceN[index];
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 2
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[index]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}else
				if(i == m_width - 1)
				{
					if(j == 0)
					{//�� 2
						tempPointN = (m_faceN[index - 1] + m_faceN[index - 2]) * 0.5f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 1
						tempPointN = m_faceN[upIndex - 1];
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 3
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}else
				{
					if(j == 0)
					{//�� 3
						tempPointN = (m_faceN[index] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					if(j == m_height - 1)
					{//�� 3
						tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex + 1]) * 0.33333333333f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}else
					{//�� 6
						tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex] + m_faceN[upIndex + 1] +
							m_faceN[index - 2] + m_faceN[index - 1] + m_faceN[index]) * 0.16666666666f;
						*(pN ++) = tempPointN.x;
						*(pN ++) = tempPointN.y;
						*(pN ++) = tempPointN.z;
					}
				}
			}
		}
	}
}
bool XTerrain::getMousePoint(XVector2 &outPoint)
{
	if(!m_isInited) return false;
	if(gFrameworkData.p3DWorld == NULL) return false;
	//�������еĵ�
	float len = 10.0f;
	float tempLen;
	XVector3 tempV3;
	bool isTrue = false;
	int index = 0;
	for(int i = 0;i < m_height;i += 2)	//���ｵ�;��ȼ��ټ�����
	{//����û�п��ǵ��ڵ�����
		index = i * m_width;
		for(int j = 0;j < m_width;j += 2) 	//���ｵ�;��ȼ��ټ�����
		{
			tempV3.set(j,m_terrainData[index + j],i);
			
			tempV3 = gFrameworkData.p3DWorld->worldToScreen(tempV3,XRect(0.0f,0.0f,getWindowWidth(),getWindowHeight()));
			if(tempV3.z < 0.0f) continue;	//���������ĵ㲻��
			tempLen = getMousePos().getLengthSqure(tempV3.x,tempV3.y);
			if(tempLen < len)
			{
				len = tempLen;
				outPoint.set(j,i);
				isTrue = true;
			}
		}
	}
	return isTrue;
}
bool XTerrain::getMousePointEx(const XVector3 &n,const XVector3 &f,XVector2 &outPoint)
{
	if(!m_isInited) return false;
	XVector3 out,p[3];//n;
	float tempD = -1.0f;
	float ret = 0.0f;
	//int index;
	//XVector3 tempN(0.0f,1.0f,0.0f);
	//����������е���
	//int pIndex;
	unsigned int *pIndex = m_index;
	for(int i = 0;i < m_allFaceSum;++ i,pIndex += 3)
	{
		p[0] = m_v + (*pIndex) * 3;
		p[1] = m_v + (*(pIndex + 1)) * 3;
		p[2] = m_v + (*(pIndex + 2)) * 3;
		//tempN = getNormal(p[0],p[1],p[2]);
		ret = XMath::canPickingByRay(n,f,p,m_faceN[i],out);
		if(ret >= 0.0f && (ret < tempD || tempD < 0.0f))
		{//��������ĵ�
			tempD = ret;
			outPoint.set(out.x,out.z);
		}
	}

	return (tempD >= 0.0f);
}
void XTerrain::drawNormal(int step)
{
	if(!m_isInited) return;
	XVector3 ps;

	for(int i = 0,index = 0;i < m_allFaceSum;i += step,index += step * 3)
	{
		ps.set(m_v[m_index[index + 0] * 3 + 0],m_v[m_index[index + 0] * 3 + 1],m_v[m_index[index + 0] * 3 + 2]);
		ps += XVector3(m_v[m_index[index + 1] * 3 + 0],m_v[m_index[index + 1] * 3 + 1],m_v[m_index[index + 1] * 3 + 2]);
		ps += XVector3(m_v[m_index[index + 2] * 3 + 0],m_v[m_index[index + 2] * 3 + 1],m_v[m_index[index + 2] * 3 + 2]);
		ps *= 0.333333f;
		XRender::drawLine(ps,m_faceN[i],20.0f);
	}
}
namespace X3D
{
	void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY)//������Ӧ,������ߴ�ĵ��ΰ���3�α�������ֵ����Ҫ�ĵ���
	{
		if(inX < 2 || inY < 2) return;	//��С�Ĳ�ֵ
		float *tempData = XMem::createArrayMem<float>(outX * inY);	//�м����
		if(tempData == NULL) return;
		XBezierInterpolation BZInter;	//���ڼ��㱴������ֵ
		//ˮƽ��������ֵ
		float p0,p1,p2,p3;
		float tWF,tHF;
		int tW,tH;
		int tempIndex;
		float v0,v1,v2;
		for(int h = 0;h < inY;++ h)
		{
			tempIndex = h * inX;
			for(int w = 0;w < outX;++ w)
			{//���������������
				tWF = (float)w / (float)outX * (inX - 1.0f);	//
				tW = tWF;
				if(tW == inX - 1)
				{//���һ����֮��Ĳ�ֵ��ֱ�������һ�������ֵ
					tempData[w + h * outX] = inData[tW + tempIndex];
				}else
				{
					p0 = inData[tW + tempIndex];
					p3 = inData[tW + 1 + tempIndex];
					v1 = (p0 + p3) * 0.5f;
					if(tW == 0) v0 = p0 - (v1 - p0);
					else v0 = (p0 + inData[tW - 1 + tempIndex]) * 0.5f;
					if(tW == inX - 2) v2 = p3 + p3 - v1;
					else v2 = (p3 + inData[tW + 2 + tempIndex]) * 0.5f;
					p1 = (v1 - v0) * 0.5f + p0;
					p2 = p3 - (v2 - v1) * 0.5f;
					BZInter.init(p0,p1,p2,p3);
					tempData[w + h * outX] = BZInter.getBezierSplineValue(tWF - tW);
				}
			}
		}
		//��ֱ��������ֵ
		for(int w = 0;w < outX;++ w)
		{
			for(int h = 0;h < outY;++ h)
			{
				tHF = (float)h / (float)outY * (inY - 1.0f);
				tH = tHF;
				if(tH == inY - 1)
				{
					outData[w + h * outX] = tempData[w + tH * outX];
				}else
				{
					p0 = tempData[w + tH * outX];
					p3 = tempData[w + (tH + 1) * outX];
					v1 = (p0 + p3) * 0.5f;
					if(tH == 0) v0 = p0 - (v1 - p0);
					else v0 = (p0 + tempData[w + (tH - 1) * outX]) * 0.5f;
					if(tH == inY - 2) v2 = p3 + p3 - v1;
					else v2 = (p3 + tempData[w + (tH + 2) * outX]) * 0.5f;
					p1 = (v1 - v0) * 0.5f + p0;
					p2 = p3 - (v2 - v1) * 0.5f;

				//	if(tH == 0) p1 = (p0 + p3) * 0.5f;
				//	else p1 = p0 + (p0 - tempData[w + (tH - 1) * outX]) * 0.5f;
				//	if(tH == inY - 2) p2 = (p0 + p3) * 0.5f;
				//	else p2 = p3 - (tempData[w + (tH + 2) * outX] - p3) * 0.5f;
					BZInter.init(p0,p1,p2,p3);
					outData[w + h * outX] = BZInter.getBezierSplineValue(tHF - tH);
				}
			}
		}
		//XMem::XDELETE_ARRAY(tempData);
	}
}
void XTerrain::myFitTerrain(const float *inData,int inX,int inY,float *outData)
{
	if(inX < 2 || inY < 2) return;	//��С�Ĳ�ֵ
	//3�α�������ֵ
//	int outX = m_width;
//	int outY = m_height;
//	if(m_tempFitTerrain == NULL) 
//	{//��һ�ν���
//		m_tempFitTerrain = XMem::createArrayMem<float>(outX * inY);
//		if(m_tempFitTerrain == NULL) return;
//		m_oInX = inX;
//		m_oInY = inY;
//	}else
//	if(m_oInY != inY)
//	{//�������ݷ����仯
//		XMem::XDELETE_ARRAY(m_tempFitTerrain);
//		m_tempFitTerrain = XMem::createArrayMem<float>(outX * inY);
//		if(m_tempFitTerrain == NULL) return;
//		m_oInY = inY;
//	}
//	XBezierInterpolation BZInter;	//���ڼ��㱴������ֵ
//	//ˮƽ��������ֵ
//	float p0,p1,p2,p3;
//	float tWF,tHF;
//	int tW,tH;
//	int tempIndex;
//	float v0,v1,v2;
//	for(int h = 0;h < inY;++ h)
//	{
//		tempIndex = h * inX;
//		for(int w = 0;w < outX;++ w)
//		{//���������������
//			tWF = (float)w / (float)outX * (inX - 1.0f);	//
//			tW = tWF;
//			if(tW == inX - 1)
//			{//���һ����֮��Ĳ�ֵ��ֱ�������һ�������ֵ
//				m_tempFitTerrain[w + h * outX] = inData[tW + tempIndex];
//			}else
//			{
//				p0 = inData[tW + tempIndex];
//				p3 = inData[tW + 1 + tempIndex];
//				v1 = (p0 + p3) * 0.5f;
//				if(tW == 0) v0 = p0 - (v1 - p0);
//				else v0 = (p0 + inData[tW - 1 + tempIndex]) * 0.5f;
//				if(tW == inX - 2) v2 = p3 + p3 - v1;
//				else v2 = (p3 + inData[tW + 2 + tempIndex]) * 0.5f;
//				p1 = (v1 - v0) * 0.5f + p0;
//				p2 = p3 - (v2 - v1) * 0.5f;
//				BZInter.init(p0,p1,p2,p3);
//				m_tempFitTerrain[w + h * outX] = BZInter.getBezierSplineValue(tWF - tW);
//			}
//		}
//	}
//	//��ֱ��������ֵ
//	for(int w = 0;w < outX;++ w)
//	{
//		for(int h = 0;h < outY;++ h)
//		{
//			tHF = (float)h / (float)outY * (inY - 1.0f);
//			tH = tHF;
//			if(tH == inY - 1)
//			{
//				outData[w + h * outX] = m_tempFitTerrain[w + tH * outX];
//			}else
//			{
//				p0 = m_tempFitTerrain[w + tH * outX];
//				p3 = m_tempFitTerrain[w + (tH + 1) * outX];
//				v1 = (p0 + p3) * 0.5f;
//				if(tH == 0) v0 = p0 - (v1 - p0);
//				else v0 = (p0 + m_tempFitTerrain[w + (tH - 1) * outX]) * 0.5f;
//				if(tH == inY - 2) v2 = p3 + p3 - v1;
//				else v2 = (p3 + m_tempFitTerrain[w + (tH + 2) * outX]) * 0.5f;
//				p1 = (v1 - v0) * 0.5f + p0;
//				p2 = p3 - (v2 - v1) * 0.5f;
//
//				BZInter.init(p0,p1,p2,p3);
//				outData[w + h * outX] = BZInter.getBezierSplineValue(tHF - tH);
//			}
//		}
//	}
	//˫���Բ�ֵ
	float zoom = (float)m_width / (float)inX;
	float tempSize = 1.0f / zoom;
	float xx,yy;
	int x,y,indexS,indexD,indexTS,indexTD;
	//���������Ҫ�����ݽ��д���
	for(int h = 0;h < m_height;++ h)
	{
		yy = h * tempSize;
		y = yy;
		yy = yy - y;
		indexTD = h * m_width;
		indexTS = y * inX;
		if(y < inY - 1)
		{//���һ��
			for(int w = 0;w < m_width;++ w)
			{
				xx = w * tempSize;
				x = xx;			//��������
				xx = xx - x;	//С������
				
				indexD = w + indexTD;
				indexS = x + indexTS;
				if(x < inX - 1)
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS + inX]
						+ xx * (1.0f - yy) * inData[indexS + 1] + xx * yy * inData[indexS + inX + 1];
				}else
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS + inX]
						+ xx * (1.0f - yy) * inData[indexS - 1] + xx * yy * inData[indexS + inX - 1];
				}
			}
		}else
		{
			for(int w = 0;w < m_width;++ w)
			{
				xx = w * tempSize;
				x = xx;		//��������
				xx = xx - x;	//С������
				
				indexD = w + indexTD;
				indexS = x + indexTS;
				if(x < inX - 1)
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS - inX]
						+ xx * (1.0f - yy) * inData[indexS + 1] + xx * yy * inData[indexS - inX + 1];
				}else
				{
					outData[indexD] = inData[indexS] * (1.0f - xx) * (1.0f - yy) + (1.0f - xx) * yy * inData[indexS - inX]
						+ xx * (1.0f - yy) * inData[indexS - 1] + xx * yy * inData[indexS - inX - 1];
				}
			}
		}
	}
}
bool XTerrain::setSubject(XTerrain *T,bool cv,bool cn,bool cc,bool ct,bool ci)	//����Ϊt�Ĵ���������û�����ԣ�����ȫ����t��
{//����û�н����ϸ������У��(���֮����Ҫ�ϸ����߼���������)
	if(T == NULL ||
		m_pSubject != NULL) return false;
	m_vbo.setAsSubject(T->m_vbo,XTrue,XTrue,m_withColor,XTrue,XTrue,cv,cn,cc,ct,ci);

	m_subjectV = cv;
	m_subjectN = cn;
	m_subjectC = cc;
	m_subjectT = ct;
	m_subjectI = ci;
	if(m_subjectV)
	{
		XMem::XDELETE_ARRAY(m_v);
		m_v = T->m_v;
	}
	if(m_subjectN)
	{
		XMem::XDELETE_ARRAY(m_n);
		m_n = T->m_n;
	}
	if(m_subjectC)
	{
		XMem::XDELETE_ARRAY(m_c);
		m_c = T->m_c;
	}
	if(m_subjectT)
	{
		XMem::XDELETE_ARRAY(m_t);
		m_t = T->m_t;
	}
	if(m_subjectI)
	{
		XMem::XDELETE_ARRAY(m_index);
		m_index = T->m_index;
	}
	m_pSubject = T;
	return true;
}
void XTerrain::releaseMemory()
{
	if(m_pSubject == NULL)
	{
		XMem::XDELETE_ARRAY(m_v);
		XMem::XDELETE_ARRAY(m_n);
		XMem::XDELETE_ARRAY(m_faceN);
		//XMem::XDELETE_ARRAY(m_pointN);
		XMem::XDELETE_ARRAY(m_t);
		XMem::XDELETE_ARRAY(m_c);
		XMem::XDELETE_ARRAY(m_index);
	}else
	{
		if(!m_subjectV) XMem::XDELETE_ARRAY(m_v);
		if(!m_subjectN) XMem::XDELETE_ARRAY(m_n);
		if(!m_subjectC) XMem::XDELETE_ARRAY(m_c);
		if(!m_subjectT) XMem::XDELETE_ARRAY(m_t);
		if(!m_subjectI) XMem::XDELETE_ARRAY(m_index);
		m_pSubject = NULL;
	}
}
bool XTerrain::saveMtl(const char * filename)
{
	if(filename == NULL || !m_isInited) return false;
	//����д���ļ�����
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return XFalse;	//�ļ���ʧ��
	fprintf(fp,"# xiajia_1981@163.com\n");
	fprintf(fp,"newmtl XTerrain_mtl\n");
	fprintf(fp,"illum 2\n");
	//����д����ʵ������Ϣ(��δ���)
	fprintf(fp,"Ka 0.588000 0.588000 0.588000\n");
	fprintf(fp,"Kd 0.588000 0.588000 0.588000\n");
	fprintf(fp,"Ks 0.000000 0.000000 0.000000\n");
	fprintf(fp,"Ns 10.000000\n");
	fprintf(fp,"map_Kd %s\n",m_textureName.c_str());
	fclose(fp);
	return true;
}
bool XTerrain::saveToObj(const char * filename)	//�����ݱ���Ϊobj��ʽ��ģ���ļ�(��δ��������)
{
	if(filename == NULL || !m_isInited) return false;
	//����д���ļ�����
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return XFalse;	//�ļ���ʧ��
	fprintf(fp,"# xiajia_1981@163.com\n");
	if(m_withTexture)
	{//����ͼ
		std::string mtlName = filename;
		mtlName[mtlName.length() - 3] = 'm';
		mtlName[mtlName.length() - 2] = 't';
		mtlName[mtlName.length() - 1] = 'l';
		fprintf(fp,"mtllib %s\n",mtlName.c_str());
		saveMtl(mtlName.c_str());
	}
	//����д�붥������
	int index = 0;
	for(int h = 0;h < m_height;++ h)
	{
		for(int w = 0;w < m_width;++ w,++index)
		{
			fprintf(fp,"v %f %f %f\n",(float)w,m_terrainData[index],(float)h);
		}
	}
	//����д����ͼ����
	if(m_withTexture)
	{
		for(int h = 0;h < m_height;++ h)
		{
			for(int w = 0;w < m_width;++ w)
			{
				fprintf(fp,"vt %f %f\n",1.0f / m_width * w,1.0f - 1.0f / m_height * h);
			}
		}
	}
	//����д�뷨������
	XVector3 tempPointN;
	for(int j = 0;j < m_height;++ j)
	{
		index = (j * m_width - j) << 1;
		int upIndex = index - (m_width - 1) * 2;;
		for(int i = 0;i < m_width;++ i,index += 2,upIndex += 2)
		{
			if(i == 0)
			{
				if(j == 0) tempPointN = m_faceN[index]; else	//�� 1	����Ҫ����
				if(j == m_height - 1) tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1]) * 0.5; else //�� 2
					tempPointN = (m_faceN[upIndex] + m_faceN[upIndex + 1] + m_faceN[index]) * 0.33333333333f;	//�� 3
			}else
			if(i == m_width - 1)
			{
				if(j == 0) tempPointN = (m_faceN[index - 1] + m_faceN[index - 2]) * 0.5f;else //�� 2
				if(j == m_height - 1) tempPointN = m_faceN[upIndex - 1]; else//�� 1
					tempPointN = (m_faceN[upIndex - 1] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;//�� 3
			}else
			{
				if(j == 0) tempPointN = (m_faceN[index] + m_faceN[index - 1] + m_faceN[index - 2]) * 0.33333333333f;else//�� 3
				if(j == m_height - 1) tempPointN = (m_faceN[upIndex] + m_faceN[upIndex - 1] + m_faceN[upIndex + 1]) * 0.33333333333f;else//�� 3
					tempPointN = (m_faceN[upIndex - 1] + m_faceN[upIndex] + m_faceN[upIndex + 1] +
						m_faceN[index - 2] + m_faceN[index - 1] + m_faceN[index]) * 0.16666666666f;//�� 6
			}
			fprintf(fp,"vn %f %f %f\n",tempPointN.x,tempPointN.y,tempPointN.z);
		}
	}
	//д������
	fprintf(fp,"g XTerrain\n");	
	//д���������
	fprintf(fp,"usemtl XTerrain_mtl\n");
	//����д������Ϣ
	if(m_withTexture)
	{
		index = 0;
		for(int i = 0;i < m_allFaceSum;++ i,index += 3)
		{
			fprintf(fp,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",m_index[index] + 1,m_index[index] + 1,m_index[index] + 1,
					m_index[index + 1] + 1,m_index[index + 1] + 1,m_index[index + 1] + 1,
					m_index[index + 2] + 1,m_index[index + 2] + 1,m_index[index + 2] + 1);
		}
	}else
	{
		index = 0;
		for(int i = 0;i < m_allFaceSum;++ i,index += 3)
		{
			fprintf(fp,"f %d//%d %d//%d %d//%d\n",m_index[index] + 1,m_index[index] + 1,
					m_index[index + 1] + 1,m_index[index + 1] + 1,
					m_index[index + 2] + 1,m_index[index + 2] + 1);
		}
	}
	fclose(fp);
	return true;
}
XBool XTerrainOptimization::init(const char * filename)
{//ֱ�Ӵ��ļ����г�ʼ��
	if(m_isInited) return XFalse;
	XCurPixel tmpPixel;
	if(!tmpPixel.load(filename)) return XFalse;
	int size = tmpPixel.getWidth() * tmpPixel.getHeight();
	if(size <= 0) return XFalse;
	float *tmpData = XMem::createArrayMem<float>(size);
	if(tmpData == NULL) return XFalse;
	unsigned char * p = tmpPixel.getPixel();
	switch(tmpPixel.getColorMode())
	{
	case COLOR_RGBA:
		for(int i = 0;i < size;++ i)
		{
			tmpData[i] = p[i << 2];
		}
		break;
	case COLOR_RGB:
		for(int i = 0;i < size;++ i)
		{
			tmpData[i] = p[i * 3];
		}
		break;
	case COLOR_GRAY:
		for(int i = 0;i < size;++ i)
		{
			tmpData[i] = p[i];
		}
		break;
	default:
		tmpPixel.release();
		XMem::XDELETE_ARRAY(tmpData);
		return XFalse;
		break;
	}
	if(!init(tmpData,tmpPixel.getWidth(),tmpPixel.getHeight()))
	{
		XMem::XDELETE_ARRAY(tmpData);
		return false;
	}
	XMem::XDELETE_ARRAY(tmpData);

	m_isInited = XTrue;
	return XTrue;
}
XBool XTerrainOptimization::init(const float *data,int w,int h)
{//ֱ�Ӵ����ݽ��г�ʼ��
	if(m_isInited) return XFalse;
	m_w = w;
	m_h = h;
	m_dataSize = w * h;
	m_data = XMem::createArrayMem<float>(m_dataSize);
	if(m_data == NULL) return XFalse;
	memcpy(m_data,data,sizeof(float) * m_dataSize);//ֱ�ӿ�������
	//������Ҫ��ʼ������������
	//��ʼ����������
	XTerrainVertexInfo tmpVInfo;
	int index = 0;
	for(int i = 0;i < h;++ i)
	{
		for(int j = 0;j < w;++ j,++ index)
		{
			tmpVInfo.x = j;
			tmpVInfo.y = i;
			tmpVInfo.index = index;
			tmpVInfo.h = m_data[tmpVInfo.index];
			tmpVInfo.faceIndex[0] = -1;
			tmpVInfo.faceIndex[1] = -1;
			tmpVInfo.faceIndex[2] = -1;
			tmpVInfo.faceIndex[3] = -1;
			tmpVInfo.faceIndex[4] = -1;
			tmpVInfo.faceIndex[5] = -1;
			tmpVInfo.faceSum = 0;
			//�����Ȳ���ʼ��������ص�����Ϣ
			m_vertexInfos.push_back(tmpVInfo);
		}
	}
	//�����ʼ������Ϣ
	XTerrainFaceInfo tmpFInfo;
	index = 0;
	for(int i = 0;i < h - 1;++ i)
	{
		for(int j = 0;j < w;++ j,++ index)
		{
			//if(i > 0)
			//{
			//	if(j > 0)
			//	{//0����
			//		tmpFInfo.isEnable = true;
			//		tmpFInfo.index[0] = index - w;
			//		tmpFInfo.index[1] = index - 1;
			//		tmpFInfo.index[2] = index;
			//	//	tmpFInfo.normal = ;	//������Ҫ����
			//		m_faceInfos.push_back(tmpFInfo);
			//		//���¶�����Ϣ
			//		m_vertexInfos[tmpFInfo.index[0]].faceIndex[4] = (int)(m_faceInfos.size()) - 1;
			//		m_vertexInfos[tmpFInfo.index[1]].faceIndex[2] = (int)(m_faceInfos.size()) - 1;
			//		m_vertexInfos[tmpFInfo.index[2]].faceIndex[0] = (int)(m_faceInfos.size()) - 1;
			//	}
			//	//1����
			//	tmpFInfo.isEnable = true;
			//	tmpFInfo.index[0] = index - w;
			//	tmpFInfo.index[1] = index;
			//	tmpFInfo.index[2] = index - w + 1;
			////	tmpFInfo.normal = ;	//������Ҫ����
			//	m_faceInfos.push_back(tmpFInfo);
			//	//���¶�����Ϣ
			//	m_vertexInfos[tmpFInfo.index[0]].faceIndex[5] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[1]].faceIndex[1] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[2]].faceIndex[3] = (int)(m_faceInfos.size()) - 1;
			//	//2����
			//	tmpFInfo.isEnable = true;
			//	tmpFInfo.index[0] = index - w + 1;
			//	tmpFInfo.index[1] = index;
			//	tmpFInfo.index[2] = index + 1;
			////	tmpFInfo.normal = ;	//������Ҫ����
			//	m_faceInfos.push_back(tmpFInfo);
			//	//���¶�����Ϣ
			//	m_vertexInfos[tmpFInfo.index[0]].faceIndex[4] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[1]].faceIndex[2] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[2]].faceIndex[0] = (int)(m_faceInfos.size()) - 1;
			//}
			if(j > 0)
			{
			//	//3����
			//	tmpFInfo.isEnable = true;
			//	tmpFInfo.index[0] = index - 1;
			//	tmpFInfo.index[1] = index + w - 1;
			//	tmpFInfo.index[2] = index;
			////	tmpFInfo.normal = ;	//������Ҫ����
			//	m_faceInfos.push_back(tmpFInfo);
			//	//���¶�����Ϣ
			//	m_vertexInfos[tmpFInfo.index[0]].faceIndex[5] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[1]].faceIndex[1] = (int)(m_faceInfos.size()) - 1;
			//	m_vertexInfos[tmpFInfo.index[2]].faceIndex[3] = (int)(m_faceInfos.size()) - 1;
				//4����
				tmpFInfo.isEnable = true;
				tmpFInfo.index[0] = index;
				tmpFInfo.index[1] = index + w - 1;
				tmpFInfo.index[2] = index + w;
				tmpFInfo.normal = XMath::getNormal(XVector3(m_vertexInfos[tmpFInfo.index[0]].x,m_vertexInfos[tmpFInfo.index[0]].h,m_vertexInfos[tmpFInfo.index[0]].y),
					XVector3(m_vertexInfos[tmpFInfo.index[1]].x,m_vertexInfos[tmpFInfo.index[1]].h,m_vertexInfos[tmpFInfo.index[1]].y),
					XVector3(m_vertexInfos[tmpFInfo.index[2]].x,m_vertexInfos[tmpFInfo.index[2]].h,m_vertexInfos[tmpFInfo.index[2]].y));	//������Ҫ����
				m_faceInfos.push_back(tmpFInfo);
				//���¶�����Ϣ
				m_vertexInfos[tmpFInfo.index[0]].faceIndex[4] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[0]].faceSum;
				m_vertexInfos[tmpFInfo.index[1]].faceIndex[2] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[1]].faceSum;
				m_vertexInfos[tmpFInfo.index[2]].faceIndex[0] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[2]].faceSum;
			}
			if(j < w - 1)
			{
				//5����
				tmpFInfo.isEnable = true;
				tmpFInfo.index[0] = index;
				tmpFInfo.index[1] = index + w;
				tmpFInfo.index[2] = index + 1;
				tmpFInfo.normal = XMath::getNormal(XVector3(m_vertexInfos[tmpFInfo.index[0]].x,m_vertexInfos[tmpFInfo.index[0]].h,m_vertexInfos[tmpFInfo.index[0]].y),
					XVector3(m_vertexInfos[tmpFInfo.index[1]].x,m_vertexInfos[tmpFInfo.index[1]].h,m_vertexInfos[tmpFInfo.index[1]].y),
					XVector3(m_vertexInfos[tmpFInfo.index[2]].x,m_vertexInfos[tmpFInfo.index[2]].h,m_vertexInfos[tmpFInfo.index[2]].y));	//������Ҫ����
				m_faceInfos.push_back(tmpFInfo);
				//���¶�����Ϣ
				m_vertexInfos[tmpFInfo.index[0]].faceIndex[5] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[0]].faceSum;
				m_vertexInfos[tmpFInfo.index[1]].faceIndex[1] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[1]].faceSum;
				m_vertexInfos[tmpFInfo.index[2]].faceIndex[3] = (int)(m_faceInfos.size()) - 1;
				++ m_vertexInfos[tmpFInfo.index[2]].faceSum;
			}
		}
	}

	m_isInited = XTrue;
	return XTrue;
}
XBool XTerrainOptimization::saveToObj(const char * filename,const char *texName)	//����Ϊָ���ļ�����obj�ļ�
{
	if(!m_isInited || filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return XFalse;	//�ļ���ʧ��
	fprintf(fp,"# xiajia_1981@163.com\n");
	//д������ļ���
	std::string mtlName = filename;
	mtlName[mtlName.length() - 3] = 'm';
	mtlName[mtlName.length() - 2] = 't';
	mtlName[mtlName.length() - 1] = 'l';
	fprintf(fp,"mtllib %s\n",mtlName.c_str());
	//����д�붥������
	int tmpSum = 0;
	for(unsigned int i = 0;i < m_vertexInfos.size();++ i)
	{
		if(m_vertexInfos[i].faceSum > 0)
		{
			++ tmpSum;
			m_vertexInfos[i].lastIndex = tmpSum;
			fprintf(fp,"v %f %f %f\n",(float)m_vertexInfos[i].x,m_vertexInfos[i].h,(float)m_vertexInfos[i].y);
		}
	}
	fprintf(fp,"# %d vertexes\n",tmpSum);
	printf("%d vertexes\n",tmpSum);
	//д����ͼ����
	if(texName != NULL)
	{
		for(unsigned int i = 0;i < m_vertexInfos.size();++ i)
		{
			if(m_vertexInfos[i].faceSum > 0)
			{
				fprintf(fp,"vt %f %f\n",1.0f / m_w * m_vertexInfos[i].x,1.0f - 1.0f / m_h * m_vertexInfos[i].y);
			}
		}
	}
	//д�뷨������,���ߵ����ݿ����Ż���������ͬ�ķ��߿���������
	std::vector<XVector3> normals;	//���ߵ��б�
	XVector3 n;
	float sum = 0.0f;
	tmpSum = 0;
	bool flag = true;
	for(unsigned int i = 0;i < m_vertexInfos.size();++ i)
	{
		if(m_vertexInfos[i].faceSum > 0)
		{
			n = XVector3::zero;
			sum = 0.0f;
			for(int j = 0;j < 6;++ j)
			{
				if(m_vertexInfos[i].faceIndex[j] >= 0)
				{
					n += m_faceInfos[m_vertexInfos[i].faceIndex[j]].normal;
					sum += 1.0f;
				}
			}
			n = XMath::normalize(n / sum);
			//��������
			flag = false;
			for(unsigned int j = 0;j < normals.size();++ j)
			{
				if(normals[j].x == n.x &&
					normals[j].y == n.y &&
					normals[j].z == n.z)
				{//��ͬ
					flag = true;
					m_vertexInfos[i].normalIndex = j + 1;
					break;
				}
			}
			if(!flag)
			{
				normals.push_back(n);
				fprintf(fp,"vn %f %f %f\n",n.x,n.y,n.z);
				++ tmpSum;
				m_vertexInfos[i].normalIndex = tmpSum;
			}
		}
	}
	//д������
	fprintf(fp,"g XTerrain\n");	
	//д���������
	fprintf(fp,"usemtl XTerrain_mtl\n");
	//д��������
	tmpSum = 0;	//ͳ���������
	if(texName != NULL)
	{
		for(unsigned int i = 0;i < m_faceInfos.size();++ i)
		{
			if(m_faceInfos[i].isEnable)
			{
				fprintf(fp,"f %d/%d/%d %d/%d/%d %d/%d/%d\n",m_vertexInfos[m_faceInfos[i].index[0]].lastIndex,m_vertexInfos[m_faceInfos[i].index[0]].lastIndex,m_vertexInfos[m_faceInfos[i].index[0]].normalIndex,
					m_vertexInfos[m_faceInfos[i].index[1]].lastIndex,m_vertexInfos[m_faceInfos[i].index[1]].lastIndex,m_vertexInfos[m_faceInfos[i].index[1]].normalIndex,
					m_vertexInfos[m_faceInfos[i].index[2]].lastIndex,m_vertexInfos[m_faceInfos[i].index[2]].lastIndex,m_vertexInfos[m_faceInfos[i].index[2]].normalIndex);
				++ tmpSum;
			}
		}
	}else
	{
		for(unsigned int i = 0;i < m_faceInfos.size();++ i)
		{
			if(m_faceInfos[i].isEnable)
			{
				fprintf(fp,"f %d//%d %d//%d %d//%d\n",m_vertexInfos[m_faceInfos[i].index[0]].lastIndex,m_vertexInfos[m_faceInfos[i].index[0]].normalIndex,
					m_vertexInfos[m_faceInfos[i].index[1]].lastIndex,m_vertexInfos[m_faceInfos[i].index[1]].normalIndex,
					m_vertexInfos[m_faceInfos[i].index[2]].lastIndex,m_vertexInfos[m_faceInfos[i].index[2]].normalIndex);
				++ tmpSum;
			}
		}
	}
	fprintf(fp,"# %d faces\n",tmpSum);
	printf("%d faces\n",tmpSum);

	fclose(fp);
	//����д������ļ�����
	fp = NULL;
	if((fp = fopen(mtlName.c_str(),"w")) == NULL) return XFalse;	//�ļ���ʧ��
	fprintf(fp,"# xiajia_1981@163.com\n");
	fprintf(fp,"newmtl XTerrain_mtl\n");
	fprintf(fp,"illum 2\n");
	//����д����ʵ������Ϣ(��δ���)
	fprintf(fp,"Ka 0.588000 0.588000 0.588000\n");
	fprintf(fp,"Kd 0.588000 0.588000 0.588000\n");
	fprintf(fp,"Ks 0.000000 0.000000 0.000000\n");
	fprintf(fp,"Ns 10.000000\n");
	if(texName != NULL) fprintf(fp,"map_Kd %s\n",texName);
	fclose(fp);

	return XTrue;
}
void XTerrainOptimization::optimization()
{//�Ż�������������Ҫ��һ������
	//�������Ż�(����ʹ���������Ż����յ��Ż�Ч������)
	int tmpIndex[9];	//��������6���������
	XVector3 tmpNormal[8];
/*	int minSize = XEE_Min(m_w,m_h);
	for(int i = 1;i < minSize;i *= 2)
	{
		int tmpW = m_w * i;
		printf("����:%f\n",i / (float)(minSize));
		int j = 0;
		for(int y = i;y < m_h;y += (i << 1))
		{
			j = y * m_w;
			for(int x = i;x < m_w;x += (i << 1),j += (i << 1))
		//	for(unsigned int j = 0;j < m_vertexInfos.size();++ j)
			{//�������еĵ�Ѱ�ҿ����Ż��Ķ���
				if(m_vertexInfos[j].faceSum != 6) continue;	//ֱ�Ӽ���
				if(m_vertexInfos[j].y < i) continue;
				if(m_vertexInfos[j].x < i) continue;
				if(m_vertexInfos[j].y >= m_h - i) continue;
				if(m_vertexInfos[j].x >= m_w - i) continue;
				tmpIndex[0] = j;			tmpIndex[1] = j - tmpW;		tmpIndex[2] = j - tmpW - i;
				tmpIndex[3] = j - i;		tmpIndex[4] = j + tmpW - i;	tmpIndex[5] = j + tmpW;
				tmpIndex[6] = j + tmpW + i;	tmpIndex[7] = j + i;		tmpIndex[8] = j - tmpW + i;
				if(m_vertexInfos[tmpIndex[2]].faceIndex[5] == -1 ||
					m_vertexInfos[tmpIndex[1]].faceIndex[3] == -1 ||
					m_vertexInfos[tmpIndex[1]].faceIndex[4] == -1 ||
					m_vertexInfos[tmpIndex[1]].faceIndex[5] == -1 ||
					m_vertexInfos[tmpIndex[8]].faceIndex[3] == -1 ||
					m_vertexInfos[tmpIndex[8]].faceIndex[4] == -1 ||
					m_vertexInfos[tmpIndex[3]].faceIndex[1] == -1 ||
					m_vertexInfos[tmpIndex[3]].faceIndex[2] == -1 ||
					m_vertexInfos[tmpIndex[3]].faceIndex[5] == -1 ||
					m_vertexInfos[tmpIndex[7]].faceIndex[0] == -1 ||
					m_vertexInfos[tmpIndex[7]].faceIndex[3] == -1 ||
					m_vertexInfos[tmpIndex[7]].faceIndex[4] == -1 ||
					m_vertexInfos[tmpIndex[4]].faceIndex[1] == -1 ||
					m_vertexInfos[tmpIndex[4]].faceIndex[2] == -1 ||
					m_vertexInfos[tmpIndex[5]].faceIndex[0] == -1 ||
					m_vertexInfos[tmpIndex[5]].faceIndex[1] == -1 ||
					m_vertexInfos[tmpIndex[5]].faceIndex[2] == -1 ||
					m_vertexInfos[tmpIndex[6]].faceIndex[0] == -1) continue;
				bool flag = false;
				for(int h = m_vertexInfos[j].y - i + 1;h < m_vertexInfos[j].y + i;++ h)
				{
					for(int w = m_vertexInfos[j].x - i + 1;w < m_vertexInfos[j].x + i;++ w)
					{
						if(w != m_vertexInfos[j].x && h != m_vertexInfos[j].y
							&& m_vertexInfos[w + h * m_w].faceSum > 0)
						{//��Щ����붼��Ч
							flag = true;
							break;
						}
					}
					if(flag) break;
				}
				if(flag) continue;
				//������߶��Ƿ�һ��
				//����1:�߶ȷ���(�˴������Ż�)
				//if((int)(m_vertexInfos[tmpIndex[1]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[2]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[3]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[4]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[5]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[6]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[7]].h) != (int)(m_vertexInfos[tmpIndex[0]].h) ||
				//	(int)(m_vertexInfos[tmpIndex[8]].h) != (int)(m_vertexInfos[tmpIndex[0]].h)) continue; 
				//����2�����߷���(��δʵ��)
				tmpNormal[0] = m_faceInfos[m_vertexInfos[tmpIndex[1]].faceIndex[3]].normal;
				tmpNormal[1] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[0]].normal;
				tmpNormal[2] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[1]].normal;
				tmpNormal[3] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[2]].normal;
				tmpNormal[4] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[3]].normal;
				tmpNormal[5] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[4]].normal;
				tmpNormal[6] = m_faceInfos[m_vertexInfos[tmpIndex[0]].faceIndex[5]].normal;
				tmpNormal[7] = m_faceInfos[m_vertexInfos[tmpIndex[6]].faceIndex[0]].normal;
				if(abs(tmpNormal[0].dot(tmpNormal[1])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[2])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[3])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[4])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[5])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[6])) <= 0.99f ||
					abs(tmpNormal[0].dot(tmpNormal[7])) <= 0.99f) continue;

				//������ǿ����޳���
				m_vertexInfos[j].faceSum = 0;
				//��֮��ص���ȫ�����ó�Ϊ��Ч
				for(int h = 0;h < 6;++ h)
				{
					//if(m_vertexInfos[j].faceIndex[h] < 0) printf("haha\n");
					m_faceInfos[m_vertexInfos[j].faceIndex[h]].isEnable = false;
					m_vertexInfos[j].faceIndex[h] = -1;
				}
				//�޸��ٽӵ����Ϣ
				m_vertexInfos[tmpIndex[1]].faceIndex[3] = -1;
				m_vertexInfos[tmpIndex[1]].faceIndex[4] = -1;
				m_vertexInfos[tmpIndex[1]].faceIndex[5] = -1;
				m_vertexInfos[tmpIndex[1]].faceSum -= 3;
				m_vertexInfos[tmpIndex[5]].faceIndex[0] = -1;
				m_vertexInfos[tmpIndex[5]].faceIndex[1] = -1;
				m_vertexInfos[tmpIndex[5]].faceIndex[2] = -1;
				m_vertexInfos[tmpIndex[5]].faceSum -= 3;
				m_vertexInfos[tmpIndex[3]].faceIndex[1] = -1;
				m_vertexInfos[tmpIndex[3]].faceIndex[2] = -1;
				m_vertexInfos[tmpIndex[3]].faceIndex[5] = -1;
				m_vertexInfos[tmpIndex[3]].faceSum -= 3;
				m_vertexInfos[tmpIndex[7]].faceIndex[0] = -1;
				m_vertexInfos[tmpIndex[7]].faceIndex[3] = -1;
				m_vertexInfos[tmpIndex[7]].faceIndex[4] = -1;
				m_vertexInfos[tmpIndex[7]].faceSum -= 3;
				//�ǵ���޸�
				int tmpIndexT = m_vertexInfos[tmpIndex[2]].faceIndex[5];
				m_faceInfos[tmpIndexT].index[1] = tmpIndex[4];
				m_faceInfos[tmpIndexT].index[2] = tmpIndex[8];
				m_faceInfos[tmpIndexT].normal = getNormal(XVector3(m_vertexInfos[tmpIndex[2]].x,m_vertexInfos[tmpIndex[2]].h,m_vertexInfos[tmpIndex[2]].y),
					XVector3(m_vertexInfos[tmpIndex[4]].x,m_vertexInfos[tmpIndex[4]].h,m_vertexInfos[tmpIndex[4]].y),
					XVector3(m_vertexInfos[tmpIndex[8]].x,m_vertexInfos[tmpIndex[8]].h,m_vertexInfos[tmpIndex[8]].y));
				m_vertexInfos[tmpIndex[4]].faceIndex[1] = tmpIndexT;
				m_vertexInfos[tmpIndex[8]].faceIndex[3] = tmpIndexT;

				tmpIndexT = m_vertexInfos[tmpIndex[6]].faceIndex[0];
				m_faceInfos[tmpIndexT].index[0] = tmpIndex[8];
				m_faceInfos[tmpIndexT].index[1] = tmpIndex[4];
				m_faceInfos[tmpIndexT].normal = getNormal(XVector3(m_vertexInfos[tmpIndex[8]].x,m_vertexInfos[tmpIndex[8]].h,m_vertexInfos[tmpIndex[8]].y),
					XVector3(m_vertexInfos[tmpIndex[4]].x,m_vertexInfos[tmpIndex[4]].h,m_vertexInfos[tmpIndex[4]].y),
					XVector3(m_vertexInfos[tmpIndex[6]].x,m_vertexInfos[tmpIndex[6]].h,m_vertexInfos[tmpIndex[6]].y));
				m_vertexInfos[tmpIndex[4]].faceIndex[2] = tmpIndexT;
				m_vertexInfos[tmpIndex[8]].faceIndex[4] = tmpIndexT;
			}
		}
	}*/
	int operatorSum;
	while(true)
	{
		operatorSum = 0;
		//ˮƽ�������Ż�(��δ����)
		for(unsigned int j = 0;j < m_vertexInfos.size();++ j)
		{
			if(m_vertexInfos[j].faceSum < 3) continue;
			if(m_vertexInfos[j].faceIndex[3] == -1 ||
				m_vertexInfos[j].faceIndex[4] == -1 ||
				m_vertexInfos[j].faceIndex[5] == -1) continue;
			//�ȸ�
			if(m_faceInfos[m_vertexInfos[j].faceIndex[4]].index[2] != 
				m_faceInfos[m_vertexInfos[j].faceIndex[5]].index[1]) continue;
			//�߶�һ��(���յ�����Ӧ������ķ��߽Ƕ�)
			tmpIndex[0] = j;
			tmpIndex[1] = m_faceInfos[m_vertexInfos[j].faceIndex[3]].index[0];
			tmpIndex[2] = m_faceInfos[m_vertexInfos[j].faceIndex[3]].index[1];
			tmpIndex[3] = m_faceInfos[m_vertexInfos[j].faceIndex[4]].index[2];
			tmpIndex[4] = m_faceInfos[m_vertexInfos[tmpIndex[3]].faceIndex[2]].index[2];
			tmpIndex[5] = m_faceInfos[m_vertexInfos[j].faceIndex[5]].index[2];
			//����1:�߶ȷ���
			//if((int)(m_vertexInfos[tmpIndex[1]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[2]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[3]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[4]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[5]].h) != (int)(m_vertexInfos[j].h)) continue;
			//����2�����߷���
			tmpNormal[0] = m_faceInfos[m_vertexInfos[j].faceIndex[3]].normal;
			tmpNormal[1] = m_faceInfos[m_vertexInfos[j].faceIndex[4]].normal;
			tmpNormal[2] = m_faceInfos[m_vertexInfos[j].faceIndex[5]].normal;
			tmpNormal[3] = m_faceInfos[m_vertexInfos[tmpIndex[4]].faceIndex[0]].normal;
			if(abs(tmpNormal[0].dot(tmpNormal[1])) <= 0.99f ||
				abs(tmpNormal[0].dot(tmpNormal[2])) <= 0.99f ||
				abs(tmpNormal[0].dot(tmpNormal[3])) <= 0.99f) continue;
			//���Լ���
			m_faceInfos[m_vertexInfos[j].faceIndex[4]].isEnable = false;
			m_faceInfos[m_vertexInfos[j].faceIndex[5]].isEnable = false;

			m_faceInfos[m_vertexInfos[j].faceIndex[3]].index[2] = tmpIndex[5];	//���¼�����ķ���
			m_faceInfos[m_vertexInfos[j].faceIndex[3]].normal = XMath::getNormal(XVector3(m_vertexInfos[tmpIndex[1]].x,m_vertexInfos[tmpIndex[1]].h,m_vertexInfos[tmpIndex[1]].y),
					XVector3(m_vertexInfos[tmpIndex[2]].x,m_vertexInfos[tmpIndex[2]].h,m_vertexInfos[tmpIndex[2]].y),
					XVector3(m_vertexInfos[tmpIndex[5]].x,m_vertexInfos[tmpIndex[5]].h,m_vertexInfos[tmpIndex[5]].y));
			m_faceInfos[m_vertexInfos[tmpIndex[4]].faceIndex[0]].index[1] = tmpIndex[2];
			m_faceInfos[m_vertexInfos[tmpIndex[4]].faceIndex[0]].normal = XMath::getNormal(XVector3(m_vertexInfos[tmpIndex[5]].x,m_vertexInfos[tmpIndex[5]].h,m_vertexInfos[tmpIndex[5]].y),
					XVector3(m_vertexInfos[tmpIndex[2]].x,m_vertexInfos[tmpIndex[2]].h,m_vertexInfos[tmpIndex[2]].y),
					XVector3(m_vertexInfos[tmpIndex[4]].x,m_vertexInfos[tmpIndex[4]].h,m_vertexInfos[tmpIndex[4]].y));
			m_vertexInfos[tmpIndex[5]].faceIndex[3] = m_vertexInfos[j].faceIndex[3];
			m_vertexInfos[tmpIndex[2]].faceIndex[2] = m_vertexInfos[tmpIndex[3]].faceIndex[2];
			//�޸��ٽӵĽǵ�
			m_vertexInfos[j].faceIndex[3] = -1;
			m_vertexInfos[j].faceIndex[4] = -1;
			m_vertexInfos[j].faceIndex[5] = -1;
			m_vertexInfos[j].faceSum -= 3;
			m_vertexInfos[tmpIndex[3]].faceIndex[0] = -1;
			m_vertexInfos[tmpIndex[3]].faceIndex[1] = -1;
			m_vertexInfos[tmpIndex[3]].faceIndex[2] = -1;
			m_vertexInfos[tmpIndex[3]].faceSum -= 3;
	
			++ operatorSum;
		}
		//��ֱ�������Ż�(��δ����)
		for(unsigned int j = 0;j < m_vertexInfos.size();++ j)
		{
			if(m_vertexInfos[j].faceSum < 3) continue;
			if(m_vertexInfos[j].faceIndex[1] == -1 ||
				m_vertexInfos[j].faceIndex[2] == -1 ||
				m_vertexInfos[j].faceIndex[5] == -1) continue;
			//�ȸ�
			if(m_faceInfos[m_vertexInfos[j].faceIndex[2]].index[2] != 
				m_faceInfos[m_vertexInfos[j].faceIndex[5]].index[2]) continue;
			//�߶�һ��(���յ�����Ӧ������ķ��߽Ƕ�)
			tmpIndex[0] = j;
			tmpIndex[1] = m_faceInfos[m_vertexInfos[j].faceIndex[5]].index[1];
			tmpIndex[2] = m_faceInfos[m_vertexInfos[tmpIndex[1]].faceIndex[2]].index[2];
			tmpIndex[3] = m_faceInfos[m_vertexInfos[j].faceIndex[5]].index[2];
			tmpIndex[4] = m_faceInfos[m_vertexInfos[j].faceIndex[2]].index[0];
			tmpIndex[5] = m_faceInfos[m_vertexInfos[j].faceIndex[1]].index[0];
			//����1:�߶ȷ���
			//if((int)(m_vertexInfos[tmpIndex[1]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[2]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[3]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[4]].h) != (int)(m_vertexInfos[j].h)) continue;
			//if((int)(m_vertexInfos[tmpIndex[5]].h) != (int)(m_vertexInfos[j].h)) continue;
			//����2�����߷���
			tmpNormal[0] = m_faceInfos[m_vertexInfos[j].faceIndex[1]].normal;
			tmpNormal[1] = m_faceInfos[m_vertexInfos[j].faceIndex[2]].normal;
			tmpNormal[2] = m_faceInfos[m_vertexInfos[j].faceIndex[5]].normal;
			tmpNormal[3] = m_faceInfos[m_vertexInfos[tmpIndex[2]].faceIndex[0]].normal;
			if(abs(tmpNormal[0].dot(tmpNormal[1])) <= 0.99f ||
				abs(tmpNormal[0].dot(tmpNormal[2])) <= 0.99f ||
				abs(tmpNormal[0].dot(tmpNormal[3])) <= 0.99f) continue;
			//���Լ���
			m_faceInfos[m_vertexInfos[j].faceIndex[2]].isEnable = false;
			m_faceInfos[m_vertexInfos[j].faceIndex[5]].isEnable = false;

			m_faceInfos[m_vertexInfos[j].faceIndex[1]].index[1] = tmpIndex[1];
			m_faceInfos[m_vertexInfos[j].faceIndex[1]].normal = XMath::getNormal(XVector3(m_vertexInfos[tmpIndex[5]].x,m_vertexInfos[tmpIndex[5]].h,m_vertexInfos[tmpIndex[5]].y),
					XVector3(m_vertexInfos[tmpIndex[1]].x,m_vertexInfos[tmpIndex[1]].h,m_vertexInfos[tmpIndex[1]].y),
					XVector3(m_vertexInfos[tmpIndex[4]].x,m_vertexInfos[tmpIndex[4]].h,m_vertexInfos[tmpIndex[4]].y));
			m_faceInfos[m_vertexInfos[tmpIndex[2]].faceIndex[0]].index[0] = tmpIndex[4];
			m_faceInfos[m_vertexInfos[tmpIndex[2]].faceIndex[0]].normal = XMath::getNormal(XVector3(m_vertexInfos[tmpIndex[4]].x,m_vertexInfos[tmpIndex[4]].h,m_vertexInfos[tmpIndex[4]].y),
					XVector3(m_vertexInfos[tmpIndex[1]].x,m_vertexInfos[tmpIndex[1]].h,m_vertexInfos[tmpIndex[1]].y),
					XVector3(m_vertexInfos[tmpIndex[2]].x,m_vertexInfos[tmpIndex[2]].h,m_vertexInfos[tmpIndex[2]].y));;
			m_vertexInfos[tmpIndex[1]].faceIndex[1] = m_vertexInfos[j].faceIndex[1];
			m_vertexInfos[tmpIndex[4]].faceIndex[4] = m_vertexInfos[tmpIndex[3]].faceIndex[4];
			//�޸��ٽӵĽǵ�
			m_vertexInfos[j].faceIndex[1] = -1;
			m_vertexInfos[j].faceIndex[2] = -1;
			m_vertexInfos[j].faceIndex[5] = -1;
			m_vertexInfos[j].faceSum -= 3;
			m_vertexInfos[tmpIndex[3]].faceIndex[0] = -1;
			m_vertexInfos[tmpIndex[3]].faceIndex[3] = -1;
			m_vertexInfos[tmpIndex[3]].faceIndex[4] = -1;
			m_vertexInfos[tmpIndex[3]].faceSum -= 3;
	
			++ operatorSum;
		}
		printf("�Ż�:%d\n",operatorSum);
		if(operatorSum == 0) break;
	}
}
#if !WITH_INLINE_FILE
#include "XTerrain.inl" 
#endif
}