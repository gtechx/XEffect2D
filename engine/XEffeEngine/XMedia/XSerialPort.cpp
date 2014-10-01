#include "XSerialPort.h"
#include <tchar.h>
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
// ��ʹ�ö���ʱ
#define TIMEOUT_READ_INTERVAL			0xFFFFFFFF
#define TIMEOUT_READ_TOTAL_MULTIPLIER	0
#define TIMEOUT_READ_TOTAL_CONSTANT		1000
// д��ʱΪ��
#define TIMEOUT_WRITE_TOTAL_MULTIPLIER	0
#define TIMEOUT_WRITE_TOTAL_CONSTANT	5000
// �Ƽ�������/�����������ע�⣺ʵ��ֵ��ϵͳ���ã�
#define BUFFER_INPUT_RECOMMEND	10000
#define BUFFER_OUTPUT_RECOMMEND	10000
// �첽��ȡ/д�����ʱ�ȴ��¼��ĳ�ʱʱ��
#define TIMEOUT_READCOMM_EVENT	4000
#define TIMEOUT_WRITECOMM_EVENT	2000
// һЩͨѶЭ��ʹ�õĺ�
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// �򿪴���
_XBool _XSerialPort::open(int nPort,int nBaud,int nParity,_XSerialPortMode mode)
{
    if(m_bOpened) return XTrue;	//��ֹ�ظ���

    TCHAR szPort[50];
    TCHAR szComParams[50];
    DCB dcb;
	m_mode = mode;

	//if(nPort < 10) wsprintf(szPort,_T("COM%d"),nPort);
	//else wsprintf(szPort,_T("\\\\.\\COM%d"),nPort);
	wsprintf(szPort,_T("\\\\.\\COM%d"),nPort);
	// API�������ļ���Windows�н������豸�����ļ��Դ�
	m_hIDComDev = CreateFile(
					  szPort,
					  GENERIC_READ | GENERIC_WRITE,
					  0,
					  NULL,
					  OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// �첽��д
					  NULL);

	if(m_hIDComDev == NULL) return XFalse;

	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));

	//���ó�ʱ
	COMMTIMEOUTS CommTimeOuts;
	//if(m_mode == SP_MODE_AUTO)
	//{
	//	CommTimeOuts.ReadIntervalTimeout		= 0x100;
	//	CommTimeOuts.ReadTotalTimeoutMultiplier = 2;
	//	CommTimeOuts.ReadTotalTimeoutConstant	= 5000;
	//	CommTimeOuts.WriteTotalTimeoutMultiplier= 10;
	//	CommTimeOuts.WriteTotalTimeoutConstant	= 1000;
	//}else
	//{
		CommTimeOuts.ReadIntervalTimeout		= TIMEOUT_READ_INTERVAL;
		CommTimeOuts.ReadTotalTimeoutMultiplier = TIMEOUT_READ_TOTAL_MULTIPLIER;
		CommTimeOuts.ReadTotalTimeoutConstant	= TIMEOUT_READ_TOTAL_CONSTANT;
		CommTimeOuts.WriteTotalTimeoutMultiplier= TIMEOUT_WRITE_TOTAL_MULTIPLIER;
		CommTimeOuts.WriteTotalTimeoutConstant	= TIMEOUT_WRITE_TOTAL_CONSTANT;
	//}

	SetCommTimeouts(m_hIDComDev,&CommTimeOuts);

	wsprintf(szComParams,_T("COM%d:%d,n,8,1" ),nPort,nBaud);
	//�����첽��ȡ/д������¼�
	m_overlappedRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_overlappedWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	//��ȡ/���ô����豸����
	dcb.DCBlength = sizeof ( DCB );
	GetCommState ( m_hIDComDev, &dcb );
	dcb.fBinary = true; 
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;			// 8 Bits ����
	dcb.StopBits = ONESTOPBIT;	// һλֹͣλ
	dcb.Parity = nParity;				//ȱʡ��У��
//	unsigned char ucSet;
//	ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_XONXOFF) != 0);

	if(!SetCommState(m_hIDComDev,&dcb) ||
		!SetupComm(m_hIDComDev,BUFFER_INPUT_RECOMMEND,BUFFER_OUTPUT_RECOMMEND) ||
		m_overlappedRead.hEvent == NULL ||
		m_overlappedWrite.hEvent == NULL) 
	{
		//DWORD dwError = GetLastError();
		if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
		if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);

		CloseHandle(m_hIDComDev);
		return XFalse;
	}
	// ��ջ�����
	PurgeComm(m_hIDComDev, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	if(m_mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		m_threadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
		{//�߳̽���ʧ��
			printf("Server command thread create error!\n");
			return XFalse;
		}
	}

    m_bOpened = XTrue;
    return m_bOpened;
}
////����1
//DWORD WINAPI _XSerialPort::recvThread(void * pParam)
//{
//	_XSerialPort &pPar = *(_XSerialPort *)pParam;
//	DWORD dwEvtMask = 0,dwError,nBytesRead;
//	pPar.m_threadState = STATE_START;
//	int buffsize = 1024;
//	char *buff = createArrayMem<char>(buffsize);
//	COMSTAT cs;
//	if(!SetCommMask(pPar.m_hIDComDev,EV_RXCHAR)) printf("Error!\n");
//	while(true)
//	{
//		if(pPar.m_threadState == STATE_SET_TO_END) break;
//		if(WaitCommEvent(pPar.m_hIDComDev, &dwEvtMask,NULL) && (dwEvtMask & EV_RXCHAR))
//		{
//			if(pPar.m_waitingDataTime > 0) Sleep(pPar.m_waitingDataTime);	//�ȴ�һ��ʱ�䣬�ȴ����ݽ������
//			ClearCommError(pPar.m_hIDComDev,&dwError,&cs);
//			if(cs.cbInQue)
//			{
//				if(cs.cbInQue > buffsize)
//				{//���ݴ�С���������С���������Ļ���
//					XDELETE_ARRAY(buff);
//					buffsize = (cs.cbInQue << 1);
//					buff = createArrayMem<char>(buffsize);
//				}
//				if(ReadFile(pPar.m_hIDComDev,buff,buffsize,&nBytesRead,&pPar.m_overlappedRead))
//				{
//					pPar.m_recordMutex.Lock();
//					if(pPar.m_withRecord)
//						pPar.recordRecvAData((unsigned char *)buff,nBytesRead);	//�����Ҫ¼�ƵĻ�������д���ļ�
//					pPar.m_recordMutex.Unlock();
//
//					if(pPar.m_funRecv != NULL) 
//						pPar.m_funRecv(pPar.m_pClass,(unsigned char *)buff,nBytesRead);
//					//printf("���յ�����!\n");
//#ifdef SP_WITH_STATISTICS
//					if(pPar.m_withStatistics)
//					{
//						++ pPar.m_recvTimesAll;
//						pPar.m_recvBytesAll += nBytesRead;
//						++ pPar.m_recvTimesNow;
//						pPar.m_recvBytesNow += nBytesRead;
//					}
//#endif
//				}
//			//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
//			}
//		}
//	//	Sleep(1);
//	}
//	XDELETE_ARRAY(buff);
//	pPar.m_threadState = STATE_END;
//	return 0;
//}
//����2
DWORD WINAPI _XSerialPort::recvThread(void * pParam)
{
	_XSerialPort &pPar = *(_XSerialPort *)pParam;
	DWORD dwEvtMask = 0,dwError,nBytesRead;
	pPar.m_threadState = STATE_START;
	int buffsize = 1024;
	char *buff = createArrayMem<char>(buffsize);
	COMSTAT cs;
	int offset;
	if(!SetCommMask(pPar.m_hIDComDev,EV_RXCHAR)) printf("Error!\n");
	while(true)
	{
		if(pPar.m_threadState == STATE_SET_TO_END) break;
		if(!WaitCommEvent(pPar.m_hIDComDev, &dwEvtMask,NULL) || !(dwEvtMask & EV_RXCHAR))
		{//û������
			Sleep(1);
			continue;	//�ȴ������ź�
		}
		//���濪ʼ��������ֱ��û�����ݽ���Ϊ֮
		offset = 0;
		while(true)
		{				
			ClearCommError(pPar.m_hIDComDev,&dwError,&cs);
			if(cs.cbInQue <= 0) break;	//û��������Ҫ����
			if(offset == buffsize || buffsize - offset < cs.cbInQue)
			{//�ڴ治��
				char * tmp = buff;
				buffsize = buffsize << 1;	//�ڴ�ռ�����һ��
				if(buffsize - offset < cs.cbInQue) buffsize = offset + cs.cbInQue;	//�������һ���ڴ�ռ���Ȼ�����ã������󵽹���Ϊֹ
				buff = createArrayMem<char>(buffsize);
				memcpy(buff,tmp,offset);
				XDELETE_ARRAY(tmp);
			}
			if(ReadFile(pPar.m_hIDComDev,buff + offset,buffsize - offset,&nBytesRead,&pPar.m_overlappedRead))
			{
				offset += nBytesRead;
				if(pPar.m_waitingDataTime > 0) Sleep(pPar.m_waitingDataTime);	//�ȴ�һ��ʱ�䣬�ȴ����ݽ������
				else break;
			}else break;
		}
		if(offset > 0 && !pPar.m_withPlay)	//���ŵ�ʱ�������ֱ�Ӷ���
		{//���յ����ݣ�������д���
			pPar.m_recordMutex.Lock();
			if(pPar.m_withRecord)
				pPar.recordRecvAData((unsigned char *)buff,offset);	//�����Ҫ¼�ƵĻ�������д���ļ�
			pPar.m_recordMutex.Unlock();

			if(pPar.m_funRecv != NULL) 
				pPar.m_funRecv(pPar.m_pClass,(unsigned char *)buff,offset);
			//printf("���յ�����!\n");
#ifdef SP_WITH_STATISTICS
			if(pPar.m_withStatistics)
			{
				++ pPar.m_recvTimesAll;
				pPar.m_recvBytesAll += offset;
				++ pPar.m_recvTimesNow;
				pPar.m_recvBytesNow += offset;
			}
#endif
		}
	//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
	}
	XDELETE_ARRAY(buff);
	pPar.m_threadState = STATE_END;
	return 0;
}
// �رմ���
_XBool _XSerialPort::close()
{
	if(!m_bOpened || m_hIDComDev == NULL) return XTrue;
	if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
	if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);
	if(m_mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		m_threadState = STATE_SET_TO_END;
		SetCommMask(m_hIDComDev,0);
		while(true)
		{//�ȴ��߳̽���
			if(m_threadState == STATE_END) break;
			Sleep(1);
		}
	}
	CloseHandle(m_hIDComDev);
	m_bOpened = XFalse;
	m_hIDComDev = NULL;

	return XTrue;
}
// �򴮿�д�����ݣ����ڲ�ʹ��
//_XBool _XSerialPort::writeCommByte(unsigned char ucByte)
//{
//	_XBool bWriteStat;
//	DWORD dwBytesWritten;
//
//	bWriteStat = WriteFile(m_hIDComDev,(LPSTR)&ucByte,1,&dwBytesWritten,&m_overlappedWrite);
//
//	// ��ѯ�첽д���Ƿ���ɣ�δ��������ȴ�
//	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
//	{
//		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
//		{
//			dwBytesWritten = 0;
//		}else 
//		{
//			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
//			m_overlappedWrite.Offset += dwBytesWritten;
//		}
//	}
//
//	return XTrue;
//}
// �򴮿ڷ�������
int _XSerialPort::sendData(const unsigned char *buffer,int size)
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	//DWORD dwBytesWritten = 0;
	//for(int i = 0;i < size;++ i) 
	//{
	//	writeCommByte(buffer[i]);
	//	dwBytesWritten++;
	//}

	//return (int)dwBytesWritten;
	_XBool bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile(m_hIDComDev,buffer,size,&dwBytesWritten,&m_overlappedWrite);
	// ��ѯ�첽д���Ƿ���ɣ�δ��������ȴ�
	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
	{
		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
		{
			dwBytesWritten = 0;
		}else 
		{
			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
			m_overlappedWrite.Offset += dwBytesWritten;
		}
	}
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		++ m_sendTimesAll;
		m_sendBytesAll += dwBytesWritten;
		++ m_sendTimesNow;
		m_sendBytesNow += dwBytesWritten;
	}
