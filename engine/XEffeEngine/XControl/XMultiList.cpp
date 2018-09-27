#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
void XMultiList::ctrlProc(void *pClass,int id,int eventID)
{
	XMultiList &pPar = *(XMultiList *)pClass;
	if(id == pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
				pPar.m_verticalSlider.setCurValue(temp);
			}else
			{//΢���ƶ�ʱ��Ĵ���
				if(pPar.m_verticalSlider.getCurValue() > pPar.m_showStartLine)
				{
					++ temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
					pPar.m_verticalSlider.setCurValue(temp);
				}else
				if(pPar.m_verticalSlider.getCurValue() < pPar.m_showStartLine)
				{
					-- temp;
					pPar.m_showStartLine = temp;
					pPar.updateSelectLine();
					pPar.updateShowChange();
					pPar.m_verticalSlider.setCurValue(temp);
				}
			}
		}else
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				//��Ҫ���´�ֱƬѡ�����
				pPar.m_showStartLine = temp;
				pPar.updateSelectLine();
				pPar.updateShowChange();
			}
		}
		return;
	}
	if(id == pPar.m_horizontalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
				pPar.m_horizontalSlider.setCurValue(temp);
			}else
			{
				if(pPar.m_horizontalSlider.getCurValue() > pPar.m_showStartRow)
				{
					++ temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}else
				if(pPar.m_horizontalSlider.getCurValue() < pPar.m_showStartRow)
				{
					-- temp;
					pPar.m_showStartRow = temp;
					pPar.updateShowChange();
					pPar.m_horizontalSlider.setCurValue(temp);
				}
			}
		}else
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_showStartRow)
			{
				pPar.m_showStartRow = temp;
				pPar.updateShowChange();
			}
		}
		return;
	}
}
XMultiListSkin::XMultiListSkin()
:m_isInited(XFalse)
,mutiListNormal(NULL)			//�����б����ͨ״̬
,mutiListDisable(NULL)			//�����б����Ч״̬
,mutiListSelect(NULL)			//�����б��б�ѡ���еı���
,mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
,mutiListTitle(NULL)			//�����б�ı��ⱳ��
,mutiListTitleEnd(NULL)		//�����б�ı���ָ���
{}
XBool XMultiListSkin::init(const char *normal,const char *disable,const char *select,const char *move,
							const char *title,const char *titleEnd,XResPos resPos)
{
	if(m_isInited) return XFalse;
	//�������е���ͼ������Ϊ��
	if(normal == NULL || disable == NULL || select == NULL || 
		move == NULL || title == NULL || titleEnd == NULL) return XFalse;

	int ret = 1;
	//���濪ʼ������ͼ
	if((mutiListNormal = createATextureData(normal,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListSelect = createATextureData(select,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListDisable = createATextureData(disable,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListMove = createATextureData(move,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListTitle = createATextureData(title,resPos)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListTitleEnd = createATextureData(titleEnd,resPos)) == NULL) ret = 0;

	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define MULTILIST_CONFIG_FILENAME "MultiList.txt"
bool XMultiListSkin::loadFromFolder(const char *filename,XResPos resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,MULTILIST_CONFIG_FILENAME);
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
	if((mutiListNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListMove = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListTitle = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListTitleEnd = createATextureData(tempFilename,resPos)) == NULL)
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
	return true;
}
bool XMultiListSkin::loadFromPacker(const char *filename,XResPos resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,MULTILIST_CONFIG_FILENAME);
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
	if((mutiListNormal = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListDisable = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListSelect = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListMove = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListTitle = createATextureData(tempFilename,resPos)) == NULL)
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
	if((mutiListTitleEnd = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		releaseTex();
		XMem::XDELETE_ARRAY(p);
		return XFalse;
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
bool XMultiListSkin::loadFromWeb(const char *filename,XResPos resPos)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XMultiListSkin::initEx(const char *filename,XResPos resPos)
{
	if(m_isInited || filename == NULL) return XFalse;
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
XMultiList::XMultiList()
	:m_isInited(XFalse)					//�Ƿ��ʼ��
	, m_mutiListNormal(NULL)			//�����б����ͨ״̬
	, m_mutiListDisable(NULL)		//�����б����Ч״̬
	, m_mutiListSelect(NULL)			//�����б��б�ѡ���еı���
	, m_mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
	, m_mutiListTitle(NULL)			//�����б�ı��ⱳ��
	, m_mutiListTitleEnd(NULL)		//�����б�ı���ָ���
	, m_needShowVSlider(XFalse)			//�Ƿ���Ҫ��ʾ��ֱ������
	, m_needShowHSlider(XFalse)			//�Ƿ���Ҫ��ʾˮƽ������
	, m_tableRowSum(0)				//����е�����
	, m_tableRow(NULL)				//�е�����
	, m_tableLineSum(0)				//����е�����
	, m_tableBox(NULL)				//����Ԫ�ص�����
	, m_mouseLeftButtonDown(XFalse)
	, m_mouseMoveDown(XFalse)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	//,m_funSelectFun(NULL)
	, m_withMouseDrag(XFalse)
	//,m_funDClick(NULL)
	, m_haveSelect(false)
	, m_selectLineOrder(-1)
	, m_mDragA(0)
	, m_mDragB(0)
{
	m_ctrlType = CTRL_OBJ_MUTILIST;
}
void XMultiList::releaseTempMemory()
{
	if (m_tableRow != NULL)
	{
		//ɾ�����е�����Ϣ
		XMultiListOneRow *tempRow = m_tableRow, *temp;
		for (int i = 0; i < m_tableRowSum; ++i)
		{
			temp = tempRow;
			tempRow = tempRow->nextRow;
			//XMem::XDELETE_ARRAY(temp->title);
			XMem::XDELETE(temp);
		}
		m_tableRow = NULL;
	}

	if (m_tableBox != NULL)
	{
		//ɾ�����б���е�Ԫ��
		XMultiListOneBox *tempBox = m_tableBox, *temp1;
		for (int i = 0; i < m_tableLineSum; ++i)
		{
			for (int j = 0; j < m_tableRowSum; ++j)
			{
				temp1 = tempBox;
				tempBox = tempBox->nextBox;
				//XMem::XDELETE_ARRAY(temp1->string);
				XMem::XDELETE(temp1);
			}
		}
		m_tableBox = NULL;
	}
	m_tableLineSum = 0;
	m_tableRowSum = 0;
}
void XMultiList::release()
{	
	if(!m_isInited) return;
	releaseTempMemory();

	XCtrlManager.decreaseAObject(this);	//�������������ע�ᵱǰ���
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
XBool XMultiList::init(const XVec2& position,		//�ռ����ڵ�λ��
		const XRect& Area,					//�ؼ���ʵ����ʾ����
		const XMultiListSkin &tex,		//�ؼ�����ͼ
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const XSlider &vSlider,	//��ֱ������
		const XSlider &hSlider)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//����Ϊ����
		return XFalse;
	}
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = Area;
	m_withoutTex = XFalse;

	m_mutiListNormal = tex.mutiListNormal;			//�����б����ͨ״̬
	m_mutiListDisable = tex.mutiListDisable;			//�����б����Ч״̬
	m_mutiListSelect = tex.mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex.mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex.mutiListTitle;			//�����б�ı��ⱳ��
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//�����б�ı���ָ���

	m_scale.set(1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(vSlider);		//��ֱ������
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) return XFalse;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		m_tableRow->text.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
			tempRow->nextRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.set(0.0f);
		m_tableBox->text.setACopy(m_caption);
		m_tableBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
				tempBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::initPlus(const char * path,		//�ؼ�����ͼ
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		XResPos resPos)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(path == NULL) return XFalse;
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXMULTILIST_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	XMultiListSkin * tex = (XMultiListSkin *)m_resInfo->m_pointer;
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex->mutiListDisable == NULL || tex->mutiListMove == NULL || tex->mutiListNormal == NULL
		|| tex->mutiListSelect == NULL || tex->mutiListTitle == NULL || tex->mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position.reset();
	m_mouseRect = tex->m_mouseRect;
	m_withoutTex = XFalse;

	m_mutiListNormal = tex->mutiListNormal;			//�����б����ͨ״̬
	m_mutiListDisable = tex->mutiListDisable;			//�����б����Ч״̬
	m_mutiListSelect = tex->mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex->mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex->mutiListTitle;			//�����б�ı��ⱳ��
	m_mutiListTitleEnd = tex->mutiListTitleEnd;		//�����б�ı���ָ���

	m_scale.set(1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setScale(m_scale);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	//m_verticalSlider.setACopy(vSlider);		//��ֱ������
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resPos);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	//m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resPos);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) return XFalse;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		m_tableRow->text.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
			tempRow->nextRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.reset();
		m_tableBox->text.setACopy(m_caption);
		m_tableBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
				tempBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::initWithoutSkin(const XRect& area,
		const XFontUnicode& font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum)
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position.reset();
	m_mouseRect = area;
	m_withoutTex = XTrue;

	m_scale.set(1.0f);
	if(!m_caption.setACopy(font)) XFalse;
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.set(0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize);
	m_caption.setScale(m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),XRect(0.0f,0.0f,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVec2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	XCtrlManager.decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),XRect(0.0f,0.0f,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVec2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH));
	XCtrlManager.decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);

	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if(m_tableRow == NULL) return XFalse;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setScale(m_scale * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
		m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		m_tableRow->text.setPosition(m_tableRow->position);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		m_tableRow->title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return XFalse;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
			tempRow->nextRow->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_curTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
			tempRow->nextRow->title = "";
			setTitleStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
//	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	m_canShowLineSum = m_showPixHight / m_curTextHeight;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() - 
			XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
//	m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return XFalse;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.reset();
		m_tableBox->text.setACopy(m_caption);
		m_tableBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);

		XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = XTrue;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
				tempBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
				XObjManager.decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setScale(m_scale * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox->textStr = "";
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiList::updateSliderState()				//���ݱ�����������»�������״̬
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	//��ֱ������
	if(m_canShowLineSum < m_tableLineSum)
	{//�����ֱ������ʾ��Χ������Ҫ������ֱ��ʾ����������ѡ��Χ
		m_needShowVSlider = XTrue;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}else
	{
		m_showStartLine = 0;
		m_needShowVSlider = XFalse;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}
	//ˮƽ������
	if(m_tableRowSum <= 0)
	{//���û��������Ҫ��ʾˮƽ������
		m_needShowHSlider = XFalse;
	}else
	{
		int width = 0;
		float rowWidth[MUTILIST_MAX_ROW_SUM];
		memset(rowWidth,0,MUTILIST_MAX_ROW_SUM * sizeof(float));
		//����1:ֻҪ����1������ʾˮƽ������
	/*	if(m_tableRowSum > 1)
		{
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}*/
	/*	//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL)
			{
				break;
			}else
			{
				tempRow = tempRow->nextRow;
			}
			++i;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - i,0);
		//	m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}else
		{
			if(m_needShowHSlider)
			{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
				m_rowDx = 0;
			}
			m_needShowHSlider = 0;
			m_showStartRow = 0;
			m_horizontalSlider.setCurValue(0);
		}*/
		//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(true)
		{
			rowWidth[i] = tempRow->pixSize.x * m_scale.x;
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
			++ i;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum && flag == 0)
			{
				flag = i;
				//break;
			}
		}
		if(flag != 0)
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
			m_needShowHSlider = XTrue;
			//m_horizontalSlider.setRange(m_tableRowSum - i,0);
			m_horizontalSlider.setRange(width - m_showPixWidth + MUTILIST_TITLE_EXPAND_LENGTH * m_scale.x,0);	//ע�������56��������չ��
		}else
		{
		//	if(m_needShowHSlider)
		//	{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
		//		m_rowDx = 0;
		//	}
			m_needShowHSlider = XFalse;
			m_showStartRow = 0;
			m_horizontalSlider.setCurValue(0);
		}
	}
}
void XMultiList::setTitleStr(const char *str)
{//δ��������
	if(str == NULL) return;
	int index = 0;
	int offset = strlen(str) + 1;
	char *tmp = XMem::createArrayMem<char>(offset);
	strcpy_s(tmp,offset,str);
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		if(index == 0) offset = XString::getCharPosition(tmp + index,';');
		else offset = XString::getCharPosition(tmp + index + 1,';');
		if(offset > 0)
		{
			if(index == 0)
			{
				tmp[index + offset] = '\0';
				setTitleStr(tmp + index,i);
				index += offset;
			}else
			{
				tmp[index + offset + 1] = '\0';
				setTitleStr(tmp + index + 1,i);
				index += offset + 1;
			}
		}else
		{
			if(tmp[index + 1] == '\0') break;
			else
			{
				setTitleStr(tmp + index + 1,i);
				break;
			}
		}
	}
	XMem::XDELETE_ARRAY(tmp);
}
std::string XMultiList::getTitleStr()
{
	if(m_tableRowSum == 0) return XString::gNullStr;
	std::string ret = m_tableRow->title;
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 1;i < m_tableRowSum;++ i)
	{
		tempRow = tempRow->nextRow;
		ret += ";" + tempRow->title;
	}
	return ret + ";";
}
const std::string& XMultiList::getTitleStr(int order)const
{
	if(order < 0 || order >= m_tableRowSum) return XString::gNullStr;
	XMultiListOneRow *tempRow;
	if(order == 0)
	{
		tempRow = m_tableRow;
	}else
	{
		tempRow = m_tableRow;
		for(int i = 1;i <= order;++ i)
		{
			tempRow = tempRow->nextRow;
		}
	}
	return tempRow->title;
}
void XMultiList::setTitleStr(const char *str,int order)
{
	if(str == NULL) return;
	if(order < 0 || order >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ������
	XMultiListOneRow *tempRow;
	if(order == 0)
	{
		tempRow = m_tableRow;
	}else
	{
		tempRow = m_tableRow;
		for(int i = 1;i <= order;++ i)
		{
			tempRow = tempRow->nextRow;
		}
	}
	//�޸�ָ���е��ַ���
	tempRow->title = str;

	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				memcpy(&tempRow->title[tempRow->stringShowWidth - 2], &"..", 3);
				//tempRow->title[tempRow->stringShowWidth] = '\0';
				//tempRow->title[tempRow->stringShowWidth - 1] = '.';
				//tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				memcpy(&tempRow->title[tempRow->stringShowWidth - 3], &"...", 4);
				//tempRow->title[tempRow->stringShowWidth] = '\0';
				//tempRow->title[tempRow->stringShowWidth - 1] = '.';
				//tempRow->title[tempRow->stringShowWidth - 2] = '.';
				//tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
}
void XMultiList::setBoxStr(const char *str,int line,int row)	//����ĳһ����Ԫ�������
{
	//��������������ж�
	if(str == NULL) return;
	if(line < 0 || line >= m_tableLineSum) return;
	if(row < 0 || row >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ���ı��
	XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	//�����ڴ�ռ�
	tempBox->textStr = str;
	//������ʾ�ַ�
	if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
			{
				memcpy(&tempBox->textStr[tempRow->stringShowWidth - 2], &"..", 3);
				//tempBox->textStr[tempRow->stringShowWidth] = '\0';
				//tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				//tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				memcpy(&tempBox->textStr[tempRow->stringShowWidth - 3], &"...", 4);
				//tempBox->textStr[tempRow->stringShowWidth] = '\0';
				//tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
				//tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
				//tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempBox->textStr[tempRow->stringShowWidth] = '\0';
		}
		tempBox->text.setString(tempBox->textStr.c_str());
		tempBox->textStr = str;
	}else
	{
		tempBox->text.setString(tempBox->textStr.c_str());
	}
}
const char *XMultiList::getBoxStr(int line,int row)
{//����������ܻ�������
	//��������������ж�
	if(line < 0 || line >= m_tableLineSum) return NULL;
	if(row < 0 || row >= m_tableRowSum) return NULL;
	//Ѱ�ҵ�ָ���ı��
	XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	return tempBox->textStr.c_str();
}
void XMultiList::updateShowChange()				//�����б��ı仯��Ϣ�����б���еı�����ʾ���
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(m_tableRowSum <= 0) return;	//���û�б��ֱ�ӷ���
	//�������ʾ
	XMultiListOneRow *tempRow = m_tableRow;
	int curWidthPix = 0;
	int row = 0;
	int showMaxRow = 0;
//	int showDx = m_showStartRow; //��ʾ�����ݱ���ˮƽƫ��	m_showStartRow(ˮƽƫ����)
	while(true)
	{
		if(curWidthPix + tempRow->pixSize.x * m_scale.x > m_showStartRow && curWidthPix - m_showStartRow <= m_showPixWidth)
		{
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth)
			{//��ʾ�ұ߲���
				tempRow->isShow = 1;	//ͷ
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//��ߵ��и�λ��
				tempRow->right = tempRow->pixSize.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT < 
						m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / 
						m_mutiListTitle->textureSize.y < m_showStartRow - curWidthPix)
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(tempRow->pixSize.x * m_scale.x - (m_showStartRow - curWidthPix)) / m_mutiListTitle->textureSize.x,m_scale.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow <= m_showPixWidth && curWidthPix >= m_showStartRow)
			{//����������ʾ
				tempRow->isShow = 1;//����
				curWidthPix += tempRow->pixSize.x * m_scale.x;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->text.disClip();
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = tempRow->pixSize.x * m_scale.x;
				}else
				{
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = 0.0f;					//�ұߵ��и�λ��
			}else
			if(curWidthPix >= m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//��ʾ��߲���
				tempRow->isShow = 1;	//β
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(0.0f,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = m_showPixWidth - (curWidthPix - m_showStartRow);
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth - (curWidthPix - m_showStartRow)) / m_mutiListTitle->textureSize.x,m_scale.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			if(curWidthPix < m_showStartRow && curWidthPix + tempRow->pixSize.x * m_scale.x - m_showStartRow > m_showPixWidth)
			{//������Ҫ��ͷȥβ��Ŀǰû�д���
				tempRow->isShow = 2; //�м�
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - curWidthPix) / m_scale.x;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (curWidthPix - m_showStartRow)) / m_scale.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - curWidthPix;
					tempRow->pixLen = m_showPixWidth;
					if(tempRow->pixSize.x * m_scale.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_scale.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (curWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - curWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setScale((float)(m_showPixWidth) / m_mutiListTitle->textureSize.x,m_scale.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				curWidthPix += tempRow->pixSize.x * m_scale.x;
			}else
			{
				printf("haha\n");
			}
		}else
		{
			curWidthPix += tempRow->pixSize.x * m_scale.x;
			tempRow->isShow = 0;
		}
	//	if(row < m_showStartRow)
	//	{//����ˮƽƫ��
	//		showDx += tempRow->pixSize.x * m_scale.x;
	//	}
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			++ row;
			tempRow = tempRow->nextRow;
		}
	}
	//�����б������ȫ������
	if(m_tableLineSum <= 0) return;	//���û����ֱ�ӷ���
	XMultiListOneBox *tempBox = m_tableBox;
	tempRow = m_tableRow;
	if(tempRow == NULL) return;
	row = 0;
	int line = 0;
	while(true)
	{
		if(line >= m_showStartLine && line < m_showStartLine + m_canShowLineSum)
		{
			//if(row >= m_showStartRow && row <= showMaxRow)
			assert(tempRow != NULL);
			if(tempRow->isShow != 0)
			{
				tempBox->isShow = XTrue;
				tempBox->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y + m_curTextHeight * (line - m_showStartLine + 1));
				if(tempRow->needChip != 0) tempBox->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempBox->text.getTextSize().y);
				else tempBox->text.disClip();
			}else
			{
				tempBox->isShow = XFalse;
			}
		}else
		{
			tempBox->isShow = XFalse;
		}
		if(tempBox->nextBox == NULL) break;
		else tempBox = tempBox->nextBox;
		++row;
		if(row >= m_tableRowSum)
		{
			row = 0;
			++ line;
			tempRow = m_tableRow;
		}else
		{
			assert(tempRow != NULL);
			tempRow = tempRow->nextRow;
		}
	}
}
//ע�����ﴫ��ĳߴ�Ϊ����֮ǰ�ĳߴ磬Ҳ�����ǿռ����ű���Ϊ1��ʱ��óߴ�
XBool XMultiList::setRowWidth(int width,int order)	//��������һ�еĿ��
{
	if (width < MUTILIST_MIN_WIDTH) return XFalse;
	if (width > m_showPixWidth / m_scale.x) width = m_showPixWidth / m_scale.x;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	//�ҵ���Ӧ��һ��
	XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < order;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	
	tempRow->pixSize.set(width,tempRow->pixSize.y);	//�ı䵱ǰ�е��п�
	tempRow->stringShowWidth = tempRow->pixSize.x * m_scale.x / m_curTextWidth;	//�ı䵱ǰ�е���ʾ�ַ����
	if(!m_withoutTex)
	{
		tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x + tempRow->pixSize.x * m_scale.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		tempRow->m_spriteTitle.setScale(tempRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
	}
	tempRow->text.setPosition(tempRow->position);
	//�ı䵱ǰ�е���ʾ�ַ�
	std::string str = tempRow->title;
	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title.c_str())) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			if(XString::isAtUnicodeEnd(tempRow->title.c_str(),tempRow->stringShowWidth - 2))
			{
				memcpy(&tempRow->title[tempRow->stringShowWidth - 2], &"..", 3);
				//tempRow->title[tempRow->stringShowWidth] = '\0';
				//tempRow->title[tempRow->stringShowWidth - 1] = '.';
				//tempRow->title[tempRow->stringShowWidth - 2] = '.';
			}else
			{
				memcpy(&tempRow->title[tempRow->stringShowWidth - 3], &"...", 4);
				//tempRow->title[tempRow->stringShowWidth] = '\0';
				//tempRow->title[tempRow->stringShowWidth - 1] = '.';
				//tempRow->title[tempRow->stringShowWidth - 2] = '.';
				//tempRow->title[tempRow->stringShowWidth - 3] = '.';
			}
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title.c_str());
		tempRow->title = str;
	}else
	{
		tempRow->text.setString(tempRow->title.c_str());
	}
	//XMem::XDELETE_ARRAY(str);
	//�޸ĺ���������е�λ��
	while(true)
	{
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			tempRow = tempRow->nextRow;
		}
	}
	//�ı����е�Ԫ������
	int i,j;
	std::string tmpStr;
	XMultiListOneBox *tempBox = m_tableBox;
	for(i = 0;i < m_tableLineSum;++ i)
	{
		tempRow = m_tableRow;
		for(j = 0;j < m_tableRowSum;++ j)
		{
			if(j == order)
			{//�ı��ַ���
				tmpStr = tempBox->textStr;
				if((int)(strlen(tempBox->textStr.c_str())) >= tempRow->stringShowWidth)
				{//���������ʾ����
					if(tempRow->stringShowWidth >= 2)
					{//����3���ַ�������ʾ...������ʾ..
						if(XString::isAtUnicodeEnd(tempBox->textStr.c_str(),tempRow->stringShowWidth - 2))
						{
							memcpy(&tempBox->textStr[tempRow->stringShowWidth - 2], &"..", 3);
							//tempBox->textStr[tempRow->stringShowWidth] = '\0';
							//tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							//tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
						}else
						{
							memcpy(&tempBox->textStr[tempRow->stringShowWidth - 3], &"...", 4);
							//tempBox->textStr[tempRow->stringShowWidth] = '\0';
							//tempBox->textStr[tempRow->stringShowWidth - 1] = '.';
							//tempBox->textStr[tempRow->stringShowWidth - 2] = '.';
							//tempBox->textStr[tempRow->stringShowWidth - 3] = '.';
						}
					}else
					{
						tempBox->textStr[tempRow->stringShowWidth] = '\0';
					}
					tempBox->text.setString(tempBox->textStr.c_str());
					tempBox->textStr = tmpStr;
				}else
				{
					tempBox->text.setString(tempBox->textStr.c_str());
				}
				//XMem::XDELETE_ARRAY(tmpStr);
			}
			if(j > order)
			{//�ı�λ��
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
			}

			tempRow = tempRow->nextRow;
			tempBox = tempBox->nextBox;
		}
	}
	//������ʾ����
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiList::draw()	//��溯��
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		XFColor tmpColor;
		//�������󱳾�
		if (m_isEnable) tmpColor = XCCS::normalColor * m_color;
		else tmpColor = XCCS::specialColor * m_color;

		XRender::drawFillRectA(m_position + m_mouseRect.getLT() * m_scale,
			XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale, tmpColor, true);

		if(m_actionMD.getIsEnd())
		{
			if (m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
			{//��ʾѡ����
				XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,
					(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + (m_selectLineOrder - m_showStartLine) * m_curTextHeight),
					XVec2(m_showPixWidth, m_curTextHeight), XCCS::downColor * m_color);
				//m_spriteSelect.draw(m_mutiListSelect);
			}
		}else
		{
			switch(m_actionType)
			{
			case MLTLST_ACTION_TYPE_IN:		//ѡ�����
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,
						(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + 
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_showPixWidth,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::downColor * m_color);
				}
				break;
			case MLTLST_ACTION_TYPE_MOVE:	//ѡ���ƶ�
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{//�������
					float pos = XMath::lineSlerp<float>((m_actionPosition - m_showStartLine) * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					XRender::drawFillRectExA(m_position + XVec2( + m_mouseRect.left * m_scale.x,
						(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + pos),
						XVec2(m_showPixWidth,m_curTextHeight),
						XCCS::downColor * m_color);
				}else if(m_actionPosition < m_showStartLine)
				{//�ϱ�Խ��
					float pos = XMath::lineSlerp<float>(-1.0f * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					float h = m_curTextHeight;
					if(pos < 0.0f)
					{
						h += pos;
						pos = 0.0f;
					}
					if(h > 0.0f)
					{
						XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,
							(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + pos),
							XVec2(m_showPixWidth,h),
							XCCS::downColor * m_color);
					}
				}else if(m_actionPosition >= m_showStartLine + m_canShowLineSum)
				{//�±�Խ��
					float pos = XMath::lineSlerp<float>(m_canShowLineSum * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					float h = m_curTextHeight;
					if(pos + h > m_canShowLineSum * m_curTextHeight)
					{
						h += (m_canShowLineSum * m_curTextHeight - pos - h);
					}
					if(h > 0.0f)
					{
						XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,
							(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + pos),
							XVec2(m_showPixWidth,h),
							XCCS::downColor * m_color);
					}
				}
				break;
			case MLTLST_ACTION_TYPE_DCLICK:	//˫��
			case MLTLST_ACTION_TYPE_OUT:	//ȡ��ѡ��
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,
						(m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_scale.y + 
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_showPixWidth,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::downColor * m_color);
				}
				break;
			}
		}
		//������ʾ�б������
		if(m_tableRowSum > 0)
		{
			XFColor boColor = XCCS::blackOnColor * m_color;
			XFColor mColor = XCCS::mouseColor * m_color;
			XFColor bnColor = XCCS::blackNormalColor * m_color;
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					XRender::drawFillRectExA(XVec2(tempRow->posX, tempRow->position.y),
						XVec2(tempRow->pixLen, DEFAULT_TITLE_HEIGHT * m_scale.y), boColor);
				//	tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow == 1)
					{//���ܻ�Խ��
						int x = tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x - 
							DEFAULT_END_WIDTH * tempRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
					//	if(x >= m_curMouseRect.left)
						{
							XRender::drawLine(XVec2(x, m_tableRow->position.y), XVec2(x, m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y),
								1.0f, mColor);
							//����зֽ���
							XRender::drawLine(XVec2(x, m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_scale.y), XVec2(x,
								m_tableRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y),
								1.0f, bnColor);
							//tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
						}
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) 
		{
			int x = m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x - 
								DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT;
			XRender::drawLine(x,m_changeRow->position.y,x,m_changeRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale.y,1,
				XCCS::blackDownColor * m_color);
			//m_spriteMove.draw(m_mutiListMove);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_mutiListDisable);
		else m_spriteBackGround.draw(m_mutiListNormal);
		if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
		{//������Ҫ����ѡ�񶯻��Ķ���(��δ����)
			m_spriteSelect.draw(m_mutiListSelect);
		}
		//������ʾ�б������
		if(m_tableRowSum > 0)
		{
			XMultiListOneRow *tempRow = m_tableRow;
			while(true)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow == 1)tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				XMultiListOneBox *tempBox = m_tableBox;
				while(true)
				{
					if(tempBox->isEnable && tempBox->isShow) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) m_spriteMove.draw(m_mutiListMove);
	}
	if(m_withoutTex)
	{//������������������Ŀ�λ
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				0.5f * m_color.r,0.5f * m_color.g,0.5f * m_color.b,m_color.a,true);
		}else
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				0.65f * m_color.r,0.65f * m_color.g,0.65f * m_color.b,m_color.a,true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale, 
				0.65f * m_color.r,0.65f * m_color.g,0.65f * m_color.b,m_color.a,true);
		}
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
	if(m_withoutTex)
	{//����������½ǵĿ�λ
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				0.25f * m_color.r, 0.25f * m_color.g, 0.25f * m_color.b, m_color.a, true);
		}else
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				0.45f * m_color.r, 0.45f * m_color.g, 0.45f * m_color.b, m_color.a, true);
		}
	}
}
XBool XMultiList::mouseProc(const XVec2& p, XMouseState mouseState)					//������궯������Ӧ����
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if (m_isSilent) return XFalse;

	if (m_needShowVSlider)
	{
		m_verticalSlider.mouseProc(p, mouseState);
		if (isInRect(p))
		{//������Ӧ�����¼�
			switch (mouseState)
			{
			case MOUSE_WHEEL_UP_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() - 1.0f, true);
				break;
			case MOUSE_WHEEL_DOWN_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() + 1.0f, true);
				break;
			}
		}
	}
	if (m_needShowHSlider)
	{
		m_horizontalSlider.mouseProc(p, mouseState);
		if (isInRect(p) && (!m_needShowVSlider || !m_horizontalSlider.getVisible() ||
			m_horizontalSlider.getCurValue() >= m_horizontalSlider.getMaxValue()))
		{//������Ӧ�����¼�
			float stepLen = (m_horizontalSlider.getMaxValue() - m_horizontalSlider.getMinValue()) * 0.05f;
			switch (mouseState)
			{
			case MOUSE_WHEEL_UP_DOWN:
				m_horizontalSlider.setCurValue(m_horizontalSlider.getCurValue() - stepLen, true);
				break;
			case MOUSE_WHEEL_DOWN_DOWN:
				m_horizontalSlider.setCurValue(m_horizontalSlider.getCurValue() + stepLen, true);
				break;
			}
		}
	}
	//�����ж�����޸ı���������ж�
	if (m_tableRowSum > 0)
	{
		if (mouseState == MOUSE_LEFT_BUTTON_DOWN || mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//�������
			XRect tempRect;
			XMultiListOneRow *tempRow = m_tableRow;
			int order = 0;
			//	int showDx = m_showStartRow;	//�����Ҫ����������ڿ�ʼ��ʾ����һ�е�ƫ��������Ϊ�е�λ�����ȶ��ڵ�һ�еľ���λ�ã�������Ҫ����ʵ�ʵ���ʾ������Ե���ʾλ��
			while (true)
			{
				if (tempRow->isShow != 0)
				{//��ʾ�����Ĳ����޸�
					if (m_withoutTex)
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x -
							DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_scale.y / DEFAULT_TITLE_HEIGHT * 2.0f,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}
					else
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x -
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_scale.x,
							tempRow->position.y + tempRow->pixSize.y * m_scale.y);
					}
					if (tempRect.isInRect(p))
					{//����϶���Ч
						m_mouseLeftButtonDown = XTrue;
						m_startX = p.x;
						//	m_rowDx = m_showStartRow;
						m_changeRowOrder = order;
						m_changeRow = tempRow;
						//���ÿ�ʼ��ʾ�ƶ�ͼ��
						m_needShowMove = XTrue;
						if (!m_withoutTex)
						{
							m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x -
								m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
								m_changeRow->position.y);
						}
						break;
					}
				}
				//	if(order < m_showStartRow)
				//	{
				///		showDx += tempRow->pixSize.x * m_scale.x;
				//	}
				if (tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
				++order;
			}
		}
		if (mouseState == MOUSE_MOVE && m_mouseLeftButtonDown)
		{//�϶���Ч
			//�����϶��Ľ��
			int dx = p.x - m_startX;
			m_startX = p.x;
			if (m_changeRow->position.x - m_showStartRow - m_tableRow->position.x +
				m_changeRow->pixSize.x * m_scale.x + dx > m_showPixWidth || //�ұ�Խ��
				p.x < m_curMouseRect.left)	//���Խ��
			{//����ƶ�������ȡ���ƶ���������չ�����
				setRowWidth((m_showPixWidth - m_changeRow->position.x + m_showStartRow + m_tableRow->position.x) /
					m_scale.x, m_changeRowOrder);
				m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = XFalse;
			}
			else
				if (m_changeRow->pixSize.x *m_scale.x + dx < MUTILIST_MIN_WIDTH * m_scale.x)
				{//���С����Сֵ����ʹ����Сֵ����ȡ���ƶ�
					setRowWidth(MUTILIST_MIN_WIDTH, m_changeRowOrder);
					m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
					m_mouseLeftButtonDown = XFalse;
				}
				else
				{
					if (!setRowWidth(m_changeRow->pixSize.x + dx / m_scale.x, m_changeRowOrder))
					{//�϶�ʧ����ȥ������
						m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
						m_mouseLeftButtonDown = XFalse;
					}
					if (!m_withoutTex)
					{
						m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_scale.x -
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							m_changeRow->position.y);
					}
				}
		}
	}
	if (mouseState == MOUSE_LEFT_BUTTON_UP)
	{//���������ȥ������
		if (m_mouseLeftButtonDown)
		{
			m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
			m_mouseLeftButtonDown = XFalse;
		}
		if (isInRect(p)) m_isBeChoose = XTrue;
	}
	//�����ж���ѡ
	if (m_tableLineSum > 0)
	{
		if (mouseState == MOUSE_LEFT_BUTTON_DOWN
			|| mouseState == MOUSE_LEFT_BUTTON_DCLICK)
		{//�������
			//�����ѡ��Χ
			XRect tempRect;
			if (m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y,
					XVec2(m_showPixWidth, m_showPixHight));
			}
			else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y,
					XVec2(m_showPixWidth, m_showPixHight));
			}
			if (tempRect.isInRect(p))
			{//�����ѡ������һ��
				int DLine = (p.y - tempRect.top) / m_curTextHeight;
				if (DLine + m_showStartLine < m_tableLineSum)
				{//��ѡ��Ч
					bool isSameChoose = (m_selectLineOrder == DLine + m_showStartLine);
					if (!m_haveSelect) setAction(MLTLST_ACTION_TYPE_IN, DLine + m_showStartLine); else
						if (!isSameChoose) setAction(MLTLST_ACTION_TYPE_MOVE, m_selectLineOrder);
					m_haveSelect = XTrue;
					m_selectLineOrder = DLine + m_showStartLine;
					updateSelectLine();
					//�����갴����Ч
					m_mouseMoveDown = XTrue;
					m_oldLineOrder = m_selectLineOrder;
					//if(m_funSelectFun != NULL) m_funSelectFun(m_pClass,m_objectID);
					if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_SELECT);
					else XCtrlManager.eventProc(m_objectID, MLTLST_SELECT);
					if (mouseState == MOUSE_LEFT_BUTTON_DCLICK)
					{
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_DCLICK);
						else XCtrlManager.eventProc(m_objectID, MLTLST_DCLICK);
						if (isSameChoose) setAction(MLTLST_ACTION_TYPE_DCLICK, m_selectLineOrder);
					}
				}
			}
		}
		if (mouseState == MOUSE_MOVE && m_mouseMoveDown)
		{//�϶���Ч
			//�����ѡ��Χ(ע��������չ��20��Ϊ��ʵ���϶�������Χ�Ĵ���)
			XRect tempRect;
			if (m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + DEFAULT_TITLE_HEIGHT * m_scale.y - 100,
					XVec2(m_showPixWidth, m_showPixHight));
			}
			else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.x + m_mutiListTitle->textureSize.y * m_scale.y - 100,
					XVec2(m_showPixWidth, m_showPixHight));
			}
			if (tempRect.isInRect(p))
			{
				int DLine = (p.y - tempRect.top - 100) / m_curTextHeight;
				if (DLine < 0 && m_showStartLine > 0)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if (m_withMouseDrag && moveUpLine(m_oldLineOrder))
					{
						m_mDragA = m_oldLineOrder;
						m_mDragB = --m_oldLineOrder;
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_LINE_CHANGE);
						else XCtrlManager.eventProc(m_objectID, MLTLST_LINE_CHANGE);

						--m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}
				else if (DLine >= m_canShowLineSum && m_showStartLine < m_tableLineSum - m_canShowLineSum)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if (m_withMouseDrag && moveDownLine(m_oldLineOrder))
					{
						m_mDragA = m_oldLineOrder;
						m_mDragB = ++m_oldLineOrder;
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_LINE_CHANGE);
						else XCtrlManager.eventProc(m_objectID, MLTLST_LINE_CHANGE);

						++m_showStartLine;
						m_verticalSlider.setCurValue(m_showStartLine);
						updateSliderState();
						updateShowChange();
					}
				}
				else if (DLine + m_showStartLine < m_oldLineOrder)
				{//�����ƶ�
					if (m_withMouseDrag && moveUpLine(m_oldLineOrder))
					{
						m_mDragA = m_oldLineOrder;
						m_mDragB = --m_oldLineOrder;
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_LINE_CHANGE);
						else XCtrlManager.eventProc(m_objectID, MLTLST_LINE_CHANGE);
					}
				}
				else if (DLine + m_showStartLine > m_oldLineOrder)
				{//�����ƶ�
					if (m_withMouseDrag && moveDownLine(m_oldLineOrder))
					{
						m_mDragA = m_oldLineOrder;
						m_mDragB = ++m_oldLineOrder;
						if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_LINE_CHANGE);
						else XCtrlManager.eventProc(m_objectID, MLTLST_LINE_CHANGE);
					}
				}
			}
		}
	}
	if (mouseState == MOUSE_LEFT_BUTTON_UP && m_mouseMoveDown)
	{
		m_mouseMoveDown = XFalse;
		m_oldLineOrder = m_selectLineOrder;
	}
	return XTrue;
}
//�ļ���������ʽΪXMultiList_ID.dat
XBool XMultiList::exportData(const char *fileName)	//���ݵ���
{//���ݵ������ļ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiList_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//д������Ϣ
	XMultiListOneRow *tempRow = m_tableRow;
	int strLength = 0;
	float tempWidth = 0;
	fwrite(&m_tableRowSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		tempWidth = tempRow->pixSize.x;
		fwrite(&(tempWidth),sizeof(float),1,fp);
		strLength = strlen(tempRow->title.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempRow->title.c_str(),strLength + 1,1,fp);
		tempRow = tempRow->nextRow;
	}
	//д������Ϣ
	XMultiListOneBox *tempBox = m_tableBox;
	fwrite(&m_tableLineSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		strLength = strlen(tempBox->textStr.c_str());
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempBox->textStr.c_str(),strLength + 1,1,fp);
		tempBox = tempBox->nextBox;
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiList::importData(const char *fileName)	//���ݵ���
{//���ļ��н����ݵ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiList_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//��ȡ����Ϣ
	int strLength = 0;
	int tempSum = 0;
	float tempWidth = 0;
	char *str;
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableRowSum)
	{//��񲻷���
		setRowSum(tempSum);
	}
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		fread(&tempWidth,sizeof(float),1,fp);
		setRowWidth(tempWidth,i);
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;
		fread(str,strLength + 1,1,fp);
		setTitleStr(str,i);
		XMem::XDELETE_ARRAY(str);
	}
	//��ȡ�����Ϣ
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableLineSum)
	{//��񲻷���
		setLineSum(tempSum);
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		fread(&(strLength),sizeof(int),1,fp);
		str = XMem::createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;
		fread(str,strLength + 1,1,fp);
		setBoxStr(str,i / m_tableRowSum,i % m_tableRowSum);
		XMem::XDELETE_ARRAY(str);
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiList::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	TiXmlElement elmNode(m_ctrlName.c_str());
	XBool flag = XTrue;
	//д�����
	XMultiListOneRow *tempRow = m_tableRow;
	if(!XXml::addLeafNode(elmNode,"RowSum",XString::toString(m_tableRowSum))) flag = XFalse;
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Row",tempRow->title)) flag = XFalse;
		tempRow = tempRow->nextRow;
	}
	XMultiListOneBox *tempBox = m_tableBox;
	if(!XXml::addLeafNode(elmNode,"LineSum",XString::toString(m_tableLineSum))) flag = XFalse;
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		if(!XXml::addLeafNode(elmNode,"Box",tempBox->textStr)) flag = XFalse;
		tempBox = tempBox->nextBox;
	}
	if(e.InsertEndChild(elmNode) == NULL) flag = XFalse;
	return flag;
}
XBool XMultiList::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	TiXmlNode * keyNode = e->FirstChildElement(m_ctrlName.c_str());
	if(keyNode == NULL) return XFalse;
	//�����ȡ
	XBool flag = XTrue;
	int tmpI = 0;
	std::string tmpStr = "";
	if(!XXml::getXmlAsInt(keyNode,"RowSum",tmpI)) return XFalse;
	if(tmpI != m_tableRowSum) setRowSum(tmpI);//��񲻷���
	int index = 0;
	TiXmlNode *tmpNode = XXml::getXmlAsString(keyNode,"Row",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setTitleStr(tmpStr.c_str(),index);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Row",tmpStr);
	}

	if(!XXml::getXmlAsInt(keyNode,"LineSum",tmpI)) return XFalse;
	if(tmpI != m_tableLineSum) setLineSum(tmpI);//��񲻷���
	index = 0;
	tmpNode = XXml::getXmlAsString(keyNode,"Box",tmpStr);
	while(tmpNode != NULL)
	{
	//	if(tmpNode != NULL)
	//	{
			setBoxStr(tmpStr.c_str(),index / m_tableRowSum,index % m_tableRowSum);
			++ index;
	//	}else
	//		break;
		tmpNode = XXml::getNextXmlAsString(tmpNode,"Box",tmpStr);
	}
	return flag;
}
void XMultiList::initANewRowData(XMultiListOneRow * upRow,int i)
{
	if(i == 0)
	{//��һ��Ԫ��
		XMultiListOneRow& ref = *m_tableRow;
		ref.isEnable = XTrue;

		ref.order = 0;
		ref.text.setACopy(m_caption);
		ref.text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(ref.text));
#endif
		ref.text.setScale(m_scale * m_fontSize);
		ref.pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		ref.position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		ref.text.setPosition(ref.position);
		if(!m_withoutTex)
		{
			ref.m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(ref.m_spriteTitle));
#endif
			ref.m_spriteTitle.setPosition(ref.position);
			ref.m_spriteTitle.setScale(ref.pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			ref.m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			ref.m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(ref.m_spriteTitleEnd));
#endif
			ref.m_spriteTitleEnd.setPosition(ref.position.x + ref.pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * ref.pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,ref.position.y);
			ref.m_spriteTitleEnd.setScale(ref.pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				ref.pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			ref.m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		ref.stringShowWidth = ref.pixSize.x * m_scale.x / m_curTextWidth;
		ref.title = "";
		setTitleStr("Title",0);	//����Ĭ����ʾ
		ref.isShow = XTrue;
	}else
	{
		XMultiListOneRow& ref = *upRow->nextRow;
		ref.isEnable = XTrue;
		ref.order = i;
		ref.text.setACopy(m_caption);
		ref.text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(ref.text));
#endif
		ref.text.setScale(m_scale * m_fontSize);
		ref.pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getScale().x);
		ref.position.set(upRow->position.x + upRow->pixSize.x * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y);
		ref.text.setPosition(ref.position);
		if(!m_withoutTex)
		{
			ref.m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(ref.m_spriteTitle));
