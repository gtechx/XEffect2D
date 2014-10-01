//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XCheckTexture::_XCheckTexture()
:m_isInited(XFalse)
,checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ		
,checkDischoose(NULL)			//ѡ��ťδѡ�е���ͼ		
,checkDisableChoosed(NULL)		//��Ч״̬��ѡ��ťѡ�е���ͼ		
,checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ	
{}
_XBool _XCheckTexture::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	//ע����������״̬����ͼ������Ҫ�У����򽫲��ܳ�ʼ��
	if(choosed == NULL || disChoose == NULL || disableChoosed == NULL || disableDischoose == NULL) return XFalse;
	int ret = 1;
	
	if((checkChoosed = createATextureData(choosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDischoose = createATextureData(disChoose,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableChoosed = createATextureData(disableChoosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableDischoose = createATextureData(disableDischoose,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define CHECK_CONFIG_FILENAME ("Check.txt")
_XBool _XCheckTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,CHECK_CONFIG_FILENAME);
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//�ⲿ��Դ
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //��Ϣ�ļ���ȡʧ��
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//chooose
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//dischoose
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//on
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//disable
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
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
		int offset = 0;
		//normal
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
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//on
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//disable
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
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

_XCheck::_XCheck()
:m_isInited(XFalse)					//�������Ƿ񱻳�ʼ��
,m_checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ
,m_checkDischoose(NULL)		//ѡ��ťδѡ�е���ͼ
,m_checkDisableChoosed(NULL)	//��Ч״̬��ѡ��ťѡ�е���ͼ
,m_checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ
,m_funInit(NULL)				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funRelease(NULL)				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funMouseOn(NULL)				//�������ʱ����
,m_funMouseDown(NULL)			//��갴��ʱ����		
,m_funMouseUp(NULL)				//��굯��ʱ����
,m_funStateChange(NULL)			//�ؼ�״̬�ı�ʱ����
,m_pClass(NULL)
,m_state(XFalse)
,m_withCaption(XTrue)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_CHECK;
}
void _XCheck::release()
{
	_XCtrlManger.decreaseAObject(this);	//ע��������
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//ע�⣺����Ĭ���ĸ�״̬����ͼ�ߴ�һ�£����򽫻�������벻��������
_XBool _XCheck::init(const _XVector2 & position,	//�ؼ���λ��
		const _XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const _XVector2 &textPosition)		//�����λ��
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;
	m_mouseRect = Area;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return XFalse;
	m_checkChoosed = tex.checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = tex.checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = tex.checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = tex.checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ
	m_withoutTex = XFalse;
	m_comment.init();

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = textPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XCheck::initWithoutTex(const char *caption,const _XFontUnicode &font,
	float captionSize,const _XRect& area,const _XVector2 &captionPosition)
{//��δ���
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;
	m_comment.init();

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = captionPosition;		//���ֵ����λ��

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XCheck::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		if(m_state)
		{
			if(m_isEnable) 
			{//����X
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			}else 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
			}
		}else
		{
			if(m_isEnable) 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA,true);
			}else 
			{
				drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			}
		}
	}else
	{
		if(m_state)
		{
			if(m_isEnable) m_sprite.draw(m_checkChoosed);
			else m_sprite.draw(m_checkDisableChoosed);
		}else
		{
			if(m_isEnable) m_sprite.draw(m_checkDischoose);
			else m_sprite.draw(m_checkDisableDischoose);
		}
	}
	if(m_withCaption) m_caption.draw();	//�����Ҫ�������������
}
void _XCheck::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildPos();
}
void _XCheck::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	if(!m_withoutTex) m_sprite.setSize(m_size);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildSize();
}
_XBool _XCheck::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if((m_withCaption && m_mouseClickArea.isInRect(x,y)) || m_nowMouseRect.isInRect(x,y))
	{//������Ч��ִ����ز���
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState == MOUSE_LEFT_BUTTON_UP)
		{
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬
			stateChange();
			if(m_withAction)
			{//�������һ����̬Ч��
			//	m_isInAction = XTrue;
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_size;
			}
			return XTrue;
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	return XFalse;
}
_XBool _XCheck::keyboardProc(int keyOrder,_XKeyState keyState)
{//�س����߿ո���Ըı�����ؼ���ѡ��״̬
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if((keyOrder == XKEY_RETURN || keyOrder == XKEY_SPACE) && m_isBeChoose)
		{//���¿ո�����߻س�������Ч
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
			stateChange();
		}
	}
	return XTrue;
}
void _XCheck::setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass)
{//ʵ����������Բ������жϣ��������������ע��
	m_funInit = funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funRelease = funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funMouseOn = funMouseOn;				//�������ʱ����
	m_funMouseDown = funMouseDown;			//��갴��ʱ����		
	m_funMouseUp = funMouseUp;				//��굯��ʱ����
	m_funStateChange = funStateChange;			//�ؼ�״̬�ı�ʱ����
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
_XBool _XCheck::setACopy(const _XCheck &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;

	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif

	m_pVariable = temp.m_pVariable;
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_checkChoosed = temp.m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = temp.m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	m_funInit = temp.m_funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funRelease = temp.m_funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funMouseOn = temp.m_funMouseOn;				//�������ʱ���ã������Ŀǰû��ʵ����Ч��
	m_funMouseDown = temp.m_funMouseDown;			//��갴��ʱ���ã������Ŀǰû��ʵ����Ч��		
	m_funMouseUp = temp.m_funMouseUp;				//��굯��ʱ����
	m_funStateChange = temp.m_funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = temp.m_pClass;

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;		//���ֵ����λ��
	m_textSize = temp.m_textSize;			//���ֵĳߴ�

	m_state = temp.m_state;				//��ѡ���ѡ��״̬
	m_textColor = temp.m_textColor;				//��ѡ���ѡ��״̬
	m_mouseClickArea = temp.m_mouseClickArea;	//���������Ӧ��Χ

	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	return XTrue;
}
_XVector2 _XCheck::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption)
	{
		if(m_nowMouseRect.left <= m_mouseClickArea.left) left = m_nowMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_nowMouseRect.top <= m_mouseClickArea.top) top = m_nowMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_nowMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_nowMouseRect.right;
		if(m_nowMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_nowMouseRect.bottom;
	}else
	{
		left = m_nowMouseRect.left;
		top = m_nowMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	switch(order)
	{
	case 0: return _XVector2(left,top);
	case 1: return _XVector2(right,top);
	case 2: return _XVector2(right,bottom);
	case 3: return _XVector2(left,bottom);
	}

	return _XVector2::zero;
}
void _XCheck::update(int stepTime)
{
	m_comment.update(stepTime);
	//if(m_isInAction)
	//{//���ڶ��������м��㶯����ʵʩ
	//	m_actionMoveData.move(stepTime);
	//	if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//�����������
	//	setSize(m_actionMoveData.getNowData() * m_oldSize);
	//	_XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
	//	tmp = tmp * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize) * 0.5f;
	//	setPosition(m_oldPos - tmp);
	//}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_size.x,m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_size.y);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}