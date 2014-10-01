#ifndef _JIA_XCONFIGMANAGER_
#define _JIA_XCONFIGMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
#include "XEffeEngine.h"
#include "XMedia/XNetServer.h"
#include "XMedia/XNetClient.h"
#include <vector>
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
//ItemID|ItemType|ItemRangeMin|ItemRangeMax|ItemDefValue|ItemNowValue|ItemNameLen|ItemName|
//.......
//if(ItemType == CFG_DATA_TYPE_CUSTOM)
//ItemID|ItemType|CItemSum|CItem0ID|CItem0Type|........
enum _XConfigDataType
{
	CFG_DATA_TYPE_INT,
	CFG_DATA_TYPE_CHAR,
	CFG_DATA_TYPE_UCHAR,
	CFG_DATA_TYPE_FLOAT,
	CFG_DATA_TYPE_XBOOL,	//ע�������Ӧ_XBool���ͣ�������bool���ͣ����򽫻��п�����ɴ���
	CFG_DATA_TYPE_RADIOS,	//��ѡ��
	CFG_DATA_TYPE_CUSTOM,	//�Զ���		���粻֧��
	CFG_DATA_TYPE_XSPRITE,	//��������	���粻֧��
	CFG_DATA_TYPE_NULL,		//��Ч��������
};

#define CFG_MANAGER_W_SPACE (5.0f)
#define CFG_MANAGER_H_SPACE (5.0f)
#define CFG_MNG_H_FONT	(32.0f)
#define CFG_MNG_H_SLD	(24.0f)
#define CFG_MAX_ITEMS_SUM (10000)
//һ��������Ľṹ
union _XConfigValue
{
	int valueI;
	float valueF;
	_XBool valueB;
};
#define CFG_DEFAULT_FILENAME ("xConfig.dat")
#define CFG_INJECT_FILENAME ("xCFGInject.dat")
#define CFG_DEFAULT_GROUPNAME ("Default")
//�û��Զ���ؼ��Ļ���
//����û���Ҫ�Զ���ɱ����ù����������࣬��ô�������������ಢʵ����ؽӿ�
class _XCFGItemBasic:public _XObjectBasic,public _XBasicOprate
{
public:
	virtual bool save(FILE *fp) = 0;	//���浱ǰ����
	virtual bool load(FILE *fp) = 0;	//��ȡ��ǰ����
	virtual bool update() = 0;			//ʹ���ڲ�������������UI������
	virtual bool defValue() = 0;		//ȡĬ��ֵ
	virtual bool enable() = 0;			//ʹ������Ա��޸Ŀ���
	virtual bool disable() = 0;			//���ܱ�����

	virtual ~_XCFGItemBasic() {}
	//Ϊ��֧��������ṩ�Ľӿ�
	virtual unsigned char *getInfo(int &len,int id) = 0;					//������������ͬ�����ַ�����len������
	virtual void setValueFromStr(unsigned char * str) = 0;	//�����緢�͵��ַ�������ȡ����
	virtual bool needSendStr() = 0;							//�����Ƿ���Ҫ����״̬���ͻ���
	virtual unsigned char *sendStr(int &len,int id) = 0;					//�ڲ����������ɿ��Է��͵�����
};
class _XConfigItem
{
private:
	int m_ID;
public:
	bool m_isEnable;
	_XConfigDataType m_type;		//�����������
	_XConfigValue m_rangeMin;		//ȡֵ��Χ
	_XConfigValue m_rangeMax;	
	_XConfigValue m_defaultValue;	//Ĭ��ֵ
	_XConfigValue m_nowValue;		//��ǰֵ
	void * m_pVariable;				//Ŀ���ָ��(Ψһ)
	void (* m_changeProc)();		//�����仯�Ļص�����
	_XControlBasic *m_pCtrl;		//�ؼ���ָ��
	string m_name;					//��λ���������(Ψһ)
	bool m_isActive;	//��ǰ�������Ƿ��������أ�����Ǳ����ģ�����ֵ���ܱ��ⲿ�ı�
	_XCFGItemBasic * m_customIt;	//�û��Զ����������(Ψһ)

