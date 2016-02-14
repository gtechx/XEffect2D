#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#define WITH_OFXOSC
#ifdef WITH_OFXOSC
#include "ofxOsc.h"
#else
#include "XOsc.h"
#endif
#include "GLanguage.h"

struct GOscData
{
	std::string data;		//����
	std::string address;	//��ַ
	int dataType;			//��������
};
struct GKMOscData
{
	int keyValue;
	int keyState;
	std::string data;		//����
	std::string address;	//��ַ
	int dataType;			//��������
};
enum GSendMode
{
	CHOOSE_ADDR,	//����ѡ�е�ַ
	CHOOSE_DATA,	//����ѡ������
	NULL_DATA,		//���Ϳ�����
	CURRENT_DATA,	//���͵�ǰ����
	ALL_LIST_DATA,	//�����б��е���������
};
#define CFG_FILENAME ("config.xml")

class GGame:public XE::XGameBasic
{
protected:
	GGame(const GGame&);
	GGame &operator= (const GGame&);
public:
	GGame()
		:m_isSenderOpen(false)
		,m_isSendState(false)
		,m_recvState(false)
		//,m_mouseOnTimer(0)
		//,m_isShowMsg(false)
		,m_recvSum(0)
	{}
	virtual ~GGame(){}
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(float stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const XE::XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	void ctrlEventProc(int id,int eventID);//TODO:�ؼ�����Ϣ������
	//������Ϊ�˴���ʵʱ�仯���ƶ�����ı���
	int m_windowPosX;		//���ڵ�λ��
	int m_windowPosY;		//���ڵ�λ��
	int m_windowSizeW;		//���ڵĳߴ�
	int m_windowSizeH;		//���ڵĳߴ�
	XE::XMoveData m_moveData;
	bool m_isFirst;
	bool m_isExit;	//�Ƿ񴥷����˳��¼�
	friend void windowExitFun(void *p);
private:
	XE::XTab m_tab;
	XE::XMouseRightButtonMenu m_mouseRBtnMnu; //�������Ҽ��˵�
	//�����ǹ��ڽ���Ԫ�صĶ���
	XE::XText m_textIP;
	XE::XEdit m_textIPEdt;
	XE::XText m_textPort;
	XE::XEdit m_textPortEdt;
	XE::XButton m_openBtn;
	//XButton m_closeBtn;
	//++++++++++++++++++++++++++++++
	//������Ϊ�����Ӽ���ģ�������ı���
	XE::XButton m_addKMBtn;
	XE::XButton m_delKMBtn;
	XE::XButton m_clearKMBtn;	//���
	XE::XButton m_modifyKMBtn;	//�޸İ�ť
	XE::XCombo m_keySelectCmb;	//������ѡ��
	XE::XCombo m_keyStateCmb;	//����״̬��ѡ��
	XE::XText m_oscDataTxt;
	XE::XEdit m_oscDataEdt;
	XE::XText m_oscType;
	XE::XCombo m_oscDataCmb;
	XE::XText m_oscAddress;
	XE::XEdit m_oscAddressEdt;
	XE::XMultiList m_dataKMLst;
	std::vector<GKMOscData *> m_keyMapOscList;	//ӳ������
	std::string keyValueToStr(int keyValue);
	std::string oscDataTypeToStr(int type);
	int keyValueToIndex(XE::XKeyValue k);
	XE::XBool checkKMData(int index = -1);	//��鵱ǰ�������Ƿ�Ϸ�
	void sendSelectData();	//���͵�ǰѡ������
	void sendKMData(int index);
	int m_mouseX;
	int m_mouseY;
	//------------------------------
	XE::XButton m_addBtn;
	XE::XButton m_delBtn;
	XE::XButton m_clearBtn;	//���
	XE::XButton m_modifyBtn;	//�޸İ�ť
	XE::XText m_textData;
	XE::XEdit m_textDataEdt;
	XE::XText m_textType;
	XE::XCombo m_textDataCmb;
	XE::XText m_textAddress;
	XE::XEdit m_textAddressEdt;
	XE::XText m_textTimes;
	XE::XEdit m_textTimesEdt;
	XE::XText m_textSpacing;
	XE::XEdit m_textSpacingEdt;
	XE::XButton m_sendBtn;
	//XButton m_stopBtn;
	XE::XCombo m_sendModeCmb;
	XE::XMultiList m_dataLst;
	XE::XCheck m_sendAsMsg;
	XE::XCheck m_supportACKMode;
	XE::XButton m_clearRecvBtn;	//������տ�İ�ť

	XE::XCombo m_languageCmb;		//����ѡ��
	XE::XCombo m_codeModeCmb;		//���뷽ʽ��ѡ��

//	XE::XFontUnicode m_textRIP;
//	XE::XEdit m_textRIPEdt;
	XE::XText m_textRPort;
	XE::XEdit m_textRPortEdt;
	XE::XButton m_openRBtn;
	//XE::XButton m_closeRBtn;

	XE::XButton m_saveBtn;		//���水ť
	XE::XButton m_readBtn;		//��ȡ�����ļ��İ�ť
	void createUI();
	bool saveToFile(const char *filename = NULL);		//���浽�ļ�
	bool readFromFile(const char *filename = NULL);	//���ļ���ȡ
	void addALineData(const GOscData *tmp);
	void setLineData(const GOscData *tmp,int index);
	void addALineData(const GKMOscData *tmp);
	void setLineData(const GKMOscData *tmp,int index);

	bool m_recvState;		//���յ�״̬
	int m_recvSum;	//���յ�ͳ�Ƽ���

	XE::XMultiText m_recvText;
	XE::XFontUnicode m_allMsgText;	//�������ʾ����
	//�����ǹ��ڹ��ܵĶ���
	//���Ͳ���
#ifdef WITH_OFXOSC
	ofxOscSender m_sender;
#else
	XE::XOscSender m_sender;
#endif
	bool m_isSenderOpen;	//�Ƿ��Ѿ��򿪷����豸
	std::vector<GOscData *> m_datasVector;
	bool m_isSendState;		//�Ƿ����ڷ��͹�����
	int m_timer;	//���ͼ�ʱ��
	int m_times;	//���ʹ����ۼ���
	int m_setTimer;
	int m_setTimes;
	void sendOverProc();
	void sendButtonDown();
	XE::XBool checkNowData();	//��鵱ǰ���ݵ���ȷ��
	void sendNowData(int mode);
	std::string m_nowSendAddr;		//��ǰ���͵����ݵ�������

#ifdef WITH_OFXOSC
	ofxOscReceiver m_receiver;
#else
	XE::XOscReceiver m_receiver;
#endif
	//�����������������ʾ
	//int m_mouseOnTimer;	//���ڼ�¼���û���ƶ�������ͣ״̬��ʱ��	
	//bool m_isShowMsg;	//�Ƿ��Ѿ���ʾ��ʾ
	//void ctrlMsgProc();
private:
	void ctrlStateManager();	//���ݵ�ǰ��������½���ؼ�״̬
	void setMassage(const char * str)
	{
		XE::LogStr(str);
		m_allMsgText.setString(str);
	}
	void updateLanguage();	//��������ѡ����½����ϵ�����(Ŀǰ��δ��Ч)
	enum GLanguageIndex
	{
		INDEX_ENGLISH,
		INDEX_CHINESE,
	};
	GLanguageIndex getLanguageIndex() 
	{
		switch(m_languageCmb.getCurChooseOrder())
		{
		case 0: return INDEX_ENGLISH;
		case 1: return INDEX_CHINESE;
		}
		return INDEX_ENGLISH;
	}

	GLanguage m_language;	//�����Ե�֧��
};

#endif