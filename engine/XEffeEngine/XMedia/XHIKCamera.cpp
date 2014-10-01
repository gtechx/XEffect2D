//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XHIKCamera.h"
#include "stdio.h"

#if SUPPORT_FOR_HIK
#include "../XColorSpace.h"

//����Ļص�����
unsigned char* pOutRGB;	//�����RGB��ɫ
_XBool *haveNewFrame;		//��Ͻӿ�
//int *width = NULL;
//int *height = NULL;
void CALLBACK DecCBFun(long/*nPort*/,char * pBuf,long/*nSize*/, FRAME_INFO * pFrameInfo,long/*nReserved1*/,long) 
{
	if(pFrameInfo->nType == T_YV12) YUVToRGB((unsigned char *)pBuf,pOutRGB,pFrameInfo->nWidth,pFrameInfo->nHeight);
	//YUVToRGB((unsigned char *)pBuf,pOutRGB,*width,*height);
	* haveNewFrame = XTrue;
}
//ʵʱ����Ļص�����
LONG lPort;
void CALLBACK g_RealDataCallBack_V30(LONG/*lRealHandle*/, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD/*dwUser*/)
{
	switch(dwDataType)
	{
	case NET_DVR_SYSHEAD:					//ϵͳͷ
		if(!PlayM4_GetPort(&lPort)) break;	//��ȡ���ſ�δʹ�õ�ͨ����
		if(dwBufSize > 0)
		{
			if(!PlayM4_SetStreamOpenMode(lPort,STREAME_REALTIME)) break;		//����ʵʱ������ģʽ
			if(!PlayM4_OpenStream(lPort,pBuffer,dwBufSize,1024*1024)) break;	//�����ӿ�,1024 * 1024�����ݺ�����
			PlayM4_SetDecCallBack(lPort,DecCBFun);
			if(!PlayM4_SetDecCBStream(lPort,1)) break;
			if(!PlayM4_Play(lPort,NULL)) break;//���ſ�ʼ
		}//T_YV12
	case NET_DVR_STREAMDATA:   //��������
		if (dwBufSize > 0 && lPort != -1)
		{
			if(!PlayM4_InputData(lPort,pBuffer,dwBufSize)) break;
		}
	}
}
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG/*lUserID*/, LONG/*lHandle*/, void *)/*pUser*/
{
	switch(dwType) 
	{
	case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
		printf("----------reconnect--------\n");
		break;
	default:
		break;
	}
}
_XBool _XHIKCamera::upDateFrame()
{
	if(!m_isInited) return XFalse;
	if(isNewFrame() && m_isWork != 0)
	{//������ͼ����
		memcpy(m_dataRGBOut,m_dataRGB,m_buffSize);
		m_cameraTex.updateTexture(m_dataRGBOut);
		return XTrue;
	}
	return XFalse;
}
void _XHIKCamera::release()
{
	if(!m_isInited) return;
	PlayM4_Stop(lPort);
	PlayM4_CloseFile(lPort);
	NET_DVR_Logout(m_lUserID);
	NET_DVR_Cleanup();

	XDELETE_ARRAY(m_dataRGB);
	XDELETE_ARRAY(m_dataRGBOut);
	//������Ҫɾ����Դ
	m_isInited = XFalse;
}
_XBool _XHIKCamera::init(_XCameraData &data)
{
	if(m_isInited) return XTrue;
	m_cameraWidth = data.w;
	m_cameraHeight = data.h;
	//width = &m_cameraWidth;
	//height = & m_cameraHeight;
	m_buffSize = m_cameraWidth * m_cameraHeight * 3;

	m_cameraTexWidth = m_cameraWidth;
	m_cameraTexHeight = m_cameraHeight;
	//��ʼ������ͷ�������豸
	NET_DVR_Init();	//��ʼ��
	m_lUserID = NET_DVR_Login_V30(data.ipAddress,data.port,data.userName,data.pwd,&m_structDeviceInfo);
	if(m_lUserID < 0)	
	{//�����豸ʧ��
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return XFalse;
	}
	//���濪ʼ���ӻص������������ڴ�ռ�
	m_dataRGB = createArrayMem<unsigned char>(m_buffSize);	//�����ڴ�ռ�
	if(m_dataRGB == NULL) return XFalse;
	memset(m_dataRGB,0,m_buffSize);
	m_dataRGBOut = createArrayMem<unsigned char>(m_buffSize);
	if(m_dataRGBOut == NULL) return XFalse;

	haveNewFrame = &m_haveNewFrame;
	pOutRGB = m_dataRGB;

	NET_DVR_SetExceptionCallBack_V30(0,NULL,g_ExceptionCallBack,NULL);

	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	ClientInfo.lChannel = 0;			//Ԥ��ͨ����
	ClientInfo.lLinkMode = 0;			//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	ClientInfo.sMultiCastIP = NULL;		//�ಥ��ַ����Ҫ�ಥԤ��ʱ����

	//BOOL bPreviewBlock = XFalse;			//�������������Ƿ�������0����1����
	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,XFalse);
	if(m_lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V30 error\n");
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		return XFalse;
	}

	if(!NET_DVR_SetRealDataCallBack(m_lRealPlayHandle,g_RealDataCallBack_V30,0))
	{
		printf("NET_DVR_SetRealDataCallBack error\n");
	}

	m_cameraTex.createTexture(m_cameraWidth,m_cameraHeight,COLOR_RGB);	//���������2��n�η���չ�������ɲ�����
	m_cameraSprite.init(m_cameraWidth,m_cameraHeight,0);

	m_haveNewFrame = XFalse;
	m_isInited = XTrue;
	return XTrue;
}

