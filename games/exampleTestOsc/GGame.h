#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "ofxOsc.h"

struct _GOscData
{
	std::string data;		//����
	std::string address;	//��ַ
	int dataType;			//��������
};
struct _GKMOscData
{
	int keyValue;
	int keyState;
	std::string data;		//����
	std::string address;	//��ַ
	int dataType;			//��������
};
#define CFG_FILENAME ("config.xml")

class _GGame
{
protected:
	_GGame()
		:m_isSenderOpen(false)
		,m_isSendState(false)
		,m_recvState(false)
		,m_mouseOnTimer(0)
		,m_isShowMsg(false)
		,m_recvSum(0)
	{}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
public:
	bool init();				//TODO:���������Ϸ�ĳ�ʼ��
	void move(int stepTime);	//TODO:��������Ϸ��idle��Ҳ����ͨ�������߼��̵߳ķ�ʽ����
	void draw();				//TODO:���Ƶĺ���
	void input(const _XInputEvent &event);	//TODO:���̻������������¼�
	void release();				//TODO:�������ʱ����Դ�ͷ�
	//������Ϊ�˴���ʵʱ�仯���ƶ�����ı���
	int m_windowPosX;		//���ڵ�λ��
	int m_windowPosY;		//���ڵ�λ��
	int m_windowSizeW;		//���ڵĳߴ�
	int m_windowSizeH;		//���ڵĳߴ�
	_XMoveData m_moveData;
	bool m_isFirst;
	bool m_isExit;	//�Ƿ񴥷����˳��¼�
	friend void windowExitFun(void *p);
private:
	_XTab m_tab;
	//�����ǹ��ڽ���Ԫ�صĶ���
	_XText m_textIP;
	_XEdit m_textIPEdt;
	_XText m_textPort;
	_XEdit m_textPortEdt;
	_XButton m_openBtn;
	//_XButton m_closeBtn;
	//++++++++++++++++++++++++++++++
	//������Ϊ�����Ӽ���ģ�������ı���
	_XButton m_addKMBtn;
	_XButton m_delKMBtn;
	_XButton m_clearKMBtn;	//���
	_XButton m_modifyKMBtn;	//�޸İ�ť
	_XCombo m_keySelectCmb;	//������ѡ��
	_XCombo m_keyStateCmb;	//����״̬��ѡ��
	_XText m_oscDataTxt;
	_XEdit m_oscDataEdt;
	_XText m_oscType;
	_XCombo m_oscDataCmb;
	_XText m_oscAddress;
	_XEdit m_oscAddressEdt;
	_XMultiList m_dataKMLst;
	std::vector<_GKMOscData *> m_keyMapOscList;	//ӳ������
	std::string keyValueToStr(int keyValue);
	std::string oscDataTypeToStr(int type);
	int keyValueToIndex(_XKeyValue k);
	_XBool checkKMData(int index = -1);	//��鵱ǰ�������Ƿ�Ϸ�
	void sendSelectData();	//���͵�ǰѡ������
	void sendKMData(int index);
	int m_mouseX;
	int m_mouseY;
	//------------------------------
	_XButton m_addBtn;
	_XButton m_delBtn;
	_XButton m_clearBtn;	//���
	_XButton m_modifyBtn;	//�޸İ�ť
	_XText m_textData;
	_XEdit m_textDataEdt;
	_XText m_textType;
	_XCombo m_textDataCmb;
	_XText m_textAddress;
	_XEdit m_textAddressEdt;
	_XText m_textTimes;
	_XEdit m_textTimesEdt;
	_XText m_textSpacing;
	_XEdit m_textSpacingEdt;
	_XButton m_sendBtn;
	//_XButton m_stopBtn;
	_XCombo m_sendModeCmb;
	_XMultiList m_dataLst;
	_XCheck m_sendAsMsg;
	_XCheck m_supportACKMode;
	_XButton m_clearRecvBtn;	//������տ�İ�ť

//	_XFontUnicode m_textRIP;
//	_XEdit m_textRIPEdt;
	_XText m_textRPort;
	_XEdit m_textRPortEdt;
	_XButton m_openRBtn;
	//_XButton m_closeRBtn;

	_XButton m_saveBtn;		//���水ť
	_XButton m_readBtn;		//��ȡ�����ļ��İ�ť
	bool saveToFile(const char *filename = NULL);		//���浽�ļ�
	bool readFromFile(const char *filename = NULL);	//���ļ���ȡ
	void addALineData(const _GOscData *tmp);
	void setLineData(const _GOscData *tmp,int index);
	void addALineData(const _GKMOscData *tmp);
	void setLineData(const _GKMOscData *tmp,int index);

	bool m_recvState;		//���յ�״̬
	int m_recvSum;	//���յ�ͳ�Ƽ���

	_XMultiText m_recvText;
	_XFontUnicode m_allMsgText;	//�������ʾ����
	friend void ctrlFun(void *,int);
	friend void ctrlFunX(void *,int);
	//�����ǹ��ڹ��ܵĶ���
	//���Ͳ���
	ofxOscSender m_sender;
	bool m_isSenderOpen;	//�Ƿ��Ѿ��򿪷����豸
	std::vector<_GOscData *> m_datasVector;
	bool m_isSendState;		//�Ƿ����ڷ��͹�����
	int m_timer;	//���ͼ�ʱ��
	int m_times;	//���ʹ����ۼ���
	int m_setTimer;
	int m_setTimes;
	void sendOverProc();
	void sendButtonDown();
	_XBool checkNowData();	//��鵱ǰ���ݵ���ȷ��
	void sendNowData(int mode);
	std::string m_nowSendAddr;		//��ǰ���͵����ݵ�������

	ofxOscReceiver m_receiver;
	//�����������������ʾ
	int m_mouseOnTimer;	//���ڼ�¼���û���ƶ�������ͣ״̬��ʱ��	
	bool m_isShowMsg;	//�Ƿ��Ѿ���ʾ��ʾ
	void ctrlMsgProc();
private:
	void ctrlStateManager();	//���ݵ�ǰ��������½���ؼ�״̬
};

#endif