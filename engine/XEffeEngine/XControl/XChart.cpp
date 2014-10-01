#include "glew.h"
#include "XChart.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
//1�������ߵ�Ψһ�ԣ�����ѡ���Ƿ���ʾĳһ���ߣ����Ǳ�������Ҫ��һ������ʾ����
//2���ߵ��߶�Խ������β�ֵ��֤�ߵ������Ҳ��ử��ȥ
//3������ϵ��ʼ����������ʾ�ڵ����ݵ�����
//4������ֵ�Ĵ�������˵����ֻ��һ����

void chartChkCtrlFun(void *pClass,int objectID)
{
	_XChart &pPar = *(_XChart *)pClass;
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(objectID == pPar.m_dataChecks[i].getControlID() && !pPar.m_dataChecks[i].getState())
		{
			if(!pPar.checkCanHideLine())
			{//�޸�Ϊ���ܸı�
				pPar.m_dataChecks[i].setState(XTrue);
				return;
			}
			break;
		}
	}
	pPar.m_neddUpdateNowLineData = true;
}
_XFColor chartLineColor[CHART_MAX_LINE_SUM];
bool _XChart::checkCanHideLine()
{
	int sum = 0;
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_dataChecks[i].getState() && m_datas[i].size() > 1) ++ sum;
		if(sum > 0) return true;
	}
	return false;
}
bool _XChart::initWithoutTex(const _XRect &rect,const char *caption,const _XFontUnicode &font)
{
	if(m_isInited) return false;

	chartLineColor[0] = _XFColor::red;
	chartLineColor[1] = _XFColor::green;
	chartLineColor[2] = _XFColor::blue;
	chartLineColor[3] = _XFColor::yellow;
	chartLineColor[4] = _XFColor::brown;
	chartLineColor[5] = _XFColor::burlyWood;
	chartLineColor[6] = _XFColor::royalBlue;
	chartLineColor[7] = _XFColor::greenYellow;
	chartLineColor[8] = _XFColor::hotPink;
	chartLineColor[9] = _XFColor::aqua;
	chartLineColor[10] = _XFColor::cornflowerBlue;
	chartLineColor[11] = _XFColor::aquamarine;

	m_dataRect = rect;
	m_position.set(0.0f,0.0f);
	m_size.set(1.0f,1.0f);

	m_horizontalNetSum = 10;
	m_verticalNetSum = 10;

	m_showDataRange.set(0.0f,0.5f);	//������������õ����ݣ���������������ͨ����������ݼ��������
	m_baseValue.set(0.0f,0.0f);	//ԭ��Ļ�׼��ֵ
	m_sizeValue.set(0.5f,2.0);	//����Ĵ�С

	m_caption.setACopy(font);
	m_caption.setString(caption);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_textFont.setACopy(font);
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_textFont.setSize(0.5f,0.5f);
	m_textFont.setColor(0.5f,0.5f,0.5f,1.0f);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_textFont);
#endif

	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].initWithoutTex(" ",m_caption,0.5f,_XRect(0.0f,0.0f,16.0f,16.0f),_XVector2(16.0f,8.0f));
	//	m_dataChecks[i].setSize(0.5f);
		m_dataChecks[i].setPosition(m_position.x + m_dataRect.right + 16.0f,m_position.y + m_dataRect.top + 17.0f * i);
		m_dataChecks[i].setState(XTrue);
		m_dataChecks[i].setStateChangeCB(chartChkCtrlFun,this);
		m_dataChecks[i].setTextColor(_XFColor(0.5f,0.5f,0.5f,1.0f));
		m_dataChecks[i].disVisible();
		m_dataChecks[i].setWithAction(XFalse);
		_XCtrlManger.decreaseAObject(&m_dataChecks[i]);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_dataChecks[i]);
