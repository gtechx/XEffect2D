#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFontTTF.h"
#include "XFontUnicode.h"
#include "XResourcePack.h"
//
//#include "XObjectManager.h"
#include "../XShaderGLSL.h"
namespace XE{
#define FONT_UNICODE_CONFIG_FILENAME "FontUnicodeConfig.dat"
//int testE = 0;
XFontUnicode::XFontUnicode()
	:m_texture(NULL)
	//,m_isAcopy(0)
	,m_fontPageText(NULL)
	,m_pageSum(0)
	,m_pageW(0)
	,m_pageH(0)
	,m_libFontSum(0)
	,m_layout(0,0)
	,m_cp(NULL)
{
	m_textPosition.resize(m_maxStringLen);
	m_textRect.resize(m_maxStringLen);
	m_textPageOrder.resize(m_maxStringLen);
	m_objType = OBJ_FONTUNICODE;
//	++ testE;
//	printf("%d\n",testE);
}
XFontUnicode::XFontUnicode(int maxStrLen)
	:XFontBasic(maxStrLen)
	,m_texture(NULL)
	//,m_isAcopy(0)
	,m_fontPageText(NULL)
	,m_pageSum(0)
	,m_pageW(0)
	,m_pageH(0)
	,m_libFontSum(0)
	,m_layout(0,0)
	,m_cp(NULL)
{
	m_textPosition.resize(m_maxStringLen);
	m_textRect.resize(m_maxStringLen);
	m_textPageOrder.resize(m_maxStringLen);
	m_objType = OBJ_FONTUNICODE;

//	++ testE;
//	printf("%d\n",testE);
}
XFontUnicode::~XFontUnicode()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		//for(int i = 0;i < m_pageSum;++ i)
		//{
		//	m_texture[i].release();
		//}
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);

		XMem::XDELETE(m_cp);
	}
//	testE --;
//	printf("%d\n",testE);

