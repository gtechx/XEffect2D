#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XProgress.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
namespace XE{
XProgressSkin::XProgressSkin()
	:m_isInited(XFalse)
	, progressBackGround(NULL)			//�������ı�����ͼ
	, progressMove(NULL)				//���������ƶ��仯����ͼ
	, progressUpon(NULL)				//���������ϲ�����ֻ��������Ч
{}
XBool XProgressSkin::init(const char *backgound,const char *move,const char *upon,XResPos resPos)
{
	if(m_isInited ||
		move == NULL) return XFalse;	//���������ٱ���Ҫ���м��ƶ�����ͼ
	int ret = 1;
	if(backgound != NULL && 
		(progressBackGround = createATextureData(backgound,resPos)) == NULL) ret = 0;
	if(move != NULL && ret != 0 && 
		(progressMove = createATextureData(move,resPos)) == NULL) ret = 0;
	if(upon != NULL && ret != 0 &&
		(progressUpon = createATextureData(upon,resPos)) == NULL) ret = 0;

	if(ret == 0)
	{
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		XMem::XDELETE(progressUpon);
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define PROGRESS_CONFIG_FILENAME "Progress.txt"
bool XProgressSkin::loadFromFolder(const char *filename, XResPos resPos)	//���ļ�����������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, PROGRESS_CONFIG_FILENAME);
	FILE *fp = NULL;
	if ((fp = fopen(tempFilename, "r")) == NULL) return XFalse; //��Ϣ�ļ���ȡʧ��
	//���濪ʼ���ζ�ȡ����
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	//normal
	if (fscanf(fp, "%d:", &flag) != 1) { fclose(fp); return XFalse; }
	if (flag == 0)
	{//û����ͨ״̬�ǲ��е�
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%s", resFilename) != 1) { fclose(fp); return XFalse; }
	sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, resFilename);
	if ((progressBackGround = createATextureData(tempFilename, resPos)) == NULL)
	{//��Դ��ȡʧ��
		fclose(fp);
		return XFalse;
	}
	//down
	if (fscanf(fp, "%d:", &flag) != 1) { fclose(fp); return XFalse; }
	if (flag == 0)
	{//û����ͨ״̬�ǲ��е�
		XMem::XDELETE(progressBackGround);
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%s", resFilename) != 1) { fclose(fp); return XFalse; }
	sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, resFilename);
	if ((progressMove = createATextureData(tempFilename, resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE(progressBackGround);
		fclose(fp);
		return XFalse;
	}
	//on
	if (fscanf(fp, "%d:", &flag) != 1) { fclose(fp); return XFalse; }
	if (flag == 0)
	{//û����ͨ״̬�ǲ��е�
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		fclose(fp);
		return XFalse;
	}
	if (fscanf(fp, "%s", resFilename) != 1) { fclose(fp); return XFalse; }
	sprintf_s(tempFilename, MAX_FILE_NAME_LENGTH, "%s/%s", filename, resFilename);
	if ((progressUpon = createATextureData(tempFilename, resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		fclose(fp);
		return XFalse;
	}
	//��ȡ��������
	int l, t, r, b, x, y;
	if (fscanf(fp, "%d,%d,%d,%d,", &l, &t, &r, &b) != 4) { fclose(fp); return XFalse; }
	m_mouseRect.set(l, t, r, b);
	if (fscanf(fp, "%d,%d,", &x, &y) != 2) { fclose(fp); return XFalse; }
	m_fontPosition.set(x, y);
	//�������ݶ�ȡ���
	fclose(fp);
	return true;
}
bool XProgressSkin::loadFromPacker(const char *filename,XResPos resPos)	//��ѹ������������Դ
{
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,PROGRESS_CONFIG_FILENAME);
	unsigned char *p = XResPack.getFileData(tempFilename);
	if(p == NULL) return XFalse;
	//���濪ʼ���ζ�ȡ����
	int flag = 0;
	char resFilename[MAX_FILE_NAME_LENGTH] = "";
	//normal
	int offset = 0;
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
	if((progressBackGround = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//down
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((progressMove = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	//on
	if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),':') + 1;
	if(flag == 0)
	{//û����ͨ״̬�ǲ��е�
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XMem::XDELETE_ARRAY(p);return XFalse;}
	offset += XString::getCharPosition((char *)(p + offset),'\n') + 1;
	sprintf_s(tempFilename,MAX_FILE_NAME_LENGTH,"%s/%s",filename,resFilename);
	if((progressUpon = createATextureData(tempFilename,resPos)) == NULL)
	{//��Դ��ȡʧ��
		XMem::XDELETE(progressBackGround);
		XMem::XDELETE(progressMove);
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
bool XProgressSkin::loadFromWeb(const char *filename,XResPos resPos)		//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XProgressSkin::initEx(const char *filename,XResPos resPos)
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
XBool XProgress::init(const XVec2& position,//�ؼ����ڵ�λ��
	const XRect& area,	//����������ķ�Χ
	const XProgressSkin &tex,	//�ؼ�����ͼ
	const XFontUnicode *font, float captionSize, const XVec2& textPosition,	//�ؼ�������
	int mode)
{
	if (m_isInited ||
		area.getHeight() <= 0 || area.getWidth() <= 0 ||	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
		tex.progressMove == NULL ||//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
		captionSize < 0) return XFalse;
	m_mouseRect = area;
	m_position = position;

	if (mode != 0) m_mode = 1;
	else m_mode = 0;

	m_progressBackGround = tex.progressBackGround;
	m_progressMove = tex.progressMove;
	m_progressUpon = tex.progressUpon;
	m_withoutTex = XFalse;

	m_curValue = 0.0f;
	m_scale.set(1.0f);
	if (m_mode == 0)
	{
		m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
			m_position + m_mouseRect.getRB() * m_scale);
	}
	else
	{
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y, m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y, m_position.y + m_mouseRect.right * m_scale.x);
	}

	//��ʼ��������ʾ����Ϣ
	m_textPosition = textPosition;
	m_textSize.set(captionSize);
	m_textColor.set(0.0f, 1.0f);

	//��ʼ����ʾ�ľ���
	m_spriteBackground.init(m_progressBackGround->texture.m_w, m_progressBackGround->texture.m_h, 1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackground);
#endif
	m_spriteBackground.setPosition(m_position);
	m_spriteBackground.setScale(m_scale);
	m_spriteBackground.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_progressMove->texture.m_w, m_progressMove->texture.m_h, 1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setPosition(m_position);
	m_spriteMove.setScale(m_scale);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);
	m_spriteMove.setClipRect(m_mouseRect.left, m_mouseRect.top, m_mouseRect.left + 1.0f, m_mouseRect.bottom);

	m_spriteUpon.init(m_progressUpon->texture.m_w, m_progressUpon->texture.m_h, 1);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteUpon);
#endif
	m_spriteUpon.setPosition(m_position);
	m_spriteUpon.setScale(m_scale);
	m_spriteUpon.setIsTransformCenter(POINT_LEFT_TOP);
	if (m_mode != 0)
	{
		//m_caption.setAngle(90.0f);
		m_spriteBackground.setAngle(90.0f);
		m_spriteMove.setAngle(90.0f);
		m_spriteUpon.setAngle(90.0f);
	}

	m_isVisible = m_isEnable = m_isActive = XTrue;
	//ע������ؼ�
	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	if (font == NULL) m_isShowFont = XFalse;
	else setFont(*font);
	return XTrue;
}
XBool XProgress::initWithoutSkin(const XRect& area,
	const XFontUnicode* font, float captionSize,	//�ؼ�������
	const XVec2& textPosition, int mode)
{
	if (m_isInited ||
		captionSize < 0) return XFalse;
	m_mouseRect = area;
	m_position.reset();

	if (mode != 0) m_mode = 1;
	else m_mode = 0;

	m_withoutTex = XTrue;

	m_curValue = 0.0f;
	m_scale.set(1.0f);
	if (m_mode == 0)
	{
		m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
			m_position + m_mouseRect.getRB() * m_scale);
	}
	else
	{
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y, m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y, m_position.y + m_mouseRect.right * m_scale.x);
	}

	//��ʼ��������ʾ����Ϣ
	m_textPosition = textPosition;
	m_textSize.set(captionSize);
	m_textColor.set(0.0f, 1.0f);

	m_isVisible = m_isEnable = m_isActive = XTrue;
	//ע������ؼ�
	XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	XObjManager.addAObject(this);
#endif

	m_isInited = XTrue;
	if (font == NULL) m_isShowFont = XFalse;
	else setFont(*font);
	return XTrue;
}
void XProgress::setFont(const XFontUnicode& font)
{
	if (m_isShowFont || !m_isInited) return;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_caption);
#endif
	m_isShowFont = XTrue;
	m_withNumber = false;

	m_caption.setString("0%");
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE);
	if (m_mode != 0)
	{
		m_caption.setAngle(90.0f);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y, 
			m_position.y + m_textPosition.x * m_scale.x);
	}
	else m_caption.setPosition(m_position + m_textPosition * m_scale);

