#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"

//˵�����������ѡ�ƶ��Ĺ��ܣ�һ��ѡ�����ؼ�����ͳһ���ƶ���(��δ���)
//����ͨ������Ctrl�������ж���ؼ���ѡ��
//����ѡ��״̬�Ŀؼ���Ҫ�б�ѡ��ı�ʶ
//��ѡ�Ŀؼ����Խ���ͳһ���ƶ���ɾ��

struct GCtrlInfo
{
	int ctrlID;	//ID
	std::string ctrlName;
	XE::XControlBasic * pCtrl;
	XE::XBool withCallback;	//�Ƿ�ʹ�ûص�����
	//Ŀǰֻ֧��tab�Ĵ�����ϵ
	std::string affiliationStr;	//����������ϵ���ַ���:������ϵ��������ʽΪ ctrname:tabname;
	//������XE::XCombo���������
	int allMenuSum;		//�ܲ˵�����
	int showMenuSum;	//��ʾ�Ĳ˵�����
	
	std::string ctrlComment;		//�ؼ�˵��
	std::string codeExplanation;	//����˵��

	//������Ϊ�˸�����ʾ������ı���
	XE::XBool isMouseOn;	//�Ƿ��������״̬

 	GCtrlInfo()
		:pCtrl(NULL)
		,withCallback(XFalse)
		,allMenuSum(0)
		,showMenuSum(0)
		,affiliationStr("NULL")
		,isMouseOn(XFalse)
		,ctrlComment("NULL")
		,codeExplanation("NULL")
	{}
};
#define CFG_FILENAME ("UIConfig.xml")
class GGame:public XE::XGameBasic
{
public:
	GGame()
		:m_ctrlID(0)
		,m_curChooseCtrlID(-1)
		,m_isCtrlDown(false)
		,m_isMouseSelect(false)
		,m_isMouseDrag(false)
	{}
	virtual ~GGame(){}
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void drawUp();
	void ctrlEventProc(int id,int eventID){}

	int m_curChooseCtrlID;	//��ǰѡ��������ID
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����Ϊ��ѡ����
	std::vector<int> m_curChooseCtrls;		//��ǰ����ѡ��Ŀؼ���ʹ��Ctrl+���ѡ��
	bool m_isCtrlDown;						//ctrl�Ƿ���
	int isSelect(int id);					//�ж�ָ��ID�Ŀؼ��Ƿ��Ѿ�ѡ��
	//����϶�Ƭѡ
	bool m_isMouseSelect;	//����Ƿ���Ƭѡ״̬
	XE::XVec2 m_mouseDownPos;	//��갴�µ�λ��
	XE::XVec2 m_mouseCurPos;		//��ǰ�����λ��
	void updateMouseSelect();
	//����϶��ؼ�
	bool m_isMouseDrag;	//�Ƿ�����϶�״̬
	XE::XVec2 m_mouseDragPos;
	//----------------------------------------------------------

	XE::XSubWindow m_subWindow;	//�Ӵ���
	bool createUI();

	std::vector<GCtrlInfo> m_ctrls;
	int m_ctrlID;
	XE::XVec2 m_curCtrlPos;
	//�����ǹ��ڽ����ؼ�����ز���
	XE::XBool addACtrl(XE::XCtrlObjType type);	//����һ���ؼ�
	XE::XBool delACtrl();//ɾ��һ���ؼ�
	XE::XBool delSelectCtrl();	//ɾ������ѡ��Ŀؼ�
	int getNewMouseChooseCtrl(float x,float y);			//��ȡĿǰ���ѡ��Ŀؼ��ı��
	XE::XCtrlObjType getCurChooseCtrlType()const;	//��ȡĿǰѡ��Ŀؼ�������
	void setCurChooseCtrlType(XE::XCtrlObjType type);
	bool updateCtrlList(float x,float y);	//��������λ�ø��¿ؼ��б������Ƿ��пؼ���������
	void updateCtrlProperty();		//����ǰѡ��Ŀռ�����Ը��µ��ֽ�����
	int getCtrlIDFromList();	//���б���л�ȡ��ǰѡ��Ŀؼ���ID

