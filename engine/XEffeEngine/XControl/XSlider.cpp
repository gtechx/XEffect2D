#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include"XSlider.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XSliderSkin::XSliderSkin()
	:m_isInited(XFalse)
	, sliderLineNormal(NULL)
	, sliderLineDown(NULL)
	, sliderLineOn(NULL)
	, sliderLineDisable(NULL)
	, sliderButtonNormal(NULL)
	, sliderButtonDown(NULL)
	, sliderButtonOn(NULL)
	, sliderButtonDisable(NULL)
{}
XBool XSliderSkin::init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
						  const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,XResPos resPos)
{
	if(m_isInited ||
		LNormal == NULL || BNormal == NULL) return XFalse;

	int ret = 1;
	//�ֱ�������Դ	
	if((sliderLineNormal = createATextureData(LNormal,resPos)) == NULL) ret = 0;
	if(LDown != NULL && ret != 0 &&
		(sliderLineDown = createATextureData(LDown,resPos)) == NULL) ret = 0;
	if(LOn != NULL && ret != 0 &&
		(sliderLineOn = createATextureData(LOn,resPos)) == NULL) ret = 0;
	if(LDisable != NULL && ret != 0 &&
		(sliderLineDisable = createATextureData(LDisable,resPos)) == NULL) ret = 0;
	if(ret != 0 &&
		(sliderButtonNormal = createATextureData(BNormal,resPos)) == NULL) ret = 0;
	if(BDown != NULL && ret != 0 &&
		(sliderButtonDown = createATextureData(BDown,resPos)) == NULL) ret = 0;
	if(BOn != NULL && ret != 0 &&
		(sliderButtonOn = createATextureData(BOn,resPos)) == NULL) ret = 0;
	if(BDisable != NULL && ret != 0 &&
		(sliderButtonDisable = createATextureData(BDisable,resPos)) == NULL) ret = 0;
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define SLIDER_CONFIG_FILENAME "Slider.txt"
bool XSliderSkin::loadFromFolder(const char *filename,XResPos resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
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
	if((sliderLineNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		fclose(fp);
		return XFalse;
	}
	//down
	if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
	if(flag != 0)
	{
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineDown = createATextureData(tempFilename,resPos)) == NULL)
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
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineOn = createATextureData(tempFilename,resPos)) == NULL)
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
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((sliderButtonNormal = createATextureData(tempFilename,resPos)) == NULL)
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
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonDown = createATextureData(tempFilename,resPos)) == NULL)
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
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonOn = createATextureData(tempFilename,resPos)) == NULL)
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
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	return true;
}
bool XSliderSkin::loadFromPacker(const char *filename,XResPos resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,SLIDER_CONFIG_FILENAME);
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
	if((sliderLineNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineDown = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//on
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineOn = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//disable
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderLineDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//normal
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
	if((sliderButtonNormal = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonDown = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonOn = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
		if((sliderButtonDisable = createATextureData(tempFilename,resPos)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XMem::XDELETE_ARRAY(p);
			return XFalse;
		}
	}
	//��ȡ��������
	int l,t,r,b;
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRect.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	m_mouseRectButton.set(l,t,r,b);
	if(sscanf((char *)(p + offset),"%d,%d,",&l,&t) != 2)
	{
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(l,t);
	}else
	{
		m_fontPosition.reset();
	}
	//�������ݶ�ȡ���
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XSliderSkin::loadFromWeb(const char *filename,XResPos resPos)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XSliderSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
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
XSlider::XSlider()
	:m_isInited(XFalse)
	, m_curSliderState(SLIDER_STATE_NORMAL)
	, m_typeVorH(SLIDER_TYPE_VERTICAL)
	, m_sliderLineNormal(NULL)
	, m_sliderLineDown(NULL)
	, m_sliderLineOn(NULL)
	, m_sliderLineDisable(NULL)
	, m_sliderButtonNormal(NULL)
	, m_sliderButtonDown(NULL)
	, m_sliderButtonOn(NULL)
	, m_sliderButtonDisable(NULL)
	, m_minValue(0.0f)
	, m_maxValue(100.0f)
	, m_curValue(0.0f)
	, m_mouseFlyArea(50.0f)	//���ֵʹ��һ��ϵͳ�趨�����ֵ�����ڴ����н����޸ģ����ǲ�����ģ����ǿ��ǵ������ڵ������ԣ�����Ĭ��������������
	, m_needFont(XFalse)
	, m_pVariable(NULL)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	, m_textColor(0.0f, 1.0f)
	, m_isInteger(false)
{
	m_ctrlType = CTRL_OBJ_SLIDER;
}
void XSlider::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_RELEASE);
	else XCtrlManager.eventProc(m_objectID,SLD_RELEASE);
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
void XSlider::setTexture(const XSliderSkin &tex)
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
XBool XSlider::init(const XVec2& position,	//�ؼ����ڵ�λ��
		const XRect& Area,				//�ؼ��������Ӧ��Χ
		const XRect& buttonArea,		//������������Ӧ��Χ
		const XSliderSkin &tex,float maxValue,float minValue,XSliderType type)
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.sliderLineNormal == NULL || tex.sliderButtonNormal == NULL) return XFalse;	//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(minValue >= maxValue) XMath::swapTwoSum(minValue, maxValue);
	m_mouseRect = Area;
	m_buttonArea = buttonArea;
	m_position = position;

	m_typeVorH = type;
	m_withoutTex = XFalse;
	m_comment.init();

	setTexture(tex);

	m_scale.set(1.0f);

	m_minValue = minValue;
	m_maxValue = maxValue;
	m_curValue = minValue;
	m_upValue = minValue;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//�ߵ�λ�ò���Ҫ����
	m_lineSprite.init(m_sliderLineNormal->texture.m_w,m_sliderLineNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_lineSprite);
#endif
	m_lineSprite.setPosition(m_position);
	m_lineSprite.setScale(m_scale);
	m_lineSprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);

	m_buttonSprite.init(m_sliderButtonNormal->texture.m_w,m_sliderButtonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_buttonSprite);
#endif
	m_buttonSprite.setScale(m_scale);
	m_buttonSprite.setIsTransformCenter(POINT_LEFT_TOP);
	updateButtonData();

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_INIT);
	else XCtrlManager.eventProc(m_objectID,SLD_INIT);
	stateChange();

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
//int XSlider::initWithoutSkin(const XRect& area,const XRect& buttonArea,XSliderType type,float max,float min,const XVec2& fontPosition)
XBool XSlider::initWithoutSkin(const XRect& area,float maxValue,float minValue,XSliderType type,const XVec2& fontPosition,bool narrowBtn)
{
	if(m_isInited) return false;
	if(minValue >= maxValue) XMath::swapTwoSum(minValue, maxValue);
	m_mouseRect = area;
	m_typeVorH = type;
	if(narrowBtn)
	{
		if (m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top, area.top, area.bottom * 0.5f, area.bottom);
		else m_buttonArea.set(area.left, area.left, area.right, area.right * 0.5f);
	}else
	{
		if (m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_buttonArea.set(area.top, area.bottom);
		else m_buttonArea.set(area.left, area.right);
	}
	m_textPosition = fontPosition;
	m_position.set(0.0f);

	m_withoutTex = XTrue;
	m_comment.init();

	m_scale.set(1.0f);

	m_minValue = minValue;
	m_maxValue = maxValue;
	m_curValue = minValue;
	m_upValue = minValue;
	m_dataChanged = XFalse;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;

	//�ߵ�λ�ò���Ҫ����
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);

	updateButtonData();

	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_INIT);
	else XCtrlManager.eventProc(m_objectID,SLD_INIT);
	stateChange();

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XSlider::setFont(const char *caption,const XFontUnicode& font,float captionSize,const XVec2& fontPosition)
{
	if(caption != NULL) m_fontString = caption;
	else m_fontString = "";
	if(!m_caption.setACopy(font)) return XFalse;
	m_caption.setColor(m_textColor * m_color);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	//m_textColor.setColor(XFColor::black);
	m_caption.setColor(m_textColor * m_color);	
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize);
	m_caption.setScale(m_textSize * m_scale);
	char str[1024];
	if(m_isInteger)
		sprintf_s(str,1024,m_fontString.c_str(),(int)m_curValue);
	else
		sprintf_s(str,1024,m_fontString.c_str(),m_curValue);
	m_caption.setString(str);
	m_textPosition = fontPosition;
	m_caption.setPosition(m_position + m_textPosition * m_scale);
	if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_caption.setAngle(90.0f);
	else m_caption.setAngle(0.0f);
	m_needFont = XTrue;
	return XTrue;
}
void XSlider::setPosition(const XVec2& p)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position = p;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if (m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}
	updateChildPos();
	updateButtonData();
	if(!m_withoutTex) m_lineSprite.setPosition(tmpPos);
	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	//����Ҫ������꺯��,�������û�����߼����⣬����ƶ��쳣
