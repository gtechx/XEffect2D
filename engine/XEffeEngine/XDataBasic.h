#ifndef _JIA_XDATABASIC_
#define _JIA_XDATABASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "stdio.h" 
//#include "XBasicClass.h"
#include "XThread.h"
namespace XE{
//����һ������ת�����洢����ʽ�������ܡ��������
//Ϊ�����ݵİ�ȫ������ʹ��AB��������ķ�ʽ�����Խϴ�Χ��ȷ��������һ�����еı���
#define DATABASIC_DEFAULT_FILE_NAME_A "databasicA.dat"
#define DATABASIC_DEFAULT_FILE_NAME_B "databasicB.dat"

enum XAnalyzingFileMode
{
    ANALYZING_BUFFERING,    //���屣��    //���ݲ���ȫ��Ӳ����ȫ��Ч
    ANALYZING_IMMEDIATELY    //��ʱ����    //���ݰ�ȫ��Ӳ�����
};
class XDataBasic
{
private:
	static const int m_databasicKeyLength = 128;
    int m_ID;                    //������ݵ�ID�����ڴ������ݹ���
    int m_versionsOrder;        //���ڼ�¼���ݵİ汾��ÿ�����ݵĸı䶼��ı���汾���Ա���ȷ�����ļ����Ⱥ�˳��
    int m_dataLength;            //���ݳ���
    unsigned char *m_pData;        //����
    unsigned char m_checkData;    //У���ַ�
    char m_curBackOrder;        //��ǰ�����ı������
    XBool m_isInited;            //�Ƿ��ʼ��
	XBool m_isSaveAsynchronous;	//�Ƿ�ʹ���첽���ݱ���
	XBool m_needSave;			//�����Ƿ���Ҫ����
    int m_secretKeyLength;        //��Կ����
    unsigned char *m_secretKey;    //����ʹ�õ���Կ��������Խ��м򵥵ļ��ܺ�У�飩

    XBool EncryptData();            //��������
    XBool DeCryptData();            //��������
    unsigned char getCheckData();        //����У����
    XBool checkDataIsRigth();    //��������Ƿ����У��

    XBool mergeDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    XBool mergeDataBack(const char *fileNameA,const char *fileNameB);    //������������ڲ����ã������������������߼�
    XBool analyzingDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    XBool analyzingData(const char *fileName);    //�����ݱ��浽�ļ�

	char *m_fileName;			//���ڱ������ݵ��ļ���
	
	XAnalyzingFileMode m_saveMode;	//�ļ��Ĵ洢ģʽ

	XThreadState m_isEnd;	//�Ƿ����
	
	XDataBasic(const XDataBasic &)	//���ؿ������캯��Ϊ˽�з�ֹ����Ҫ�Ĵ���
	{}
public:
    XDataBasic();
	~XDataBasic(){release();}
	void release();

    XBool init(int ID,XBool isSaveAsynchronous = XFalse,XAnalyzingFileMode saveMode = ANALYZING_BUFFERING);
    //�ڴ������ݼ��ת��
    XBool mergeData(const void *pData,int dataLength);        //���ⲿ�ϲ����ݵ��ڲ�
    int analyzingData(void *pData,int dataLength);			//���ڲ����ݽ������ⲿ,�����ڲ����ݵĴ�С��-1Ϊ����
    //�ļ������ݼ��ת��
    XBool mergeData(const char *fileName = NULL);            //���ļ�����ȡ����
    XBool analyzingDataManager(const char *fileName = NULL);        //�����ݱ��浽�ļ���������������֮ǰ�û������öԱ���ľ���������е���
    //���������ݼ��ת��(Ŀǰ��ʱ�����ṩ���֧��)

	//�첽���ݱ����߳�
#ifdef XEE_OS_WINDOWS
	static DWORD WINAPI saveDataThread(void * pParam);
#endif
#ifdef XEE_OS_LINUX
	static void *saveDataThread(void * pParam); 
#endif 
	pthread_t m_saveDataThreadP;

	void endDataBasic();	//�˳�ʱ�ر��߳�
};
inline void XDataBasic::endDataBasic()
{
    if(!m_isInited) return;
	if(m_isSaveAsynchronous) m_isEnd = STATE_SET_TO_END;
}
inline XBool XDataBasic::checkDataIsRigth()    //��������Ƿ����У��
{
    if(!m_isInited ||
		m_dataLength <= 0 ||
		m_pData == NULL) return XFalse;
    return getCheckData() == m_checkData;
}
inline void XDataBasic::release()
{
	if(!m_isInited) return;
	if(m_isSaveAsynchronous) waitThreadEnd(m_isEnd);//�ȴ��߳̽���
	XMem::XDELETE_ARRAY(m_secretKey);
	m_isInited = XFalse;
}
inline int XDataBasic::analyzingData(void *pData,int dataLength)    //���ڲ����ݽ������ⲿ
{
    if(!m_isInited ||
		dataLength <= 0) return -1;
    if(m_pData == NULL || m_dataLength <= 0) return 0;
    if(dataLength <= m_dataLength) memcpy(pData,m_pData,dataLength);
    else memcpy(pData,m_pData,m_dataLength);
    return m_dataLength;
}
}
#endif