	int getCtrlIndexByID(int id)const;
	int getCtrlIndexByName(const std::string &name)const;
	int getCurChooseCtrlIndex()const{return getCtrlIndexByID(m_curChooseCtrlID);}
	XE::XBool saveToFile();		//���������ļ�
	XE::XBool loadFromFile();	//��ȡ�����ļ�
	XE::XBool createCode();		//��������

	friend void ctrlFun(void *pClass,int id);
	XE::XTab m_tab;
	//�ؼ��б�
	XE::XRadios m_ctrlChooseRds;
	//����
	XE::XMultiList m_chooseCtrlMltlst;	//��ʾ��ǰ����ѡ��Ŀؼ�
	XE::XText m_curMousePosTxt;
	XE::XButton m_delBtn;	//ɾ����ť
		//��������
	XE::XText m_ctrlPosTxt;	//�ؼ����ڵ�λ��
	XE::XEdit m_ctrlPosXEdt;	//�ؼ�������
	XE::XEdit m_ctrlPosYEdt;	//�ؼ�������
	XE::XText m_ctrlPixelSizeTxt;	//�ؼ������ش�С
	XE::XEdit m_ctrlPixelSizeXEdt;	//�ؼ������ؿ��
	XE::XEdit m_ctrlPixelSizeYEdt;	//�ؼ������ظ߶�
	XE::XText m_ctrlScaleSizeTxt;	//�ؼ������Ŵ�С
	XE::XEdit m_ctrlScaleSizeXEdt;	//�ؼ������ſ��
	XE::XEdit m_ctrlScaleSizeYEdt;	//�ؼ������Ÿ߶�
	XE::XText m_ctrlNameTxt;		//�ؼ�������
	XE::XEdit m_ctrlNameEdt;
	XE::XText m_ctrlAffiliationTxt;	//�ؼ��Ĵ�����ϵ
	XE::XEdit m_ctrlAffiliationEdt;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�¼��������
	XE::XText m_ctrlCommentTxt;
	XE::XEdit m_ctrlCommentEdt;		//�ؼ���˵��
	XE::XText m_ctrlExplanationTxt;
	XE::XEdit m_ctrlExplanationEdt;	//�ؼ��Ĵ���ע��
	//-----------------------------------------------------
		//˽������
	XE::XText m_ctrlCaptionTxt;	//�ؼ��ı���
	XE::XEdit m_ctrlCaptionEdt;
	XE::XCheck m_withCallbackChk;	//�Ƿ�ʹ�ûص�����
		//combo�Ĳ���
	XE::XText m_cmbAllMenuSumTxt;	//�����˵��Ĳ˵�����
	XE::XEdit m_cmbAllMenuSumEdt;
	XE::XText m_cmbShowMenuSumTxt;	//�����˵�����ʾ�Ĳ˵�����
	XE::XEdit m_cmbShowMenuSumEdt;	

	void updateAffiliation();
	void clearAffiliation();	//���������ϵ
	//ϵͳ
	XE::XText m_winSizeTxt;	//���ڳߴ�
	XE::XEdit m_winSizeXEdt;
	XE::XEdit m_winSizeYEdt;
	XE::XText m_prjNameTxt;	//��������
	XE::XEdit m_prjNameEdt;
	XE::XButton m_saveBtn;		//���水ť
	XE::XButton m_readBtn;		//��ȡ��ť
	XE::XButton m_codeBtn;		//���ɴ��밴ť
	//�����Ǳ���
	//�ؼ��б�
	int m_winWidth;
	int m_winHeight;
	//����
	//ϵͳ

	//��������ʵ�ּ����ƶ�����
	XE::XKeyPressModel m_keyUp;
	XE::XKeyPressModel m_keyDown;
	XE::XKeyPressModel m_keyLeft;
	XE::XKeyPressModel m_keyRight;
};

#endif