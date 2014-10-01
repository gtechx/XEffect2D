#include "XSonovoProtocol.h"
#include "XTimer.h"
#include "XMath/XMath.h"

void SPRecvCB(void *pClass,unsigned char * data,int len)
{
	_XSonovoProtocol &pPar = *(_XSonovoProtocol *)pClass;
	//�����ǽ�������
	if(pPar.m_sendThreadState == STATE_SET_TO_END ||
		pPar.m_sendThreadState == STATE_END) return;	//����ֱ�Ӷ���
	_XSPData tempData;
	tempData.data = createArrayMem<unsigned char>(len);
	if(tempData.data == NULL) return;
	memcpy(tempData.data,data,len);
	tempData.dataLen = len;
	//���յ�������������ݽ��д���
	if(tempData.dataLen < 3)
	{
		LogStr(" - data ERROR");
		pPar.showData(tempData,"����: ");
		XDELETE_ARRAY(tempData.data);
		return;
	}
	unsigned short crcCheck = CRC16_XModen(tempData.data,tempData.dataLen - 2);
	if(tempData.data[tempData.dataLen - 2] != (crcCheck >> 8)
		|| tempData.data[tempData.dataLen - 1] != (crcCheck & 0xff))
	{
		LogStr(" - CRC ERROR");
		pPar.showData(tempData,"����: ");
		XDELETE_ARRAY(tempData.data);
		return;
	}

	if(pPar.m_withLog) pPar.showData(tempData,"����: ");

	if(pPar.m_info.devType == DEV_MASTER)
	{//��
		if(pPar.m_withStatistics) 
		{
			++ pPar.m_comunicateTimesNow;
			pPar.m_delayTimeNow += pPar.m_delayTime;
		}
		pPar.m_recvMutex.Lock();
		pPar.m_recvData.push_back(tempData);
		pPar.m_recvMutex.Unlock();

		pPar.m_sendMutex.Lock();	//��������ǰ���񲻱�Ҫ����Ҫ��һ������
		pPar.m_needRecv = XFalse;
		pPar.m_sendMutex.Unlock();
	}else
	{//��
		pPar.answerProc(tempData);		//�ظ�
		XDELETE_ARRAY(tempData.data);	//������Ҫ�ͷ���Դ
	}
}
DWORD WINAPI _XSonovoProtocol::sendThread(void * pParam)
{
	_XSonovoProtocol &pPar = *(_XSonovoProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;
	int upTime = getCurrentTicks();
	while(pPar.m_sendThreadState != STATE_SET_TO_END)
	{
		//���﷢������
		if(pPar.m_info.devType == DEV_MASTER)
		{
			pPar.m_sendMutex.Lock();
			if(pPar.m_needRecv)  
			{
				pPar.m_delayTime += getCurrentTicks() - upTime;
				upTime = getCurrentTicks();
			}else
			{
				upTime = getCurrentTicks();
			}
			pPar.m_sendMutex.Unlock();
		}
		pPar.m_sendMutex.Lock();
		if(pPar.m_sendData.size() > 0 && !pPar.m_needRecv)	//�����Ҫ���յĻ���Ҫ�ȴ�������ɲ��ܼ�������
		{//��������Ҫ����,���﷢������
			pPar.m_nowSendData = pPar.m_sendData[0];
			pPar.m_sendData.pop_front();
			pPar.sendNowData();
			XDELETE_ARRAY(pPar.m_nowSendData.data);	//�������֮�����ݶ���
		}
		pPar.m_sendMutex.Unlock();
		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
_XBool _XSonovoProtocol::openDevice(const _XSPInfo &info)	//���豸
{
	if(m_isInited) return XFalse;
	m_info = info;

	if(!m_serialPort.open(m_info.nPort,m_info.nBaud,m_info.nParity,SP_MODE_AUTO)) return XFalse;
	m_serialPort.setCallBackFun(SPRecvCB,this);

	m_needRecv = XFalse;
	//��������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

	m_isInited = XTrue;
	return XTrue;
}
void _XSonovoProtocol::answerProc(_XSPData &data)			//����ָ��������ش�����
{
	switch(data.data[0])
	{
	case CMD_WRITE:
		{//�����ݽ��д���
			if(data.dataLen <= 5) 
			{
				LogStr("���������!");
				return;
			}
			int start = data.data[1];
			int sum = (data.dataLen - 4) >> 1;
			if(sum <= 0 || sum > m_info.buffSize - start || m_info.buff == NULL)
			{//���ݷǷ�������д����
				_XSPData tempData;
				unsigned char tmp[] = {CMD_WRITE_ACK,0x00,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
			}else
			{//��������
				for(int i = 0;i < sum;++ i)
				{
					m_info.buff[start + i] = (data.data[2 + (i << 1)] << 8) + data.data[2 + (i << 1) + 1];
				}
				//memcpy(m_info.buff + start,data.data + 3,sum << 1);
				//�������ݳɹ�
				_XSPData tempData;
				unsigned char tmp[] = {CMD_WRITE_ACK,0x01,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ:
		{//�����ݽ��д���
			if(data.dataLen != 5)
			{
				LogStr("���������!");
				return;
			}
			int start = data.data[1];
			int sum = data.data[2];
			if(sum <= 0 || sum > m_info.buffSize - start || m_info.buff == NULL)
			{//��ȡ���ݴ���
				_XSPData tempData;
				unsigned char tmp[] = {CMD_READ_ACK,0x00,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
			}else
			{//��������
				//�������ݳɹ�
				_XSPData tempData;
				tempData.dataLen = (sum << 1) + 3 + 2;
				tempData.data = createArrayMem<unsigned char>(tempData.dataLen);
				tempData.data[0] = CMD_READ_ACK;
				tempData.data[1] = 0x01;
				tempData.data[2] = start;
				for(int i = 0;i < sum;++ i)
				{
					tempData.data[3 + (i << 1)] = (m_info.buff[start + i] >> 8) % 256;
					tempData.data[3 + (i << 1) + 1] = m_info.buff[start + i] % 256;
				}
				//memcpy(tempData.data + 3,m_info.buff + start,sum << 1);
				pushAData(tempData);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	default:	//�յ����������
		LogStr("�յ����������!");
		break;
	}
}
_XBool _XSonovoProtocol::pushAData(_XSPData &data)			//���Ͷ���������һ������
{
	if(data.data == NULL || data.dataLen <= 0) return XFalse;
	_XSPData tempData;
	tempData.dataLen = data.dataLen;
	tempData.data = createArrayMem<unsigned char>(data.dataLen);
	if(tempData.data == NULL) return XFalse;
	memcpy(tempData.data,data.data,data.dataLen);
	m_sendMutex.Lock();
	m_sendData.push_back(tempData);
	m_sendMutex.Unlock();
	return XTrue;
}
void _XSonovoProtocol::sendNowData()
{
	if(m_nowSendData.data == NULL) return;
	unsigned short crcCheck = CRC16_XModen(m_nowSendData.data,m_nowSendData.dataLen - 2);	//��������ֽ�ΪУ��λ
//	m_nowSendData.data[0] = m_modbusState.deviceID;
	m_nowSendData.data[m_nowSendData.dataLen - 2] = (crcCheck >> 8);
	m_nowSendData.data[m_nowSendData.dataLen - 1] = (crcCheck & 0xff);
#if WITH_LOG
	if(m_withLog)  showData(m_nowSendData,"���ͣ�");
#endif
	m_serialPort.sendData(m_nowSendData.data,m_nowSendData.dataLen);
	if(m_info.devType == DEV_MASTER) m_needRecv = XTrue;	//ֻ������ʱ�ű����Ҫ�ȴ�����
	m_delayTime = 0;
}
#include "XBasicWindow.h"
void _XSonovoProtocol::update()
{
	if(!m_withStatistics) return;
	m_statisticsTimer += XEE::frameTime;
	if(m_statisticsTimer >= 5000)
	{
		if(m_comunicateTimesNow > 0.0f)
		{
			float tmp = m_comunicateTimesNow;
			m_delayTimeAvg = m_delayTimeNow / tmp;	//ƽ������
		}
		//����ͳ��
		m_delayTimeNow = 0;
		m_comunicateTimesNow = 0;
		m_statisticsTimer = 0;
	}
	m_serialPort.update();
}
void _XSonovoProtocol::writeOneRegister(int addr,int value)
{
	if(m_info.devType == DEV_SLAVE) return;
	unsigned char data[6];
	data[0] = CMD_WRITE;
	data[1] = addr;
	data[2] = (value >> 8) % 256;
	data[3] = value % 256;
	_XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XSonovoProtocol::readRegisters(int startAddr,int sum)	//��ȡ������ּĴ���
{
	if(m_info.devType == DEV_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[5];
	data[0] = CMD_READ;
	data[1] = startAddr;
	data[2] = sum;
	_XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}