#endif
		glGenBuffersARB(1,&m_v[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v[i]);
		glGenBuffersARB(1,&m_nowV[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowV[i]);
		glGenBuffersARB(1,&m_nowPoint[i]);
	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowPoint[i]);
		m_linePointSum[i] = 0;
		m_linePointsSum[i] = 0;
	}
	//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
	m_downHeight = 20.0f;			//����������ʾ�ĸ߶�
	m_allLineRectHeight = 100.0f;	//������ʾ�����ߵ�����ĸ߶�

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isInited = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return true;
}
void _XChart::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	checkRange();	//�������ݱ�֤���ݵĺϷ���
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].draw();
	}
	if(m_neddUpdateNowLineData)
	{
		updateRange();
		m_neddUpdateNowLineData = false;
	}
	if(m_neadUpdateAllLineData)
	{
		allLineUpdateVbo();
		m_neadUpdateAllLineData = false;
	}
	char tmpStr[64];
	m_caption.draw();
	//��������
	_XRect tmpDrawRect(m_position.x + m_dataRect.left * m_size.x,m_position.y + m_dataRect.top * m_size.y,
		m_position.x + m_dataRect.right * m_size.x,m_position.y + m_dataRect.bottom * m_size.y);	//������ݵ�����
	drawFillBoxExA(_XVector2(tmpDrawRect.left,tmpDrawRect.top),
		_XVector2(tmpDrawRect.getWidth(),tmpDrawRect.getHeight()),0.9f * m_color.fR,0.9f * m_color.fG,0.9f * m_color.fB,m_color.fA);
	//�����
		//��ֱ
	char printMode[] = "%.4f";
	printMode[2] = abs(m_dataAccuracyY - 1) % 10 + '0';
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	float tmpDataValue;
	float tmpX;
	int ex = 0;
	tmpDataValue = m_firstNetValue.y;
	float pixelPerValueY = tmpDrawRect.getHeight() / m_sizeValue.y;	//ÿ�����ض�Ӧ����ֵ
	float pixelPerValueX = tmpDrawRect.getWidth() / m_sizeValue.x;	//ÿ�����ض�Ӧ����ֵ
	for(int i = 0;i < m_verticalNetSum ;++ i)
	{
		tmpX = (tmpDataValue - m_baseValue.y) * pixelPerValueY;
		drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
			tmpDrawRect.left - 5.0f * m_size.x,tmpDrawRect.bottom - tmpX,1.0f,0.1f * m_color.fR,0.1f * m_color.fG,0.1f * m_color.fB,m_color.fA);
		if(getApproximateData(tmpDataValue,1,ex) == 0.0f || ex < m_dataAccuracyY)
		{//�������,����
			drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
				tmpDrawRect.right,tmpDrawRect.bottom - tmpX,1.0f,0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA);
		}else
		{
			drawLine(tmpDrawRect.left,tmpDrawRect.bottom - tmpX,
				tmpDrawRect.right,tmpDrawRect.bottom - tmpX,1.0f,0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,1);
		}

		sprintf(tmpStr,printMode,tmpDataValue);
		m_textFont.setString(tmpStr);
		m_textFont.setPosition(tmpDrawRect.left - m_textFont.getMaxPixelWidth(),tmpDrawRect.bottom - tmpX);
		m_textFont.draw();
		tmpDataValue += m_stepWidth.y;
	}
		//ˮƽ
	printMode[2] = abs(m_dataAccuracyX - 1) % 10 + '0';
	m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	tmpDataValue = m_firstNetValue.x;
	for(int i = 0;i < m_horizontalNetSum;++ i)
	{
		tmpX = (tmpDataValue - m_baseValue.x) * pixelPerValueX;
		drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.bottom,
			tmpDrawRect.left + tmpX,tmpDrawRect.bottom + 5.0f * m_size.y,1.0f,0.1f * m_color.fR,0.1f * m_color.fG,0.1f * m_color.fB,m_color.fA);

		if(getApproximateData(tmpDataValue,1,ex) == 0.0f || ex < m_dataAccuracyX)
		{
			drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.top,
				tmpDrawRect.left + tmpX,tmpDrawRect.bottom,1.0f,0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA);
		}else
		{
			drawLine(tmpDrawRect.left + tmpX,tmpDrawRect.top,
				tmpDrawRect.left + tmpX,tmpDrawRect.bottom,1.0f,0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,1);
		}

		m_textFont.setPosition(tmpDrawRect.left + tmpX,tmpDrawRect.bottom);
		sprintf(tmpStr,printMode,tmpDataValue);
		m_textFont.setString(tmpStr);
		m_textFont.draw();
		tmpDataValue += m_stepWidth.x;
	}
	//����������
	if(tmpDrawRect.isInRect(XEE::mousePosition))
	{//�������ڷ�Χ��
		m_textFont.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_textFont.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
		drawLine(tmpDrawRect.left,XEE::mousePosition.y,
			tmpDrawRect.right,XEE::mousePosition.y,1.0f,0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA,1);
		drawLine(XEE::mousePosition.x,tmpDrawRect.top,
			XEE::mousePosition.x,tmpDrawRect.bottom,1.0f,0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA,1);
		sprintf(tmpStr,"%f",(tmpDrawRect.bottom - XEE::mousePosition.y) / pixelPerValueY + m_baseValue.y);
		m_textFont.setString(tmpStr);
		if(XEE::mousePosition.y < tmpDrawRect.getCenter().y) m_textFont.setPosition(tmpDrawRect.left,XEE::mousePosition.y);
		else m_textFont.setPosition(tmpDrawRect.left,XEE::mousePosition.y - m_textFont.getMaxPixelHeight());
		m_textFont.draw();

		sprintf(tmpStr,"%f",(XEE::mousePosition.x - tmpDrawRect.left) / pixelPerValueX + m_baseValue.x);
		m_textFont.setString(tmpStr);
		if(XEE::mousePosition.x < tmpDrawRect.getCenter().x) m_textFont.setPosition(XEE::mousePosition.x,tmpDrawRect.bottom - 16.0f * m_size.x);
		else m_textFont.setPosition(XEE::mousePosition.x - m_textFont.getMaxPixelWidth(),tmpDrawRect.bottom - 16.0f * m_size.x);
		m_textFont.draw();
	}
	//�������
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		if(m_dataChecks[i].getState())
		{
			if(m_linePointSum[i] > 0) drawLinesVbo(m_nowV[i],m_linePointSum[i],1.0f,chartLineColor[i].fR * m_color.fR,chartLineColor[i].fG * m_color.fG,chartLineColor[i].fB * m_color.fB,m_color.fA);
			if(m_withPoint && m_linePointsSum[i] > 0) drawPointsVbo(m_nowPoint[i],m_linePointsSum[i],2,chartLineColor[i].fR * m_color.fR,chartLineColor[i].fG * m_color.fG,chartLineColor[i].fB * m_color.fB,m_color.fA);
		}
		//m_dataChecks[i].draw();
		drawFillBox(tmpDrawRect.right + 1.0f * m_size.x,tmpDrawRect.top + (1.0f + 17.0f * i) * m_size.y,
			14.0f * m_size.x,14.0f * m_size.y,chartLineColor[i].fR * m_color.fR,chartLineColor[i].fG * m_color.fG,chartLineColor[i].fB * m_color.fB,m_color.fA);
	}
	//�������ϵ
	drawLine(tmpDrawRect.left,tmpDrawRect.top,
		tmpDrawRect.left,tmpDrawRect.bottom,1.0f,0.0f,0.0f,0.0f,m_color.fA);
	drawLine(tmpDrawRect.left,tmpDrawRect.bottom,
		tmpDrawRect.right,tmpDrawRect.bottom,1.0f,0.0f,0.0f,0.0f,m_color.fA);
	//����������������
	drawFillBoxExA(_XVector2(tmpDrawRect.left,tmpDrawRect.bottom + m_downHeight * m_size.y),
			_XVector2(tmpDrawRect.getWidth(),m_allLineRectHeight * m_size.y),0.9f * m_color.fR,0.9f * m_color.fG,0.9f * m_color.fB,m_color.fA);
	//�������ߺ��յ���
	if(m_dataRange.getWidth() > 0.0f)
	{
		float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * tmpDrawRect.getWidth() + tmpDrawRect.left;
		drawLine(sX,tmpDrawRect.bottom + m_downHeight * m_size.y,
			sX,tmpDrawRect.bottom + (m_downHeight + m_allLineRectHeight) * m_size.y,1.0f,0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA);

		float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left) / m_dataRange.getWidth() * tmpDrawRect.getWidth() + tmpDrawRect.left;
		drawLine(eX,tmpDrawRect.bottom + m_downHeight * m_size.y,
			eX,tmpDrawRect.bottom + (m_downHeight + m_allLineRectHeight) * m_size.y,1.0f,0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA);
	}
	//������еĵ�	
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		drawLinesVbo(m_v[i],m_datas[i].size(),1.0f,chartLineColor[i].fR * m_color.fR,chartLineColor[i].fG * m_color.fG,chartLineColor[i].fB * m_color.fB,m_color.fA);
	}
}
void _XChart::allLineUpdateVbo()
{
	if(m_maxLineLen <= 1) return;	//û����Ҫ��������
	if(m_drawDataBuff == NULL || m_drawDataBuffSize != m_maxLineLen)
	{
		m_drawDataBuff = createArrayMem<float>(m_maxLineLen * 2);
		m_drawDataBuffSize = m_maxLineLen;
	}
	float pixelPerValueY = m_allLineRectHeight * m_size.y / m_dataRange.getHeight();	//ÿ�����ض�Ӧ����ֵ
	float pixelPerValueX = m_dataRect.getWidth() * m_size.x / m_dataRange.getWidth();	//ÿ�����ض�Ӧ����ֵ
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		for(int j = 0;j < m_datas[i].size();++ j)
		{
			m_drawDataBuff[j << 1] = (m_datas[i][j].x - m_dataRange.left) * pixelPerValueX + m_position.x + m_dataRect.left;
			m_drawDataBuff[(j << 1) + 1] = - (m_datas[i][j].y - m_dataRange.top) * pixelPerValueY + 
				m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_size.y;
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_v[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,m_datas[i].size() * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
}
void _XChart::nowLineUpdateVbo()
{
	if(m_maxLineLen <= 1) return;	//û����Ҫ��������
	if(m_drawDataBuff == NULL || m_drawDataBuffSize != m_maxLineLen)
	{
		m_drawDataBuff = createArrayMem<float>(m_maxLineLen * 2);
		m_drawDataBuffSize = m_maxLineLen;
	}
	int index;
	float tmpValue;
	bool flagHead = false;
	bool flagEnd = false;
	float pixelPerValueY = m_dataRect.getHeight() * m_size.y / m_sizeValue.y;	//ÿ�����ض�Ӧ����ֵ
	float pixelPerValueX = m_dataRect.getWidth() * m_size.x / m_sizeValue.x;	//ÿ�����ض�Ӧ����ֵ
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		index = 0;
		flagHead = false;
		flagEnd = false;
		for(int j = 0;j < m_datas[i].size();++ j)
		{//������δ����Խ�������β�ֵ
			if(m_datas[i][j].x >= m_showDataRange.x && m_datas[i][j].x <= m_showDataRange.y)
			{//�����յ㶼��Ҫ������ʵ��Χ
				m_drawDataBuff[index << 1] = (m_datas[i][j].x - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_size.x;
				m_drawDataBuff[(index << 1) + 1] = - (m_datas[i][j].y - m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_size.y;
				++ index;
			}else
			if(j < m_datas[i].size() - 1 && m_datas[i][j].x < m_showDataRange.x && m_datas[i][j + 1].x >= m_showDataRange.x)
			{//��Խǰ�߽�
				tmpValue = lineSlerp(m_datas[i][j].y,m_datas[i][j + 1].y,(m_showDataRange.x - m_datas[i][j].x) / (m_datas[i][j + 1].x - m_datas[i][j].x));
				m_drawDataBuff[index << 1] = (m_showDataRange.x - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_size.x;
				m_drawDataBuff[(index << 1) + 1] = - (tmpValue- m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_size.y;
				++ index;
				flagHead = true;
			}else
			if(j > 0 && m_datas[i][j].x > m_showDataRange.y && m_datas[i][j - 1].x <= m_showDataRange.y)
			{//��Խ��߽�
				tmpValue = lineSlerp(m_datas[i][j - 1].y,m_datas[i][j].y,(m_showDataRange.y - m_datas[i][j - 1].x) / (m_datas[i][j].x - m_datas[i][j - 1].x));
				m_drawDataBuff[index << 1] = (m_showDataRange.y - m_baseValue.x) * pixelPerValueX + m_position.x + m_dataRect.left * m_size.x;
				m_drawDataBuff[(index << 1) + 1] = - (tmpValue - m_baseValue.y) * pixelPerValueY + m_position.y + m_dataRect.bottom * m_size.y;
				++ index;
				flagEnd = true;
			}
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowV[i]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB,index * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
		m_linePointSum[i] = index;
		if(m_withPoint)
		{
			if(flagHead)
			{
				if(flagEnd)
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 2) * 2 * sizeof(float),m_drawDataBuff + 2,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 2;
				}else
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 1) * 2 * sizeof(float),m_drawDataBuff + 2,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 1;
				}
			}else
			{
				if(flagEnd)
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,(index - 1) * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index - 1;
				}else
				{
					glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_nowPoint[i]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB,index * 2 * sizeof(float),m_drawDataBuff,GL_STATIC_DRAW_ARB);
					m_linePointsSum[i] = index;
				}
			}
		}
	}
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);	//ȡ����
}
void _XChart::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_dataRect.left * m_size.x + m_dataRect.getWidth() * 0.5f * m_size.x,m_position.y - 16.0f * m_size.y);
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].setPosition(m_position.x + (m_dataRect.right + 16.0f) * m_size.x,m_position.y + (m_dataRect.top + 17.0f * i) * m_size.y);
	}
	m_neadUpdateAllLineData = true;
	m_neddUpdateNowLineData = true;
}
void _XChart::setSize(float x,float y)
{
	if(!m_isInited) return;
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_dataRect.left * m_size.x + m_dataRect.getWidth() * 0.5f * m_size.x,m_position.y - 16.0f * m_size.y);
	m_caption.setSize(m_size);
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].setPosition(m_position.x + (m_dataRect.right + 16.0f) * m_size.x,m_position.y + (m_dataRect.top + 17.0f * i) * m_size.y);
		m_dataChecks[i].setSize(m_size);
	}
	m_textFont.setSize(m_size * 0.5f);
	m_neadUpdateAllLineData = true;
	m_neddUpdateNowLineData = true;
}
_XBool _XChart::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].mouseProc(x,y,mouseState);
	}
	_XRect tmpRect;
	tmpRect.set(m_position.x + m_dataRect.left * m_size.x,m_position.y + (m_dataRect.bottom + m_downHeight) * m_size.y,
		m_position.x + m_dataRect.right * m_size.x,m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_size.y);
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		if(tmpRect.isInRect(x,y) && m_dataRange.getWidth() > 0.0f)
		{
			float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() * m_size.x + m_position.x + m_dataRect.left * m_size.x;
			float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() * m_size.x + m_position.x + m_dataRect.left * m_size.x;
			if(x >= sX - 5 && x <= sX + 5)
			{//ѡ��ͷ
				m_chooseArm = 1;
			}else
			if(x >= eX - 5 && x <= eX + 5)
			{//ѡ��β
				m_chooseArm = 2;
			}else
			{//û��ѡ��
				m_chooseArm = 0;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:
		m_chooseArm = 0;
		break;
	case MOUSE_MOVE://�ƶ�����
		switch(m_chooseArm)
		{
		case 1:
			if(m_dataRange.getWidth() > 0.0f)
			{
				//float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() + m_position.x + m_dataRect.left;
				float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() * m_size.x + m_position.x + m_dataRect.left * m_size.x;
				if(x < eX - 10)
				{
					m_showDataRange.x = (x - (m_position.x + m_dataRect.left * m_size.x)) / (m_dataRect.getWidth() * m_size.x) * m_dataRange.getWidth() + m_dataRange.left;
					if(m_showDataRange.x < m_dataRange.left) m_showDataRange.x = m_dataRange.left;
				}
			}
			break;
		case 2:
			if(m_dataRange.getWidth() > 0.0f)
			{
				float sX = (m_baseValue.x - m_dataRange.left) / m_dataRange.getWidth() * m_dataRect.getWidth() * m_size.x + m_position.x + m_dataRect.left * m_size.x;
				//float eX = (m_baseValue.x + m_sizeValue.x - m_dataRange.left)/m_dataRange.getWidth() * m_dataRect.getWidth() + m_position.x + m_dataRect.left;
				if(x > sX + 10)	
				{
					m_showDataRange.y = (x - (m_position.x + m_dataRect.left * m_size.x)) / (m_dataRect.getWidth() * m_size.x) * m_dataRange.getWidth() + m_dataRange.left;
					if(m_showDataRange.y > m_dataRange.right) m_showDataRange.y = m_dataRange.right;
				}
			}
			break;
		}
		if(m_chooseArm != 0)
		{
			m_baseValue.x = m_showDataRange.x;
			m_sizeValue.x = m_showDataRange.y - m_showDataRange.x;
			m_neddUpdateNowLineData = true;
			//updateRange();
		}
		break;
	}
	return XTrue;
}
void _XChart::checkRange()
{
	if(m_dataRange.getWidth() > 0.0f)
	{
		float temp = m_dataRange.getWidth() / m_dataRect.getWidth() * 9.6f;
		if(m_showDataRange.x < m_dataRange.left ||
			m_showDataRange.y < m_dataRange.left ||
			m_showDataRange.x > m_dataRange.right ||
			m_showDataRange.y > m_dataRange.right ||
			m_showDataRange.y - m_showDataRange.x < temp)
		{//��С����
			m_showDataRange.x = m_dataRange.left;
			m_showDataRange.y = m_showDataRange.x + temp;
			m_neddUpdateNowLineData = true;
		}
	}
}
void _XChart::updateRange()
{//����x��Χ�ı仯����y�ķ�Χ���������������ϵ��������
	bool flag = true;
	float tmpValue;
	_XRect tmpRange;
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1)
		{
			m_dataChecks[i].disVisible();
			continue;
		}else
		{
			m_dataChecks[i].setVisible();
		}
		if(m_datas[i].size() <= 1 || !m_dataChecks[i].getState()) continue;
		for(int j = 0;j < m_datas[i].size();++ j)
		{
			if(m_datas[i][j].x >= m_showDataRange.x && m_datas[i][j].x <= m_showDataRange.y)
			{//�����ڷ�Χ��
				if(flag)
				{
					tmpRange.set(m_datas[i][j].x,m_datas[i][j].y,m_datas[i][j].x,m_datas[i][j].y);
					flag = false;
				}else
				{
					if(m_datas[i][j].y < tmpRange.top) tmpRange.top = m_datas[i][j].y;
					if(m_datas[i][j].y > tmpRange.bottom) tmpRange.bottom = m_datas[i][j].y;
				}
			}
			if(j < m_datas[i].size() - 1 && m_datas[i][j].x < m_showDataRange.x && m_datas[i][j + 1].x >= m_showDataRange.x)
			{//��Խǰ�߽磬������Ҫ��ֵ������ʹ�����Բ�ֵ
				tmpValue = lineSlerp(m_datas[i][j].y,m_datas[i][j + 1].y,(m_showDataRange.x - m_datas[i][j].x) / (m_datas[i][j + 1].x - m_datas[i][j].x));
				if(flag)
				{
					tmpRange.top = tmpValue;
					tmpRange.bottom = tmpValue;
					flag = false;
				}else
				{
					if(tmpValue < tmpRange.top) tmpRange.top = tmpValue;
					if(tmpValue > tmpRange.bottom) tmpRange.bottom = tmpValue;
				}
			}
			if(j > 0 && m_datas[i][j].x > m_showDataRange.y && m_datas[i][j - 1].x < m_showDataRange.y)
			{//��Խ��߽磬������Ҫ��ֵ������ʹ�����Բ�ֵ
				tmpValue = lineSlerp(m_datas[i][j - 1].y,m_datas[i][j].y,(m_showDataRange.y - m_datas[i][j - 1].x) / (m_datas[i][j].x - m_datas[i][j - 1].x));
				if(flag)
				{
					tmpRange.top = tmpValue;
					tmpRange.bottom = tmpValue;
					flag = false;
				}else
				{
					if(tmpValue < tmpRange.top) tmpRange.top = tmpValue;
					if(tmpValue > tmpRange.bottom) tmpRange.bottom = tmpValue;
				}
			}
		}
	}
	m_baseValue.set(m_showDataRange.x,tmpRange.top - tmpRange.getHeight() * 0.05f);	//���¶���һ����أ���ֹ�����б�
	m_sizeValue.set(m_showDataRange.y - m_showDataRange.x,tmpRange.getHeight() * 1.1f);

	m_stepWidth.y = getApproximateData(m_sizeValue.y / m_verticalNetSum,2,m_dataAccuracyY);
	float tmpData = pow(10.0f,m_dataAccuracyY - 1);
	m_stepWidth.y = m_stepWidth.y * tmpData * 10.0f;
	//�����һ�����λ��
	if(m_baseValue.y <= 0 && m_baseValue.y + m_sizeValue.y >= 0)
	{//��Խ��Ľ���
		m_firstNetValue.y = m_baseValue.y - fmod(m_baseValue.y,m_stepWidth.y);
	}else
	{
		m_firstNetValue.y = (int)((m_baseValue.y + m_stepWidth.y) / tmpData) * tmpData;
	}
	
	m_stepWidth.x = getApproximateData(m_sizeValue.x / m_horizontalNetSum,2,m_dataAccuracyX);
	tmpData = pow(10.0f,m_dataAccuracyX - 1);
	m_stepWidth.x = m_stepWidth.x * tmpData * 10.0f;
	//�����һ�����λ��
	if(m_baseValue.x <= 0 && m_baseValue.x + m_sizeValue.x >= 0)
	{//��Խ��Ľ���
		m_firstNetValue.x = m_baseValue.x - fmod(m_baseValue.x,m_stepWidth.x);
	}else
	{
		m_firstNetValue.x = (int)((m_baseValue.x + m_stepWidth.x) / tmpData) * tmpData;
	}
	nowLineUpdateVbo();

	m_nowMouseRect.set(m_position.x + m_dataRect.left * m_size.x,
		m_position.y + (m_dataRect.top - 32.0f) * m_size.x,
		m_position.x + m_dataRect.right * m_size.x,
		m_position.y + (m_dataRect.bottom + m_downHeight + m_allLineRectHeight) * m_size.x);
	float right;
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		right = m_dataChecks[i].getBox(2).x;
		if(right > m_nowMouseRect.right) m_nowMouseRect.right = right;
	}
}
void _XChart::insertData(const _XVector2 &data,int lineIndex)
{
	if(lineIndex < 0 || lineIndex >= CHART_MAX_LINE_SUM) return;	//���ݲ��Ϸ�
	//����Ҫ��������ʹ��ԭʼ�������㷨�����Խ����Ż�
	if(m_dataSum == 0)
	{
		m_dataRange.set(data.x,data.y,data.x,data.y);
	}else
	{
		if(data.x < m_dataRange.left) m_dataRange.left = data.x;
		if(data.x > m_dataRange.right) m_dataRange.right = data.x;
		if(data.y < m_dataRange.top) m_dataRange.top = data.y;
		if(data.y > m_dataRange.bottom) m_dataRange.bottom = data.y;
	}
	m_datas[lineIndex].push_back(data);
	for(int i = 0;i < m_datas[lineIndex].size();++ i)
	{//�����ݽ�������
		if(m_datas[lineIndex][i].x > data.x)
		{//�����ݲ���
			for(int j = m_datas[lineIndex].size() - 1;j > i;-- j)
			{
				m_datas[lineIndex][j] = m_datas[lineIndex][j - 1];
			}
			m_datas[lineIndex][i] = data;
			break;//�������֮���˳�
		}
	}
	if(m_datas[lineIndex].size() > m_maxLineLen) m_maxLineLen = m_datas[lineIndex].size();
	++ m_dataSum;	//�������
	m_neddUpdateNowLineData = true;
	//updateRange();
	m_neadUpdateAllLineData = true;	
	//allLineUpdateVbo();
}
_XBool _XChart::readDataFromFile(const char * filename)	//���ļ��ж�ȡ��������
{
	if(filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return false;
	int sum = 0;
	char tmpString[1024];
	if(fscanf(fp,"name:%s\n",tmpString) != 1) {fclose(fp);return XFalse;}
	if(fscanf(fp,"lineSum:%d,\n",&sum) != 1) {fclose(fp);return XFalse;}
	m_caption.setString(tmpString);
	//����������е�ԭʼ����
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_datas[i].clear();
		m_linePointSum[i] = 0;
		m_dataSum = 0;
	}

	//�������ζ�ȡ����
	_XVector2 tmpPoint;
	int tmpSum;
	for(int i = 0;i < sum;++ i)
	{
		if(fscanf(fp,"lineName:%s\n",tmpString) != 1) {fclose(fp);return XFalse;}
		m_dataChecks[i].setCaptionText(tmpString);
		if(fscanf(fp,"pointSum:%d,\n",&tmpSum) != 1) {fclose(fp);return XFalse;}
		for(int j = 0;j < tmpSum;++ j)
		{
			if(fscanf(fp,"%f,%f,\n",&tmpPoint.x,&tmpPoint.y) != 2) {fclose(fp);return XFalse;}
			insertData(tmpPoint,i);
		}
	}
	fclose(fp);	
	return XTrue;
}
_XBool _XChart::saveDataToFile(const char * filename)	//�ӽ��������ݱ��浽�ļ���
{
	if(filename == NULL) return XFalse;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL) return false;
	//����д������
	int sum = 0;
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() > 1) ++ sum;
	}
	fprintf(fp,"name:%s\n",m_caption.getString());
	fprintf(fp,"lineSum:%d,\n",sum);
	if(sum <= 0)
	{
		fclose(fp);	
		return XTrue;
	}
	//��������д��ÿ���ߵ�����
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		if(m_datas[i].size() <= 1) continue;
		fprintf(fp,"lineName:%s\n",m_dataChecks[i].getCaptionString());
		fprintf(fp,"pointSum:%d,\n",m_datas[i].size());
		for(int j = 0;j < m_datas[i].size();++ j)
		{
			fprintf(fp,"%f,%f,\n",m_datas[i][j].x,m_datas[i][j].y);
		}
	}
	fclose(fp);	
	return XTrue;
}
void _XChart::release()
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_drawDataBuff);
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		glDeleteBuffersARB(1,&m_v[i]);
		glDeleteBuffersARB(1,&m_nowV[i]);
		glDeleteBuffersARB(1,&m_nowPoint[i]);
	}
	m_isInited = false;
}
void _XChart::setColor(float r,float g,float b,float a) 
{
	m_color.setColor(r,g,b,a);
	m_caption.setColor(m_color);
	m_textFont.setColor(m_color);
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].setColor(m_color);
	}
}
void _XChart::setAlpha(float a)
{
	m_color.setA(a);
	m_caption.setAlpha(a);
	m_textFont.setAlpha(a);
	for(int i = 0;i < CHART_MAX_LINE_SUM;++ i)
	{
		m_dataChecks[i].setAlpha(a);
	}
}