#endif
	return dwBytesWritten;
}
// ��ѯ���ܻ��������Ƿ�������(ֻ��ѯ,����ȡ)
int _XSerialPort::readDataWaiting()
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);

	return (int)ComStat.cbInQue;
}
//��ȡ���Դ��ڵ�����
int _XSerialPort::readData(void *buffer,int limit)
{
    if(!m_bOpened || m_hIDComDev == NULL) return 0;

    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;
	// ��ȡ֮ǰ�����������Ϣ
    ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);
    if(!ComStat.cbInQue) return 0;

    dwBytesRead = (DWORD)ComStat.cbInQue;

    if(limit < (int)dwBytesRead) dwBytesRead = (DWORD)limit;

    bReadStatus = ReadFile(m_hIDComDev,buffer,dwBytesRead,&dwBytesRead,&m_overlappedRead);
	// ��ѯ�첽��ȡ�Ƿ���ɣ�δ��������ȴ�
    if(!bReadStatus)
	{
        if(GetLastError() == ERROR_IO_PENDING)
		{
            WaitForSingleObject(m_overlappedRead.hEvent,TIMEOUT_READCOMM_EVENT);
            return (int)dwBytesRead;
        }
        return 0;
    }
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		++ m_recvTimesAll;
		m_recvBytesAll += dwBytesRead;
		++ m_recvTimesNow;
		m_recvBytesNow += dwBytesRead;
	}
