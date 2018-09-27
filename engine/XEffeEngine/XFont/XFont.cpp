#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
#include "XFont.h"
#include "XResourcePack.h"

#include "XObjectManager.h"
#include "XFontUnicode.h"
namespace XE{
XFont::XFont()
{
	m_textPosition.resize(m_maxStringLen);
	m_textRect.resize(m_maxStringLen);
	m_objType = OBJ_FONTX;
}
XFont::XFont(int maxStrLen)
:XFontBasic(maxStrLen)
{
	m_textPosition.resize(m_maxStringLen);
	m_textRect.resize(m_maxStringLen);
	m_objType = OBJ_FONTX;
}
XFont::~XFont()
{
	m_sprite.release();
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(this);
#endif
}
XBool XFont::init(const char *filename,const XVec2& size,const XVec2& layout,
					XResPos resPos,XBool/*withFBO*/)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	if(filename == NULL) return XFalse;

	m_resoursePosition = resPos;

	m_string[0] = '\0';
	if(!m_sprite.init(filename,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif

	m_position.reset();
	m_setPosition.set(0.0f);
	m_angle = 0.0f;			
	m_distance = 0.0f;		
	m_size = size;			
	m_layout = layout;
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isCliped = XFalse;
	
	m_isInited = XTrue;			
	return XTrue;
}
bool XFont::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;//�ļ�����ʧ��
	if(fscanf(fp,"%f,%f,%f,%f,",&m_size.x,&m_size.y,&m_layout.x,&m_layout.y) != 4)
	{
		fclose(fp);
		return XFalse;
	}
	fclose(fp);
	return true;
}
bool XFont::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL) return XFalse;
	if(sscanf((char *)p,"%f,%f,%f,%f,",&m_size.x,&m_size.y,&m_layout.x,&m_layout.y) != 4)
	{
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFont::loadFromWeb(const char *filename)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XFont::initEx(const char *filename,XResPos resPos,XBool/*withFBO*/)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	if(filename == NULL) return XFalse;

	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;

	m_string[0] = '\0';
	if(!m_sprite.init(filename,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif
	//������ļ��ж�ȡ����
	char filenameTemp[MAX_FONT_UNICODE_FILE_NAME_LENGTH];
	strcpy_s(filenameTemp,MAX_FONT_UNICODE_FILE_NAME_LENGTH,filename);
	int filenameLength = strlen(filename);
	memcpy(filenameTemp + filenameLength - 3, &"dat", 3);
	//filenameTemp[filenameLength - 3] = 'd';
	//filenameTemp[filenameLength - 2] = 'a';
	//filenameTemp[filenameLength - 1] = 't';
//	XVec2 size(0.0f,0.0f);
//	XVec2 layout(0.0f,0.0f);
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filenameTemp)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filenameTemp)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filenameTemp)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filenameTemp) && !loadFromFolder(filenameTemp) &&
			!loadFromWeb(filenameTemp)) return false;
		break;
	}

	m_position.set(0,0);
	m_setPosition.set(0,0);
	m_angle = 0.0f;			
	m_distance = 0.0f;		
//	m_size = size;			
//	m_layout = layout;
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isCliped = XFalse;
	
	m_isInited = XTrue;			
	return XTrue;
}
XBool XFont::initFromTTF(const char * filename,int fontSize,XResPos resPos,XBool/*withFBO*/)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	if(filename == NULL) return XFalse;

	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;

	int fontOrder = -1;
	if (m_fontPtSize >= 0) fontOrder = XTTFFont.loadTTFFile(filename, m_fontPtSize, fontSize, m_resoursePosition);
	else fontOrder = XTTFFont.loadTTFFile(filename, fontSize, fontSize, m_resoursePosition);
	if(fontOrder < 0) return XFalse;
	unsigned int tex;
	XVec2 layout;
	if(!XTTFFont.getTextureFont(fontOrder,tex,layout)) return XFalse;
	//�ͷ���Դ
	XTTFFont.releaseTTFFile(fontOrder);
	int w = XMath::getMinWellSize2n(layout.x * fontSize * 0.5f);
	int h = XMath::getMinWellSize2n(layout.y * fontSize);
	m_sprite.init(w,h,COLOR_RGBA,tex,POINT_LEFT_TOP);

	m_string[0] = '\0';
	//if(!m_sprite.init(filename,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif
