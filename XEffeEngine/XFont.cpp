//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFont.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XBasicSDL.h"
#include "XObjectManager.h"

_XFont::_XFont()
:m_isCliped(0)
,m_clipRect(0,0,0,0)
,m_isPassword(0)
,m_isVisiable(1)
{
	m_textPosition = new _XVector2[STRING_MAX_SIZE];
	m_textRect = new _XRect[STRING_MAX_SIZE];
}

_XFont::~_XFont()
{
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	m_sprite.release();
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

int _XFont::init(const char *filename,const _XVector2I& size,const _XVector2I& layout,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The font have initted!\n");
		return 1;
	}
	if(filename == NULL) return 0;

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	if(m_sprite.init(filename,m_resoursePosition,POINT_LEFT_TOP) == 0) return 0;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTX);
#endif

	m_position.set(0,0);
	m_setPosition.set(0,0);
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = 0;

	m_isCliped = 0;
	
	m_isInited = 1;			
	return 1;
}

int _XFont::init(const char *filename,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The font have initted!\n");
		return 1;
	}
	if(filename == NULL) return 0;

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	if(m_sprite.init(filename,m_resoursePosition,POINT_LEFT_TOP) == 0) return 0;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTX);
#endif
	//下面从文件中读取数据
	char filenameTemp[MAX_FONT_UNICODE_FILE_NAME_LENGTH];
	strcpy(filenameTemp,filename);
	int filenameLength = strlen(filename);
	filenameTemp[filenameLength - 3] = 'd';
	filenameTemp[filenameLength - 2] = 'a';
	filenameTemp[filenameLength - 1] = 't';
	FILE *fp = NULL;
	_XVector2I size(0,0);
	_XVector2I layout(0,0);
	if(m_resoursePosition == 0)
	{
		if((fp = fopen(filenameTemp,"r")) == NULL)
		{//文件单开失败
			return 0;
		}
		fscanf(fp,"%d,%d,%d,%d,",&size.x,&size.y,&layout.x,&layout.y);
		fclose(fp);
	}else
	{
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(filenameTemp);
		unsigned char *p = NULL;
		if((p = createArrayMem<unsigned char>(lengthTemp + 1)) == NULL)
		{
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(filenameTemp,p);
		*(p + lengthTemp) = '\0';
		sscanf((char *)p,"%d,%d,%d,%d,",&size.x,&size.y,&layout.x,&layout.y);
		XDELETE_ARRAY(p);
	}

	m_position.set(0,0);
	m_setPosition.set(0,0);
	m_angle = 0;			
	m_distance = 0;;		
	m_size = size;			
	m_layout = layout;
	m_showSize.x = 1.0f;
	m_showSize.y = 1.0f;
	m_sprite.setSize(m_showSize);
	m_isPassword = 0;

	m_isCliped = 0;
	
	m_isInited = 1;			
	return 1;
}

void _XFont::draw()
{
	if(m_isVisiable == 0) return;
	if(m_isCliped != 0 && m_clipRect.bottom <= 0)
	{//这种情况什么也不用画
		return;
	}
	if(m_needUpdateData != 0)
	{
		updateData();
		m_needUpdateData = 0;
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);
		m_sprite.draw();
	}
}

