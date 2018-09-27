#ifndef _JIA_XOBJECTMANAGER_
#define _JIA_XOBJECTMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.11.9
//--------------------------------
#include "XCommonDefine.h"
#include "XMath\XVector2.h"
#include "XObjectBasic.h"
#include "XInputEventCommon.h"
namespace XE{
/**********************************
//�༭����Ĺ��ܼ�˵��
1����ctrl + h�����Ƿ���ʾ���Դ���
2����ctrl + u�����Ƿ�ʼ�༭����
3����ctrl + p�����ı����Դ��ڵ�λ��
4����Q����ȥ�����в���
5����E����NodeLine����ı༭ת��
6����S�����ߴ�����
7����R������ת
8����M�����ƶ�
9����D����NodeLine��ɾ����ǰ ѡ�еĵ�
10����A����NodeLine�������λ������һ����
11����C�����ı���ת����
12����T�������÷�ת״̬
13����left����right����up����down�����ƶ�����
14����N��:�Ƿ������ѡ���
15����B�����Ƿ���ʾ�����ID
16����Delete�������б���ɾ��ѡ������
17����Shift����Ƭѡ���������ͬʱ�ı��������״̬
***********************************/

#define OBJ_MANAGER_WITH_ID (1)	//��ʾ���ID����������Ľ������ݹ���ã����������

//��������͵Ķ���
enum XObjectOptionType
{
	OBJ_OPTION_SIZE_ON,			//����Ŵ�
	OBJ_OPTION_SIZE_DOWN,		//�����С
	OBJ_OPTIONX_ON,			//�������
	OBJ_OPTIONX_DOWN,			//�������
	OBJ_OPTION_Y_ON,			//�������
	OBJ_OPTION_Y_DOWN,			//�������
	OBJ_OPTION_ROTATE_ON,		//��ת�Ƕ�����
	OBJ_OPTION_ROTATE_DOWN,		//��ת�Ƕȼ�С
	OBJ_OPTION_ROTATE_MODE_CHANGE,	//��תģʽ�仯
	OBJ_OPTION_TURNOVER_MODE_CHANGE,//��תģʽ�仯
};	
enum XObjectOption
{
	OBJ_OPTION_NULL,			//�޶���
	OBJ_OPTION_POSITION,		//λ�ñ仯
	OBJ_OPTION_SIZE,			//�ߴ�仯
	OBJ_OPTION_ROTATE,			//��ת
	OBJ_OPTION_ROTATEMODE,		//�ı���תģʽ
	OBJ_OPTION_TURNOVERMODE,	//�ı䷭תģʽ

	OBJ_OPTION_EDIT,			//������ڱ༭״̬
};
enum XObjectOptionState	//�����ǰ���õ�״̬
{
	OBJ_OPTION_STATE_NULL,					//û�������κ�״̬
	OBJ_OPTION_STATE_CAN_NOT_CHOOSE,		//����ѡ��״̬
	OBJ_OPTION_STATE_NO_MOUSE_PROC,			//������������
	OBJ_OPTION_STATE_NO_KEY_PROC,			//����������̲���
};
enum XObjectMouseState	//��������״̬״̬
{
	OBJ_STATE_NULL,			//��״̬
	OBJ_STATE_MOUSE_DOWN,	//��갴��״̬
	OBJ_STATE_MOUSE_MOVE,	//����϶�״̬
	OBJ_STATE_MOUSE_ON,		//�������״̬
};
class XFontUnicode;
class XMultiListBasic;
//�����Ҫʹ�õ�����Ϣ
struct XObjectInfo
{
	XVec2 m_position;	//�����λ��
	XVec2 m_scale;		//����ĳߴ�
	float m_angle;			//����ĽǶ�
//	float m_alpha;			//�����͸����
	float length;			//����NodeLine��˵����Ҫ��ʾ���ĳ���

	XObjectBasic *m_pObject;	//�����ָ��
	XObjectMouseState m_objectMouseState;	//��������״̬
	XVec2 m_objectMousePoint;//��갴����λ��
	XObjectOption m_objectKeyOption;	//�����ǰ�������¶�Ӧ�Ĺ��� 0����Ч��1���ƶ���2�����ţ�3����ת��4���ı���תģʽ��5�����÷�ת״̬
	XObjectOptionState m_objectSetState;	//�����ǰ�����õ�״̬��0��û������״̬��1������Ϊ����ѡ��״̬��2������Ϊ������������״̬��3������Ϊ�����ܼ��̲���״̬
	int m_objectEditParm;	//������б༭��ʱ��ĸ�������������NodeLine�������ѡ�еĵ�ı��
	//��������������û�����б��Ĳ������Ӧ�������������
	XBool m_objCanEdit;		//����Ƿ���Ա༭		
	XBool m_objBeSelect;		//����Ƿ�ѡ��

