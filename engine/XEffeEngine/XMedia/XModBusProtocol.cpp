#include "XModBusProtocol.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
#include "XLogBook.h"
#include "XTimer.h"
#include "XMath/XMath.h"

inline void showData(const _XModBusData &data,const std::string &title)
{
	std::string tmp = title + "data:";
	char tmpStr[32];
	for(int i = 0;i < data.dataLen;++ i)
	{
		sprintf(tmpStr,"0x%02x ",data.data[i]);
		tmp += tmpStr;
	}
	LogStr(tmp.c_str());
}
void recvCallback(void *pClass,unsigned char * data,int len)
{
	_XModBusProtocol &pPar = *(_XModBusProtocol *)pClass;
	if(pPar.m_sendThreadState == STATE_SET_TO_END ||
		pPar.m_sendThreadState == STATE_END) return;	//����ֱ�Ӷ���
	_XModBusData tempData;
	tempData.data = createArrayMem<unsigned char>(len);
	if(tempData.data == NULL) return;
	memcpy(tempData.data,data,len);
//	tempData.data = data;
	tempData.dataLen = len;
	tempData.delayTime = pPar.m_delayTime;
	if(data[0] == pPar.m_modbusState.deviceID || pPar.m_modbusState.workType == TYPE_MASTER)
	{//�Լ�������
		if(tempData.dataLen < 3)
		{
			LogStr(" - data ERROR");
			showData(tempData,"����: ");
			XDELETE_ARRAY(tempData.data);
			return;
		}
		unsigned short crcCheck = CRC16_Modbus(tempData.data,tempData.dataLen - 2);
		if(tempData.data[tempData.dataLen - 1] != (crcCheck >> 8)
			|| tempData.data[tempData.dataLen - 2] != (crcCheck & 0xff))
		{
			LogStr(" - CRC ERROR");
			showData(tempData,"����: ");
			XDELETE_ARRAY(tempData.data);
			return;
		}
#if WITH_LOG
		if(pPar.m_withLog) showData(tempData,"����: ");
#endif
		if(pPar.m_modbusState.workType == TYPE_MASTER)
		{//���ķ�ʽ
			pPar.m_recvMutex.Lock();
			switch(pPar.m_nowSendData.data[1])
			{
			case CMD_READ_COIL_VOLUME:
			case CMD_READ_INPUT:
			case CMD_READ_HOLD_REGISTER:
			case CMD_READ_INPUT_REGISTER:
				tempData.addr = (pPar.m_nowSendData.data[2] << 8) + pPar.m_nowSendData.data[3];
				break;
			case CMD_SET_ONE_COIL:
			case CMD_SET_ONE_REGISTER:
			case CMD_SET_COILS:
			case CMD_SET_REGISTERS:
				tempData.addr = (tempData.data[2] << 8) + tempData.data[3];
				break;
			}
			if(pPar.m_withStatistics) 
			{
				++ pPar.m_comunicateTimesNow;
				pPar.m_delayTimeNow += tempData.delayTime;
			}
			pPar.m_recvData.push_back(tempData);
			pPar.m_recvMutex.Unlock();
			pPar.m_sendMutex.Lock();	//��������ǰ���񲻱�Ҫ����Ҫ��һ������
			pPar.m_needRecv = XFalse;
			pPar.m_delaySendTime = 0;
			XDELETE_ARRAY(pPar.m_nowSendData.data);
			pPar.m_sendMutex.Unlock();
		}else
		{//�ӵķ�ʽ
			pPar.answerProc(tempData);
			XDELETE_ARRAY(tempData.data);	
		}
	}else
	{//���˵�����
#if WITH_LOG
		if(pPar.m_withLog)  showData(tempData,"����:���˵����� ");
#endif
		XDELETE_ARRAY(tempData.data);	//�ͷ�����
	}
}
_XBool _XModBusProtocol::openDevice(_XModbusState &modbusState)
{
	if(m_isInited) return XFalse;
	m_modbusState = modbusState;

	//if(!m_serialPort.open(m_modbusState.nPort,m_modbusState.nBaud,m_modbusState.nParity)) return XFalse;
	if(!m_serialPort.open(m_modbusState.nPort,m_modbusState.nBaud,m_modbusState.nParity,SP_MODE_AUTO)) return XFalse;
	m_serialPort.setCallBackFun(recvCallback,this);
	m_serialPort.setWaitingDataTime(modbusState.maxWaitingDataTime);
	//���濪�������̺߳ͷ����߳�
//	m_recvThreadState = STATE_BEFORE_START;
	m_sendThreadState = STATE_BEFORE_START;
//	m_tempDataLen = 0;
//	m_headLen = 0;
	m_needRecv = XFalse;
	m_connectState = XTrue;
	m_delaySendTime = 0;
//	if(m_modbusState.workType == TYPE_MASTER) m_modbusNeedHeadLen = 3;
//	if(m_modbusState.workType == TYPE_SLAVE) m_modbusNeedHeadLen = 7;

//	if(CreateThread(0,0,recvThread,this,0,NULL) == 0) return XFalse;	//���������߳�
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

	m_isInited = XTrue;
	return XTrue;
}
_XBool _XModBusProtocol::pushAData(_XModBusData &data)
{
	if(data.data == NULL || data.dataLen <= 0) return XFalse;
	_XModBusData tempData;
	tempData.dataLen = data.dataLen;
	tempData.data = createArrayMem<unsigned char>(data.dataLen);
	if(tempData.data == NULL) return XFalse;
	memcpy(tempData.data,data.data,data.dataLen);
	m_sendMutex.Lock();
	m_sendData.push_back(tempData);
	m_sendMutex.Unlock();
	return XTrue;
}
void _XModBusProtocol::answerProc(_XModBusData &data)
{
	switch(data.data[1])
	{
	case CMD_READ_COIL_VOLUME:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < start + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x81,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				int len = 0;
				if(sum % 8 == 0) len = sum / 8;
				else len = sum / 8 + 1;
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(len + 5);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x81,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = len + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_COIL_VOLUME;
				tempData.data[2] = len;
				//����ȡֵ
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(m_modbusState.coilsBuff[(i + start) >> 3],(i + start) % 8);
					setBit(tempData.data[3 + (i >> 3)],i % 8,temp);
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = start;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_INPUT:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.inputOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.inputBuffSize == 0 || m_modbusState.inputBuffSize < start + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x82,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				int len = 0;
				if(sum % 8 == 0) len = sum / 8;
				else len = sum / 8 + 1;
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(len + 5);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x82,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = len + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_INPUT;
				tempData.data[2] = len;
				//����ȡֵ
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(m_modbusState.inputBuff[(i + start) >> 3],(i + start) % 8);
					setBit(tempData.data[3 + (i >> 3)],i % 8,temp);
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_HOLD_REGISTER:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < start + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x83,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(sum * 2 + 5);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x83,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = sum * 2 + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_HOLD_REGISTER;
				tempData.data[2] = sum * 2;
				//����ȡֵ
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&tempData.data[3 + i * 2],&m_modbusState.hRegisterBuff[start + i],2);
					tempData.data[3 + (i << 1) + 0] = m_modbusState.hRegisterBuff[start + i] >> 8;
					tempData.data[3 + (i << 1) + 1] = m_modbusState.hRegisterBuff[start + i] & 0xff;
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = start;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_READ_INPUT_REGISTER:
		{
			unsigned short start = (data.data[2] << 8) + data.data[3] - m_modbusState.iRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.iRegisterBuffSize == 0 || m_modbusState.iRegisterBuffSize < start + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x84,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(sum * 2 + 5);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x84,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				tempData.dataLen = sum * 2 + 5;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_READ_INPUT_REGISTER;
				tempData.data[2] = sum * 2;
				//����ȡֵ
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&tempData.data[3 + i * 2],&m_modbusState.iRegisterBuff[start + i],2);
					tempData.data[3 + (i << 1) + 0] = m_modbusState.iRegisterBuff[start + i] >> 8;
					tempData.data[3 + (i << 1) + 1] = m_modbusState.iRegisterBuff[start + i] & 0xff;
				}
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = start;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_ONE_COIL:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short value = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < pos)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x85,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x85,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				if(value == 0x0000) setBit(m_modbusState.coilsBuff[pos >> 3],pos % 8,0);
				if(value == 0xff00) setBit(m_modbusState.coilsBuff[pos >> 3],pos % 8,1);
				value = getBit(m_modbusState.coilsBuff[pos >> 3],pos % 8);
				if(value != 0) value = 0xff00;
				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_ONE_COIL;
				pos += m_modbusState.coilsOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = value >> 8;
				tempData.data[5] = value & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_ONE_REGISTER:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short value = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < pos)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x86,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x86,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}

				m_modbusState.hRegisterBuff[pos] = value;
				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_ONE_REGISTER;
				pos += m_modbusState.hRegisterOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = value >> 8;
				tempData.data[5] = value & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = pos;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_COILS:		///++++++++++
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.coilsOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.coilsBuffSize == 0 || m_modbusState.coilsBuffSize < pos + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x8f,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x8f,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				//����ֵ
				int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					temp = getBit(data.data[7 + (i >> 3)],i % 8);
					setBit(m_modbusState.coilsBuff[(i + pos) >> 3],(i + pos) % 8,temp);
				}

				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_COILS;
				pos += m_modbusState.coilsOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = sum >> 8;
				tempData.data[5] = sum & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = pos;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	case CMD_SET_REGISTERS:
		{
			unsigned short pos = (data.data[2] << 8) + data.data[3] - m_modbusState.hRegisterOffset;
			unsigned short sum = (data.data[4] << 8) + data.data[5];
			if(m_modbusState.hRegisterBuffSize == 0 || m_modbusState.hRegisterBuffSize < pos + sum)
			{//ֱ�ӷ��ش���
				_XModBusData tempData;
				unsigned char retData[] = {0x00,0x90,0x01,0x00,0x00};
				retData[0] = m_modbusState.deviceID;
				tempData.data = retData;
				tempData.dataLen = sizeof(retData);
				pushAData(tempData);
			}else
			{//��������
				_XModBusData tempData;
				tempData.data = createArrayMem<unsigned char>(8);
				if(tempData.data == NULL)
				{//����ʧ��
					unsigned char retData[] = {0x00,0x90,0x01,0x00,0x00};
					retData[0] = m_modbusState.deviceID;
					tempData.data = retData;
					tempData.dataLen = sizeof(retData);
					pushAData(tempData);
					return;
				}
				//����ֵ
				//int temp = 0;
				for(int i = 0;i < sum;++ i)
				{
					//memcpy(&m_modbusState.hRegisterBuff[pos + i],&data.data[7 + (i << 1)],2);
					m_modbusState.hRegisterBuff[pos + i] = (data.data[7 + (i << 1)] << 8) + data.data[7 + (i << 1) + 1];
				}

				tempData.dataLen = 8;
				tempData.data[0] = m_modbusState.deviceID;
				tempData.data[1] = CMD_SET_REGISTERS;
				pos += m_modbusState.hRegisterOffset;
				tempData.data[2] = pos >> 8;
				tempData.data[3] = pos & 0xff;
				tempData.data[4] = sum >> 8;
				tempData.data[5] = sum & 0xff;
				pushAData(tempData);
				XDELETE_ARRAY(tempData.data);
				data.addr = pos;	//add by xiajia
				if(m_callBackFun != NULL) m_callBackFun(data,m_pClass);
			}
		}
		break;
	}
}
_XModBusData _XModBusProtocol::popData()
{
	_XModBusData tempData;
	if(m_modbusState.workType == TYPE_MASTER)
	{
		m_recvMutex.Lock();
		if(m_recvData.size() > 0)
		{
			tempData = m_recvData[0];
			m_recvData.pop_front();
		}
		m_recvMutex.Unlock();
	}
	return tempData;
}
void _XModBusProtocol::release()
{
	if(!m_isInited) return;
	//�ȴ��߳��˳�
	//if(m_recvThreadState == STATE_START)
	//{
	//	m_recvThreadState = STATE_SET_TO_END;
	//	while(true)
	//	{
	//		if(m_recvThreadState == STATE_END) break;
	//		Sleep(1);
	//	}
	//}
	waitThreadEnd(m_sendThreadState);
	//������е�����
	for(int i = 0;i < m_sendData.size();++ i)
	{
		XDELETE_ARRAY(m_sendData[i].data);
	}
	m_sendData.clear();
	for(int i = 0;i < m_sendData.size();++ i)
	{
		XDELETE_ARRAY(m_recvData[i].data);
	}
	m_recvData.clear();
//	XDELETE_ARRAY(m_tempDataBuff);
//	m_tempDataLen = 0;

	m_serialPort.close();
	m_isInited = XFalse;
}
//#define MODBUS_MAX_DATA_LEN (1024)
//void _XModBusProtocol::recvDataProc(unsigned char * data,int len)
//{
//	if(len == 0) return;
//
//	if(m_tempDataLen == 0)
//	{//ͷ�в�����
//		if(m_headLen + len >= m_modbusNeedHeadLen)
//		{//ͷ����
//			int offset = m_modbusNeedHeadLen - m_headLen;
//			memcpy(m_modbusHead + m_headLen,data,offset);
//			m_headLen = 0;
//			//����ͷ
//			if(m_modbusState.workType == TYPE_MASTER)
//			{
//				switch(m_modbusHead[1])
//				{
//					//��ȷ����
//				case CMD_READ_COIL_VOLUME:
//				case CMD_READ_INPUT:
//				case CMD_READ_HOLD_REGISTER:
//				case CMD_READ_INPUT_REGISTER:
//					m_tempDataLen = m_modbusHead[2] + 5;
//					break;
//				case CMD_SET_ONE_COIL:
//				case CMD_SET_ONE_REGISTER:
//				case CMD_SET_COILS:;
//				case CMD_SET_REGISTERS:
//					m_tempDataLen = 8;
//					break;
//					//���󷵻�
//				case ERROR_READ_COIL_VOLUME:
//				case ERROR_READ_INPUT:
//				case ERROR_READ_HOLD_REGISTER:
//				case ERROR_READ_INPUT_REGISTER:
//				case ERROR_SET_ONE_COIL:
//				case ERROR_SET_ONE_REGISTER:
//				case ERROR_SET_COILS:
//				case ERROR_SET_REGISTERS:
//					m_tempDataLen = 5;
//					break;
//				}
//			}else
//			{
//				switch(m_modbusHead[1])
//				{
//					//��ȷ����
//				case CMD_READ_COIL_VOLUME:
//				case CMD_READ_INPUT:
//				case CMD_READ_HOLD_REGISTER:
//				case CMD_READ_INPUT_REGISTER:
//				case CMD_SET_ONE_COIL:
//				case CMD_SET_ONE_REGISTER:
//					m_tempDataLen = 8;
//					break;
//
//				case CMD_SET_COILS:
//				case CMD_SET_REGISTERS:
//					m_tempDataLen = m_modbusHead[6] + 9;
//					break;
//				}
//			}
//
//			m_tempDataBuff = createArrayMem<unsigned char>(m_tempDataLen);
//			if(m_tempDataBuff == NULL)
//			{//��������
//				m_tempDataLen = 0;
//				m_headLen = 0;
//				return;
//			}
//			memcpy(m_tempDataBuff,m_modbusHead,m_modbusNeedHeadLen);
//			m_tempDataOffset = m_modbusNeedHeadLen;
//			recvDataProc(data + offset,len - offset);
//		}else
//		{//ͷ������
//			memcpy(m_modbusHead + m_headLen,data,len);
//			m_headLen += len;
//			return;
//		}
//	}else
//	{//ͷ�Ѿ����
//		if(m_tempDataOffset + len >= m_tempDataLen)
//		{//���ݰ��Ѿ�����
//			int need = m_tempDataLen - m_tempDataOffset;
//			memcpy(m_tempDataBuff + m_tempDataOffset,data,need);
//			if(m_tempDataBuff[0] == m_modbusState.deviceID)
//			{//�Ƿ����ҵ����ݰ��򽫰��������
//				if(m_modbusState.workType == TYPE_MASTER)
//				{//���ķ�ʽ
//					if(m_needRecv && (m_nowSendData.data[1] == m_tempDataBuff[1] || m_nowSendData.data[1] == m_tempDataBuff[1] + 0x80))
//					{//��ȷ��Ӧ��(�����߼�������)
//						_XModBusData tempData;
//						tempData.data = m_tempDataBuff;
//						tempData.dataLen = m_tempDataLen;
//						tempData.delayTime = m_delayTime;
//						//����Ҫ����CRC16_ModbusУ��
//						unsigned short crcCheck = CRC16_Modbus(tempData.data,tempData.dataLen - 2);
//						if(tempData.data[tempData.dataLen - 1] != (crcCheck >> 8)
//							|| tempData.data[tempData.dataLen - 2] != (crcCheck & 0xff))
//						{//У��ʧ�ܶ�������
//							XDELETE_ARRAY(m_tempDataBuff);
//						}else
//						{//У��ɹ�
//							m_recvMutex.Lock();
//							m_recvData.push_back(tempData);
//							m_recvMutex.Unlock();
//							m_sendMutex.Lock();
//							m_needRecv = XFalse;
//							XDELETE_ARRAY(m_nowSendData.data);
//							m_sendMutex.Unlock();
//						}
//					}else
//					{//����Ļش�ֱ�Ӷ�������
//						XDELETE_ARRAY(m_tempDataBuff);
//					}
//				}else
//				{//�ӵķ�ʽ(ֱ�Ӵ�����յ������ݣ����������)(��δ���)
//					_XModBusData tempData;
//					tempData.data = m_tempDataBuff;
//					tempData.dataLen = m_tempDataLen;
//					tempData.delayTime = m_delayTime;
//					for(int i = 0;i < tempData.dataLen;++ i)
//					{
//						printf("0x%02x ",tempData.data[i]);
//					}
//					printf(" - \n");
//					answerProc(tempData);
//					//�ͷ��ڴ�ռ�
//					XDELETE_ARRAY(m_tempDataBuff);
//				}
//			}else
//			{//�������ݰ�
//				XDELETE_ARRAY(m_tempDataBuff);
//			}
//			//�����������µ�����
//			len -= need;
//			m_tempDataOffset = 0;
//			m_tempDataLen = 0;
//			m_tempDataBuff = NULL;
//			recvDataProc(data + need,len);
//
//			return;
//		}else
//		{//���ݰ��в�����
//			memcpy(m_tempDataBuff + m_tempDataOffset,data,len);
//			m_tempDataOffset += len;
//			return;
//		}
//	}
//}
//DWORD WINAPI _XModBusProtocol::recvThread(void * pParam)
//{
//	_XModBusProtocol &pPar = *(_XModBusProtocol *)pParam;
//	pPar.m_recvThreadState = STATE_START;
//	unsigned char tempBuff[MODBUS_MAX_DATA_LEN];	//�������ݳ��Ȳ��ᳬ��
//	int ret = 0;
//	while(true)
//	{
//		if(pPar.m_recvThreadState == STATE_SET_TO_END) break;
//		//if(!pPar.m_needRecv) continue;
//		//�����������
//		ret = pPar.m_serialPort.readData(tempBuff,MODBUS_MAX_DATA_LEN);
//		if(ret > 0)
//		{//��ȡ����Ч����
//			pPar.recvDataProc(tempBuff,ret);
//		}else
//		if(ret < 0)
//		{
//			pPar.m_connectState = XFalse;
//			break;
//		}
//		Sleep(1);
//	}
//	pPar.m_recvThreadState = STATE_END;
//	return 0;
//}
void _XModBusProtocol::sendNowData()
{
	if(m_nowSendData.data == NULL) return;
	++ m_sendTime;
	unsigned short crcCheck = CRC16_Modbus(m_nowSendData.data,m_nowSendData.dataLen - 2);	//��������ֽ�ΪУ��λ
//	m_nowSendData.data[0] = m_modbusState.deviceID;
	m_nowSendData.data[m_nowSendData.dataLen - 1] = (crcCheck >> 8);
	m_nowSendData.data[m_nowSendData.dataLen - 2] = (crcCheck & 0xff);
#if WITH_LOG
	if(m_withLog)  showData(m_nowSendData,std::string("����:T-") + toString(m_sendTime) + " ");
#endif
	m_serialPort.sendData(m_nowSendData.data,m_nowSendData.dataLen);
	if(m_modbusState.workType == TYPE_MASTER) m_needRecv = XTrue;	//ֻ������ʱ�ű����Ҫ�ȴ�����
	m_delayTime = 0;
}
DWORD WINAPI _XModBusProtocol::sendThread(void * pParam)
{
	_XModBusProtocol &pPar = *(_XModBusProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;
	int upTime = getCurrentTicks();
	while(pPar.m_sendThreadState != STATE_SET_TO_END)
	{
		if(pPar.m_modbusState.workType == TYPE_MASTER)
		{
			pPar.m_sendMutex.Lock();
			if(pPar.m_needRecv)  
			{
				pPar.m_delayTime += getCurrentTicks() - upTime;
				upTime = getCurrentTicks();

				if(pPar.m_delayTime >= pPar.m_modbusState.outTime)
				{//�����ʱ�����·���,��Ŀǰ���ط����ƻ����������ƶ�������߼����⣬��Ҫ�ڿ��ǣ�
					if(pPar.m_sendTime == pPar.m_modbusState.maxResendTimes)
					{
						LogStr("�����ط�����");
						//����1��������ӶϿ�,����Ҫ�������Ӳ��ܼ���ʹ��
					//	pPar.m_connectState = XFalse;	//���������쳣
					//	break;
						//����2��������ǰ��Ҫ���͵����ݣ���Ȼ���Լ�������
						pPar.m_needRecv = XFalse;
						XDELETE_ARRAY(pPar.m_nowSendData.data);
					}else
					{//�ط�
						pPar.sendNowData();
					}
				}
				pPar.m_sendMutex.Unlock();
				continue;
			}else
			{
				pPar.m_delaySendTime += getCurrentTicks() - upTime;
				pPar.m_sendMutex.Unlock();

				upTime = getCurrentTicks();
			}
		}
		//���﷢������
		pPar.m_sendMutex.Lock();
	//	if(pPar.m_sendData.size() > 0 && (pPar.m_modbusState.workType != TYPE_MASTER || !pPar.m_needRecv))
		if(pPar.m_sendData.size() > 0 && !pPar.m_needRecv)	//�����Ҫ���յĻ���Ҫ�ȴ�������ɲ��ܼ�������
		{//��������Ҫ����,���﷢������
			if(pPar.m_modbusState.workType == TYPE_MASTER && 
				pPar.m_delaySendTime < pPar.m_modbusState.maxR2STime)
			{//���պͷ���֮���ʱ�����ҪΪ5��������
			}else
			{
				pPar.m_nowSendData = pPar.m_sendData[0];
				pPar.m_sendData.pop_front();
				pPar.m_sendTime = 0;
				pPar.sendNowData();
				if(pPar.m_modbusState.workType == TYPE_SLAVE)
				{//����ʱ����һ�ξͶ�������
					XDELETE_ARRAY(pPar.m_nowSendData.data);
				}
			}
		}
		pPar.m_sendMutex.Unlock();
		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
int getModbusCMDDataSum(const _XModBusData &CMD)	//�������н�����������
{
	if(CMD.dataLen <= 0) return 0;
	switch(CMD.data[1])
	{
	case CMD_READ_COIL_VOLUME:
	case CMD_READ_INPUT:
		return CMD.data[2] << 3;
	case CMD_READ_HOLD_REGISTER:
	case CMD_READ_INPUT_REGISTER:
		return CMD.data[2] >> 1;
	case CMD_SET_ONE_COIL:
	case CMD_SET_ONE_REGISTER:
		return 1;
	case CMD_SET_COILS:
	case CMD_SET_REGISTERS:
		return 0;
	}
	return 0;
}
bool getModbusCMDData(const _XModBusData &CMD,int index,unsigned int &data)
{//����Э���ȡ����Ĳ�����ַ
	if(CMD.dataLen <= 0) return false;
	if(index < 0 || index >= getModbusCMDDataSum(CMD)) return false;
	int len;
	switch(CMD.data[1])
	{
	case CMD_READ_COIL_VOLUME:
	case CMD_READ_INPUT:
		len = index >> 3;
		data = getBit(CMD.data[3 + len],index % 8);
		break;
	case CMD_READ_HOLD_REGISTER:
	case CMD_READ_INPUT_REGISTER:
		data = (CMD.data[3 + (index << 1)] << 8) + CMD.data[4 + (index << 1)];
		break;
	case CMD_SET_ONE_COIL:
	case CMD_SET_ONE_REGISTER:
		data = (CMD.data[4] << 8) + CMD.data[5];
		break;
	case CMD_SET_COILS:
	case CMD_SET_REGISTERS:
		return false;	//����Ķ���֧��
	}
	return true;
}
void _XModBusProtocol::readCoilState(int startAddr,int sum,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_READ_COIL_VOLUME;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XModBusProtocol::readInputState(int startAddr,int sum,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_READ_INPUT;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XModBusProtocol::writeOneCoilState(int addr,int value,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_SET_ONE_COIL;
	data[2] = (addr >> 8) % 256;
	data[3] = addr % 256;
	data[4] = (value >> 8) % 256;
	data[5] = value % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XModBusProtocol::writeCoilsState(int startAddr,int sum,unsigned char *value,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0 || value == NULL) return;
	int len = sum >> 3;
	if(sum % 8 != 0) ++ len;
	unsigned char *data = createArrayMem<unsigned char>(6 + 1 + len + 2);
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_SET_REGISTERS;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	data[6] = len;
	//���渳ֵ
	for(int i = 0;i < len;++ i)
	{
		data[7 + i] =  value[i];
	}
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = (6 + 1 + len + 2);
	pushData(tempData);
	XDELETE_ARRAY(data);
}
void _XModBusProtocol::readHoldRegisters(int startAddr,int sum,int arm)	//��ȡ������ּĴ���
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_READ_HOLD_REGISTER;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XModBusProtocol::readInputRegisters(int startAddr,int sum,int arm)	//��ȡ������ּĴ���
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_READ_INPUT_REGISTER;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
void _XModBusProtocol::writeRegisters(int startAddr,int sum,const unsigned short *value,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	if(sum <= 0 || value == NULL) return;
	unsigned char *data = createArrayMem<unsigned char>(6 + 1 + (sum << 1) + 2);
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_SET_REGISTERS;
	data[2] = (startAddr >> 8) % 256;
	data[3] = startAddr % 256;
	data[4] = (sum >> 8) % 256;
	data[5] = sum % 256;
	data[6] = sum << 1;
	for(int i = 0;i < sum;++ i)
	{
		data[7 + (i << 1)] = (value[i] >> 8) % 256;
		data[8 + (i << 1)] = value[i] % 256;
	}
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = (6 + 1 + (sum << 1) + 2);
	pushData(tempData);
	XDELETE_ARRAY(data);
}
void _XModBusProtocol::writeOneRegister(int addr,int value,int arm)
{
	if(m_modbusState.workType == TYPE_SLAVE) return;
	unsigned char data[8];
	data[0] = arm;//m_modbusState.deviceID;
	data[1] = CMD_SET_ONE_REGISTER;
	data[2] = (addr >> 8) % 256;
	data[3] = addr % 256;
	data[4] = (value >> 8) % 256;
	data[5] = value % 256;
	_XModBusData tempData;
	tempData.data = data;
	tempData.dataLen = sizeof(data);
	pushData(tempData);
}
#include "XBasicWindow.h"
void _XModBusProtocol::update()
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