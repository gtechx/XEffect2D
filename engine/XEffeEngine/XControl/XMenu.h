#ifndef _JIA_XMENU_
#define _JIA_XMENU_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.4
//--------------------------------
#include "XControlBasic.h"
namespace XE{
//��ʼ�����ⲿ�ֵĹ��������ƹ�����δʵ��
//�޸Ľ���
//1���������ϲ˵����������				*
//2�����Ӳ˵��Ĳ˵�����Ҫ��������		*
//3������֧��״̬��˵���				*
//4��֧��ͼ��							*
//5��֧����Ч�˵���						*
//6��֧�ֿ�ݼ������ȼ��̲���			* (�������)
//7��֧�ֲ˵��������ڱ߽�ʱ������Ӧ����
//8��֧�ֶ�̬�޸Ĳ˵������ʾ����		*
//9���Ż���������Ч��
//10���������ڵ��������ж�����
//11�����ӷָ���						*
//12�����ǹ�������ʵ��					+�Ѿ��з���
//13�����˵�����ʱ���Ӳ˵����·���֧��
//14���˵���ʽ�Ӹ��߿�

enum XMenuType
{//�˵�������
	MENU_TYPE_VERTICAL,		//��ֱ��
	MENU_TYPE_HORIZONTA,	//ˮƽ��
};
class XMenuItem;
class XMenuGroup	//һ���˵���
{
public:
	XVec2 m_insertPos;	//����˵����λ��
	int m_width;				//��Ŀ��
	XMenuType m_type;
	XMenuItem *m_parent;		//�������ָ��
	std::vector<XMenuItem *> m_items;
	void draw();
	XMenuGroup()
		:m_width(-1)	//�������峤������ȷ��
		,m_type(MENU_TYPE_HORIZONTA)
		,m_parent(NULL)
		,m_chooseIndex(-1)
	{}
	~XMenuGroup()
	{
		for(unsigned int i = 0;i < m_items.size();++ i)
		{
			XMem::XDELETE(m_items[i]);
		}
		m_items.clear();
	}
	void setOpen();	//���ô�
	void disOpen();
	XBool isInRect(const XVec2& p);
	XMenuItem * getItem(const XVec2& p);	//��ȡ��ǰ��x��yλ�õ������ָ��
	XMenuItem * getItemByNameInGroup(const std::string& name);
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	void postPross();
	void updatePos();	//�������λ�ñ仯
	void updateSize();	//����������ű仯
	void updateColor();
	void setHotChooseActive(XBool flag);
	XMenuItem *getItemByHotChooseKey(char k);
	XMenuItem *getItemByHotKey(char k);
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�������������¼����ƵĴ���
	int m_chooseIndex;
	void addIndex();//���ӱ��	
	void desIndex();	//���ٱ��
	void initIndex(XBool orderFlag = XTrue);	//��ʼ����� orderFlag�Ƿ������ʼ��
	void resetChoose();	//ȡ��ѡ��
	void openChoose();	//��ѡ�����
	void closeChoose();	//�ر�ѡ�����
	XBool setChooseItem(const XMenuItem *it);
	XMenuItem *getChooseItem();	//��ȡ��ǰ���б�ѡ��Ĳ˵���
	//-----------------------------------------------------
};
enum XMenuItemType
{//��δ���
	MENU_ITEM_TYPE_BTN,	//������Ĳ˵���
	MENU_ITEM_TYPE_CHK,	//ѡ����Ĳ˵���
	MENU_ITEM_TYPE_DLT,	//�ָ�����Ĳ˵���
};
class XMenuItem	//һ���˵���
{
public:
	static const int m_maxMenuItemLength = 1024;	//�˵������ֵ���󳤶�

	int m_levelIndex;		//�����Ĳ�εı��
	XVec2 m_position;	//�ŵ�λ��
	XVec2 m_scaleSize;	//���Ŵ�С
	XVec2 m_size;		//���سߴ��С
	XMenuGroup *m_owner;			//��������
	XMenuGroup *m_child;			//�������ָ��
	std::string m_originalName;		//ԭʼ�����ƣ������ھ���Ψһ��,��󳤶��ܵ�m_maxMenuItemLength����
	std::string m_showName;			//��ʾ������
	XMenuItemType m_type;			//�˵��������
	XBool m_isOpen;	//�Ƿ��ڴ�״̬
	XBool m_isChecked;				//�Ƿ񱻵��	//ѡ����Ŀؼ������������ѡ��
	XFColor m_color;
	//void changeCheckedState();		//�ı�ѡ��״̬
	void setCheckState(XBool state);	//����ѡ����˵����ѡ��״̬
	void setString(const std::string& name);
	void setPosition(const XVec2& p);
	void setScale(const XVec2& s);
	void setColor(const XFColor& c);
	//++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ���޳��԰�ť��ʹ�ö�����ı���
	XFontUnicode m_font;			//��ʾ������
	XBool m_isEnable;				//�Ƿ���Ч״̬
	XBool m_isVisible;				//�Ƿ�ɼ�
	XBool m_isMouseOn;				//�Ƿ����������״̬
	XBool mouseProc(const XVec2& p,XMouseState mouseState);	//�����Ƿ񴥷����¼�
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//��������icon��֧��
	XBool m_withIcon;	//�Ƿ���ͼ���֧��
	XSprite m_spriteIcon;	//icon����ͼ
	XBool setIcon(const char *filename,XResPos res = RES_SYS_DEF);
	void disIcon();	//ȡ��Icon
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ�ּ��̲���������ı�����2014��7��30�� ע�⣺��������̫����ʾ�ؼ����㲻��֧���ȼ�����
	char m_hotKey;			//��ǰ�˵���Ŀ�ݼ�,ctrl + m_hotkeyʹ�ò���ֱ����Ч
	char m_hotChooseKey;	//����alt֮����m_hotChooseKey���ǰ�˵�
	XBool m_isHotChooseActive;
	XBool setHotKey(char k);	//���ÿ�ݼ�//-1Ϊ��Ч
	XBool setHotChooseKey(char k);	//���ÿ�ѡ��//-1Ϊ��Ч
	XMenuItem *getItemByHotKey(char k);//ͨ����ݼ������Ҳ˵���
	XMenuItem *getItemByHotChooseKey(char k);//���Ӳ˵���Ѱ��ָ�����ȼ��Ĳ˵���
	void setHotChooseActive(XBool flag);//�����ȼ�����״̬
	//----------------------------------------------
private:
	void (*m_callbackFun)(void *pClass,const std::string& pos);
	void *m_pClass;
public:
	XMenuItem()
		:m_owner(NULL)
		,m_child(NULL)
		,m_isOpen(XFalse)
		,m_position(0.0f)
		,m_scaleSize(1.0f)
		,m_size(0.0f)
		,m_callbackFun(NULL)
		,m_pClass(NULL)
		,m_isEnable(XTrue)
		,m_isVisible(XTrue)
		,m_isMouseOn(XFalse)
		,m_levelIndex(0)
		,m_type(MENU_ITEM_TYPE_BTN)
		,m_withIcon(XFalse)
		,m_hotKey(-1)
		,m_hotChooseKey(-1)
		,m_isHotChooseActive(XFalse)
		,m_color(1.0f,1.0f)
	{}
	~XMenuItem(){XMem::XDELETE(m_child);}
	void draw();
	void setCallbackFun(void (*callbackFun)(void *,const std::string& ),void *p);	//���ûص����������˵������ʱ�������صĻص�����
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string& ),void *p);	//�������е�������Ļص�����
	void beTriggered();	//�˵������ʱ�����������
	std::string getFullStr() const;	//��ȡ��ǰ�˵����ȫ·��

	XBool init(const std::string& name,const XFontUnicode& font,float fontsize,XMenuGroup *owner,XMenuItemType type = MENU_ITEM_TYPE_BTN);
	void setOpen();	//���ô�
	void disOpen();	//���ùر�
	XBool isInRect(const XVec2& p);
	XMenuItem * getItem(const XVec2& p);
	void postPross();
	void updateStr();	//�����ַ�������ʾ
	void resetChoose();
};
class XMenu:public XControlBasic
{
private:
	XBool m_isInited;
	XMenuItem m_rootItem;	//����
	XMenuType m_type;		//���˵�������

	XFontUnicode m_font;	//����
	float m_fontSize;
//	XVec2 m_position;	//λ��
//	XVec2 m_scale;
	XBool m_isClicked;		//�Ƿ񱻵��
	XMenuItem *m_oldChooseItem;	//֮ǰѡ���ѡ��
	XBool m_needPostProcess;	//	�Ƿ���Ҫ���ڴ���

	void postProcess();	//���ڴ���Ĺ�������״̬�����ı�֮�������ı��״̬
	std::string m_curChooseMenuStr;	//��ǰѡ��Ĳ˵��ַ���
	static void ctrlProc(void *,const std::string& );
public:
	enum XMenuEvent
	{
		MNU_CHOOSE,	//������ѡ���¼�
	};
	const std::string& getCurChooseMenuStr(){return m_curChooseMenuStr;}	//��ȡ��ǰѡ��Ĳ˵������ַ���
public:
	//ʹ��";"��Ϊ�˵���ķָ��
	XBool addAItem(const std::string& pos,const std::string& name,int width = -1,XMenuItemType type = MENU_ITEM_TYPE_BTN);
	XBool getCheckState(const std::string& pos);
	XBool setCheckState(const std::string& pos,XBool state);
	XBool setDisable(const std::string& pos);
	XBool setEnable(const std::string& pos);
	XBool getEnable(const std::string& pos);
	XBool setMenuItemStr(const std::string& pos,const std::string& name);	//�޸Ĳ˵�������֣����в˵�������ֱ���ҪΨһ�����������߼�����
	XBool setItemIcon(const std::string& pos,const char *filename,XResPos res = RES_SYS_DEF);
	XBool setHotKey(const std::string& pos,char k);
	char getHotKey(const std::string& pos);
	XBool setHotChooseKey(const std::string& pos,char k);
	char getHotChooseKey(const std::string& pos);
	XMenuItem * getItemByName(const std::string& name);
	const std::string getItemFullStr(const XMenuItem * it);
private:
	XMenuItem * getItemFromGroup(char *p,XMenuGroup *g);	//����g�в�������Ϊp�Ĳ˵���
	XBool getIsPath(XMenuItem *s,XMenuItem *d);	//�ж�s�Ƿ���d��·����

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԼ��̲�����һЩ����
	XBool m_hotChooseKeyIsActive;	//�Ƿ񼤻��ȼ�
	//------------------------------------------------------
	void closeAll();	//�۵����еĲ˵���
public:
//	XBool setCBFunction(const std::string& pos,void (*callbackFun)(void *,const std::string& ),void *p);//���ò˵���Ӧ�Ļص�����
//	XBool setAllCBFunction(void (*callbackFun)(void *,const std::string& ),void *p);	//����������Ļص�����

	XBool initWithoutSkin(const XFontUnicode& font,float captionSize = 1.0f,XMenuType type = MENU_TYPE_HORIZONTA);
	XBool initWithoutSkin(XMenuType type = MENU_TYPE_HORIZONTA) {return initWithoutSkin(getDefaultFont(),1.0f,type);}
protected:
	void draw();
	void drawUp(){};							//do nothing
	XBool mouseProc(const XVec2& p,XMouseState mouseState);
	XBool keyboardProc(int keyOrder,XKeyState keyState);

	void insertChar(const char *,int){;}
	XBool canGetFocus(const XVec2& p);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	XBool canLostFocus(const XVec2&){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	XMenu()
		:m_isInited(XFalse)
		,m_isClicked(XFalse)
		,m_oldChooseItem(NULL)
		,m_needPostProcess(XFalse)
		,m_hotChooseKeyIsActive(XFalse)
		,m_type(MENU_TYPE_HORIZONTA)
	{
		m_ctrlType = CTRL_OBJ_MENU;
	}
	~XMenu(){release();}
	void release();
	//Ϊ���������Ŀؼ��ӹ죬����Ҫʵ������Ľӿ�
	using XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(const XVec2& p);

	using XObjectBasic::setScale;	//���⸲�ǵ�����
	void setScale(const XVec2& s);

	void setTextColor(const XFColor& color);	//�����������ɫ
	const XFColor& getTextColor() const {return XFColor::white;}	//��ȡ�ؼ��������ɫ

	using XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(const XFColor& c);
	void setAlpha(float a);

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	XBool isInRect(const XVec2& p) {return m_rootItem.isInRect(p);}		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	XVec2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	XMenu(const XMenu &temp);
	XMenu& operator = (const XMenu& temp);
};
#if WITH_INLINE_FILE
#include "XMenu.inl"
#endif
}
#endif