//	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
		m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
}
void XSlider::updateButtonData()
{
	//���㻬���λ��
	if(m_typeVorH == SLIDER_TYPE_VERTICAL)
	{//��ֱ����
		m_curButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_scale.y + (m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_scale.x * 0.5f,
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_scale.y + 
			(m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_scale.y * (m_curValue - m_minValue)/(m_maxValue - m_minValue));
	}else
	if(m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽ����
		m_curButtonPosition.set(m_position.x + (m_mouseRect.left - m_buttonArea.left) * m_scale.y + 
			(m_mouseRect.getWidth() - m_buttonArea.getWidth()) * m_scale.x * (m_curValue - m_minValue)/(m_maxValue - m_minValue),
			m_position.y + (m_mouseRect.top - m_buttonArea.top) * m_scale.y + (m_mouseRect.getHeight() - m_buttonArea.getHeight()) * m_scale.y * 0.5f);
	}
	if(!m_withoutTex) m_buttonSprite.setPosition(m_curButtonPosition);
//	m_curButtonArea.set(m_curButtonPosition.x + m_buttonArea.left * m_scale.x,m_curButtonPosition.y + m_buttonArea.top * m_scale.y,
//		m_curButtonPosition.x + m_buttonArea.right * m_scale.x,m_curButtonPosition.y + m_buttonArea.bottom * m_scale.y);
	if(m_needFont)
	{
		char str[1024];
		if(m_isInteger)
			sprintf_s(str,1024,m_fontString.c_str(),(int)m_curValue);
		else
			sprintf_s(str,1024,m_fontString.c_str(),m_curValue);
		m_caption.setString(str);
	}
	if(m_pVariable != NULL) * m_pVariable = m_curValue;	//��Ҫ�������������ĺ�����
}
void XSlider::setScale(const XVec2& s)
{
	if(s.x <= 0 || s.y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_scale = s;
	XVec2 tmpPos = m_position;
	XVec2 tmpSize = m_scale;
	if (m_isInAction && !m_actionMoveData.getIsEnd())
	{
		tmpPos = m_mouseRect.getSize();
		tmpPos = tmpPos * (m_actionMoveData.getCurData() * m_scale - m_scale) * 0.5f;
		tmpPos = m_position - tmpPos;
		tmpSize = m_actionMoveData.getCurData() * m_scale;
	}

	updateChildScale();
	if(!m_withoutTex)
	{
		m_lineSprite.setScale(tmpSize);
		m_buttonSprite.setScale(tmpSize);
	}
	updateButtonData();

	m_curMouseRect.set(tmpPos + m_mouseRect.getLT() * tmpSize,
		tmpPos + m_mouseRect.getRB() * tmpSize);
	//����Ҫ������꺯��,�������û�����߼����⣬����ƶ��쳣
//	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	if(m_needFont)
	{
		m_caption.setPosition(tmpPos + m_textPosition * tmpSize);
		m_caption.setScale(m_textSize * tmpSize);
	}
}
void XSlider::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		switch (m_curSliderState)
		{
		case SLIDER_STATE_NORMAL:
			XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::downColor * m_color, true);
			XRender::drawFillRectExA(m_curButtonPosition + m_buttonArea.getLT() * m_scale,
				m_buttonArea.getSize() * m_scale, XCCS::normalColor * m_color, true);
		//	m_lineSprite.draw(m_sliderLineNormal);
		//	m_buttonSprite.draw(m_sliderButtonNormal);
			break;
		case SLIDER_STATE_DOWN:
			XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::mouseColor * m_color, true);
			XRender::drawFillRectExA(m_curButtonPosition + m_buttonArea.getLT() * m_scale,
				m_buttonArea.getSize() * m_scale, XCCS::lightSpecialColor * m_color, true);
			//m_lineSprite.draw(m_sliderLineDown);
			//m_buttonSprite.draw(m_sliderButtonDown);
			break;
		case SLIDER_STATE_ON:
			XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::specialColor * m_color, true);
			XRender::drawFillRectExA(m_curButtonPosition + m_buttonArea.getLT() * m_scale,
				m_buttonArea.getSize() * m_scale, XCCS::onColor * m_color, true);
			//m_lineSprite.draw(m_sliderLineOn);
			//m_buttonSprite.draw(m_sliderButtonOn);
			break;
		case SLIDER_STATE_DISABLE:
			XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
				m_mouseRect.getSize() * m_scale, XCCS::blackDownColor * m_color, true);
			XRender::drawFillRectExA(m_curButtonPosition + m_buttonArea.getLT() * m_scale,
				m_buttonArea.getSize() * m_scale, XCCS::downColor * m_color, true);
			//m_lineSprite.draw(m_sliderLineDisable);
			//m_buttonSprite.draw(m_sliderButtonDisable);
			break;
		}	
	}else
	{
		switch (m_curSliderState)
		{
		case SLIDER_STATE_NORMAL:
			m_lineSprite.draw(m_sliderLineNormal);
			m_buttonSprite.draw(m_sliderButtonNormal);
			break;
		case SLIDER_STATE_DOWN:
			m_lineSprite.draw(m_sliderLineDown);
			m_buttonSprite.draw(m_sliderButtonDown);
			break;
		case SLIDER_STATE_ON:
			m_lineSprite.draw(m_sliderLineOn);
			m_buttonSprite.draw(m_sliderButtonOn);
			break;
		case SLIDER_STATE_DISABLE:
			m_lineSprite.draw(m_sliderLineDisable);
			m_buttonSprite.draw(m_sliderButtonDisable);
			break;
		}
	}
	if(m_needFont) m_caption.draw();
}
float XSlider::getMapValue(float v)
{
	float ret = 0.0f;
	switch (m_typeVorH)
	{
	case SLIDER_TYPE_VERTICAL:		//��ֱ����
		ret = m_minValue + (m_maxValue - m_minValue) * (v - m_position.y - (m_mouseRect.top + m_mouseRect.getWidth() * 0.5f) * m_scale.y) /
			((m_mouseRect.getHeight() - m_mouseRect.getWidth()) * m_scale.y); 
		break;
	case SLIDER_TYPE_HORIZONTAL:	//ˮƽ����
		ret = m_minValue + (m_maxValue - m_minValue) * (v - m_position.x - (m_mouseRect.left + m_mouseRect.getHeight() * 0.5f) * m_scale.x) / 
			((m_mouseRect.getWidth() - m_mouseRect.getHeight()) * m_scale.x); 
		break;
	}
	return XMath::clamp(ret, m_minValue, m_maxValue);
}
XBool XSlider::mouseProc(const XVec2& p,XMouseState mouseState)
{
	m_upMousePoint.set(p);
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;
	//SliderLine,���������ߵ��¼�

	if(m_curSliderState == SLIDER_STATE_DOWN)
	{//ע�������ǻ������϶�ʱ��Ϊ���ǳ�����Χ���϶���Ȼ��Ч���������������⴦��
		XRect tempFly(m_curMouseRect.getLT() - m_mouseFlyArea, m_curMouseRect.getRB() + m_mouseFlyArea);
		if(tempFly.isInRect(p))
		{//�϶�(ע����������϶�������Χ����ʹ�����һ�ε���Чֵ)
			switch(mouseState)
			{
			case MOUSE_MOVE://���¼���Slider�ĵ�ǰֵ
				if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_curValue = getMapValue(p.y);else
				if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_curValue = getMapValue(p.x);
				updateButtonData();
				m_dataChanged = XTrue;	//�����ֵ�����ı�
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_MOVE);
				else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_MOVE);
				break;
			case MOUSE_LEFT_BUTTON_UP://��������¼����ڰ�ť�ϰ��µĵ����¼�
				m_curSliderState = SLIDER_STATE_ON;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_UP);
				else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_UP);
				if(m_dataChanged)
				{
					stateChange();
					if(m_eventProc != NULL)
						m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
					else
						XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
					m_dataChanged = XFalse;
				}
				break;
			}
			m_isBeChoose = XTrue;
		}else
		{
			m_curSliderState = SLIDER_STATE_NORMAL;
			if(m_dataChanged)
			{
				stateChange();
				if(m_eventProc != NULL)
					m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
		}
	}
	if(m_curMouseRect.isInRect(p))
	{//��궯���ڷ�Χ��
		switch(mouseState)
		{
		case MOUSE_MOVE:
			if(m_curSliderState != SLIDER_STATE_NORMAL) break; //����
			m_curSliderState = SLIDER_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_ON);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_ON);
			if(m_withAction)
			{//�������һ����̬Ч��
				m_isInAction = XTrue;
				m_actionMoveData.set(1.0f,1.1f,0.02f,MD_MODE_SIN_MULT,1);
				m_lightMD.set(1.0f,2.0f,0.002f,MD_MODE_SIN_MULT);
				m_oldPos = m_position;
				m_oldSize = m_scale;
			} 
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if(m_curSliderState != SLIDER_STATE_NORMAL && m_curSliderState != SLIDER_STATE_ON) break;
			m_upValue = m_curValue;

			//���¼���Slider�ĵ�ǰֵ
			if(m_typeVorH == SLIDER_TYPE_VERTICAL) m_curValue = getMapValue(p.y);else
			if(m_typeVorH == SLIDER_TYPE_HORIZONTAL) m_curValue = getMapValue(p.x);

			updateButtonData();
			m_dataChanged = XTrue;	//�����ֵ�����ı�
			m_curSliderState = SLIDER_STATE_DOWN;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_DOWN);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_DOWN);
			break;
		case MOUSE_LEFT_BUTTON_UP:
			if(m_curSliderState != SLIDER_STATE_DOWN) break;
			//����ᴥ����ֵ�ı��¼�(��������¼�Ӧ���������ϰ��µĵ���)(*����ò�Ʋ��ᷢ��*)
			m_curSliderState = SLIDER_STATE_ON;
			if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_MOUSE_UP);
			else XCtrlManager.eventProc(m_objectID,SLD_MOUSE_UP);
			if(m_dataChanged)
			{
				stateChange();
				if(m_eventProc != NULL)
					m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
				else
					XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
				m_dataChanged = XFalse;
			}
			break;
		case MOUSE_WHEEL_UP_DOWN:
			if(m_curSliderState != SLIDER_STATE_ON) break;
			if(m_curValue >= m_maxValue) break;
			m_curValue += m_keyOneValue * 0.1f;
			if(m_curValue >= m_maxValue) m_curValue = m_maxValue;
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse;
			break;
		case MOUSE_WHEEL_DOWN_DOWN:
			if(m_curSliderState != SLIDER_STATE_ON) break;
			if(m_curValue <= m_minValue) break;
			m_curValue -= m_keyOneValue * 0.1f;
			if(m_curValue <= m_minValue) m_curValue = m_minValue;
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse; 
			break;
		}
		if (!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(p + XVec2(0.0f, 16.0f));
		}
		if (mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//�����������������˵������ʧ
	}else
	{
		if(m_curSliderState == SLIDER_STATE_ON) m_curSliderState = SLIDER_STATE_NORMAL;
		if (m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	return XTrue;
}
XBool XSlider::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(m_isSilent) return XFalse;

	if(keyState == KEY_STATE_UP && m_curSliderState == SLIDER_STATE_NORMAL)
	{//��������ʱ������Ӧ
		//ֻ������ͨ״̬�²���ʹ�ÿ�ݼ�����ʽ�ظ�����
		//���˰�ť����Ч״̬֮��,����״̬��������Ӧ��ݼ���Ϣ
		XBool flagChanged = XFalse;
		if(m_typeVorH == SLIDER_TYPE_VERTICAL)
		{//��ֱ
			switch(keyOrder)
			{
			case XKEY_UP:
				if(m_curValue <= m_minValue) break;
				if(m_curValue - m_keyOneValue < m_minValue) m_curValue = m_minValue;
				else m_curValue -= m_keyOneValue;
				flagChanged = XTrue;
				break;
			case XKEY_DOWN:
				if(m_curValue >= m_maxValue) break;
				if(m_curValue + m_keyOneValue > m_maxValue) m_curValue = m_maxValue;
				else m_curValue += m_keyOneValue;
				flagChanged = XTrue;
				break;
			}
		}else
		{//ˮƽ
			switch(keyOrder)
			{
			case XKEY_LEFT:
				if(m_curValue <= m_minValue) break;
				if(m_curValue - m_keyOneValue < m_minValue) m_curValue = m_minValue;
				else m_curValue -= m_keyOneValue;
				flagChanged = XTrue;
				break;
			case XKEY_RIGHT:
				if(m_curValue >= m_maxValue) break;
				if(m_curValue + m_keyOneValue > m_maxValue) m_curValue = m_maxValue;
				else m_curValue += m_keyOneValue;
				flagChanged = XTrue;
				break;
			}
		}
		if(flagChanged)
		{
			updateButtonData();
			stateChange();
			if(m_eventProc != NULL)
				m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
			else
				XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
			m_dataChanged = XFalse;
		}
	}
	return XTrue;
}
void XSlider::setCurValue(float temp,bool withEvent)
{
	temp = XMath::clamp(temp, m_minValue, m_maxValue);
	if(temp == m_curValue) return;
	m_curValue = temp;
	updateButtonData();
	if(withEvent)
	{
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,SLD_VALUE_CHANGE);
		else XCtrlManager.eventProc(m_objectID,SLD_VALUE_CHANGE);
		m_dataChanged = XFalse;
	}
}
void XSlider::setRange(float maxValue,float minValue, bool withEvent)
{
	if(minValue >= maxValue) XMath::swapTwoSum(minValue, maxValue);
	m_maxValue = maxValue;
	m_minValue = minValue;
	m_keyOneValue = (m_maxValue - m_minValue) * 0.01f;/// 20.0f;

	float tmpV = m_curValue;
	m_curValue = XMath::clamp(m_curValue, m_minValue, m_maxValue);
	updateButtonData();
	if (withEvent && tmpV != m_curValue)
	{
		if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, SLD_VALUE_CHANGE);
		else XCtrlManager.eventProc(m_objectID, SLD_VALUE_CHANGE);
	}
	m_dataChanged = XFalse;
}
XBool XSlider::setACopy(const XSlider &temp)			//����һ������
{
	if(&temp == this ||
		!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_needFont = temp.m_needFont;
	if(m_needFont)
	{//��������
		m_caption.setACopy(temp.m_caption);	//����
		m_fontString = temp.m_fontString;
		m_textPosition = temp.m_textPosition;		//���ֵ����λ��
		m_textSize = temp.m_textSize;			//���ֵĳߴ�
		m_textColor = temp.m_textColor;		//���ֵ���ɫ
	}
	m_pVariable = temp.m_pVariable;
	m_isInteger = temp.m_isInteger;

	m_isInited = temp.m_isInited;
	m_curSliderState = temp.m_curSliderState;			//��������״̬

	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
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

	m_buttonArea = temp.m_buttonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
//	m_curButtonArea = temp.m_curButtonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
	m_buttonSprite.setACopy(temp.m_buttonSprite);	//������ʾ��ͼ�ľ���
	m_lineSprite.setACopy(temp.m_lineSprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_buttonSprite);
	XObjManager.decreaseAObject(&m_lineSprite);
#endif

	m_upMousePoint = temp.m_upMousePoint;		//�ϴ�����ڹ�������λ��
	m_mouseFlyArea = temp.m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ
	m_curButtonPosition = temp.m_curButtonPosition;//��ǰ���鰴ť��λ��
	m_minValue = temp.m_minValue;			//�����������ֵ
	m_maxValue = temp.m_maxValue;			//����������Сֵ
	m_curValue = temp.m_curValue;			//�������ĵ�ǰֵ
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
void XSlider::update(float stepTime)
{
	m_comment.update(stepTime);
	if(m_isInAction)
	{//���ڶ��������м��㶯����ʵʩ
		m_actionMoveData.move(stepTime);
		if(m_actionMoveData.getIsEnd()) 
			m_isInAction = false;	//�����������
		setScale(m_scale);
		setPosition(m_position);
		//setScale(m_actionMoveData.getCurData() * m_oldSize);
		//setPosition(m_oldPos - 
		//	m_mouseRect.getSize() * (m_actionMoveData.getCurData() * m_oldSize - m_oldSize) * 0.5f);
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVec2 pos = m_oldPos + m_mouseRect.getLT() * m_oldSize + m_mouseRect.getSize() * m_oldSize.x * 0.5f;
		XVec2 size = m_mouseRect.getSize() * m_oldSize * m_lightMD.getCurData() * 0.5f;
		m_lightRect.set(pos - size,pos + size);
	}
}
#if !WITH_INLINE_FILE
#include "XSlider.inl"
#endif
}