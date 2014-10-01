//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XProgress.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XProgressTexture::_XProgressTexture()
:m_isInited(XFalse)
,progressBackGround(NULL)			//�������ı�����ͼ
,progressMove(NULL)				//���������ƶ��仯����ͼ
,progressUpon(NULL)				//���������ϲ�����ֻ��������Ч
{}
_XBool _XProgressTexture::init(const char *backgound,const char *move,const char *upon,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		move == NULL) return XFalse;	//���������ٱ���Ҫ���м��ƶ�����ͼ
	int ret = 1;
	if(backgound != NULL && 
		(progressBackGround = createATextureData(backgound,resoursePosition)) == NULL) ret = 0;
	if(move != NULL && ret != 0 && 
		(progressMove = createATextureData(move,resoursePosition)) == NULL) ret = 0;
	if(upon != NULL && ret != 0 &&
		(progressUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XDELETE(progressBackGround);
		XDELETE(progressMove);
		XDELETE(progressUpon);
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define PROGRESS_CONFIG_FILENAME ("Progress.txt")
_XBool _XProgressTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,PROGRESS_CONFIG_FILENAME);
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//�ⲿ��Դ
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //��Ϣ�ļ���ȡʧ��
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//normal
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressBackGround = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//down
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE(progressBackGround);
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressMove = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE(progressBackGround);
			fclose(fp);
			return XFalse;
		}
		//on
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE(progressBackGround);
			XDELETE(progressMove);
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE(progressBackGround);
			XDELETE(progressMove);
			fclose(fp);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b,x,y;
		if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
		m_mouseRect.set(l,t,r,b);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_fontPosition.set(x,y);
		//�������ݶ�ȡ���
		fclose(fp);
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//normal
		int offset = 0;
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressBackGround = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE(progressBackGround);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressMove = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE(progressBackGround);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//on
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE(progressBackGround);
			XDELETE(progressMove);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((progressUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE(progressBackGround);
			XDELETE(progressMove);
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b,x,y;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(x,y);
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XProgress::init(const _XVector2& position,//�ؼ����ڵ�λ��
		const _XRect &Area,	//����������ķ�Χ
		const _XProgressTexture &tex,	//�ؼ�����ͼ
		_XNumber *font,float captionSize,const _XVector2& textPosition,	//�ؼ�������
		int mode)
{
	if(m_isInited ||
		Area.getHeight() <= 0 || Area.getWidth() <= 0 ||	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
		tex.progressMove == NULL ||//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
		captionSize < 0) return XFalse;
	m_mouseRect = Area;
	m_position = position;

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_progressBackGround = tex.progressBackGround;		
	m_progressMove = tex.progressMove;				
	m_progressUpon = tex.progressUpon;	
	m_withoutTex = XFalse;

	m_nowValue = 0.0f;
	if(font == NULL) m_isShowFont = XFalse;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_caption);
#endif
		m_isShowFont = XTrue;
	}
	m_size.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}

	//��ʼ��������ʾ����Ϣ
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(NUMBER_ALIGNMENT_MODE_MIDDLE);
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	//��ʼ����ʾ�ľ���
	m_spriteBackground.init(m_progressBackGround->texture.m_w,m_progressBackGround->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackground);
#endif
	m_spriteBackground.setPosition(m_position);
	m_spriteBackground.setSize(m_size);
	m_spriteBackground.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_progressMove->texture.m_w,m_progressMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setPosition(m_position);
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);
	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + 1.0f,m_mouseRect.bottom);

	m_spriteUpon.init(m_progressUpon->texture.m_w,m_progressUpon->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteUpon);
#endif
	m_spriteUpon.setPosition(m_position);
	m_spriteUpon.setSize(m_size);
	m_spriteUpon.setIsTransformCenter(POINT_LEFT_TOP);
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_spriteBackground.setAngle(90);
		m_spriteMove.setAngle(90);
		m_spriteUpon.setAngle(90);
	}

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	//ע������ؼ�
	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XProgress::initWithoutTex(const _XRect &area,
	_XNumber* font,float captionSize,	//�ؼ�������
	const _XVector2& textPosition,int mode)
{
	if(m_isInited ||
		captionSize < 0) return XFalse;
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_withoutTex = XTrue;

	m_nowValue = 0.0f;
	if(font == NULL) m_isShowFont = XFalse;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_caption);
