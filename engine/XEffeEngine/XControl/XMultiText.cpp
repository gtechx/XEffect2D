#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
namespace XE{
void XMultiText::ctrlProc(void*pClass,int id,int eventID)
{
	XMultiText &pPar = *(XMultiText *)pClass;
	if(id == pPar.m_verticalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
		//	printf("V ID:%d\n",pPar.m_objectID);
		//	printf("V Cur value:%f\n",pPar.m_verticalSlider.getCurValue());
			//��Ӧ��ֵ�ĸı�
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				//��Ҫ���´�ֱƬѡ�����
				pPar.setStartLine(temp);
				pPar.updateLineStr();
			}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
		//	printf("V ID:%d\n",pPar.m_objectID);
		//	printf("V Cur value:%f\n",pPar.m_verticalSlider.getCurValue());
			//��Ӧ��ֵ�ĸı�
			int temp = pPar.m_verticalSlider.getCurValue();
			if(temp != pPar.m_showStartLine)
			{
				pPar.setStartLine(temp);
				pPar.updateLineStr();
				pPar.m_verticalSlider.setCurValue(temp);
			}else
			{//΢���ƶ�ʱ��Ĵ���
				if(pPar.m_verticalSlider.getCurValue() > pPar.m_showStartLine)
				{
					++ temp;
					pPar.setStartLine(temp);
					pPar.updateLineStr();
					pPar.m_verticalSlider.setCurValue(temp);
				}else
				if(pPar.m_verticalSlider.getCurValue() < pPar.m_showStartLine)
				{
					-- temp;
					pPar.setStartLine(temp);
					pPar.updateLineStr();
					pPar.m_verticalSlider.setCurValue(temp);
				}
			}
		}
		return;
	}
	if(id == pPar.m_horizontalSlider.getControlID())
	{
		if(eventID == XSlider::SLD_MOUSE_MOVE)
		{
		//	printf("H ID:%d\n",pPar.m_objectID);
		//	printf("H Cur value:%f\n",pPar.m_horizontalSlider.getCurValue());
			//��Ӧ��ֵ�ĸı�
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_curStartPixels)
			{
				pPar.m_curStartPixels = temp;
				pPar.updateLineStr();
			}
			//if(temp != pPar.m_lineStr[0].showStart)
			//{
			//	for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//	{
			//		pPar.m_lineStr[i].showStart = temp;
			//	}
			//	pPar.updateLineStr();
			//}
		}else
		if(eventID == XSlider::SLD_VALUE_CHANGE)
		{
		//	printf("H ID:%d\n",pPar.m_objectID);
		//	printf("H Cur value:%f\n",pPar.m_horizontalSlider.getCurValue());
			//��Ӧ��ֵ�ĸı�
			int temp = pPar.m_horizontalSlider.getCurValue();
			if(temp != pPar.m_curStartPixels)
			{
				//printf("%d\n",temp);
				pPar.m_curStartPixels = temp;
				pPar.updateLineStr();
				pPar.m_horizontalSlider.setCurValue(temp);
			}
			//if(temp != pPar.m_lineStr[0].showStart)
			//{
			//	for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//	{
			//		pPar.m_lineStr[i].showStart = temp;
			//	}
			//	pPar.updateLineStr();
			//	pPar.m_horizontalSlider.setCurValue(temp);
			//}else
			//{
			//	if(pPar.m_horizontalSlider.getCurValue() > pPar.m_lineStr[0].showStart)
			//	{
			//		++ temp;
			//		for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//		{
			//			pPar.m_lineStr[i].showStart = temp;
			//		}
			//		pPar.updateLineStr();
			//		pPar.m_horizontalSlider.setCurValue(temp);
			//	}else
			//	if(pPar.m_horizontalSlider.getCurValue() < pPar.m_lineStr[0].showStart)
			//	{
			//		-- temp;
			//		for(int i = 0;i < pPar.m_canShowLineSum;++ i)
			//		{
			//			pPar.m_lineStr[i].showStart = temp;
			//		}
			//		pPar.updateLineStr();
			//		pPar.m_horizontalSlider.setCurValue(temp);
			//	}
			//}
		}
		return;
	}
}
XMultiText::XMultiText()
	:m_isInited(XFalse)
	, m_multiEditNormal(NULL)			//�������ͨ״̬
	, m_multiEditDisable(NULL)		//�������Ч״̬
	, m_multiEditSelect(NULL)			//�����Ƭѡ��ɫ
	//,m_funInputChenge(NULL)			//�������ݷ����ı��ʱ�����
	//,m_funInputOver(NULL)			//ȷ���������֮���ʱ�����
	, m_lineStr(NULL)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	//,m_funSelect(NULL)
	//,m_pClass(NULL)
	, m_curMaxLineSum(0)
{
	m_ctrlType = CTRL_OBJ_MUTITEXT;
}
void XMultiText::release()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	XMem::XDELETE_ARRAY(m_lineStr);
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		XMem::XDELETE(*it);
	}
	m_curStr.clear();
	for (auto it = m_freeStr.begin(); it != m_freeStr.end(); ++it)
	{
		XMem::XDELETE(*it);
	}
	m_freeStr.clear();
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
void XMultiText::setStartLine(int temp)
{
	if(m_haveSelect && m_selectLineOrder >= m_showStartLine
		&& m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//�ظ�ԭ�е�����
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
	}
	m_showStartLine = temp;
	if(m_haveSelect && 
		(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_canShowLineSum + m_showStartLine))
	{//�޸��µ�����
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
	}
}
XBool XMultiText::init(const XVec2& position,	//�ؼ����ڵ�λ��
		const XRect& area,	//�ؼ��ķ�Χ
		const XMultiTextSkin& tex,	//�ؼ�����ͼ
		const char *str,const XFontUnicode& font,float strSize,		//�ؼ������弰�����Ϣ
		//const XVec2& strPosition,
		//const XMouseRightButtonMenu &mouseMenu,
		const XSlider &vSlider,const XSlider &hSlider)	//�ؼ����������������ؼ�
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(area.getWidth() <= 0 || area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = area;
	m_withoutTex = XFalse;

	m_multiEditNormal = tex.editNormal;			//�������ͨ״̬
	m_multiEditDisable = tex.editDisable;		//�������Ч״̬
	m_multiEditSelect = tex.editSelect;			//�����Ƭѡ��ɫ
	m_multiEditUpon = tex.editUpon;
	m_scale.set(1.0f);

	//m_textPosition = strPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_spriteBackGround.init(m_multiEditNormal->texture.m_w,m_multiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_verticalSlider.setACopy(vSlider);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

	m_horizontalSlider.setACopy(hSlider);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//���㵱ǰ������ʾ������
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_multiEditSelect->texture.m_w,m_multiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		//m_lineStr[i].showString = "";
	}
	//Ϊ�����ַ��������ڴ�ռ�
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
XBool XMultiText::initPlus(const char *path,
		const char *str,const XFontUnicode& font, float strSize,
		XResPos resPos)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	m_resInfo = XResManager.loadResource(path,RESOURCE_TYPEXEDIT_TEX,resPos);
	if(m_resInfo == NULL) return XFalse;
	XEditSkin * tex = (XEditSkin *)m_resInfo->m_pointer;
	if(tex->editNormal == NULL || tex->editInsert == NULL || tex->editDisable == NULL || tex->editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position.reset();
	m_mouseRect = tex->m_mouseRect;
	m_withoutTex = XFalse;

	m_multiEditNormal = tex->editNormal;			//�������ͨ״̬
	m_multiEditDisable = tex->editDisable;		//�������Ч״̬
	m_multiEditSelect = tex->editSelect;			//�����Ƭѡ��ɫ
	m_multiEditUpon = tex->editUpon;
	m_scale.set(1.0f);

	//m_textPosition = strPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_spriteBackGround.init(m_multiEditNormal->texture.m_w,m_multiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setScale(m_scale);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	//m_verticalSlider.setACopy(vSlider);
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resPos);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

	//m_horizontalSlider.setACopy(hSlider);
	sprintf_s(tempPath,MAX_FILE_NAME_LENGTH,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resPos);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//���㵱ǰ������ʾ������
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_multiEditSelect->texture.m_w,m_multiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		//m_lineStr[i].showString = "";
	}
	//Ϊ�����ַ��������ڴ�ռ�
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
XBool XMultiText::initWithoutSkin(const XRect& area,
		const char *str,const XFontUnicode& font, float strSize)
{
	if(m_isInited ||	//��ֹ�ظ���ʼ��
		//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
		strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
//	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position.reset();
	m_mouseRect = area;
	m_withoutTex = XTrue;

	m_scale.set(1.0f);
	m_textSize.set(strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

//	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,XVec2::zero);
	m_verticalSlider.initWithoutSkin(XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
		m_position.y + m_mouseRect.top * m_scale.y);
	m_verticalSlider.setScale(m_scale);
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_verticalSlider.setWithAction(XFalse);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

//	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		XRect(0,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,XVec2::zero);
	m_horizontalSlider.initWithoutSkin(XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_HORIZONTAL);
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_scale.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale.y);
	m_horizontalSlider.setScale(m_scale);
	m_horizontalSlider.setEventProc(ctrlProc,this);
	m_horizontalSlider.setWithAction(XFalse);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getScale().x * 0.5f;
	m_curTextHeight = font.getTextSize().y * font.getScale().y;
	//���㵱ǰ������ʾ������
	m_canShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_curTextHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_curShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_curStartPixels = 0;
	m_curMaxLineWidthPixels = 0;
	m_maxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() - 
			XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.set(0.0f,1.0f);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		if(!m_lineStr[i].curText.setACopy(font)) return XFalse;
		m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
		m_lineStr[i].curText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].curText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].curText.setColor(m_textColor);
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	//	m_lineStr[i].showLength = m_curShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		//m_lineStr[i].showString = "";
	}
	//Ϊ�����ַ��������ڴ�ռ�
	std::string *tmpStr = NULL;
	for(int i = 0;i < MAX_LINE_SUM;++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL)
		{
			XMem::XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_freeStr.push_back(tmpStr);
	}

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectLineOrder = -1;

	m_isVisible = m_isEnable = m_isActive = XTrue;

	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif
	m_isInited = XTrue;
	setString(str);
	return XTrue;
}
void XMultiText::updateLineStr()
{//��ʵ�ʵ��ַ������䵽����ȥ
	//if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	//�����������������޸�
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].curText.disClip();
	}
