//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCombo.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void funComboMenuState(void *pClass,int)
{
	_XCombo &pPar = *(_XCombo *) pClass;
	pPar.changeDownMenuState();
//	pPar.m_mouseButtonDownIsUseable = 1;
}
void funComboMenuStart(void *pClass,int)
{
	_XCombo &pPar = *(_XCombo *) pClass;
	if(pPar.m_menuStartDrawOrder > 0)
	{
		-- pPar.m_menuStartDrawOrder;
		pPar.updateString();
		if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum < pPar.m_menuSum) pPar.m_buttom[2].enable();
		if(pPar.m_menuStartDrawOrder == 0) pPar.m_buttom[1].disable();
	}
// 	pPar.m_mouseButtonDownIsUseable = 1;
}
void funComboMenuEnd(void *pClass,int)
{
	_XCombo &pPar = *(_XCombo *) pClass;
	if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum < pPar.m_menuSum)
	{
		++ pPar.m_menuStartDrawOrder;
		pPar.updateString();
		if(pPar.m_menuStartDrawOrder > 0) pPar.m_buttom[1].enable();
		if(pPar.m_menuStartDrawOrder + pPar.m_menuDrawSum >= pPar.m_menuSum) pPar.m_buttom[2].disable();
	}
// 	pPar.m_mouseButtonDownIsUseable = 1;
}
void funComboMenu(void *pClass,int ID)
{
	_XCombo &pPar = *(_XCombo *) pClass;
	for(int i = 0;i < pPar.m_menuDrawSum;++ i)
	{
		if(pPar.m_buttom[i + 3].getControlID() == ID)
		{
			//pPar.m_isDrawDownMenu = XFalse;
			pPar.changeDownMenuState();
			if(pPar.m_nowChooseOrder != i + pPar.m_menuStartDrawOrder)
			{
				pPar.m_nowChooseOrder = i + pPar.m_menuStartDrawOrder;
				pPar.updateString();
				if(pPar.m_funDataChange != NULL) pPar.m_funDataChange(pPar.m_pClass,pPar.m_objectID);
			}
			break;
		}
	}
// 	pPar.m_mouseButtonDownIsUseable = 1;
}