void _XFont::updateData()
{
	m_needShowTextSum = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	int now_x = 0;
	int now_y = 0;
	m_lineSum = 0;
	char tempHead = ' ';
	char tempEnd = '}';
	int wordsSum = -1;
	float textPixelX = (m_size.x + m_distance) * m_showSize.x;	//一个字符的宽度，像素
	float textPixelY = (m_size.y + m_distanceY) * m_showSize.y;	//一个字符的高度，像素
	//为了对齐方式的引入这里需要先做适当的处理
	//1、统计总行数
	int allLineSum = 1;
	int nowLineWidth = 0;	//统计当前行的宽度
	for(int i =0;i < STRING_MAX_SIZE;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] == '\n') allLineSum ++;	//这是总行数
		else
		{
			if(allLineSum == 1) nowLineWidth ++; //统计第一行的宽度
		}
	}
	for(int i =0;i < STRING_MAX_SIZE;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] < tempHead || m_string[i] > tempEnd)
		{//遇到非法的字符
			if(m_string[i] == '\n')
			{//如果是换行符则开始换行
				m_lineSum ++;
				wordsSum = -1;
				//统计当前行的宽度
				nowLineWidth = 0;
				for(int j = i + 1;j < STRING_MAX_SIZE;++ j)
				{
					if(m_string[j] == '\0' || m_string[j] == '\n') break;
					if(m_string[j] >= tempHead && m_string[j] <= tempEnd) nowLineWidth ++;
				}
			}
			continue;
		}else
		{
			if(m_isPassword == 0)
			{//非密码模式
				now_x = ((m_string[i] - tempHead) % m_layout.x) * m_size.x;
				if(m_layout.y == 0) 
				{
					now_y = 0;
				}else
				{
					now_y = ((m_string[i] - tempHead) / m_layout.x) * m_size.y;
				}
			}else
			{//密码模式
				now_x = (('*' - tempHead) % m_layout.x) * m_size.x;
				if(m_layout.y == 0) 
				{
					now_y = 0;
				}else
				{
					now_y = (('*' - tempHead) / m_layout.x) * m_size.y;
				}
			}
			wordsSum ++;
		}
		if(m_isCliped == 0)
		{
			m_textRect[m_needShowTextSum].set(now_x,now_y,now_x + m_size.x,now_y + m_size.y);
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * m_lineSum;
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * m_lineSum;
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * (m_lineSum - allLineSum);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * (m_lineSum - allLineSum);
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//居中，每个文字左移半个宽度
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth * 0.5f) - m_angleSin * textPixelY * m_lineSum;
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth * 0.5f) + m_angleCos * textPixelY * m_lineSum;
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth * 0.5f) - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth * 0.5f) + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth * 0.5f) - m_angleSin * textPixelY * (m_lineSum - allLineSum);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth * 0.5f) + m_angleCos * textPixelY * (m_lineSum - allLineSum);
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
			{//右对齐，每个文字左移整个宽度
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth) - m_angleSin * textPixelY * m_lineSum;
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth) + m_angleCos * textPixelY * m_lineSum;
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth) - m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth) + m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f);
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].x = m_position.x + m_angleCos * textPixelX * (wordsSum - nowLineWidth) - m_angleSin * textPixelY * (m_lineSum - allLineSum);
					m_textPosition[m_needShowTextSum].y = m_position.y + m_angleSin * textPixelX * (wordsSum - nowLineWidth) + m_angleCos * textPixelY * (m_lineSum - allLineSum);
				}
			}
		//	m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * textPixelX * wordsSum - m_angleSin * textPixelY * m_lineSum,
		//		m_position.y + m_angleSin * textPixelX * wordsSum + m_angleCos * textPixelY * m_lineSum);
			m_needShowTextSum ++;
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
			//下面是y方向的判断
			float tempY = 0.0f;
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
			{//左对齐，啥也不用做
				tempY = (m_size.y + m_distanceY) * m_lineSum;
			}else
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
			{//居中，每个文字左移半个宽度
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum * 0.5f);
			}else
			if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
			{//右对齐，每个文字左移整个宽度
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum);
			}
			//float tempY = (m_size.y + m_distanceY) * m_lineSum;	//字体左边的距离
			if(m_clipRect.bottom < tempY + m_distance)
			{//下线之下不显示
				continue;
			}
			//if(m_clipRect.top >= tempY + (m_size.y + m_distanceY))
			if(m_clipRect.top >= tempY + m_size.y)
			{//上线之上不显示
				continue;
			}
			if(tempY >= m_clipRect.top && tempY + (m_size.y + m_distanceY) <= m_clipRect.bottom)
			{//需要全部描画
				top = 0;
				bottom = 0;
			}else
			{
				if(tempY < m_clipRect.top)
				{
					top = m_clipRect.top - tempY;
				}else
				{
					top = 0;
				}
				if(tempY + (m_size.y + m_distanceY) > m_clipRect.bottom)
				{
					bottom = tempY + (m_size.y + m_distanceY) - m_clipRect.bottom;
				}else
				{
					bottom = 0;
				}
			}
			//下面是x方向的判断
			float tempX = 0.0f;
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				tempX = (m_size.x + m_distance) * (float)wordsSum;
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//居中，每个文字左移半个宽度
				tempX = (m_size.x + m_distance) * (wordsSum - nowLineWidth * 0.5f);
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
			{//右对齐，每个文字左移整个宽度
				tempX = (m_size.x + m_distance) * (wordsSum - nowLineWidth);
			}
			//float tempX = (m_size.x + m_distance) * (float)wordsSum;	//字体左边的距离
			if(tempX + m_distance >= m_clipRect.right) 
			{
				continue;	//字范围外面不需要画出来
			}
			//if(tempX + (m_size.x + m_distance) <= m_clipRect.left)
			if(tempX + m_size.x <= m_clipRect.left)
			{
				continue;
			}
			if(tempX >= m_clipRect.left && tempX + (m_size.x + m_distance) <= m_clipRect.right)
			{//需要全部描画
				left = 0;
				right = 0;
			}else
			{
				if(tempX < m_clipRect.left)
				{
					left = m_clipRect.left - tempX;
				}else
				{
					left = 0;
				}
				if(tempX + (m_size.x + m_distance) > m_clipRect.right)
				{
					right = tempX + (m_size.x + m_distance) - m_clipRect.right;
				}else
				{
					right = 0;
				}
			}
			m_textRect[m_needShowTextSum].set(now_x + left,now_y + top,now_x + m_size.x - right,now_y + m_size.y - bottom);

			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
			{//左对齐，啥也不用做
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelX * wordsSum));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelX * wordsSum));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelX * wordsSum),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelX * wordsSum));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
			{//居中，每个文字左移半个宽度
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth * 0.5f)));
				}
			}else
			if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
			{//右对齐，每个文字左移整个宽度
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
				{//左对齐，啥也不用做
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
				{//居中，每个文字左移半个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)));
				}else
				if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
				{//右对齐，每个文字左移整个宽度
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)),
						m_position.y + m_angleCos * top * m_showSize.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_showSize.x + textPixelX * (wordsSum - nowLineWidth)));
				}
			}

			//m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_showSize.y 
			//	- m_angleSin * textPixelY * m_lineSum
			//	+ m_angleCos * (left * m_showSize.x + textPixelX * wordsSum),
			//	m_position.y + m_angleCos * top * m_showSize.x 
			//	+ m_angleCos * textPixelY * m_lineSum
			//	+ m_angleSin * (left * m_showSize.x + textPixelX * wordsSum));
			m_needShowTextSum ++;
		}
	}
}

