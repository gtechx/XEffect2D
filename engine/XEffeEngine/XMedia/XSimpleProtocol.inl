INLINE void XSimpleProtocol::setCallBackFun(void (*p)(const XSimProData &,void *),void *pC)
{
	m_callBackFun = p;
	m_pClass = pC;
}
INLINE XBool XSimpleProtocol::openDevice(int portIndex,int bitRate,int parity,XBool withEventMode)
{
	if(m_isInited) return XFalse;
	//�򿪴����豸
	if(!m_serialPort.open(portIndex,bitRate,parity,SP_MODE_AUTO,withEventMode)) return XFalse;
	m_serialPort.setCallBackFun(simpleProctocolRecvCB,this);
	//��������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

	m_isInited = XTrue;
	return XTrue;
}
INLINE XBool XSimpleProtocol::openDevice(const XSerialPortInfo &info)
{
	if(m_isInited) return XFalse;
	//�򿪴����豸
	if(!m_serialPort.open(info)) return XFalse;
	m_serialPort.setCallBackFun(simpleProctocolRecvCB,this);
	//��������߳�
	m_sendThreadState = STATE_BEFORE_START;
	if(CreateThread(0,0,sendThread,this,0,NULL) == 0) return XFalse;	//���������߳�

	m_isInited = XTrue;
	return XTrue;
}
INLINE void XSimpleProtocol::release()
{
	if(!m_isInited) return;
	m_sendThreadState = STATE_SET_TO_END;
	m_sendData.clear();	//��շ��Ͷ���
	waitThreadEnd(m_sendThreadState);
	m_serialPort.close();
	m_isInited = false;
}
INLINE char XSimpleProtocol::getState(int index) const
{
	if(index < 0 || index >= m_maxXSPStateSum) return 0;
	return m_state[index];
}
INLINE int XSimpleProtocol::getData(int index) const
{
	if(index < 0 || index >= m_maxXSPValueSum) return 0;
	return m_data[index];
}
INLINE XBool XSimpleProtocol::pushAData(XSimProData &data)	//���Ͷ���������һ������
{
	m_sendMutex.Lock();
	m_sendData.push_back(data);
	m_sendMutex.Unlock();
	return XTrue;
}
INLINE int XSimpleProtocol::decodeData(unsigned char *data)				//��ԭʼ���ݽ���ΪĿ������
{
	if(m_isSigned && data[0] & 0x40)
	{//����
		//return - (((data[0] << 7) + data[1]) & 0x1fff);
		return - ((((data[0] & 0x7f) << 7) + (data[1] & 0x7f)) & 0x1fff);
	}else
	{
		return ((data[0] & 0x7f) << 7) + (data[1] & 0x7f);
		//return (data[0] << 7) + data[1];
	}
}
INLINE void XSimpleProtocol::encodeData(int src,unsigned char *data)	//��Ŀ�����ݱ���ΪԴ����
{
	if(m_isSigned && src < 0)
	{//����
		src = -src;
		data[0] = ((src >> 7) & 0x7f) | 0x40;
		data[1] = src & 0x7f;
	}else
	{
		data[0] = (src >> 7) & 0x7f;
		data[1] = src & 0x7f;
	}
}