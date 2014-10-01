//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"	//������Ҫʹ��shader������Ҫ����ļ���������Ҫ����
#include "XSprite.h"
#include "XBasicWindow.h"
#include "XBasicOpenGL.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XTextureInformation.h"
#include "XObjectManager.h"

#if WITH_XSPRITE_EX
_XBool _XSprite::init(const char * filename,					//�ļ���
		_XResourcePosition resoursePosition,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		const _XVector2 &changePoint)
{
	if(m_isInited ||
		filename == NULL) return XFalse; 
	m_resoursePosition = resoursePosition;
	if(!m_textureData.load(filename,m_resoursePosition)) return XFalse;

	m_turnOverMode = TURN_OVER_MODE_NULL;	//��תģʽ
	m_position.set(0.0f,0.0f);			//λ��
	m_size.set(1.0f,1.0f);				//���ųߴ�
	m_pixelSize = m_textureData.textureSize;			//���سߴ�
	m_angle = 0.0f;					//�Ƕ�
	m_changeCenter = changePoint;		//��ת�������ŵ�����
//	if(m_textureData.isEnableInsideClip != 0)
//	{//�����Ҫ�ڲ��ü�
//		setClipRect(_XRect(0.0f,0.0f,m_textureData.clipInsideRect.getWidth(),m_textureData.clipInsideRect.getHeight()));				//�ü�������
//	}else
//	{//�������Ҫ�ڲ��ü�
//		m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
//		m_clipRect.set(0.0f,0.0f,1.0f,1.0f);				//�ü�������
//	}
	m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
	m_clipRect.set(0.0f,0.0f,1.0f,1.0f);

	//������Ϊ�˱��ڼ������ʱ����
	m_sinAngle = 0.0f;	//sin(m_angle * DEGREE2RADIAN);				//�Ƕȵ���ʱ����
	m_cosAngle = 1.0f;	//cos(m_angle * DEGREE2RADIAN);

	m_needUpdateData = XTrue;			//�Ƿ���Ҫ�����ڲ�����
	updateData();
	m_isVisible = XTrue;
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XSprite::init(int tempW,int tempH,int needSizeCheck,const _XVector2 &changePoint)
{
	if(m_isInited) return XTrue;

	m_turnOverMode = TURN_OVER_MODE_NULL;	//��תģʽ
	m_position.set(0.0f,0.0f);			//λ��
	m_size.set(1.0f,1.0f);				//���ųߴ�

	//������Ҫ�鹹��ͼ����
	if(!m_textureData.loadEmpty()) return XFalse;
	m_pixelSize.set(tempW,tempH);
	if(needSizeCheck != 0 && isNPOT(tempW,tempH))
	{
		int w = (int)powf(2.0, ceilf(logf((float)tempW)/logf(2.0f)));
		int h = (int)powf(2.0, ceilf(logf((float)tempH)/logf(2.0f)));
		//����ͨ�������ڲ��ü��ķ�ʽ������������
		m_textureData.textureSize = m_pixelSize;
		m_textureData.texture.m_w = w;
		m_textureData.texture.m_h = h;

		m_textureData.isEnableInsideClip = 1;
		m_textureData.clipInsideRect.left = (w - tempW) * 0.5f;
		m_textureData.clipInsideRect.right = m_textureData.clipInsideRect.left + tempW;
		m_textureData.clipInsideRect.top = (h - tempH) * 0.5f;
		m_textureData.clipInsideRect.bottom = m_textureData.clipInsideRect.top + tempH;
		m_textureData.textureMove.set(0.0f,0.0f);
		m_textureData.textureMove2 = m_textureData.textureMove;
		//m_textureData.textureMove.set(m_textureData.clipInsideRect.left,m_textureData.clipInsideRect.top);
		//m_textureData.textureMove2 = m_textureData.textureMove;
	}else
	{
		m_textureData.texture.m_w = tempW;
		m_textureData.texture.m_h = tempH;
	}

	m_angle = 0.0f;					//�Ƕ�
	m_changeCenter = changePoint;		//��ת�������ŵ�����
	m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
	m_clipRect.set(0.0f,0.0f,1.0f,1.0f);				//�ü�������
	//������Ϊ�˱��ڼ������ʱ����
	m_sinAngle = 0.0f;//sin(m_angle * DEGREE2RADIAN);				//�Ƕȵ���ʱ����
	m_cosAngle = 1.0f;//cos(m_angle * DEGREE2RADIAN);

	m_needUpdateData = XTrue;			//�Ƿ���Ҫ�����ڲ�����
	updateData();
	m_isVisible = XTrue;
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XSprite::init(_XTextureData & texData,const _XVector2 &changePoint)
{
	if(m_isInited) return XTrue;
	//m_resoursePosition = resoursePosition;
	m_textureData = texData;

	m_turnOverMode = TURN_OVER_MODE_NULL;	//��תģʽ
	m_position.set(0.0f,0.0f);			//λ��
	m_size.set(1.0f,1.0f);				//���ųߴ�
	m_pixelSize = m_textureData.textureSize;			//���سߴ�
	m_angle = 0.0f;					//�Ƕ�
	m_changeCenter = changePoint;		//��ת�������ŵ�����
//	if(m_textureData.isEnableInsideClip != 0)
//	{//�����Ҫ�ڲ��ü�
//		setClipRect(_XRect(0.0f,0.0f,m_textureData.clipInsideRect.getWidth(),m_textureData.clipInsideRect.getHeight()));				//�ü�������
//	}else
//	{//�������Ҫ�ڲ��ü�
//		m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
//		m_clipRect.set(0.0f,0.0f,1.0f,1.0f);				//�ü�������
//	}
	m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
	m_clipRect.set(0.0f,0.0f,1.0f,1.0f);

	//������Ϊ�˱��ڼ������ʱ����
	m_sinAngle = 0.0f;//sin(m_angle * DEGREE2RADIAN);				//�Ƕȵ���ʱ����
	m_cosAngle = 1.0f;//cos(m_angle * DEGREE2RADIAN);

	m_needUpdateData = XTrue;			//�Ƿ���Ҫ�����ڲ�����
	updateData();
	m_isVisible = XTrue;
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XSprite::init(int w,int h,_XColorMode colorMode,unsigned int tex,const _XVector2 &changePoint)
{
	if(m_isInited) return XTrue;
	//m_resoursePosition = resoursePosition;
	if(!m_textureData.createWithTexture(w,h,colorMode,tex)) return XFalse;

	m_turnOverMode = TURN_OVER_MODE_NULL;	//��תģʽ
	m_position.set(0.0f,0.0f);			//λ��
	m_size.set(1.0f,1.0f);				//���ųߴ�
	m_pixelSize = m_textureData.textureSize;			//���سߴ�
	m_angle = 0.0f;					//�Ƕ�
	m_changeCenter = changePoint;		//��ת�������ŵ�����
//	if(m_textureData.isEnableInsideClip != 0)
//	{//�����Ҫ�ڲ��ü�
//		setClipRect(_XRect(0.0f,0.0f,m_textureData.clipInsideRect.getWidth(),m_textureData.clipInsideRect.getHeight()));				//�ü�������
//	}else
//	{//�������Ҫ�ڲ��ü�
//		m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
//		m_clipRect.set(0.0f,0.0f,1.0f,1.0f);				//�ü�������
//	}
	m_needClip = XFalse;				//�Ƿ���Ҫ�ü�
	m_clipRect.set(0.0f,0.0f,1.0f,1.0f);

	//������Ϊ�˱��ڼ������ʱ����
	m_sinAngle = 0.0f;//sin(m_angle * DEGREE2RADIAN);				//�Ƕȵ���ʱ����
	m_cosAngle = 1.0f;//cos(m_angle * DEGREE2RADIAN);

	m_needUpdateData = XTrue;			//�Ƿ���Ҫ�����ڲ�����
	updateData();
	m_isVisible = XTrue;
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
//void _XSprite::updateData()
//{
//	if(!m_needUpdateData) return ;
//	m_needUpdateData = XFalse;
//	if(m_textureData.isEnableInsideClip == 0)
//	{//��ʹ���ڲ���Դ����
//		//�����ǽǶȲü���ʵ��
//		float vw = 1.0f;
//		float vh = 1.0f;
//		float uw = 1.0f;
//		float uh = 1.0f;
//		_XRect tempClipRect;
//		tempClipRect.set(0.0f,0.0f,1.0f,1.0f);
//		if(m_needClip)
//		{//�����Ҫ�ⲿ�ü�
//			tempClipRect.set(m_clipRect.left / m_pixelSize.x,m_clipRect.top / m_pixelSize.y,m_clipRect.right / m_pixelSize.x,m_clipRect.bottom / m_pixelSize.y);
//		}
//		//v�ü�x
//		if(m_needClip)
//		{
//			uw = tempClipRect.getWidth();
//			uh = tempClipRect.getHeight();
//			vw = m_clipRect.getWidth();
//			vh = m_clipRect.getHeight();
//		}else
//		{
//			vw = m_pixelSize.x;
//			vh = m_pixelSize.y;
//			uw = 1.0f;
//			uh = 1.0f;
//		}
//		if(m_needAngleClip != 0)
//		{
//			_XRect uRect(0.0f,0.0f,uw,uh);
//			_XRect vRect(0.0f,0.0f,vw,vh);
//			_XVector2 cPoint = vRect.getCenter();
//		//	_XVector2 uPoint[7];
//		//	_XVector2 vPoint[7];
//		//	m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,vPoint,uPoint);
//		//	for(int i = 0;i < m_pointSum;++ i)
//		//	{
//		//		m_vPointer[i << 1]			= vPoint[i].x;
//		//		m_vPointer[(i << 1) + 1]	= vPoint[i].y;
//		//		m_uPointer[i << 1]			= uPoint[i].x;
//		//		m_uPointer[(i << 1) + 1]	= uPoint[i].y;
//		//	}
//			m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,m_vPointer,m_uPointer);
//		}else
//		{
//			//����Ĳ�����û������ģ�������δ��ϲü��ͷ�ת����ת��
//			m_pointSum = 4;
//			m_vPointer[0] = 0.0f;	m_vPointer[1] = 0.0f;
//			m_uPointer[0] = 0.0f;	m_uPointer[1] = 0.0f;
//			m_vPointer[2] = vw;		m_vPointer[3] = 0.0f;
//			m_uPointer[2] = uw;		m_uPointer[3] = 0.0f;
//			m_vPointer[4] = vw;		m_vPointer[5] = vh;
//			m_uPointer[4] = uw;		m_uPointer[5] = uh;
//			m_vPointer[6] = 0.0f;	m_vPointer[7] = vh;
//			m_uPointer[6] = 0.0f;	m_uPointer[7] = uh;
//		}
//		_XVector2 center(m_position.x + m_changeCenter.x * vw,m_position.y + m_changeCenter.y * vh);
//		_XVector2 dp;
//		for(int i = 0;i < m_pointSum;++ i)
//		{
//			if(m_needClip)
//			{
//				//�ü�
//				m_uPointer[i << 1] += tempClipRect.left;
//				m_uPointer[(i << 1) + 1] += tempClipRect.top;
//			}
//			//λ��
//			m_vPointer[i << 1] += m_position.x;
//			m_vPointer[(i << 1) + 1] += m_position.y;
//		}
//		//��ͼ����ļ���
//		//��ת
//		_XVector2 turnCenter;
//		if(m_needClip)
//		{
//			turnCenter.set(m_position.x + 0.5f * m_clipRect.getWidth(),
//				m_position.y + 0.5f * m_clipRect.getHeight()); //���еķ�ת������ԭͼ�����Ľ��е�
//		}else
//		{
//			turnCenter.set(m_position.x + 0.5f * m_textureData.textureSize.x,
//			m_position.y + 0.5f * m_textureData.textureSize.y); //���еķ�ת������ԭͼ�����Ľ��е�
//		}
//		if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[i << 1] = turnCenter.x + (turnCenter.x - m_vPointer[i << 1]);
//			}
//		}else
//		if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[(i << 1) + 1] = turnCenter.y + (turnCenter.y - m_vPointer[(i << 1) + 1]);
//			}
//		}else
//		if(m_turnOverMode == TURN_OVER_MODE_L2R_AND_U2D)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[i << 1] = turnCenter.x + (turnCenter.x - m_vPointer[i << 1]);
//				m_vPointer[(i << 1) + 1] = turnCenter.y + (turnCenter.y - m_vPointer[(i << 1) + 1]);
//			}
//		}
//		for(int i = 0;i < m_pointSum;++ i)
//		{
//			//����
//			m_vPointer[i << 1] = center.x + (m_vPointer[i << 1] - center.x) * m_size.x;
//			m_vPointer[(i << 1) + 1] = center.y + (m_vPointer[(i << 1) + 1] - center.y) * m_size.y;
//			//��ת
//			dp.set(m_vPointer[i << 1] - center.x,m_vPointer[(i << 1) + 1] - center.y);
//			m_vPointer[i << 1] = center.x + dp.x * m_cosAngle + dp.y * m_sinAngle;
//			m_vPointer[(i << 1) + 1] = center.y - dp.x * m_sinAngle + dp.y * m_cosAngle;
//		}
//
//		//�����Ǽ����Χ��(���ܻ�������⣬û�о�������)
//		m_rectPoint[0].set(0.0f,0.0f);
//		m_rectPoint[1].set(vw,0.0f);
//		m_rectPoint[2].set(vw,vh);
//		m_rectPoint[3].set(0.0f,vh);
//		for(int i = 0;i < 4;++ i)
//		{
//			//����
//			m_rectPoint[i].x = m_changeCenter.x * vw + (m_rectPoint[i].x - m_changeCenter.x * vw) * m_size.x;
//			m_rectPoint[i].y = m_changeCenter.y * vh + (m_rectPoint[i].y - m_changeCenter.y * vh) * m_size.y;
//			//λ��
//			m_rectPoint[i].x += m_position.x;
//			m_rectPoint[i].y += m_position.y;
//			//��ת
//			dp.set(m_rectPoint[i].x - center.x,m_rectPoint[i].y - center.y);
//			m_rectPoint[i].x = center.x + dp.x * m_cosAngle + dp.y * m_sinAngle;
//			m_rectPoint[i].y = center.y - dp.x * m_sinAngle + dp.y * m_cosAngle;
//		}
//	}else
//	{//��������Ҫ�ڲ��ü���
//		//�����ǽǶȲü���ʵ��
//		float vw = 1.0f;
//		float vh = 1.0f;
//		float uw = 1.0f;
//		float uh = 1.0f;
//		_XRect tempClipRect;
//		tempClipRect.set(0.0f,0.0f,1.0f,1.0f);
//		//���ü���һ��
//		if(m_needClip)
//		{//�������ʵ�ʵĲü����Σ����ⲿ��Ϻ�����
//			float left,top,right,bottom;
//			if(m_clipRect.left < m_textureData.textureMove.x) left = m_textureData.clipInsideRect.left;
//			else left = m_textureData.clipInsideRect.left + m_clipRect.left - m_textureData.textureMove.x;
//			if(m_clipRect.top < m_textureData.textureMove.y) top = m_textureData.clipInsideRect.top;
//			else top = m_textureData.clipInsideRect.top + m_clipRect.top - m_textureData.textureMove.y;
//			if(m_clipRect.right > m_textureData.textureSize.x - m_textureData.textureMove2.x) right = m_textureData.clipInsideRect.right;
//			else right = m_textureData.clipInsideRect.right - (m_textureData.textureSize.x - m_textureData.textureMove2.x - m_clipRect.right);
//			if(m_clipRect.bottom > m_textureData.textureSize.y - m_textureData.textureMove2.y) bottom = m_textureData.clipInsideRect.bottom;
//			else bottom = m_textureData.clipInsideRect.bottom - (m_textureData.textureSize.y - m_textureData.textureMove2.y - m_clipRect.bottom);
//
//			tempClipRect.set(left / (float)(m_textureData.texture.m_w),
//				top / (float)(m_textureData.texture.m_h),
//				right / (float)(m_textureData.texture.m_w),
//				bottom / (float)(m_textureData.texture.m_h));
//		}else
//		{
//			tempClipRect.set((float)(m_textureData.clipInsideRect.left) / (float)(m_textureData.texture.m_w),
//				(float)(m_textureData.clipInsideRect.top) / (float)(m_textureData.texture.m_h),
//				(float)(m_textureData.clipInsideRect.right) / (float)(m_textureData.texture.m_w),
//				(float)(m_textureData.clipInsideRect.bottom) / (float)(m_textureData.texture.m_h));
//		}
//
//		uw = tempClipRect.getWidth();			//��ͼ�ߴ�[0-1]
//		uh = tempClipRect.getHeight();
//		vw = uw * m_textureData.texture.m_w;	//����ߴ�pixel
//		vh = uh * m_textureData.texture.m_h;
//		if(m_needAngleClip != 0)
//		{
//		//	_XVector2 uPoint[7];
//		//	_XVector2 vPoint[7];
//			_XRect uRect;
//			_XRect vRect;
//			_XVector2 cPoint;
//			if(m_needClip)
//			{
//				float left,top;
//				if(m_textureData.textureMove.x > m_clipRect.left) left = m_textureData.textureMove.x - m_clipRect.left;
//				else left = 0.0;
//				if(m_textureData.textureMove.y > m_clipRect.top) top = m_textureData.textureMove.y - m_clipRect.top;
//				else top = 0.0;
//
//				uRect.set(0.0f,0.0f,uw,uh);
//				vRect.set(left,top,left + vw,top + vh);
//				cPoint.set(m_clipRect.getWidth() * 0.5f,m_clipRect.getHeight() * 0.5f);	
//			}else
//			{//ok
//				uRect.set(0.0f,0.0f,uw,uh);
//				vRect.set(m_textureData.textureMove.x,m_textureData.textureMove.y,
//					m_textureData.textureSize.x - m_textureData.textureMove2.x,m_textureData.textureSize.y - m_textureData.textureMove2.y);
//				cPoint.set(m_textureData.textureSize.x * 0.5f,m_textureData.textureSize.y * 0.5f);
//			}
//		//	m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,vPoint,uPoint);
//		//	for(int i = 0;i < m_pointSum;++ i)
//		//	{
//		//		m_vPointer[i << 1]			= vPoint[i].x;
//		//		m_vPointer[(i << 1) + 1]	= vPoint[i].y;
//		//		m_uPointer[i << 1]			= uPoint[i].x;
//		//		m_uPointer[(i << 1) + 1]	= uPoint[i].y;
//		//	}
//			m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,m_vPointer,m_uPointer);
//		}else
//		{
//			//����Ĳ�����û������ģ�������δ��ϲü��ͷ�ת����ת��
//			m_pointSum = 4;
//			if(m_needClip)
//			{
//				float left,top;
//				if(m_textureData.textureMove.x > m_clipRect.left) left = m_textureData.textureMove.x - m_clipRect.left;
//				else left = 0.0;
//				if(m_textureData.textureMove.y > m_clipRect.top) top = m_textureData.textureMove.y - m_clipRect.top;
//				else top = 0.0;
//				m_vPointer[0] = left;		m_vPointer[1] = top;
//				m_uPointer[0] = 0.0f;		m_uPointer[1] = 0.0f;
//				m_vPointer[2] = left + vw;	m_vPointer[3] = top;
//				m_uPointer[2] = uw;			m_uPointer[3] = 0.0f;
//				m_vPointer[4] = left + vw;	m_vPointer[5] = top + vh;
//				m_uPointer[4] = uw;			m_uPointer[5] = uh;
//				m_vPointer[6] = left;		m_vPointer[7] = top + vh;
//				m_uPointer[6] = 0.0f;		m_uPointer[7] = uh;
//			}else
//			{
//				m_vPointer[0] = m_textureData.textureMove.x;		m_vPointer[1] = m_textureData.textureMove.y;
//				m_uPointer[0] = 0.0f;								m_uPointer[1] = 0.0f;
//				m_vPointer[2] = m_textureData.textureMove.x + vw;	m_vPointer[3] = m_textureData.textureMove.y;
//				m_uPointer[2] = uw;									m_uPointer[3] = 0.0f;
//				m_vPointer[4] = m_textureData.textureMove.x + vw;	m_vPointer[5] = m_textureData.textureMove.y + vh;
//				m_uPointer[4] = uw;									m_uPointer[5] = uh;
//				m_vPointer[6] = m_textureData.textureMove.x;		m_vPointer[7] = m_textureData.textureMove.y + vh;
//				m_uPointer[6] = 0.0f;								m_uPointer[7] = uh;
//			}
//		}
//		_XVector2 center;
//		_XVector2 dp;
//		if(m_needClip)	//Ӧ��������������
//		{
//			center.set(m_position.x + m_changeCenter.x * m_clipRect.getWidth(),m_position.y + m_changeCenter.y * m_clipRect.getHeight());
//		}else
//		{
//			center.set(m_position.x + m_changeCenter.x * m_textureData.textureSize.x,
//				m_position.y + m_changeCenter.y * m_textureData.textureSize.y);
//		}
//		for(int i = 0;i < m_pointSum;++ i)
//		{
//			//�ü�
//			m_uPointer[i << 1] += tempClipRect.left;
//			m_uPointer[(i << 1) + 1] += tempClipRect.top;
//			//λ��
//			m_vPointer[i << 1] += m_position.x;
//			m_vPointer[(i << 1) + 1] += m_position.y;
//		}
//		//��ͼ����ļ���
//		//��ת
//		_XVector2 turnCenter;
//		if(m_needClip)
//		{
//			turnCenter.set(m_position.x + 0.5f * m_clipRect.getWidth(),
//				m_position.y + 0.5f * m_clipRect.getHeight()); //���еķ�ת������ԭͼ�����Ľ��е�
//		}else
//		{
//			turnCenter.set(m_position.x + 0.5f * m_textureData.textureSize.x,
//			m_position.y + 0.5f * m_textureData.textureSize.y); //���еķ�ת������ԭͼ�����Ľ��е�
//		}
//		if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[i << 1] = turnCenter.x + (turnCenter.x - m_vPointer[i << 1]);
//			}
//		}else
//		if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[(i << 1) + 1] = turnCenter.y + (turnCenter.y - m_vPointer[(i << 1) + 1]);
//			}
//		}else
//		if(m_turnOverMode == TURN_OVER_MODE_L2R_AND_U2D)
//		{
//			for(int i = 0;i < m_pointSum;++ i)
//			{
//				m_vPointer[i << 1] = turnCenter.x + (turnCenter.x - m_vPointer[i << 1]);
//				m_vPointer[(i << 1) + 1] = turnCenter.y + (turnCenter.y - m_vPointer[(i << 1) + 1]);
//			}
//		}
//		for(int i = 0;i < m_pointSum;++ i)
//		{
//			//����
//			m_vPointer[i << 1] = center.x + (m_vPointer[i << 1] - center.x) * m_size.x;
//			m_vPointer[(i << 1) + 1] = center.y + (m_vPointer[(i << 1) + 1] - center.y) * m_size.y;
//			//��ת
//			dp.set(m_vPointer[i << 1] - center.x,m_vPointer[(i << 1) + 1] - center.y);
//			m_vPointer[i << 1] = center.x + dp.x * m_cosAngle + dp.y * m_sinAngle;
//			m_vPointer[(i << 1) + 1] = center.y - dp.x * m_sinAngle + dp.y * m_cosAngle;
//		}
//
//		//�����Ǽ����Χ��(���ܻ�������⣬û�о�������)
//		m_rectPoint[0].set(0.0f,0.0f);
//		m_rectPoint[1].set(vw,0.0f);
//		m_rectPoint[2].set(vw,vh);
//		m_rectPoint[3].set(0.0f,vh);
//		for(int i = 0;i < 4;++ i)
//		{
//			//����
//			m_rectPoint[i].x = m_changeCenter.x * vw + (m_rectPoint[i].x - m_changeCenter.x * vw) * m_size.x;
//			m_rectPoint[i].y = m_changeCenter.y * vh + (m_rectPoint[i].y - m_changeCenter.y * vh) * m_size.y;
//			//λ��
//			m_rectPoint[i].x += m_position.x;
//			m_rectPoint[i].y += m_position.y;
//			//��ת
//			dp.set(m_rectPoint[i].x - center.x,m_rectPoint[i].y - center.y);
//			m_rectPoint[i].x = center.x + dp.x * m_cosAngle + dp.y * m_sinAngle;
//			m_rectPoint[i].y = center.y - dp.x * m_sinAngle + dp.y * m_cosAngle;
//		}
//	}
//}
//�����Ƕ���������������Ż�
void _XSprite::updateData()
{
	if(!m_needUpdateData) return;
	m_needUpdateData = XFalse;
	m_upTexDataID = m_textureData.getID();
	if(!m_textureData.isEnableInsideClip)
	{//��ʹ���ڲ���Դ����
		//�����ǽǶȲü���ʵ��
		float vw = 1.0f;
		float vh = 1.0f;
		float uw = 1.0f;
		float uh = 1.0f;
		_XRect tempClipRect(0.0f,0.0f,1.0f,1.0f);
		if(m_needClip)
		{//�����Ҫ�ⲿ�ü�
			tempClipRect.set(m_clipRect.left / m_pixelSize.x,m_clipRect.top / m_pixelSize.y,
				m_clipRect.right / m_pixelSize.x,m_clipRect.bottom / m_pixelSize.y);
		}
		//v�ü�x
		if(m_needClip)
		{
			uw = tempClipRect.getWidth();
			uh = tempClipRect.getHeight();
			vw = m_clipRect.getWidth();
			vh = m_clipRect.getHeight();
		}else
		{
			vw = m_pixelSize.x;
			vh = m_pixelSize.y;
			uw = 1.0f;
			uh = 1.0f;
		}
		if(m_needAngleClip)
		{
			_XRect uRect(0.0f,0.0f,uw,uh);
			_XRect vRect(0.0f,0.0f,vw,vh);
			_XVector2 cPoint = vRect.getCenter();
			m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,m_vPointer,m_uPointer);
		}else
		{
			//����Ĳ�����û������ģ�������δ��ϲü��ͷ�ת����ת��
			m_pointSum = 4;
			m_vPointer[0] = 0.0f;	m_vPointer[1] = 0.0f;
			m_uPointer[0] = 0.0f;	m_uPointer[1] = 0.0f;
			m_vPointer[2] = vw;		m_vPointer[3] = 0.0f;
			m_uPointer[2] = uw;		m_uPointer[3] = 0.0f;
			m_vPointer[4] = vw;		m_vPointer[5] = vh;
			m_uPointer[4] = uw;		m_uPointer[5] = uh;
			m_vPointer[6] = 0.0f;	m_vPointer[7] = vh;
			m_uPointer[6] = 0.0f;	m_uPointer[7] = uh;
		}
		float centerx = m_position.x + m_changeCenter.x * vw;
		float centery = m_position.y + m_changeCenter.y * vh;
		float dpx;
		float dpy;
		int pointDataSum = m_pointSum << 1;
		if(m_needClip)
		{
			for(int i = 0;i < pointDataSum;i += 2)
			{
				//�ü�
				m_uPointer[i] += tempClipRect.left;
				m_uPointer[i + 1] += tempClipRect.top;
				//λ��
				m_vPointer[i] += m_position.x;
				m_vPointer[i + 1] += m_position.y;
			}
		}else
		{
			for(int i = 0;i < pointDataSum;i += 2)
			{
				//λ��
				m_vPointer[i] += m_position.x;
				m_vPointer[i + 1] += m_position.y;
			}
		}
		//��ͼ����ļ���
		//��ת
		float turnCenterx;
		float turnCentery;
		if(m_needClip)
		{
			turnCenterx = m_position.x + 0.5f * m_clipRect.getWidth();
			turnCentery = m_position.y + 0.5f * m_clipRect.getHeight(); //���еķ�ת������ԭͼ�����Ľ��е�
		}else
		{
			turnCenterx = m_position.x + 0.5f * m_textureData.textureSize.x;
			turnCentery = m_position.y + 0.5f * m_textureData.textureSize.y; //���еķ�ת������ԭͼ�����Ľ��е�
		}
		switch(m_turnOverMode)
		{
		case TURN_OVER_MODE_LEFT_TO_RIGHT:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i] = turnCenterx + (turnCenterx - m_vPointer[i]);
			}
			break;
		case TURN_OVER_MODE_UP_TO_DOWN:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i + 1] = turnCentery + (turnCentery - m_vPointer[i + 1]);
			}
			break;
		case TURN_OVER_MODE_L2R_AND_U2D:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i] = turnCenterx + (turnCenterx - m_vPointer[i]);
				m_vPointer[i + 1] = turnCentery + (turnCentery - m_vPointer[i + 1]);
			}
			break;
		}
		for(int i = 0;i < pointDataSum;i += 2)
		{
			dpx = (m_vPointer[i] - centerx) * m_size.x;
			dpy = (m_vPointer[i + 1] - centery) * m_size.y;
			m_vPointer[i] = centerx + dpx * m_cosAngle + dpy * m_sinAngle;
			m_vPointer[i + 1] = centery - dpx * m_sinAngle + dpy * m_cosAngle;
		}

		//�����Ǽ����Χ��(���ܻ�������⣬û�о�������)
		m_rectPoint[0].set(0.0f,0.0f);
		m_rectPoint[1].set(vw,0.0f);
		m_rectPoint[2].set(vw,vh);
		m_rectPoint[3].set(0.0f,vh);
		turnCenterx = m_changeCenter.x * vw;
		turnCentery = m_changeCenter.y * vh;
		for(int i = 0;i < 4;++ i)
		{
			dpx = turnCenterx + (m_rectPoint[i].x - turnCenterx) * m_size.x + m_position.x - centerx;
			dpy = turnCentery + (m_rectPoint[i].y - turnCentery) * m_size.y + m_position.y - centery;
			m_rectPoint[i].x = centerx + dpx * m_cosAngle + dpy * m_sinAngle;
			m_rectPoint[i].y = centery - dpx * m_sinAngle + dpy * m_cosAngle;
		}
	}else
	{//��������Ҫ�ڲ��ü���
		//�����ǽǶȲü���ʵ��
		float vw = 1.0f / m_textureData.texture.m_w;
		float vh = 1.0f / m_textureData.texture.m_h;
		float uw = 1.0f;
		float uh = 1.0f;
		_XRect tempClipRect(0.0f,0.0f,1.0f,1.0f);
		//���ü���һ��
		if(m_needClip)
		{//�������ʵ�ʵĲü����Σ����ⲿ��Ϻ�����
			float left,top,right,bottom;
			if(m_clipRect.left < m_textureData.textureMove.x) left = m_textureData.clipInsideRect.left;
			else left = m_textureData.clipInsideRect.left + m_clipRect.left - m_textureData.textureMove.x;
			if(m_clipRect.top < m_textureData.textureMove.y) top = m_textureData.clipInsideRect.top;
			else top = m_textureData.clipInsideRect.top + m_clipRect.top - m_textureData.textureMove.y;
			if(m_clipRect.right > m_textureData.textureSize.x - m_textureData.textureMove2.x) right = m_textureData.clipInsideRect.right;
			else right = m_textureData.clipInsideRect.right - (m_textureData.textureSize.x - m_textureData.textureMove2.x - m_clipRect.right);
			if(m_clipRect.bottom > m_textureData.textureSize.y - m_textureData.textureMove2.y) bottom = m_textureData.clipInsideRect.bottom;
			else bottom = m_textureData.clipInsideRect.bottom - (m_textureData.textureSize.y - m_textureData.textureMove2.y - m_clipRect.bottom);

			tempClipRect.set(left * vw,top * vh,right * vw,bottom * vh);
		}else
		{
			tempClipRect.set(m_textureData.clipInsideRect.left * vw,
				m_textureData.clipInsideRect.top * vh,
				m_textureData.clipInsideRect.right * vw,
				m_textureData.clipInsideRect.bottom * vh);
		}

		uw = tempClipRect.getWidth();			//��ͼ�ߴ�[0-1]
		uh = tempClipRect.getHeight();
		vw = uw * m_textureData.texture.m_w;	//����ߴ�pixel
		vh = uh * m_textureData.texture.m_h;
		if(m_needAngleClip)
		{
		//	_XVector2 uPoint[7];
		//	_XVector2 vPoint[7];
			_XRect uRect;
			_XRect vRect;
			_XVector2 cPoint;
			if(m_needClip)
			{
				float left,top;
				if(m_textureData.textureMove.x > m_clipRect.left) left = m_textureData.textureMove.x - m_clipRect.left;
				else left = 0.0f;
				if(m_textureData.textureMove.y > m_clipRect.top) top = m_textureData.textureMove.y - m_clipRect.top;
				else top = 0.0f;

				uRect.set(0.0f,0.0f,uw,uh);
				vRect.set(left,top,left + vw,top + vh);
				cPoint.set(m_clipRect.getWidth() * 0.5f,m_clipRect.getHeight() * 0.5f);	
			}else
			{//ok
				uRect.set(0.0f,0.0f,uw,uh);
				vRect.set(m_textureData.textureMove.x,m_textureData.textureMove.y,
					m_textureData.textureSize.x - m_textureData.textureMove2.x,m_textureData.textureSize.y - m_textureData.textureMove2.y);
				cPoint.set(m_textureData.textureSize.x * 0.5f,m_textureData.textureSize.y * 0.5f);
			}
			m_pointSum = getEdgePointEx(vRect,uRect,cPoint,m_clipAngle * DEGREE2RADIAN,m_vPointer,m_uPointer);
		}else
		{
			//����Ĳ�����û������ģ�������δ��ϲü��ͷ�ת����ת��
			m_pointSum = 4;
			if(m_needClip)
			{
				float left,top;
				if(m_textureData.textureMove.x > m_clipRect.left) left = m_textureData.textureMove.x - m_clipRect.left;
				else left = 0.0f;
				if(m_textureData.textureMove.y > m_clipRect.top) top = m_textureData.textureMove.y - m_clipRect.top;
				else top = 0.0f;
				m_vPointer[0] = left;		m_vPointer[1] = top;
				m_uPointer[0] = 0.0f;		m_uPointer[1] = 0.0f;
				m_vPointer[2] = left + vw;	m_vPointer[3] = top;
				m_uPointer[2] = uw;			m_uPointer[3] = 0.0f;
				m_vPointer[4] = left + vw;	m_vPointer[5] = top + vh;
				m_uPointer[4] = uw;			m_uPointer[5] = uh;
				m_vPointer[6] = left;		m_vPointer[7] = top + vh;
				m_uPointer[6] = 0.0f;		m_uPointer[7] = uh;
			}else
			{
				m_vPointer[0] = m_textureData.textureMove.x;		m_vPointer[1] = m_textureData.textureMove.y;
				m_uPointer[0] = 0.0f;								m_uPointer[1] = 0.0f;
				m_vPointer[2] = m_textureData.textureMove.x + vw;	m_vPointer[3] = m_textureData.textureMove.y;
				m_uPointer[2] = uw;									m_uPointer[3] = 0.0f;
				m_vPointer[4] = m_textureData.textureMove.x + vw;	m_vPointer[5] = m_textureData.textureMove.y + vh;
				m_uPointer[4] = uw;									m_uPointer[5] = uh;
				m_vPointer[6] = m_textureData.textureMove.x;		m_vPointer[7] = m_textureData.textureMove.y + vh;
				m_uPointer[6] = 0.0f;								m_uPointer[7] = uh;
			}
		}
		//_XVector2 center;
		float centerx;
		float centery;
		//_XVector2 dp;
		float dpx;
		float dpy;
		int pointDataSum = m_pointSum << 1;
		if(m_needClip)	//Ӧ��������������
		{
			centerx = m_position.x + m_changeCenter.x * m_clipRect.getWidth();
			centery = m_position.y + m_changeCenter.y * m_clipRect.getHeight();
		}else
		{
			centerx = m_position.x + m_changeCenter.x * m_textureData.textureSize.x;
			centery = m_position.y + m_changeCenter.y * m_textureData.textureSize.y;
		}
		for(int i = 0;i < pointDataSum;i += 2)
		{
			//�ü�
			m_uPointer[i] += tempClipRect.left;
			m_uPointer[i + 1] += tempClipRect.top;
			//λ��
			m_vPointer[i] += m_position.x;
			m_vPointer[i + 1] += m_position.y;
		}
		//��ͼ����ļ���
		//��ת
		float turnCenterx;
		float turnCentery;
		if(m_needClip)
		{
			turnCenterx = m_position.x + 0.5f * m_clipRect.getWidth();
			turnCentery = m_position.y + 0.5f * m_clipRect.getHeight(); //���еķ�ת������ԭͼ�����Ľ��е�
		}else
		{
			turnCenterx = m_position.x + 0.5f * m_textureData.textureSize.x;
			turnCentery = m_position.y + 0.5f * m_textureData.textureSize.y; //���еķ�ת������ԭͼ�����Ľ��е�
		}
		switch(m_turnOverMode)
		{
		case TURN_OVER_MODE_LEFT_TO_RIGHT:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i] = turnCenterx + (turnCenterx - m_vPointer[i]);
			}
			break;
		case TURN_OVER_MODE_UP_TO_DOWN:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i + 1] = turnCentery + (turnCentery - m_vPointer[i + 1]);
			}
			break;
		case TURN_OVER_MODE_L2R_AND_U2D:
			for(int i = 0;i < pointDataSum;i += 2)
			{
				m_vPointer[i] = turnCenterx + (turnCenterx - m_vPointer[i]);
				m_vPointer[i + 1] = turnCentery + (turnCentery - m_vPointer[i + 1]);
			}
			break;
		}
		for(int i = 0;i < pointDataSum;i += 2)
		{
			dpx = (m_vPointer[i] - centerx) * m_size.x;
			dpy = (m_vPointer[i + 1] - centery) * m_size.y;
			m_vPointer[i] = centerx + dpx * m_cosAngle + dpy * m_sinAngle;
			m_vPointer[i + 1] = centery - dpx * m_sinAngle + dpy * m_cosAngle;
		}

		//�����Ǽ����Χ��(���ܻ�������⣬û�о�������)
		m_rectPoint[0].set(0.0f,0.0f);
		m_rectPoint[1].set(vw,0.0f);
		m_rectPoint[2].set(vw,vh);
		m_rectPoint[3].set(0.0f,vh);
		turnCenterx = m_changeCenter.x * vw;
		turnCentery = m_changeCenter.y * vh;
		for(int i = 0;i < 4;++ i)
		{
			dpx = turnCenterx + (m_rectPoint[i].x - turnCenterx) * m_size.x + m_position.x - centerx;
			dpy = turnCentery + (m_rectPoint[i].y - turnCentery) * m_size.y + m_position.y - centery;
			m_rectPoint[i].x = centerx + dpx * m_cosAngle + dpy * m_sinAngle;
			m_rectPoint[i].y = centery - dpx * m_sinAngle + dpy * m_cosAngle;
		}
	}
}
void _XSprite::draw()
{
	if(!m_isInited ||
		!m_isVisible) return;
	updateData();
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);

	//if(m_pShader == NULL && m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D,m_textureData.texture.m_texture);	//rational:UMR/MLK
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D,m_textureData.texture.m_texture);

		//if(m_pShader != NULL) m_pShader->useShader();
		//if(m_pShaderProc != NULL) m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		m_pShader->useShaderEx(m_textureData.texture.m_texture);
	}

	glColor4fv(m_color);
	drawInside();

	//if(m_pShader != NULL || m_pShaderProc != NULL) glUseProgram(0);	
	if(m_pShader != NULL) m_pShader->disShader();
	glDisable(GL_BLEND);
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPopAttrib();
}
void _XSprite::draw(const _XVector2 *u,const _XVector2 *v,int w,int h)
{
	if(!m_isInited ||
		!m_isVisible) return;
	updateData();
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);

	//if(m_pShader == NULL && m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D,m_textureData.texture.m_texture);	//rational:UMR/MLK
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D,m_textureData.texture.m_texture);

		//if(m_pShader != NULL) m_pShader->useShader();
		//if(m_pShaderProc != NULL) m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		m_pShader->useShaderEx(m_textureData.texture.m_texture);
	};

	glColor4fv(m_color);
	//drawInside();
	int offsetTemp = 0;
	for(int j = 0;j < h - 1;++ j)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0;i < w;++ i)
		{	//����
			offsetTemp = i + j * w;
			glTexCoord2fv(u[offsetTemp]);	//u
			glVertex2fv(v[offsetTemp]);	//v

			glTexCoord2fv(u[offsetTemp + w]);	//u
			glVertex2fv(v[offsetTemp + w]);	//v
		}
		glEnd();
	}

	//if(m_pShader != NULL || m_pShaderProc != NULL) glUseProgram(0);	
	glDisable(GL_BLEND);
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPopAttrib();
	if(m_pShader != NULL) m_pShader->disShader();
}
void _XSprite::draw(GLuint tex)
{
	if(!m_isInited ||
		!m_isVisible) return;
	updateData();
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);

	//if(m_pShader == NULL && m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D,tex);
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D,tex);

		//if(m_pShader != NULL) m_pShader->useShader();
		//if(m_pShaderProc != NULL) m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		m_pShader->useShaderEx(tex);
	}
	glColor4fv(m_color);
	drawInside();

	//if(m_pShader != NULL || m_pShaderProc != NULL) glUseProgram(0);	
	if(m_pShader != NULL) m_pShader->disShader();
	glDisable(GL_BLEND);
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPopAttrib();
}
void _XSprite::draw(const _XTextureData& texData)
{
	if(!m_isInited ||
		!m_isVisible) return;
	//�����ֳ����ݣ�������ڲ�������Ҫ����

	_XTextureData tempTexture;
	tempTexture = m_textureData;
	m_textureData = texData;
	if(m_upTexDataID != texData.getID() || m_needUpdateData)
	{//��ͬ�����ݾͲ���Ҫ����ˢ��
		m_needUpdateData = XTrue;
		updateData();
	}
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);
	glEnable(GL_BLEND);

	//if(m_pShader == NULL && m_pShaderProc == NULL)
	if(m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D,texData.texture.m_texture);
	}else
	{
		//glActiveTexture(GL_TEXTURE0);
		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D,texData.texture.m_texture);

		//if(m_pShader != NULL) m_pShader->useShader();
		//if(m_pShaderProc != NULL) m_pShaderProc();

		//glActiveTexture(GL_TEXTURE0);
		m_pShader->useShaderEx(texData.texture.m_texture);
	};
	glColor4fv(m_color);
	drawInside();

	//if(m_pShader != NULL || m_pShaderProc != NULL) glUseProgram(0);	
	glDisable(GL_BLEND);
	//if(m_pShader != NULL || m_pShaderProc != NULL) glPopAttrib();
	if(m_pShader != NULL) m_pShader->disShader();

	m_textureData = tempTexture;	//���֮��ԭ�ڲ�����
}
void _XSprite::release()
{
	if(!m_isInited) return;
	//if(glIsTexture(m_tex))
	//	glDeleteTextures(1,&m_tex);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isInited = XFalse;
}
_XSprite& _XSprite::operator = (const _XSprite& temp)	//��Ŀ�����Դ���й��ã�������
{
	if(& temp == this) return * this;	//��ֹ����ֵ

	m_isInited = temp.m_isInited;				//�Ƿ���ɳ�ʼ��
	m_resoursePosition = temp.m_resoursePosition;		//��Դλ�� 0:�ⲿ 1:�ڲ�
	m_upTexDataID = temp.m_upTexDataID;		//��Դλ�� 0:�ⲿ 1:�ڲ�

	m_turnOverMode = temp.m_turnOverMode;	
	m_position = temp.m_position;		
	m_size = temp.m_size;				
	m_pixelSize = temp.m_pixelSize;		
	m_angle = temp.m_angle;				
	m_changeCenter = temp.m_changeCenter;		
	m_needClip = temp.m_needClip;			
	m_clipRect = temp.m_clipRect;	
	m_pointSum = temp.m_pointSum;			//���������
	m_needAngleClip = temp.m_needAngleClip;	//�Ƿ���Ҫ���нǶȲü�
	m_clipAngle = temp.m_clipAngle;		//�ü��ĽǶ�[0 - 360]
	for(int i = 0;i < 14;++ i)
	{
		m_vPointer[i] = temp.m_vPointer[i];	//��������
		m_uPointer[i] = temp.m_uPointer[i];	//��ͼ����
	}
	for(int i = 0;i < 4;++ i)
	{
		m_rectPoint[i] = temp.m_rectPoint[i];
	}
	m_sinAngle = temp.m_sinAngle;	
	m_cosAngle = temp.m_cosAngle;
	m_textureData = temp.m_textureData;	
	m_color = temp.m_color;			
	m_isVisible = temp.m_isVisible;

	m_needUpdateData = temp.m_needUpdateData;
	m_blendType = temp.m_blendType;
	m_blendRGBScr = temp.m_blendRGBScr;
	m_blendRGBDst = temp.m_blendRGBDst;
	m_blendAScr = temp.m_blendAScr;
	m_blendADst = temp.m_blendADst;
	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_pShader = temp.m_pShader;
	//m_pShaderProc = temp.m_pShaderProc;

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return * this;
}
void _XSprite::setACopy(const _XSprite& temp)
{
	if(& temp == this) return;	//��ֹ����ֵ

	m_isInited = temp.m_isInited;				//�Ƿ���ɳ�ʼ��
	m_resoursePosition = temp.m_resoursePosition;		//��Դλ�� 0:�ⲿ 1:�ڲ�
	m_upTexDataID = temp.m_upTexDataID;		//��Դλ�� 0:�ⲿ 1:�ڲ�

	m_turnOverMode = temp.m_turnOverMode;	
	m_position = temp.m_position;		
	m_size = temp.m_size;				
	m_pixelSize = temp.m_pixelSize;		
	m_angle = temp.m_angle;				
	m_changeCenter = temp.m_changeCenter;		
	m_needClip = temp.m_needClip;			
	m_clipRect = temp.m_clipRect;	
	m_pointSum = temp.m_pointSum;			//���������
	m_needAngleClip = temp.m_needAngleClip;	//�Ƿ���Ҫ���нǶȲü�
	m_clipAngle = temp.m_clipAngle;		//�ü��ĽǶ�[0 - 360]
	for(int i = 0;i < 14;++ i)
	{
		m_vPointer[i] = temp.m_vPointer[i];	//��������
		m_uPointer[i] = temp.m_uPointer[i];	//��ͼ����
	}
	for(int i = 0;i < 4;++ i)
	{
		m_rectPoint[i] = temp.m_rectPoint[i];
	}
	m_sinAngle = temp.m_sinAngle;	
	m_cosAngle = temp.m_cosAngle;
	m_textureData = temp.m_textureData;	
	m_color = temp.m_color;			
	m_isVisible = temp.m_isVisible;

	m_needUpdateData = temp.m_needUpdateData;
	m_blendType = temp.m_blendType;
	m_blendRGBScr = temp.m_blendRGBScr;
	m_blendRGBDst = temp.m_blendRGBDst;
	m_blendAScr = temp.m_blendAScr;
	m_blendADst = temp.m_blendADst;
	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_pShader = temp.m_pShader;
	//m_pShaderProc = temp.m_pShaderProc;

#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
}
#else
int _XSprite::init(const char *filename,					//ͼƬ������
		_XResourcePosition resoursePosition,						//��Դ��λ��
		_XTransformMode isTransformCenter)					//�Ƿ�ʹ��������ת	
{
	if(m_isInited != 0) 
	{
		LogStr("The action have initted!");
		return 1;
	}

	m_positionX = 0.0f;//�����λ��
	m_positionY = 0.0f;
	angle = 0.0f;	//����ĽǶ�
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//��������ű���
	ysize = 1.0f;
	//������Դ��λ��
	m_resoursePosition = resoursePosition;
	setIsTransformCenter(isTransformCenter);

	if(m_textureData.load(filename,m_resoursePosition) == 0) return 0;
	//����3
	updateClipAndRotatoData();
	//����2
	//if(m_isEnableOutsideChip == 0)
	//{//���û�н����ⲿ�ü�����������м���
	//	setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
	//	m_isEnableOutsideChip = 0;
	//}else
	//{//����Ѿ��������ⲿ�ü����������������
	//	setClipRect(m_clipOutsideRect);
	//}
	//����1
	//if(m_textureData.isEnableInsideClip == 1)
	//{
	////	init(withAlpha,m_texInfo.texture.m_w,m_texInfo.texture.m_h,0);
	//	m_clipRect = m_textureData.clipInsideRect;

	//	if(m_isTransformCenter == POINT_CENTER)
	//	{
	//		m_transformCenter.set((m_clipRect.getWidth() + m_textureData.textureMove.x + m_textureData.textureMove2.x) * 0.5f - m_textureData.textureMove.x,
	//			(m_clipRect.getHeight() + m_textureData.textureMove.y + m_textureData.textureMove2.y) * 0.5f - m_textureData.textureMove.y);
	//	}else
	//	{
	//		m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
	//	}
	////	return 1;
	//}else
	//{
	//	if(m_isTransformCenter == POINT_CENTER)
	//	{
	//		m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
	//	}else
	//	{
	//		m_transformCenter.set(0.0f,0.0f);
	//	}
	//}

	//�������֮�������صļ���
	if(isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this,OBJ_SPRITE);
#endif

	m_isInited = 1;
	return 1;
}
int _XSprite::init(_XTextureData& texData,_XTransformMode isTransformCenter)
{
	if(m_isInited != 0) 
	{
		LogStr("The action have initted!");
		return 1;
	}

	m_positionX = 0.0f;//�����λ��
	m_positionY = 0.0f;
	angle = 0.0f;	//����ĽǶ�
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//��������ű���
	ysize = 1.0f;
	//������Դ��λ��
	//m_resoursePosition = resoursePosition;
	setIsTransformCenter(isTransformCenter);

	m_textureData = texData;

	updateClipAndRotatoData();

	//�������֮�������صļ���
	if(isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this,OBJ_SPRITE);
#endif

	m_isInited = 1;
	return 1;
}
int _XSprite::init(int w,int h,int colorMode,unsigned int tex,_XTransformMode isTransformCenter)
{
	if(m_isInited != 0) 
	{
		LogStr("The action have initted!");
		return 1;
	}

	m_positionX = 0.0f;//�����λ��
	m_positionY = 0.0f;
	angle = 0.0f;	//����ĽǶ�
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//��������ű���
	ysize = 1.0f;
	//������Դ��λ��
	//m_resoursePosition = resoursePosition;
	setIsTransformCenter(isTransformCenter);

	if(m_textureData.createWithTexture(w,h,colorMode,tex) == 0) return 0;

	updateClipAndRotatoData();

	//�������֮�������صļ���
	if(isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
#endif
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this,OBJ_SPRITE);
#endif

	m_isInited = 1;
	return 1;
}
//needSizeCheck �Ƿ���Ҫ�Գߴ����2��n�ηõļ�飬����������ͼû�о���2��N�ηõļ������Ͳ���Ҫ����2��n�ηõļ��
//������Ҫ����������
int _XSprite::init(int tempW,int tempH,int needSizeCheck,_XTransformMode isTransformCenter)
{
	if(m_isInited != 0) 
	{
		LogStr("The action have initted!");
		return 1;
	}

//	x = 0;			//�����λ��
//	y = 0;
	m_positionX = 0.0f;//�����λ��
	m_positionY = 0.0f;
	angle = 0.0f;	//����ĽǶ�
	sinAngle = 0.0f;
	cosAngle = 1.0f;
	xsize = 1.0f;		//��������ű���
	ysize = 1.0f;
	setIsTransformCenter(isTransformCenter);
	
	if(m_textureData.loadEmpty() == 0) return 0;

	m_textureData.texture.m_w = tempW;
	m_textureData.texture.m_h = tempH;
	//�������֮�������صļ���
	if(needSizeCheck == 1 && isNPOT(m_textureData.texture.m_w, m_textureData.texture.m_h)) 
	{
		wr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_w)/logf(2.0f)));
		hr = (int)powf(2.0, ceilf(logf((float)m_textureData.texture.m_h)/logf(2.0f)));
		xp = (wr - m_textureData.texture.m_w)/2;
		yp = (hr - m_textureData.texture.m_h)/2;
	}else 
	{
		wr = m_textureData.texture.m_w;
		hr = m_textureData.texture.m_h;
		xp= 0;
		yp= 0;
	}
	if(m_isTransformCenter == POINT_CENTER)
	{
		m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
	}else
	{
		m_transformCenter.set(0.0f,0.0f);
	}
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this,OBJ_SPRITE);
#endif
	m_isInited = 1;