	_XConfigItem()
		:m_isEnable(false)
		,m_pVariable(NULL)
		,m_changeProc(NULL)
		,m_pCtrl(NULL)
		,m_isActive(true)
		,m_customIt(NULL)
	{
		static int ID = 0;
		m_ID = ID ++;
	}
	~_XConfigItem(){XDELETE(m_pCtrl);}
	int getID()const {return m_ID;}
	void setID(int ID){m_ID = ID;}
};
class _XConfigGroup
{
private:
	void moveDownPretreatment(int pixelW,int pixelH);	//�ƶ���Ԥ�������ƶ�֮ǰ����һЩ����
	void moveDownPretreatment(const _XVector2 &size){moveDownPretreatment(size.x,size.y);}
	void moveDown(int pixelW,int pixelH);	//�����ƶ������
	void moveDown(const _XVector2 &size){moveDown(size.x,size.y);}
	void useANewRow()
	{//ʹ���µ�һ��
		m_nowInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_size.x;
		m_nowInsertPos.y = m_position.y + (32.0f + CFG_MANAGER_H_SPACE) * m_size.y;
		m_isNewRow = true;
	}
public:
	bool m_isEnable;		//���Ƿ���Ч
	_XVector2 m_maxSize;	//��Χ���ӵĴ�С�������ֵ��û�о������ŵ����ش�С
	_XVector2 m_position;	//λ��
	_XVector2 m_size;		//��С
	string m_name;			//����
	_XGroup m_group;
	//std::vector<int> m_itemsID;	//�������ID
	std::vector<_XConfigItem *> m_items;	//�������ָ��
	_XVector2 m_nowInsertPos;	//��ǰ�����λ��
	void relayout();	//�����Զ�����
	float m_width;
	float m_maxHeight;
	bool m_isMaxH;	//�Ƿ�ʹ�����߶�
	float m_maxRowWidth;					//��ǰ�е�����п�
	bool m_isNewRow;						//�Ƿ����µ�һ��
	_XVector2 calculateMaxSize();	//���㵱ǰ��ķ�Χ

	_XConfigGroup()
		:m_isEnable(false)
		,m_name(CFG_DEFAULT_GROUPNAME)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
	{}
};
enum _XConfigMode
{
	CFG_MODE_NORMAL,	//��ͨģʽ
	CFG_MODE_SERVER,	//������ģʽ
	CFG_MODE_CLIENT,	//�ͻ���ģʽ
};
enum _XConfigNetOperate
{
	CFG_NET_OP_SAVE,	//����
	CFG_NET_OP_LOAD,	//��ȡ
	CFG_NET_OP_DEFAULT,	//Ĭ��
};
class _XConfigManager
{
protected:
	_XConfigManager()
		:m_isInited(false)
		,m_isVisble(true)
		,m_position(0.0f,0.0f)
		,m_size(1.0,1.0f)
		,m_maxHeight(300.0f)
		,m_width(256.0f)
		,m_nowInsertPos(0.0f,0.0f)
		,m_configMode(CFG_MODE_NORMAL)
//		,m_minuteIndex(-1)
	//	,m_operateIndex(-1)	//û�ж���
		,m_textColor(0.0f,0.0f,0.0f,1.0f)
		,m_isMouseDown(false)
	{}
	_XConfigManager(const _XConfigManager&);
	_XConfigManager &operator= (const _XConfigManager&);
	virtual ~_XConfigManager(){release();}
public:
	static _XConfigManager& GetInstance()
	{
		static _XConfigManager m_instance;
		return m_instance;
	}
private:
	bool m_isInited;
	bool m_isVisble;	//�Ƿ�ɼ�
	std::vector<_XConfigItem *> m_pItems;	//������������б�
	std::vector<_XConfigGroup *> m_pGroups;	//����б�

	friend void callbackProc(void *pClass,int ID);	//�ص�����
	//friend void callbackProcMD(void *pClass,int ID);	//��갴���¼��Ļص�����

	_XVector2 m_position;
	_XVector2 m_size;
	float m_maxHeight;	//�߶ȣ�����ֻ���Ƹ߶�
	float m_width;		//������Ŀ��
	_XVector2 m_nowInsertPos;	//Ŀǰ�������ڵ�λ��
	_XFColor m_textColor;	//�������ɫ

	_XButton m_saveBtn;
	_XButton m_loadBtn;
	_XButton m_defaultBtn;
	_XButton m_undoBtn;
	_XButton m_redoBtn;
	_XButton m_netUpdateBtn;	//�ӷ�����ͬ��������Ϣ
	_XButton m_netInjectBtn;	//�������ע����������

