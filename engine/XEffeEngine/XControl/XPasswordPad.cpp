//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPasswordPad.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void btnFunc(void *pClass,int ID)
{
	_XPasswordPad &pPar = *(_XPasswordPad *)pClass;
	for(int i = 0;i < 10;++ i)
	{
		if(ID == pPar.m_button[i].getControlID())
		{
			printf("%d������\n",i);
			pPar.addChar('0' + i);
			return;
		}
	}
	if(ID == pPar.m_button[10].getControlID())
	{
		printf("ȷ�� ������\n");
		pPar.btn10Proc();
		return;
	}
	if(ID == pPar.m_button[11].getControlID())
	{
		printf("ȡ�� ������\n");
		pPar.btn11Proc();
		return;
	}
}

_XBool _XPasswordPadTexture::init(const char *normal,const char *down,const char *on,const char *disable,const char *BG,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(!m_bottomTex.init(normal,on,down,disable,resoursePosition)) return XFalse;

	if(BG != NULL && (backGround = createATextureData(BG,resoursePosition)) == NULL) return XFalse;

	m_isInited = XTrue;
	return XTrue;
}
#define PASSWORDPAD_CONFIG_FILENAME ("PasswordPad.txt")
_XBool _XPasswordPadTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited || filename == NULL) return XFalse;
	if(!m_bottomTex.initEx(filename,resoursePosition)) return XFalse;

	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,PASSWORDPAD_CONFIG_FILENAME);
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//�ⲿ��Դ
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == NULL) return XFalse; //��Ϣ�ļ���ȡʧ��
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		if(fscanf(fp,"%d:",&flag) != 1) {fclose(fp);return XFalse;}
		if(flag != 0)
		{
			if(fscanf(fp,"%s",resFilename) != 1) {fclose(fp);return XFalse;}
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((backGround = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				fclose(fp);
				return XFalse;
			}
		}
		int x,y;
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_data.bottonDistance.set(x,y);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_data.bottonEffectArea.set(x,y);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_data.bottonOffsetPosition.set(x,y);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_data.bottonTextPosition.set(x,y);
		if(fscanf(fp,"%d,%d,",&x,&y) != 2) {fclose(fp);return XFalse;}
		m_data.captionPosition.set(x,y);
		fclose(fp);
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		int offset = 0;
		//normal
		if(sscanf((char *)(p + offset),"%d:",&flag) != 1) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag != 0)
		{
			if(sscanf((char *)(p + offset),"%s",resFilename) != 1) {XDELETE_ARRAY(p);return XFalse;}
			offset += getCharPosition((char *)(p + offset),'\n') + 1;
			sprintf(tempFilename,"%s/%s",filename,resFilename);
			if((backGround = createATextureData(tempFilename,resoursePosition)) == NULL)
			{//��Դ��ȡʧ��
				XDELETE_ARRAY(p);
				return XFalse;
			}
		}
		int x,y;
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_data.bottonDistance.set(x,y);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_data.bottonEffectArea.set(x,y);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_data.bottonOffsetPosition.set(x,y);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_data.bottonTextPosition.set(x,y);
		if(sscanf((char *)(p + offset),"%d,%d,",&x,&y) != 2) {XDELETE_ARRAY(p);return XFalse;}
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_data.captionPosition.set(x,y);
		XDELETE_ARRAY(p);
	}

	m_isInited = XTrue;
	return XTrue;
}