//	if(m_showStartLine == 0) m_lineStr[0].lineHeadorder = 0;//����Ǵ��ַ�����ͷ��ʾ������Ҫ��ʼ����һ�е�ͷλ�ַ�����ʼ

	int maxlineWidth = 0;
	//��е�ķ���
	int i = 0;
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it, ++ i)
	{
		if(i >= m_showStartLine && i < m_showStartLine + m_canShowLineSum)
		{//��Ҫ��ʾ����;
			m_lineStr[i - m_showStartLine].isEnable = XTrue;
			m_lineStr[i - m_showStartLine].curText.setString((*it)->c_str());
			m_lineStr[i - m_showStartLine].lineWidthPixel = (*it)->length() * m_lineStr[i - m_showStartLine].curText.getTextSize().x * 0.5f;
			if(m_lineStr[i - m_showStartLine].lineWidthPixel > maxlineWidth)
			{
				maxlineWidth = m_lineStr[i - m_showStartLine].lineWidthPixel;
			}
			m_lineStr[i - m_showStartLine].curText.setClipRect(m_curStartPixels,0.0f,m_maxLineWidthPixels + m_curStartPixels,
				m_lineStr[i - m_showStartLine].curText.getTextSize().y);//�����ַ����ü�
			//����λ��Ҳ��Ҫ����Ӧ�ĸı�
			//����δ��ɣ�
			m_lineStr[i - m_showStartLine].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
				m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * (i - m_showStartLine));

//			m_lineStr[i - m_showStartLine].lineHeadorder = index;
//			m_lineStr[i - m_showStartLine].lineEndOrder = i;
		}else
		{//����Ҫ��ʾ����
			int w = (*it)->length() * m_lineStr[0].curText.getTextSize().x * 0.5f;
			if(maxlineWidth < w) maxlineWidth = w;
		}
	}
	m_curMaxLineSum = m_curStr.size();
	if(m_curMaxLineSum > m_canShowLineSum)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowVSlider = XTrue;
		m_verticalSlider.setCurValue(m_showStartLine);
		m_verticalSlider.setRange(m_curMaxLineSum - m_canShowLineSum,0.0f);
	}else
	{
		m_needShowVSlider = XFalse;
	}
	//�ж��Ƿ���Ҫ��ʾˮƽ�϶���
	m_curMaxLineWidthPixels = maxlineWidth;
	if(m_curMaxLineWidthPixels > m_maxLineWidthPixels)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowHSlider = XTrue;
		m_horizontalSlider.setCurValue(m_curStartPixels);
		m_horizontalSlider.setRange(m_curMaxLineWidthPixels - m_maxLineWidthPixels,0.0f);
	}else
	{
		m_needShowHSlider = XFalse;
		if(m_curStartPixels != 0)
		{//����ˢ��һ����ʾ���Ա��ڱ�֤��ʾ����ȷ��
			m_curStartPixels = 0;
			m_horizontalSlider.setCurValue(m_curStartPixels);
			updateLineStr();
		}else
		{
			m_curStartPixels = 0;
			m_horizontalSlider.setCurValue(m_curStartPixels);
		}
	}
}
void XMultiText::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			XRender::drawFillRectA(m_position + m_mouseRect.getLT() * m_scale,
				(m_mouseRect.getSize() - XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale, 
				XCCS::specialColor * m_color,true);
		}else 
		{
			XRender::drawFillRectA(m_position + m_mouseRect.getLT() * m_scale,
				(m_mouseRect.getSize() - XVec2(m_verticalSlider.getMouseRectWidth(),m_horizontalSlider.getMouseRectHeight())) * m_scale, 
				XCCS::normalColor * m_color,true);
		}
		//if(m_haveSelect) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//��ʾѡ��ķ�Χ
		if(m_actionMD.getIsEnd())
		{
			if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
			{//������ʾѡ��ı�ǩ
				int index = m_selectLineOrder - m_showStartLine;
				XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y + index * m_curTextHeight),
					XVec2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
					m_curTextHeight),XCCS::onColor * m_color);
			}
		}else
		{
			switch(m_actionType)
			{
			case MLTTXT_ACTION_TYPE_IN:		//ѡ�����
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x, m_mouseRect.top * m_scale.y +
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::onColor * m_color);
				}
				break;
			case MLTTXT_ACTION_TYPE_MOVE:	//ѡ���ƶ�
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{//�������
					float pos = XMath::lineSlerp<float>((m_actionPosition - m_showStartLine) * m_curTextHeight,
						(m_selectLineOrder - m_showStartLine) * m_curTextHeight,m_actionMD.getCurData());
					XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0,pos),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight),
						XCCS::onColor * m_color);
				}else
				if(m_actionPosition < m_showStartLine)
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
						XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0, pos),
							XVec2(m_maxLineWidthPixels * m_scale.x,h),
							XCCS::onColor * m_color);
					}
				}else
				if(m_actionPosition >= m_showStartLine + m_canShowLineSum)
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
						XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0.0f, pos),
							XVec2(m_maxLineWidthPixels * m_scale.x,h),
							XCCS::onColor * m_color);
					}
				}
				break;
			case MLTTXT_ACTION_TYPE_DCLICK:	//˫��
			case MLTTXT_ACTION_TYPE_OUT:	//ȡ��ѡ��
				if(m_actionPosition >= m_showStartLine && m_actionPosition < m_showStartLine + m_canShowLineSum)
				{
					XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale + XVec2(0.0f,
						(m_actionPosition - m_showStartLine) * m_curTextHeight + m_curTextHeight * (1.0f - m_actionMD.getCurData()) * 0.5f),
						XVec2(m_maxLineWidthPixels * m_scale.x,m_curTextHeight * m_actionMD.getCurData()),
						XCCS::onColor * m_color);
				}
				break;
			}
		}
		//��ʾ��ǰ������ַ���
		for(int i = 0;i < m_canShowLineSum;++ i)
		{
			if(m_lineStr[i].isEnable)
			{
				//if(m_lineStr[i].haveAllSelect)
				//{//��ʾ��һ�б�ѡ��
				//	drawFillRectExA(m_position + XVec2(m_mouseRect.left * m_scale.x,m_mouseRect.top * m_scale.y + i * m_curTextHeight),
				//		XVec2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_scale.x,
				//		m_curTextHeight),0.85f * m_color.r,0.85f * m_color.g,0.85f * m_color.b,m_color.a);
				//}
				m_lineStr[i].curText.draw();
			}
		}
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::downColor * m_color, true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::downColor * m_color, true);
		}else 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightSpecialColor * m_color, true);
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightSpecialColor * m_color, true);
		}
		//��ʾ������
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		//if(m_mutiEditUpon != NULL) m_spriteBackGround.draw(m_mutiEditUpon);
		if(!m_isEnable) 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::blackDownColor * m_color,true);
			//m_spriteBackGround.draw(m_mutiEditDisable);	//�����Ч����ʾ��Ч,����ʾ�������
		}else 
		{
			XRender::drawFillRectExA(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(),
				m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight()) * m_scale,
				XCCS::lightMouseColor * m_color, true);
			//m_spriteBackGround.draw(m_mutiEditNormal); //�����Ч����ʾ��Ч,��ʾ�������
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_multiEditDisable);	//�����Ч����ʾ��Ч,����ʾ�������
		else m_spriteBackGround.draw(m_multiEditNormal); //�����Ч����ʾ��Ч,��ʾ�������
		//if(m_haveSelect) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//��ʾѡ��ķ�Χ
		//��ʾ��ǰ������ַ���
		for(int i = 0;i < m_canShowLineSum;++ i)
		{
			if (!m_lineStr[i].isEnable) continue;
			if(m_lineStr[i].haveAllSelect)
				m_lineStr[i].spriteSelect.draw(m_multiEditSelect);
			m_lineStr[i].curText.draw();
		}
		//��ʾ������
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		if(m_multiEditUpon != NULL) m_spriteBackGround.draw(m_multiEditUpon);
	}
}
XBool XMultiText::mouseProc(const XVec2& p,XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_isSilent) return XFalse;
	
	if(m_needShowVSlider) 
	{
		m_verticalSlider.mouseProc(p,mouseState);
		if(m_curMouseRect.isInRect(p))
		{//������Ӧ�����¼�
			switch(mouseState)
			{
			case MOUSE_WHEEL_UP_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() - 1.0f);
				break;
			case MOUSE_WHEEL_DOWN_DOWN:
				m_verticalSlider.setCurValue(m_verticalSlider.getCurValue() + 1.0f);
				break;
			}
		}
	}
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(p,mouseState);
	//�ж�������Ƿ��ڷ�Χ��
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_curMouseRect.isInRect(p))
	{//��ѡ��Ч�������ѡ����ֵ
		int tmp = (p.y - m_curMouseRect.top) / m_curTextHeight;	//���㵱ǰѡ�������
		tmp = XMath::clamp(tmp,0, m_canShowLineSum - 1);
		if(tmp + m_showStartLine < m_curMaxLineSum)
		{
			if(m_haveSelect && m_selectLineOrder != tmp + m_showStartLine)
			{//��������Ҫ�޳�ԭ�е�ѡ������ֻ����һ������ѡ��״̬
				if(m_selectLineOrder - m_showStartLine >= 0 && m_selectLineOrder - m_showStartLine < m_canShowLineSum)
					m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;	//ע����������ʾһ���б�ѡ�У����ԣ�����Ҫ���ͷ��β
				setAction(MLTTXT_ACTION_TYPE_MOVE,m_selectLineOrder);	//��ǩ�ƶ�
			}
			if(!m_haveSelect || m_selectLineOrder != tmp + m_showStartLine)
			{
				if(!m_haveSelect) setAction(MLTTXT_ACTION_TYPE_IN, tmp + m_showStartLine);	//��ѡ��
				m_haveSelect = XTrue;
				m_selectLineOrder = tmp + m_showStartLine;
				if(m_eventProc != NULL) m_eventProc(m_pClass,m_objectID,MLTTXT_SELECT);
				else XCtrlManager.eventProc(m_objectID,MLTTXT_SELECT);
				//�����ѡ����
				m_lineStr[tmp].haveAllSelect = XTrue;	//ע����������ʾһ���б�ѡ�У����ԣ�����Ҫ���ͷ��β
				//����ѡȡ��λ��
				if(!m_withoutTex)
				{
					m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left, m_curMouseRect.top + m_curTextHeight * tmp);
					m_lineStr[tmp].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
						m_curTextHeight / m_multiEditSelect->textureSize.y);
				}
			}
		}
		m_isBeChoose = XTrue;
	}
	return XTrue;
}
void XMultiText::setPosition(const XVec2& p)
{
	if(!m_isInited) return;
	m_position = p;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() - 
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ

	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
	}
	if(m_haveSelect && !m_withoutTex)
	{//����ѡȡ
		int tmp = m_selectLineOrder - m_showStartLine;
		m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left,m_curMouseRect.top + m_curTextHeight * tmp);
	}
}
void XMultiText::setScale(const XVec2& s)
{
	if(!m_isInited || s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
		m_position + (m_mouseRect.getRB() -
			XVec2(m_verticalSlider.getMouseRectWidth(), m_horizontalSlider.getMouseRectHeight())) * m_scale);	//��ǰ�������Ӧ��Χ
	if(!m_withoutTex)
	{
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setScale(m_scale);
	}
	m_verticalSlider.setPosition(m_position + XVec2(m_mouseRect.right - m_verticalSlider.getMouseRectWidth(), m_mouseRect.top) * m_scale);
	m_verticalSlider.setScale(m_scale);
	m_horizontalSlider.setPosition(m_position + XVec2(m_mouseRect.left, m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_scale);
	m_horizontalSlider.setScale(m_scale);

	m_lineStr[0].curText.setScale(m_scale);
	m_textWidth = m_lineStr[0].curText.getTextSize().x * m_lineStr[0].curText.getScale().x * 0.5f;
	m_curTextHeight = m_lineStr[0].curText.getTextSize().y * m_lineStr[0].curText.getScale().y;
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_lineStr[i].curText.setPosition(m_position.x + (m_mouseRect.left - m_curStartPixels) * m_scale.x,
			m_position.y + m_mouseRect.top * m_scale.y + m_curTextHeight * i);
		m_lineStr[i].curText.setScale(m_scale);
	}
	if(m_haveSelect && !m_withoutTex)
	{//����ѡȡ
		int tmp = m_selectLineOrder - m_showStartLine;
		m_lineStr[tmp].spriteSelect.setPosition(m_curMouseRect.left,m_curMouseRect.top + m_curTextHeight * tmp);
		m_lineStr[tmp].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
			m_curTextHeight / m_multiEditSelect->textureSize.y);
	}
}
void XMultiText::checkStr()			//����ַ����ĵ��п���Ƿ��������
{
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		if ((*it)->length() < MAX_LINE_STRING_LENGTH - 2) continue;
		//���ȳ������ƣ�������нض�
		*(*it) = (*it)->substr(0,MAX_LINE_STRING_LENGTH - 2);
	}
}
void XMultiText::clearAllStrData()
{//����������ʹ�õ����ݻ���
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		m_freeStr.push_back(*it);
	}
	m_curStr.clear();
}
bool XMultiText::addAStrData(const char * str)
{
	bool ret = false;
	std::string *tmp = NULL;
	if (m_freeStr.size() > 0)
	{//�㹻
		tmp = m_freeStr[0];
		m_freeStr.pop_front();
	}
	else
	{//����
		tmp = m_curStr[0];
		m_curStr.pop_front();
		ret = true;
	}
	*tmp = str;
	m_curStr.push_back(tmp);
	return ret;
}
void XMultiText::setString(const char *str)
{
	if (!m_isInited ||	//����Ҫ��ʼ��
		str == NULL) return;		//��ָ����Ҫ�˳�
	//������Ҫ���ַ����ֶη���
	clearAllStrData();
	int len = strlen(str) + 1;	//��Ҫ���Ͻ�����
	if (len > 0)
	{
		char tmpStr[MAX_LINE_STRING_LENGTH];
		int start = 0;
		for (int i = 0; i < len; ++i)
		{
			if (str[i] < 0)
			{//����
				++i;
			}
			else
				if (str[i] == '\0' || str[i] == '\n')
				{//һ�еĽ���
					if (i - start >= MAX_LINE_STRING_LENGTH - 2)
					{//��������
						memcpy(tmpStr, str + start, MAX_LINE_STRING_LENGTH - 2);
						tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
					}
					else
					{
						memcpy(tmpStr, str + start, i - start);
						tmpStr[i - start] = '\0';
					}
					addAStrData(tmpStr);
					start = i + 1;
				}
		}
	}
	//��������������ַ�������Ҫ����һ�е������ʾ�������������ں����ͳһ����
	checkStr();

	//�����ַ���
	m_showStartLine = 0;
	updateLineStr();

	if (m_haveSelect) setAction(MLTTXT_ACTION_TYPE_OUT, m_selectLineOrder);	//ȡ��ѡ��
	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_selectLineOrder = -1;
	for (int i = 0; i < m_canShowLineSum; ++i)
	{
		m_lineStr[i].haveAllSelect = XFalse;
	}
}
void XMultiText::addALine(const char *str)		//���ַ���������ַ���
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		str == NULL) return;
	char tmpStr[MAX_LINE_STRING_LENGTH];
	if (strlen(str) >= MAX_LINE_STRING_LENGTH - 2)
	{
		memcpy(tmpStr, str, MAX_LINE_STRING_LENGTH - 2);
		tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
	}
	else
	{
		strcpy_s(tmpStr, MAX_LINE_STRING_LENGTH, str);
	}
	int index = XString::getCharPosition(tmpStr, '\n');
	if (index >= 0) tmpStr[index] = '\0';	//��������Ĳ���

	m_mutex.Lock();
	//����һ�в���
	if (addAStrData(tmpStr))
	{//ע������û�д�������ɵĺܶ�״̬���(��δ���)
		//ѡ�������Ҫ����
		if (m_haveSelect)
		{
			--m_selectLineOrder;
			if (m_selectLineOrder < 0)
			{//������Χ
				m_selectLineOrder = 0;
				m_haveSelect = XFalse;
			}
		}
		//Ŀǰ��ʾ��Χ��Ҫ�ı�
	}

	checkStr();
	if (m_needShowVSlider && m_verticalSlider.getCurValue() == m_verticalSlider.getMaxValue())
	{
		updateLineStr();
		m_verticalSlider.setCurValue(m_verticalSlider.getMaxValue());
	}
	else
	{
		updateLineStr();
	}
	m_mutex.Unlock();
}
void XMultiText::addString(const char *str)			//���ַ���������ַ���
{
	if (!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		str == NULL) return;
	int len = strlen(str) + 1;
	if (len <= 0)
	{
		addALine("");
		return;
	}
	char tmpStr[MAX_LINE_STRING_LENGTH];
	int start = 0;
	for (int i = 0; i < len; ++i)
	{
		if (str[i] < 0)
		{//����
			++i;
		}
		else
			if (str[i] == '\0' || str[i] == '\n')
			{//һ�еĽ���
				if (i - start >= MAX_LINE_STRING_LENGTH - 2)
				{//��������
					memcpy(tmpStr, str + start, MAX_LINE_STRING_LENGTH - 2);
					tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
				}
				else
				{
					memcpy(tmpStr, str + start, i - start);
					tmpStr[i - start] = '\0';
				}
				addALine(tmpStr);
				start = i + 1;
			}
	}
}
XBool XMultiText::setACopy(const XMultiText &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	
	//not finished
	if(m_isInited) release();
	if(!XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}

	m_isInited = temp.m_isInited;				//�Ƿ��ʼ��
	if(m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;
	
	m_needShowVSlider = temp.m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(temp.m_verticalSlider);		//��ֱ������
	XCtrlManager.decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setEventProc(ctrlProc,this);
	m_needShowHSlider = temp.m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(temp.m_horizontalSlider);	//ˮƽ������
	XCtrlManager.decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setEventProc(ctrlProc,this);

	std::string *tmpStr = NULL;
	for(unsigned int i = 0;i < temp.m_freeStr.size();++ i)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL) return XFalse;
		m_freeStr.push_back(tmpStr);
	}
	for(auto it = temp.m_curStr.begin();it != temp.m_curStr.end();++ it)
	{
		tmpStr = XMem::createMem<std::string>();
		if(tmpStr == NULL) return XFalse;
		*tmpStr = *(*it);
		m_curStr.push_back(tmpStr);
	}

	m_multiEditNormal = temp.m_multiEditNormal;		//�������ͨ״̬
	m_multiEditDisable = temp.m_multiEditDisable;		//�������Ч״̬
	m_multiEditSelect = temp.m_multiEditSelect;		//�����Ƭѡ��ɫ
	m_multiEditUpon = temp.m_multiEditUpon;

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//������ʾ�����ı�����ͼ
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackGround);
#endif
	//m_textPosition = temp.m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_haveSelect = temp.m_haveSelect;		//�Ƿ����ַ�������ѡ��
	m_selectLineOrder = temp.m_selectLineOrder;	//��ǰ�ĵ��б�ѡ�������һ��

	m_showStartLine = temp.m_showStartLine;		//��ǰ��ʾ�ĵ�һ�����ܵĵڼ���
	m_canShowLineSum = temp.m_canShowLineSum;		//��ǰ������ʾ������
	//m_curShowLineWidth = temp.m_curShowLineWidth;		//��ǰһ�п�����ʾ�Ŀ��
	m_curMaxLineSum = temp.m_curMaxLineSum;	//��ǰ�ַ�����ʵ�ʵ�����
	//m_curMaxLineLength = temp.m_curMaxLineLength;	//��ǰ�ַ�����ʵ�ʵ��п�

	m_curStartPixels = temp.m_curStartPixels;
	m_curMaxLineWidthPixels = temp.m_curMaxLineWidthPixels;
	m_maxLineWidthPixels = temp.m_maxLineWidthPixels;

	m_lineStr = XMem::createArrayMem<XMultiTextLineString>(m_canShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		//m_lineStr[i].showString = temp.m_lineStr[i].showString;
		m_lineStr[i].isEnable = temp.m_lineStr[i].isEnable;				//��һ���Ƿ�Ҫ��ʾ���Ƿ���Ч
	//	m_lineStr[i].showStart = temp.m_lineStr[i].showStart;				//�����ַ�����ʾ���ֵ�ͷ�ڵ�ǰ�е�ƫ��
	//	m_lineStr[i].showLength = temp.m_lineStr[i].showLength;				//��ʾ�ַ����ĳ���
		m_lineStr[i].curText.setACopy(temp.m_lineStr[i].curText);		//������ʾ��һ���ַ���������
		//m_lineStr[i].curText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].curText));
#endif
//		m_lineStr[i].lineHeadorder = temp.m_lineStr[i].lineHeadorder;			//��һ���ַ���ȫ���е�ͷλ��
//		m_lineStr[i].lineEndOrder = temp.m_lineStr[i].lineEndOrder;			//��һ���ַ���ȫ���е�βλ��
		//m_lineStr[i].lineLength = temp.m_lineStr[i].lineLength;				//��һ���ַ����ĳ���
		//m_lineStr[i].isEnterEnd = temp.m_lineStr[i].isEnterEnd;			//�Ƿ���Ϊ���ж�������ʾ1����������Ϊ��ʾ�ַ����ĳ������ƶ�����0
		m_lineStr[i].spriteSelect.setACopy(temp.m_lineStr[i].spriteSelect);		//�ַ�����ʾѡ�񱳾���ɫ�ľ���
#if WITH_OBJECT_MANAGER
		XObjManager.decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].haveAllSelect = temp.m_lineStr[i].haveAllSelect;			//�������Ƿ�汻����ѡ��
	}

