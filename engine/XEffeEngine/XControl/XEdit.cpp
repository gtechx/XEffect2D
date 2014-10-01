//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEdit.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

//��һ������Edit�ڲ����õ��ڴ�ռ䣬��Ҫ������ʵ��֮��ĸ���ճ��ʱ���ڴ��������
char copyString[MAX_INPUT_STRING_LENGTH] = "";

_XEditTexture::_XEditTexture()
:m_isInited(XFalse)
,editNormal(NULL)			//�������ͨ״̬
,editDisable(NULL)			//�������Ч״̬
,editSelect(NULL)			//�����Ƭѡ��ɫ
,editInsert(NULL)			//����������
,editUpon(NULL)
{}
_XBool _XEditTexture::init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon,_XResourcePosition resoursePosition)
{
	if(m_isInited ||	//��ֹ�ظ���ʼ��
		normal == NULL || disable == NULL || insert == NULL || select == NULL) return XFalse;//��Щ�ؼ�����Դ����Ϊ��
	int ret = 1;
	if((editNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editInsert = createATextureData(insert,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editSelect = createATextureData(select,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define EDIT_CONFIG_FILENAME ("Edit.txt")
_XBool _XEditTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
		if((editNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editInsert = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//upon
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((editUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((editInsert = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//upon
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((editUpon = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
_XEdit::_XEdit()
:m_isInited(XFalse)					//�Ƿ��ʼ��
,m_mouseRightButtonMenu(NULL)	//����Ҽ��˵�
,m_nowString(NULL)				//��ǰ������ַ���
,m_tempNowString(NULL)
//,m_insertString(NULL)			//������ַ���
//,m_selectString(NULL)			//ѡȡ���ַ���
,m_haveSelect(XFalse)
,m_editNormal(NULL)				//�������ͨ״̬
,m_editDisable(NULL)			//�������Ч״̬
,m_editSelect(NULL)				//�����Ƭѡ��ɫ
,m_editInsert(NULL)				//����������
,m_funInputChenge(NULL)			//�������ݷ����ı��ʱ�����
,m_funInputOver(NULL)			//ȷ���������֮���ʱ�����
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_nowKeyDown(XFalse)
,m_isPassword(false)
{//�����ʼ������������״̬
	if(getCapsLockState()) m_keyCapsLockState = 1;
	else m_keyCapsLockState = 0;
	if(getNumLockState()) m_keyNumLockState = 1;
	else m_keyNumLockState = 0;
	m_ctrlType = CTRL_OBJ_EDIT;
}
_XBool _XEdit::init(const _XVector2& position,		//�ؼ���λ��
		const _XRect& Area,					//�ؼ��������Ӧ����
		const _XEditTexture &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu)		//�ؼ����Ҽ��˵�
{
	if(m_isInited ||
		tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return XFalse;
	if(mouseMenu != NULL)
	{//������Ҫ�Ľ�������ʹ��ָ�룬���ܵ����޸ĵ�Ӱ��
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//�ӿؼ��������н�����ؼ�ע����
		_XCtrlManger.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//���濪ʼ��ֵ
	m_mouseRect = Area;	//��¼������Ӧ��Χ
	m_position = position;
	m_withoutTex = XFalse;
	m_comment.init();

	m_editNormal = tex.editNormal;			//�������ͨ״̬
	m_editDisable = tex.editDisable;		//�������Ч״̬
	m_editSelect = tex.editSelect;			//�����Ƭѡ��ɫ
	m_editInsert = tex.editInsert;			//����������
	m_editUpon = tex.editUpon;			//����������
	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
//	m_insertString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//��ǰ������ַ���
//	if(m_insertString == NULL) return XFalse;
//	m_insertString[0] = '\0';
//	m_selectString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);	//��ǰ������ַ���
//	if(m_selectString == NULL) return XFalse;
//	m_selectString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//��ǰѡ�������λ��
		m_nowStringLength = strlen(m_nowString);	//��ǰ�����ַ����ĳ���
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//��ǰѡ�������λ��
		m_nowStringLength = 0;	//��ǰ�����ַ����ĳ���
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XEdit::initWithoutTex(const _XRect& area,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu)
{
	if(m_isInited ||
	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
		strSize <= 0 ||		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
		(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH)) return XFalse;
	if(mouseMenu != NULL)
	{//������Ҫ�Ľ�������ʹ��ָ�룬���ܵ����޸ĵ�Ӱ��
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//�ӿؼ��������н�����ؼ�ע����
		_XCtrlManger.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//���濪ʼ��ֵ
	m_mouseRect = area;	//��¼������Ӧ��Χ
	m_position.set(0.0f,0.0f);
	m_withoutTex = XTrue;
	m_comment.init();

	m_size.set(1.0f,1.0f);
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowString[0] = '\0';
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempNowString[0] = '\0';
	m_nowShowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL) 
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//��ǰѡ�������λ��
		m_nowStringLength = strlen(m_nowString);	//��ǰ�����ַ����ĳ���
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//��ǰѡ�������λ��
		m_nowStringLength = 0;	//��ǰ�����ַ����ĳ���
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��

	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	upDateInsertShowPosition();

	//printf("Can Show String Length:%d\n",m_canShowLength);
	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;
	m_isBeChoose = XFalse; 

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XEdit::upDataShowStr()
{//��û��ʵ��
	if(m_nowStringLength <= m_nowShowStart + m_canShowLength) 
	{
		m_nowShowStart = m_nowStringLength - m_canShowLength;
		if(m_nowShowStart < 0) m_nowShowStart = 0;
		strcpy(m_nowShowString,m_nowString + m_nowShowStart);
	}else
	{
		if(isAtUnicodeEnd(m_nowString,m_nowShowStart + m_canShowLength)) 
		{
			memcpy(m_nowShowString,m_nowString + m_nowShowStart,m_canShowLength);
			m_nowShowString[m_canShowLength] = '\0';
		}else 
		{
			memcpy(m_nowShowString,m_nowString + m_nowShowStart,m_canShowLength - 1);
			m_nowShowString[m_canShowLength - 1] = '\0';
		}
	}
	//���µ���ʾ��
	m_caption.setString(m_nowShowString);
}
void _XEdit::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA,true,false,true);
		}else 
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.75f * m_color.fR,0.75f * m_color.fG,0.75f * m_color.fB,m_color.fA,true,false,true);
		}

		if(m_haveSelect) 
		{
			drawFillBoxExA(_XVector2(m_selectRect.left,m_selectRect.top),_XVector2(m_selectRect.right,m_selectRect.bottom),0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA,true,false,true);
		}

		//��ʾ��ǰ������ַ���
		m_caption.draw();
		if(m_insertActionMD.getIsEnd())
		{
			if(m_isEnable && m_isBeChoose)
			{
				if(m_timer > 250) 
				{
					float x = m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth;
					float y = m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f;
					drawLine(x,y,x,y + m_nowTextHeight,1,0.25f * m_color.fR,0.25f * m_color.fG,0.25f * m_color.fB,m_color.fA);
					//m_spriteInsert.draw(m_editInsert);
				}
			}
		}
		//�����ϲ�
	//	if(m_editUpon != NULL) 
	//	{
	//		m_spriteBackGround.draw(m_editUpon);
	//	}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_editDisable);	//�����Ч����ʾ��Ч,����ʾ�������
		else m_spriteBackGround.draw(m_editNormal);//�����Ч����ʾ��Ч,��ʾ�������

		if(m_haveSelect) m_spriteSelect.draw(m_editSelect);//��ʾѡ��ķ�Χ

		//��ʾ��ǰ������ַ���
		m_caption.draw();
		if(m_isEnable && m_isBeChoose && m_timer > 250)
			m_spriteInsert.draw(m_editInsert);

		if(m_editUpon != NULL) m_spriteBackGround.draw(m_editUpon);	
	}
}
void _XEdit::release()
{
	if(!m_isInited) return;
	XDELETE_ARRAY(m_nowString);
	XDELETE_ARRAY(m_tempNowString);
	//XDELETE_ARRAY(m_insertString);
	//XDELETE_ARRAY(m_selectString);
	XDELETE_ARRAY(m_nowShowString);
	XDELETE(m_mouseRightButtonMenu);
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
void _XEdit::setString(const char *str)
{//����������ٺܸ��ӵ��߼�����������û����ڴ�������׶λ������ڴ���Ƭѡ�׶ε�ʱ������޸Ľ�����ɽ�Ϊ���Ӱ�죬����������ܻ���һЩ�����Ի��Ĵ���
	if(!m_isInited ||
		str == NULL) return;
	m_caption.setString(str);
	strcpy(m_nowString,str);
	m_nowInsertPoint = strlen(m_nowString);	//��ǰѡ�������λ��
	m_nowStringLength = strlen(m_nowString);	//��ǰ�����ַ����ĳ���
	//ȡ������Ƭѡ����Ϣ
	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��
	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	upDateInsertShowPosition();
}
void _XEdit::mouseRightMenuProc()
{
	int TextChange = 0;
	if(m_mouseRightButtonMenu->getLastChoose() == 0)
	{//����
		if(m_haveSelect)
		{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
			//1������Ƭѡ������
#ifdef XEE_OS_WINDOWS
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
			setTextToClipboard(copyString);
#endif
#ifdef XEE_OS_LINUX
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
#endif
			//2��ɾ��Ƭѡ������
			deleteSelectStr();
			m_haveSelect = XFalse;	//Ƭѡ����
			TextChange = 1;
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 1)
	{//����
		if(m_haveSelect)
		{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
			//����Ƭѡ������
#ifdef XEE_OS_WINDOWS
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
			setTextToClipboard(copyString);
#endif
#ifdef XEE_OS_LINUX
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
#endif
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 2)
	{//ճ��
		//�жϹ����ڴ�ռ����Ƿ�������
#ifdef XEE_OS_WINDOWS
		std::string tmp = getTextFromClipboard();
		if(tmp == "") copyString[0] = '\0';	//û����
		else 
		{
			if(tmp.size() >= MAX_INPUT_STRING_LENGTH - 1)
			{
				memcpy(copyString,tmp.c_str(),MAX_INPUT_STRING_LENGTH - 1);
				copyString[MAX_INPUT_STRING_LENGTH - 1] = '\0';
			}else
			{
				memcpy(copyString,tmp.c_str(),tmp.size());
				copyString[tmp.size()] = '\0';
			}
		}
#endif
		if(copyString[0] != '\0')
		{
			char haveEnd = 0;
			for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
			{
				if(copyString[i] == '\0')
				{
					haveEnd = 1;
					break;
				}
			}
			if(haveEnd != 0)
			{
				if(m_haveSelect)
				{//����Ƭѡ(����ʵ�����滻����)
					if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
					{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
					}else
					{							
						//1��ɾ��Ƭѡ����
						deleteSelectStr();
						m_haveSelect = XFalse;	//Ƭѡ����
						//2�����븴������
						if(m_nowInsertPoint == m_nowStringLength)
						{//��������ַ�����ĩβ���
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}else
				{//û��Ƭѡ
					//�ж�ճ���������Ƿ�ᳬ���ַ������ܳ���
					if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
					{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
					}else
					{
						if(m_nowInsertPoint == m_nowStringLength)
						{//��������ַ�����ĩβ���
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
						//	m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}
			}
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 3)
	{//����(�������Ƭѡ��ȡ��Ƭѡ)
		if(m_haveSelect) m_haveSelect = XFalse;
	}
	printf("%d\n",m_mouseRightButtonMenu->getLastChoose());
	if(TextChange != 0)
	{
		upDataShowStr();	//��Ϊ�ַ������ı䣬����������Ҫ�����ַ�����ʾ
	//	m_caption.setString(m_nowString);
	//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
	//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
		if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
		//�����ݷ����仯��ʱ�򲥷Ŷ���
		if(m_withAction)
		{//�������һ����̬Ч��
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_size;
		}
	}
}
void _XEdit::changeInsertPoint(int sum)
{
	if(sum == 0) return;
	int tempNewPosition = m_nowInsertPoint + sum;
	if(tempNewPosition < 0) return;
	if(tempNewPosition > m_nowStringLength) return;
	//������Ҫ������ĵ�����
	if(!isAtUnicodeEnd(m_nowString,tempNewPosition)) 
	{//������������ٽ�Ͷ��ƶ�һλ,(�����������)
		if(sum < 0) changeInsertPoint(sum - 1);//����
		else changeInsertPoint(sum + 1);//ǰ��
		return;
	}
	//�����ƶ�
	int tempOldPosition = m_nowInsertPoint;		//�ɹ���λ��
	m_nowInsertPoint = tempNewPosition;
	//����Ƿ�Ҫ�ƶ���ʾ����
	if(m_nowInsertPoint < m_nowShowStart || m_nowInsertPoint > m_nowShowStart + m_canShowLength)
	{//������ʾ��Χ��Ҫ�޸���ʾ��Χ
		if(m_nowInsertPoint < m_nowShowStart)
		{//��ǰ�ƶ�
			if(tempOldPosition != m_nowShowStart)
			{//���֮ǰ���û���������Ŀ�ͷ������Ҫ���µ���ͷ
				m_nowShowStart = m_nowInsertPoint;
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint;
			}
		}else
		if(m_nowInsertPoint > m_nowShowStart + m_canShowLength)
		{//����ƶ�
			if(tempOldPosition != m_nowShowStart + m_canShowLength)
			{//���֮ǰ�Ĺ�겻��������β����Ҫ���¹�굽��β
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
				if(!isAtUnicodeEnd(m_nowString,m_nowShowStart)) ++m_nowShowStart; //���ͷ�ٽ硢����ƶ�һλ
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
				if(!isAtUnicodeEnd(m_nowString,m_nowShowStart)) ++m_nowShowStart; //���ͷ�ٽ硢����ƶ�һλ
				upDateInsertShowPosition();
			}
		}
		upDataShowStr();
	}else
	{//����Ҫ�޸���ʾ��Χ
		upDateInsertShowPosition();
	}
}
void _XEdit::upDataSelectShow()
{
	int tempHead = getSelectHead();
	int tempEnd = getSelectEnd();
	if(tempHead < m_nowShowStart) tempHead = m_nowShowStart;
	if(tempHead >= m_nowShowStart + m_canShowLength)
	{//����������Ҫ��ʾ
		tempHead = m_nowShowStart + m_canShowLength;
	}
	if(tempEnd <= m_nowShowStart)
	{//�������Ҳ����Ҫ��ʾ
		tempEnd = m_nowShowStart;
	}
	if(tempEnd > m_nowShowStart + m_canShowLength) tempEnd = m_nowShowStart + m_canShowLength;
	//����ʵ�ʵ��������ѡ��λ�ú�ѡ��ͼƬ��Ҫ���õĳߴ�
	if(m_withoutTex)
	{
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f,(tempEnd - tempHead) * m_nowTextWidth,m_nowTextHeight);
		}else
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f,m_canShowLength * m_nowTextWidth,m_nowTextHeight);
		}
	}else
	{
		m_spriteSelect.setPosition(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
			m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_spriteSelect.setSize((tempEnd - tempHead) * m_nowTextWidth /m_editSelect->textureSize.x,
				m_nowTextHeight / m_editSelect->textureSize.y);
		}else
		{
			m_spriteSelect.setSize(m_canShowLength * m_nowTextWidth /m_editSelect->textureSize.x,
				m_nowTextHeight / m_editSelect->textureSize.y);
		}
	}
}
_XBool _XEdit::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	//���������Ӧ�����λ��
//	_XRect temp;
//	temp.set(m_objRect.left,m_objRect.top,m_objRect.left + m_objRect.getWidth() * m_size.x,m_objRect.top + m_objRect.getHeight() * m_size.y);
	if(m_mouseRightButtonMenu != NULL)
	{//�Ҽ��˵��������������
		int  flagMouseMenu = 0;
		if(m_mouseRightButtonMenu->getVisible()) flagMouseMenu = 1;
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//������Ҽ���������������Ҫ�ڿռ䷶Χ��
			if(m_nowMouseRect.isInRect(x,y) && m_isBeChoose)
			{
				m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
			}
		}else
		{
			m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}
		if(flagMouseMenu == 1 && !m_mouseRightButtonMenu->getVisible())
		{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
			mouseRightMenuProc();//�Ҽ��˵�����Ӧ
			if(m_mouseRightButtonMenu->getLastChoose() !=  -1)
			{//����˵��в��������������Ϣ����
				return XTrue;	//���Ҽ��˵��Ĳ�����Ч�Ļ�������������Ϣ�������ظ�����
			}
		}
	} 
	if(m_nowMouseRect.isInRect(x,y))
	{//����ڷ�Χ�ڵ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		//���������Ҽ������Գ���
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//���	
			if(!m_isBeChoose)
			{
				m_isBeChoose = XTrue;//����ؼ�û�д��ڼ���״̬�򼤻�
				if(m_withAction)
				{
					m_insertActionMD.set(10.0f,1.0f,0.005f);//,MOVE_DATA_MODE_SQRT2_MULT);
				}
			}
			if(m_nowStringLength > 0)
			{//�ַ��������ַ�ʱ������ͬͨ�����ѡ�����λ��(����ĳ���10��Ϊ�˷���ѡ��ͷ����β)
				if(m_nowMouseRect.isInRect(x,y))
				{//��갴�¶�����Ч
					//�������λ��
					if(x <= m_position.x + m_textPosition.x * m_size.x)
					{//ͷ
						if(m_nowInsertPoint != m_nowShowStart) changeInsertPoint(m_nowShowStart - m_nowInsertPoint);
					}else
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
					{//β
						if(m_nowStringLength <= m_canShowLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{//����ʾ�Ľ���
							if(m_nowInsertPoint != m_nowShowStart + m_canShowLength) changeInsertPoint(m_nowShowStart + m_canShowLength - m_nowInsertPoint);
						}
					}else
					{//��Χ��
						//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
						//Ϊ�����ѡ��ʱ�ĸо��������λ����ַ��Ŀ�ȣ�ʹѡ���ʱ������ио�
						int tempInsertPoint = (x + m_nowTextWidth * 0.5f - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
						if(tempInsertPoint + m_nowShowStart > m_nowStringLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{
							if(m_nowInsertPoint != tempInsertPoint + m_nowShowStart) changeInsertPoint(tempInsertPoint + m_nowShowStart - m_nowInsertPoint);
						}
					}
					if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
					{//�����������£��������Ƭѡ����
						m_haveSelect = XFalse;
						m_selectEnd = m_nowInsertPoint;
						m_selectStart = m_nowInsertPoint;
						m_selectMouseDown = XTrue;
					}else
					if(mouseState == MOUSE_RIGHT_BUTTON_DOWN)
					{//������Ҽ����²������ٷ�Χ��Ҳ�������Ƭѡ����
						if(m_nowInsertPoint > getSelectEnd() || m_nowInsertPoint < getSelectHead())
						{
							m_haveSelect = XFalse;
						}
					}
				}
			}
		}
	}else
	{//����ڷ�Χ������ȥ����
		if(m_isMouseInRect)
		{
			m_isMouseInRect = XFalse;
			m_comment.disShow();
		}
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//���
			if(m_isBeChoose)
			{
				m_isBeChoose = XFalse;//����ؼ�û�д��ڼ���״̬�򼤻�
				if(m_withAction)
				{
					m_insertActionMD.set(1.0f,0.0f,0.002f);
				}
			}
		}
	}

	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//�κ����������²����������Ƭѡ����
		if(m_haveSelect) m_haveSelect = XFalse;
	}

	if(mouseState == MOUSE_MOVE)
	{
		if(m_selectMouseDown)
		{//�����갴�£������϶�������ΪƬѡ��������
			//����Ƭѡ��λ��
			if(x <= m_position.x + m_textPosition.x * m_size.x)
			{//ͷ
				if(m_nowShowStart > 0 && x <= m_position.x + m_textPosition.x * m_size.x - TEXT_EDGE_WIDTH) 
				{//�������ͷ��������ǰ�ƶ�
					m_selectEnd = m_nowShowStart - 1;
					if(!isAtUnicodeEnd(m_nowString,m_selectEnd)) --m_selectEnd;
				}else 
				{
					m_selectEnd = m_nowShowStart;
				}
			}else
			if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
			{//β
				if(m_nowShowStart + m_canShowLength >= m_nowStringLength)
				{//����ַ���û����ʾ��
					m_selectEnd = m_nowStringLength;
				}else
				{
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x + TEXT_EDGE_WIDTH)
					{//�������β��������ƶ�
						m_selectEnd = m_nowShowStart + m_canShowLength + 1;
						if(!isAtUnicodeEnd(m_nowString,m_selectEnd)) ++m_selectEnd;
					}else
					{
						m_selectEnd = m_nowShowStart + m_canShowLength;
					}
				}
			}else
			{//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
				int tempEnd = m_selectEnd;
				int tempPosition = (x - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
				if(tempPosition + m_nowShowStart >= m_nowStringLength) m_selectEnd = m_nowStringLength;
				else m_selectEnd = m_nowShowStart + tempPosition;

				if(!isAtUnicodeEnd(m_nowString,m_selectEnd))
				{
					if(m_selectEnd > tempEnd) ++m_selectEnd;
					else --m_selectEnd;
				}
			}
			if(m_selectEnd != m_nowInsertPoint) changeInsertPoint(m_selectEnd - m_nowInsertPoint);	//�������ҲҪ����仯
			if(m_selectEnd != m_selectStart)
			{//Ƭѡ��Ч
				m_haveSelect = XTrue;
				upDataSelectShow();
			}else m_haveSelect = XFalse;	//Ƭѡ��Ч
		}
	}	
	if(m_isBeChoose && (mouseState == MOUSE_LEFT_BUTTON_UP || mouseState == MOUSE_RIGHT_BUTTON_UP))
	{//����ǵ����¼��������Ƿ��ڷ�Χ�ڣ�ֻҪ�ռ䴦�ڼ���״̬�����������Ӧ
		if(m_selectMouseDown)
		{//Ƭѡ�����ж�
			if(m_selectEnd != m_selectStart) m_haveSelect = XTrue;	//Ƭѡ��Ч
			else m_haveSelect = XFalse;	//Ƭѡ��Ч
			m_selectMouseDown = XFalse;
		}
	}
	return XTrue;
}
_XBool _XEdit::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(m_mouseRightButtonMenu != NULL)
	{
		if(m_mouseRightButtonMenu->getVisible())
		{//����Ҽ��˵��ɼ�,�����а������������������Ӱ��
			m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState);
			if(!m_mouseRightButtonMenu->getVisible())
			{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
				mouseRightMenuProc();//�Ҽ��˵�����Ӧ
			}
			return XFalse;
		}
	}
	if(!m_isBeChoose) return XFalse;
	switch(keyState)
	{
	case KEY_STATE_DOWN:
		switch(keyOrder)
		{
		case XKEY_RSHIFT:
		case XKEY_LSHIFT://shift������
			m_keyShiftState = 1;
			break;
		case XKEY_CAPSLOCK://CapsLock������
			m_keyCapsLockState = 1;
			break;
		case XKEY_NUMLOCK://XKEY_NUMLOCK������
			m_keyNumLockState = 1;
			break;
		case XKEY_RCTRL:
		case XKEY_LCTRL://Ctrl������
			m_keyCtrlState = 1;
			break;
		case XKEY_UNKNOWN:
			break;
		default:
		//	printf("keyDown:%d\n",keyOrder);
			m_nowKeyDown = XTrue;
			m_nowKeyDownTimer = 0;
			m_nowKeyRepTimer = 1000;
			m_nowKey = keyOrder;
			if(keyProc(keyOrder)) return XTrue;	//��������ʱ������Ӧ 
			break;
		}
		break;
	case KEY_STATE_UP:
		switch(keyOrder)
		{
		case XKEY_RSHIFT:
		case XKEY_LSHIFT://shift������
			m_keyShiftState = 0;
			break;
		case XKEY_CAPSLOCK://CapsLock������
			m_keyCapsLockState = 0;
			break;
		case XKEY_NUMLOCK://XKEY_NUMLOCK������
			m_keyNumLockState = 0;
			break;
		case XKEY_RCTRL:
		case XKEY_LCTRL://Ctrl������
			m_keyCtrlState = 0;
			break;
		case XKEY_MENU://�˵��������£����൱��������һ�(�˵������������Ӧ)
			if(m_mouseRightButtonMenu != NULL)
				m_mouseRightButtonMenu->mouseProc(m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
					m_position.y + m_textPosition.y * m_size.y,MOUSE_RIGHT_BUTTON_UP);
			break;
		case XKEY_UNKNOWN:
			break;
		default:
			m_nowKeyDown = XFalse;
			break;
		}
		break;
	}

	return XFalse;
}
_XBool _XEdit::keyProc(int keyOrder)
{
	int TextChange = 0;		//�ı������Ƿ����仯
	if(m_keyCtrlState == 0)
	{//ctrlû�а��µ�����������ܴ���
		if(m_haveSelect == 0)
		{//û��Ƭѡ��ʱ��
			if(m_nowStringLength < MAX_INPUT_STRING_LENGTH - 2)
			{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
				//�����ǿɴ�ӡ�ַ��Ĵ���			
				TextChange = keyJudgement(keyOrder);
			}
		}else
		{//����Ƭѡ��ʱ��
			if(m_nowStringLength - getSelectLength() < MAX_INPUT_STRING_LENGTH - 2)
			{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
				//�����ǿɴ�ӡ�ַ��Ĵ���			
				TextChange = keyJudgement(keyOrder);
			}
		}
	}else
	{//ctrl���µ�����������Ϊ���ܼ�����
		if(keyOrder ==  XKEY_C)
		{//ctrl + c ����
			if(m_haveSelect)
			{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
				//����Ƭѡ������
#ifdef XEE_OS_WINDOWS
				memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
				copyString[getSelectLength()] = '\0';
				setTextToClipboard(copyString);
#endif
#ifdef XEE_OS_LINUX
				memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
				copyString[getSelectLength()] = '\0';
#endif
			}
		}else
		if(keyOrder ==  XKEY_V)
		{//ctrl + v ճ��
			//�жϹ����ڴ�ռ����Ƿ�������
#ifdef XEE_OS_WINDOWS
			std::string tmp = getTextFromClipboard();
			if(tmp == "") copyString[0] = '\0';	//û����
			else 
			{
				if(tmp.size() >= MAX_INPUT_STRING_LENGTH - 1)
				{
					memcpy(copyString,tmp.c_str(),MAX_INPUT_STRING_LENGTH - 1);
					copyString[MAX_INPUT_STRING_LENGTH - 1] = '\0';
				}else
				{
					memcpy(copyString,tmp.c_str(),tmp.size());
					copyString[tmp.size()] = '\0';
				}
			}
#endif
			if(copyString[0] != '\0')
			{
				char haveEnd = 0;
				for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
				{
					if(copyString[i] == '\0')
					{
						haveEnd = 1;
						break;
					}
				}
				if(haveEnd != 0)
				{
					if(m_haveSelect)
					{//����Ƭѡ(����ʵ�����滻����)
						if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
						{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
						}else
						{							
							//1��ɾ��Ƭѡ����
							deleteSelectStr();
							m_haveSelect = XFalse;	//Ƭѡ����
							//2�����븴������
							if(m_nowInsertPoint == m_nowStringLength)
							{//��������ַ�����ĩβ���
								strcat(m_nowString,copyString);
								m_nowStringLength = strlen(m_nowString);
								//m_nowInsertPoint = m_nowStringLength;
								changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
								TextChange = 1;
							}else
							{
								strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
								strcpy(m_nowString + m_nowInsertPoint,copyString);
								strcat(m_nowString,m_tempNowString);
								m_nowStringLength = strlen(m_nowString);
								//m_nowInsertPoint += strlen(copyString);
								changeInsertPoint(strlen(copyString));
								TextChange = 1;
							}
						}
					}else
					{//û��Ƭѡ
						//�ж�ճ���������Ƿ�ᳬ���ַ������ܳ���
						if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
						{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
						}else
						{
							if(m_nowInsertPoint == m_nowStringLength)
							{//��������ַ�����ĩβ���
								strcat(m_nowString,copyString);
								m_nowStringLength = strlen(m_nowString);
							//	m_nowInsertPoint = m_nowStringLength;
								changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
								TextChange = 1;
							}else
							{
								strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
								strcpy(m_nowString + m_nowInsertPoint,copyString);
								strcat(m_nowString,m_tempNowString);
								m_nowStringLength = strlen(m_nowString);
								//m_nowInsertPoint += strlen(copyString);
								changeInsertPoint(strlen(copyString));
								TextChange = 1;
							}
						}
					}
				}
			}			
		}
	}
	if(keyOrder ==  XKEY_BACKSPACE)
	{//���˼�
		if(!m_haveSelect)
		{//û��Ƭѡ��ʱ��
			if(m_nowInsertPoint > 0)
			{
				if(!isAtUnicodeEnd(m_nowString,m_nowInsertPoint - 1))
				{
					m_nowStringLength -= 2;
					changeInsertPoint(-2);
					if(m_nowStringLength == m_nowInsertPoint) m_nowString[m_nowInsertPoint] = '\0';	//�ַ���ĩβ
					else strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 2);
				}else
				{
					-- m_nowStringLength;
					changeInsertPoint(-1);
					if(m_nowStringLength == m_nowInsertPoint) m_nowString[m_nowInsertPoint] = '\0';	//�ַ���ĩβ
					else strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
				}
				TextChange = 1;
			}
		}else
		{//����Ƭѡ��ʱ��
			if(!m_selectMouseDown)
			{//Ƭѡ�Ѿ�����
				//1��ɾ��Ƭѡ����
				deleteSelectStr();
				m_haveSelect = XFalse;	//Ƭѡ����
				TextChange = 1;
			}else
			{//Ƭѡ��δ����
				//����������
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					//1��ɾ��Ƭѡ����
					deleteSelectStr();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч

				m_haveSelect = XFalse;	//Ƭѡ����
				TextChange = 1;
				//Ȼ���������¿�ʼ����
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				m_selectMouseDown = XTrue;
			}
		}
	}else
	if(keyOrder == XKEY_DELETE || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N_PERIOD))
	{//Delete����
		if(!m_haveSelect)
		{//û��Ƭѡ��ʱ��
			if(m_nowStringLength == m_nowInsertPoint)
			{//�ַ���ĩβ��������
			}else
			{
				//������Ҫ�жϺ����Ǹ��ַ��Ƿ�Ϊ���ģ����Ϊ���ĵĻ�������Ҫɾ�������ַ�
				if(m_nowString[m_nowInsertPoint] < 0)
				{
					strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 2);
					m_nowStringLength -= 2;
				}else
				{
					strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
					-- m_nowStringLength;
				}
				TextChange = 1;
			}
		}else
		{//����Ƭѡ��ʱ��
			if(!m_selectMouseDown)
			{//Ƭѡ�Ѿ�����
				//1��ɾ��Ƭѡ����
				deleteSelectStr();
				m_haveSelect = XFalse;	//Ƭѡ����
				TextChange = 1;
			}else
			{//Ƭѡ��δ����
				//����������
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					//1��ɾ��Ƭѡ����
					deleteSelectStr();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч

				TextChange = 1;
				//Ȼ���������¿�ʼ����
				m_haveSelect = XFalse;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				m_selectMouseDown = XTrue;
			}
		}
	}else
	if(keyOrder ==  XKEY_LEFT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N4))
	{//���󰸼�����(�ַ�������ı䣬���ǻ�ı�����λ��)
		if(m_keyShiftState == 0)
		{//���û�а���shift��
			if(!m_selectMouseDown)
			{
				if(!m_haveSelect) changeInsertPoint(-1);	//û��Ƭѡ,�ƶ�����λ��
				else
				{//����Ƭѡ
				//	m_nowInsertPoint = m_selectStart;
				//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
				//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
					changeInsertPoint(m_selectStart - m_nowInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//�������shift����ΪƬѡ����
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//Ƭѡ��Ч
			}
		}
	}else
	if(keyOrder ==  XKEY_RIGHT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N6))
	{//���󰸼�����(�ַ�������ı䣬���ǻ�ı�����λ��)
		if(m_keyShiftState == 0)
		{//���û�а���shift��
			if(!m_selectMouseDown)
			{
				if(!m_haveSelect) changeInsertPoint(1);	//û��Ƭѡ,�ƶ�����λ��
				else
				{//����Ƭѡ
					changeInsertPoint(m_selectEnd - m_nowInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//�������shift����ΪƬѡ����
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint < m_nowStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint < m_nowStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}
		}
	}else
	if(keyOrder ==  XKEY_HOME || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N7))
	{//home
		if(m_keyShiftState == 0)
		{//���shiftû�а���
			if(!m_selectMouseDown)
			{//���û�����ѡ��(��������ѡ���򰴼���Ч)
				if(m_nowInsertPoint != 0) changeInsertPoint(-m_nowInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//���shift������
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint > 0)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(-m_nowInsertPoint);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint > 0)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(-m_nowInsertPoint);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}
		}
	}else
	if(keyOrder ==  XKEY_END || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N1))
	{//end(���û��Ƭѡ����ʱ������Ч)
		if(m_keyShiftState == 0)
		{//���shiftû�а���
			if(!m_selectMouseDown)
			{//���û�����ѡ��(��������ѡ���򰴼���Ч)
				if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//���shift������
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint < m_nowStringLength)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_nowInsertPoint < m_nowStringLength)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
					m_selectEnd = m_nowInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}
		}
	}

	if(TextChange != 0)
	{
		//����ַ��������ı䣬��Ҫ�����ַ���������
		upDataShowStr();	//�����ַ�������ʾ
		if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
		//�����ݷ����仯��ʱ�򲥷Ŷ���
		if(m_withAction)
		{//�������һ����̬Ч��
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_size;
		}
		return XTrue;
	}
	if(keyOrder ==  XKEY_RETURN || keyOrder == XKEY_N_ENTER)
	{//�س���
		//if(m_haveSelect) m_haveSelect = XFalse;	//�س���ȡ��ѡ��
		if(m_funInputOver != NULL) m_funInputOver(m_pClass,m_objectID);
		if(m_withAction)
		{//�������һ����̬Ч��
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_size;
		}
		return XTrue;
	}
	return XFalse;
}
void _XEdit::setSize(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_size);
	//m_spriteBackGround.setPosition(m_position);
	if(!m_withoutTex) m_spriteBackGround.setSize(m_size);
	upDateInsertShowPosition();
	//m_spriteSelect.setSize(m_size);
	upDataSelectShow();
	if(!m_withoutTex) m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	updateChildSize();
}
_XBool _XEdit::setACopy(const _XEdit &temp)			//����Ϊһ���������ò�����Դ		
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();//��������Ѿ���ʼ��������Ҫ�ͷ���Դ
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;	//�Ƿ��ʼ��
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	m_selectRect = temp.m_selectRect;

	if(temp.m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu = createMem<_XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;

		m_mouseRightButtonMenu->setACopy(* (temp.m_mouseRightButtonMenu));	//����Ҽ��˵�
		//�ӿؼ��������н�����ؼ�ע����
		_XCtrlManger.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//��ʾ��ǰ������ַ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_nowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	strcpy(m_nowString,temp.m_nowString);
	m_tempNowString = createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_tempNowString == NULL) 
	{
		XDELETE_ARRAY(m_nowString);
		return XFalse;
	}
	strcpy(m_tempNowString,temp.m_tempNowString);
	m_nowShowString	= createArrayMem<char>(MAX_INPUT_STRING_LENGTH);
	if(m_nowShowString == NULL)
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE_ARRAY(m_tempNowString);
		return XFalse;
	}
	strcpy(m_nowShowString,temp.m_nowShowString);

	m_nowInsertPoint = temp.m_nowInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	m_nowStringLength = temp.m_nowStringLength;	//��ǰ�����ַ����ĳ���
	m_haveSelect = temp.m_haveSelect;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = temp.m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
	m_selectStart = temp.m_selectStart;		//ѡ�����ʼλ��
	m_selectEnd = temp.m_selectEnd;		//ѡ��Ľ���λ��

	m_editNormal = temp.m_editNormal;		//�������ͨ״̬
	m_editDisable = temp.m_editDisable;		//�������Ч״̬
	m_editSelect = temp.m_editSelect;		//�����Ƭѡ��ɫ
	m_editInsert = temp.m_editInsert;		//����������
	m_editUpon = temp.m_editUpon;		//����������

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//������ʾ�����ı�����ͼ
	m_spriteSelect.setACopy(temp.m_spriteSelect);		//������ʾѡ�����ݵı�����ɫ
	m_spriteInsert.setACopy(temp.m_spriteInsert);		//������ʾ�������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_spriteSelect);
	_XObjManger.decreaseAObject(&m_spriteInsert);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_funInputChenge = temp.m_funInputChenge;		//�������ݷ����ı��ʱ�����
	m_funInputOver = temp.m_funInputOver;		//ȷ���������֮���ʱ�����

	m_timer = temp.m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	m_keyShiftState = temp.m_keyShiftState;		//shift������״̬	0����1����
	m_keyCapsLockState = temp.m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	m_keyNumLockState = temp.m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	m_keyCtrlState = temp.m_keyCtrlState;		//Ctrl������״̬ 0����1����

	m_canShowLength = temp.m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	m_nowShowStart = temp.m_nowShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��
	m_pClass = temp.m_pClass;

	m_textColor = temp.m_textColor;
	m_nowTextWidth = temp.m_nowTextWidth;
	m_nowTextHeight = temp.m_nowTextHeight;

	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	m_isPassword = temp.m_isPassword;

	return XTrue;
}
void _XEdit::insertChar(const char * ch,int len)
{
	if(ch == NULL ||
		len <= 0 ||
		m_nowStringLength + len >= MAX_INPUT_STRING_LENGTH - 1) return;
	if(m_haveSelect)
	{//����Ƭѡ״̬
		if(!m_selectMouseDown)
		{//Ƭѡ�Ѿ�����
			//1��ɾ��Ƭѡ����
			deleteSelectStr();
			m_haveSelect = XFalse;	//Ƭѡ����

			if(m_nowInsertPoint == m_nowStringLength)
			{//ĩβ
				memcpy(m_nowString + m_nowInsertPoint,ch,len);
			//	++ m_nowInsertPoint;
				m_nowStringLength += len;
				m_nowString[m_nowStringLength] = '\0';
				changeInsertPoint(len);
			}else
			{//�м����
				//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
				strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
				memcpy(m_nowString + m_nowInsertPoint,ch,len);
				strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
			//	++ m_nowInsertPoint;
				m_nowStringLength += len;
				changeInsertPoint(len);
			}
		}else
		{//Ƭѡ��δ����
			//����������
			if(m_selectEnd != m_selectStart)
			{//Ƭѡ��Ч
				//���밴��
				//1��ɾ��Ƭѡ����
				deleteSelectStr();
				m_haveSelect = XFalse;	//Ƭѡ����
				if(m_nowInsertPoint == m_nowStringLength)
				{//ĩβ
					memcpy(m_nowString + m_nowInsertPoint,ch,len);
					//++ m_nowInsertPoint;
					m_nowStringLength += len;
					m_nowString[m_nowStringLength] = '\0';
					changeInsertPoint(len);
				}else
				{//�м����
					//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
					strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
					memcpy(m_nowString + m_nowInsertPoint,ch,len);
					strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
					//++ m_nowInsertPoint;
					m_nowStringLength += len;
					changeInsertPoint(len);
				}
			}
			//Ȼ���������¿�ʼ����
			m_haveSelect = XFalse;
			m_selectEnd = m_nowInsertPoint;
			m_selectStart = m_nowInsertPoint;
			m_selectMouseDown = XTrue;
		}
	}else
	{//������Ƭѡ
		if(m_nowInsertPoint == m_nowStringLength)
		{//ĩβ
			memcpy(m_nowString + m_nowInsertPoint,ch,len);
			//++ m_nowInsertPoint;
			m_nowStringLength += len;
			m_nowString[m_nowStringLength] = '\0';
			changeInsertPoint(len);
		}else
		{//�м����
			//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
			strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
			//m_nowString[m_nowInsertPoint] = tempChar;
			memcpy(m_nowString + m_nowInsertPoint,ch,len);
			strcpy(m_nowString + m_nowInsertPoint + len,m_tempNowString);
			//++ m_nowInsertPoint;
			m_nowStringLength += len;
			changeInsertPoint(len);
		}
		if(m_selectMouseDown)
		{//���������Ƭѡ��꣬����Ҫ������
			m_selectEnd = m_nowInsertPoint;
			m_selectStart = m_nowInsertPoint;
		}
	}

	//����ַ��������ı䣬��Ҫ�����ַ���������
	upDataShowStr();	//�����ַ�������ʾ
	if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	//�����ݷ����仯��ʱ�򲥷Ŷ���
	if(m_withAction)
	{//�������һ����̬Ч��
	//	m_isInAction = XTrue;
		m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
		m_oldPos = m_position;
		m_oldSize = m_size;
	}

	return;
}
#ifdef XEE_OS_WINDOWS	//windowsϵͳ��ͨ�����뷨�������ַ�
int _XEdit::keyJudgement(int)
#else
int _XEdit::keyJudgement(int keyOrder)
#endif
{
#ifdef XEE_OS_WINDOWS	//windowsϵͳ��ͨ�����뷨�������ַ�
	return 0;
#endif
#ifdef XEE_OS_LINUX
	int TextChange = 0;
	char tempChar = 0;
//	printf("%d\n",keyOrder);
	if(keyOrder == XKEY_SPACE)
	{
		tempChar = ' ';
		TextChange = 1;
	}else
	if(m_keyShiftState == 0)
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = '0';break;
		case XKEY_1:tempChar = '1';break;
		case XKEY_2:tempChar = '2';break;
		case XKEY_3:tempChar = '3';break;
		case XKEY_4:tempChar = '4';break;
		case XKEY_5:tempChar = '5';break;
		case XKEY_6:tempChar = '6';break;
		case XKEY_7:tempChar = '7';break;
		case XKEY_8:tempChar = '8';break;
		case XKEY_9:tempChar = '9';break;
		//���Ű���
		case XKEY_BACKQUOTE:tempChar = '`';break;
		case XKEY_MINUS:tempChar = '-';break;
		case XKEY_EQUALS:tempChar = '=';break;
		case XKEY_LEFTBRACKET:tempChar = '[';break;
		case XKEY_RIGHTBRACKET:tempChar = ']';break;
		case XKEY_SEMICOLON:tempChar = ';';break;
		case XKEY_QUOTE:tempChar = '\'';break;
		case XKEY_BACKSLASH:tempChar = '\\';break;
		case XKEY_COMMA:tempChar = ',';break;
		case XKEY_PERIOD:tempChar = '.';break;
		case XKEY_SLASH:tempChar = '/';break;
		default:TextChange = 0;break;
		}
	}else
	{
		TextChange = 1;
		switch(keyOrder)
		{
		case XKEY_0:tempChar = ')';break;
		case XKEY_1:tempChar = '!';break;
		case XKEY_2:tempChar = '@';break;
		case XKEY_3:tempChar = '#';break;
		case XKEY_4:tempChar = '$';break;
		case XKEY_5:tempChar = '%';break;
		case XKEY_6:tempChar = '^';break;
		case XKEY_7:tempChar = '&';break;
		case XKEY_8:tempChar = '*';break;
		case XKEY_9:tempChar = '(';break;
			//���Ű���
		case XKEY_BACKQUOTE:tempChar = '~';break;
		case XKEY_MINUS:tempChar = '_';break;
		case XKEY_EQUALS:tempChar = '+';break;
		case XKEY_LEFTBRACKET:tempChar = '{';break;
		case XKEY_RIGHTBRACKET:tempChar = '}';break;
		case XKEY_SEMICOLON:tempChar = ':';break;
		case XKEY_QUOTE:tempChar = '\"';break;
		case XKEY_BACKSLASH:tempChar = '|';break;
		case XKEY_COMMA:tempChar = '<';break;
		case XKEY_PERIOD:tempChar = '>';break;
		case XKEY_SLASH:tempChar = '?';break;
		default:TextChange = 0;break;
		}
	}
	//��ĸ��
	if(TextChange == 0)
	{
	//	if((m_keyShiftState == 0 && m_keyCapsLockState == 0) || (m_keyShiftState == 1 || m_keyCapsLockState == 1))
		if(m_keyShiftState == m_keyCapsLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'a';break;
			case XKEY_b:tempChar = 'b';break;
			case XKEY_c:tempChar = 'c';break;
			case XKEY_d:tempChar = 'd';break;
			case XKEY_e:tempChar = 'e';break;
			case XKEY_f:tempChar = 'f';break;
			case XKEY_g:tempChar = 'g';break;
			case XKEY_h:tempChar = 'h';break;
			case XKEY_i:tempChar = 'i';break;
			case XKEY_j:tempChar = 'j';break;
			case XKEY_k:tempChar = 'k';break;
			case XKEY_l:tempChar = 'l';break;
			case XKEY_m:tempChar = 'm';break;
			case XKEY_n:tempChar = 'n';break;
			case XKEY_o:tempChar = 'o';break;
			case XKEY_p:tempChar = 'p';break;
			case XKEY_q:tempChar = 'q';break;
			case XKEY_r:tempChar = 'r';break;
			case XKEY_s:tempChar = 's';break;
			case XKEY_t:tempChar = 't';break;
			case XKEY_u:tempChar = 'u';break;
			case XKEY_v:tempChar = 'v';break;
			case XKEY_w:tempChar = 'w';break;
			case XKEY_x:tempChar = 'x';break;
			case XKEY_y:tempChar = 'y';break;
			case XKEY_z:tempChar = 'z';break;
			default:TextChange = 0;break;
			}
		}else
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_a:tempChar = 'A';break;
			case XKEY_b:tempChar = 'B';break;
			case XKEY_c:tempChar = 'C';break;
			case XKEY_d:tempChar = 'D';break;
			case XKEY_e:tempChar = 'E';break;
			case XKEY_f:tempChar = 'F';break;
			case XKEY_g:tempChar = 'G';break;
			case XKEY_h:tempChar = 'H';break;
			case XKEY_i:tempChar = 'I';break;
			case XKEY_j:tempChar = 'J';break;
			case XKEY_k:tempChar = 'K';break;
			case XKEY_l:tempChar = 'L';break;
			case XKEY_m:tempChar = 'M';break;
			case XKEY_n:tempChar = 'N';break;
			case XKEY_o:tempChar = 'O';break;
			case XKEY_p:tempChar = 'P';break;
			case XKEY_q:tempChar = 'Q';break;
			case XKEY_r:tempChar = 'R';break;
			case XKEY_s:tempChar = 'S';break;
			case XKEY_t:tempChar = 'T';break;
			case XKEY_u:tempChar = 'U';break;
			case XKEY_v:tempChar = 'V';break;
			case XKEY_w:tempChar = 'W';break;
			case XKEY_x:tempChar = 'X';break;
			case XKEY_y:tempChar = 'Y';break;
			case XKEY_z:tempChar = 'Z';break;
			default:TextChange = 0;break;
			}
		}
	}
	//С����
	if(TextChange == 0)
	{
		if(m_keyShiftState != m_keyNumLockState)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP0:tempChar = '0';break;
			case XKEY_KP1:tempChar = '1';break;
			case XKEY_KP2:tempChar = '2';break;
			case XKEY_KP3:tempChar = '3';break;
			case XKEY_KP4:tempChar = '4';break;
			case XKEY_KP5:tempChar = '5';break;
			case XKEY_KP6:tempChar = '6';break;
			case XKEY_KP7:tempChar = '7';break;
			case XKEY_KP8:tempChar = '8';break;
			case XKEY_KP9:tempChar = '9';break;
			case XKEY_KP_PERIOD:tempChar = '.';break;
			default:TextChange = 0;break;
			}
		}
		if(TextChange == 0)
		{
			TextChange = 1;
			switch(keyOrder)
			{
			case XKEY_KP_DIVIDE:tempChar = '/';break;
			case XKEY_KP_MULTIPLY:tempChar = '*';break;
			case XKEY_KP_MINUS:tempChar = '-';break;
			case XKEY_KP_PLUS:tempChar = '+';break;
			default:TextChange = 0;break;
			}
		}
	}
	if(TextChange != 0) insertChar(&tempChar,1);
	return TextChange;
#endif
}
void _XEdit::setInputLen(int len)
{//���������ĳ���
	if(len <= 0) return;
	m_mouseRect.right = m_mouseRect.left + len;
	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
	m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	//������Ҫ�ı伸��ֵ
	if(m_nowInsertPoint < m_nowShowStart) m_nowShowStart = m_nowInsertPoint;
	if(m_nowInsertPoint - m_nowShowStart > m_canShowLength) m_nowShowStart = m_nowInsertPoint - m_canShowLength;
	upDataShowStr();
	upDateInsertShowPosition();
}
