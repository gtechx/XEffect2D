#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XDataBasic.h"
//#include "string.h"
//#include "XBasicFun.h"
//#include "XOSDefine.h"
namespace XE {
#define DEBUGXDB (0)

XDataBasic::XDataBasic()
	:m_isInited(XFalse)
	, m_dataLength(0)
	, m_pData(NULL)
	, m_checkData(0)
	, m_secretKeyLength(0)
	, m_secretKey(NULL)
	, m_curBackOrder(0)
	, m_ID(0)
	, m_versionsOrder(0)
	, m_isSaveAsynchronous(XFalse)
	, m_needSave(XFalse)
	, m_fileName(NULL)
	, m_saveMode(ANALYZING_BUFFERING)
{}
XBool XDataBasic::EncryptData()            //��������
{
	if (!m_isInited ||
		m_secretKeyLength <= 0 ||
		m_secretKey == NULL ||
		m_dataLength <= 0 ||
		m_pData == NULL) return XFalse;
	//�������������
	for (int i = 0; i < m_secretKeyLength; ++i)
	{
		m_secretKey[i] = XRand::random(256);
	}
	//��������
	unsigned char temp;
	for (int i = 0, j = 0; i < m_dataLength; ++i, ++j)
	{
		if (j >= m_secretKeyLength) j = 0;
		temp = m_secretKey[j] + (i % 256);
		m_pData[i] = temp ^ m_pData[i];
	}
	return XTrue;
}
XBool XDataBasic::DeCryptData()            //��������
{//ͬ���ļ��ܲ����ٽ���һ�ξ��ǽ���
	if (!m_isInited ||
		m_secretKeyLength <= 0 ||
		m_secretKey == NULL ||
		m_dataLength <= 0 ||
		m_pData == NULL) return XFalse;
	unsigned char temp;
	for (int i = 0, j = 0; i < m_dataLength; ++i, ++j)
	{
		if (j >= m_secretKeyLength) j = 0;
		temp = m_secretKey[j] + (i % 256);
		m_pData[i] = temp ^ m_pData[i];
	}
	return XTrue;
}
unsigned char XDataBasic::getCheckData()        //����У����
{
	if (!m_isInited ||
		m_dataLength <= 0 ||
		m_pData == NULL) return 0;
	unsigned char temp = 0;
	for (int i = 0; i < m_dataLength; ++i)
	{
		temp += m_pData[i];
	}
	return temp;
}
XBool XDataBasic::init(int ID, XBool isSaveAsynchronous, XAnalyzingFileMode saveMode)
{
	if (m_isInited) return XFalse;
	m_ID = ID;
	m_secretKeyLength = m_databasicKeyLength;
	m_isSaveAsynchronous = isSaveAsynchronous;
	m_saveMode = saveMode;
	m_secretKey = XMem::createArrayMem<unsigned char>(m_secretKeyLength);
	if (m_secretKey == NULL) return XFalse;
	for (int i = 0; i < m_secretKeyLength; ++i)
	{
		m_secretKey[i] = XRand::random(256);
	}

	if (m_isSaveAsynchronous)
	{//���ʹ���첽���ݱ��淽ʽ���￪�����溯��
		m_isEnd = STATE_BEFORE_START;
#ifdef XEE_OS_LINUX
		if (pthread_create(&m_saveDataThreadP, NULL, &saveDataThread, (void*) this) != 0)
#endif
#ifdef XEE_OS_WINDOWS
			if (CreateThread(0, 0, saveDataThread, this, 0, &m_saveDataThreadP) == 0)
#endif
			{//�����߳�ʧ��
				XMem::XDELETE_ARRAY(m_secretKey);
				return XFalse;
			}
	}

	m_isInited = XTrue;
	return XTrue;
}
//�ڴ������ݼ��ת��
XBool XDataBasic::mergeData(const void *pData, int dataLength)        //���ⲿ�ϲ����ݵ��ڲ�
{
	if (!m_isInited ||
		pData == NULL ||
		dataLength <= 0) return XFalse;

	if (m_pData == NULL)
	{//��һ�δ�������
		m_dataLength = dataLength;
		m_pData = XMem::createArrayMem<unsigned char>(m_dataLength);
		if (m_pData == NULL) return XFalse;
		memcpy(m_pData, pData, m_dataLength);
	}
	else
	{
		if (m_dataLength == dataLength)
		{//����Ҫ���·����ڴ�ռ�
			memcpy(m_pData, pData, m_dataLength);
		}
		else
		{
			XMem::XDELETE_ARRAY(m_pData);
			m_dataLength = dataLength;
			m_pData = XMem::createArrayMem<unsigned char>(m_dataLength);
			if (m_pData == NULL) return XFalse;
			memcpy(m_pData, pData, m_dataLength);
		}
	}
	return XTrue;
}
XBool XDataBasic::mergeDataBack(const char *fileName)
{
	if (!m_isInited ||
		fileName == NULL || strlen(fileName) <= 0) return XFalse;
	FILE *fp;
	if ((fp = fopen(fileName, "rb")) == NULL) return XFalse;//file open error!

	if (fread(&m_versionsOrder, sizeof(int), 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
#if DEBUGXDB
	printf("read:%s - %d\n", fileName, m_versionsOrder);
#endif
	if (fread(&m_ID, sizeof(int), 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
	if (fread(&m_dataLength, sizeof(int), 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
	if (m_dataLength > 0)
	{//��Ҫ��ȡ���ݲ���
		XMem::XDELETE_ARRAY(m_pData);
		m_pData = XMem::createArrayMem<unsigned char>(m_dataLength);
		if (m_pData == NULL)
		{
			fclose(fp);
			return XFalse;
		}
	}
	if (m_dataLength < 0)
	{
		fclose(fp);
		return XFalse;
	}
	if (m_pData == NULL || fread(m_pData, m_dataLength, 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
	if (fread(&m_secretKeyLength, sizeof(int), 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
	if (m_secretKeyLength > 0)
	{//��Ҫ��ȡ���ݲ���
		XMem::XDELETE_ARRAY(m_secretKey);
		m_secretKey = XMem::createArrayMem<unsigned char>(m_secretKeyLength);
		if (m_secretKey == NULL)
		{
			fclose(fp);
			return XFalse;
		}
	}
	if (m_secretKeyLength < 0)
	{
		fclose(fp);
		return XFalse;
	}
	if (m_secretKey == NULL || fread(m_secretKey, m_secretKeyLength, 1, fp) < 1)
	{
		fclose(fp);
		return XFalse;
	}
	if (fread(&m_checkData, sizeof(unsigned char), 1, fp) < 0)
	{
		fclose(fp);
		return XFalse;
	}
	if (m_checkData != getCheckData())
	{
		fclose(fp);
		return XFalse;
	}
	if (!DeCryptData())
	{
		fclose(fp);
		return XFalse;
	}
	fclose(fp);
	return XTrue;
}

XBool XDataBasic::mergeDataBack(const char *fileNameA, const char *fileNameB)    //������������ڲ����ã������������������߼�
{
	if (!m_isInited ||
		fileNameA == NULL || fileNameB == NULL) return XFalse;
	int version0 = 0;
	int version1 = 0;
	int stateFile0 = 1;
	int stateFile1 = 1;
	FILE *fp;
	//��ȡ��һ���ļ��İ汾��
	if ((fp = fopen(fileNameA, "rb")) == NULL)
	{//file open error!
		//printf("%s Open Error!\n",fileNameA);
		stateFile0 = 0;
	}
	if (stateFile0 == 1)
	{
		if (fread(&version0, sizeof(int), 1, fp) < 1)
		{
			stateFile0 = 0;
		}
		fclose(fp);
	}
#if DEBUGXDB
	printf("%s - %d\n", fileNameA, version0);
#endif
	//��ȡ�ڶ����ļ��İ汾��
	if ((fp = fopen(fileNameB, "rb")) == NULL)
	{//file open error!
		//printf("%s Open Error!\n",fileNameB);
		stateFile1 = 0;
	}
	if (stateFile1 == 1)
	{
		if (fread(&version1, sizeof(int), 1, fp) < 1)
		{
			stateFile1 = 0;
		}
		fclose(fp);
	}
#if DEBUGXDB
	printf("%s - %d\n", fileNameB, version1);
#endif

	if (stateFile0 == 0 && stateFile1 == 0)
	{//��������ļ������ܴ򿪣���ֱ�ӷ���ʧ��
		return XFalse;
	}

	if (stateFile0 == 0 || stateFile1 == 0)
	{//��һ���ļ��Ѿ��𻵣����û���𻵵��ļ�����ȡ����
		if (stateFile0 == 1) return mergeDataBack(fileNameA);	//�ӵ�һ���ļ�����ȡ����
		else return mergeDataBack(fileNameB);
	}

	if (stateFile0 != 0 && stateFile1 != 0)
	{//�����ʱ�����ļ���û����������ȡ����İ汾
//        int tempResult = XTrue;
		if (version0 > version1)
		{//��ȡ��һ���ļ�
			if (mergeDataBack(fileNameA)) return XTrue;
		}
		else
		{//��ȡ�ڶ����ļ�
			if (mergeDataBack(fileNameB)) return XTrue;
		}

		//�����ȡʧ�������һ���ļ�����ȡ
		if (version0 > version1)
		{//��ȡ��һ���ļ�
			if (mergeDataBack(fileNameB)) return XTrue;
		}
		else
		{//��ȡ�ڶ����ļ�
			if (mergeDataBack(fileNameA)) return XTrue;
		}
		//���ȫ����ȡʧ���򷵻�ʧ��
		return XFalse;
	}
	return XTrue;
}
//�ļ������ݼ��ת��
XBool XDataBasic::mergeData(const char *fileName)            //���ļ�����ȡ����
{//�������ļ��зֱ���ȡ���ݣ����ȫ�������򷵻ش������ȫ���ɹ�����ȡ����İ汾��������ȡ�ɹ��İ汾
	if (!m_isInited) return XFalse;
	if (fileName != NULL)
	{
		char *fileRealNameA = NULL;
		char *fileRealNameB = NULL;
		int len = strlen(fileName);
		if (len <= 0) return XFalse;
		fileRealNameA = XMem::createArrayMem<char>(len + 2);
		if (fileRealNameA == NULL) return XFalse;
		fileRealNameB = XMem::createArrayMem<char>(len + 2);
		if (fileRealNameB == NULL)
		{
			XMem::XDELETE_ARRAY(fileRealNameA);
			return XFalse;
		}
		//ȷ���ļ���
		int dotPoint = -1;
		for (int i = 0; i < len; ++i)
		{
			if (fileName[i] == '.')
			{
				dotPoint = i;
			}
		}
		if (dotPoint == -1)
		{
			memcpy(fileRealNameA, fileName, len);
			fileRealNameA[len] = 'A';
			fileRealNameA[len + 1] = '\0';
			memcpy(fileRealNameB, fileName, len);
			fileRealNameB[len] = 'B';
			fileRealNameB[len + 1] = '\0';
		}
		else
		{
			memcpy(fileRealNameA, fileName, dotPoint);
			fileRealNameA[dotPoint] = 'A';
			memcpy(fileRealNameA + dotPoint + 1, fileName + dotPoint, len - dotPoint);
			fileRealNameA[len + 1] = '\0';
			memcpy(fileRealNameB, fileName, dotPoint);
			fileRealNameB[dotPoint] = 'B';
			memcpy(fileRealNameB + dotPoint + 1, fileName + dotPoint, len - dotPoint);
			fileRealNameB[len + 1] = '\0';
		}
		if (mergeDataBack(fileRealNameA, fileRealNameB))
		{
			XMem::XDELETE_ARRAY(fileRealNameA);
			XMem::XDELETE_ARRAY(fileRealNameB);
			return XTrue;
		}
		else
		{
			XMem::XDELETE_ARRAY(fileRealNameA);
			XMem::XDELETE_ARRAY(fileRealNameB);
			return XFalse;
		}
	}
	else
	{
		return mergeDataBack(DATABASIC_DEFAULT_FILE_NAME_A, DATABASIC_DEFAULT_FILE_NAME_B);
	}
}
XBool XDataBasic::analyzingDataBack(const char *fileName)    //������������ڲ����ã������������������߼�
{
	if (!m_isInited ||
		fileName == NULL || strlen(fileName) <= 0) return XFalse;
	if (m_saveMode == ANALYZING_BUFFERING)
	{
		FILE *fp;
		if ((fp = fopen(fileName, "wb")) == NULL) return XFalse;//file open error!
		//������д�뵽�ļ�
		if (!EncryptData())
		{
			fclose(fp);
			return XFalse;
		}
#if DEBUGXDB
		printf("write:%s - %d\n", m_fileName, m_versionsOrder);
#endif	    
		if (fwrite(&m_versionsOrder, sizeof(int), 1, fp) < 1)
		{
			fclose(fp);
			return XFalse;
		}
		if (fwrite(&m_ID, sizeof(int), 1, fp) < 1)
		{
			fclose(fp);
			return XFalse;
		}
		if (m_dataLength < 0) m_dataLength = 0;
		if (fwrite(&m_dataLength, sizeof(int), 1, fp) < 1)
		{
			fclose(fp);
			return XFalse;
		}

		if (m_pData != NULL && m_dataLength > 0)
		{
			if (fwrite(m_pData, m_dataLength, 1, fp) < 1)
			{
				fclose(fp);
				return XFalse;
			}
		}
		if (m_secretKeyLength < 0) m_secretKeyLength = 0;
		if (fwrite(&m_secretKeyLength, sizeof(int), 1, fp) < 1)
		{
			fclose(fp);
			return XFalse;
		}

		if (m_secretKey != NULL && m_secretKeyLength > 0)
		{
			if (fwrite(m_secretKey, m_secretKeyLength, 1, fp) < 1)
			{
				fclose(fp);
				return XFalse;
			}
		}
		m_checkData = getCheckData();    //����У������
		if (fwrite(&m_checkData, sizeof(unsigned char), 1, fp) < 1)
		{
			fclose(fp);
			return XFalse;
		}
		if (!DeCryptData())
		{//����һ���ǳ����صĴ������е������ڻ�ԭ�Ĺ�������������ص��ƻ����޷��ٻ�ԭ
		}

		fclose(fp);
		//	printf("%s:Write well!\n",fileName);
	}
	else
	{
#ifdef XEE_OS_WINDOWS
		if (m_fileName == NULL || strlen(m_fileName) != strlen(fileName))
		{
			XMem::XDELETE_ARRAY(m_fileName);
			m_fileName = XMem::createArrayMem<char>(strlen(fileName) + 1);
			if (m_fileName == NULL) return XFalse;
		}
		strcpy(m_fileName, fileName);
		HANDLE tempFile;
		DWORD tempWriteLength;
		/*		int len = strlen(m_fileName)/2 + 1;
				wchar_t *wText = XMem::createArrayMem<wchar_t>(len);
				if(wText == NULL) return XFalse;

				MultiByteToWideChar(CP_ACP,0,m_fileName,strlen(m_fileName),wText,strlen(m_fileName));
				tempFile = CreateFileW(wText,
										GENERIC_WRITE,
										FILE_SHARE_WRITE | FILE_SHARE_READ,
										NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_WRITE_THROUGH,NULL);*/
		tempFile = CreateFile(m_fileName,
			GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_WRITE_THROUGH, NULL);
		if (INVALID_HANDLE_VALUE == tempFile) return XFalse;
		tempWriteLength = 0;

		//������д�뵽�ļ�
		if (!EncryptData())
		{
			CloseHandle(tempFile);
			return XFalse;
		}
#if DEBUGXDB
		printf("write:%s - %d\n", m_fileName, m_versionsOrder);
#endif
		if (!WriteFile(tempFile, &m_versionsOrder, sizeof(int), &tempWriteLength, NULL))
		{
			CloseHandle(tempFile);
			return XFalse;
		}
		if (!WriteFile(tempFile, &m_ID, sizeof(int), &tempWriteLength, NULL))
		{
			CloseHandle(tempFile);
			return XFalse;
		}
		if (m_dataLength < 0) m_dataLength = 0;
		if (!WriteFile(tempFile, &m_dataLength, sizeof(int), &tempWriteLength, NULL))
		{
			CloseHandle(tempFile);
			return XFalse;
		}
		if (m_pData != NULL && m_dataLength > 0)
		{
			if (!WriteFile(tempFile, m_pData, m_dataLength, &tempWriteLength, NULL))
			{
				CloseHandle(tempFile);
				return XFalse;
			}
		}
		if (m_secretKeyLength < 0) m_secretKeyLength = 0;
		if (!WriteFile(tempFile, &m_secretKeyLength, sizeof(int), &tempWriteLength, NULL))
		{
			CloseHandle(tempFile);
			return XFalse;
		}

		if (m_secretKey != NULL && m_secretKeyLength > 0)
		{
			if (!WriteFile(tempFile, m_secretKey, m_secretKeyLength, &tempWriteLength, NULL))
			{
				CloseHandle(tempFile);
				return XFalse;
			}
		}
		m_checkData = getCheckData();    //����У������
		if (!WriteFile(tempFile, &m_checkData, sizeof(unsigned char), &tempWriteLength, NULL))
		{
			CloseHandle(tempFile);
			return XFalse;
		}

		if (!DeCryptData())
		{//����һ���ǳ����صĴ������е������ڻ�ԭ�Ĺ�������������ص��ƻ����޷��ٻ�ԭ
		}

		CloseHandle(tempFile);
		//XMem::XDELETE_ARRAY(wText);
#endif
#ifdef XEE_OS_LINUX
	//Linux�µ������������滹û�����
		if (m_fileName == NULL || strlen(m_fileName) != strlen(fileName))
		{
			XMem::XDELETE_ARRAY(m_fileName);
			m_fileName = XMem::createArrayMem<char>(strlen(fileName) + 1);
			if (m_fileName == NULL) return XFalse;
		}
		strcpy(m_fileName, fileName);
		FILE *fp;
		if ((fp = fopen(m_fileName, "wb")) == NULL)
		{
			return XFalse;
		}
		//������д�뵽�ļ�
		if (!EncryptData())
		{
			CloseHandle(tempFile);
			return XFalse;
		}
		fwrite(&m_versionsOrder, sizeof(int), 1, fp);
		fwrite(&m_ID, sizeof(int), 1, fp);
		if (m_dataLength < 0) m_dataLength = 0;
		fwrite(&m_dataLength, sizeof(int), 1, fp);
		if (m_pData != NULL && m_dataLength > 0)
		{
			fwrite(m_pData, m_dataLength, 1, fp);
		}
		if (m_secretKeyLength < 0) m_secretKeyLength = 0;
		fwrite(&m_secretKeyLength, sizeof(int), 1, fp);
		if (m_secretKey != NULL && m_secretKeyLength > 0)
		{
			fwrite(m_secretKey, m_secretKeyLength, 1, fp);
		}
		m_checkData = getCheckData();    //����У������
		fwrite(&m_checkData, sizeof(unsigned char), 1, fp);

		if (!DeCryptData())
		{//����һ���ǳ����صĴ������е������ڻ�ԭ�Ĺ�������������ص��ƻ����޷��ٻ�ԭ
		}

		fclose(fp);
#endif
		//	printf("%s:Write OK!\n",m_fileName);
		//	system("pause");
	}
	return XTrue;
}
XBool XDataBasic::analyzingDataManager(const char *fileName)        //�����ݱ��浽�ļ���������������֮ǰ�û������öԱ���ľ���������е���
{
	if (!m_isInited) return XFalse;
	if (m_isSaveAsynchronous)
	{
		if (!m_needSave)
		{
			if (fileName != NULL)
			{
				if (m_fileName == NULL || strlen(fileName) != strlen(m_fileName))
				{
					XMem::XDELETE_ARRAY(m_fileName);
					int len = strlen(fileName);
					m_fileName = XMem::createArrayMem<char>(len + 1);
					if (m_fileName == NULL) return XFalse;
				}
				strcpy(m_fileName, fileName);
			}
			else
			{
				XMem::XDELETE_ARRAY(m_fileName);
			}
			m_needSave = XTrue;
			return XTrue;
		}
		else
		{//�ϴα�ǵ�������δ����
			return XFalse;
		}
	}
	else
	{
		return analyzingData(fileName);
	}
}
XBool XDataBasic::analyzingData(const char *fileName)        //�����ݱ��浽�ļ�
{
	if (!m_isInited) return XFalse;
	char *fileRealName = NULL;
	XBool result = XTrue;
	++m_versionsOrder;
	if (fileName != NULL)
	{
		int len = strlen(fileName);
		if (len <= 0) return XFalse;
		fileRealName = XMem::createArrayMem<char>(len + 2);
		if (fileRealName == NULL) return XFalse;
		//ȷ���ļ���
		int dotPoint = -1;
		for (int i = 0; i < len; ++i)
		{
			if (fileName[i] == '.') dotPoint = i;
		}
		if (dotPoint == -1)
		{
			memcpy(fileRealName, fileName, len);
			if (m_curBackOrder == 0) fileRealName[len] = 'A';
			else fileRealName[len] = 'B';
			fileRealName[len + 1] = '\0';
		}
		else
		{
			memcpy(fileRealName, fileName, dotPoint);
			if (m_curBackOrder == 0) fileRealName[dotPoint] = 'A';
			else fileRealName[dotPoint] = 'B';
			memcpy(fileRealName + dotPoint + 1, fileName + dotPoint, len - dotPoint);
			fileRealName[len + 1] = '\0';
		}
		result = analyzingDataBack(fileRealName);
		XMem::XDELETE_ARRAY(fileRealName);
	}
	else
	{//ʹ��Ĭ�ϵ��ļ�����ȡ����
		if (m_curBackOrder == 0)
			result = analyzingDataBack(DATABASIC_DEFAULT_FILE_NAME_A);
		else
			result = analyzingDataBack(DATABASIC_DEFAULT_FILE_NAME_B);
	}
	m_curBackOrder = 1 - m_curBackOrder;
	return result;
}
//���������ݼ��ת��(Ŀǰ��ʱ�����ṩ���֧��)

//�����첽���ݱ���ĺ���
DWORD WINAPI XDataBasic::saveDataThread(void * pParam)
{
	XDataBasic &pPar = *(XDataBasic *)pParam;
	pPar.m_isEnd = STATE_START;

	while (pPar.m_isEnd != STATE_SET_TO_END)
	{
		if (pPar.m_needSave)
		{//����������ݱ���
			pPar.analyzingData(pPar.m_fileName);
		}
		//if(pPar.m_isEnd == STATE_SET_TO_END) break;
		XEE::sleep(1);
	}
	pPar.m_isEnd = STATE_END;
	return 1;
}
}