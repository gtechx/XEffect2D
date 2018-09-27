#include "XStdHead.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XHIKCamera.h"

#if SUPPORT_FOR_HIK
#include "../XMath/XColorSpace.h"
namespace XE{
#ifdef _WIN64
std::vector<XHIKCamera *> XHIKCamera::m_allPs;
#endif
//����Ļص�����
void CALLBACK XHIKCamera::decCBFun(long,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, long nUser,long)
{
	//switch(pFrameInfo->nType)
	//{
	//case T_AUDIO16:printf("T_AUDIO16\n");break;
	//case T_AUDIO8:printf("T_AUDIO8\n");break;
	//case T_UYVY:printf("T_UYVY\n");break;
	//case T_YV12:printf("T_YV12\n");break;
	//case T_RGB32:printf("T_RGB32\n");break;
	//default:
	//	printf("%d\n",pFrameInfo->nType);
	//	break;
	//}
	if(pFrameInfo->nType != T_YV12) return;
#ifdef _WIN64
	XHIKCamera &ref = *XHIKCamera::m_allPs[nUser];
#else
	XHIKCamera &ref = *(XHIKCamera *)nUser;
#endif
	if(!ref.m_isGetInitData) ref.setInitData(pFrameInfo->nWidth,pFrameInfo->nHeight);
#if HIK_WITH_LOECORE
	ref.m_mutex.Lock();
	memcpy(ref.m_yuvData,pBuf,nSize);
	ref.m_mutex.Unlock();
#else
	ref.m_mutex.Lock();
	XColorSpace::YUVToRGB((unsigned char *)pBuf,ref.m_dataRGB,pFrameInfo->nWidth,pFrameInfo->nHeight);
	ref.m_mutex.Unlock();
#endif
	ref.m_haveNewFrame = XTrue;
}
//ʵʱ����Ļص�����
#ifdef VERSION_40
void CALLBACK XHIKCamera::realDataCB(LONG/*lRealHandle*/, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void * dwUser)
#endif
#ifdef VERSION_30
void CALLBACK XHIKCamera::realDataCB(LONG/*lRealHandle*/, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD dwUser)
#endif
{
//	printf("realDataCB\n");
#ifdef _WIN64
	XHIKCamera &ref = *XHIKCamera::m_allPs[(int)dwUser];
#else
	XHIKCamera &ref = *(XHIKCamera *)dwUser;
#endif
	switch(dwDataType)
	{
	case NET_DVR_SYSHEAD:					//ϵͳͷ
		if(!PlayM4_GetPort(&(ref.m_lPort))) break;	//��ȡ���ſ�δʹ�õ�ͨ����
		if(dwBufSize > 0)
		{
			if(!PlayM4_SetStreamOpenMode(ref.m_lPort,STREAME_REALTIME)) break;		//����ʵʱ������ģʽ
			if(!PlayM4_OpenStream(ref.m_lPort,pBuffer,dwBufSize,1024*1024)) break;	//�����ӿ�,1024 * 1024�����ݺ�����
#ifdef VERSION_40
#ifdef _WIN64
			PlayM4_SetDecCallBackMend(ref.m_lPort,decCBFun,(long)dwUser);
#else
			PlayM4_SetDecCallBackMend(ref.m_lPort,decCBFun,(DWORD)dwUser);
#endif
#endif
#ifdef VERSION_30
			PlayM4_SetDecCallBackMend(ref.m_lPort,decCBFun,dwUser);
#endif
			if(!PlayM4_SetDecCBStream(ref.m_lPort,1)) break;
			if(!PlayM4_Play(ref.m_lPort,NULL)) break;//���ſ�ʼ
		}//T_YV12
		break;
	case NET_DVR_STREAMDATA:   //��������
		if (dwBufSize > 0 && ref.m_lPort != -1)
		{
			if(!PlayM4_InputData(ref.m_lPort,pBuffer,dwBufSize)) break;
		}
		break;
	}
}
void CALLBACK XHIKCamera::exceptionCB(DWORD dwType, LONG/*lUserID*/, LONG/*lHandle*/, void * /*pUser*/)
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
XBool XHIKCamera::updateFrame()
{
	if(!m_isInited) return XFalse;
	if(isNewFrame() && m_isWork != 0)
	{//������ͼ����
		m_mutex.Lock();
#if HIK_WITH_LOECORE
		XColorSpace::YUVToRGB(m_yuvData,m_dataRGB,m_pixelsWidth,m_pixelsHeight);
		m_mutex.Unlock();
#ifdef WITH_GL_TEXTRUE
		m_pixelsTex.updateTexture(m_dataRGB);
#endif
#else
#ifdef WITH_GL_TEXTRUE
		m_pixelsTex.updateTexture(m_dataRGB);
#endif
		m_mutex.Unlock();
#endif
		return XTrue;
	}
	return XFalse;
}
void XHIKCamera::release()
{
	if(!m_isInited) return;
	PlayM4_Stop(m_lPort);
	PlayM4_CloseFile(m_lPort);
	NET_DVR_Logout(m_lUserID);
	NET_DVR_Cleanup();
	m_isGetInitData = false;
	XMem::XDELETE_ARRAY(m_dataRGB);
#if HIK_WITH_LOECORE
	XMem::XDELETE_ARRAY(m_yuvData);
#endif
	//������Ҫɾ����Դ
	m_isInited = XFalse;
}
XBool XHIKCamera::init(XPixelsInputInfo &data)
{
	if(m_isInited) return XTrue;
	if(data.pixelsInputType != TYPE_CAM_HIK) return false;
	
	if(!NET_DVR_Init())//��ʼ������ͷ�������豸
	{
		LogStr("�������ʼ��ʧ��");
		return false;
	}
	//����1��
#ifdef VERSION_30
	m_lUserID = NET_DVR_Login_V30(data.ipAddress,data.port,data.userName,data.pwd,&m_structDeviceInfo);
#endif
#ifdef VERSION_40
	//����2��
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
	struLoginInfo.bUseAsynLogin = false;
	struLoginInfo.wPort = data.port;
	memcpy(struLoginInfo.sDeviceAddress, data.ipAddress, 16);	//NET_DVR_DEV_ADDRESS_MAX_LEN
	memcpy(struLoginInfo.sUserName, data.userName, 64);			//NAME_LEN
	memcpy(struLoginInfo.sPassword, data.pwd, 16);				//NAME_LEN
	struLoginInfo.byProxyType = 0;
	//struLoginInfo.byUseTransport = 1;
	//struLoginInfo.byRes3[119] = 2;
	m_lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
#endif
	if(m_lUserID < 0)	
	{//�����豸ʧ��
		printf("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return XFalse;
	}
#ifdef _WIN64
	m_allPs.push_back(this);
	m_myIndex = m_allPs.size() - 1;
	NET_DVR_SetExceptionCallBack_V30(0,NULL,exceptionCB,(void *)m_myIndex);
#else
	NET_DVR_SetExceptionCallBack_V30(0,NULL,exceptionCB,this);
#endif
#ifdef VERSION_30
	//V30��ʵ�ַ�ʽ
	NET_DVR_CLIENTINFO ClientInfo = {0};
	ClientInfo.hPlayWnd = NULL;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	ClientInfo.lChannel = 0;			//Ԥ��ͨ����
	ClientInfo.lLinkMode = 0;			//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ������0��30λ��ʾ���ӷ�ʽ��0��TCP��ʽ��1��UDP��ʽ��2���ಥ��ʽ��3��RTP��ʽ;
	ClientInfo.sMultiCastIP = NULL;		//�ಥ��ַ����Ҫ�ಥԤ��ʱ����
	BOOL bPreviewBlock = XFalse;			//�������������Ƿ�������0����1����
	m_lRealPlayHandle = NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,XFalse);
	if(m_lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V30 error\n");
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		return XFalse;
	}
	if(!NET_DVR_SetRealDataCallBack(m_lRealPlayHandle,realDataCB,(DWORD)this))
	{
		printf("NET_DVR_SetRealDataCallBack error\n");
	}
	DWORD dwReturn = 0;
	NET_DVR_COMPRESSIONCFG_V30 struParams = {0}; 
	if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_V30, m_structDeviceInfo.byStartChan, &struParams,sizeof(NET_DVR_COMPRESSIONCFG_V30), &dwReturn))
	{
		printf("Error!\n");
	}
	setInitData(data.w,data.h);
