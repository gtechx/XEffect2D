#include "XStdHead.h"
#include "XSerialPort.h"
#include <tchar.h>
#include "../XLogBook.h"
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------

namespace XE{
// ��ʹ�ö���ʱ
#define TIMEOUT_READ_INTERVAL			(0xFFFFFFFF)
#define TIMEOUT_READ_TOTAL_MULTIPLIER	(0)
#define TIMEOUT_READ_TOTAL_CONSTANT		(1000)
// д��ʱΪ��
#define TIMEOUT_WRITE_TOTAL_MULTIPLIER	(0)
#define TIMEOUT_WRITE_TOTAL_CONSTANT	(5000)
// �Ƽ�������/�����������ע�⣺ʵ��ֵ��ϵͳ���ã�
#define BUFFER_INPUT_RECOMMEND	(10000)
#define BUFFER_OUTPUT_RECOMMEND	(10000)
// �첽��ȡ/д�����ʱ�ȴ��¼��ĳ�ʱʱ��
#define TIMEOUT_READCOMM_EVENT	(4000)
#define TIMEOUT_WRITECOMM_EVENT	(2000)
// һЩͨѶЭ��ʹ�õĺ�
#define FC_DTRDSR       (0x01)
#define FC_RTSCTS       (0x02)
#define FCXONXOFF      (0x04)
#define ASCII_BEL       (0x07)
#define ASCII_BS        (0x08)
#define ASCII_LF        (0x0A)
#define ASCII_CR        (0x0D)
#define ASCIIXON       (0x11)
#define ASCIIXOFF      (0x13)

// �򿪴���
XBool XSerialPort::open(const XSerialPortInfo &info)
{
    if(m_bOpened) return XTrue;	//��ֹ�ظ���
	if(!info.checkData()) return XFalse;	//�Ƿ��Ĳ���
	m_serialPortInfo = info;

    TCHAR szPort[50];
    TCHAR szComParams[50];
    DCB dcb;

	//if(nPort < 10) wsprintf(szPort,_T("COM%d"),nPort);
	//else wsprintf(szPort,_T("\\\\.\\COM%d"),nPort);
	wsprintf(szPort,_T("\\\\.\\COM%d"),m_serialPortInfo.nPort);
	// API�������ļ���Windows�н������豸�����ļ��Դ�
	int flag = 0;	//ͬ��
	if(m_serialPortInfo.withOverLaped) flag = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;	//�첽
	m_hIDComDev = CreateFile(
					  szPort,
					  GENERIC_READ | GENERIC_WRITE,
					  0,
					  NULL,
					  OPEN_EXISTING,
					  flag,
					  NULL);

	if(m_hIDComDev == NULL)
	{
		LogNull("���ڴ�ʧ��:%d",m_serialPortInfo.nPort);
		return XFalse;
	}

	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));

	//���ó�ʱ
	COMMTIMEOUTS CommTimeOuts;
	//if(m_serialPortInfo.mode == SP_MODE_AUTO)
	//{
	//	CommTimeOuts.ReadIntervalTimeout		= 0x100;
	//	CommTimeOuts.ReadTotalTimeoutMultiplier = 2;
	//	CommTimeOuts.ReadTotalTimeoutConstant	= 5000;
	//	CommTimeOuts.WriteTotalTimeoutMultiplier= 10;
	//	CommTimeOuts.WriteTotalTimeoutConstant	= 1000;
	//}else
	{
		CommTimeOuts.ReadIntervalTimeout		= TIMEOUT_READ_INTERVAL;			//�������ʱ
		CommTimeOuts.ReadTotalTimeoutMultiplier = TIMEOUT_READ_TOTAL_MULTIPLIER;	//��ʱ��ϵ��
		CommTimeOuts.ReadTotalTimeoutConstant	= TIMEOUT_READ_TOTAL_CONSTANT;		//��ʱ�䳣��
		CommTimeOuts.WriteTotalTimeoutMultiplier= TIMEOUT_WRITE_TOTAL_MULTIPLIER;	//дʱ��ϵ��
		CommTimeOuts.WriteTotalTimeoutConstant	= TIMEOUT_WRITE_TOTAL_CONSTANT;		//дʱ�䳣��
	}

	SetCommTimeouts(m_hIDComDev,&CommTimeOuts);

	wsprintf(szComParams,_T("COM%d:%d,n,8,1" ),info.nPort,info.nBaud);
	//�����첽��ȡ/д������¼�
	m_overlappedRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_overlappedWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	//��ȡ/���ô����豸����
	dcb.DCBlength = sizeof(DCB);
	if(!GetCommState(m_hIDComDev,&dcb))
	{
		LogStr("��ȡ����״̬ʧ��");
		return XFalse;
	}
	dcb.fBinary = true; 
	dcb.BaudRate = m_serialPortInfo.nBaud;
	dcb.ByteSize = m_serialPortInfo.byteSize;			// 8 Bits ����
	dcb.StopBits = m_serialPortInfo.stopBits;	//ֹͣλ����
	dcb.Parity = m_serialPortInfo.nParity;		//У�鷽ʽ
	dcb.fDtrControl = m_serialPortInfo.dtr;
	dcb.fRtsControl = m_serialPortInfo.rts;