#endif

    return (int)dwBytesRead;
}
#include "XBasicWindow.h"
void _XSerialPort::update()
{
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		m_statisticsTimer += XEE::frameTime;
		if(m_statisticsTimer >= 5000)
		{
			float tmp = m_statisticsTimer * 0.001f;
			m_sendTimesAvg = m_sendTimesNow / tmp;	//ƽ������
			m_sendBytesAvg = m_sendBytesNow / tmp;
			m_recvTimesAvg = m_recvTimesNow / tmp;
			m_recvBytesAvg = m_recvBytesNow / tmp;
			//����ͳ��
			m_sendTimesNow = 0;
			m_sendBytesNow = 0;
			m_recvTimesNow = 0;
			m_recvBytesNow = 0;
			m_statisticsTimer = 0;
		}
	}
#endif
	if(m_withPlay)
	{//�����ǻطŽ׶�
		m_playTimer += XEE::frameTime;
		//����������
		int tmpTime = getElapsedTime(m_upTime,m_nextTime);
		if(m_playTimer >= tmpTime)
		{//ʱ���Ѿ��������Ҫ������һ������
			m_playTimer -= tmpTime;
			if(m_oparetion == "stop")
			{//�������
				LogStr("play over!");
				stopPlay();
				return;
			}
			if(m_oparetion == "recv")
			{//���յ�����
				if(m_funRecv != NULL) 
					m_funRecv(m_pClass,m_nextData,m_nextDataLen);
			}
			//�����ȡ��һ������
			m_upTime = m_nextTime;
			if(!readADataFromRecord(m_oparetion,m_nextTime,m_nextData,m_nextDataLen))
			{//�������ʧ����ֱ�ӹر�
				LogStr("record file error!");
				stopPlay();
				return;
			}
		}
	}
}
void _XSerialPort::setStatistics(bool flag)
{
	if(flag == m_withStatistics) return;
	if(flag)
	{//����ͳ��
		m_withStatistics = true;
		//���ݹ���
		m_sendTimesAll = 0;		//�ܵķ��ʹ���
		m_sendBytesAll = 0;		//�ܵķ����ֽ���
		m_recvTimesAll = 0;		//�ܵĽ��մ���
		m_recvBytesAll = 0;		//�ܵĽ����ֽ���

		m_statisticsTimer = 0;	//ͳ�Ƽ�ʱ
		m_sendTimesNow = 0;
		m_sendBytesNow = 0;
		m_recvTimesNow = 0;
		m_recvBytesNow = 0;

		m_sendTimesAvg = 0.0f;	//ƽ������
		m_sendBytesAvg = 0.0f;
		m_recvTimesAvg = 0.0f;
		m_recvBytesAvg = 0.0f;
	}else
	{//�ر�ͳ��
		m_withStatistics = false;
	}
}