#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(this);
#endif

	release();
}
bool XFontUnicode::loadFromFolder(const char *filename)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL) return XFalse;

	unsigned char fileReadChar;
	m_libFontSum = 0;
	int index = 0;
	for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
	{
		if(fread(&fileReadChar,1,1,fp) == 0) break;
		if(feof(fp)) break;	//�ļ���ȡʧ�����˳�
		if(fileReadChar == '\n') continue;
		if(fileReadChar <128)
		{//ASCII
			m_fontPageText[index] = fileReadChar;
			m_fontPageText[index + 1] = ' ';
			++ m_libFontSum;
			index += UNICODE_BYTES_WIDTH;
		}else
		{//��ASCII
			m_fontPageText[index] = fileReadChar;
			if(fread(&fileReadChar,1,1,fp) == 0) return XFalse;
			m_fontPageText[index + 1] = fileReadChar;
			++ m_libFontSum;
			index += UNICODE_BYTES_WIDTH;
		}
	}
	fclose(fp);
	return !(m_libFontSum == 0);//�ַ�����û������Ԫ��
}
bool XFontUnicode::loadFromPacker(const char *filename)	//��ѹ������������Դ
{
	//static int checkedresource = 0;
	//if(checkedresource == 0)
	//{
	//	checkedresource = 1;
	//	if(ResourceTemp.checkCheckData() == 0)
	//	{//���صĴ������ﶨ����Ϸ
	//		DebugShow("Resource Data Error!\n");
	//		while(true)
	//		{
	//			Sleep(100);
	//		}
	//	}
	//}
	int lengthTemp = XResPack.getFileLength(filename);
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL) return XFalse;

	unsigned char fileReadChar;
	m_libFontSum = 0;
	int curPosition = 0;
	for(int i = 0;i < MAX_UNICODE_TEXT_SUM;++ i)
	{
		if(curPosition == lengthTemp) break;
		fileReadChar = p[curPosition];
		++ curPosition;
		if(fileReadChar == '\n') continue;
		if(fileReadChar <128)
		{//ASCII
			m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = ' ';
			m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
			++ m_libFontSum;
		}else
		{//��ASCII
			m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum] = fileReadChar;
			if(curPosition == lengthTemp) 
			{
				XMem::XDELETE_ARRAY(p);
				return XFalse;
			}
			fileReadChar = p[curPosition];
			++ curPosition;
			m_fontPageText[UNICODE_BYTES_WIDTH * m_libFontSum + 1] = fileReadChar;
			++ m_libFontSum;
		}
	}
	XMem::XDELETE_ARRAY(p);
	return !(m_libFontSum == 0);//�ַ�����û������Ԫ��
}
bool XFontUnicode::loadFromWeb(const char *filename)	//����ҳ�ж�ȡ��Դ
{
	return false;
}
XBool XFontUnicode::init(const char *filenameIn,const XVec2& size,const XVec2& layout,int pageSum,
						   XResPos resPos,XBool withFBO)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	try
	{
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_withFBO = withFBO;

	if(pageSum <= 0 || pageSum >= UNICODE_FONT_PAGE_SUM || pageSum >= 100) return XFalse;
	m_pageSum = pageSum;
	if((m_texture = XMem::createArrayMem<XTextureData>(m_pageSum)) == NULL)
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	if((m_fontPageText = XMem::createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE(m_cp);
		return XFalse;
	}

	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;

	m_string[0] = '\0';
	//����ͼƬ
	char filename[MAX_FONT_UNICODE_FILE_NAME_LENGTH];

	strcpy_s(filename,MAX_FONT_UNICODE_FILE_NAME_LENGTH,filenameIn);
	int fileNameLength = strlen(filename);
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return XFalse;
		}

	//	if(!TextureLoad(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition))
		if(!m_texture[i].load(filename,m_resoursePosition))
		{
			LogNull("pic:%s load error!\n",filename);
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return XFalse;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_sprite.setBlendModel(XGL::BLEND_MODEL0);
	m_fboSprite.setACopy(m_sprite);
	m_fboSprite.setBlendModel(XGL::BLEND_MODEL1);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif
	//��ȡ������Ӧ���ı�
	memcpy(filename + fileNameLength - 6, &"00.txt", 6);
	//filename[fileNameLength - 5] = '0';
	//filename[fileNameLength - 6] = '0';
	//filename[fileNameLength - 3] = 't';
	//filename[fileNameLength - 2] = 'x';
	//filename[fileNameLength - 1] = 't';
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_WEB:
		if(!loadFromWeb(filename))
		{
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename) && !loadFromFolder(filename) &&
			!loadFromWeb(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	}
	m_position.reset();	
	m_setPosition.reset();	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = m_distance = 0.0f;		
	m_size = size;			
	m_layout = layout;
	m_scale.set(1.0f);
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
bool XFontUnicode::loadInfoFromFolder(const char *filename,char *target)	//���ļ�����������Դ
{
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL)return XFalse;
	if(fscanf(fp,"%s\n",target) != 1)
	{
		fclose(fp);
		return XFalse;
	}
	if(fscanf(fp,"%f,%f,%f,%f,%d,",&m_size.x,&m_size.y,&m_layout.x,&m_layout.y,&m_pageSum) != 5)
	{
		fclose(fp);
		return XFalse;
	}
	fclose(fp);
	return true;
}
bool XFontUnicode::loadInfoFromPacker(const char *filename,char *target)	//��ѹ������������Դ
{
	unsigned char *p = XResPack.getFileData(filename);
	if(p == NULL) return XFalse;
	if(sscanf((char *)p,"%s\n",target) != 1)
	{
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	if(sscanf((char *)p + strlen(target),"%f,%f,%f,%f,%d,",&m_size.x,&m_size.y,&m_layout.x,&m_layout.y,&m_pageSum) != 5)
	{
		XMem::XDELETE_ARRAY(p);
		return XFalse;
	}
	XMem::XDELETE_ARRAY(p);
	return true;
}
bool XFontUnicode::loadInfoFromWeb(const char *filename,char *target)		//����ҳ�ж�ȡ��Դ
{
	return false;
}

XBool XFontUnicode::initEx(const char *filenameIn,XResPos resPos,XBool withFBO)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	try
	{
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_withFBO = withFBO;

	if(resPos == RES_SYS_DEF) resPos = getDefResPos();
	m_resoursePosition = resPos;
	//�����ȡ��ظ�ʽ����
	char filename[MAX_FONT_UNICODE_FILE_NAME_LENGTH];
	sprintf_s(filename,MAX_FONT_UNICODE_FILE_NAME_LENGTH,"%s/%s",filenameIn,FONT_UNICODE_CONFIG_FILENAME);
	char txtureFilename[MAX_FILE_NAME_LENGTH] = "";

	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadInfoFromPacker(filename,txtureFilename))
		{
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_LOCAL_FOLDER:
		if(!loadInfoFromFolder(filename,txtureFilename))
		{
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_WEB:
		if(!loadInfoFromWeb(filename,txtureFilename))
		{
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_AUTO:
		if(!loadInfoFromPacker(filename,txtureFilename) && !loadInfoFromFolder(filename,txtureFilename) &&
			!loadInfoFromWeb(filename,txtureFilename))
		{
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	}

	if(m_pageSum <= 0 || m_pageSum >= UNICODE_FONT_PAGE_SUM || m_pageSum >= 100) 
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	sprintf_s(filename,MAX_FONT_UNICODE_FILE_NAME_LENGTH,"%s/%s",filenameIn,txtureFilename);
	int fileNameLength = strlen(filename);

	if((m_texture = XMem::createArrayMem<XTextureData>(m_pageSum)) == NULL)
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	if((m_fontPageText = XMem::createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XMem::XDELETE(m_cp);
		XMem::XDELETE_ARRAY(m_texture);
		return XFalse;
	}
	m_string[0] = '\0';
	//����ͼƬ
	for(int i = 0;i < m_pageSum;++ i)
	{
		if( i< 10)
		{
			filename[fileNameLength - 5] = i%10 + '0';
		}else
		if( i< 100)
		{
			filename[fileNameLength - 5] = i%10 + '0';
			filename[fileNameLength - 6] = (i/10)%10 + '0';
		}else
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return XFalse;
		}

	//	if(!TextureLoad(&m_texture[i],filename,1,&m_pageW,&m_pageH,m_resoursePosition))
		if(!m_texture[i].load(filename,m_resoursePosition))
		{
			LogNull("pic:%s load error!\n",filename);
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return XFalse;
		}
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_sprite.setBlendModel(XGL::BLEND_MODEL0);
	m_fboSprite.setACopy(m_sprite);
	m_fboSprite.setBlendModel(XGL::BLEND_MODEL1);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif
	//��ȡ������Ӧ���ı�
	memcpy(filename + fileNameLength - 6, &"00.txt", 6);
	//filename[fileNameLength - 5] = '0';
	//filename[fileNameLength - 6] = '0';
	//filename[fileNameLength - 3] = 't';
	//filename[fileNameLength - 2] = 'x';
	//filename[fileNameLength - 1] = 't';
	switch(resPos)
	{
	case RES_LOCAL_PACK:
		if(!loadFromPacker(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_LOCAL_FOLDER:
		if(!loadFromFolder(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_WEB:
		if(!loadFromWeb(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	case RES_AUTO:
		if(!loadFromPacker(filename) && !loadFromFolder(filename) &&
			!loadFromWeb(filename))
		{
			XMem::XDELETE_ARRAY(m_texture);
			XMem::XDELETE_ARRAY(m_fontPageText);
			XMem::XDELETE(m_cp);
			return false;
		}
		break;
	}

	m_position.reset();	
	m_setPosition.reset();	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = m_distance = 0.0f;
	m_scale.set(1.0f);
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
XBool XFontUnicode::initFromTTF(const char *filenameIn,int fontSize,XResPos resPos,
								  bool withOutLine,XBool withFBO,bool withOpr)
{
	if(m_isInited) 
	{
		LogStr("The font have initted!");
		return XTrue;
	}
	try
	{
		m_cp = new XSCounter;
		if(m_cp == NULL) return XFalse;
	}catch(...)
	{
		return XFalse;
	}
	m_withFBO = withFBO;
	m_resoursePosition = resPos;
	//�����ȡ��ظ�ʽ����
	char filename[MAX_FONT_UNICODE_FILE_NAME_LENGTH];
	strcpy_s(filename,MAX_FONT_UNICODE_FILE_NAME_LENGTH,filenameIn);
	int fileNameLength = strlen(filename);

	int fontOrder = -1;
	if (m_fontPtSize >= 0) fontOrder = XTTFFont.loadTTFFile(filename, m_fontPtSize, fontSize, m_resoursePosition);
	else fontOrder = XTTFFont.loadTTFFile(filename, fontSize, fontSize, m_resoursePosition);
	if(fontOrder < 0)
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	//XTTFFont.setOutline(fontOrder,1);	//Ч���ǳ�������
	std::vector<unsigned int> texs;
	XVec2 layout;
	if(!XTTFFont.getTextureFontUnicode(fontOrder,texs,layout,withOutLine, withOpr)) return XFalse;
	//�ͷ���Դ
	XTTFFont.releaseTTFFile(fontOrder);
	XVec2 size(fontSize,fontSize);
	int w = XMath::getMinWellSize2n(layout.x * fontSize);
	int h = XMath::getMinWellSize2n(layout.y * fontSize);

	if(texs.size() <= 0 || texs.size() >= UNICODE_FONT_PAGE_SUM) 
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_pageSum = texs.size();

	if((m_texture = XMem::createArrayMem<XTextureData>(m_pageSum)) == NULL)
	{
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	if((m_fontPageText = XMem::createArrayMem<char>(MAX_UNICODE_TEXT_SUM)) == NULL)
	{
		XMem::XDELETE(m_cp);
		XMem::XDELETE_ARRAY(m_texture);
		return XFalse;
	}
	m_string[0] = '\0';

	filename[fileNameLength - 3] = filenameIn[fileNameLength - 3];
	filename[fileNameLength - 2] = filenameIn[fileNameLength - 2];
	filename[fileNameLength - 1] = filenameIn[fileNameLength - 1];
	//����ͼƬ
	for(int i = 0;i < m_pageSum;++ i)
	{
		m_texture[i].createWithTexture(w,h,COLOR_RGBA,texs[i]);
	}

	m_pageW = m_texture[0].texture.m_w;
	m_pageH = m_texture[0].texture.m_h;

	//��ʼ������
	if(!m_sprite.init(m_pageW,m_pageH,1,POINT_LEFT_TOP)) 
	{
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);
		XMem::XDELETE(m_cp);
		return XFalse;
	}
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_sprite.setBlendModel(XGL::BLEND_MODEL0);
	m_fboSprite.setACopy(m_sprite);
	m_fboSprite.setBlendModel(XGL::BLEND_MODEL1);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif
	//������Ҫת�ַ�
#if WITH_FULL_ALL_CHINESE == 0
	const char tempStr[] = TEXT_FONT_UNICODE_MODE;
	int length = strlen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 2
	char tempStr[] = TEXT_FONT_UNICODE_MODE;
	int length = strlen(TEXT_FONT_UNICODE_MODE);
#endif
#if WITH_FULL_ALL_CHINESE == 1
	std::string tmp = TEXT_FONT_UNICODE_MODE0;
	tmp += TEXT_FONT_UNICODE_MODE1;
	tmp += TEXT_FONT_UNICODE_MODE2;
	tmp += TEXT_FONT_UNICODE_MODE3;
	const char *tempStr = tmp.c_str();
	int length = (int)(tmp.size()) + 1;
#endif
	int j = 0;
	m_libFontSum = 0;
	for(int i = 0;i < length;)
	{
		m_fontPageText[j] = tempStr[i];
		if(tempStr[i] < 0)
		{
			m_fontPageText[j + 1] = tempStr[i + 1];
			i += 2;
		}else
		{
			m_fontPageText[j + 1] = ' ';
			++ i;
		}
		j += 2;
		++ m_libFontSum;
	}
	m_fontPageText[j] = '\0';
	m_fontPageText[j + 1] = '\0';
	m_position.reset();
	m_setPosition.reset();	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	m_angle = m_distance = 0.0f;
	m_size = size;
	m_layout = layout;
	m_scale.set(1.0f);
	m_sprite.setScale(m_scale);
	m_isPassword = XFalse;

	m_isInited = XTrue;		

	m_isCliped = XFalse;
	return XTrue;
}
void XFontUnicode::draw()
{
	if(!m_isInited) return;
//	if((m_isCliped != 0 && m_clipRect.top >= m_size.y)
//		|| (m_isCliped != 0 && m_clipRect.bottom <= 0))
	if((m_isCliped && m_clipRect.bottom <= 0) ||//�������ʲôҲ���û�
		!m_isVisible) return;
	if(m_needUpdateData)
	{
		m_needUpdateData = XFalse;
		updateData();
	}
	if(m_withFBO)
	{//��δ���
		if(m_needUpdateFBO)
		{//����FBO����δ�����������ԣ�
			int minX,minY,maxX,maxY;
			XVec2 tmp = getBox(0);
			minX = maxX = tmp.x;
			minY = maxY = tmp.y;
			for(int i = 1;i < 4;++ i)
			{
				tmp = getBox(i);
				if(minX > tmp.x) minX = tmp.x;else
				if(maxX < tmp.x) maxX = tmp.x;
				if(minY > tmp.y) minY = tmp.y;else
				if(maxY < tmp.y) maxY = tmp.y;
			}
			XVec2 pos(minX,minY);
			int w = m_pageW;
			int h = m_pageH;
			if(m_pFbo == NULL || w > m_pFbo->getWidth(0) || h > m_pFbo->getHeight(0))
			{
				XMem::XDELETE(m_pFbo);
				m_pFbo = XMem::createMem<XFBO>();
				if(m_pFbo == NULL) return;
				if(!m_pFbo->init(w,h)) return;
			}
			m_pFbo->useFBO();
			m_pFbo->attachTex();
			XEG.clearScreen(XFColor::zero);
			for(int i = 0;i < m_needShowTextSum;++ i)
			{
				//if(m_textRect[i].getWidth() <= 0 || m_textRect[i].getHeight() <= 0) continue;
				m_sprite.setClipRect(m_textRect[i]);
				m_sprite.setPosition(m_textPosition[i] - pos);

				m_sprite.draw(&(m_texture[m_textPageOrder[i]]));
			}
			m_pFbo->removeFBO();
			m_fboSprite.setClipRect(XRect(0.0f, 0.0f, maxX - minX, maxY - minY));
			m_fboSprite.setPosition(pos);
			m_needUpdateFBO = XFalse;	//�������
		}
		if(m_pFbo != NULL) m_fboSprite.draw(m_pFbo->getTexture(0));
	}else
	{
		//������Щ��������ļ����������￼��ͨ��ʹ��FBOһ�����
		for(int i = 0;i < m_needShowTextSum;++ i)
		{
			//if(m_textRect[i].getWidth() <= 0 || m_textRect[i].getHeight() <= 0) continue;
			m_sprite.setClipRect(m_textRect[i]);
			m_sprite.setPosition(m_textPosition[i]);

			m_sprite.draw(&(m_texture[m_textPageOrder[i]]));
		}
	}
}
void XFontUnicode::updateData()
{
	if(!m_isInited) return;
	if(m_withFBO) m_needUpdateFBO = XTrue;
	m_needShowTextSum = m_maxPixelWidth = m_maxPixelHeight = 0;
	int cur_x = 0;
	int cur_y = 0;
	char tempChar[2];
	int charPoint = 0;	//�ַ�����λ�õ�ָ��
	int pageNumble = -1;
	int wordsSumD = 0;		//˫�ֽ�
	int wordsSumS = 0;		//���ֽ�
	XBool curIsD = XFalse;		//��ǰ��ʾ���������ʱ���ֽڻ��Ƕ��ֽ�
	//Ϊ�˶��뷽ʽ������������Ҫ�����ʵ��Ĵ���
	//1��ͳ��������
	int allLineSum = 1;
	int curLineWidthD = 0;	//��ǰ���е������ַ�����
	int curLineWidthS = 0;	//��ǰ���е�ASCII�ַ�����
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;
		if((unsigned char)m_string[i] < 128) 
		{//ASCII
			if(m_string[i] == '\n') ++ allLineSum;	//����������
			else 
			{
				if(allLineSum == 1) ++ curLineWidthS; //ͳ�Ƶ�һ�еĿ��
			}
		}else
		{//��ASCII
			if(allLineSum == 1) ++ curLineWidthD; //ͳ�Ƶ�һ�еĿ��
			++ i;
		}
	}

	m_lineSum = 0;	//����ͳ������
	float textPixelDX = (m_size.x + m_distance) * m_scale.x;			//˫�ֽ��ַ��Ŀ��
	float textPixelSX = (m_size.x * 0.5f + m_distance) * m_scale.x;	//���ֽ��ַ��Ŀ��
	float textPixelY = (m_size.y + m_distanceY) * m_scale.y;			//�ַ��Ŀ��
	for(int i =0;i < m_maxStringLen;++ i)
	{
		if(m_string[i] == '\0') break;	//�ַ�������
		if((unsigned char)m_string[i] < 128)
		{//ASCII
			if(!m_isPassword)//������ģʽ
				tempChar[0] = m_string[i];
			else//����ģʽ
				tempChar[0] = '*';

			tempChar[1] = ' ';
			//++ wordsSumS;
			curIsD = XFalse;
			if(m_string[i] == '\n')
			{//����ǻ��з���ʼ����
				++ m_lineSum;
				wordsSumS = wordsSumD = 0;
				//��������ʱͳ�Ƶ�ǰ�е��ַ�������
				curLineWidthD = curLineWidthS = 0;
				for(int j = i + 1;j < m_maxStringLen;++ j)
				{
					if((unsigned char)m_string[j] < 128) 
					{//ASCII
						if(m_string[j] == '\n' || m_string[j] == '\0') break;	//��ǰ�н���
						else ++ curLineWidthS; //ͳ�Ƶ�һ�еĿ��
					}else
					{//��ASCII
						++ curLineWidthD; //ͳ�Ƶ�һ�еĿ��
						++ j;
					}
				}
				continue;	//���з�����Ҫ���κδ���ֱ�Ӽ���
			}
		}else
		{//��ASCII
			if(!m_isPassword)
			{//������ģʽ
				tempChar[0] = m_string[i];
				tempChar[1] = m_string[i + 1];
				//++ wordsSumD;
				curIsD = XTrue;
			}else
			{//����ģʽ
				tempChar[0] = '*';
				tempChar[1] = ' ';
				//++ wordsSumS;
				curIsD = XFalse;
			}
			++ i;
		}
		//�����ַ�
		charPoint = getTextIndex(tempChar);
		if(charPoint == m_libFontSum || charPoint < 0)
		{//����Ҳ�������ַ�����Ϊ�ո�������
			//charPoint = 0;
			if(curIsD) ++ wordsSumD;
			else ++wordsSumS;
			continue;
		}
		//�����ַ���ҳ�棬�������������
		pageNumble = charPoint / (int)(m_layout.x * m_layout.y);
		cur_x = (charPoint % (int)(m_layout.x)) * m_size.x;

		if(m_layout.y == 0) cur_y = 0;
		else cur_y = ((charPoint / (int)(m_layout.x)) % (int)(m_layout.y)) * m_size.y;

		if(!m_isCliped)
		{
			m_textRect[m_needShowTextSum].set(cur_x,cur_y,cur_x + m_size.x,cur_y + m_size.y);
			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT://����룬ɶҲ������
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE://����룬ɶҲ������
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT://����룬ɶҲ������
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						- m_angleSin * textPixelY * m_lineSum,
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						+ m_angleCos * textPixelY * m_lineSum);
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						- m_angleSin * textPixelY * (m_lineSum - allLineSum),
						m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS)
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum));
					break;
				}
				break;
			}

		//	m_textPosition[m_needShowTextSum].set(m_position.x + m_angleCos * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		- m_angleSin * textPixelY * m_lineSum,
		//		m_position.y + m_angleSin * (textPixelDX * wordsSumD + textPixelSX * wordsSumS)
		//		+ m_angleCos * textPixelY * m_lineSum);
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			m_needShowTextSum ++;

			if(curIsD) ++ wordsSumD;
			else ++wordsSumS;
			if(textPixelDX * wordsSumD + textPixelSX * wordsSumS > m_maxPixelWidth)
			{
				m_maxPixelWidth = textPixelDX * wordsSumD + textPixelSX * wordsSumS;
			}
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
			{
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
			}
		}else
		{
			int left,right,top,bottom;
			//�����ж�y�����Ƿ񳬳���Χ��Ŀǰ��δʵ��
		//	if(m_clipRect.top <= 0)
		//	{
		//		top = 0;
		//	}else
		//	{
		//		top = m_clipRect.top;
		//	}
			float tempY = 0.0f;
			switch(m_alignmentModeY)
			{
			case FONT_ALIGNMENT_MODE_Y_UP://����룬ɶҲ������
				tempY = (m_size.y + m_distanceY) * m_lineSum;
				break;
			case FONT_ALIGNMENT_MODE_Y_MIDDLE://���У�ÿ���������ư�����
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum * 0.5f);
				break;
			case FONT_ALIGNMENT_MODE_Y_DOWN://�Ҷ��룬ÿ�����������������
				tempY = (m_size.y + m_distanceY) * (m_lineSum - allLineSum);
				break;
			}
			//float tempY = (m_size.y + m_distanceY) * m_lineSum;	//������ߵľ���
			if(m_clipRect.bottom < tempY + m_distance)
			{//����֮�²���ʾ
				if(curIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			//if(m_clipRect.top >= tempY + (m_size.y + m_distanceY))
			if(m_clipRect.top >= tempY + m_size.y)
			{//����֮�ϲ���ʾ
				if(curIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			//�����ж�x�����Ƿ񳬳���Χ�����������Χ����ʾ
			float tempX = 0.0f;
			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT://����룬ɶҲ������
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE://���У�ÿ���������ư�����
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - 
					(m_size.x + m_distance) * curLineWidthD * 0.5f - (m_size.x * 0.5f + m_distance) * curLineWidthS * 0.5f;
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT://�Ҷ��룬ÿ�����������������
				tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS - 
					(m_size.x + m_distance) * curLineWidthD - (m_size.x * 0.5f + m_distance) * curLineWidthS;
				break;
			}
			//float tempX = (m_size.x + m_distance) * wordsSumD + (m_size.x * 0.5f + m_distance) * wordsSumS;	//������ߵľ���
			if(tempX + m_distance >= m_clipRect.right) 
			{//�ұ�֮�Ҳ���ʾ
				if(curIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;	//�ַ�Χ���治��Ҫ������
			}
		//	if((curIsD == 0 && temp + (m_size.x * 0.5 + m_distance) <= m_clipRect.left) 
			//if((curIsD == 0 && temp + (m_size.x + m_distance) <= m_clipRect.left) //������ʵ��Χ��ֱ�Ӳ�����
			//	|| (curIsD == 1 && temp + (m_size.x + m_distance) <= m_clipRect.left))	//??
			//if(tempX + (m_size.x + m_distance) <= m_clipRect.left)
			if(tempX + m_size.x <= m_clipRect.left)
			{//���֮����ʾ
				if(curIsD) ++ wordsSumD;
				else ++wordsSumS;
				continue;
			}
			if(tempY >= m_clipRect.top && tempY + (m_size.y + m_distanceY) <= m_clipRect.bottom)
			{//��Ҫȫ���軭
				top = bottom = 0;
			}else
			{
				if(tempY < m_clipRect.top) top = m_clipRect.top - tempY;
				else top = 0;
 				if(tempY + (m_size.y + m_distanceY) > m_clipRect.bottom)
					bottom = tempY + (m_size.y + m_distanceY) - m_clipRect.bottom;
				else bottom = 0;
			}
			//��Ӣ���ַ��ü���ʱ���������⣬����������ַ����ﲻ��������
			if(tempX >= m_clipRect.left && tempX + (m_size.x + m_distance) <= m_clipRect.right)
			{//��Ҫȫ���軭
				left = right = 0;
			}else
			{
				if(tempX < m_clipRect.left) left = m_clipRect.left - tempX;
				else left = 0;
				if(tempX + (m_size.x + m_distance) > m_clipRect.right)
					right = tempX + (m_size.x + m_distance) - m_clipRect.right;
				else right = 0;
			}
			m_textRect[m_needShowTextSum].set(cur_x + left,cur_y + top,cur_x + m_size.x - right,cur_y + m_size.y - bottom);
			switch(m_alignmentModeX)
			{
			case FONT_ALIGNMENT_MODE_X_LEFT:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_MIDDLE:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD * 0.5f - textPixelSX * curLineWidthS * 0.5f));
					break;
				}
				break;
			case FONT_ALIGNMENT_MODE_X_RIGHT:
				switch(m_alignmentModeY)
				{
				case FONT_ALIGNMENT_MODE_Y_UP:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * m_lineSum
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * m_lineSum
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS));
					break;
				case FONT_ALIGNMENT_MODE_Y_MIDDLE:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum * 0.5f)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS));
					break;
				case FONT_ALIGNMENT_MODE_Y_DOWN:
					m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
						- m_angleSin * textPixelY * (m_lineSum - allLineSum)
						+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS),
						m_position.y + m_angleCos * top * m_scale.x 
						+ m_angleCos * textPixelY * (m_lineSum - allLineSum)
						+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS - textPixelDX * curLineWidthD - textPixelSX * curLineWidthS));
					break;
				}
				break;
			}
		//	m_textPosition[m_needShowTextSum].set(m_position.x - m_angleSin * top * m_scale.y 
		//		- m_angleSin * textPixelY * m_lineSum
		//		+ m_angleCos * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS),
		//		m_position.y + m_angleCos * top * m_scale.x 
		//		+ m_angleCos * textPixelY * m_lineSum
		//		+ m_angleSin * (left * m_scale.x + textPixelDX * wordsSumD + textPixelSX * wordsSumS));
			m_textPageOrder[m_needShowTextSum] = pageNumble;
			++m_needShowTextSum;

			if(curIsD) ++ wordsSumD;
			else ++wordsSumS;
			if(textPixelDX * wordsSumD + textPixelSX * wordsSumS > m_maxPixelWidth)
				m_maxPixelWidth = textPixelDX * wordsSumD + textPixelSX * wordsSumS;
			if(textPixelY * (m_lineSum + 1) > m_maxPixelHeight)
				m_maxPixelHeight = textPixelY * (m_lineSum + 1);
		}
	}
}
XFontUnicode& XFontUnicode::operator = (const XFontUnicode& temp)
{
	if(this == &temp) return *this;		//��ֹ��������
	if(!temp.m_isInited) return *this;	//���Ŀ��û�г�ʼ����ֱ���˳�
	
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		//for(int i = 0;i < m_pageSum;++ i)
		//{
		//	m_texture[i].release();
		//}
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);

		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;
	//release();

	XFontBasic::operator =(temp);
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif

	//����ĸ����������Լ�����Դ����������Լ�����Դ����Ҫ�ͷŵ�
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
		
	m_pageSum = temp.m_pageSum;

	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;
	m_textPageOrder =temp.m_textPageOrder;

	m_layout = temp.m_layout;		
	return *this;
}
XBool XFontUnicode::setACopy(const XFontUnicode& temp)
{
	if(this == &temp) return XFalse;		//��ֹ��������
	if(!temp.m_isInited) return XFalse;	//���Ŀ��û�г�ʼ����ֱ���˳�

	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		//for(int i = 0;i < m_pageSum;++ i)
		//{
		//	m_texture[i].release();
		//}
		XMem::XDELETE_ARRAY(m_texture);
		XMem::XDELETE_ARRAY(m_fontPageText);

		XMem::XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	//release();
	if(!XFontBasic::setACopy(temp)) return XFalse;
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif

	//����ĸ����������Լ�����Դ����������Լ�����Դ����Ҫ�ͷŵ�
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
		
	m_pageSum = temp.m_pageSum;

	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;
	m_textPageOrder =temp.m_textPageOrder;

	m_layout = temp.m_layout;		
	return XTrue;
}
XFontUnicode::XFontUnicode(const XFontUnicode& temp)
:XFontBasic(temp)
{
	//if(this == &temp) return;		//��ֹ��������
#if WITH_OBJECT_MANAGER	//��������������ע����Щ���
	XObjManager.decreaseAObject(&m_sprite);
	XObjManager.decreaseAObject(&m_fboSprite);
	XObjManager.addAObject(this);
#endif
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	
//	m_isAcopy = 1;

	m_pageW = temp.m_pageW;			
	m_pageH = temp.m_pageH;

	m_fontPageText = temp.m_fontPageText;	//*
	m_texture = temp.m_texture;				//*						
	m_libFontSum = temp.m_libFontSum;		
	
	m_pageSum = temp.m_pageSum;

	m_textPosition = temp.m_textPosition;
	m_textRect = temp.m_textRect;
	m_textPageOrder =temp.m_textPageOrder;

	m_layout = temp.m_layout;		
}
void XFontUnicode::setMaxStrLen(int maxStrLen)
{
	char *tempXtr = m_string;	//����ɵ�����
//	XMem::XDELETE_ARRAY(m_string);

	if(maxStrLen < 2) m_maxStringLen = 2;
	else m_maxStringLen = maxStrLen;
	m_string = XMem::createArrayMem<char>(m_maxStringLen);
	m_string[0] = '\0';
	m_textPosition.resize(m_maxStringLen);
	m_textRect.resize(m_maxStringLen);
	m_textPageOrder.resize(m_maxStringLen);

	this->setString(tempXtr);
	XMem::XDELETE_ARRAY(tempXtr);

	m_needUpdateData = XTrue;
}
#if !WITH_INLINE_FILE
#include "XFontUnicode.inl"
#endif
}