#endif
		m_isShowFont = XTrue;
	}
	m_size.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}

	//��ʼ��������ʾ����Ϣ
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(NUMBER_ALIGNMENT_MODE_MIDDLE);
//	m_textPosition = tex->m_fontPosition;
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	//��ʼ����ʾ�ľ���
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
//		m_spriteBackground.setAngle(90);
//		m_spriteMove.setAngle(90);
//		m_spriteUpon.setAngle(90);
	}

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	//ע������ؼ�
	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isInited = XTrue;
	return XTrue;
}
void _XProgress::setSize(float x,float y)
{
	if(x <= 0 || y <= 0) return;
	m_size.set(x,y);
	if(!m_withoutTex)
	{
		m_spriteBackground.setSize(m_size);
		m_spriteMove.setSize(m_size);
		m_spriteUpon.setSize(m_size);
	}
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
	m_caption.setSize(m_size);
}
void _XProgress::setPosition(float x,float y)
{
	m_position.set(x,y);
	if(!m_withoutTex)
	{
		m_spriteBackground.setPosition(m_position);
		m_spriteMove.setPosition(m_position);
		m_spriteUpon.setPosition(m_position);
	}
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
}
void _XProgress::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
			_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
			0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
		drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
			_XVector2(m_mouseRect.getWidth() * m_nowValue/100.0f * m_size.x,m_mouseRect.getHeight() * m_size.y),
			0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA);
		if(m_isShowFont) m_caption.draw();	//��ʾ����
	}else
	{
		if(m_progressBackGround != NULL) m_spriteBackground.draw(m_progressBackGround);
		if(m_progressMove != NULL) m_spriteMove.draw(m_progressMove);
		if(m_isShowFont) m_caption.draw();	//��ʾ����
		if(m_progressUpon != NULL) m_spriteUpon.draw(m_progressUpon);
	}
}
void _XProgress::setValue(float temp)
{
	if(!m_isInited) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
	int tempI = temp;
	if(tempI == (int)m_nowValue) return;
	m_nowValue = temp;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",tempI);
	m_caption.setNumber(tempC);
	if(!m_withoutTex)
		m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + m_mouseRect.getWidth() * m_nowValue/100.0f,m_mouseRect.bottom);
}
_XProgress::_XProgress()
:m_isInited(XFalse)
,m_progressBackGround(NULL)		
,m_progressMove(NULL)			
,m_progressUpon(NULL)		
,m_nowValue(0.0f)
,m_isShowFont(XTrue)
,m_isACopy(XFalse)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_PROGRESS;
}
void _XProgress::release()
{
	if(!m_isInited) return;
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XProgress::setACopy(const _XProgress &temp)		//����һ������
{
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;

	m_nowValue = temp.m_nowValue;					//�������ĵ�ǰֵ
	if(!m_isInited)
	{//���û�о�����ʼ���ͽ��и����Ļ�������Ҫע��ؼ�
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	m_mode = temp.m_mode;					//�������ĵ�ǰֵ
	if(temp.m_isShowFont)
	{
		if(!m_caption.setACopy(temp.m_caption))	return XFalse;				//�������ı���
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_caption);
#endif
	}
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_progressBackGround = temp.m_progressBackGround;	//�������ı�����ͼ
	m_progressMove = temp.m_progressMove;			//���������ƶ��仯����ͼ
	m_progressUpon = temp.m_progressUpon;			//���������ϲ�����ֻ��������Ч

	m_spriteBackground.setACopy(temp.m_spriteBackground);	//������ʾ�����ľ���
	m_spriteMove.setACopy(temp.m_spriteMove);			//������ʾ��ͼ�ľ���
	//m_movePosition = temp.m_movePosition;			//����������ڱ�����λ��
	m_spriteUpon.setACopy(temp.m_spriteUpon);			//������ʾ�ϲ����ֵľ���
	//m_uponPosition = temp.m_uponPosition;			//��������ڱ�����λ��
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackground);
	_XObjManger.decreaseAObject(&m_spriteMove);
	_XObjManger.decreaseAObject(&m_spriteUpon);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ��
	m_textSize = temp.m_textSize;				//���ֵĳߴ�
	m_textColor = temp.m_textColor;				//���ֵĳߴ�

	m_isShowFont = temp.m_isShowFont;				//�Ƿ���ʾ��������
	m_isACopy = XTrue;
	return XTrue;
}