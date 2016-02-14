#ifndef _JIA_XCONFIGMANAGER_
#define _JIA_XCONFIGMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XOprateDes.h"
#include <vector>
#include "XControl\XGroup.h"
#include "XInputEventCommon.h"
namespace XE{
//Ŀǰ�Ǽ�ʵ�֣�֮����Ҫ�ḻ��ǿ��������ǿ�����ݣ��ο����˵��
/*ͳһ������ϸ����ƣ�
1��ÿ����������ID
2������������
3�����û������Ϊ����
4�����Ա������ÿؼ�λ����Ϣ
5�������Զ��Ű�
6�����ݴ��ڳߴ��Զ���Ӧ
7����������ֵ�ĸı�����ָ��ͻص����ַ�ʽ
8�������齨��̬����
9���������ͣ�ȡֵ��Χ��Ĭ��ֵ����ǰֵ�����ƣ���ֵ�仯���ͣ����Ի������������Ƿ�ɱ䣬����bool������ֵ
10���������ռλ���(��OBJ)�Ա�������Щ�ط���ʾͼƬ���������Ķ�����

11������΢����ť
*/
//����һ��ͳһ������ý�����࣬�ฺ�����в�����ͼ�λ����úͱ����ȡ��Ĭ��ֵ���ݲ���
//���紫���ʽ˵��:
//GroupSum��
//Group0NameLen|Group0Name|
//GroupItemsSum|
//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefValue|ItemCurValue|ItemNameLen|ItemName|
//.......
//if(ItemType == CFG_DATA_TYPE_CUSTOM)
//ItemID|ItemType|CItemSum|CItem0ID|CItem0Type|........
enum XConfigDataType
{
	CFG_DATA_TYPE_INT,
	CFG_DATA_TYPE_CHAR,
	CFG_DATA_TYPE_UCHAR,
	CFG_DATA_TYPE_FLOAT,
	CFG_DATA_TYPE_XBOOL,	//ע�������ӦXBool���ͣ�������bool���ͣ����򽫻��п�����ɴ���
	CFG_DATA_TYPE_RADIOS,	//��ѡ��
	CFG_DATA_TYPE_CUSTOM,	//�Զ���		���粻֧��
	CFG_DATA_TYPE_XSPRITE,	//��������	���粻֧��
	CFG_DATA_TYPE_NULL,		//��Ч��������
};

#define CFG_MANAGER_W_SPACE (5.0f)
#define CFG_MANAGER_H_SPACE (5.0f)
#define CFG_MNG_H_FONT	(32.0f)
#define CFG_MNG_H_SLD	(24.0f)
//һ��������Ľṹ
union XConfigValue
{
	int valueI;
	float valueF;
	XBool valueB;
};
#define CFG_DEFAULT_FILENAME "xConfig.dat"
#define CFG_INJECT_FILENAME "xCFGInject.dat"
#define CFG_DEFAULT_GROUPNAME "Default"
//�û��Զ���ؼ��Ļ���
//����û���Ҫ�Զ���ɱ����ù����������࣬��ô�������������ಢʵ����ؽӿ�
class XCFGItemBasic:public XObjectBasic,public XBasicOprate
{
public:
	virtual bool save(FILE *fp) = 0;	//���浱ǰ����
	virtual bool load(FILE *fp) = 0;	//��ȡ��ǰ����
	virtual bool update() = 0;			//ʹ���ڲ�������������UI������
	virtual bool defValue() = 0;		//ȡĬ��ֵ
	virtual bool enable() = 0;			//ʹ������Ա��޸Ŀ���
	virtual bool disable() = 0;			//���ܱ�����

	virtual ~XCFGItemBasic() {}
	//Ϊ��֧��������ṩ�Ľӿ�
	virtual unsigned char *getInfo(int &len,int id) = 0;					//������������ͬ�����ַ�����len������
	virtual void setValueFromStr(unsigned char * str) = 0;	//�����緢�͵��ַ�������ȡ����
	virtual bool needSendStr() = 0;							//�����Ƿ���Ҫ����״̬���ͻ���
	virtual unsigned char *sendStr(int &len,int id) = 0;					//�ڲ����������ɿ��Է��͵�����
};
class XConfigItem
{
private:
	int m_ID;
public:
	bool m_isEnable;
	bool m_isActive;	//��ǰ�������Ƿ��������أ�����Ǳ����ģ�����ֵ���ܱ��ⲿ�ı�
	XConfigDataType m_type;		//�����������
	XConfigValue m_rangeMin;		//ȡֵ��Χ
	XConfigValue m_rangeMax;	
	XConfigValue m_defaultValue;	//Ĭ��ֵ
	XConfigValue m_curValue;		//��ǰֵ
	void * m_pVariable;				//Ŀ���ָ��(Ψһ)
	//void (* m_changeProc)();		//�����仯�Ļص�����
	void (* m_changeProc)(void *pObj,void *pClass);		//�����仯�Ļص�����
	void *m_pClass;
	XControlBasic *m_pCtrl;		//�ؼ���ָ��
	std::string m_name;					//��λ���������(Ψһ)
	XCFGItemBasic * m_customIt;	//�û��Զ����������(Ψһ)
	void release();	//ע�⣺����������ܷ���������������Ϊ���������е��������Ҫ�ͷ������Դ
	XConfigItem()
		:m_isEnable(false)
		,m_pVariable(NULL)
		,m_changeProc(NULL)
		,m_pClass(NULL)
		,m_pCtrl(NULL)
		,m_isActive(true)
		,m_customIt(NULL)
	{
		static int ID = 0;
		m_ID = ID ++;
	}
	~XConfigItem(){XMem::XDELETE(m_pCtrl);}
	int getID()const {return m_ID;}
	void setID(int ID){m_ID = ID;}
};
class XConfigGroup
{
private:
	void moveDownPretreatment(int pixelW,int pixelH);	//�ƶ���Ԥ�������ƶ�֮ǰ����һЩ����
	void moveDownPretreatment(const XVector2 &size){moveDownPretreatment(size.x,size.y);}
	void moveDown(int pixelW,int pixelH);	//�����ƶ������
	void moveDown(const XVector2 &size){moveDown(size.x,size.y);}
	void useANewRow();//ʹ���µ�һ��
public:
	bool m_isEnable;		//���Ƿ���Ч
	bool m_isMaxH;	//�Ƿ�ʹ�����߶�
	bool m_isNewRow;						//�Ƿ����µ�һ��
	XVector2 m_maxSize;	//��Χ���ӵĴ�С�������ֵ��û�о������ŵ����ش�С
	XVector2 m_position;	//λ��
	XVector2 m_scale;		//��С
	std::string m_name;			//����
	XGroup m_group;
	//std::vector<int> m_itemsID;	//�������ID
	std::vector<XConfigItem *> m_items;	//�������ָ��
	XVector2 m_curInsertPos;	//��ǰ�����λ��
	void relayout();	//�����Զ�����
	float m_width;
	float m_maxHeight;
	float m_maxRowWidth;					//��ǰ�е�����п�
	XVector2 calculateMaxSize();	//���㵱ǰ��ķ�Χ

	XConfigGroup()
		:m_isEnable(false)
		,m_name(CFG_DEFAULT_GROUPNAME)
		,m_position(0.0f,0.0f)
		,m_scale(1.0f,1.0f)
	{}
};
enum XConfigMode
{
	CFG_MODE_NORMAL,	//��ͨģʽ
	CFG_MODE_SERVER,	//������ģʽ
	CFG_MODE_CLIENT,	//�ͻ���ģʽ
};
enum XConfigNetOperate
{
	CFG_NET_OP_SAVE,	//����
	CFG_NET_OP_LOAD,	//��ȡ
	CFG_NET_OP_DEFAULT,	//Ĭ��
};
class XNetClient;
class XNetServer;
class XButton;
class XConfigManager//:public XDragObjectBase
{
public:
	static const int m_cfgMaxItemsSum = 10000;
public:
	XConfigManager();
	virtual ~XConfigManager();
protected:
	XConfigManager(const XConfigManager&);
	XConfigManager &operator= (const XConfigManager&);
public:
	static bool m_withConfigManager;	//�Ƿ�ʹ�������ù�����
	void setWithBackground(bool flag)
	{
		m_withBackground = flag;
		for(unsigned int i = 0;i < m_pGroups.size();++ i)
		{
			m_pGroups[i]->m_group.setWithRect(!flag);
		}
	}
	bool getWithBackground()const{return m_withBackground;}
private:
	bool m_isInited;
	bool m_isVisble;	//�Ƿ�ɼ�
	bool m_isNewRow;						//�Ƿ����µ�һ��
	bool m_withBackground;					//�Ƿ���ʾ����
	std::vector<XConfigItem *> m_pItems;	//������������б�
	std::vector<XConfigGroup *> m_pGroups;	//����б�

	//friend void callbackProcMD(void *pClass,int ID);	//��갴���¼��Ļص�����
	static void ctrlProc(void*,int,int);

	XVector2 m_position;
	XVector2 m_scale;
	float m_maxHeight;	//�߶ȣ�����ֻ���Ƹ߶�
	float m_width;		//������Ŀ��
	XVector2 m_curInsertPos;	//Ŀǰ�������ڵ�λ��
	XFColor m_textColor;	//�������ɫ

	XButton *m_saveBtn;
	XButton *m_loadBtn;
	XButton *m_defaultBtn;
	XButton *m_undoBtn;
	XButton *m_redoBtn;
	XButton *m_netUpdateBtn;	//�ӷ�����ͬ��������Ϣ
	XButton *m_netInjectBtn;	//�������ע����������

	void relayout();	//�����Զ�����
	void release();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�ͻ��˵Ĺ��ܰ���˵��:
	//���棺Ҫ��������˱������ݣ�����Ҳ��������			���
	//��ȡ��Ҫ��������˶�ȡ���ݣ���������ͬ�����ͻ���		���
	//Ĭ�ϣ�Ҫ��������˻ָ�Ĭ�����ݣ���������ͬ�����ͻ���	���
	//ע�룺�ͻ��˶�ȡ���ݣ���������ͬ������������			���
	//������������صĶ���
	XConfigMode m_configMode;
	XNetClient *m_netClient;
	XNetServer *m_netServer;

	unsigned char *getConfigInfo(int &len);			//��������Ľṹת�����ַ���
	void setOperateToServer(XConfigNetOperate op);	//�ͻ�������������Ͳ�������
	void sendSynchToServer();						//�����������ͬ������
	void sendCFGInfo();
	void sendItemValue(const XConfigItem * it);
	void sendInject();	//�����������ע����Ϣ
	void updateNet();
	void updateInfo(unsigned char *data);
	void setItemValue(int ID,const XConfigValue value);
	void updateItemToCFG(XConfigItem * it);		//���ڲ����ݸ��µ�����
	void updateItemFromCFG(XConfigItem * it);		//�ӽ�����µ��ڲ�����

	//���ַ����н���һ��Item,�������item��ָ������itemԭ����ID
	bool createAItemFromStr(const unsigned char * str,int &offset,unsigned char *groupName,
		std::vector<XConfigItem *> *itemsList,std::vector<int> *itemsIDListD);
//-----------------------------------------------------
private:
	float m_maxRowWidth;					//��ǰ������п�
	void moveDown(int pixelW,int pixelH);	//����ƫ����������һ�������
	void useANewRow();//ʹ���µ�һ��
	void relayoutGroup(int index,bool flag = true);	//flag���ڱ��Group�Ƿ���Ҫ�����Ű�
public:
	bool init(XConfigMode mode = CFG_MODE_NORMAL);
	void update();	//������������ڲ�������UI����������
	void draw();
	void setItemActive(bool isActive,void * p);	//����ĳ���������Ƿ�Ϊ������ʽ(Ĭ��ȫ��Ϊ������ʽ)
	void setPosition(float x,float y);
	void setScale(float s){ setScale(s, s); }
	void setScale(float x,float y);
	void setTextColor(const XFColor& color);	//�����������ɫ
	XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setMaxHeight(float h);
	void setVisible();	//������ʾ
	void disVisible();	//���ò���ʾ
	bool getIsVisble()const{return m_isVisble;}

	template<typename T>
	int addAItem(T *p,XConfigDataType type,const char * name,
		T max,T min,T def,
		void (* changeProc)(void *,void *) = NULL,const char * groupName = NULL,void *pClass = NULL);	//����ID��-1Ϊʧ��
	int addCustomItem(XCFGItemBasic *it,const char * groupName);
	int addSpecialItem(void * it,XConfigDataType type,const char * groupName);
	bool isSpecialItemExist(void *it);

	bool isItemExist(void * p);					//����������Ƿ��Ѿ�����
	bool isCustomItemExist(XCFGItemBasic *it);

private:
	bool getItemValueFromStr(XConfigItem *it,const char *str);	//���ַ����ж�ȡָ�������������
public:
	bool save(const char *filename = NULL);
	bool load(const char *filename = NULL){return loadEx(filename);}
	bool loadEx(const char *filename = NULL);	//��ǿ��Ķ�ȡ����������ļ�������Ȼ����޶ȵĶ�ȡ����
	void setDefault();	//�ָ�Ĭ��ֵ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ������϶�������ı���
private:
	bool m_isMouseDown;		//�Ƿ���갴��
	XVector2 m_mousePos;	//��갴�µ�λ��
public:
	XBool mouseProc(float x,float y,XMouseState mouseState);		//������궯������Ӧ����
	XBool inputProc(const XInputEvent &input)
	{
		if(!m_isInited) return XFalse;
		if(input.type == EVENT_MOUSE)
			return mouseProc(input.mouseX,input.mouseY,input.mouseState);
		return XFalse;
	}
	//------------------------------------------------------------
	XConfigItem *getItemByID(int ID);			//ͨ��ID��ȡ�������ָ��
	XConfigItem *getItemByVariable(void *p);	//ͨ������ָ�����������ָ��
	XConfigItem *getItemByName(const char *name,int start = 0);	//ͨ������������Ʋ���������������ͬ����������򷵻ص�һ��

	bool decreaseAItem(void *p);	//����һ���������δ��������
	bool clear();					//ɾ�����е�Ⱥ���������
	//����Ľӿڶ���δʵ��
	//bool setCurValue(int ID);	//����ָ����ĵ�ǰֵ
	//bool addAItem(XConfigItem * it);	//����һ��������

	//������������صĲ���
	bool addGroup(const char * name);
	bool renameGroup(const char *oldName,const char *newName);	//�ı��������
	bool isGroupExist(const char * name);	//�ж�����Ƿ����
	XConfigGroup *getGroup(const char * name);
};
#if WITH_INLINE_FILE
#include "XConfigManager.inl"
#endif
}
#endif