//	unsigned char ucSet;
//	ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FCXONXOFF) != 0);

	if(!SetCommState(m_hIDComDev,&dcb) ||
		!SetupComm(m_hIDComDev,BUFFER_INPUT_RECOMMEND,BUFFER_OUTPUT_RECOMMEND) ||
		m_overlappedRead.hEvent == NULL ||
		m_overlappedWrite.hEvent == NULL) 
	{
		//DWORD dwError = GetLastError();
		if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
		if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);

		CloseHandle(m_hIDComDev);
		LogStr("����״̬����ʧ��");
		return XFalse;
	}
	// ��ջ�����
	if(!PurgeComm(m_hIDComDev, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT)) return XFalse;
	if(m_serialPortInfo.mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		m_threadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
		{//�߳̽���ʧ��
			LogStr("���߳̽���ʧ��");
			return XFalse;
		}
	}

    m_bOpened = XTrue;
    return m_bOpened;
}
////����1
//DWORD WINAPI XSerialPort::recvThread(void * pParam)
//{
//	XSerialPort &pPar = *(XSerialPort *)pParam;
//	DWORD dwEvtMask = 0,dwError,nBytesRead;
//	pPar.m_threadState = STATE_START;
//	int buffsize = 1024;
//	char *buff = XMem::createArrayMem<char>(buffsize);
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
//					XMem::XDELETE_ARRAY(buff);
//					buffsize = (cs.cbInQue << 1);
//					buff = XMem::createArrayMem<char>(buffsize);
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
//						++ pPar.m_recvTimesCur;
//						pPar.m_recvBytesCur += nBytesRead;
//					}
//#endif
//				}
//			//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
//			}
//		}
//	//	Sleep(1);
//	}
//	XMem::XDELETE_ARRAY(buff);
//	pPar.m_threadState = STATE_END;
//	return 0;
//}
//����2
DWORD WINAPI XSerialPort::recvThread(void * pParam)
{
	XSerialPort &pPar = *(XSerialPort *)pParam;
	DWORD dwEvtMask = 0,dwError,nBytesRead;
	pPar.m_threadState = STATE_START;
	int buffsize = 1024;
	char *buff = XMem::createArrayMem<char>(buffsize);
	COMSTAT cs;
	int offset;
	if(pPar.m_serialPortInfo.withEventMode && !SetCommMask(pPar.m_hIDComDev,EV_RXCHAR))// | EV_RXFLAG));// | EV_CTS | EV_DSR)) 
		LogStr("SerialPort SetCommMask Error!");
	while(pPar.m_threadState != STATE_SET_TO_END)
	{
		//if(pPar.m_threadState == STATE_SET_TO_END) break;
		if((pPar.m_serialPortInfo.withEventMode && (!WaitCommEvent(pPar.m_hIDComDev, &dwEvtMask,NULL) || !(dwEvtMask & EV_RXCHAR)))
			|| (!pPar.m_serialPortInfo.withEventMode && pPar.readDataWaiting() <= 0))
		//if( pPar.readDataWaiting() <= 0)
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
			if(offset == buffsize || buffsize - offset < 0 || buffsize - offset < cs.cbInQue)
			{//�ڴ治��
				char * tmp = buff;
				buffsize = buffsize << 1;	//�ڴ�ռ�����һ��
				if(buffsize - offset < 0 || buffsize - offset < cs.cbInQue) buffsize = offset + cs.cbInQue;	//�������һ���ڴ�ռ���Ȼ�����ã������󵽹���Ϊֹ
				buff = XMem::createArrayMem<char>(buffsize);
				memcpy(buff,tmp,offset);
				XMem::XDELETE_ARRAY(tmp);
			}
			//if(ReadFile(pPar.m_hIDComDev,buff + offset,buffsize - offset,&nBytesRead,&pPar.m_overlappedRead))
			if(ReadFile(pPar.m_hIDComDev,buff + offset,cs.cbInQue,&nBytesRead,&pPar.m_overlappedRead))
			{
				offset += nBytesRead;
				if(pPar.m_serialPortInfo.waitingDataTime > 0) Sleep(pPar.m_serialPortInfo.waitingDataTime);	//�ȴ�һ��ʱ�䣬�ȴ����ݽ������
				else break;
			}else
			{
				if(pPar.m_serialPortInfo.withOverLaped)
				{//�첽
					if(GetLastError() == ERROR_IO_PENDING) //�ص��������ڽ���
					{
						if(GetOverlappedResult(pPar.m_hIDComDev,&pPar.m_overlappedRead,&nBytesRead,TRUE))
						{//����û�в��Թ�
							offset += nBytesRead;
							if(pPar.m_serialPortInfo.waitingDataTime > 0) Sleep(pPar.m_serialPortInfo.waitingDataTime);	//�ȴ�һ��ʱ�䣬�ȴ����ݽ������
							else break;
						}
					}
				}else
				{//ͬ��
					break;
				}
			}
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
				++ pPar.m_recvTimesCur;
				pPar.m_recvBytesCur += offset;
			}
#endif
		}
	//	PurgeComm(pPar.m_hIDComDev,PURGE_RXCLEAR);
		Sleep(1);
	}
	XMem::XDELETE_ARRAY(buff);
	pPar.m_threadState = STATE_END;
	return 0;
}
// �رմ���
XBool XSerialPort::close()
{
	if(!m_bOpened || m_hIDComDev == NULL) return XTrue;
	if(m_serialPortInfo.mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		SetCommMask(m_hIDComDev,0);
		waitThreadEnd(m_threadState);
	}
	if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
	if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);
	CloseHandle(m_hIDComDev);
	m_bOpened = XFalse;
	m_hIDComDev = NULL;

	return XTrue;
}
// �򴮿�д�����ݣ����ڲ�ʹ��
//XBool XSerialPort::writeCommByte(unsigned char ucByte)
//{
//	XBool bWriteStat;
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
int XSerialPort::sendData(const unsigned char *buffer,int size)
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;
	if(buffer == NULL || size <= 0)
	{
		LogStr("���ڷ��ʹ��������!");
		return 0;
	}

	//DWORD dwBytesWritten = 0;
	//for(int i = 0;i < size;++ i) 
	//{
	//	writeCommByte(buffer[i]);
	//	++ dwBytesWritten;
	//}

	//return (int)dwBytesWritten;
	XBool bWriteStat;
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
		++ m_sendTimesCur;
		m_sendBytesCur += dwBytesWritten;
	}