#endif
			ref.m_spriteTitle.setPosition(ref.position);
			ref.m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			ref.m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			ref.m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(ref.m_spriteTitleEnd));
#endif
			ref.m_spriteTitleEnd.setPosition(ref.position.x + m_tableRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			ref.m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y);
			ref.m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		ref.stringShowWidth = m_tableRow->pixSize.x * m_scale.x / m_curTextWidth;
		ref.title = "";
		setTitleStr("Title",i);	//����Ĭ����ʾ
		ref.isShow = XTrue;
	}
}
XBool XMultiList::setRowSum(int rowSum)		//��������������������հף�����ɾ��
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		rowSum < 0 ||	//�Ƿ�����ֱ�ӷ�Χʧ��
		rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;		//�����е����ֵ�����˳�	
	if(rowSum == m_tableRowSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	//�������ʵ������޸Ŀؼ��е�����
	if(rowSum > m_tableRowSum)
	{//���
		int i,j;
		int oldRowSum = m_tableRowSum;
		m_tableRowSum = rowSum;
		//��ӱ�������
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
			m_tableRow = XMem::createMem<XMultiListOneRow>();
			if(m_tableRow == NULL) return XFalse;

			initANewRowData(NULL,0);

			tempRow = m_tableRow;
			for(i = 1;i < m_tableRowSum;++ i)
			{
				tempRow->nextRow = XMem::createMem<XMultiListOneRow>();
				if(tempRow->nextRow == NULL) return XFalse;

				initANewRowData(tempRow,i);

				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
			tempRow->nextRow = NULL;	//���һ����β

		}else
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= oldRowSum)
				{//�����µ���
					assert(temp != NULL);
					temp->nextRow = XMem::createMem<XMultiListOneRow>();
					if(temp->nextRow == NULL) return XFalse;

					initANewRowData(temp,i);

					temp = temp->nextRow;
					temp->nextRow = tempRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}
		//��ӱ������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}else
		{
			if(m_tableLineSum > 0)
			{
				for(i = 0;i < m_tableLineSum;++ i)
				{
					tempRow = m_tableRow;
					for(j = 0;j < m_tableRowSum;++ j)
					{
						if(j >= oldRowSum)	
						{
							//if(temp1 == NULL) return XFalse;//����oldRowSum > 0����temp1����NULL
							assert(temp1 != NULL);
							temp1->nextBox = XMem::createMem<XMultiListOneBox>();
							if(temp1->nextBox == NULL) return XFalse;

							temp1 = temp1->nextBox;
							temp1->isEnable = XTrue;

							temp1->isShow = XTrue;
							temp1->order.set(j,i);
							temp1->text.setACopy(m_caption);
							temp1->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
							XObjManager.decreaseAObject(&(temp1->text));
#endif
							temp1->text.setScale(m_scale * m_fontSize);
							temp1->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							temp1->textStr = "";
							setBoxStr("Line",i,j);

							temp1->nextBox = tempBox;
							tempRow = tempRow->nextRow;
						}else
						{
							temp1 = tempBox;
							tempBox = tempBox->nextBox;
							tempRow = tempRow->nextRow;
						}
					}
				}
			}
		}
	}else
	{//ɾ��
		int i,j;
		//ɾ����������
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= rowSum)
				{//ɾ������Ԫ�ص�����
					temp->nextRow = tempRow->nextRow;
					//XMem::XDELETE_ARRAY(tempRow->title);
					XMem::XDELETE(tempRow);
					tempRow = temp->nextRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}else
		{//ȫ��ɾ��
			tempRow = m_tableRow;
			for(i = 0;i < m_tableRowSum;++ i)
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(temp->title);
				XMem::XDELETE(temp);
			}
			m_tableRowSum = 0;
			m_tableRow = NULL;
		}
		//ɾ���������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j >= rowSum)
					{//ɾ�����ڵ�Ԫ��
						//�ͷ���Դ
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			//������ص���ֵ
			m_tableRowSum = rowSum;
		}else
		{
			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiList::initANewBoxData(XMultiListOneBox * curBox,XMultiListOneRow * curRow,int i,int j)
{
	curBox->isEnable = XTrue;

	curBox->isShow = XTrue;
	curBox->order.set(j,i);
	curBox->text.setACopy(m_caption);
	curBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&(curBox->text));
#endif
	curBox->text.setScale(m_scale * m_fontSize);
	curBox->text.setPosition(curRow->position.x,curRow->position.y + m_curTextHeight * (i + 1));
	curBox->textStr = "";
	setBoxStr("Line",i,j);
}
XBool XMultiList::setLineSum(int lineSum)	//���������������Ĳ�����հף�����ɾ��
{
	if (!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		lineSum < 0 ||			//�Ƿ�����ֱ�ӷ�Χʧ��
		m_tableRowSum <= 0) return XFalse;	//�Ƿ�����ֱ�ӷ�Χʧ��
	if (lineSum == m_tableLineSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	int i, j;
	if (m_haveSelect && m_selectLineOrder >= lineSum)
	{
		if (m_haveSelect) setAction(MLTLST_ACTION_TYPE_OUT, m_selectLineOrder);
		m_haveSelect = XFalse;
	}
	//�������ʵ������޸Ŀؼ��е�����
	if (lineSum > m_tableLineSum)
	{//���
		int oldLine = m_tableLineSum;
		m_tableLineSum = lineSum;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow;
		if (oldLine == 0)
		{//ȫ�����
			m_tableBox = XMem::createMem<XMultiListOneBox>();
			if (m_tableBox == NULL) return XFalse;

			initANewBoxData(m_tableBox, m_tableRow, 0, 0);

			tempBox = m_tableBox;
			for (i = 0; i < m_tableLineSum; ++i)
			{
				tempRow = m_tableRow;
				for (j = 0; j < m_tableRowSum; ++j)
				{
					if (j == 0 && i == 0)
					{
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
					if (tempBox->nextBox == NULL) return XFalse;

					tempBox = tempBox->nextBox;
					initANewBoxData(tempBox, tempRow, i, j);

					tempRow = tempRow->nextRow;
					//ע������û�м���ڴ����ʧ��
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}
		else
		{//���
			for (i = 0; i < m_tableLineSum; ++i)
			{
				tempRow = m_tableRow;
				for (j = 0; j < m_tableRowSum; ++j)
				{
					if (i >= oldLine || (i == oldLine - 1 && j == m_tableRowSum - 1))	//���һ��Ԫ��ҲҪ��ô����
					{
						if (i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
						{//���һ��Ԫ��
							continue;
						}
						tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
						if (tempBox->nextBox == NULL) return XFalse;

						tempBox = tempBox->nextBox;
						tempBox->isEnable = XTrue;

						tempBox->isShow = XTrue;
						tempBox->order.set(j, i);
						tempBox->text.setACopy(m_caption);
						tempBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
						XObjManager.decreaseAObject(&(tempBox->text));
#endif
						tempBox->text.setScale(m_scale * m_fontSize);
						tempBox->text.setPosition(tempRow->position.x, tempRow->position.y + m_curTextHeight * (i + 1));
						tempBox->textStr = "";
						if (j + 1 >= m_tableRowSum) setBoxStr("Line", i + 1, 0);
						else setBoxStr("Line", i, j + 1);

						tempRow = tempRow->nextRow;
						//ע������û�м���ڴ����ʧ��
					}
					else
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}
	}
	else
	{//ɾ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;
		if (lineSum == 0)
		{//ȫ��ɾ��
			for (i = 0; i < m_tableLineSum; ++i)
			{
				for (j = 0; j < m_tableRowSum; ++j)
				{//ɾ�����Ԫ��
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(temp1->string);
					XMem::XDELETE(temp1);
				}
			}
			m_tableLineSum = lineSum;
			m_tableBox = NULL;
		}
		else
		{
			for (i = 0; i < m_tableLineSum; ++i)
			{
				for (j = 0; j < m_tableRowSum; ++j)
				{
					if (i >= lineSum)
					{//ɾ�����Ԫ��
						temp1->nextBox = tempBox->nextBox;
						//XMem::XDELETE_ARRAY(tempBox->string);
						XMem::XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}
					else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			m_tableLineSum = lineSum;
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::deleteLine(int order)		//ɾ��ĳһ��
{
	if(!m_isInited ||							//���û�г�ʼ��ֱ���˳�
		order < 0 || order >= m_tableLineSum) return XFalse;		//�Ƿ�������
	int i,j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ,ɾ����һ��
		for(i = 0;i < m_tableRowSum;++ i)
		{
			temp1 = tempBox->nextBox;
			//XMem::XDELETE_ARRAY(tempBox->string);
			XMem::XDELETE(tempBox);
			tempBox = temp1;
		}
		m_tableBox = tempBox;
		//�ı���浥Ԫ���λ��
		if(m_tableLineSum > 1)
		{
			tempBox = m_tableBox;
			m_tableBox->order.reset();
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			for(i = 0;i < m_tableLineSum - 1;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i == 0 && j == 0)
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->order.set(i,j);
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{//����ɾ��Ҫ��
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				}else if(i > order)
				{//���ú��浥Ԫ���λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	--m_tableLineSum;
	if(m_haveSelect)
	{//����ѡ��
		if(m_selectLineOrder == order)
		{//����ɾ������ѡ�����һ�У���ȡ��ѡ��
			if(m_haveSelect) setAction(MLTLST_ACTION_TYPE_OUT,m_selectLineOrder);
			m_haveSelect = XFalse;
			updateSelectLine();
		}else
		if(m_selectLineOrder > order)
		{
			--m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::deleteRow(int order)		//ɾ��ĳһ��
{
	if(!m_isInited ||							//���û�г�ʼ��ֱ���˳�
		order < 0 || order >= m_tableRowSum) return XFalse;		//�Ƿ�������
	if(order == 0)
	{
		int i,j;
		//ɾ����ͷ
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		//XMem::XDELETE_ARRAY(tempRow->title);
		XMem::XDELETE(tempRow);
		if(m_tableRowSum == 1)
		{//���ֻ��һ��
			m_tableRow = NULL;
		}else
		{
			m_tableRow = temp;
			//���ñ����λ��
			m_tableRow->order = 0;
			m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
				m_mouseRect.top) * m_scale);
			m_tableRow->text.setPosition(m_tableRow->position);
			if(!m_withoutTex)
			{
				m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			for(i = 1;i < m_tableRowSum - 1;++ i)	//�Ѿ�ɾ����һ��Ԫ��
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = temp->nextRow;
			}
		}
		//ɾ�����Ԫ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ�ص�ɾ�����⴦��������������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
				//	tempRow = tempRow->nextRow;
					continue;
				}else
				if(j == order)
				{
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
				//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						if(m_tableRowSum > 1)
						{
							tempBox->order.set(j,i);
							tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
							tempRow = tempRow->nextRow;
						}
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//ɾ����һ��Ԫ��
		tempBox = m_tableBox;
		temp1 = tempBox->nextBox;
		//XMem::XDELETE_ARRAY(tempBox->string);
		XMem::XDELETE(tempBox);
		if(m_tableRowSum == 1)
		{//���ֻ��һ�У���ɾ��֮��û�е�λ���
			m_tableBox = NULL;
			m_tableLineSum = 0;
		}else
		{
			assert(temp1 != NULL);
			tempBox = temp1;
			m_tableBox = tempBox;
			m_tableBox->order.reset();
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 0);
		}
	}else
	{
		int i,j;
		//ɾ����ͷ
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneRow *temp = tempRow->nextRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order)
			{
				temp->nextRow = tempRow->nextRow;
				//XMem::XDELETE_ARRAY(tempRow->title);
				XMem::XDELETE(tempRow);
				tempRow = temp->nextRow;
				if(tempRow != NULL)
				{
					tempRow->order = i;
					tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
						m_position.y + m_mouseRect.top * m_scale.y);
					tempRow->text.setPosition(temp->nextRow->position);
					if(!m_withoutTex)
					{
						tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
							m_tableRow->position.y);
					}
				}
			}else
			if(i > order)
			{
				tempRow->order = i;
				tempRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				tempRow->text.setPosition(temp->nextRow->position);
				if(!m_withoutTex)
				{
					tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//ɾ�����Ԫ��
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order)
				{
					assert(temp1 != NULL);
					temp1->nextBox = tempBox->nextBox;
					//XMem::XDELETE_ARRAY(tempBox->string);
					XMem::XDELETE(tempBox);
					tempBox = temp1->nextBox;
					//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						tempBox->order.set(j,i);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
		}
	}
	m_tableRowSum--;
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::insertALine(int order)		//��order������һ��
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum) return XFalse;
	int i,j;
	if(order == 0)
	{//��ͷ������
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *temp1 = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.reset();
		m_tableBox->text.setACopy(m_caption);
		m_tableBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line",0,0);
		tempBox = m_tableBox;
		for(i = 0;i < m_tableRowSum - 1;++ i)
		{
			tempBox->nextBox = XMem::createMem<XMultiListOneBox>();
			if(tempBox->nextBox == NULL) return XFalse;

			tempBox->nextBox->isEnable = XTrue;
			tempBox->nextBox->isShow = XTrue;
			tempBox->nextBox->order.set(i,0);
			tempBox->nextBox->text.setACopy(m_caption);
			tempBox->nextBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&(tempBox->nextBox->text));
#endif
			tempBox->nextBox->text.setScale(m_scale * m_fontSize);
			tempBox->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * 1);
			tempBox->nextBox->textStr = "";
			setBoxStr("Line",0,i + 1);
			tempBox = tempBox->nextBox;
			tempRow = tempRow->nextRow;
		}
		tempBox->nextBox = temp1;
		//�����������Ԫ�������ƶ�
		tempBox = m_tableBox;

		++ m_tableLineSum;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
					continue;
				}
				tempBox->order.set(j,i);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
				tempBox = tempBox->nextBox;
				tempRow = tempRow->nextRow;
			}
		}

		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				m_selectLineOrder ++;
				updateSelectLine();
			}
		}
	}else
	{
		XMultiListOneRow *tempRow = m_tableRow;
		XMultiListOneBox *tempBox = m_tableBox;
		XMultiListOneBox *temp1 = NULL;

		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if(temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
					temp1->nextBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
				//	if(j == m_tableRowSum - 1)
				//	{//���һ����ӵ�Ԫ��
				//		temp1->nextBox = tempBox;
				//	}
					tempRow = tempRow->nextRow;
				}else if(i > order)
				{//�ı�λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}
			}
		}
		++ m_tableLineSum;
		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				++ m_selectLineOrder;
				updateSelectLine();
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::insertARow(int order)		//��order������һ��
{
	if (!m_isInited) return XFalse;
	if (order < 0 || order >= m_tableRowSum) return XFalse;
	if (m_tableRowSum >= MUTILIST_MAX_ROW_SUM) return XFalse;	//�����е����ֵ�����˳�
	int i, j;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp = NULL;
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	if (order == 0)
	{//��ͷ
		//�ı������
		tempRow = m_tableRow;
		m_tableRow = XMem::createMem<XMultiListOneRow>();
		if (m_tableRow == NULL) return XFalse;

		initANewRowData(NULL, 0);
		/*		m_tableRow->isEnable = 1;

				m_tableRow->order = 0;
				m_tableRow->text.setACopy(m_caption);
				m_tableRow->text.setScale(m_scale * m_fontSize);
				m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
				m_tableRow->position.set(m_objRect.left + m_edgeDistance.left * m_scale.x + MUTILIST_MIN_WIDTH * m_scale.x * 0,
					m_objRect.top + m_edgeDistance.top * m_scale.y);
				m_tableRow->text.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
				m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
				m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
				m_tableRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
				m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
				m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
				m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

				m_tableRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
				m_tableRow->title = NULL;
				setTileStr("Title",0);	//����Ĭ����ʾ
				m_tableRow->isShow = 1;*/

		m_tableRow->nextRow = tempRow;
		tempRow = m_tableRow;
		for (i = 0; i < m_tableRowSum; ++i)
		{
			tempRow->nextRow->order = i + 1;
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			if (!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x -
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı�������
		++m_tableRowSum;
		//�����һ��Ԫ��
		tempBox = m_tableBox;
		m_tableBox = XMem::createMem<XMultiListOneBox>();
		if (m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = XTrue;
		m_tableBox->order.reset();
		m_tableBox->text.setACopy(m_caption);
		m_tableBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setScale(m_scale * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x, m_tableRow->position.y + m_curTextHeight * 1);
		m_tableBox->textStr = "";
		setBoxStr("Line", 0, 0);
		m_tableBox->nextBox = tempBox;
		tempBox = m_tableBox;
		for (i = 0; i < m_tableLineSum; ++i)
		{
			tempRow = m_tableRow;
			for (j = 0; j < m_tableRowSum; j++)
			{
				if (i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
				else if (j == order)
				{//����
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if (temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j, i);
					temp1->nextBox->text.setACopy(m_caption);
					temp1->nextBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x, tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line", i, j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}
				else if (j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j, i);
					temp1->nextBox->text.setPosition(tempRow->position.x, tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
				else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	else
	{
		//�ı������
		tempRow = m_tableRow;
		for (i = 0; i < m_tableRowSum + 1; ++i)
		{
			if (i == order)
			{//����
				temp->nextRow = XMem::createMem<XMultiListOneRow>();
				if (temp->nextRow == NULL) return XFalse;

				initANewRowData(temp, i);
				/*	temp->nextRow->isEnable = 1;
					temp->nextRow->order = i;
					temp->nextRow->text.setACopy(m_caption);
					temp->nextRow->pixSize.set(MUTILIST_MIN_WIDTH * m_scale.x,m_tableRow->text.m_scale.y * m_tableRow->text.m_showSize.x);
					temp->nextRow->position.set(temp->position.x + temp->pixSize.x,
						m_objRect.top + m_edgeDistance.top * m_scale.y);
					temp->nextRow->text.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
					temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitle.setScale(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
					temp->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
					temp->nextRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
					temp->nextRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
					temp->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

					temp->nextRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_scale.x * m_tableRow->text.m_showSize.x * 0.5);
					temp->nextRow->title = NULL;
					setTileStr("Title",i);	//����Ĭ����ʾ
					temp->nextRow->isShow = 1;*/

				temp = temp->nextRow;
				temp->nextRow = tempRow;
			}
			else if (i > order)
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_scale.x,
					m_position.y + m_mouseRect.top * m_scale.y);
				temp->nextRow->text.setPosition(temp->nextRow->position);
				if (!m_withoutTex)
				{
					temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_scale.x -
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
			else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//�ı�������
		++m_tableRowSum;
		tempBox = m_tableBox;
		for (i = 0; i < m_tableLineSum; ++i)
		{
			tempRow = m_tableRow;
			for (j = 0; j < m_tableRowSum; j++)
			{
				if (i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
				else if (j == order)
				{//����
					temp1->nextBox = XMem::createMem<XMultiListOneBox>();
					if (temp1->nextBox == NULL) return XFalse;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = XTrue;
					temp1->nextBox->order.set(j, i);
					temp1->nextBox->text.setACopy(m_caption);
					temp1->nextBox->text.setAlpha(m_color.a);
#if WITH_OBJECT_MANAGER
					XObjManager.decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setScale(m_scale * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x, tempRow->position.y + m_curTextHeight * (i + 1));
					temp1->nextBox->textStr = "";
					setBoxStr("Line", i, j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}
				else if (j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j, i);
					temp1->nextBox->text.setPosition(tempRow->position.x, tempRow->position.y + m_curTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
				else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::moveDownLine(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	//���濪ʼ�ƶ�
	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp1 = NULL;
	XMultiListOneBox temp2;
	//2�е�6���˵�Ľڵ�
	XMultiListOneBox * point0 = NULL;
	XMultiListOneBox * point1 = NULL;
	XMultiListOneBox * point2 = NULL;
	XMultiListOneBox * point3 = NULL;
	XMultiListOneBox * point4 = NULL;
	XMultiListOneBox * point5 = NULL;
	int i,j;
	if(order == 0)
	{//�ƶ����ǵ�һ��
		temp1 = m_tableBox;
		point1 = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		//point0->nextBox = point3;
		assert(point2 != NULL);
		point2->nextBox = point5;
		assert(point4 != NULL);
		point4->nextBox = point1;
		m_tableBox = point3;
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order - 1 && j == m_tableRowSum - 1)
				{
					point0 = tempBox;
					point1 = tempBox->nextBox;
				}
				if(i == order)
				{
					temp1 = tempBox;
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		assert(point0 != NULL);
		assert(point2 != NULL);
		assert(point4 != NULL);
		point0->nextBox = point3;
		point2->nextBox = point5;
		point4->nextBox = point1;
	}
	if(m_haveSelect)
	{
		if(m_selectLineOrder == order)
		{
			++ m_selectLineOrder;
			updateSelectLine();
		}else
		if(m_selectLineOrder == order + 1)
		{
			-- m_selectLineOrder;
			updateSelectLine();
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
XBool XMultiList::moveRightRow(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	int i = 0,j = 0;
	XMultiListOneRow *tempRow = m_tableRow;
	XMultiListOneRow *temp1 = NULL;
	XMultiListOneRow *point0 = NULL;
	XMultiListOneRow *point1 = NULL;

	XMultiListOneBox *tempBox = m_tableBox;
	XMultiListOneBox *temp2 = NULL;
	XMultiListOneBox *point2 = NULL;
	XMultiListOneBox *point3 = NULL;
	if(order == 0)
	{
		//�ƶ���ͷ
	//	point0 = tempRow;
	//	tempRow = tempRow->nextRow;
		temp1 = tempRow->nextRow;
		point1 = temp1->nextRow;

	//	point0->nextRow = temp1;
		m_tableRow = temp1;
		temp1->order = tempRow->order;
		temp1->position = tempRow->position;
		temp1->text.setPosition(temp1->position);

		temp1->nextRow = tempRow;
		temp1->nextRow->order = temp1->order + 1;
		temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
		temp1->nextRow->text.setPosition(temp1->nextRow->position);
		if(!m_withoutTex)
		{
			temp1->m_spriteTitle.setPosition(temp1->position);
			temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
			temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
			temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
				temp1->nextRow->position.y);
		}
		temp1->nextRow->nextRow = point1;

		//�ƶ�����
		if(m_tableLineSum > 1)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						tempBox = tempBox->nextBox;
						temp2 = tempBox->nextBox;
						point3 = temp2->nextBox;

						point2->nextBox = temp2;
						temp2->order = tempBox->order;
						temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox = tempBox;
						if(j == m_tableRowSum -1)
						{
							tempRow = m_tableRow;
							temp2->nextBox->order.set(0,temp2->order.y);
							j = 0;
							++ i;
						}else
						{//��������ǲ�����ֵ�
							tempRow = tempRow->nextRow;
							temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
							++ j;
						}
						temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 2));
						temp2->nextBox->nextBox = point3;
						if(i >= m_tableLineSum - 1)
						{
							break;
						}
					}else
					{
						tempRow = tempRow->nextRow;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//����һ��Ԫ��
	//	point2 = tempBox;
		tempBox = m_tableBox;
		temp2 = tempBox->nextBox;
		point3 = temp2->nextBox;

		m_tableBox = temp2;
		temp2->order = tempBox->order;
		temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox = tempBox;

		tempRow = tempRow->nextRow;
		temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);

		temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
		temp2->nextBox->nextBox = point3;
	}else
	{
		//�ƶ���ͷ
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order - 1)
			{
				point0 = tempRow;
				tempRow = tempRow->nextRow;
				temp1 = tempRow->nextRow;
				point1 = temp1->nextRow;

				point0->nextRow = temp1;
				temp1->order = tempRow->order;
				temp1->position = tempRow->position;
				temp1->text.setPosition(temp1->position);
				temp1->nextRow = tempRow;
				temp1->nextRow->order = temp1->order + 1;
				temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_scale.x,m_position.y + m_mouseRect.top * m_scale.y);			
				temp1->nextRow->text.setPosition(temp1->nextRow->position);
				if(!m_withoutTex)
				{
					temp1->m_spriteTitle.setPosition(temp1->position);
					temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,temp1->position.y);
					temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
					temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_scale.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
						temp1->nextRow->position.y);
				}
				temp1->nextRow->nextRow = point1;
				break;
			}
			tempRow = tempRow->nextRow;
		}
		//�ƶ�����
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order - 1)
				{
					point2 = tempBox;
					tempBox = tempBox->nextBox;
					temp2 = tempBox->nextBox;
					point3 = temp2->nextBox;

					point2->nextBox = temp2;
					temp2->order = tempBox->order;
					temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox = tempBox;
					if(j == m_tableRowSum -1)
					{//��������ǲ�����ֵ�
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(0,temp2->order.y);
						j = 0;
						++ i;
					}else
					{
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
						++ j;
					}
					temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					temp2->nextBox->nextBox = point3;
					if(i >= m_tableLineSum - 1)
					{
						break;
					}
				}else
				{
					tempRow = tempRow->nextRow;
					tempBox = tempBox->nextBox;
				}
			}
		}
	}
	updateSliderState();
	updateShowChange();
	return XTrue;
}
void XMultiList::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		m_tableRow->text.setPosition(m_tableRow->position);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;
		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_scale.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_scale.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	//updateSliderState();
	updateShowChange();	//������������������ƶ���ʱ���bug
}
void XMultiList::setScale(const XVec2& s)	//���óߴ�
{
	if(!m_isInited ||
		s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	if(!m_withoutTex)
	{
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setScale(m_scale);
		m_spriteMove.setScale(m_scale);
	}
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + m_mouseRect.getRB() * m_scale);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_caption.setScale(m_scale * m_fontSize);
	m_curTextWidth = m_caption.getTextSize().x * m_caption.getScale().x * 0.5f;
	m_curTextHeight = m_caption.getTextSize().y * m_caption.getScale().y;
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x;
	if(!m_withoutTex)
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_scale.y;
		m_spriteSelect.setScale((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_curTextHeight / m_mutiListSelect->textureSize.y);
	}else
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_scale.y;
	}
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position + XVec2(m_mouseRect.left + MUTILIST_MIN_WIDTH * 0,
			m_mouseRect.top) * m_scale);
		m_tableRow->text.setPosition(m_tableRow->position);
		m_tableRow->text.setScale(m_scale * m_fontSize);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setScale(m_tableRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setScale(m_tableRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
		}
		XMultiListOneRow *tempRow = m_tableRow;

		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->text.setScale(m_scale * m_fontSize);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitle.setScale(tempRow->nextRow->pixSize.x * m_scale.x / m_mutiListTitle->textureSize.x,m_scale.y);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->nextRow->m_spriteTitleEnd.setScale(tempRow->nextRow->pixSize.y * m_scale.y/ m_mutiListTitle->textureSize.y,
					tempRow->nextRow->pixSize.y * m_scale.y/m_mutiListTitle->textureSize.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_curTextHeight * 1);
			m_tableBox->text.setScale(m_scale * m_fontSize);

			XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_curTextHeight * (i + 1));
					tempBox->text.setScale(m_scale * m_fontSize);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	updateSliderState();
	updateShowChange();
}
XBool XMultiList::keyboardProc(int keyOrder, XKeyState keyState)
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//�����Ч��ֱ���˳�
	if (m_isSilent) return XFalse;
	if (keyState == KEY_STATE_UP)
	{
		switch (keyOrder)
		{
		case XKEY_UP:
			if (m_haveSelect)
			{//����Ѿ�ѡ��
				if (m_selectLineOrder > 0)
					--m_selectLineOrder;
			}
			else
			{//�����δѡ��
				if (!m_haveSelect) setAction(MLTLST_ACTION_TYPE_IN, 0); else
					if (m_selectLineOrder != 0) setAction(MLTLST_ACTION_TYPE_MOVE, m_selectLineOrder);
				m_selectLineOrder = 0;
				m_haveSelect = XTrue;
			}
			//���ѡ���Ƿ�����ʾ��Χ��(��δ���)
			if (m_selectLineOrder < m_showStartLine ||
				m_selectLineOrder >= m_showStartLine + m_canShowLineSum)
			{//����ʾ��Χ��
				if (m_selectLineOrder <= m_tableLineSum - m_canShowLineSum)
					m_verticalSlider.setCurValue(m_selectLineOrder, true);
				else m_verticalSlider.setCurValue(m_tableLineSum - m_canShowLineSum, true);
			}
			updateSelectLine();
			updateSliderState();
			updateShowChange();
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_SELECT);
			else XCtrlManager.eventProc(m_objectID, MLTLST_SELECT);
			break;
		case XKEY_DOWN:
			if (m_haveSelect)
			{
				if (m_selectLineOrder < m_tableLineSum - 1)
					++m_selectLineOrder;
			}
			else
			{
				if (!m_haveSelect) setAction(MLTLST_ACTION_TYPE_IN, 0); else
					if (m_selectLineOrder != 0) setAction(MLTLST_ACTION_TYPE_MOVE, m_selectLineOrder);
				m_selectLineOrder = 0;
				m_haveSelect = XTrue;
			}
			//���ѡ���Ƿ�����ʾ��Χ��(��δ���)
			if (m_selectLineOrder < m_showStartLine ||
				m_selectLineOrder >= m_showStartLine + m_canShowLineSum)
			{//����ʾ��Χ��
				if (m_selectLineOrder - m_canShowLineSum <= m_tableLineSum - m_canShowLineSum)
					m_verticalSlider.setCurValue(m_selectLineOrder - m_canShowLineSum + 1, true);
				else m_verticalSlider.setCurValue(m_tableLineSum - m_canShowLineSum, true);
			}
			updateSelectLine();
			updateSliderState();
			updateShowChange();
			if (m_eventProc != NULL) m_eventProc(m_pClass, m_objectID, MLTLST_SELECT);
			else XCtrlManager.eventProc(m_objectID, MLTLST_SELECT);
			break;
		case XKEY_LEFT:		//����ѡ��(��δ���)
			if (m_needShowHSlider)
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_LEFT, KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_RIGHT:	//����ѡ��(��δ���)
			if (m_needShowHSlider)
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_RIGHT, KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		}
	}
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMultiList.inl"
#endif
}