int _XFont::setACopy(const _XFont &temp)
{
	if(this == &temp) return 0;		//防止自生拷贝
	if(temp.m_isInited == 0) return 0;	//如果目标没有初始化则直接退出

	if(_XFontBasic::setACopy(temp) == 0) return 0;
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTX);
#endif

	m_isVisiable = temp.m_isVisiable;		
	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}

	m_layout = temp.m_layout;		//字体图片的布局
	m_isPassword = temp.m_isPassword;			//是否已经初始化
	return 1;
}

_XFont& _XFont::operator = (const _XFont& temp)
{
	if(&temp == this) return * this;	//防止没必要的自我拷贝
	if(temp.m_isInited == 0) return * this;	//如果目标没有初始化则直接退出

	_XFontBasic::operator =(temp);
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTX);
#endif

	m_isVisiable = temp.m_isVisiable;		
	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}
	m_layout = temp.m_layout;		//字体图片的布局
	m_isPassword = temp.m_isPassword;			//是否已经初始化

	return * this;
}

_XFont::_XFont(const _XFont &temp)
:_XFontBasic(temp)
{
	if(this == &temp) return;		//防止自生拷贝
#if WITH_OBJECT_MANAGER	//在物件管理的类中注销这些物件
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
	_XObjectManager::GetInstance().addAObject(this,OBJ_FONTX);
#endif
	m_isVisiable = temp.m_isVisiable;		
	m_isCliped = temp.m_isCliped;		
	m_clipRect = temp.m_clipRect;		

	for(int i = 0;i < STRING_MAX_SIZE;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}
	m_layout = temp.m_layout;			//字体图片的布局
	m_isPassword = temp.m_isPassword;	//是否已经初始化
}