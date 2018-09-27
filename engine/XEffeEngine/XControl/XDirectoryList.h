#ifndef _JIA_XDIRECTORYLIST_
#define _JIA_XDIRECTORYLIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XSlider.h"
#include "XEdit.h"
#include "XCheck.h"
#include "XButton.h"
#include "XDirectory.h"
namespace XE{
//�������ڽ����У���δ��������
//����һ����ʵĿ¼��Ϣ�Ŀؼ����࣬�������һ�������һ����ť��2���϶�����1��չʾ�����
class XDirListSkin
{
private:
	XBool m_isInited;

	bool loadFromFolder(const char *filename,XResPos resPos);	//���ļ�����������Դ
	bool loadFromPacker(const char *filename,XResPos resPos);	//��ѹ������������Դ
	bool loadFromWeb(const char *filename,XResPos resPos);		//����ҳ�ж�ȡ��Դ
public:
	XTextureData *dirListNormal;			//��ͨ״̬
	XTextureData *dirListDisable;			//��Ч״̬

	XRect m_mouseRect;			//������Ӧ��Χ

	XDirListSkin();
	~XDirListSkin(){release();}
	XBool init(const char *normal,const char *disable,XResPos resPos = RES_SYS_DEF);
	XBool initEx(const char *filename,XResPos resPos = RES_SYS_DEF);
	void release();
};
//Ŀ¼�б���һ����Ϣ
class XDirListOneLine
{
public:
	XFontUnicode m_font;
	char *m_string;		//�ַ�����
	XCheck *m_check;
	XBool m_needCheck;
	XBool m_isEnable;		//�Ƿ���Ч

	//XVec2 m_pos;		//��ǰ�����ڵ�λ��
	XFileInfo *m_file;			//ָ���ļ���Ϣ��ָ��
	XDirListOneLine()
		:m_isEnable(XFalse)
		,m_needCheck(XFalse)
		,m_check(NULL)
		,m_string(NULL)
		,m_file(NULL)
	{}
	~XDirListOneLine(){release();}
	void release();
};
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (MIN_FONT_CTRL_SIZE)		//Ĭ�ϵĻ��������
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_DIRLIST_BT_SIZE
#define DEFAULT_DIRLIST_BT_SIZE (MIN_FONT_CTRL_SIZE + 8.0f)
#endif	//DEFAULT_DIRLIST_BT_SIZE
#ifndef DEFAULT_DIRLIST_CK_SIZE
#define DEFAULT_DIRLIST_CK_SIZE (MIN_FONT_CTRL_SIZE - 4.0f)
#endif	//DEFAULT_DIRLIST_CK_SIZE
class XDirectoryList:public XControlBasic
{
private:
	XBool m_isInited;	//�Ƿ��ʼ��

	XDirectory m_dir;	//���ڽ���·��

	XSprite m_spriteBackGround;
	const XTextureData *m_dirListNormal;
	const XTextureData *m_dirListDisable;

	XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	XSlider m_verticalSlider;		//��ֱ������
	XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	XSlider m_horizontalSlider;	//ˮƽ������
	XButton m_button;	//��ť
	XEdit m_edit;
	XCheck m_check;

	XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������
	
	int m_curLineSum;				//��ǰһ��ӵ�ж�����
	int m_mouseTime;				//��������ʱ���ж��Ƿ�˫��

	int m_maxLineWidth;				//��ǰ������п�[����]
	int m_curLineLeft;				//��ǰ�ļ��[����]
	int m_canShowMaxLineWidth;		//������ʾ������п�[����]

	std::vector<XDirListOneLine *> m_lineData;	//ÿһ�е�����
	int m_showPixWidth;				//��ʾ��������ؿ��
	int m_showPixHight;				//��ʾ��������ظ߶�

	XFontUnicode m_caption;
	XVec2 m_fontSize;
	XFColor m_textColor;			//���ֵ���ɫ
	float m_curTextWidth;			//��ǰ��������
	float m_curTextHeight;			//��ǰ������߶�
	void furlFolder(int index,XBool flag = XTrue);		//��£ĳ���ļ���,�ڿؼ��ص���flag = XTrue;
	void updateHSliderState();	//����ˮƽ����״̬
	void updateShowPosition();	//������ʾ��״̬

	//�������ڲ��ؼ��Ļص�����
	static void ctrlProc(void *,int,int);

	XResourceInfo *m_resInfo;
	XBool m_withoutTex;	//û����ͼ����ʽ

	XBool m_canChangePath;
public:
	XBool getCanChangePath() const {return m_canChangePath;}
	void setCanChangePath(XBool flag);
public:
	XBool init(const XVec2& position,
		const XRect& Area,	
		XDirListSkin & tex,
		const XFontUnicode& font,
		float fontSize,
		const XCheck &check,
		const XButton &button,
		const XEdit &edit,
		const XSlider &vSlider,	//��ֱ������
		const XSlider &hSlider);
	XBool initEx(const XVec2& position,	//������ӿڵļ�
		XDirListSkin & tex,
		const XFontUnicode& font,
		float fontSize,
		const XCheck &check,
		const XButton &button,
		const XEdit &edit,
		const XSlider &vSlider,
		const XSlider &hSlider);
	XBool initPlus(const char * path,
		const XFontUnicode& font,float fontSize = 1.0f,
		XResPos resPos = RES_SYS_DEF);
	XBool initWithoutSkin(const XRect& area,	
		const XFontUnicode& font,float fontSize = 1.0f);
	XBool initWithoutSkin(const XRect& area) {return initWithoutSkin(area,getDefaultFont(),1.0f);}
	XBool initWithoutSkin(const XVec2& pixelSize) 
	{
		return initWithoutSkin(XRect(XVec2::zero,pixelSize),getDefaultFont(),1.0f);
	}
	const char * getSelectFileName() const;	//��ȡȫ·��
	int getSelectLineOrder() const;
protected:
	//��������������Ҫ�����Ż���Ҳ���Խ����Ż�
	void draw();
	void drawUp();
	void update(float stepTime);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);					//������궯������Ӧ����
	XBool keyboardProc(int keyOrder,XKeyState keyState);
	void insertChar(const char *,int) {;}
	XBool canGetFocus(const XVec2& p);//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2& ){return XTrue;}
public:
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;		//���⸲�ǵ�����
	void setScale(const XVec2& s);			//���óߴ�

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p);						//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);						//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using XObjectBasic::setColor;		//���⸲�ǵ�����
	virtual void setColor(const XFColor& c);
	virtual void setAlpha(float a);
	//virtual void justForTest() {;}

	void release();
	XDirectoryList()
		:m_isInited(XFalse)
		,m_resInfo(NULL)
		,m_withoutTex(XFalse)
		,m_canChangePath(XTrue)
	{
		m_ctrlType = CTRL_OBJ_DIRECTORYLIST;
	}
	~XDirectoryList(){release();}
};
#if WITH_INLINE_FILE
#include "XDirectoryList.inl"
#endif
}
#endif