	int m_objLineOrder;		//������б���еĶ�Ӧ��ϵ
	int m_lineObjOrder;		//�б����������Ķ�Ӧ��ϵ
#if OBJ_MANAGER_WITH_ID
	XFontUnicode *m_fontID;		//��ʾ�����ID������ǳ��������ܣ���Ҫ����
#endif
	XObjectInfo()
		:m_pObject(NULL)
		, m_objectMouseState(OBJ_STATE_NULL)
		, m_objectKeyOption(OBJ_OPTION_NULL)
		, m_objectSetState(OBJ_OPTION_STATE_NULL)
		, m_objLineOrder(-1)
		, m_lineObjOrder(-1)
		, m_objCanEdit(XFalse)	//��ʼ�����е�������ǲ��ɱ༭��
		, m_objBeSelect(XFalse)	//��ʼ���������ѡ��
#if OBJ_MANAGER_WITH_ID
		, m_fontID(NULL)
#endif
	{}
};
//�������һ������ϵͳ��������Ӧ����ֻ��һ��ʵ��
class XObjectManager
{
public:
	XObjectManager();
	virtual ~XObjectManager();
protected:
	XObjectManager(const XObjectManager&);
	XObjectManager &operator= (const XObjectManager&);
private:
	int m_curMouseOnObjectSum;			//��ǰ�������on״̬�µ��������
	//�������������Ҫ��һ���ӿ�������
	XVec2 m_curMousePosition;			//��굱ǰ��λ��

	void objectKeyOption(unsigned int order, XObjectOptionType optionType);	//����Ӧ��������в�����OptionType���Ƕ���

	//��Ҫһ�������֧��������ʾ�����Ϣ
	std::vector<XObjectInfo> m_objInfo;
	//��������������û�����б��Ĳ������Ӧ�������������
	XBool m_canAddObj;		//�Ƿ�����ע�����,���������������ʹ��ע�����ʧ��
#if OBJ_MANAGER_WITH_ID
	char m_showObjID;		//�Ƿ���ʾObjID
	void setShowObjID();
#endif
	void release();
	XFontUnicode *m_font;		//��ʾ���ַ�
	XBool m_isInited;	//�Ƿ��ʼ��
	XBool checkNeedUpdate(int order);	//��������Ϣ�Ƿ���Ҫ����
	void updateObjInfo(int order);		//���������Ϣ
	void updateObjStateInfo(int order);	//���������״̬��Ϣ
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˱༭�����������
	XMultiListBasic *m_mutiList;	//�����б��
	void upDateMultiListData();		//���¶����б���е�����
	//--------------------------------------------------
	static void ctrlProc(void*, int, int);
	XBool m_isShowUI;		//�Ƿ���ʾ����Ԫ��
	XBool m_isOption;		//�Ƿ���Բ���
	XBool m_canSelect;		//�Ƿ���������ѡ���
	char m_editWindowsPos;	//�༭���ڵ�λ��	1	0
	void setEditWidowsPos();					//	2	3

	void chengeCanSelectState();	//�ı�����Ƿ���Ա���ѡ�ı�ǩ
	void setShow();//������ʾ
	void disShow();//���ò���ʾ
	void setOption();
	void disOption();
public:
	//������Ϊ��ʵ�ְ��������������������
	XKeyState m_keyState[4];//��Ӧ�����������ĸ�����
	int m_keyTimer[4];	//�ĸ������ļ�ʱ
	int m_keyCurTime[4];	//�ĸ������ĵ�ǰʱ����
	int m_keyMaxTime;	//���������Ӧʱ��
	int m_keyMinTime;	//�����������Ӧʱ��

	XKeyState m_ctrlKeyState;	//ctrl�����İ���״̬
public:
	XBool init(XResPos resPos = RES_SYS_DEF);
	int addAObject(XObjectBasic * object);			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	void decreaseAObject(unsigned int objectID);							//ȥ��һ�������ע��
	void decreaseAObject(const void * object);					//ȥ��һ�������ע��
	int findObjectID(const void * object) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	XObjectType getObjectType(unsigned int objectID) const;	//��ȡ���������
	void *getObject(unsigned int objectID) const;				//��ȡ�����ָ��

	void mouseProc(const XVec2& p, XMouseState eventType);	//���������¼�����Ӧ
	void keyProc(int keyID, XKeyState keyState);		//keyState:0���£�1����
	int getCurObjectSum() const;				//��ȡ��ǰע����������
	int getCurMouseOnObjectSum() const;			//��ȡ��ǰ�������On״̬�µ��������
	int getCurChooseObjectSum() const;			//��ȡ��ǰ��ѡ�е��������
	void getCurChooseObjectID(int *ID) const;	//��ȡ��ǰ��ѡ������������ID����

	void draw();			//��������һЩ��Ϣ
	void move(float delay);	//����
private:	//2014.4.18��������ʵʱ��ʾ���λ��
	XFontUnicode *m_mousePosFont;	//ʵʱ��ʾ���λ�õ��ַ���
};
#if WITH_INLINE_FILE
#include "XObjectManager.inl"
#endif
}
#endif