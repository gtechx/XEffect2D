#include "XStdHead.h"
#include "XSonovoProtocol.h"
#include "XTimer.h"
#include "XMath/XMath.h"

namespace XE{
void XSonovoProtocol::SPRecvCB(void *pClass,unsigned char * data,int len)
{
	XSonovoProtocol &pPar = *(XSonovoProtocol *)pClass;
	//�����ǽ�������
	if(pPar.m_sendThreadState == STATE_SET_TO_END ||
		pPar.m_sendThreadState == STATE_END) return;	//����ֱ�Ӷ���
	XSPData tempData;
	tempData.data = XMem::createArrayMem<unsigned char>(len);
	if(tempData.data == NULL) return;
	memcpy(tempData.data,data,len);
	tempData.dataLen = len;
	//���յ�������������ݽ��д���
	if(tempData.dataLen < 3)
	{
		LogStr(" - data ERROR");
		pPar.showData(tempData,"����: ");
		XMem::XDELETE_ARRAY(tempData.data);
		return;
	}
	unsigned short crcCheck = XMath::CRC16XModen(tempData.data,tempData.dataLen - 2);
	if(tempData.data[tempData.dataLen - 2] != (crcCheck >> 8)
		|| tempData.data[tempData.dataLen - 1] != (crcCheck & 0xff))
	{
		LogStr(" - CRC ERROR");
		pPar.showData(tempData,"����: ");
		XMem::XDELETE_ARRAY(tempData.data);
		return;
	}

	if(pPar.m_withLog) pPar.showData(tempData,"����: ");

	if(pPar.m_info.devType == DEV_MASTER)
	{//��
		if(pPar.m_withStatistics) 
		{
			++ pPar.m_comunicateTimesCur;
			pPar.m_delayTimeCur += pPar.m_delayTime;
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
		XMem::XDELETE_ARRAY(tempData.data);	//������Ҫ�ͷ���Դ
	}
}
DWORD WINAPI XSonovoProtocol::sendThread(void * pParam)
{
	XSonovoProtocol &pPar = *(XSonovoProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;
	int upTime = XTime::getCurrentTicks();
	while(pPar.m_sendThreadState != STATE_SET_TO_END)
	{
		//���﷢������
		if(pPar.m_info.devType == DEV_MASTER)
		{
			pPar.m_sendMutex.Lock();
			if(pPar.m_needRecv)  
			{
				pPar.m_delayTime += XTime::getCurrentTicks() - upTime;
				upTime = XTime::getCurrentTicks();
			}else
			{
				upTime = XTime::getCurrentTicks();
			}
			pPar.m_sendMutex.Unlock();
		}
		pPar.m_sendMutex.Lock();
		if(pPar.m_sendData.size() > 0 && !pPar.m_needRecv)	//�����Ҫ���յĻ���Ҫ�ȴ�������ɲ��ܼ�������
		{//��������Ҫ����,���﷢������
			pPar.m_curSendData = pPar.m_sendData[0];
			pPar.m_sendData.pop_front();
			pPar.sendCurData();
			XMem::XDELETE_ARRAY(pPar.m_curSendData.data);	//�������֮�����ݶ���
		}
		pPar.m_sendMutex.Unlock();
		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
XBool XSonovoProtocol::openDevice(const XSPInfo &info)	//���豸
{
	if(m_isInited) return XFalse;
	m_info = info;

	if(!m_serialPort.open(info.serialPortInfo)) return XFalse;
	m_serialPort.setCallBackFun(SPRecvCB,this);

	m_needRecv = XFalse;
	//��������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

	m_isInited = XTrue;
	return XTrue;
}
void XSonovoProtocol::answerProc(XSPData &data)			//����ָ��������ش�����
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
				XSPData tempData;
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
				XSPData tempData;
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
				XSPData tempData;
				unsigned char tmp[] = {CMD_READ_ACK,0x00,0x00,0x00};
				tempData.data = tmp;
				tempData.dataLen = sizeof(tmp);
				pushAData(tempData);
			}else
			{//��������
				//�������ݳɹ�
				XSPData tempData;
				tempData.dataLen = (sum << 1) + 3 + 2;
				tempData.data = XMem::createArrayMem<unsigned char>(tempData.dataLen);
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
XBool XSonovoProtocol::pushAData(XSPData &data)			//���Ͷ���������һ������
{
	if(data.data == NULL || data.dataLen <= 0) return XFalse;
	XSPData tempData;
	tempData.dataLen = data.dataLen;
	tempData.data = XMem::createArrayMem<unsigned char>(data.dataLen);
	if(tempData.data == NULL) return XFalse;
	memcpy(tempData.data,data.data,data.dataLen);
	m_sendMutex.Lock();
	m_sendData.push_back(tempData);
	m_sendMutex.Unlock();
	return XTrue;
}
void XSonovoProtocol::sendCurData()
{
	if(m_curSendData.data == NULL) return;
	unsigned short crcCheck = XMath::CRC16XModen(m_curSendData.data,m_curSendData.dataLen - 2);	//��������ֽ�ΪУ��λ
//	m_curSendData.data[0] = m_modbusState.deviceID;
	m_curSendData.data[m_curSendData.dataLen - 2] = (crcCheck >> 8);
	m_curSendData.data[m_curSendData.dataLen - 1] = (crcCheck & 0xff);
#if WITH_LOG
	if(m_withLog)  showData(m_curSendData,"���ͣ�");
#endif
	m_serialPort.sendData(m_curSendData.data,m_curSendData.dataLen);
	if(m_info.devType == DEV_MASTER) m_needRecv = XTrue;	//ֻ������ʱ�ű����Ҫ�ȴ�����
	m_delayTime = 0;
}
void XSonovoProtocol::update()
{
	if(!m_withStatistics) return;
	if (gFrameworkData.pFrameWork == NULL) m_statisticsTimer += 1;
	else m_statisticsTimer += gFrameworkData.pFrameWork->getLastFrameTime();
	if(m_statisticsTimer >= 5000)
	{
		if(m_comunicateTimesCur > 0.0f)
		{
			float tmp = m_comunicateTimesCur;
			m_delayTimeAvg = m_delayTimeCur / tmp;	//ƽ������
		}
		//����ͳ��
		m_delayTimeCur = 0;
		m_comunicateTimesCur = 0;
		m_statisticsTimer = 0;
	}
	m_serialPort.update();
}
void XSonovoProtocol::writeOneRegister(int addr,int value)
{
	if(m_info.devType == DEV_SLAVE) return;
	unsigned char data[6];
	data[0] = CMD_WRITE;
	data[1] = addr;
	data[2] = (value >> 8) % 256;
	data[3] = value % 256;
	XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void XSonovoProtocol::readRegisters(int startAddr,int sum)	//��ȡ������ּĴ���
{
	if(m_info.devType == DEV_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[5];
	data[0] = CMD_READ;
	data[1] = startAddr;
	data[2] = sum;
	XSPData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
#if !WITH_INLINE_FILE
#include "XSonovoProtocol.inl"
#endif
}