	m_caption.setScale(m_textSize * m_scale);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
}
void XProgress::setNumber(const XNumber &number)
{
	if (m_isShowFont || !m_isInited) return;
	m_captionN.setACopy(number);
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_captionN);
#endif
	m_isShowFont = XTrue;
	m_withNumber = true;

	m_captionN.setNumber("0%");
	m_captionN.setAlignmentMode(NUMBER_ALIGNMENT_MODE_MIDDLE);
	if (m_mode != 0)
	{
		m_captionN.setAngle(90);
		m_captionN.setPosition(m_position.x - m_textPosition.y * m_scale.y,
			m_position.y + m_textPosition.x * m_scale.x);
	}
	else m_captionN.setPosition(m_position + m_textPosition * m_scale);

	m_captionN.setScale(m_textSize * m_scale);
	m_captionN.setColor(m_textColor);							//�����������ɫΪ��ɫ
}
void XProgress::setScale(const XVec2& s)
{
	if (s.x <= 0 || s.y <= 0) return;
	m_scale = s;
	if (!m_withoutTex)
	{
		m_spriteBackground.setScale(m_scale);
		m_spriteMove.setScale(m_scale);
		m_spriteUpon.setScale(m_scale);
	}
	if (m_mode == 0)
	{
		if (m_withNumber)
			m_captionN.setPosition(m_position + m_textPosition * m_scale);
		else
			m_caption.setPosition(m_position + m_textPosition * m_scale);
		m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
			m_position + m_mouseRect.getRB() * m_scale);
	}
	else
	{
		if (m_withNumber)
			m_captionN.setPosition(m_position.x - m_textPosition.y * m_scale.y,
				m_position.y + m_textPosition.x * m_scale.x);
		else
			m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,
				m_position.y + m_textPosition.x * m_scale.x);
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,
			m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,
			m_position.y + m_mouseRect.right * m_scale.x);
	}
	if (m_withNumber)
		m_captionN.setScale(m_scale);
	else
		m_caption.setScale(m_scale);
}
void XProgress::setPosition(const XVec2& p)
{
	m_position = p;
	if (!m_withoutTex)
	{
		m_spriteBackground.setPosition(m_position);
		m_spriteMove.setPosition(m_position);
		m_spriteUpon.setPosition(m_position);
	}
	if (m_mode == 0)
	{
		if (m_withNumber)
			m_captionN.setPosition(m_position + m_textPosition * m_scale);
		else
			m_caption.setPosition(m_position + m_textPosition * m_scale);
		m_curMouseRect.set(m_position + m_mouseRect.getLT() * m_scale,
			m_position + m_mouseRect.getRB() * m_scale);
	}
	else
	{
		if (m_withNumber)
			m_captionN.setPosition(m_position.x - m_textPosition.y * m_scale.y,
				m_position.y + m_textPosition.x * m_scale.x);
		else
			m_caption.setPosition(m_position.x - m_textPosition.y * m_scale.y,
				m_position.y + m_textPosition.x * m_scale.x);
		m_curMouseRect.set(m_position.x - m_mouseRect.top * m_scale.y,
			m_position.y + m_mouseRect.left * m_scale.x,
			m_position.x - m_mouseRect.bottom * m_scale.y,
			m_position.y + m_mouseRect.right * m_scale.x);
	}
}
void XProgress::draw()
{
	if (!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if (m_withoutTex)
	{
		//��汳��
		XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
			m_mouseRect.getSize() * m_scale,
			XCCS::downColor * m_color);
		if (m_withVirtual)
		{
			if (m_curValue >= m_valueMD.getCurData())
			{
				XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
					m_mouseRect.getSize() * XVec2(m_curValue * 0.01f, 1.0f) * m_scale,
					XCCS::onColor * m_color * XFColor(0.75f, 1.0f));
				XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
					m_mouseRect.getSize() * XVec2(m_valueMD.getCurData() * 0.01f, 1.0f) * m_scale,
					XCCS::onColor * m_color);
			}
			else
			{
				XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
					XVec2(m_mouseRect.getWidth() * m_valueMD.getCurData() * 0.01f, m_mouseRect.getHeight()) * m_scale,
					XCCS::onColor * m_color * XFColor(0.75f, 1.0f));
				XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
					m_mouseRect.getSize() * XVec2(m_curValue * 0.01f, 1.0f) * m_scale,
					XCCS::onColor * m_color);
			}
		}
		else
		{
			XRender::drawFillRectExA(m_position + m_mouseRect.getLT() * m_scale,
				m_mouseRect.getSize() * XVec2(m_curValue * 0.01f, 1.0f) * m_scale,
				XCCS::onColor * m_color);
		}
		if (m_isShowFont)
		{
			if (m_withNumber) m_captionN.draw();
			else m_caption.draw();	//��ʾ����
		}
	}
	else
	{
		if (m_progressBackGround != NULL) m_spriteBackground.draw(m_progressBackGround);
		if (m_progressMove != NULL) m_spriteMove.draw(m_progressMove);
		if (m_isShowFont)
		{
			if (m_withNumber) m_captionN.draw();
			else m_caption.draw();	//��ʾ����
		}
		if (m_progressUpon != NULL) m_spriteUpon.draw(m_progressUpon);
	}
}
void XProgress::setValue(float temp)
{
	if (!m_isInited) return;
	temp = XMath::clamp(temp, 0.0f, 100.0f);
	//	int tempI = temp;
	//	if(tempI == (int)m_curValue) return;	//�õ�����
	if ((int)(m_curValue) != (int)(temp)) 	//�õ�Ч��
	{
		char tempC[20] = "100%";
		sprintf_s(tempC, 20, "%d%%", (int)(temp));
		if (m_withNumber) m_captionN.setNumber(tempC);
		else m_caption.setString(tempC);
	}
	if (m_curValue == temp) return;
	m_curValue = temp;
	if (!m_withoutTex)
		m_spriteMove.setClipRect(m_mouseRect.left, m_mouseRect.top,
			m_mouseRect.left + m_mouseRect.getWidth() * m_curValue / 100.0f, m_mouseRect.bottom);

	m_valueMD.setEx(m_valueMD.getCurData(), m_curValue, 0.5f);
}
XProgress::XProgress()
	:m_isInited(XFalse)
	, m_progressBackGround(NULL)
	, m_progressMove(NULL)
	, m_progressUpon(NULL)
	, m_curValue(0.0f)
	, m_isShowFont(false)
	, m_isACopy(XFalse)
	, m_resInfo(NULL)
	, m_withoutTex(XFalse)
	, m_withVirtual(false)
{
	m_ctrlType = CTRL_OBJ_PROGRESS;
}
void XProgress::release()
{
	if (!m_isInited) return;
	XCtrlManager.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(this);
#endif
	if (m_resInfo != NULL)
	{
		XResManager.releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
XBool XProgress::setACopy(const XProgress &temp)		//����һ������
{
	if (&temp == this) return XFalse;
	if (!temp.m_isInited ||
		!XControlBasic::setACopy(temp)) return XFalse;

	m_curValue = temp.m_curValue;					//�������ĵ�ǰֵ
	if (!m_isInited)
	{//���û�о�����ʼ���ͽ��и����Ļ�������Ҫע��ؼ�
		XCtrlManager.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		XObjManager.addAObject(this);
#endif
	}
	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	m_mode = temp.m_mode;					//�������ĵ�ǰֵ
	if (temp.m_isShowFont)
	{
		if (temp.m_withNumber)
		{
			if (!m_captionN.setACopy(temp.m_captionN))	return XFalse;				//�������ı���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&m_captionN);
#endif
		}
		else
		{
			if (!m_caption.setACopy(temp.m_caption))	return XFalse;				//�������ı���
#if WITH_OBJECT_MANAGER
			XObjManager.decreaseAObject(&m_caption);
#endif
		}
	}
	if (m_resInfo != NULL) XResManager.releaseResource(m_resInfo);
	m_resInfo = XResManager.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_progressBackGround = temp.m_progressBackGround;	//�������ı�����ͼ
	m_progressMove = temp.m_progressMove;			//���������ƶ��仯����ͼ
	m_progressUpon = temp.m_progressUpon;			//���������ϲ�����ֻ��������Ч

	m_spriteBackground.setACopy(temp.m_spriteBackground);	//������ʾ�����ľ���
	m_spriteMove.setACopy(temp.m_spriteMove);			//������ʾ��ͼ�ľ���
	//m_movePosition = temp.m_movePosition;			//����������ڱ�����λ��
	m_spriteUpon.setACopy(temp.m_spriteUpon);			//������ʾ�ϲ����ֵľ���
	//m_uponPosition = temp.m_uponPosition;			//��������ڱ�����λ��
#if WITH_OBJECT_MANAGER
	XObjManager.decreaseAObject(&m_spriteBackground);
	XObjManager.decreaseAObject(&m_spriteMove);
	XObjManager.decreaseAObject(&m_spriteUpon);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ��
	m_textSize = temp.m_textSize;				//���ֵĳߴ�
	m_textColor = temp.m_textColor;				//���ֵĳߴ�

	m_withVirtual = temp.m_withVirtual;
	m_valueMD = temp.m_valueMD;

	m_isShowFont = temp.m_isShowFont;				//�Ƿ���ʾ��������
	m_withNumber = temp.m_withNumber;				//�Ƿ���ʾ��������
	m_isACopy = XTrue;
	return XTrue;
}
#if !WITH_INLINE_FILE
#include "XProgress.inl"
#endif
}