	void relayout();	//�����Զ�����
	void release();
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�ͻ��˵Ĺ��ܰ���˵��:
	//���棺Ҫ��������˱������ݣ�����Ҳ��������			���
	//��ȡ��Ҫ��������˶�ȡ���ݣ���������ͬ�����ͻ���		���
	//Ĭ�ϣ�Ҫ��������˻ָ�Ĭ�����ݣ���������ͬ�����ͻ���	���
	//ע�룺�ͻ��˶�ȡ���ݣ���������ͬ������������			���
	//������������صĶ���
	_XConfigMode m_configMode;
	_XNetClient m_netClient;
	_XNetServer m_netServer;

	unsigned char *getConfigInfo(int &len);			//��������Ľṹת�����ַ���
	void setOperateToServer(_XConfigNetOperate op);	//�ͻ�������������Ͳ�������
	void sendSynchToServer();						//�����������ͬ������
	void sendCFGInfo();
	void sendItemValue(const _XConfigItem * it);
	void sendInject();	//�����������ע����Ϣ
	void updateNet();
	void updateInfo(unsigned char *data);
	void setItemValue(int ID,const _XConfigValue value);
	void updateItemToCFG(_XConfigItem * it);		//���ڲ����ݸ��µ�����
	void updateItemFromCFG(_XConfigItem * it);		//�ӽ�����µ��ڲ�����

	//���ַ����н���һ��Item,�������item��ָ������itemԭ����ID
	bool createAItemFromStr(const unsigned char * str,int &offset,unsigned char *groupName,
		std::vector<_XConfigItem *> *itemsList,std::vector<int> *itemsIDListD);
//-----------------------------------------------------
private:
	float m_maxRowWidth;					//��ǰ������п�
	bool m_isNewRow;						//�Ƿ����µ�һ��
	void moveDown(int pixelW,int pixelH)	//����ƫ����������һ�������
	{
		m_nowInsertPos.y += (pixelH + CFG_MANAGER_H_SPACE) * m_size.y;
		if(m_isNewRow || pixelW > m_maxRowWidth) m_maxRowWidth = pixelW;
		m_isNewRow = false;
		if(m_nowInsertPos.y >= m_position.y + m_maxHeight)
		{//������Ҫ����
			useANewRow();
		}
	}
	void useANewRow()
	{//ʹ���µ�һ��
		m_nowInsertPos.x += (m_maxRowWidth + CFG_MANAGER_W_SPACE) * m_size.x;
		m_nowInsertPos.y = m_position.y + (32.0f + CFG_MANAGER_H_SPACE) * m_size.x;
		m_isNewRow = true;
	}
	void relayoutGroup(int index,bool flag = true);	//flag���ڱ��Group�Ƿ���Ҫ�����Ű�
public:
	bool init(_XConfigMode mode = CFG_MODE_NORMAL);
	void update();	//������������ڲ�������UI����������
	void draw();
	void setItemActive(bool isActive,void * p);	//����ĳ���������Ƿ�Ϊ������ʽ(Ĭ��ȫ��Ϊ������ʽ)
	void setPosition(float x,float y)
	{
		if(!m_isInited) return;
		m_position.set(x,y);
		relayout();	//���²���
	}
	void setSize(float x,float y)
	{
		if(!m_isInited) return;
		m_size.set(x,y);
		relayout();	//���²���
	}
	void setTextColor(const _XFColor& color);	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setMaxHeight(float h)
	{
		if(h < 1) h = 1;
		m_maxHeight = h;
		relayout();
	}
	void setVisible();	//������ʾ
	void disVisible();	//���ò���ʾ
	bool getIsVisble(){return m_isVisble;}

	template<typename T>
	int addAItem(T *p,_XConfigDataType type,const char * name,
		T max,T min,T def,
		void (* changeProc)() = NULL,const char * groupName = NULL);	//����ID��-1Ϊʧ��
	int addCustomItem(_XCFGItemBasic *it,const char * groupName);
	int addSpecialItem(void * it,_XConfigDataType type,const char * groupName);
	bool isSpecialItemExist(void *it)
	{
		if(it == NULL) return false;
		for(int i = 0;i < m_pItems.size();++ i)
		{
			if(m_pItems[i]->m_pVariable == it) return true;
		}
		return false;
	}