//	m_funInputChenge = temp.m_funInputChenge;		//�������ݷ����ı��ʱ�����
//	m_funInputOver = temp.m_funInputOver;		//ȷ���������֮���ʱ�����
//	m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	m_textColor = temp.m_textColor;
	m_textWidth = temp.m_textWidth;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	m_curTextHeight = temp.m_curTextHeight;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	return XTrue;
}
XBool XMultiText::exportData(const char *fileName)		//���ݵ���
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
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
	for(auto it = m_curStr.begin();it != m_curStr.end();++ it)
	{
		fprintf(fp,"%s\n",(*it)->c_str());
	}
	fclose(fp);
	return XTrue;
}
XBool XMultiText::importData(const char *fileName)		//���ݵ���
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
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
	//���ж�ȡ���ݲ�����
	char tempStr[1024];
	if(fscanf(fp,"%s\n",tempStr) == 1)
		addALine(tempStr);
	fclose(fp);
	return XTrue;
}
XBool XMultiText::saveState(TiXmlNode &e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	std::string tmpStr = "";
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		tmpStr += (*(*it)) + "\n";
	}
	if(!XXml::addLeafNode(e,m_ctrlName.c_str(),tmpStr)) return XFalse;
	return XTrue;
}
XBool XMultiText::loadState(TiXmlNode *e)
{
	if(!m_needSaveAndLoad) return XTrue;	//�������Ҫ������ֱ�ӷ���
	std::string tmpStr;
	if(XXml::getXmlAsString(e,m_ctrlName.c_str(),tmpStr) == NULL) return XFalse;
	setString(tmpStr.c_str());
	return XTrue;
}
XBool XMultiText::deleteSelectLine()		//ɾ����ǰѡ���һ��
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return XFalse;
	std::string tmpStr = "";
	auto sT = m_curStr.begin() + m_selectLineOrder;
	for (auto it = m_curStr.begin(); it != m_curStr.end(); ++it)
	{
		if(it == sT) continue;
		tmpStr += (*(*it)) + "\n";
	}
	setString(tmpStr.c_str());
	//setString�Ѿ�������ȡ��ѡ��Ĳ���
	//m_haveSelect = false;	//ȡ��ѡ��
	return XTrue;
}
XBool XMultiText::moveUpSelectLine()	//��ѡ���һ������
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return XFalse;
	if(!moveUpLine(m_selectLineOrder))
	{//�ƶ�ʧ��
		return XFalse;
	}else
	{
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
		-- m_selectLineOrder;
		if(m_selectLineOrder < m_showStartLine)
		{
			m_showStartLine = m_selectLineOrder;
			updateLineStr();
		}
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
XBool XMultiText::moveDownSelectLine()	//��ѡ���һ������
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return XFalse;
	if(!moveDownLine(m_selectLineOrder))
	{//�ƶ�ʧ��
		return XFalse;
	}else
	{
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XFalse;
		++ m_selectLineOrder;
		if(m_selectLineOrder >= m_showStartLine + m_canShowLineSum)
		{
			++ m_showStartLine;
			updateLineStr();
		}
		m_lineStr[m_selectLineOrder - m_showStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setPosition(m_curMouseRect.left,
				m_curMouseRect.top + m_curTextHeight * (m_selectLineOrder - m_showStartLine));
			m_lineStr[m_selectLineOrder - m_showStartLine].spriteSelect.setScale(m_curMouseRect.getWidth() / m_multiEditSelect->textureSize.x,
							m_curTextHeight / m_multiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
XBool XMultiText::insertALine(const char *str,int lineOrder)	//���ı���ָ���в���һ��
{
	if(!m_isInited ||		//���û�г�ʼ��ֱ���˳�
		!m_haveSelect) return XFalse;
	if(lineOrder < 0 || lineOrder >= m_curMaxLineSum) return XFalse;	//�Ƿ���ֵ���˳�
	if(str == NULL || strlen(str) == 0) return XFalse;
	char tmpStr[MAX_LINE_STRING_LENGTH];
	if(strlen(str) >= MAX_LINE_STRING_LENGTH - 2)
	{
		memcpy(tmpStr,str,MAX_LINE_STRING_LENGTH - 2);
		tmpStr[MAX_LINE_STRING_LENGTH - 1] = '\0';
	}else
	{
		strcpy_s(tmpStr,MAX_LINE_STRING_LENGTH,str);
	}
	int index = XString::getCharPosition(tmpStr,'\n');
	if(index >= 0) tmpStr[index] = '\0';	//��������Ĳ���

	if(m_freeStr.size() == 0)
	{
		if(lineOrder == 0) return XFalse;	//�Ѿ����ˣ������������
		//������һ���ٽ������
		std::string *tmp = m_curStr[0];
		m_curStr.pop_back();
		*tmp = tmpStr;
		m_curStr.insert(m_curStr.begin() + lineOrder - 1,tmp);
		if(m_haveSelect && m_selectLineOrder < lineOrder) 
		{
			-- m_selectLineOrder;
			if(m_selectLineOrder < 0)
			{//������Χ
				m_selectLineOrder = 0;
				m_haveSelect = XFalse;
			}
		}
	}else
	{
		std::string *tmp = m_freeStr[0];
		m_freeStr.pop_back();
		*tmp = tmpStr;
		m_curStr.insert(m_curStr.begin() + lineOrder,tmp);
		if(m_haveSelect && m_selectLineOrder >= lineOrder) ++ m_selectLineOrder;
	}
	checkStr();
	updateLineStr();
	return XTrue;
}
XBool XMultiText::keyboardProc(int keyOrder,XKeyState keyState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable ||
		!m_isBeChoose) return XFalse;		//�����Ч��ֱ���˳�
	if(m_isSilent) return XFalse;
	if(keyState == KEY_STATE_UP)
	{
		switch(keyOrder)
		{
		case XKEY_UP:
			if(m_needShowVSlider) 
			{
				m_verticalSlider.m_isBeChoose = XTrue;
				m_verticalSlider.keyboardProc(XKEY_UP,KEY_STATE_UP);
				m_verticalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_DOWN:
			if(m_needShowVSlider) 
			{
				m_verticalSlider.m_isBeChoose = XTrue;
				m_verticalSlider.keyboardProc(XKEY_DOWN,KEY_STATE_UP);
				m_verticalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_LEFT:		//����ѡ��(��δ���)
			if(m_needShowHSlider) 
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_LEFT,KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		case XKEY_RIGHT:	//����ѡ��(��δ���)
			if(m_needShowHSlider) 
			{
				m_horizontalSlider.m_isBeChoose = XTrue;
				m_horizontalSlider.keyboardProc(XKEY_RIGHT,KEY_STATE_UP);
				m_horizontalSlider.m_isBeChoose = XFalse;
			}
			break;
		}
	}
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XMultiText.inl"
#endif
}