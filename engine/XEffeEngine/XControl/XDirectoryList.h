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
#include "XMedia/XDirectory.h"
//�������ڽ����У���δ��������
//����һ����ʵĿ¼��Ϣ�Ŀؼ����࣬�������һ�������һ����ť��2���϶�����1��չʾ�����
class _XDirListTexture
{
private:
	_XBool m_isInited;
public:
	_XTextureData *dirListNormal;			//��ͨ״̬
	_XTextureData *dirListDisable;			//��Ч״̬

	_XRect m_mouseRect;			//������Ӧ��Χ

	_XDirListTexture();
	~_XDirListTexture(){release();}
	_XBool init(const char *normal,const char *disable,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
//Ŀ¼�б���һ����Ϣ
class _XDirListOneLine
{
public:
	_XFontUnicode m_font;
	char *m_string;		//�ַ�����
	_XCheck *m_check;
	_XBool m_needCheck;
	_XBool m_isEnable;		//�Ƿ���Ч

	//_XVector2 m_pos;		//��ǰ�����ڵ�λ��
	_XFile *m_file;			//ָ���ļ���Ϣ��ָ��
	_XDirListOneLine()
		:m_isEnable(XFalse)
		,m_needCheck(XFalse)
		,m_check(NULL)
		,m_string(NULL)
		,m_file(NULL)
	{}
	~_XDirListOneLine(){release();}
	void release();
};
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//Ĭ�ϵĻ��������
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_DIRLIST_BT_SIZE
#define DEFAULT_DIRLIST_BT_SIZE (40)
#endif	//DEFAULT_DIRLIST_BT_SIZE
#ifndef DEFAULT_DIRLIST_CK_SIZE
#define DEFAULT_DIRLIST_CK_SIZE (28)
#endif	//DEFAULT_DIRLIST_CK_SIZE
class _XDirectoryList:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��

	_XDirectory m_dir;	//���ڽ���·��

	_XSprite m_spriteBackGround;
	const _XTextureData *m_dirListNormal;
	const _XTextureData *m_dirListDisable;

	_XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	_XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������
	_XButton m_button;	//��ť
	_XEdit m_edit;
	_XCheck m_check;

	_XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������
	
	int m_nowLineSum;				//��ǰһ��ӵ�ж�����
	int m_mouseTime;				//��������ʱ���ж��Ƿ�˫��

	int m_maxLineWidth;				//��ǰ������п�[����]
	int m_nowLineLeft;				//��ǰ�ļ��[����]
	int m_canShowMaxLineWidth;		//������ʾ������п�[����]

	std::vector<_XDirListOneLine *> m_lineData;	//ÿһ�е�����
	int m_showPixWidth;				//��ʾ��������ؿ��
	int m_showPixHight;				//��ʾ��������ظ߶�

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_nowTextWidth;			//��ǰ��������
	float m_nowTextHeight;			//��ǰ������߶�
	void furlFolder(int index,_XBool flag = XTrue);		//��£ĳ���ļ���,�ڿؼ��ص���flag = XTrue;
	void updateHSliderState();	//����ˮƽ����״̬
	void updateShowPosition();	//������ʾ��״̬

	//�������ڲ��ؼ��Ļص�����
	friend void funDirListButton(void *pClass,int objectID);
	friend void funDirListValueChangeV(void *pClass,int objectID);
	friend void funDirListValueChangeH(void *pClass,int objectID);
	friend void funDirListCheck(void *pClass,int objectID);

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ

	_XBool m_canChangePath;
public:
	_XBool getCanChangePath() const {return m_canChangePath;}
	void setCanChangePath(_XBool flag);
public:
	_XBool init(const _XVector2& position,
		const _XRect& Area,	
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider);
	_XBool initEx(const _XVector2& position,	//������ӿڵļ�
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,
		const _XSlider &hSlider);
	_XBool initPlus(const char * path,
		_XFontUnicode &font,float fontSize = 1.0f,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,	
		_XFontUnicode &font,float fontSize = 1.0f);
	_XBool initWithoutTex(const _XRect& area) {return initWithoutTex(area,XEE::systemFont,1.0f);}
	_XBool initWithoutTex(const _XVector2& pixelSize) 
	{
		return initWithoutTex(_XRect(0.0f,0.0f,pixelSize.x,pixelSize.y),XEE::systemFont,1.0f);
	}
	const char * getSelectFileName() const;	//��ȡȫ·��
	int getSelectLineOrder() const;
protected:
	//��������������Ҫ�����Ż���Ҳ���Խ����Ż�
	void draw();
	void drawUp();
	void update(int stepTime);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *,int) {;}
	_XBool canGetFocus(float x,float y);//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);			//���óߴ�

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);						//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);						//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	virtual void setColor(float r,float g,float b,float a)
	{
		m_color.setColor(r,g,b,a);
		m_caption.setColor(m_color);
		m_spriteBackGround.setColor(m_color);
		m_verticalSlider.setColor(m_color);
		m_horizontalSlider.setColor(m_color);
		m_button.setColor(m_color);
		m_edit.setColor(m_color);
		m_check.setColor(m_color);
		for(int i = 0;i < m_lineData.size();++ i)
		{
			m_lineData[i]->m_font.setColor(m_color);
			if(m_lineData[i]->m_check != NULL) m_lineData[i]->m_check->setColor(m_color); 
		}
		updateChildColor();
	}
	virtual void setAlpha(float a)
	{
		m_color.setA(a);
		m_caption.setAlpha(a);
		m_spriteBackGround.setAlpha(a);
		m_verticalSlider.setAlpha(a);
		m_horizontalSlider.setAlpha(a);
		m_button.setAlpha(a);
		m_edit.setAlpha(a);
		m_check.setAlpha(a);
		for(int i = 0;i < m_lineData.size();++ i)
		{
			m_lineData[i]->m_font.setAlpha(a);
			if(m_lineData[i]->m_check != NULL) m_lineData[i]->m_check->setAlpha(a); 
		}
		updateChildAlpha();
	}
	//virtual void justForTest() {;}

	void release();
	_XDirectoryList()
		:m_isInited(XFalse)
		,m_resInfo(NULL)
		,m_withoutTex(XFalse)
		,m_canChangePath(XTrue)
	{
		m_ctrlType = CTRL_OBJ_DIRECTORYLIST;
	}
	~_XDirectoryList(){release();}
};
#include "XDirectoryList.inl"

#endif