_XComboTexture::_XComboTexture()
:m_isInited(XFalse)
,comboInputNormal(NULL)				//�����˵���������ͨ״̬
,comboInputDisable(NULL)			//��Ч״̬�µİ�ť��ͼ
{}
_XBool _XComboTexture::init(const char *inputNormal,const char *inputDisable,
		 const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		 const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		 const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		 const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		 _XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	if(inputNormal == NULL || inputDisable == NULL ||
		downButtonNormal == NULL || downButtonOn == NULL || downButtonDown == NULL || downButtonDisable == NULL ||
		downMenuUpNormal == NULL || downMenuUpOn == NULL || downMenuUpDown == NULL || downMenuUpDisable == NULL ||
		downMenuNormal == NULL || downMenuOn == NULL || downMenuDown == NULL || downMenuDisable == NULL ||
		downMenuDownNormal == NULL || downMenuDownOn == NULL || downMenuDownDown == NULL || downMenuDownDisable == NULL)
	{//��ͼ�����ݲ���Ϊ��
		return XFalse;
	}
	int ret = 1;
	//���������������е���ͼ��Դ
	if((comboInputNormal = createATextureData(inputNormal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(comboInputDisable = createATextureData(inputDisable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		!downButtonTex.init(downButtonNormal,downButtonDown,downButtonOn,downButtonDisable,resoursePosition))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuUpButtonTex.init(downMenuUpNormal,downMenuUpDown,downMenuUpOn,downMenuUpDisable,resoursePosition))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuButtonTex.init(downMenuNormal,downMenuDown,downMenuOn,downMenuDisable,resoursePosition))
	{
		ret = 0;
	}
	if(ret != 0 && 
		!downMenuDownButtonTex.init(downMenuDownNormal,downMenuDownDown,downMenuDownOn,downMenuDownDisable,resoursePosition))
	{
		ret = 0;
	}
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define COMBO_CONFIG_FILENAME ("Combo.txt")
_XBool _XComboTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited || filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,COMBO_CONFIG_FILENAME);
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
		if((comboInputNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((comboInputDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if(!downButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuUpButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuDownButtonTex.initEx(tempFilename,resoursePosition))
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b;
		if(fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {fclose(fp);return XFalse;}
		m_mouseRect.set(l,t,r,b);
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
		if((comboInputNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((comboInputDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if(!downButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuUpButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuButtonTex.initEx(tempFilename,resoursePosition))
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
		if(!downMenuDownButtonTex.initEx(tempFilename,resoursePosition))
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b;
		if(sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b) != 4) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
_XCombo::_XCombo()
:m_isInited(XFalse)
,m_comboInputNormal(NULL)			//�����˵���������ͨ״̬
,m_comboInputDisable(NULL)			//��Ч״̬�µİ�ť��ͼ
,m_funDataChange(NULL)
,m_pClass(NULL)
,m_menuData(NULL)
,m_buttom(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
	m_ctrlType = CTRL_OBJ_COMBO;
}
void _XCombo::release()
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_buttom);
	XDELETE_ARRAY(m_menuData);
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
_XBool _XCombo::init(const _XComboTexture &tex,	//�ؼ�����ͼ
		const _XVector2& position,		//�ؼ���λ��
		const _XRect& inputArea,		//��������Ч��Χ
		const _XRect& downButtonArea,	//������ť����Ӧ��Χ
		const _XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
		const _XRect& downMenuArea,		//ѡ�������Ӧ��Χ
		const _XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize)				//����Ĵ�С
{
	if(m_isInited ||				//��ֹ�ظ���ʼ��
		tex.comboInputNormal == NULL ||	//����������ͼΪ�գ�������ͼ������Ϊ�գ��򷵻�ʧ��
		menuSum <= 0 ||					//�˵��в���û�в˵���
		drawMenuSum <= 0 || drawMenuSum > menuSum ||	
		fontSize <= 0 ||
		inputArea.getHeight() <= 0 || inputArea.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;

	//������ͼ
	m_comboInputNormal = tex.comboInputNormal;					//�����˵���������ͨ״̬
	m_comboInputDisable = tex.comboInputDisable;				//��Ч״̬�µİ�ť��ͼ

	//������ֵ
	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_menuDrawSum = drawMenuSum;
	m_menuStartDrawOrder = 0;
	m_inputArea = inputArea;
	m_downButtonArea = downButtonArea;
	m_downMenuUpArea = downMenuUpArea;
	m_downMenuArea = downMenuArea;
	m_downMenuDownArea = downMenuDownArea;
	m_withoutTex = XFalse;
	m_comment.init();
	m_mouseRect = inputArea;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_caption.setString(" ");
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_textSize.set(fontSize,fontSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	//������ť
	m_buttom = createArrayMem<_XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL)return XFalse;
	//�����ʼ�����а�ť
	//_XButtonTexture tempButtonTex;
	//tempButtonTex.set(m_comboDownButtonNormal,m_comboDownButtonDown,m_comboDownButtonOn,m_comboDownButtonDisable);
	if(!m_buttom[0].init(_XVector2(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y),
		m_downButtonArea,tex.downButtonTex," ",font,m_textSize.x,_XVector2::zero)) return XFalse;		//�����˵���ߵ������˵��İ�ť
	m_buttom[0].setSize(m_size);
	m_buttom[0].setMouseDownCB(funComboMenuState,this);
	//tempButtonTex.set(m_comboDownMenuUpNormal,m_comboDownMenuUpDown,m_comboDownMenuUpOn,m_comboDownMenuUpDisable);
	if(!m_buttom[1].init(_XVector2(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y),
		m_downMenuUpArea,tex.downMenuUpButtonTex," ",font,m_textSize.x,_XVector2::zero)) return XFalse;		//�����˵����˵�����İ�ť
	m_buttom[1].setSize(m_size);
	m_buttom[1].setMouseDownCB(funComboMenuStart,this);
	//tempButtonTex.set(m_comboDownMenuDownNormal,m_comboDownMenuDownDown,m_comboDownMenuDownOn,m_comboDownMenuDownDisable);
	_XVector2 tempPosition;
	tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	if(!m_buttom[2].init(tempPosition,m_downMenuDownArea,tex.downMenuDownButtonTex," ",font,m_textSize.x,_XVector2::zero)) return XFalse;		//�����˵����˵�������İ�ť
	m_buttom[2].setSize(m_size);
	m_buttom[2].setMouseDownCB(funComboMenuEnd,this);

	//tempButtonTex.set(m_comboDownMenuNormal,m_comboDownMenuDown,m_comboDownMenuOn,m_comboDownMenuDisable);
	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
		if(!m_buttom[3 + i].init(tempPosition,m_downMenuArea,tex.downMenuButtonTex," ",font,m_textSize.x,
			_XVector2(m_downMenuArea.left + m_downMenuArea.getWidth() * 0.5f,m_downMenuArea.top + m_downMenuArea.getHeight() * 0.5f))) return XFalse;		//�����˵����˵�������İ�ť
		m_buttom[3 + i].setSize(m_size);
		m_buttom[3 + i].setMouseDownCB(funComboMenu,this);
	}
	//�ӿؼ���������ע����Щ��ť
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		_XCtrlManger.decreaseAObject(&(m_buttom[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_buttom[i]);
#endif
	}
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	//����˵��п�����ʾ����������
	m_menuTextWidth = (downMenuArea.getWidth() - downButtonArea.getWidth() - COMBO_LEFT_DISTANSE) * m_size.x/ (m_caption.getTextSize().x * m_caption.getSize().x * 0.5f);	//�����Ҫ����
	if(m_menuTextWidth < 3) return XFalse;

	m_sprite.init(m_comboInputNormal->texture.m_w,m_comboInputNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//���ݴ洢
	m_menuData = createArrayMem<char>(COMBO_MAX_MENU_LENGTH);
	if(m_menuData == NULL) return XFalse;		//�ڴ����ʧ��
	m_menuData[0] = '\0';
	char tempMenu[] = "MenuXXXX\n";
	for(int i = 0;i < m_menuSum;++ i)
	{//��ʼ�����еĲ˵��������Ϊ�ո�
		tempMenu[4] = (i /1000) % 10 + '0';
		tempMenu[5] = (i /100) % 10 + '0';
		tempMenu[6] = (i /10) % 10 + '0';
		tempMenu[7] = i % 10 + '0';
		strcat(m_menuData,tempMenu);
	}
	m_isDrawDownMenu = XFalse;
	m_nowChooseOrder = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;

	updateString();
	return XTrue;
}
_XBool _XCombo::initWithoutTex(int inputLen,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize)
{
	if(m_isInited ||				//��ֹ�ظ���ʼ��
		inputLen <= 0 ||
		menuSum <= 0 ||					//�˵��в���û�в˵���
		drawMenuSum <= 0 || drawMenuSum > menuSum ||
		fontSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	//������ֵ
	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_menuDrawSum = drawMenuSum;
	m_menuStartDrawOrder = 0;
	m_inputArea.set(0.0f,0.0f,inputLen,DEFAULT_COMBO_BT_SIZE);
	m_downButtonArea.set(0.0f,0.0f,DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_BT_SIZE);

	m_downMenuUpArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_downMenuArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_MN_HEIGHT);
	m_downMenuDownArea.set(0.0f,0.0f,inputLen + DEFAULT_COMBO_BT_SIZE,DEFAULT_COMBO_UD_HEIGHT);
	m_withoutTex = XTrue;
	m_comment.init();
	m_mouseRect.set(0.0f,0.0f,inputLen,DEFAULT_COMBO_BT_SIZE);

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_caption.setString(" ");
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_textSize.set(fontSize,fontSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	//������ť
	m_buttom = createArrayMem<_XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL) return XFalse;
	//�����ʼ�����а�ť
	if(!m_buttom[0].initWithoutTex("+",font,m_textSize.x,m_downButtonArea,m_downButtonArea.getCenter())) return XFalse;	//�����˵���ߵ������˵��İ�ť
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y);
	m_buttom[0].setSize(m_size);
	m_buttom[0].setMouseDownCB(funComboMenuState,this);
	if(!m_buttom[1].initWithoutTex("-",font,m_textSize.x,m_downMenuUpArea,m_downMenuUpArea.getCenter())) return XFalse;//�����˵����˵�����İ�ť
	m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y);
	m_buttom[1].setSize(m_size);
	m_buttom[1].setMouseDownCB(funComboMenuStart,this);
	_XVector2 tempPosition;
	tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	if(!m_buttom[2].initWithoutTex("��",font,m_textSize.x,m_downMenuDownArea,m_downMenuDownArea.getCenter())) return XFalse;		//�����˵����˵�������İ�ť
	m_buttom[2].setPosition(tempPosition);
	m_buttom[2].setSize(m_size);
	m_buttom[2].setMouseDownCB(funComboMenuEnd,this);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
		if(!m_buttom[3 + i].initWithoutTex(" ",font,m_textSize.x,m_downMenuArea,
			_XVector2(m_downMenuArea.left + m_downMenuArea.getWidth() * 0.5f,m_downMenuArea.top + m_downMenuArea.getHeight() * 0.5f))) return XFalse;		//�����˵����˵�������İ�ť
		m_buttom[3 + i].setPosition(tempPosition);
		m_buttom[3 + i].setSize(m_size);
		m_buttom[3 + i].setMouseDownCB(funComboMenu,this);
	}
	//�ӿؼ���������ע����Щ��ť
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		_XCtrlManger.decreaseAObject(&(m_buttom[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_buttom[i]);
#endif
	}
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	//����˵��п�����ʾ����������
	m_menuTextWidth = (m_downMenuArea.getWidth() - m_downButtonArea.getWidth() - COMBO_LEFT_DISTANSE) * m_size.x/ (m_caption.getTextSize().x * m_caption.getSize().x * 0.5f);	//�����Ҫ����
	if(m_menuTextWidth < 3) return XFalse;

	//���ݴ洢
	m_menuData = createArrayMem<char>(COMBO_MAX_MENU_LENGTH);
	if(m_menuData == NULL) return XFalse;		//�ڴ����ʧ��
	m_menuData[0] = '\0';
	char tempMenu[] = "MenuXXXX\n";
	for(int i = 0;i < m_menuSum;++ i)
	{//��ʼ�����еĲ˵��������Ϊ�ո�
		tempMenu[4] = (i /1000) % 10 + '0';
		tempMenu[5] = (i /100) % 10 + '0';
		tempMenu[6] = (i /10) % 10 + '0';
		tempMenu[7] = i % 10 + '0';
		strcat(m_menuData,tempMenu);
	}
	m_isDrawDownMenu = XFalse;
	m_nowChooseOrder = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;

	updateString();
	return XTrue;
}
void _XCombo::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_buttom[0].draw();
	if(m_withoutTex)
	{
		if(m_isEnable)
		{
			drawFillBoxExA(m_position + _XVector2(m_inputArea.left * m_size.x,m_inputArea.top * m_size.y),
					_XVector2(m_inputArea.getWidth() * m_size.x,
					m_inputArea.getHeight() * m_size.y),0.8f * m_color.fR,0.8f * m_color.fG,0.8f * m_color.fB,m_color.fA,true); 
		}else
		{
			drawFillBoxExA(m_position + _XVector2(m_inputArea.left * m_size.x,m_inputArea.top * m_size.y),
					_XVector2(m_inputArea.getWidth() * m_size.x,
					m_inputArea.getHeight() * m_size.y),0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,true); 
		}
	}else
	{
		if(m_isEnable) m_sprite.draw(m_comboInputNormal);
		else m_sprite.draw(m_comboInputDisable);
	}
	m_caption.draw();
}
void _XCombo::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_isDrawDownMenu || m_isInAction)
	{
		m_buttom[1].draw();
		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			m_buttom[i + 3].draw();
		}
		m_buttom[2].draw();
	}
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].drawUp();
	}
	m_comment.draw();
}
_XBool _XCombo::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	//if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(isInRect(x,y))
	{
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
	}else
	{
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
	}
	switch(mouseState)
	{
	case MOUSE_LEFT_BUTTON_DOWN:
	case MOUSE_LEFT_BUTTON_DCLICK:
		//m_mouseButtonDownIsUseable = 0;
		if(!m_allArea.isInRect(x,y)) 
		{
			if(m_isDrawDownMenu) changeDownMenuState();
		}else m_isBeChoose = XTrue;	//�ؼ����ڽ���״̬

		if(_XRect(m_position.x + m_inputArea.left * m_size.x,
			m_position.y + m_inputArea.top * m_size.y,
			m_position.x + m_inputArea.right * m_size.x,
			m_position.y + m_inputArea.bottom * m_size.y).isInRect(x,y))
		{
			changeDownMenuState();
		}
		break;
	case MOUSE_WHEEL_UP_DOWN:
		if(isInRect(x,y))
		{
			if(!m_isDrawDownMenu)
			{
				if(m_nowChooseOrder > 0)
				{
					-- m_nowChooseOrder;
					updateString();
					if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
				}
			}else
			{//�൱������ѡ��
				funComboMenuStart(this,0);
			}
		}
		break;
	case MOUSE_WHEEL_DOWN_DOWN:
		if(isInRect(x,y))
		{
			if(!m_isDrawDownMenu)
			{
				if(m_nowChooseOrder < m_menuSum - 1)
				{
					++ m_nowChooseOrder;
					updateString();
					if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
				}
			}else
			{
				funComboMenuEnd(this,0);
			}
		}
		break;
	}
	m_buttom[0].mouseProc(x,y,mouseState);	//������ť������¼�������Ӧ
	if(m_isDrawDownMenu)
	{
		for(int i = 1;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].mouseProc(x,y,mouseState);
		}
	}
//	if(mouseState == MOUSE_STATE_DOWN && m_mouseButtonDownIsUseable == 0)
//	{
//		m_isDrawDownMenu = 0;
//	}
	return XTrue;
}
_XBool _XCombo::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	if(keyState == KEY_STATE_UP)
	{
		switch(keyOrder)
		{
		case XKEY_UP:
			if(m_nowChooseOrder > 0)
			{
				-- m_nowChooseOrder;
				updateString();
				if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
			}
			break;
		case XKEY_DOWN:
			if(m_nowChooseOrder < m_menuSum - 1)
			{
				++ m_nowChooseOrder;
				updateString();
				if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
			}
			break;
		}
	}
	return XTrue;
}
_XBool _XCombo::setMenuStr(const char *str)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		str == NULL) return XFalse;
	int sum = getCharSum(str,';');
	if(sum < 0) sum = 0;
	int len = strlen(str);
	if(str[len - 1] != ';') sum += 1;		//������';'����
	if(sum > m_menuSum) sum = m_menuSum;	//����������ֻѡ������
	int offset = 0;
	for(int i = 0;i < sum;++ i)
	{
		len = getCharPosition(str + offset,';');
		string tmpStr = str + offset;
		if(len >= 0) tmpStr.at(len) = '\0';
		if(!setMenuStr(tmpStr.c_str(),i)) return XFalse;
		offset += len + 1;
	}
	return XTrue;
}
_XBool _XCombo::setMenuStr(const char *str,int order)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		order < 0 || order >= m_menuSum ||
		str == NULL ||
		strlen(str) >= COMBO_MAX_MENU_LENGTH) return XFalse;
	char temp1[COMBO_MAX_MENU_LENGTH];
	strcpy(temp1,str);
	//�������ַ����Ƿ����Ҫ��
	for(int i = 0;i < COMBO_MAX_MENU_LENGTH;++ i)
	{
		if(temp1[i] == '\0') break;
		if(temp1[i] == '\n')
		{
			temp1[i] = '\0';
			break;
		}
	}

	int len = strlen(m_menuData);
	int addLen = strlen(temp1);
	char temp[COMBO_MAX_MENU_LENGTH];
	if(order == 0)
	{//��һ��Ԫ��
		int i;
		for(i = 0;i < len;++i)
		{
			if(m_menuData[i] == '\n')
			{
				strcpy(temp,m_menuData + i);
				break;
			}
		}
		if(i >= len) return XFalse;
		if(len + addLen - i>= COMBO_MAX_MENU_LENGTH) return XFalse;
		strcpy(m_menuData,temp1);
		strcat(m_menuData,temp);
		updateString();
	}else
	{//�����Ԫ��
		int start = 0;
		int end = 0;
		int i = 0;
		int j = 0;
		for(i = 0;i < len;++ i)
		{
			if(m_menuData[i] == '\n')
			{
				j++;
				if(j == order) start = i;
				if(j == order + 1)
				{
					end = i;
					break;
				}
			}
		}
		if(start == 0 && end == 0) return XFalse;	//û���ҵ����ʵ�λ��
		if(len + addLen - (end - start) >= COMBO_MAX_MENU_LENGTH) return XFalse;
		strcpy(temp,m_menuData + end);
		m_menuData[start + 1] = '\0';
		strcat(m_menuData,temp1);
		strcat(m_menuData,temp);
		updateString();
	}
	return XTrue;
}
void _XCombo::updateString()
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	char temp[COMBO_MAX_MENU_LENGTH];
	int start = 0;
	int order = 0;
	int len = strlen(m_menuData);
	for(int i = 0;i < len;++ i)
	{
		if(m_menuData[i] == '\n')
		{
			if(order >= m_menuStartDrawOrder && order < m_menuStartDrawOrder + m_menuDrawSum)
			{
				memcpy(temp,m_menuData + start,i - start);
				temp[i - start] = '\0';
				if(i - start >= m_menuTextWidth)
				{//���������ʾ���ȣ�����ʾ"..."
					temp[m_menuTextWidth] = '\0';
					temp[m_menuTextWidth - 1] = '.';
					if(isAtUnicodeEnd(temp,m_menuTextWidth - 2)) temp[m_menuTextWidth - 2] = '.';
				}
				m_buttom[order - m_menuStartDrawOrder + 3].setCaptionText(temp);
				if(order == m_nowChooseOrder)
				{
					m_caption.setString(temp);
				}
			}else
			if(order == m_nowChooseOrder)
			{
				memcpy(temp,m_menuData + start,i - start);
				temp[i - start] = '\0';
				if(i - start >= m_menuTextWidth && m_menuTextWidth > 1)
				{//���������ʾ���ȣ�����ʾ"..."
					temp[m_menuTextWidth] = '\0';
					temp[m_menuTextWidth - 1] = '.';
					if(isAtUnicodeEnd(temp,m_menuTextWidth - 2)) temp[m_menuTextWidth - 2] = '.';
				}
				m_caption.setString(temp);
			}
			start = i + 1;
			order ++;
		}
	}
}
void _XCombo::setSize(float x,float y)			//���ÿؼ��ĳߴ�
{
	if(x <= 0 || y <= 0 ||	//�ߴ��ֵ�������
		!m_isInited) return;				//���û�г�ʼ������ֱ�ӷ���	
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y);
	m_buttom[0].setSize(m_size);
	m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y);
	m_buttom[1].setSize(m_size);
	m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	m_buttom[2].setSize(m_size);
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
		m_buttom[3 + i].setSize(m_size);
	}
	if(!m_withoutTex) m_sprite.setSize(m_size);
}
void _XCombo::setPosition(float x,float y)	//���ÿռ��λ��
{
	if(!m_isInited) return;				//���û�г�ʼ������ֱ�ӷ���
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y);
	m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y);
	m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
	}
	if(!m_withoutTex) m_sprite.setPosition(m_position);
}
_XBool _XCombo::setACopy(const _XCombo &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();	//������Ҫ�ͷ�ԭ�е���Դ
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_comboInputNormal = temp.m_comboInputNormal;					//�����˵���������ͨ״̬
	m_comboInputDisable = temp.m_comboInputDisable;					//��Ч״̬�µİ�ť��ͼ

	m_menuSum = temp.m_menuSum;							//�����˵��е��ܲ˵�������
	m_menuDrawSum = temp.m_menuDrawSum;					//�����˵�����ʾ�Ĳ˵�������
	m_menuStartDrawOrder = temp.m_menuStartDrawOrder;	//�����˵��п�ʼ��ʾ�ĵ�һ���˵���ı��
	m_menuTextWidth = temp.m_menuTextWidth;				//�����˵�����ʾ�����ֵ����ֿ��

	m_isDrawDownMenu = temp.m_isDrawDownMenu;			//�Ƿ���ʾ�����˵�
	m_nowChooseOrder = temp.m_nowChooseOrder;			//��ǰѡ��Ĳ˵���ı��

	m_caption.setACopy(temp.m_caption);			//���ֵ�����
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_sprite.setACopy(temp.m_sprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_textSize = temp.m_textSize;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_textColor = temp.m_textColor;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_menuData = createArrayMem<char>(COMBO_MAX_MENU_LENGTH);
	if(m_menuData == NULL) return XFalse;		//�ڴ����ʧ�� 	
	strcpy(m_menuData,temp.m_menuData);

	m_buttom = createArrayMem<_XButton>(m_menuDrawSum + 3);
	if(m_buttom == NULL) 
	{
		XDELETE_ARRAY(m_menuData);
		return XFalse;
	}

	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setACopy(temp.m_buttom[i]);	//�������ƵĻ����ص��������������
		if(i >= 3) m_buttom[i].setMouseDownCB(funComboMenu,this);
		_XCtrlManger.decreaseAObject(&(m_buttom[i]));	//�ӿؼ���������ע����Щ��ť
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_buttom[i]);
#endif
	}
	//Ϊ�˱��������������ע��ص�����
	m_buttom[0].setMouseDownCB(funComboMenuState,this);
	m_buttom[1].setMouseDownCB(funComboMenuStart,this);
	m_buttom[2].setMouseDownCB(funComboMenuEnd,this);

	m_funDataChange = temp.m_funDataChange;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_inputArea = temp.m_inputArea;			//��������Ӧ��Χ
	m_downButtonArea = temp.m_downButtonArea;	//������ť����Ӧ��Χ
	m_downMenuUpArea = temp.m_downMenuUpArea;		//�Ϸ�ҳ����Ӧ��Χ
	m_downMenuArea = temp.m_downMenuArea;		//ѡ�������Ӧ��Χ
	m_downMenuDownArea = temp.m_downMenuDownArea;	//�·�ҳ����Ӧ��Χ
	m_allArea = temp.m_allArea;
	m_actionMoveData = temp.m_actionMoveData;
	return XTrue;
}
_XVector2 _XCombo::getBox(int order)
{
	if(!m_isInited) return _XVector2::zero;
	if(m_isDrawDownMenu)
	{//�Ƿ���ʾ�����˵�
		switch(order)
		{
		case 0: return _XVector2(m_allArea.left,m_allArea.top);
		case 1: return _XVector2(m_allArea.right,m_allArea.top);
		case 2: return _XVector2(m_allArea.right,m_allArea.bottom);
		case 3: return _XVector2(m_allArea.left,m_allArea.bottom);
		}
	}else
	{//
		switch(order)
		{
		case 0: return _XVector2(m_allArea.left,m_allArea.top);
		case 1: return _XVector2(m_allArea.right,m_allArea.top);
		case 2: return _XVector2(m_allArea.right,m_position.y + m_inputArea.bottom * m_size.y);
		case 3: return _XVector2(m_allArea.left,m_position.y + m_inputArea.bottom * m_size.y);
		}
	}
	return _XVector2::zero;
}
void _XCombo::changeDownMenuState()
{
	if(!m_isDrawDownMenu)
	{
		m_isDrawDownMenu = XTrue;
	//	m_buttom[1].disable();
	//	if(m_menuSum > m_menuDrawSum) m_buttom[2].enable();
	//	else m_buttom[2].disable();
		if(m_nowChooseOrder <= 0
			|| m_menuSum <= m_menuDrawSum) //ѡ����ǵ�һ�������ȫ��ѡ���£���Ĭ�ϴӵ�һ�ʼ
			m_menuStartDrawOrder = 0;
		else
		{
			int tmp = m_menuDrawSum >> 1;
			if(m_nowChooseOrder >= tmp) m_menuStartDrawOrder = m_nowChooseOrder - tmp;
			if(m_nowChooseOrder + tmp >= m_menuSum) m_menuStartDrawOrder = m_menuSum - m_menuDrawSum;
		}
		if(m_menuStartDrawOrder == 0) m_buttom[1].disable();
		else m_buttom[1].enable();
		if(m_menuStartDrawOrder + m_menuDrawSum >= m_menuSum) m_buttom[2].disable();
		else m_buttom[2].enable();

		updateString();
		if(m_withAction)
		{//�������һ����̬Ч��
			m_isInAction = XTrue;
			m_isDraw = true;
			m_actionMoveData.set(0.0f,1.0f,0.005f);
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XFalse);
			}
		}
		for(int i = 0;i < m_menuDrawSum + 2;++ i)
		{//����������ť�ɼ�
			m_buttom[1 + i].setVisible();
		}
	}else
	{
		m_isDrawDownMenu = XFalse;
		if(m_withAction)
		{//�������һ����̬Ч��
			m_isInAction = XTrue;
			m_isDraw = false;
			m_actionMoveData.set(1.0f,0.0f,0.005f);
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{
				m_buttom[1 + i].setWithAction(XFalse);
			}
		}
		if(!m_withAction)
		{
			for(int i = 0;i < m_menuDrawSum + 2;++ i)
			{//����������ť���ɼ�
				m_buttom[1 + i].disVisible();
			}
		}
	}
}
std::string _XCombo::getMenuStr() const
{
	char tmp[COMBO_MAX_MENU_LENGTH];
	memcpy(tmp,m_menuData,COMBO_MAX_MENU_LENGTH);
	for(int i = 0;i < COMBO_MAX_MENU_LENGTH;++ i)
	{
		if(tmp[i] == '\0') break;
		if(tmp[i] == '\n') tmp[i] = ';';
	}
	return tmp;
}
std::string _XCombo::getMenuStr(int order)
{
	if(order < 0 || order >= m_menuSum) return "";
	char tmp[COMBO_MAX_MENU_LENGTH];
	memcpy(tmp,m_menuData,COMBO_MAX_MENU_LENGTH);
	int s = 0;
	int index = 0;
	for(int i = 0;i < COMBO_MAX_MENU_LENGTH;++ i)
	{
		if(tmp[i] == '\0')
		{
			if(index == order)
			{
				tmp[i] = '\0';
				return tmp + s;
			}
			break;
		}
		if(tmp[i] == '\n')
		{
			if(index == order)
			{
				tmp[i] = '\0';
				return tmp + s;
			}
			++ index;
			if(index == order) s = i + 1;
		}
	}
	return "";
}