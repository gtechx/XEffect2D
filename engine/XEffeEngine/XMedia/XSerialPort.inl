INLINE XBool XSerialPort::open(int nPort,int nBaud,int nParity,
		XSerialPortMode mode,XBool withEventMode)	//�򿪴����豸
{
	if(m_bOpened) return XTrue;	//��ֹ�ظ���
	m_serialPortInfo.nPort = nPort;
	m_serialPortInfo.nBaud = nBaud;
	m_serialPortInfo.nParity = nParity;
	m_serialPortInfo.mode = mode;
	m_serialPortInfo.withEventMode = withEventMode;
	return open(m_serialPortInfo);
}
INLINE int XSerialPort::readDataWaiting()	// ��ѯ���ܻ��������Ƿ�������(ֻ��ѯ,����ȡ)
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;
	DWORD dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);
	return (int)ComStat.cbInQue;
}
INLINE void XSerialPort::setCallBackFun(void (* funRecv)(void *,unsigned char *,int),void *pClass)
{
	m_funRecv = funRecv;
	m_pClass = pClass;
}
INLINE void XSerialPort::recordRecvAData(const unsigned char *data,int len)
{//���ļ���д���ȡ��һ������
	if(!m_withRecord || m_recordFile == NULL) return;
	fprintf(m_recordFile,"recv:");	//д���־
	fprintf(m_recordFile,(XTime::sysTime2String(XTime::getTimeMs()) + ":").c_str());	//д��ʱ��
	for(int i = 0;i < len;++ i)
	{
		fprintf(m_recordFile,"0x%02x ",data[i]);
	}
	fprintf(m_recordFile,"\n");
}
INLINE XBool XSerialPort::startRecord()	//��ʼ¼��
{
	if(m_withRecord || m_withPlay) return XFalse;
	m_recordMutex.Lock();
	if((m_recordFile = fopen(SP_RECORDE_FILENAME,"w")) == NULL) return XFalse;	//��¼���ļ�
	fprintf(m_recordFile,("start:" + XTime::sysTime2String(XTime::getTimeMs()) + ":\n").c_str());	//д��ʱ��
	m_withRecord = XTrue;
	m_recordMutex.Unlock();
	return XTrue;
}
INLINE XBool XSerialPort::endRecord()		//����¼��
{
	if(!m_withRecord) return XFalse;
	m_recordMutex.Lock();
	fprintf(m_recordFile,("stop:" + XTime::sysTime2String(XTime::getTimeMs()) + ":\n").c_str());	//д��ʱ��
	fclose(m_recordFile);
	m_recordFile = NULL;
	m_withRecord = XFalse;
	m_recordMutex.Unlock();
	return XTrue;
}
INLINE XBool XSerialPort::stopPlay()		//��������
{
	if(!m_withPlay) return XFalse;
	//�ر������ļ�(��δ���)
	fclose(m_playFile);
	m_withPlay = XFalse;
	return XTrue;
}
INLINE bool XSerialPortInfo::readFromFile(const char * filename)
{
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlBase::SetCondenseWhiteSpace(false);
	TiXmlDocument doc(filename);
	if(!doc.LoadFile()) return false;
	return readFromFile(doc.FirstChild("root"));
}
INLINE bool XSerialPortInfo::writeToFile(const char * filename)
{
	if(filename != NULL && filename[0] == '\0') return false;
	TiXmlDocument doc;
	TiXmlDeclaration declaration("1.0","gb2312","yes"); 
	doc.InsertEndChild(declaration);
	TiXmlElement elmRoot("root");
	if(!writeToFile(elmRoot)) return false;
	return doc.SaveFile(filename);
}