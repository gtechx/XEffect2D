#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEdit.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "../XImm.h"
#include "XResourcePack.h"

namespace XE{
char XEdit::m_copyString[m_maxInputStringLength] = "";
XEditSkin::XEditSkin()
:m_isInited(XFalse)
,editNormal(NULL)			//�������ͨ״̬
,editDisable(NULL)			//�������Ч״̬
,editSelect(NULL)			//�����Ƭѡ��ɫ
,editInsert(NULL)			//����������
,editUpon(NULL)
{}
XBool XEditSkin::init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon,XResourcePosition resoursePosition)
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
#define EDIT_CONFIG_FILENAME "Edit.txt"
bool XEditSkin::loadFromFolder(const char *filename,XResourcePosition resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
	if((editNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editInsert = createATextureData(tempFilename,resPos)) == NULL)
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
		if((editUpon = createATextureData(tempFilename,resPos)) == NULL)
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
	return true;
}
bool XEditSkin::loadFromPacker(const char *filename,XResourcePosition resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
	if((editNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	if((editInsert = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//upon
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag != 0)
	{
		if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
		offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((editUpon = createATextureData(tempFilename,resPos)) == NULL)
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
	//�������ݶ�ȡ���
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XEditSkin::loadFromWeb(const char *filename,XResourcePosition resPos)		//����ҳ�ж�ȡ��Դ
{
	return true;
}
XBool XEditSkin::initEx(const char *filename,XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,EDIT_CONFIG_FILENAME);
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
XEdit::XEdit()
	:m_isInited(XFalse)					//�Ƿ��ʼ��
	,m_mouseRightButtonMenu(NULL)	//����Ҽ��˵�
	,m_curString(NULL)				//��ǰ������ַ���
	,m_tempCurString(NULL)
	//,m_insertString(NULL)			//������ַ���
	//,m_selectString(NULL)			//ѡȡ���ַ���
	,m_haveSelect(XFalse)
	,m_editNormal(NULL)				//�������ͨ״̬
	,m_editDisable(NULL)			//�������Ч״̬
	,m_editSelect(NULL)				//�����Ƭѡ��ɫ
	,m_editInsert(NULL)				//����������
	//,m_funInputChenge(NULL)			//�������ݷ����ı��ʱ�����
	//,m_funInputOver(NULL)			//ȷ���������֮���ʱ�����
	,m_resInfo(NULL)
	,m_withoutTex(XFalse)
	,m_curKeyDown(XFalse)
	,m_isPassword(false)
	,m_withPromptStr(false)
	,m_editType(TYPE_STRING)
{//�����ʼ������������״̬
	if(XEE::getCapsLockState()) m_keyCapsLockState = 1;
	else m_keyCapsLockState = 0;
	if(XEE::getNumLockState()) m_keyNumLockState = 1;
	else m_keyNumLockState = 0;
	m_ctrlType = CTRL_OBJ_EDIT;
}
XBool XEdit::init(const XVector2& position,		//�ؼ���λ��
		const XRect& Area,					//�ؼ��������Ӧ����
		const XEditSkin &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		XMouseRightButtonMenu * mouseMenu)		//�ؼ����Ҽ��˵�
{
	if(m_isInited ||
		tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= m_maxInputStringLength) return XFalse;
	if(mouseMenu != NULL)
	{//������Ҫ�Ľ�������ʹ��ָ�룬���ܵ����޸ĵ�Ӱ��
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//�ӿؼ��������н�����ؼ�ע����
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
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
	m_scale.set(1.0f,1.0f);
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curString == NULL) 
	{
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curString[0] = '\0';
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempCurString[0] = '\0';
//	m_insertString = XMem::createArrayMem<char>(m_maxInputStringLength);	//��ǰ������ַ���
//	if(m_insertString == NULL) return XFalse;
//	m_insertString[0] = '\0';
//	m_selectString = XMem::createArrayMem<char>(m_maxInputStringLength);	//��ǰ������ַ���
//	if(m_selectString == NULL) return XFalse;
//	m_selectString[0] = '\0';
	m_curShowString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_tempCurString);
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curShowString[0] = '\0';

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_curString,str);
		m_curInsertPoint = strlen(m_curString);	//��ǰѡ�������λ��
		m_curStringLength = strlen(m_curString);	//��ǰ�����ַ����ĳ���
	}else
	{
		m_caption.setString(" ");
		m_curString[0] = '\0';
		m_curInsertPoint = 0;	//��ǰѡ�������λ��
		m_curStringLength = 0;	//��ǰ�����ַ����ĳ���
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
//	m_selectText.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
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

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
XBool XEdit::initWithoutSkin(const XRect& area,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		XMouseRightButtonMenu * mouseMenu)
{
	if(m_isInited ||
	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
		strSize <= 0 ||		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
		(str != NULL && strlen(str) >= m_maxInputStringLength)) return XFalse;
	if(mouseMenu != NULL)
	{//������Ҫ�Ľ�������ʹ��ָ�룬���ܵ����޸ĵ�Ӱ��
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;	
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//�ӿؼ��������н�����ؼ�ע����
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//���濪ʼ��ֵ
	m_mouseRect = area;	//��¼������Ӧ��Χ
	m_position.set(0.0f,0.0f);
	m_withoutTex = XTrue;
	m_comment.init();

	m_scale.set(1.0f,1.0f);
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curString == NULL) 
	{
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curString[0] = '\0';
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_tempCurString[0] = '\0';
	m_curShowString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_tempCurString);
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE(m_mouseRightButtonMenu);
		return XFalse;
	}		
	m_curShowString[0] = '\0';

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_curString,str);
		m_curInsertPoint = strlen(m_curString);	//��ǰѡ�������λ��
		m_curStringLength = strlen(m_curString);	//��ǰ�����ַ����ĳ���
	}else
	{
		m_caption.setString(" ");
		m_curString[0] = '\0';
		m_curInsertPoint = 0;	//��ǰѡ�������λ��
		m_curStringLength = 0;	//��ǰ�����ַ����ĳ���
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��

	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
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

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void XEdit::upDataShowStr()
{//��û��ʵ��
	if(m_curStringLength <= m_curShowStart + m_canShowLength) 
	{
		m_curShowStart = m_curStringLength - m_canShowLength;
		if(m_curShowStart < 0) m_curShowStart = 0;
		if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //���ͷ�ٽ硢����ƶ�һλ
		strcpy(m_curShowString,m_curString + m_curShowStart);
	}else
	{
		if(XString::isAtUnicodeEnd(m_curString,m_curShowStart + m_canShowLength)) 
		{
			memcpy(m_curShowString,m_curString + m_curShowStart,m_canShowLength);
			m_curShowString[m_canShowLength] = '\0';
		}else 
		{
			memcpy(m_curShowString,m_curString + m_curShowStart,m_canShowLength - 1);
			m_curShowString[m_canShowLength - 1] = '\0';
		}
	}
	//���µ���ʾ��
	if(m_withPromptStr && strlen(m_curString) <= 0)
	{//�������ʾ����
		m_caption.setColor(XFColor(0.65f,0.65f,0.65f,1.0f) * m_color);
		m_caption.setString(XString::getCanShowString(m_promptStr.c_str(),m_canShowLength).c_str());
	}else
	{//���û����ʾ����
		m_caption.setColor(m_textColor * m_color);
		m_caption.setString(m_curShowString);
	}
	if(!isStringPassable())
		m_caption.setColor(XFColor::red * m_color);
}
void XEdit::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::downColor * m_color,true,false,true);
		}else 
		{
			XRender::drawFillBoxExA(m_position + XVector2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y),
				XVector2(m_mouseRect.getWidth() * m_scale.x,m_mouseRect.getHeight() * m_scale.y),XCCS::normalColor * m_color,true,false,true);
		}

		if(m_haveSelect) 
			XRender::drawFillBoxExA(XVector2(m_selectRect.left,m_selectRect.top),XVector2(m_selectRect.right,m_selectRect.bottom),XCCS::specialColor * m_color,true,false,true);

		//��ʾ��ǰ������ַ���
		m_caption.draw();
		if(m_insertActionMD.getIsEnd())
		{
			if(m_isEnable && m_isBeChoose)
			{
				if(m_timer > 250) 
				{
					float x = m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth;
					float y = m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f;
					XRender::drawLine(x,y,x,y + m_curTextHeight,1,XCCS::blackDownColor * m_color);
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
void XEdit::setImmPos()
{
	float x = m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth;
	float y = m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f;
	XImm::setCompositionPos(x,y);
}
void XEdit::release()
{
	if(!m_isInited) return;
	XMem::XDELETE_ARRAY(m_curString);
	XMem::XDELETE_ARRAY(m_tempCurString);
	//XMem::XDELETE_ARRAY(m_insertString);
	//XMem::XDELETE_ARRAY(m_selectString);
	XMem::XDELETE_ARRAY(m_curShowString);
	XMem::XDELETE(m_mouseRightButtonMenu);
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
void XEdit::setString(const char *str)
{//����������ٺܸ��ӵ��߼�����������û����ڴ�������׶λ������ڴ���Ƭѡ�׶ε�ʱ������޸Ľ�����ɽ�Ϊ���Ӱ�죬����������ܻ���һЩ�����Ի��Ĵ���
	if(!m_isInited ||
		str == NULL) return;
	if(!isStringPassable()) return;	//������ݲ�����Ҫ����ֱ�ӷ���
	m_caption.setString(str);
	strcpy(m_curString,str);
	m_curInsertPoint = strlen(m_curString);	//��ǰѡ�������λ��
	m_curStringLength = strlen(m_curString);	//��ǰ�����ַ����ĳ���
	//ȡ������Ƭѡ����Ϣ
	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = XFalse;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��
	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	m_curShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	upDateInsertShowPosition();
}
bool XEdit::stringPlaster()
{
#ifdef XEE_OS_WINDOWS
	std::string tmp = XString::getTextFromClipboard();
	if(tmp == "") m_copyString[0] = '\0';	//û����
	else 
	{
		if(tmp.size() >= m_maxInputStringLength - 1)
		{
			memcpy(m_copyString,tmp.c_str(),m_maxInputStringLength - 1);
			m_copyString[m_maxInputStringLength - 1] = '\0';
		}else
		{
			memcpy(m_copyString,tmp.c_str(),tmp.size());
			m_copyString[tmp.size()] = '\0';
		}
	}
#endif
	if(m_copyString[0] == '\0' ||
		!getCopyStrHaveEnd()) return false;
	if(m_haveSelect)
	{//����Ƭѡ(����ʵ�����滻����)
		if(m_curStringLength + strlen(m_copyString) - getSelectLength() > m_maxInputStringLength - 2)
		{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
		}else
		{							
			//1��ɾ��Ƭѡ����
			deleteSelectStr();
			m_haveSelect = XFalse;	//Ƭѡ����
			//2�����븴������
			if(m_curInsertPoint == m_curStringLength)
			{//��������ַ�����ĩβ���
				strcat(m_curString,m_copyString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint = m_curStringLength;
				changeInsertPoint(m_curStringLength - m_curInsertPoint);
				return true;
			}else
			{
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				strcpy(m_curString + m_curInsertPoint,m_copyString);
				strcat(m_curString,m_tempCurString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint += strlen(m_copyString);
				changeInsertPoint(strlen(m_copyString));
				return true;
			}
		}
	}else
	{//û��Ƭѡ
		//�ж�ճ���������Ƿ�ᳬ���ַ������ܳ���
		if(m_curStringLength + strlen(m_copyString) > m_maxInputStringLength - 2)
		{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
		}else
		{
			if(m_curInsertPoint == m_curStringLength)
			{//��������ַ�����ĩβ���
				strcat(m_curString,m_copyString);
				m_curStringLength = strlen(m_curString);
			//	m_curInsertPoint = m_curStringLength;
				changeInsertPoint(m_curStringLength - m_curInsertPoint);
				return true;
			}else
			{
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				strcpy(m_curString + m_curInsertPoint,m_copyString);
				strcat(m_curString,m_tempCurString);
				m_curStringLength = strlen(m_curString);
				//m_curInsertPoint += strlen(m_copyString);
				changeInsertPoint(strlen(m_copyString));
				return true;
			}
		}
	}
	return false;
}
void XEdit::mouseRightMenuProc()
{
	int TextChange = 0;
	switch(m_mouseRightButtonMenu->getLastChoose())
	{
	case 0://����
		if(!m_haveSelect) break;//����Ƭѡ��û��Ƭѡ��ʲôҲ����
		//1������Ƭѡ������
#ifdef XEE_OS_WINDOWS
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
		XString::setTextToClipboard(m_copyString);
#endif
#ifdef XEE_OS_LINUX
		memcpy(m_copyString,m_curString + getSelectHead(),getSelectLength());
		m_copyString[getSelectLength()] = '\0';
#endif
		//2��ɾ��Ƭѡ������
		deleteSelectStr();
		m_haveSelect = XFalse;	//Ƭѡ����
		TextChange = 1;
		break;
	case 1://����
		stringCopy();
		break;
	case 2://ճ��
		if(stringPlaster()) TextChange = 1;
		break;
	case 3://����(�������Ƭѡ��ȡ��Ƭѡ)
		if(m_haveSelect) m_haveSelect = XFalse;
		break;
	}
//	printf("%d\n",m_mouseRightButtonMenu->getLastChoose());
	if(TextChange == 0) return;	//���û�з����仯����ֱ���˳�
	upDataShowStr();	//��Ϊ�ַ������ı䣬����������Ҫ�����ַ�����ʾ
//	m_caption.setString(m_curString);
//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x + m_curInsertPoint * m_caption.m_scale.x * m_caption.m_showSize.x * 0.5,
//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
	//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
	//�����ݷ����仯��ʱ�򲥷Ŷ���
	if(m_withAction)
	{//�������һ����̬Ч��
	//	m_isInAction = XTrue;
		m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
		m_oldPos = m_position;
		m_oldSize = m_scale;
	}
}
void XEdit::changeInsertPoint(int sum)
{
	if(sum == 0) return;
	int tempNewPosition = m_curInsertPoint + sum;
	if(tempNewPosition < 0) return;
	if(tempNewPosition > m_curStringLength) return;
	//������Ҫ������ĵ�����
	if(!XString::isAtUnicodeEnd(m_curString,tempNewPosition)) 
	{//������������ٽ�Ͷ��ƶ�һλ,(�����������)
		if(sum < 0) changeInsertPoint(sum - 1);//����
		else changeInsertPoint(sum + 1);//ǰ��
		return;
	}
	//�����ƶ�
	int tempOldPosition = m_curInsertPoint;		//�ɹ���λ��
	m_curInsertPoint = tempNewPosition;
	//����Ƿ�Ҫ�ƶ���ʾ����
	if(m_curInsertPoint < m_curShowStart || m_curInsertPoint > m_curShowStart + m_canShowLength)
	{//������ʾ��Χ��Ҫ�޸���ʾ��Χ
		if(m_curInsertPoint < m_curShowStart)
		{//��ǰ�ƶ�
			if(tempOldPosition != m_curShowStart)
			{//���֮ǰ���û���������Ŀ�ͷ������Ҫ���µ���ͷ
				m_curShowStart = m_curInsertPoint;
				upDateInsertShowPosition();
			}else
			{
				m_curShowStart = m_curInsertPoint;
			}
		}else
		if(m_curInsertPoint > m_curShowStart + m_canShowLength)
		{//����ƶ�
			if(tempOldPosition != m_curShowStart + m_canShowLength)
			{//���֮ǰ�Ĺ�겻��������β����Ҫ���¹�굽��β
				m_curShowStart = m_curInsertPoint - m_canShowLength;
				if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //���ͷ�ٽ硢����ƶ�һλ
				upDateInsertShowPosition();
			}else
			{
				m_curShowStart = m_curInsertPoint - m_canShowLength;
				if(!XString::isAtUnicodeEnd(m_curString,m_curShowStart)) ++m_curShowStart; //���ͷ�ٽ硢����ƶ�һλ
				upDateInsertShowPosition();
			}
		}
		upDataShowStr();
	}else
	{//����Ҫ�޸���ʾ��Χ
		upDateInsertShowPosition();
	}
}
void XEdit::upDataSelectShow()
{
	int tempHead = getSelectHead();
	int tempEnd = getSelectEnd();
	if(tempHead < m_curShowStart) tempHead = m_curShowStart;
	if(tempHead >= m_curShowStart + m_canShowLength)
	{//����������Ҫ��ʾ
		tempHead = m_curShowStart + m_canShowLength;
	}
	if(tempEnd <= m_curShowStart)
	{//�������Ҳ����Ҫ��ʾ
		tempEnd = m_curShowStart;
	}
	if(tempEnd > m_curShowStart + m_canShowLength) tempEnd = m_curShowStart + m_canShowLength;
	//����ʵ�ʵ��������ѡ��λ�ú�ѡ��ͼƬ��Ҫ���õĳߴ�
	if(m_withoutTex)
	{
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
				m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f,(tempEnd - tempHead) * m_curTextWidth,m_curTextHeight);
		}else
		{
			m_selectRect.set(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
				m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f,m_canShowLength * m_curTextWidth,m_curTextHeight);
		}
	}else
	{
		m_spriteSelect.setPosition(m_position.x + m_textPosition.x * m_scale.x + (tempHead - m_curShowStart) * m_curTextWidth,
			m_position.y + m_textPosition.y * m_scale.y - m_curTextHeight * 0.5f);
		if(tempEnd - tempHead < m_canShowLength)
		{
			m_spriteSelect.setScale((tempEnd - tempHead) * m_curTextWidth /m_editSelect->textureSize.x,
				m_curTextHeight / m_editSelect->textureSize.y);
		}else
		{
			m_spriteSelect.setScale(m_canShowLength * m_curTextWidth /m_editSelect->textureSize.x,
				m_curTextHeight / m_editSelect->textureSize.y);
		}
	}
}
XBool XEdit::mouseProc(float x,float y,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������
	//���������Ӧ�����λ��
//	XRect temp;
//	temp.set(m_objRect.left,m_objRect.top,m_objRect.left + m_objRect.getWidth() * m_scale.x,m_objRect.top + m_objRect.getHeight() * m_scale.y);
	if(m_mouseRightButtonMenu != NULL)
	{//�Ҽ��˵��������������
		XBool flagMouseMenu = m_mouseRightButtonMenu->getVisible();
		bool ret = false;
		if(mouseState == MOUSE_RIGHT_BUTTON_UP)
		{//������Ҽ���������������Ҫ�ڿؼ���Χ��
			if(m_curMouseRect.isInRect(x,y) && m_isBeChoose)
				ret = m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}else
		{
			ret = m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}
		if(!flagMouseMenu && m_mouseRightButtonMenu->getVisible())
		{//�Ҽ��˵����֣�������Ҫ�����Ҽ��˵���״̬
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,0); 
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,1); 
#ifdef XEE_OS_WINDOWS	//��ѡ�����ճ��
			m_mouseRightButtonMenu->setMnuState(XString::getTextFromClipboard() != "",2); 
#else
			m_mouseRightButtonMenu->setMnuState(m_copyString[0] != '\0',2); 
#endif
			m_mouseRightButtonMenu->setMnuState(m_haveSelect,3); 
		}
		//if(flagMouseMenu && !m_mouseRightButtonMenu->getVisible())
		if(ret)
		{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
			mouseRightMenuProc();//�Ҽ��˵�����Ӧ
			if(m_mouseRightButtonMenu->getLastChoose() != -1)//����˵��в��������������Ϣ����
				return XTrue;	//���Ҽ��˵��Ĳ�����Ч�Ļ�������������Ϣ�������ظ�����
		}
	} 
	if(m_curMouseRect.isInRect(x,y))
	{//����ڷ�Χ�ڵ��
		if(!m_isMouseInRect)
		{
			m_isMouseInRect = XTrue;
			m_comment.setShow();
			setCommentPos(x,y + 16.0f);
		}
		if(mouseState != MOUSE_MOVE && m_comment.getIsShow())
			m_comment.disShow();	//�����������������˵������ʧ
		//���������Ҽ������Գ���
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN)
		{//���	
			if(!m_isBeChoose)
			{
				m_isBeChoose = XTrue;//����ؼ�û�д��ڼ���״̬�򼤻�
				if(m_withAction)
					m_insertActionMD.set(10.0f,1.0f,0.005f);//,MOVE_DATA_MODE_SQRT2_MULT);
			}
			if(m_curStringLength > 0 &&//�ַ��������ַ�ʱ������ͬͨ�����ѡ�����λ��(����ĳ���10��Ϊ�˷���ѡ��ͷ����β)
				m_curMouseRect.isInRect(x,y))
			{//��갴�¶�����Ч
				//�������λ��
				if(x <= m_position.x + m_textPosition.x * m_scale.x)
				{//ͷ
					if(m_curInsertPoint != m_curShowStart) changeInsertPoint(m_curShowStart - m_curInsertPoint);
				}else
				if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x)
				{//β
					if(m_curStringLength <= m_canShowLength)
					{
						if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
					}else
					{//����ʾ�Ľ���
						if(m_curInsertPoint != m_curShowStart + m_canShowLength) changeInsertPoint(m_curShowStart + m_canShowLength - m_curInsertPoint);
					}
				}else
				{//��Χ��
					//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
					//Ϊ�����ѡ��ʱ�ĸо��������λ����ַ��Ŀ�ȣ�ʹѡ���ʱ������ио�
					int tempInsertPoint = (x + m_curTextWidth * 0.5f - m_position.x - m_textPosition.x * m_scale.x) / m_curTextWidth;
					if(tempInsertPoint + m_curShowStart > m_curStringLength)
					{
						if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
					}else
					{
						if(m_curInsertPoint != tempInsertPoint + m_curShowStart) changeInsertPoint(tempInsertPoint + m_curShowStart - m_curInsertPoint);
					}
				}
				switch(mouseState)
				{
				case MOUSE_LEFT_BUTTON_DOWN://�����������£��������Ƭѡ����
					m_haveSelect = XFalse;
					m_selectEnd = m_curInsertPoint;
					m_selectStart = m_curInsertPoint;
					m_selectMouseDown = XTrue;
					break;
				case MOUSE_LEFT_BUTTON_DCLICK:	//��������Ϊ˫��ȫѡ
					chooseAllStr();
					break;
				case MOUSE_RIGHT_BUTTON_DOWN://������Ҽ����²������ٷ�Χ��Ҳ�������Ƭѡ����
					if(m_curInsertPoint > getSelectEnd() || m_curInsertPoint < getSelectHead())
						m_haveSelect = XFalse;
					break;
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
		if((mouseState == MOUSE_LEFT_BUTTON_DOWN || 
			mouseState == MOUSE_LEFT_BUTTON_DCLICK || 
			mouseState == MOUSE_RIGHT_BUTTON_DOWN) &&//���
			m_isBeChoose)
		{
			//setLostFocus();	//����ʧȥ����
			m_isBeChoose = XFalse;//����ؼ�û�д��ڼ���״̬�򼤻�
			if(m_withAction)
				m_insertActionMD.set(1.0f,0.0f,0.002f);
		}
	}
	//˫��Ϊȫѡ
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN)// || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//�κ����������²����������Ƭѡ����
		if(m_haveSelect) m_haveSelect = XFalse;
	}

	if(mouseState == MOUSE_MOVE &&
		m_selectMouseDown)
	{//�����갴�£������϶�������ΪƬѡ��������
		//����Ƭѡ��λ��
		if(x <= m_position.x + m_textPosition.x * m_scale.x)
		{//ͷ
			if(m_curShowStart > 0 && x <= m_position.x + m_textPosition.x * m_scale.x - m_textEdgeWidth) 
			{//�������ͷ��������ǰ�ƶ�
				m_selectEnd = m_curShowStart - 1;
				if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd)) --m_selectEnd;
			}else 
			{
				m_selectEnd = m_curShowStart;
			}
		}else
		if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x)
		{//β
			if(m_curShowStart + m_canShowLength >= m_curStringLength)
			{//����ַ���û����ʾ��
				m_selectEnd = m_curStringLength;
			}else
			{
				if(x >= m_position.x + (m_mouseRect.right - m_textEdgeWidth) * m_scale.x + m_textEdgeWidth)
				{//�������β��������ƶ�
					m_selectEnd = m_curShowStart + m_canShowLength + 1;
					if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd)) ++m_selectEnd;
				}else
				{
					m_selectEnd = m_curShowStart + m_canShowLength;
				}
			}
		}else
		{//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
			int tempEnd = m_selectEnd;
			int tempPosition = (x - m_position.x - m_textPosition.x * m_scale.x) / m_curTextWidth;
			if(tempPosition + m_curShowStart >= m_curStringLength) m_selectEnd = m_curStringLength;
			else m_selectEnd = m_curShowStart + tempPosition;

			if(!XString::isAtUnicodeEnd(m_curString,m_selectEnd))
			{
				if(m_selectEnd > tempEnd) ++m_selectEnd;
				else --m_selectEnd;
			}
		}
		if(m_selectEnd != m_curInsertPoint) changeInsertPoint(m_selectEnd - m_curInsertPoint);	//�������ҲҪ����仯
		if(m_selectEnd != m_selectStart)
		{//Ƭѡ��Ч
			m_haveSelect = XTrue;
			upDataSelectShow();
		}else m_haveSelect = XFalse;	//Ƭѡ��Ч
	}
	if(m_isBeChoose && (mouseState == MOUSE_LEFT_BUTTON_UP || mouseState == MOUSE_RIGHT_BUTTON_UP) &&//����ǵ����¼��������Ƿ��ڷ�Χ�ڣ�ֻҪ�ؼ����ڼ���״̬�����������Ӧ
		m_selectMouseDown)//Ƭѡ�����ж�
	{
		m_haveSelect = (m_selectEnd != m_selectStart);
		//if(m_selectEnd != m_selectStart) m_haveSelect = XTrue;	//Ƭѡ��Ч
		//else m_haveSelect = XFalse;	//Ƭѡ��Ч
		m_selectMouseDown = XFalse;
	}
	return XTrue;
}
XBool XEdit::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withAction && m_isInAction) return XFalse;	//���֧�ֶ������Ŷ������ڲ��Ŷ�����ô������������

	if(m_mouseRightButtonMenu != NULL &&
		m_mouseRightButtonMenu->getVisible())
	{//����Ҽ��˵��ɼ�,�����а������������������Ӱ��
		//m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState);
		//if(!m_mouseRightButtonMenu->getVisible())
		if(m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState))
		{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
			mouseRightMenuProc();//�Ҽ��˵�����Ӧ
		}
		return XFalse;
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
			m_curKeyDown = XTrue;
			m_curKeyDownTimer = 0;
			m_curKeyRepTimer = 1000;
			m_curKey = keyOrder;
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
				m_mouseRightButtonMenu->mouseProc(m_position.x + m_textPosition.x * m_scale.x + (m_curInsertPoint - m_curShowStart) * m_curTextWidth,
					m_position.y + m_textPosition.y * m_scale.y,MOUSE_RIGHT_BUTTON_UP);
			break;
		case XKEY_UNKNOWN:
			break;
		default:
			m_curKeyDown = XFalse;
			break;
		}
		break;
	}

	return XFalse;
}
XBool XEdit::keyProc(int keyOrder)
{
	int TextChange = 0;		//�ı������Ƿ����仯
	if(m_keyCtrlState == 0)
	{//ctrlû�а��µ�����������ܴ���
		if(m_haveSelect == 0)
		{//û��Ƭѡ��ʱ��
			if(m_curStringLength < m_maxInputStringLength - 2)
			{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
				//�����ǿɴ�ӡ�ַ��Ĵ���			
				TextChange = keyJudgement(keyOrder);
			}
		}else
		{//����Ƭѡ��ʱ��
			if(m_curStringLength - getSelectLength() < m_maxInputStringLength - 2)
			{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
				//�����ǿɴ�ӡ�ַ��Ĵ���			
				TextChange = keyJudgement(keyOrder);
			}
		}
	}else
	{//ctrl���µ�����������Ϊ���ܼ�����
		switch(keyOrder)
		{
		case XKEY_A://ctrl + a ȫѡ	//20150625�¼���
			chooseAllStr();
			break;
		case XKEY_C://ctrl + c ����
			stringCopy();
			break;
		case XKEY_V://ctrl + v ճ��  //������Ҫ��������������жϵ�ǰ�����Ƿ���Ч(��δ���)
			if(stringPlaster()) TextChange = 1;
			break;
		}
	}
	if(keyOrder ==  XKEY_BACKSPACE)
	{//���˼�
		if(!m_haveSelect)
		{//û��Ƭѡ��ʱ��
			if(m_curInsertPoint > 0)
			{
				if(!XString::isAtUnicodeEnd(m_curString,m_curInsertPoint - 1))
				{
					m_curStringLength -= 2;
					changeInsertPoint(-2);
					if(m_curStringLength == m_curInsertPoint) m_curString[m_curInsertPoint] = '\0';	//�ַ���ĩβ
					else strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 2);
				}else
				{
					-- m_curStringLength;
					changeInsertPoint(-1);
					if(m_curStringLength == m_curInsertPoint) m_curString[m_curInsertPoint] = '\0';	//�ַ���ĩβ
					else strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 1);
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
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				m_selectMouseDown = XTrue;
			}
		}
	}else
	if(keyOrder == XKEY_DELETE || (m_keyShiftState == m_keyNumLockState && keyOrder ==  XKEY_N_PERIOD))
	{//Delete����
		if(!m_haveSelect)
		{//û��Ƭѡ��ʱ��
			if(m_curStringLength == m_curInsertPoint)
			{//�ַ���ĩβ��������
			}else
			{
				//������Ҫ�жϺ����Ǹ��ַ��Ƿ�Ϊ���ģ����Ϊ���ĵĻ�������Ҫɾ�������ַ�
				if(m_curString[m_curInsertPoint] < 0)
				{
					strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 2);
					m_curStringLength -= 2;
				}else
				{
					strcpy(m_curString + m_curInsertPoint,m_curString + m_curInsertPoint + 1);
					-- m_curStringLength;
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
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
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
				//	m_curInsertPoint = m_selectStart;
				//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x + m_curInsertPoint * m_caption.m_scale.x * m_caption.m_showSize.x * 0.5,
				//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
					changeInsertPoint(m_selectStart - m_curInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//�������shift����ΪƬѡ����
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint > 0)
				{
					changeInsertPoint(-1);
					m_selectEnd = m_curInsertPoint;
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
					changeInsertPoint(m_selectEnd - m_curInsertPoint);
					m_haveSelect = XFalse;
				}
			}
		}else
		{//�������shift����ΪƬѡ����
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint < m_curStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint < m_curStringLength)
				{
					changeInsertPoint(1);
					m_selectEnd = m_curInsertPoint;
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
				if(m_curInsertPoint != 0) changeInsertPoint(-m_curInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//���shift������
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint > 0)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(-m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint > 0)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(-m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
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
				if(m_curInsertPoint != m_curStringLength) changeInsertPoint(m_curStringLength - m_curInsertPoint);
				if(m_haveSelect) m_haveSelect = XFalse;
			}
		}else
		{//���shift������
			if(!m_haveSelect)
			{//��ǿ�ʼƬѡ
				m_haveSelect = XFalse;
				m_selectEnd = m_curInsertPoint;
				m_selectStart = m_curInsertPoint;
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint < m_curStringLength)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(m_curStringLength - m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
				}
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = XTrue;
					upDataSelectShow();
				}else m_haveSelect = XFalse;	//Ƭѡ��Ч
			}else
			{//�޸�Ƭѡ���
				//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
				if(m_curInsertPoint < m_curStringLength)
				{//��ѡ���ƶ���ͷ
					changeInsertPoint(m_curStringLength - m_curInsertPoint);
					m_selectEnd = m_curInsertPoint;
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
		//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
		else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
		//�����ݷ����仯��ʱ�򲥷Ŷ���
		if(m_withAction)
		{//�������һ����̬Ч��
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_scale;
		}
		return XTrue;
	}
	if(keyOrder ==  XKEY_RETURN || keyOrder == XKEY_N_ENTER)
	{//�س���
		//if(m_haveSelect) m_haveSelect = XFalse;	//�س���ȡ��ѡ��
		//if(m_funInputOver != NULL) m_funInputOver(m_pClass,m_objectID);
		if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_OVER);
		else XCtrlManager.eventProc(m_objectID,EDT_INPUT_OVER);
		if(m_withAction)
		{//�������һ����̬Ч��
		//	m_isInAction = XTrue;
			m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
			m_oldPos = m_position;
			m_oldSize = m_scale;
		}
		return XTrue;
	}
	return XFalse;
}
void XEdit::setLostFocus() 
{
	if(m_haveSelect) m_haveSelect = XFalse;
	m_isBeChoose = XFalse;
	m_selectMouseDown = XFalse;
	m_curKeyDown = XFalse;
	//printf("ʧȥ����!\n");
	//���������ڵ�������ʱ�����bug�����ڽǵ��bug
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_OVER);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_OVER);
}
void XEdit::setScale(float x,float y)
{
	if(x <= 0 || y <= 0 ||
		!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_scale.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_scale.x,m_position.y + m_textPosition.y * m_scale.y);
	m_caption.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
		m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_scale.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_scale.y);
