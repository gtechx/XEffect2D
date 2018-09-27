#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFrame.h"
#include "XResourcePack.h"
#include "XShaderGLSL.h"	
#include "XFile.h"
namespace XE {
bool XFrame::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		LogStr("Action text file error!");
		return XFalse;
	}
	if (fscanf(fp, "%d:", &m_allKeyFramesSum) != 1) { fclose(fp); return XFalse; }
	if (m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%d:", &m_allFramesSum) != 1) { fclose(fp); return XFalse; }
	if (m_allFramesSum <= 0 || m_allFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		fclose(fp);
		return XFalse;
	}
	{//��ȡ��Ƿ�	D��default, M��menutrue
		char tempFlag = ' ';
		if (fscanf(fp, "%c:", &tempFlag) != 1) { fclose(fp); return XFalse; }
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFrameArray[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (fscanf(fp, "%d,", &m_keyFrameArray[i]) != 1) { fclose(fp); return XFalse; }
				//�µ������зǷ���֡��ţ�Ϊ�հ�֡
				if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n", filename, i);
					//	fclose(fp);
					//	return XFalse;
				}
			}
		}
	}
	{//��ȡ��Ƿ�	D��default, M��menutrue
		char tempFlag = ' ';
		if (fscanf(fp, "%c:", &tempFlag) != 1) { fclose(fp); return XFalse; }
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFramePosition[i].set(0.0f);
			}
		}
		else
		{//��ȡƫ����Ϣ
			int temp_x;
			int temp_y;
			int temp_z;
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (fscanf(fp, "(%d,%d,%d),", &temp_z, &temp_x, &temp_y) != 3) { fclose(fp); return XFalse; }
				m_keyFramePosition[i].set(temp_x, temp_y);
			}
		}
	}
	fclose(fp);
	return true;
}
bool XFrame::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	unsigned char *p = XResPack.getFileData(filename);
	if (p == NULL) return XFalse;

	int offset = 0;
	if (sscanf((char *)(p + offset), "%d:", &m_allKeyFramesSum) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if (sscanf((char *)(p + offset), "%d:", &m_allFramesSum) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
	offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
	if (m_allFramesSum <= 0 || m_allFramesSum >= m_maxFrameSum)
	{
		LogNull("Action text file data error:%s!\n", filename);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	{//��ȡ��Ƿ�	D��default, M��menutrue
		char tempFlag = ' ';
		if (sscanf((char *)(p + offset), "%c:", &tempFlag) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
		offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFrameArray[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (sscanf((char *)(p + offset), "%d,", &m_keyFrameArray[i]) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				if (m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
				{
					LogNull("Blank frame:%s -> %d!\n", filename, i);
					//	XMem::XDELETE_ARRAY(p);
					//	return XFalse;
				}
			}
		}
	}
	{//��ȡ��Ƿ�	D��default, M��menutrue
		char tempFlag = ' ';
		if (sscanf((char *)(p + offset), "%c:", &tempFlag) != 1) { XMem::XDELETE_ARRAY(p); return XFalse; }
		offset += XString::getCharPosition((char *)(p + offset), ':') + 1;
		if (tempFlag == 'D' || tempFlag == 'd')
		{
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				m_keyFramePosition[i].set(0.0f);
			}
		}
		else
		{//��ȡƫ����Ϣ
			int temp_x;
			int temp_y;
			int temp_z;
			for (int i = 0; i < m_allFramesSum; ++i)
			{
				if (sscanf((char *)(p + offset), "(%d,%d,%d),", &temp_z, &temp_x, &temp_y) != 3) { XMem::XDELETE_ARRAY(p); return XFalse; }
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				offset += XString::getCharPosition((char *)(p + offset), ',') + 1;
				m_keyFramePosition[i].set(temp_x, temp_y);
			}
		}
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFrame::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XFrame::init(const char *filename, XResPos resPos)
{
	if (m_isInited)
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if (resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;
	try
	{
		m_cp = new XSCounter;
		if (m_cp == NULL) return XFalse;
	}
	catch (...)
	{
		return XFalse;
	}
	//���������ڴ�
	m_texnum = XMem::createArrayMem<XTexture>(m_maxFrameSum);
	m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	m_keyFrameArray = XMem::createArrayMem<int>(m_maxFrameSum);
	wre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	hre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	xpa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	ypa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	if (m_texnum == NULL || m_frameName == NULL || m_keyFrameArray == NULL
		|| wre == NULL || hre == NULL || xpa == NULL || ypa == NULL)
	{
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		return XFalse;
	}
	//�����λ��
	x = y = 0.0f;
	setAngle(0.0f);
	//��������ű���
	xsize = ysize = 1.0f;

	strcpy_s(m_frameName, MAX_FILE_NAME_LENGTH, filename);

	//�����������е�����֡ͼƬ
	//load action's pictrue
	//note:the path of the action format is /namefold/name+action+xxx(frame).xxx
	//get name length
/*	int nameLength =0;
	for(int i = 0;i < 256;++ i)
	{
		if(m_frameName[i] == '\0')
		{
			nameLength = i;
			break;
		}
	}*/
	int nameLength = strlen(m_frameName);
	if (nameLength < 7)
	{
		LogStr("Action file path error!");
		//exit(1);
		return XFalse;
	}
	//read text file		//�����ȡ����֡�������ı��ĵ�����
	//char numble[3];
	char tempFrameName[MAX_FILE_NAME_LENGTH];
	strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);

	//for(int i = nameLength -1;i >0;-- i)
	//{
	//	if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
	//	{//find the key char
	//		tempFrameName[i + 1] = 'f';
	//		tempFrameName[i + 2] = 'r';
	//		tempFrameName[i + 3] = 'a';
	//		tempFrameName[i + 4] = 'm';
	//		tempFrameName[i + 5] = 'e';
	//		tempFrameName[i + 6] = '.';
	//		tempFrameName[i + 7] = 't';
	//		tempFrameName[i + 8] = 'x';
	//		tempFrameName[i + 9] = 't';
	//		tempFrameName[i +10] = '\0';
	//		break;
	//	}
	//}	
	int index = XFile::getPathDeepByChar(tempFrameName);
	tempFrameName[index + 1] = '\0';
	strcat_s(tempFrameName, MAX_FILE_NAME_LENGTH, "frame.txt");
	//�������Ҫ���Ƕ��߳���Դ���⣬Ψһ��һ�����߳����Ѿ����˻��⴦��
	switch (m_resoursePosition)
	{
	case RES_LOCAL_PACK:
		if (!loadFromPacker(tempFrameName)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if (!loadFromFolder(tempFrameName)) return false;
		break;
	case RES_WEB:
		if (!loadFromWeb(tempFrameName)) return false;
		break;
	case RES_AUTO:
		if (!loadFromPacker(tempFrameName) && !loadFromFolder(tempFrameName) &&
			!loadFromWeb(tempFrameName)) return false;
		break;
	}

	strcpy_s(tempFrameName, MAX_FILE_NAME_LENGTH, m_frameName);
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		tempFrameName[nameLength - 5] = '0' + i % 10;;
		tempFrameName[nameLength - 6] = '0' + (i / 10) % 10;;
		tempFrameName[nameLength - 7] = '0' + (i / 100) % 10;

		if (!m_texnum[i].load(tempFrameName, m_resoursePosition))
		{
			LogStr("The action pictrue load error!");
			return XFalse;
		}
		else
		{
			//�������֮�������صļ���
			if (XMath::isNPOT(m_texnum[i].m_w, m_texnum[i].m_h))
			{
				wre[i] = XMath::getMinWellSize2n(m_texnum[i].m_w);
				hre[i] = XMath::getMinWellSize2n(m_texnum[i].m_h);
				xpa[i] = (wre[i] - m_texnum[i].m_w) / 2;
				ypa[i] = (hre[i] - m_texnum[i].m_h) / 2;
			}
			else
			{
				wre[i] = m_texnum[i].m_w;
				hre[i] = m_texnum[i].m_h;
				xpa[i] = 0;
				ypa[i] = 0;
			}
		}
	}
	//Ϊ������֡����ƫ�ƶ��������
	m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//�ܵ�����֡��������
	m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
	for (int i = 0; i < m_allKeyFramesSum; ++i)
	{
		m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
		m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
	}

	m_isInited = XTrue;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
XBool XFrame::releaseMem()
{
	//	if(m_isACopy == 0 && m_texnum != NULL)
	//	{
	//		for(int i = 0;i < m_maxFrameSum;++ i)
	//		{
	//			m_texnum[i].release();
	//		}
	//	}
		//if(m_haveSelfMemery != 0)
		//{
		//	XMem::XDELETE_ARRAY(m_texnum);
		//	//XMem::XDELETE_ARRAY(m_width);
		//	//XMem::XDELETE_ARRAY(m_height);
		//	XMem::XDELETE_ARRAY(m_frameName);
		//	XMem::XDELETE_ARRAY(m_keyFrameArray);
		//	XMem::XDELETE_ARRAY(wre);
		//	XMem::XDELETE_ARRAY(hre);
		//	XMem::XDELETE_ARRAY(xpa);
		//	XMem::XDELETE_ARRAY(ypa);
		//	m_haveSelfMemery = 0;
		//}
	XMem::XDELETE_ARRAY(m_centerDW);
	XMem::XDELETE_ARRAY(m_centerDH);
	XMem::XDELETE_ARRAY(m_keyFramePosition);

	m_isInited = XFalse;
	return XTrue;
}
void XFrame::draw(const GLuint *pTexnum) const
{//����֡����
	if (!m_isVisible) return;
	if (m_isDisappearAtEnd && m_isEnd) return;	//�������֮����ʧ

	int keyFrameIndex = m_keyFrameArray[(int)(m_curFramesNumble)];
	if (keyFrameIndex < 0 || keyFrameIndex >= m_allKeyFramesSum) return;

	//	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	XGL::EnableTexture2D();
	if (!m_isFlash) XGL::setBlendAlpha();
	else XGL::setBlendAdd();

#if WITHXSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#endif
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, (wre[keyFrameIndex] << 1), 0, (hre[keyFrameIndex] << 1), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//+++++++++++++++++++
	//���������Ҫ�����Ż�������ѵ�һ֡���ڼ���ͼƬ���ģ����Ե�һ֡���Բ��ܷ�������ƫ�ƣ�
	//float dh = m_centerDW[keyFrameIndex] * xsize * sinAngle + m_centerDH[keyFrameIndex] * ysize * (cosAngle - 1) + m_centerDH[keyFrameIndex] * (ysize - 1.0f);
	//float dw = m_centerDW[keyFrameIndex] * xsize * (1 - cosAngle) + m_centerDH[keyFrameIndex] * ysize * sinAngle - m_centerDW[keyFrameIndex] * (xsize - 1.0f);
	float dh = m_centerDW[keyFrameIndex] * xsize * sinAngle + m_centerDH[keyFrameIndex] * ysize * cosAngle - m_centerDH[keyFrameIndex];
	float dw = m_centerDW[keyFrameIndex] - m_centerDW[keyFrameIndex] * xsize * cosAngle + m_centerDH[keyFrameIndex] * ysize * sinAngle;
	int halfW = m_texnum[keyFrameIndex].m_w >> 1;
	int halfH = m_texnum[keyFrameIndex].m_h >> 1;
	//-------------------
	glTranslatef(x + m_keyFramePosition[keyFrameIndex].x + dw + halfW,
		y + m_keyFramePosition[keyFrameIndex].y - dh + halfH, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(xsize, ysize, 0.0f);

	//	if(m_pShaderProc == NULL)
	if (m_pShader == NULL)
	{
		if (pTexnum != NULL) XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		else XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);
	}
	else
	{
		//glActiveTexture(GL_TEXTURE0);
		//XGL::EnableTexture2D();
		//if(pTexnum != NULL) XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		//else XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);

		//m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		if (pTexnum != NULL) m_pShader->useShaderEx(pTexnum[keyFrameIndex]);//XGL::BindTexture2D(pTexnum[keyFrameIndex]);
		else m_pShader->useShaderEx(m_texnum[keyFrameIndex].m_texture);//XGL::BindTexture2D(m_texnum[keyFrameIndex].m_texture);
		//m_pShader->useShaderEx();
	}

	glBegin(GL_QUADS);
	glColor4f(colorRed, colorGreen, colorBlue, alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_texnum[keyFrameIndex].m_w * m_texnum[keyFrameIndex].m_h;
#endif
	if (!m_isOverturn)
	{
		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex]);
		//glTexCoord2f(0, 0);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex]);
		//glTexCoord2f(1, 0);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(1, 1);
		glVertex2f(halfW, halfH);

		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(0, 1);
		glVertex2f(-halfW, halfH);
	}
	else
	{
		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex]);
		//glTexCoord2f(0, 0);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex]);
		//glTexCoord2f(1, 0);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[keyFrameIndex] + m_texnum[keyFrameIndex].m_w, ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(1, 1);
		glVertex2f(-halfW, halfH);

		glTexCoord2f(xpa[keyFrameIndex], ypa[keyFrameIndex] + m_texnum[keyFrameIndex].m_h);
		//glTexCoord2f(0, 1);
		glVertex2f(halfW, halfH);
	}
	glEnd();
	//if(m_pShaderProc != NULL) glUseProgram(0);				//ֹͣʹ��shader
	if (m_pShader != NULL) m_pShader->disShader();				//ֹͣʹ��shader

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
#if WITHXSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	XGL::DisableBlend();
	//if(m_pShaderProc != NULL) glPopAttrib();
}
void XFrame::move(float timeDelay)
{
	if (!m_isInited || m_isEnd) return;
	if (m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
	m_curFramesNumble += m_actionSpeed * timeDelay;			//�ƶ�����֡����
	if (m_curFramesNumble >= m_allFramesSum)
	{//������ŵ����һ֡�������֡
		m_curFramesNumble = 0.0f;

		if (!m_isEndImmediately)
		{//������ǿ������̽����Ķ��������ﲥ�����һ��ѭ��֮�󣬼��ɽ���
			if (m_isSetEnd) m_isEnd = XTrue;
		}
		if (!m_isLoop) m_isEnd = XTrue;	//����ǲ�ѭ���Ķ�������Ҳ���ԱȽϽ���
	}
	else
	if (m_curFramesNumble < 0.0f)
	{//����ǵ��򲥷ŵĻ��������ж��Ƿ񲥷����(�Լ�������ɴ���)
		m_curFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if (!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if (!m_isLoop) m_isEnd = XTrue;
	}
}
void XFrame::setAttribute(const XVec2& position, XBool loop, XBool endImmediately, int startFrame, float actionSpeed, XBool disappearAtEnd, XBool isOverturn)
{
	if (!m_isInited) return; //����Ҫ��ʼ��֮����ܽ����������
	x = position.x;			//�����λ��
	y = position.y;

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_curFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	m_isDisappearAtEnd = disappearAtEnd;

	if ((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
	{
		m_isOverturn = isOverturn;

		m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//�ܵ�����֡��������
		m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
		for (int i = 0; i < m_allKeyFramesSum; ++i)
		{
			m_keyFramePosition[i].x = 2.0f * m_centerX - m_texnum[i].m_w - m_keyFramePosition[i].x;
			m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
			m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
		}
	}
}

XFrame::XFrame()
	:m_isInited(XFalse)
	, m_resoursePosition(RES_AUTO)
	, angle(0.0f)
	, angleRadian(0.0f)
	, sinAngle(0.0f)
	, cosAngle(1.0f)
	, m_isFlash(XFalse)
	//,m_pShaderProc(NULL)
	, m_pShader(NULL)
	, x(0.0f), y(0.0f)
	, xsize(1.0f), ysize(1.0f)
	, alpha(1.0f)
	, colorRed(1.0f)
	, colorGreen(1.0f)
	, colorBlue(1.0f)
	, m_isOverturn(XFalse)	//Ĭ������²���ת
	, m_isVisible(XTrue)
	, m_allFramesSum(0)							//����֡��������֡��
	, m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
	, m_curFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
	, m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
	, m_isLoop(XFalse)								//����֡�Ƿ����ѭ��
	, m_startFrame(0)							//����һ֡��ʼ
	, m_isEnd(XTrue)								//����֡�Ƿ񲥷����
	, m_isEndImmediately(XFalse)					//�����Ƿ�Ϊ���̽�������
	, m_isSetEnd(XFalse)							//is end by user			//�Ƿ���������֡����
	, m_isDisappearAtEnd(XFalse)					//����֡�Ƿ��ڲ������֮����ʧ
	, m_centerX(0.0f)
	, m_centerY(0.0f)
	//,m_haveSelfMemery(1)	//��־�ڹ��캯���з������ڴ�ռ�
	, m_isACopy(XFalse)
	, m_cp(NULL)
	, m_texnum(NULL)
	, m_frameName(NULL)
	, m_keyFrameArray(NULL)
	, wre(NULL)
	, hre(NULL)
	, xpa(NULL)
	, ypa(NULL)
{
	////���в����
	//m_texnum = XMem::createArrayMem<XTexture>(m_maxFrameSum);
	//m_frameName = XMem::createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//m_keyFrameArray = XMem::createArrayMem<int>(m_maxFrameSum);

	//wre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//hre = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//xpa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//ypa = XMem::createArrayMem<GLint>(m_maxFrameSum);
	//˽�п��ܱ仯��
	m_centerDW = XMem::createArrayMem<float>(m_maxFrameSum);
	m_centerDH = XMem::createArrayMem<float>(m_maxFrameSum);
	m_keyFramePosition = XMem::createArrayMem<XVec2>(m_maxFrameSum);
}

//XFrame::XFrame(char withNoMemry)
//:m_isInited(0)
//,m_resoursePosition(RESOURCE_OUTSIDE)
//,angle(0.0f)
//,angleRadian(0.0f)
//,sinAngle(0.0f)
//,cosAngle(1.0f)
//,m_isFlash(0)
//,m_pShaderProc(NULL)
//,x(0.0f),y(0.0f)
//,xsize(1.0f),ysize(1.0f)
//,alpha(1.0f)
//,colorRed(1.0f)
//,colorGreen(1.0f)
//,colorBlue(1.0f)
//,m_isOverturn(0)	//Ĭ������²���ת
//,m_isVisible(1)
//,m_allFramesSum(0)							//����֡��������֡��
//,m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
//,m_curFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
//,m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
//,m_isLoop(0)								//����֡�Ƿ����ѭ��
//,m_startFrame(0)							//����һ֡��ʼ
//,m_isEnd(1)								//����֡�Ƿ񲥷����
//,m_isEndImmediately(0)					//�����Ƿ�Ϊ���̽�������
//,m_isSetEnd(0)							//is end by user			//�Ƿ���������֡����
//,m_isDisappearAtEnd(0)					//����֡�Ƿ��ڲ������֮����ʧ
//,m_centerX(0.0f)
//,m_centerY(0.0f)
//,m_haveSelfMemery(0)	//��־�ڹ��캯���з������ڴ�ռ�
//,m_isACopy(0)
//,m_texnum(NULL)
//,m_frameName(NULL)
//,m_keyFrameArray(NULL)
//,wre(NULL)
//,hre(NULL)
//,xpa(NULL)
//,ypa(NULL)
//{
//	m_centerDW = XMem::createArrayMem<float>(m_maxFrameSum);
//	m_centerDH = XMem::createArrayMem<float>(m_maxFrameSum);
//	m_keyFramePosition = XMem::createArrayMem<XVec2>(m_maxFrameSum);
//}
XFrame::~XFrame()
{
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
	}
	releaseMem();
}
XFrame& XFrame::operator = (const XFrame& temp)
{
	if (&temp == this) return *this;	//��ֹû��Ҫ�����ҿ���
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	angleRadian = temp.angleRadian;		//���ȱ�־�ĽǶ�
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת

	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_curFramesNumble = temp.m_curFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//�ܵ�����֡��������
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW, temp.m_centerDW, sizeof(float) * m_maxFrameSum);
	memcpy(m_centerDH, temp.m_centerDH, sizeof(float) * m_maxFrameSum);
	memcpy(m_keyFramePosition, temp.m_keyFramePosition, sizeof(XVec2) * m_maxFrameSum);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void XFrame::setACopy(const XFrame& temp)
{
	if (&temp == this) return;			//��ֹû��Ҫ�����ҿ���
	if (temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if (m_cp != NULL && --m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XMem::XDELETE_ARRAY(m_texnum);
		XMem::XDELETE_ARRAY(m_frameName);
		XMem::XDELETE_ARRAY(m_keyFrameArray);
		XMem::XDELETE_ARRAY(wre);
		XMem::XDELETE_ARRAY(hre);
		XMem::XDELETE_ARRAY(xpa);
		XMem::XDELETE_ARRAY(ypa);
		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisible = temp.m_isVisible;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	angleRadian = temp.angleRadian;		//���ȱ�־�ĽǶ�
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת

	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	//m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_curFramesNumble = temp.m_curFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//�ܵ�����֡��������
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW, temp.m_centerDW, sizeof(float) * m_maxFrameSum);
	memcpy(m_centerDH, temp.m_centerDH, sizeof(float) * m_maxFrameSum);
	memcpy(m_keyFramePosition, temp.m_keyFramePosition, sizeof(XVec2) * m_maxFrameSum);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
XFrame::XFrame(const XFrame &temp)
{
	if (&temp == this) return;			//��ֹû��Ҫ�����ҿ���
}
#if !WITH_INLINE_FILE
#include "XFrame.inl"
#endif
}