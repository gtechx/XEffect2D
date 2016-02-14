#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XCheckSkin::XCheckSkin()
:m_isInited(XFalse)
,checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ		
,checkDischoose(NULL)			//ѡ��ťδѡ�е���ͼ		
,checkDisableChoosed(NULL)		//��Ч״̬��ѡ��ťѡ�е���ͼ		
,checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ	
{}
XBool XCheckSkin::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,XResourcePosition resoursePosition)
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
#define CHECK_CONFIG_FILENAME "Check.txt"
bool XCheckSkin::loadFromFolder(const char *filename,XResourcePosition resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,CHECK_CONFIG_FILENAME);
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
	if((checkChoosed = createATextureData(tempFilename,resPos)) == NULL)
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
	if((checkDischoose = createATextureData(tempFilename,resPos)) == NULL)
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
	if((checkDisableChoosed = createATextureData(tempFilename,resPos)) == NULL)
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
	if((checkDisableDischoose = createATextureData(tempFilename,resPos)) == NULL)
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
	return true;
}
bool XCheckSkin::loadFromPacker(const char *filename,XResourcePosition resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,CHECK_CONFIG_FILENAME);
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((checkChoosed = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((checkDischoose = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((checkDisableChoosed = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf(tempFilename,"%s/%s",filename,resFilename);
	if((checkDisableDischoose = createATextureData(tempFilename,resPos)) == NULL)
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
	//�������ݶ�ȡ���
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XCheckSkin::loadFromWeb(const char *filename,XResourcePosition resPos)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XCheckSkin::initEx(const char *filename,XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = getDefResPos();
	switch(resoursePosition)
	{
	case RESOURCE_LOCAL_PACK:
		if(!loadFromPacker(filename,resoursePosition)) return false;
		break;
	case RESOURCE_LOCAL_FOLDER:
		if(!loadFromFolder(filename,resoursePosition)) return false;
		break;
	case RESOURCE_WEB:
		if(!loadFromWeb(filename,resoursePosition)) return false;
		break;
	case RESOURCE_AUTO:
		if(!loadFromPacker(filename,resoursePosition) && !loadFromFolder(filename,resoursePosition) &&
			!loadFromWeb(filename,resoursePosition)) return false;
		break;
	}
	m_isInited = XTrue;
	return XTrue;
}

XCheck::XCheck()
:m_isInited(XFalse)					//�������Ƿ񱻳�ʼ��
,m_checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ
,m_checkDischoose(NULL)		//ѡ��ťδѡ�е���ͼ
,m_checkDisableChoosed(NULL)	//��Ч״̬��ѡ��ťѡ�е���ͼ
,m_checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ
,m_state(XFalse)
,m_withCaption(XTrue)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_style(CHK_STYLE_NORMAL)
{
	m_ctrlType = CTRL_OBJ_CHECK;
}
void XCheck::release()
{
	XCtrlManager.decreaseAObject(this);	//ע��������
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CHK_RELEASE);
	else XCtrlManager.eventProc(m_objectID,CHK_RELEASE);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//ע�⣺����Ĭ���ĸ�״̬����ͼ�ߴ�һ�£����򽫻�������벻��������
XBool XCheck::init(const XVector2 & position,	//�ؼ���λ��
		const XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const XCheckSkin &tex,			//�ؼ�����ͼ
		const char *caption,const XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const XVector2 &textPosition)		//�����λ��
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

	if(!m_caption.setACopy(font)) return XFalse;			//�������ı���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_scale.set(1.0f,1.0f);
	m_textPosition = textPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setScale(m_scale);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CHK_INIT);
	else XCtrlManager.eventProc(m_objectID,CHK_INIT);

	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XCheck::initWithoutSkin(const char *caption,const XFontUnicode &font,
	float captionSize,const XRect& area,const XVector2 &captionPosition)
{//��δ���
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;
	m_comment.init();

	if(!m_caption.setACopy(font)) return XFalse;			//�������ı���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_scale.set(1.0f,1.0f);
	m_textPosition = captionPosition;		//���ֵ����λ��

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CHK_INIT);
	else XCtrlManager.eventProc(m_objectID,CHK_INIT);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XCheck::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		if(m_state)
		{
			switch (m_style)
			{
			case XCheck::CHK_STYLE_NORMAL:
				if(m_isEnable) 
				{//����X
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::normalColor * m_color,true);
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f - XVector2(2.0f,2.0f);
					XRender::drawMaskRight(m_position + tmpSize + XVector2(2.0f,2.0f),tmpSize,2.0f,XCCS::downColor * m_color);
				}else 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true);
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f - XVector2(2.0f,2.0f);
					XRender::drawMaskRight(m_position + tmpSize + XVector2(2.0f,2.0f),tmpSize,2.0f,XCCS::blackDownColor * m_color);
				}
				break;
			case XCheck::CHK_STYLE_BOX:
				if(m_isEnable) 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::normalColor * m_color,true,false,true);
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f - XVector2(4.0f,4.0f);
					XRender::drawFillBoxA(m_position + XVector2(4.0f,4.0f),tmpSize * 2.0f,XCCS::mouseColor * m_color);
				}else 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true,false,true);
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f - XVector2(4.0f,4.0f);
					XRender::drawFillBoxA(m_position + XVector2(4.0f,4.0f),tmpSize * 2.0f,XCCS::mouseColor * m_color);
				}
				break;
			case XCheck::CHK_STYLE_MINI:
				if(m_isEnable) 
				{
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f;
					XRender::drawCircleLineEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.5f,1.0f,XCCS::downColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x - 2.0f,tmpSize.y - 2.0f) * 0.5f,XCCS::normalColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.25f,XCCS::mouseColor * m_color);
				}else 
				{
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f;
					XRender::drawCircleLineEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.5f,1.0f,XCCS::downColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x - 2.0f,tmpSize.y - 2.0f) * 0.5f,XCCS::downColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.25f,XCCS::mouseColor * m_color);
				}
				break;
			}
		}else
		{
			switch (m_style)
			{
			case CHK_STYLE_NORMAL:
				if(m_isEnable) 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::onColor * m_color,true);
				}else 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true);
				}
				break;
			case CHK_STYLE_BOX:
				if(m_isEnable) 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::onColor * m_color,true,false,true);
				}else 
				{
					XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
						XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true,false,true);
				}
				break;
			case CHK_STYLE_MINI:
				if(m_isEnable) 
				{
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f;
					XRender::drawCircleLineEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.5f,1.0f,XCCS::downColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x - 2.0f,tmpSize.y - 2.0f) * 0.5f,XCCS::onColor * m_color);
				}else 
				{
					XVector2 tmpSize = XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_scale * 0.5f;
					XRender::drawCircleLineEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x,tmpSize.y) * 0.5f,1.0f,XCCS::downColor * m_color);
					XRender::drawCircleEx(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y) + tmpSize,
						min(tmpSize.x - 2.0f,tmpSize.y - 2.0f) * 0.5f,XCCS::downColor * m_color);
				}
				break;
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
void XCheck::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	updateChildPos();
}
void XCheck::setScale(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_scale.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	if(!m_withoutTex) m_sprite.setScale(m_scale);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	updateChildScale();
}
XBool XCheck::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if((m_withCaption && m_mouseClickArea.isInRect(x,y)) || m_curMouseRect.isInRect(x,y))
	{//������Ч��ִ����ز���
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//�����������������˵������ʧ
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN)
		{
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;

			if(m_eventProc != NULL)
			{
				m_eventProc(m_pClass,m_objectID,CHK_MOUSE_UP);
				m_eventProc(m_pClass,m_objectID,CHK_STATE_CHANGE);
			}else 
			{
				XCtrlManager.eventProc(m_objectID,CHK_MOUSE_UP);
				XCtrlManager.eventProc(m_objectID,CHK_STATE_CHANGE);
			}
			m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬
			stateChange();
			if(m_withAction)
			{//�������һ����̬Ч��
			//	m_isInAction = XTrue;
				m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_scale;
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
XBool XCheck::keyboardProc(int keyOrder,XKeyState keyState)
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
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,CHK_STATE_CHANGE);
			else XCtrlManager.eventProc(m_objectID,CHK_STATE_CHANGE);
			stateChange();
		}
	}
	return XTrue;
}
XBool XCheck::setACopy(const XCheck &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!XControlBasic::setACopy(temp)) return XFalse;

	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//�������ı���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif

	m_pVariable = temp.m_pVariable;
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_checkChoosed = temp.m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = temp.m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_sprite);
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
XVector2 XCheck::getBox(int order)
{
	if(!m_isInited) return XVector2::zero;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption)
	{
		if(m_curMouseRect.left <= m_mouseClickArea.left) left = m_curMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_curMouseRect.top <= m_mouseClickArea.top) top = m_curMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_curMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_curMouseRect.right;
		if(m_curMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_curMouseRect.bottom;
	}else
	{
		left = m_curMouseRect.left;
		top = m_curMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	switch(order)
	{
	case 0: return XVector2(left,top);
	case 1: return XVector2(right,top);
	case 2: return XVector2(right,bottom);
	case 3: return XVector2(left,bottom);
	}

	return XVector2::zero;
}
void XCheck::update(float stepTime)
{
	m_comment.update(stepTime);
	//if(m_isInAction)
	//{//���ڶ��������м��㶯����ʵʩ
	//	m_actionMoveData.move(stepTime);
	//	if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//�����������
	//	setSize(m_actionMoveData.getCurData() * m_oldSize);
	//	XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
	//	tmp = tmp * (m_actionMoveData.getCurData() * m_oldSize - m_oldSize) * 0.5f;
	//	setPosition(m_oldPos - tmp);
	//}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_scale.x,m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_scale.y);
		XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}
void XCheck::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex && !m_lightMD.getIsEnd())
	{
		switch (m_style)
		{
		case CHK_STYLE_NORMAL:
		case CHK_STYLE_BOX:
			XRender::drawRect(m_lightRect,1.0f * m_lightMD.getCurData() * 2.0f,1.0f,1.0f,1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f);
			break;
		case CHK_STYLE_MINI:
			XRender::drawCircleLineEx(m_lightRect.getCenter(),min(m_lightRect.getWidth(),m_lightRect.getHeight()) * 0.5f,
				1.0f * m_lightMD.getCurData() * 2.0f,XFColor(1.0f,1.0f,1.0f,(1.0f - m_lightMD.getCurTimer()) * 0.5f));
			break;
		}
	}
	m_comment.draw();
}
#if !WITH_INLINE_FILE
#include "XCheck.inl"
#endif
}