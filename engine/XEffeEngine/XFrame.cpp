//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"	//������Ҫʹ��shader������Ҫ����ļ���������Ҫ����
#include "XFrame.h"
#include "XBasicWindow.h"
#include "XBasicOpenGL.h"

#include "XResourcePack.h"
#include "XLogBook.h"
//_XResourcePack ResourceTemp;

_XBool _XFrame::init(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) 
	{
		LogStr("The action have initted!");
		return XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	//���������ڴ�
	m_texnum = createArrayMem<_XTexture>(MAX_FRAME_SUM);
	m_frameName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	m_keyFrameArray = createArrayMem<int>(MAX_FRAME_SUM);
	wre = createArrayMem<GLint>(MAX_FRAME_SUM);
	hre = createArrayMem<GLint>(MAX_FRAME_SUM);
	xpa = createArrayMem<GLint>(MAX_FRAME_SUM);
	ypa = createArrayMem<GLint>(MAX_FRAME_SUM);
	if(m_texnum == NULL || m_frameName == NULL || m_keyFrameArray == NULL
			|| wre == NULL || hre == NULL || xpa == NULL || ypa == NULL)
	{
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		return XFalse;
	}

	x = 0;			//�����λ��
	y = 0;
	setAngle(0);
	xsize = 1;		//��������ű���
	ysize = 1;

	strcpy(m_frameName,filename);

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
	if(nameLength < 7)
	{
		LogStr("Action file path error!");
		//exit(1);
		return XFalse;
	}
	//read text file		//�����ȡ����֡�������ı��ĵ�����
	char numble[3];
	char tempFrameName[256];
	strcpy(tempFrameName,m_frameName);

	for(int i = nameLength -1;i >0;-- i)
	{
		if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
		{//find the key char
			tempFrameName[i + 1] = 'f';
			tempFrameName[i + 2] = 'r';
			tempFrameName[i + 3] = 'a';
			tempFrameName[i + 4] = 'm';
			tempFrameName[i + 5] = 'e';
			tempFrameName[i + 6] = '.';
			tempFrameName[i + 7] = 't';
			tempFrameName[i + 8] = 'x';
			tempFrameName[i + 9] = 't';
			tempFrameName[i +10] = '\0';
			break;
		}
	}	
	//�������Ҫ���Ƕ��߳���Դ���⣬Ψһ��һ�����߳����Ѿ����˻��⴦��
	if(m_resoursePosition == RESOURCE_LOCAL_PACK)
	{
	/*	static int checkedresource = 0;
		if(checkedresource == 0)
		{
			checkedresource = 1;
			if(ResourceTemp.checkCheckData() == 0)
			{//���صĴ������ﶨ����Ϸ
				DebugShow("Resource Data Error!\n");
				while(true)
				{
					Sleep(100);
				}
			}
		}*/

		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFrameName);
		if(p == NULL) return XFalse;

		int offset = 0;
		if(sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%d:",&m_allFramesSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]) != 1) {XDELETE_ARRAY(p);return XFalse;}
					offset += getCharPosition((char *)(p + offset),',') + 1;
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						XDELETE_ARRAY(p);
						return XFalse;
					}
				}
			}
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(sscanf((char *)(p + offset),"%c:",&tempFlag) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//��ȡƫ����Ϣ
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(sscanf((char *)(p + offset),"(%d,%d,%d),",&temp_z,&temp_x,&temp_y) != 3) {XDELETE_ARRAY(p);return XFalse;}
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		XDELETE_ARRAY(p);
	}else
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(tempFrameName,"rb")) == NULL)
		{
			LogStr("Action text file error!");
			return XFalse;
		}
		if(fscanf(fp,"%d:",&m_allKeyFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%d:",&m_allFramesSum) != 1) {fclose(fp);return XFalse;}
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			LogNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			return XFalse;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(fscanf(fp,"%c:",&tempFlag) != 1) {fclose(fp);return XFalse;}
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(fscanf(fp,"%d,",&m_keyFrameArray[i]) != 1) {fclose(fp);return XFalse;}
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						LogNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						fclose(fp);
						return XFalse;
					}
				}
			}
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			if(fscanf(fp,"%c:",&tempFlag) != 1) {fclose(fp);return XFalse;}
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//��ȡƫ����Ϣ
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					if(fscanf(fp,"(%d,%d,%d),",&temp_z,&temp_x,&temp_y) != 3) {fclose(fp);return XFalse;}
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		fclose(fp);
	}

	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		numble[0] = '0' + i%10;
		numble[1] = '0' + (i/10)%10;
		numble[2] = '0' + (i/100)%10;
		tempFrameName[nameLength - 5] = numble[0];
		tempFrameName[nameLength - 6] = numble[1];
		tempFrameName[nameLength - 7] = numble[2];

		if(!m_texnum[i].load(tempFrameName,m_resoursePosition))
		{
			LogStr("The action pictrue load error!");
			return XFalse;
		}else
		{
			//�������֮�������صļ���
			if(isNPOT(m_texnum[i].m_w,m_texnum[i].m_h)) 
			{
				wre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_w)/logf(2.0f)));
				hre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_h)/logf(2.0f)));
				xpa[i] = (wre[i] - m_texnum[i].m_w)/2;
				ypa[i] = (hre[i] - m_texnum[i].m_h)/2;
			}else 
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
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
		m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
	}

	m_isInited = XTrue;
	m_isEnd = XFalse;
	m_isSetEnd = XFalse;
	return XTrue;
}
_XBool _XFrame::releaseMem()
{
//	if(m_isACopy == 0 && m_texnum != NULL)
//	{
//		for(int i = 0;i < MAX_FRAME_SUM;++ i)
//		{
//			m_texnum[i].release();
//		}
//	}
	//if(m_haveSelfMemery != 0)
	//{
	//	XDELETE_ARRAY(m_texnum);
	//	//XDELETE_ARRAY(m_width);
	//	//XDELETE_ARRAY(m_height);
	//	XDELETE_ARRAY(m_frameName);
	//	XDELETE_ARRAY(m_keyFrameArray);
	//	XDELETE_ARRAY(wre);
	//	XDELETE_ARRAY(hre);
	//	XDELETE_ARRAY(xpa);
	//	XDELETE_ARRAY(ypa);
	//	m_haveSelfMemery = 0;
	//}
	XDELETE_ARRAY(m_centerDW);
	XDELETE_ARRAY(m_centerDH);
	XDELETE_ARRAY(m_keyFramePosition);

	m_isInited = XFalse;
	return XTrue;
}
void _XFrame::draw(const GLuint *pTexnum) const
{//����֡����
	if(!m_isVisible) return;
	if(m_isDisappearAtEnd && m_isEnd) return;	//�������֮����ʧ

	int temp_Frame = m_keyFrameArray[(int)(m_nowFramesNumble)];

//	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	if(!m_isFlash) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_BLEND);
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
#endif
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, wre[temp_Frame] << 1, 0, hre[temp_Frame] << 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//+++++++++++++++++++
	//���������Ҫ�����Ż�������ѵ�һ֡���ڼ���ͼƬ���ģ����Ե�һ֡���Բ��ܷ�������ƫ�ƣ�
	//float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * (cosAngle - 1) + m_centerDH[temp_Frame] * (ysize - 1.0f);
	//float dw = m_centerDW[temp_Frame] * xsize * (1 - cosAngle) + m_centerDH[temp_Frame] * ysize * sinAngle - m_centerDW[temp_Frame] * (xsize - 1.0f);
	float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * cosAngle - m_centerDH[temp_Frame];
	float dw = m_centerDW[temp_Frame] - m_centerDW[temp_Frame] * xsize * cosAngle + m_centerDH[temp_Frame] * ysize * sinAngle;
	int halfW = m_texnum[temp_Frame].m_w >> 1;
	int halfH = m_texnum[temp_Frame].m_h >> 1;
	//-------------------
	glTranslatef(x + m_keyFramePosition[temp_Frame].x + dw + halfW,
		y + m_keyFramePosition[temp_Frame].y - dh +  halfH, 0);
	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

//	if(m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		//else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);

		//m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		if(pTexnum != NULL) m_pShader->useShaderEx(pTexnum[temp_Frame]);//glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else m_pShader->useShaderEx(m_texnum[temp_Frame].m_texture);//glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);
		//m_pShader->useShaderEx();
	}

	glBegin(GL_QUADS);
	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_texnum[temp_Frame].m_w * m_texnum[temp_Frame].m_h;
