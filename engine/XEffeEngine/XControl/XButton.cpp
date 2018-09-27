#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButton.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XButtonSkin::XButtonSkin()
:m_isInited(XFalse)
,buttonNormal(NULL)			//��ͨ״̬�µİ�ť��ͼ
,buttonDown(NULL)			//����״̬�µİ�ť��ͼ
,buttonOn(NULL)				//����״̬�µİ�ť��ͼ
,buttonDisable(NULL)		//��Ч״̬�µİ�ť��ͼ
,m_areaPointSum(0)
,m_pArea(NULL)
{}
XBool XButtonSkin::init(const char *normal,const char *down,const char *on,const char *disable,XResPos resPos)
{
	if(m_isInited) return XFalse;
	if(normal == NULL) return XFalse;
	int ret = 1;

	if(normal != NULL && 
		(buttonNormal = createATextureData(normal,resPos)) == NULL) ret = 0;
	if(down != NULL && ret != 0 && 
		(buttonDown = createATextureData(down,resPos)) == NULL) ret = 0;
	if(on != NULL && ret != 0 && 
		(buttonOn = createATextureData(on,resPos)) == NULL) ret = 0;
	if(disable != NULL && ret != 0 && 
		(buttonDisable = createATextureData(disable,resPos)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define BUTTON_CONFIG_FILENAME "Button.txt"
bool XButtonSkin::loadFromFolder(const char *filename,XResPos resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDown = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonOn = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDisable = createATextureData(tempFilename,resPos)) == NULL)
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
		m_pArea = XMem::createArrayMem<XVec2>(m_areaPointSum);	//rational:MLK
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
	return true;
}
bool XButtonSkin::loadFromPacker(const char *filename,XResPos resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;
	//���濪ʼ���ζ�ȡ����
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	int offset = 0;
	//normal
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDown = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//on
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonOn = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//disable
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((buttonDisable = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//��ȡ��������
	int l,t,r,b,x,y;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_fontPosition.set(x,y);
	//�����ȡ�������Ϣ
	if(sscanf((char *)(p + offset),"%d,",&m_areaPointSum) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	if(m_areaPointSum > 0)
	{
		m_pArea = XMem::createArrayMem<XVec2>(m_areaPointSum);
		if(m_pArea == NULL)
		{
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
		//�����ȡ����������
		for(int i = 0;i < m_areaPointSum;++ i)
		{
			if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XMem::XDELETE_ARRAY(p);return XFalse;}
			offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
			m_pArea[i].set(x,y);
		}
	}
	//�������ݶ�ȡ���
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XButtonSkin::loadFromWeb(const char *filename,XResPos resPos)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XButtonSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	//�ȴ������ļ�
	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename,resPos)) return false;
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resPos)) return false;
		break;
	case RES_WEB:
		if(!loadFromWeb(filename,resPos)) return false;
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename,resPos) && !loadFromFolder(filename,resPos) &&
			!loadFromWeb(filename,resPos)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}