_XPasswordPad::_XPasswordPad()
:m_isInited(XFalse)
,m_mode(0)
//,m_position(0,0)
,m_stage(1)
,m_couter(0)
//,m_isVisible(XFalse)
{
	m_oldPassword[0] = '\0';
	m_newPassword[0] = '\0';
	m_enterPassword[0] = '\0';
	m_showString[0] = '\0';
	m_ctrlType = CTRL_OBJ_PASSWORDPAD;
}
_XPasswordPad::~_XPasswordPad()
{
	_XCtrlManger.decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//���̵�������4*3
_XBool _XPasswordPad::init(const _XVector2& position,_XPasswordPadTexture *tex,const _XFontUnicode& font,const _XPasswordPadData& data)
{
	if(m_isInited || tex == NULL) return XFalse;

	m_texture = tex;
	m_passwardPadData = data;
	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_caption.setPosition(m_passwardPadData.captionPosition.x,m_passwardPadData.captionPosition.y);
	m_caption.setColor(1.0f,1.0f,1.0f,1.0f);
	//if(m_buttonTexture.set(m_texture->buttonNormal,m_texture->buttonDown,m_texture->buttonOn,m_texture->buttonDisable) == 0) return XFalse;
	m_size.set(1.0f,1.0f);
	//��ʼ����ť
	m_button[0].init(m_passwardPadData.bottonOffsetPosition,_XRect(0,0,m_passwardPadData.bottonEffectArea.x,m_passwardPadData.bottonEffectArea.y),
		m_texture->m_bottomTex,"0",m_caption,1.0f,m_passwardPadData.bottonTextPosition);
	m_button[0].setPosition(m_passwardPadData.bottonOffsetPosition);
	_XCtrlManger.decreaseAObject(&m_button[0]);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_button[0]);
#endif
	for(int i = 1;i < 12;++ i)
	{
		m_button[i].setACopy(m_button[0]);
		m_button[i].setPosition(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);
		_XCtrlManger.decreaseAObject(&m_button[i]);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_button[i]);
#endif
	}
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setMouseUpCB(btnFunc,this);
	}
	m_button[0].setCaptionText("0");
	m_button[1].setCaptionText("1");
	m_button[2].setCaptionText("2");
	m_button[3].setCaptionText("3");
	m_button[4].setCaptionText("4");
	m_button[5].setCaptionText("5");
	m_button[6].setCaptionText("6");
	m_button[7].setCaptionText("7");
	m_button[8].setCaptionText("8");
	m_button[9].setCaptionText("9");
	m_button[10].setCaptionText("ȷ��");
	m_button[10].setCaptionPosition(m_passwardPadData.bottonTextPosition);
	m_button[11].setCaptionText("ȡ��");
	m_button[11].setCaptionPosition(m_passwardPadData.bottonTextPosition);

	m_sprite.init(m_texture->backGround->texture.m_w,m_texture->backGround->texture.m_h,1);
	m_sprite.setPosition(0.0f,0.0f);
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_sprite);
#endif
	m_mouseRect.set(0.0f,0.0f,m_texture->backGround->texture.m_w,m_texture->backGround->texture.m_h);

	m_withoutTex = XFalse;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = XTrue;
	setPosition(position);
	return XTrue;
}
_XBool _XPasswordPad::initWithoutTex(const _XFontUnicode& font)
{
	if(m_isInited) return XFalse;

	//m_texture = tex;
	//m_passwardPadData = data;
	m_passwardPadData.bottonOffsetPosition.set(10.0f,42.0f);	//��ť����ƫ������
	m_passwardPadData.bottonEffectArea.set(64.0f,32.0f);		//��ť��Ӧ������
	m_passwardPadData.bottonDistance.set(69.0f,37.0f);		//��ť֮��ľ���
	m_passwardPadData.bottonTextPosition.set(32.0f,16.0f);	//��ť�����ֵ�λ��
	m_passwardPadData.captionPosition.set(6.0f,5.0f);		//�����ֵ�λ��

	if(!m_caption.setACopy(font)) return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
	m_caption.setPosition(m_passwardPadData.captionPosition.x,m_passwardPadData.captionPosition.y);
	m_caption.setColor(1.0f,1.0f,1.0f,1.0f);
	//if(m_buttonTexture.set(m_texture->buttonNormal,m_texture->buttonDown,m_texture->buttonOn,m_texture->buttonDisable) == 0) return XFalse;
	m_size.set(1.0f,1.0f);

	//��ʼ����ť
	m_button[0].initWithoutTex("0",m_caption,_XRect(0.0f,0.0f,64.0f,32.0f));
	//m_button[0].init(m_passwardPadData.bottonOffsetPosition,_XRect(0,0,m_passwardPadData.bottonEffectArea.x,m_passwardPadData.bottonEffectArea.y),
	//	m_texture->m_bottomTex,"0",m_caption,1.0f,m_passwardPadData.bottonTextPosition);
	m_button[0].setPosition(m_passwardPadData.bottonOffsetPosition);
	_XCtrlManger.decreaseAObject(&m_button[0]);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.decreaseAObject(&m_button[0]);
#endif
	for(int i = 1;i < 12;++ i)
	{
		m_button[i].setACopy(m_button[0]);
		m_button[i].setPosition(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y);

		_XCtrlManger.decreaseAObject(&m_button[i]);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjManger.decreaseAObject(&m_button[i]);
#endif
	}
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setMouseUpCB(btnFunc,this);
	}
	m_button[0].setCaptionText("0");
	m_button[1].setCaptionText("1");
	m_button[2].setCaptionText("2");
	m_button[3].setCaptionText("3");
	m_button[4].setCaptionText("4");
	m_button[5].setCaptionText("5");
	m_button[6].setCaptionText("6");
	m_button[7].setCaptionText("7");
	m_button[8].setCaptionText("8");
	m_button[9].setCaptionText("9");
	m_button[10].setCaptionText("ȷ��");
	m_button[10].setCaptionPosition(m_passwardPadData.bottonTextPosition);
	m_button[11].setCaptionText("ȡ��");
	m_button[11].setCaptionPosition(m_passwardPadData.bottonTextPosition);

	m_mouseRect.set(0.0f,0.0f,222,190);

	m_isVisible = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XCtrlManger.addACtrl(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjManger.addAObject(this);
#endif

	m_withoutTex = XTrue;
	m_isInited = XTrue;
	setPosition(_XVector2::zero);
	return XTrue;
}
void _XPasswordPad::draw()
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isVisible) return;	//������ɼ�ֱ���˳�
	if(!m_withoutTex)
	{
		m_sprite.draw(m_texture->backGround);
	}else
	{
		drawFillBoxExA(m_position,_XVector2(m_mouseRect.getWidth(),m_mouseRect.getHeight()) * m_size,
			0.35 * m_color.fR,0.35 * m_color.fG,0.35 * m_color.fB,m_color.fA,true,true);
		drawFillBoxExA(m_position + _XVector2(5,2) * m_size,_XVector2(212,38) * m_size,
			0.15 * m_color.fR,0.15 * m_color.fG,0.15 * m_color.fB,m_color.fA,false,true);
	}
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].draw();
	}
	m_caption.draw();
}
_XBool _XPasswordPad::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited ||	//���û�г�ʼ��ֱ���˳�
		!m_isActive ||		//û�м���Ŀؼ������տ���
		!m_isVisible ||	//������ɼ�ֱ���˳�
		!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	for(int i = 0;i < 12;++ i)
	{
		m_button[i].mouseProc(x,y,mouseState);
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP && isInRect(x,y))
	{
		m_isBeChoose = XTrue;
	}
	return XTrue;
}
void _XPasswordPad::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//���ø��������λ��
	m_caption.setPosition(m_position + m_passwardPadData.captionPosition * m_size);
	for(int i = 0;i < 12;++ i)
	{
		m_button[i].setPosition(m_position + _XVector2(m_passwardPadData.bottonOffsetPosition.x + (i % 3) * m_passwardPadData.bottonDistance.x,
			m_passwardPadData.bottonOffsetPosition.y + (int)(i / 3) * m_passwardPadData.bottonDistance.y) * m_size);
	}
	m_sprite.setPosition(m_position);

	updateChildPos();
}
void _XPasswordPad::setStart(int mode)	//������������忪ʼ����
{
	if(m_stage != 0)
	{
		m_stage = 0;
		m_couter = 0;
		if(mode != 0)
		{
			m_mode = 1;
			m_caption.setString("������������");
		}else
		{
			m_mode = 0;
			m_caption.setString("����������");
		}
		m_isVisible = XTrue;
		m_enterPasswordLength = 0;
		m_enterPassword[m_enterPasswordLength] = '\0';
		m_showString[m_enterPasswordLength] = '\0';
		for(int i = 0;i < 12;++ i)
		{
			m_button[i].enable();
		}
		m_button[10].disable();
		m_button[11].setCaptionText("�˳�");
	}
}
void _XPasswordPad::btn10Proc()
{
	if(m_stage != 0) return;
	int len;
	if(m_mode == 0)
	{//��������ģʽ
		int flag = 0;
		if(strlen(m_oldPassword) == strlen(m_enterPassword))
		{
			len = (int)(strlen(m_oldPassword));
			for(int i = 0;i < len;++ i)
			{
				if(m_oldPassword[i] != m_enterPassword[i])
				{
					flag = 1;
					break;
				}
			}
		}else
		{
			flag = 1;
		}
		if(flag == 0)
		{//����������ȷ
			m_stage = 1;
			m_caption.setString("����������ȷ");
		}else
		{//�����������
			m_couter ++;
			if(m_couter >= MAX_ENTER_TRY)
			{
				m_stage = 2;
				m_caption.setString("�����������");
			}else
			{
				m_enterPasswordLength = 0;
				m_enterPassword[m_enterPasswordLength] = '\0';
				m_showString[m_enterPasswordLength] = '\0';
				m_button[10].disable();
				m_button[11].setCaptionText("�˳�");
				m_caption.setString("��������������");
			}
		}
	}else
	{//�޸�����ģʽ
		if(m_couter == 0)
		{//Ҫ��������������
			m_couter ++;
			strcpy(m_newPassword,m_enterPassword);

			m_enterPasswordLength = 0;
			m_enterPassword[m_enterPasswordLength] = '\0';
			m_showString[m_enterPasswordLength] = '\0';
			m_button[10].disable();
			m_button[11].setCaptionText("�˳�");
			m_caption.setString("��������������");
		}else
		{//������������Ƿ�һ��
			int flag = 0;
			if(strlen(m_newPassword) == strlen(m_enterPassword))
			{
				len = (int)(strlen(m_newPassword));
				for(int i = 0;i < len;++ i)
				{
					if(m_newPassword[i] != m_enterPassword[i])
					{
						flag = 1;
						break;
					}
				}
			}else
			{
				flag = 1;
			}
			if(flag == 0)
			{
				m_stage = 1;
				m_caption.setString("�����޸����");
			}else
			{
				m_stage = 2;
				m_caption.setString("�������벻ƥ��");
			}
		}
	}
}
void _XPasswordPad::btn11Proc()
{
	if(m_stage != 0) return;
	if(m_enterPasswordLength <= 0)
	{
		m_stage = 3;
		m_caption.setString("�����˳�...");
	}else
	{
		m_enterPasswordLength --;
		m_showString[m_enterPasswordLength] = '\0';
		m_enterPassword[m_enterPasswordLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswordLength < MIN_PASSWARD_LENGTH)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswordLength > 0)
		{
			m_button[11].setCaptionText("����");
		}else
		{
			m_button[11].setCaptionText("�˳�");
		}
	}
}
void _XPasswordPad::addChar(char temp)
{
	if(m_stage != 0) return;
	if(m_enterPasswordLength < MAX_PASSWARD_LENGTH)
	{
		m_enterPassword[m_enterPasswordLength] = temp;
		m_showString[m_enterPasswordLength] = '*';
		m_enterPasswordLength ++;
		m_showString[m_enterPasswordLength] = '\0';
		m_enterPassword[m_enterPasswordLength] = '\0';
		m_caption.setString(m_showString);
		if(m_enterPasswordLength < MIN_PASSWARD_LENGTH)
		{
			m_button[10].disable();
		}else
		{
			m_button[10].enable();
		}
		if(m_enterPasswordLength > 0)
		{
			m_button[11].setCaptionText("����");
		}
	}
}