#include "XStdHead.h"
#include "XSimpleProtocol.h"
namespace XE{
void XSimpleProtocol::simpleProctocolRecvCB(void *pClass,unsigned char * data,int len)
{
	XSimpleProtocol &pPar = *(XSimpleProtocol *)pClass;
	pPar.recvDataProc(data,len);	//�����������
}
void XSimpleProtocol::recvDataProc(unsigned char *data,int len)
{
	if(data == NULL || len <= 0) return;
	if(m_recvedDataLen != 0)
	{//ԭ�����ݲ�����
		switch(m_recvedDataLen)
		{
		case 1:
			if(data[0] >= 0x80)
			{//֮ǰ�յ��������ǲ������İ�
				LogNull("Error Data:0x%02x",m_recvedData[0]);
				m_recvedDataLen = 0;
				recvDataProc(data,len);
			}else
			{//��ȷ������
				m_recvedData[1] = data[0];
				++ m_recvedDataLen;
				recvDataProc(data + 1,len - 1);
			}
			break;
		case 2:
			if(data[0] >= 0x80)
			{//֮ǰ�յ��������ǲ������İ�
				LogNull("Error Data:0x%02x,0x%02x", m_recvedData[0], m_recvedData[1]);
				m_recvedDataLen = 0;
				recvDataProc(data,len);
			}else
			{//��ȷ������
				m_recvedData[2] = data[0];
				//�յ�һ�����������ݰ�
				//m_data[m_recvedData[0] - 0xa0] = (m_recvedData[1] << 7) + m_recvedData[2];
				m_data[m_recvedData[0] - 0xa0] = decodeData(m_recvedData + 1);
				if(m_callBackFun != NULL)
				{
					XSimProData tmp;
					tmp.type = SIMPRO_TYPE_DATA;
					tmp.index = m_recvedData[0] - 0xa0;
					tmp.data = decodeData(m_recvedData + 1);//(m_recvedData[1] << 7) + m_recvedData[2];
					m_callBackFun(tmp,m_pClass);
				}

				m_recvedDataLen = 0;;
				recvDataProc(data + 1,len - 1);
			}
			break;
		}
	}else
	{//û�жѵ�����
		if(data[0] >= 0x80)
		{//��Ч������
			if((data[0] & 0xf0) >= 0xA0)
			{//��������
				m_recvedData[0] = data[0];
				++ m_recvedDataLen;
			}else
			{//״̬����
				m_state[(data[0] & 0x0f)] = ((data[0] & 0xf0) == 0x80)?1:0;
				if(m_callBackFun != NULL)
				{
					XSimProData tmp;
					tmp.type = SIMPRO_TYPE_STATE;
					tmp.index = (data[0] & 0x0f);
					tmp.data = ((data[0] & 0xf0) == 0x80)?1:0;
					m_callBackFun(tmp,m_pClass);
				}
			}
			recvDataProc(data + 1,len - 1);
		}else
		{//��������ͷ����
			std::string tmp = "Error Data:";
			char tmpStr[16];
			for(int i = 0;i < len;++ i)
			{
				if(data[i] >= 0x80)
				{
					LogStr(tmp.c_str());
					recvDataProc(data + i,len - i);
					return;
				}else
				{//���������
					sprintf(tmpStr,"0x%02x ",data[i]);
					tmp += tmpStr;
				}
			}
			//�������ݶ�������
			LogStr(tmp.c_str());
		}
	}
}
DWORD WINAPI XSimpleProtocol::sendThread(void * pParam)
{
	XSimpleProtocol &pPar = *(XSimpleProtocol *)pParam;
	pPar.m_sendThreadState = STATE_START;

	while(pPar.m_sendThreadState != STATE_SET_TO_END)
	{
		//if(pPar.m_sendThreadState == STATE_SET_TO_END) break;
		//���﷢������
		if(pPar.m_sendData.size() > 0)
		{//���﷢��һ������
			pPar.m_sendMutex.Lock();
			XSimProData tmp = pPar.m_sendData[0];
			pPar.m_sendData.pop_front();
			pPar.m_sendMutex.Unlock();
			//���淢��tmp
			pPar.sendData(tmp);
		}

		Sleep(1);
	}
	pPar.m_sendThreadState = STATE_END;
	return 0;
}
void XSimpleProtocol::sendData(const XSimProData&tmp)
{
	if(tmp.type == SIMPRO_TYPE_STATE)
	{
		if(tmp.index < 0 || tmp.index >= m_maxXSPStateSum) return;	//���ݲ��Ϸ�
		unsigned char data;
		if(tmp.data == 0)
		{
			data = 0x90 + tmp.index;
			m_state[tmp.index] = 0;
		}else
		{
			data = 0x80 + tmp.index;
			m_state[tmp.index] = 1;
		}
		m_serialPort.sendData(&data,1);
		//���﷢�����֮����Ҫ�޸ı��ص�����
	}else
	{
		if(tmp.index < 0 || tmp.index >= m_maxXSPValueSum) return;	//���ݲ��Ϸ�
		unsigned char data[3];
		data[0] = tmp.index + 0xa0;
		m_data[tmp.index] = tmp.data;
		encodeData(tmp.data,data + 1);
		m_serialPort.sendData(data,3);
	}
}
#if !WITH_INLINE_FILE
#include "XSimpleProtocol.inl"
#endif
}