XButton::XButton()
	:m_isInited(XFalse)
	, m_buttonNormal(NULL)
	, m_buttonDown(NULL)
	, m_buttonOn(NULL)
	, m_buttonDisable(NULL)
	, m_hotKey(-1)
	, m_curButtonState(BUTTON_STATE_NORMAL)
	, m_upMousePoint(-1.0f)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	, m_withNormalIcon(XFalse)
	, m_withDisableIcon(XFalse)
	, m_iconSize(1.0f)
	, m_iconPosition(0.0f)
	, m_symbolType(BTN_SYMBOL_NULL)
	, m_buttonStyle(BTN_STYLE_NORMAL)
	, m_isCheck(false)
{
	m_ctrlType = CTRL_OBJ_BUTTON;
}
void XButton::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = false;
}
void XButton::setTexture(const XButtonSkin& tex)
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
XBool XButton::initProc(const XFontUnicode& font,const char *caption,float captionSize)
{
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.set(0.0f,1.0f);
	if(m_curButtonState == BUTTON_STATE_DISABLE) m_caption.setColor((m_textColor * m_color).anti());
	else m_caption.setColor(m_textColor * m_color);							//�����������ɫΪ��ɫ

	m_scale.set(1.0f);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	m_textSize.set(captionSize);
	m_caption.setScale(m_textSize * m_scale);

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);

	m_curButtonState = BUTTON_STATE_NORMAL;

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_INIT);
	else XCtrlManager.eventProc(m_objectID,BTN_INIT);

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	return XTrue;
}
XBool XButton::init(const XVec2& position,	//�ؼ����ڵ�λ��
		const XRect& Area,				//�ؼ������Ӧ������
		const XButtonSkin &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const XFontUnicode& font,float captionSize,const XVec2& textPosition)	//�ؼ�����������Ϣ
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
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//������ж��룬���������λ����Ҫ����
	m_isInited = XTrue;
	return XTrue;
}
XBool XButton::initWithoutSkin(const char *caption,const XFontUnicode& font,
		float captionSize,const XRect& area,const XVec2& textPosition)	//û����ͼ����ʽ,ֱ����д����ͼ������ͼ(��δʵ�֣�δ��ɹ���֮һ)
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0 ||
		area.getHeight() <= 0 || area.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_comment.init();
	m_mouseRect = area;
	m_position.reset();
	m_textPosition = textPosition;
	m_withoutTex = XTrue;

	if(!initProc(font,caption,captionSize)) return XFalse;						//�����������ɫΪ��ɫ

	m_isInited = XTrue;
	return XTrue;
}
void XButton::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if(m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}
	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
	if(!m_withoutTex) m_sprite.setPosition(tmpPos);

	if(m_withNormalIcon) m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
	if(m_withDisableIcon) m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);

	mouseProc(m_upMousePoint,MOUSE_MOVE);
	updateChildPos();
}
void XButton::setScale(const XVec2& s)
{
	if(!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if(m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}
	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
	m_caption.setScale(m_textSize * tmpSize);
	if(!m_withoutTex) m_sprite.setScale(tmpSize);

	if (m_withNormalIcon)
	{
		m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
		m_normalIcon.setScale(m_iconSize * tmpSize);
	}
	if (m_withDisableIcon)
	{
		m_normalIcon.setPosition(tmpPos + m_iconPosition * tmpSize);
		m_disableIcon.setScale(m_iconSize * tmpSize);
	}

	mouseProc(m_upMousePoint,MOUSE_MOVE);
	updateChildScale();
}
XBool XButton::mouseProc(const XVec2& p,XMouseState mouseState)
{
	m_upMousePoint = p;
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible) return XFalse; 	//������ɼ�ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;
	
	if(m_curMouseRect.isInRect(p))
	{//��궯���ڷ�Χ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
		}
		m_comment.updatePosition(p + XVec2(0.0f, 16.0f));
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//�����������������˵������ʧ
		if(m_isEnable)
		{
			if(mouseState == MOUSE_MOVE && m_curButtonState == BUTTON_STATE_NORMAL)
			{
				m_curButtonState = BUTTON_STATE_ON;

				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_ON);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_ON);

				if(m_withAction)
				{//�������һ����̬Ч��
					m_isInAction = XTrue;
					m_actionMoveData.set(1.0f,1.1f,0.02f,MD_MODE_SIN_MULT,1);
					m_lightMD.set(1.0f,2.0f,0.002f,MD_MODE_SIN_MULT);
					m_oldPos = m_position;
					m_oldSize = m_scale;
				}
			}else
			if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && 
				(m_curButtonState == BUTTON_STATE_NORMAL || m_curButtonState == BUTTON_STATE_ON))
			{
				m_curButtonState = BUTTON_STATE_DOWN;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_DOWN);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_DOWN);
			}else
			if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curButtonState == BUTTON_STATE_DOWN)
			{
				m_curButtonState = BUTTON_STATE_ON;
				if(m_buttonStyle == BTN_STYLE_CHECK)
					m_isCheck = !m_isCheck;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,BTN_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID,BTN_MOUSE_UP);
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
		if(m_isEnable &&
			(m_curButtonState == BUTTON_STATE_ON || m_curButtonState == BUTTON_STATE_DOWN))
		{
			m_curButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
XBool XButton::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XTrue;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;

	if(keyState == KEY_STATE_UP &&	//��������ʱ������Ӧ
		m_curButtonState == BUTTON_STATE_NORMAL &&	//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
		(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose)))
	{
		if(m_buttonStyle == BTN_STYLE_CHECK)
			m_isCheck = !m_isCheck;
		if(m_eventProc != NULL)
		{
			m_eventProc(m_pClass,m_objectID,BTN_MOUSE_DOWN);
			m_eventProc(m_pClass,m_objectID,BTN_MOUSE_UP);
		}else
		{
			XCtrlManager.eventProc(m_objectID,BTN_MOUSE_DOWN);
			XCtrlManager.eventProc(m_objectID,BTN_MOUSE_UP);
		}
		return XTrue;
	}
	return XFalse;
}
void XButton::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		XVec2 tmpPos = m_position;
		XVec2 tmpSize = m_scale;
		if(m_isInAction && !m_actionMoveData.getIsEnd())
		{
			tmpPos = m_mouseRect.getSize() * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
			tmpPos = m_position - tmpPos;
			tmpSize = m_actionMoveData.getCurData() * m_scale;
		}

		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::downColor * m_color,true);//,true); 
			//		m_mouseRect.getHeight()) * tmpSize,0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);//,true); 
			else
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::normalColor * m_color,true);//,true); 
			//		m_mouseRect.getHeight()) * tmpSize,0.75f * m_color.r,0.75f * m_color.g,0.75f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_DOWN:
			XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
				m_mouseRect.getSize() * tmpSize,
				XCCS::downColor * m_color,true);//,true); 
		//		m_mouseRect.getHeight()) * tmpSize,0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_ON:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::downColor * m_color,true);//,true);
			//		m_mouseRect.getHeight()) * tmpSize,0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a,true);//,true); 
			else
				XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
					m_mouseRect.getSize() * tmpSize,
					XCCS::onColor * m_color,true);//,true);
			//		m_mouseRect.getHeight()) * tmpSize,0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a,true);//,true); 
			break;
		case BUTTON_STATE_DISABLE:
			XRender::drawFillRectExA(tmpPos + m_mouseRect.getLT() * tmpSize,
				m_mouseRect.getSize() * tmpSize,
				XCCS::disableColor * m_color,true,false,true);
		//		m_mouseRect.getHeight()) * tmpSize,0.35f * m_color.r,0.35f * m_color.g,0.35f * m_color.b,m_color.a,true);//,true); 
			break;
		}
	}else
	{
		switch(m_curButtonState)
		{
		case BUTTON_STATE_NORMAL:
			if(m_buttonStyle == BTN_STYLE_CHECK && m_isCheck)
				m_sprite.draw(m_buttonDown);
			else
				m_sprite.draw(m_buttonNormal);
			break;
		case BUTTON_STATE_DOWN:m_sprite.draw(m_buttonDown);break;
		case BUTTON_STATE_ON:m_sprite.draw(m_buttonOn);break;
		case BUTTON_STATE_DISABLE:m_sprite.draw(m_buttonDisable);break;
		}
	}
	//��ʾ��ť������
	m_caption.draw();
	if(m_curButtonState == BUTTON_STATE_DISABLE)
	{
		if(m_withDisableIcon) m_disableIcon.draw();else
		if(m_withNormalIcon) m_normalIcon.draw();
	}else
	{
		if(m_withNormalIcon) m_normalIcon.draw();else
		if(m_withDisableIcon) m_disableIcon.draw();
	}
	//������ʾ����
	if(m_symbolType != BTN_SYMBOL_NULL)
	{
		float tmpSize = (std::min)(m_curMouseRect.getHeight(),m_curMouseRect.getWidth()) * 0.5f * 0.65f;
		if(tmpSize <= 0.0f) tmpSize = 1.0f;
		switch(m_symbolType)
		{
		case BTN_SYMBOL_LINE:		//����
			XRender::drawLine(m_caption.getPosition() + XVec2(-tmpSize,0.0f),
				m_caption.getPosition() + XVec2(tmpSize,0.0f),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_CIRCLE:		//Բ��
			XRender::drawCircleLine(m_caption.getPosition(),tmpSize,36,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_RECT:		//����
			XRender::drawRect(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_CROSS:		//ʮ�ּ�
			XRender::drawCross(m_caption.getPosition(),tmpSize,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_TRIANGLE:	//������
			XRender::drawTriangle(m_caption.getPosition(),tmpSize,2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_MASK_RIGHT:	//�Ժ�
			XRender::drawMaskRight(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_MASK_WRONG:	//���
			XRender::drawMaskWrong(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_LEFT:		//��
			XRender::drawLeft(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_RIGHT:		//��
			XRender::drawRight(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_UP:			//��
			XRender::drawUp(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_DOWN:		//��
			XRender::drawDown(m_caption.getPosition(),XVec2(tmpSize),2.0f,m_caption.getColor());
			break;
		case BTN_SYMBOL_SAVE:
			XRender::drawSave(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_LOAD:
			XRender::drawLoad(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_DEF:
			XRender::drawDefault(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_DOWNLOAD:
			XRender::drawDownload(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		case BTN_SYMBOL_UPDATE:
			XRender::drawUpdate(m_caption.getPosition(), XVec2(tmpSize), 2.0f, m_caption.getColor());
			break;
		}
	}
}
XBool XButton::setACopy(const XButton &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited || !XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_curButtonState = temp.m_curButtonState;		//��ǰ�İ�ť״̬

	if(!m_caption.setACopy(temp.m_caption))	 return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif

	m_withNormalIcon = temp.m_withNormalIcon;		//�Ƿ�ӵ����ͨicon
	if(m_withNormalIcon)
	{
		m_normalIcon.setACopy(temp.m_normalIcon);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_normalIcon);
#endif
	}
	m_withDisableIcon = temp.m_withDisableIcon;		//�Ƿ�ӵ����Чicon
	if(m_withDisableIcon)
	{
		m_disableIcon.setACopy(temp.m_disableIcon);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_disableIcon);
#endif
	}
	m_iconSize = temp.m_iconSize;		//icon�ĳߴ�
	m_iconPosition = temp.m_iconPosition;	//icon��λ��

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;			//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;				//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_sprite.setACopy(temp.m_sprite);	//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
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
	m_symbolType = temp.m_symbolType;
	return XTrue;
}
void XButton::setNormalIcon(const char * filename,XResPos resPos)
{
	if(filename == NULL) return;
	if(m_withNormalIcon) m_normalIcon.release();
	if(m_normalIcon.init(filename,resPos,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_normalIcon);
#endif
		m_withNormalIcon = true;

		m_normalIcon.setColor(m_color);
		m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
		m_normalIcon.setScale(m_iconSize * m_scale);
	}
}
void XButton::setNormalIcon(const XSprite &icon)
{
	if(m_withNormalIcon) m_normalIcon.release();
	m_normalIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_normalIcon);
#endif
	m_withNormalIcon = true;

	m_normalIcon.setColor(m_color);
	m_normalIcon.setPosition(m_position + m_iconPosition * m_scale);
	m_normalIcon.setScale(m_iconSize * m_scale);
}
void XButton::setDisableIcon(const char * filename,XResPos resPos)
{
	if(filename == NULL) return;
	if(m_withDisableIcon) m_disableIcon.release();
	if(m_disableIcon.init(filename,resPos,POINT_LEFT_TOP))
	{
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&m_disableIcon);
#endif
		m_withDisableIcon = true;

		m_disableIcon.setColor(m_color);
		m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
		m_disableIcon.setScale(m_iconSize * m_scale);
	}
}
void XButton::setDisableIcon(const XSprite &icon)
{
	if(m_withDisableIcon) m_disableIcon.release();
	m_disableIcon.setACopy(icon);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_disableIcon);
#endif
	m_withDisableIcon = true;

	m_disableIcon.setColor(m_color);
	m_disableIcon.setPosition(m_position + m_iconPosition * m_scale);
	m_disableIcon.setScale(m_iconSize * m_scale);
}
void XButton::removeIcon()	//ȥ�����е�icon
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
void XButton::update(float stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd())
			m_isInAction = false;	//�����������
		setScale(m_scale);
		setPosition(m_position);
	}
	//��ɢЧ���ı���
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVec2 pos = m_oldPos + m_mouseRect.getSize() * m_oldSize * 0.5f;
		XVec2 size = m_mouseRect.getSize() * m_oldSize * m_lightMD.getCurData() * 0.5f;
		m_lightRect.set(pos - size,pos + size);
	}
}
void XButton::setWidth(int width)
{
	if(!m_withoutTex || width <= 0 || !m_isInited) return;	//���ʹ����ͼ��������ֱ�ӷ���
	m_mouseRect.right = m_mouseRect.left + width;
	m_textPosition.x = m_mouseRect.getCenter().x;
	m_caption.setPosition(m_position + m_textPosition * m_scale);

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
}
#if !WITH_INLINE_FILE
#include "XButton.inl"
#endif
}