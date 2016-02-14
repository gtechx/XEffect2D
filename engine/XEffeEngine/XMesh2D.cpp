#include "XStdHead.h"
#include "XMesh2D.h"
#include "XMath\XMatrix2x2.h"
namespace XE{
void XMesh2D::drawWireframe()	//����߿�
{
	if(!m_isInited) return;
	if(m_v.size() <= 0) return;	//���û��������ֱ�ӷ���
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_position.x + m_center.x,m_position.y + m_center.y, 0);

	if(m_angle != 0.0f) glRotatef(m_angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(m_scale.x != 1.0f || m_scale.y != 1.0f) glScalef(m_scale.x, m_scale.y, 0);	//�������ŵı���

	int index;
	glColor4fv(m_color);	//����Ϊָ������ɫ
	glBegin(GL_LINES);
	for(int i = 0;i < m_sumY;++ i)
	{
		index = i * m_sumX;
		for(int j = 0;j < m_sumX;++ j,++ index)
		{
			if(j + 1 < m_sumX)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + 1] - m_center);
			}
			if(i + 1 < m_sumY)
			{
				glVertex2fv(m_v[index] - m_center);
				glVertex2fv(m_v[index + m_sumX] - m_center);
			}
		}
	}
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void XMesh2D::drawTexture(unsigned int tex)
{
	if(!m_isInited || tex == 0) return;
	if(m_v.size() <= 0) return;	//���û��������ֱ�ӷ���
	if(!m_withTexture)
	{
		drawWireframe();
		return;
	}
	XGL::EnableTexture2D();
	XGL::BindTexture2D(tex);
	XGL::EnableBlend();
	XGL::SetBlendFunc(XGL::srcBlendMode[4],XGL::dstBlendMode[5]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_position.x + m_center.x,m_position.y + m_center.y, 0);
//	glTranslatef(m_position.x,m_position.y, 0);

	if(m_angle != 0.0f) glRotatef(m_angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(m_scale.x != 1.0f || m_scale.y != 1.0f) glScalef(m_scale.x, m_scale.y, 0);	//�������ŵı���

	glColor4fv(m_color);	//����Ϊָ������ɫ
	//���濪ʼ���
	int index;
	for(int i = 0;i < m_sumY - 1;++ i)
	{
		index = i * m_sumX;
		glBegin(GL_TRIANGLE_STRIP);
		for(int j = 0;j < m_sumX;++ j,++ index)
		{
			if(m_withColor) 
				glColor4fv(m_c[index]);
			glTexCoord2fv(m_t[index]);
			glVertex2fv(m_v[index] - m_center);
			if(m_withColor) 
				glColor4fv(m_c[index + m_sumX]);
			glTexCoord2fv(m_t[index + m_sumX]);
			glVertex2fv(m_v[index + m_sumX] - m_center);
		}
		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	XGL::DisableBlend();
}
bool XMesh2D::saveToFile(const char *filename)		//�����ݱ��浽�ļ�������ʹ��OBJ�����ݸ�ʽ
{
	if(!m_isInited || filename == NULL) return false;
	FILE *fp;
	if((fp = fopen(filename,"w")) == NULL) return false;
	fprintf(fp,"%d,%d,\n",m_sumX,m_sumY);
	if(m_withTexture) fprintf(fp,"1,");
	else fprintf(fp,"0,");
	if(m_withColor) fprintf(fp,"1,\n");
	else fprintf(fp,"0,\n");
	//д�붥������
	for(int i = 0;i < m_v.size();++ i)
	{
		fprintf(fp,"%f,%f,\n",m_v[i].x,m_v[i].y);
	}
	if(m_withTexture)
	{
		for(int i = 0;i < m_t.size();++ i)
		{
			fprintf(fp,"%f,%f,\n",m_t[i].x,m_t[i].y);
		}
	}
	if(m_withColor)
	{
		for(int i = 0;i < m_c.size();++ i)
		{
			fprintf(fp,"%f,%f,%f,%f,\n",m_c[i].fR,m_c[i].fG,m_c[i].fB,m_c[i].fA);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::loadFromFile(const char *filename)	//���ļ��ж�ȡ����
{
	if(filename == NULL) return false;
	FILE *fp;
	int tmpFlag[2] = {0,0};
	if((fp = fopen(filename,"r")) == NULL) return false;
	clearAllData();
	if(fscanf(fp,"%d,%d,\n",&m_sumX,&m_sumY) != 2) return false;
	if(fscanf(fp,"%d,%d,\n",&tmpFlag[0],&tmpFlag[1]) != 2) return false;
	m_withTexture = (tmpFlag[0] != 0);
	m_withColor = (tmpFlag[1] != 0);
	//д�붥������
	XVector2 tmp;
	for(int i = 0;i < m_sumX * m_sumY;++ i)
	{
		if(fscanf(fp,"%f,%f,\n",&tmp.x,&tmp.y) != 2) return false;
		m_v.push_back(tmp);
	}
	if(m_withTexture)
	{
		for(int i = 0;i < m_sumX * m_sumY;++ i)
		{
			if(fscanf(fp,"%f,%f,\n",&tmp.x,&tmp.y) != 2) return false;
			m_t.push_back(tmp);
		}
	}
	if(m_withColor)
	{
		XFColor tmpC;
		for(int i = 0;i < m_sumX * m_sumY;++ i)
		{
			if(fscanf(fp,"%f,%f,%f,%f,\n",&tmpC.fR,&tmpC.fG,&tmpC.fB,&tmpC.fA) != 4) return false;
			m_c.push_back(tmpC);
		}
	}
	fclose(fp);
	return true;
}
bool XMesh2D::init(int w,int h,int sumX,int sumY,bool withT,bool withC)
{
	if(m_isInited) return false;
	if(w <= 0 || h <= 0 || sumX <= 1 || sumY <= 1) return false;	//�������ݲ��Ϸ�
	m_sumX = sumX;
	m_sumY = sumY;
	m_withTexture = withT;
	m_withColor = withC;
	clearAllData();
	XVector2 size = XVector2(w,h) / XVector2(sumX - 1,sumY - 1);
	XVector2 sizeT = XVector2(1.0f,1.0f) / XVector2(sumX - 1,sumY - 1);
	for(int i = 0;i < m_sumY;++ i)
	{
		for(int j = 0;j < m_sumX;++ j)
		{
			m_v.push_back(XVector2(size.x * j,size.y * i));
			if(withT)
				m_t.push_back(XVector2(sizeT.x * j,sizeT.y * i));
			if(withC)
				m_c.push_back(XFColor::white);
		}
	}
	m_isInited = true;
	return true;
}
int XMesh2D::getNearestVertex(const XVector2 &pos,float maxD)	//��ȡ��pos����Ķ���ı�ţ�����С��maxD
{
	//��local�ռ䵽world�ռ�
//	XMatrix2x2 tmp = XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale);
//	XVector2 ret = (pos - m_center) * tmp + m_position + m_center;
//	XRender::drawPoint(ret,4,1,1,1,1);
	//��world�ռ䵽local�ռ�
//	XMatrix2x2 tmp = XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale);
//	XVector2 ret = tmp.inverse() * (pos - m_center - m_position) + m_center;
//	printf("%f,%f\n",ret.x,ret.y);
	if(m_v.size() <= 0) return -1;	//��Ч������
	//��pos������ռ�ӳ�䵽mesh��local�ռ�
	XVector2 local = (XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale)).inverse() * (pos - m_center - m_position) + m_center;
	//�������еĶ���Ѱ�Ҿ�������Ķ���
	float minD = m_v[0].getLengthSqure(local);
	int index = 0;
	float tmpD;
	for(int i = 1;i < m_v.size();++ i)
	{
		tmpD = m_v[i].getLengthSqure(local);
		if(tmpD < minD)
		{
			minD = tmpD;
			index = i;
		}
	}
	if(minD <= maxD * maxD) return index;
	else return -1;
}
void XMesh2D::setVertexPos(const XVector2 &pos,int index)	//���ö����λ��
{
	if(index < 0 || index >= m_v.size()) return;	//�Ƿ�������
	m_v[index] = (XMath::getMatrix2D(m_angle * DEGREE2RADIAN) * XMath::getMatrix2D(m_scale)).inverse() * (pos - m_center - m_position) + m_center;
}
}