#if IS_USE_SOLIDIFY
	m_needSolidify = 1;
	m_glListOrder = glGenLists(1); //��ȡ��ʾ�б�ı��
	if(m_glListOrder < 0) return 0;
#endif
	return 1;
}
void _XSprite::drawBasic(const GLuint *pTexnum)
{
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;

	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);

	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);	//������ͼ�Ĵ�С

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0) glRotatef(angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//�������ŵı���
	
	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//����Ϊָ������ɫ
#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	drawInside();

	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL || m_pShader != NULL) glPopAttrib();
}
void _XSprite::updatePointArray()
{
	m_pointArraySize = 4;	//��ͨ�����4���
	if(m_turnOverMode != TURN_OVER_MODE_NULL)
	{
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//����Ҫ�ü�
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
			m_pointArray[0].set(xp + m_textureData.texture.m_w * m_shapeData[0],yp + m_textureData.texture.m_h * m_shapeData[1]);
			m_pointArray[1].set(m_textureData.texture.m_w * m_shapeData[2] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[3] -m_transformCenter.y);

			m_pointArray[2].set(xp + m_textureData.texture.m_w * m_shapeData[4],yp + m_textureData.texture.m_h * m_shapeData[5]);
			m_pointArray[3].set(m_textureData.texture.m_w * m_shapeData[6] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[7] -m_transformCenter.y);

			m_pointArray[4].set(xp + m_textureData.texture.m_w * m_shapeData[8],yp + m_textureData.texture.m_h * m_shapeData[9]);
			m_pointArray[5].set(m_textureData.texture.m_w * m_shapeData[10] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[11] -m_transformCenter.y);

			m_pointArray[6].set(xp + m_textureData.texture.m_w * m_shapeData[12],yp + m_textureData.texture.m_h * m_shapeData[13]);
			m_pointArray[7].set(m_textureData.texture.m_w * m_shapeData[14] - m_transformCenter.x,m_textureData.texture.m_h * m_shapeData[15] -m_transformCenter.y);
		}else
		{//��Ҫ�ü�(������)
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_clipRect.getWidth() * m_clipRect.getHeight();
#endif
			m_pointArray[0].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[0],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[1]);
			m_pointArray[1].set(m_clipRect.getWidth() * m_shapeData[2] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[3] - m_transformCenter.y);

			m_pointArray[2].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[4],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[5]);
			m_pointArray[3].set(m_clipRect.getWidth() * m_shapeData[6] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[7] - m_transformCenter.y);

			m_pointArray[4].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[8],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[9]);
			m_pointArray[5].set(m_clipRect.getWidth() * m_shapeData[10] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[11] - m_transformCenter.y);

			m_pointArray[6].set(xp + m_clipRect.left + m_clipRect.getWidth() * m_shapeData[12],yp + m_clipRect.top + m_clipRect.getHeight() * m_shapeData[13]);
			m_pointArray[7].set(m_clipRect.getWidth() * m_shapeData[14] - m_transformCenter.x,m_clipRect.getHeight() * m_shapeData[15] - m_transformCenter.y);
		}
	}else
	{
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
			m_pointArray[0].set(xp,yp);
			m_pointArray[1].set(-m_transformCenter.x, -m_transformCenter.y);

			m_pointArray[2].set(xp, yp + m_textureData.texture.m_h);
			m_pointArray[3].set(-m_transformCenter.x,m_textureData.texture.m_h - m_transformCenter.y);

			m_pointArray[4].set(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
			m_pointArray[5].set(m_textureData.texture.m_w - m_transformCenter.x,m_textureData.texture.m_h - m_transformCenter.y);

			m_pointArray[6].set(xp + m_textureData.texture.m_w, yp);
			m_pointArray[7].set(m_textureData.texture.m_w - m_transformCenter.x,-m_transformCenter.y);
		}else
		{
#ifdef GET_ALL_PIXEL
			m_thisDrawArea += m_clipRect.getWidth() * m_clipRect.getHeight();
#endif
			m_pointArray[0].set(xp + m_clipRect.left,yp + m_clipRect.top);
			m_pointArray[1].set(-m_transformCenter.x, -m_transformCenter.y);

			m_pointArray[2].set(xp + m_clipRect.left, yp + m_clipRect.bottom);
			m_pointArray[3].set(-m_transformCenter.x, m_clipRect.getHeight() - m_transformCenter.y);

			m_pointArray[4].set(xp + m_clipRect.right, yp + m_clipRect.bottom);
			m_pointArray[5].set(m_clipRect.getWidth() - m_transformCenter.x,m_clipRect.getHeight() - m_transformCenter.y);

			m_pointArray[6].set(xp + m_clipRect.right, yp + m_clipRect.top);
			m_pointArray[7].set(m_clipRect.getWidth() - m_transformCenter.x, -m_transformCenter.y);
		}
	}
	if(m_needAngleClip != 0)
	{//�����Ȳ����Ƿ�ת�Լ��ü���ɵ�Ӱ�죬Ҳ����˵��ת�Ͳü����������������߼��Ĳ�ͳһ
		_XVector2 tempArrayV[4];
		_XVector2 tempArrayU[4];
		for(int i = 0;i < 4;++ i)
		{
			tempArrayV[i] = m_pointArray[i << 1];
			tempArrayU[i] = m_pointArray[(i << 1) + 1];
		}
		vector4Sort(tempArrayV,tempArrayU);
		m_pointArraySize = getEdgePoint(m_clipAngle * DEGREE2RADIAN,m_pointArray,tempArrayV,tempArrayU);
	}
	//���潫���ݸ��µ���ͼ������
	for(int i = 0; i < m_pointArraySize;++ i)
	{
		int index = (i << 1);
		m_uPoint[index] = m_pointArray[index].x;
		m_uPoint[index + 1] = m_pointArray[index].y;
		m_vPoint[index] = m_pointArray[index + 1].x;
		m_vPoint[index + 1] = m_pointArray[index + 1].y;
	}
}
void _XSprite::drawBasic(const _XTextureData *pTexData)
{
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//�����ֳ������ﱸ�ݵ����ݲ����������ݶ����ݣ����ԱȽϱ���
		tempText = m_textureData;
		m_textureData = *pTexData;
		//���¼��к���ת
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//�����߼��ϴ����ظ����õ����⣬��Ҫ���߼������������ֹ�ظ��ļ��㷴������
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	if(m_blendType == BLEND_TWO_DATA) glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
	else glBlendFuncSeparate(srcBlendMode[m_blendRGBScr],dstBlendMode[m_blendRGBDst],srcBlendMode[m_blendAScr],dstBlendMode[m_blendADst]);

	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, wr << 1, 0,hr << 1, -1, 1);	//������ͼ�Ĵ�С

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0.0f) glRotatef(angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//�������ŵı���

	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//����Ϊָ������ɫ

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	drawInside();

	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL || m_pShader != NULL) glPopAttrib();

	if(pTexData != NULL) 
	{//�����ֳ�
		m_textureData = tempText;
		tempText.setNoInited();
	}
}
void _XSprite::drawWithoutBlend(const GLuint *pTexnum)
{
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

//	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
//	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, wr << 1, 0, hr << 1, -1, 1);	//������ͼ�Ĵ�С

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0) glRotatef(angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//�������ŵı���
	
	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//����Ϊָ������ɫ

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	drawInside();

	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL || m_pShader != NULL) glPopAttrib();
}
void _XSprite::drawWithoutBlend(const _XTextureData *pTexData)
{
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//�����ֳ������ﱸ�ݵ����ݲ����������ݶ����ݣ����ԱȽϱ���
		tempText = m_textureData;
		m_textureData = *pTexData;
		//���¼��к���ת
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//�����߼��ϴ����ظ����õ����⣬��Ҫ���߼������������ֹ�ظ��ļ��㷴������
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

//	glBlendFunc(srcBlendMode[m_blendTypeScr],dstBlendMode[m_blendTypeDst]);
//	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);	//������ͼ�Ĵ�С

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(m_positionX + m_transformCenter.x,m_positionY + m_transformCenter.y, 0);

	if(angle != 0.0f) glRotatef(angle, 0, 0, 1);	//����δָ���ĽǶ�
	if(xsize != 1.0f || ysize != 1.0f) glScalef(xsize, ysize, 0);	//�������ŵı���

	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);	//����Ϊָ������ɫ

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_thisDrawArea;
#endif
	drawInside();

	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL || m_pShader != NULL) glPopAttrib();

	if(pTexData != NULL) 
	{//�����ֳ�
		m_textureData = tempText;
		tempText.setNoInited();
	}
}
void _XSprite::draw(const GLuint *pTexnum)
{//ͼƬ����	
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
#if IS_USE_SOLIDIFY
	if(m_needSolidify != 0)
	{//��Ҫ�̻�
		glNewList(m_glListOrder,GL_COMPILE_AND_EXECUTE);
		drawBasic(pTexnum);
		glEndList();
		m_needSolidify = 0;
	}else
	{//����Ҫ�̻�
		glCallList(m_glListOrder);	
	}
#else
	drawBasic(pTexnum);
#endif
}
void _XSprite::draw(const _XTextureData *pTexData)
{//ͼƬ����	
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
#if IS_USE_SOLIDIFY
	if(m_isUseSolidify != 0)
	{//ʹ�ù̻�
		if(m_needSolidify != 0)
		{//��Ҫ�̻�
			glNewList(m_glListOrder,GL_COMPILE_AND_EXECUTE);
			drawBasic(pTexData);
			glEndList();
			m_needSolidify = 0;
		}else
		{//����Ҫ�̻�
			glCallList(m_glListOrder);	
		}
	}else
	{
		drawBasic(pTexData);
	}
#else
	drawBasic(pTexData);
#endif
}
void _XSprite::drawEx(const GLuint *pTexnum)
{//ͼƬ����	
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
		
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	int halfW;
	int halfH;
	halfW = m_textureData.texture.m_w >> 1;
	halfH = m_textureData.texture.m_h >> 1;
	glTranslatef(m_positionX + halfW, m_positionY + halfH, 0);

	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, (* pTexnum));
		else glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}
	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
		glTexCoord2f(xp, yp);
		glVertex2f(-halfW, -halfH);
		glTexCoord2f(xp + m_textureData.texture.m_w, yp);
		glVertex2f(halfW, -halfH);
		glTexCoord2f(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
		glVertex2f(halfW, halfH);
		glTexCoord2f(xp, yp + m_textureData.texture.m_h);
		glVertex2f(-halfW, halfH);
	glEnd();																		
	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

//	glDisable(GL_BLEND);
	if(m_pShaderProc != NULL || m_pShader != NULL) glPopAttrib();
}
void _XSprite::drawEx(const _XTextureData *pTexData)
{//ͼƬ����
	if(m_isVisible == 0) return;	//����ʾ��ʱ��ֱ�ӷ���
	if(xsize <= 0 || ysize <= 0 || alpha <= 0) return;
	_XTextureData tempText;
	if(pTexData != NULL) 
	{//�����ֳ������ﱸ�ݵ����ݲ����������ݶ����ݣ����ԱȽϱ���
		tempText = m_textureData;
		m_textureData = *pTexData;
		//���¼��к���ת
		updateClipAndRotatoData();
		//setPosition(m_setPosition);	//�����߼��ϴ����ظ����õ����⣬��Ҫ���߼������������ֹ�ظ��ļ��㷴������
	}
	if(m_needUpdateInsideData != 0)
	{
		m_needUpdateInsideData = 0;
		m_needUpdatePointArray = 0;
		updateInsideData();
		updatePointArray();
	}else
	if(m_needUpdatePointArray != 0)
	{
		m_needUpdatePointArray = 0;
		updatePointArray();
	}

	if(m_pShaderProc != NULL || m_pShader != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
		
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,wr << 1, 0,hr << 1, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	int halfW;
	int halfH;
	halfW = m_textureData.texture.m_w >> 1;
	halfH = m_textureData.texture.m_h >> 1;
	glTranslatef(m_positionX + halfW, m_positionY + halfH, 0);

	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

	if(m_pShaderProc == NULL && m_pShader == NULL)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textureData.texture.m_texture);

		if(m_pShaderProc != NULL) m_pShaderProc();
		if(m_pShader != NULL) m_pShader->useShader();

		glActiveTexture(GL_TEXTURE0);
	}

	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_textureData.texture.m_w * m_textureData.texture.m_h;