	bool isItemExist(void * p);					//����������Ƿ��Ѿ�����
	bool isCustomItemExist(_XCFGItemBasic *it);

private:
	bool getItemValueFromStr(_XConfigItem *it,const char *str);	//���ַ����ж�ȡָ�������������
public:
	bool save(const char *filename = NULL);
	bool load(const char *filename = NULL){return loadEx(filename);}
	bool loadEx(const char *filename = NULL);	//��ǿ��Ķ�ȡ����������ļ�������Ȼ����޶ȵĶ�ȡ����
	void setDefault();	//�ָ�Ĭ��ֵ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ��ʵ������϶�������ı���
private:
	bool m_isMouseDown;		//�Ƿ���갴��
	_XVector2 m_mousePos;	//��갴�µ�λ��
public:
	_XBool mouseProc(float x,float y,_XMouseState mouseState)		//������궯������Ӧ����
	{//��δ���
		_XRect tmpRect(m_position.x,m_position.y,
			m_position.x + 32.0f * m_size.x,m_position.y + 32.0f * m_size.y);
		switch(mouseState)
		{
		case MOUSE_MOVE:
			if(m_isMouseDown)
			{//���������ק�¼�
				_XVector2 tmp = _XVector2(x,y) - m_mousePos;
				setPosition(m_position.x + tmp.x,m_position.y + tmp.y);
				m_mousePos.set(x,y);
				//if(!tmpRect.isInRect(x,y)) m_isMouseDown = false;
			}
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
		case MOUSE_LEFT_BUTTON_DCLICK:
			if(tmpRect.isInRect(x,y))
			{
				m_isMouseDown = true;
				m_mousePos.set(x,y);
			}
			break;
		case MOUSE_LEFT_BUTTON_UP:
			m_isMouseDown = false;
			break;
		}
		return XFalse;
	}
	//------------------------------------------------------------
	_XConfigItem *getItemByID(int ID);			//ͨ��ID��ȡ�������ָ��
	_XConfigItem *getItemByVariable(void *p);	//ͨ������ָ�����������ָ��
	_XConfigItem *getItemByName(const char *name,int start = 0);	//ͨ������������Ʋ���������������ͬ����������򷵻ص�һ��

	bool decreaseAItem(void *p);	//����һ���������δ��������
	bool clear();					//ɾ�����е�Ⱥ���������
	//����Ľӿڶ���δʵ��
	//bool setNowValue(int ID);	//����ָ����ĵ�ǰֵ
	//bool addAItem(_XConfigItem * it);	//����һ��������

	//������������صĲ���
	bool addGroup(const char * name);
	bool renameGroup(const char *oldName,const char *newName)	//�ı��������
	{
		if(oldName == NULL || newName == NULL) return false;
		if(isGroupExist(newName)) return false;
		_XConfigGroup *gp = getGroup(oldName);
		if(gp == NULL) return false;
		gp->m_name = newName;
		gp->m_group.setCaptionText(newName);
		return true;
	}
	bool isGroupExist(const char * name);	//�ж�����Ƿ����
	_XConfigGroup *getGroup(const char * name);
};

//��addBuff�����ݽӵ�baseBuff֮������ڴ�ռ䲻��������㹻���ڴ�ռ�
extern unsigned char *spliceData(unsigned char * baseBuff,int &baseLen,int &offset,const unsigned char * addBuff,int len);

