//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XDirectoryList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XDirListTexture::_XDirListTexture()
:m_isInited(XFalse)
,dirListNormal(NULL)
,dirListDisable(NULL)
{}
_XBool _XDirListTexture::init(const char *normal,const char *disable,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		normal == NULL || disable == NULL) return XFalse;
	
	if((dirListNormal = createATextureData(normal,resoursePosition)) == NULL) return XFalse;
	if((dirListDisable = createATextureData(disable,resoursePosition)) == NULL)
	{
		XDELETE(dirListNormal);
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define DIRLIST_CONFIG_FILENAME ("DirevtoryList.txt")
_XBool _XDirListTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		filename == NULL) return XFalse;
	//����������ļ��ж�ȡ����
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,DIRLIST_CONFIG_FILENAME);
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//���ļ��ж�ȡ��Դ
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
		if((dirListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//disable
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			fclose(fp);
			return XFalse;
		}
		if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((dirListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
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
	if(resoursePosition == RESOURCE_LOCAL_PACK)
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
		if((dirListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE_ARRAY(p);
			return XFalse;
		}
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
		if((dirListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
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
void funDirListButton(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
//	printf("�����ַ���Ϊ��%s\n",pPar.m_edit.getString());
	//�����û�������ˢ���б�
	//����1�����·����ͬ��ˢ�����ݣ������Ļ�������޷�ˢ������
	//if(pPar.m_dir.m_nowDirectory.directoryName == NULL
	//	|| !fileNameCompare(pPar.m_dir.m_nowDirectory.directoryName,pPar.m_edit.getString()))
	//{
	//	pPar.m_dir.release();
	//	pPar.m_dir.init(pPar.m_edit.getString());
	//}else
	//{
	//	return;	//��ͬ�򲻸�����Ϣ
	//}
	//����2������·���Ƿ���ͬ��ˢ������
	pPar.m_dir.release();
	pPar.m_dir.init(pPar.m_edit.getString());

	//��ʼ��������Ӧ����ʾ��Ϣ
	int size = pPar.m_lineData.size();
	for(int i = 0;i < size;++ i)
	{
		pPar.m_lineData[i]->release();
		XDELETE(pPar.m_lineData[i]);
	}
	pPar.m_lineData.clear();
	pPar.m_showStartLine = 0;
	pPar.m_nowLineLeft = 0;
	//���涨��������ʾ��������Ϣ
	_XDirListOneLine * tempLine = NULL;
	pPar.m_nowLineSum = pPar.m_dir.m_nowDirectory.files.size();
	for(int i = 0;i < pPar.m_nowLineSum;++ i)
	{
		tempLine = createMem<_XDirListOneLine>();
		tempLine->m_font.setACopy(pPar.m_caption);
		tempLine->m_font.setColor(pPar.m_color);
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&(tempLine->m_font));
#endif
		if(pPar.m_dir.m_nowDirectory.files[i]->isDirectory)
		{//��Ŀ¼
			tempLine->m_check = createMem<_XCheck>();
			tempLine->m_check->setACopy(pPar.m_check);
			tempLine->m_check->setColor(pPar.m_color);
			tempLine->m_needCheck = XTrue;
	//		tempLine->m_check->setPosition(pPar.m_position + _XVector2(0.0f,42.0f + pPar.m_nowTextHeight * i));
			_XCtrlManger.decreaseAObject(tempLine->m_check);
			tempLine->m_check->setStateChangeCB(funDirListCheck,&pPar);
#if WITH_OBJECT_MANAGER
			_XObjManger.decreaseAObject(tempLine->m_check);
#endif
		}else
		{
			tempLine->m_needCheck = XFalse;
		}
	//	tempLine->m_pos.set(pPar.m_position + _XVector2(0.0f,42.0f + pPar.m_nowTextHeight * i));
	//	tempLine->m_font.setPosition(tempLine->m_pos);
		tempLine->m_font.setSize(pPar.m_fontSize * pPar.m_size);
		tempLine->m_string = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
		sprintf(tempLine->m_string,"|-%s",pPar.m_dir.m_nowDirectory.files[i]->filename);
		tempLine->m_font.setString(tempLine->m_string);
		tempLine->m_isEnable = XTrue;
		tempLine->m_file = pPar.m_dir.m_nowDirectory.files[i];
		pPar.m_lineData.push_back(tempLine);
	}
	if(pPar.m_nowLineSum > pPar.m_canShowLineSum) 
	{
		pPar.m_needShowVSlider = XTrue;
		pPar.m_verticalSlider.setNowValue(0.0f);
		pPar.m_verticalSlider.setRange(pPar.m_nowLineSum - pPar.m_canShowLineSum,0.0f);
	}else 
	{
		pPar.m_needShowVSlider = XFalse;
		pPar.m_showStartLine = 0;	//��ʼ������
		pPar.m_verticalSlider.setNowValue(0.0f);
	}
	pPar.updateHSliderState();
	//��������֮��ȡ��ѡ������
	pPar.m_haveSelect = XFalse;
}
void funDirListValueChangeV(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	pPar.m_showStartLine = pPar.m_verticalSlider.getNowValue();
	//����λ��
	pPar.updateShowPosition();
}
void funDirListValueChangeH(void *pClass,int)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	pPar.m_nowLineLeft = (int)(pPar.m_horizontalSlider.getNowValue()) * pPar.m_nowTextWidth * 2.0f;
	//����λ��
	pPar.updateShowPosition();
}
void funDirListCheck(void *pClass,int objectID)
{
	_XDirectoryList &pPar = *(_XDirectoryList *)pClass;
	int size = 0;
	if(pPar.m_nowLineSum - pPar.m_showStartLine > pPar.m_canShowLineSum) size = pPar.m_canShowLineSum;
	else size = pPar.m_nowLineSum - pPar.m_showStartLine;
	for(int i = 0;i < size;++ i)
	{
		_XDirListOneLine *tempLineData = pPar.m_lineData[i + pPar.m_showStartLine];
		if(tempLineData->m_needCheck && tempLineData->m_check->getControlID() == objectID)
		{//ȷ�ϵ��֮��������Ҫչ���ڲ��ṹ
			if(tempLineData->m_check->getState())
			{//չ��
			//	printf("չ��%s\n",tempLineData->m_string);
				int insertSum = tempLineData->m_file->directory->files.size();	//Ҫ���������
				int sum = pPar.m_nowLineSum - i - pPar.m_showStartLine - 1;
				std::vector<_XDirListOneLine *> tempBuff;		//���ڱ�����ʱ������
				for(int j = 0;j < sum;++ j)
				{//������ʱ����
					tempBuff.push_back(pPar.m_lineData[pPar.m_nowLineSum - 1 - j]);
					pPar.m_lineData.pop_back();
				}	
				//����������
				_XDirListOneLine * tempLine = NULL;
				//_XVector2 pos = tempLineData->m_pos;
				for(int j = 0;j < insertSum;++ j)
				{
					tempLine = createMem<_XDirListOneLine>();
					tempLine->m_font.setACopy(pPar.m_caption);
					tempLine->m_font.setColor(pPar.m_color);
#if WITH_OBJECT_MANAGER
					_XObjManger.decreaseAObject(&(tempLine->m_font));
#endif
				//	tempLine->m_pos.set(pos.x,pos.y + pPar.m_nowTextHeight * (j + 1));
					if(tempLineData->m_file->directory->files[j]->isDirectory)
					{//��Ŀ¼
						tempLine->m_check = createMem<_XCheck>();
						tempLine->m_check->setACopy(pPar.m_check);
						tempLine->m_check->setColor(pPar.m_color);
						tempLine->m_needCheck = XTrue;
				//		tempLine->m_check->setPosition(pos.x + (float)(tempLineData->m_file->directory->level) * pPar.m_nowTextWidth * 2.0f,
				//			tempLine->m_pos.y);
						_XCtrlManger.decreaseAObject(tempLine->m_check);
						tempLine->m_check->setStateChangeCB(funDirListCheck,&pPar);
#if WITH_OBJECT_MANAGER
						_XObjManger.decreaseAObject(tempLine->m_check);
#endif
					}else
					{
						tempLine->m_needCheck = XFalse;
					}
				//	tempLine->m_font.setPosition(tempLine->m_pos);
					tempLine->m_font.setSize(pPar.m_fontSize * pPar.m_size);
					tempLine->m_string = createArrayMem<char>(MAX_FILE_NAME_LENGTH);
					char tempStr[MAX_FILE_NAME_LENGTH] = "";
					for(int k = 0;k < tempLineData->m_file->directory->level;++ k)
					{
						tempStr[2 * k] = '|';
						tempStr[2 * k + 1] = ' ';
						tempStr[2 * k + 2] = '\0';
					}
					sprintf(tempLine->m_string,"%s|-%s",tempStr,tempLineData->m_file->directory->files[j]->filename);
					tempLine->m_font.setString(tempLine->m_string);
					tempLine->m_isEnable = XTrue;
					tempLine->m_file = tempLineData->m_file->directory->files[j];
					pPar.m_lineData.push_back(tempLine);
				}
				//����������
				for(int j = 0;j < sum;++ j)
				{
			//		tempBuff[sum - 1 - j]->m_pos.set(pos.x,pos.y + pPar.m_nowTextHeight * (j + insertSum + 1));
			//		if(tempBuff[sum - 1 - j]->m_needCheck) tempBuff[sum - 1 - j]->m_check->setPosition(tempBuff[sum - 1 - j]->m_check->getPosition().x,
			//			tempBuff[sum - 1 - j]->m_pos.y);
			//		tempBuff[sum - 1 - j]->m_font.setPosition(tempBuff[sum - 1 - j]->m_pos);
					pPar.m_lineData.push_back(tempBuff[sum - 1 - j]);
					tempBuff.pop_back();
				}
				pPar.m_nowLineSum += insertSum;
				pPar.m_verticalSlider.setRange(pPar.m_nowLineSum - pPar.m_canShowLineSum,0.0f);
				if(pPar.m_nowLineSum > pPar.m_canShowLineSum) pPar.m_needShowVSlider = XTrue;
				else 
				{
					pPar.m_needShowVSlider = XFalse;
					pPar.m_showStartLine = 0;	//��ʼ������
					pPar.m_verticalSlider.setNowValue(0.0f);
				}
				pPar.updateHSliderState();
			}else
			{//�۵�
				pPar.furlFolder(i + pPar.m_showStartLine);
			}
			break;
		}
	}
}
_XBool _XDirectoryList::init(const _XVector2& position,
	const _XRect& Area,
	_XDirListTexture &tex,
	_XFontUnicode &font,
	float fontSize,
	const _XCheck &check,
	const _XButton &button,
	const _XEdit &edit,
	const _XSlider &vSlider,	//��ֱ������
	const _XSlider &hSlider)
{
	if(m_isInited) return XTrue;
	if(tex.dirListNormal == NULL || tex.dirListDisable == NULL) return XFalse;
	if(fontSize <= 0) return XFalse;
	m_position = position;
	m_dirListNormal = tex.dirListNormal;
	m_dirListDisable = tex.dirListDisable;
	m_spriteBackGround.init(m_dirListNormal->texture.m_w,m_dirListNormal->texture.m_h,1);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_needShowVSlider = XFalse;			
	m_verticalSlider.setACopy(vSlider);	
	m_needShowHSlider = XFalse;		
	m_horizontalSlider.setACopy(hSlider);
	m_check.setACopy(check);
	m_button.setACopy(button);
	m_edit.setACopy(edit);
	m_withoutTex = XFalse;
	//���ûص�����
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	//m_mouseRect.set(1.0f,1.0f,351.0f,361.0f);
	m_mouseRect = Area;
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position = position;
	m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//��ȡ��ǰ·��
	m_edit.setString(tempDirectoryName);//��ʼ��Ϊ��ǰ·��

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

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
_XBool _XDirectoryList::initPlus(const char * path,
		_XFontUnicode &font,
		float fontSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited ||
		path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XDIRLIST_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XDirListTexture * tex = (_XDirListTexture *)m_resInfo->m_pointer;
	if(tex->dirListNormal == NULL || tex->dirListDisable == NULL) return XFalse;
	if(fontSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	m_dirListNormal = tex->dirListNormal;
	m_dirListDisable = tex->dirListDisable;
	m_spriteBackGround.init(m_dirListNormal->texture.m_w,m_dirListNormal->texture.m_h,1);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_needShowVSlider = XFalse;			
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	m_needShowHSlider = XFalse;		
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	sprintf(tempPath,"%s/Check",path);
	m_check.initPlus(tempPath," ",font,1.0f,resoursePosition);
	sprintf(tempPath,"%s/Button",path);
	m_button.initPlus(tempPath," ",font,1.0f,resoursePosition);
	sprintf(tempPath,"%s/Edit",path);
	m_edit.initPlus(tempPath," ",font,1.0f,NULL,resoursePosition);
	m_withoutTex = XFalse;
	//���ûص�����
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	m_mouseRect = tex->m_mouseRect;
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position.set(0.0f,0.0f);
	m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()));
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//��ȡ��ǰ·��
	m_edit.setString(tempDirectoryName);//��ʼ��Ϊ��ǰ·��

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

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
_XBool _XDirectoryList::initWithoutTex(const _XRect& area,	
	_XFontUnicode &font,
	float fontSize)
{
	if(m_isInited) return XTrue;
	if(fontSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);

	m_mouseRect = area;

	m_needShowVSlider = XFalse;			
//	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,_XVector2::zero);
	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight()),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	m_needShowHSlider = XFalse;		
//	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,_XVector2::zero);
	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_SLIDER_WIDTH));
	m_check.initWithoutTex(" ",font,0.5f,_XRect(0,0,DEFAULT_DIRLIST_CK_SIZE,DEFAULT_DIRLIST_CK_SIZE),
		_XVector2(DEFAULT_DIRLIST_CK_SIZE,DEFAULT_DIRLIST_CK_SIZE));
	m_button.initWithoutTex(" ",font,1.0f,_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_DIRLIST_BT_SIZE),
		_XVector2(DEFAULT_SLIDER_WIDTH >> 1,DEFAULT_DIRLIST_BT_SIZE >> 1));
	m_edit.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth(),DEFAULT_DIRLIST_BT_SIZE)," ",font,1.0f,NULL);
	m_withoutTex = XTrue;
	//���ûص�����
	m_button.setMouseUpCB(funDirListButton,this);
	m_verticalSlider.setDataChangeCB(funDirListValueChangeV,funDirListValueChangeV,this);
	m_horizontalSlider.setDataChangeCB(funDirListValueChangeH,funDirListValueChangeH,this);

	m_caption.setACopy(font);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(fontSize,fontSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_size.set(1.0f,1.0f);
	m_showPixWidth = m_mouseRect.getWidth() * m_size.x;
	m_showPixHight = m_mouseRect.getHeight() * m_size.y;

	m_haveSelect = XFalse;
	m_selectLineOrder = 0;
	m_showStartLine = 0;
	m_canShowLineSum = m_showPixHight/m_nowTextHeight;

	m_nowLineLeft = 0;
	m_maxLineWidth = 0;
	m_canShowMaxLineWidth = m_mouseRect.getWidth();

	_XCtrlManger.decreaseAObject(&m_check);
	_XCtrlManger.decreaseAObject(&m_verticalSlider);
	_XCtrlManger.decreaseAObject(&m_horizontalSlider);
	_XCtrlManger.decreaseAObject(&m_button);
	_XCtrlManger.decreaseAObject(&m_edit);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_spriteBackGround);
	_XObjManger.decreaseAObject(&m_verticalSlider);
	_XObjManger.decreaseAObject(&m_horizontalSlider);
	_XObjManger.decreaseAObject(&m_check);
	_XObjManger.decreaseAObject(&m_button);
	_XObjManger.decreaseAObject(&m_edit);
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_position.set(0.0f,0.0f);
	m_edit.setPosition(m_position);
	char tempDirectoryName[MAX_FILE_NAME_LENGTH];
	GetCurrentDirectory(MAX_FILE_NAME_LENGTH,tempDirectoryName);	//��ȡ��ǰ·��
	m_edit.setString(tempDirectoryName);//��ʼ��Ϊ��ǰ·��

	m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
	m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()));
	m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()));

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
void _XDirectoryList::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.8f * m_color.fR,0.8f * m_color.fG,0.8f * m_color.fB,m_color.fA); 

			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_verticalSlider.getMouseRectHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_horizontalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.5f * m_color.fR,0.5f * m_color.fG,0.5f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.2f * m_color.fR,0.2f * m_color.fG,0.2f * m_color.fB,m_color.fA);
		}else
		{
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.6f * m_color.fR,0.6f * m_color.fG,0.6f * m_color.fB,m_color.fA); 

			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_verticalSlider.getMouseRectHeight() * m_size.y),0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_horizontalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.65f * m_color.fR,0.65f * m_color.fG,0.65f * m_color.fB,m_color.fA);
			drawFillBoxExA(m_position + _XVector2(m_mouseRect.right * m_size.x,
				(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.45f * m_color.fR,0.45f * m_color.fG,0.45f * m_color.fB,m_color.fA);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_dirListDisable);
		else m_spriteBackGround.draw(m_dirListNormal);
	}
	//��ʾѡ���״̬
	if(m_haveSelect)
	{
		if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
		{//��Ҫ��ʾ
			drawBox(m_position.x + m_mouseRect.left * m_size.x + m_mouseRect.getWidth() * m_size.x * 0.5f,
				m_position.y + (m_mouseRect.top + m_edit.getMouseRect().getHeight() + (m_selectLineOrder - m_showStartLine + 0.5f) * m_nowTextHeight) * m_size.y,	//42.0fΪ�����ĸ�
				m_mouseRect.getWidth() * m_size.x * 0.5f,
				m_nowTextHeight * m_size.y * 0.5f,0.5f,0.0f,0.0f,0.0f,m_color.fA);
		}
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
	m_button.draw();
	m_edit.draw();
	//��ʾ�����е���Ϣ
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		tempLine->m_font.draw();
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->draw();
		}
	}
}
void _XDirectoryList::drawUp()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_needShowVSlider) m_verticalSlider.drawUp();
	if(m_needShowHSlider) m_horizontalSlider.drawUp();
	m_button.drawUp();
	m_edit.drawUp();
	//��ʾ�����е���Ϣ
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->drawUp();
		}
	}
}
void _XDirectoryList::update(int stepTime)
{
	if(m_needShowVSlider) m_verticalSlider.update(stepTime);
	if(m_needShowHSlider) m_horizontalSlider.update(stepTime);
	m_button.update(stepTime);
	m_edit.update(stepTime);
	//��ʾ�����е���Ϣ
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = 0;i < lineSum;++ i)
	{
		tempLine = m_lineData[i + m_showStartLine];
		if(tempLine->m_needCheck && tempLine->m_check != NULL) 
		{
			if(tempLine->m_check->getPosition().x >= m_position.x
				&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth())
				tempLine->m_check->update(stepTime);
		}
	}
	//���˫����ʱ
	m_mouseTime += stepTime;
}
#pragma comment(lib, "shell32.lib")	//for ShellExecute()!
_XBool _XDirectoryList::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited || 	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return XTrue;	//������ɼ�ֱ���˳�
	if(m_needShowVSlider) m_verticalSlider.mouseProc(x,y,mouseState);
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(x,y,mouseState);
	m_edit.mouseProc(x,y,mouseState);
	m_button.mouseProc(x,y,mouseState);
	//�����ļ��еĵ����������
	int lineSum = 0;
	if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
	else lineSum = m_nowLineSum - m_showStartLine;
	_XDirListOneLine * tempLine;
	for(int i = m_showStartLine;i < lineSum + m_showStartLine;++ i)
	{
		tempLine = m_lineData[i];
		if(tempLine->m_needCheck && tempLine->m_check != NULL
			&& tempLine->m_check->getPosition().x >= m_position.x
			&& tempLine->m_check->getPosition().x < m_position.x + m_canShowMaxLineWidth - tempLine->m_check->getMouseRect().getWidth()) 
		{
			if(tempLine->m_check->mouseProc(x,y,mouseState))
			{//�����궯�������˱仯�������������״̬��Ϣ
				if(m_nowLineSum - m_showStartLine >= m_canShowLineSum) lineSum = m_canShowLineSum;
				else lineSum = m_nowLineSum - m_showStartLine;
			}
		}
	}
	//��������ѡ�����
	if(mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
	{//�������Ƿ��ڷ�Χ��
		_XRect tempRect;
		tempRect.set(m_mouseRect.left * m_size.x + m_position.x,
			(m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y + m_position.y,
			m_mouseRect.right * m_size.x + m_position.x,
			(m_mouseRect.bottom + m_edit.getMouseRect().getHeight()) * m_size.y + m_position.y);
		if(tempRect.isInRect(x,y))
		{//���ڵ����Χ֮��
			int lineOrder = (y - (m_mouseRect.top + m_edit.getMouseRect().getHeight()) * m_size.y - m_position.y) / (m_nowTextHeight * m_size.y);
			if(lineOrder >= 0 && lineOrder < m_canShowLineSum)
			{
				if(lineOrder + m_showStartLine < m_nowLineSum)
				{
					m_haveSelect = XTrue;
					m_selectLineOrder = lineOrder + m_showStartLine;
				}
			}
			m_isBeChoose = XTrue;
		}
		if(m_mouseTime < 500)
		{//˫����ָ���ļ�
			//printf("˫���¼�\n");
			//char tempStr[MAX_FILE_NAME_LENGTH];
			//sprintf(tempStr,"explorer %s",getSelectFileName());
			//system(tempStr);
			if(m_haveSelect)
			{
				if(m_lineData[m_selectLineOrder]->m_needCheck)
				{//���ļ���
					if(m_lineData[m_selectLineOrder]->m_check->getState()) m_lineData[m_selectLineOrder]->m_check->setState(XFalse);
					else m_lineData[m_selectLineOrder]->m_check->setState(XTrue);
					funDirListCheck(this,m_lineData[m_selectLineOrder]->m_check->getControlID());
				}else
				{
					ShellExecute(NULL,"open",m_lineData[m_selectLineOrder]->m_file->allPath,NULL,NULL,SW_SHOW);
				}
			}
			m_mouseTime = 1000;
		}else
		{
			m_mouseTime = 0;
		}
	}else
	if(mouseState == MOUSE_MOVE || mouseState == MOUSE_RIGHT_BUTTON_DOWN || mouseState == MOUSE_MIDDLE_BUTTON_DOWN)
	{
		m_mouseTime = 1000;
	}
	return XTrue;
}
void _XDirectoryList::furlFolder(int index,_XBool flag)
{//��Ҫ�������µ������ļ���
	//ע������û�н�����������Ϸ��Լ۲�ڲ����ú�������Ҫ��֤���������������ȷ
	if(!m_lineData[index]->m_needCheck) return;			//��������ļ���ֱ�ӷ���
	if((flag && m_lineData[index]->m_check->getState())
		|| (!flag && !m_lineData[index]->m_check->getState())) return;	//�Ѿ����۵��õ�
	int insert = m_lineData[index]->m_file->directory->files.size();
	//�۵��������ļ���
	for(int i = 0;i < insert;++ i)
	{
		furlFolder(index + i + 1,XFalse);
	}
	//�۵�����
	_XDirListOneLine *tempLineData = m_lineData[index];
//	printf("�۵�%s\n",tempLineData->m_string);
	int insertSum = tempLineData->m_file->directory->files.size();	//Ҫ���������
	int sum = m_nowLineSum - index - insertSum - 1;
	for(int j = 0;j < insertSum;++ j)
	{//�ͷŶ������Դ
		m_lineData[j + index + 1]->release();
	}
	//�ƶ�
	//_XVector2 pos = tempLineData->m_pos;
	for(int j = 0;j < sum;++ j)
	{
		m_lineData[j + index + 1] = m_lineData[j + index + 1 + insertSum];
		tempLineData = m_lineData[j + index + 1];
		//�ı�λ��
	//	tempLineData->m_pos.set(pos.x,pos.y + m_nowTextHeight * (j + 1));
	//	tempLineData->m_font.setPosition(tempLineData->m_pos);
	//	if(tempLineData->m_needCheck)
	//	{
	//		tempLineData->m_check->setPosition(tempLineData->m_check->getPosition().x,pos.y);
	//	}
	}
	//ɾ����β��
	for(int j = 0;j < insertSum;++ j)
	{
		m_lineData.pop_back();
	}
	m_nowLineSum -= insertSum;
	m_verticalSlider.setRange(m_nowLineSum - m_canShowLineSum,0.0f);
	if(m_nowLineSum > m_canShowLineSum) m_needShowVSlider = XTrue;
	else 
	{
		m_needShowVSlider = XFalse;
		m_showStartLine = 0;	//��ʼ������
		m_verticalSlider.setNowValue(0.0f);
	}
	updateHSliderState();
}
void _XDirectoryList::updateHSliderState()
{
	//�������е���Ŀ��Ѱ�ҵ�������Ŀ��Ȼ����вü����߸�λ
	m_maxLineWidth = 0;				//��ǰ������п�[����]

	for(int i = 0;i < m_lineData.size();++ i)
	{
		m_lineData[i]->m_font.disClip();
		if(m_lineData[i]->m_font.getMaxPixelWidth() > m_maxLineWidth)
		{
			m_maxLineWidth = m_lineData[i]->m_font.getMaxPixelWidth();	//Ѱ������
		}
	}
	if(m_maxLineWidth > m_canShowMaxLineWidth * m_size.x)
	{//���Խ��
		if(!m_needShowHSlider)
		{//��δ���вü�
			m_needShowHSlider = XTrue;
			m_nowLineLeft = 0;
			m_horizontalSlider.setRange((m_maxLineWidth - m_canShowMaxLineWidth * m_size.x)/(m_nowTextWidth * m_size.x * 2.0f) + 1.0f,0.0f);
			m_horizontalSlider.setNowValue(0.0f);
		}else
		{//�Ѿ����и��ü�
			float tempValue = (m_maxLineWidth - m_canShowMaxLineWidth * m_size.x)/(m_nowTextWidth * m_size.x * 2.0f) + 1.0f;
			if(m_horizontalSlider.getNowValue() >= tempValue)
			{
				m_horizontalSlider.setNowValue(tempValue);
			}
			m_horizontalSlider.setRange(tempValue,0.0f);
		}
	}else
	{//��Խ��û�б�Ҫ���вü�
		if(m_needShowHSlider)
		{//ȡ�����вü�����
			m_needShowHSlider = XFalse;
			m_nowLineLeft = 0;
			m_horizontalSlider.setNowValue(0.0f);
			for(int i = 0;i < m_lineData.size();++ i)
			{
				m_lineData[i]->m_font.disClip();	//ȡ�����вü�
			}
		}
	}
}
void _XDirectoryList::updateShowPosition()
{
	int size = 0;
	if(m_nowLineSum - m_showStartLine > m_canShowLineSum) size = m_canShowLineSum;
	else size = m_nowLineSum - m_showStartLine;
	for(int i = 0;i < size;++ i)
	{
		m_lineData[i + m_showStartLine]->m_font.setSize(m_size);
		m_lineData[i + m_showStartLine]->m_font.setClipRect(m_nowLineLeft,0.0f,m_nowLineLeft + m_canShowMaxLineWidth,32.0f);	//����ĸ߶�
		m_lineData[i + m_showStartLine]->m_font.setPosition(m_position + _XVector2(0.0f - m_nowLineLeft,m_edit.getMouseRect().getHeight() + m_nowTextHeight * i) * m_size);
		if(m_lineData[i + m_showStartLine]->m_needCheck)
		{
			m_lineData[i + m_showStartLine]->m_check->setSize(m_size);
			m_lineData[i + m_showStartLine]->m_check->setPosition(m_position.x + 
				((m_lineData[i + m_showStartLine]->m_file->directory->level - 1) * m_nowTextWidth * 2.0f - m_nowLineLeft) * m_size.x,
				m_position.y + (m_edit.getMouseRect().getHeight() + m_nowTextHeight * i) * m_size.y);
		}
	}
}
void _XDirectoryList::release()
{
	if(!m_isInited) return;
	//�����ͷ����е���Դ
	for(int i = 0;i < m_nowLineSum;++ i)
	{
		m_lineData[i]->release();
		XDELETE(m_lineData[i]);
	}
	m_lineData.clear();
	m_lineData.swap(std::vector<_XDirListOneLine *>());

	_XCtrlManger.decreaseAObject(this);	//�������������ע�ᵱǰ���
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