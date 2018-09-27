#include "XStdHead.h"
#include "XGLFont.h"

namespace XE {
void XGLFont::create(const char *typeface, int Height, int BOLD, int italic)
{
	LOGFONT logfont; //�߼�����
	// ������������
	m_height = Height;
	logfont.lfHeight = Height;		//�ָ�
	logfont.lfWidth = 0;			//�ֿ�
	logfont.lfEscapement = 0;		//�ַ����Ƕ�(1/10��)
	logfont.lfOrientation = 0;
	logfont.lfWeight = NULL;		//����
	logfont.lfItalic = italic;		//б��
	logfont.lfUnderline = FALSE;	//�»���
	logfont.lfStrikeOut = FALSE;	//ɾ����
	logfont.lfCharSet = DEFAULT_CHARSET;			//OEM_CHARSET; //ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_TT_PRECIS;	//OUT_DEFAULT_PRECIS;	//OUT_TT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = ANTIALIASED_QUALITY;
	logfont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	//logfont.lfFaceName[0] = NULL;
	strcpy(logfont.lfFaceName, typeface); //��������
	// ��������
	if (m_isInited) release();
	m_hFont = CreateFontIndirect(&logfont);
	m_isInited = true;
}
void XGLFont::release()
{
	if (!m_isInited) return;
	if (m_hFont != nullptr)
	{
		DeleteObject(m_hFont);
		m_hFont = nullptr;
	}
	m_isInited = false;
}
void XGLFont::drawStr(const XVec2& pos, const std::string& strText, const XFColor& c)
{
	glColor4fv(c);
	glRasterPos2fv(pos);

	if (!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);	//����

	HDC hdc = wglGetCurrentDC();	//�豸����
	HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);	//������ѡ�볡��
//	int nSmooth = 1000, nOldSmooth;
//	SystemParametersInfo(SPI_GETFONTSMOOTHINGCONTRAST, 0, &nOldSmooth, FALSE);
//	SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST, 0, &nSmooth, FALSE);
//	SetBkMode(hdc, TRANSPARENT);

	const char* pStr = &strText[0];
	int len = 0;
	for (int i = 0; i < strText.size(); ++i)
	{
		if (IsDBCSLeadByte(pStr[i])) //�Ƿ�˫�ֽ�(����)
			++i;
		++len;
	}
	m_buff.fitSize((len + 1) * sizeof(wchar_t));
	wchar_t* wstring = (wchar_t*)m_buff.getBuffer();

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strText.c_str(), -1, wstring, len);
	wstring[len] = L'\0';

	unsigned int nListNum = glGenLists(1);
	glPushMatrix();
	for (int i = 0; i < len;++ i)
	{
		wglUseFontBitmapsW(hdc, wstring[i], 1, nListNum);
		glCallList(nListNum);
	}
	glPopMatrix();
	glDeleteLists(nListNum, 1);
//	SystemParametersInfo(SPI_SETFONTSMOOTHINGCONTRAST, 0, &nOldSmooth, FALSE);
	::SelectObject(hdc, hOldFont);
}
void XGLFont::drawStrOutLine(const XVec2& pos, const std::string& strText, float z, const XFColor& c)
{
	glColor4fv(c);

	if (!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);	//����

	HDC hdc = wglGetCurrentDC();	//�豸����
	HFONT hOldFont = (HFONT)::SelectObject(hdc, m_hFont);	//������ѡ�볡��
	const char* pStr = &strText[0];
	int len = 0;
	for (int i = 0; i < strText.size(); ++i)
	{
		if (IsDBCSLeadByte(pStr[i])) //�Ƿ�˫�ֽ�(����)
			++i;
		++len;
	}

	m_buff.fitSize((len + 1) * sizeof(wchar_t));
	wchar_t* wstring = (wchar_t*)m_buff.getBuffer();
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strText.c_str(), -1, wstring, len);
	wstring[len] = L'\0';

	GLYPHMETRICSFLOAT pgmf[1];
	unsigned int nListNum = glGenLists(1);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0.0f);
	glScalef(m_height, -m_height, 1.0f);
	for (int i = 0; i < len; ++i)
	{
		//wglUseFontOutlinesW(hdc, wstring[i], 1, nListNum, 0.0f, z, WGL_FONT_LINES, pgmf);
		wglUseFontOutlinesW(hdc, wstring[i], 1, nListNum, 0.0f, z, WGL_FONT_POLYGONS, pgmf);
		glCallList(nListNum);
	}
	glPopMatrix();
	glDeleteLists(nListNum, 1);
	::SelectObject(hdc, hOldFont);
}
}