template<typename T>
int _XConfigManager::addAItem(T *p,_XConfigDataType type,const char * name,
	T max,T min,T def,
	void (* changeProc)(),const char * groupName)	//����ID��-1Ϊʧ��
{
	if(type >= CFG_DATA_TYPE_CUSTOM || p == NULL) return -1;	//���ݲ��Ϸ�
	if(p != NULL && isItemExist(p)) return -1; 
	_XConfigItem *pItem = createMem<_XConfigItem>();
	if(pItem == NULL) return -1;
	if(type == CFG_DATA_TYPE_CUSTOM) return -1;	//�Զ���ؼ�����ʹ�����ַ�ʽ���
	if(name == NULL) pItem->m_name = " ";
	else pItem->m_name = name;
	pItem->m_changeProc = changeProc;
	char tempStr[1024];
	switch(type)
	{
	case CFG_DATA_TYPE_INT:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_nowValue.valueI = * (int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_CHAR:
		pItem->m_defaultValue.valueI = (char)def;
		pItem->m_nowValue.valueI = * (char *)p;
		pItem->m_rangeMin.valueI = (char)min;
		pItem->m_rangeMax.valueI = (char)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_UCHAR:
		pItem->m_defaultValue.valueI = (unsigned char)def;
		pItem->m_nowValue.valueI = * (unsigned char *)p;
		pItem->m_rangeMin.valueI = (unsigned char)min;
		pItem->m_rangeMax.valueI = (unsigned char)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueI,pItem->m_rangeMin.valueI,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.0f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.0f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			pCtrl->setNowValue(* (unsigned char *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_FLOAT:
		pItem->m_defaultValue.valueF = (float)def;
		pItem->m_nowValue.valueF = * (float *)p;
		pItem->m_rangeMin.valueF = (float)min;
		pItem->m_rangeMax.valueF = (float)max;
		{
			_XSliderEx *pCtrl = createMem<_XSliderEx>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(_XRect(0.0f,CFG_MNG_H_FONT,m_width,CFG_MNG_H_FONT + CFG_MNG_H_SLD),
				pItem->m_rangeMax.valueF,pItem->m_rangeMin.valueF,SLIDER_TYPE_HORIZONTAL,_XVector2(0.0,16.0f));
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			if(name == NULL) pCtrl->setFontEx("%%.4f",XEE::systemFont,1.0f);
			else
			{
				sprintf(tempStr,"%s:%%.4f",name);
				pCtrl->setFontEx(tempStr,XEE::systemFont,1.0f);
			}

			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,callbackProc,this);
			//���ﲻ���������ݣ�����������ݵĻ�����������ݱ仯��ʱ����ûص�������ʱ�������Ѿ��ı������ִ�лص������е���ش���
			//pCtrl->setConnectVar((float *)p);
			pCtrl->setNowValue(* (float *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_XBOOL:
		pItem->m_defaultValue.valueB = (_XBool)def;
		pItem->m_nowValue.valueB = *(_XBool *)p;
		pItem->m_rangeMin.valueB = (_XBool)min;
		pItem->m_rangeMax.valueB = (_XBool)max;
		{
			_XCheck *pCtrl = createMem<_XCheck>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			if(name == NULL) pCtrl->initWithoutTex(" ",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),
				_XVector2(CFG_MNG_H_FONT,CFG_MNG_H_FONT * 0.5f));
			else pCtrl->initWithoutTex(name,XEE::systemFont,1.0f,_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),
				_XVector2(CFG_MNG_H_FONT,CFG_MNG_H_FONT * 0.5f));
			pCtrl->setSize(m_size);
			pCtrl->setCallbackFun(NULL,NULL,NULL,NULL,NULL,callbackProc,this);
			pCtrl->setConnectVar((_XBool *)p);
			pCtrl->setTextColor(m_textColor);
			if(*(_XBool *)p) pCtrl->setState(XTrue);
			else pCtrl->setState(XFalse);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	case CFG_DATA_TYPE_RADIOS:
		pItem->m_defaultValue.valueI = (int)def;
		pItem->m_nowValue.valueI = *(int *)p;
		pItem->m_rangeMin.valueI = (int)min;
		pItem->m_rangeMax.valueI = (int)max;
		{
			_XRadios *pCtrl = createMem<_XRadios>();
			if(pCtrl == NULL) return -1;
			if(m_configMode != CFG_MODE_CLIENT) pCtrl->setWithUndo(true);
			pCtrl->initWithoutTex(1,_XVector2(0.0f,CFG_MNG_H_FONT + 2.0f),_XRect(0.0f,0.0f,CFG_MNG_H_FONT,CFG_MNG_H_FONT),XEE::systemFont,1.0f,
				_XVector2(CFG_MNG_H_FONT + 2.0f,CFG_MNG_H_FONT * 0.5f));
			pCtrl->setRadiosText(name);
			pCtrl->setSize(m_size);
			pCtrl->setTextColor(m_textColor);
			pCtrl->setCallbackFun(callbackProc,this);
			pCtrl->setChoosed(*(int *)p);
			pCtrl->stateChange();
			pItem->m_pCtrl = pCtrl;
		}
		break;
	}
	pItem->m_isEnable = true;
	pItem->m_pVariable = p;
	pItem->m_type = type;
	m_pItems.push_back(pItem);
	if(groupName == NULL)
	{//ʹ��Ĭ����
		_XConfigGroup *gp = m_pGroups[0];
		gp->m_items.push_back(pItem);
		if(pItem->m_pCtrl != NULL) 
		{
			gp->m_group.pushChild(pItem->m_pCtrl);
			if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_pCtrl->disVisible();
		}
		relayout();
	}else
	{
		_XConfigGroup *gp = getGroup(groupName);
		if(gp != NULL)
		{
			gp->m_items.push_back(pItem);
			if(pItem->m_pCtrl != NULL) 
			{
				gp->m_group.pushChild(pItem->m_pCtrl);
				if(gp->m_group.getState() == STATE_MINISIZE) pItem->m_pCtrl->disVisible();
			}
			relayout();
		}
	}
	return pItem->getID();
}

#define _XCFGManager _XConfigManager::GetInstance()
#endif