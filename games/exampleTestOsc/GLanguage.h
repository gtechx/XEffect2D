#ifndef _JIA_GLANGUAGE_
#define _JIA_GLANGUAGE_
#include "XLanguage.h"
class GLanguage:public XE::XLanguageBasic
{
protected:
	virtual XE::XBool loadFromFile(const std::string &filename);	//���ļ��ж�ȡ�ַ���
	virtual XE::XBool saveToFile(const std::string &filename);	//���ļ��ж�ȡ�ַ���
public:
	virtual XE::XBool setCurrentLanguage(XE::XLanguage language,const std::string &filename);	//��ָ���ļ��������ַ�����Դ��ʹ֮��Ч
	GLanguage()
		:m_tabTitileStr("���Ͷ˿�����;������������;�������ģ��;���ն�����;ϵͳ����;")
		,m_openBtnOpenStr("��")
		,m_openBtnCloseStr("�ر�")
		,m_addKMBtnStr("���")
		,m_delKMBtnStr("ɾ��")
		,m_clearKMBtnStr("���")
		,m_modifyKMBtnStr("�޸�")
		,m_oscDataTxtStr("����:")
		,m_oscTypeStr("����:")
		,m_oscAddressStr("��ַ:")
		,m_addBtnStr("���")
		,m_delBtnStr("ɾ��")
		,m_clearBtnStr("���")
		,m_modifyBtnStr("�޸�")
		,m_textDataStr("����:")
		,m_textTypeStr("����:")
		,m_textAddressStr("��ַ:")
		,m_textTimesStr("����:")
		,m_textSpacingStr("���:")
		,m_sendBtnOpenStr("ֹͣ����")
		,m_sendBtnCloseStr("��ʼ����")
		,m_sendModeCmbStr("����ѡ�е�ַ;����ѡ������;���Ϳ�����;���͵�ǰ����;")
		,m_sendAsMsgStr("��Ϊ��Ϣ����")
		,m_supportACKModeStr("�Ƿ��ʴ����")
		,m_clearRecvBtnStr("������տ�")
		,m_openRBtnOpenStr("ֹͣ����")
		,m_openRBtnCloseStr("��ʼ����")
		,m_saveBtnStr("����")
		,m_readBtnStr("��ȡ")
		//������һЩ�ؼ���ע��
		,m_textIPEdtCommentStr("����OSC���Ͷ˵�IP��ַ������IPʱ��������127.0.0.1")
		,m_textPortEdtCommentStr("����OSC���Ͷ˵Ķ˿ںţ��˿ں���Ҫ����ն˵Ķ˿ں�һ�£�Ĭ��Ϊ12345")
		,m_openBtnCommentStr("�򿪻��߹ر�OSC��������ȷ��IP��ַ�Ͷ˿���ȷ�����򲻼������ȷ��")
		,m_addBtnCommentStr("����߱༭�õ�OSC������Ϣ��ӵ��ұߵ��б����Ա���֮�����")
		,m_delBtnCommentStr("ɾ������б���ѡ�е�����")
		,m_clearBtnCommentStr("����ұ��б���е���������")
		,m_modifyBtnCommentStr("���ұ��б���ѡ��һ�����ݲ��޸ģ����¸ð�ť���޸ĺ�����ݸ��µ��б���")
		,m_textDataEdtCommentStr("OSC��Ϣ�ж�Ӧ�����ݣ���ȷ��������������������������ƥ��")
		,m_textDataCmbCommentStr("OSC��Ϣ�����ݵ��������ͣ���ȷ����������������ƥ��")
		,m_textAddressEdtCommentStr("OSC��Ϣ�е�ַ")
		,m_textTimesEdtCommentStr("OSC��Ϣ��Ҫѭ�����͵Ĵ������������ͽ�����������ֵ��ѭ������")
		,m_textSpacingEdtCommentStr("���η���֮��ļ��ʱ�䣬��λΪ����(ms)��1����Ϊǧ��֮һ��")
		,m_sendModeCmbCommentStr("ѡ����Ҫ�ķ���ģʽ���������ַ���ģʽ���������ʣ������Ķ�˵���ĵ�")
		,m_sendAsMsgCommentStr("��ѡʱ��Ҫ���͵�������Ϊһ����Ϣ���ͣ�������Ϊһ�����ݰ�����")
		,m_sendBtnCommentStr("�����趨�������ݻ�ֹͣ���ͣ�����ǰ����ȷ��OSC�������Ѿ���ȷ��")
		,m_textRPortEdtCommentStr("OSC�������Ķ˿ںţ���������Ӧ�������Ķ˿ں�һ�²�����ȷ���գ�Ĭ��Ϊ12345")
		,m_openRBtnCommentStr("�򿪻�ر�OSC����������ȷ���������˿ں�������ȷ")
		,m_supportACKModeCommentStr("Ӧ��ģʽ������ʱ�������յ�ĳ��ַ�Ŀ�����ʱ�����Զ����ظõ�ַ������")
		,m_clearRecvBtnCommentStr("���¸ð�ť��ս����б������б��е��������ݽ������")
		,m_languageCmbCommentStr("ѡ��ʹ�õ�����")
		,m_codeModeCmbCommentStr("ѡ��ʹ�õı��뷽ʽ")
		,m_saveBtnCommentStr("����ǰ�����ϵ��������ݱ��浽�ļ���")
		,m_readBtnCommentStr("�������ļ��ж�ȡ��һ�α������������")
		,m_addKMBtnCommentStr("����߱༭�õ���������OSC��ӳ����ӵ�ӳ���б���")
		,m_delKMBtnCommentStr("ɾ��ӳ���б���ѡ�����")
		,m_clearKMBtnCommentStr("�����ߵ�ӳ���б�")
		,m_modifyKMBtnCommentStr("����߱༭�õ�ֵ���µ�ӳ���б���ѡ�е���")
		,m_keySelectCmbCommentStr("��Ӧ�������߼��̰���")
		,m_keyStateCmbCommentStr("������״̬")
		,m_oscDataEdtCommentStr("OSC����")
		,m_oscDataCmbCommentStr("OSC��������")
		,m_oscAddressEdtCommentStr("OSC��ַ")
	{}
	virtual ~GLanguage(){}
	//�����Ƕ���Ҫ���ַ������ж���
	std::string m_tabTitileStr;
	std::string m_openBtnOpenStr;
	std::string m_openBtnCloseStr;
	std::string m_addKMBtnStr;
	std::string m_delKMBtnStr;
	std::string m_clearKMBtnStr;
	std::string m_modifyKMBtnStr;
	std::string m_oscDataTxtStr;
	std::string m_oscTypeStr;
	std::string m_oscAddressStr;
	std::string m_addBtnStr;
	std::string m_delBtnStr;
	std::string m_clearBtnStr;
	std::string m_modifyBtnStr;
	std::string m_textDataStr;
	std::string m_textTypeStr;
	std::string m_textAddressStr;
	std::string m_textTimesStr;
	std::string m_textSpacingStr;
	std::string m_sendBtnOpenStr;
	std::string m_sendBtnCloseStr;
	std::string m_sendModeCmbStr;
	std::string m_sendAsMsgStr;
	std::string m_supportACKModeStr;
	std::string m_clearRecvBtnStr;
	std::string m_openRBtnOpenStr;
	std::string m_openRBtnCloseStr;
	std::string m_saveBtnStr;
	std::string m_readBtnStr;
	//������һЩ�ؼ���ע��
	std::string m_textIPEdtCommentStr;//("����OSC���Ͷ˵�IP��ַ������IPʱ��������127.0.0.1");
	std::string m_textPortEdtCommentStr;//("����OSC���Ͷ˵Ķ˿ںţ��˿ں���Ҫ����ն˵Ķ˿ں�һ�£�Ĭ��Ϊ12345");
	std::string m_openBtnCommentStr;//("�򿪻��߹ر�OSC��������ȷ��IP��ַ�Ͷ˿���ȷ�����򲻼������ȷ��");
	std::string m_addBtnCommentStr;//("����߱༭�õ�OSC������Ϣ��ӵ��ұߵ��б����Ա���֮�����");
	std::string m_delBtnCommentStr;//("ɾ������б���ѡ�е�����");
	std::string m_clearBtnCommentStr;//("����ұ��б���е���������");
	std::string m_modifyBtnCommentStr;//("���ұ��б���ѡ��һ�����ݲ��޸ģ����¸ð�ť���޸ĺ�����ݸ��µ��б���");
	std::string m_textDataEdtCommentStr;//("OSC��Ϣ�ж�Ӧ�����ݣ���ȷ��������������������������ƥ��");
	std::string m_textDataCmbCommentStr;//("OSC��Ϣ�����ݵ��������ͣ���ȷ����������������ƥ��");
	std::string m_textAddressEdtCommentStr;//("OSC��Ϣ�е�ַ");
	std::string m_textTimesEdtCommentStr;//("OSC��Ϣ��Ҫѭ�����͵Ĵ������������ͽ�����������ֵ��ѭ������");
	std::string m_textSpacingEdtCommentStr;//("���η���֮��ļ��ʱ�䣬��λΪ����(ms)��1����Ϊǧ��֮һ��");
	std::string m_sendModeCmbCommentStr;//("ѡ����Ҫ�ķ���ģʽ���������ַ���ģʽ���������ʣ������Ķ�˵���ĵ�");
	std::string m_sendAsMsgCommentStr;//("��ѡʱ��Ҫ���͵�������Ϊһ����Ϣ���ͣ�������Ϊһ�����ݰ�����");
	std::string m_sendBtnCommentStr;//("�����趨�������ݻ�ֹͣ���ͣ�����ǰ����ȷ��OSC�������Ѿ���ȷ��");
	std::string m_textRPortEdtCommentStr;//("OSC�������Ķ˿ںţ���������Ӧ�������Ķ˿ں�һ�²�����ȷ���գ�Ĭ��Ϊ12345");
	std::string m_openRBtnCommentStr;//("�򿪻�ر�OSC����������ȷ���������˿ں�������ȷ");
	std::string m_supportACKModeCommentStr;//("Ӧ��ģʽ������ʱ�������յ�ĳ��ַ�Ŀ�����ʱ�����Զ����ظõ�ַ������");
	std::string m_clearRecvBtnCommentStr;//("���¸ð�ť��ս����б������б��е��������ݽ������");
	std::string m_languageCmbCommentStr;//("ѡ��ʹ�õ�����");
	std::string m_codeModeCmbCommentStr;//("ѡ����뷽ʽ");
	std::string m_saveBtnCommentStr;//("����ǰ�����ϵ��������ݱ��浽�ļ���");
	std::string m_readBtnCommentStr;//("�������ļ��ж�ȡ��һ�α������������");
	std::string m_addKMBtnCommentStr;//("����߱༭�õ���������OSC��ӳ����ӵ�ӳ���б���");
	std::string m_delKMBtnCommentStr;//("ɾ��ӳ���б���ѡ�����");
	std::string m_clearKMBtnCommentStr;//("�����ߵ�ӳ���б�");
	std::string m_modifyKMBtnCommentStr;//("����߱༭�õ�ֵ���µ�ӳ���б���ѡ�е���");
	std::string m_keySelectCmbCommentStr;//("��Ӧ�������߼��̰���");
	std::string m_keyStateCmbCommentStr;//("������״̬");
	std::string m_oscDataEdtCommentStr;//("OSC����");
	std::string m_oscDataCmbCommentStr;//("OSC��������");
	std::string m_oscAddressEdtCommentStr;//("OSC��ַ");
};
#endif