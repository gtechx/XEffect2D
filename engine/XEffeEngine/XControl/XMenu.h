#ifndef _JIA_XMENU_
#define _JIA_XMENU_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.7.4
//--------------------------------
#include "XControlBasic.h"
#include "../XBasicWindow.h"

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

#define MAX_MENU_TEXT_LENGTH (1024)	//�˵������ֵ���󳤶�
enum _XMenuType
{//�˵�������
	MENU_TYPE_VERTICAL,		//��ֱ��
	MENU_TYPE_HORIZONTA,	//ˮƽ��
};
class _XMenuItem;
class _XMenuGroup	//һ���˵���
{
public:
	_XVector2 m_insertPos;	//����˵����λ��
	int m_width;				//��Ŀ��
	_XMenuType m_type;
	_XMenuItem *m_parent;		//�������ָ��
	std::vector<_XMenuItem *> m_items;
	void draw();
	_XMenuGroup()
		:m_width(-1)	//�������峤������ȷ��
		,m_type(MENU_TYPE_HORIZONTA)
		,m_parent(NULL)
		,m_chooseIndex(-1)
	{}
	~_XMenuGroup()
	{
		for(int i = 0;i < m_items.size();++ i)
		{
			XDELETE(m_items[i]);
		}
		m_items.clear();
	}
	void setOpen();	//���ô�
	void disOpen();
	_XBool isInRect(float x,float y);
	_XMenuItem * getItem(float x,float y);	//��ȡ��ǰ��x��yλ�õ������ָ��
	_XMenuItem * getItemByNameInGroup(const std::string &name);
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	void postPross();
	void updatePos();	//�������λ�ñ仯
	void updateSize();	//����������ű仯
	void updateColor();
	void setHotChooseActive(_XBool flag);
	_XMenuItem *getItemByHotChooseKey(char k);
	_XMenuItem *getItemByHotKey(char k);
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�������������¼����ƵĴ���
	int m_chooseIndex;
	void addIndex();//���ӱ��	
	void desIndex();	//���ٱ��
	void initIndex(_XBool orderFlag = XTrue);	//��ʼ����� orderFlag�Ƿ������ʼ��
	void resetChoose();	//ȡ��ѡ��
	void openChoose();	//��ѡ�����
	void closeChoose();	//�ر�ѡ�����
	_XBool setChooseItem(const _XMenuItem *it);
	_XMenuItem *getChooseItem();	//��ȡ��ǰ���б�ѡ��Ĳ˵���
	//-----------------------------------------------------
};
enum _XMenuItemType
{//��δ���
	MENU_ITEM_TYPE_BTN,	//������Ĳ˵���
	MENU_ITEM_TYPE_CHK,	//ѡ����Ĳ˵���
	MENU_ITEM_TYPE_DLT,	//�ָ�����Ĳ˵���
};
class _XMenuItem	//һ���˵���
{
public:
	int m_levelIndex;		//�����Ĳ�εı��
	_XVector2 m_position;	//�ŵ�λ��
	_XVector2 m_scaleSize;	//���Ŵ�С
	_XVector2 m_size;		//���سߴ��С
	_XBool m_isOpen;	//�Ƿ��ڴ�״̬
	_XMenuGroup *m_owner;			//��������
	_XMenuGroup *m_child;			//�������ָ��
	std::string m_originalName;		//ԭʼ�����ƣ������ھ���Ψһ��,��󳤶��ܵ�MAX_MENU_TEXT_LENGTH����
	std::string m_showName;			//��ʾ������
	_XMenuItemType m_type;			//�˵��������
	_XBool m_isChecked;				//�Ƿ񱻵��	//ѡ����Ŀؼ������������ѡ��
	_XFColor m_color;
	//void changeCheckedState();		//�ı�ѡ��״̬
	void setCheckState(_XBool state);	//����ѡ����˵����ѡ��״̬
	void setString(const std::string &name);
	void setPosition(float x,float y);
	void setSize(float x,float y);
	void setColor(const _XFColor &c);
	//++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ���޳��԰�ť��ʹ�ö�����ı���
	_XFontUnicode m_font;			//��ʾ������
	_XBool m_isEnable;				//�Ƿ���Ч״̬
	_XBool m_isVisible;				//�Ƿ�ɼ�
	_XBool m_isMouseOn;				//�Ƿ����������״̬
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//�����Ƿ񴥷����¼�
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//��������icon��֧��
	_XBool m_withIcon;	//�Ƿ���ͼ���֧��
	_XSprite m_spriteIcon;	//icon����ͼ
	_XBool setIcon(const char *filename,_XResourcePosition res = RESOURCE_SYSTEM_DEFINE);
	void disIcon();	//ȡ��Icon
	//----------------------------------------------
	//++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ�ּ��̲���������ı�����2014��7��30�� ע�⣺��������̫����ʾ�ؼ����㲻��֧���ȼ�����
	char m_hotKey;			//��ǰ�˵���Ŀ�ݼ�,ctrl + m_hotkeyʹ�ò���ֱ����Ч
	char m_hotChooseKey;	//����alt֮����m_hotChooseKey���ǰ�˵�
	_XBool m_isHotChooseActive;
	_XBool setHotKey(char k);	//���ÿ�ݼ�//-1Ϊ��Ч
	_XBool setHotChooseKey(char k);	//���ÿ�ѡ��//-1Ϊ��Ч
	_XMenuItem *getItemByHotKey(char k);//ͨ����ݼ������Ҳ˵���
	_XMenuItem *getItemByHotChooseKey(char k);//���Ӳ˵���Ѱ��ָ�����ȼ��Ĳ˵���
	void setHotChooseActive(_XBool flag);//�����ȼ�����״̬
	//----------------------------------------------
private:
	void (*m_callbackFun)(void *pClass,const std::string &pos);
	void *m_pClass;
public:
	_XMenuItem()
		:m_owner(NULL)
		,m_child(NULL)
		,m_isOpen(XFalse)
		,m_position(0.0f,0.0f)
		,m_scaleSize(1.0f,1.0f)
		,m_size(0.0f,0.0f)
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
		,m_color(1.0f,1.0f,1.0f,1.0f)
	{}
	~_XMenuItem(){XDELETE(m_child);}
	void draw();
	void setCallbackFun(void (*callbackFun)(void *,const std::string &),void *p);	//���ûص����������˵������ʱ�������صĻص�����
	void setAllCallBackFun(void (*callbackFun)(void *,const std::string &),void *p);	//�������е�������Ļص�����
	void beTriggered();	//�˵������ʱ�����������
	std::string getFullStr() const;	//��ȡ��ǰ�˵����ȫ·��

	_XBool init(const std::string &name,const _XFontUnicode &font,float fontsize,_XMenuGroup *owner,_XMenuItemType type = MENU_ITEM_TYPE_BTN);
	void setOpen();	//���ô�
	void disOpen();	//���ùر�
	_XBool isInRect(float x,float y);
	_XMenuItem * getItem(float x,float y);
	void postPross();
	void updateStr();	//�����ַ�������ʾ
	void resetChoose();
};
class _XMenu:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XMenuItem m_rootItem;	//����
	_XMenuType m_type;		//���˵�������

	_XFontUnicode m_font;	//����
	float m_fontSize;
//	_XVector2 m_position;	//λ��
//	_XVector2 m_size;
	_XBool m_isClicked;		//�Ƿ񱻵��
	_XMenuItem *m_oldChooseItem;	//֮ǰѡ���ѡ��
	_XBool m_needPostProcess;	//	�Ƿ���Ҫ���ڴ���

	void postProcess();	//���ڴ���Ĺ�������״̬�����ı�֮�������ı��״̬
public:
	//ʹ��";"��Ϊ�˵���ķָ��
	_XBool addAItem(const std::string &pos,const std::string &name,int width = -1,_XMenuItemType type = MENU_ITEM_TYPE_BTN);
	_XBool getCheckState(const std::string &pos);
	_XBool setCheckState(const std::string &pos,_XBool state);
	_XBool setDisable(const std::string &pos);
	_XBool setEnable(const std::string &pos);
	_XBool getEnable(const std::string &pos);
	_XBool setMenuItemStr(const std::string &pos,const std::string &name);	//�޸Ĳ˵�������֣����в˵�������ֱ���ҪΨһ�����������߼�����
	_XBool setItemIcon(const std::string &pos,const char *filename,_XResourcePosition res = RESOURCE_SYSTEM_DEFINE);
	_XBool setHotKey(const std::string &pos,char k);
	char getHotKey(const std::string &pos);
	_XBool setHotChooseKey(const std::string &pos,char k);
	char getHotChooseKey(const std::string &pos);
	_XMenuItem * getItemByName(const std::string &name);
	std::string getItemFullStr(const _XMenuItem * it);
private:
	_XMenuItem * getItemFromGroup(char *p,_XMenuGroup *g);	//����g�в�������Ϊp�Ĳ˵���
	_XBool getIsPath(_XMenuItem *s,_XMenuItem *d);	//�ж�s�Ƿ���d��·����

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǶԼ��̲�����һЩ����
	_XBool m_hotChooseKeyIsActive;	//�Ƿ񼤻��ȼ�
	//------------------------------------------------------
	void closeAll();	//�۵����еĲ˵���
public:
	_XBool setCBFunction(const std::string &pos,void (*callbackFun)(void *,const std::string &),void *p);//���ò˵���Ӧ�Ļص�����
	_XBool setAllCBFunction(void (*callbackFun)(void *,const std::string &),void *p);	//����������Ļص�����

	_XBool initWithoutTex(const _XFontUnicode &font,float captionSize = 1.0f,_XMenuType type = MENU_TYPE_HORIZONTA);
	_XBool initWithoutTex(_XMenuType type = MENU_TYPE_HORIZONTA) {return initWithoutTex(XEE::systemFont,1.0f,type);}
protected:
	void draw();
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);

	void insertChar(const char *,int){;}
	_XBool canGetFocus(float x,float y);				//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float,float){return XTrue;}	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus();	//����ʧȥ����
public:
	_XMenu()
		:m_isInited(XFalse)
		,m_isClicked(XFalse)
		,m_oldChooseItem(NULL)
		,m_needPostProcess(XFalse)
		,m_hotChooseKeyIsActive(XFalse)
		,m_type(MENU_TYPE_HORIZONTA)
	{
		m_ctrlType = CTRL_OBJ_XMENU;
	}
	~_XMenu(){release();}
	void release();
	//Ϊ���������Ŀؼ��ӹ죬����Ҫʵ������Ľӿ�
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return _XFColor::white;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);
	void setAlpha(float a);

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y) {return m_rootItem.isInRect(x,y);}		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	////virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XMenu(const _XMenu &temp);
	_XMenu& operator = (const _XMenu& temp);

};
#include "XMenu.inl"
#endif