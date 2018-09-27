#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontBasic.h"
#include "..\XShaderGLSL.h"
namespace XE{
XBool XFontBasic::setACopy(const XFontBasic &temp)
{
	if(this == &temp) return XFalse;				//��ֹ��������
	m_isVisible = temp.m_isVisible;
	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;
	m_isPassword = temp.m_isPassword;
	m_fontPtSize = temp.m_fontPtSize;

	m_withFBO = temp.m_withFBO;
	m_needUpdateFBO = temp.m_needUpdateFBO;
	if(temp.m_pFbo != NULL)
	{
		XMem::XDELETE(m_pFbo);
		m_pFbo = XMem::createMem<XFBO>();
		if(m_pFbo == NULL) return XFalse;
		if(!m_pFbo->init(temp.m_pFbo->getWidth(0),temp.m_pFbo->getHeight(0))) return XFalse;
	}else
	{
		XMem::XDELETE(m_pFbo);
	}
	m_fboSprite.setACopy(temp.m_fboSprite);		//��ͼ

//	m_maxStringLen = temp.m_maxStringLen;
//	strcpy_s(m_string,m_maxStringLen,temp.m_string);			//��Ҫ��ʵ���ַ���
	//����Ĭ�ϵ��ַ������ȿ��ܲ�һ����������Ĳ������������
	if(m_maxStringLen >= temp.m_maxStringLen)
		strcpy_s(m_string,m_maxStringLen,temp.m_string);

	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	m_position = temp.m_position;			//�����λ��
	m_setPosition = temp.m_setPosition;		//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;					//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;			//����֮��ľ���	
	m_distanceY = temp.m_distanceY;			//����֮��ľ���	
	m_size = temp.m_size;					//���ֵĳߴ磨���ش�С��
	m_scale = temp.m_scale;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
	return XTrue;
}
XFontBasic::~XFontBasic()	//������麯����Ϊ�˷�ֹͨ������ָ��ɾ�����������ʱ��ɵ��ڴ�й¶
{
	XMem::XDELETE_ARRAY(m_string);
	XMem::XDELETE(m_pFbo);
}
XFontBasic& XFontBasic::operator = (const XFontBasic& temp)
{
	if(this == &temp) return * this;		//��ֹ��������
	m_isVisible = temp.m_isVisible;
	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;
	m_isPassword = temp.m_isPassword;
	m_fontPtSize = temp.m_fontPtSize;

	m_withFBO = temp.m_withFBO;
	m_needUpdateFBO = temp.m_needUpdateFBO;
	if(temp.m_pFbo != NULL)
	{
		XMem::XDELETE(m_pFbo);
		m_pFbo = XMem::createMem<XFBO>();
		if(m_pFbo == NULL) return *this;
		if(!m_pFbo->init(temp.m_pFbo->getWidth(0),temp.m_pFbo->getHeight(0))) return *this;
	}else
	{
		XMem::XDELETE(m_pFbo);
	}
	m_fboSprite.setACopy(temp.m_fboSprite);

//	m_maxStringLen = temp.m_maxStringLen;
//	strcpy_s(m_string,m_maxStringLen,temp.m_string);		//��Ҫ��ʵ���ַ���
	//����Ĭ�ϵ��ַ������ȿ��ܲ�һ����������Ĳ������������
	if(m_maxStringLen >= temp.m_maxStringLen)
		strcpy_s(m_string,m_maxStringLen,temp.m_string);

	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_distanceY = temp.m_distanceY;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_scale = temp.m_scale;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
	return * this;
}
XFontBasic::XFontBasic(const XFontBasic& temp)
{
	if(this == &temp) return;		//��ֹ��������
	m_isVisible = temp.m_isVisible;
	m_isCliped = temp.m_isCliped;
	m_clipRect = temp.m_clipRect;
	m_isPassword = temp.m_isPassword;
	m_fontPtSize = temp.m_fontPtSize;

	m_maxStringLen = temp.m_maxStringLen;
	m_string = XMem::createArrayMem<char>(m_maxStringLen);
	m_string[0] = '\0';

	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	strcpy_s(m_string,m_maxStringLen,temp.m_string);		//��Ҫ��ʵ���ַ���
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_distanceY = temp.m_distanceY;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_scale = temp.m_scale;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
}
//������������Ѿ����ص�bug�������λ�û��߽Ƕȵ������Ϣ�ı䵫����û�е���updateData(),��ʱ�����������õ�ֵ��ʱ�����ǲ���ȷ��
XVec2 XFontBasic::getBox(int order)
{
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	switch(m_alignmentModeX)
	{
	case FONT_ALIGNMENT_MODE_X_LEFT:
		switch(m_alignmentModeY)
		{
		case FONT_ALIGNMENT_MODE_Y_UP:
			switch(order)
			{
			case 0:return m_setPosition;
			case 1:return m_setPosition + XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth;
			case 2:return m_setPosition + XVec2(m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos);
			case 3:return m_setPosition + XVec2(-m_angleSin, m_angleCos) * m_maxPixelHeight;
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_MIDDLE:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(m_angleSin, -m_angleCos) * m_maxPixelHeight * 0.5f;
			case 1:return m_setPosition + XVec2(m_maxPixelWidth * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_maxPixelWidth * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos);
			case 2:return m_setPosition + XVec2(m_maxPixelWidth * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_maxPixelWidth * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
			case 3:return m_setPosition + XVec2(-m_angleSin, m_angleCos) * m_maxPixelHeight * 0.5f;
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_DOWN:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(m_angleSin, -m_angleCos);
			case 1:return m_setPosition + XVec2(m_maxPixelWidth * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_maxPixelWidth * m_angleSin - m_maxPixelHeight * m_angleCos);
			case 2:return m_setPosition + XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth;
			case 3:return m_setPosition;
			}
			break;
		}
		break;
	case FONT_ALIGNMENT_MODE_X_MIDDLE:
		switch(m_alignmentModeY)
		{
		case FONT_ALIGNMENT_MODE_Y_UP:
			switch(order)
			{
			case 0:return m_setPosition - XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth * 0.5f;
			case 1:return m_setPosition + XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth * 0.5f;
			case 2:return m_setPosition + XVec2(m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
			case 3:return m_setPosition + XVec2(- m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
				- m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_MIDDLE:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(- m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				- m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos);
			case 1:return m_setPosition + XVec2(m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos);
			case 2:return m_setPosition + XVec2(m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
			case 3:return m_setPosition + XVec2(m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_DOWN:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(- m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * m_angleSin,
				- m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * m_angleCos);
			case 1:return m_setPosition + XVec2(m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * m_angleCos);
			case 2:return m_setPosition + XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth * 0.5f;
			case 3:return m_setPosition - XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth * 0.5f;
			}
			break;
		}
		break;
	case FONT_ALIGNMENT_MODE_X_RIGHT:
		switch(m_alignmentModeY)
		{
		case FONT_ALIGNMENT_MODE_Y_UP:
			switch(order)
			{
			case 0:return m_setPosition - XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth;
			case 1:return m_setPosition;
			case 2:return m_setPosition + XVec2(-m_angleSin, m_angleCos) * m_maxPixelHeight;
			case 3:return m_setPosition + XVec2(- m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
				- m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos);
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_MIDDLE:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(- m_maxPixelWidth * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				- m_maxPixelWidth * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos);
			case 1:return m_setPosition + XVec2(m_angleSin, -m_angleCos) * m_maxPixelHeight * 0.5f;
			case 2:return m_setPosition + XVec2(-m_angleSin, m_angleCos) * m_maxPixelHeight * 0.5f;
			case 3:return m_setPosition + XVec2(- m_maxPixelWidth * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				- m_maxPixelWidth * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
			}
			break;
		case FONT_ALIGNMENT_MODE_Y_DOWN:
			switch(order)
			{
			case 0:return m_setPosition + XVec2(- m_maxPixelWidth * m_angleCos + m_maxPixelHeight * m_angleSin,
				- m_maxPixelWidth * m_angleSin - m_maxPixelHeight * m_angleCos);
			case 1:return m_setPosition + XVec2(m_angleSin, -m_angleCos) * m_maxPixelHeight;
			case 2:return m_setPosition;
			case 3:return m_setPosition - XVec2(m_angleCos, m_angleSin) * m_maxPixelWidth;
			}
			break;
		}
		break;
	}
	return XVec2::zero;
}
#if !WITH_INLINE_FILE
#include "XFontBasic.inl"
#endif
}