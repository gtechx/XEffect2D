//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XModelStl.h"

bool _XModelStl::load(const char * filename)
{
	if(m_isInited) return false;
	if(filename == NULL) return false;
	FILE * fp = NULL;
	if((fp = fopen(filename,"rb")) == NULL) return false;
	int fLen = getFileLen(fp);
	fread(m_modelName,1,sizeof(m_modelName),fp);	//��ȡ�ļ���
	fread(&m_faceSum,1,sizeof(m_faceSum),fp);
	if(m_faceSum <= 0 || fLen != m_faceSum * 50 + 84) 
	{
		fclose(fp);
		return false;
	}
	m_normalData = createArrayMem<float>(m_faceSum * 9);
	m_vectorData = createArrayMem<float>(m_faceSum * 9);
	if(m_normalData == NULL || m_vectorData == NULL)
	{
		XDELETE_ARRAY(m_normalData);
		XDELETE_ARRAY(m_vectorData);
		fclose(fp);
		return false;
	}
	//�������ζ�ȡ�����Ϣ
	float temp[12];
	char tempFlag[2];
	for(int i = 0;i < m_faceSum;++ i)
	{
		fread(temp,1,sizeof(temp),fp);
		fread(tempFlag,1,sizeof(tempFlag),fp);
		m_normalData[i * 9 + 0] = temp[0];
		m_normalData[i * 9 + 1] = temp[1];
		m_normalData[i * 9 + 2] = temp[2];
		m_normalData[i * 9 + 3] = temp[0];
		m_normalData[i * 9 + 4] = temp[1];
		m_normalData[i * 9 + 5] = temp[2];
		m_normalData[i * 9 + 6] = temp[0];
		m_normalData[i * 9 + 7] = temp[1];
		m_normalData[i * 9 + 8] = temp[2];
		m_vectorData[i * 9 + 0] = temp[3];
		m_vectorData[i * 9 + 1] = temp[4];
		m_vectorData[i * 9 + 2] = temp[5];
		m_vectorData[i * 9 + 3] = temp[6];
		m_vectorData[i * 9 + 4] = temp[7];
		m_vectorData[i * 9 + 5] = temp[8];
		m_vectorData[i * 9 + 6] = temp[9];
		m_vectorData[i * 9 + 7] = temp[10];
		m_vectorData[i * 9 + 8] = temp[11];
	}
	fclose(fp);
	//���ݶ�ȡ���֮����VBO
	if(!vbo.init(m_faceSum * 3,m_vectorData,NULL,m_normalData,NULL)) 
	{
		m_isInited = true;
		release();
		return false;
	}
	m_isInited = true;
	return true;
}
void _XModelStl::draw()
{
	if(!m_isInited) return;
	updateMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(m_matrix);
	//glTranslatef(m_position.x,m_position.y,m_position.z);
	//glRotatef(m_angle.x,1,0,0);
	//glRotatef(m_angle.y,0,1,0);
	//glRotatef(m_angle.z,0,0,1);
	//glScalef(m_size.x,m_size.y,m_size.z);

	_X3DWorld::GetInstance().m_worldMaterial.usetMaterial();	//���û�в�����Ϣ��ʹ��Ĭ�ϵĲ���
	glColor4fv(m_color);
	_X3DWorld::GetInstance().useShadow(XFalse,SHADER_SHADOW);

	vbo.drawByArray(GL_TRIANGLES);

	_X3DWorld::GetInstance().removeShadow();

	glPopMatrix();
}