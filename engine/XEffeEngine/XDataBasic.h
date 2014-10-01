#ifndef _JIA_XDATABASIC_
#define _JIA_XDATABASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "stdio.h" 

#ifdef XEE_OS_WINDOWS
#include "windows.h"
#endif

#include "XBasicClass.h"

//����һ������ת�����洢����ʽ�������ܡ��������
//Ϊ�����ݵİ�ȫ������ʹ��AB��������ķ�ʽ�����Խϴ�Χ��ȷ��������һ�����еı���
#define DATABASIC_DEFAULT_FILE_NAME_A "databasicA.dat"
#define DATABASIC_DEFAULT_FILE_NAME_B "databasicB.dat"
#define DATABASIC_KEY_LENGTH 128

enum _XAnalyzingFileMode
{
    ANALYZING_BUFFERING,    //���屣��    //���ݲ���ȫ��Ӳ����ȫ��Ч
    ANALYZING_IMMEDIATELY    //��ʱ����    //���ݰ�ȫ��Ӳ�����
};

class _XDataBasic
{
private:
    int m_ID;                    //������ݵ�ID�����ڴ������ݹ���
    int m_versionsOrder;        //���ڼ�¼���ݵİ汾��ÿ�����ݵĸı䶼��ı���汾���Ա���ȷ�����ļ����Ⱥ�˳��
    int m_dataLength;            //���ݳ���
    unsigned char *m_pData;        //����
    unsigned char m_checkData;    //У���ַ�
    int m_secretKeyLength;        //��Կ����
    unsigned char *m_secretKey;    //����ʹ�õ���Կ��������Խ��м򵥵ļ��ܺ�У�飩
    _XBool m_isInited;            //�Ƿ��ʼ��

    _XBool EncryptData();            //��������
    _XBool DeCryptData();            //��������
    unsigned char getCheckData();        //����У����
    _XBool checkDataIsRigth();    //��������Ƿ����У��
    char m_nowBackOrder;        //��ǰ�����ı������

    _XBool mergeDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    _XBool mergeDataBack(const char *fileNameA,const char *fileNameB);    //������������ڲ����ã������������������߼�
    _XBool analyzingDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    _XBool analyzingData(const char *fileName);    //�����ݱ��浽�ļ�

	_XBool m_isSaveAsynchronous;	//�Ƿ�ʹ���첽���ݱ���
	_XBool m_needSave;			//�����Ƿ���Ҫ����
	char *m_fileName;			//���ڱ������ݵ��ļ���
	
	_XAnalyzingFileMode m_saveMode;	//�ļ��Ĵ洢ģʽ

	_XThreadState m_isEnd;	//�Ƿ����
	
	_XDataBasic(const _XDataBasic &)	//���ؿ������캯��Ϊ˽�з�ֹ����Ҫ�Ĵ���
	{}
public:
    _XDataBasic();
    ~_XDataBasic();
	void release();

    _XBool init(int ID,_XBool isSaveAsynchronous = XFalse,_XAnalyzingFileMode saveMode = ANALYZING_BUFFERING);
    //�ڴ������ݼ��ת��
    _XBool mergeData(const unsigned char *pData,int dataLength);        //���ⲿ�ϲ����ݵ��ڲ�
    int analyzingData(unsigned char *pData,int dataLength);			//���ڲ����ݽ������ⲿ,�����ڲ����ݵĴ�С��-1Ϊ����
    //�ļ������ݼ��ת��
    _XBool mergeData(const char *fileName = NULL);            //���ļ�����ȡ����
    _XBool analyzingDataManager(const char *fileName = NULL);        //�����ݱ��浽�ļ���������������֮ǰ�û������öԱ���ľ���������е���
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
inline void _XDataBasic::endDataBasic()
{
    if(!m_isInited) return;
	if(m_isSaveAsynchronous) m_isEnd = STATE_SET_TO_END;
}

#endif