//void _XHIKCamera::init()
//{
//	pYUV = new unsigned char[m_width * m_height * 1.5];
//	pRGB = new unsigned char[m_width * m_height * 3];
//	pRGB_old = new unsigned char[m_width * m_height * 3];
//	pRGB_old2 = new unsigned char[m_width * m_height * 3];
//	for(int i = 0; i < m_width * m_height * 3 ; i ++)
//	{
//		pRGB_old2[i] = -1;	//����
//	}
//	memcpy(pRGB,pRGB_old2,m_width * m_height * 3);
//	memcpy(pRGB_old,pRGB_old2,m_width * m_height * 3);
//
//	transferOK = XFalse;
//	//���ûص�����
//	NET_DVR_SetExceptionCallBack_V30(0,NULL,g_ExceptionCallBack,NULL);
//	NET_DVR_CLIENTINFO ClientInfo = {0};
//	ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
//	ClientInfo.lChannel = 0;       //Ԥ��ͨ����
//	ClientInfo.lLinkMode = 0;       //���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
//	ClientInfo.sMultiCastIP = NULL;   //�ಥ��ַ����Ҫ�ಥԤ��ʱ����
//
//	BOOL bPreviewBlock = XFalse;       //�������������Ƿ�������0����1����
//	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,0);
//	if(m_lRealPlayHandle < 0)
//	{
//		printf("NET_DVR_RealPlay_V30 error\n");
//		NET_DVR_Logout(m_lUserID);
//		NET_DVR_Cleanup();
//		return;
//	}
//
//	if(!NET_DVR_SetRealDataCallBack(m_lRealPlayHandle,g_RealDataCallBack_V30,0))
//	{
//		printf("NET_DVR_SetRealDataCallBack error\n");
//	}
//
//	//DWORD dwReturn = 0;
//	//memset(&m_CcdParam, 0, sizeof(m_CcdParam));
//	//m_CcdParam.dwSize = sizeof(m_CcdParam);
//	//if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam), &dwReturn))
//	//{
//	//}else
//	//{
//	//}
//	//cout <<(int)m_CcdParam.struWhiteBalance.byWhiteBalanceMode<<endl;
//	//m_CcdParam.struWhiteBalance.byWhiteBalanceMode = 1;
//	//if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam)))
//	//{
//	//}else
//	//{
//	//}
//
//	//if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_CCDPARAMCFG, -1, &m_CcdParam, sizeof(m_CcdParam), &dwReturn))
//	//{
//	//}else
//	//{
//	//}
//	//cout <<(int)m_CcdParam.struWhiteBalance.byWhiteBalanceMode<<endl;
//}

#endif //SUPPORT_FOR_HIK