#endif
	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
		glTexCoord2f(xp, yp);
		glVertex2f(-halfW, -halfH);
		glTexCoord2f(xp + m_textureData.texture.m_w, yp);
		glVertex2f(halfW, -halfH);
		glTexCoord2f(xp + m_textureData.texture.m_w, yp + m_textureData.texture.m_h);
		glVertex2f(halfW, halfH);
		glTexCoord2f(xp, yp + m_textureData.texture.m_h);
		glVertex2f(-halfW, halfH);
	glEnd();																		
	if(m_pShaderProc != NULL || m_pShader != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	if(m_pShaderProc != NULL) glPopAttrib();
//	glDisable(GL_BLEND);
	if(pTexData != NULL) 
	{//�����ֳ�
		m_textureData = tempText;
		tempText.setNoInited();
	}
}
void _XSprite::setLeft2Right()
{
	if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT) return;	//��ֹ�ظ��ļ���
	m_turnOverMode = TURN_OVER_MODE_LEFT_TO_RIGHT;
	//���ҷ�ת
	m_shapeData[0] = 0.0f;m_shapeData[1] = 0.0f;
	m_shapeData[2] = 1.0f;m_shapeData[3] = 0.0f;

	m_shapeData[4] = 1.0f;m_shapeData[5] = 0.0f;
	m_shapeData[6] = 0.0f;m_shapeData[7] = 0.0f;

	m_shapeData[8] = 1.0f;m_shapeData[9] = 1.0f;
	m_shapeData[10] = 0.0f;m_shapeData[11] = 1.0f;

	m_shapeData[12] = 0.0f;m_shapeData[13] = 1.0f;
	m_shapeData[14] = 1.0f;m_shapeData[15] = 1.0f;
	//setPosition(m_setPosition);
	m_needUpdateInsideData = 1;
}
void _XSprite::setUp2Down()
{
	if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN) return;	//��ֹ�ظ��ļ���
	m_turnOverMode = TURN_OVER_MODE_UP_TO_DOWN;
	//���·�ת
	m_shapeData[0] = 0.0f;m_shapeData[1] = 0.0f;
	m_shapeData[2] = 0.0f;m_shapeData[3] = 1.0f;

	m_shapeData[4] = 1.0f;m_shapeData[5] = 0.0f;
	m_shapeData[6] = 1.0f;m_shapeData[7] = 1.0f;

	m_shapeData[8] = 1.0f;m_shapeData[9] = 1.0f;
	m_shapeData[10] = 1.0f;m_shapeData[11] = 0.0f;

	m_shapeData[12] = 0.0f;m_shapeData[13] = 1.0f;
	m_shapeData[14] = 0.0f;m_shapeData[15] = 0.0f;
	//setPosition(m_setPosition);
	m_needUpdateInsideData = 1;
}
_XSprite::_XSprite()
:m_objType(OBJ_SPRITE)
,alpha(1.0f)
,m_isInited(0)
//,m_isOverturn(0)	//Ĭ������²���ת
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_resoursePosition(RESOURCE_LOCAL_FOLDER)
#if IS_USE_SOLIDIFY
,m_glListOrder(-1)
,m_needSolidify(0)		//Ĭ���������Ҫ�̻�
#endif
,m_isEnableOutsideChip(0)
,m_isACopy(0)
,m_transformCenter(0.0f,0.0f)
,m_changeTransformCenterManual(0)
,m_clipOutsideRect(0,0,100,100)
,m_blendType(BLEND_TWO_DATA)
,m_blendTypeScr(4)
,m_blendTypeDst(5)
,m_blendRGBScr(4)
,m_blendRGBDst(5)
,m_blendAScr(4)
,m_blendADst(5)
,m_pShaderProc(NULL)
,m_pShader(NULL)
,m_turnOverMode(TURN_OVER_MODE_NULL)
,m_needUpdateInsideData(0)
,m_needUpdatePointArray(0)
,m_needAngleClip(0)
,m_isVisible(1)
,m_isTransformCenter(POINT_LEFT_TOP)
{
	m_shapeData[0] = 0.0f; m_shapeData[1] = 0.0f;	//�������״����������
	m_shapeData[2] = 0.0f; m_shapeData[3] = 0.0f;	//�������״����������
	m_shapeData[4] = 1.0f; m_shapeData[5] = 0.0f;	//�������״����������
	m_shapeData[6] = 1.0f; m_shapeData[7] = 0.0f;	//�������״����������
	m_shapeData[8] = 1.0f; m_shapeData[9] = 1.0f;	//�������״����������
	m_shapeData[10] = 1.0f; m_shapeData[11] = 1.0f;	//�������״����������
	m_shapeData[12] = 0.0f; m_shapeData[13] = 1.0f;	//�������״����������
	m_shapeData[14] = 0.0f; m_shapeData[15] = 1.0f;	//�������״����������

//	m_frameName = createArrayMem<char>(MAX_SPRITE_FILENAME_LENGTH);
	setIsTransformCenter(POINT_CENTER);
//	m_isTransformCenter = POINT_CENTER;
}
void _XSprite::setIsTransformCenter(_XTransformMode temp)	//����Ϊ������ת
{
	if(m_isTransformCenter == temp) return;	//��ֹ�ظ��ļ���
	if(temp != POINT_CENTER)
	{
		m_isTransformCenter = POINT_LEFT_TOP;
		if(m_textureData.isEnableInsideClip == 0)
		{
			m_transformCenter.set(0.0f,0.0f);
		}else
		{//��һ��������
			m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
		}
	}else
	{
		m_isTransformCenter = POINT_CENTER;
		if(m_textureData.isEnableInsideClip == 0)
		{
			if(m_textureData.texture.getIsEnable() == 0)
			{
				m_transformCenter.set(0.0f,0.0f);
			}else
			{
				m_transformCenter.set(m_textureData.texture.m_w * 0.5f,m_textureData.texture.m_h * 0.5f);
			}
		}else
		{//��һ��ò��������
			m_transformCenter.set(m_clipOutsideRect.getWidth() * 0.5f + m_textureData.clipInsideRect.left - m_textureData.textureMove.x - m_clipRect.left,
				m_clipOutsideRect.getHeight() * 0.5f + m_textureData.clipInsideRect.top - m_textureData.textureMove.y - m_clipRect.top);
		}
	}
	m_needUpdateInsideData = 1;
}
void _XSprite::setACopy(const _XSprite& temp)
{
	if(& temp == this) return;	//��ֹ����ֵ
	angle = temp.angle;					//����ĽǶ�
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	m_isVisible = temp.m_isVisible;

	m_blendType = temp.m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	m_blendRGBScr = temp.m_blendRGBScr;
	m_blendRGBDst = temp.m_blendRGBDst;
	m_blendAScr = temp.m_blendAScr;
	m_blendADst = temp.m_blendADst;
	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_turnOverMode = temp.m_turnOverMode;
	for(int i = 0;i < 16;++ i)
	{
		m_shapeData[i] = temp.m_shapeData[i];
	}
	m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_textureData = temp.m_textureData;
	m_positionX = temp.m_positionX;
	m_positionX = temp.m_positionX;							//�����λ��
	m_isTransformCenter = temp.m_isTransformCenter;
	m_transformCenter = temp.m_transformCenter;
	m_changeTransformCenterManual = temp.m_changeTransformCenterManual;
	m_clipRect = temp.m_clipRect;
	m_isEnableOutsideChip = temp.m_isEnableOutsideChip;
	m_clipOutsideRect = temp.m_clipOutsideRect;
	m_setPosition = temp.m_setPosition;
	m_setTransformCenter = temp.m_setTransformCenter;

	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
//	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת
	wr = temp.wr;
	hr = temp.hr;
	xp = temp.xp;
	yp = temp.yp;
//	memcpy(m_frameName,temp.m_frameName,sizeof(char) * MAX_SPRITE_FILENAME_LENGTH);
#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
	m_glListOrder = temp.m_glListOrder;		//��ʾ�б���
	m_needSolidify = temp.m_needSolidify;		//�Ƿ���Ҫ�̻�
#endif

	m_isInited = temp.m_isInited;
	m_needUpdateInsideData = temp.m_needUpdateInsideData;
#if WITH_OBJECT_MANAGER
	if(m_isACopy == 0) _XObjManger.addAObject(this,OBJ_SPRITE);
#endif
	m_isACopy = 1;
}
_XSprite& _XSprite::operator = (const _XSprite& temp)
{
	if(& temp == this) return *this;	//��ֹ����ֵ
	angle = temp.angle;					//����ĽǶ�
	sinAngle = temp.sinAngle;					//����ĽǶ�
	cosAngle = temp.cosAngle;					//����ĽǶ�
	m_isVisible = temp.m_isVisible;					//����ĽǶ�

	m_blendType = temp.m_blendType;	//��Ϸ�ʽ��0���򵥻��2���ӻ�ϡ�1�����ӵ�4���ӻ��
	m_blendRGBScr = temp.m_blendRGBScr;
	m_blendRGBDst = temp.m_blendRGBDst;
	m_blendAScr = temp.m_blendAScr;
	m_blendADst = temp.m_blendADst;
	m_blendTypeScr = temp.m_blendTypeScr;
	m_blendTypeDst = temp.m_blendTypeDst;
	m_turnOverMode = temp.m_turnOverMode;
	for(int i = 0;i < 16;++ i)
	{
		m_shapeData[i] = temp.m_shapeData[i];
	}
	m_pShaderProc = temp.m_pShaderProc;
	m_pShader = temp.m_pShader;
	m_textureData = temp.m_textureData;
	m_positionX = temp.m_positionX;
	m_positionX = temp.m_positionX;							//�����λ��
	m_isTransformCenter = temp.m_isTransformCenter;
	m_transformCenter = temp.m_transformCenter;
	m_changeTransformCenterManual = temp.m_changeTransformCenterManual;
	m_clipRect = temp.m_clipRect;
	m_isEnableOutsideChip = temp.m_isEnableOutsideChip;
	m_clipOutsideRect = temp.m_clipOutsideRect;
	m_setPosition = temp.m_setPosition;
	m_setTransformCenter = temp.m_setTransformCenter;

	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
//	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת
	wr = temp.wr;
	hr = temp.hr;
	xp = temp.xp;
	yp = temp.yp;
//	memcpy(m_frameName,temp.m_frameName,sizeof(char) * MAX_SPRITE_FILENAME_LENGTH);

#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
	m_glListOrder = temp.m_glListOrder;		//��ʾ�б���
	m_needSolidify = temp.m_needSolidify;		//�Ƿ���Ҫ�̻�
#endif
	m_isInited = temp.m_isInited;
	m_needUpdateInsideData = temp.m_needUpdateInsideData;
#if WITH_OBJECT_MANAGER
	if(m_isACopy == 0) _XObjManger.addAObject(this,OBJ_SPRITE);
#endif
	m_isACopy = 1;
	return *this;
}
_XSprite::_XSprite(const _XSprite& temp)
{
	if(& temp == this) return;	//��ֹ����ֵ
}
void _XSprite::updateInsideData()
{
	if(m_textureData.isEnableInsideClip == 0)
	{
		m_positionX = m_setPosition.x;
		m_positionY = m_setPosition.y;
	}else
	{
		//����x����
		if(m_clipOutsideRect.left <= m_textureData.textureMove.x)
		{
			if(m_turnOverMode == TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left;
				}else
				{
					m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
						+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
						+ m_clipOutsideRect.top * ysize * sinAngle;
				}
			}else
			{//���ڷ�ת����ת����������
				if(m_isTransformCenter == POINT_LEFT_TOP)
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove.x - m_clipOutsideRect.left) * (1.0f - xsize)
								+ (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}else
						{//ok	//�ϱ߽��Ƿ�Խ��Ӧ�û�������
							m_positionX = m_setPosition.x + (m_textureData.textureMove.x - m_clipOutsideRect.left) * (1.0f - xsize)
								+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ (m_textureData.textureMove.x - m_clipOutsideRect.left - (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right))) 
									* xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN) 
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left;
							}else
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top) * ysize * sinAngle;
							}
						}else
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) 
										* ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left	//T2D ok
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
										* ysize * sinAngle;
							}
						}
					}
				}else
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
					{//ok !
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + m_textureData.textureMove.x * (1.0f - xsize)
								+ m_clipOutsideRect.top * ysize * sinAngle
								+ m_textureData.textureMove.x * xsize * (1.0f - cosAngle);
						}else
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ m_textureData.textureMove.x * (1.0f - xsize)
								+ m_clipOutsideRect.top * ysize * sinAngle
								- (m_textureData.textureMove2.x - m_textureData.textureMove.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) 
									* xsize * (1.0f - cosAngle);
						}
					}else//L2R 
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok !
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
								+ m_clipOutsideRect.top * ysize * sinAngle
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle);
						}else
						{
							m_positionX = m_setPosition.x + m_textureData.textureMove.x - m_clipOutsideRect.left * xsize
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
									* ysize * sinAngle
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle);
						}
					}
				}
			}
		}else
		{
			if(m_turnOverMode == TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionX = m_setPosition.x;
				}else
				{
					m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
						+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
						+ m_clipOutsideRect.top * ysize * sinAngle;
				}
			}else
			{//���ڷ�ת����ת����������
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x;
						}else
						{//ok
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right))  * xsize
								- (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * (1.0f - cosAngle)
								+ 0.0f * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN) //T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x;
							}else
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top)* ysize * sinAngle;
							}
						}else
						{
							if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))
										* ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x
									+ 0.0f * xsize * (1.0f - cosAngle)
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom) - (m_textureData.textureMove.y - m_clipOutsideRect.top))
										* ysize * sinAngle;
							}
						}
					}
				}else 
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)
					{
						if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
						{//ok
							m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
								+ m_clipOutsideRect.top * ysize * sinAngle;
						}else
						{//ok +
							m_positionX = m_setPosition.x + (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize
								+ m_clipOutsideRect.left * (1.0f - xsize) 
								+ (m_textureData.textureSize.x - m_clipOutsideRect.right - (m_textureData.textureMove2.x - m_clipOutsideRect.left)) 
								* xsize * (1.0f - cosAngle)
								+ m_clipOutsideRect.top * ysize * sinAngle;
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok
						if(m_clipOutsideRect.top >= m_textureData.textureMove.y)
						{
							m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
								+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
									* ysize * sinAngle;
						}else
						{
							if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
							{
								m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
									+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
									+ m_textureData.textureMove.y * ysize * sinAngle;
							}else
							{
								m_positionX = m_setPosition.x + m_clipOutsideRect.left * (1.0f - xsize)
									+ m_clipOutsideRect.left * xsize * (1.0f - cosAngle)
									+ (m_textureData.textureMove.y - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
										* ysize * sinAngle;
							}
						}
					}
				}
			}
		}
		//����y����
		if(m_clipOutsideRect.top <= m_textureData.textureMove.y)
		{
			if(m_turnOverMode == TURN_OVER_MODE_NULL)
			{
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top;
				}else 
				{
					m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
						- m_clipOutsideRect.left * xsize * sinAngle
						+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
				}
			}else
			{//���ڷ�ת����ת����������
				if(m_isTransformCenter == POINT_LEFT_TOP)
				{
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN) 
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize)
								+ 0.0f * xsize * sinAngle
								+ (m_textureData.textureMove.y - m_clipOutsideRect.top) * ysize * (1.0f - cosAngle);
						}else
						{//ok
							if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
							{
								m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize) 
									+ (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
									+ 0.0f * xsize * sinAngle
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
										* ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y + (m_textureData.textureMove.y - m_clipOutsideRect.top) * (1.0f - ysize) 
									+ (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
									+ 0.0f * xsize * sinAngle
									+ (m_textureData.textureMove.y - m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
										* ysize * (1.0f - cosAngle);
							}
						}
					}else//T2Dʱ�����
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F ok 
					{//2k
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{
							m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
								+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * sinAngle
								+ 0.0f * ysize * (1.0f - cosAngle);
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
									- (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top
									- (m_textureData.textureMove.x - m_clipOutsideRect.left - (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)))
										* xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}
					}
				}else 
				{
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + m_textureData.textureMove.y * (1.0f - ysize)
								+ m_textureData.textureMove.y * ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}else
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
								+ m_textureData.textureMove.y * (1.0f - ysize)
								+ (m_textureData.textureMove.y - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)))
									* ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F ok
					{
					//	if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
					//	{//ok
							m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
								+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * ysize * sinAngle;
					//	}else
					//	{
					//		m_positionY = m_setPosition.y + m_textureData.textureMove.y - m_clipOutsideRect.top * ysize
					//			+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle)
					//			- m_clipOutsideRect.left * ysize * sinAngle;
					//	}
					}
				}
			}
		}else
		{
			if(m_turnOverMode == TURN_OVER_MODE_NULL)
			{//û�з�ת
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					m_positionY = m_setPosition.y;
				}else 
				{
					m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
						- m_clipOutsideRect.left * xsize * sinAngle
						+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
				}
			}else
			{//���ڷ�ת����ת�����������
				if(m_isTransformCenter == POINT_LEFT_TOP) 
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F
					{//���������ת�Ƕ�ʱ���������
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y;
							}else
							{
								m_positionY = m_setPosition.y
									+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{
								m_positionY = m_setPosition.y
									- (m_textureData.textureMove.x - m_clipOutsideRect.left) * xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}else
							{
								m_positionY = m_setPosition.y
									+ (m_textureData.textureMove2.x - (m_textureData.textureSize.x - m_clipOutsideRect.right) - (m_textureData.textureMove.x - m_clipOutsideRect.left)) 
										* xsize * sinAngle
									+ 0.0f * ysize * (1.0f - cosAngle);
							}
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{
						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y;// + m_textureData.textureMove2.y * ysize;
						}else
						{//ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))  * ysize
									+ 0.0f * xsize * sinAngle
									- (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize * (1.0f - cosAngle);
						}
					}
				}else 
				{
					if(m_turnOverMode == TURN_OVER_MODE_LEFT_TO_RIGHT)	//L2F
					{//ok!
						if(m_clipOutsideRect.left >= m_textureData.textureMove.x)
						{//ok +
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- m_clipOutsideRect.left * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}else
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- (m_textureData.textureSize.x - m_clipOutsideRect.right - (m_textureData.textureMove2.x - m_clipOutsideRect.left)) 
										 * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}
						}else
						{
							if(m_textureData.textureSize.x - m_clipOutsideRect.right >= m_textureData.textureMove2.x)
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									- m_textureData.textureMove.x * xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}else
							{//ok
								m_positionY = m_setPosition.y + m_clipOutsideRect.top *(1.0f - ysize)
									+ (m_textureData.textureMove2.x - m_textureData.textureMove.x - (m_textureData.textureSize.x - m_clipOutsideRect.right)) 
										* xsize * sinAngle
									+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
							}
						}
					}else
					if(m_turnOverMode == TURN_OVER_MODE_UP_TO_DOWN)	//T2D ok
					{//ok !
 						if(m_textureData.textureSize.y - m_clipOutsideRect.bottom >= m_textureData.textureMove2.y)
						{//ok
							m_positionY = m_setPosition.y + m_clipOutsideRect.top * (1.0f - ysize) 
								- m_clipOutsideRect.left * xsize * sinAngle
								+ m_clipOutsideRect.top * ysize * (1.0f - cosAngle);
						}else	//��ʹ��m_textureData.textureMove2.y��Ϊ�ü�ʱ
						{ // ok
							m_positionY = m_setPosition.y + (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom)) * ysize
								+ m_clipOutsideRect.top * (1.0f - ysize) 
								+ (m_clipOutsideRect.top - (m_textureData.textureMove2.y - (m_textureData.textureSize.y - m_clipOutsideRect.bottom))) 
									* ysize * (1.0f - cosAngle)
								- m_clipOutsideRect.left * xsize * sinAngle;
						}
					}
				}
			}
		}
	}
}
void _XSprite::updateClipAndRotatoData()
{
	if(m_isEnableOutsideChip != 0)
	{
		setClipRect(m_clipOutsideRect);
	}else
	{
		setClipRect(0.0f,0.0f,m_textureData.textureSize.x,m_textureData.textureSize.y);
		m_isEnableOutsideChip = 0;
		m_needUpdateInsideData = 1;
	}
}
void _XSprite::setClipRect(float left,float top,float right,float bottom)
{
	m_isEnableOutsideChip = 1;
	//�������ж�
	if(left < 0) left = 0;
	if(top < 0) top = 0;
	if(right > m_textureData.texture.m_w) right = m_textureData.texture.m_w;
	if(bottom > m_textureData.texture.m_h) bottom = m_textureData.texture.m_h;

	if(left > m_textureData.texture.m_w)
	{
		left = m_textureData.texture.m_w;
		right = m_textureData.texture.m_w;
	}

	if(bottom > m_textureData.texture.m_h)
	{
		top = m_textureData.texture.m_h;
		bottom = m_textureData.texture.m_h;
	}

	m_clipOutsideRect.set(left,top,right,bottom);
	if(m_textureData.isEnableInsideClip != 0)
	{//����ڲ��Ѿ������ݽ����˲ü������������ƴ��
		if(left < m_textureData.textureMove.x) left = m_textureData.clipInsideRect.left;
		else left += m_textureData.clipInsideRect.left - m_textureData.textureMove.x;
		if(top < m_textureData.textureMove.y) top = m_textureData.clipInsideRect.top;
		else top += m_textureData.clipInsideRect.top - m_textureData.textureMove.y;
		if(right > m_textureData.clipInsideRect.getWidth() + m_textureData.textureMove.x) right = m_textureData.clipInsideRect.right;
		else right += m_textureData.clipInsideRect.left - m_textureData.textureMove.x;
		if(bottom > m_textureData.clipInsideRect.getHeight() + m_textureData.textureMove.y) bottom = m_textureData.clipInsideRect.bottom;
		else bottom += m_textureData.clipInsideRect.top - m_textureData.textureMove.y;
	}

	m_clipRect.set(left,top,right,bottom);	
	if(m_changeTransformCenterManual == 0)
	{
		if(m_isTransformCenter == POINT_CENTER)
		{
			if(m_textureData.isEnableInsideClip != 0)
			{
				m_transformCenter.set(m_clipOutsideRect.getWidth() * 0.5f + m_textureData.clipInsideRect.left - m_textureData.textureMove.x - m_clipRect.left,
					m_clipOutsideRect.getHeight() * 0.5f + m_textureData.clipInsideRect.top - m_textureData.textureMove.y - m_clipRect.top);
				//setPosition(m_setPosition);
			}else
			{
				m_transformCenter.set(m_clipRect.getWidth() * 0.5f,m_clipRect.getHeight() * 0.5f);
			}
		}else
		{
			if(m_textureData.isEnableInsideClip != 0)
			{
				if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x;
				else m_transformCenter.x = 0;
				if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y;
				else m_transformCenter.y = 0; 
				//setPosition(m_setPosition);
		//		m_transformCenter.set(-m_textureData.textureMove.x,-m_textureData.textureMove.y);
			}else
			{
				m_transformCenter.set(0.0f,0.0f);
			}
		}
	}else
	{
		if(m_textureData.isEnableInsideClip != 0)
		{
			if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y; 
			//setPosition(m_setPosition);
		}
	}
	m_needUpdateInsideData = 1;
}
int _XSprite::release()
{
	if(m_isInited == 0) return 1;
#if IS_USE_SOLIDIFY
	if(m_isACopy == 0)
	{
		if(m_glListOrder >=0) glDeleteLists(m_glListOrder,1);
		m_glListOrder = -1;
	}
#endif
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	m_isACopy = 0;
	m_isInited = 0;
	return 1;
}
_XSprite::~_XSprite()
{
	release();
//	XDELETE_ARRAY(m_frameName);
	//m_isInited = 0;
}
_XVector2 _XSprite::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(order < 0 || order >= 4) return ret;
	if(m_isTransformCenter == POINT_LEFT_TOP)
	{//���ϽǶ���
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//����Ҫ�ü�
			if(order == 0) ret.set(xp + m_setPosition.x,yp + m_setPosition.y); else
			if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * xsize * cosAngle, 
				yp + m_setPosition.y + m_textureData.texture.m_w * xsize * sinAngle); else
			if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * xsize * cosAngle + m_textureData.texture.m_h * ysize * (- sinAngle), 
				yp + m_setPosition.y + m_textureData.texture.m_w * xsize * sinAngle + m_textureData.texture.m_h * ysize * cosAngle); else
			if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.texture.m_h * ysize * (- sinAngle), 
				yp + m_setPosition.y + m_textureData.texture.m_h * ysize * cosAngle);
		}else
		{//��Ҫ�ü�
			if(m_isEnableOutsideChip == 0)
			{
				if(order == 0) ret.set(xp + m_setPosition.x,
					yp + m_setPosition.y); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * xsize * cosAngle, 
					yp + m_setPosition.y + m_textureData.textureSize.x * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * xsize * cosAngle + m_textureData.textureSize.y * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_textureData.textureSize.x * xsize * sinAngle + m_textureData.textureSize.y * ysize * cosAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.textureSize.y * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_textureData.textureSize.y * ysize * cosAngle);
			}else
			{
				if(order == 0) ret.set(xp + m_setPosition.x,
					yp + m_setPosition.y); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * xsize * cosAngle, 
					yp + m_setPosition.y + m_clipOutsideRect.getWidth() * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * xsize * cosAngle + m_clipOutsideRect.getHeight() * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * ysize * cosAngle + m_clipOutsideRect.getWidth() * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getHeight() * ysize * (- sinAngle), 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * ysize * cosAngle);
			}
		}
	}else
	{//���Ķ��䣬��ת����û�и㶨
		if(m_isEnableOutsideChip == 0 && m_textureData.isEnableInsideClip == 0)
		{//����Ҫ�ü�
			if(order == 0) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f - 0.5f * xsize * cosAngle) + m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f - 0.5f * ysize * cosAngle) - m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f + 0.5f * xsize * cosAngle) + m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f - 0.5f * ysize * cosAngle) + m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f + 0.5f * xsize * cosAngle) - m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f + 0.5f * ysize * cosAngle) + m_textureData.texture.m_w * 0.5f * xsize * sinAngle); else
			if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.texture.m_w * (0.5f - 0.5f * xsize * cosAngle) - m_textureData.texture.m_h * 0.5f * ysize * sinAngle,
				yp + m_setPosition.y + m_textureData.texture.m_h * (0.5f + 0.5f * ysize * cosAngle) - m_textureData.texture.m_w * 0.5f * xsize * sinAngle);
		}else
		{//��Ҫ�ü�
			if(m_isEnableOutsideChip == 0)
			{
				if(order == 0) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f - 0.5f * xsize * cosAngle) + m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f - 0.5f * ysize * cosAngle) - m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f + 0.5f * xsize * cosAngle) + m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f - 0.5f * ysize * cosAngle) + m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f + 0.5f * xsize * cosAngle) - m_textureData.textureSize.y * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f + 0.5f * ysize * cosAngle) + m_textureData.textureSize.x * 0.5f * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_textureData.textureSize.x * (0.5f - 0.5f * xsize * cosAngle) - m_textureData.textureSize.y * 0.5f * ysize * sinAngle, 
					yp + m_setPosition.y + m_textureData.textureSize.y * (0.5f + 0.5f * ysize * cosAngle) - m_textureData.textureSize.x * 0.5f * xsize * sinAngle);
			}else
			{
				if(order == 0) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f - 0.5f * xsize * cosAngle) + m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f - 0.5f * ysize * cosAngle) - m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 1) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f + 0.5f * xsize * cosAngle) + m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle, 
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f - 0.5f * ysize * cosAngle) + m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 2) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f + 0.5f * xsize * cosAngle) - m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f + 0.5f * ysize * cosAngle) + m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle); else
				if(order == 3) ret.set(xp + m_setPosition.x + m_clipOutsideRect.getWidth() * (0.5f - 0.5f * xsize * cosAngle) - m_clipOutsideRect.getHeight() * 0.5f * ysize * sinAngle,
					yp + m_setPosition.y + m_clipOutsideRect.getHeight() * (0.5f + 0.5f * ysize * cosAngle) - m_clipOutsideRect.getWidth() * 0.5f * xsize * sinAngle);
			}
		}
	}
	return ret;
}
#endif