#endif
#ifdef VERSION_40
	//V40��ʵ�ַ�ʽ
	NET_DVR_PREVIEWINFO struPreViewInfo = {0};
	struPreViewInfo.lChannel = 0;
	struPreViewInfo.hPlayWnd = NULL;
	struPreViewInfo.dwLinkMode = 1;
	struPreViewInfo.byProtoType = 1;
#ifdef _WIN64
	m_lRealPlayHandle = NET_DVR_RealPlay_V40(m_lUserID, &struPreViewInfo, realDataCB, (void *)m_myIndex);
#else
	m_lRealPlayHandle = NET_DVR_RealPlay_V40(m_lUserID, &struPreViewInfo, realDataCB, this);
#endif
	if(m_lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		return XFalse;
	}
	while(!m_isGetInitData)
	{//����ȴ���ʼ������
		//if(m_isGetInitData) break;
		Sleep(1);
	}
	//���濪ʼ���ӻص������������ڴ�ռ�
	data.w = m_pixelsWidth;
	data.h = m_pixelsHeight;
#endif
//	m_cameraTexWidth = m_cameraWidth;
//	m_cameraTexHeight = m_cameraHeight;

	m_colorMode = COLOR_RGB;
#ifdef WITH_GL_TEXTRUE
	m_pixelsTex.createTexture(m_pixelsWidth,m_pixelsHeight,m_colorMode);	//���������2��n�η���չ�������ɲ�����
	if(data.needReset) m_pixelsTex.reset();
	m_pixelsSprite.init(m_pixelsWidth,m_pixelsHeight,0);
#endif
	m_haveNewFrame = XFalse;
	m_isInited = XTrue;
	return XTrue;
}
void XHIKCamera::setInitData(int w,int h)
{
	if(m_isGetInitData) return;
	m_pixelsWidth = w;
	m_pixelsHeight = h;
	m_buffSize = m_pixelsWidth * m_pixelsHeight * 3;
	m_dataRGB = XMem::createArrayMem<unsigned char>(m_buffSize);	//�����ڴ�ռ�
	memset(m_dataRGB,0,m_buffSize);
#if HIK_WITH_LOECORE
	m_yuvData =  XMem::createArrayMem<unsigned char>(m_buffSize >> 1);
	memset(m_yuvData,0,m_buffSize >> 1);
#endif
	m_isGetInitData = true;
}
//void XHIKCamera::init()
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
//int isupport = NET_DVR_IsSupport(); //�ж����пͻ��˵�PC�����Ƿ����Ҫ��
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
}
#endif //SUPPORT_FOR_HIK