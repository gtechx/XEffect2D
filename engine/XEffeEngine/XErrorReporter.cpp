#include "XStdHead.h"
#include "XErrorReporter.h"
#include "XWindowCommon.h"
#include "XControl\XEdit.h"
#include "XFile.h"
#include "XControl\XControlManager.h"
#include "XInputEventCommon.h"
namespace XE{
XErrorReporter::XErrorReporter()
	:m_isInited(false)
	,m_isBtnChecked(false)
	,m_needShow(false)
	,m_isMainThreadProc(false)
{
	m_screenShot = XMem::createMem<XSprite>();
	if(m_screenShot == NULL) LogStr("Mem Error!");
	m_background = XMem::createMem<XSprite>();
	if(m_background == NULL) LogStr("Mem Error!");
	m_button = XMem::createMem<XButton>();
	if(m_button == NULL) LogStr("Mem Error!");
	m_font = XMem::createMem<XFontUnicode>();
	if(m_font == NULL) LogStr("Mem Error!");
	m_fontDescrip = XMem::createMem<XFontUnicode>();
	if(m_fontDescrip == NULL) LogStr("Mem Error!");
	m_edit = XMem::createMem<XEdit>();
	if(m_edit == NULL) LogStr("Mem Error!");
}
XErrorReporter::~XErrorReporter()
{
	XMem::XDELETE(m_screenShot);
	XMem::XDELETE(m_background);
	XMem::XDELETE(m_button);
	XMem::XDELETE(m_font);
	XMem::XDELETE(m_fontDescrip);
	XMem::XDELETE(m_edit);
}
void XErrorReporter::ctrlProc(void *pClass,int id,int eventID)
{
	if(eventID == XButton::BTN_MOUSE_DOWN)
		((XErrorReporter *)pClass)->m_isBtnChecked = true;
}
bool XErrorReporter::reportFile(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	XSystemTime t;
	XTime::getTimeMs(t);
	//�������̨
	printf("---------------- Head Of File ----------------\n");
	printf("ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d %03d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	printf("����������%s\n",errorDescription.c_str());
	printf("ִ���ļ���%s\n",XFile::getCurrentExeFileFullPath().c_str());
	printf("�ļ���%s @ %d ��\n",filename.c_str(),line);
	printf("������%s\n",funName.c_str());
	printf("����ʱ�䣺%s %s\n",__DATE__,__TIME__);
	printf("��Ŀ���ƣ�%s\n",XEG.m_windowData.windowTitle.c_str());
	printf("�汾�ţ�%s\n",XEG.m_windowData.versionString.c_str());
	printf("---------------- End Of File ----------------");
	//������ļ�
	FILE *fp = NULL;
	if((fp = fopen(ERROR_REPORTER_TXT_FILENAME,"w")) == NULL) return false;
	fprintf(fp,"---------------- Head Of File ----------------\n");
	fprintf(fp,"ʱ�䣺%d-%d-%d %d:%d:%d %d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	fprintf(fp,"����������%s\n",errorDescription.c_str());
	fprintf(fp,"ִ���ļ���%s\n",XFile::getCurrentExeFileFullPath().c_str());
	fprintf(fp,"�ļ���%s @ %d ��\n",filename.c_str(),line);
	fprintf(fp,"������%s\n",funName.c_str());
	fprintf(fp,"����ʱ�䣺%s %s\n",__DATE__,__TIME__);
	fprintf(fp,"��Ŀ���ƣ�%s\n",XEG.m_windowData.windowTitle.c_str());
	fprintf(fp,"�汾�ţ�%s\n",XEG.m_windowData.versionString.c_str());
	if(m_isInited)
	{//����û���ע
		if(m_edit->getString() != NULL)
			fprintf(fp,"��ע��%s\n",m_edit->getString());
	}
	fprintf(fp,"---------------- End Of File ----------------");
	fclose(fp);

	return true;
}
int XErrorReporter::inputEvent()
{//���Ҳ��֧�ֶ��߳�
	int ret = 0;
	XInputEvent tmpEvent;

	while(getInputEvent(tmpEvent))	//ע��SDL���¼������ǲ�֧�ֶ��̵߳ģ�ֻ�������߳��е���
	{
		switch(tmpEvent.type)
		{
		case EVENT_EXIT:ret = 1;break;
		case EVENT_KEYBOARD:
			if(tmpEvent.keyState == KEY_STATE_DOWN)
			{
				switch(tmpEvent.keyValue)
				{
				case XKEY_ESCAPE:ret = 1;break;
				}
			}
			break;
		case EVENT_MOUSE:
			if(tmpEvent.mouseState == MOUSE_LEFT_BUTTON_DOWN
				|| tmpEvent.mouseState == MOUSE_RIGHT_BUTTON_DOWN
				|| tmpEvent.mouseState == MOUSE_MIDDLE_BUTTON_DOWN)
			{//�հ����������������˳�
				if(!m_button->isInRect(getMousePos().x,getMousePos().y)
					&& !m_edit->isInRect(getMousePos().x,getMousePos().y))
					ret = 1;
			}
			break;
		}
		XEG.engineInputEvent(tmpEvent);
	}
	return ret;
}
void XErrorReporter::reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	if(!m_needShow)
	{
		m_errDescription = errorDescription;	//��������
		m_errFilename = filename;		//���������ļ���
		m_errFunname = funName;		//�������ĺ�����
		m_errLine = line;					//���������к�
		m_needShow = true;	//�Ƿ���Ҫ��ʾ���󱨸����
	}
	if(GetCurrentThreadId() == XEG.getMainThreadID()) 
	{//��������߳���ֱ�ӽ����˳�����
		errorProc();
		exit(1);	//ֱ���˳�����
	}else
	{//����������߳����������̲߳��ȴ����̴߳���
		//����1�����߳��Զ��������߳��м��״̬������Ӧ�Ĵ���
		//���⣬������߳������̴߳��ڻ�����ϵ����ô���������
		//while(true) Sleep(1);
		//����2���������̲߳��ܽ���OpenGL��Ⱦ���������߳��в�����ͼ�δ���
		//���Դ���������ͼ�λ�����ֱ������򵥵����
		//m_isInited = false;	
		//errorProc();
		//exit(1);	//ֱ���˳�����
		//����3��ֱ���л������ģ�����ɽ�ͼ����
		//Sleep(100);	//�ȴ����߳̽��մ���������ܽ��մ�����������Դ���
		//if(!wglMakeCurrent(XEE::wHDC,XEE::wCurrentHGLRC))
		//{
		//	printf("ʹ�ø���������\n");
		//	wglMakeCurrent(XEE::wHDC,XEE::wCopyHGLRC);
		//}
		//errorProc();
		//exit(1);	//ֱ���˳�����
		//����4���ȵȴ����̴߳������ָ��ʱ�������߳�û�д������Լ��򵥴���
		Sleep(500);
		if(m_isMainThreadProc)
		{
			printf("���߳̽����˴�����!\n");
			while(true) Sleep(1);
		}
		printf("���߳�δ���մ�����!\n");
		m_isInited = false;	
		errorProc();
		exit(1);	//ֱ���˳�����
	}
}
void XErrorReporterDraw()
{
	if(gFrameworkData.pErrReporter != NULL) 
		XErrReporter.draw();
}
void XErrorReporter::errorProc()
{
	if(!m_needShow) return;
	if(m_isInited)
	{//����Ѿ���ʼ������������ͼ�λ�����Ĵ���
		XWindow.setCurcor(true);//��ʾ�����
		XEG.gameShot(ERROR_REPORTER_PIC_FILENAME,getWindowWidth(),getWindowHeight(),false);//��Ļ��ͼ
		m_screenShot->init(ERROR_REPORTER_PIC_FILENAME,RESOURCE_LOCAL_FOLDER);
		m_screenShot->setIsTransformCenter(POINT_LEFT_TOP);
		if(XEG.m_windowData.withCustomTitle) m_screenShot->setPosition(0.0f,-MIN_FONT_CTRL_SIZE);
		m_screenShot->setScale((float)(getSceneWidth()) / (float)(getWindowWidth()),(float)(getSceneHeight()) / (float)(getWindowHeight()));
		m_edit->setVisible();
		m_button->setVisible();
		std::string temp = "��������:";
		temp += m_errDescription;
		m_fontDescrip->setString(temp.c_str());
		XEG.m_drawAfterCtrl = XErrorReporterDraw;
		while(!inputEvent()) 
		{//������ѭ��
			if(m_isBtnChecked) break;
			XEG.engineIdle();
			//TODO:������������߼�
			XEG.clearScreen();			//�����Ļ
			//TODO:������ʾ�͸�������Ҫ��ʾ������
			XEG.begin2DDraw();
			XEG.updateScreen();			//������Ļ������
			XEE::sleep(1);
		}
	}
	reportFile(m_errDescription,m_errFilename,m_errLine,m_errFunname);
	LogRelease();	//���������־��Ϣ
	XEG.engineRelease();
}
bool XErrorReporter::init(XResourcePosition resoursePosition)	//��ʼ��
{
	if(m_isInited) return false;
	if(!m_background->init((getCommonResPos() + "ResourcePack/pic/ErrorReporter/Back.png").c_str(),
		resoursePosition)) return false;
	if(!m_button->initPlus((getCommonResPos() + "ResourcePack/pic/ErrorReporter/btn").c_str(),
		" ",getDefaultFont(),1.0f,resoursePosition)) return false;
	if(!m_edit->initWithoutSkin(XRect(0.0f,0.0f,448.0f,40.0f),"�˴����뱸ע.",getDefaultFont(),1.0f)) return false;
	m_edit->setPosition(getSceneWidth() * 0.5f - 220.0f,getSceneHeight() * 0.5f + 20.0f);
	m_edit->disVisible();
	m_background->setPosition(getSceneWidth() * 0.5f - 256.0f,getSceneHeight() * 0.5f - 128.0f);
	m_button->setPosition(getSceneWidth() * 0.5f - 77.0f,getSceneHeight() * 0.5f + 65.0f);
	m_button->setEventProc(ctrlProc,this);
	m_button->disVisible();
	m_font->setACopy(getDefaultFont());
	m_font->setPosition(getSceneWidth() * 0.5f - 220.0f,getSceneHeight() * 0.5f - 50.0f);
	m_font->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_font->setMaxStrLen(512);
	m_font->setScale(0.5f);
	m_font->setColor(0.0f,0.0f,0.0f,1.0f);
	m_font->setString("�ǳ���Ǹ,���������˲����޸��Ĵ���,�����˳�.�����Ե�[ȷ��]��ť\n\
���ɴ��󱨸沢�˳�,���߰��¼���[Esc]���˳�.��Ҳ���԰��������\n\
����˳�.���ǵļ�����Ա�ᾡ���޸��������!");
	m_fontDescrip->setACopy(getDefaultFont());
	m_fontDescrip->setPosition(getSceneWidth() * 0.5f - 220.0f,getSceneHeight() * 0.5f + 1.0f);
	m_fontDescrip->setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_fontDescrip->setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_fontDescrip->setMaxStrLen(512);
	m_fontDescrip->setScale(0.5f);
	m_fontDescrip->setColor(0.0f,0.0f,0.0f,1.0f);
	m_fontDescrip->setString("��������");
	m_isInited = true;
	return true;
}
void XErrorReporter::draw()
{
	m_screenShot->draw();
	m_background->draw();
	m_font->draw();
	m_fontDescrip->draw();
	XCtrlManager.draw(m_button);
	XCtrlManager.draw(m_edit);
}
}