//	m_selectText.setScale(m_scale);
	//m_spriteBackGround.setPosition(m_position);
	if(!m_withoutTex) m_spriteBackGround.setScale(m_scale);
	upDateInsertShowPosition();
	//m_spriteSelect.setScale(m_scale);
	upDataSelectShow();
	if(!m_withoutTex) m_spriteInsert.setScale(m_textSize.x * m_scale.x,m_textSize.y * m_scale.y);

	updateChildScale();
}
XBool XEdit::setACopy(const XEdit &temp)			//����Ϊһ���������ò�����Դ		
{
	if(&temp == this ||	//��ֹ����ֵ
		!temp.m_isInited) return XFalse;
	if(m_isInited) release();//��������Ѿ���ʼ��������Ҫ�ͷ���Դ
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;	//�Ƿ��ʼ��
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	m_selectRect = temp.m_selectRect;

	if(temp.m_mouseRightButtonMenu != NULL)
	{
		m_mouseRightButtonMenu = XMem::createMem<XMouseRightButtonMenu>();
		if(m_mouseRightButtonMenu == NULL) return XFalse;

		m_mouseRightButtonMenu->setACopy(* (temp.m_mouseRightButtonMenu));	//����Ҽ��˵�
		//�ӿؼ��������н�����ؼ�ע����
		XCtrlManager.decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//��ʾ��ǰ������ַ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_curString = XMem::createArrayMem<char>(m_maxInputStringLength);
	strcpy(m_curString,temp.m_curString);
	m_tempCurString = XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_tempCurString == NULL) 
	{
		XMem::XDELETE_ARRAY(m_curString);
		return XFalse;
	}
	strcpy(m_tempCurString,temp.m_tempCurString);
	m_curShowString	= XMem::createArrayMem<char>(m_maxInputStringLength);
	if(m_curShowString == NULL)
	{
		XMem::XDELETE_ARRAY(m_curString);
		XMem::XDELETE_ARRAY(m_tempCurString);
		return XFalse;
	}
	strcpy(m_curShowString,temp.m_curShowString);

	m_curInsertPoint = temp.m_curInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	m_curStringLength = temp.m_curStringLength;	//��ǰ�����ַ����ĳ���
	m_haveSelect = temp.m_haveSelect;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = temp.m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
	m_selectStart = temp.m_selectStart;		//ѡ�����ʼλ��
	m_selectEnd = temp.m_selectEnd;		//ѡ��Ľ���λ��

	m_editNormal = temp.m_editNormal;		//�������ͨ״̬
	m_editDisable = temp.m_editDisable;		//�������Ч״̬
	m_editSelect = temp.m_editSelect;		//�����Ƭѡ��ɫ
	m_editInsert = temp.m_editInsert;		//����������
	m_editUpon = temp.m_editUpon;		//����������

	m_editType = temp.m_editType;		//����������

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//������ʾ�����ı�����ͼ
	m_spriteSelect.setACopy(temp.m_spriteSelect);		//������ʾѡ�����ݵı�����ɫ
	m_spriteInsert.setACopy(temp.m_spriteInsert);		//������ʾ�������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
	XObjManager.decreaseAObject(&m_spriteSelect);
	XObjManager.decreaseAObject(&m_spriteInsert);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	//m_funInputChenge = temp.m_funInputChenge;		//�������ݷ����ı��ʱ�����
	//m_funInputOver = temp.m_funInputOver;		//ȷ���������֮���ʱ�����
	//m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	m_keyShiftState = temp.m_keyShiftState;		//shift������״̬	0����1����
	m_keyCapsLockState = temp.m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	m_keyNumLockState = temp.m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	m_keyCtrlState = temp.m_keyCtrlState;		//Ctrl������״̬ 0����1����

	m_canShowLength = temp.m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	m_curShowStart = temp.m_curShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��

	m_textColor = temp.m_textColor;
	m_curTextWidth = temp.m_curTextWidth;
	m_curTextHeight = temp.m_curTextHeight;

	m_oldPos = temp.m_oldPos;				//��������ʱ��λ��
	m_oldSize = temp.m_oldSize;			//��������ʱ�Ĵ�С
	m_lightMD = temp.m_lightMD;
	m_lightRect = temp.m_lightRect;

	m_isPassword = temp.m_isPassword;

	return XTrue;
}
void XEdit::insertChar(const char * ch,int len)
{//������Ҫ��������������жϵ�ǰ�����Ƿ���Ч(��δ���)
	if(!m_isBeChoose) return;
	if(ch == NULL ||
		len <= 0 ||
		m_curStringLength + len >= m_maxInputStringLength - 1) return;
	if(m_haveSelect)
	{//����Ƭѡ״̬
		if(!m_selectMouseDown)
		{//Ƭѡ�Ѿ�����
			//1��ɾ��Ƭѡ����
			deleteSelectStr();
			m_haveSelect = XFalse;	//Ƭѡ����

			if(m_curInsertPoint == m_curStringLength)
			{//ĩβ
				memcpy(m_curString + m_curInsertPoint,ch,len);
			//	++ m_curInsertPoint;
				m_curStringLength += len;
				m_curString[m_curStringLength] = '\0';
			}else
			{//�м����
				//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
				strcpy(m_tempCurString,m_curString + m_curInsertPoint);
				memcpy(m_curString + m_curInsertPoint,ch,len);
				strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
			//	++ m_curInsertPoint;
				m_curStringLength += len;
			}
			changeInsertPoint(len);
		}else
		{//Ƭѡ��δ����
			//����������
			if(m_selectEnd != m_selectStart)
			{//Ƭѡ��Ч
				//���밴��
				//1��ɾ��Ƭѡ����
				deleteSelectStr();
				m_haveSelect = XFalse;	//Ƭѡ����
				if(m_curInsertPoint == m_curStringLength)
				{//ĩβ
					memcpy(m_curString + m_curInsertPoint,ch,len);
					//++ m_curInsertPoint;
					m_curStringLength += len;
					m_curString[m_curStringLength] = '\0';
				}else
				{//�м����
					//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
					strcpy(m_tempCurString,m_curString + m_curInsertPoint);
					memcpy(m_curString + m_curInsertPoint,ch,len);
					strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
					//++ m_curInsertPoint;
					m_curStringLength += len;
				}
				changeInsertPoint(len);
			}
			//Ȼ���������¿�ʼ����
			m_haveSelect = XFalse;
			m_selectEnd = m_curInsertPoint;
			m_selectStart = m_curInsertPoint;
			m_selectMouseDown = XTrue;
		}
	}else
	{//������Ƭѡ
		if(m_curInsertPoint == m_curStringLength)
		{//ĩβ
			memcpy(m_curString + m_curInsertPoint,ch,len);
			//++ m_curInsertPoint;
			m_curStringLength += len;
			m_curString[m_curStringLength] = '\0';
		}else
		{//�м����
			//strcpy(m_curString + m_curInsertPoint + 1,m_curString + m_curInsertPoint);
			strcpy(m_tempCurString,m_curString + m_curInsertPoint);
			//m_curString[m_curInsertPoint] = tempChar;
			memcpy(m_curString + m_curInsertPoint,ch,len);
			strcpy(m_curString + m_curInsertPoint + len,m_tempCurString);
			//++ m_curInsertPoint;
			m_curStringLength += len;
		}
		changeInsertPoint(len);

		if(m_selectMouseDown)
		{//���������Ƭѡ��꣬����Ҫ������
			m_selectEnd = m_curInsertPoint;
			m_selectStart = m_curInsertPoint;
		}
	}

	//����ַ��������ı䣬��Ҫ�����ַ���������
	upDataShowStr();	//�����ַ�������ʾ
	//if(m_funInputChenge != NULL) m_funInputChenge(m_pClass,m_objectID);
	if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,EDT_INPUT_CHANGE);
	else XCtrlManager.eventProc(m_objectID,EDT_INPUT_CHANGE);
	//�����ݷ����仯��ʱ�򲥷Ŷ���
	if(m_withAction)
	{//�������һ����̬Ч��
	//	m_isInAction = XTrue;
		m_lightMD.set(1.0f,2.0f,0.002f,MOVE_DATA_MODE_SIN_MULT);
		m_oldPos = m_position;
		m_oldSize = m_scale;
	}

	return;
}
#ifdef XEE_OS_WINDOWS	//windowsϵͳ��ͨ�����뷨�������ַ�
int XEdit::keyJudgement(int)
#else
int XEdit::keyJudgement(int keyOrder)
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
void XEdit::setInputLen(int len)
{//���������ĳ���
	if(len <= 0) return;
	m_mouseRect.right = m_mouseRect.left + len;
	m_textPosition.set(m_mouseRect.left + m_textEdgeWidth,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_curMouseRect.set(m_position.x + m_mouseRect.left * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y,
	m_position.x + m_mouseRect.right * m_scale.x,m_position.y + m_mouseRect.bottom * m_scale.y);
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * m_textEdgeWidth) * m_scale.x / m_curTextWidth;
	//������Ҫ�ı伸��ֵ
	if(m_curInsertPoint < m_curShowStart) m_curShowStart = m_curInsertPoint;
	if(m_curInsertPoint - m_curShowStart > m_canShowLength) m_curShowStart = m_curInsertPoint - m_canShowLength;
	upDataShowStr();
	upDateInsertShowPosition();
}
void XEdit::update(float stepTime)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	m_comment.update(stepTime);
	if(m_isEnable && m_isBeChoose)
	{//��������˸�ļ�ʱ
		m_timer += stepTime;
		if(m_timer > 500) m_timer -= 500;
		if(m_curKeyDown)
		{
			m_curKeyDownTimer += stepTime;
			if(m_curKeyDownTimer > 500)
			{//��ʼ�ظ�����
				m_curKeyRepTimer += stepTime;
				if(m_curKeyRepTimer >= 50)
				{
					m_curKeyRepTimer = 0;
					keyProc(m_curKey);
				}
			}
		}
	}
	if(!m_lightMD.getIsEnd())
	{
		m_lightMD.move(stepTime);
		XVector2 pos(m_oldPos.x + m_mouseRect.getWidth() * 0.5f * m_oldSize.x,
			m_oldPos.y + m_mouseRect.getHeight() * 0.5f * m_oldSize.y);
		XVector2 size(m_mouseRect.getWidth() * m_oldSize.x * m_lightMD.getCurData() * 0.5f,
			m_mouseRect.getHeight() * m_oldSize.y * m_lightMD.getCurData() * 0.5f);
		m_lightRect.set(pos.x - size.x,pos.y - size.y,pos.x + size.x,pos.y + size.y);
	}
	if(!m_insertActionMD.getIsEnd())
	{
		m_insertActionMD.move(stepTime);
	}
	if(m_mouseRightButtonMenu != NULL)
		m_mouseRightButtonMenu->update(stepTime);
}
#if !WITH_INLINE_FILE
#include "XEdit.inl"
#endif
}