#endif
	if(!m_isOverturn)
	{
		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(halfW, halfH);

		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(-halfW, halfH);
	}else
	{
		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2f(halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2f(-halfW, -halfH);

		glTexCoord2f(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(-halfW, halfH);

		glTexCoord2f(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2f(halfW, halfH);
	}
	glEnd();
	//if(m_pShaderProc != NULL) glUseProgram(0);				//ֹͣʹ��shader
	if(m_pShader != NULL) m_pShader->disShader();				//ֹͣʹ��shader

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
#if WITH_XSPRITE_EX
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
#endif
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_BLEND);

	//if(m_pShaderProc != NULL) glPopAttrib();
}
void _XFrame::move(int timeDelay)
{
	if(!m_isInited || m_isEnd) return;
	if(m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
	m_nowFramesNumble += m_actionSpeed * timeDelay;			//�ƶ�����֡����
	if(m_nowFramesNumble >= m_allFramesSum) 
	{//������ŵ����һ֡�������֡
		m_nowFramesNumble =0;

		if(!m_isEndImmediately)
		{//������ǿ������̽����Ķ��������ﲥ�����һ��ѭ��֮�󣬼��ɽ���
			if(m_isSetEnd) m_isEnd = XTrue;
		}
		if(!m_isLoop) m_isEnd = XTrue;	//����ǲ�ѭ���Ķ�������Ҳ���ԱȽϽ���
	}else	
	if(m_nowFramesNumble < 0) 
	{//����ǵ��򲥷ŵĻ��������ж��Ƿ񲥷����(�Լ�������ɴ���)
		m_nowFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if(!m_isEndImmediately && m_isSetEnd) m_isEnd = XTrue;
		if(!m_isLoop) m_isEnd = XTrue;
	}
}
void _XFrame::setAttribute(const _XVector2& position,_XBool loop,_XBool endImmediately,int startFrame,float actionSpeed,_XBool disappearAtEnd,_XBool isOverturn)
{
	if(!m_isInited) return; //����Ҫ��ʼ��֮����ܽ����������
	x = position.x;			//�����λ��
	y = position.y;

	m_isLoop = loop;
	m_isEndImmediately = endImmediately;

	m_startFrame = startFrame;
	m_nowFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	m_isDisappearAtEnd = disappearAtEnd;

	if((m_isOverturn && !isOverturn) || (!m_isOverturn && isOverturn))
	{
		m_isOverturn = isOverturn;

		m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//�ܵ�����֡��������
		m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_keyFramePosition[i].x = 2.0f * m_centerX - m_texnum[i].m_w - m_keyFramePosition[i].x;
			m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
			m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
		}
	}
}

_XFrame::_XFrame()
:m_isInited(XFalse)
,m_resoursePosition(RESOURCE_LOCAL_FOLDER)
,angle(0.0f)
,angleRadian(0.0f)
,sinAngle(0.0f)
,cosAngle(1.0f)
,m_isFlash(XFalse)
//,m_pShaderProc(NULL)
,m_pShader(NULL)
,x(0.0f),y(0.0f)
,xsize(1.0f),ysize(1.0f)
,alpha(1.0f)
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_isOverturn(XFalse)	//Ĭ������²���ת
,m_isVisible(XTrue)
,m_allFramesSum(0)							//����֡��������֡��
,m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
,m_nowFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
,m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
,m_isLoop(XFalse)								//����֡�Ƿ����ѭ��
,m_startFrame(0)							//����һ֡��ʼ
,m_isEnd(XTrue)								//����֡�Ƿ񲥷����
,m_isEndImmediately(XFalse)					//�����Ƿ�Ϊ���̽�������
,m_isSetEnd(XFalse)							//is end by user			//�Ƿ���������֡����
,m_isDisappearAtEnd(XFalse)					//����֡�Ƿ��ڲ������֮����ʧ
,m_centerX(0.0f)
,m_centerY(0.0f)
//,m_haveSelfMemery(1)	//��־�ڹ��캯���з������ڴ�ռ�
,m_isACopy(XFalse)
,m_cp(NULL)
,m_texnum(NULL)
,m_frameName(NULL)
,m_keyFrameArray(NULL)
,wre(NULL)
,hre(NULL)
,xpa(NULL)
,ypa(NULL)
{
	////���в����
	//m_texnum = createArrayMem<_XTexture>(MAX_FRAME_SUM);
	//m_frameName = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
	//m_keyFrameArray = createArrayMem<int>(MAX_FRAME_SUM);

	//wre = createArrayMem<GLint>(MAX_FRAME_SUM);
	//hre = createArrayMem<GLint>(MAX_FRAME_SUM);
	//xpa = createArrayMem<GLint>(MAX_FRAME_SUM);
	//ypa = createArrayMem<GLint>(MAX_FRAME_SUM);
	//˽�п��ܱ仯��
	m_centerDW = createArrayMem<float>(MAX_FRAME_SUM);
	m_centerDH = createArrayMem<float>(MAX_FRAME_SUM);
	m_keyFramePosition = createArrayMem<_XVector2>(MAX_FRAME_SUM);
}

//_XFrame::_XFrame(char withNoMemry)
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
//,m_nowFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
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
//	m_centerDW = createArrayMem<float>(MAX_FRAME_SUM);
//	m_centerDH = createArrayMem<float>(MAX_FRAME_SUM);
//	m_keyFramePosition = createArrayMem<_XVector2>(MAX_FRAME_SUM);
//}
_XFrame::~_XFrame()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	releaseMem();
}
_XFrame& _XFrame::operator = (const _XFrame& temp)
{
	if(&temp == this) return * this;	//��ֹû��Ҫ�����ҿ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
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
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
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

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
	return *this;
}
void _XFrame::setACopy(const _XFrame& temp)
{
	if(&temp == this) return;			//��ֹû��Ҫ�����ҿ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
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
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
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

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = XTrue;
}
_XFrame::_XFrame(const _XFrame &temp)
{
	if(&temp == this) return;			//��ֹû��Ҫ�����ҿ���
}