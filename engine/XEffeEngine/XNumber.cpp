//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
#include "XNumber.h"
#include "XResourcePack.h"
#include "XBasicWindow.h"
#include "XObjectManager.h"

_XNumber::_XNumber()
:m_isInited(XFalse)
,m_rotateBasicPoint(0.0f,0.0f)
,m_angleSin(0.0f)
,m_angleCos(1.0f)
,m_needUpdateData(XFalse)
,m_needShowTextSum(0)
,m_maxPixelWidth(0)
,m_maxPixelHeight(0)
,m_alignmentMode(NUMBER_ALIGNMENT_MODE_LEFT)
,m_isVisible(XTrue)
{//�ַ��������ʱ��ʹ�õ��򱣴棬���Ƿ���ʹ��������˳�����磺-12.5%�����ᱣ��Ϊ-5.12%
	m_string = createArrayMem<char>(MAX_NUMBER_LENGTH);
	m_string[0] = '\0';
	m_textPosition = createArrayMem<_XVector2>(MAX_NUMBER_LENGTH);
	m_textRect = createArrayMem<_XRect>(MAX_NUMBER_LENGTH);
	m_objType = OBJ_NUMBER;
}
_XNumber::~_XNumber()
{
	release();
	m_isInited = XFalse;
}
_XBool _XNumber::release()
{
	if(!m_isInited) return XFalse;
	XDELETE_ARRAY(m_string);
	XDELETE_ARRAY(m_textPosition);
	XDELETE_ARRAY(m_textRect);
	m_sprite.release();
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = XFalse;
	return XTrue;
}
_XBool _XNumber::init(const char *fileName,const _XVector2 &size,const _XVector2 &layout,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	m_position.set(0.0f,0.0f);
	m_setPosition.set(0.0f,0.0f);
	setAngle(0);
	m_alpha = 1.0f;
	m_distance = 0;
	m_size = size;
	if(layout.x <= 0 || layout.y <= 0) return XFalse;
	m_layout = layout;
	m_showSize.set(1.0,1.0);
	if(!m_sprite.init(fileName,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(&m_sprite);
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
//��δ���
_XBool _XNumber::initEx(const char *fileName,	//����ͼƬ������
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(fileName == NULL) return XFalse;

	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	m_position.set(0.0f,0.0f);
	m_setPosition.set(0.0f,0.0f);
	setAngle(0);
	m_alpha = 1.0f;
	m_distance = 0;

	//������ļ��ж�ȡ����
	_XVector2 size;
	_XVector2 layout;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	strcpy(tempFilename,fileName);
	int len = strlen(tempFilename);
	tempFilename[len - 3] = 't';
	tempFilename[len - 2] = 'x';
	tempFilename[len - 1] = 't';
	if(m_resoursePosition == RESOURCE_LOCAL_FOLDER)
	{
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == NULL) return XFalse;
		int x,y;
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		size.set(x,y);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		layout.set(x,y);
		fclose(fp);
	}else
	if(m_resoursePosition == RESOURCE_LOCAL_PACK)
	{
		unsigned char *p  = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p != NULL)
		{
			int x,y,a,b;
			if(sscanf((char *)p,"%d,%d,%d,%d",&x,&y,&a,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
			size.set(x,y);
			layout.set(a,b);
			XDELETE(p);
		}else
		{
			return XFalse;
		}
	}

	m_size = size;
	if(layout.x <= 0 || layout.y <= 0) return XFalse;
	m_layout = layout;

	m_showSize.set(1.0,1.0);
	if(!m_sprite.init(fileName,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(&m_sprite);
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XNumber::initFromTTF(const char *filename,	//ttf����������
		int fontSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;

	m_resoursePosition = resoursePosition;

	m_string[0] = '\0';
	m_position.set(0.0f,0.0f);
	m_setPosition.set(0.0f,0.0f);
	setAngle(0);
	m_alpha = 1.0f;
	m_distance = 0;

	int fontOrder = _XFontTTF::GetInstance().loadTTFFile(filename,fontSize,m_resoursePosition);
	if(fontOrder < 0) return XFalse;
	unsigned int tex;
	_XVector2 layout;
	if(!_XFontTTF::GetInstance().getTextureNumber(fontOrder,tex,layout)) return XFalse;
	//�ͷ���Դ
	_XFontTTF::GetInstance().releaseTTFFile(fontOrder);
	int w = getMinWellSize2n(layout.x * fontSize * 0.5f);
	int h = getMinWellSize2n(layout.y * fontSize);
	m_sprite.init(w,h,COLOR_RGBA,tex,POINT_LEFT_TOP);

	m_size.set(fontSize * 0.5f,fontSize);
	if(layout.x <= 0 || layout.y <= 0) return XFalse;
	m_layout = layout;

	m_showSize.set(1.0,1.0);
	//if(!m_sprite.init(fileName,m_resoursePosition,POINT_LEFT_TOP)) return XFalse;
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(&m_sprite);
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
void _XNumber::draw()
{
	if(!m_isVisible ||
		!m_isInited) return;
	if(m_needUpdateData)
	{
		m_needUpdateData = XFalse;
		updateData();
	}
	for(int i = 0;i < m_needShowTextSum;++ i)
	{
		m_sprite.setClipRect(m_textRect[i]);
		m_sprite.setPosition(m_textPosition[i]);
		m_sprite.draw();
	}
}
void _XNumber::updateData()
{
	m_needShowTextSum = 0;
	int now_x = 0;
	int now_y = 0;
	m_maxPixelWidth = 0;
	m_maxPixelHeight = 0;
	float textPixelX = (m_size.x + m_distance) * m_showSize.x;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		if(m_string[i] == '\0') break;
		if(m_string[i] < '0' || m_string[i] > '9')
		{//�ַ�
			switch(m_string[i])
			{
			case '+':
				now_x = 10;
				break;
			case '-':
				now_x = 11;
				break;
			case '*':
				now_x = 12;
				break;
			case '/':
				now_x = 13;
				break;
			case '%':
				now_x = 14;
				break;
			case '.':
				now_x = 15;
				break;
			default:
				continue;
				break;
			}
			if(m_layout.y == 0) 
			{
				now_y = 0;
			}else
			{
				now_y = (now_x / (int)(m_layout.x)) * m_size.y;
			}
			now_x = (now_x % (int)(m_layout.x)) * m_size.x;
		}else
		{//����
			now_x = ((m_string[i] - '0') % (int)(m_layout.x)) * m_size.x;
			if(m_layout.y == 0) 
			{
				now_y = 0;
			}else
			{
				now_y = ((m_string[i] - '0') / (int)(m_layout.x)) * m_size.y;
			}
		}

		m_textRect[m_needShowTextSum].set(now_x,now_y,now_x + m_size.x,now_y + m_size.y);
		m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * textPixelX * i,
			m_position.y + m_angleSin * textPixelX * i);
		m_needShowTextSum ++;

		if(textPixelX * (i + 1) > m_maxPixelWidth)
		{
			m_maxPixelWidth = textPixelX * (i + 1);
		}
	}
	m_maxPixelHeight = m_size.y * m_showSize.y;
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_LEFT)
	{//����룬ɶҲ������
	}else
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_MIDDLE)
	{//���У�ÿ���������ư�����
		for(int i = 0;i < m_needShowTextSum;++ i)
		{
			m_textPosition[i].x -= m_maxPixelWidth * 0.5f * m_angleCos;
			m_textPosition[i].y -= m_maxPixelWidth * 0.5f * m_angleSin;
		}
	}else
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_RIGHT)
	{//�Ҷ��룬ÿ�����������������
		for(int i = 0;i < m_needShowTextSum;++ i)
		{
			m_textPosition[i].x -= m_maxPixelWidth * m_angleCos;
			m_textPosition[i].y -= m_maxPixelWidth * m_angleSin;
		}
	}
}
_XBool _XNumber::setNumber(int temp)
{
	char tempNumber[MAX_NUMBER_LENGTH];
	int nowCharPoint = 0;
	if(temp < 0) 
	{
		tempNumber[nowCharPoint] = '-';
		++ nowCharPoint;
		temp = -temp;
	}
	while(true)
	{
		tempNumber[nowCharPoint] = temp%10 + '0';
		temp = temp / 10.0f;
		++ nowCharPoint;
		if(nowCharPoint >= MAX_NUMBER_LENGTH) return XFalse;
		if(temp == 0) break;
	}
	int i = 0;;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//����Ƿ���λ�򲻱�
		m_string[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_string[nowCharPoint] = '\0';

	m_needUpdateData = XTrue;
	return XTrue;
}
_XBool _XNumber::setNumber(float temp,int decimalPartLength)
{
	char tempNumber[MAX_NUMBER_LENGTH];
	if(decimalPartLength < 0) decimalPartLength = 0;
	int tempZ = 0;
	float tempBack = temp;
	for(int i = 0;i < decimalPartLength;++ i)
	{
		tempBack *= 10.0;
	}
	tempZ = tempBack;
	int nowCharPoint = 0;
	if(tempZ < 0) 
	{
		tempNumber[nowCharPoint] = '-';
		++ nowCharPoint;
		tempZ = -tempZ;
	}

	//������������������Ϊ0����Ҫ��������ַ�����ǰ���һ��0
	char flag = 0;
	if(temp < 1.0 && temp > -1.0 && tempZ != 0)
	{//�������������0������Ҫ������
		flag = 1;
	}

	int sumLength = 0;
	while(true)
	{
		tempNumber[nowCharPoint] = tempZ%10 + '0';
		tempZ = tempZ * 0.1f;// / 10.0f;
		++ nowCharPoint;
		++ sumLength;
		if(nowCharPoint >= MAX_NUMBER_LENGTH) return XFalse;
		if(sumLength == decimalPartLength && decimalPartLength > 0)
		{
			tempNumber[nowCharPoint] = '.';
			++ nowCharPoint;
		}
		if(tempZ == 0) break;
	}

	//������������������Ϊ0����Ҫ��������ַ�����ǰ���һ��0
	if(flag == 1)
	{
		tempNumber[nowCharPoint] = '0';
		++ nowCharPoint;
	}

	int i = 0;
	if(tempNumber[0] < '0' || tempNumber[0] > '9')
	{//����Ƿ���λ�򲻱�
		m_string[0] = tempNumber[0];
		for(i = 1;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i];
		}
	}else
	{
		for(i = 0;i < nowCharPoint;++i)
		{
			m_string[i] = tempNumber[nowCharPoint - i - 1];
		}
	}
	m_string[nowCharPoint] = '\0';

	m_needUpdateData = XTrue;
	return XTrue;
}
_XBool _XNumber::setACopy(const _XNumber &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ����ֱ���˳�
	//if(m_isInited != 0) 
	//{
	//	m_sprite.release()
	//	//return XFalse;	//��������Ѿ���ʼ�������Ҫ�˳������򽫻�����ڴ�й¶
	//}
	m_isVisible = temp.m_isVisible;
	m_alignmentMode = temp.m_alignmentMode;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;
	m_needUpdateData = temp.m_needUpdateData;
	m_needShowTextSum = temp.m_needShowTextSum;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}

	strcpy(m_string,temp.m_string);
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_alpha = temp.m_alpha;
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_layout = temp.m_layout;		//����ͼƬ�Ĳ���
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//��ͼ
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(&m_sprite);
	_XObjManger.addAObject(this);	//addAObject�ڲ��Ѿ��������ظ�ע������⣬�ⲿ��ʱ�������������
#endif
	m_isInited = temp.m_isInited;
	return XTrue;
}
_XNumber& _XNumber::operator = (const _XNumber& temp)
{
	if(& temp == this) return * this;	//��ֹ����ֵ
	if(!temp.m_isInited) return * this;	//���Ŀ��û�г�ʼ����ֱ���˳�
	//if(m_isInited != 0) 
	//{
	//	m_sprite.release()
	//	//return XFalse;	//��������Ѿ���ʼ�������Ҫ�˳������򽫻�����ڴ�й¶
	//}
	m_isVisible = temp.m_isVisible;
	m_alignmentMode = temp.m_alignmentMode;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;
	m_needUpdateData = temp.m_needUpdateData;
	m_needShowTextSum = temp.m_needShowTextSum;
	for(int i = 0;i < MAX_NUMBER_LENGTH;++ i)
	{
		m_textPosition[i] = temp.m_textPosition[i];
		m_textRect[i] = temp.m_textRect[i];
	}

	strcpy(m_string,temp.m_string);
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_alpha = temp.m_alpha;
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_layout = temp.m_layout;		//����ͼƬ�Ĳ���
	m_showSize = temp.m_showSize;
	m_rotateBasicPoint = temp.m_rotateBasicPoint;
	m_sprite.setACopy(temp.m_sprite);					//��ͼ
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	_XObjManger.decreaseAObject(&m_sprite);
	_XObjManger.addAObject(this);	//addAObject�ڲ��Ѿ��������ظ�ע������⣬�ⲿ��ʱ�������������
#endif
	m_isInited = temp.m_isInited;

	return * this;
}
_XVector2 _XNumber::getBox(int order)
{//��ʱ�����ǽǶ�
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_LEFT)
	{
		switch(order)
		{
		case 0: return m_setPosition;
		case 1: return _XVector2(m_setPosition.x + m_maxPixelWidth * m_angleCos,
			m_setPosition.y + m_maxPixelWidth * m_angleSin); 
		case 2: return _XVector2(m_setPosition.x + m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos); 
		case 3: return _XVector2(m_setPosition.x - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelHeight * m_angleCos);
		}
	}else
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_MIDDLE)
	{
		switch(order)
		{
		case 0: return _XVector2(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos,
			m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin);
		case 1: return _XVector2(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos,
			m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin);
		case 2: return _XVector2(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
		case 3: return _XVector2(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
		}
	}else
	if(m_alignmentMode == NUMBER_ALIGNMENT_MODE_RIGHT)
	{
		switch(order)
		{
		case 0: return _XVector2(m_setPosition.x - m_maxPixelWidth * m_angleCos,
			m_setPosition.y - m_maxPixelWidth * m_angleSin);
		case 1: return m_setPosition;
		case 2: return _XVector2(m_setPosition.x - m_maxPixelHeight * m_angleSin,
			m_setPosition.y + m_maxPixelHeight * m_angleCos);
		case 3: return _XVector2(m_setPosition.x - m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
			m_setPosition.y - m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos);
		}
	}
	return _XVector2::zero;
}