#endif
	return dwBytesWritten;
}
//��ȡ���Դ��ڵ�����
int XSerialPort::readData(void *buffer,int limit)
{
    if(!m_bOpened || m_hIDComDev == NULL) return 0;
	if(buffer == NULL || limit <= 0)
	{
		LogStr("���ջ������ݷǷ�!");
		return 0;
	}

    DWORD dwErrorFlags;
    COMSTAT ComStat;
	// ��ȡ֮ǰ�����������Ϣ
    ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);
    if(!ComStat.cbInQue) return 0;

    DWORD dwBytesRead = (DWORD)ComStat.cbInQue;

    if(limit < (int)dwBytesRead) dwBytesRead = (DWORD)limit;
	// ��ѯ�첽��ȡ�Ƿ���ɣ�δ��������ȴ�
    if(!ReadFile(m_hIDComDev,buffer,dwBytesRead,&dwBytesRead,&m_overlappedRead))
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
		++ m_recvTimesCur;
		m_recvBytesCur += dwBytesRead;
	}
#endif

    return (int)dwBytesRead;
}
void XSerialPort::update()
{
#ifdef SP_WITH_STATISTICS
	if(m_withStatistics)
	{
		if (gFrameworkData.pFrameWork == NULL) m_statisticsTimer += 1;
		else m_statisticsTimer += gFrameworkData.pFrameWork->getLastFrameTime();
		if(m_statisticsTimer >= 5000)
		{
			float tmp = m_statisticsTimer * 0.001f;
			m_sendTimesAvg = m_sendTimesCur / tmp;	//ƽ������
			m_sendBytesAvg = m_sendBytesCur / tmp;
			m_recvTimesAvg = m_recvTimesCur / tmp;
			m_recvBytesAvg = m_recvBytesCur / tmp;
			//����ͳ��
			m_sendTimesCur = 0;
			m_sendBytesCur = 0;
			m_recvTimesCur = 0;
			m_recvBytesCur = 0;
			m_statisticsTimer = 0;
		}
	}
#endif
	if(m_withPlay)
	{//�����ǻطŽ׶�
		if (gFrameworkData.pFrameWork == NULL) m_playTimer += 1;
		else m_playTimer += gFrameworkData.pFrameWork->getLastFrameTime();
		//����������
		int tmpTime = XTime::getElapsedTime(m_upTime,m_nextTime);
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
void XSerialPort::setStatistics(bool flag)
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
		m_sendTimesCur = 0;
		m_sendBytesCur = 0;
		m_recvTimesCur = 0;
		m_recvBytesCur = 0;

		m_sendTimesAvg = 0.0f;	//ƽ������
		m_sendBytesAvg = 0.0f;
		m_recvTimesAvg = 0.0f;
		m_recvBytesAvg = 0.0f;
	}else
	{//�ر�ͳ��
		m_withStatistics = false;
	}
}
bool XSerialPortInfo::checkData() const		//������ݵĺϷ���
{
	if(nPort < 0) return false;
	if(nBaud != 110 && nBaud != 300 && nBaud != 600 &&
		nBaud != 1200 && nBaud != 2400 && nBaud != 4800 &&
		nBaud != 9600 && nBaud != 14400 && nBaud != 19200 &&
		nBaud != 38400 && nBaud != 56000 && nBaud != 57600 &&
		nBaud != 115200 && nBaud != 128000 && nBaud != 256000) return false;
	if(nParity < 0 || nParity > 4) return false;
	if(dtr < 0 || dtr > 2) return false;
	if(rts < 0 || rts > 3) return false;
	if(byteSize < 4 || byteSize > 8) return false;
	if(!withOverLaped && withEventMode) return false;
	return true;
}
XBool XSerialPort::readADataFromRecord(std::string &oparetion,XSystemTime &t,unsigned char *date,int &len)	//�Ӽ�¼�ļ��ж�ȡһ������
{
	if(m_playFile == NULL) return XFalse;
	len = 0;
	char lineData[2048];
	if(fgets(lineData,2048,m_playFile) == NULL) return XFalse;
	int ret = XString::getCharPosition(lineData,':');
	if(ret <= 0) return XFalse;	//����
	lineData[ret] = '\0';
	oparetion = lineData;
	int timeEnd = XString::getCharPosition(lineData + ret + 1,':');
	if(timeEnd <= 0) return XFalse;	//����
	lineData[ret + 1 + timeEnd] = '\0';
	t = XTime::string2sysTime(lineData + ret + 1);
	if(oparetion == "recv")
	{//�����ȡʱ�������
		int offset = ret + 1 + timeEnd;
		int tmpI = 0;
		for(len = 0;;++ len)
		{
			if(sscanf(lineData + offset + 1,"0x%02x ",&tmpI) != 1) break;
			date[len] = tmpI;
			ret = XString::getCharPosition(lineData + offset + 1,' ');
			if(ret <= 0) break;
			offset += ret + 1;
		}
	}
	return true;
}
XBool XSerialPort::startPlay()		//��ʼ����
{
	if(m_withPlay || m_withRecord) return XFalse;
	m_playTimer = 0;
	//��¼���ļ�(��δ���)
	if((m_playFile = fopen(SP_RECORDE_FILENAME,"r")) == NULL) return XFalse;	//��¼���ļ�
	m_withPlay = XTrue;
	if(!readADataFromRecord(m_oparetion,m_upTime,m_nextData,m_nextDataLen))
	{//�������ʧ����ֱ�ӹر�
		LogStr("record file error!");
		stopPlay();
	}
	if(m_oparetion != "start")
	{
		LogStr("record file error!");
		stopPlay();
	}
	//��ȡ��һ������
	if(!readADataFromRecord(m_oparetion,m_nextTime,m_nextData,m_nextDataLen))
	{//�������ʧ����ֱ�ӹر�
		LogStr("record file error!");
		stopPlay();
	}
	return XTrue;
}
bool XSerialPortInfo::readFromFile(TiXmlNode * node)
{
	if(node == NULL) return false;
	bool ret = true;
	int tmpI;
	bool tmpB;
	if(XXml::getXmlAsInt(node,"serialPort_mode",tmpI))
		mode = (XSerialPortMode)tmpI;
	else ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_port",nPort)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_baud",nBaud)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_parity",nParity)) ret = false;
	if(XXml::getXmlAsBool(node,"serialPort_withEvent",tmpB))
		withEventMode = tmpB;
	else ret = false;
	if(XXml::getXmlAsBool(node,"serialPort_withOLP",tmpB))
		withOverLaped = tmpB;
	else ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_stopBits",stopBits)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_dtr",dtr)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_rts",rts)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_WDT",waitingDataTime)) ret = false;
	if(!XXml::getXmlAsInt(node,"serialPort_byteSize",byteSize)) ret = false;
	return ret;
}
bool XSerialPortInfo::writeToFile(TiXmlElement &element)
{
	if(!XXml::addLeafNode(element,"serialPort_mode",XString::toString(mode))) return false;
	if(!XXml::addLeafNode(element,"serialPort_port",XString::toString(nPort))) return false;
	if(!XXml::addLeafNode(element,"serialPort_baud",XString::toString(nBaud))) return false;
	if(!XXml::addLeafNode(element,"serialPort_parity",XString::toString(nParity))) return false;
	if(!XXml::addLeafNode(element,"serialPort_withEvent",withEventMode)) return false;
	if(!XXml::addLeafNode(element,"serialPort_withOLP",withOverLaped)) return false;
	if(!XXml::addLeafNode(element,"serialPort_stopBits",XString::toString(stopBits))) return false;
	if(!XXml::addLeafNode(element,"serialPort_dtr",XString::toString(dtr))) return false;
	if(!XXml::addLeafNode(element,"serialPort_rts",XString::toString(rts))) return false;
	if(!XXml::addLeafNode(element,"serialPort_WDT",XString::toString(waitingDataTime))) return false;
	if(!XXml::addLeafNode(element,"serialPort_byteSize",XString::toString(byteSize))) return false;
	return true;
}
#if !WITH_INLINE_FILE
#include "XSerialPort.inl"
#endif
}