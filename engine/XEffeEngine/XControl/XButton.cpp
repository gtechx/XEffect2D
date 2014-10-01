//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButton.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XButtonTexture::_XButtonTexture()
:m_isInited(XFalse)
,buttonNormal(NULL)			//��ͨ״̬�µİ�ť��ͼ
,buttonDown(NULL)			//����״̬�µİ�ť��ͼ
,buttonOn(NULL)				//����״̬�µİ�ť��ͼ
,buttonDisable(NULL)		//��Ч״̬�µİ�ť��ͼ
,m_areaPointSum(0)
,m_pArea(NULL)
{}
_XBool _XButtonTexture::init(const char *normal,const char *down,const char *on,const char *disable,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(normal == NULL) return XFalse;
	int ret = 1;

	if(normal != NULL && 
		(buttonNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(down != NULL && ret != 0 && 
		(buttonDown = createATextureData(down,resoursePosition)) == NULL) ret = 0;
	if(on != NULL && ret != 0 && 
		(buttonOn = createATextureData(on,resoursePosition)) == NULL) ret = 0;
	if(disable != NULL && ret != 0 && 
		(buttonDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define BUTTON_CONFIG_FILENAME ("Button.txt")
_XBool _XButtonTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
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
		if((buttonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//down
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((buttonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((buttonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		//�����ȡ�������Ϣ
		if(fscanf(fp,"%d,",&m_areaPointSum) != 1) {fclose(fp);return XFalse;}
		if(m_areaPointSum > 0)
		{
			m_pArea = createArrayMem<_XVector2>(m_areaPointSum);	//rational:MLK
			if(m_pArea == NULL)
			{
				releaseTex();
				fclose(fp);
				return XFalse;
			}
			//�����ȡ����������
			for(int i = 0;i < m_areaPointSum;++ i)
			{
				if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
				m_pArea[i].set(x,y);
			}
		}
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
		if((buttonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((buttonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((buttonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((buttonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		//�����ȡ�������Ϣ
		if(sscanf((char *)(p + offset),"%d,",&m_areaPointSum) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		if(m_areaPointSum > 0)
		{
			m_pArea = createArrayMem<_XVector2>(m_areaPointSum);
			if(m_pArea == NULL)
			{
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
			//�����ȡ����������
			for(int i = 0;i < m_areaPointSum;++ i)
			{
				if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
				offset += getCharPosition((char *)(p + offset),'\n') + 1;
				m_pArea[i].set(x,y);
			}
		}
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}

_XButton::_XButton()
:m_isInited(XFalse)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_funInit(NULL)
,m_funRelease(NULL)
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)
,m_pClass(NULL)
,m_hotKey(-1)
,m_nowButtonState(BUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_withNormalIcon(XFalse)
,m_withDisableIcon(XFalse)
,m_iconSize(1.0f,1.0f)
,m_iconPosition(0.0f,0.0f)
{
	m_ctrlType = CTRL_OBJ_BUTTON;
}
void _XButton::release()
{
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
void _XButton::setTexture(const _XButtonTexture& tex)
{
	if(tex.buttonNormal == NULL) return;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
_XBool _XButton::initProc(const _XFontUnicode &font,const char *caption,float captionSize)
{
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	return XTrue;
}
_XBool _XButton::init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ������Ӧ������
		const _XButtonTexture &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &textPosition)	//�ؼ�����������Ϣ
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_comment.init();
	m_mouseRect = Area;
	m_position = position;
	m_textPosition = textPosition;
	if(tex.buttonNormal == NULL) return XFalse;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_withoutTex = XFalse;

	if(!initProc(font,caption,captionSize)) return XFalse;						//�����������ɫΪ��ɫ

	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//������ж��룬���������λ����Ҫ����
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButton::initWithoutTex(const char *caption,const _XFontUnicode &font,
		float captionSize,const _XRect& area,const _XVector2 &textPosition)	//û����ͼ����ʽ,ֱ����д����ͼ������ͼ(��δʵ�֣�δ��ɹ���֮һ)
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(area.getHeight() <= 0 || area.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_comment.init();
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);
	m_textPosition = textPosition;
	m_withoutTex = XTrue;

	if(!initProc(font,caption,captionSize)) return XFalse;						//�����������ɫΪ��ɫ

	m_isInited = XTrue;
	return XTrue;
}
void _XButton::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);

	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	if(m_withDisableIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);

	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	updateChildPos();
}
void _XButton::setSize(float x,float y)
{
	if(!m_isInited ||
		x <= 0 || y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	if(!m_withoutTex) m_sprite.setSize(m_size);

	if(m_withNormalIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	if(m_withNormalIcon) m_normalIcon.setSize(m_iconSize * m_size);
	if(m_withDisableIcon) m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	if(m_withDisableIcon) m_disableIcon.setSize(m_iconSize * m_size);

	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	updateChildSize();
}
_XBool _XButton::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	
	if(m_nowMouseRect.isInRect(x,y))
	{//��궯���ڷ�Χ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(m_isEnable)
		{
			if(mouseState == MOUSE_MOVE && m_nowButtonState == BUTTON_STATE_NORMAL)
			{
				m_nowButtonState = BUTTON_STATE_ON;
				if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
				if(m_withAction)
				{//�������һ����̬Ч��
					m_isInAction = XTrue;
					m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
					m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
					m_oldPos = m_position;
					m_oldSize = m_size;
				}
			}else
			if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
				(m_nowButtonState == BUTTON_STATE_NORMAL || m_nowButtonState == BUTTON_STATE_ON))
			{
				m_nowButtonState = BUTTON_STATE_DOWN;
				if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
			}else
			if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowButtonState == BUTTON_STATE_DOWN)
			{
				m_nowButtonState = BUTTON_STATE_ON;
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬
			}
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(m_isEnable)
		{
			if(m_nowButtonState == BUTTON_STATE_ON || m_nowButtonState == BUTTON_STATE_DOWN)
			{
				m_nowButtonState = BUTTON_STATE_NORMAL;
			}
		}
	}
	return XTrue;
}
_XBool _XButton::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if(m_nowButtonState == BUTTON_STATE_NORMAL)
		{//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
			if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose))
			{
				if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				return XTrue;
			}
		}
	}
	return XFalse;
}
void _XButton::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		switch(m_nowButtonState)
		{
		case BUTTON_STATE_NORMAL:
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true);//,true); 
			break;
		case BUTTON_STATE_DOWN:
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);//,true); 
			break;
		case BUTTON_STATE_ON:
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA,true);//,true);
			break;
		case BUTTON_STATE_DISABLE:
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.35f * m_color.fR,0.35f * m_color.fG,0.35f * m_color.fB,m_color.fA,true,false,true);
			break;
		}
	}else
	{
		switch(m_nowButtonState)
		{
		case BUTTON_STATE_NORMAL:m_sprite.draw(m_buttonNormal);break;
		case BUTTON_STATE_DOWN:m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON:m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE:m_sprite.draw(m_buttonDisable);break;
		}
	}
	//��ʾ��ť������
	m_caption.draw();
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		if(m_withDisableIcon) m_disableIcon.draw();else
		if(m_withNormalIcon) m_normalIcon.draw();
	}else
	{
		if(m_withNormalIcon) m_normalIcon.draw();else
		if(m_withDisableIcon) m_disableIcon.draw();
	}
}
void _XButton::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;
	m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
_XBool _XButton::setACopy(const _XButton &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_nowButtonState = temp.m_nowButtonState;		//��ǰ�İ�ť״̬

	if(!m_caption.setACopy(temp.m_caption))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif

	m_withNormalIcon = temp.m_withNormalIcon;		//�Ƿ�ӵ����ͨicon
	if(m_withNormalIcon)
	{
		m_normalIcon.setACopy(temp.m_normalIcon);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_normalIcon);
#endif
	}
	m_withDisableIcon = temp.m_withDisableIcon;		//�Ƿ�ӵ����Чicon
	if(m_withDisableIcon)
	{
		m_disableIcon.setACopy(temp.m_disableIcon);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_disableIcon);
#endif
	}
	m_iconSize = temp.m_iconSize;		//icon�ĳߴ�
	m_iconPosition = temp.m_iconPosition;	//icon��λ��

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;			//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;				//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_funInit = temp.m_funInit;
	m_funRelease = temp.m_funRelease;
	m_funMouseOn = temp.m_funMouseOn;
	m_funMouseDown = temp.m_funMouseDown;
	m_funMouseUp = temp.m_funMouseUp;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_sprite.setACopy(temp.m_sprite);	//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textColor = temp.m_textColor;
	m_textSize = temp.m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼�����λ��

	m_actionMoveData = temp.m_actionMoveData;	//��̬Ч���ı���
	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	m_hotKey = temp.m_hotKey;
	return XTrue;
}
void _XButton::setNormalIcon(const char * filename,_XResourcePosition resoursePosition)
{
	if(filename == NULL) return;
	if(m_withNormalIcon) m_normalIcon.release();
	if(m_normalIcon.init(filename,resoursePosition,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_normalIcon);
#endif
		m_withNormalIcon = true;

		m_normalIcon.setColor(m_color);
		m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
		m_normalIcon.setSize(m_iconSize * m_size);
	}
}
void _XButton::setNormalIcon(const _XSprite &icon)
{
	if(m_withNormalIcon) m_normalIcon.release();
	m_normalIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_normalIcon);
#endif
	m_withNormalIcon = true;

	m_normalIcon.setColor(m_color);
	m_normalIcon.setPosition(m_position + m_iconPosition * m_size);
	m_normalIcon.setSize(m_iconSize * m_size);
}
void _XButton::setDisableIcon(const char * filename,_XResourcePosition resoursePosition)
{
	if(filename == NULL) return;
	if(m_withDisableIcon) m_disableIcon.release();
	if(m_disableIcon.init(filename,resoursePosition,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_disableIcon);
#endif
		m_withDisableIcon = true;

		m_disableIcon.setColor(m_color);
		m_disableIcon.setPosition(m_position + m_iconPosition * m_size);
		m_disableIcon.setSize(m_iconSize * m_size);
	}
}
void _XButton::setDisableIcon(const _XSprite &icon)
{
	if(m_withDisableIcon) m_disableIcon.release();
	m_disableIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_disableIcon);
#endif
	m_withDisableIcon = true;

	m_disableIcon.setColor(m_color);
	m_disableIcon.setPosition(m_position + m_iconPosition * m_size);
	m_disableIcon.setSize(m_iconSize * m_size);
}
void _XButton::removeIcon()	//ȥ�����е�icon
{
	if(m_withNormalIcon)
	{
		m_normalIcon.release();
		m_withNormalIcon = false;
	}
	if(m_withDisableIcon)
	{
		m_disableIcon.release();
		m_withDisableIcon = false;
	}
}
void _XButton::update(int stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
		{
			m_isInAction = false;	//�����������
			setSize(m_oldSize);
			setPosition(m_oldPos);
		}else
		{
			setSize(m_actionMoveData.getNowData() * m_oldSize);
			_XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
			tmp = tmp * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize) * 0.5f;
			setPosition(m_oldPos - tmp);
		}
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
			m_oldPos.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}