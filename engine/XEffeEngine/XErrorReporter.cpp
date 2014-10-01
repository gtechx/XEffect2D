#include "XEffeEngine.h"
#include "XErrorReporter.h"
void errorReporterBtnProc(void *pClass,int)
{
	((_XErrorReporter *)pClass)->m_isBtnChecked = true;
}
bool _XErrorReporter::reportFile(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	_XSystemTime t;
	getTimeMs(t);
	//�������̨
	printf("---------------- Head Of File ----------------\n");
	printf("ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d %03d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	printf("����������%s\n",errorDescription.c_str());
	printf("ִ���ļ���%s\n",getCurrentExeFileFullPath().c_str());
	printf("�ļ���%s @ %d ��\n",filename.c_str(),line);
	printf("������%s\n",funName.c_str());
	printf("����ʱ�䣺%s %s\n",__DATE__,__TIME__);
	printf("��Ŀ���ƣ�%s\n",XEE::windowData.windowTitle.c_str());
	printf("�汾�ţ�%s\n",XEE::windowData.versionString.c_str());
	printf("---------------- End Of File ----------------");
	//������ļ�
	FILE *fp = NULL;
	if((fp = fopen(ERROR_REPORTER_TXT_FILENAME,"w")) == NULL) return false;
	fprintf(fp,"---------------- Head Of File ----------------\n");
	fprintf(fp,"ʱ�䣺%d-%d-%d %d:%d:%d %d\n",t.year,t.month,t.day,t.hour,t.minute,t.second,t.millisecond);
	fprintf(fp,"����������%s\n",errorDescription.c_str());
	fprintf(fp,"ִ���ļ���%s\n",getCurrentExeFileFullPath().c_str());
	fprintf(fp,"�ļ���%s @ %d ��\n",filename.c_str(),line);
	fprintf(fp,"������%s\n",funName.c_str());
	fprintf(fp,"����ʱ�䣺%s %s\n",__DATE__,__TIME__);
	fprintf(fp,"��Ŀ���ƣ�%s\n",XEE::windowData.windowTitle.c_str());
	fprintf(fp,"�汾�ţ�%s\n",XEE::windowData.versionString.c_str());
	if(m_isInited)
	{//����û���ע
		if(m_edit.getString() != NULL)
			fprintf(fp,"��ע��%s\n",m_edit.getString());
	}
	fprintf(fp,"---------------- End Of File ----------------");
	fclose(fp);

	return true;
}
int _XErrorReporter::inputEvent()
{//���Ҳ��֧�ֶ��߳�
	int ret = 0;
	_XInputEvent tmpEvent;

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
				if(!m_button.isInRect(XEE::mousePosition.x,XEE::mousePosition.y)
					&& !m_edit.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
					ret = 1;
			}
			break;
		}
		XEE::inputEvent(tmpEvent);
	}
	return ret;
}
void _XErrorReporter::reportError(const std::string & errorDescription,const std::string &filename,int line,const std::string &funName)
{
	if(!m_needShow)
	{
		m_errDescription = errorDescription;	//��������
		m_errFilename = filename;		//���������ļ���
		m_errFunname = funName;		//�������ĺ�����
		m_errLine = line;					//���������к�
		m_needShow = true;	//�Ƿ���Ҫ��ʾ���󱨸����
	}
	if(GetCurrentThreadId() == XEE::mainThreadHandle) 
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
void _XErrorReporter::errorProc()
{
	if(!m_needShow) return;
	if(m_isInited)
	{//����Ѿ���ʼ������������ͼ�λ�����Ĵ���
		_XWindow.setCurcor(true);//��ʾ�����
		gameShot(ERROR_REPORTER_PIC_FILENAME,XEE::windowData.w,XEE::windowData.h,false);//��Ļ��ͼ
		m_screenShot.init(ERROR_REPORTER_PIC_FILENAME,RESOURCE_LOCAL_FOLDER);
		m_edit.setVisible();
		m_button.setVisible();
		string temp = "��������:";
		temp += m_errDescription;
		m_fontDescrip.setString(temp.c_str());
		while(!inputEvent()) 
		{//������ѭ��
			if(m_isBtnChecked) break;
			XEE::engineIdle();
			//TODO:������������߼�
			XEE::clearScreen();			//�����Ļ
			//TODO:������ʾ�͸�������Ҫ��ʾ������
			XEE::begin2DDraw();
			draw();

			XEE::updateScreen();			//������Ļ������
			mySleep(1);
		}
	}
	reportFile(m_errDescription,m_errFilename,m_errLine,m_errFunname);
	_XLogbook::GetInstance().releaseLog();	//���������־��Ϣ
	XEE::release();
}
bool _XErrorReporter::init(_XResourcePosition resoursePosition)	//��ʼ��
{
	if(m_isInited) return false;
	if(!m_background.init((XEE::windowData.commonResourcePos + "ResourcePack/pic/ErrorReporter/Back.png").c_str(),
		resoursePosition)) return false;
	if(!m_button.initPlus((XEE::windowData.commonResourcePos + "ResourcePack/pic/ErrorReporter/btn").c_str(),
		" ",XEE::systemFont,1.0f,resoursePosition)) return false;
	if(!m_edit.initWithoutTex(_XRect(0.0f,0.0f,448.0f,40.0f),"�˴����뱸ע.",XEE::systemFont,1.0f)) return false;
	m_edit.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f + 20.0f);
	m_edit.disVisible();
	m_background.setPosition(XEE::windowData.w * 0.5f - 256.0f,XEE::windowData.h * 0.5f - 128.0f);
	m_button.setPosition(XEE::windowData.w * 0.5f - 77.0f,XEE::windowData.h * 0.5f + 65.0f);
	m_button.setCallbackFun(NULL,NULL,NULL,errorReporterBtnProc,NULL,this);
	m_button.disVisible();
	m_font.setACopy(XEE::systemFont);
	m_font.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f - 50.0f);
	m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_font.setMaxStrLen(512);
	m_font.setSize(0.5f);
	m_font.setColor(0.0f,0.0f,0.0f,1.0f);
	m_font.setString("�ǳ���Ǹ,���������˲����޸��Ĵ���,�����˳�.�����Ե�[ȷ��]��ť\n\
���ɴ��󱨸沢�˳�,���߰��¼���[Esc]���˳�.��Ҳ���԰��������\n\
����˳�.���ǵļ�����Ա�ᾡ���޸��������!");
	m_fontDescrip.setACopy(XEE::systemFont);
	m_fontDescrip.setPosition(XEE::windowData.w * 0.5f - 220.0f,XEE::windowData.h * 0.5f + 1.0f);
	m_fontDescrip.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_fontDescrip.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_fontDescrip.setMaxStrLen(512);
	m_fontDescrip.setSize(0.5f);
	m_fontDescrip.setColor(0.0f,0.0f,0.0f,1.0f);
	m_fontDescrip.setString("��������");
	m_isInited = true;
	return true;
}