//	char filenameTemp[MAX_FONT_UNICODE_FILE_NAME_LENGTH];
//	strcpy_s(filenameTemp,MAX_FONT_UNICODE_FILE_NAME_LENGTH,filename);
//	int filenameLength = strlen(filename);
//	filenameTemp[filenameLength - 3] = 'd';
//	filenameTemp[filenameLength - 2] = 'a';
//	filenameTemp[filenameLength - 1] = 't';
//	FILE *fp = NULL;
//	XVector2I size(0,0);
//	XVector2I layout(0,0);
//	if(m_resoursePosition == RES_LOCAL_FOLDER)
//	{
//		if((fp = fopen(filenameTemp,"r")) == NULL)
//		{//�ļ�����ʧ��
//			return XFalse;
//		}
//		fscanf(fp,"%f,%f,%f,%f,",&size.x,&size.y,&layout.x,&layout.y);
//		fclose(fp);
//	}else
//	{
//		unsigned char *p = XResPack.getFileData(filenameTemp);
//		if(p == NULL) return XFalse;
//		sscanf((char *)p,"%f,%f,%f,%f,",&size.x,&size.y,&layout.x,&layout.y);
//		XMem::XDELETE_ARRAY(p);
//	}

	m_position.reset();
	m_setPosition.reset();
	m_angle = 0.0f;			
	m_distance = 0.0f;		
	m_size.set(fontSize * 0.5f,fontSize);			
	m_layout = layout;
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isCliped = XFalse;
	
	m_isInited = XTrue;
	return XTrue;
}
void XFont::draw()
{
	if(!m_isVisible) return;
	if(m_isCliped && m_clipRect.bottom <= 0)
	{//�������ʲôҲ���û�
		return;
	}
	if(m_needUpdateData)
	{
		updateData();
		m_needUpdateData = XFalse;
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);
		m_sprite.draw();
	}
}
void XFont::updateData()
{
	m_needShowTextSum = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	int cur_x = 0;
	int cur_y = 0;
	m_lineSum = 0;
	char tempHead = ' ';
	char tempEnd = '}';
	int wordsSum = -1;
	float textPixelX = (m_size.x + m_distance) * m_scale.x;	//һ���ַ��Ŀ�ȣ�����
	float textPixelY = (m_size.y + m_distanceY) * m_scale.y;	//һ���ַ��ĸ߶ȣ�����
	//Ϊ�˶��뷽ʽ������������Ҫ�����ʵ��Ĵ���
	//1��ͳ��������
	int allLineSum = 1;
	int curLineWidth = 0;	//ͳ�Ƶ�ǰ�еĿ��
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] == '\n') ++ allLineSum;	//����������
		else
		{
			if(allLineSum == 1) ++ curLineWidth; //ͳ�Ƶ�һ�еĿ��
		}
	}
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] < tempHead || m_string[i] > tempEnd)
		{//�����Ƿ����ַ�
			if(m_string[i] == '\n')
			{//����ǻ��з���ʼ����
				++ m_lineSum;
				wordsSum = -1;
				//ͳ�Ƶ�ǰ�еĿ��
				curLineWidth = 0;
				for(int j = i + 1;j < m_maxStringLen;++ j)
				{
					if(m_string[j] == '\0' || m_string[j] == '\n') break;
					if(m_string[j] >= tempHead && m_string[j] <= tempEnd) ++ curLineWidth;
				}
			}
			continue;
		}else
		{
			if(!m_isPassword)
			{//������ģʽ
				cur_x = ((m_string[i] - tempHead) % (int)(m_layout.x)) * m_size.x;
				if(m_layout.y == 0) 
				{
					cur_y = 0;
				}else
				{
					cur_y = ((m_string[i] - tempHead) / (int)(m_layout.x)) * m_size.y;
				}
			}else
			{//����ģʽ
				cur_x = (('*' - tempHead) % (int)(m_layout.x)) * m_size.x;
				if(m_layout.y == 0) 
				{
					cur_y = 0;
				}else
				{
					cur_y = (('*' - tempHead) / (int)(m_layout.x)) * m_size.y;
				}
			}
			++ wordsSum;
		}
		if(!m_isCliped)
		{
			m_textRect[m_needShowTextSum].set(cur_x,cur_y,cur_x + m_size.x,cur_y + m_size.y);
			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT://����룬ɶҲ������
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP://����룬ɶҲ������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * m_lineSum,
						m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE://���У�ÿ���������ư�����
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN://�Ҷ��룬ÿ�����������������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE://���У�ÿ���������ư�����
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP://����룬ɶҲ������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth * 0.5f) - m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * textPixelX * (wordsSum - curLineWidth * 0.5f) + m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE://���У�ÿ���������ư�����
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth * 0.5f) - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_angleSin * textPixelX * (wordsSum - curLineWidth * 0.5f) + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN://�Ҷ��룬ÿ�����������������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth * 0.5f) - m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_angleSin * textPixelX * (wordsSum - curLineWidth * 0.5f) + m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT://�Ҷ��룬ÿ�����������������
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP://����룬ɶҲ������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth) - m_angleSin * textPixelY * m_lineSum,
						m_angleSin * textPixelX * (wordsSum - curLineWidth) + m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE://���У�ÿ���������ư�����
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth) - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_angleSin * textPixelX * (wordsSum - curLineWidth) + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN://�Ҷ��룬ÿ�����������������
					m_textPosition[m_needShowTextSum] = m_position + XVec2(m_angleCos * textPixelX * (wordsSum - curLineWidth) - m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_angleSin * textPixelX * (wordsSum - curLineWidth) + m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			}
		//	m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * m_lineSum,
		//		m_position.y + m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * m_lineSum);
			++ m_needShowTextSum;
			if(textPixelX * (wordsSum + 1) > m_maxPixelWidth)
			{
				m_maxPixelWidth = textPixelX * (wordsSum + 1);
			}
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
			{
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
			}
		}else
		{
			int left,right,top,bottom;
			//������y������ж�
			float tempY = 0.0f;
			switch(m_alignmentModeY)
			{
			case FONT_ALIGNMENT_MODE_Y_UP://����룬ɶҲ������
				tempY = (m_size.y + m_distanceY) * m_lineSum;
				break;
			case FONT_ALIGNMENT_MODE_Y_MIDDLE://���У�ÿ���������ư�����
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum * 0.5f);
				break;
			case FONT_ALIGNMENT_MODE_Y_DOWN://�Ҷ��룬ÿ�����������������
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum);
				break;
			}
			//float tempY = (m_size.y + m_distanceY) * m_lineSum;	//������ߵľ���
			if(m_clipRect.bottom < tempY + m_distance) continue;//����֮�²���ʾ
			//if(m_clipRect.top >= tempY + (m_size.y + m_distanceY))
			if(m_clipRect.top >= tempY + m_size.y) continue;//����֮�ϲ���ʾ
			if(tempY >= m_clipRect.top && tempY + (m_size.y + m_distanceY) <= m_clipRect.bottom)
			{//��Ҫȫ���軭
				top = 0;
				bottom = 0;
			}else
			{
				if(tempY < m_clipRect.top) top = m_clipRect.top - tempY;
				else top = 0;

				if(tempY + (m_size.y + m_distanceY) > m_clipRect.bottom) bottom = tempY + (m_size.y + m_distanceY) - m_clipRect.bottom;
				else bottom = 0;
			}
			//������x������ж�
			float tempX = 0.0f;
			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT://����룬ɶҲ������
				tempX = (m_size.x + m_distance) * (float)wordsSum;
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE://���У�ÿ���������ư�����
				tempX = (m_size.x + m_distance) * (wordsSum - curLineWidth * 0.5f);
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT://�Ҷ��룬ÿ�����������������
				tempX = (m_size.x + m_distance) * (wordsSum - curLineWidth);
				break;
			}
			//float tempX = (m_size.x + m_distance) * (float)wordsSum;	//������ߵľ���
			if(tempX + m_distance >= m_clipRect.right) continue;	//�ַ�Χ���治��Ҫ������
			//if(tempX + (m_size.x + m_distance) <= m_clipRect.left)
			if(tempX + m_size.x <= m_clipRect.left) continue;
			if(tempX >= m_clipRect.left && tempX + (m_size.x + m_distance) <= m_clipRect.right)
			{//��Ҫȫ���軭
				left = 0;
				right = 0;
			}else
			{
				if(tempX < m_clipRect.left) left = m_clipRect.left - tempX;
				else left = 0;

				if(tempX + (m_size.x + m_distance) > m_clipRect.right) right = tempX + (m_size.x + m_distance) - m_clipRect.right;
				else right = 0;
			}
			m_textRect[m_needShowTextSum].set(cur_x + left,cur_y + top,cur_x + m_size.x - right,cur_y + m_size.y - bottom);

			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelX * wordsSum));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelX * wordsSum));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelX * wordsSum));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth * 0.5f)));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelX * (wordsSum - curLineWidth)));
					break;
				}
				break;
			}

			//m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
			//	- m_angleSin * textPixelY * m_lineSum
			//	+ m_angleCos * (left * m_scale.x + textPixelX * wordsSum),
			//	m_position.y + m_angleCos * top * m_scale.x 
			//	+ m_angleCos * textPixelY * m_lineSum
			//	+ m_angleSin * (left * m_scale.x + textPixelX * wordsSum));
			++ m_needShowTextSum;
			if(textPixelX * (wordsSum + 1) > m_maxPixelWidth)
			{
				m_maxPixelWidth = textPixelX * (wordsSum + 1);
			}
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
			{
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
			}
		}
	}
}
XBool XFont::setACopy(const XFont &temp)
{
	if(this == &temp ||		//��ֹ��������
		!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ����ֱ���˳�

	if(!XFontBasic::setACopy(temp)) return XFalse;
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif
		
	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;

	m_layout = temp.m_layout;		//����ͼƬ�Ĳ���
	return XTrue;
}
XFont& XFont::operator = (const XFont& temp)
{
	if(&temp == this ||	//��ֹû��Ҫ�����ҿ���
		!temp.m_isInited) return * this;	//���Ŀ��û�г�ʼ����ֱ���˳�

	XFontBasic::operator =(temp);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif

	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;

	m_layout = temp.m_layout;		//����ͼƬ�Ĳ���

	return * this;
}
XFont::XFont(const XFont &temp)
:XFontBasic(temp)
{
	if(this == &temp) return;		//��ֹ��������
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.addAObject(this);
#endif	

	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;

	m_layout = temp.m_layout;			//����ͼƬ�Ĳ���
}
#if !WITH_INLINE_FILE
#include "XFont.inl"
#endif
}