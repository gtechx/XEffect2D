//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSlider.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XSliderTexture::_XSliderTexture()
:m_isInited(XFalse)
,sliderLineNormal(NULL)			
,sliderLineDown(NULL)			
,sliderLineOn(NULL)					
,sliderLineDisable(NULL)			
,sliderButtonNormal(NULL)			
,sliderButtonDown(NULL)			
,sliderButtonOn(NULL)					
,sliderButtonDisable(NULL)			
{}
_XBool _XSliderTexture::init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
						  const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		LNormal == NULL || BNormal == NULL) return XFalse;

	int ret = 1;
	//�ֱ�������Դ	
	if((sliderLineNormal = createATextureData(LNormal,resoursePosition)) == NULL) ret = 0;
	if(LDown != NULL && ret != 0 &&
		(sliderLineDown = createATextureData(LDown,resoursePosition)) == NULL) ret = 0;
	if(LOn != NULL && ret != 0 &&
		(sliderLineOn = createATextureData(LOn,resoursePosition)) == NULL) ret = 0;
	if(LDisable != NULL && ret != 0 &&
		(sliderLineDisable = createATextureData(LDisable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(sliderButtonNormal = createATextureData(BNormal,resoursePosition)) == NULL) ret = 0;
	if(BDown != NULL && ret != 0 &&
		(sliderButtonDown = createATextureData(BDown,resoursePosition)) == NULL) ret = 0;
	if(BOn != NULL && ret != 0 &&
		(sliderButtonOn = createATextureData(BOn,resoursePosition)) == NULL) ret = 0;
	if(BDisable != NULL && ret != 0 &&
		(sliderButtonDisable = createATextureData(BDisable,resoursePosition)) == NULL) ret = 0;
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define SLIDER_CONFIG_FILENAME ("Slider.txt")
_XBool _XSliderTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
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
		if((sliderLineNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//down
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//on
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//disable
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((sliderButtonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				fclose(fp);
				return XFalse;
			}
		}
		//��ȡ��������
		int l,t,r,b;
		if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
		m_mouseRect.set(l,t,r,b);
		if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
		m_mouseRectButton.set(l,t,r,b);
		if(fscanf(fp,"%d,%d,",&l,&t) != 2) {fclose(fp);return XFalse;}
		m_fontPosition.set(l,t);
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
		if((sliderLineNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//on
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//disable
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderLineDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//normal
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
		if((sliderButtonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((sliderButtonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		//��ȡ��������
		int l,t,r,b;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRectButton.set(l,t,r,b);
		if(sscanf((char *)(p + offset),"%d,%d,",&l,&t) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(l,t);
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
_XSlider::_XSlider()
:m_isInited(XFalse)
,m_nowSliderState(SLIDER_STATE_NORMAL)
,m_typeVorH(SLIDER_TYPE_VERTICAL)
,m_sliderLineNormal(NULL)			
,m_sliderLineDown(NULL)			
,m_sliderLineOn(NULL)			
,m_sliderLineDisable(NULL)		
,m_sliderButtonNormal(NULL)			
,m_sliderButtonDown(NULL)	
,m_sliderButtonOn(NULL)				
,m_sliderButtonDisable(NULL)		
,m_funInit(NULL)	
,m_funRelease(NULL)	
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)	
,m_funMouseMove(NULL)
,m_funValueChange(NULL)
,m_pClass(NULL)
,m_minValue(0)	
,m_maxValue(100)	
,m_nowValue(0)		
,m_mouseFlyArea(50,50)	//���ֵʹ��һ��ϵͳ�趨�����ֵ�����ڴ����н����޸ģ����ǲ�����ģ����ǿ��ǵ������ڵ������ԣ�����Ĭ��������������
,m_needFont(XFalse)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_textColor(0.0f,0.0f,0.0f,1.0f)
{
	m_ctrlType = CTRL_OBJ_SLIDER;
}
void _XSlider::release()
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
void _XSlider::setTexture(const _XSliderTexture &tex)
{
	m_sliderLineNormal = tex.sliderLineNormal;
	m_sliderButtonNormal = tex.sliderButtonNormal;
	if(tex.sliderLineDown != NULL) m_sliderLineDown = tex.sliderLineDown;
	else m_sliderLineDown = tex.sliderLineNormal;
	if(tex.sliderLineOn != NULL) m_sliderLineOn = tex.sliderLineOn;
	else m_sliderLineOn = tex.sliderLineNormal;
	if(tex.sliderLineDisable != NULL) m_sliderLineDisable = tex.sliderLineDisable;
	else m_sliderLineDisable = tex.sliderLineNormal;

	if(tex.sliderButtonDown != NULL) m_sliderButtonDown = tex.sliderButtonDown;
	else m_sliderButtonDown = tex.sliderButtonNormal;
	if(tex.sliderButtonOn != NULL) m_sliderButtonOn = tex.sliderButtonOn;
	else m_sliderButtonOn = tex.sliderButtonNormal;
	if(tex.sliderButtonDisable != NULL) m_sliderButtonDisable = tex.sliderButtonDisable;
	else m_sliderButtonDisable = tex.sliderButtonNormal;
}
_XBool _XSlider::init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ��������Ӧ��Χ
		const _XRect& buttonArea,		//������������Ӧ��Χ
		const _XSliderTexture &tex,float max,float min,_XSliderType type)
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.sliderLineNormal == NULL || tex.sliderButtonNormal == NULL) return XFalse;	//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(min >= max) return XFalse;
	m_mouseRect = Area;
	m_buttonArea = buttonArea;
	m_position = position;

	m_typeVorH = type;
	m_withoutTex = XFalse;

	setTexture(tex);

	m_size.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_nowValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//�ߵ�λ�ò���Ҫ����
	m_lineSprite.init(m_sliderLineNormal->texture.m_w,m_sliderLineNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_lineSprite);
#endif
	m_lineSprite.setPosition(m_position);
	m_lineSprite.setSize(m_size);
	m_lineSprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);

	m_buttonSprite.init(m_sliderButtonNormal->texture.m_w,m_sliderButtonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_buttonSprite);
#endif
	m_buttonSprite.setSize(m_size);
	m_buttonSprite.setIsTransformCenter(POINT_LEFT_TOP);
	updateButtonData();

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
//int _XSlider::initWithoutTex(const _XRect& area,const _XRect& buttonArea,_XSliderType type,float max,float min,const _XVector2 &fontPosition)
_XBool _XSlider::initWithoutTex(const _XRect& area,float max,float min,_XSliderType type,const _XVector2 &fontPosition)
{
	if(m_isInited ||
		min >= max) return XFalse;
	m_mouseRect = area;
	m_typeVorH = type;
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top,area.top,area.bottom,area.bottom);
	else m_buttonArea.set(area.left,area.left,area.right,area.right);
	m_textPosition = fontPosition;
	m_position.set(0.0f,0.0f);

	m_withoutTex = XTrue;

	m_size.set(1.0f,1.0f);

	m_minValue = min;		
	m_maxValue = max;	
	m_nowValue = min;	
	m_upValue = min;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//�ߵ�λ�ò���Ҫ����
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);

	updateButtonData();

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);
	stateChange();

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XSlider::setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition)
{
	if(caption != NULL) strcpy(m_fontString,caption);
	else m_fontString[0] = '\0';
	m_caption.setACopy(font);
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	//m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor * m_color);	
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	char str[1024];
	sprintf(str,m_fontString,m_nowValue);
	m_caption.setString(str);
	m_textPosition = fontPosition;
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
_XBool _XSlider::setFontEx(const char *caption,const _XFontUnicode &font,float captionSize)
{
	if(caption != NULL) strcpy(m_fontString,caption);
	else m_fontString[0] = '\0';
	m_caption.setACopy(font);
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	//m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor * m_color);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	char str[1024];
	sprintf(str,m_fontString,m_nowValue);
	m_caption.setString(str);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
void _XSlider::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	updateChildPos();
	updateButtonData();
	if(!m_withoutTex) m_lineSprite.setPosition(m_position);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//����Ҫ������꺯��
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}
void _XSlider::updateButtonData()
{
	//���㻬���λ��
	if(m_typeVorH == SLIDER_TYPE_VERTICAL)
	{//��ֱ����
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + (m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * 0.5f,
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + 
			(m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * (m_nowValue - m_minValue)/(m_maxValue - m_minValue));
	}else
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽ����
		m_nowButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_size.y + 
			(m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_size.x * (m_nowValue - m_minValue)/(m_maxValue - m_minValue),
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_size.y + (m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_size.y * 0.5f);
	}
	if(!m_withoutTex) m_buttonSprite.setPosition(m_nowButtonPosition);
//	m_nowButtonArea.set(m_nowButtonPosition.x + m_buttonArea.left * m_size.x,m_nowButtonPosition.y + m_buttonArea.top * m_size.y,
//		m_nowButtonPosition.x + m_buttonArea.right * m_size.x,m_nowButtonPosition.y + m_buttonArea.bottom * m_size.y);
	if(m_needFont)
	{
		char str[1024];
		sprintf(str,m_fontString,m_nowValue);
		m_caption.setString(str);
	}
	if(m_pVariable != NULL) * m_pVariable = m_nowValue;	//��Ҫ�������������ĺ�����
}
void _XSlider::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	updateChildSize();
	if(!m_withoutTex)
	{
		m_lineSprite.setSize(m_size);
		m_buttonSprite.setSize(m_size);
	}
	updateButtonData();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x +  + m_mouseRect.right * m_size.x,m_position.y +  + m_mouseRect.bottom * m_size.y);
	//����Ҫ������꺯��
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	}
}
void _XSlider::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		if(m_nowSliderState == SLIDER_STATE_NORMAL)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true);
		//	m_lineSprite.draw(m_sliderLineNormal);
		//	m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_nowSliderState == SLIDER_STATE_DOWN)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.4f * m_color.fR,0.4f * m_color.fG,0.4f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineDown);
			//m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_nowSliderState == SLIDER_STATE_ON)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.85f * m_color.fR,0.85f * m_color.fG,0.85f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineOn);
			//m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_nowSliderState == SLIDER_STATE_DISABLE)
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),
				0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA,true);
			drawFillBoxExA(m_nowButtonPosition + _XVector2(m_buttonArea.left * m_size.x,m_buttonArea.top * m_size.y),
				_XVector2(m_buttonArea.getWidth() * m_size.x,m_buttonArea.getHeight() * m_size.y),
				0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true);
			//m_lineSprite.draw(m_sliderLineDisable);
			//m_buttonSprite.draw(m_sliderButtonDisable);
		}	
	}else
	{
		if(m_nowSliderState == SLIDER_STATE_NORMAL)
		{
			m_lineSprite.draw(m_sliderLineNormal);
			m_buttonSprite.draw(m_sliderButtonNormal);
		}else
		if(m_nowSliderState == SLIDER_STATE_DOWN)
		{
			m_lineSprite.draw(m_sliderLineDown);
			m_buttonSprite.draw(m_sliderButtonDown);
		}else
		if(m_nowSliderState == SLIDER_STATE_ON)
		{
			m_lineSprite.draw(m_sliderLineOn);
			m_buttonSprite.draw(m_sliderButtonOn);
		}else
		if(m_nowSliderState == SLIDER_STATE_DISABLE)
		{
			m_lineSprite.draw(m_sliderLineDisable);
			m_buttonSprite.draw(m_sliderButtonDisable);
		}
	}
	if(m_needFont) m_caption.draw();
}
_XBool _XSlider::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	//SliderLine,���������ߵ��¼�

	if(m_nowSliderState == SLIDER_STATE_DOWN)
	{//ע�������ǻ������϶�ʱ��Ϊ���ǳ�����Χ���϶���Ȼ��Ч���������������⴦��
		_XRect tempFly(m_nowMouseRect.left - m_mouseFlyArea.x,m_nowMouseRect.top - m_mouseFlyArea.y,
			m_nowMouseRect.right + m_mouseFlyArea.x,m_nowMouseRect.bottom + m_mouseFlyArea.y);
		if(tempFly.isInRect(x,y))
		{//�϶�(ע����������϶�������Χ����ʹ�����һ�ε���Чֵ)
			if(mouseState == MOUSE_MOVE)
			{//���¼���Slider�ĵ�ǰֵ
				if(m_typeVorH == SLIDER_TYPE_VERTICAL)
				{//��ֱ����
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}else
				if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
				{//ˮƽ����
					m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
					if(m_nowValue < m_minValue) m_nowValue = m_minValue;
					if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
				}
				updateButtonData();
				m_dataChanged = XTrue;	//�����ֵ�����ı�
				if(m_funMouseMove != NULL) m_funMouseMove(m_pClass,m_objectID);
			}else
			if(mouseState == MOUSE_LEFT_BUTTON_UP)
			{//��������¼����ڰ�ť�ϰ��µĵ����¼�
				m_nowSliderState = SLIDER_STATE_ON;
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				if(m_dataChanged) stateChange();
				if(m_funValueChange != NULL && m_dataChanged)
				{
					m_dataChanged = XFalse;
					m_funValueChange(m_pClass,m_objectID);
				}
			}
			m_isBeChoose = XTrue;
		}else
		{
			m_nowSliderState = SLIDER_STATE_NORMAL;
			if(m_dataChanged) stateChange();
			if(m_funValueChange != NULL && m_dataChanged)
			{
				m_dataChanged = XFalse;
				m_funValueChange(m_pClass,m_objectID);
			}
		}
	}
	if(m_nowMouseRect.isInRect(x,y))
	{//��궯���ڷ�Χ��
		if(mouseState == MOUSE_MOVE)
		{
			if(m_nowSliderState == SLIDER_STATE_NORMAL)
			{//����
				m_nowSliderState = SLIDER_STATE_ON;
				if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
				if(m_withAction)
				{//�������һ����̬Ч��
					m_isInAction = XTrue;
					m_actionMoveData.set(1.0f,1.1f,0.02f,MOVE_DATA_MODE_SIN_MULT,1);
					m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
					m_oldPos = m_position;
					m_oldSize = m_size;
				}
			}
		}else
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK) && (m_nowSliderState == SLIDER_STATE_NORMAL || m_nowSliderState == SLIDER_STATE_ON))
		{
			m_upValue = m_nowValue;

			//���¼���Slider�ĵ�ǰֵ
			if(m_typeVorH == SLIDER_TYPE_VERTICAL)
			{//��ֱ����
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}else
			if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
			{//ˮƽ����
				m_nowValue = m_minValue + (m_maxValue - m_minValue) * (x - m_position.x - m_mouseRect.left * m_size.x) / (m_mouseRect.getWidth() * m_size.x); 
				if(m_nowValue < m_minValue) m_nowValue = m_minValue;
				if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
			}
			updateButtonData();
			m_dataChanged = XTrue;	//�����ֵ�����ı�
			m_nowSliderState = SLIDER_STATE_DOWN;
			if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowSliderState == SLIDER_STATE_DOWN)
		{//����ᴥ����ֵ�ı��¼�(��������¼�Ӧ���������ϰ��µĵ���)(*����ò�Ʋ��ᷢ��*)
			m_nowSliderState = SLIDER_STATE_ON;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
			if(m_dataChanged) stateChange();
			if(m_funValueChange != NULL && m_dataChanged)
			{
				m_dataChanged = XFalse;
				m_funValueChange(m_pClass,m_objectID);
			}
		}
	}else
	{
		if(m_nowSliderState == SLIDER_STATE_ON) m_nowSliderState = SLIDER_STATE_NORMAL;
	}
	return XTrue;
}
_XBool _XSlider::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if(m_nowSliderState == SLIDER_STATE_NORMAL)	//ֻ������ͨ״̬�²���ʹ�ÿ�ݼ�����ʽ�ظ�����
		{//���˰�ť����Ч״̬֮��,����״̬��������Ӧ��ݼ���Ϣ
			_XBool flagChanged = XFalse;
			if(m_typeVorH == SLIDER_TYPE_VERTICAL)
			{//��ֱ
				if(keyOrder ==  XKEY_UP && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = XTrue;
				}else
				if(keyOrder ==  XKEY_DOWN && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = XTrue;
				}
			}else
			{//ˮƽ
				if(keyOrder ==  XKEY_LEFT && m_nowValue > m_minValue)
				{
					if(m_nowValue - m_keyOneValue < m_minValue) m_nowValue = m_minValue;
					else m_nowValue -= m_keyOneValue;
					flagChanged = XTrue;
				}else
				if(keyOrder ==  XKEY_RIGHT && m_nowValue < m_maxValue)
				{
					if(m_nowValue + m_keyOneValue > m_maxValue) m_nowValue = m_maxValue;
					else m_nowValue += m_keyOneValue;
					flagChanged = XTrue;
				}
			}
			if(flagChanged)
			{
				updateButtonData();
				stateChange();
				if(m_funValueChange != NULL)
				{
					m_dataChanged = XTrue;
					m_funValueChange(m_pClass,m_objectID);
				}
			}
		}
	}
	return XTrue;
}
void _XSlider::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),
							  void (*funValueChange)(void *,int),
							  void (*funMouseMove)(void *,int),
							  void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;
	m_funMouseUp = funMouseUp;
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
void _XSlider::setNowValue(float temp,bool cbFun)
{
	if(temp < m_minValue) temp = m_minValue;
	if(temp > m_maxValue) temp = m_maxValue;
	if(temp == m_nowValue) return;
	m_nowValue = temp;
	updateButtonData();
	if(m_funValueChange != NULL && cbFun)
	{
		m_dataChanged = XFalse;
		m_funValueChange(m_pClass,m_objectID);
	}
}
void _XSlider::setRange(float max,float min)
{
	if(min >= max) return;	//��������ǲ��Ϸ���
	m_maxValue = max;
	m_minValue = min;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;/// 20.0f;

	if(m_nowValue < m_minValue) m_nowValue = m_minValue;
	if(m_nowValue > m_maxValue) m_nowValue = m_maxValue;
	updateButtonData();
	if(m_funValueChange != NULL)
	{
		m_dataChanged = XFalse;
		m_funValueChange(m_pClass,m_objectID);
	}
}
_XBool _XSlider::setACopy(const _XSlider &temp)			//����һ������
{
	if(&temp == this ||
		!temp.m_isInited ||
		!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}
	m_needFont = temp.m_needFont;
	if(m_needFont)
	{//��������
		m_caption.setACopy(temp.m_caption);	//����
		strcpy(m_fontString,temp.m_fontString);
		m_textPosition = temp.m_textPosition;		//���ֵ����λ��
		m_textSize = temp.m_textSize;			//���ֵĳߴ�
		m_textColor = temp.m_textColor;		//���ֵ���ɫ
	}
	m_pVariable = temp.m_pVariable;

	m_isInited = temp.m_isInited;
	m_nowSliderState = temp.m_nowSliderState;			//��������״̬

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_typeVorH = temp.m_typeVorH;						//������������
	m_withoutTex = temp.m_withoutTex;						//������������

	m_sliderLineNormal = temp.m_sliderLineNormal;			//�������ߵ���ͨ״̬
	m_sliderLineDown = temp.m_sliderLineDown;			//�������ߵİ���״̬
	m_sliderLineOn = temp.m_sliderLineOn;				//�������ߵ�����״̬
	m_sliderLineDisable = temp.m_sliderLineDisable;			//�������ߵ���Ч״̬
	m_sliderButtonNormal = temp.m_sliderButtonNormal;		//��������ť����ͨ״̬
	m_sliderButtonDown = temp.m_sliderButtonDown;			//��������ť�İ���״̬
	m_sliderButtonOn = temp.m_sliderButtonOn;			//��������ť������״̬
	m_sliderButtonDisable = temp.m_sliderButtonDisable;		//��������ť����Ч״̬

	m_funInit = temp.m_funInit;		//��ʼ��
	m_funRelease = temp.m_funRelease;		//��Դ�ͷ�
	m_funMouseOn = temp.m_funMouseOn;		//�������
	m_funMouseDown = temp.m_funMouseDown;	//��갴��
	m_funMouseUp = temp.m_funMouseUp;		//��굯��
	m_funMouseMove = temp.m_funMouseMove;	//����϶�����ʱ����
	m_funValueChange = temp.m_funValueChange;	//����������ֵ�仯
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_buttonArea = temp.m_buttonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
//	m_nowButtonArea = temp.m_nowButtonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
	m_buttonSprite.setACopy(temp.m_buttonSprite);	//������ʾ��ͼ�ľ���
	m_lineSprite.setACopy(temp.m_lineSprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_buttonSprite);
	_XObjManger.decreaseAObject(&m_lineSprite);
#endif

	m_upMousePoint = temp.m_upMousePoint;		//�ϴ�����ڹ�������λ��
	m_mouseFlyArea = temp.m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ
	m_nowButtonPosition = temp.m_nowButtonPosition;//��ǰ���鰴ť��λ��
	m_minValue = temp.m_minValue;			//�����������ֵ
	m_maxValue = temp.m_maxValue;			//����������Сֵ
	m_nowValue = temp.m_nowValue;			//�������ĵ�ǰֵ
	m_upValue = temp.m_upValue;			//����������һ��ֵ(�������������Ŀ����������϶����鳬����Χ֮����Իָ����϶�֮ǰ��֪������Ŀǰ���ڼ򵥵Ŀ��ǣ������������)
	m_dataChanged = temp.m_dataChanged;			//����������ֵ�Ƿ��б��޸�
	m_keyOneValue = temp.m_keyOneValue;		//����һ�ΰ����Ļ�����(ע�����ֵ����Ҫ>=0,���򽫻���ɴ���)

	m_actionMoveData = temp.m_actionMoveData;	//��̬Ч���ı���
	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	return XTrue;
}
void _XSlider::update(int stepTime)
{
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) m_isInAction = false;	//�����������
		setSize(m_actionMoveData.getNowData() * m_oldSize);
		_XVector2 tmp(m_mouseRect.getWidth(),m_mouseRect.getHeight());
		tmp = tmp * (m_actionMoveData.getNowData() * m_oldSize - m_oldSize) * 0.5f;
		setPosition(m_oldPos - tmp);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		_XVector2 pos(m_oldPos.x + m_mouseRect.left * m_oldSize.x + m_mouseRect.getWidth() * m_oldSize.x * 0.5f,
			m_oldPos.y + m_mouseRect.top * m_oldSize.y + m_mouseRect.getHeight() * m_oldSize.y * 0.5f);
		_XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getNowData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getNowData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
}